#include "nlohmann/json.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>

using tcp           = boost::asio::ip::tcp;     // from <boost/asio/ip/tcp.hpp>
namespace http      = boost::beast::http;       // from <boost/beast/http.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

int main()
{
    try
    {
        // 1. 定义IO上下文和接受器
        boost::asio::io_context ioc;
        tcp::acceptor           acceptor{ioc, tcp::endpoint(tcp::v4(), 10080)};

        while (true)
        {
            // 2. 接受新的连接
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            // 3. 读取HTTP请求
            boost::beast::flat_buffer        buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            // 4. 检查是否是WebSocket升级请求
            if (websocket::is_upgrade(req))
            {
                // WebSocket握手
                websocket::stream<tcp::socket> ws{std::move(socket)};
                ws.accept(req);

                // 处理WebSocket消息
                while (true)
                {
                    boost::beast::flat_buffer ws_buffer;
                    ws.read(ws_buffer);
                    std::string msg = boost::beast::buffers_to_string(ws_buffer.data());

                    // 回显消息
                    ws.text(ws.got_text());
                    ws.write(boost::asio::buffer("Echo: " + msg));
                }
            }
            else
            {
                // 5. 创建HTTP响应
                http::response<http::string_body> res{http::status::ok, req.version()};
                res.set(http::field::server, "Boost.Beast Server");
                res.set(http::field::content_type, "text/plain");
                res.keep_alive(req.keep_alive());

                // 6. 根据HTTP方法和路径处理请求
                if (req.method() == http::verb::get)
                {
                    // 处理不同的 GET 请求路径
                    if (req.target() == "/")
                    {
                        res.body() = "hello boost::beast!";
                    }
                    else if (req.target() == "/get_json")
                    {
                        nlohmann::json j;
                        j["id"]   = "37";
                        j["name"] = "rock";
                        res.set("Access-Control-Allow-Origin", "*");
                        res.set("Content-Type", "application/json");
                        res.body() = j.dump();
                    }
                    else
                    {
                        res.result(http::status::not_found);
                        res.body() = "GET route not found.";
                    }
                }
                else if (req.method() == http::verb::post)
                {
                    // 处理不同的 POST 请求路径
                    if (req.target() == "/echo_json")
                    {
                        nlohmann::json j;
                        try
                        {
                            j          = nlohmann::json::parse(req.body());
                            res.body() = j.dump(2);
                        }
                        catch (const std::exception& e)
                        {
                            res.result(http::status::bad_request);
                            res.body() = e.what();
                        }
                    }
                    else
                    {
                        res.result(http::status::not_found);
                        res.body() = "POST route not found.";
                    }
                }
                else
                {
                    // 处理其他请求方法
                    res.result(http::status::bad_request);
                    res.body() = "Unsupported HTTP method.";
                }

                res.prepare_payload();

                // 7. 发送HTTP响应
                http::write(socket, res);

                // 8. 关闭连接
                socket.shutdown(tcp::socket::shutdown_send);
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}