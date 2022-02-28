# include <gtest/gtest.h>
# include "common.h"

TEST(test_common, GetInt_1)
{
    EXPECT_EQ(GetInt(), 100);
}

TEST(test_common, GetInt_2)
{
    EXPECT_EQ(GetInt(), 0);
    std::cout << "EXPECT_EQ don't interrupt test case!" << std::endl;
}

TEST(test_common, GetInt_3)
{
    ASSERT_EQ(GetInt(), 0);
    std::cout << "ASSERT_EQ don't interrupt test case!" << std::endl;
}

TEST(test_common, GetBool)
{
    EXPECT_EQ(GetBool(), true);
}

TEST(test_common, GetVersionInfo)
{
    EXPECT_STREQ(GetVersionInfo().c_str(), "1.0.0");
}