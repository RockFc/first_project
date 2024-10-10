#pragma once

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
};
}  // namespace wy
