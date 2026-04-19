#ifndef IEQUATABLE_H
#define IEQUATABLE_H

namespace stdads {

    /// @brief CRTP base class that provides equality and inequality operators.
    ///
    /// This class enables a derived type to automatically gain `operator==` and
    /// `operator!=` by implementing a single member function:
    ///
    ///     bool Equals(const Derived& other) const;
    ///
    /// The comparison operators are defined in terms of this `Equals` function,
    /// ensuring consistent equality semantics across all derived types.
    ///
    /// Example usage:
    /// @code
    /// struct MyType : public IEquatable<MyType> {
    ///     int value;
    ///
    ///     bool Equals(const MyType& other) const {
    ///         return value == other.value;
    ///     }
    /// };
    /// @endcode
    ///
    /// @tparam Derived The type that inherits from this class and implements
    /// `bool Equals(const Derived&) const`.
    template<typename Derived>
    struct IEquatable {

        /// @brief Equality operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is equal to rhs, false otherwise.
        friend bool operator==(const Derived& lhs, const Derived& rhs) { return lhs.Equals(rhs); }

        /// @brief Inequality operator 
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is not equal to rhs, false otherwise.
        friend bool operator!=(const Derived& lhs, const Derived& rhs) { return !lhs.Equals(rhs); }
    };
}

#endif