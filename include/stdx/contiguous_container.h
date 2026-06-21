#ifndef CONTIGUOUS_CONTAINER_H
#define CONTIGUOUS_CONTAINER_H

#include <stdexcept>

#include <stdx/container.h>
#include <stdx/iterator.h>

namespace stdx {

    /// @brief CRTP base class for contiguous-storage containers (e.g. array_list, fixed_array_list).
    ///        Provides element access, iterators, and element-order comparison on top of container<>.
    /// @tparam Derived The concrete container type inheriting this base.
    /// @tparam T       The element type stored in the container.
    template<typename Derived, typename T>
    class contiguous_container
        : public stdx::container<Derived, T>
    {
    public:

        using size_type       = typename container<Derived, T>::size_type;
        using reference       = typename container<Derived, T>::reference;
        using const_reference = typename container<Derived, T>::const_reference;

        using iterator               = T*;
        using const_iterator         = const T*;
        using reverse_iterator       = stdx::reverse_iterator<iterator>;
        using const_reverse_iterator = stdx::reverse_iterator<const_iterator>;

        // ===== Element Access =====

        /// @brief Returns a reference to the element at position `index` with bounds checking.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        /// @exception std::out_of_range If index >= size().
        reference at(size_type index);
        const_reference at(size_type index) const;

        /// @brief Returns a reference to the element at position `index` without bounds checking.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        reference operator[](size_type index) { return derived().m_data[index]; }
        const_reference operator[](size_type index) const { return derived().m_data[index]; }

        /// @brief Returns a pointer to the underlying element storage. The range [data(), data() + size()) is always valid.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          If the container is empty, data() may be nullptr and is not dereferenceable.
        /// @return Pointer to the first element, or nullptr if empty.
        T* data() { return derived().m_data; }
        const T* data() const { return derived().m_data; }

        /// @brief Returns a reference to the last element. Behavior is undefined if the container is empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reference to the last element.
        reference back() { return derived().m_data[derived().m_size - 1]; }
        const_reference back() const { return derived().m_data[derived().m_size - 1]; }

        /// @brief Returns a reference to the first element. Behavior is undefined if the container is empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reference to the first element.
        reference front() { return derived().m_data[0]; }
        const_reference front() const { return derived().m_data[0]; }


        // ==== Iterators ====

        /// @brief Returns an iterator to the first element. If the container is empty, the iterator equals end().
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Iterator to the first element.
        iterator begin() { return iterator(derived().m_data); }
        const_iterator begin() const { return const_iterator(derived().m_data); }
        const_iterator cbegin() const { return const_iterator(derived().m_data); }

        /// @brief Returns a past-the-end sentinel iterator. The iterator is not dereferenceable.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Iterator past the last element.
        iterator end() { return iterator(derived().m_data + derived().m_size); }
        const_iterator end() const { return const_iterator(derived().m_data + derived().m_size); }
        const_iterator cend() const { return const_iterator(derived().m_data + derived().m_size); }

        /// @brief Returns a reverse iterator to the last element (first element of the reversed range). Equals rend() if empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reverse iterator to the last element.
        reverse_iterator rbegin() { return reverse_iterator(derived().m_data + derived().m_size); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(derived().m_data + derived().m_size); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(derived().m_data + derived().m_size); }

        /// @brief Returns a reverse past-the-end sentinel iterator. The iterator is not dereferenceable.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reverse iterator past the first element (in reverse order).
        reverse_iterator rend() { return reverse_iterator(derived().m_data); }
        const_reverse_iterator rend() const { return const_reverse_iterator(derived().m_data); }
        const_reverse_iterator crend() const { return const_reverse_iterator(derived().m_data); }


        // ==== Comparison ====

        /// @brief Compares two containers element-by-element using operator==, stopping at the first mismatch.
        /// @details Time:  O(n) — compares up to min(size(), other.size()) elements
        ///          Space: O(1)
        ///          Requires T to be equality-comparable (operator== defined).
        /// @param other Container to compare against.
        /// @return True if both containers have equal size and all elements compare equal.
        bool equals(const Derived& other) const;

        /// @brief Performs a lexicographic less-than comparison using operator<.
        /// @details Time:  O(n) — compares up to min(size(), other.size()) elements
        ///          Space: O(1)
        ///          Requires T to be less-than-comparable (operator< defined).
        /// @param other Container to compare against.
        /// @return True if this container is lexicographically less than `other`.
        bool less_than(const Derived& other) const;

    protected:
        contiguous_container() = default;
        ~contiguous_container() = default;
        contiguous_container(const contiguous_container&) = default;
        contiguous_container(contiguous_container&&) noexcept = default;
        contiguous_container& operator=(const contiguous_container&) = default;
        contiguous_container& operator=(contiguous_container&&) noexcept = default;

    private:
        Derived&       derived()       { return static_cast<Derived&>(*this); }
        const Derived& derived() const { return static_cast<const Derived&>(*this); }
    };


    template<typename Derived, typename T>
    inline contiguous_container<Derived, T>::reference contiguous_container<Derived, T>::at(size_type index)
    {
        if (index >= this->size())
        {
            throw std::out_of_range("Index outside the bounds of the container");
        }
        return data()[index];
    }

    template<typename Derived, typename T>
    inline contiguous_container<Derived, T>::const_reference contiguous_container<Derived, T>::at(size_type index) const
    {
        if (index >= this->size())
        {
            throw std::out_of_range("Index outside the bounds of the container");
        }
        return data()[index];
    }

    template<typename Derived, typename T>
    inline bool contiguous_container<Derived, T>::equals(const Derived& other) const
    {
        if (this->size() != other.size())
        {
            return false;
        }

        for (size_type i = 0; i < this->size(); ++i)
        {
            if (!(data()[i] == other.data()[i]))
            {
                return false;
            }
        }
        return true;
    }

    template<typename Derived, typename T>
    inline bool contiguous_container<Derived, T>::less_than(const Derived& other) const
    {
        for (size_type i = 0; i < this->size() && i < other.size(); ++i)
        {
            if (data()[i] < other.data()[i])
            {
                return true;
            }
            if (other.data()[i] < data()[i])
            {
                return false;
            }
        }
        return this->size() < other.size();
    }


}

#endif
