# include <gtest/gtest.h>
# include "calc.h"
using namespace std;

TEST(Test_Calc, Add)
{
    EXPECT_TRUE(Calc::Add(3,5)==8);
}