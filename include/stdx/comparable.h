#ifndef COMPARABLE_H
#define COMPARABLE_H

#include <stdx/equatable.h>

namespace stdx {

/// @brief CRTP base class that provides full comparison operators.
///
/// This class extends @ref equatable to automatically provide all relational
/// comparison operators (`<`, `>`, `<=`, `>=`) for a derived type by requiring
/// the implementation of a single member function:
///
///     bool lessThan(const Derived& other) const;
///
/// All comparison operators are defined in terms of `lessThan`, ensuring
/// consistent and minimal implementation effort for derived types.
///
/// Example usage:
/// @code
/// struct MyType : public comparable<MyType> {
///     int value;
///
///     bool equals(const MyType& other) const {
///         return value == other.value;
///     }
///
///     bool lessThan(const MyType& other) const {
///         return value < other.value;
///     }
/// };
/// @endcode
///
/// @note The lessThan function should define a strict weak ordering:
/// - Irreflexive: a.lessThan(a) == false
/// - Asymmetric: if a.lessThan(b) then !b.lessThan(a)
/// - Transitive: if a.lessThan(b) and b.lessThan(c), then a.lessThan(c)
///
/// @tparam Derived The type that inherits from this class and implements:
/// - bool equals(const Derived&) const  (from equatable)
/// - bool lessThan(const Derived&) const
template <typename Derived>
class Comparable : public Equatable<Derived>
{
  public:
    /// @brief Less than operator
    /// @param lhs left hand sid
    /// @param rhs right had side
    /// @return True if lhs is less than rhs, false otherwise.
    friend bool operator<(const Derived& lhs, const Derived& rhs)
    {
        return lhs.lessThan(rhs);
    }

    /// @brief Greater than operator
    /// @param lhs left hand side
    /// @param rhs right had side
    /// @return True if lhs is greater than rhs, false otherwise.
    friend bool operator>(const Derived& lhs, const Derived& rhs)
    {
        return rhs.lessThan(lhs);
    }

    /// @brief Less than or equal to operator
    /// @param lhs left hand side
    /// @param rhs right had side
    /// @return True if lhs is less than or equal to rhs, false otherwise.
    friend bool operator<=(const Derived& lhs, const Derived& rhs)
    {
        return !rhs.lessThan(lhs);
    }

    /// @brief Greater than or equal to operator
    /// @param lhs left hand side
    /// @param rhs right had side
    /// @return True if lhs is greater than or equal to rhs, false otherwise.
    friend bool operator>=(const Derived& lhs, const Derived& rhs)
    {
        return !lhs.lessThan(rhs);
    }

  protected:
    // ==== comparable should not be constructed directly ====
    Comparable() = default; // NOLINT(bugprone-crtp-constructor-accessibility)
};
} // namespace stdx

#endif
