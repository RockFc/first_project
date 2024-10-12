#include "wy_http_svr.h"
#include "wy_http_message.h"

namespace wy
{
bool HttpSvrImp::Start()
{
    // websocket
    RegisterWs();

    // static files
    RegisterStaticRoute("/", "../html");
    RegisterStaticRoute("/s/", "../htmls");

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

    // support https
    m_server.https_port = m_httpsPort;
    hssl_ctx_opt_t param;
    memset(&param, 0, sizeof(param));
    param.crt_file = "../cert/server.crt";
    param.key_file = "../cert/server.key";
    param.endpoint = HSSL_SERVER;
    if (m_server.newSslCtx(&param) != 0)
    {
        fprintf(stderr, "new SSL_CTX failed!\n");
        return false;
    }

    m_server.setThreadNum(4);
    return 0 == m_server.start() ? true : false;
}

void HttpSvrImp::Stop()
{
    m_server.stop();
}

uint16_t HttpSvrImp::GetPort()
{
    return m_server.port;
}

void HttpSvrImp::SetPort(uint16_t port)
{
    m_server.port = port;
}

uint16_t HttpSvrImp::GetHttpsPort()
{
    return m_server.https_port;
}

void HttpSvrImp::SetHttpsPort(uint16_t port)
{
    m_server.https_port = port;
}

void HttpSvrImp::Broadcast(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
    for (auto channel : m_wsChannels)
    {
        channel->send(msg);
    }
}

void HttpSvrImp::RegisterHttpRoute(const http_method& method,
                                   const std::string& path,
                                   HttpMethodFunc     func)
{
    if (method == http_method::HTTP_GET)
    {
        m_router.GET(path.c_str(), func);
    }
    else if (method == http_method::HTTP_POST)
    {
        m_router.POST(path.c_str(), func);
    }
    else
    {
        throw std::runtime_error("method not support");
    }
}

void HttpSvrImp::RegisterStaticRoute(const std::string& path, const std::string& dir)
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
        printf("onopen: GET %s, %s, ws_channel_size: %ld\n", req->Path().c_str(),
               channel->peeraddr().c_str(), m_wsChannels.size());
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