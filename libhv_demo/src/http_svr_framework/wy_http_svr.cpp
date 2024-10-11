#include "wy_http_svr.h"
#include "wy_http_message.h"

namespace wy
{
bool HttpSvrImp::Start()
{
    // websocket
    RegisterWs();

    // static files
    RegisterHttpStatic("/", "./html");

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
    m_server.setThreadNum(4);
    return m_server.start();
}

void HttpSvrImp::Stop()
{
    m_server.stop();
}

uint16_t HttpSvrImp::Port()
{
    return m_server.port;
}

void HttpSvrImp::SetPort(uint16_t port)
{
    m_server.port = port;
}

void HttpSvrImp::Broadcast(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
    for (auto channel : m_wsChannels)
    {
        channel->send(msg);
    }
}

void HttpSvrImp::RegisterHttpInterface(const http_method& mothod,
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

void HttpSvrImp::RegisterHttpStatic(const std::string& path, const std::string& dir)
{
    m_router.Static(path.c_str(), dir.c_str());
}

void HttpSvrImp::RegisterWs()
{
    // m_ws.setPingInterval(10000);
    m_ws.onopen = [this](const WebSocketChannelPtr& channel, const HttpRequestPtr& req)
    {
        std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
        m_wsChannels.insert(channel);
        printf("onopen: GET %s, ws_channel_size: %ld\n", req->Path().c_str(), m_wsChannels.size());
    };
    m_ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg)
    {
        printf("onmessage: %s\n", msg.c_str());
        channel->send(msg);
    };
    m_ws.onclose = [this](const WebSocketChannelPtr& channel)
    {
        std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
        m_wsChannels.erase(channel);
        printf("onclose: %s, ws_channel_size: %ld\n", channel->peeraddr().c_str(),
               m_wsChannels.size());
    };
}
}  // namespace wy