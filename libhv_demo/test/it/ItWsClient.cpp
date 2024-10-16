#include "HttpMessage.h"
#include "hv/WebSocketClient.h"
#include <gtest/gtest.h>
#include <iostream>

class WsServerTest : public testing::Test
{
public:
    void static SetUpTestCase()
    {
        InitWsCli();
    }
    void static TearDownCase() {}

    static int InitWsCli()
    {
        // 设置 WebSocket 服务器的 URL
        const char* url = "ws://localhost:8080/ws";  // 使用一个公开的 WebSocket Echo 测试服务器

        // 连接到服务器
        int ret = g_client.open(url);
        if (ret != 0)
        {
            std::cerr << "Failed to connect to WebSocket server, ret=" << ret << std::endl;
            return -1;
        }

        // 连接成功后的回调
        g_client.onopen = []()
        {
            std::cout << "Connected to WebSocket server" << std::endl;
        };

        // 接收消息时的回调
        g_client.onmessage = [](const std::string& msg)
        {
            // 反序列化接收到的二进制消息
            auto pack = std::make_shared<wy::ComMsgPack>();
            pack->Deserialize(msg.data(), msg.size());
            pack->Print();
        };

        // 连接关闭时的回调
        g_client.onclose = []()
        {
            std::cout << "WebSocket connection closed" << std::endl;
        };
    }

    static int SendBinMsg(std::shared_ptr<wy::ComMsgPack> pack)
    {
        size_t dataSize;
        void*  data = pack->Serialize(dataSize);
        if (!data)
        {
            std::cout << "Failed to serialize data." << std::endl;
            return false;
        }

        auto ret = g_client.send(static_cast<const char*>(data), dataSize, WS_OPCODE_BINARY);
        delete[] data;
        return ret;
    }

protected:
    virtual void SetUp() {}
    virtual void TearDown()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

private:
    static hv::WebSocketClient g_client;
};
hv::WebSocketClient WsServerTest::g_client;

TEST_F(WsServerTest, SendBinMsg_1)
{
    auto pack  = std::make_shared<wy::ComMsgPack>();
    pack->id   = 100;  // 业务无需填写，填了也会被覆盖
    pack->name = "hello, I am client";

    for (size_t i = 1; i <= 5; i++)
    {
        pack->score = 100 + i;
        SendBinMsg(pack);
    }
}

TEST_F(WsServerTest, WaitForRecv_1)
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
}