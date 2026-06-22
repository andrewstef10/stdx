#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdx/equatable.h>
#include <stdx/comparable.h>
#include <stdx/contiguous_container.h>
#include <stdx/memory.h>

#include <cstddef>
#include <utility>

namespace stdx {

    /**
     * @brief Dynamic-size array. Elements are stored contiguously, giving O(1) random access and amortized O(1) append.
     * @tparam T            Element type.
     * @tparam Allocator    Allocator type. Defaults to stdx::allocator<T>.
     * @tparam GrowthPolicy Policy controlling how capacity grows on reallocation. Defaults to stdx::doubling_growth.
     */
    template<typename T, typename Allocator = stdx::allocator<T>, typename GrowthPolicy = stdx::doubling_growth<>>
    class array_list
        : public stdx::contiguous_container<array_list<T, Allocator, GrowthPolicy>, T>
        , public stdx::equatable<array_list<T, Allocator, GrowthPolicy>>
        , public stdx::comparable<array_list<T, Allocator, GrowthPolicy>>
    {
    public:
        friend stdx::container<array_list, T>;
        friend stdx::contiguous_container<array_list, T>;

        using size_type       = typename container<array_list, T>::size_type;
        using reference       = typename container<array_list, T>::reference;
        using const_reference = typename container<array_list, T>::const_reference;

        using iterator               = typename contiguous_container<array_list, T>::iterator;
        using const_iterator         = typename contiguous_container<array_list, T>::const_iterator;
        using reverse_iterator       = typename contiguous_container<array_list, T>::reverse_iterator;
        using const_reverse_iterator = typename contiguous_container<array_list, T>::const_reverse_iterator;

        using allocator_type  = Allocator;
        using pointer         = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

        using growth_type     = GrowthPolicy;

        
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
        growth_type get_growth_policy() const noexcept { return m_growth; }


        // ===== Element Access =====

        /// @brief Returns a pointer to the underlying element storage. The range [data(), data() + size()) is always valid.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          If the container is empty, data() may be nullptr and is not dereferenceable.
        /// @return Pointer to the first element, or nullptr if empty.
        T* data() { return m_data; }
        const T* data() const { return m_data; }


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

        /// @brief Swaps the contents of this container with `other`. Iterators remain valid but now refer to the other container.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @param other Container to swap with.
        void swap(array_list& other);

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
        growth_type m_growth;

        void grow(size_type required_capacity);

        void resize(size_type new_capacity);

        /// @brief Constructs a T at `location` from `args`, routed through the allocator. Hook used by the
        ///        shared insert helpers in contiguous_container. Perfect-forwards, so it copy-, move-, or
        ///        emplace-constructs with no extra overhead.
        /// @param location Raw storage to construct into.
        /// @param args     Arguments forwarded to T's constructor.
        template<typename... Args>
        void construct(T* location, Args&&... args)
        {
            std::allocator_traits<Allocator>::construct(m_alloc, location, std::forward<Args>(args)...);
        }

        /// @brief Reallocating insert: grows the buffer and places a new element at `index` in a single pass.
        /// @details Each existing element is moved exactly once (no separate grow-then-shift). The new element is
        ///          constructed first, so a throwing element constructor leaves the container unchanged.
        /// @param index Position at which the new element is constructed (0..m_size).
        /// @param args  Arguments forwarded to T's constructor for the new element.
        /// @return Iterator to the newly constructed element.
        template<typename... Args>
        iterator emplace_realloc(size_type index, Args&&... args);
    };


    // ===== Inline array_list Implementation =====

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list()
        : contiguous_container<array_list, T>()
        , m_size(0)
        , m_data(nullptr)
        , m_capacity(0)
        , m_alloc()
        , m_growth()
    {
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list(const array_list<T, Allocator, growth_policy>& other)
        : contiguous_container<array_list, T>(other)
        , m_size(0)
        , m_data(nullptr)
        , m_capacity(0)
        , m_alloc(other.m_alloc)
        , m_growth(other.m_growth)
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
        : contiguous_container<array_list, T>(std::move(other))
        , m_size(other.m_size)
        , m_data(other.m_data)
        , m_capacity(other.m_capacity)
        , m_alloc(std::move(other.m_alloc))
        , m_growth(std::move(other.m_growth))
    {
        other.m_size = 0;
        other.m_data = nullptr;
        other.m_capacity = 0;
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::~array_list()
    {
        this->clear();
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
            this->clear();
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
            this->clear();
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
        emplace_back(val);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::push_back(T&& val)
    {
        emplace_back(std::move(val));
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
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + --m_size);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline typename array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::insert(const_iterator pos, const_reference value)
    {
        return emplace(pos, value);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline typename array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::insert(const_iterator pos, T&& value)
    {
        return emplace(pos, std::move(value));
    }

    template<typename T, typename Allocator, typename growth_policy>
    template<typename... Args>
    inline typename array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::emplace(const_iterator pos, Args&&... args)
    {
        if (m_size == m_capacity)
        {
            return emplace_realloc(static_cast<size_type>(pos - m_data), std::forward<Args>(args)...);
        }
        return this->emplace_inplace(static_cast<size_type>(pos - m_data), std::forward<Args>(args)...);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::swap(array_list<T, Allocator, growth_policy>& other)
    {
        // Only the storage (data/size/capacity) is exchanged; m_alloc and m_growth are intentionally
        // left in place. This is correct because both are stateless here. A stateful growth policy or
        // a non-always-equal allocator would require swapping (or propagating) them as well.
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
        resize(m_growth(m_capacity, required_capacity));
    }

    template<typename T, typename Allocator, typename growth_policy>
    template<typename... Args>
    inline typename array_list<T, Allocator, growth_policy>::iterator array_list<T, Allocator, growth_policy>::emplace_realloc(size_type index, Args&&... args)
    {
        const size_type NEW_CAPACITY = m_growth(m_capacity, m_size + 1);
        T* newData = std::allocator_traits<Allocator>::allocate(m_alloc, NEW_CAPACITY);

        // Construct the new element first so a throwing T constructor leaves *this unchanged.
        try
        {
            std::allocator_traits<Allocator>::construct(m_alloc, newData + index, std::forward<Args>(args)...);
        }
        catch (...)
        {
            std::allocator_traits<Allocator>::deallocate(m_alloc, newData, NEW_CAPACITY);
            throw;
        }

        // Move existing elements into the new buffer around the gap; each element is moved exactly once.
        size_type i = 0;
        for (; i < index; ++i)
        {
            std::allocator_traits<Allocator>::construct(m_alloc, newData + i, std::move_if_noexcept(m_data[i]));
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
        }
        for (; i < m_size; ++i)
        {
            std::allocator_traits<Allocator>::construct(m_alloc, newData + i + 1, std::move_if_noexcept(m_data[i]));
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
        }

        // Deallocate old memory and update members
        std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
        m_data = newData;
        m_capacity = NEW_CAPACITY;
        ++m_size;
        return iterator(m_data + index);
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline void array_list<T, Allocator, growth_policy>::resize(size_type new_capacity)
    {
        T* newData = std::allocator_traits<Allocator>::allocate(m_alloc, new_capacity);

        // Copy container contents to new memory (newData)
        for (size_t i = 0; i < m_size; ++i)
        {
            std::allocator_traits<Allocator>::construct(m_alloc, newData + i, std::move_if_noexcept(m_data[i]));
            std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
        }

        // Deallocate old memory (m_data) and update members
        std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
        m_data = newData;
        m_capacity = new_capacity;
    }

}

#endif