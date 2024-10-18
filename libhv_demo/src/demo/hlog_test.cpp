#include "hv/hlog.h"
#include "hv/hthread.h"
#include <thread>

int main(int argc, char** argv)
{
    // 设置日志文件路径
    hlog_set_file("rock_hlog.log");

    // 设置日志级别（DEBUG、INFO、WARN、ERROR、FATAL）
    hlog_set_level(LOG_LEVEL_DEBUG);
    hlog_set_format("[%y-%m-%d %H:%M:%S.%z][%l] %s");

    // 示例日志
    hlogd("This is a debug log.");
    hlogw("This is a warning log.");
    hloge("This is an error log.");

    hlogi("This is an info log. [%ld-%ld]", hv_getpid(), hv_gettid());

    std::thread t(
        []()
        {
            hlogi("This is an info log from another thread.[ %ld-%ld]", hv_getpid(), hv_gettid());
        });

    hlog_fsync();
    t.join();

    return 0;
}