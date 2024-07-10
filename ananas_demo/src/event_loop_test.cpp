
#include "ananas/net/EventLoop.h"
#include "ananas/util/Logger.h"
#include "ananas/net/Application.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "common.h"

using namespace ananas;

int main(int ac, char* av[]) {
    LogManager::Instance().Start();
    auto log = LogManager::Instance().CreateLog(logALL, logConsole);

    auto& app = Application::Instance();
    auto& loop = *app.BaseLoop();

    loop.Execute([log]() {
        INF(log) << "Hello, test timer...";
    });

    loop.ScheduleAfterWithRepeat<3>(std::chrono::seconds(1), [log]() {
        INF(log) << "timer tick...";
    });

    loop.Execute([log](){
        INF(log) << "I am in thread pool...";
    });

    // shutdown after 7s
    loop.ScheduleAfter(std::chrono::seconds(7), [&app, log]() {
        WRN(log) << "after 2s,  stop app.";
        sleep(2);
        app.Exit();
    });

    app.Run(ac, av);

    return 0;
}