#pragma once

#include "hv/EventLoopThread.h"
#include "hv/htime.h"
#include "wy_http_message.h"
#include "wy_http_svr.h"

namespace wy
{
class HttpBusiness
{
public:
    bool Init()
    {
        // 建议在业务类中注册Http路由,当然也可在外部（例如main函数中）注册
        wy::HttpSvr::Instance().RegisterRoute(
            http_method::HTTP_POST, "/login",
            std::function<bool(std::shared_ptr<wy::LoginReqtMsg>,
                               std::shared_ptr<wy::LoginRspMsg>)>(
                std::bind(&wy::HttpBusiness::Login, this, std::placeholders::_1,
                          std::placeholders::_2)));

        wy::HttpSvr::Instance().RegisterRoute(
            http_method::HTTP_POST, "/logout",
            std::function<bool(std::shared_ptr<wy::LogoutMsg>, std::shared_ptr<wy::LogoutMsg>)>(
                std::bind(&wy::HttpBusiness::Logout, this, std::placeholders::_1,
                          std::placeholders::_2)));

        // wy::HttpSvr::Instance().RegisterRoute(
        //     http_method::HTTP_GET, "/get_json",
        //     std::function<bool(std::shared_ptr<wy::GetJsonMsg>,
        //     std::shared_ptr<wy::GetJsonMsg>)>(
        //         std::bind(&wy::HttpBusiness::GetJson, this, std::placeholders::_1,
        //                   std::placeholders::_2)));

        // 除上述bind方式外，还可以使用lambda表达式(推荐使用)注册路由
        // 由于RegisterRoute是模板函数，所有类型参数必须显式指定
        wy::HttpSvr::Instance().RegisterRoute(
            http_method::HTTP_GET, "/get_json",
            std::function<bool(std::shared_ptr<wy::GetJsonMsg>, std::shared_ptr<wy::GetJsonMsg>)>(
                [this](auto req, auto rsp)
                {
                    return GetJson(req, rsp);
                }));

        // 启动Http服务
        if (wy::HttpSvr::Instance().Start())
        {
            printf("http server start success on %d\n", wy::HttpSvr::Instance().GetPort());
            return true;
        }
        else
        {
            printf("http server start failed\n");
            return false;
        }
    }

    bool Login(std::shared_ptr<LoginReqtMsg> req, std::shared_ptr<LoginRspMsg> rsp)
    {
        if (req->username == "admin" && req->password == "123456")
        {
            printf("login success\n");
            rsp->success = true;
            StartTimer();
            m_timerIsRunning = true;
            return true;
        }
        else
        {
            printf("login failed\n");
            rsp->success = false;
            return false;
        }
    }

    bool Logout(std::shared_ptr<LogoutMsg> req, std::shared_ptr<LogoutMsg> rsp)
    {
        if (req->username == "admin")
        {
            printf("logout success\n");
            rsp->username    = "admin";
            m_timerIsRunning = false;
        }
        return true;
    }

    bool Echo(std::shared_ptr<EchoMsg> req, std::shared_ptr<EchoMsg> rsp)
    {
        *rsp = *req;
        return true;
    }

    bool GetJson(std::shared_ptr<GetJsonMsg>, std::shared_ptr<GetJsonMsg> rsp)
    {
        char       str[DATETIME_FMT_BUFLEN] = {0};
        datetime_t dt                       = datetime_now();
        datetime_fmt(&dt, str);
        rsp->time = str;
        rsp->id   = 7;
        return true;
    }

private:
    void StartTimer()
    {
        if (m_timerIsRunning)
        {
            return;
        }

        const hv::EventLoopPtr& loop = m_loopThread.loop();
        // runEvery 2s
        loop->setInterval(2000,
                          [&loop, this](hv::TimerID id)
                          {
                              if (!m_timerIsRunning.load())
                              {
                                  loop->killTimer(id);
                              }

                              printf("ontime\n");

                              auto       json                     = std::make_shared<hv::Json>();
                              char       str[DATETIME_FMT_BUFLEN] = {0};
                              datetime_t dt                       = datetime_now();
                              datetime_fmt(&dt, str);
                              (*json)["time_str"] = str;
                              (*json)["msg"]      = "you are logined!";
                              wy::HttpSvr::Instance().Broadcast(json->dump(2));
                          });
        if (!m_loopThread.isRunning())
        {
            m_loopThread.start();
        }
    }

private:
    hv::EventLoopThread m_loopThread;
    std::atomic<bool>   m_timerIsRunning{false};
};
}  // namespace wy
