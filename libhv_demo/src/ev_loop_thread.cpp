// 主要用于定时器或延迟执行任务
// 与ev_loop的区别是，ev_loop在当前线程中执行的的，而ev_loop_thread是在单独的子线程中执行
// UpdClient 等都是使用ev_loop_thread进行异步操作

#include "hv/hv.h"

#include "hv/EventLoopThread.h"

using namespace hv;

static void onTimer(TimerID timerID, int n) {
    printf("tid=%ld timerID=%lu time=%lus n=%d\n", hv_gettid(), (unsigned long)timerID, (unsigned long)time(NULL), n);
}

int main(int argc, char* argv[]) {
    HV_MEMCHECK;

    printf("main tid=%ld\n", hv_gettid());

    EventLoopThread loop_thread;
    const EventLoopPtr& loop = loop_thread.loop();

    // runEvery 1s
    loop->setInterval(1000, std::bind(onTimer, std::placeholders::_1, 100));

    // runAfter 10s
    loop->setTimeout(10000, [&loop](TimerID timerID){
        loop->stop();
    });

    loop_thread.start();

    loop->queueInLoop([](){
        printf("queueInLoop tid=%ld\n", hv_gettid());
    });

    loop->runInLoop([](){
        printf("runInLoop tid=%ld\n", hv_gettid());
    });

    // wait loop_thread exit
    loop_thread.join();

    return 0;
}
