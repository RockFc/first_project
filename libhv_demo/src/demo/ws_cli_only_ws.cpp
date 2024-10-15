#include "hv/WebSocketClient.h"
#include "ws_svr_bin.h"
#include <iostream>

hv::WebSocketClient client;

int InitWsCli()
{
    // 设置 WebSocket 服务器的 URL
    const char* url = "ws://localhost:10080/ws";  // 使用一个公开的 WebSocket Echo 测试服务器

    // 连接到服务器
    int ret = client.open(url);
    if (ret != 0)
    {
        std::cerr << "Failed to connect to WebSocket server, ret=" << ret << std::endl;
        return -1;
    }

    // 连接成功后的回调
    client.onopen = []()
    {
        std::cout << "Connected to WebSocket server" << std::endl;
    };

    // 接收消息时的回调
    client.onmessage = [](const std::string& msg)
    {
        // 反序列化接收到的二进制消息
        MyStruct deserialized = deserialize(msg.data(), msg.size());
        printMyStruct(&deserialized);
    };

    // 连接关闭时的回调
    client.onclose = []()
    {
        std::cout << "WebSocket connection closed" << std::endl;
    };
}

int SendBinMsg(const MyStruct& pack)
{
    // 序列化结构体
    size_t dataSize;
    void*  serializedData = serialize(&pack, &dataSize);
    if (!serializedData)
    {
        printf("Failed to serialize data.\n");
        return -1;
    }

    // 将二进制数据存储到 std::string 中
    std::string body(reinterpret_cast<const char*>(serializedData), dataSize);
    return client.send(body.data(), body.size(), WS_OPCODE_BINARY);
}

int main()
{
    InitWsCli();

    // 发送二进制消息到服务器
    MyStruct pack;
    pack.intValue    = 123;
    pack.stringValue = "Hello, Websocket server!";
    pack.doubleValue = 3.1415926;
    SendBinMsg(pack);

    // 等待一段时间，以便服务器有足够的时间处理消息
    std::this_thread::sleep_for(std::chrono::seconds(3));
    // 关闭 WebSocket 连接
    client.close();

    return 0;
}