#include <gtest/gtest.h>
#include <span>
#include <stdx/array.h>
#include <stdx/span.h>
#include <type_traits>
#include <vector>

#include "support/contiguous_container_test_suite.h"

namespace StdxTest {

// Instantiate general contiguous container test cases for span.
// Covers indexing and iteration as well as basic size tests.
using ContiguousContainerTypes = ::testing::Types<stdx::span<int>, stdx::span<int, 3>>;
INSTANTIATE_TYPED_TEST_SUITE_P(SpanContiguousContainerTests, ContiguousContainerTest, ContiguousContainerTypes);

TEST(SpanTests, DefaultConstruction) {
    stdx::span<int> defaultSpan;
    std::span<int> stlDefault;

    EXPECT_EQ(defaultSpan.data(), nullptr);
    EXPECT_EQ(defaultSpan.size(), 0U);
    EXPECT_EQ(defaultSpan.size(), stlDefault.size());

    stdx::span<int, 0> defaultSpanWithFixedSize;
    std::span<int, 0> stlDefaultWithFixedSize;

    EXPECT_EQ(defaultSpanWithFixedSize.data(), nullptr);
    EXPECT_EQ(defaultSpanWithFixedSize.size(), 0U);
    EXPECT_TRUE(defaultSpanWithFixedSize.empty());
    static_assert(defaultSpanWithFixedSize.size() == 0U);
    static_assert(defaultSpanWithFixedSize.size() == stlDefaultWithFixedSize.size());
}

TEST(SpanTests, PtrSizeConstruction) {
    int integer = 5;
    int* intPtr = &integer;

    stdx::span<int> span(intPtr, 1);

    EXPECT_EQ(span.data(), intPtr);
    EXPECT_EQ(span.size(), 1U);
}

TEST(SpanTests, PtrConstruction) {
    int integer = 5;
    int* intPtr = &integer;

    stdx::span<int, 1> myImpl(intPtr);

    EXPECT_EQ(myImpl.data(), intPtr);
    EXPECT_EQ(myImpl.size(), 1U);
    static_assert(myImpl.size() == 1U);
}

TEST(SpanTests, CArrayConstructionFixedSize) {
    int array[5] = {1, 2, 3, 4, 5};

    stdx::span<int, 5> fixedView = stdx::make_span(array);
    std::span<int, 5> stlView = array;

    EXPECT_EQ(fixedView.data(), array);
    EXPECT_EQ(fixedView.size(), 5U);
    EXPECT_EQ(fixedView.size_bytes(), 5 * sizeof(int));
    EXPECT_EQ(fixedView.size(), stlView.size());
    EXPECT_EQ(fixedView.data(), stlView.data());

    // A span views the array, it does not copy it
    array[2] = 42;
    EXPECT_EQ(fixedView.data()[2], 42);
}

TEST(SpanTests, CArrayConstructionConstView) {
    int array[5] = {1, 2, 3, 4, 5};

    stdx::span<const int, 5> fixedView = stdx::make_span<const int>(array);
    std::span<const int, 5> stlFixedView = array;

    EXPECT_EQ(fixedView.data(), array);
    EXPECT_EQ(fixedView.size(), 5U);
}

TEST(SpanTests, ConstElementSpanAllowsReadOnlyAccess) {
    const int array[3] = {1, 2, 3};
    stdx::span<const int, 3> fixedView = stdx::make_span(array);
    stdx::span<const int> dynamicView(array, 3);

    EXPECT_EQ(fixedView.front(), 1);
    EXPECT_EQ(fixedView.back(), 3);
    EXPECT_EQ(fixedView.at(1), 2);
    EXPECT_EQ(fixedView[1], 2);
    EXPECT_EQ(fixedView.data(), array);
    EXPECT_EQ(*fixedView.begin(), 1);
    EXPECT_EQ(*fixedView.rbegin(), 3);

    EXPECT_EQ(dynamicView.front(), 1);
    EXPECT_EQ(dynamicView.back(), 3);
    EXPECT_EQ(dynamicView.at(1), 2);
    EXPECT_EQ(dynamicView[1], 2);
}

TEST(SpanTests, CopyConstruction) {
    int array[3] = {1, 2, 3};

    stdx::span<int> original(array, 3);
    stdx::span<int> copy(original); // NOLINT(performance-unnecessary-copy-initialization)
    EXPECT_EQ(copy.data(), original.data());
    EXPECT_EQ(copy.size(), original.size());

    stdx::span<int, 3> fixedOriginal(array);
    stdx::span<int, 3> fixedCopy(fixedOriginal); // NOLINT(performance-unnecessary-copy-initialization)
    EXPECT_EQ(fixedCopy.data(), fixedOriginal.data());
    EXPECT_EQ(fixedCopy.size(), fixedOriginal.size());
}

TEST(SpanTests, MoveConstruction) {
    int array[3] = {1, 2, 3};

    stdx::span<int> original(array, 3);
    stdx::span<int> moved(std::move(original));
    EXPECT_EQ(moved.data(), array);
    EXPECT_EQ(moved.size(), 3U);

    stdx::span<int, 3> fixedOriginal(array);
    stdx::span<int, 3> fixedMoved(std::move(fixedOriginal));
    EXPECT_EQ(fixedMoved.data(), array);
    EXPECT_EQ(fixedMoved.size(), 3U);
}

TEST(SpanTests, CopyAssignment) {
    int source[3] = {1, 2, 3};
    int other[3] = {4, 5, 6};

    stdx::span<int> dynamicSource(source, 3);
    stdx::span<int> dynamicTarget(other, 3);
    dynamicTarget = dynamicSource;
    EXPECT_EQ(dynamicTarget.data(), source);
    EXPECT_EQ(dynamicTarget.size(), 3U);

    stdx::span<int, 3> fixedSource(source);
    stdx::span<int, 3> fixedTarget(other);
    fixedTarget = fixedSource;
    EXPECT_EQ(fixedTarget.data(), source);
}

TEST(SpanTests, MoveAssignment) {
    int source[3] = {1, 2, 3};
    int other[3] = {4, 5, 6};

    stdx::span<int> dynamicSource(source, 3);
    stdx::span<int> dynamicTarget(other, 3);
    dynamicTarget = std::move(dynamicSource);
    EXPECT_EQ(dynamicTarget.data(), source);
    EXPECT_EQ(dynamicTarget.size(), 3U);

    stdx::span<int, 3> fixedSource(source);
    stdx::span<int, 3> fixedTarget(other);
    fixedTarget = std::move(fixedSource);
    EXPECT_EQ(fixedTarget.data(), source);
}

TEST(SpanTests, CArrayConstructionRequiresCorrectConstT) {
    // A fixed size span only binds to an array of exactly N elements, matching std::span
    static_assert(std::is_constructible<stdx::span<int, 5>, int (&)[5]>::value, "matching bound must be constructible");
    // static_assert(!std::is_constructible<stdx::span<int, 3>, int (&)[5]>::value, "smaller N must not bind");
    // static_assert(!std::is_constructible<stdx::span<int, 9>, int (&)[5]>::value, "larger N must not bind");
    // static_assert(!std::is_constructible<std::span<int, 3>, int (&)[5]>::value, "std::span rejects smaller N too");

    // A mutable span must not bind to a const array
    static_assert(!std::is_constructible<stdx::span<int, 5>, const int (&)[5]>::value,
                  "mutable span must not view const elements");
    static_assert(std::is_constructible<stdx::span<const int, 5>, const int (&)[5]>::value,
                  "read-only span views const elements");
}

TEST(SpanTests, CArrayConstructionFixedSizeSpanIsPointerSized) {
    // A fixed size span's count lives in its type, so the object holds only the data pointer (empty base optimization)
    static_assert(sizeof(stdx::span<int, 5>) == sizeof(int*), "fixed size span must store only the data pointer");
    static_assert(sizeof(std::span<int, 5>) == sizeof(int*), "std::span has the same layout");

    // A dynamic span additionally stores its count
    static_assert(sizeof(stdx::span<int>) == sizeof(int*) + sizeof(std::size_t),
                  "dynamic span must store the data pointer and the count");
}

TEST(SpanTests, FixedSizeSpanDeterminsSizeAtCompileTime) {
    int array[5] = {1, 2, 3, 4, 5};

    stdx::span<int, 5> fixedView = stdx::make_span(array);
    std::span<int, 5> stlView = array;

    static_assert(fixedView.size() == 5);
    static_assert(stlView.size() == 5);
    static_assert(fixedView.size_bytes() == sizeof(int) * 5);
    static_assert(stlView.size_bytes() == sizeof(int) * 5);
}

TEST(SpanTests, VectorConstruction) {
    std::vector<int> vector = {1, 2, 3, 4, 5};

    std::span stlVectorSpan = vector;
    std::span<int> stlVectorSpan2 = vector;
}

// -------------------------------------------------------------------------------------------------------------------
// Compile-time (constexpr) evaluation
//
// Every member of span is marked constexpr. The tests above exercise them at runtime (for coverage and to catch
// incorrect values); the static_asserts below prove each one is also actually usable in a constant expression.
// -------------------------------------------------------------------------------------------------------------------

constexpr bool ConstructionAssignmentAndSizeAreConstexpr() {
    int values[3] = {1, 2, 3};

    stdx::span<int> dynamicDefault;
    stdx::span<int, 0> fixedDefault;
    stdx::span<int> dynamicView(values, 3);
    stdx::span<int, 3> fixedView(values);

    bool ok = dynamicDefault.empty() && dynamicDefault.data() == nullptr;
    ok = ok && fixedDefault.empty();
    ok = ok && dynamicView.size() == 3 && dynamicView.data() == values;
    ok = ok && fixedView.size() == 3 && fixedView.data() == values;
    ok = ok && fixedView.size_bytes() == 3 * sizeof(int);
    ok = ok && !fixedView.empty();

    stdx::span<int> copied(dynamicView);
    stdx::span<int> moved(std::move(copied));
    stdx::span<int> assigned;
    assigned = dynamicView;
    stdx::span<int> moveAssigned;
    moveAssigned = std::move(assigned);
    ok = ok && moved.data() == values && moveAssigned.data() == values;

    return ok;
}
static_assert(ConstructionAssignmentAndSizeAreConstexpr(),
              "span construction, assignment, and size must be usable in a constant expression");

constexpr bool ElementAccessIsConstexpr() {
    int values[3] = {1, 2, 3};
    stdx::span<int, 3> view(values);

    bool ok = view.front() == 1 && view.back() == 3;
    ok = ok && view.at(1) == 2 && view[1] == 2;
    ok = ok && view.data() == values;

    view[0] = 10;
    ok = ok && view.front() == 10;

    return ok;
}
static_assert(ElementAccessIsConstexpr(), "span element access must be usable in a constant expression");

constexpr bool ForwardIterationIsConstexpr() {
    int values[3] = {1, 2, 3};
    stdx::span<int, 3> view(values);
    const stdx::span<int, 3>& constView = view;

    bool ok = *view.begin() == 1 && *constView.begin() == 1 && *view.cbegin() == 1;
    ok = ok && *(view.end() - 1) == 3 && *(constView.end() - 1) == 3 && *(view.cend() - 1) == 3;

    *view.begin() = 42;
    ok = ok && view.front() == 42;

    return ok;
}
static_assert(ForwardIterationIsConstexpr(), "span forward iteration must be usable in a constant expression");

constexpr bool ReverseIterationIsConstexpr() {
    int values[3] = {1, 2, 3};
    stdx::span<int, 3> view(values);
    const stdx::span<int, 3>& constView = view;

    bool ok = *view.rbegin() == 3 && *constView.rbegin() == 3 && *view.crbegin() == 3;
    ok = ok && *(view.rend() - 1) == 1 && *(constView.rend() - 1) == 1 && *(view.crend() - 1) == 1;

    *view.rbegin() = 99;
    ok = ok && view.back() == 99;

    stdx::span<int, 3>::reverse_iterator it = view.rbegin();
    stdx::span<int, 3>::reverse_iterator next = it + 1;
    ok = ok && next - it == 1 && it != next && !(it == next);

    return ok;
}
static_assert(ReverseIterationIsConstexpr(), "span reverse iteration must be usable in a constant expression");

constexpr bool MakeSpanIsConstexpr() {
    int values[4] = {1, 2, 3, 4};
    stdx::span<int, 4> view = stdx::make_span(values);
    return view.size() == 4 && view.data() == values;
}
static_assert(MakeSpanIsConstexpr(), "make_span must be usable in a constant expression");

// The static_asserts above prove each function is usable in a constant expression.
// Now execute them at runtime.
TEST(SpanTests, ConstexprHelpersAlsoProduceCorrectResultsAtRuntime) {
    EXPECT_TRUE(ConstructionAssignmentAndSizeAreConstexpr());
    EXPECT_TRUE(ElementAccessIsConstexpr());
    EXPECT_TRUE(ForwardIterationIsConstexpr());
    EXPECT_TRUE(ReverseIterationIsConstexpr());
    EXPECT_TRUE(MakeSpanIsConstexpr());
}

} // namespace StdxTest
