// 主要用于定时器或延迟执行任务

#include "hv/hv.h"

#include "hv/EventLoop.h"

using namespace hv;

static void onTimer(TimerID timerID, int n)
{
    printf("tid=%ld timerID=%lu time=%lus n=%d\n", hv_gettid(), ( unsigned long )timerID, ( unsigned long )time(NULL), n);
}

int main(int argc, char* argv[])
{
    HV_MEMCHECK;

    printf("main tid=%ld\t time=%lus\n", hv_gettid(), ( unsigned long )time(NULL));

    auto loop = std::make_shared< EventLoop >();

    // runEvery 1s
    loop->setInterval(1000, std::bind(onTimer, std::placeholders::_1, 1));

    loop->setInterval(2000, std::bind(onTimer, std::placeholders::_1, 2));

    loop->setInterval(4000, std::bind(onTimer, std::placeholders::_1, 4));

    loop->setInterval(4000, [loop]( TimerID timerID ){
        loop->queueInLoop(
        [timerID]()
        {
            printf("queueInLoop in timer tid=%ld timerID=%lu  time=%lus\n", hv_gettid(), ( unsigned long )timerID, ( unsigned long )time(NULL));
        });
        loop->runInLoop(
        [timerID]()
        {
            printf("runInLoop in timer tid=%ld timerID=%lu time=%lus\n", hv_gettid(), ( unsigned long )timerID, ( unsigned long )time(NULL));
        });
        
    });

    // runAfter 10s
    loop->setTimeout(10000,
                     [ &loop ](TimerID timerID)
                     {
                         loop->stop();
                     });

    loop->queueInLoop(
        []()
        {
            printf("queueInLoop tid=%ld\t time=%lus\n", hv_gettid(), ( unsigned long )time(NULL));
        });

    loop->runInLoop(
        []()
        {
            printf("runInLoop tid=%ld\t time=%lus\n", hv_gettid(), ( unsigned long )time(NULL));
        });

    // run until loop stopped
    loop->run();

    return 0;
}
