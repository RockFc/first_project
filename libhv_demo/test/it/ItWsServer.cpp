#include <gtest/gtest.h>
#include <iostream>
#define private public
#define protected public
#include "WsServer.h"
#undef private
#undef protected

int main()
{
    HV_MEMCHECK;
    std::cout << "Hello, Websocket server!" << std::endl;

    wy::WsSvrImp ws_server;
    ws_server.RegisterGetDataFunc(
        [&ws_server](std::shared_ptr<wy::ComMsgPack> pack)
        {
            pack->Print();
            // 模拟业务处理
            std::this_thread::sleep_for(std::chrono::seconds(1));

            auto rspPack   = std::make_shared<wy::ComMsgPack>();
            rspPack->id    = pack->id;
            rspPack->name  = "hello, I am it testcase!";
            rspPack->score = pack->score * 2;
            std::cout << "[1]m_wsMsgToChannel.size:" << ws_server.m_wsMsgToChannel.size()
                      << std::endl;
            ws_server.SendData(rspPack);
            std::cout << "[2]m_wsMsgToChannel.size:" << ws_server.m_wsMsgToChannel.size()
                      << std::endl;
            return true;
        });

    ws_server.Start();
    while (getchar() != '\n')
        ;
    hv::async::cleanup();
    return 0;
}