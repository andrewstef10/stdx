#include <gtest/gtest.h>
#include <stdx/fixed_array_list.h>

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "support/contiguous_container_test_suite.h"
#include "support/list_test_suite.h"

namespace stdx_test {

// ===== Test Helpers =====

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

struct DestructorCounter
{
    int* count;
    explicit DestructorCounter(int* c)
        : count(c)
    {
    }
    DestructorCounter(const DestructorCounter&) = default;
    DestructorCounter& operator=(const DestructorCounter&) = default;
    ~DestructorCounter()
    {
        if (count)
            ++(*count);
    }
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

// Instantiate general contiguous container test cases.
// Covers indexing and iteration as well as basic size tests.
using FixedArrayListTypes = ::testing::Types<stdx::FixedArrayList<int, 3>, stdx::FixedArrayList<int, 10>>;
INSTANTIATE_TYPED_TEST_SUITE_P(FixedArrayListContiguousContainerTests, ContiguousContainerTest, FixedArrayListTypes);

// Instantiate general list container test cases. Uses a larger fixed capacity than FixedArrayListTypes since several
// ListTest cases grow a container past TestValues<T>::SIZE (up to SIZE + 2 elements).
using FixedArrayListListTypes = ::testing::Types<stdx::FixedArrayList<int, 8>>;
INSTANTIATE_TYPED_TEST_SUITE_P(FixedArrayListListTests, ListTest, FixedArrayListListTypes);

// ===== Constructor / Assignment Tests =====

TEST(FixedArrayListConstructorTest, DefaultConstructorIsEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListConstructorTest, DefaultConstructorCapacityIsN)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(5u, v.capacity());
    EXPECT_EQ(5u, v.maxSize());
}

TEST(FixedArrayListConstructorTest, CopyConstructorFromEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    stdx::FixedArrayList<int, 4> dst(src);
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(FixedArrayListConstructorTest, CopyConstructorFromNonEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::FixedArrayList<int, 4> dst(src);
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(FixedArrayListConstructorTest, CopyConstructorIsDeepCopy)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(10);
    src.pushBack(20);

    stdx::FixedArrayList<int, 4> dst(src);
    dst[0] = 99;

    EXPECT_EQ(10, src[0]);
    EXPECT_EQ(99, dst[0]);
}

TEST(FixedArrayListConstructorTest, CopyConstructorFromFull)
{
    stdx::FixedArrayList<int, 3> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::FixedArrayList<int, 3> dst(src);
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(FixedArrayListConstructorTest, MoveConstructorFromEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    stdx::FixedArrayList<int, 4> dst(std::move(src));
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(FixedArrayListConstructorTest, MoveConstructorFromNonEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::FixedArrayList<int, 4> dst(std::move(src));
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(FixedArrayListConstructorTest, MoveConstructorLeavesSourceEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(1);
    src.pushBack(2);

    stdx::FixedArrayList<int, 4> dst(std::move(src));
    EXPECT_TRUE(src.empty());
    EXPECT_EQ(0u, src.size());
}

TEST(FixedArrayListConstructorTest, MoveConstructorWithMoveOnlyType)
{
    stdx::FixedArrayList<MoveOnly, 3> src;
    src.emplaceBack(42);
    src.emplaceBack(99);

    stdx::FixedArrayList<MoveOnly, 3> dst(std::move(src));
    ASSERT_EQ(2u, dst.size());
    EXPECT_EQ(42, dst[0].value);
    EXPECT_EQ(99, dst[1].value);
}

TEST(FixedArrayListConstructorTest, CopyAssignmentFromEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    stdx::FixedArrayList<int, 4> dst;
    dst = src;
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(FixedArrayListConstructorTest, CopyAssignmentFromNonEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(10);
    src.pushBack(20);

    stdx::FixedArrayList<int, 4> dst;
    dst = src;
    ASSERT_EQ(2u, dst.size());
    EXPECT_EQ(10, dst[0]);
    EXPECT_EQ(20, dst[1]);
}

TEST(FixedArrayListConstructorTest, CopyAssignmentOverwritesExisting)
{
    stdx::FixedArrayList<int, 5> src;
    src.pushBack(10);
    src.pushBack(20);

    stdx::FixedArrayList<int, 5> dst;
    dst.pushBack(99);
    dst.pushBack(88);
    dst.pushBack(77);

    dst = src;
    ASSERT_EQ(2u, dst.size());
    EXPECT_EQ(10, dst[0]);
    EXPECT_EQ(20, dst[1]);
}

TEST(FixedArrayListConstructorTest, CopyAssignmentIsDeepCopy)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(5);

    stdx::FixedArrayList<int, 4> dst;
    dst = src;
    dst[0] = 42;

    EXPECT_EQ(5, src[0]);
    EXPECT_EQ(42, dst[0]);
}

TEST(FixedArrayListConstructorTest, CopyAssignmentSelfAssignment)
{
    stdx::FixedArrayList<int, 4> v;
    v.pushBack(1);
    v.pushBack(2);
    stdx::FixedArrayList<int, 4> self = v;
    v = self;
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(FixedArrayListConstructorTest, MoveAssignmentFromEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    stdx::FixedArrayList<int, 4> dst;
    dst = std::move(src);
    EXPECT_EQ(0u, dst.size());
    EXPECT_TRUE(dst.empty());
}

TEST(FixedArrayListConstructorTest, MoveAssignmentFromNonEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(1);
    src.pushBack(2);
    src.pushBack(3);

    stdx::FixedArrayList<int, 4> dst;
    dst = std::move(src);
    ASSERT_EQ(3u, dst.size());
    EXPECT_EQ(1, dst[0]);
    EXPECT_EQ(2, dst[1]);
    EXPECT_EQ(3, dst[2]);
}

TEST(FixedArrayListConstructorTest, MoveAssignmentLeavesSourceEmpty)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(1);
    src.pushBack(2);

    stdx::FixedArrayList<int, 4> dst;
    dst = std::move(src);

    EXPECT_TRUE(src.empty());
    EXPECT_EQ(0u, src.size());
}

TEST(FixedArrayListConstructorTest, MoveAssignmentOverwritesExisting)
{
    stdx::FixedArrayList<int, 4> src;
    src.pushBack(7);

    stdx::FixedArrayList<int, 4> dst;
    dst.pushBack(1);
    dst.pushBack(2);
    dst = std::move(src);

    ASSERT_EQ(1u, dst.size());
    EXPECT_EQ(7, dst[0]);
}

TEST(FixedArrayListConstructorTest, DestructorCallsElementDestructors)
{
    int count = 0;
    {
        stdx::FixedArrayList<DestructorCounter, 3> v;
        v.emplaceBack(&count);
        v.emplaceBack(&count);
        EXPECT_EQ(0, count);
    }
    EXPECT_EQ(2, count);
}

TEST(FixedArrayListConstructorTest, MoveConstructorDestroysSourceElements)
{
    int srcCount = 0;
    int dstCount = 0;
    {
        stdx::FixedArrayList<DestructorCounter, 3> src;
        src.emplaceBack(&srcCount);
        src.emplaceBack(&srcCount);

        stdx::FixedArrayList<DestructorCounter, 3> dst(std::move(src));
        // src's moved-from elements must be destroyed as part of the move, not leaked
        EXPECT_EQ(2, srcCount);

        // Repoint dst's elements so their destructors count separately at scope exit
        for (auto& e : dst)
        {
            e.count = &dstCount;
        }
    }
    EXPECT_EQ(2, dstCount);
}

TEST(FixedArrayListConstructorTest, MoveAssignmentDestroysSourceElements)
{
    int srcCount = 0;
    {
        stdx::FixedArrayList<DestructorCounter, 3> src;
        src.emplaceBack(&srcCount);
        src.emplaceBack(&srcCount);

        stdx::FixedArrayList<DestructorCounter, 3> dst;
        dst = std::move(src);
        // src's moved-from elements must be destroyed as part of the move, not leaked
        EXPECT_EQ(2, srcCount);

        // Detach dst's elements so the outer scope exit doesn't add to srcCount
        for (auto& e : dst)
        {
            e.count = nullptr;
        }
    }
    EXPECT_EQ(2, srcCount);
}

TEST(FixedArrayListConstructorTest, MoveAssignmentDestroysSourceElementsWhenDestinationSmaller)
{
    int srcCount = 0;
    int dstCount = 0;
    {
        stdx::FixedArrayList<DestructorCounter, 4> src;
        src.emplaceBack(&srcCount);
        src.emplaceBack(&srcCount);
        src.emplaceBack(&srcCount);

        stdx::FixedArrayList<DestructorCounter, 4> dst;
        dst.emplaceBack(&dstCount);

        dst = std::move(src);
        // All of src's moved-from elements must be destroyed: both the one that was move-assigned
        // over dst's existing element and the two that were move-constructed into new slots
        EXPECT_EQ(3, srcCount);
        // dst's existing element was assigned over, not destroyed
        EXPECT_EQ(0, dstCount);
        ASSERT_EQ(3u, dst.size());

        for (auto& e : dst)
        {
            e.count = nullptr;
        }
    }
    EXPECT_EQ(3, srcCount);
    EXPECT_EQ(0, dstCount);
}

TEST(FixedArrayListConstructorTest, MoveAssignmentDestroysSourceAndExcessDestinationElements)
{
    int srcCount = 0;
    int dstCount = 0;
    {
        stdx::FixedArrayList<DestructorCounter, 4> src;
        src.emplaceBack(&srcCount);

        stdx::FixedArrayList<DestructorCounter, 4> dst;
        dst.emplaceBack(&dstCount);
        dst.emplaceBack(&dstCount);
        dst.emplaceBack(&dstCount);

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

// ===== Element Access Tests =====

TEST(FixedArrayListElementAccessTest, AtValidIndex)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    EXPECT_EQ(10, v.at(0));
    EXPECT_EQ(20, v.at(1));
    EXPECT_EQ(30, v.at(2));
}

TEST(FixedArrayListElementAccessTest, AtConstValidIndex)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_EQ(10, cv.at(0));
    EXPECT_EQ(20, cv.at(1));
}

TEST(FixedArrayListElementAccessTest, AtReturnsReference)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(5);
    v.at(0) = 99;
    EXPECT_EQ(99, v.at(0));
}

TEST(FixedArrayListElementAccessTest, AtBoundaryLastElement)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(3, v.at(2));
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST(FixedArrayListElementAccessTest, AtOutOfRange)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    EXPECT_THROW(v.at(1), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(FixedArrayListElementAccessTest, AtConstOutOfRange)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_THROW(cv.at(1), std::out_of_range);
    EXPECT_THROW(cv.at(99), std::out_of_range);
}

TEST(FixedArrayListElementAccessTest, AtOnEmptyContainerThrows)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(FixedArrayListElementAccessTest, SubscriptOperator)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(FixedArrayListElementAccessTest, SubscriptOperatorConst)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_EQ(10, cv[0]);
    EXPECT_EQ(20, cv[1]);
}

TEST(FixedArrayListElementAccessTest, SubscriptOperatorReturnsReference)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(5);
    v[0] = 42;
    EXPECT_EQ(42, v[0]);
}

TEST(FixedArrayListElementAccessTest, DataPointerNonNullAfterPushBack)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    EXPECT_NE(nullptr, v.data());
}

TEST(FixedArrayListElementAccessTest, DataPointerConsistentWithSubscript)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    EXPECT_EQ(&v[0], v.data());
    EXPECT_EQ(10, v.data()[0]);
    EXPECT_EQ(20, v.data()[1]);
}

TEST(FixedArrayListElementAccessTest, DataPointerConst)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(5);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_NE(nullptr, cv.data());
    EXPECT_EQ(5, *cv.data());
}

TEST(FixedArrayListElementAccessTest, FrontElement)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(1, v.front());
}

TEST(FixedArrayListElementAccessTest, FrontElementConst)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(42);
    v.pushBack(99);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_EQ(42, cv.front());
}

TEST(FixedArrayListElementAccessTest, FrontReturnsReference)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(5);
    v.front() = 99;
    EXPECT_EQ(99, v.front());
}

TEST(FixedArrayListElementAccessTest, BackElement)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    EXPECT_EQ(3, v.back());
}

TEST(FixedArrayListElementAccessTest, BackElementConst)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(100);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_EQ(100, cv.back());
}

TEST(FixedArrayListElementAccessTest, BackReturnsReference)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(5);
    v.back() = 77;
    EXPECT_EQ(77, v.back());
}

TEST(FixedArrayListElementAccessTest, FrontAndBackSameWhenSizeOne)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(7);
    EXPECT_EQ(v.front(), v.back());
    EXPECT_EQ(&v.front(), &v.back());
}

TEST(FixedArrayListElementAccessTest, FrontAndBackDistinctWhenSizeTwo)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(2, v.back());
    EXPECT_NE(&v.front(), &v.back());
}

// ===== Capacity Tests =====

TEST(FixedArrayListCapacityTest, EmptyOnDefault)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListCapacityTest, NotEmptyAfterPushBack)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    EXPECT_FALSE(v.empty());
}

TEST(FixedArrayListCapacityTest, EmptyAfterClear)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListCapacityTest, SizeDefault)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(0u, v.size());
}

TEST(FixedArrayListCapacityTest, SizeAfterPushBack)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    EXPECT_EQ(1u, v.size());
    v.pushBack(2);
    EXPECT_EQ(2u, v.size());
    v.pushBack(3);
    EXPECT_EQ(3u, v.size());
}

TEST(FixedArrayListCapacityTest, SizeAfterPopBack)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();
    EXPECT_EQ(1u, v.size());
    v.popBack();
    EXPECT_EQ(0u, v.size());
}

TEST(FixedArrayListCapacityTest, CapacityAlwaysN)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(5u, v.capacity());
    v.pushBack(1);
    EXPECT_EQ(5u, v.capacity());
    v.pushBack(2);
    EXPECT_EQ(5u, v.capacity());
}

TEST(FixedArrayListCapacityTest, CapacityUnchangedAfterClear)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.clear();
    EXPECT_EQ(5u, v.capacity());
}

TEST(FixedArrayListCapacityTest, MaxSizeAlwaysN)
{
    stdx::FixedArrayList<int, 7> v;
    EXPECT_EQ(7u, v.maxSize());
    v.pushBack(1);
    EXPECT_EQ(7u, v.maxSize());
}

TEST(FixedArrayListCapacityTest, MaxSizeEqualsCapacity)
{
    stdx::FixedArrayList<int, 4> v;
    EXPECT_EQ(v.maxSize(), v.capacity());
}

TEST(FixedArrayListCapacityTest, DifferentNValues)
{
    stdx::FixedArrayList<int, 1> v1;
    stdx::FixedArrayList<int, 10> v10;
    stdx::FixedArrayList<int, 100> v100;
    EXPECT_EQ(1u, v1.capacity());
    EXPECT_EQ(10u, v10.capacity());
    EXPECT_EQ(100u, v100.capacity());
}

// ===== Modifier Tests =====

TEST(FixedArrayListModifierTest, PushBackCopyIncreasesSize)
{
    stdx::FixedArrayList<int, 5> v;
    int val = 42;
    v.pushBack(val);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, val);
}

TEST(FixedArrayListModifierTest, PushBackCopyMultiple)
{
    stdx::FixedArrayList<int, 5> v;
    for (int i = 0; i < 5; ++i)
    {
        v.pushBack(i);
    }
    ASSERT_EQ(5u, v.size());
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(i, v[i]);
    }
}

TEST(FixedArrayListModifierTest, PushBackMove)
{
    stdx::FixedArrayList<std::string, 5> v;
    std::string s = "hello";
    v.pushBack(std::move(s));
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ("hello", v[0]);
}

TEST(FixedArrayListModifierTest, PushBackMoveSourceEmptied)
{
    stdx::FixedArrayList<std::string, 5> v;
    std::string s = "hello";
    v.pushBack(std::move(s));
    EXPECT_TRUE(s.empty());
}

TEST(FixedArrayListModifierTest, PushBackCopyAtCapacityThrows)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    int val = 4;
    EXPECT_THROW(v.pushBack(val), std::length_error);
}

TEST(FixedArrayListModifierTest, PushBackMoveAtCapacityThrows)
{
    stdx::FixedArrayList<std::string, 2> v;
    v.pushBack(std::string("a"));
    v.pushBack(std::string("b"));
    EXPECT_THROW(v.pushBack(std::string("c")), std::length_error);
}

TEST(FixedArrayListModifierTest, PushBackThrowDoesNotChangeSize)
{
    stdx::FixedArrayList<int, 2> v;
    v.pushBack(1);
    v.pushBack(2);
    int val = 3;
    EXPECT_THROW(v.pushBack(val), std::length_error);
    EXPECT_EQ(2u, v.size());
}

TEST(FixedArrayListModifierTest, PopBackReducesSize)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();
    EXPECT_EQ(1u, v.size());
}

TEST(FixedArrayListModifierTest, PopBackRemovesLastElement)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.popBack();
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(FixedArrayListModifierTest, PopBackToEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(42);
    v.popBack();
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListModifierTest, PopBackCallsDestructor)
{
    int count = 0;
    stdx::FixedArrayList<DestructorCounter, 3> v;
    v.emplaceBack(&count);
    v.emplaceBack(&count);
    EXPECT_EQ(0, count);
    v.popBack();
    EXPECT_EQ(1, count);
    v.popBack();
    EXPECT_EQ(2, count);
}

TEST(FixedArrayListModifierTest, PopBackThenPushBack)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.popBack();
    v.pushBack(99);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(99, v[2]);
}

TEST(FixedArrayListModifierTest, EmplaceBackIntoEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.emplaceBack(42);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
}

TEST(FixedArrayListModifierTest, EmplaceBackIncreasesSize)
{
    stdx::FixedArrayList<int, 5> v;
    v.emplaceBack(1);
    v.emplaceBack(2);
    v.emplaceBack(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(FixedArrayListModifierTest, EmplaceBackWithMultipleArgs)
{
    stdx::FixedArrayList<Point, 5> v;
    v.emplaceBack(3, 7);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(3, v[0].x);
    EXPECT_EQ(7, v[0].y);
}

TEST(FixedArrayListModifierTest, EmplaceBackConstructsInPlace)
{
    stdx::FixedArrayList<MoveOnly, 3> v;
    v.emplaceBack(99);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(99, v[0].value);
}

TEST(FixedArrayListModifierTest, EmplaceBackNoCopyOrMove)
{
    ConstructionCounter::direct_constructions = 0;
    ConstructionCounter::copy_constructions = 0;
    ConstructionCounter::move_constructions = 0;

    stdx::FixedArrayList<ConstructionCounter, 5> v;
    v.emplaceBack(5);

    EXPECT_EQ(1, ConstructionCounter::direct_constructions);
    EXPECT_EQ(0, ConstructionCounter::copy_constructions);
    EXPECT_EQ(0, ConstructionCounter::move_constructions);
}

TEST(FixedArrayListModifierTest, EmplaceBackAtCapacityThrows)
{
    stdx::FixedArrayList<int, 2> v;
    v.emplaceBack(1);
    v.emplaceBack(2);
    EXPECT_THROW(v.emplaceBack(3), std::length_error);
}

TEST(FixedArrayListModifierTest, EmplaceBackWithStringArgs)
{
    stdx::FixedArrayList<std::string, 3> v;
    v.emplaceBack(3, 'z');
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ("zzz", v[0]);
}

TEST(FixedArrayListModifierTest, InsertAtBeginCopy)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListModifierTest, InsertAtEndCopy)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    auto it = v.insert(v.end(), 3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(3, *it);
}

TEST(FixedArrayListModifierTest, InsertAtMiddleCopy)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(3);
    auto it = v.insert(v.begin() + 1, 2);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(2, *it);
}

TEST(FixedArrayListModifierTest, InsertIntoEmptyCopy)
{
    stdx::FixedArrayList<int, 5> v;
    auto it = v.insert(v.end(), 42);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, *it);
}

TEST(FixedArrayListModifierTest, InsertAtBeginMove)
{
    stdx::FixedArrayList<std::string, 5> v;
    v.pushBack("world");
    std::string s = "hello";
    auto it = v.insert(v.begin(), std::move(s));
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("world", v[1]);
    EXPECT_EQ("hello", *it);
}

TEST(FixedArrayListModifierTest, InsertAtEndMove)
{
    stdx::FixedArrayList<std::string, 5> v;
    v.pushBack("hello");
    std::string s = "world";
    auto it = v.insert(v.end(), std::move(s));
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("world", v[1]);
    EXPECT_EQ("world", *it);
}

TEST(FixedArrayListModifierTest, InsertAtMiddleMove)
{
    stdx::FixedArrayList<std::string, 5> v;
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

TEST(FixedArrayListModifierTest, InsertCopyAtCapacityThrows)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    int val = 4;
    EXPECT_THROW(v.insert(v.begin(), val), std::length_error);
}

TEST(FixedArrayListModifierTest, InsertMoveAtCapacityThrows)
{
    stdx::FixedArrayList<std::string, 2> v;
    v.pushBack("a");
    v.pushBack("b");
    EXPECT_THROW(v.insert(v.begin(), std::string("c")), std::length_error);
}

TEST(FixedArrayListModifierTest, InsertSelfReferenceWithSpareCapacity)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListModifierTest, EmplaceIntoEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    auto it = v.emplace(v.end(), 42);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, *it);
}

TEST(FixedArrayListModifierTest, EmplaceAtBegin)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListModifierTest, EmplaceAtEnd)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    auto it = v.emplace(v.end(), 3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(3, *it);
}

TEST(FixedArrayListModifierTest, EmplaceAtMiddle)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(3);
    auto it = v.emplace(v.begin() + 1, 2);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
    EXPECT_EQ(2, *it);
}

TEST(FixedArrayListModifierTest, EmplaceWithMultipleArgs)
{
    stdx::FixedArrayList<Point, 5> v;
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

TEST(FixedArrayListModifierTest, EmplaceAtCapacityThrows)
{
    stdx::FixedArrayList<int, 2> v;
    v.emplaceBack(1);
    v.emplaceBack(2);
    EXPECT_THROW(v.emplace(v.begin(), 3), std::length_error);
}

TEST(FixedArrayListModifierTest, EraseFirst)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListModifierTest, EraseLast)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.end() - 1);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(v.end(), it);
}

TEST(FixedArrayListModifierTest, EraseMiddle)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.begin() + 1);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(3, v[1]);
    EXPECT_EQ(3, *it);
}

TEST(FixedArrayListModifierTest, EraseOnlyElement)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(42);
    auto it = v.erase(v.begin());
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.end(), it);
}

TEST(FixedArrayListModifierTest, EraseCallsDestructor)
{
    int count = 0;
    stdx::FixedArrayList<DestructorCounter, 3> v;
    v.emplaceBack(&count);
    v.emplaceBack(&count);
    v.emplaceBack(&count);
    v.erase(v.begin());
    EXPECT_EQ(1, count);
}

TEST(FixedArrayListModifierTest, EraseRangeEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    auto it = v.erase(v.begin() + 1, v.begin() + 1);
    EXPECT_EQ(2u, v.size());
    EXPECT_EQ(v.begin() + 1, it);
}

TEST(FixedArrayListModifierTest, EraseRangeAll)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    auto it = v.erase(v.begin(), v.end());
    EXPECT_EQ(0u, v.size());
    EXPECT_EQ(v.end(), it);
}

TEST(FixedArrayListModifierTest, EraseRangePartial)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    v.pushBack(5);
    auto it = v.erase(v.begin() + 1, v.begin() + 3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(4, v[1]);
    EXPECT_EQ(5, v[2]);
    EXPECT_EQ(4, *it);
}

TEST(FixedArrayListModifierTest, EraseRangeFromBegin)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    auto it = v.erase(v.begin(), v.begin() + 2);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(3, v[0]);
    EXPECT_EQ(4, v[1]);
    EXPECT_EQ(v.begin(), it);
    EXPECT_EQ(3, *it);
}

TEST(FixedArrayListModifierTest, EraseRangeToEnd)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    auto it = v.erase(v.begin() + 2, v.end());
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(v.end(), it);
}

TEST(FixedArrayListModifierTest, EraseRangeAllowsSubsequentInsert)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.erase(v.begin(), v.end());
    EXPECT_TRUE(v.empty());
    v.pushBack(99);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(99, v[0]);
}

TEST(FixedArrayListModifierTest, ClearEmptyContainer)
{
    stdx::FixedArrayList<int, 5> v;
    v.clear();
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListModifierTest, ClearNonEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.clear();
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListModifierTest, ClearPreservesCapacity)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.clear();
    EXPECT_EQ(5u, v.capacity());
}

TEST(FixedArrayListModifierTest, ClearCallsDestructors)
{
    int count = 0;
    stdx::FixedArrayList<DestructorCounter, 3> v;
    v.emplaceBack(&count);
    v.emplaceBack(&count);
    v.emplaceBack(&count);
    EXPECT_EQ(0, count);
    v.clear();
    EXPECT_EQ(3, count);
}

TEST(FixedArrayListModifierTest, ClearThenPushBack)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.clear();
    v.pushBack(99);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ(99, v[0]);
}

TEST(FixedArrayListModifierTest, ClearAllowsRefillToCapacity)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.clear();
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
    EXPECT_EQ(30, v[2]);
}

TEST(FixedArrayListModifierTest, SwapTwoNonEmpty)
{
    stdx::FixedArrayList<int, 5> a;
    a.pushBack(1);
    a.pushBack(2);

    stdx::FixedArrayList<int, 5> b;
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

TEST(FixedArrayListModifierTest, SwapWithEmpty)
{
    stdx::FixedArrayList<int, 5> a;
    a.pushBack(1);
    a.pushBack(2);

    stdx::FixedArrayList<int, 5> b;

    a.swap(b);

    EXPECT_TRUE(a.empty());
    ASSERT_EQ(2u, b.size());
    EXPECT_EQ(1, b[0]);
    EXPECT_EQ(2, b[1]);
}

TEST(FixedArrayListModifierTest, SwapTwoEmptyContainers)
{
    stdx::FixedArrayList<int, 5> a;
    stdx::FixedArrayList<int, 5> b;
    a.swap(b);
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
}

TEST(FixedArrayListModifierTest, SwapBothContainersUsableAfterSwap)
{
    stdx::FixedArrayList<int, 5> a, b;
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

TEST(FixedArrayListModifierTest, SwapMoveOnlyType)
{
    stdx::FixedArrayList<MoveOnly, 3> a;
    a.emplaceBack(1);
    a.emplaceBack(2);

    stdx::FixedArrayList<MoveOnly, 3> b;
    b.emplaceBack(10);

    a.swap(b);

    ASSERT_EQ(1u, a.size());
    EXPECT_EQ(10, a[0].value);
    ASSERT_EQ(2u, b.size());
    EXPECT_EQ(1, b[0].value);
    EXPECT_EQ(2, b[1].value);
}

// ===== Iterator Tests =====

TEST(FixedArrayListIteratorTest, BeginEqualsEndWhenEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(v.begin(), v.end());
}

TEST(FixedArrayListIteratorTest, BeginNotEqualsEndWhenNonEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    EXPECT_NE(v.begin(), v.end());
}

TEST(FixedArrayListIteratorTest, BeginPointsToFirstElement)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(42);
    v.pushBack(7);
    EXPECT_EQ(42, *v.begin());
}

TEST(FixedArrayListIteratorTest, EndIsOnePastLast)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    EXPECT_EQ(2, *(v.end() - 1));
}

TEST(FixedArrayListIteratorTest, ForwardIteration)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListIteratorTest, RangeBasedForLoop)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListIteratorTest, ConstBeginEnd)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(5);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_NE(cv.begin(), cv.end());
    EXPECT_EQ(5, *cv.begin());
}

TEST(FixedArrayListIteratorTest, ConstBeginEqualsEndWhenEmpty)
{
    const stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(v.begin(), v.end());
}

TEST(FixedArrayListIteratorTest, CBeginCEnd)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(3);
    v.pushBack(4);
    EXPECT_EQ(3, *v.cbegin());
    EXPECT_NE(v.cbegin(), v.cend());
}

TEST(FixedArrayListIteratorTest, CBeginEqualsCBeginConst)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    const stdx::FixedArrayList<int, 5>& cv = v;
    EXPECT_EQ(v.cbegin(), cv.begin());
    EXPECT_EQ(v.cend(), cv.end());
}

TEST(FixedArrayListIteratorTest, ReverseIterationEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    EXPECT_EQ(v.rbegin(), v.rend());
}

TEST(FixedArrayListIteratorTest, ReverseIteration)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListIteratorTest, ConstReverseIteration)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    const stdx::FixedArrayList<int, 5>& cv = v;

    auto it = cv.rbegin();
    EXPECT_EQ(20, *it);
    ++it;
    EXPECT_EQ(10, *it);
    ++it;
    EXPECT_EQ(cv.rend(), it);
}

TEST(FixedArrayListIteratorTest, CRBeginCREnd)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListIteratorTest, MutableIteratorAllowsWrite)
{
    stdx::FixedArrayList<int, 5> v;
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

// ===== Comparison Tests =====

TEST(FixedArrayListComparisonTest, EqualsTwoEmpty)
{
    stdx::FixedArrayList<int, 5> a, b;
    EXPECT_TRUE(a.equals(b));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(FixedArrayListComparisonTest, EqualsSameElements)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(3);
    EXPECT_TRUE(a.equals(b));
    EXPECT_TRUE(a == b);
}

TEST(FixedArrayListComparisonTest, EqualsNotEqualElements)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(3);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(FixedArrayListComparisonTest, EqualsNotEqualSizesASmaller)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(3);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(a == b);
}

TEST(FixedArrayListComparisonTest, EqualsNotEqualSizesALarger)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    b.pushBack(1);
    b.pushBack(2);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(a == b);
}

TEST(FixedArrayListComparisonTest, EqualsEmptyVsNonEmpty)
{
    stdx::FixedArrayList<int, 5> a, b;
    b.pushBack(1);
    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(b.equals(a));
}

TEST(FixedArrayListComparisonTest, LessThanLexicographic)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(4);
    EXPECT_TRUE(a.lessThan(b));
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(FixedArrayListComparisonTest, LessThanBySize)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(2);
    b.pushBack(3);
    EXPECT_TRUE(a.lessThan(b));
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(FixedArrayListComparisonTest, LessThanEqualIsNotLess)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    a.pushBack(2);
    b.pushBack(1);
    b.pushBack(2);
    EXPECT_FALSE(a.lessThan(b));
    EXPECT_FALSE(a < b);
}

TEST(FixedArrayListComparisonTest, LessThanBothEmpty)
{
    stdx::FixedArrayList<int, 5> a, b;
    EXPECT_FALSE(a.lessThan(b));
    EXPECT_FALSE(a < b);
}

TEST(FixedArrayListComparisonTest, LessThanFirstElementDecides)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    b.pushBack(2);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(FixedArrayListComparisonTest, GreaterThan)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(5);
    b.pushBack(3);
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(FixedArrayListComparisonTest, GreaterThanEqualNotGreater)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    b.pushBack(1);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(FixedArrayListComparisonTest, LessOrEqualWhenEqual)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    b.pushBack(1);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b <= a);
}

TEST(FixedArrayListComparisonTest, LessOrEqualWhenLess)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    b.pushBack(2);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
}

TEST(FixedArrayListComparisonTest, GreaterOrEqualWhenEqual)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(1);
    b.pushBack(1);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b >= a);
}

TEST(FixedArrayListComparisonTest, GreaterOrEqualWhenGreater)
{
    stdx::FixedArrayList<int, 5> a, b;
    a.pushBack(3);
    b.pushBack(2);
    EXPECT_TRUE(a >= b);
    EXPECT_FALSE(b >= a);
}

TEST(FixedArrayListComparisonTest, AllOperatorsEmptyContainers)
{
    stdx::FixedArrayList<int, 5> a, b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a >= b);
}

// ===== Member Types Tests =====

TEST(FixedArrayListMemberTypesTest, SizeType)
{
    static_assert(std::is_same<stdx::FixedArrayList<int, 5>::size_type, std::size_t>::value, "");
}

TEST(FixedArrayListMemberTypesTest, Reference)
{
    static_assert(std::is_same<stdx::FixedArrayList<int, 5>::reference, int&>::value, "");
    static_assert(std::is_same<stdx::FixedArrayList<double, 5>::reference, double&>::value, "");
}

TEST(FixedArrayListMemberTypesTest, ConstReference)
{
    static_assert(std::is_same<stdx::FixedArrayList<int, 5>::const_reference, const int&>::value, "");
}

TEST(FixedArrayListMemberTypesTest, ReferenceIsActuallyMutable)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    stdx::FixedArrayList<int, 5>::reference ref = v[0];
    ref = 42;
    EXPECT_EQ(42, v[0]);
}

TEST(FixedArrayListMemberTypesTest, ConstReferenceFromConstContainer)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(7);
    const stdx::FixedArrayList<int, 5>& cv = v;
    stdx::FixedArrayList<int, 5>::const_reference cref = cv[0];
    EXPECT_EQ(7, cref);
}

// ===== Non-Default-Constructible Type Tests =====

TEST(FixedArrayListNonDefaultConstructibleTest, EmplaceBackPoint)
{
    stdx::FixedArrayList<Point, 5> v;
    v.emplaceBack(1, 2);
    v.emplaceBack(3, 4);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0].x);
    EXPECT_EQ(2, v[0].y);
    EXPECT_EQ(3, v[1].x);
    EXPECT_EQ(4, v[1].y);
}

TEST(FixedArrayListNonDefaultConstructibleTest, MoveOnlyPushBack)
{
    stdx::FixedArrayList<MoveOnly, 3> v;
    v.pushBack(MoveOnly(1));
    v.pushBack(MoveOnly(2));
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0].value);
    EXPECT_EQ(2, v[1].value);
}

TEST(FixedArrayListNonDefaultConstructibleTest, MoveOnlyCopyConstructorIsDeleted)
{
    static_assert(!std::is_copy_constructible<MoveOnly>::value, "");
}

// ===== Resize Tests =====

namespace {

struct ResizeDestructorCounter
{
    static int destructions;
    int value;
    ResizeDestructorCounter()
        : value(0)
    {
    }
    explicit ResizeDestructorCounter(int v)
        : value(v)
    {
    }
    ~ResizeDestructorCounter()
    {
        ++destructions;
    }
    ResizeDestructorCounter(const ResizeDestructorCounter&) = default;
    ResizeDestructorCounter& operator=(const ResizeDestructorCounter&) = default;
    ResizeDestructorCounter(ResizeDestructorCounter&&) noexcept = default;
    ResizeDestructorCounter& operator=(ResizeDestructorCounter&&) noexcept = default;
};
int ResizeDestructorCounter::destructions = 0;

} // namespace

// --- resize(count) ---

TEST(FixedArrayListResizeTest, ResizeSameSizeIsNoop)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.resize(3);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(5u, v.capacity());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(FixedArrayListResizeTest, ResizeToZeroFromEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.resize(0);
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListResizeTest, ResizeToZeroFromNonEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.resize(0);
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListResizeTest, ResizeLargerFromEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.resize(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(0, v[1]);
    EXPECT_EQ(0, v[2]);
}

TEST(FixedArrayListResizeTest, ResizeLargerPreservesExistingElements)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    v.resize(4);
    ASSERT_EQ(4u, v.size());
    EXPECT_EQ(10, v[0]);
    EXPECT_EQ(20, v[1]);
}

TEST(FixedArrayListResizeTest, ResizeLargerDefaultInitializesNewElements)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(10);
    v.pushBack(20);
    v.resize(5);
    ASSERT_EQ(5u, v.size());
    EXPECT_EQ(0, v[2]);
    EXPECT_EQ(0, v[3]);
    EXPECT_EQ(0, v[4]);
}

TEST(FixedArrayListResizeTest, ResizeSmallerReducesSize)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    v.pushBack(5);
    v.resize(2);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(FixedArrayListResizeTest, ResizeSmallerCapacityRemainsN)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.resize(1);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(5u, v.capacity());
}

TEST(FixedArrayListResizeTest, ResizeSmallerDestroysRemovedElements)
{
    ResizeDestructorCounter::destructions = 0;
    {
        stdx::FixedArrayList<ResizeDestructorCounter, 5> v;
        v.emplaceBack(1);
        v.emplaceBack(2);
        v.emplaceBack(3);
        ResizeDestructorCounter::destructions = 0;
        v.resize(1);
        EXPECT_EQ(2, ResizeDestructorCounter::destructions);
        EXPECT_EQ(1u, v.size());
        EXPECT_EQ(1, v[0].value);
    }
}

TEST(FixedArrayListResizeTest, ResizeLargerWithNonTrivialTypeDefaultInserted)
{
    stdx::FixedArrayList<std::string, 5> v;
    v.pushBack("hello");
    v.resize(3);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("", v[1]);
    EXPECT_EQ("", v[2]);
}

TEST(FixedArrayListResizeTest, ResizeSmallerWithNonTrivialType)
{
    stdx::FixedArrayList<std::string, 5> v;
    v.pushBack("a");
    v.pushBack("b");
    v.pushBack("c");
    v.resize(1);
    ASSERT_EQ(1u, v.size());
    EXPECT_EQ("a", v[0]);
}

TEST(FixedArrayListResizeTest, ResizeThenPushBackWorks)
{
    stdx::FixedArrayList<int, 5> v;
    v.resize(3);
    v.pushBack(99);
    ASSERT_EQ(4u, v.size());
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(0, v[1]);
    EXPECT_EQ(0, v[2]);
    EXPECT_EQ(99, v[3]);
}

TEST(FixedArrayListResizeTest, ResizeSmallerThenLargerDefaultInitializesNewElements)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListResizeTest, ResizeToExactCapacitySucceeds)
{
    stdx::FixedArrayList<int, 4> v;
    v.pushBack(1);
    v.resize(4);
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(4u, v.capacity());
}

TEST(FixedArrayListResizeTest, ResizeExceedsCapacityThrows)
{
    stdx::FixedArrayList<int, 3> v;
    EXPECT_THROW(v.resize(4), std::length_error);
}

TEST(FixedArrayListResizeTest, ResizeExceedsCapacityDoesNotChangeSize)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    EXPECT_THROW(v.resize(4), std::length_error);
    EXPECT_EQ(2u, v.size());
}

// --- resize(count, value) ---

TEST(FixedArrayListResizeTest, ResizeWithValueSameSizeIsNoop)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.resize(3, 99);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(FixedArrayListResizeTest, ResizeWithValueToZeroFromNonEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.resize(0, 99);
    EXPECT_EQ(0u, v.size());
    EXPECT_TRUE(v.empty());
}

TEST(FixedArrayListResizeTest, ResizeWithValueLargerFromEmpty)
{
    stdx::FixedArrayList<int, 5> v;
    v.resize(3, 42);
    ASSERT_EQ(3u, v.size());
    EXPECT_EQ(42, v[0]);
    EXPECT_EQ(42, v[1]);
    EXPECT_EQ(42, v[2]);
}

TEST(FixedArrayListResizeTest, ResizeWithValueLargerPreservesExistingElements)
{
    stdx::FixedArrayList<int, 5> v;
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

TEST(FixedArrayListResizeTest, ResizeWithValueSmallerReducesSize)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.pushBack(4);
    v.resize(2, 99);
    ASSERT_EQ(2u, v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
}

TEST(FixedArrayListResizeTest, ResizeWithValueSmallerCapacityRemainsN)
{
    stdx::FixedArrayList<int, 5> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);
    v.resize(1, 99);
    EXPECT_EQ(1u, v.size());
    EXPECT_EQ(5u, v.capacity());
}

TEST(FixedArrayListResizeTest, ResizeWithValueSmallerDestroysRemovedElements)
{
    ResizeDestructorCounter::destructions = 0;
    {
        stdx::FixedArrayList<ResizeDestructorCounter, 5> v;
        v.emplaceBack(1);
        v.emplaceBack(2);
        v.emplaceBack(3);
        ResizeDestructorCounter filler{0};
        ResizeDestructorCounter::destructions = 0;
        v.resize(1, filler);
        EXPECT_EQ(2, ResizeDestructorCounter::destructions);
        EXPECT_EQ(1u, v.size());
        EXPECT_EQ(1, v[0].value);
    }
}

TEST(FixedArrayListResizeTest, ResizeWithValueWithString)
{
    stdx::FixedArrayList<std::string, 5> v;
    v.pushBack("hello");
    v.resize(4, "world");
    ASSERT_EQ(4u, v.size());
    EXPECT_EQ("hello", v[0]);
    EXPECT_EQ("world", v[1]);
    EXPECT_EQ("world", v[2]);
    EXPECT_EQ("world", v[3]);
}

TEST(FixedArrayListResizeTest, ResizeWithValueToExactCapacitySucceeds)
{
    stdx::FixedArrayList<int, 4> v;
    v.pushBack(1);
    v.resize(4, 7);
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(7, v[1]);
    EXPECT_EQ(7, v[2]);
    EXPECT_EQ(7, v[3]);
}

TEST(FixedArrayListResizeTest, ResizeWithValueExceedsCapacityThrows)
{
    stdx::FixedArrayList<int, 3> v;
    EXPECT_THROW(v.resize(4, 99), std::length_error);
}

TEST(FixedArrayListResizeTest, ResizeWithValueExceedsCapacityDoesNotChangeSize)
{
    stdx::FixedArrayList<int, 3> v;
    v.pushBack(1);
    v.pushBack(2);
    EXPECT_THROW(v.resize(4, 99), std::length_error);
    EXPECT_EQ(2u, v.size());
}

TEST(FixedArrayListResizeTest, ResizeWithValueSmallerThenLargerUsesValue)
{
    stdx::FixedArrayList<int, 5> v;
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

} // namespace stdx_test
