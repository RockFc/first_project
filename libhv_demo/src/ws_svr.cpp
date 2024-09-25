/*
 * sample http server
 * more detail see examples/httpd
 *
 */

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
    // curl -v http://ip:port/ping
    router.GET("/ping",
               [](HttpRequest* req, HttpResponse* resp)
               {
                   return resp->String("pong");
               });

    // curl -v http://ip:port/data
    router.GET("/data",
               [](HttpRequest* req, HttpResponse* resp)
               {
                   static char data[] = "0123456789";
                   return resp->Data(data, 10 /*, false */);
               });

    // curl -v http://ip:port/paths
    router.GET("/paths",
               [&router](HttpRequest* req, HttpResponse* resp)
               {
                   return resp->Json(router.Paths());
               });

    // curl -v http://ip:port/get?env=1
    router.GET("/get",
               [](const HttpContextPtr& ctx)
               {
                   hv::Json resp;
                   resp["origin"]  = ctx->ip();
                   resp["url"]     = ctx->url();
                   resp["args"]    = ctx->params();
                   resp["headers"] = ctx->headers();
                   return ctx->send(resp.dump(2));
               });

    router.GET("/geterrjson",
               [](const HttpContextPtr& ctx)
               {
                   std::string rsp_str = R"(
                  {
                  "body":
                        {
                          "app_id": "123456",
                          "app_secret": "abcdefg"
                        }}
                  }
                  )";
                   return ctx->send(rsp_str);
               });

    router.GET("/geterrcode",
               [](const HttpContextPtr& ctx)
               {
                   ctx->setStatus(( http_status )404);
                   return ctx->send();
               });

    // curl -v http://ip:port/echo -d "hello,world!"
    router.POST("/echo",
                [](const HttpContextPtr& ctx)
                {
                    return ctx->send(ctx->body(), ctx->type());
                });

    // curl -v http://ip:port/user/123
    router.GET("/user/{id}",
               [](const HttpContextPtr& ctx)
               {
                   hv::Json resp;
                   resp["id"] = ctx->param("id");
                   return ctx->send(resp.dump(2));
               });

    // curl -v http://ip:port/async
    router.GET("/async",
               [](const HttpRequestPtr& req, const HttpResponseWriterPtr& writer)
               {
                   writer->Begin();
                   writer->WriteHeader("X-Response-tid", hv_gettid());
                   writer->WriteHeader("Content-Type", "text/plain");
                   writer->WriteBody("This is an async response.\n");
                   writer->End();
               });

    // curl -v http://ip:port/get_json
    router.GET("/get_json",
               [](const HttpContextPtr& ctx)
               {
                   hv::Json resp;
                   resp["id"]   = "17";
                   resp["name"] = "rock";
                   ctx->setHeader("Access-Control-Allow-Origin", "*");
                   return ctx->send(resp.dump(2));
               });

    // curl -v http://ip:port/echo -d "hello,world!"
    router.POST("/echo_json",
                [](const HttpContextPtr& ctx)
                {
                    hv::Json req;
                    try
                    {
                        req = hv::Json::parse(ctx->body());
                    }
                    catch (const std::exception& e)
                    {
                        ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                        return ctx->send(e.what());
                    }
                    return ctx->send(req.dump(2));
                });

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

    // press Enter to stop
    while (getchar() != '\n')
        ;
    hv::async::cleanup();
    return 0;
}