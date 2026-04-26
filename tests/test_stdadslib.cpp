#include <gtest/gtest.h>
#include <stdads/stdadslib.h>

TEST(StrtolTest, ParsesSimpleDecimal)
{
    EXPECT_EQ(123, stdads::strtol("123"));
}

TEST(StrtolTest, ParsesNegativeDecimal)
{
    EXPECT_EQ(-456, stdads::strtol("-456"));
}

TEST(StrtolTest, ParsesPositiveSign)
{
    EXPECT_EQ(789, stdads::strtol("+789"));
}

TEST(StrtolTest, ParsesDecimalWithLeadingZeros)
{
    EXPECT_EQ(123, stdads::strtol("00123"));
}


//
// WHITESPACE
//

TEST(StrtolTest, SkipsLeadingWhitespace)
{
    EXPECT_EQ(42, stdads::strtol("    42"));
}

TEST(StrtolTest, SkipsWhitespaceWithSign)
{
    EXPECT_EQ(-99, stdads::strtol("   -99"));
}

//
// BASE SPECIFIED
//

TEST(StrtolTest, ParsesBinaryBase2)
{
    EXPECT_EQ(10, stdads::strtol("1010", 2));
    EXPECT_EQ(10, stdads::strtol("0001010", 2));
}

TEST(StrtolTest, ParsesHexBase16)
{
    EXPECT_EQ(255, stdads::strtol("FF", 16));
}

TEST(StrtolTest, ParsesLowercaseHex)
{
    EXPECT_EQ(255, stdads::strtol("ff", 16));
}

TEST(StrtolTest, ParsesBase36)
{
    EXPECT_EQ(35, stdads::strtol("z", 36));
    EXPECT_EQ(35, stdads::strtol("Z", 36));
    EXPECT_EQ(1223, stdads::strtol("xZ", 36));
    EXPECT_EQ(1223, stdads::strtol("0xZ", 36));
}

//
// BASE ZERO AUTO DETECTION
//

TEST(StrtolTest, BaseZeroDecimal)
{
    EXPECT_EQ(123, stdads::strtol("123", 0));
}

TEST(StrtolTest, BaseZeroOctal)
{
    EXPECT_EQ(8, stdads::strtol("010", 0));   // octal
}

TEST(StrtolTest, BaseZeroHex)
{
    EXPECT_EQ(26, stdads::strtol("0x1A", 0));
    EXPECT_EQ(26, stdads::strtol("0X1A", 0));
}

//
// HEX PREFIX WHEN BASE 16
//

TEST(StrtolTest, HexPrefixWithBase16)
{
    EXPECT_EQ(26, stdads::strtol("0x1A", 16));
}

TEST(StrtolTest, HexPrefixUppercase)
{
    EXPECT_EQ(26, stdads::strtol("0X1A", 16));
}

TEST(StrtolTest, HexPrefixInvalidForCoverage)
{
    EXPECT_EQ(10, stdads::strtol("0a", 16));
    EXPECT_EQ(0, stdads::strtol("X1A", 16));
    EXPECT_EQ(0, stdads::strtol("x1A", 16));
}

//
// PARTIAL PARSING
//

TEST(StrtolTest, StopsAtInvalidCharacter)
{
    char* end = nullptr;
    long result = stdads::strtol("123abc", 10, &end);

    EXPECT_EQ(123, result);
    EXPECT_STREQ("abc", end);
}

TEST(StrtolTest, StopsImmediatelyIfNoDigits)
{
    char* end = nullptr;
    const char* str = "abc";
    long result = stdads::strtol(str, 10, &end);

    EXPECT_EQ(0, result);
    EXPECT_EQ(end, str);
}

TEST(StrtolTest, ParsesThenStopsOnSpace)
{
    char* end = nullptr;
    long result = stdads::strtol("42 99", 10, &end);

    EXPECT_EQ(42, result);
    EXPECT_STREQ(" 99", end);
}

//
// INVALID BASE
//

TEST(StrtolTest, InvalidBaseReturnsZero)
{
    EXPECT_EQ(0, stdads::strtol("123", -1));
    EXPECT_EQ(0, stdads::strtol("123", 37));
}

//
// NULL ENDPTR
//

TEST(StrtolTest, WorksWithNullEndPtr)
{
    EXPECT_EQ(123, stdads::strtol("123", 10, nullptr));
}

//
// SIGN WITH BASE 2
//

TEST(StrtolTest, NegativeBinary)
{
    EXPECT_EQ(-5, stdads::strtol("-101", 2));
}

//
// LEADING ZERO WITHOUT BASE ZERO
//

TEST(StrtolTest, LeadingZeroBase10)
{
    EXPECT_EQ(10, stdads::strtol("010", 10));  // decimal, not octal
}

//
// EMPTY STRING
//

TEST(StrtolTest, EmptyString)
{
    char* end = nullptr;
    const char* emptyStr = "";
    EXPECT_EQ(0, stdads::strtol(emptyStr, 10, &end));
    EXPECT_EQ(end, emptyStr);
}

//
// ONLY SIGN
//

TEST(StrtolTest, OnlySign)
{
    char* end = nullptr;
    const char* onlySign = "-";
    EXPECT_EQ(0, stdads::strtol(onlySign, 10, &end));
    EXPECT_EQ(end, onlySign);

    onlySign = "+";
    EXPECT_EQ(0, stdads::strtol(onlySign, 10, &end));
    EXPECT_EQ(end, onlySign);
}