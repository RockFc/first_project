/*************************************************************
Copyright: BGI , Ltd.
Description: Http服务类。
Author: fuchao
Date: 2024-10-10
History:
**************************************************************/

#pragma once

#include "hv/HttpServer.h"
#include "hv/WebSocketServer.h"
#include "hv/hasync.h"   // import hv::async
#include "hv/hthread.h"  // import hv_gettid
#include <set>

#define TEST_HTTPS 0

namespace wy
{
class HttpSvrImp
{
public:
    explicit HttpSvrImp(uint16_t port = 8080) : m_port(port){};
    ~HttpSvrImp() = default;

    bool     Start();
    void     Stop();
    uint16_t GetPort();
    void     SetPort(uint16_t port);
    void     Broadcast(const std::string& msg);

    template <typename Req, typename Rsp>
    void RegisterRoute(const http_method&                                              method,
                       const std::string&                                              path,
                       std::function<bool(std::shared_ptr<Req>, std::shared_ptr<Rsp>)> func)
    {
        RegisterHttpRoute(method, path,
                          [method, path, func](const HttpContextPtr& ctx)
                          {
                              try
                              {
                                  printf("method[%d], path[%s]\n", ( int )method, path.c_str());
                                  auto req = std::make_shared<Req>();
                                  auto rsp = std::make_shared<Rsp>();

                                  if (!ctx->body().empty())
                                  {
                                      const auto& req_json = hv::Json::parse(ctx->body());
                                      printf("req body: %s\n", ctx->body().c_str());
                                      req->FromJson(req_json);
                                  }

                                  if (nullptr == func)
                                  {
                                      ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                      return ctx->send("http api func is nullptr");
                                  }

                                  if (!func(req, rsp))
                                  {
                                      ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                      return ctx->send("http api return false");
                                  }
                                  return ctx->send(rsp->ToJson()->dump());
                              }
                              catch (const std::exception& e)
                              {
                                  ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                  return ctx->send(e.what());
                              }
                          });
    }

private:
    using HttpMethodFunc = std::function<int(const HttpContextPtr&)>;

    void RegisterHttpRoute(const http_method& method, const std::string& path, HttpMethodFunc func);
    void RegisterStaticRoute(const std::string& path, const std::string& dir);
    void RegisterWs();

private:
    hv::HttpService      m_router;
    hv::WebSocketService m_ws;
    hv::HttpServer       m_server;
    uint16_t             m_port = 8080;

    std::set<WebSocketChannelPtr>         m_wsChannels;
    std::mutex                            m_wsChannelsMutex;
    std::map<std::string, HttpMethodFunc> m_httpFuncs;
};

class HttpSvr
{
public:
    static HttpSvrImp& Instance()
    {
        static HttpSvrImp instance;
        return instance;
    }

private:
    HttpSvr()                          = default;
    ~HttpSvr()                         = default;
    HttpSvr(const HttpSvr&)            = delete;
    HttpSvr& operator=(const HttpSvr&) = delete;
    HttpSvr(HttpSvr&&)                 = delete;
    HttpSvr& operator=(HttpSvr&&)      = delete;
};
}  // namespace wy