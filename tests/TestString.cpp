#include <gtest/gtest.h>
#include <stdads/String.h>

// memchar tests
TEST(MemchrTest, FindCharacter) {
    const char* str = "Hello, World!";
    const void* result = stdads::memchr(str, 'W', 13);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(*(static_cast<const char*>(result)), 'W');
}

TEST(MemchrTest, FindsExistingByte) {
    const char buffer[] = "hello world";
    const void* result = stdads::memchr(buffer, 'w', sizeof(buffer));
    EXPECT_EQ(static_cast<const void*>(buffer + 6), result);
}

TEST(MemchrTest, FindsLastByte)
{
    const char buffer[] = "hello";
    const void* result = stdads::memchr(buffer, 'o', sizeof(buffer));
    EXPECT_EQ(static_cast<const void*>(buffer + 4), result);
}

TEST(MemchrTest, FindsFirstOccurrenceOnly) {
    const char buffer[] = "banana";
    const void* result = stdads::memchr(buffer, 'a', sizeof(buffer));
    EXPECT_EQ(static_cast<const void*>(buffer + 1), result); // first 'a'
}

TEST(MemchrTest, ReturnsNullWhenNotFound) {
    const char buffer[] = "abcdef";
    const void* result = stdads::memchr(buffer, 'z', sizeof(buffer));
    EXPECT_EQ(nullptr, result);
}

TEST(MemchrTest, RespectsLengthLimit) {
    const char buffer[] = "hello world";

    // Search only first 5 bytes: "hello"
    const void* result = stdads::memchr(buffer, 'w', 5);
    EXPECT_EQ(nullptr, result); // 'w' is outside search range
}

TEST(MemchrTest, WorksWithBinaryData) {
    unsigned char buffer[] = {0x00, 0x10, 0xFF, 0x20, 0xFF};
    const void* result = stdads::memchr(buffer, 0xFF, sizeof(buffer));
    EXPECT_EQ(static_cast<const void*>(buffer + 2), result);
}

TEST(MemchrTest, FindsNullByte) {
    const char buffer[] = {'a', 'b', '\0', 'c', 'd'};
    const void* result = stdads::memchr(buffer, '\0', sizeof(buffer));
    EXPECT_EQ(static_cast<const void*>(buffer + 2), result);
}

TEST(MemchrTest, ZeroLengthReturnsNull) {
    const char buffer[] = "test";
    const void* result = stdads::memchr(buffer, 't', 0);
    EXPECT_EQ(nullptr, result);
}

TEST(MemchrTest, NullPointerWithZeroLength) {
    const void* result = stdads::memchr(nullptr, 'a', 0);
    EXPECT_EQ(nullptr, result);
}


// memcmp tests
TEST(MemcmpTest, EqualBuffers) {
    const char a[] = "hello";
    const char b[] = "hello";
    int result = stdads::memcmp(a, b, 5);
    EXPECT_EQ(0, result);
}

TEST(MemcmpTest, ReferenceEquals)
{
    const char a[] = "hello";
    int result = stdads::memcmp(a, a, 5);
    EXPECT_EQ(0, result);
}

TEST(MemcmpTest, FirstLessThanSecond) {
    const char a[] = "abc";
    const char b[] = "abd";
    int result = stdads::memcmp(a, b, 3);
    EXPECT_LT(result, 0);  // 'c' < 'd'
}

TEST(MemcmpTest, FirstGreaterThanSecond) {
    const char a[] = "abe";
    const char b[] = "abd";
    int result = stdads::memcmp(a, b, 3);
    EXPECT_GT(result, 0);  // 'e' > 'd'
}

TEST(MemcmpTest, PartialCompareEqual) {
    const char a[] = "hello123";
    const char b[] = "hello999";
    int result = stdads::memcmp(a, b, 5); // only compare "hello"
    EXPECT_EQ(0, result);
}

TEST(MemcmpTest, RespectsLengthLimit) {
    const char a[] = "abcXXX";
    const char b[] = "abcYYY";
    int result = stdads::memcmp(a, b, 3); // compare only "abc"
    EXPECT_EQ(0, result);
}

TEST(MemcmpTest, BinaryDataComparison) {
    unsigned char a[] = {0x01, 0x02, 0xFF};
    unsigned char b[] = {0x01, 0x02, 0x10};
    int result = stdads::memcmp(a, b, 3);
    EXPECT_GT(result, 0);  // 0xFF > 0x10
}

TEST(MemcmpTest, FindsDifferenceAtCorrectIndex) {
    unsigned char a[] = {0x10, 0x20, 0x30, 0x40};
    unsigned char b[] = {0x10, 0x20, 0x31, 0x40};
    int result = stdads::memcmp(a, b, 4);
    EXPECT_LT(result, 0);  // 0x30 < 0x31
}

TEST(MemcmpTest, ZeroLengthReturnsEqual) {
    const char a[] = "abc";
    const char b[] = "xyz";
    int result = stdads::memcmp(a, b, 0);
    EXPECT_EQ(0, result);

    result = stdads::memcmp(a, a, 0);
    EXPECT_EQ(0, result);
}

TEST(MemcmpTest, NullptrWithZeroLength) {
    int result = stdads::memcmp(nullptr, nullptr, 0);
    EXPECT_EQ(0, result);
}


// memcpy tests
TEST(MemcpyTest, BasicCopy) {
    char src[] = "hello";
    char dst[6] = {};
    void* ret = stdads::memcpy(dst, src, 6);
    EXPECT_EQ(dst, ret);
    EXPECT_STREQ("hello", dst);
}

TEST(MemcpyTest, PartialCopy) {
    char src[] = "hello";
    char dst[6] = {};
    stdads::memcpy(dst, src, 3);
    EXPECT_EQ('h', dst[0]);
    EXPECT_EQ('e', dst[1]);
    EXPECT_EQ('l', dst[2]);
}

TEST(MemcpyTest, BinaryDataCopy) {
    unsigned char src[] = {0x01, 0xFF, 0x10, 0x20};
    unsigned char dst[4] = {};
    stdads::memcpy(dst, src, 4);
    EXPECT_EQ(0x01, dst[0]);
    EXPECT_EQ(0xFF, dst[1]);
    EXPECT_EQ(0x10, dst[2]);
    EXPECT_EQ(0x20, dst[3]);
}

TEST(MemcpyTest, ZeroLengthCopy) {
    char src[] = "hello";
    char dst[] = "world";
    stdads::memcpy(dst, src, 0);
    EXPECT_STREQ("world", dst); // unchanged
}

TEST(MemcpyTest, LargeCopy) {
    const std::size_t size = 1024;
    unsigned char src[size];
    unsigned char dst[size];

    for (std::size_t i = 0; i < size; ++i)
        src[i] = static_cast<unsigned char>(i % 256);

    stdads::memcpy(dst, src, size);

    for (std::size_t i = 0; i < size; ++i)
        EXPECT_EQ(src[i], dst[i]);
}

TEST(MemcpyTest, ReturnsDestinationPointer) {
    char src[] = "abc";
    char dst[4];
    void* ret = stdads::memcpy(dst, src, 4);
    EXPECT_EQ(dst, ret);
}

TEST(MemcpyTest, NullptrWithZeroLength) {
    void* ret = stdads::memcpy(nullptr, nullptr, 0);
    EXPECT_EQ(nullptr, ret);
}


// memset tests
TEST(MemsetTest, BasicSet) {
    char buffer[10];
    void* ret = stdads::memset(buffer, 'A', 10);

    EXPECT_EQ(buffer, ret);
    for (int i = 0; i < 10; ++i)
        EXPECT_EQ('A', buffer[i]);
}

TEST(MemsetTest, PartialSet) {
    char buffer[10] = {};
    stdads::memset(buffer, 'B', 5);

    for (int i = 0; i < 5; ++i)
        EXPECT_EQ('B', buffer[i]);
    for (int i = 5; i < 10; ++i)
        EXPECT_EQ(0, buffer[i]);
}

TEST(MemsetTest, BinaryValueSet) {
    unsigned char buffer[4];
    stdads::memset(buffer, 0xFF, 4);

    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(0xFF, buffer[i]);
}

TEST(MemsetTest, ZeroValueSet) {
    char buffer[8];
    stdads::memset(buffer, 0, 8);

    for (int i = 0; i < 8; ++i)
        EXPECT_EQ(0, buffer[i]);
}

TEST(MemsetTest, ZeroLengthDoesNothing) {
    char buffer[5] = {'a','b','c','d','e'};
    stdads::memset(buffer, 'X', 0);
    EXPECT_EQ('a', buffer[0]);
    EXPECT_EQ('b', buffer[1]);
    EXPECT_EQ('c', buffer[2]);
    EXPECT_EQ('d', buffer[3]);
    EXPECT_EQ('e', buffer[4]);
}

TEST(MemsetTest, LargeBufferSet) {
    const std::size_t size = 1024;
    unsigned char buffer[size];
    stdads::memset(buffer, 0xAB, size);

    for (std::size_t i = 0; i < size; ++i)
        EXPECT_EQ(0xAB, buffer[i]);
}

TEST(MemsetTest, ReturnsPointer) {
    char buffer[4];
    void* ret = stdads::memset(buffer, 1, 4);
    EXPECT_EQ(buffer, ret);
}

TEST(MemsetTest, NullptrWithZeroLength) {
    void* ret = stdads::memset(nullptr, 0, 0);
    EXPECT_EQ(nullptr, ret);
}


// strcat tests
TEST(StrCatTest, AppendToEmptyString) {
    char dest[32] = "";
    const char src[] = "hello";
    char* result = stdads::strcat(dest, src);
    EXPECT_EQ(dest, result);
    EXPECT_STREQ("hello", dest);
}

TEST(StrCatTest, AppendNonEmptyStrings) {
    char dest[32] = "hello";
    const char src[] = " world";
    stdads::strcat(dest, src);
    EXPECT_STREQ("hello world", dest);
}

TEST(StrCatTest, AppendSingleCharacter) {
    char dest[8] = "a";
    const char src[] = "b";
    stdads::strcat(dest, src);
    EXPECT_STREQ("ab", dest);
}

TEST(StrCatTest, AppendEmptySource) {
    char dest[16] = "hello";
    const char src[] = "";
    stdads::strcat(dest, src);
    EXPECT_STREQ("hello", dest);
}

TEST(StrCatTest, AppendToLongerString) {
    char dest[64] = "The quick ";
    const char src[] = "brown fox";
    stdads::strcat(dest, src);
    EXPECT_STREQ("The quick brown fox", dest);
}


// strchar tests
TEST(StrChrTest, FindsCharacterAtBeginning) {
    const char str[] = "hello";
    const char* result = stdads::strchr(str, 'h');
    ASSERT_NE(nullptr, result);
    EXPECT_EQ('h', *result);
    EXPECT_EQ(str, result);
}

TEST(StrChrTest, FindsCharacterInMiddle) {
    const char str[] = "hello";
    const char* result = stdads::strchr(str, 'l');
    ASSERT_NE(nullptr, result);
    EXPECT_EQ('l', *result);
    EXPECT_EQ(&str[2], result);
}

TEST(StrChrTest, FindsCharacterAtEnd) {
    const char str[] = "hello";
    const char* result = stdads::strchr(str, 'o');
    ASSERT_NE(nullptr, result);
    EXPECT_EQ('o', *result);
    EXPECT_EQ(&str[4], result);
}

TEST(StrChrTest, ReturnsFirstOccurrence) {
    const char str[] = "banana";
    const char* result = stdads::strchr(str, 'a');
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(&str[1], result);
}

TEST(StrChrTest, CharacterNotFoundReturnsNullptr) {
    const char str[] = "hello";
    const char* result = stdads::strchr(str, 'z');
    EXPECT_EQ(nullptr, result);
}

TEST(StrChrTest, FindsNullTerminator) {
    const char str[] = "hello";
    const char* result = stdads::strchr(str, '\0');
    ASSERT_NE(nullptr, result);
    EXPECT_EQ('\0', *result);
    EXPECT_EQ(str + 5, result);
}

TEST(StrChrTest, EmptyString) {
    const char str[] = "";
    const char* result = stdads::strchr(str, 'a');
    EXPECT_EQ(nullptr, result);
}

TEST(StrChrTest, EmptyStringFindNullTerminator) {
    const char str[] = "";
    const char* result = stdads::strchr(str, '\0');
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(str, result);
}


// strcmp tests
TEST(StrcmpTest, comparison) {
    EXPECT_EQ(0, stdads::strcmp("", ""));
    EXPECT_LT(stdads::strcmp("a", "b"), 0);
    EXPECT_GT(stdads::strcmp("b", "a"), 0);
    EXPECT_EQ(0, stdads::strcmp("test", "test"));
    EXPECT_LT(stdads::strcmp("abc", "abcd"), 0);
    EXPECT_GT(stdads::strcmp("abcd", "abc"), 0);
}

TEST(StrcmpTest, BothEmpty) {
    const char* a = "";
    const char* b = "";
    int result = stdads::strcmp(a, b);
    EXPECT_EQ(0, result);
}

TEST(StrcmpTest, EqualStrings) {
    const char* a = "hello";
    const char* b = "hello";
    int result = stdads::strcmp(a, b);
    EXPECT_EQ(0, result);
}

TEST(StrcmpTest, FirstLessThanSecond) {
    const char* a = "abc";
    const char* b = "abd";
    int result = stdads::strcmp(a, b);
    EXPECT_LT(result, 0);
}

TEST(StrcmpTest, FirstGreaterThanSecond) {
    const char* a = "abe";
    const char* b = "abd";
    int result = stdads::strcmp(a, b);
    EXPECT_GT(result, 0);
}

TEST(StrcmpTest, PrefixShorterIsLess) {
    const char* a = "abc";
    const char* b = "abcd";
    int result = stdads::strcmp(a, b);
    EXPECT_LT(result, 0);
}

TEST(StrcmpTest, PrefixLongerIsGreater) {
    const char* a = "abcd";
    const char* b = "abc";
    int result = stdads::strcmp(a, b);
    EXPECT_GT(result, 0);
}

TEST(StrcmpTest, CaseSensitivity) {
    const char* a = "abc";
    const char* b = "Abc";
    int result = stdads::strcmp(a, b);
    EXPECT_GT(result, 0); // 'a' > 'A' in ASCII
}

TEST(StrcmpTest, SpecialCharacters) {
    const char* a = "a\nb";
    const char* b = "a\tb";
    int result = stdads::strcmp(a, b);
    EXPECT_GT(result, 0); // '\n' (10) > '\t' (9)
}

TEST(StrcmpTest, EmbeddedNullTerminates) {
    const char a[] = {'a', 'b', '\0', 'c'};
    const char b[] = {'a', 'b', '\0', 'd'};
    int result = stdads::strcmp(a, b);
    EXPECT_EQ(0, result); // must stop at first '\0'
}

TEST(StrcmpTest, NullptrBoth) {
    const char* a = nullptr;
    const char* b = nullptr;
    int result = stdads::strcmp(a, b);
    EXPECT_EQ(0, result);
}


// strcpy tests
TEST(StrcpyTest, BasicCopy) {
    char src[] = "hello";
    char dst[6] = {};
    char* ret = stdads::strcpy(dst, src);
    EXPECT_EQ(dst, ret);
    EXPECT_STREQ("hello", dst);
}

TEST(StrcpyTest, EmptyStringCopy) {
    char src[] = "";
    char dst[1] = {'X'};
    stdads::strcpy(dst, src);
    EXPECT_EQ('\0', dst[0]);
}

TEST(StrcpyTest, SingleCharacterCopy) {
    char src[] = "A";
    char dst[2];
    stdads::strcpy(dst, src);
    EXPECT_EQ('A', dst[0]);
    EXPECT_EQ('\0', dst[1]);
}

TEST(StrcpyTest, IncludesNullTerminator) {
    char src[] = "abc";
    char dst[4];
    stdads::strcpy(dst, src);
    EXPECT_EQ('a', dst[0]);
    EXPECT_EQ('b', dst[1]);
    EXPECT_EQ('c', dst[2]);
    EXPECT_EQ('\0', dst[3]);  // must copy terminator
}

TEST(StrcpyTest, ReferenceEqual) {
    char src[] = "abc";
    stdads::strcpy(src, src);
    EXPECT_EQ('a', src[0]);
    EXPECT_EQ('b', src[1]);
    EXPECT_EQ('c', src[2]);
    EXPECT_EQ('\0', src[3]);  // must copy terminator
}

TEST(StrcpyTest, BinarySafeCopy) {
    const char src[] = {'a', 'b', '\0', 'c', 'd', '\0'};
    char dst[6];
    stdads::strcpy(dst, src);
    EXPECT_EQ('a', dst[0]);
    EXPECT_EQ('b', dst[1]);
    EXPECT_EQ('\0', dst[2]);  // stop at first null
}

TEST(StrcpyTest, LargeStringCopy) {
    const std::size_t size = 1024;
    char src[size];
    char dst[size];

    for (std::size_t i = 0; i < size - 1; ++i)
        src[i] = 'A';
    src[size - 1] = '\0';

    stdads::strcpy(dst, src);
    EXPECT_STREQ(src, dst);
}

TEST(StrcpyTest, ReturnsDestinationPointer) {
    char src[] = "abc";
    char dst[4];
    char* ret = stdads::strcpy(dst, src);
    EXPECT_EQ(dst, ret);
}


// strlen tests
TEST(StrlenTest, EmptyString) {
    const char* s = "";
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(0u, result);
}

TEST(StrlenTest, SingleCharacter) {
    const char* s = "a";
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(1u, result);
}

TEST(StrlenTest, NormalString) {
    const char* s = "hello";
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(5u, result);
}

TEST(StrlenTest, StringWithSpaces) {
    const char* s = "hello world";
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(11u, result);
}

TEST(StrlenTest, StringWithSpecialCharacters) {
    const char* s = "a\tb\nc";
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(5u, result); // '\t' and '\n' count as characters
}

TEST(StrlenTest, StopsAtNullTerminator) {
    const char s[] = {'a', 'b', '\0', 'c', 'd'};
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(2u, result); // must stop at first '\0'
}

TEST(StrlenTest, LongString) {
    const char* s = "abcdefghijklmnopqrstuvwxyz";
    std::size_t result = stdads::strlen(s);
    EXPECT_EQ(26u, result);
}


// String class constructors
TEST(StringConstructorTest, DefaultConstructorCreatesEmptyString) {
    stdads::String s;
    EXPECT_EQ(0u, s.Size());
    EXPECT_STREQ("", s.c_str());

    stdads::String<100> largeStr;
    EXPECT_EQ(0u, largeStr.Size());
    EXPECT_STREQ("", largeStr.c_str());

    stdads::String<10> smallStr;
    EXPECT_EQ(0u, smallStr.Size());
    EXPECT_STREQ("", smallStr.c_str());

    stdads::String<0> zeroCapacityStr;
    EXPECT_EQ(0u, zeroCapacityStr.Size());
    EXPECT_STREQ("", zeroCapacityStr.c_str());
}

TEST(StringConstructorTest, DefaultConstructorCapacity) {
    stdads::String s;
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());

    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES - 10> smallStr;
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES - 10, smallStr.Capacity());

    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES + 10> largeStr;
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES + 10, largeStr.Capacity());

    stdads::String<0> zeroCapacityStr;
    EXPECT_EQ(0, zeroCapacityStr.Capacity());
}

TEST(StringConstructorTest, CStringConstructorCopiesString) {
    stdads::String s("hello");
    EXPECT_EQ(5u, s.Size());
    EXPECT_STREQ("hello", s.c_str());
}

TEST(StringConstructorTest, CStringConstructorWithEmptyString) {
    stdads::String s("");
    EXPECT_EQ(0u, s.Size());
    EXPECT_STREQ("", s.c_str());
}

TEST(StringConstructorTest, CStringConstructorCreatesDeepCopy) {
    const char* cstr = "hello";
    stdads::String str(cstr);
    EXPECT_NE(cstr, str.c_str()); // Addresses must differ (deep copy)
}

TEST(StringConstructorTest, CStringConstructorCapacity) {
    stdads::String s("hello");
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());

    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES - 10> smallStr("hello");
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES - 10, smallStr.Capacity());

    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES + 10> largeStr("hello");
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES + 10, largeStr.Capacity());

    stdads::String<0> zeroCapacityStr("");
    EXPECT_EQ(0, zeroCapacityStr.Capacity());
}

TEST(StringConstructorTest, CStringConstructorGrows) {
    const char* cstr = "abcdefghijklmnopqrstuvwxyz"; // 26 chars > than default 24 characters;

    stdads::String s(cstr);
    EXPECT_EQ(26u, s.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES * 2, s.Capacity()); // capacity always doubles when growing
    EXPECT_STREQ(cstr, s.c_str());

    stdads::String<10> smallStr(cstr);
    EXPECT_EQ(26u, smallStr.Size());
    EXPECT_EQ(40u, smallStr.Capacity()); // needs to grow twice
    EXPECT_STREQ(cstr, smallStr.c_str());
}

TEST(StringConstructorTest, CharConstructorCopiesChar) {
    stdads::String s('a');
    EXPECT_EQ(1u, s.Size());
    EXPECT_STREQ("a", s.c_str());
}

TEST(StringConstructorTest, CharConstructorCreatesDeepCopy) {
    const char c = 'a';
    stdads::String str(c);
    EXPECT_NE(&c, str.c_str()); // Addresses must differ (deep copy)
}

TEST(StringConstructorTest, CharConstructorCapacity) {
    stdads::String s('a');
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());

    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES - 10> smallStr('a');
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES - 10, smallStr.Capacity());

    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES + 10> largeStr('a');
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES + 10, largeStr.Capacity());
}

TEST(StringConstructorTest, CharConstructorGrows) {
    stdads::String<0> s('!');
    EXPECT_EQ(1u, s.Size());
    EXPECT_EQ(1u, s.Capacity()); // 0 capacity edge case. Should grow to 1.
    EXPECT_STREQ("!", s.c_str());
}

TEST(StringConstructorTest, CopyConstructorCopiesContent) {
    stdads::String original("hello");
    stdads::String copy(original);
    EXPECT_EQ(original.Size(), copy.Size());
    EXPECT_STREQ(original.c_str(), copy.c_str());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, copy.Capacity());

    stdads::String<100> largeStr("hello");
    stdads::String<100> largeCopy(largeStr);
    EXPECT_EQ(largeStr.Size(), largeCopy.Size());
    EXPECT_STREQ(largeStr.c_str(), largeCopy.c_str());
    EXPECT_EQ(100u, largeCopy.Capacity());
}

TEST(StringConstructorTest, CopyConstructorCreatesDeepCopy) {
    stdads::String original("hello");
    stdads::String copy(original);
    EXPECT_NE(original.c_str(), copy.c_str()); // Addresses must differ (deep copy)
}

TEST(StringConstructorTest, CopyConstructorDifferentCapacities) {
    stdads::String<10> original("hello");
    stdads::String<15> copy(original);
    EXPECT_EQ(original.Size(), copy.Size());
    EXPECT_STREQ(original.c_str(), copy.c_str());
    EXPECT_NE(original.c_str(), copy.c_str()); // Addresses must differ (deep copy)
    EXPECT_EQ(15u, copy.Capacity());
}

TEST(StringConstructorTest, CopyConstructorDifferentCapacitiesGrows) {
    stdads::String<10> original("hello");
    stdads::String<4> copy(original);
    EXPECT_EQ(original.Size(), copy.Size());
    EXPECT_STREQ(original.c_str(), copy.c_str());
    EXPECT_NE(original.c_str(), copy.c_str()); // Addresses must differ (deep copy)
    EXPECT_EQ(8u, copy.Capacity());
}


// String class assignment operators
TEST(StringAssignmentTest, AssignFromAnotherString)
{
    stdads::String a("hello");
    stdads::String b("world");
    a = b;

    EXPECT_EQ(5u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, a.Capacity());
    EXPECT_STREQ("world", a.c_str());
    EXPECT_NE(a.c_str(), b.c_str()); // deep copy
    EXPECT_NE(&a, &b);
}

TEST(StringAssignmentTest, AssignSelf)
{
    stdads::String a("hello");
    const char* before = a.c_str();
    a = a;

    EXPECT_EQ(5u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, a.Capacity());
    EXPECT_STREQ("hello", a.c_str());
    EXPECT_EQ(before, a.c_str()); // no reallocation required
}

TEST(StringAssignmentTest, AssignFromEmptyString)
{
    stdads::String a("hello");
    stdads::String empty;
    a = empty;

    EXPECT_EQ(0u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, a.Capacity());
    EXPECT_STREQ("", a.c_str());
    EXPECT_NE(a.c_str(), empty.c_str()); // deep copy
    EXPECT_NE(&a, &empty);
}

TEST(StringAssignmentTest, AssignFromAnotherStringGrowsCapacity)
{
    stdads::String a("hello");
    stdads::String b("abcdefghijklmnopqrstuvwxyz");
    a = b;

    EXPECT_EQ(26u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES * 2, a.Capacity());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", a.c_str());
    EXPECT_NE(a.c_str(), b.c_str()); // deep copy
    EXPECT_NE(&a, &b);


    stdads::String<10> c("goodbye");
    stdads::String<10> d("abcdefghijklmnopqrstuvwxyz");
    c = d;

    EXPECT_EQ(26u, c.Size());
    EXPECT_EQ(40u, c.Capacity());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", c.c_str());
    EXPECT_NE(c.c_str(), d.c_str()); // deep copy
    EXPECT_NE(&c, &d);
}

TEST(StringAssignmentTest, AssignFromDifferentCapacity)
{
    stdads::String a("hello");
    stdads::String<50> b("abcdefghijklmnopqrstuvwxyz");
    a = b;

    EXPECT_EQ(26u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES * 2, a.Capacity());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", a.c_str());
    EXPECT_NE(a.c_str(), b.c_str()); // deep copy
    EXPECT_NE(static_cast<void*>(&a), static_cast<void*>(&b));
}

TEST(StringAssignmentTest, AssignFromCString)
{
    stdads::String s("hello");
    const char* cstr = "world";
    s = cstr;

    EXPECT_EQ(5u, s.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());
    EXPECT_STREQ("world", s.c_str());
    EXPECT_NE(s.c_str(), cstr); // deep copy
}

TEST(StringAssignmentTest, AssignFromEmptyCString)
{
    stdads::String s("hello");
    s = "";

    EXPECT_EQ(0u, s.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());
    EXPECT_STREQ("", s.c_str());
}

TEST(StringAssignmentTest, AssignSameCStringContent)
{
    stdads::String s("hello");
    const char* before = s.c_str();
    s = before;

    EXPECT_EQ(5u, s.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());
    EXPECT_STREQ("hello", s.c_str());
    EXPECT_EQ(s.c_str(), before); // buffer should be reused
}

TEST(StringAssignmentTest, AssignSameCStringOverlappingMemory)
{
    stdads::String s("hello");
    const char* before = s.c_str();
    s = before + 1;

    EXPECT_EQ(4u, s.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());
    EXPECT_STREQ("ello", s.c_str());
    EXPECT_EQ(s.c_str(), before); // buffer should be reused
}

TEST(StringAssignmentTest, AssignFromCStringGrowsCapacity)
{
    stdads::String a("hello");
    const char* b = "abcdefghijklmnopqrstuvwxyz";
    a = b;

    EXPECT_EQ(26u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES * 2, a.Capacity());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", a.c_str());
    EXPECT_NE(a.c_str(), b); // deep copy

    
    stdads::String<10> c("goodbye");
    const char* d = "abcdefghijklmnopqrstuvwxyz";
    c = d;

    EXPECT_EQ(26u, c.Size());
    EXPECT_EQ(40u, c.Capacity());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", c.c_str());
    EXPECT_NE(c.c_str(), d); // deep copy
}

TEST(StringAssignmentTest, AssignFromCStringDoesNotShrinksCapacity)
{
    stdads::String a("abcdefghijklmnopqrstuvwxyz");
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES * 2, a.Capacity());
    
    const char* b = "hello";
    a = b;

    EXPECT_EQ(5u, a.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES * 2, a.Capacity());
    EXPECT_STREQ(a.c_str(), b);
    EXPECT_NE(a.c_str(), b); // deep copy
}

TEST(StringAssignmentTest, AssignFromChar)
{
    stdads::String s("hello");
    s = '!';

    EXPECT_EQ(1u, s.Size());
    EXPECT_EQ(stdads::DEFAULT_STRING_CAPACITY_BYTES, s.Capacity());
    EXPECT_STREQ("!", s.c_str());
}

TEST(StringAssignmentTest, AssignFromCharGrowsCapacity) {
    const char c = '!';
    stdads::String<0> s("");
    s = c;
    EXPECT_EQ(1u, s.Size());
    EXPECT_EQ(1u, s.Capacity()); // 0 capacity edge case. Should grow to 1.
    EXPECT_STREQ("!", s.c_str());
    EXPECT_NE(&c, s.c_str());
}

TEST(StringAssignmentTest, AssignFromCharDoesNotShrinksCapacity) {
    const char c = '!';
    stdads::String<1> s("hello");
    EXPECT_EQ(8u, s.Capacity());

    s = c;

    EXPECT_EQ(1u, s.Size());
    EXPECT_EQ(8u, s.Capacity());
    EXPECT_STREQ("!", s.c_str());
    EXPECT_NE(&c, s.c_str());
}


// String class equality operators
TEST(StringEqualityTest, EqualStringsSameContent) {
    stdads::String a("hello");
    stdads::String b("hello");
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(StringEqualityTest, NotEqualDifferentContent) {
    stdads::String a("hello");
    stdads::String b("world");
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(StringEqualityTest, NotEqualDifferentLength) {
    stdads::String a("hello");
    stdads::String b("hello!");
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(StringEqualityTest, EqualEmptyStrings) {
    stdads::String a;
    stdads::String b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(StringEqualityTest, EmptyAndNonEmptyNotEqual) {
    stdads::String a;
    stdads::String b("hello");
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(StringEqualityTest, SelfEquality) {
    stdads::String a("hello");
    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a != a);
}

TEST(StringEqualityTest, CaseSensitive) {
    stdads::String a("hello");
    stdads::String b("Hello");
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(StringEqualityTest, DifferentCapacities) {
    stdads::String a("hello");
    stdads::String<5> b("hello");
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(b == a);
    EXPECT_FALSE(b != a);

    // grow b
    b = "helloworld";
    EXPECT_EQ(10u, b.Capacity());
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(b == a);
    EXPECT_TRUE(b != a);
}

TEST(StringEqualityTest, EqualCStringsSameContent) {
    stdads::String a("hello");
    const char* b = "hello";
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(b == a);
    EXPECT_FALSE(b != a);

    EXPECT_TRUE("hello" == a);
    EXPECT_FALSE("hello" != a);
}

TEST(StringEqualityTest, CStringNotEqualDifferentContent) {
    stdads::String a("hello");
    const char* b ="world";
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(b == a);
    EXPECT_TRUE(b != a);
}

TEST(StringEqualityTest, CStringNotEqualDifferentLength) {
    stdads::String a("hello");
    const char* b ="hello!";
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(b == a);
    EXPECT_TRUE(b != a);
}

TEST(StringEqualityTest, EqualEmptyCStrings) {
    stdads::String a;
    const char* b = "";
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(b == a);
    EXPECT_FALSE(b != a);
}

TEST(StringEqualityTest, EmptyAndNonEmptyCStringNotEqual) {
    stdads::String a;
    const char* b = "hello";
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(b == a);
    EXPECT_TRUE(b != a);
}

TEST(StringEqualityTest, EqualCharSameContent) {
    stdads::String a('!');
    char b = '!';
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(b == a);
    EXPECT_FALSE(b != a);

    EXPECT_TRUE('!' == a);
    EXPECT_FALSE('!' != a);
}

TEST(StringEqualityTest, CharNotEqualDifferentContent) {
    stdads::String a('a');
    char b = 'b';
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(b == a);
    EXPECT_TRUE(b != a);
}

TEST(StringEqualityTest, EmptyStringAndCharNotEqual) {
    stdads::String a;
    char b = 'b';
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(b == a);
    EXPECT_TRUE(b != a);
}


// String class isEmpty
TEST(StringEmptyTest, IsEmpty) {
    stdads::String emptyStr;
    EXPECT_TRUE(emptyStr.Empty());

    stdads::String nonEmptyStr("Not Empty");
    EXPECT_FALSE(nonEmptyStr.Empty());

    stdads::String<100> largeEmptyStr;
    EXPECT_TRUE(largeEmptyStr.Empty());

    stdads::String<100> largeNonEmptyStr("Not Empty");
    EXPECT_FALSE(largeNonEmptyStr.Empty());
}


// String class addition assignment operators
TEST(StringAppendAssignTest, AppendAnotherString)
{
    stdads::String a("hello");
    stdads::String b(" world");
    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES>& result = (a += b);

    EXPECT_EQ(&a, &result);
    EXPECT_EQ(11u, a.Size());
    EXPECT_STREQ("hello world", a.c_str());
}

TEST(StringAppendAssignTest, AppendEmptyString)
{
    stdads::String a("hello");
    stdads::String empty;
    a += empty;

    EXPECT_EQ(5u, a.Size());
    EXPECT_STREQ("hello", a.c_str());
}

TEST(StringAppendAssignTest, AppendToEmptyString)
{
    stdads::String a;
    stdads::String b("hello");
    a += b;

    EXPECT_EQ(5u, a.Size());
    EXPECT_STREQ("hello", a.c_str());
}

TEST(StringAppendAssignTest, AppendSelf)
{
    stdads::String a("abc");
    a += a;

    EXPECT_EQ(6u, a.Size());
    EXPECT_STREQ("abcabc", a.c_str());
}

TEST(StringAppendAssignTest, AppendDifferentCapacityString)
{
    stdads::String a;
    stdads::String<10> b("hello");
    a += b;

    EXPECT_EQ(5u, a.Size());
    EXPECT_STREQ("hello", a.c_str());
}

TEST(StringAppendAssignTest, AppendCString)
{
    stdads::String s("hello");
    stdads::String<stdads::DEFAULT_STRING_CAPACITY_BYTES>& result = (s += " world");

    EXPECT_EQ(&s, &result);
    EXPECT_EQ(11u, s.Size());
    EXPECT_STREQ("hello world", s.c_str());
}

TEST(StringAppendAssignTest, AppendEmptyCString)
{
    stdads::String s("hello");
    s += "";

    EXPECT_EQ(5u, s.Size());
    EXPECT_STREQ("hello", s.c_str());
}

TEST(StringAppendAssignTest, AppendCStringToEmptyString)
{
    stdads::String s;
    s += "hello";

    EXPECT_EQ(5u, s.Size());
    EXPECT_STREQ("hello", s.c_str());
}


// String class clear
TEST(StringClearTest, ClearNonEmptyString)
{
    stdads::String s("hello");
    s.Clear();

    EXPECT_EQ(0u, s.Size());
    EXPECT_STREQ("", s.c_str());
}

TEST(StringClearTest, ClearEmptyString)
{
    stdads::String s;
    s.Clear();

    EXPECT_EQ(0u, s.Size());
    EXPECT_STREQ("", s.c_str());
}

TEST(StringClearTest, ClearThenAppendString)
{
    stdads::String s("hello");

    s.Clear();
    s += "world";

    EXPECT_EQ(5u, s.Size());
    EXPECT_STREQ("world", s.c_str());
}

TEST(StringClearTest, ClearThenAppendChar)
{
    stdads::String s("hello");
    s.Clear();
    s += 'a';

    EXPECT_EQ(1u, s.Size());
    EXPECT_STREQ("a", s.c_str());
}

TEST(StringClearTest, ClearThenAssignCString)
{
    stdads::String s("hello");
    s.Clear();
    s = "test";

    EXPECT_EQ(4u, s.Size());
    EXPECT_STREQ("test", s.c_str());
}

// String class operator[]
TEST(StringIndexTests, AccessElements)
{
    stdads::String s("hello");
    EXPECT_EQ('h', s[0]);
    EXPECT_EQ('e', s[1]);
    EXPECT_EQ('o', s[4]);
    EXPECT_EQ('o', s[s.Size() - 1]);
}

TEST(StringIndexTests, AccessNullTerminator)
{
    stdads::String s("hello");
    EXPECT_EQ('\0', s[5]);
    EXPECT_EQ('\0', s[s.Size()]);
}

TEST(StringIndexTests, ModifyCharacter)
{
    stdads::String s("hello");
    s[0] = 'H';
    EXPECT_EQ('H', s[0]);
    s[2] = 'X';
    EXPECT_EQ('X', s[2]);
    s[4] = 'Z';
    EXPECT_EQ('Z', s[4]);
}

TEST(StringIndexTests, ConstObjectAccess)
{
    const stdads::String s("hello");
    EXPECT_EQ('e', s[1]);
    EXPECT_EQ('l', s[3]);
    EXPECT_EQ('o', s[4]);
    EXPECT_EQ('\0', s[5]);
}