#include "wy_http_business.h"
#include "wy_http_svr.h"

int main()
{
    HV_MEMCHECK;
    wy::HttpBusiness business;
    wy::HttpSvr::Instance().add_http_interface(
        http_method::HTTP_POST, "/login",
        std::function<bool(std::shared_ptr<wy::LoginReqtMsg>, std::shared_ptr<wy::LoginRspMsg>)>(
            std::bind(&wy::HttpBusiness::Login, &business, std::placeholders::_1,
                      std::placeholders::_2)));

    if (0 == wy::HttpSvr::Instance().start())
    {
        printf("http server start success on %d\n", wy::HttpSvr::Instance().port());
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