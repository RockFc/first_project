# include <gtest/gtest.h>
# include <time.h>
#include <unistd.h>
# define private public
# define protected public
# include "counter.h"
# undef private
# undef protected

//使用测试夹具（test fixture）
class CounterTest : public testing::Test
{
protected:
    //1.如果测试对象不需要初始化和反初始化，则可以不重载这两个函数    
    //2.如果测试对象需要构造后需要初始化与反初始化，则需要重载SetUp/TearDown函数，这两个函数在测试用例开始前和结束后被自动调用
    //3.初始化函数中，也可以自定义辅助测试的内容，例如断言每个用例花费时间不超过5s
    virtual void SetUp()
    {
        //调用测试对象初始化函数
        m_co.Init();
        //记录用例开始执行时间
        m_start_time = time(nullptr);
    }

    virtual void TearDown()
    {
        //调用测试对象的反初始化函数
        m_co.Uinit();
        //判断用例执行是否超时
        const time_t end_time = time(nullptr);
        EXPECT_TRUE(end_time - m_start_time <= 5) << "The test took too long.";
    }

    Counter m_co;
    time_t m_start_time;
};

//每个测试用例都拥有独立的夹具类CounterTest对象
TEST(CounterTest, AddOnce_1)
{
    EXPECT_EQ(m_co.AddOnce(), 1);
}

TEST(CounterTest, AddOnce_1)
{
    EXPECT_EQ(m_co.AddOnce(), 1);
}

TEST(CounterTest, SubOnce_1)
{
    EXPECT_EQ(m_co.AddOnce(), 1);
    EXPECT_EQ(m_co.SubOnce(), 0);
}

TEST(CounterTest, SubOnce_2)
{
    EXPECT_EQ(m_co.AddOnce(), 1);
    sleep(6);
    EXPECT_EQ(m_co.SubOnce(), 0);
}