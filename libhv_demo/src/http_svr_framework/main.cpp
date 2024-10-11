#include "wy_http_business.h"
#include "wy_http_svr.h"

int main()
{
    HV_MEMCHECK;
    wy::HttpBusiness business;
    wy::HttpSvr::Instance().AddHttpInterface(
        http_method::HTTP_POST, "/login",
        std::function<bool(std::shared_ptr<wy::LoginReqtMsg>, std::shared_ptr<wy::LoginRspMsg>)>(
            std::bind(&wy::HttpBusiness::Login, &business, std::placeholders::_1,
                      std::placeholders::_2)));

    wy::HttpSvr::Instance().AddHttpInterface(
        http_method::HTTP_POST, "/echo_json",
        std::function<bool(std::shared_ptr<wy::EchoMsg>, std::shared_ptr<wy::EchoMsg>)>(std::bind(
            &wy::HttpBusiness::Echo, &business, std::placeholders::_1, std::placeholders::_2)));

    wy::HttpSvr::Instance().AddHttpInterface(
        http_method::HTTP_GET, "/get_json",
        std::function<bool(std::shared_ptr<wy::GetJsonMsg>, std::shared_ptr<wy::GetJsonMsg>)>(
            std::bind(&wy::HttpBusiness::GetJson, &business, std::placeholders::_1,
                      std::placeholders::_2)));

    if (0 == wy::HttpSvr::Instance().Start())
    {
        printf("http server start success on %d\n", wy::HttpSvr::Instance().Port());
    }
    else
    {
        printf("http server start failed\n");
        return -1;
    }

    while (getchar() != '\n')
        ;
    hv::async::cleanup();
    return 0;
}