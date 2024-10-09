#pragma once

#include "hv/json.hpp"

struct HttpMsgBase
{
    int msgID;

    virtual ~HttpMsgBase() {}
    // 纯虚函数：将对象转换为 JSON
    virtual hv::Json toJson() const = 0;
    // 纯虚函数：从 JSON 中构造对象
    virtual bool fromJson(const hv::Json& json) = 0;
};

struct LoginRequestMsg : public HttpMsgBase
{
    std::string username;
    std::string password;

    // 实现 toJson 函数
    hv::Json toJson() const override
    {
        hv::Json json;
        json["msgID"]    = msgID;
        json["username"] = username;
        json["password"] = password;
        return json;
    }

    // 实现 fromJson 函数
    bool fromJson(const hv::Json& json) override
    {
        if (json.contains("msgID") && json.contains("username") && json.contains("password"))
        {
            msgID    = json["msgID"];
            username = json["username"];
            password = json["password"];
            return true;
        }
        return false;
    }
};

struct LoginResponseMsg : public HttpMsgBase
{
    bool success;

    // 实现 toJson 函数
    hv::Json toJson() const override
    {
        hv::Json json;
        json["msgID"]   = msgID;
        json["success"] = success;
        return json;
    }

    // 实现 fromJson 函数
    bool fromJson(const hv::Json& json) override
    {
        if (json.contains("msgID") && json.contains("success"))
        {
            msgID   = json["msgID"];
            success = json["success"];
            return true;
        }
        return false;
    }
};
