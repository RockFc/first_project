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
