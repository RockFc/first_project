#include "wy_http_svr.h"

int main(int argc, char** argv)
{
    HV_MEMCHECK;

    wy::HttpSvr server;
    if (0 == server.start())
    {
        printf("http server start success on %d\n", server.port());
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