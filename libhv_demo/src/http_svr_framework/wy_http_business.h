#pragma once

#include "hv/htime.h"
#include "wy_http_message.h"

namespace wy
{
class HttpBusiness
{
public:
    bool Login(std::shared_ptr<LoginReqtMsg> req, std::shared_ptr<LoginRspMsg> rsp)
    {
        if (req->username == "admin" && req->password == "123456")
        {
            printf("login success\n");
            rsp->success = true;
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
};
}  // namespace wy
