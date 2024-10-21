#include "hv/WebSocketClient.h"
#include <chrono>
#include <iostream>
#include <thread>

// WebSocket 初始化函数
void InitWsCli(const std::string& url)
{
    // 创建 WebSocket 客户端
    auto client = std::make_shared<hv::WebSocketClient>();

    // 设置连接成功回调
    client->onopen = [client, url]()
    {
        std::cout << "Connected to WebSocket server: " << url << std::endl;
        client->send("Hello, " + url);
    };

    // 设置接收消息回调
    client->onmessage = [url](const std::string& msg)
    {
        std::cout << "Received message: " << msg << " from " << url << std::endl;
    };

    // 设置连接关闭回调
    client->onclose = []()
    {
        std::cout << "WebSocket connection closed" << std::endl;
    };

    // 连接到服务器
    int ret = client->open(url.c_str());
    if (ret != 0)
    {
        std::cerr << "Failed to connect to WebSocket server, ret=" << ret << std::endl;
        return;
    }

    // 等待一段时间，以便观察连接状态
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main()
{
    // 启动两个线程，分别连接到不同的 WebSocket 服务器
    std::string url1 = "ws://localhost:10080/ws/notify/1";
    std::thread thread1(InitWsCli, url1);

    std::string url2 = "ws://localhost:10080/ws/notify/2";
    std::thread thread2(InitWsCli, url2);

    // 等待线程完成
    thread1.join();
    thread2.join();

    return 0;
}