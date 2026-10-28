#ifndef CONTIGUOUS_CONTAINER_H
#define CONTIGUOUS_CONTAINER_H

#include <cstddef>

#include <stdx/container.h>
#include <stdx/internal/internal_contiguous_container.h>
#include <stdx/iterator.h>

namespace stdx {

/// @brief CRTP base class for contiguous-storage containers (e.g. array_list, fixed_array_list).
///        Provides element access, iterators, and element-order comparison on top of container<>.
/// @tparam Derived The concrete container type inheriting this base.
/// @tparam T       The element type stored in the container.
template <typename Derived, typename T>
class contiguous_container : public stdx::container<Derived, T> {
  public:
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = stdx::reverse_iterator<iterator>;
    using const_reverse_iterator = stdx::reverse_iterator<const_iterator>;

    // ==========================
    // ===== Element Access =====
    // ==========================

    /// @brief Returns a reference to the first element in the container.
    /// @details Behavior is undefined if the container is empty.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Reference to the first element.
    T& front() { return derived().data()[0]; }

    /// @brief Returns a constant reference to the first element in the container.
    /// @details Behavior is undefined if the container is empty.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Reference to the first element.
    const T& front() const { return derived().data()[0]; }

    /// @brief Returns a reference to the last element in the container.
    /// @details Behavior is undefined if the container is empty.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Reference to the last element.
    T& back() { return derived().data()[derived().size() - 1]; }

    /// @brief Returns a constant reference to the last element in the container.
    /// @details Behavior is undefined if the container is empty.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant reference to the last element.
    const T& back() const { return derived().data()[derived().size() - 1]; }

    /// @brief Returns a reference to the element at specified location `index`, with bounds checking.
    /// @details If index is not within the range of the container, an exception of type std::out_of_range is thrown.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @param index Position of the element to return.
    /// @return Reference to the requested element.
    /// @exception std::out_of_range if `index` is out of range (index >= size()).
    T& at(std::size_t index) { return const_cast<T&>(static_cast<const contiguous_container*>(this)->at(index)); }

    /// @brief Returns a constant reference to the element at specified location `index`, with bounds checking.
    /// @details If index is not within the range of the container, an exception of type std::out_of_range is thrown.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @param index Position of the element to return.
    /// @return Constant reference to the requested element.
    /// @exception std::out_of_range if `index` is out of range (index >= size()).
    const T& at(std::size_t index) const { return internal::contiguous_container::at(derived(), index); }

    /// @brief Returns a reference to the element at specified location `index`, without bounds checking.
    /// @details Time:  O(1)
    ///          Space: O(1)
    /// @param index Position of the element to return.
    /// @return Reference to the requested element.
    T& operator[](std::size_t index) { return derived().data()[index]; }

    /// @brief Returns a constant reference to the element at specified location `index`, without bounds checking.
    /// @details Time:  O(1)
    ///          Space: O(1)
    /// @param index Position of the element to return.
    /// @return Constant reference to the requested element.
    const T& operator[](std::size_t index) const { return derived().data()[index]; }

    // ===================
    // ==== Iterators ====
    // ===================

    /// @brief Returns an iterator to the first element of the container.
    /// @details If the container is empty, the returned iterator will be equal to end().
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Contiguous iterator to the first element.
    iterator begin() { return iterator(derived().data()); }

    /// @brief Returns a constant iterator to the first element of the container.
    /// @details If the container is empty, the returned iterator will be equal to end().
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant contiguous iterator to the first element.
    const_iterator begin() const { return const_iterator(derived().data()); }

    /// @brief Returns a constant iterator to the first element of the container.
    /// @details If the container is empty, the returned iterator will be equal to end().
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant contiguous iterator to the first element.
    const_iterator cbegin() const { return const_iterator(derived().data()); }

    /// @brief Returns an iterator past the last element of the container.
    /// @details This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Contiguous iterator past the last element.
    iterator end() { return iterator(derived().data() + derived().size()); }

    /// @brief Returns a const iterator past the last element of the container.
    /// @details This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant contiguous iterator past the last element.
    const_iterator end() const { return const_iterator(derived().data() + derived().size()); }

    /// @brief Returns a const iterator past the last element of the container.
    /// @details This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant contiguous iterator past the last element.
    const_iterator cend() const { return const_iterator(derived().data() + derived().size()); }

    /// @brief Returns a reverse iterator to the first element of the reversed container.
    /// @details It corresponds to the last element of the non-reversed container.
    ///          If the container is empty, the returned iterator will be equal to end().
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Reverse contiguous iterator to the first element.
    reverse_iterator rbegin() { return reverse_iterator(derived().data() + derived().size()); }

    /// @brief Returns a constant reverse iterator to the first element of the reversed container.
    /// @details It corresponds to the last element of the non-reversed container.
    ///          If the container is empty, the returned iterator will be equal to end().
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant reverse contiguous iterator to the first element.
    const_reverse_iterator rbegin() const { return const_reverse_iterator(derived().data() + derived().size()); }

    /// @brief Returns a constant reverse iterator to the first element of the reversed container.
    /// @details It corresponds to the last element of the non-reversed container.
    ///          If the container is empty, the returned iterator will be equal to end().
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant reverse contiguous iterator to the first element.
    const_reverse_iterator crbegin() const { return const_reverse_iterator(derived().data() + derived().size()); }

    /// @brief Returns a reverse iterator past the last element of the reversed container.
    /// @details It corresponds to the element preceding the first element of the non-reversed container.
    ///          This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Reverse contiguous iterator to the element following the last element.
    reverse_iterator rend() { return reverse_iterator(derived().data()); }

    /// @brief Returns a constant reverse iterator past the last element of the reversed container.
    /// @details It corresponds to the element preceding the first element of the non-reversed container.
    ///          This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant reverse contiguous iterator to the element following the last element.
    const_reverse_iterator rend() const { return const_reverse_iterator(derived().data()); }

    /// @brief Returns a constant reverse iterator past the last element of the reversed container.
    /// @details It corresponds to the element preceding the first element of the non-reversed container.
    ///          This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
    ///          Time:  O(1)
    ///          Space: O(1)
    /// @return Constant reverse contiguous iterator to the element following the last element.
    const_reverse_iterator crend() const { return const_reverse_iterator(derived().data()); }

    // ====================
    // ==== Comparison ====
    // ====================

    /// @brief Compares two containers element-by-element using operator==, stopping at the first mismatch.
    /// @details Time:  O(n)
    ///          Space: O(1)
    ///          Requires T to be equality-comparable (operator== defined).
    /// @param other Container to compare against.
    /// @return True if both containers have equal size and all elements compare equal.
    bool equals(const Derived& other) const { return internal::contiguous_container::equals(derived(), other); }

    /// @brief Performs a lexicographic less-than comparison using operator<.
    /// @details Time:  O(n)
    ///          Space: O(1)
    ///          Requires T to be less-than-comparable (operator< defined).
    /// @param other Container to compare against.
    /// @return True if this container is lexicographically less than `other`.
    bool less_than(const Derived& other) const { return internal::contiguous_container::less_than(derived(), other); }

    // ===================
    // ==== Modifiers ====
    // ===================

    /// @brief Removes the element at `pos`, shifting elements after `pos` to the left.
    /// @details Time:  O(n) — shifts elements after pos forward by one
    ///          Space: O(1)
    ///          Invalidates the iterator at pos and all iterators/references after it.
    /// @param pos Iterator to the element to remove.
    /// @return Iterator to the element that followed the removed one, or end() if pos was the last element.
    iterator erase(const_iterator pos) { return erase(pos, pos + 1); }

    /// @brief Removes all elements in the range [first, last), shifting elements after `last` to the left.
    /// @details Time:  O(n) — shifts elements after last forward
    ///          Space: O(1)
    ///          Invalidates iterators from first onward. Returns last unchanged if the range is empty.
    /// @param first Iterator to the first element to remove.
    /// @param last  Iterator past the last element to remove.
    /// @return Iterator to the element that followed the last removed element.
    iterator erase(const_iterator first, const_iterator last);

  protected:
    contiguous_container() = default;
    ~contiguous_container() = default;
    contiguous_container(const contiguous_container&) = default;
    contiguous_container(contiguous_container&&) noexcept = default;
    contiguous_container& operator=(const contiguous_container&) = default;
    contiguous_container& operator=(contiguous_container&&) noexcept = default;

  private:
    Derived& derived() { return static_cast<Derived&>(*this); }
    const Derived& derived() const { return static_cast<const Derived&>(*this); }
};

template <typename Derived, typename T>
inline typename contiguous_container<Derived, T>::iterator
contiguous_container<Derived, T>::erase(const_iterator first, const_iterator last) {
    const std::size_t FIRST = static_cast<std::size_t>(first - derived().data());
    const std::size_t LAST = static_cast<std::size_t>(last - derived().data());

    // Close the gap by moving the tail elements down onto it
    namespace icc = internal::contiguous_container;
    const std::size_t NEW_SIZE = icc::shift_left(derived(), FIRST, LAST);

    // Destroy the moved-from tail elements
    while (derived().size() > NEW_SIZE) {
        derived().pop_back();
    }

    return iterator(derived().data() + FIRST);
}

} // namespace stdx

#endif
