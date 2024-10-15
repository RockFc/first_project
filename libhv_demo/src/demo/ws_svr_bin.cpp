/*
 * sample http server
 * more detail see examples/httpd
 *
 */

#include "ws_svr_bin.h"
#include "hv/EventLoop.h"
#include "hv/HttpServer.h"
#include "hv/WebSocketServer.h"
#include "hv/hasync.h"   // import hv::async
#include "hv/hthread.h"  // import hv_gettid

using namespace hv;

#define TEST_HTTPS 0

std::map<uint16_t, HttpContextPtr> m_httpContex;       // 保存http请求应答上下文
std::mutex                         m_httpContexMutex;  // 保护http请求应答上下文
std::atomic<uint16_t>              m_httpCtxId{0};     // http请求应答上下文自增id

void AddHttpContex(uint16_t id, HttpContextPtr ctx)
{
    std::lock_guard<std::mutex> lock(m_httpContexMutex);
    m_httpContex[id] = ctx;
}

bool SendData(MyStruct pack)
{
    auto                         id = pack.intValue;
    std::unique_lock<std::mutex> lock(m_httpContexMutex);
    auto                         it = m_httpContex.find(id);
    if (m_httpContex.end() == it)
    {
        return false;
    }
    auto   ctx = it->second;
    size_t dataSize;
    void*  serializedData = serialize(&pack, &dataSize);
    if (!serializedData)
    {
        printf("Failed to serialize data.\n");
        return false;
    }
    std::string body(reinterpret_cast<const char*>(serializedData), dataSize);
    ctx->setBody(body);
    m_httpContex.erase(id);
    lock.unlock();
    return ctx->send();
}

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
        printf("onmessage: %s\n", msg.c_str());
        channel->send(msg);
    };
    ws.onclose = [](const WebSocketChannelPtr& channel)
    {
        printf("onclose\n");
    };

    HttpService router;
    auto        loop = std::make_shared<EventLoop>();
    /* Static file service */
    // curl -v http://ip:port/
    router.Static("/", "./html");

    /* Forward proxy service */
    router.EnableForwardProxy();
    // curl -v http://httpbin.org/get --proxy http://127.0.0.1:10080
    router.AddTrustProxy("*httpbin.org");

    /* Reverse proxy service */
    // curl -v http://ip:port/httpbin/get
    router.Proxy("/httpbin/", "http://httpbin.org/");

    /* API handlers */
    // curl -v http://ip:port/echo -d "hello,world!"
    router.POST("/post_bin",
                [](const HttpContextPtr& ctx)
                {
                    // 解析二进制数据
                    const std::string body           = ctx->body();
                    auto              serializedData = body.data();
                    size_t            dataSize       = ctx->body().size();
                    MyStruct          deserialized   = deserialize(serializedData, dataSize);
                    printMyStruct(&deserialized);

                    // 模拟业务处理
                    deserialized.stringValue = "hello,http!";
                    deserialized.doubleValue *= 2;

                    // 序列化二进制数据
                    size_t dataSize2;
                    void*  serializedData2 = serialize(&deserialized, &dataSize2);
                    if (!serializedData2)
                    {
                        ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                        return ctx->send("Failed to serialize data");
                    }
                    std::string bodystr(reinterpret_cast<const char*>(serializedData2), dataSize2);

                    ctx->setContentType(http_content_type::APPLICATION_OCTET_STREAM);
                    ctx->setBody(bodystr);
                    return ctx->send();
                });

    router.POST("/post_bin_delay",
                [&loop](const HttpContextPtr& ctx)
                {
                    // 解析二进制数据
                    const std::string body           = ctx->body();
                    auto              serializedData = body.data();
                    size_t            dataSize       = ctx->body().size();
                    MyStruct          deserialized   = deserialize(serializedData, dataSize);
                    printMyStruct(&deserialized);
                    m_httpCtxId++;

                    ctx->setContentType(http_content_type::APPLICATION_OCTET_STREAM);

                    AddHttpContex(m_httpCtxId.load(), ctx);
                    loop->setTimeout(5000,
                                     [deserialized](TimerID)
                                     {
                                         // 模拟业务处理
                                         MyStruct pack;
                                         pack.intValue    = m_httpCtxId.load();
                                         pack.stringValue = "hello,http!";
                                         pack.doubleValue = deserialized.doubleValue * 2;
                                         SendData(pack);
                                     });
                    return 0;
                });

    /**
     * @brief 发送业务消息包到http客户端。
     * @param msg 业务消息包。
     */

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
#if TEST_HTTPS
    server.https_port = 8443;
    hssl_ctx_opt_t param;
    memset(&param, 0, sizeof(param));
    param.crt_file = "cert/server.crt";
    param.key_file = "cert/server.key";
    param.endpoint = HSSL_SERVER;
    if (server.newSslCtx(&param) != 0)
    {
        fprintf(stderr, "new SSL_CTX failed!\n");
        return -20;
    }
#endif

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