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

// std::vector< std::vector< std::string > > g_vv;

// TEST_F( ContainerTest, vector_vector_add_1 )
// {
//     std::vector< std::string >* p_v = new std::vector< std::string >;
//     for ( size_t i = 0; i < LOOP_CNT; i++ )
//     {
//         p_v->push_back( std::to_string( i ) );
//         if ( i % 10 == 0 )
//         {
//             g_vv.push_back( *p_v );
//             p_v = new std::vector< std::string >;
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
    std::set< std::string >* p_s = new std::set< std::string >;
    for ( size_t i = 0; i < LOOP_CNT; i++ )
    {
        p_s->insert( std::to_string( i ) );
        if ( i % 10 == 0 )
        {
            g_vs.push_back( *p_s );
            p_s = new std::set< std::string >;
        }
    }
}
