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

#define TEST_HTTPS 0

namespace wy
{
class HttpSvr
{
public:
    explicit HttpSvr(uint16_t port = 8080) : m_port(port){};
    HttpSvr(const HttpSvr&)            = delete;
    HttpSvr& operator=(const HttpSvr&) = delete;
    HttpSvr(HttpSvr&&)                 = delete;
    HttpSvr& operator=(HttpSvr&&)      = delete;
    ~HttpSvr()                         = default;

    bool     start();
    void     stop();
    uint16_t port();

private:
    void register_routes();
    void register_ws();

private:
    hv::HttpService      m_router;
    hv::WebSocketService m_ws;
    hv::HttpServer       m_server;
    uint16_t             m_port = 8080;
};
}  // namespace wy