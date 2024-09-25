#include "crow.h"
#include <mutex>
#include <unordered_set>

int main()
{
    crow::SimpleApp app;

    std::mutex                                       mtx;
    std::unordered_set<crow::websocket::connection*> users;

    CROW_WEBSOCKET_ROUTE(app, "/ws")
        .onopen(
            [&](crow::websocket::connection& conn)
            {
                CROW_LOG_INFO << "new websocket connection from " << conn.get_remote_ip();
                std::lock_guard<std::mutex> _(mtx);
                users.insert(&conn);
            })
        .onclose(
            [&](crow::websocket::connection& conn, const std::string& reason, uint16_t)
            {
                CROW_LOG_INFO << "websocket connection closed: " << reason;
                std::lock_guard<std::mutex> _(mtx);
                users.erase(&conn);
            })
        .onmessage(
            [&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary)
            {
                std::lock_guard<std::mutex> _(mtx);
                for (auto u : users)
                    if (is_binary)
                        u->send_binary(data);
                    else
                        u->send_text(data);
            });

    CROW_ROUTE(app, "/")
    (
        []
        {
            char name[256];
            gethostname(name, 256);
            crow::mustache::context x;
            x["servername"] = name;

            auto page = crow::mustache::load("ws.html");
            return page.render(x);
        });

    CROW_ROUTE(app, "/get_json")
    (
        []()
        {
            crow::json::wvalue x;
            x["hello"] = "world";
            crow::response res{x};
            res.add_header("Access-Control-Allow-Origin", "*");  // for CORS
            return res;
        });

    CROW_ROUTE(app, "/echo_json")
        .methods(crow::HTTPMethod::Post)(
            [](const crow::request& req)
            {
                auto x = crow::json::load(req.body);
                if (!x)
                    return crow::response(400);
                return crow::response{req.body};
            });

    app.port(10080).multithreaded().run();
}
