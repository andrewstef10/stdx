#ifndef ICOMPARABLE_H
#define ICOMPARABLE_H

#include <stdads/IEquatable.h>

namespace stdads {

    /// @brief CRTP base class that provides full comparison operators.
    ///
    /// This class extends @ref IEquatable to automatically provide all relational
    /// comparison operators (`<`, `>`, `<=`, `>=`) for a derived type by requiring
    /// the implementation of a single member function:
    ///
    ///     bool LessThan(const Derived& other) const;
    ///
    /// All comparison operators are defined in terms of `LessThan`, ensuring
    /// consistent and minimal implementation effort for derived types.
    ///
    /// Example usage:
    /// @code
    /// struct MyType : public IComparable<MyType> {
    ///     int value;
    ///
    ///     bool Equals(const MyType& other) const {
    ///         return value == other.value;
    ///     }
    ///
    ///     bool LessThan(const MyType& other) const {
    ///         return value < other.value;
    ///     }
    /// };
    /// @endcode
    ///
    /// @note The LessThan function should define a strict weak ordering:
    /// - Irreflexive: a.LessThan(a) == false
    /// - Asymmetric: if a.LessThan(b) then !b.LessThan(a)
    /// - Transitive: if a.LessThan(b) and b.LessThan(c), then a.LessThan(c)
    ///
    /// @tparam Derived The type that inherits from this class and implements:
    /// - bool Equals(const Derived&) const  (from IEquatable)
    /// - bool LessThan(const Derived&) const
    template<typename Derived>
    struct IComparable : public IEquatable<Derived> {

        /// @brief Less than operator
        /// @param lhs left hand sid
        /// @param rhs right had side
        /// @return True if lhs is less than rhs, false otherwise.
        friend bool operator<(const Derived& lhs, const Derived& rhs) { return lhs.LessThan(rhs); }

        /// @brief Greater than operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is greater than rhs, false otherwise.
        friend bool operator>(const Derived& lhs, const Derived& rhs) { return rhs.LessThan(lhs); }

        /// @brief Less than or equal to operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is less than or equal to rhs, false otherwise.
        friend bool operator<=(const Derived& lhs, const Derived& rhs) { return !rhs.LessThan(lhs); }

        /// @brief Greater than or equal to operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is greater than or equal to rhs, false otherwise.
        friend bool operator>=(const Derived& lhs, const Derived& rhs) { return !lhs.LessThan(rhs); }
    };
}

#endif