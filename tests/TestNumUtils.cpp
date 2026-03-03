#include <gtest/gtest.h>
#include <stdads/NumUtils.h>


// ---------- WITHIN (Inclusive) ----------

TEST(WithinTests, ValueInsideRange)
{
    EXPECT_TRUE(stdads::Within(5, 1, 10));
    EXPECT_TRUE(stdads::Within(-5, -10, -1));
}

TEST(WithinTests, ValueOnLowerBound)
{
    EXPECT_TRUE(stdads::Within(1, 1, 10));
    EXPECT_TRUE(stdads::Within(-10, -10, -1));
}

TEST(WithinTests, ValueOnUpperBound)
{
    EXPECT_TRUE(stdads::Within(10, 1, 10));
    EXPECT_TRUE(stdads::Within(-1, -10, -1));
}

TEST(WithinTests, ValueBelowRange)
{
    EXPECT_FALSE(stdads::Within(0, 1, 10));
    EXPECT_FALSE(stdads::Within(-11, -10, -1));
}

TEST(WithinTests, ValueAboveRange)
{
    EXPECT_FALSE(stdads::Within(11, 1, 10));
    EXPECT_FALSE(stdads::Within(0, -10, -1));
}


// ---------- WITHIN EXCLUSIVE ----------

TEST(WithinExclusiveTests, ValueInsideRange)
{
    EXPECT_TRUE(stdads::WithinExclusive(5, 1, 10));
    EXPECT_TRUE(stdads::WithinExclusive(-5, -10, -1));
}

TEST(WithinExclusiveTests, ValueOnLowerBound)
{
    EXPECT_FALSE(stdads::WithinExclusive(1, 1, 10));
    EXPECT_FALSE(stdads::WithinExclusive(-10, -10, -1));
}

TEST(WithinExclusiveTests, ValueOnUpperBound)
{
    EXPECT_FALSE(stdads::WithinExclusive(10, 1, 10));
    EXPECT_FALSE(stdads::WithinExclusive(-1, -10, -1));
}

TEST(WithinExclusiveTests, ValueBelowRange)
{
    EXPECT_FALSE(stdads::WithinExclusive(0, 1, 10));
    EXPECT_FALSE(stdads::WithinExclusive(-11, -10, -1));
}

TEST(WithinExclusiveTests, ValueAboveRange)
{
    EXPECT_FALSE(stdads::WithinExclusive(11, 1, 10));
    EXPECT_FALSE(stdads::WithinExclusive(0, -10, -1));
}


// ---------- OUTSIDE (Exclusive) ----------

TEST(OutsideTests, ValueInsideRange)
{
    EXPECT_FALSE(stdads::Outside(5, 1, 10));
    EXPECT_FALSE(stdads::Outside(-5, -10, -1));
}

TEST(OutsideTests, ValueOnLowerBound)
{
    EXPECT_FALSE(stdads::Outside(1, 1, 10));
    EXPECT_FALSE(stdads::Outside(-10, -10, -1));
}

TEST(OutsideTests, ValueOnUpperBound)
{
    EXPECT_FALSE(stdads::Outside(10, 1, 10));
    EXPECT_FALSE(stdads::Outside(-1, -10, -1));
}

TEST(OutsideTests, ValueBelowRange)
{
    EXPECT_TRUE(stdads::Outside(0, 1, 10));
    EXPECT_TRUE(stdads::Outside(-11, -10, -1));
}

TEST(OutsideTests, ValueAboveRange)
{
    EXPECT_TRUE(stdads::Outside(11, 1, 10));
    EXPECT_TRUE(stdads::Outside(0, -10, -1));
}


// ---------- OUTSIDE INCLUSIVE ----------

TEST(OutsideExclusiveTests, ValueInsideRange)
{
    EXPECT_FALSE(stdads::OutsideExclusive(5, 1, 10));
    EXPECT_FALSE(stdads::OutsideExclusive(-5, -10, -1));
}

TEST(OutsideExclusiveTests, ValueOnLowerBound)
{
    EXPECT_TRUE(stdads::OutsideExclusive(1, 1, 10));
    EXPECT_TRUE(stdads::OutsideExclusive(-11, -10, -1));
}

TEST(OutsideExclusiveTests, ValueOnUpperBound)
{
    EXPECT_TRUE(stdads::OutsideExclusive(10, 1, 10));
    EXPECT_TRUE(stdads::OutsideExclusive(-1, -10, -1));
}

TEST(OutsideExclusiveTests, ValueBelowRange)
{
    EXPECT_TRUE(stdads::OutsideExclusive(0, 1, 10));
    EXPECT_TRUE(stdads::OutsideExclusive(-11, -10, -1));
}

TEST(OutsideExclusiveTests, ValueAboveRange)
{
    EXPECT_TRUE(stdads::OutsideExclusive(11, 1, 10));
    EXPECT_TRUE(stdads::OutsideExclusive(0, -10, -1));
}


// ---------- CLAMP ----------

TEST(ClampTests, ValueInsideRange)
{
    int val = 5;
    int negVal = -5;
    EXPECT_FALSE(stdads::Clamp(val, 1, 10));
    EXPECT_FALSE(stdads::Clamp(negVal, -10, -1));
    EXPECT_EQ(5, val);
    EXPECT_EQ(-5, negVal);
}

TEST(ClampTests, ValueOnLowerBound)
{
    int val = 1;
    int negVal = -10;
    EXPECT_FALSE(stdads::Clamp(val, 1, 10));
    EXPECT_FALSE(stdads::Clamp(negVal, -10, -1));
    EXPECT_EQ(1, val);
    EXPECT_EQ(-10, negVal);
}

TEST(ClampTests, ValueOnUpperBound)
{
    int val = 10;
    int negVal = -1;
    EXPECT_FALSE(stdads::Clamp(val, 1, 10));
    EXPECT_FALSE(stdads::Clamp(negVal, -10, -1));
    EXPECT_EQ(10, val);
    EXPECT_EQ(-1, negVal);
}

TEST(ClampTests, ValueBelowRange)
{
    int val = 0;
    int negVal = -11;
    EXPECT_TRUE(stdads::Clamp(val, 1, 10));
    EXPECT_TRUE(stdads::Clamp(negVal, -10, -1));
    EXPECT_EQ(1, val);
    EXPECT_EQ(-10, negVal);
}

TEST(ClampTests, ValueAboveRange)
{
    int val = 11;
    int* valAddress = &val;
    int negVal = 0;
    EXPECT_TRUE(stdads::Clamp(val, 1, 10));
    EXPECT_TRUE(stdads::Clamp(negVal, -10, -1));
    EXPECT_EQ(10, val);
    EXPECT_EQ(-1, negVal);
}