#include "wy_http_message.h"

namespace wy
{
std::shared_ptr<hv::Json> LoginReqtMsg::ToJson() const
{
    auto json           = std::make_shared<hv::Json>();
    (*json)["username"] = username;
    (*json)["password"] = password;
    return json;
}

bool LoginReqtMsg::FromJson(std::shared_ptr<hv::Json> json)
{
    if (json->contains("username") && json->contains("password"))
    {
        username = (*json)["username"];
        password = (*json)["password"];
        return true;
    }
    return false;
}

std::shared_ptr<hv::Json> LoginRspMsg::ToJson() const
{
    auto json          = std::make_shared<hv::Json>();
    (*json)["success"] = success;
    return json;
}

bool LoginRspMsg::FromJson(std::shared_ptr<hv::Json> json)
{
    if (json->contains("success"))
    {
        success = (*json)["success"];
        return true;
    }
    return false;
}
}  // namespace wy