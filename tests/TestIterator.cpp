#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>
#include <vector>

#include <stdads/Iterator.h>

using namespace stdads;

/// @brief Test class used to test creating a custom random access iterator
/// @tparam T Iterator value type
template <typename T>
class TestRandomAccessIterator : public RandomAccessIterator<TestRandomAccessIterator<T>, T> {
public:
    // ===== Constructors =====
    TestRandomAccessIterator() : ptr_(0) {}
    TestRandomAccessIterator(T* ptr) : ptr_(ptr) {}
    TestRandomAccessIterator(const TestRandomAccessIterator&) = default;
    TestRandomAccessIterator(TestRandomAccessIterator&&) = default;
    ~TestRandomAccessIterator() = default;
    TestRandomAccessIterator& operator=(const TestRandomAccessIterator&) = default;
    TestRandomAccessIterator& operator=(TestRandomAccessIterator&&) = default;

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    TestRandomAccessIterator(const TestRandomAccessIterator<U>& other) : ptr_(&(*other)) {} // Const conversion constructor


    // ==== Required by ForwardIterator ====
    void Increment() { ++ptr_; }
    bool Equals(const TestRandomAccessIterator& other) const { return ptr_ == other.ptr_; }
    T& Dereference() const { return *ptr_; }

    // ==== Required by Bidirectional Iterator ====
    void Decrement() { --ptr_; }

    // ==== Required by Random Access Iterator ====
    void Advance(std::ptrdiff_t n) { ptr_ += n; }
    std::ptrdiff_t DistanceTo(const TestRandomAccessIterator& other) const { return ptr_ - other.ptr_; }
    bool LessThan(const TestRandomAccessIterator& other) const { return ptr_ < other.ptr_; }

private:
    T* ptr_;
};

// ============================================================
// TestRandomAccessIterator Tests (covers IteratorBase via CRTP)
// ============================================================

TEST(TestRandomAccessIteratorTest, PointerConstructorAndDereferenceAndArrow)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> it(arr);

    EXPECT_EQ(*it, 1);
    EXPECT_EQ(it.operator->(), &arr[0]);
}

TEST(TestRandomAccessIteratorTest, DefaultConstructor)
{
    TestRandomAccessIterator<int> it;

    // Cannot safely dereference, but should be constructible
    SUCCEED();
}

TEST(TestRandomAccessIteratorTest, CopyConstructor)
{
    int arr[] = {1, 2};

    TestRandomAccessIterator<int> it(arr);
    TestRandomAccessIterator<int> copy(it);
    EXPECT_EQ(*copy, 1);
}

TEST(TestRandomAccessIteratorTest, CopyAssignment)
{
    int arr[] = {1, 2};

    TestRandomAccessIterator<int> a(arr);
    TestRandomAccessIterator<int> b;
    b = a;

    EXPECT_EQ(*b, 1);
}

TEST(TestRandomAccessIteratorConstructorTest, MoveConstructor)
{
    int arr[] = {9};

    TestRandomAccessIterator<int> it(arr);
    TestRandomAccessIterator<int> moved(std::move(it));

    EXPECT_EQ(*moved, 9);
}

TEST(TestRandomAccessIteratorConstructorTest, MoveAssignment)
{
    int arr[] = {7};

    TestRandomAccessIterator<int> it(arr);
    TestRandomAccessIterator<int> target;

    target = std::move(it);

    EXPECT_EQ(*target, 7);
}

TEST(TestRandomAccessIteratorTest, PreIncrement)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> it(arr);

    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(TestRandomAccessIteratorTest, PostIncrement)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> it(arr);

    TestRandomAccessIterator<int> old = it++;
    EXPECT_EQ(*old, 1);
    EXPECT_EQ(*it, 2);
}

TEST(TestRandomAccessIteratorTest, PreDecrement)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> it(arr + 1);

    --it;
    EXPECT_EQ(*it, 1);
}

TEST(TestRandomAccessIteratorTest, PostDecrement)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> it(arr + 1);

    TestRandomAccessIterator<int> old = it--;
    EXPECT_EQ(*old, 2);
    EXPECT_EQ(*it, 1);
}

TEST(TestRandomAccessIteratorTest, AdvanceAndArithmetic)
{
    int arr[] = {10, 20, 30, 40};
    TestRandomAccessIterator<int> it(arr);

    it += 2;
    EXPECT_EQ(*it, 30);

    it -= 1;
    EXPECT_EQ(*it, 20);

    TestRandomAccessIterator<int> it2 = it + 2;
    EXPECT_EQ(*it2, 40);

    TestRandomAccessIterator<int> it3 = it2 - 3;
    EXPECT_EQ(*it3, 10);
}

TEST(TestRandomAccessIteratorTest, AdditionSymmetry)
{
    int arr[] = {1, 2, 3, 4};

    TestRandomAccessIterator<int> it(arr);

    TestRandomAccessIterator<int> a = it + 2;
    TestRandomAccessIterator<int> b = 2 + it;

    EXPECT_EQ(*a, *b);
    EXPECT_EQ(a, b);
}

TEST(TestRandomAccessIteratorTest, Distance)
{
    int arr[] = {1, 2, 3, 4};
    TestRandomAccessIterator<int> a(arr);
    TestRandomAccessIterator<int> b(arr + 3);

    EXPECT_EQ(b - a, 3);
    EXPECT_EQ(a - b, -3);

    TestRandomAccessIterator<const int> cit(arr + 2);
    EXPECT_EQ(cit - a, 2);
    EXPECT_EQ(a - cit, -2);
}

TEST(TestRandomAccessIteratorTest, IndexOperator)
{
    int arr[] = {5, 6, 7, 8};
    TestRandomAccessIterator<int> it(arr);

    EXPECT_EQ(it[0], 5);
    EXPECT_EQ(it[2], 7);
}

TEST(TestRandomAccessIteratorTest, EqualityAndInequality)
{
    int arr[] = {1, 2};
    TestRandomAccessIterator<int> a(arr);
    TestRandomAccessIterator<int> b(arr);
    TestRandomAccessIterator<int> c(arr + 1);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a == c);
}

TEST(TestRandomAccessIteratorTest, RelationalOperators)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> a(arr);
    TestRandomAccessIterator<int> aCopy(arr);
    TestRandomAccessIterator<int> b(arr + 1);

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

TEST(TestRandomAccessIteratorTest, ComparisonOperatorsBetweenConstAndNormal)
{
    int arr[] = {1, 2, 3};
    TestRandomAccessIterator<int> a(arr);
    TestRandomAccessIterator<const int> caCopy(arr);
    TestRandomAccessIterator<const int> b(arr + 1);

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

TEST(TestRandomAccessIteratorTest, NonConstToConstConversion)
{
    int arr[] = {1, 2, 3};

    TestRandomAccessIterator<int> it(arr);
    TestRandomAccessIterator<const int> cit = it;  // implicit conversion

    EXPECT_EQ(*cit, 1);
}

TEST(TestRandomAccessIteratorTest, ConvertedIteratorTracksSameLocation)
{
    int arr[] = {10, 20, 30};

    TestRandomAccessIterator<int> it(arr + 1);
    TestRandomAccessIterator<const int> cit = it;

    EXPECT_EQ(*cit, 20);

    ++it;
    ++cit;

    EXPECT_EQ(*it, 30);
    EXPECT_EQ(*cit, 30);
}

// Uncomment to test compile failure
// TEST(TestRandomAccessIteratorTest, ConstToNonConstShouldNotCompile)
// {
//     const int arr[] = {1, 2, 3};

//     TestRandomAccessIterator<const int> cit(arr);
//     TestRandomAccessIterator<int> it = cit;  // should NOT compile
// }

// ============================================================
// ReverseIterator Tests
// ============================================================

TEST(ReverseIteratorTest, BaseConstructorAndDereference)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 3));

    EXPECT_EQ(*rit, 3);
}

TEST(ReverseIteratorTest, DefaultConstructor)
{
    ReverseIterator<TestRandomAccessIterator<int>> rit;
    SUCCEED();
}

TEST(ReverseIteratorTest, CopyConstructor)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<int>> b(a);
    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, CopyAssignment)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<int>> b;
    b = a;

    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, MoveConstructor)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<int>> b(std::move(a));
    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, MoveAssignment)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<int>> b;
    b = std::move(a);

    EXPECT_EQ(*b, 3);
}

TEST(ReverseIteratorTest, ArrowOperator)
{
    int arr[] = {10, 20};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 2));

    EXPECT_EQ(rit.operator->(), &arr[1]);
}

TEST(ReverseIteratorTest, PreIncrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 3));

    ++rit;
    EXPECT_EQ(*rit, 2);
}

TEST(ReverseIteratorTest, PostIncrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 3));

    ReverseIterator<TestRandomAccessIterator<int>> old = rit++;
    EXPECT_EQ(*old, 3);
    EXPECT_EQ(*rit, 2);
}

TEST(ReverseIteratorTest, PreDecrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 2));

    --rit;
    EXPECT_EQ(*rit, 3);
}

TEST(ReverseIteratorTest, PostDecrement)
{
    int arr[] = {1, 2, 3};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 2));

    ReverseIterator<TestRandomAccessIterator<int>> old = rit--;
    EXPECT_EQ(*old, 2);
    EXPECT_EQ(*rit, 3);
}

TEST(ReverseIteratorTest, Arithmetic)
{
    int arr[] = {1, 2, 3, 4};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 4));

    rit += 2;
    EXPECT_EQ(*rit, 2);

    rit -= 1;
    EXPECT_EQ(*rit, 3);

    ReverseIterator<TestRandomAccessIterator<int>> r2 = rit + 1;
    EXPECT_EQ(*r2, 2);

    ReverseIterator<TestRandomAccessIterator<int>> r3 = r2 - 2;
    EXPECT_EQ(*r3, 4);
}

TEST(ReverseIteratorTest, AdditionSymmetry)
{
    int arr[] = {1, 2, 3, 4};

    using It = TestRandomAccessIterator<int>;
    ReverseIterator<It> rit(It(arr + 4));

    ReverseIterator<It> a = rit + 2;
    ReverseIterator<It> b = 2 + rit;

    EXPECT_EQ(*a, *b);
    EXPECT_EQ(a, b);
}

TEST(ReverseIteratorTest, Distance)
{
    int arr[] = {1, 2, 3, 4};

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 4));
    ReverseIterator<TestRandomAccessIterator<int>> b(TestRandomAccessIterator<int>(arr + 1));

    EXPECT_EQ(a - b, -3);
    EXPECT_EQ(b - a, 3);

    ReverseIterator<TestRandomAccessIterator<const int>> crit(TestRandomAccessIterator<const int>(arr + 2));
    EXPECT_EQ(a - crit, -2);
    EXPECT_EQ(crit - a, 2);
}

TEST(ReverseIteratorTest, IndexOperator)
{
    int arr[] = {10, 20, 30, 40};
    ReverseIterator<TestRandomAccessIterator<int>> rit(TestRandomAccessIterator<int>(arr + 4));

    EXPECT_EQ(rit[0], 40);
    EXPECT_EQ(rit[2], 20);
}

TEST(ReverseIteratorTest, Comparisons)
{
    int arr[] = {1, 2, 3};

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<int>> aCopy(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<int>> b(TestRandomAccessIterator<int>(arr + 2));

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

    ReverseIterator<TestRandomAccessIterator<int>> a(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<const int>> caCopy(TestRandomAccessIterator<int>(arr + 3));
    ReverseIterator<TestRandomAccessIterator<const int>> b(TestRandomAccessIterator<int>(arr + 2));

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
    TestRandomAccessIterator<int> it(arr + 2);

    ReverseIterator<TestRandomAccessIterator<int>> rit(it);

    EXPECT_EQ(rit.Base(), it);
}

TEST(ReverseIteratorTest, NonConstToConstConversion)
{
    int arr[] = {1, 2, 3};

    using It  = TestRandomAccessIterator<int>;
    using CIt = TestRandomAccessIterator<const int>;

    ReverseIterator<It> rit(It(arr + 3));
    ReverseIterator<CIt> crit = rit;

    EXPECT_EQ(*crit, 3);
}

TEST(ReverseIteratorTest, ConvertedIteratorTracksSamePosition)
{
    int arr[] = {5, 6, 7};

    using It  = TestRandomAccessIterator<int>;
    using CIt = TestRandomAccessIterator<const int>;

    ReverseIterator<It> rit(It(arr + 3));
    ReverseIterator<CIt> crit = rit;

    ++rit;
    ++crit;

    EXPECT_EQ(*rit, 6);
    EXPECT_EQ(*crit, 6);
}

// Uncomment to test compile failure
// TEST(TestRandomAccessIteratorTest, ConstToNonConstShouldNotCompile)
// {
//     const int arr[] = {1, 2, 3};

//     using It  = TestRandomAccessIterator<int>;
//     using CIt = TestRandomAccessIterator<const int>;

//     ReverseIterator<CIt> crit(CIt(arr + 3));
//     ReverseIterator<It> rit = crit; // should NOT compile
// }

// ============================================================
// std::sort (requires RandomAccessIterator)
// ============================================================

TEST(STLCompatibilityTest, SortWithTestRandomAccessIterator)
{
    int arr[] = {4, 1, 3, 2};

    TestRandomAccessIterator<int> begin(arr);
    TestRandomAccessIterator<int> end(arr + 4);

    std::sort(begin, end);

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
}

TEST(STLCompatibilityTest, SortWithReverseIterator)
{
    int arr[] = {1, 2, 3, 4};

    using It = TestRandomAccessIterator<int>;
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

    TestRandomAccessIterator<int> begin(arr);
    TestRandomAccessIterator<int> end(arr + 4);

    int sum = std::accumulate(begin, end, 0);

    EXPECT_EQ(sum, 10);
}

TEST(STLCompatibilityTest, WorksWithSTLAfterConstConversion)
{
    int arr[] = {3, 1, 2};

    TestRandomAccessIterator<int> it(arr);
    TestRandomAccessIterator<const int> cit = it;

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

    std::copy(TestRandomAccessIterator<int>(src),
              TestRandomAccessIterator<int>(src + 3),
              TestRandomAccessIterator<int>(dst));

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

    TestRandomAccessIterator<int> begin = TestRandomAccessIterator<int>(arr);
    TestRandomAccessIterator<int> end   = TestRandomAccessIterator<int>(arr + 5);

    EXPECT_EQ(std::distance(begin, end), 5);
}

// ============================================================
// std::advance
// ============================================================

TEST(STLCompatibilityTest, Advance)
{
    int arr[] = {10, 20, 30};

    TestRandomAccessIterator<int> it = TestRandomAccessIterator<int>(arr);

    std::advance(it, 2);

    EXPECT_EQ(*it, 30);
}

// ============================================================
// std::find
// ============================================================

TEST(STLCompatibilityTest, Find)
{
    int arr[] = {5, 10, 15};

    TestRandomAccessIterator<int> begin = TestRandomAccessIterator<int>(arr);
    TestRandomAccessIterator<int> end   = TestRandomAccessIterator<int>(arr + 3);

    TestRandomAccessIterator<int> it = std::find(begin, end, 10);

    EXPECT_NE(it, end);
    EXPECT_EQ(*it, 10);
}

// ============================================================
// std::reverse
// ============================================================

TEST(STLCompatibilityTest, Reverse)
{
    int arr[] = {1, 2, 3, 4};

    std::reverse(TestRandomAccessIterator<int>(arr), TestRandomAccessIterator<int>(arr + 4));

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
        TestRandomAccessIterator<int>(arr),
        TestRandomAccessIterator<int>(arr + 5),
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

    TestRandomAccessIterator<int> it = std::lower_bound(
        TestRandomAccessIterator<int>(arr),
        TestRandomAccessIterator<int>(arr + 4),
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

    TestRandomAccessIterator<int> begin = TestRandomAccessIterator<int>(arr);
    TestRandomAccessIterator<int> nth   = TestRandomAccessIterator<int>(arr + 2);
    TestRandomAccessIterator<int> end   = TestRandomAccessIterator<int>(arr + 4);

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
        TestRandomAccessIterator<int>(sorted),
        TestRandomAccessIterator<int>(sorted + 3)
    ));

    EXPECT_FALSE(std::is_sorted(
        TestRandomAccessIterator<int>(unsorted),
        TestRandomAccessIterator<int>(unsorted + 3)
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
        TestRandomAccessIterator<int>(arr),
        TestRandomAccessIterator<int>(arr + 3),
        v.begin()
    );

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}