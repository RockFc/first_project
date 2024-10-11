/*************************************************************
Copyright: BGI , Ltd.
Description: Http服务类。封装了HTTP和WebSocket服务的启动、停止以及接口注册功能。
Author: fuchao
Date: 2024-10-11
History:
**************************************************************/

#pragma once

#include "hv/HttpServer.h"
#include "hv/WebSocketServer.h"
#include "hv/hasync.h"
#include "hv/hthread.h"
#include <mutex>
#include <set>

namespace wy
{

/**
 * @class HttpSvrImp
 * @brief HttpSvrImp类负责HTTP服务的核心功能，包括接口注册、WebSocket支持以及广播功能。
 */
class HttpSvrImp
{
public:
    /**
     * @brief 构造函数，初始化Http服务的端口号，默认为8080。
     * @param port 服务端口号，默认值为8080。
     */
    explicit HttpSvrImp(uint16_t port = 8080) : m_port(port){};

    /**
     * @brief 析构函数，默认析构。
     */
    ~HttpSvrImp() = default;

    /**
     * @brief 启动HTTP服务。
     * @return 成功返回true，失败返回false。
     */
    bool Start();

    /**
     * @brief 停止HTTP服务。
     */
    void Stop();

    /**
     * @brief 获取当前服务的端口号。
     * @return 返回当前服务的端口号。
     */
    uint16_t GetPort();

    /**
     * @brief 设置服务端口号。
     * @param port 要设置的服务端口号。
     */
    void SetPort(uint16_t port);

    /**
     * @brief 向所有WebSocket连接的客户端广播消息。
     * @param msg 要广播的消息内容。
     */
    void Broadcast(const std::string& msg);

    /**
     * @brief 模板函数，用于注册处理HTTP请求的路由。
     * @tparam Req 请求对象类型。
     * @tparam Rsp 响应对象类型。
     * @param method HTTP请求的方法（如GET、POST等）。
     * @param path 请求路径。
     * @param func 处理请求的函数对象，接收Req和Rsp，返回布尔值。
     */
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

                                  // 如果请求体不为空，解析请求体为Json对象并赋值给req
                                  if (!ctx->body().empty())
                                  {
                                      const auto& req_json = hv::Json::parse(ctx->body());
                                      printf("req body: %s\n", ctx->body().c_str());
                                      req->FromJson(req_json);  // 将Json数据转换为请求对象
                                  }

                                  // 如果处理函数为空，则返回错误
                                  if (nullptr == func)
                                  {
                                      ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                      return ctx->send("http api func is nullptr");
                                  }

                                  // 调用处理函数，处理请求，并返回处理结果
                                  if (!func(req, rsp))
                                  {
                                      ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                      return ctx->send("http api return false");
                                  }

                                  // 将响应对象转换为Json格式并发送给客户端
                                  return ctx->send(rsp->ToJson()->dump());
                              }
                              catch (const std::exception& e)
                              {
                                  // 捕获异常并返回错误信息
                                  ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                                  return ctx->send(e.what());
                              }
                          });
    }

private:
    using HttpMethodFunc = std::function<int(const HttpContextPtr&)>;  // 定义HTTP处理函数类型

    /**
     * @brief 注册HTTP请求的具体路由和处理函数。
     * @param method HTTP请求的方法（GET、POST等）。
     * @param path 请求路径。
     * @param func HTTP处理函数。
     */
    void RegisterHttpRoute(const http_method& method, const std::string& path, HttpMethodFunc func);

    /**
     * @brief 注册静态文件路径，用于处理静态文件的请求。
     * @param path 静态文件的访问路径。
     * @param dir 静态文件所在的目录。
     */
    void RegisterStaticRoute(const std::string& path, const std::string& dir);

    /**
     * @brief 注册WebSocket服务，处理WebSocket连接。
     */
    void RegisterWs();

private:
    hv::HttpService      m_router;       // HTTP路由处理器
    hv::WebSocketService m_ws;           // WebSocket服务处理器
    hv::HttpServer       m_server;       // HTTP服务器
    uint16_t             m_port = 8080;  // 服务器端口号，默认为8080

    std::set<WebSocketChannelPtr> m_wsChannels;  // WebSocket连接的客户端集合，用于广播
    std::mutex                    m_wsChannelsMutex;  // 保护WebSocket连接的互斥锁

    std::map<std::string, HttpMethodFunc> m_httpFuncs;  // 存储注册的HTTP接口和对应的处理函数
};

/**
 * @class HttpSvr
 * @brief HttpSvr类是HttpSvrImp的单例接口，方便获取实例。
 */
class HttpSvr
{
public:
    /**
     * @brief 获取HttpSvrImp的单例实例。
     * @return HttpSvrImp实例的引用。
     */
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