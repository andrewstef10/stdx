#include <gtest/gtest.h>
#include <stdads/math.h>

TEST(MathTest, fabs) {
    EXPECT_EQ(0.0f, stdads::fabs(0.0f));
    EXPECT_EQ(1.0f, stdads::fabs(1.0f));
    EXPECT_EQ(1.0f, stdads::fabs(-1.0f));
}

TEST(MathTest, pow) {
    EXPECT_EQ(1.0f, stdads::pow(2.0f, 0));
    EXPECT_EQ(2.0f, stdads::pow(2.0f, 1));
    EXPECT_EQ(4.0f, stdads::pow(2.0f, 2));
    EXPECT_EQ(8.0f, stdads::pow(2.0f, 3));

    EXPECT_EQ(0.5f, stdads::pow(2.0f, -1));
    EXPECT_EQ(0.25f, stdads::pow(2.0f, -2));
    EXPECT_EQ(0.125f, stdads::pow(2.0f, -3));
}
