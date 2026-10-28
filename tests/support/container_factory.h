#ifndef STDX_TEST_CONTAINER_FACTORY_H
#define STDX_TEST_CONTAINER_FACTORY_H

#include <cstddef>
#include <initializer_list>

#include <stdx/array.h>
#include <stdx/span.h>

namespace StdxTest {

/// @brief Sample values used for testing in typed test suites (see contiguous_container_test_suite.h for an example),
/// so a suite can run against any value_type. Specialize this for value types a raw int literal can't construct.
template <typename T>
struct TestValues {

    /// @brief The number of test values provided
    static constexpr std::size_t SIZE = 3;

    /// @brief The test values array
    static constexpr T VALUES[] = {static_cast<T>(10), static_cast<T>(20), static_cast<T>(30)};

    /// @brief An additional value to test updating values in the container
    static constexpr T UPDATED = static_cast<T>(99);
};

// template <>
// struct TestValues<std::string> {
//     static std::string V0 = "v0";
//     static std::string V1 { return "v1"; }
//     static std::string V2 { return "v2"; }
//     static std::string UPDATED { return "updated"; }
// };

/// @brief Builds instances of `Container` for use in shared/typed test suites.
///
/// The default implementation works for any container constructible from a
/// std::initializer_list<Container::value_type>. Containers with a different construction story don't match this shape
/// and get their own specialization below (e.g. stdx::array's aggregate init).
template <typename Container>
struct ContainerFactory {
    using T = typename Container::value_type;

    /// @brief Builds a container holding `values`, in order.
    static Container Make(std::initializer_list<T> values) { return Container(values); }

    /// @brief Builds a container holding values defined in TestValues<T>
    static Container MakeFromTestValues() {
        return Container(std::begin(TestValues<T>::VALUES), std::end(TestValues<T>::VALUES));
    }
};

/// @brief stdx::array has no initializer_list constructor -- it's a fixed-N aggregate, so `values.size()` must equal N.
template <typename T, std::size_t N>
struct ContainerFactory<stdx::array<T, N>> {
    static_assert(N == TestValues<T>::SIZE, "N must match the size of provided test value for type T");

    /// @brief Builds a container holding `values`, in order.
    static stdx::array<T, N> Make(std::initializer_list<T> values) {
        stdx::array<T, N> container{};
        std::size_t i = 0;
        for (const T& value : values) {
            container[i++] = value;
        }
        return container;
    }

    /// @brief Builds a container holding values defined in TestValues<T>
    static stdx::array<T, N> MakeFromTestValues() {
        stdx::array<T, N> container{};
        std::size_t i = 0;
        for (const T& value : TestValues<T>::VALUES) {
            container[i++] = value;
        }
        return container;
    }
};

/// @brief stdx::span is non-owning, so it has no constructor from an initializer_list or iterator pair -- it must
/// view storage that outlives the returned span. Each function below rewrites a function-local static buffer on
/// every call and returns a span over it, so the buffer always reflects the most recent call's values. This is safe
/// as long as a test never keeps two spans from the same factory function alive at once, which none of the shared
/// suites do.
template <typename T, std::size_t N>
struct ContainerFactory<stdx::span<T, N>> {
    static_assert(N == TestValues<T>::SIZE, "N must match the size of provided test values for type T");

    /// @brief Builds a span viewing a buffer holding `values`, in order.
    static stdx::span<T, N> Make(std::initializer_list<T> values) {
        static T storage[N];
        std::size_t i = 0;
        for (const T& value : values) {
            storage[i++] = value;
        }
        return stdx::span<T, N>(storage);
    }

    /// @brief Builds a span viewing a buffer holding values defined in TestValues<T>
    static stdx::span<T, N> MakeFromTestValues() {
        static T storage[N];
        std::size_t i = 0;
        for (const T& value : TestValues<T>::VALUES) {
            storage[i++] = value;
        }
        return stdx::span<T, N>(storage);
    }
};

/// @brief Dynamic-extent specialization: count is a runtime value, so Make is not limited to TestValues<T>::SIZE
/// elements the way the fixed-extent specialization above is.
template <typename T>
struct ContainerFactory<stdx::span<T>> {
    /// @brief Builds a span viewing a buffer holding `values`, in order.
    static stdx::span<T> Make(std::initializer_list<T> values) {
        static T storage[TestValues<T>::SIZE];
        std::size_t i = 0;
        for (const T& value : values) {
            storage[i++] = value;
        }
        return stdx::span<T>(storage, i);
    }

    /// @brief Builds a span viewing a buffer holding values defined in TestValues<T>
    static stdx::span<T> MakeFromTestValues() {
        static T storage[TestValues<T>::SIZE];
        std::size_t i = 0;
        for (const T& value : TestValues<T>::VALUES) {
            storage[i++] = value;
        }
        return stdx::span<T>(storage, TestValues<T>::SIZE);
    }
};

} // namespace StdxTest

#endif
