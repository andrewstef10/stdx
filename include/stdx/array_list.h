#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <cstddef>
#include <stdexcept>

#include <stdx/comparable.h>
#include <stdx/container.h>
#include <stdx/iterator.h>
#include <stdx/memory.h>
#include <utility>

namespace stdx {

    /**
     * @brief Dynamic-size array. Elements are stored contiguously, giving O(1) random access and amortized O(1) append.
     * @tparam T            Element type.
     * @tparam Allocator    Allocator type. Defaults to stdx::allocator<T>.
     * @tparam growth_policy Policy controlling how capacity grows on reallocation. Defaults to stdx::doubling_growth.
     */
    template<typename T, typename Allocator = stdx::allocator<T>, typename growth_policy = stdx::doubling_growth<>>
    class array_list
        : public container<array_list<T, Allocator, growth_policy>, T>
        , public comparable<array_list<T, Allocator, growth_policy>>
    {
    public:
        friend container<array_list, T>;

        using size_type       = typename container<array_list, T>::size_type;
        using reference       = typename container<array_list, T>::reference;
        using const_reference = typename container<array_list, T>::const_reference;

        using allocator_type  = Allocator;
        using pointer         = std::allocator_traits<Allocator>::pointer;
        using const_pointer   = std::allocator_traits<Allocator>::const_pointer;

        using iterator = T*;
        using const_iterator = const T*;
        using reverse_iterator = stdx::reverse_iterator<iterator>;
        using const_reverse_iterator = stdx::reverse_iterator<const_iterator>;

        
        /// @brief Default constructor. Constructs an empty container with no elements and no allocated storage.
        /// @details Time:  O(1)
        ///          Space: O(1)
        array_list();

        /// @brief Copy constructor. Constructs a container with a copy of each element in `other`, in the same order.
        /// @details Time:  O(n), where n = other.size()
        ///          Space: O(n)
        /// @param other Container to copy from.
        array_list(const array_list& other);

        /// @brief Move constructor. Acquires ownership of the elements in `other`; `other` is left in a valid empty state.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param other Container to move from.
        array_list(array_list&& other);

        /// @brief Destructor. Destroys all elements and releases allocated memory.
        /// @details Time:  O(n), where n = size()
        ///          Space: O(1)
        ~array_list();

        /// @brief Copy assignment operator. Replaces the contents with a copy of `other`.
        /// @details Time:  O(n), where n = max(size(), other.size())
        ///          Space: O(n) if reallocation is required; O(1) otherwise
        /// @param other Container to copy from.
        /// @return Reference to this container.
        array_list& operator=(const array_list& other);

        /// @brief Move assignment operator. Replaces the contents by acquiring the elements of `other`.
        /// @details Time:  O(1) when allocators are equal or POCMA is true; O(n) otherwise, where n = other.size()
        ///          Space: O(1)
        /// @param other Container to move from.
        /// @return Reference to this container.
        array_list& operator=(array_list&& other);

        /// @brief Returns the allocator associated with the container.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return The associated allocator.
        allocator_type get_allocator() const noexcept { return m_alloc; }

        /// @brief Returns the growth policy associated with the container.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return The associated growth policy.
        growth_policy get_growth_policy() const noexcept { return m_growth_policy; }


        // ===== Element Access =====

        /// @brief Returns a reference to the element at position `index` with bounds checking.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        /// @exception std::out_of_range if `index` is out of range (index >= size()).
        reference at(size_type index);
        const_reference at(size_type index) const;

        /// @brief Returns a reference to the element at position `index` without bounds checking.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        reference operator[](size_type index) { return m_data[index]; }
        const_reference operator[](size_type index) const { return m_data[index]; }

        /// @brief Returns a pointer to the underlying element storage. The range [data(), data() + size()) is always valid.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          If the container is empty, data() may be nullptr and is not dereferenceable.
        /// @return Pointer to the first element, or nullptr if empty.
        T* data() { return m_data; }
        const T* data() const { return m_data; }

        /// @brief Returns a reference to the last element. Behavior is undefined if the container is empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reference to the last element.
        reference back() { return m_data[m_size - 1]; }
        const_reference back() const { return m_data[m_size - 1]; }

        /// @brief Returns a reference to the first element. Behavior is undefined if the container is empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reference to the first element.
        reference front() { return m_data[0]; }
        const_reference front() const { return m_data[0]; }


        // ==== Iterators ====

        /// @brief Returns an iterator to the first element. If the container is empty, the iterator equals end().
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Iterator to the first element.
        iterator begin() { return iterator(m_data); }
        const_iterator begin() const { return const_iterator(m_data); }
        const_iterator cbegin() const { return const_iterator(m_data); }

        /// @brief Returns a past-the-end sentinel iterator. The iterator is not dereferenceable.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Iterator past the last element.
        iterator end() { return iterator(m_data + m_size); }
        const_iterator end() const { return const_iterator(m_data + m_size); }
        const_iterator cend() const { return const_iterator(m_data + m_size); }

        /// @brief Returns a reverse iterator to the last element (first element of the reversed range). Equals rend() if empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reverse iterator to the last element.
        reverse_iterator rbegin() { return reverse_iterator(m_data + m_size); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(m_data + m_size); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(m_data + m_size); }

        /// @brief Returns a reverse past-the-end sentinel iterator. The iterator is not dereferenceable.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Reverse iterator past the first element (in reverse order).
        reverse_iterator rend() { return reverse_iterator(m_data); }
        const_reverse_iterator rend() const { return const_reverse_iterator(m_data); }
        const_reverse_iterator crend() const { return const_reverse_iterator(m_data); }


        // ===== Capacity =====

        /// @brief Returns the number of elements the container can hold without reallocating.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return Current allocated capacity in number of elements.
        size_type capacity() const { return m_capacity; }

        /// @brief Requests that the array_list capacity be at least enough to contain `n` elements.
        /// @details Time:  O(n) — moves all existing elements if reallocation occurs
        ///          Space: O(n) — allocates a new buffer of size n when growing
        ///          If n is greater than the current array_list capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
        ///          In all other cases, the function call does not cause a reallocation and the array_list capacity is not affected.
        ///          Invalidates all iterators and references if reallocation occurs.
        /// @param n Minimum required capacity.
        void reserve(size_type n);

        /// @brief Requests the removal of unused capacity by reallocating the buffer to exactly size() elements. No-op if capacity() == size().
        /// @details Time:  O(n) — moves all existing elements if reallocation occurs
        ///          Space: O(n) — allocates a new buffer of size size() when shrinking
        ///          Invalidates all iterators and references if reallocation occurs.
        void shrink_to_fit();


        // ==== Modifiers ====

        /// @brief Appends a copy of `val` to the end of the container. Reallocates if size() == capacity().
        /// @details Time:  O(1) amortized — O(n) on reallocation
        ///          Space: O(1) amortized — O(n) on reallocation
        ///          Invalidates all iterators and references if reallocation occurs.
        /// @param val Value to copy-construct into the new element.
        void push_back(const_reference val);

        /// @brief Appends `val` to the end of the container by moving it. Reallocates if size() == capacity().
        /// @details Time:  O(1) amortized — O(n) on reallocation
        ///          Space: O(1) amortized — O(n) on reallocation
        ///          Invalidates all iterators and references if reallocation occurs.
        /// @param val Value to move-construct into the new element.
        void push_back(T&& val);

        /// @brief Constructs an element in-place at the end of the container, forwarding `args` to T's constructor.
        /// @details Time:  O(1) amortized — O(n) on reallocation
        ///          Space: O(1) amortized — O(n) on reallocation
        ///          Invalidates all iterators and references if reallocation occurs.
        /// @param args Arguments to forward to the constructor of T.
        template<typename... Args>
        void emplace_back(Args&&... args);

        /// @brief Removes the last element. Behavior is undefined if the container is empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          Invalidates the end() iterator and any reference to the last element.
        void pop_back();

        /// @brief Inserts a copy of `value` before `pos`. Reallocates if size() == capacity().
        /// @details Time:  O(n) — shifts elements after pos to make room
        ///          Space: O(1) amortized — O(n) on reallocation
        ///          Invalidates all iterators and references if reallocation occurs; otherwise invalidates from pos onward.
        /// @param pos Iterator before which the element is inserted. May be end().
        /// @param value Value to copy-construct into the new element.
        /// @return Iterator to the inserted element.
        iterator insert(const_iterator pos, const_reference value);

        /// @brief Inserts `value` before `pos` by moving it. Reallocates if size() == capacity().
        /// @details Time:  O(n) — shifts elements after pos to make room
        ///          Space: O(1) amortized — O(n) on reallocation
        ///          Invalidates all iterators and references if reallocation occurs; otherwise invalidates from pos onward.
        /// @param pos Iterator before which the element is inserted. May be end().
        /// @param value Value to move-construct into the new element.
        /// @return Iterator to the inserted element.
        iterator insert(const_iterator pos, T&& value);

        /// @brief Constructs an element in-place before `pos`, forwarding `args` to T's constructor.
        /// @details Time:  O(n) — shifts elements after pos to make room
        ///          Space: O(1) amortized — O(n) on reallocation
        ///          Invalidates all iterators and references if reallocation occurs; otherwise invalidates from pos onward.
        /// @param pos Iterator before which the element is constructed. May be end().
        /// @param args Arguments to forward to the constructor of T.
        /// @return Iterator to the emplaced element.
        template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        /// @brief Removes the element at `pos`.
        /// @details Time:  O(n) — shifts elements after pos forward by one
        ///          Space: O(1)
        ///          Invalidates the iterator at pos and all iterators/references after it.
        /// @param pos Iterator to the element to remove.
        /// @return Iterator to the element that followed the removed one, or end() if pos was the last element.
        iterator erase(const_iterator pos);

        /// @brief Removes all elements in the range [first, last).
        /// @details Time:  O(n) — shifts elements after last forward
        ///          Space: O(1)
        ///          Invalidates iterators from first onward. Returns last unchanged if the range is empty.
        /// @param first Iterator to the first element to remove.
        /// @param last  Iterator past the last element to remove.
        /// @return Iterator to the element that followed the last removed element.
        iterator erase(const_iterator first, const_iterator last);

        /// @brief Destroys all elements. size() becomes zero; capacity() is unchanged.
        /// @details Time:  O(n) — destructs each element
        ///          Space: O(1)
        ///          Invalidates all iterators and references to elements.
        void clear();

        /// @brief Swaps the contents of this container with `other`. Iterators remain valid but now refer to the other container.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param other Container to swap with.
        void swap(array_list& other);


        // ==== Comparison ====

        /// @brief Compare two array_list's for equality.
        /// @details Time:  O(n) — compares up to min(size(), other.size()) elements
        ///          Space: O(1)
        ///          The equality comparison is performed by comparing the elements sequentially using operator==, stopping at the first mismatch.
        ///          Requires T to be equality-comparable (operator== defined).
        /// @param other Array list to compare against.
        /// @return True if both containers have equal size and all elements compare equal.
        bool equals(const array_list& other) const;

        /// @brief Performs a lexicographic less-than comparison using operator<.
        /// @details Time:  O(n) — compares up to min(size(), other.size()) elements
        ///          Space: O(1)
        ///          Requires T to be less-than-comparable (operator< defined).
        /// @param other Array list to compare against.
        /// @return True if this container is lexicographically less than `other`.
        bool less_than(const array_list& other) const;

    private:

        /// @brief Current number of elements stored in the container
        size_type m_size;

        /// @brief Pointer to underlying array of elements
        T* m_data;

        /// @brief Size of the allocated storage capacity
        size_type m_capacity;

        /// @brief Allocator for this array_list
        allocator_type m_alloc;

        /// @brief Growth policy for this data structure
        growth_policy m_growth_policy;

        void grow(size_type required_capacity);

        void resize(size_type new_capacity);
    };


    // ==== array_list Non Member functions ====


    // ===== Inline array_list Implementation =====

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list()
    : container<array_list, T>()
    , m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc()
    , m_growth_policy()
    {
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list(const array_list<T, Allocator, growth_policy>& other)
    : container<array_list, T>(other)
    , m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(other.m_alloc)
    , m_growth_policy(other.m_growth_policy)
    {
        // Grow capacity to at least the size of other if needed
        if (m_capacity < other.m_size)
        {
            grow(other.m_size);
        }

        // Copy construct the all elements in other
        for (; m_size < other.m_size; ++m_size)
        {
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, other.m_data[m_size]);
        }
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list(array_list<T, Allocator, growth_policy>&& other)
    : container<array_list, T>(std::move(other))
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
    , m_data(other.m_data)
    , m_alloc(std::move(other.m_alloc))
    , m_growth_policy(std::move(other.m_growth_policy))
    {
        other.m_size = 0;
        other.m_data = nullptr;
        other.m_capacity = 0;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::~array_list()
    {
        clear();
        std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>& array_list<T, Allocator, growth_policy>::operator=(const array_list<T, Allocator, growth_policy>& other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value)
        {
            // Allocator should be copied. Need to deallocate our own memory and reallocate with a copy of other's allocator.
            clear();
            std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
            m_capacity = 0;
            m_alloc = other.m_alloc;
        }

        // Grow capacity to at least the size of other if needed
        if (m_capacity < other.m_size)
        {
            grow(other.m_size);
        }

        // Assign elements from other
        if (m_size < other.m_size) // this container contains less elements than other
        {
            // Copy assign all elements from other up to current size
            size_type i = 0U;
            for (; i < m_size; ++i)
            {
                m_data[i] = other.m_data[i];
            }

            // Copy construct remaining elements from other
            for (; i < other.m_size; ++i)
            {
                std::allocator_traits<Allocator>::construct(m_alloc, m_data + i, other.m_data[i]);
            }
        }
        else // this container contains the same or more elements than other
        {
            // Copy assign all elements from other
            size_type i = 0U;
            for (; i < other.m_size; ++i)
            {
                m_data[i] = other.m_data[i];
            }

            // Remove remaining elements in this
            for (; i < m_size; ++i)
            {
                std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
            }
        }

        m_size = other.m_size;
        return *this;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>& array_list<T, Allocator, growth_policy>::operator=(array_list<T, Allocator, growth_policy>&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        constexpr bool POCMA = std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value;
        if (POCMA || m_alloc == other.m_alloc)
        {
            // Either our allocator and the memory our allocator allocates need to stay together (if POCMA) OR allocators are equal
            // Its safe to steal other's buffer and deallocate our own
            clear();
            std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);

            if (POCMA)
            {
                m_alloc = std::move(other.m_alloc);
            }

            m_data = other.m_data;
            m_capacity = other.m_capacity;
        }
        else
        {
            // POCMA false AND allocators are not equal
            // We must perform element-wise move into own storage, O(n)
            if (m_capacity < other.m_size)
            {
                grow(other.m_size);
            }


            // Assign elements from other
            if (m_size < other.m_size) // this container contains less elements than other
            {
                // Move assign all elements from other up to current size
                size_type i = 0U;
                for (; i < m_size; ++i)
                {
                    m_data[i] = std::move(other.m_data[i]);
                }

                // Move construct remaining elements from other
                for (; i < other.m_size; ++i)
                {
                    std::allocator_traits<Allocator>::construct(m_alloc, m_data + i, std::move(other.m_data[i]));
                }
            }
            else // this container contains the same or more elements than other
            {
                // Move assign all elements from other
                size_type i = 0U;
                for (; i < other.m_size; ++i)
                {
                    m_data[i] = std::move(other.m_data[i]);
                }

                // Remove remaining elements in this
                for (; i < m_size; ++i)
                {
                    std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
                }
            }
        }

        m_size = other.m_size;
        other.m_size = 0;
        other.m_data = nullptr;
        other.m_capacity = 0;

        return *this;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline auto array_list<T, Allocator, growth_policy>::at(size_type index) -> reference
    {
        if (index >= m_size) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the array_list");
        }
        return m_data[index];
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline auto array_list<T, Allocator, growth_policy>::at(size_type index) const -> const_reference
    {
        if (index >= m_size) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the array_list");
        }
        return m_data[index];
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline bool array_list<T, Allocator, growth_policy>::equals(const array_list<T, Allocator, growth_policy>& other) const
    {
        if (m_size != other.m_size)
        {
            return false;
        }

        for (size_t i = 0; i < m_size; ++i)
        {
            if (!(m_data[i] == other.m_data[i]))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline bool array_list<T, Allocator, growth_policy>::less_than(const array_list<T, Allocator, growth_policy>& other) const
    {
        for (size_type i = 0; i < m_size && i < other.m_size; ++i)
        {
            if (m_data[i] < other.m_data[i])
            {
                return true;
            }
            if (other.m_data[i] < m_data[i])
            {
                return false;
            }
        }
        return m_size < other.m_size;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::reserve(size_type n)
    {
        if (m_capacity < n)
        {
            grow(n);
        }
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::shrink_to_fit()
    {
        if (m_capacity > m_size)
        {
            resize(m_size);
        }
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::push_back(const_reference val)
    {
        if (m_size == m_capacity)
        {
            grow(m_size + 1);
        }

        std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, val);
        ++m_size;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::push_back(T&& val)
    {
        if (m_size == m_capacity)
        {
            grow(m_size + 1);
        }

        std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::move(val));
        ++m_size;
    }

    template<typename T, typename Allocator, typename growth_policy>
    template<typename... Args>
    inline void array_list<T, Allocator, growth_policy>::emplace_back(Args&&... args)
    {
        if (m_size == m_capacity)
        {
            grow(m_size + 1);
        }

        std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::forward<Args>(args)...);
        ++m_size;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::pop_back()
    {
        --m_size;
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + m_size);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::insert(const_iterator pos, const_reference value)
    {
        size_type index = static_cast<size_type>(pos - m_data);

        if (m_size == m_capacity)
        {
            grow(m_size + 1);
        }

        if (index < m_size)
        {
            // Construct a new element at the end of the list and shift all elements up 1
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::move(m_data[m_size - 1]));
            for (size_type i = m_size - 1; i > index; --i)
            {
                m_data[i] = std::move(m_data[i - 1]);
            }

            // Assign the value at index
            m_data[index] = value;
        }
        else
        {
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, value);
        }

        ++m_size;
        return iterator(m_data + index);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::insert(const_iterator pos, T&& value)
    {
        size_type index = static_cast<size_type>(pos - m_data);

        if (m_size == m_capacity)
        {
            grow(m_size + 1);
        }

        if (index < m_size)
        {
            // Construct a new element at the end of the list and shift all elements up 1
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::move(m_data[m_size - 1]));
            for (size_type i = m_size - 1; i > index; --i)
            {
                m_data[i] = std::move(m_data[i - 1]);
            }

            // Assign the value at index
            m_data[index] = std::move(value);
        }
        else
        {
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::move(value));
        }

        ++m_size;
        return iterator(m_data + index);
    }

    template<typename T, typename Allocator, typename growth_policy>
    template<typename... Args>
    inline array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::emplace(const_iterator pos, Args&&... args)
    {
        size_type index = static_cast<size_type>(pos - m_data);

        if (m_size == m_capacity)
        {
            grow(m_size + 1);
        }

        if (index < m_size)
        {
            // Construct a new element at the end of the list and shift all elements up 1
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::move(m_data[m_size - 1]));
            for (size_type i = m_size - 1; i > index; --i)
            {
                m_data[i] = std::move(m_data[i - 1]);
            }

            // Assign the value at index
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + index);
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + index, std::forward<Args>(args)...);
        }
        else
        {
            std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size, std::forward<Args>(args)...);
        }

        ++m_size;
        return iterator(m_data + index);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::erase(const_iterator pos)
    {
        size_type index = static_cast<size_type>(pos - m_data);

        // shift elements to the right of pos down one
        size_type i = index;
        for (; i < m_size - 1; ++i)
        {
            m_data[i] = std::move(m_data[i + 1]);
        }

        // Destroy the last element
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);

        --m_size;
        return iterator(m_data + index);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::erase(const_iterator first, const_iterator last)
    {
        size_type ifirst = static_cast<size_type>(first - m_data);

        // remove values in range [first, last)
        size_type left = ifirst;
        size_type right = static_cast<size_type>(last - m_data);
        for (; right < m_size; ++left, ++right)
        {
            m_data[left] = std::move(m_data[right]);
        }

        // destroy the remaining elements
        while (m_size > left)
        {
            --m_size;
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + m_size);
        }

        return iterator(m_data + ifirst);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::clear()
    {
        while (m_size > 0)
        {
            --m_size;
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + m_size);
        }
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::swap(array_list<T, Allocator, growth_policy>& other)
    {
        T* tempData = other.m_data;
        size_type tempSize = other.m_size;
        size_type tempCapacity = other.m_capacity;

        other.m_data = m_data;
        other.m_size = m_size;
        other.m_capacity = m_capacity;
        
        m_data = tempData;
        m_size = tempSize;
        m_capacity = tempCapacity;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::grow(size_type required_capacity)
    {
        resize(m_growth_policy(m_capacity, required_capacity));
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::resize(size_type new_capacity)
    {
        T* tempData = std::allocator_traits<Allocator>::allocate(m_alloc, new_capacity);

        // Copy container contents to new memory (tempData)
        for (size_t i = 0; i < m_size; ++i)
        {
            std::allocator_traits<Allocator>::construct(m_alloc, tempData + i, std::move_if_noexcept(m_data[i]));
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
        }

        // Deallocate old memory (m_data) and update members
        std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
        m_capacity = new_capacity;
        m_data = tempData;
    }

}

#endif