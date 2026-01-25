#include <gtest/gtest.h>
#include <stdads/math.h>

TEST(AbsTest, Int) {
    EXPECT_EQ(10, stdads::abs(-10));
    EXPECT_EQ(10, stdads::abs(10));
    EXPECT_EQ(0,  stdads::abs(0));
}

TEST(AbsTest, Long) {
    long neg = -123456L;
    long pos = 123456L;

    EXPECT_EQ(pos, stdads::abs(neg));
    EXPECT_EQ(pos, stdads::abs(pos));
    EXPECT_EQ(0L,  stdads::abs(0L));
}

TEST(AbsTest, LongLong) {
    long long neg = -9876543210LL;
    long long pos = 9876543210LL;

    EXPECT_EQ(pos, stdads::abs(neg));
    EXPECT_EQ(pos, stdads::abs(pos));
    EXPECT_EQ(0LL, stdads::abs(0LL));
}

TEST(AbsTest, Float) {
    float neg = -3.5f;
    float pos = 3.5f;

    EXPECT_FLOAT_EQ(pos, stdads::abs(neg));
    EXPECT_FLOAT_EQ(pos, stdads::abs(pos));
    EXPECT_FLOAT_EQ(0.0f, stdads::abs(0.0f));
}

TEST(AbsTest, Double) {
    double neg = -3.1415926535;
    double pos = 3.1415926535;

    EXPECT_DOUBLE_EQ(pos, stdads::abs(neg));
    EXPECT_DOUBLE_EQ(pos, stdads::abs(pos));
    EXPECT_DOUBLE_EQ(0.0, stdads::abs(0.0));
}

TEST(AbsTest, LongDouble) {
    long double neg = -1.23456789012345L;
    long double pos = 1.23456789012345L;

    EXPECT_EQ(pos, stdads::abs(neg));
    EXPECT_EQ(pos, stdads::abs(pos));
    EXPECT_EQ(0.0L, stdads::abs(0.0L));
}

TEST(PowTest, FloatRaisedToInt) {
    float base = 2.0f;
    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, 0));
    EXPECT_FLOAT_EQ(2.0f, stdads::pow(base, 1));
    EXPECT_FLOAT_EQ(4.0f, stdads::pow(base, 2));
    EXPECT_FLOAT_EQ(8.0f, stdads::pow(base, 3));

    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, -0));
    EXPECT_FLOAT_EQ(0.5f, stdads::pow(base, -1));
    EXPECT_FLOAT_EQ(0.25f, stdads::pow(base, -2));
    EXPECT_FLOAT_EQ(0.125f, stdads::pow(base, -3));

    base = 1.5f;
    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, 0));
    EXPECT_FLOAT_EQ(1.5f, stdads::pow(base, 1));
    EXPECT_FLOAT_EQ(2.25f, stdads::pow(base, 2));
    EXPECT_FLOAT_EQ(3.375f, stdads::pow(base, 3));

    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, -0));
    EXPECT_FLOAT_EQ(1.0f / 1.5f, stdads::pow(base, -1));
    EXPECT_FLOAT_EQ(1.0f / 2.25f, stdads::pow(base, -2));
    EXPECT_FLOAT_EQ(1.0f / 3.375f, stdads::pow(base, -3));

    base = -1.5f;
    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, 0));
    EXPECT_FLOAT_EQ(-1.5f, stdads::pow(base, 1));
    EXPECT_FLOAT_EQ(2.25f, stdads::pow(base, 2));
    EXPECT_FLOAT_EQ(-3.375f, stdads::pow(base, 3));

    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, -0));
    EXPECT_FLOAT_EQ(-1.0f / 1.5f, stdads::pow(base, -1));
    EXPECT_FLOAT_EQ(1.0f / 2.25f, stdads::pow(base, -2));
    EXPECT_FLOAT_EQ(-1.0f / 3.375f, stdads::pow(base, -3));

    base = 0.0f;
    EXPECT_FLOAT_EQ(0.0f, stdads::pow(base, 5));
    EXPECT_FLOAT_EQ(1.0f, stdads::pow(base, 0));
}

TEST(PowTest, DoubleRaisedToInt) {
    double base = 2.0;
    EXPECT_DOUBLE_EQ(1.0, stdads::pow(base, 0));
    EXPECT_DOUBLE_EQ(2.0, stdads::pow(base, 1));
    EXPECT_DOUBLE_EQ(4.0, stdads::pow(base, 2));
    EXPECT_DOUBLE_EQ(8.0, stdads::pow(base, 3));

    EXPECT_DOUBLE_EQ(1.0f, stdads::pow(base, -0));
    EXPECT_DOUBLE_EQ(0.5, stdads::pow(base, -1));
    EXPECT_DOUBLE_EQ(0.25, stdads::pow(base, -2));
    EXPECT_DOUBLE_EQ(0.125, stdads::pow(base, -3));

    base = 1.5;
    EXPECT_DOUBLE_EQ(1.0, stdads::pow(base, 0));
    EXPECT_DOUBLE_EQ(1.5, stdads::pow(base, 1));
    EXPECT_DOUBLE_EQ(2.25, stdads::pow(base, 2));
    EXPECT_DOUBLE_EQ(3.375, stdads::pow(base, 3));

    EXPECT_DOUBLE_EQ(1.0, stdads::pow(base, -0));
    EXPECT_DOUBLE_EQ(1.0 / 1.5, stdads::pow(base, -1));
    EXPECT_DOUBLE_EQ(1.0 / 2.25, stdads::pow(base, -2));
    EXPECT_DOUBLE_EQ(1.0 / 3.375, stdads::pow(base, -3));

    base = -1.5;
    EXPECT_DOUBLE_EQ(1.0, stdads::pow(base, 0));
    EXPECT_DOUBLE_EQ(-1.5, stdads::pow(base, 1));
    EXPECT_DOUBLE_EQ(2.25, stdads::pow(base, 2));
    EXPECT_DOUBLE_EQ(-3.375, stdads::pow(base, 3));

    EXPECT_DOUBLE_EQ(1.0, stdads::pow(base, -0));
    EXPECT_DOUBLE_EQ(-1.0 / 1.5, stdads::pow(base, -1));
    EXPECT_DOUBLE_EQ(1.0 / 2.25, stdads::pow(base, -2));
    EXPECT_DOUBLE_EQ(-1.0 / 3.375, stdads::pow(base, -3));

    base = 0.0;
    EXPECT_DOUBLE_EQ(0.0, stdads::pow(base, 5));
    EXPECT_DOUBLE_EQ(1.0, stdads::pow(base, 0));
}
