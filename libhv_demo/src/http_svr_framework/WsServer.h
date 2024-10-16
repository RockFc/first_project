/*************************************************************
Copyright: BGI , Ltd.
Description: Http服务类。封装了HTTP和WebSocket服务的启动、停止以及接口注册功能。
Author: fuchao
Date: 2024-10-11
History:
**************************************************************/

#pragma once

#include "HttpMessage.h"
#include "hv/HttpServer.h"
#include "hv/WebSocketServer.h"
#include "hv/hasync.h"
#include "hv/hthread.h"
#include <mutex>
#include <set>

namespace wy
{

using HttpGetDataCallBack
    = std::function<bool(std::shared_ptr<ComMsgPack>)>;  // Http接收数据函数类型

/**
 * @class WsSvrImp
 * @brief HttpSvrImp类负责HTTP服务的核心功能，包括接口注册、WebSocket支持以及广播功能。
 */
class WsSvrImp
{
public:
    /**
     * @brief 构造函数，默认构造。
     */
    explicit WsSvrImp() = default;

    /**
     * @brief 析构函数，默认析构。
     */
    ~WsSvrImp() = default;

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
     * @brief 获取当前服务的http端口号。
     * @return 返回当前服务的http端口号。
     */
    uint16_t GetPort();

    /**
     * @brief 设置服务http端口号。
     * @param port 要设置的服务http端口号。
     */
    void SetPort(uint16_t port);

    /**
     * @brief 获取当前服务的https端口号。
     * @return 返回当前服务的https端口号。
     * */
    uint16_t GetHttpsPort();

    /**
     * @brief 设置服务https端口号。
     * @param port 要设置的服务https端口号。
     * */
    void SetHttpsPort(uint16_t port);

    /**
     * @brief 向所有WebSocket连接的客户端广播消息。
     * @param msg 要广播的二进制消息内容。
     **/
    void Broadcast(const std::string& msg);

    /**
     * @brief 向所有WebSocket连接的客户端广播消息。
     * @param pack 要广播的业务结构体。
     **/
    void Broadcast(std::shared_ptr<ComMsgPack> pack);

    /**
     * @brief 注册获取数据函数，用于处理WebSocket连接。
     * @param func 获取数据的函数指针。
     * */
    void RegisterGetDataFunc(HttpGetDataCallBack func);

    /**
     * @brief 向指定的WebSocket连接发送数据。
     * @param pack 要发送的数据。
     * @return 发送成功返回true，发送失败返回false。
     * */
    bool SendData(std::shared_ptr<ComMsgPack> pack);

private:
    void RegisterWs();

    bool IsTextString(const std::string& data);

    bool SendBinData(WebSocketChannelPtr wsChannel, std::shared_ptr<ComMsgPack> pack);

    std::shared_ptr<ComMsgPack> CreatePackFromBinData(const std::string& msg);

private:
    hv::HttpService      m_router;             // HTTP路由处理器
    hv::WebSocketService m_ws;                 // WebSocket服务处理器
    hv::HttpServer       m_server;             // HTTP服务器
    uint16_t             m_port      = 8080;   // 服务器http端口号，默认为8080
    uint16_t             m_httpsPort = 10043;  // 服务器https端口号，默认为10043

    std::set<WebSocketChannelPtr> m_wsChannels;  // WebSocket连接的客户端集合，用于广播
    std::mutex                    m_wsChannelsMutex;  // 保护WebSocket连接的互斥锁

    HttpGetDataCallBack m_httpGetDataFunc = nullptr;  // 保存Http获取数据处理函数

    std::map<size_t, WebSocketChannelPtr> m_wsMsgToChannel;  // WebSocket消息到连接的映射
    std::mutex          m_wsMsgToChannelMutex;  // 保护WebSocket消息到连接的互斥锁
    std::atomic<size_t> m_wsMsgId = 0;          // WebSocket消息ID，用于区分不同的消息
};

/**
 * @class WsSvr
 * @brief WsSvr类是WsSvrImp的单例接口，方便获取实例。
 */
class WsSvr
{
public:
    /**
     * @brief 获取HttpSvrImp的单例实例。
     * @return HttpSvrImp实例的引用。
     */
    static WsSvrImp& Instance()
    {
        static WsSvrImp instance;
        return instance;
    }

private:
    WsSvr()                        = default;
    ~WsSvr()                       = default;
    WsSvr(const WsSvr&)            = delete;
    WsSvr& operator=(const WsSvr&) = delete;
    WsSvr(WsSvr&&)                 = delete;
    WsSvr& operator=(WsSvr&&)      = delete;
};

}  // namespace wy