#include <gtest/gtest.h>
#include <stdads/Ctype.h>

// ---------------- isAlpha ----------------

TEST(IsAlphaTests, LowercaseLetters)
{
    EXPECT_TRUE(stdads::IsAlpha('a'));
    EXPECT_TRUE(stdads::IsAlpha('z'));
    EXPECT_TRUE(stdads::IsAlpha('m'));
}

TEST(IsAlphaTests, UppercaseLetters)
{
    EXPECT_TRUE(stdads::IsAlpha('A'));
    EXPECT_TRUE(stdads::IsAlpha('Z'));
    EXPECT_TRUE(stdads::IsAlpha('M'));
}

TEST(IsAlphaTests, DigitsReturnFalse)
{
    EXPECT_FALSE(stdads::IsAlpha('0'));
    EXPECT_FALSE(stdads::IsAlpha('9'));
}

TEST(IsAlphaTests, SymbolsReturnFalse)
{
    EXPECT_FALSE(stdads::IsAlpha('!'));
    EXPECT_FALSE(stdads::IsAlpha('@'));
    EXPECT_FALSE(stdads::IsAlpha(' '));
}

TEST(IsAlphaTests, BoundaryCharacters)
{
    EXPECT_FALSE(stdads::IsAlpha('`'));   // before 'a'
    EXPECT_FALSE(stdads::IsAlpha('{'));   // after 'z'
    EXPECT_FALSE(stdads::IsAlpha('@'));   // before 'A'
    EXPECT_FALSE(stdads::IsAlpha('['));   // after 'Z'
}


// ---------------- IsDigit ----------------

TEST(IsDigitTests, ValidDigits)
{
    EXPECT_TRUE(stdads::IsDigit('0'));
    EXPECT_TRUE(stdads::IsDigit('5'));
    EXPECT_TRUE(stdads::IsDigit('9'));
}

TEST(IsDigitTests, LettersReturnFalse)
{
    EXPECT_FALSE(stdads::IsDigit('a'));
    EXPECT_FALSE(stdads::IsDigit('Z'));
}

TEST(IsDigitTests, SymbolsReturnFalse)
{
    EXPECT_FALSE(stdads::IsDigit('-'));
    EXPECT_FALSE(stdads::IsDigit(' '));
}


// ---------------- IsDigitOfBase ----------------

TEST(IsDigitOfBaseTests, Base10Valid)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 10));
    }
}

TEST(IsDigitOfBaseTests, Base10Invalid)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('0' - 1, 10));
    EXPECT_FALSE(stdads::IsDigitOfBase('9' + 1, 10));
    EXPECT_FALSE(stdads::IsDigitOfBase('a', 10));
    EXPECT_FALSE(stdads::IsDigitOfBase('A', 10));
    EXPECT_FALSE(stdads::IsDigitOfBase('-', 10));
    EXPECT_FALSE(stdads::IsDigitOfBase(' ', 10));
    EXPECT_FALSE(stdads::IsDigitOfBase('\0', 10));
}

TEST(IsDigitOfBaseTests, Base2Valid)
{
    EXPECT_TRUE(stdads::IsDigitOfBase('0', 2));
    EXPECT_TRUE(stdads::IsDigitOfBase('1', 2));
}


TEST(IsDigitOfBaseTests, Base2Invalid)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('0' - 1, 2));
    EXPECT_FALSE(stdads::IsDigitOfBase('2', 2));
    EXPECT_FALSE(stdads::IsDigitOfBase('a', 2));
    EXPECT_FALSE(stdads::IsDigitOfBase('A', 2));
    EXPECT_FALSE(stdads::IsDigitOfBase('-', 2));
    EXPECT_FALSE(stdads::IsDigitOfBase(' ', 2));
    EXPECT_FALSE(stdads::IsDigitOfBase('\0', 2));
}

TEST(IsDigitOfBaseTests, Base8Valid)
{
    for (char c = '0'; c <= '7'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 8));
    }
}


TEST(IsDigitOfBaseTests, Base8Invalid)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('0' - 1, 8));
    EXPECT_FALSE(stdads::IsDigitOfBase('8', 8));
    EXPECT_FALSE(stdads::IsDigitOfBase('a', 8));
    EXPECT_FALSE(stdads::IsDigitOfBase('A', 8));
    EXPECT_FALSE(stdads::IsDigitOfBase('-', 8));
    EXPECT_FALSE(stdads::IsDigitOfBase(' ', 8));
    EXPECT_FALSE(stdads::IsDigitOfBase('\0', 8));
}

TEST(IsDigitOfBaseTests, Base1Valid)
{
    EXPECT_TRUE(stdads::IsDigitOfBase('1', 1));
}

TEST(IsDigitOfBaseTests, Base1Invalid)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('0', 1));
    EXPECT_FALSE(stdads::IsDigitOfBase('2', 1));
    EXPECT_FALSE(stdads::IsDigitOfBase('a', 1));
    EXPECT_FALSE(stdads::IsDigitOfBase('A', 1));
    EXPECT_FALSE(stdads::IsDigitOfBase('-', 1));
    EXPECT_FALSE(stdads::IsDigitOfBase(' ', 1));
    EXPECT_FALSE(stdads::IsDigitOfBase('\0', 1));
}

TEST(IsDigitOfBaseTests, Base16Number)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 16));
    }
}

TEST(IsDigitOfBaseTests, Base16Lowercase)
{
    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 16));
    }
}

TEST(IsDigitOfBaseTests, Base16Uppercase)
{
    for (char c = 'A'; c <= 'F'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 16));
    }
}

TEST(IsDigitOfBaseTests, Base16Invalid)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('0' - 1, 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('9' + 1, 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('a' - 1, 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('A' - 1, 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('g', 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('G', 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('-', 16));
    EXPECT_FALSE(stdads::IsDigitOfBase(' ', 16));
    EXPECT_FALSE(stdads::IsDigitOfBase('\0', 16));
}

TEST(IsDigitOfBaseTests, Base36Number)
{
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 36));
    }
}

TEST(IsDigitOfBaseTests, Base36Lowercase)
{
    for (char c = 'a'; c <= 'z'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 36));
    }
}

TEST(IsDigitOfBaseTests, Base36Uppercase)
{
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        EXPECT_TRUE(stdads::IsDigitOfBase(c, 36));
    }
}

TEST(IsDigitOfBaseTests, Base36Invalid)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('0' - 1, 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('9' + 1, 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('a' - 1, 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('A' - 1, 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('z' + 1, 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('Z' + 1, 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('-', 36));
    EXPECT_FALSE(stdads::IsDigitOfBase(' ', 36));
    EXPECT_FALSE(stdads::IsDigitOfBase('\0', 36));
}

TEST(IsDigitOfBaseTests, Base35CharOutOfRange)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('z', 35));
}

TEST(IsDigitOfBaseTests, InvalidBaseLow)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('1', 0));
    EXPECT_FALSE(stdads::IsDigitOfBase('1', -1));
}

TEST(IsDigitOfBaseTests, InvalidBaseHigh)
{
    EXPECT_FALSE(stdads::IsDigitOfBase('1', 37));
    EXPECT_FALSE(stdads::IsDigitOfBase('z', 100));
}


// ---------------- CharToDigit ----------------

TEST(CharToDigitTests, Base10Valid)
{
    int i = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 10));
        ++i;
    }
}

TEST(CharToDigitTests, Base10Invalid)
{
    EXPECT_EQ(-1, stdads::CharToDigit('0' - 1, 10));
    EXPECT_EQ(-1, stdads::CharToDigit('9' + 1, 10));
    EXPECT_EQ(-1, stdads::CharToDigit('a', 10));
    EXPECT_EQ(-1, stdads::CharToDigit('A', 10));
    EXPECT_EQ(-1, stdads::CharToDigit('-', 10));
    EXPECT_EQ(-1, stdads::CharToDigit(' ', 10));
    EXPECT_EQ(-1, stdads::CharToDigit('\0', 10));
}

TEST(CharToDigitTests, Base2Valid)
{
    EXPECT_EQ(0, stdads::CharToDigit('0', 2));
    EXPECT_EQ(1, stdads::CharToDigit('1', 2));
}


TEST(CharToDigitTests, Base2Invalid)
{
    EXPECT_EQ(-1, stdads::CharToDigit('0' - 1, 2));
    EXPECT_EQ(-1, stdads::CharToDigit('2', 2));
    EXPECT_EQ(-1, stdads::CharToDigit('a', 2));
    EXPECT_EQ(-1, stdads::CharToDigit('A', 2));
    EXPECT_EQ(-1, stdads::CharToDigit('-', 2));
    EXPECT_EQ(-1, stdads::CharToDigit(' ', 2));
    EXPECT_EQ(-1, stdads::CharToDigit('\0', 2));
}

TEST(CharToDigitTests, Base8Valid)
{
    int i = 0;
    for (char c = '0'; c <= '7'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 8));
        ++i;
    }
}


TEST(CharToDigitTests, Base8Invalid)
{
    EXPECT_EQ(-1, stdads::CharToDigit('0' - 1, 8));
    EXPECT_EQ(-1, stdads::CharToDigit('8', 8));
    EXPECT_EQ(-1, stdads::CharToDigit('a', 8));
    EXPECT_EQ(-1, stdads::CharToDigit('A', 8));
    EXPECT_EQ(-1, stdads::CharToDigit('-', 8));
    EXPECT_EQ(-1, stdads::CharToDigit(' ', 8));
    EXPECT_EQ(-1, stdads::CharToDigit('\0', 8));
}

TEST(CharToDigitTests, Base1Valid)
{
    EXPECT_EQ(1, stdads::CharToDigit('1', 1));
}

TEST(CharToDigitTests, Base1Invalid)
{
    EXPECT_EQ(-1, stdads::CharToDigit('0', 1));
    EXPECT_EQ(-1, stdads::CharToDigit('2', 1));
    EXPECT_EQ(-1, stdads::CharToDigit('a', 1));
    EXPECT_EQ(-1, stdads::CharToDigit('A', 1));
    EXPECT_EQ(-1, stdads::CharToDigit('-', 1));
    EXPECT_EQ(-1, stdads::CharToDigit(' ', 1));
    EXPECT_EQ(-1, stdads::CharToDigit('\0', 1));
}

TEST(CharToDigitTests, Base16Number)
{
    int i = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 16));
        ++i;
    }
}

TEST(CharToDigitTests, Base16Lowercase)
{
    int i = 10;
    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 16));
        ++i;
    }
}

TEST(CharToDigitTests, Base16Uppercase)
{
    int i = 10;
    for (char c = 'A'; c <= 'F'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 16));
        ++i;
    }
}

TEST(CharToDigitTests, Base16Invalid)
{
    EXPECT_EQ(-1, stdads::CharToDigit('0' - 1, 16));
    EXPECT_EQ(-1, stdads::CharToDigit('9' + 1, 16));
    EXPECT_EQ(-1, stdads::CharToDigit('a' - 1, 16));
    EXPECT_EQ(-1, stdads::CharToDigit('A' - 1, 16));
    EXPECT_EQ(-1, stdads::CharToDigit('g', 16));
    EXPECT_EQ(-1, stdads::CharToDigit('G', 16));
    EXPECT_EQ(-1, stdads::CharToDigit('-', 16));
    EXPECT_EQ(-1, stdads::CharToDigit(' ', 16));
    EXPECT_EQ(-1, stdads::CharToDigit('\0', 16));
}

TEST(CharToDigitTests, Base36Number)
{
    int i = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 36));
        ++i;
    }
}

TEST(CharToDigitTests, Base36Lowercase)
{
    int i = 10;
    for (char c = 'a'; c <= 'z'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 36));
        ++i;
    }
}

TEST(CharToDigitTests, Base36Uppercase)
{
    int i = 10;
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        EXPECT_EQ(i, stdads::CharToDigit(c, 36));
        ++i;
    }
}

TEST(CharToDigitTests, Base36Invalid)
{
    EXPECT_EQ(-1, stdads::CharToDigit('0' - 1, 36));
    EXPECT_EQ(-1, stdads::CharToDigit('9' + 1, 36));
    EXPECT_EQ(-1, stdads::CharToDigit('a' - 1, 36));
    EXPECT_EQ(-1, stdads::CharToDigit('A' - 1, 36));
    EXPECT_EQ(-1, stdads::CharToDigit('z' + 1, 36));
    EXPECT_EQ(-1, stdads::CharToDigit('Z' + 1, 36));
    EXPECT_EQ(-1, stdads::CharToDigit('-', 36));
    EXPECT_EQ(-1, stdads::CharToDigit(' ', 36));
    EXPECT_EQ(-1, stdads::CharToDigit('\0', 36));
}

TEST(CharToDigitTests, Base35CharOutOfRange)
{
    EXPECT_EQ(-1, stdads::CharToDigit('z', 35));
}

TEST(CharToDigitTests, InvalidBaseLow)
{
    EXPECT_EQ(-1, stdads::CharToDigit('1', 0));
    EXPECT_EQ(-1, stdads::CharToDigit('1', -1));
}

TEST(CharToDigitTests, InvalidBaseHigh)
{
    EXPECT_EQ(-1, stdads::CharToDigit('1', 37));
    EXPECT_EQ(-1, stdads::CharToDigit('z', 100));
}


// ---------------- ToLower ----------------

TEST(ToLowerTests, ConvertsUppercaseLetters)
{
    EXPECT_EQ(stdads::ToLower('A'), 'a');
    EXPECT_EQ(stdads::ToLower('Z'), 'z');
    EXPECT_EQ(stdads::ToLower('M'), 'm');
}

TEST(ToLowerTests, BoundaryCharacters)
{
    EXPECT_EQ(stdads::ToLower('A'), 'a');
    EXPECT_EQ(stdads::ToLower('Z'), 'z');
}

TEST(ToLowerTests, LowercaseUnchanged)
{
    EXPECT_EQ(stdads::ToLower('a'), 'a');
    EXPECT_EQ(stdads::ToLower('z'), 'z');
    EXPECT_EQ(stdads::ToLower('m'), 'm');
}

TEST(ToLowerTests, DigitsUnchanged)
{
    EXPECT_EQ(stdads::ToLower('0'), '0');
    EXPECT_EQ(stdads::ToLower('9'), '9');
}

TEST(ToLowerTests, SymbolsUnchanged)
{
    EXPECT_EQ(stdads::ToLower('!'), '!');
    EXPECT_EQ(stdads::ToLower('@'), '@');
    EXPECT_EQ(stdads::ToLower('['), '[');
}

TEST(ToLowerTests, WhitespaceUnchanged)
{
    EXPECT_EQ(stdads::ToLower(' '), ' ');
    EXPECT_EQ(stdads::ToLower('\n'), '\n');
    EXPECT_EQ(stdads::ToLower('\t'), '\t');
}

TEST(ToLowerTests, AdjacentAsciiCharacters)
{
    EXPECT_EQ(stdads::ToLower('@'), '@');  // before 'A'
    EXPECT_EQ(stdads::ToLower('['), '[');  // after 'Z'
}


// ---------------- ToUpper ----------------

TEST(ToUpperTests, ConvertsLowercaseLetters)
{
    EXPECT_EQ(stdads::ToUpper('a'), 'A');
    EXPECT_EQ(stdads::ToUpper('z'), 'Z');
    EXPECT_EQ(stdads::ToUpper('m'), 'M');
}

TEST(ToUpperTests, BoundaryCharacters)
{
    EXPECT_EQ(stdads::ToUpper('a'), 'A');
    EXPECT_EQ(stdads::ToUpper('z'), 'Z');
}

TEST(ToUpperTests, UppercaseUnchanged)
{
    EXPECT_EQ(stdads::ToUpper('A'), 'A');
    EXPECT_EQ(stdads::ToUpper('Z'), 'Z');
    EXPECT_EQ(stdads::ToUpper('M'), 'M');
}

TEST(ToUpperTests, DigitsUnchanged)
{
    EXPECT_EQ(stdads::ToUpper('0'), '0');
    EXPECT_EQ(stdads::ToUpper('9'), '9');
}

TEST(ToUpperTests, SymbolsUnchanged)
{
    EXPECT_EQ(stdads::ToUpper('!'), '!');
    EXPECT_EQ(stdads::ToUpper('@'), '@');
    EXPECT_EQ(stdads::ToUpper('{'), '{');
}

TEST(ToUpperTests, WhitespaceUnchanged)
{
    EXPECT_EQ(stdads::ToUpper(' '), ' ');
    EXPECT_EQ(stdads::ToUpper('\n'), '\n');
    EXPECT_EQ(stdads::ToUpper('\t'), '\t');
}

TEST(ToUpperTests, AdjacentAsciiCharacters)
{
    EXPECT_EQ(stdads::ToUpper('`'), '`');  // before 'a'
    EXPECT_EQ(stdads::ToUpper('{'), '{');  // after 'z'
}