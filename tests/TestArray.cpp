#include <gtest/gtest.h>
#include <stdads/Array.h>


// ===== Helper Type =====

struct TestObject {
    static int constructed;
    static int destructed;
    int value;

    TestObject()
    : value(42) {
        ++constructed;
    }

    TestObject(int val)
    : value(val) {
        ++constructed;
    }

    ~TestObject() {
        ++destructed;
    }
};

int TestObject::constructed = 0;
int TestObject::destructed = 0;


// ===== Default Constructor =====

TEST(ArrayTest, DefaultConstructor_ClassType_CallsConstructors) {
    TestObject::constructed = 0;
    stdads::Array<TestObject, 5> arr;
    EXPECT_EQ(TestObject::constructed, 5);
}

TEST(ArrayTest, DefaultConstructor_PrimitiveType_NoCrash) {
    // We cannot reliably test "garbage values"
    // but we can ensure construction succeeds
    stdads::Array<int, 5> arr;
    SUCCEED();
}


// ===== Destructor =====

TEST(ArrayTest, Destructor_DestructsAllElements) {
    TestObject::constructed = 0;
    TestObject::destructed = 0;
    stdads::Array<TestObject, 5>* arr = new stdads::Array<TestObject, 5>();
    EXPECT_EQ(TestObject::constructed, 5);
    EXPECT_EQ(TestObject::destructed, 0);

    delete arr;
    EXPECT_EQ(TestObject::destructed, 5);

    stdads::Array<int, 5>* arr2 = new stdads::Array<int, 5>();
    delete arr2;
}


// ===== Aggregate initialization constructor =====

TEST(ArrayTest, AggregateInitialization_Zeros) {
    stdads::Array<int, 1000> arr{};
    for (std::size_t i = 0 ; i < 1000; ++i)
    {
        EXPECT_EQ(0, arr[i]);
    }

    TestObject::constructed = 0;
    stdads::Array<TestObject, 1000> arr2{};
    EXPECT_EQ(1000, TestObject::constructed);
    for (std::size_t i = 0 ; i < 1000; ++i)
    {
        EXPECT_EQ(42, arr2[i].value);
    }
}

TEST(ArrayTest, AggregateInitialization_Partial) {
    std::array<int, 3> arr1{1};
    EXPECT_EQ(1, arr1[0]);
    EXPECT_EQ(0, arr1[1]);
    EXPECT_EQ(0, arr1[2]);

    std::array<int, 3> arr2{1, 2};
    EXPECT_EQ(1, arr2[0]);
    EXPECT_EQ(2, arr2[1]);
    EXPECT_EQ(0, arr2[2]);

    TestObject::constructed = 0;
    stdads::Array<TestObject, 3> arr3{TestObject(10)};
    EXPECT_EQ(3, TestObject::constructed);
    EXPECT_EQ(10, arr3[0].value);
    EXPECT_EQ(42, arr3[1].value);
    EXPECT_EQ(42, arr3[2].value);

    // TestObject::constructed = 0;
    // std::array<TestObject, 3> arr4{TestObject(10)};
    // EXPECT_EQ(3, TestObject::constructed);
    // EXPECT_EQ(10, arr4[0].value);
    // EXPECT_EQ(42, arr4[1].value);
    // EXPECT_EQ(42, arr4[2].value);

    stdads::Array<TestObject, 3> arr4{TestObject(10), TestObject(11)};
    EXPECT_EQ(6, TestObject::constructed);
    EXPECT_EQ(10, arr4[0].value);
    EXPECT_EQ(11, arr4[1].value);
    EXPECT_EQ(42, arr4[2].value);
}

TEST(ArrayTest, AggregateInitialization_Full) {
    stdads::Array<int, 3> arr{1, 2, 3};
    EXPECT_EQ(1, arr[0]);
    EXPECT_EQ(2, arr[1]);
    EXPECT_EQ(3, arr[2]);
}


// ==== Other Implicitly defined functions ====

TEST(ArrayTest, CopyConstructor)
{
    stdads::Array<int, 5> a{11};
    stdads::Array<int, 5> b(a);
    EXPECT_EQ(b[0], 11);
}

TEST(ArrayTest, CopyAssignment)
{
    stdads::Array<int, 5> a{11};
    stdads::Array<int, 5> b;
    b = a;
    EXPECT_EQ(b[0], 11);
}

TEST(ArrayTest, MoveConstructor)
{
    stdads::Array<int, 5> a{11};
    stdads::Array<int, 5> b(std::move(a));
    EXPECT_EQ(b[0], 11);
}

TEST(ArrayTest, MoveAssignment)
{
    stdads::Array<int, 5> a{11};
    stdads::Array<int, 5> b;
    b = std::move(a);
    EXPECT_EQ(b[0], 11);
}


// ===== Size / Empty =====

TEST(ArrayTest, Size_ReturnsCorrectValue) {
    stdads::Array<int, 7> arr;
    EXPECT_EQ(arr.Size(), 7);

    stdads::Array<int, 0> emptyArr;
    EXPECT_EQ(emptyArr.Size(), 0);
}

TEST(ArrayTest, Empty_ReturnsFalseForNonZeroSize) {
    stdads::Array<int, 3> arr;
    EXPECT_FALSE(arr.Empty());
}

TEST(ArrayTest, Empty_ReturnsTrueForZeroSize) {
    stdads::Array<int, 0> arr;
    EXPECT_TRUE(arr.Empty());
}


// ===== operator[] =====

TEST(ArrayTest, OperatorIndex_ReadWrite) {
    stdads::Array<int, 3> arr;
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, OperatorIndex_ConstAccess) {
    stdads::Array<int, 2> arr;
    arr[0] = 5;
    arr[1] = 6;

    const stdads::Array<int, 2>& carr = arr;

    EXPECT_EQ(carr[0], 5);
    EXPECT_EQ(carr[1], 6);
}


// ===== At() =====

TEST(ArrayTest, At_ValidAccess) {
    stdads::Array<int, 3> arr;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    EXPECT_EQ(arr.At(0), 1);
    EXPECT_EQ(arr.At(1), 2);
    EXPECT_EQ(arr.At(2), 3);
}

TEST(ArrayTest, At_ConstAccess) {
    stdads::Array<int, 2> arr;
    arr[0] = 10;
    arr[1] = 20;

    const stdads::Array<int, 2>& carr = arr;

    EXPECT_EQ(carr.At(0), 10);
    EXPECT_EQ(carr.At(1), 20);
}

TEST(ArrayTest, At_OutOfRange_Throws) {
    stdads::Array<int, 3> arr;

    EXPECT_THROW(arr.At(3), std::out_of_range);
    EXPECT_THROW(arr.At(100), std::out_of_range);

    try {
        arr.At(3);
    }
    catch (const std::out_of_range& e) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }

    try {
        arr.At(100);
    }
    catch (const std::out_of_range& e) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}

TEST(ArrayTest, At_ConstOutOfRange_Throws) {
    stdads::Array<int, 3> arr;
    const stdads::Array<int, 3>& carr = arr;

    EXPECT_THROW(carr.At(3), std::out_of_range);

    try {
        carr.At(3);
    }
    catch (const std::out_of_range& e) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}


// ===== Data() =====

TEST(ArrayTest, Data_ReturnsPointerToFirstElement) {
    stdads::Array<int, 3> arr;

    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    int* ptr = arr.Data();

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
}

TEST(ArrayTest, Data_ConstVersion) {
    stdads::Array<int, 2> arr;
    arr[0] = 7;
    arr[1] = 8;

    const stdads::Array<int, 2>& carr = arr;

    const int* ptr = carr.Data();

    EXPECT_EQ(ptr[0], 7);
    EXPECT_EQ(ptr[1], 8);
}


// ===== Back() =====

TEST(ArrayTest, FrontBack) {
    stdads::Array<int, 3> arr{1, 2, 3};
    const stdads::Array<int, 3> constArr{1, 2, 3};

    EXPECT_EQ(arr.Front(), 1);
    EXPECT_EQ(arr.Back(), 3);
    EXPECT_EQ(constArr.Front(), 1);
    EXPECT_EQ(constArr.Back(), 3);
}


// ===== Copy Constructor =====

TEST(ArrayTest, CopyConstructor_CopiesElements) {
    stdads::Array<int, 3> arr;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    stdads::Array<int, 3> copy(arr);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);

    stdads::Array<int, 3> copy2 = arr;
    EXPECT_EQ(copy2[0], 1);
    EXPECT_EQ(copy2[1], 2);
    EXPECT_EQ(copy2[2], 3);
}


// ===== Assignment Operator =====

TEST(ArrayTest, AssignmentOperator_CopiesElements) {
    stdads::Array<int, 3> arr1;
    arr1[0] = 10;
    arr1[1] = 20;
    arr1[2] = 30;

    stdads::Array<int, 3> arr2;
    arr2 = arr1;
    EXPECT_EQ(arr2[0], 10);
    EXPECT_EQ(arr2[1], 20);
    EXPECT_EQ(arr2[2], 30);

    EXPECT_NE(arr1.Data(), arr2.Data());
}

TEST(ArrayTest, AssignmentOperator_SelfAssignment) {
    stdads::Array<int, 3> arr;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    arr = arr;

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}


// ===== Begin() / End() =====

TEST(ArrayIteratorTest, BeginPointsToFirstElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};
    stdads::Array<int, 3>::Iterator it = arr.Begin();
    stdads::Array<int, 3>::ConstIterator cit = arr.CBegin();
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*cit, 1);
}

TEST(ArrayIteratorTest, EndPointsPastLastElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};
    const stdads::Array<int, 3>::Iterator begin = arr.Begin();
    stdads::Array<int, 3>::Iterator end = arr.End();
    stdads::Array<int, 3>::ConstIterator cend = arr.CEnd();
    EXPECT_EQ(end - begin, 3);
    EXPECT_EQ(begin - end, -3);
    EXPECT_EQ(cend - begin, 3);
    EXPECT_EQ(begin - cend, -3);

    // end is not dereferenceable. Ensure decrementing gives us the last element
    EXPECT_EQ(*--end, 3);
    EXPECT_EQ(*--cend, 3);
}

TEST(ArrayIteratorTest, IterateForwardFromBeginToEnd)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    int expected[] = {1, 2, 3};
    int i = 0;
    int j = 0;

    for (stdads::Array<int, 3>::Iterator it = arr.Begin(); it != arr.End(); ++it)
    {
        EXPECT_EQ(*it, expected[i++]);
    }
    for (stdads::Array<int, 3>::ConstIterator cit = arr.CBegin(); cit != arr.CEnd(); ++cit)
    {
        EXPECT_EQ(*cit, expected[j++]);
    }
    
    EXPECT_EQ(3, i);
    EXPECT_EQ(3, j);
}

TEST(ArrayIteratorTest, ConstIterationMatchesNonConst)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    int i = 0;
    stdads::Array<int, 3>::Iterator it = arr.Begin();
    stdads::Array<int, 3>::ConstIterator cit = arr.CBegin();
    for (; it != arr.End() && cit != arr.CEnd(); ++it, ++cit)
    {
        EXPECT_EQ(*it, *cit);
        ++i;
    }
    EXPECT_EQ(3, i);
}

TEST(ArrayIteratorTest, BeginEqualsEndForEmptyArray)
{
    stdads::Array<int, 0> arr;
    EXPECT_EQ(arr.Begin(), arr.End());
    EXPECT_EQ(arr.CBegin(), arr.CEnd());
}

TEST(ArrayIteratorTest, ConvertIteratorToConstIterator)
{
    stdads::Array<int, 3> arr{1,2,3};

    stdads::Array<int, 3>::Iterator it = arr.Begin();
    stdads::Array<int, 3>::ConstIterator cit = it;

    const stdads::Array<int, 3> carr{1,2,3};
    stdads::Array<int, 3>::ConstIterator cbegin = carr.Begin(); // must be const iterator
    stdads::Array<int, 3>::ConstIterator cend = carr.End(); // must be const iterator
}

TEST(ArrayIteratorTest, SingleElementForwardIteration)
{
    stdads::Array<int, 1> arr{99};

    stdads::Array<int, 1>::Iterator it = arr.Begin();
    stdads::Array<int, 1>::ConstIterator cit = arr.CBegin();
    EXPECT_EQ(*it, 99);
    EXPECT_EQ(*cit, 99);

    ++it;
    ++cit;
    EXPECT_EQ(it, arr.End());
    EXPECT_EQ(cit, arr.CEnd());
}


// ===== RBegin() / REnd() =====

TEST(ArrayIteratorTest, RBeginPointsToLastElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};
    stdads::Array<int, 3>::ReverseIterator rit = arr.RBegin();
    stdads::Array<int, 3>::ConstReverseIterator crit = arr.CRBegin();
    EXPECT_EQ(*rit, 3);
    EXPECT_EQ(*crit, 3);
}

TEST(ArrayIteratorTest, REndPointsBeforeFirstElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    const stdads::Array<int, 3>::ReverseIterator rbegin = arr.RBegin();
    stdads::Array<int, 3>::ReverseIterator rend = arr.REnd();
    stdads::Array<int, 3>::ConstReverseIterator crend = arr.CREnd();
    EXPECT_EQ(rend - rbegin, 3);
    EXPECT_EQ(rbegin - rend, -3);
    EXPECT_EQ(crend - rbegin, 3);
    EXPECT_EQ(rbegin - crend, -3);

    // end is not dereferenceable. Ensure decrementing gives us the last element
    EXPECT_EQ(*--rend, 1);
    EXPECT_EQ(*--crend, 1);
}

TEST(ArrayIteratorTest, IterateReverseFromRBeginToREnd)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    int expected[] = {3, 2, 1};
    int i = 0;
    int j = 0;

    for (stdads::Array<int, 3>::ReverseIterator rit = arr.RBegin(); rit != arr.REnd(); ++rit)
    {
        EXPECT_EQ(*rit, expected[i++]);
    }
    for (stdads::Array<int, 3>::ConstReverseIterator crit = arr.CRBegin(); crit != arr.CREnd(); ++crit)
    {
        EXPECT_EQ(*crit, expected[j++]);
    }
    
    EXPECT_EQ(3, i);
    EXPECT_EQ(3, j);
}

TEST(ArrayIteratorTest, ConstReverseIterationMatchesNonConst)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    int i = 0;
    stdads::Array<int, 3>::ReverseIterator rit = arr.RBegin();
    stdads::Array<int, 3>::ConstReverseIterator crit = arr.CRBegin();
    for (; rit != arr.REnd() && crit != arr.CREnd(); ++rit, ++crit)
    {
        EXPECT_EQ(*rit, *crit);
        ++i;
    }
    EXPECT_EQ(3, i);
}

TEST(ArrayIteratorTest, RBeginEqualsREndForEmptyArray)
{
    stdads::Array<int, 0> arr;
    EXPECT_EQ(arr.RBegin(), arr.REnd());
    EXPECT_EQ(arr.CRBegin(), arr.CREnd());
}

TEST(ArrayIteratorTest, ConvertReverseIteratorToConstReverseIterator)
{
    stdads::Array<int, 3> arr{1,2,3};

    stdads::Array<int, 3>::ReverseIterator rit = arr.RBegin();
    stdads::Array<int, 3>::ConstReverseIterator crit = rit;

    const stdads::Array<int, 3> carr{1,2,3};
    stdads::Array<int, 3>::ConstReverseIterator crbegin = carr.RBegin(); // must be const iterator
    stdads::Array<int, 3>::ConstReverseIterator crend = carr.REnd(); // must be const iterator
}

TEST(ArrayIteratorTest, ReverseIteratorBaseRelationship)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    stdads::Array<int, 3>::ReverseIterator rit = arr.RBegin();
    stdads::Array<int, 3>::Iterator base = rit.Base();

    // Base() should point to End()
    EXPECT_EQ(base, arr.End());
}

TEST(ArrayIteratorTest, SingleElementReverseIteration)
{
    stdads::Array<int, 1> arr{99};

    stdads::Array<int, 1>::ReverseIterator rit = arr.RBegin();
    stdads::Array<int, 1>::ConstReverseIterator crit = arr.CRBegin();
    EXPECT_EQ(*rit, 99);
    EXPECT_EQ(*crit, 99);

    ++rit;
    ++crit;
    EXPECT_EQ(rit, arr.REnd());
    EXPECT_EQ(crit, arr.CREnd());
}


// ==== Fill() ====
TEST(ArrayFillTest, FillsEmptyArray)
{
    stdads::Array<int, 5> arr;
    arr.Fill(42);

    for (std::size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST(ArrayFillTest, FillsEmptyArrayClassType)
{
    stdads::Array<TestObject, 5> arr;
    const TestObject testObj(10);
    arr.Fill(testObj);

    for (std::size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(arr[i].value, 10);
    }
}

TEST(ArrayFillTest, FillsAllElementsOverrides)
{
    stdads::Array<int, 5> arr{1, 2, 3, 4, 5};
    for (std::size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(arr[i], i + 1);
    }

    arr.Fill(42);

    for (std::size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(arr[i], 42);
    }
}


// ==== Swap() ====

TEST(ArraySwapTest, Swaps)
{
    stdads::Array<int, 3> a = {0, 1, 2};
    stdads::Array<int, 3> b = {3, 4, 5};
    a.Swap(b);

    for (std::size_t i = 0; i < 3; ++i)
    {
        EXPECT_EQ(a[i], 3 + i);
        EXPECT_EQ(b[i], i);
    }
}

TEST(ArraySwapTest, SwapDoesNotAlias)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{4, 5, 6};

    int* a_ptr = &a[0];
    int* b_ptr = &b[0];

    a.Swap(b);

    // After swap, pointers should still refer to their own storage
    EXPECT_EQ(&a[0], a_ptr);
    EXPECT_EQ(&b[0], b_ptr);
}


// ==== operator+ ====

// ==============================
// Basic Functionality
// ==============================

TEST(ArrayConcatTest, BasicConcatenation)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 2> b{4, 5};

    auto result = a + b;

    static_assert(std::is_same<decltype(result), stdads::Array<int, 5>>::value, "Wrong return type");

    EXPECT_EQ(1, result[0]);
    EXPECT_EQ(2, result[1]);
    EXPECT_EQ(3, result[2]);
    EXPECT_EQ(4, result[3]);
    EXPECT_EQ(5, result[4]);
}

// ==============================
// Different Sizes
// ==============================

TEST(ArrayConcatTest, DifferentSizes)
{
    stdads::Array<int, 1> a{42};
    stdads::Array<int, 4> b{1, 2, 3, 4};

    auto result = a + b;

    EXPECT_EQ(42, result[0]);
    EXPECT_EQ(1, result[1]);
    EXPECT_EQ(2, result[2]);
    EXPECT_EQ(3, result[3]);
    EXPECT_EQ(4, result[4]);
}

// ==============================
// Empty Arrays
// ==============================

TEST(ArrayConcatTest, LeftEmpty)
{
    stdads::Array<int, 0> a;
    stdads::Array<int, 3> b{1, 2, 3};

    stdads::Array<int, 3> result = a + b;

    EXPECT_EQ(1, result[0]);
    EXPECT_EQ(2, result[1]);
    EXPECT_EQ(3, result[2]);
}

TEST(ArrayConcatTest, RightEmpty)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 0> b;

    stdads::Array<int, 3> result = a + b;

    EXPECT_EQ(1, result[0]);
    EXPECT_EQ(2, result[1]);
    EXPECT_EQ(3, result[2]);
}

// Uncomment to confirm compile error
// TEST(ArrayConcatTest, BothEmpty)
// {
//     stdads::Array<int, 0> a;
//     stdads::Array<int, 0> b;
//     auto result = a + b;
// }

// ==============================
// Order Preservation
// ==============================

TEST(ArrayConcatTest, OrderPreserved)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{4, 5, 6};

    auto result = a + b;

    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(a[i], result[i]);

    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(b[i], result[i + 3]);
}

// ==============================
// Chaining
// ==============================

TEST(ArrayConcatTest, Chaining)
{
    stdads::Array<int, 1> a{1};
    stdads::Array<int, 1> b{2};
    stdads::Array<int, 1> c{3};

    auto result = a + b + c;

    EXPECT_EQ(1, result[0]);
    EXPECT_EQ(2, result[1]);
    EXPECT_EQ(3, result[2]);
}

// ==============================
// Non-Trivial Type
// ==============================

TEST(ArrayConcatTest, NonTrivialType)
{
    stdads::Array<TestObject, 2> a{TestObject(1), TestObject(2)};
    stdads::Array<TestObject, 2> b{TestObject(3), TestObject(4)};

    TestObject::constructed = 0;
    auto result = a + b;

    EXPECT_EQ(4, TestObject::constructed);
    EXPECT_EQ(1, result[0].value);
    EXPECT_EQ(2, result[1].value);
    EXPECT_EQ(3, result[2].value);
    EXPECT_EQ(4, result[3].value);
}




// === operator== / operator!= ====

TEST(ArrayRelationalTest, EqualitySameValues)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 3};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(ArrayRelationalTest, InequalityDifferentValues)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 4};

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// ==============================
// Less Than (Lexicographical)
// ==============================

TEST(ArrayRelationalTest, LessThanBasic)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 4};

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(ArrayRelationalTest, LessThanFirstElement)
{
    stdads::Array<int, 3> a{0, 9, 9};
    stdads::Array<int, 3> b{1, 0, 0};

    EXPECT_TRUE(a < b);
}

TEST(ArrayRelationalTest, LessThanMiddleElement)
{
    stdads::Array<int, 3> a{1, 2, 9};
    stdads::Array<int, 3> b{1, 3, 0};

    EXPECT_TRUE(a < b);
}

// ==============================
// Greater Than
// ==============================

TEST(ArrayRelationalTest, GreaterThanBasic)
{
    stdads::Array<int, 3> a{1, 2, 4};
    stdads::Array<int, 3> b{1, 2, 3};

    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
}

// ==============================
// Less Than or Equal
// ==============================

TEST(ArrayRelationalTest, LessEqualEqualCase)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 3};

    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b <= a);
}

TEST(ArrayRelationalTest, LessEqualLessCase)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 4};

    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
}

// ==============================
// Greater Than or Equal
// ==============================

TEST(ArrayRelationalTest, GreaterEqualEqualCase)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 3};

    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b >= a);
}

TEST(ArrayRelationalTest, GreaterEqualGreaterCase)
{
    stdads::Array<int, 3> a{1, 2, 4};
    stdads::Array<int, 3> b{1, 2, 3};

    EXPECT_TRUE(a >= b);
    EXPECT_FALSE(b >= a);
}

// ==============================
// All Operators Consistency
// ==============================

TEST(ArrayRelationalTest, OperatorConsistency)
{
    stdads::Array<int, 3> a{1, 2, 3};
    stdads::Array<int, 3> b{1, 2, 4};

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a >= b);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// ==============================
// Self Comparison
// ==============================

TEST(ArrayRelationalTest, SelfComparison)
{
    stdads::Array<int, 3> a{1, 2, 3};

    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a != a);
    EXPECT_FALSE(a < a);
    EXPECT_FALSE(a > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a >= a);
}

// ==============================
// Larger Array Lexicographical
// ==============================

TEST(ArrayRelationalTest, LargerArrayComparison)
{
    stdads::Array<int, 5> a{1, 2, 3, 4, 5};
    stdads::Array<int, 5> b{1, 2, 3, 4, 6};

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
}

// ==============================
// Edge Case: All Equal Except Last
// ==============================

TEST(ArrayRelationalTest, LastElementDifference)
{
    stdads::Array<int, 4> a{1, 2, 3, 4};
    stdads::Array<int, 4> b{1, 2, 3, 5};

    EXPECT_TRUE(a < b);
}

// ==============================
// Edge Case: Completely Different
// ==============================

TEST(ArrayRelationalTest, CompletelyDifferent)
{
    stdads::Array<int, 3> a{9, 9, 9};
    stdads::Array<int, 3> b{1, 1, 1};

    EXPECT_TRUE(a > b);
}
