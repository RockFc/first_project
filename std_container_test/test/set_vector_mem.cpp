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

const size_t LOOP_CNT = 1000000;

// std::vector< std::vector<std::string> > g_v;

// TEST_F( ContainerTest, vector_add_1 )
// {
//     for ( size_t i = 0; i < LOOP_CNT; i++ )
//     {
//         std::vector<std::string> v;
//         v.push_back( std::to_string( i ) );
//         if ( i % 10 == 0)
//         {
//             g_v.push_back(v);
//         }
//     }
// }

// std::set< std::string > g_s;

// TEST_F( ContainerTest, set_add_1 )
// {
//     for ( size_t i = 0; i < LOOP_CNT; i++ )
//     {
//         g_s.insert( std::to_string( i ) );
//     }
// }

std::vector< std::set< std::string > > g_vs;

TEST_F( ContainerTest, vector_set_add_1 )
{
    for ( size_t i = 0; i < LOOP_CNT; i++ )
    {
        std::set< std::string > s;
        s.insert( std::to_string( i ) );
        if ( i % 10 == 0 )
        {
            g_vs.push_back( s );
        }
    }
}
