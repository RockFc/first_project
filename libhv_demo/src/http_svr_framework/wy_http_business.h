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
    void Init()
    {
        // 建议在业务类中注册Http路由,当然也可在外部（例如main函数中）注册
        wy::HttpSvr::Instance().RegisterRoute(
            http_method::HTTP_GET, "/get_json",
            std::function<bool(std::shared_ptr<wy::GetJsonMsg>, std::shared_ptr<wy::GetJsonMsg>)>(
                std::bind(&wy::HttpBusiness::GetJson, this, std::placeholders::_1,
                          std::placeholders::_2)));
    }

    bool Login(std::shared_ptr<LoginReqtMsg> req, std::shared_ptr<LoginRspMsg> rsp)
    {
        if (req->username == "admin" && req->password == "123456")
        {
            printf("login success\n");
            rsp->success = true;
            ontime();
            return true;
        }
        else
        {
            printf("login failed\n");
            rsp->success = false;
            return false;
        }
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
    void ontime()
    {
        const hv::EventLoopPtr& loop = m_loopThread.loop();
        // runEvery 2s
        loop->setInterval(2000,
                          [](hv::TimerID)
                          {
                              printf("ontime\n");
                              wy::HttpSvr::Instance().Broadcast("you are logined!");
                          });

        // runEvery 5s
        loop->setInterval(5000,
                          [](hv::TimerID)
                          {
                              auto       json                     = std::make_shared<hv::Json>();
                              char       str[DATETIME_FMT_BUFLEN] = {0};
                              datetime_t dt                       = datetime_now();
                              datetime_fmt(&dt, str);
                              (*json)["time_str"] = str;
                              (*json)["id"]       = 7;
                              wy::HttpSvr::Instance().Broadcast(json->dump(2));
                          });
        m_loopThread.start();
    }

private:
    hv::EventLoopThread m_loopThread;
};
}  // namespace wy
