#include <stdio.h>
#include <signal.h>

#include "workflow/WFFacilities.h"
#include "srpc/rpc_types.h"

#include "config/config.h"
#include "srpc_demo.srpc.h"

using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);
static srpc::RPCConfig config;

void sig_handler(int signo)
{
    wait_group.done();
}

void init()
{
    if (config.load("./server.conf") == false)
    {
        perror("Load config failed");
        exit(1);
    }

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
}

class ServiceImpl : public srpc_demo::Service
{
public:
    void Echo(EchoRequest *req, EchoResponse *resp, RPCContext *ctx) override
    {
        // 4. delete the following codes and fill your logic
        fprintf(stderr, "get req. %s\n", req->DebugString().c_str());
        resp->set_message("Hi back");
    }
};

int main()
{
    // 1. load config
    init();

    // 2. start server
    SRPCServer server;
    ServiceImpl impl;
    server.add_service(&impl);

    config.load_filter(server);

    if (server.start(config.server_port()) == 0)
    {
        // 3. success and wait
        fprintf(stderr, "srpc_demo SRPC server started, port %u\n", config.server_port());
        wait_group.wait();
        server.stop();
    }
    else
        perror("server start");

    return 0;
}

