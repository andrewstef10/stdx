#include <cstddef>
#include <gtest/gtest.h>
#include <stdx/array_list.h>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "support/contiguous_container_test_suite.h"
#include "support/list_test_suite.h"

namespace stdx_test {

// Instantiate general contiguous container test cases for array list.
// Covers indexing and iteration as well as basic size tests.
using ArrayListTypes = ::testing::Types<stdx::ArrayList<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(ArrayListContiguousContainerTests, ContiguousContainerTest, ArrayListTypes);

// Instantiate general list container test cases.
INSTANTIATE_TYPED_TEST_SUITE_P(ArrayListListTests, ListTest, ArrayListTypes);

// ===== Constructor / Assignment Tests =====

TEST(ArrayListConstructorTest, DefaultConstructorEmpty)
{
    stdx::ArrayList<int> v;
    EXPECT_EQ(0u, v.size());
    EXPECT_EQ(0u, v.capacity());
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(nullptr, v.data());
}

TEST(ArrayListConstructorTest, CopyConstructorFromEmpty)
{
    stdx::ArrayList<int> src;
    stdx::ArrayList<int> dst(src);
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(ArrayListConstructorTest, CopyConstructorFromNonEmpty)
{
    stdx::ArrayList<int> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::ArrayList<int> dst(src);
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(ArrayListConstructorTest, CopyConstructorIsDeepCopy)
{
    stdx::ArrayList<int> src;
    src.pushBack(10);
    src.pushBack(20);

    stdx::ArrayList<int> dst(src);
    ASSERT_EQ(2u, dst.size());
    dst[0] = 99;

    EXPECT_EQ(10, src[0]);
    EXPECT_EQ(99, dst[0]);
}

TEST(ArrayListConstructorTest, MoveConstructorFromEmpty)
{
    stdx::ArrayList<int> src;
    stdx::ArrayList<int> dst(std::move(src));
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(ArrayListConstructorTest, MoveConstructorFromNonEmpty)
{
    stdx::ArrayList<int> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::ArrayList<int> dst(std::move(src));
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(ArrayListConstructorTest, MoveConstructorSourceBecomesEmpty)
{
    stdx::ArrayList<int> src;
    src.pushBack(1);
    src.pushBack(2);

    stdx::ArrayList<int> dst(std::move(src));
    EXPECT_TRUE(src.empty());
    EXPECT_EQ(0u, src.size());
    EXPECT_EQ(nullptr, src.data());
}

TEST(ArrayListConstructorTest, CopyAssignmentEmpty)
{
    stdx::ArrayList<int> src;
    stdx::ArrayList<int> dst;
    dst = src;
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(ArrayListConstructorTest, CopyAssignmentNonEmpty)
{
    stdx::ArrayList<int> src;
    src.pushBack(10);
    src.pushBack(20);

    stdx::ArrayList<int> dst;
    dst = src;
    ASSERT_EQ(2u, dst.size());
    EXPECT_EQ(10, dst[0]);
    EXPECT_EQ(20, dst[1]);
}

TEST(ArrayListConstructorTest, CopyAssignmentOverwritesExisting)
{
    stdx::ArrayList<int> src;
    src.pushBack(10);
    src.pushBack(20);

    stdx::ArrayList<int> dst;
    dst.pushBack(99);
    dst.pushBack(88);
    dst.pushBack(77);

    dst = src;
    ASSERT_EQ(2u, dst.size());
    EXPECT_EQ(10, dst[0]);
    EXPECT_EQ(20, dst[1]);
}

TEST(ArrayListConstructorTest, CopyAssignmentIsDeepCopy)
{
    stdx::ArrayList<int> src;
    src.pushBack(5);

    stdx::ArrayList<int> dst;
    dst = src;
    ASSERT_EQ(1u, dst.size());
    dst[0] = 42;

    EXPECT_EQ(5, src[0]);
    EXPECT_EQ(42, dst[0]);
}

TEST(ArrayListConstructorTest, MoveAssignmentEmpty)
{
    stdx::ArrayList<int> src;
    stdx::ArrayList<int> dst;
    dst = std::move(src);
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(ArrayListConstructorTest, MoveAssignmentNonEmpty)
{
    stdx::ArrayList<int> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::ArrayList<int> dst;
    dst = std::move(src);
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(ArrayListConstructorTest, MoveAssignmentSourceBecomesValid)
{
    stdx::ArrayList<int> src;
    src.pushBack(1);
    src.pushBack(2);

    stdx::ArrayList<int> dst;
    dst = std::move(src);

    EXPECT_TRUE(src.empty());
    EXPECT_EQ(0u, src.size());
    EXPECT_EQ(nullptr, src.data());
}

TEST(ArrayListConstructorTest, MoveAssignmentOverwritesExisting)
{
    stdx::ArrayList<int> src;
    src.pushBack(7);

    stdx::ArrayList<int> dst;
    dst.pushBack(1);
    dst.pushBack(2);
    dst = std::move(src);

    ASSERT_EQ(1u, dst.size());
    EXPECT_EQ(7, dst[0]);
}

// ===== Element Access Tests =====

TEST(ArrayListElementAccessTest, AtValidIndex)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    EXPECT_EQ(10, v.at(0));
    EXPECT_EQ(20, v.at(1));
    EXPECT_EQ(30, v.at(2));
}

TEST(ArrayListElementAccessTest, AtConstValidIndex)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_EQ(10, cv.at(0));
    EXPECT_EQ(20, cv.at(1));
}

TEST(ArrayListElementAccessTest, AtReturnsReference)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    v.at(0) = 99;
    EXPECT_EQ(99, v.at(0));
}

TEST(ArrayListElementAccessTest, AtBoundaryLastElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(3, v.at(2));                    // last valid index
    EXPECT_THROW(v.at(3), std::out_of_range); // one past end
}

TEST(ArrayListElementAccessTest, AtOutOfRange)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    EXPECT_THROW(v.at(1), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(ArrayListElementAccessTest, AtConstOutOfRange)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_THROW(cv.at(1), std::out_of_range);
    EXPECT_THROW(cv.at(99), std::out_of_range);
}

TEST(ArrayListElementAccessTest, AtOnEmptyContainerThrows)
{
    stdx::ArrayList<int> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(ArrayListElementAccessTest, AtConstOnEmptyContainerThrows)
{
    const stdx::ArrayList<int> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(ArrayListElementAccessTest, SubscriptOperator)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(ArrayListElementAccessTest, SubscriptOperatorConst)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_EQ(10, cv[0]);
    EXPECT_EQ(20, cv[1]);
}

TEST(ArrayListElementAccessTest, SubscriptOperatorReturnsReference)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    v[0] = 42;
    EXPECT_EQ(42, v[0]);
}

TEST(ArrayListElementAccessTest, DataPointerNullWhenEmpty)
{
    stdx::ArrayList<int> v;
    EXPECT_EQ(nullptr, v.data());
}

TEST(ArrayListElementAccessTest, DataPointerNullWhenEmptyConst)
{
    const stdx::ArrayList<int> v;
    EXPECT_EQ(nullptr, v.data());
}

TEST(ArrayListElementAccessTest, DataPointerNonNull)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    EXPECT_NE(nullptr, v.data());
}

TEST(ArrayListElementAccessTest, DataPointerConsistentWithSubscript)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    EXPECT_EQ(&v[0], v.data());
    EXPECT_EQ(10, v.data()[0]);
    EXPECT_EQ(20, v.data()[1]);
}

TEST(ArrayListElementAccessTest, DataPointerConst)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_NE(nullptr, cv.data());
    EXPECT_EQ(5, *cv.data());
}

TEST(ArrayListElementAccessTest, FrontElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(1, v.front());
}

TEST(ArrayListElementAccessTest, FrontElementConst)
{
    stdx::ArrayList<int> v;
    v.pushBack(42);
    v.pushBack(99);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_EQ(42, cv.front());
}

TEST(ArrayListElementAccessTest, FrontReturnsReference)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    v.front() = 99;
    EXPECT_EQ(99, v.front());
}

TEST(ArrayListElementAccessTest, BackElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(3, v.back());
}

TEST(ArrayListElementAccessTest, BackElementConst)
{
    stdx::ArrayList<int> v;
    v.pushBack(100);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_EQ(100, cv.back());
}

TEST(ArrayListElementAccessTest, BackReturnsReference)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    v.back() = 77;
    EXPECT_EQ(77, v.back());
}

TEST(ArrayListElementAccessTest, FrontAndBackSameWhenSizeOne)
{
    stdx::ArrayList<int> v;
    v.pushBack(7);
    EXPECT_EQ(v.front(), v.back());
    EXPECT_EQ(&v.front(), &v.back());
}

TEST(ArrayListElementAccessTest, FrontAndBackDistinctWhenSizeTwo)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(2, v.back());
    EXPECT_NE(&v.front(), &v.back());
}

// ===== Iterator Tests =====

TEST(ArrayListIteratorTest, BeginEqualsEndWhenEmpty)
{
    stdx::ArrayList<int> v;
    EXPECT_EQ(v.begin(), v.end());
}

TEST(ArrayListIteratorTest, BeginNotEqualsEndWhenNonEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    EXPECT_NE(v.begin(), v.end());
}

TEST(ArrayListIteratorTest, BeginPointsToFirstElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(42);
    v.pushBack(7);
    EXPECT_EQ(42, *v.begin());
}

TEST(ArrayListIteratorTest, EndIsOnePastLast)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    EXPECT_EQ(2, *(v.end() - 1));
}

TEST(ArrayListIteratorTest, ForwardIteration)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);

    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        EXPECT_EQ(expected++, *it);
    }
    EXPECT_EQ(4, expected);
}

TEST(ArrayListIteratorTest, RangeBasedForLoop)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);

    int sum = 0;
    for (int x : v)
    {
        sum += x;
    }
    EXPECT_EQ(60, sum);
}

TEST(ArrayListIteratorTest, ConstBeginEnd)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_NE(cv.begin(), cv.end());
    EXPECT_EQ(5, *cv.begin());
}

TEST(ArrayListIteratorTest, ConstBeginEqualsEndWhenEmpty)
{
    const stdx::ArrayList<int> v;
    EXPECT_EQ(v.begin(), v.end());
}

TEST(ArrayListIteratorTest, CBeginCEnd)
{
    stdx::ArrayList<int> v;
    v.pushBack(3);
    v.pushBack(4);
    EXPECT_EQ(3, *v.cbegin());
    EXPECT_NE(v.cbegin(), v.cend());
}

TEST(ArrayListIteratorTest, CBeginEqualsCBeginConst)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    const stdx::ArrayList<int>& cv = v;
    EXPECT_EQ(v.cbegin(), cv.begin());
    EXPECT_EQ(v.cend(), cv.end());
}

TEST(ArrayListIteratorTest, ReverseIterationEmpty)
{
    stdx::ArrayList<int> v;
    EXPECT_EQ(v.rbegin(), v.rend());
}

TEST(ArrayListIteratorTest, ReverseIteration)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);

    std::vector<int> result;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
    {
        result.push_back(*it);
    }
    ASSERT_EQ(3u, result.size());
    EXPECT_EQ(3, result[0]);
    EXPECT_EQ(2, result[1]);
    EXPECT_EQ(1, result[2]);
}

TEST(ArrayListIteratorTest, ConstReverseIteration)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    const stdx::ArrayList<int>& cv = v;

    auto it = cv.rbegin();
    EXPECT_EQ(20, *it);
    ++it;
    EXPECT_EQ(10, *it);
    ++it;
    EXPECT_EQ(cv.rend(), it);
}

TEST(ArrayListIteratorTest, CRBeginCREnd)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);

    std::vector<int> result;
    for (auto it = v.crbegin(); it != v.crend(); ++it)
    {
        result.push_back(*it);
    }
    ASSERT_EQ(2u, result.size());
    EXPECT_EQ(2, result[0]);
    EXPECT_EQ(1, result[1]);
}

TEST(ArrayListIteratorTest, MutableIteratorAllowsWrite)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);

    for (auto it = v.begin(); it != v.end(); ++it)
    {
        *it *= 2;
    }
    EXPECT_EQ(2, v[0]);
    EXPECT_EQ(4, v[1]);
    EXPECT_EQ(6, v[2]);
}

// ===== Capacity Tests =====

TEST(ArrayListCapacityTest, EmptyOnDefault)
{
    stdx::ArrayList<int> v;
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListCapacityTest, NotEmptyAfterPushBack)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    EXPECT_FALSE(v.empty());
}

TEST(ArrayListCapacityTest, EmptyAfterClear)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListCapacityTest, SizeDefault)
{
    stdx::ArrayList<int> v;
    EXPECT_EQ(0u, v.size());
}

TEST(ArrayListCapacityTest, SizeAfterPushBack)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    EXPECT_EQ(1u, v.size());
    v.pushBack(2);
    EXPECT_EQ(2u, v.size());
    v.pushBack(3);
    EXPECT_EQ(3u, v.size());
}

TEST(ArrayListCapacityTest, SizeAfterPopBack)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();
    EXPECT_EQ(1u, v.size());
    v.popBack();
    EXPECT_EQ(0u, v.size());
}

TEST(ArrayListCapacityTest, CapacityDefault)
{
    stdx::ArrayList<int> v;
    EXPECT_EQ(0u, v.capacity());
}

TEST(ArrayListCapacityTest, CapacityAtLeastSize)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_GE(v.capacity(), v.size());
}

TEST(ArrayListCapacityTest, ReserveIncreasesCapacity)
{
    stdx::ArrayList<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10u);
}

TEST(ArrayListCapacityTest, ReserveDoesNotChangeSize)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.reserve(100);
    EXPECT_EQ(2u, v.size());
}

TEST(ArrayListCapacityTest, ReserveNoopWhenSufficient)
{
    stdx::ArrayList<int> v;
    v.reserve(10);
    std::size_t cap = v.capacity();
    v.reserve(5);
    EXPECT_EQ(cap, v.capacity());
}

TEST(ArrayListCapacityTest, ReserveZeroOnEmptyIsNoop)
{
    stdx::ArrayList<int> v;
    v.reserve(0);
    EXPECT_EQ(0u, v.size());
    EXPECT_EQ(0u, v.capacity());
}

TEST(ArrayListCapacityTest, ReservePreservesElements)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.reserve(100);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(ArrayListCapacityTest, ShrinkToFitReducesCapacityToSize)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3); // capacity doubles to 4 with DoublingGrowth
    ASSERT_GE(v.capacity(), v.size());
    v.shrinkToFit();
    EXPECT_EQ(v.size(), v.capacity());
}

TEST(ArrayListCapacityTest, ShrinkToFitPreservesElements)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    v.shrinkToFit();
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
    EXPECT_EQ(30, v[2]);
}

TEST(ArrayListCapacityTest, ShrinkToFitOnAlreadyFitContainer)
{
    stdx::ArrayList<int> v; // exact_growth: capacity always equals size
    v.pushBack(1);
    v.pushBack(2);
    ASSERT_EQ(v.size(), v.capacity());
    v.shrinkToFit();
    EXPECT_EQ(v.size(), v.capacity());
}

TEST(ArrayListCapacityTest, ShrinkToFitOnEmpty)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> v;
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();
    v.popBack();
    EXPECT_EQ(0u, v.size());
    v.shrinkToFit();
    EXPECT_EQ(0u, v.capacity());
}

// ===== Modifier Tests =====

TEST(ArrayListModifierTest, PushBackCopyIncreasesSize)
{
    stdx::ArrayList<int> v;
    int val = 42;
    v.pushBack(val);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, val); // source unchanged
}

TEST(ArrayListModifierTest, PushBackCopyMultiple)
{
    stdx::ArrayList<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.pushBack(i);
    }
    ASSERT_EQ(10u, v.size());
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(i, v[i]);
    }
}

TEST(ArrayListModifierTest, PushBackMove)
{
    stdx::ArrayList<std::string> v;
    std::string s = "hello";
    v.pushBack(std::move(s));
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ("hello", v[0]);
}

TEST(ArrayListModifierTest, PushBackMoveSourceEmptied)
{
    stdx::ArrayList<std::string> v;
    std::string s = "hello";
    v.pushBack(std::move(s));
    EXPECT_TRUE(s.empty());
}

TEST(ArrayListModifierTest, PushBackTriggersGrow)
{
    // Verifies elements survive a grow reallocation
    stdx::ArrayList<int> v;
    for (int i = 0; i < 5; ++i)
    {
        v.pushBack(i * 10);
    }
    ASSERT_EQ(5u, v.size());
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(i * 10, v[i]);
    }
}

TEST(ArrayListModifierTest, PopBackReducesSize)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();
    EXPECT_EQ(1u, v.size());
}

TEST(ArrayListModifierTest, PopBackRemovesLastElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.popBack();
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(ArrayListModifierTest, PopBackToEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(42);
    v.popBack();
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListModifierTest, InsertAtBeginCopy)
{
    stdx::ArrayList<int> v;
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.insert(v.begin(), 1);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(1, *it);
    EXPECT_EQ(v.begin(), it);
}

TEST(ArrayListModifierTest, InsertAtEndCopy)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    auto it = v.insert(v.end(), 3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(3, *it);
}

TEST(ArrayListModifierTest, InsertAtMiddleCopy)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(3);
    auto it = v.insert(v.begin() + 1, 2);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(2, *it);
}

TEST(ArrayListModifierTest, InsertIntoEmptyCopy)
{
    stdx::ArrayList<int> v;
    auto it = v.insert(v.end(), 42);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, *it);
}

TEST(ArrayListModifierTest, InsertAtBeginMove)
{
    stdx::ArrayList<std::string> v;
    v.pushBack("world");
    std::string s = "hello";
    auto it = v.insert(v.begin(), std::move(s));
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("world", v[1]);
    EXPECT_EQ("hello", *it);
}

TEST(ArrayListModifierTest, InsertAtEndMove)
{
    stdx::ArrayList<std::string> v;
    v.pushBack("hello");
    std::string s = "world";
    auto it = v.insert(v.end(), std::move(s));
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("world", v[1]);
    EXPECT_EQ("world", *it);
}

TEST(ArrayListModifierTest, InsertAtMiddleMove)
{
    stdx::ArrayList<std::string> v;
    v.pushBack("a");
    v.pushBack("c");
    std::string s = "b";
    auto it = v.insert(v.begin() + 1, std::move(s));
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ("a", v[0]);
    EXPECT_EQ("b", v[1]);
    EXPECT_EQ("c", v[2]);
    EXPECT_EQ("b", *it);
}

TEST(ArrayListModifierTest, InsertSelfReferenceWithSpareCapacity)
{
    stdx::ArrayList<int> v;
    v.reserve(8); // ensure the in-place (non-reallocating) path is taken
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    // value aliases an element already in the container
    auto it = v.insert(v.begin(), v[2]);
    ASSERT_EQ(4u, v.size());
    EXPECT_EQ(30, v[0]); // the original v[2] value, not a shifted-over one
    EXPECT_EQ(10, v[1]);
    EXPECT_EQ(20, v[2]);
    EXPECT_EQ(30, v[3]);
    EXPECT_EQ(30, *it);

    it = v.insert(v.begin(), std::move(v[2]));
    ASSERT_EQ(5u, v.size());
    EXPECT_EQ(20, v[0]);
    EXPECT_EQ(30, v[1]); // the original v[2] value, not a shifted-over one
    EXPECT_EQ(10, v[2]);
    EXPECT_EQ(20, v[3]);
    EXPECT_EQ(30, v[4]);
    EXPECT_EQ(20, *it);
}

TEST(ArrayListModifierTest, EraseFirst)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.begin());
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(2, v[0]);
    EXPECT_EQ(3, v[1]);
    EXPECT_EQ(v.begin(), it);
    EXPECT_EQ(2, *it);
}

TEST(ArrayListModifierTest, EraseLast)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.end() - 1);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(v.end(), it);
}

TEST(ArrayListModifierTest, EraseMiddle)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.begin() + 1);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(3, v[1]);
    EXPECT_EQ(3, *it);
}

TEST(ArrayListModifierTest, EraseOnlyElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(42);
    auto it = v.erase(v.begin());
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.end(), it);
}

TEST(ArrayListModifierTest, EraseConstIteratorOverload)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    stdx::ArrayList<int>::const_iterator pos = v.cbegin() + 1;
    auto it = v.erase(pos);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(3, v[1]);
    EXPECT_EQ(3, *it);
}

TEST(ArrayListModifierTest, EraseConstIteratorFirst)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    auto it = v.erase(v.cbegin());
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(20, v[0]);
    EXPECT_EQ(v.begin(), it);
}

TEST(ArrayListModifierTest, EraseRangeEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    // Empty range [begin+1, begin+1)
    auto it = v.erase(v.begin() + 1, v.begin() + 1);
    EXPECT_EQ(2u, v.size());
    EXPECT_EQ(v.begin() + 1, it);
}

TEST(ArrayListModifierTest, EraseRangeAll)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.begin(), v.end());
    EXPECT_EQ(0u, v.size());
    EXPECT_EQ(v.end(), it);
}

TEST(ArrayListModifierTest, EraseRangePartial)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    v.pushBack(5);
    auto it = v.erase(v.begin() + 1, v.begin() + 3); // removes 2 and 3
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(4, v[1]);
    EXPECT_EQ(5, v[2]);
    EXPECT_EQ(4, *it);
}

TEST(ArrayListModifierTest, EraseRangeFromBegin)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    auto it = v.erase(v.begin(), v.begin() + 2); // removes 1 and 2
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(3, v[0]);
    EXPECT_EQ(4, v[1]);
    EXPECT_EQ(v.begin(), it);
    EXPECT_EQ(3, *it);
}

TEST(ArrayListModifierTest, EraseRangeToEnd)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    auto it = v.erase(v.begin() + 2, v.end()); // removes 3 and 4
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(v.end(), it);
}

TEST(ArrayListModifierTest, EraseRangeConstIteratorOverload)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    stdx::ArrayList<int>::const_iterator first = v.cbegin();
    stdx::ArrayList<int>::const_iterator last = v.cbegin() + 2;
    auto it = v.erase(first, last);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(30, v[0]);
    EXPECT_EQ(v.begin(), it);
}

TEST(ArrayListModifierTest, EraseRangeConstIteratorEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    auto it = v.erase(v.cbegin() + 1, v.cbegin() + 1);
    EXPECT_EQ(2u, v.size());
    EXPECT_EQ(v.begin() + 1, it);
}

TEST(ArrayListModifierTest, ClearEmptyContainer)
{
    stdx::ArrayList<int> v;
    v.clear();
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListModifierTest, ClearNonEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.clear();
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListModifierTest, ClearPreservesCapacity)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    std::size_t cap = v.capacity();
    v.clear();
    EXPECT_EQ(cap, v.capacity());
}

TEST(ArrayListModifierTest, ClearThenPushBack)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.clear();
    v.pushBack(99);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(99, v[0]);
}

TEST(ArrayListModifierTest, SwapTwoNonEmpty)
{
    stdx::ArrayList<int> a;
    a.pushBack(1);
    a.pushBack(2);

    stdx::ArrayList<int> b;
    b.pushBack(10);
    b.pushBack(20);
    b.pushBack(30);

    a.swap(b);

    ASSERT_EQ(3u, a.size());
    EXPECT_EQ(10, a[0]);
    EXPECT_EQ(20, a[1]);
    EXPECT_EQ(30, a[2]);

    ASSERT_EQ(2u, b.size());
    EXPECT_EQ(1, b[0]);
    EXPECT_EQ(2, b[1]);
}

TEST(ArrayListModifierTest, SwapWithEmpty)
{
    stdx::ArrayList<int> a;
    a.pushBack(1);
    a.pushBack(2);

    stdx::ArrayList<int> b;

    a.swap(b);

    EXPECT_TRUE(a.empty());
    ASSERT_EQ(2u, b.size());
    EXPECT_EQ(1, b[0]);
    EXPECT_EQ(2, b[1]);
}

TEST(ArrayListModifierTest, SwapTwoEmptyContainers)
{
    stdx::ArrayList<int> a;
    stdx::ArrayList<int> b;
    a.swap(b);
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
}

TEST(ArrayListModifierTest, SwapPreservesCapacity)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> a;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3); // cap=4

    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> b;
    b.pushBack(10); // cap=1

    std::size_t capA = a.capacity();
    std::size_t capB = b.capacity();

    a.swap(b);

    EXPECT_EQ(capB, a.capacity());
    EXPECT_EQ(capA, b.capacity());
}

TEST(ArrayListModifierTest, SwapBothContainersUsableAfterSwap)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    b.pushBack(2);
    a.swap(b);
    a.pushBack(3);
    b.pushBack(4);
    ASSERT_EQ(2u, a.size());
    ASSERT_EQ(2u, b.size());
    EXPECT_EQ(2, a[0]);
    EXPECT_EQ(3, a[1]);
    EXPECT_EQ(1, b[0]);
    EXPECT_EQ(4, b[1]);
}

// ===== Comparison Tests =====

TEST(ArrayListComparisonTest, EqualsTwoEmpty)
{
    stdx::ArrayList<int> a, b;
    EXPECT_TRUE(a.equals(b));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(ArrayListComparisonTest, EqualsSameElements)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(3);
    EXPECT_TRUE(a.equals(b));
    EXPECT_TRUE(a == b);
}

TEST(ArrayListComparisonTest, EqualsNotEqualElements)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(3);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(ArrayListComparisonTest, EqualsNotEqualSizesASmaller)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(3);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(a == b);
}

TEST(ArrayListComparisonTest, EqualsNotEqualSizesALarger)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    b.pushBack(1);
    b.pushBack(2);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(a == b);
}

TEST(ArrayListComparisonTest, EqualsEmptyVsNonEmpty)
{
    stdx::ArrayList<int> a, b;
    b.pushBack(1);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(b.equals(a));
}

TEST(ArrayListComparisonTest, EqualsSingleElement)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(5);
    b.pushBack(5);
    EXPECT_TRUE(a.equals(b));

    stdx::ArrayList<int> c;
    c.pushBack(6);
    EXPECT_FALSE(a.equals(c));
}

TEST(ArrayListComparisonTest, LessThanLexicographic)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(4);
    EXPECT_TRUE(a.lessThan(b));
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(b.lessThan(a));
}

TEST(ArrayListComparisonTest, LessThanBySize)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(3);
    EXPECT_TRUE(a.lessThan(b));
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(ArrayListComparisonTest, LessThanEqualIsNotLess)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(2);
    EXPECT_FALSE(a.lessThan(b));
    EXPECT_FALSE(a < b);
}

TEST(ArrayListComparisonTest, LessThanBothEmpty)
{
    stdx::ArrayList<int> a, b;
    EXPECT_FALSE(a.lessThan(b));
    EXPECT_FALSE(a < b);
}

TEST(ArrayListComparisonTest, LessThanFirstElementDecides)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    b.pushBack(2);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(ArrayListComparisonTest, GreaterThan)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(5);
    b.pushBack(3);
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(ArrayListComparisonTest, GreaterThanEqualNotGreater)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    b.pushBack(1);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(ArrayListComparisonTest, LessOrEqualWhenEqual)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    b.pushBack(1);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b <= a);
}

TEST(ArrayListComparisonTest, LessOrEqualWhenLess)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    b.pushBack(2);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
}

TEST(ArrayListComparisonTest, GreaterOrEqualWhenEqual)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(1);
    b.pushBack(1);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b >= a);
}

TEST(ArrayListComparisonTest, GreaterOrEqualWhenGreater)
{
    stdx::ArrayList<int> a, b;
    a.pushBack(3);
    b.pushBack(2);
    EXPECT_TRUE(a >= b);
    EXPECT_FALSE(b >= a);
}

TEST(ArrayListComparisonTest, AllOperatorsEmptyContainers)
{
    stdx::ArrayList<int> a, b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a >= b);
}

// ===== Growth Policy Tests =====

TEST(ArrayListGrowthPolicyTest, ExactGrowthCapacityEqualsSize)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::ExactGrowth<>> v;
    v.pushBack(1);
    EXPECT_EQ(1u, v.capacity());
    v.pushBack(2);
    EXPECT_EQ(2u, v.capacity());
    v.pushBack(3);
    EXPECT_EQ(3u, v.capacity());
}

TEST(ArrayListGrowthPolicyTest, DoublingGrowthCapacitySequence)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> v;
    v.pushBack(1); // req=1, cap: 0->1 = 1
    EXPECT_EQ(1u, v.capacity());
    v.pushBack(2); // req=2, cap: 1->2 = 2
    EXPECT_EQ(2u, v.capacity());
    v.pushBack(3); // req=3, cap: 2->4 = 4
    EXPECT_EQ(4u, v.capacity());
    v.pushBack(4); // req=4, fits in 4
    EXPECT_EQ(4u, v.capacity());
    v.pushBack(5); // req=5, cap: 4->8 = 8
    EXPECT_EQ(8u, v.capacity());
}

TEST(ArrayListGrowthPolicyTest, DoublingGrowthPreservesElements)
{
    stdx::ArrayList<int, stdx::Allocator<int>, stdx::DoublingGrowth<>> v;
    for (int i = 0; i < 8; ++i)
    {
        v.pushBack(i * 3);
    }
    ASSERT_EQ(8u, v.size());
    for (int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(i * 3, v[i]);
    }
}

// ===== emplaceBack / emplace Tests =====

namespace {

struct Point
{
    int x, y;
    Point(int x, int y)
        : x(x)
        , y(y)
    {
    }
};

struct MoveOnly
{
    int value;
    explicit MoveOnly(int v)
        : value(v)
    {
    }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
};

struct ConstructionCounter
{
    static int direct_constructions;
    static int copy_constructions;
    static int move_constructions;

    int value;

    explicit ConstructionCounter(int v)
        : value(v)
    {
        ++direct_constructions;
    }
    ConstructionCounter(const ConstructionCounter& o)
        : value(o.value)
    {
        ++copy_constructions;
    }
    ConstructionCounter(ConstructionCounter&& o) noexcept
        : value(o.value)
    {
        ++move_constructions;
    }
    ConstructionCounter& operator=(const ConstructionCounter&) = default;
    ConstructionCounter& operator=(ConstructionCounter&&) = default;
};

int ConstructionCounter::direct_constructions = 0;
int ConstructionCounter::copy_constructions = 0;
int ConstructionCounter::move_constructions = 0;

} // namespace

TEST(ArrayListModifierTest, EmplaceBackIntoEmpty)
{
    stdx::ArrayList<int> v;
    v.emplaceBack(42);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
}

TEST(ArrayListModifierTest, EmplaceBackIncreasesSize)
{
    stdx::ArrayList<int> v;
    v.emplaceBack(1);
    v.emplaceBack(2);
    v.emplaceBack(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(ArrayListModifierTest, EmplaceBackWithMultipleArgs)
{
    stdx::ArrayList<Point> v;
    v.emplaceBack(3, 7);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(3, v[0].x);
    EXPECT_EQ(7, v[0].y);
}

TEST(ArrayListModifierTest, EmplaceBackConstructsInPlace)
{
    stdx::ArrayList<MoveOnly> v;
    v.emplaceBack(99);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(99, v[0].value);
}

TEST(ArrayListModifierTest, EmplaceBackNoCopyOrMove)
{
    ConstructionCounter::direct_constructions = 0;
    ConstructionCounter::copy_constructions = 0;
    ConstructionCounter::move_constructions = 0;

    stdx::ArrayList<ConstructionCounter> v;
    v.reserve(1);
    v.emplaceBack(5);

    EXPECT_EQ(1, ConstructionCounter::direct_constructions);
    EXPECT_EQ(0, ConstructionCounter::copy_constructions);
    EXPECT_EQ(0, ConstructionCounter::move_constructions);
}

TEST(ArrayListModifierTest, EmplaceBackWithStringArgs)
{
    stdx::ArrayList<std::string> v;
    v.emplaceBack(3, 'z');
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ("zzz", v[0]);
}

TEST(ArrayListModifierTest, EmplaceBackTriggersGrow)
{
    stdx::ArrayList<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.emplaceBack(i);
    }
    ASSERT_EQ(10u, v.size());
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(i, v[i]);
    }
}

TEST(ArrayListModifierTest, EmplaceIntoEmpty)
{
    stdx::ArrayList<int> v;
    auto it = v.emplace(v.end(), 42);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, *it);
}

TEST(ArrayListModifierTest, EmplaceAtBegin)
{
    stdx::ArrayList<int> v;
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.emplace(v.begin(), 1);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(1, *it);
    EXPECT_EQ(v.begin(), it);
}

TEST(ArrayListModifierTest, EmplaceAtEnd)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    auto it = v.emplace(v.end(), 3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(3, *it);
}

TEST(ArrayListModifierTest, EmplaceAtMiddle)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(3);
    auto it = v.emplace(v.begin() + 1, 2);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(2, *it);
}

TEST(ArrayListModifierTest, EmplaceWithMultipleArgs)
{
    stdx::ArrayList<Point> v;
    v.emplaceBack(1, 2);
    auto it = v.emplace(v.begin(), 10, 20);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(10, v[0].x);
    EXPECT_EQ(20, v[0].y);
    EXPECT_EQ(1, v[1].x);
    EXPECT_EQ(2, v[1].y);
    EXPECT_EQ(10, it->x);
    EXPECT_EQ(20, it->y);
}

TEST(ArrayListModifierTest, EmplaceConstructsInPlace)
{
    stdx::ArrayList<MoveOnly> v;
    v.emplaceBack(1);
    auto it = v.emplace(v.begin(), 99);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(99, v[0].value);
    EXPECT_EQ(1, v[1].value);
    EXPECT_EQ(99, it->value);
}

TEST(ArrayListModifierTest, EmplaceNoCopyOrMoveIntoReservedSlot)
{
    ConstructionCounter::direct_constructions = 0;
    ConstructionCounter::copy_constructions = 0;
    ConstructionCounter::move_constructions = 0;

    stdx::ArrayList<ConstructionCounter> v;
    v.reserve(1);
    v.emplace(v.end(), 7);

    EXPECT_EQ(1, ConstructionCounter::direct_constructions);
    EXPECT_EQ(0, ConstructionCounter::copy_constructions);
    EXPECT_EQ(0, ConstructionCounter::move_constructions);
}

// ===== Member Types =====

TEST(ArrayListMemberTypesTest, ValueType)
{
    static_assert(std::is_same<stdx::ArrayList<int>::value_type, int>::value, "");
    static_assert(std::is_same<stdx::ArrayList<std::string>::value_type, std::string>::value, "");
}

TEST(ArrayListMemberTypesTest, SizeType)
{
    static_assert(std::is_same<stdx::ArrayList<int>::size_type, std::size_t>::value, "");
}

TEST(ArrayListMemberTypesTest, DifferenceType)
{
    static_assert(std::is_same<stdx::ArrayList<int>::difference_type, std::ptrdiff_t>::value, "");
}

TEST(ArrayListMemberTypesTest, Reference)
{
    static_assert(std::is_same<stdx::ArrayList<int>::reference, int&>::value, "");
    static_assert(std::is_same<stdx::ArrayList<double>::reference, double&>::value, "");
}

TEST(ArrayListMemberTypesTest, ConstReference)
{
    static_assert(std::is_same<stdx::ArrayList<int>::const_reference, const int&>::value, "");
    static_assert(std::is_same<stdx::ArrayList<double>::const_reference, const double&>::value, "");
}

TEST(ArrayListMemberTypesTest, AllocatorType)
{
    static_assert(std::is_same<stdx::ArrayList<int>::allocator_type, stdx::Allocator<int>>::value, "");
}

TEST(ArrayListMemberTypesTest, Pointer)
{
    static_assert(std::is_same<stdx::ArrayList<int>::pointer, int*>::value, "");
}

TEST(ArrayListMemberTypesTest, ConstPointer)
{
    static_assert(std::is_same<stdx::ArrayList<int>::const_pointer, const int*>::value, "");
}

TEST(ArrayListMemberTypesTest, Iterator)
{
    static_assert(std::is_same<stdx::ArrayList<int>::iterator, int*>::value, "");
}

TEST(ArrayListMemberTypesTest, ConstIterator)
{
    static_assert(std::is_same<stdx::ArrayList<int>::const_iterator, const int*>::value, "");
}

TEST(ArrayListMemberTypesTest, ReverseIterator)
{
    using expected = stdx::ReverseIterator<int*>;
    static_assert(std::is_same<stdx::ArrayList<int>::reverse_iterator, expected>::value, "");
}

TEST(ArrayListMemberTypesTest, ConstReverseIterator)
{
    using expected = stdx::ReverseIterator<const int*>;
    static_assert(std::is_same<stdx::ArrayList<int>::const_reverse_iterator, expected>::value, "");
}

TEST(ArrayListMemberTypesTest, ReferenceIsActuallyMutable)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    stdx::ArrayList<int>::reference ref = v[0];
    ref = 42;
    EXPECT_EQ(42, v[0]);
}

TEST(ArrayListMemberTypesTest, ConstReferenceFromConstContainer)
{
    stdx::ArrayList<int> v;
    v.pushBack(7);
    const stdx::ArrayList<int>& cv = v;
    stdx::ArrayList<int>::const_reference cref = cv[0];
    EXPECT_EQ(7, cref);
}

TEST(ArrayListMemberTypesTest, PointerPointsToElement)
{
    stdx::ArrayList<int> v;
    v.pushBack(5);
    stdx::ArrayList<int>::pointer p = v.data();
    ASSERT_NE(nullptr, p);
    EXPECT_EQ(5, *p);
}

TEST(ArrayListMemberTypesTest, ConstPointerFromConstData)
{
    stdx::ArrayList<int> v;
    v.pushBack(9);
    const stdx::ArrayList<int>& cv = v;
    stdx::ArrayList<int>::const_pointer p = cv.data();
    ASSERT_NE(nullptr, p);
    EXPECT_EQ(9, *p);
}

// ===== max_size =====

TEST(ArrayListCapacityTest, MaxSizeIsMaxSizeT)
{
    stdx::ArrayList<int> v;
    std::vector<int> vec;
    EXPECT_EQ(vec.max_size(), v.maxSize());
}

TEST(ArrayListCapacityTest, MaxSizeIsConstant)
{
    stdx::ArrayList<int> empty;
    stdx::ArrayList<int> nonempty;
    nonempty.pushBack(1);
    nonempty.pushBack(2);
    EXPECT_EQ(empty.maxSize(), nonempty.maxSize());
}

TEST(ArrayListCapacityTest, MaxSizeGreaterThanSize)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    EXPECT_GT(v.maxSize(), v.size());
}

TEST(ArrayListCapacityTest, MaxSizeGreaterThanCapacity)
{
    stdx::ArrayList<int> v;
    v.reserve(1000);
    EXPECT_GT(v.maxSize(), v.capacity());
}

// ===== Resize Tests =====

namespace {

struct DestructorCounter
{
    static int destructions;
    int value;
    DestructorCounter()
        : value(0)
    {
    }
    explicit DestructorCounter(int v)
        : value(v)
    {
    }
    ~DestructorCounter()
    {
        ++destructions;
    }
    DestructorCounter(const DestructorCounter&) = default;
    DestructorCounter& operator=(const DestructorCounter&) = default;
    DestructorCounter(DestructorCounter&&) noexcept = default;
    DestructorCounter& operator=(DestructorCounter&&) noexcept = default;
};
int DestructorCounter::destructions = 0;

} // namespace

// --- resize(count) ---

TEST(ArrayListResizeTest, ResizeSameSizeIsNoop)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    std::size_t capBefore = v.capacity();
    v.resize(3);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(capBefore, v.capacity());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(ArrayListResizeTest, ResizeToZeroFromEmpty)
{
    stdx::ArrayList<int> v;
    v.resize(0);
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListResizeTest, ResizeToZeroFromNonEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.resize(0);
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListResizeTest, ResizeLargerFromEmpty)
{
    stdx::ArrayList<int> v;
    v.resize(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(0, v[1]);
    EXPECT_EQ(0, v[2]);
}

TEST(ArrayListResizeTest, ResizeLargerPreservesExistingElements)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    v.resize(5);
    ASSERT_EQ(5u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
    EXPECT_EQ(30, v[2]);
}

TEST(ArrayListResizeTest, ResizeLargerDefaultInitializesNewElements)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.resize(5);
    ASSERT_EQ(5u, v.size());
    EXPECT_EQ(0, v[2]);
    EXPECT_EQ(0, v[3]);
    EXPECT_EQ(0, v[4]);
}

TEST(ArrayListResizeTest, ResizeLargerCapacityAtLeastCount)
{
    stdx::ArrayList<int> v;
    v.resize(10);
    EXPECT_GE(v.capacity(), 10u);
}

TEST(ArrayListResizeTest, ResizeLargerWhenCapacityAlreadySufficient)
{
    stdx::ArrayList<int> v;
    v.reserve(10);
    v.pushBack(1);
    v.pushBack(2);
    std::size_t capBefore = v.capacity();
    v.resize(5);
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(capBefore, v.capacity());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(0, v[2]);
    EXPECT_EQ(0, v[3]);
    EXPECT_EQ(0, v[4]);
}

TEST(ArrayListResizeTest, ResizeLargerToExactCapacityDoesNotReallocate)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2); // size=2, capacity=2
    v.popBack();   // size=1, capacity=2
    const int* dataBefore = v.data();
    v.resize(2); // count == capacity: must NOT reallocate
    EXPECT_EQ(dataBefore, v.data());
    EXPECT_EQ(2u, v.size());
}

TEST(ArrayListResizeTest, ResizeSmallerReducesSize)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    v.pushBack(5);
    v.resize(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(ArrayListResizeTest, ResizeSmallerPreservesCapacity)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    std::size_t capBefore = v.capacity();
    v.resize(1);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(capBefore, v.capacity());
}

TEST(ArrayListResizeTest, ResizeSmallerDestroysRemovedElements)
{
    DestructorCounter::destructions = 0;
    {
        stdx::ArrayList<DestructorCounter> v;
        v.reserve(3);
        v.emplaceBack(1);
        v.emplaceBack(2);
        v.emplaceBack(3);
        DestructorCounter::destructions = 0;
        v.resize(1);
        EXPECT_EQ(2, DestructorCounter::destructions);
        EXPECT_EQ(1u, v.size());
        EXPECT_EQ(1, v[0].value);
    }
}

TEST(ArrayListResizeTest, ResizeLargerWithNonTrivialTypeDefaultInserted)
{
    stdx::ArrayList<std::string> v;
    v.pushBack("hello");
    v.resize(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("", v[1]);
    EXPECT_EQ("", v[2]);
}

TEST(ArrayListResizeTest, ResizeSmallerWithNonTrivialType)
{
    stdx::ArrayList<std::string> v;
    v.pushBack("a");
    v.pushBack("b");
    v.pushBack("c");
    v.resize(1);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ("a", v[0]);
}

TEST(ArrayListResizeTest, ResizeThenPushBackWorks)
{
    stdx::ArrayList<int> v;
    v.resize(3);
    v.pushBack(99);
    ASSERT_EQ(4u, v.size());
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(0, v[1]);
    EXPECT_EQ(0, v[2]);
    EXPECT_EQ(99, v[3]);
}

TEST(ArrayListResizeTest, ResizeSmallerThenLargerDefaultInitializesNewElements)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    v.resize(1);
    v.resize(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(0, v[1]);
    EXPECT_EQ(0, v[2]);
}

// --- resize(count, value) ---

TEST(ArrayListResizeTest, ResizeWithValueSameSizeIsNoop)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    std::size_t capBefore = v.capacity();
    v.resize(3, 99);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(capBefore, v.capacity());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(ArrayListResizeTest, ResizeWithValueToZeroFromNonEmpty)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.resize(0, 99);
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(ArrayListResizeTest, ResizeWithValueLargerFromEmpty)
{
    stdx::ArrayList<int> v;
    v.resize(3, 42);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, v[1]);
    EXPECT_EQ(42, v[2]);
}

TEST(ArrayListResizeTest, ResizeWithValueLargerPreservesExistingElements)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.resize(5, 99);
    ASSERT_EQ(5u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
    EXPECT_EQ(99, v[2]);
    EXPECT_EQ(99, v[3]);
    EXPECT_EQ(99, v[4]);
}

TEST(ArrayListResizeTest, ResizeWithValueSmallerReducesSize)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    v.resize(2, 99);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(ArrayListResizeTest, ResizeWithValueSmallerPreservesCapacity)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    std::size_t capBefore = v.capacity();
    v.resize(1, 99);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(capBefore, v.capacity());
}

TEST(ArrayListResizeTest, ResizeWithValueLargerCapacityAtLeastCount)
{
    stdx::ArrayList<int> v;
    v.resize(10, 5);
    EXPECT_GE(v.capacity(), 10u);
}

TEST(ArrayListResizeTest, ResizeWithValueLargerWhenCapacityAlreadySufficient)
{
    stdx::ArrayList<int> v;
    v.reserve(10);
    v.pushBack(1);
    std::size_t capBefore = v.capacity();
    v.resize(4, 7);
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(capBefore, v.capacity());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(7, v[1]);
    EXPECT_EQ(7, v[2]);
    EXPECT_EQ(7, v[3]);
}

TEST(ArrayListResizeTest, ResizeWithValueLargerToExactCapacityDoesNotReallocate)
{
    stdx::ArrayList<int> v;
    v.pushBack(1);
    v.pushBack(2); // size=2, capacity=2
    v.popBack();   // size=1, capacity=2
    const int* dataBefore = v.data();
    v.resize(2, 99); // count == capacity: must NOT reallocate
    EXPECT_EQ(dataBefore, v.data());
    EXPECT_EQ(2u, v.size());
    EXPECT_EQ(99, v[1]);
}

TEST(ArrayListResizeTest, ResizeWithValueSmallerDestroysRemovedElements)
{
    DestructorCounter::destructions = 0;
    {
        stdx::ArrayList<DestructorCounter> v;
        v.reserve(3);
        v.emplaceBack(1);
        v.emplaceBack(2);
        v.emplaceBack(3);
        DestructorCounter filler{0};
        DestructorCounter::destructions = 0;
        v.resize(1, filler);
        EXPECT_EQ(2, DestructorCounter::destructions);
        EXPECT_EQ(1u, v.size());
        EXPECT_EQ(1, v[0].value);
    }
}

TEST(ArrayListResizeTest, ResizeWithValueWithString)
{
    stdx::ArrayList<std::string> v;
    v.pushBack("hello");
    v.resize(4, "world");
    ASSERT_EQ(4u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("world", v[1]);
    EXPECT_EQ("world", v[2]);
    EXPECT_EQ("world", v[3]);
}

TEST(ArrayListResizeTest, ResizeWithValueSmallerThenLargerUsesValue)
{
    stdx::ArrayList<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    v.resize(1, 0);
    v.resize(3, 55);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(55, v[1]);
    EXPECT_EQ(55, v[2]);
}

// ===== Move Assignment Leak Tests =====

namespace {

/// Stateful allocator for exercising the element-wise move assignment path. Each default-constructed
/// instance gets a unique id, so two default-constructed containers compare unequal and (with POCMA
/// false) move assignment cannot steal the buffer. Tracks outstanding allocations across all
/// instances of an instantiation to expose leaked buffers.
template <typename T, bool POCMA>
struct TrackingAllocator
{
    using value_type = T;
    using is_always_equal = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::integral_constant<bool, POCMA>;
    using propagate_on_container_swap = std::false_type;

    template <typename U>
    struct rebind
    {
        using other = TrackingAllocator<U, POCMA>;
    };

    /// Allocations not yet deallocated, across all instances of this instantiation
    static int live_allocations;

    int id;

    TrackingAllocator()
        : id(nextId()++)
    {
    }
    TrackingAllocator(const TrackingAllocator&) = default;
    TrackingAllocator& operator=(const TrackingAllocator&) = default;
    template <typename U>
    TrackingAllocator(const TrackingAllocator<U, POCMA>& other)
        : id(other.id)
    {
    }

    T* allocate(std::size_t n)
    {
        ++live_allocations;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* ptr, std::size_t) noexcept
    {
        if (ptr != nullptr)
        {
            --live_allocations;
        }
        ::operator delete(ptr);
    }

    friend bool operator==(const TrackingAllocator& lhs, const TrackingAllocator& rhs) noexcept
    {
        return lhs.id == rhs.id;
    }
    friend bool operator!=(const TrackingAllocator& lhs, const TrackingAllocator& rhs) noexcept
    {
        return lhs.id != rhs.id;
    }

  private:
    static int& nextId()
    {
        static int id = 0;
        return id;
    }
};

template <typename T, bool POCMA>
int TrackingAllocator<T, POCMA>::live_allocations = 0;

/// Element that bumps a caller-owned counter on destruction. Moved-from objects still count:
/// every construction must be balanced by exactly one destruction.
struct CountedElement
{
    int* count;
    explicit CountedElement(int* c)
        : count(c)
    {
    }
    CountedElement(const CountedElement&) = default;
    CountedElement& operator=(const CountedElement&) = default;
    ~CountedElement()
    {
        if (count)
            ++(*count);
    }
};

} // namespace

TEST(ArrayListMoveAssignmentTest, UnequalAllocatorsDoesNotLeakBuffer)
{
    using alloc = TrackingAllocator<int, false>;
    using list = stdx::ArrayList<int, alloc>;
    alloc::live_allocations = 0;
    {
        list src;
        src.pushBack(1);
        src.pushBack(2);
        src.pushBack(3);

        list dst;
        dst.pushBack(9);

        // Unequal allocators + POCMA false force the element-wise path
        ASSERT_TRUE(src.getAllocator() != dst.getAllocator());

        dst = std::move(src);
        ASSERT_EQ(3u, dst.size());
        EXPECT_EQ(1, dst[0]);
        EXPECT_EQ(2, dst[1]);
        EXPECT_EQ(3, dst[2]);
        EXPECT_TRUE(src.empty());
    }
    // Every buffer allocated by either container must have been deallocated
    EXPECT_EQ(0, alloc::live_allocations);
}

TEST(ArrayListMoveAssignmentTest, UnequalAllocatorsDestroysSourceElements)
{
    using list = stdx::ArrayList<CountedElement, TrackingAllocator<CountedElement, false>>;
    int srcCount = 0;
    int dstCount = 0;
    {
        list src;
        src.reserve(4); // avoid growth reallocs, which destroy elements and would inflate srcCount
        src.emplaceBack(&srcCount);
        src.emplaceBack(&srcCount);
        src.emplaceBack(&srcCount);

        list dst;
        dst.reserve(4); // avoid a realloc during assignment so dstCount stays untouched
        dst.emplaceBack(&dstCount);

        ASSERT_TRUE(src.getAllocator() != dst.getAllocator());
        ASSERT_EQ(0, srcCount);
        ASSERT_EQ(0, dstCount);

        dst = std::move(src);
        // src's moved-from elements must be destroyed as part of the move, not leaked
        EXPECT_EQ(3, srcCount);
        // dst's element was assigned over, not destroyed
        EXPECT_EQ(0, dstCount);
        ASSERT_EQ(3u, dst.size());

        // Detach dst's elements so the scope exit doesn't add to srcCount
        for (auto& e : dst)
        {
            e.count = nullptr;
        }
    }
    EXPECT_EQ(3, srcCount);
    EXPECT_EQ(0, dstCount);
}

TEST(ArrayListMoveAssignmentTest, UnequalAllocatorsDestroysSourceAndExcessDestinationElements)
{
    using list = stdx::ArrayList<CountedElement, TrackingAllocator<CountedElement, false>>;
    int srcCount = 0;
    int dstCount = 0;
    {
        list src;
        src.reserve(4); // avoid growth reallocs, which destroy elements and would inflate srcCount
        src.emplaceBack(&srcCount);

        list dst;
        dst.reserve(4);
        dst.emplaceBack(&dstCount);
        dst.emplaceBack(&dstCount);
        dst.emplaceBack(&dstCount);

        ASSERT_TRUE(src.getAllocator() != dst.getAllocator());
        ASSERT_EQ(0, srcCount);
        ASSERT_EQ(0, dstCount);

        dst = std::move(src);
        // src's moved-from element must be destroyed as part of the move
        EXPECT_EQ(1, srcCount);
        // dst's two excess elements beyond src's size must be destroyed
        EXPECT_EQ(2, dstCount);
        ASSERT_EQ(1u, dst.size());

        for (auto& e : dst)
        {
            e.count = nullptr;
        }
    }
    EXPECT_EQ(1, srcCount);
    EXPECT_EQ(2, dstCount);
}

TEST(ArrayListMoveAssignmentTest, EqualAllocatorsStealBufferWithoutLeak)
{
    using alloc = TrackingAllocator<int, false>;
    using list = stdx::ArrayList<int, alloc>;
    alloc::live_allocations = 0;
    {
        list src;
        src.pushBack(1);
        src.pushBack(2);
        src.pushBack(3);

        list dst(src); // copy constructor copies src's allocator, so allocators compare equal
        dst.pushBack(4);

        ASSERT_TRUE(src.getAllocator() == dst.getAllocator());

        dst = std::move(src);
        ASSERT_EQ(3u, dst.size());
        EXPECT_EQ(1, dst[0]);
        EXPECT_EQ(2, dst[1]);
        EXPECT_EQ(3, dst[2]);
    }
    EXPECT_EQ(0, alloc::live_allocations);
}

TEST(ArrayListMoveAssignmentTest, PocmaPropagatesAllocatorWithoutLeak)
{
    using alloc = TrackingAllocator<int, true>;
    using list = stdx::ArrayList<int, alloc>;
    alloc::live_allocations = 0;
    {
        list src;
        src.pushBack(1);
        src.pushBack(2);

        list dst;
        dst.pushBack(9);

        const int SRC_ID = src.getAllocator().id;
        ASSERT_TRUE(src.getAllocator() != dst.getAllocator());

        dst = std::move(src);
        // POCMA true: dst must adopt src's allocator along with its buffer
        EXPECT_EQ(SRC_ID, dst.getAllocator().id);
        ASSERT_EQ(2u, dst.size());
        EXPECT_EQ(1, dst[0]);
        EXPECT_EQ(2, dst[1]);
    }
    EXPECT_EQ(0, alloc::live_allocations);
}

} // namespace stdx_test
