#include <gtest/gtest.h>
#include <stdx/ctype.h>

// ---------------- isAlpha ----------------

TEST(IsAlphaTests, LowercaseLetters)
{
    EXPECT_TRUE(stdx::isAlpha('a'));
    EXPECT_TRUE(stdx::isAlpha('z'));
    EXPECT_TRUE(stdx::isAlpha('m'));
}

TEST(IsAlphaTests, UppercaseLetters)
{
    EXPECT_TRUE(stdx::isAlpha('A'));
    EXPECT_TRUE(stdx::isAlpha('Z'));
    EXPECT_TRUE(stdx::isAlpha('M'));
}

TEST(IsAlphaTests, DigitsReturnFalse)
{
    EXPECT_FALSE(stdx::isAlpha('0'));
    EXPECT_FALSE(stdx::isAlpha('9'));
}

TEST(IsAlphaTests, SymbolsReturnFalse)
{
    EXPECT_FALSE(stdx::isAlpha('!'));
    EXPECT_FALSE(stdx::isAlpha('@'));
    EXPECT_FALSE(stdx::isAlpha(' '));
}

TEST(IsAlphaTests, BoundaryCharacters)
{
    EXPECT_FALSE(stdx::isAlpha('`')); // before 'a'
    EXPECT_FALSE(stdx::isAlpha('{')); // after 'z'
    EXPECT_FALSE(stdx::isAlpha('@')); // before 'A'
    EXPECT_FALSE(stdx::isAlpha('[')); // after 'Z'
}

// ---------------- IsDigit ----------------

TEST(IsDigitTests, ValidDigits)
{
    EXPECT_TRUE(stdx::isDigit('0'));
    EXPECT_TRUE(stdx::isDigit('5'));
    EXPECT_TRUE(stdx::isDigit('9'));
}

TEST(IsDigitTests, LettersReturnFalse)
{
    EXPECT_FALSE(stdx::isDigit('a'));
    EXPECT_FALSE(stdx::isDigit('Z'));
}

TEST(IsDigitTests, SymbolsReturnFalse)
{
    EXPECT_FALSE(stdx::isDigit('-'));
    EXPECT_FALSE(stdx::isDigit(' '));
}

// ---------------- IsDigitOfBase ----------------

TEST(IsDigitOfBaseTests, Base10Valid)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 10));
    }
}

TEST(IsDigitOfBaseTests, Base10Invalid)
{
    EXPECT_FALSE(stdx::isDigitOfBase('0' - 1, 10));
    EXPECT_FALSE(stdx::isDigitOfBase('9' + 1, 10));
    EXPECT_FALSE(stdx::isDigitOfBase('a', 10));
    EXPECT_FALSE(stdx::isDigitOfBase('A', 10));
    EXPECT_FALSE(stdx::isDigitOfBase('-', 10));
    EXPECT_FALSE(stdx::isDigitOfBase(' ', 10));
    EXPECT_FALSE(stdx::isDigitOfBase('\0', 10));
}

TEST(IsDigitOfBaseTests, Base2Valid)
{
    EXPECT_TRUE(stdx::isDigitOfBase('0', 2));
    EXPECT_TRUE(stdx::isDigitOfBase('1', 2));
}

TEST(IsDigitOfBaseTests, Base2Invalid)
{
    EXPECT_FALSE(stdx::isDigitOfBase('0' - 1, 2));
    EXPECT_FALSE(stdx::isDigitOfBase('2', 2));
    EXPECT_FALSE(stdx::isDigitOfBase('a', 2));
    EXPECT_FALSE(stdx::isDigitOfBase('A', 2));
    EXPECT_FALSE(stdx::isDigitOfBase('-', 2));
    EXPECT_FALSE(stdx::isDigitOfBase(' ', 2));
    EXPECT_FALSE(stdx::isDigitOfBase('\0', 2));
}

TEST(IsDigitOfBaseTests, Base8Valid)
{
    for (char c = '0'; c <= '7'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 8));
    }
}

TEST(IsDigitOfBaseTests, Base8Invalid)
{
    EXPECT_FALSE(stdx::isDigitOfBase('0' - 1, 8));
    EXPECT_FALSE(stdx::isDigitOfBase('8', 8));
    EXPECT_FALSE(stdx::isDigitOfBase('a', 8));
    EXPECT_FALSE(stdx::isDigitOfBase('A', 8));
    EXPECT_FALSE(stdx::isDigitOfBase('-', 8));
    EXPECT_FALSE(stdx::isDigitOfBase(' ', 8));
    EXPECT_FALSE(stdx::isDigitOfBase('\0', 8));
}

TEST(IsDigitOfBaseTests, Base1Valid)
{
    EXPECT_TRUE(stdx::isDigitOfBase('1', 1));
}

TEST(IsDigitOfBaseTests, Base1Invalid)
{
    EXPECT_FALSE(stdx::isDigitOfBase('0', 1));
    EXPECT_FALSE(stdx::isDigitOfBase('2', 1));
    EXPECT_FALSE(stdx::isDigitOfBase('a', 1));
    EXPECT_FALSE(stdx::isDigitOfBase('A', 1));
    EXPECT_FALSE(stdx::isDigitOfBase('-', 1));
    EXPECT_FALSE(stdx::isDigitOfBase(' ', 1));
    EXPECT_FALSE(stdx::isDigitOfBase('\0', 1));
}

TEST(IsDigitOfBaseTests, Base16Number)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 16));
    }
}

TEST(IsDigitOfBaseTests, Base16Lowercase)
{
    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 16));
    }
}

TEST(IsDigitOfBaseTests, Base16Uppercase)
{
    for (char c = 'A'; c <= 'F'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 16));
    }
}

TEST(IsDigitOfBaseTests, Base16Invalid)
{
    EXPECT_FALSE(stdx::isDigitOfBase('0' - 1, 16));
    EXPECT_FALSE(stdx::isDigitOfBase('9' + 1, 16));
    EXPECT_FALSE(stdx::isDigitOfBase('a' - 1, 16));
    EXPECT_FALSE(stdx::isDigitOfBase('A' - 1, 16));
    EXPECT_FALSE(stdx::isDigitOfBase('g', 16));
    EXPECT_FALSE(stdx::isDigitOfBase('G', 16));
    EXPECT_FALSE(stdx::isDigitOfBase('-', 16));
    EXPECT_FALSE(stdx::isDigitOfBase(' ', 16));
    EXPECT_FALSE(stdx::isDigitOfBase('\0', 16));
}

TEST(IsDigitOfBaseTests, Base36Number)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 36));
    }
}

TEST(IsDigitOfBaseTests, Base36Lowercase)
{
    for (char c = 'a'; c <= 'z'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 36));
    }
}

TEST(IsDigitOfBaseTests, Base36Uppercase)
{
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        EXPECT_TRUE(stdx::isDigitOfBase(c, 36));
    }
}

TEST(IsDigitOfBaseTests, Base36Invalid)
{
    EXPECT_FALSE(stdx::isDigitOfBase('0' - 1, 36));
    EXPECT_FALSE(stdx::isDigitOfBase('9' + 1, 36));
    EXPECT_FALSE(stdx::isDigitOfBase('a' - 1, 36));
    EXPECT_FALSE(stdx::isDigitOfBase('A' - 1, 36));
    EXPECT_FALSE(stdx::isDigitOfBase('z' + 1, 36));
    EXPECT_FALSE(stdx::isDigitOfBase('Z' + 1, 36));
    EXPECT_FALSE(stdx::isDigitOfBase('-', 36));
    EXPECT_FALSE(stdx::isDigitOfBase(' ', 36));
    EXPECT_FALSE(stdx::isDigitOfBase('\0', 36));
}

TEST(IsDigitOfBaseTests, Base35CharOutOfRange)
{
    EXPECT_FALSE(stdx::isDigitOfBase('z', 35));
}

TEST(IsDigitOfBaseTests, InvalidBaseLow)
{
    EXPECT_FALSE(stdx::isDigitOfBase('1', 0));
    EXPECT_FALSE(stdx::isDigitOfBase('1', -1));
}

TEST(IsDigitOfBaseTests, InvalidBaseHigh)
{
    EXPECT_FALSE(stdx::isDigitOfBase('1', 37));
    EXPECT_FALSE(stdx::isDigitOfBase('z', 100));
}

// ---------------- CharToDigit ----------------

TEST(CharToDigitTests, Base10Valid)
{
    int i = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 10));
        ++i;
    }
}

TEST(CharToDigitTests, Base10Invalid)
{
    EXPECT_EQ(-1, stdx::charToDigit('0' - 1, 10));
    EXPECT_EQ(-1, stdx::charToDigit('9' + 1, 10));
    EXPECT_EQ(-1, stdx::charToDigit('a', 10));
    EXPECT_EQ(-1, stdx::charToDigit('A', 10));
    EXPECT_EQ(-1, stdx::charToDigit('-', 10));
    EXPECT_EQ(-1, stdx::charToDigit(' ', 10));
    EXPECT_EQ(-1, stdx::charToDigit('\0', 10));
}

TEST(CharToDigitTests, Base2Valid)
{
    EXPECT_EQ(0, stdx::charToDigit('0', 2));
    EXPECT_EQ(1, stdx::charToDigit('1', 2));
}

TEST(CharToDigitTests, Base2Invalid)
{
    EXPECT_EQ(-1, stdx::charToDigit('0' - 1, 2));
    EXPECT_EQ(-1, stdx::charToDigit('2', 2));
    EXPECT_EQ(-1, stdx::charToDigit('a', 2));
    EXPECT_EQ(-1, stdx::charToDigit('A', 2));
    EXPECT_EQ(-1, stdx::charToDigit('-', 2));
    EXPECT_EQ(-1, stdx::charToDigit(' ', 2));
    EXPECT_EQ(-1, stdx::charToDigit('\0', 2));
}

TEST(CharToDigitTests, Base8Valid)
{
    int i = 0;
    for (char c = '0'; c <= '7'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 8));
        ++i;
    }
}

TEST(CharToDigitTests, Base8Invalid)
{
    EXPECT_EQ(-1, stdx::charToDigit('0' - 1, 8));
    EXPECT_EQ(-1, stdx::charToDigit('8', 8));
    EXPECT_EQ(-1, stdx::charToDigit('a', 8));
    EXPECT_EQ(-1, stdx::charToDigit('A', 8));
    EXPECT_EQ(-1, stdx::charToDigit('-', 8));
    EXPECT_EQ(-1, stdx::charToDigit(' ', 8));
    EXPECT_EQ(-1, stdx::charToDigit('\0', 8));
}

TEST(CharToDigitTests, Base1Valid)
{
    EXPECT_EQ(1, stdx::charToDigit('1', 1));
}

TEST(CharToDigitTests, Base1Invalid)
{
    EXPECT_EQ(-1, stdx::charToDigit('0', 1));
    EXPECT_EQ(-1, stdx::charToDigit('2', 1));
    EXPECT_EQ(-1, stdx::charToDigit('a', 1));
    EXPECT_EQ(-1, stdx::charToDigit('A', 1));
    EXPECT_EQ(-1, stdx::charToDigit('-', 1));
    EXPECT_EQ(-1, stdx::charToDigit(' ', 1));
    EXPECT_EQ(-1, stdx::charToDigit('\0', 1));
}

TEST(CharToDigitTests, Base16Number)
{
    int i = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 16));
        ++i;
    }
}

TEST(CharToDigitTests, Base16Lowercase)
{
    int i = 10;
    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 16));
        ++i;
    }
}

TEST(CharToDigitTests, Base16Uppercase)
{
    int i = 10;
    for (char c = 'A'; c <= 'F'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 16));
        ++i;
    }
}

TEST(CharToDigitTests, Base16Invalid)
{
    EXPECT_EQ(-1, stdx::charToDigit('0' - 1, 16));
    EXPECT_EQ(-1, stdx::charToDigit('9' + 1, 16));
    EXPECT_EQ(-1, stdx::charToDigit('a' - 1, 16));
    EXPECT_EQ(-1, stdx::charToDigit('A' - 1, 16));
    EXPECT_EQ(-1, stdx::charToDigit('g', 16));
    EXPECT_EQ(-1, stdx::charToDigit('G', 16));
    EXPECT_EQ(-1, stdx::charToDigit('-', 16));
    EXPECT_EQ(-1, stdx::charToDigit(' ', 16));
    EXPECT_EQ(-1, stdx::charToDigit('\0', 16));
}

TEST(CharToDigitTests, Base36Number)
{
    int i = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 36));
        ++i;
    }
}

TEST(CharToDigitTests, Base36Lowercase)
{
    int i = 10;
    for (char c = 'a'; c <= 'z'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 36));
        ++i;
    }
}

TEST(CharToDigitTests, Base36Uppercase)
{
    int i = 10;
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        EXPECT_EQ(i, stdx::charToDigit(c, 36));
        ++i;
    }
}

TEST(CharToDigitTests, Base36Invalid)
{
    EXPECT_EQ(-1, stdx::charToDigit('0' - 1, 36));
    EXPECT_EQ(-1, stdx::charToDigit('9' + 1, 36));
    EXPECT_EQ(-1, stdx::charToDigit('a' - 1, 36));
    EXPECT_EQ(-1, stdx::charToDigit('A' - 1, 36));
    EXPECT_EQ(-1, stdx::charToDigit('z' + 1, 36));
    EXPECT_EQ(-1, stdx::charToDigit('Z' + 1, 36));
    EXPECT_EQ(-1, stdx::charToDigit('-', 36));
    EXPECT_EQ(-1, stdx::charToDigit(' ', 36));
    EXPECT_EQ(-1, stdx::charToDigit('\0', 36));
}

TEST(CharToDigitTests, Base35CharOutOfRange)
{
    EXPECT_EQ(-1, stdx::charToDigit('z', 35));
}

TEST(CharToDigitTests, InvalidBaseLow)
{
    EXPECT_EQ(-1, stdx::charToDigit('1', 0));
    EXPECT_EQ(-1, stdx::charToDigit('1', -1));
}

TEST(CharToDigitTests, InvalidBaseHigh)
{
    EXPECT_EQ(-1, stdx::charToDigit('1', 37));
    EXPECT_EQ(-1, stdx::charToDigit('z', 100));
}

// ---------------- ToLower ----------------

TEST(ToLowerTests, ConvertsUppercaseLetters)
{
    EXPECT_EQ(stdx::toLower('A'), 'a');
    EXPECT_EQ(stdx::toLower('Z'), 'z');
    EXPECT_EQ(stdx::toLower('M'), 'm');
}

TEST(ToLowerTests, BoundaryCharacters)
{
    EXPECT_EQ(stdx::toLower('A'), 'a');
    EXPECT_EQ(stdx::toLower('Z'), 'z');
}

TEST(ToLowerTests, LowercaseUnchanged)
{
    EXPECT_EQ(stdx::toLower('a'), 'a');
    EXPECT_EQ(stdx::toLower('z'), 'z');
    EXPECT_EQ(stdx::toLower('m'), 'm');
}

TEST(ToLowerTests, DigitsUnchanged)
{
    EXPECT_EQ(stdx::toLower('0'), '0');
    EXPECT_EQ(stdx::toLower('9'), '9');
}

TEST(ToLowerTests, SymbolsUnchanged)
{
    EXPECT_EQ(stdx::toLower('!'), '!');
    EXPECT_EQ(stdx::toLower('@'), '@');
    EXPECT_EQ(stdx::toLower('['), '[');
}

TEST(ToLowerTests, WhitespaceUnchanged)
{
    EXPECT_EQ(stdx::toLower(' '), ' ');
    EXPECT_EQ(stdx::toLower('\n'), '\n');
    EXPECT_EQ(stdx::toLower('\t'), '\t');
}

TEST(ToLowerTests, AdjacentAsciiCharacters)
{
    EXPECT_EQ(stdx::toLower('@'), '@'); // before 'A'
    EXPECT_EQ(stdx::toLower('['), '['); // after 'Z'
}

// ---------------- ToUpper ----------------

TEST(ToUpperTests, ConvertsLowercaseLetters)
{
    EXPECT_EQ(stdx::toUpper('a'), 'A');
    EXPECT_EQ(stdx::toUpper('z'), 'Z');
    EXPECT_EQ(stdx::toUpper('m'), 'M');
}

TEST(ToUpperTests, BoundaryCharacters)
{
    EXPECT_EQ(stdx::toUpper('a'), 'A');
    EXPECT_EQ(stdx::toUpper('z'), 'Z');
}

TEST(ToUpperTests, UppercaseUnchanged)
{
    EXPECT_EQ(stdx::toUpper('A'), 'A');
    EXPECT_EQ(stdx::toUpper('Z'), 'Z');
    EXPECT_EQ(stdx::toUpper('M'), 'M');
}

TEST(ToUpperTests, DigitsUnchanged)
{
    EXPECT_EQ(stdx::toUpper('0'), '0');
    EXPECT_EQ(stdx::toUpper('9'), '9');
}

TEST(ToUpperTests, SymbolsUnchanged)
{
    EXPECT_EQ(stdx::toUpper('!'), '!');
    EXPECT_EQ(stdx::toUpper('@'), '@');
    EXPECT_EQ(stdx::toUpper('{'), '{');
}

TEST(ToUpperTests, WhitespaceUnchanged)
{
    EXPECT_EQ(stdx::toUpper(' '), ' ');
    EXPECT_EQ(stdx::toUpper('\n'), '\n');
    EXPECT_EQ(stdx::toUpper('\t'), '\t');
}

TEST(ToUpperTests, AdjacentAsciiCharacters)
{
    EXPECT_EQ(stdx::toUpper('`'), '`'); // before 'a'
    EXPECT_EQ(stdx::toUpper('{'), '{'); // after 'z'
}
