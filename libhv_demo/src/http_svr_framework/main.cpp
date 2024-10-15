#include "HttpBusiness.h"
#include "HttpServer.h"

// 测试函数
void DoComMsgPackTest()
{
    // 创建一个结构体实例
    wy::ComMsgPack original{42, "Hello, World!", 95.5};

    // 序列化结构体
    size_t dataSize;
    void*  serializedData = original.Serialize(dataSize);
    if (!serializedData)
    {
        std::cout << "Failed to serialize data." << std::endl;
        return;
    }

    std::cout << "Serialized data size: " << dataSize << " bytes" << std::endl;

    // 反序列化结构体
    wy::ComMsgPack deserialized;
    deserialized.Deserialize(serializedData, dataSize);
    std::cout << "Deserialized values:" << std::endl;
    std::cout << "id: " << deserialized.id << std::endl;
    std::cout << "name: " << deserialized.name << std::endl;
    std::cout << "score: " << deserialized.score << std::endl;

    // 释放分配的内存
    free(serializedData);
}

int main()
{
    HV_MEMCHECK;
    wy::HttpBusiness business;
    business.Init();
    // wy::HttpSvr::Instance().RegisterRoute(
    //     http_method::HTTP_POST, "/login",
    //     std::function<bool(std::shared_ptr<wy::LoginReqtMsg>, std::shared_ptr<wy::LoginRspMsg>)>(
    //         std::bind(&wy::HttpBusiness::Login, &business, std::placeholders::_1,
    //                   std::placeholders::_2)));

    wy::HttpSvr::Instance().RegisterRoute(
        http_method::HTTP_POST, "/echo_json",
        std::function<bool(std::shared_ptr<wy::EchoMsg>, std::shared_ptr<wy::EchoMsg>)>(std::bind(
            &wy::HttpBusiness::Echo, &business, std::placeholders::_1, std::placeholders::_2)));

    // wy::HttpSvr::Instance().RegisterRoute(
    //     http_method::HTTP_GET, "/get_json",
    //     std::function<bool(std::shared_ptr<wy::GetJsonMsg>, std::shared_ptr<wy::GetJsonMsg>)>(
    //         std::bind(&wy::HttpBusiness::GetJson, &business, std::placeholders::_1,
    //                   std::placeholders::_2)));

    // if (wy::HttpSvr::Instance().Start())
    // {
    //     printf("http server start success on %d\n", wy::HttpSvr::Instance().GetPort());
    // }
    // else
    // {
    //     printf("http server start failed\n");
    //     return -1;
    // }

    while (getchar() != '\n')
        ;
    hv::async::cleanup();
    return 0;
}