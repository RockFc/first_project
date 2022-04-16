#include "common.h"
#include <gtest/gtest.h>

TEST( test_common, GetInt_1 )
{
    EXPECT_EQ( GetInt(), 100 );
}

TEST( test_common, GetInt_2 )
{
    //如果检测失败，则会打印后面的日志信息
    EXPECT_EQ( GetInt(), 100 ) << "EXPECT_EQ don't interrupt test case!";
    std::cout << "EXPECT_EQ don't interrupt test case!" << std::endl;
}

TEST( test_common, GetInt_3 )
{
    ASSERT_EQ( GetInt(), 100 ) << "ASSERT_EQ interrupt test case!";
    std::cout << "ASSERT_EQ interrupt test case!" << std::endl;
}

TEST( test_common, GetBool_1 )
{
    EXPECT_EQ( GetBool(), true );
}

TEST( test_common, GetString_1 )
{
    EXPECT_STREQ( GetString().c_str(), "1.0.0" );
}

TEST( test_common, GetString_2 )
{
    EXPECT_STRNE( GetString().c_str(), "1.0.1" );
}