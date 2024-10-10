#include "wy_http_svr.h"
#include "wy_http_message.h"

namespace wy
{
bool HttpSvrImp::start()
{
    // websocket
    register_ws();

    // static files
    register_http_static("/", "./html");

    // middleware
    m_router.AllowCORS();
    m_router.Use(
        [](HttpRequest*, HttpResponse* resp)
        {
            resp->SetHeader("X-Request-tid", hv::to_string(hv_gettid()));
            return HTTP_STATUS_NEXT;
        });

    // start server
    m_server.service = &m_router;
    m_server.ws      = &m_ws;
    m_server.port    = m_port;
    return m_server.start();
}

void HttpSvrImp::stop()
{
    m_server.stop();
}

uint16_t HttpSvrImp::port()
{
    return m_server.port;
}

void HttpSvrImp::set_port(uint16_t port)
{
    m_server.port = port;
}

void HttpSvrImp::broadcast(const std::string& msg)
{
    for (auto channel : m_ws_channels)
    {
        channel->send(msg);
    }
}

void HttpSvrImp::boradcast(std::shared_ptr<hv::Json> msg)
{
    return broadcast(msg->dump(2));
}

void HttpSvrImp::register_http_interface(const http_method& mothod,
                                         const std::string& path,
                                         HttpMethodFunc     func)
{
    if (mothod == http_method::HTTP_GET)
    {
        m_router.GET(path.c_str(), func);
    }
    else if (mothod == http_method::HTTP_POST)
    {
        m_router.POST(path.c_str(), func);
    }
    else
    {
        throw std::runtime_error("mothod not support");
    }
}

void HttpSvrImp::register_http_static(const std::string& path, const std::string& dir)
{
    m_router.Static(path.c_str(), dir.c_str());
}

void HttpSvrImp::register_ws()
{
    // m_ws.setPingInterval(10000);
    m_ws.onopen = [this](const WebSocketChannelPtr& channel, const HttpRequestPtr& req)
    {
        m_ws_channels.insert(channel);
        printf("onopen: GET %s\n", req->Path().c_str());
    };
    m_ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg)
    {
        printf("onmessage: %s\n", msg.c_str());
        channel->send(msg);
    };
    m_ws.onclose = [](const WebSocketChannelPtr& channel)
    {
        printf("onclose: %s\n", channel->peeraddr().c_str());
    };
}
}  // namespace wy