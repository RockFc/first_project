#include <gtest/gtest.h>
#include <sys/resource.h>
#include <unistd.h>
#define private public
#define protected public
#include "std_container.h"
#undef private
#undef protected

// 使用测试夹具（test fixture）
class ContainerTest : public testing::Test
{
public:
    void static SetUpTestCase() {}
    void static TearDownCase()
    {
        struct rusage r_usage;
        getrusage( RUSAGE_SELF, &r_usage );
        std::cout << "TearDownCase  Memory usage: " << r_usage.ru_maxrss << " KB" << std::endl;
    }

protected:
    virtual void SetUp() {}

    virtual void TearDown()
    {
        struct rusage r_usage;
        getrusage( RUSAGE_SELF, &r_usage );
        std::cout << "TearDown  Memory usage: " << r_usage.ru_maxrss << " KB" << std::endl;
    }
};

std::vector<std::string> g_v;
std::list<std::string> g_l;

const size_t LOOP_CNT= 100000;

TEST_F( ContainerTest, vector_add_1 )
{
    for ( size_t i = 0; i < LOOP_CNT; i++ )
    {
        g_v.push_back( std::to_string( i ));
    }
}

TEST_F( ContainerTest, vector_query_1 )
{
    for ( size_t i = 0; i < LOOP_CNT; i++ )
    {
        auto it = std::find( g_v.begin(), g_v.end(), std::to_string( i ));
        ASSERT_TRUE( it != g_v.end());
    }
}

TEST_F( ContainerTest, vector_erase_1 )
{
    for (size_t i = 0; i < LOOP_CNT; i++)
    {
        if(i % 10 == 0)
        {
            auto it = std::find( g_v.begin(), g_v.end(), std::to_string( i ));
            g_v.erase( it );
        }
    }
}

TEST_F( ContainerTest, vector_clear_1 )
{
    g_v.clear();
}

TEST_F( ContainerTest, list_add_1 )
{
    for ( size_t i = 0; i < LOOP_CNT; i++ )
    {
        g_l.push_back( std::to_string( i ));
    }
}

TEST_F( ContainerTest, list_query_1 )
{
    for ( size_t i = 0; i < LOOP_CNT; i++ )
    {
        auto it = std::find( g_l.begin(), g_l.end(), std::to_string( i ));
        ASSERT_TRUE( it != g_l.end());
    }
}

TEST_F( ContainerTest, list_erase_1 )
{
    for (size_t i = 0; i < LOOP_CNT; i++)
    {
        if(i % 10 == 0)
        {
            auto it = std::find( g_l.begin(), g_l.end(), std::to_string( i ));
            g_l.erase( it );
        }
    }
}

TEST_F( ContainerTest, list_clear_1 )
{
    g_l.clear();
}