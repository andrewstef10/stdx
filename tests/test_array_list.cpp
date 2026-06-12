#include <gtest/gtest.h>
#include <stdx/array_list.h>
#include <vector>

TEST(VectorConstructorTest, SizeofVector) {
    EXPECT_EQ(sizeof(std::vector<int>), sizeof(stdx::array_list<int>));
    EXPECT_EQ(sizeof(std::vector<double>), sizeof(stdx::array_list<double>));
    EXPECT_EQ(sizeof(std::vector<std::vector<char>>), sizeof(stdx::array_list<std::vector<char>>));
}

// vector class constructors
TEST(VectorConstructorTest, DefaultConstructorCreatesEmptyVector) {
    stdx::array_list<int> v;
    EXPECT_EQ(0ULL, v.size());
    EXPECT_EQ(0ULL, v.capacity());
}

