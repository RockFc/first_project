#pragma once

#include "hv/http_content.h"

namespace wy
{
struct HttpMsgBase
{
    virtual ~HttpMsgBase() {}
    // 纯虚函数：将对象转换为 JSON
    virtual std::shared_ptr<hv::Json> ToJson() const = 0;
    // 纯虚函数：从 JSON 中构造对象
    virtual bool FromJson(std::shared_ptr<hv::Json> json) = 0;
};

struct LoginReqtMsg : public HttpMsgBase
{
    // 业务字段
    std::string username;
    std::string password;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(std::shared_ptr<hv::Json> json) override;
};

struct LoginRspMsg : public HttpMsgBase
{
    // 业务字段
    bool success;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(std::shared_ptr<hv::Json> json) override;
};
}  // namespace wy
