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

bool LoginReqtMsg::FromJson(const hv::Json& json)
{
    if (json.contains("username") && json.contains("password"))
    {
        username = json["username"];
        password = json["password"];
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

bool LoginRspMsg::FromJson(const hv::Json& json)
{
    if (json.contains("success"))
    {
        success = json["success"];
        return true;
    }
    return false;
}

std::shared_ptr<hv::Json> EchoMsg::ToJson() const
{
    auto json       = std::make_shared<hv::Json>();
    (*json)["name"] = name;
    (*json)["id"]   = id;
    return json;
}
bool EchoMsg::FromJson(const hv::Json& json)
{
    if (json.contains("name") && json.contains("id"))
    {
        name = json["name"];
        id   = json["id"];
        return true;
    }
    return false;
}

std::shared_ptr<hv::Json> GetJsonMsg::ToJson() const
{
    auto json       = std::make_shared<hv::Json>();
    (*json)["time"] = time;
    (*json)["id"]   = id;
    return json;
}

bool GetJsonMsg::FromJson(const hv::Json& json)
{
    if (json.contains("time") && json.contains("id"))
    {
        time = json["time"];
        id   = json["id"];
        return true;
    }
    return false;
}

}  // namespace wy