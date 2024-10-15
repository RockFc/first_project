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
    virtual bool FromJson(const hv::Json& json) = 0;
};

struct LoginReqtMsg : public HttpMsgBase
{
    // 业务字段
    std::string username;
    std::string password;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(const hv::Json& json) override;
};

struct LoginRspMsg : public HttpMsgBase
{
    // 业务字段
    bool success;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(const hv::Json& json) override;
};

struct LogoutMsg : public HttpMsgBase
{
    std::string username;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(const hv::Json& json) override;
};

struct EchoMsg : public HttpMsgBase
{
    std::string name;
    uint32_t    id;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(const hv::Json& json) override;
};

struct GetJsonMsg : public HttpMsgBase
{
    std::string time;
    uint32_t    id;

    std::shared_ptr<hv::Json> ToJson() const override;
    bool                      FromJson(const hv::Json& json) override;
};

// 定义结构体
struct ComMsgPack
{
    int         id;
    std::string name;
    double      score;

    // 序列化成员函数：将结构体序列化为字节流
    // 返回值是序列化后的字节流，dataSize 是序列化数据的大小
    void* Serialize(size_t& dataSize) const
    {
        // 计算序列化数据的总大小
        size_t nameLength = name.length();
        dataSize          = sizeof(id) + sizeof(size_t) + nameLength + sizeof(score);

        // 分配内存存储序列化的数据
        void* data = malloc(dataSize);
        if (!data)
        {
            return nullptr;  // 分配失败
        }

        // 使用 memcpy 进行序列化
        size_t offset = 0;

        // 复制 id
        memcpy(( char* )data + offset, &id, sizeof(id));
        offset += sizeof(id);

        // 复制 name 的长度和内容
        memcpy(( char* )data + offset, &nameLength, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(( char* )data + offset, name.data(), nameLength);
        offset += nameLength;

        // 复制 score
        memcpy(( char* )data + offset, &score, sizeof(score));

        return data;
    }

    // 反序列化成员函数：从字节流中重建结构体
    // 传入字节流和数据大小
    void Deserialize(const void* data, size_t dataSize)
    {
        size_t offset = 0;

        // 使用 memcpy 进行反序列化
        // 反序列化 id
        memcpy(&id, ( char* )data + offset, sizeof(id));
        offset += sizeof(id);

        // 反序列化 name 的长度和内容
        size_t nameLength;
        memcpy(&nameLength, ( char* )data + offset, sizeof(size_t));
        offset += sizeof(size_t);

        name.resize(nameLength);
        memcpy(&name[0], ( char* )data + offset, nameLength);
        offset += nameLength;

        // 反序列化 score
        memcpy(&score, ( char* )data + offset, sizeof(score));
    }
};
}  // namespace wy
