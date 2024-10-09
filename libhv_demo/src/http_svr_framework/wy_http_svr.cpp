#include "wy_http_svr.h"

namespace wy
{
bool HttpSvr::start()
{
    // websocket
    register_ws();

    // route
    register_routes();

    // middleware
    m_router.AllowCORS();
    m_router.Use(
        [](HttpRequest* req, HttpResponse* resp)
        {
            resp->SetHeader("X-Request-tid", hv::to_string(hv_gettid()));
            return HTTP_STATUS_NEXT;
        });

    // start server
    m_server.service = &m_router;
    m_server.ws      = &m_ws;
    m_server.port    = m_port;
    return m_server.start();
}

void HttpSvr::stop()
{
    m_server.stop();
}

uint16_t HttpSvr::port()
{
    return m_server.port;
}

void HttpSvr::register_routes()
{
    m_router.Static("/", "./html");

    m_router.GET("/get_json",
                 [](const HttpContextPtr& ctx)
                 {
                     hv::Json resp;
                     resp["id"]   = "17";
                     resp["name"] = "rock";
                     ctx->setHeader("Content-Type", "application/json");
                     return ctx->send(resp.dump(2));
                 });

    m_router.POST("/echo_json",
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
}

void HttpSvr::register_ws()
{
    // m_ws.setPingInterval(10000);
    m_ws.onopen = [](const WebSocketChannelPtr& channel, const HttpRequestPtr& req)
    {
        printf("onopen: GET %s\n", req->Path().c_str());
    };
    m_ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg)
    {
        printf("onmessage: %s\n", msg.c_str());
        channel->send(msg);
    };
    m_ws.onclose = [](const WebSocketChannelPtr& channel)
    {
        printf("onclose\n");
    };
}
}  // namespace wy