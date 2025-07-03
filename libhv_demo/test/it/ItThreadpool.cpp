#include "hv/hthread.h"
#include "hv/hthreadpool.h"
#include <gtest/gtest.h>
#include <iostream>

class HThreadPoolTest : public testing::Test
{
public:
    void static SetUpTestCase() {}
    void static TearDownCase() {}

protected:
    virtual void SetUp() {}
    virtual void TearDown()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

TEST_F(HThreadPoolTest, setMaxThreadNum_1)
{
    HThreadPool tp;
    tp.setMaxThreadNum(2);
    tp.start();
    for (size_t i = 0; i < 10; ++i)
    {
        tp.commit(
            [](size_t i) {
                printf("thread[%ld]: task[%ld] start sleep\n", hv_gettid(), i);
                hv_sleep(4);
            },
            i);
    }
    printf("current task size [%ld] in queue, current thread num [%d]\n", tp.taskNum(),
           tp.currentThreadNum());
    hv_sleep(4);
    tp.setMaxThreadNum(4);
    tp.tryScaleUp();
    printf("current task size [%ld] in queue, current thread num [%d]\n", tp.taskNum(),
           tp.currentThreadNum());
    tp.wait();
}