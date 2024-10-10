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
using HttpMethodFunc = std::function<int(const HttpContextPtr&)>;
class HttpSvrImp
{
public:
    explicit HttpSvrImp(uint16_t port = 8080) : m_port(port){};
    ~HttpSvrImp() = default;

    bool     start();
    void     stop();
    uint16_t port();
    void     set_port(uint16_t port);
    void     broadcast(const std::string& msg);
    void     boradcast(std::shared_ptr<hv::Json> msg);

    template <typename Req, typename Rsp>
    void add_http_interface(const http_method&                                              mothod,
                            const std::string&                                              path,
                            std::function<bool(std::shared_ptr<Req>, std::shared_ptr<Rsp>)> func)
    {
        register_http_interface(mothod, path,
                                [mothod, path, func](const HttpContextPtr& ctx)
                                {
                                    printf("mothod[%d], path[%s]\n", ( int )mothod, path.c_str());
                                    auto req = std::make_shared<Req>();
                                    auto rsp = std::make_shared<Rsp>();
                                    auto req_json
                                        = std::make_shared<hv::Json>(hv::Json::parse(ctx->body()));
                                    printf("ctx->body(): %s\n", ctx->body().c_str());
                                    printf("req_json: %s\n", req_json->dump(2).c_str());
                                    req->FromJson(req_json);

                                    if (!func(req, rsp))
                                    {
                                        ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                        return ctx->send("http interface error");
                                    }

                                    return ctx->send(rsp->ToJson()->dump(2));
                                });
    }

private:
    void register_http_interface(const http_method& mothod,
                                 const std::string& path,
                                 HttpMethodFunc     func);
    void register_http_static(const std::string& path, const std::string& dir);
    void register_ws();
    // 所有接口需要在这里注册
    void register_routes();

private:
    hv::HttpService      m_router;
    hv::WebSocketService m_ws;
    hv::HttpServer       m_server;
    uint16_t             m_port = 8080;

    std::set<WebSocketChannelPtr>         m_ws_channels;
    std::map<std::string, HttpMethodFunc> m_http_funcs;
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