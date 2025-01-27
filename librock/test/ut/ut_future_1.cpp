#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#define private public
#define protected public
#include "future/Future.h"
#undef private
#undef protected

class FutureTest : public testing::Test
{
public:
    void static SetUpTestCase() {}
    void static TearDownCase() {}

protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(FutureTest, SetValue_1)
{
    rock::Promise<int> pm;
    rock::Future<int>  fut = pm.GetFuture();
    pm.SetValue(42);
    int result = fut.Wait();
    EXPECT_EQ(result, 42);
    std::cout << "result: " << result << std::endl;
}

TEST_F(FutureTest, SetValue_2)
{
    try
    {
        rock::Promise<int> pm;
        rock::Future<int>  fut = pm.GetFuture();
        std::thread        t(
            [&pm]
            {
                try
                {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    pm.SetValue(27);
                }
                catch (const std::exception& e)
                {
                    pm.SetException(std::make_exception_ptr(e));  // 传播异常
                    std::cerr << "Exception in thread: " << e.what() << '\n';
                }
            });
        t.detach();

        int result = fut.Wait(std::chrono::milliseconds(2 * 1000));
        if (fut.valid())
        {
            EXPECT_EQ(result, 27);
            std::cout << "result: " << result << std::endl;
        }
        else
        {
            std::cout << "future is invalid" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception in main: " << e.what() << '\n';
    }
}

TEST_F(FutureTest, Then_1)
{
    rock::Future<int> fut = rock::MakeReadyFuture(42);
    fut.Then(
           [](int value)
           {
               std::cout << "Got value: " << value << std::endl;
               return value + 1;
           })
        .Then(
            [](int value)
            {
                std::cout << "New value: " << value << std::endl;
            });
}
