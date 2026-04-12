#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>
#include <vector>

#include <stdads/Iterator.h>

using namespace stdads;

// ============================================================
// PtrIterator Tests (covers IteratorBase via CRTP)
// ============================================================

TEST(PtrIteratorTest, PointerConstructorAndDereferenceAndArrow)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> it(arr);

    EXPECT_EQ(*it, 1);
    EXPECT_EQ(it.operator->(), &arr[0]);
}

TEST(PtrIteratorTest, DefaultConstructor)
{
    PtrIterator<int> it;

    // Cannot safely dereference, but should be constructible
    SUCCEED();
}

TEST(PtrIteratorTest, CopyConstructor)
{
    int arr[] = {1, 2};

    PtrIterator<int> it(arr);
    PtrIterator<int> copy(it);
    EXPECT_EQ(*copy, 1);
}

TEST(PtrIteratorTest, CopyAssignment)
{
    int arr[] = {1, 2};

    PtrIterator<int> a(arr);
    PtrIterator<int> b;
    b = a;

    EXPECT_EQ(*b, 1);
}

TEST(PtrIteratorConstructorTest, MoveConstructor)
{
    int arr[] = {9};

    PtrIterator<int> it(arr);
    PtrIterator<int> moved(std::move(it));

    EXPECT_EQ(*moved, 9);
}

TEST(PtrIteratorConstructorTest, MoveAssignment)
{
    int arr[] = {7};

    PtrIterator<int> it(arr);
    PtrIterator<int> target;

    target = std::move(it);

    EXPECT_EQ(*target, 7);
}

TEST(PtrIteratorTest, PreIncrement)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> it(arr);

    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(PtrIteratorTest, PostIncrement)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> it(arr);

    PtrIterator<int> old = it++;
    EXPECT_EQ(*old, 1);
    EXPECT_EQ(*it, 2);
}

TEST(PtrIteratorTest, PreDecrement)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> it(arr + 1);

    --it;
    EXPECT_EQ(*it, 1);
}

TEST(PtrIteratorTest, PostDecrement)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> it(arr + 1);

    PtrIterator<int> old = it--;
    EXPECT_EQ(*old, 2);
    EXPECT_EQ(*it, 1);
}

TEST(PtrIteratorTest, AdvanceAndArithmetic)
{
    int arr[] = {10, 20, 30, 40};
    PtrIterator<int> it(arr);

    it += 2;
    EXPECT_EQ(*it, 30);

    it -= 1;
    EXPECT_EQ(*it, 20);

    PtrIterator<int> it2 = it + 2;
    EXPECT_EQ(*it2, 40);

    PtrIterator<int> it3 = it2 - 3;
    EXPECT_EQ(*it3, 10);
}

TEST(PtrIteratorTest, AdditionSymmetry)
{
    int arr[] = {1, 2, 3, 4};

    PtrIterator<int> it(arr);

    PtrIterator<int> a = it + 2;
    PtrIterator<int> b = 2 + it;

    EXPECT_EQ(*a, *b);
    EXPECT_EQ(a, b);
}

TEST(PtrIteratorTest, Distance)
{
    int arr[] = {1, 2, 3, 4};
    PtrIterator<int> a(arr);
    PtrIterator<int> b(arr + 3);

    EXPECT_EQ(b - a, 3);
    EXPECT_EQ(a - b, -3);

    PtrIterator<const int> cit(arr + 2);
    EXPECT_EQ(cit - a, 2);
    EXPECT_EQ(a - cit, -2);
}

TEST(PtrIteratorTest, IndexOperator)
{
    int arr[] = {5, 6, 7, 8};
    PtrIterator<int> it(arr);

    EXPECT_EQ(it[0], 5);
    EXPECT_EQ(it[2], 7);
}

TEST(PtrIteratorTest, EqualityAndInequality)
{
    int arr[] = {1, 2};
    PtrIterator<int> a(arr);
    PtrIterator<int> b(arr);
    PtrIterator<int> c(arr + 1);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a == c);
}

TEST(PtrIteratorTest, RelationalOperators)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> a(arr);
    PtrIterator<int> aCopy(arr);
    PtrIterator<int> b(arr + 1);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a <= aCopy);
    EXPECT_TRUE(a >= aCopy);

    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b <= a);
    EXPECT_FALSE(a >= b);
}

TEST(PtrIteratorTest, ComparisonOperatorsBetweenConstAndNormal)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> a(arr);
    PtrIterator<const int> caCopy(arr);
    PtrIterator<const int> b(arr + 1);

    EXPECT_TRUE(a == caCopy);
    EXPECT_FALSE(a != caCopy);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a <= caCopy);
    EXPECT_TRUE(a >= caCopy);

    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b <= a);
    EXPECT_FALSE(a >= b);
}

TEST(PtrIteratorTest, NonConstToConstConversion)
{
    int arr[] = {1, 2, 3};

    PtrIterator<int> it(arr);
    PtrIterator<const int> cit = it;  // implicit conversion

    EXPECT_EQ(*cit, 1);
}

TEST(PtrIteratorTest, ConvertedIteratorTracksSameLocation)
{
    int arr[] = {10, 20, 30};

    PtrIterator<int> it(arr + 1);
    PtrIterator<const int> cit = it;

    EXPECT_EQ(*cit, 20);

    ++it;
    ++cit;

    EXPECT_EQ(*it, 30);
    EXPECT_EQ(*cit, 30);
}

// Uncomment to test compile failure
// TEST(PtrIteratorTest, ConstToNonConstShouldNotCompile)
// {
//     const int arr[] = {1, 2, 3};

//     PtrIterator<const int> cit(arr);
//     PtrIterator<int> it = cit;  // should NOT compile
// }

// ============================================================
// ReverseIterator Tests
// ============================================================

TEST(ReverseIteratorTest, BaseConstructorAndDereference)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 3));

    EXPECT_EQ(*rit, 3);
}

TEST(ReverseIteratorTest, DefaultConstructor)
{
    ReverseIterator<PtrIterator<int>> rit;
    SUCCEED();
}

TEST(ReverseIteratorTest, CopyConstructor)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<int>> b(a);
    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, CopyAssignment)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<int>> b;
    b = a;

    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, MoveConstructor)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<int>> b(std::move(a));
    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, MoveAssignment)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<int>> b;
    b = std::move(a);

    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, ArrowOperator)
{
    int arr[] = {10, 20};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 2));

    EXPECT_EQ(rit.operator->(), &arr[1]);
}

TEST(ReverseIteratorTest, PreIncrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 3));

    ++rit;
    EXPECT_EQ(*rit, 2);
}

TEST(ReverseIteratorTest, PostIncrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 3));

    ReverseIterator<PtrIterator<int>> old = rit++;
    EXPECT_EQ(*old, 3);
    EXPECT_EQ(*rit, 2);
}

TEST(ReverseIteratorTest, PreDecrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 2));

    --rit;
    EXPECT_EQ(*rit, 3);
}

TEST(ReverseIteratorTest, PostDecrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 2));

    ReverseIterator<PtrIterator<int>> old = rit--;
    EXPECT_EQ(*old, 2);
    EXPECT_EQ(*rit, 3);
}

TEST(ReverseIteratorTest, Arithmetic)
{
    int arr[] = {1, 2, 3, 4};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 4));

    rit += 2;
    EXPECT_EQ(*rit, 2);

    rit -= 1;
    EXPECT_EQ(*rit, 3);

    ReverseIterator<PtrIterator<int>> r2 = rit + 1;
    EXPECT_EQ(*r2, 2);

    ReverseIterator<PtrIterator<int>> r3 = r2 - 2;
    EXPECT_EQ(*r3, 4);
}

TEST(ReverseIteratorTest, AdditionSymmetry)
{
    int arr[] = {1, 2, 3, 4};

    using It = PtrIterator<int>;
    ReverseIterator<It> rit(It(arr + 4));

    ReverseIterator<It> a = rit + 2;
    ReverseIterator<It> b = 2 + rit;

    EXPECT_EQ(*a, *b);
    EXPECT_EQ(a, b);
}

TEST(ReverseIteratorTest, Distance)
{
    int arr[] = {1, 2, 3, 4};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 4));
    ReverseIterator<PtrIterator<int>> b(PtrIterator<int>(arr + 1));

    EXPECT_EQ(a - b, -3);
    EXPECT_EQ(b - a, 3);

    ReverseIterator<PtrIterator<const int>> crit(PtrIterator<const int>(arr + 2));
    EXPECT_EQ(a - crit, -2);
    EXPECT_EQ(crit - a, 2);
}

TEST(ReverseIteratorTest, IndexOperator)
{
    int arr[] = {10, 20, 30, 40};
    ReverseIterator<PtrIterator<int>> rit(PtrIterator<int>(arr + 4));

    EXPECT_EQ(rit[0], 40);
    EXPECT_EQ(rit[2], 20);
}

TEST(ReverseIteratorTest, Comparisons)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<int>> aCopy(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<int>> b(PtrIterator<int>(arr + 2));

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
    EXPECT_FALSE(a != aCopy);
    EXPECT_TRUE(a == aCopy);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a <= aCopy);
    EXPECT_TRUE(a >= aCopy);

    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b <= a);
    EXPECT_FALSE(a >= b);
}

TEST(ReverseIteratorTest, ComparisonsBetweenConstAndNormal)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<PtrIterator<int>> a(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<const int>> caCopy(PtrIterator<int>(arr + 3));
    ReverseIterator<PtrIterator<const int>> b(PtrIterator<int>(arr + 2));

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != caCopy);
    EXPECT_TRUE(a == caCopy);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a <= caCopy);
    EXPECT_TRUE(a >= caCopy);

    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b <= a);
    EXPECT_FALSE(a >= b);

    EXPECT_FALSE(a < caCopy);
    EXPECT_FALSE(caCopy < a);
}

TEST(ReverseIteratorTest, BaseFunction)
{
    int arr[] = {1, 2, 3};
    PtrIterator<int> it(arr + 2);

    ReverseIterator<PtrIterator<int>> rit(it);

    EXPECT_EQ(rit.Base(), it);
}

TEST(ReverseIteratorTest, NonConstToConstConversion)
{
    int arr[] = {1, 2, 3};

    using It  = PtrIterator<int>;
    using CIt = PtrIterator<const int>;

    ReverseIterator<It> rit(It(arr + 3));
    ReverseIterator<CIt> crit = rit;

    EXPECT_EQ(*crit, 3);
}

TEST(ReverseIteratorTest, ConvertedIteratorTracksSamePosition)
{
    int arr[] = {5, 6, 7};

    using It  = PtrIterator<int>;
    using CIt = PtrIterator<const int>;

    ReverseIterator<It> rit(It(arr + 3));
    ReverseIterator<CIt> crit = rit;

    ++rit;
    ++crit;

    EXPECT_EQ(*rit, 6);
    EXPECT_EQ(*crit, 6);
}

// Uncomment to test compile failure
// TEST(PtrIteratorTest, ConstToNonConstShouldNotCompile)
// {
//     const int arr[] = {1, 2, 3};

//     using It  = PtrIterator<int>;
//     using CIt = PtrIterator<const int>;

//     ReverseIterator<CIt> crit(CIt(arr + 3));
//     ReverseIterator<It> rit = crit; // should NOT compile
// }

// ============================================================
// std::sort (requires RandomAccessIterator)
// ============================================================

TEST(STLCompatibilityTest, SortWithPtrIterator)
{
    int arr[] = {4, 1, 3, 2};

    PtrIterator<int> begin(arr);
    PtrIterator<int> end(arr + 4);

    std::sort(begin, end);

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
}

TEST(STLCompatibilityTest, SortWithReverseIterator)
{
    int arr[] = {1, 2, 3, 4};

    using It = PtrIterator<int>;
    It begin(arr);
    It end(arr + 4);
    
    ReverseIterator<It> rbegin(end);
    ReverseIterator<It> rend(begin);

    std::sort(rbegin, rend);

    // Reverse sort should produce descending in original array
    EXPECT_EQ(arr[0], 4);
    EXPECT_EQ(arr[1], 3);
    EXPECT_EQ(arr[2], 2);
    EXPECT_EQ(arr[3], 1);
}

// ============================================================
// std::accumulate
// ============================================================

TEST(STLCompatibilityTest, Accumulate)
{
    int arr[] = {1, 2, 3, 4};

    PtrIterator<int> begin(arr);
    PtrIterator<int> end(arr + 4);

    int sum = std::accumulate(begin, end, 0);

    EXPECT_EQ(sum, 10);
}

TEST(STLCompatibilityTest, WorksWithSTLAfterConstConversion)
{
    int arr[] = {3, 1, 2};

    PtrIterator<int> it(arr);
    PtrIterator<const int> cit = it;

    int sum = std::accumulate(cit, cit + 3, 0);

    EXPECT_EQ(sum, 6);
}

// ============================================================
// std::copy
// ============================================================

TEST(STLCompatibilityTest, Copy)
{
    int src[] = {1, 2, 3};
    int dst[] = {0, 0, 0};

    std::copy(PtrIterator<int>(src),
              PtrIterator<int>(src + 3),
              PtrIterator<int>(dst));

    EXPECT_EQ(dst[0], 1);
    EXPECT_EQ(dst[1], 2);
    EXPECT_EQ(dst[2], 3);
}

// ============================================================
// std::distance
// ============================================================

TEST(STLCompatibilityTest, Distance)
{
    int arr[] = {1, 2, 3, 4, 5};

    PtrIterator<int> begin = PtrIterator<int>(arr);
    PtrIterator<int> end   = PtrIterator<int>(arr + 5);

    EXPECT_EQ(std::distance(begin, end), 5);
}

// ============================================================
// std::advance
// ============================================================

TEST(STLCompatibilityTest, Advance)
{
    int arr[] = {10, 20, 30};

    PtrIterator<int> it = PtrIterator<int>(arr);

    std::advance(it, 2);

    EXPECT_EQ(*it, 30);
}

// ============================================================
// std::find
// ============================================================

TEST(STLCompatibilityTest, Find)
{
    int arr[] = {5, 10, 15};

    PtrIterator<int> begin = PtrIterator<int>(arr);
    PtrIterator<int> end   = PtrIterator<int>(arr + 3);

    PtrIterator<int> it = std::find(begin, end, 10);

    EXPECT_NE(it, end);
    EXPECT_EQ(*it, 10);
}

// ============================================================
// std::reverse
// ============================================================

TEST(STLCompatibilityTest, Reverse)
{
    int arr[] = {1, 2, 3, 4};

    std::reverse(PtrIterator<int>(arr), PtrIterator<int>(arr + 4));

    EXPECT_EQ(arr[0], 4);
    EXPECT_EQ(arr[1], 3);
    EXPECT_EQ(arr[2], 2);
    EXPECT_EQ(arr[3], 1);
}

// ============================================================
// std::binary_search (requires sorted + random access)
// ============================================================

TEST(STLCompatibilityTest, BinarySearch)
{
    int arr[] = {1, 2, 3, 4, 5};

    bool found = std::binary_search(
        PtrIterator<int>(arr),
        PtrIterator<int>(arr + 5),
        3
    );

    EXPECT_TRUE(found);
}

// ============================================================
// std::lower_bound
// ============================================================

TEST(STLCompatibilityTest, LowerBound)
{
    int arr[] = {1, 2, 4, 5};

    PtrIterator<int> it = std::lower_bound(
        PtrIterator<int>(arr),
        PtrIterator<int>(arr + 4),
        3
    );

    EXPECT_EQ(*it, 4);
}

// ============================================================
// std::nth_element
// ============================================================

TEST(STLCompatibilityTest, NthElement)
{
    int arr[] = {4, 1, 3, 2};

    PtrIterator<int> begin = PtrIterator<int>(arr);
    PtrIterator<int> nth   = PtrIterator<int>(arr + 2);
    PtrIterator<int> end   = PtrIterator<int>(arr + 4);

    std::nth_element(begin, nth, end);

    EXPECT_EQ(arr[2], 3); // third smallest element
}

// ============================================================
// std::is_sorted
// ============================================================

TEST(STLCompatibilityTest, IsSorted)
{
    int sorted[] = {1, 2, 3};
    int unsorted[] = {3, 1, 2};

    EXPECT_TRUE(std::is_sorted(
        PtrIterator<int>(sorted),
        PtrIterator<int>(sorted + 3)
    ));

    EXPECT_FALSE(std::is_sorted(
        PtrIterator<int>(unsorted),
        PtrIterator<int>(unsorted + 3)
    ));
}

// ============================================================
// Interoperability with std::vector (copy in/out)
// ============================================================

TEST(STLCompatibilityTest, InteropWithVector)
{
    int arr[] = {1, 2, 3};

    std::vector<int> v(3);

    std::copy(
        PtrIterator<int>(arr),
        PtrIterator<int>(arr + 3),
        v.begin()
    );

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}