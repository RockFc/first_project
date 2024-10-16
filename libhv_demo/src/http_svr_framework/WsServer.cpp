#include "WsServer.h"

namespace wy
{
bool WsSvrImp::Start()
{
    // websocket
    RegisterWs();

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

void WsSvrImp::Stop()
{
    m_server.stop();
}

uint16_t WsSvrImp::GetPort()
{
    return m_server.port;
}

void WsSvrImp::SetPort(uint16_t port)
{
    m_server.port = port;
}

uint16_t WsSvrImp::GetHttpsPort()
{
    return m_server.https_port;
}

void WsSvrImp::SetHttpsPort(uint16_t port)
{
    m_server.https_port = port;
}

void WsSvrImp::Broadcast(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
    for (auto channel : m_wsChannels)
    {
        channel->send(msg.data(), msg.size(), WS_OPCODE_BINARY);
    }
}

void WsSvrImp::Broadcast(std::shared_ptr<ComMsgPack> pack)
{
    std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
    for (auto channel : m_wsChannels)
    {
        SendBinData(channel, pack);
    }
}

void WsSvrImp::RegisterGetDataFunc(HttpGetDataCallBack func)
{
    m_httpGetDataFunc = func;
}

bool WsSvrImp::SendData(std::shared_ptr<ComMsgPack> pack)
{
    std::unique_lock<std::mutex> lock(m_wsMsgToChannelMutex);
    auto                         it = m_wsMsgToChannel.find(pack->id);
    if (it == m_wsMsgToChannel.end())
    {
        std::cout << "Failed to find channel for msg id: " << pack->id << std::endl;
        return false;
    }
    auto wsChannel = it->second;
    m_wsMsgToChannel.erase(it);
    lock.unlock();
    return SendBinData(wsChannel, pack);
}

void WsSvrImp::RegisterWs()
{
    // m_ws.setPingInterval(10000);
    m_ws.onopen = [this](const WebSocketChannelPtr& channel, const HttpRequestPtr& req)
    {
        std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
        m_wsChannels.insert(channel);
        printf("onopen: GET %s, %s, ws_channel_size: %ld\n", req->Path().c_str(),
               channel->peeraddr().c_str(), m_wsChannels.size());
    };
    m_ws.onmessage = [this](const WebSocketChannelPtr& channel, const std::string& msg)
    {
        if (IsTextString(msg))
        {
            printf("onmessage: recv text msg, msg:[%s]\n", msg.c_str());
            channel->send("please send bin data!");
            return;
        }
        auto pack = CreatePackFromBinData(msg);
        pack->id  = ++m_wsMsgId;
        if (nullptr == m_httpGetDataFunc)
        {
            printf("onmessage: recv bin msg, but no GetDataFunc!\n");
            return;
        }
        std::unique_lock<std::mutex> lock(m_wsMsgToChannelMutex);
        m_wsMsgToChannel.insert({pack->id, channel});
        lock.unlock();

        // 使用方在执行GetDataFunc时，需要将msg的id保存起来,以便后续SendData时传回来
        m_httpGetDataFunc(pack);
    };
    m_ws.onclose = [this](const WebSocketChannelPtr& channel)
    {
        std::lock_guard<std::mutex> lock(m_wsChannelsMutex);
        m_wsChannels.erase(channel);
        printf("onclose: %s, ws_channel_size: %ld\n", channel->peeraddr().c_str(),
               m_wsChannels.size());
    };
}

bool WsSvrImp::IsTextString(const std::string& data)
{
    size_t printableCount    = 0;
    size_t nonPrintableCount = 0;

    for (char c : data)
    {
        // 检查字符是否是可打印字符或者常见的控制字符
        if (isprint(c) || c == '\n' || c == '\r' || c == '\t')
        {
            ++printableCount;
        }
        else
        {
            ++nonPrintableCount;
        }
    }

    // 如果所有字符都是可打印字符或常见的控制字符，则认为是文本
    return nonPrintableCount == 0;
}

bool WsSvrImp::SendBinData(WebSocketChannelPtr wsChannel, std::shared_ptr<ComMsgPack> pack)
{
    size_t dataSize;
    void*  data = pack->Serialize(dataSize);
    if (!data)
    {
        std::cout << "Failed to serialize data." << std::endl;
        return false;
    }

    auto ret = wsChannel->send(static_cast<const char*>(data), dataSize, WS_OPCODE_BINARY);
    delete[] data;
    return 0 != ret;
}

std::shared_ptr<ComMsgPack> WsSvrImp::CreatePackFromBinData(const std::string& msg)
{
    auto pack = std::make_shared<ComMsgPack>();
    pack->Deserialize(msg.data(), msg.size());
    return pack;
}

}  // namespace wy