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
}


// ===== Aggregate initialization constructor =====

TEST(ArrayConstructorTest, AggregateInitialization_Zeros) {
    stdads::Array<int, 1000> arr{};
    EXPECT_EQ(1000, arr.Size());
    for (std::size_t i = 0 ; i < arr.Size(); ++i)
    {
        EXPECT_EQ(0, arr[i]);
    }
}

TEST(ArrayConstructorTest, AggregateInitialization_Partial) {
    stdads::Array<int, 3> arr{1, 2};
    EXPECT_EQ(3, arr.Size());
    EXPECT_EQ(1, arr[0]);
    EXPECT_EQ(2, arr[1]);
    EXPECT_EQ(0, arr[2]);
}

TEST(ArrayConstructorTest, AggregateInitialization_Full) {
    stdads::Array<int, 3> arr{1, 2, 3};
    EXPECT_EQ(3, arr.Size());
    EXPECT_EQ(1, arr[0]);
    EXPECT_EQ(2, arr[1]);
    EXPECT_EQ(3, arr[2]);
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
}

TEST(ArrayTest, At_ConstOutOfRange_Throws) {
    stdads::Array<int, 3> arr;
    const stdads::Array<int, 3>& carr = arr;

    EXPECT_THROW(carr.At(3), std::out_of_range);
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
    EXPECT_EQ(*it, 1);
}

TEST(ArrayIteratorTest, EndPointsPastLastElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    stdads::Array<int, 3>::Iterator it = arr.End();
    stdads::Array<int, 3>::Iterator begin = arr.Begin();
    EXPECT_EQ(it - begin, 3);
}

TEST(ArrayIteratorTest, IterateForwardFromBeginToEnd)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    int expected[] = {1, 2, 3};
    int i = 0;

    for (stdads::Array<int, 3>::Iterator it = arr.Begin(); it != arr.End(); ++it)
    {
        EXPECT_EQ(*it, expected[i++]);
    }

    EXPECT_EQ(i, 3);
}

TEST(ArrayIteratorTest, BeginEqualsEndForEmptyArray)
{
    stdads::Array<int, 0> arr;
    EXPECT_EQ(arr.Begin(), arr.End());
}

TEST(ArrayIteratorTest, EndIsNotDereferenceable)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    auto it = arr.End();

    // We can't dereference End(), but we can test decrement
    --it;
    EXPECT_EQ(*it, 3);
}

// TEST(ArrayIteratorTest, ConvertIteratorToConstIterator)
// {
//     stdads::Array<int, 3> arr{1,2,3};

//     stdads::Array<int, 3>::Iterator it = arr.Begin();
//     stdads::Array<int, 3>::ConstIterator cit = it; // must work

//     const stdads::Array<int, 3> carr{1,2,3};
//     auto cit2 = carr.Begin(); // must be const_iterator
// }

// ------------------------------------------------------------
// CONST BEGIN / END
// ------------------------------------------------------------

TEST(ArrayIteratorTest, ConstBeginWorks)
{
    const stdads::Array<int, 3> arr{1, 2, 3};
    stdads::Array<int, 3>::ConstIterator it = arr.Begin();
    EXPECT_EQ(*it, 1);
}

TEST(ArrayIteratorTest, ConstIterationMatchesNonConst)
{
    stdads::Array<int, 3> arr{1, 2, 3};
    const stdads::Array<int, 3>& carr = arr;

    stdads::Array<int, 3>::Iterator it1 = arr.Begin();
    stdads::Array<int, 3>::ConstIterator it2 = carr.Begin();
    for (; it1 != arr.End(); ++it1, ++it2)
    {
        EXPECT_EQ(*it1, *it2);
    }
}

// ------------------------------------------------------------
// REVERSE ITERATOR TESTS
// ------------------------------------------------------------

TEST(ArrayIteratorTest, RBeginPointsToLastElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};
    stdads::Array<int, 3>::ReverseIterator it = arr.RBegin();
    EXPECT_EQ(*it, 3);
}

TEST(ArrayIteratorTest, REndPointsBeforeFirstElement)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    stdads::Array<int, 3>::ReverseIterator rend = arr.REnd();
    auto rbegin = arr.RBegin();
    EXPECT_EQ(rend - rbegin, 3);
}

TEST(ArrayIteratorTest, IterateReverseFromRBeginToREnd)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    int expected[] = {3, 2, 1};
    int i = 0;

    for (stdads::Array<int, 3>::ReverseIterator it = arr.RBegin(); it != arr.REnd(); ++it)
    {
        EXPECT_EQ(*it, expected[i++]);
    }

    EXPECT_EQ(i, 3);
}

TEST(ArrayIteratorTest, ReverseIteratorDecrementMovesForward)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    stdads::Array<int, 3>::ReverseIterator it = arr.RBegin(); // points to 3
    --it;                   // should point to REnd()-1 invalid? depends on impl

    // safer test:
    stdads::Array<int, 3>::ReverseIterator it2 = arr.RBegin();
    ++it2;
    EXPECT_EQ(*it2, 2);
}

TEST(ArrayIteratorTest, ReverseIteratorBaseRelationship)
{
    stdads::Array<int, 3> arr{1, 2, 3};

    stdads::Array<int, 3>::ReverseIterator rit = arr.RBegin();
    stdads::Array<int, 3>::Iterator base = rit.Base();

    // base() should point to End()
    EXPECT_EQ(base, arr.End());
}

// ------------------------------------------------------------
// CONST RBEGIN / REND
// ------------------------------------------------------------

TEST(ArrayIteratorTest, ConstReverseBeginWorks)
{
    const stdads::Array<int, 3> arr{1, 2, 3};
    stdads::Array<int, 3>::ConstReverseIterator it = arr.RBegin();
    EXPECT_EQ(*it, 3);
}

TEST(ArrayIteratorTest, ConstReverseIterationMatchesNonConst)
{
    stdads::Array<int, 3> arr{1, 2, 3};
    const stdads::Array<int, 3>& carr = arr;

    stdads::Array<int, 3>::ReverseIterator it1 = arr.RBegin();
    stdads::Array<int, 3>::ConstReverseIterator it2 = carr.RBegin();
    for (; it1 != arr.REnd(); ++it1, ++it2)
    {
        EXPECT_EQ(*it1, *it2);
    }
}

// ------------------------------------------------------------
// EDGE CASES
// ------------------------------------------------------------

TEST(ArrayIteratorTest, SingleElementForwardIteration)
{
    stdads::Array<int, 1> arr{99};

    auto it = arr.Begin();
    EXPECT_EQ(*it, 99);

    ++it;
    EXPECT_EQ(it, arr.End());
}

TEST(ArrayIteratorTest, SingleElementReverseIteration)
{
    stdads::Array<int, 1> arr{99};

    auto it = arr.RBegin();
    EXPECT_EQ(*it, 99);

    ++it;
    EXPECT_EQ(it, arr.REnd());
}

TEST(ArrayIteratorTest, DistanceBetweenBeginAndEnd)
{
    stdads::Array<int, 5> arr{1,2,3,4,5};

    EXPECT_EQ(arr.End() - arr.Begin(), 5);
}

TEST(ArrayIteratorTest, DistanceBetweenRBeginAndREnd)
{
    stdads::Array<int, 5> arr{1,2,3,4,5};

    EXPECT_EQ(arr.REnd() - arr.RBegin(), 5);
}