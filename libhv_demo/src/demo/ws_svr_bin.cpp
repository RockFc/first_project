/*
 * sample http server
 * more detail see examples/httpd
 *
 */

#include "ws_svr_bin.h"
#include "hv/HttpServer.h"
#include "hv/WebSocketServer.h"
#include "hv/hasync.h"   // import hv::async
#include "hv/hthread.h"  // import hv_gettid

using namespace hv;

/*
 *
 * @server  bin/3t_http_svr 8080
 *
 * @client  curl -v http://127.0.0.1:10080/ping
 *          curl -v https://127.0.0.1:8443/ping --insecure
 *          bin/curl -v http://127.0.0.1:10080/ping
 *          bin/curl -v https://127.0.0.1:8443/ping
 *
 */
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
        printf("onmessage: %s\n", msg.c_str());
        channel->send(msg);
    };
    ws.onclose = [](const WebSocketChannelPtr& channel)
    {
        printf("onclose\n");
    };

    HttpService router;

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
                    deserialized.intValue    = 100;
                    deserialized.stringValue = "hello,http!";
                    deserialized.doubleValue = 300.0;

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
    // press Enter to stop
    while (getchar() != '\n')
        ;
    hv::async::cleanup();
    return 0;
}