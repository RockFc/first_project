/*
 * sample http server
 * more detail see examples/httpd
 *
 */

#include "hv/EventLoop.h"
#include "hv/HttpServer.h"
#include "hv/WebSocketServer.h"
#include "hv/hasync.h"   // import hv::async
#include "hv/hthread.h"  // import hv_gettid
#include "ws_svr_bin.h"

using namespace hv;

#define TEST_HTTPS 0

int main(int argc, char** argv)
{
    HV_MEMCHECK;

    int port = 0;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }
    if (port == 0)
        port = 10080;

    WebSocketService ws;
    // ws.setPingInterval(10000);
    ws.onopen = [](const WebSocketChannelPtr& channel, const HttpRequestPtr& req)
    {
        printf("onopen: GET %s\n", req->Path().c_str());
    };
    ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg)
    {
        // 反序列化二进制数据
        MyStruct deserialized = deserialize(msg.data(), msg.size());
        printMyStruct(&deserialized);

        // 模拟业务处理
        MyStruct pack;
        pack.intValue    = deserialized.intValue + 1;
        pack.stringValue = "hello, websocket client";
        pack.doubleValue = 2 * deserialized.doubleValue;

        // 序列化二进制数据
        size_t dataSize;
        void*  serializedData = serialize(&pack, &dataSize);
        if (!serializedData)
        {
            printf("Failed to serialize data.\n");
            return;
        }
        std::string body(reinterpret_cast<const char*>(serializedData), dataSize);

        // 发送二进制数据
        channel->send(body);
    };
    ws.onclose = [](const WebSocketChannelPtr& channel)
    {
        printf("onclose\n");
    };

    HttpService router;
    auto        loop = std::make_shared<EventLoop>();

    // middleware
    router.AllowCORS();
    router.Use(
        [](HttpRequest* req, HttpResponse* resp)
        {
            resp->SetHeader("X-Request-tid", hv::to_string(hv_gettid()));
            return HTTP_STATUS_NEXT;
        });

    HttpServer server;
    server.service = &router;
    server.ws      = &ws;
    server.port    = port;

    // uncomment to test multi-processes
    // server.setProcessNum(4);
    // uncomment to test multi-threads
    // server.setThreadNum(4);

    server.start();

    printf("press Enter to stop server...\n");

    loop->run();
    while (getchar() != '\n')
        ;
    hv::async::cleanup();
    return 0;
}