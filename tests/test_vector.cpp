#include <gtest/gtest.h>
#include <stdx/vector.h>

// vector class constructors
TEST(VectorConstructorTest, DefaultConstructorCreatesEmptyVector) {
    stdx::vector<int> v;
    EXPECT_EQ(0U, v.size());
    EXPECT_EQ(0U, v.capacity());
}