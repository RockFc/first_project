# include <gtest/gtest.h>
# define private public
# define protected public
# include "counter.h"
# undef private
# undef protected

class CounterTest : public testing::Test
{
public:
    void static SetUpTestCase()
    {
        m_co = std::make_shared<Counter>();
    }
    void static TearDownCase()
    {
        m_co.reset();
    }

protected:
    virtual void SetUp(){}

    virtual void TearDown(){}

    static std::shared_ptr<Counter> m_co;
};
std::shared_ptr<Counter> CounterTest::m_co = nullptr;


TEST_F(CounterTest, AddOnce_1)
{
    EXPECT_EQ(m_co->AddOnce(), 1);
}

TEST_F(CounterTest, AddOnce_2)
{
    EXPECT_EQ(m_co->AddOnce(), 1);
    //这时候由于所有用例共用一个静态测试对象，所以进入此用例的时候，m_co的计数值已经变成了1
    //递加一次后，已经变成了2
    std::cout << "use static test object......." << std::endl;
    EXPECT_EQ(m_co->GetCurNum(), 2);
    EXPECT_EQ(m_co->AddOnce(), 3);
}

TEST_F(CounterTest, SubOnce_1)
{
    EXPECT_EQ(m_co->AddOnce(), 1);
    EXPECT_EQ(m_co->SubOnce(), 0);
    std::cout << "use static test object......." << std::endl;  
    EXPECT_EQ(m_co->AddOnce(), 4);
    EXPECT_EQ(m_co->SubOnce(), 3);
}