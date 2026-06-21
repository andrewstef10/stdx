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
        using pointer         = std::allocator_traits<Allocator>::pointer;
        using const_pointer   = std::allocator_traits<Allocator>::const_pointer;

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
    };


    // ===== Inline array_list Implementation =====

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list()
    : contiguous_container<array_list, T>()
    , m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc()
    , m_growth()
    {
    }

    template<typename T, typename Allocator, typename growth_policy>
    inline array_list<T, Allocator, growth_policy>::array_list(const array_list<T, Allocator, growth_policy>& other)
    : contiguous_container<array_list, T>(other)
    , m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
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
    , m_capacity(other.m_capacity)
    , m_data(other.m_data)
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
        resize(m_growth(m_capacity, required_capacity));
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



    /**
     * @brief Fixed-capacity array list. Elements are stored contiguously in a stack-allocated buffer, giving O(1) random access and O(1) append up to N elements.
     * @details The capacity is fixed at N elements at compile time. No heap memory is ever allocated or freed.
     *          All operations that would exceed the capacity throw std::length_error.
     * @tparam T Element type.
     * @tparam N Maximum number of elements the container can hold.
     */
    template<typename T, std::size_t N>
    class fixed_array_list
        : public stdx::contiguous_container<fixed_array_list<T, N>, T>
        , public stdx::equatable<fixed_array_list<T, N>>
        , public stdx::comparable<fixed_array_list<T, N>>
    {
    public:
        friend stdx::container<fixed_array_list, T>;
        friend stdx::contiguous_container<fixed_array_list, T>;

        using size_type       = typename container<fixed_array_list, T>::size_type;
        using reference       = typename container<fixed_array_list, T>::reference;
        using const_reference = typename container<fixed_array_list, T>::const_reference;

        using iterator               = typename contiguous_container<fixed_array_list, T>::iterator;
        using const_iterator         = typename contiguous_container<fixed_array_list, T>::const_iterator;
        using reverse_iterator       = typename contiguous_container<fixed_array_list, T>::reverse_iterator;
        using const_reverse_iterator = typename contiguous_container<fixed_array_list, T>::const_reverse_iterator;


        // ==== Constructors / Destructor ====

        /// @brief Default constructor. Constructs an empty container; no elements are live but the internal buffer is ready.
        /// @details Time:  O(1)
        ///          Space: O(1) — N * sizeof(T) bytes are always reserved on the stack regardless of element count
        fixed_array_list();

        /// @brief Copy constructor. Constructs a container with a copy of each element in `other`, in the same order.
        /// @details Time:  O(n), where n = other.size()
        ///          Space: O(1)
        /// @param other Container to copy from.
        fixed_array_list(const fixed_array_list& other);

        /// @brief Move constructor. Moves each element individually from `other`; the buffer cannot be transferred.
        ///        `other` is left in a valid empty state after the operation.
        /// @details Time:  O(n), where n = other.size() — elements are moved one-by-one since each object owns its own stack buffer
        ///          Space: O(1)
        /// @param other Container to move from.
        fixed_array_list(fixed_array_list&& other);

        /// @brief Destructor. Destroys all live elements; the stack buffer is reclaimed with the object.
        /// @details Time:  O(n), where n = size()
        ///          Space: O(1)
        ~fixed_array_list();

        /// @brief Copy assignment operator. Replaces the contents with a copy of `other`.
        /// @details Time:  O(n), where n = max(size(), other.size())
        ///          Space: O(1) — no allocation is performed
        /// @param other Container to copy from.
        /// @return Reference to this container.
        fixed_array_list& operator=(const fixed_array_list& other);

        /// @brief Move assignment operator. Replaces the contents by moving each element from `other` into this container's buffer.
        ///        `other` is left in a valid empty state after the operation.
        /// @details Time:  O(n), where n = other.size() — elements are moved one-by-one since each object owns its own stack buffer
        ///          Space: O(1) — no allocation is performed
        /// @param other Container to move from.
        /// @return Reference to this container.
        fixed_array_list& operator=(fixed_array_list&& other);


        // ==== Capacity ====

        /// @brief Returns the maximum number of elements this container can hold. Always equal to N.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          Overrides container::max_size() to reflect the compile-time fixed limit.
        /// @return N — the compile-time fixed capacity.
        size_type constexpr max_size() const noexcept { return static_cast<size_type>(N); }

        /// @brief Returns the number of elements this container can hold without throwing. Always equal to N.
        /// @details Time:  O(1)
        ///          Space: O(1)
        /// @return N — the compile-time fixed capacity.
        size_type constexpr capacity() const noexcept { return static_cast<size_type>(N); }


        // ==== Modifiers ====

        /// @brief Appends a copy of `val` to the end of the container.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          Invalidates the end() iterator.
        /// @param val Value to copy-construct into the new element.
        /// @exception std::length_error If size() == N.
        void push_back(const_reference val);

        /// @brief Appends `val` to the end of the container by moving it.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          Invalidates the end() iterator.
        /// @param val Value to move-construct into the new element.
        /// @exception std::length_error If size() == N.
        void push_back(T&& val);

        /// @brief Constructs an element in-place at the end of the container, forwarding `args` to T's constructor.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          Invalidates the end() iterator.
        /// @param args Arguments to forward to the constructor of T.
        /// @exception std::length_error If size() == N.
        template<typename... Args>
        void emplace_back(Args&&... args);

        /// @brief Removes the last element. Behavior is undefined if the container is empty.
        /// @details Time:  O(1)
        ///          Space: O(1)
        ///          Invalidates the end() iterator and any reference to the last element.
        void pop_back();

        /// @brief Inserts a copy of `value` before `pos`, shifting elements after `pos` to the right.
        /// @details Time:  O(n) — shifts elements after pos to make room
        ///          Space: O(1) — no allocation is performed
        ///          Invalidates iterators and references from pos onward.
        /// @param pos Iterator before which the element is inserted. May be end().
        /// @param value Value to copy-construct into the new element.
        /// @return Iterator to the inserted element.
        /// @exception std::length_error If size() == N.
        iterator insert(const_iterator pos, const_reference value);

        /// @brief Inserts `value` before `pos` by moving it, shifting elements after `pos` to the right.
        /// @details Time:  O(n) — shifts elements after pos to make room
        ///          Space: O(1) — no allocation is performed
        ///          Invalidates iterators and references from pos onward.
        /// @param pos Iterator before which the element is inserted. May be end().
        /// @param value Value to move-construct into the new element.
        /// @return Iterator to the inserted element.
        /// @exception std::length_error If size() == N.
        iterator insert(const_iterator pos, T&& value);

        /// @brief Constructs an element in-place before `pos`, forwarding `args` to T's constructor.
        /// @details Time:  O(n) — shifts elements after pos to make room
        ///          Space: O(1) — no allocation is performed
        ///          Invalidates iterators and references from pos onward.
        /// @param pos Iterator before which the element is constructed. May be end().
        /// @param args Arguments to forward to the constructor of T.
        /// @return Iterator to the emplaced element.
        /// @exception std::length_error If size() == N.
        template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        /// @brief Removes the element at `pos`, shifting elements after `pos` to the left.
        /// @details Time:  O(n) — shifts elements after pos forward by one
        ///          Space: O(1)
        ///          Invalidates the iterator at pos and all iterators/references after it.
        /// @param pos Iterator to the element to remove.
        /// @return Iterator to the element that followed the removed one, or end() if pos was the last element.
        iterator erase(const_iterator pos);

        /// @brief Removes all elements in the range [first, last), shifting elements after `last` to the left.
        /// @details Time:  O(n) — shifts elements after last forward
        ///          Space: O(1)
        ///          Invalidates iterators from first onward. Returns last unchanged if the range is empty.
        /// @param first Iterator to the first element to remove.
        /// @param last  Iterator past the last element to remove.
        /// @return Iterator to the element that followed the last removed element.
        iterator erase(const_iterator first, const_iterator last);

        /// @brief Destroys all elements. size() becomes zero; capacity() remains N.
        /// @details Time:  O(n) — destructs each element
        ///          Space: O(1)
        ///          Invalidates all iterators and references to elements.
        void clear();

        /// @brief Swaps the contents of this container with `other` element-by-element.
        ///        Iterators remain valid but now refer to the other container's elements.
        /// @details Time:  O(n) — swaps elements one-by-one since each object owns its own stack buffer
        ///          Space: O(1)
        /// @param other Container to swap with.
        void swap(fixed_array_list& other);

    private:

        /// @brief Current number of live elements in the container.
        size_type m_size;

        /// @brief Fixed sized storage for this container
        T m_data[N == 0 ? 1 : N];
    };


    // ===== Inline fixed_array_list Implementation =====

    template<typename T, std::size_t N>
    inline fixed_array_list<T, N>::fixed_array_list()
        : m_size(0)
    {
    }

    template<typename T, std::size_t N>
    inline fixed_array_list<T, N>::fixed_array_list(const fixed_array_list& other)
        : m_size(0)
    {
    }

    template<typename T, std::size_t N>
    inline fixed_array_list<T, N>::fixed_array_list(fixed_array_list&& other)
        : m_size(0)
    {
    }

    template<typename T, std::size_t N>
    inline fixed_array_list<T, N>::~fixed_array_list()
    {
    }

    template<typename T, std::size_t N>
    inline fixed_array_list<T, N>& fixed_array_list<T, N>::operator=(const fixed_array_list& other)
    {
        return *this;
    }

    template<typename T, std::size_t N>
    inline fixed_array_list<T, N>& fixed_array_list<T, N>::operator=(fixed_array_list&& other)
    {
        return *this;
    }

    template<typename T, std::size_t N>
    inline void fixed_array_list<T, N>::push_back(const_reference val)
    {
    }

    template<typename T, std::size_t N>
    inline void fixed_array_list<T, N>::push_back(T&& val)
    {
    }

    template<typename T, std::size_t N>
    template<typename... Args>
    inline void fixed_array_list<T, N>::emplace_back(Args&&... args)
    {
    }

    template<typename T, std::size_t N>
    inline void fixed_array_list<T, N>::pop_back()
    {
    }

    template<typename T, std::size_t N>
    inline typename fixed_array_list<T, N>::iterator fixed_array_list<T, N>::insert(const_iterator pos, const_reference value)
    {
        return iterator{};
    }

    template<typename T, std::size_t N>
    inline typename fixed_array_list<T, N>::iterator fixed_array_list<T, N>::insert(const_iterator pos, T&& value)
    {
        return iterator{};
    }

    template<typename T, std::size_t N>
    template<typename... Args>
    inline typename fixed_array_list<T, N>::iterator fixed_array_list<T, N>::emplace(const_iterator pos, Args&&... args)
    {
        return iterator{};
    }

    template<typename T, std::size_t N>
    inline typename fixed_array_list<T, N>::iterator fixed_array_list<T, N>::erase(const_iterator pos)
    {
        return iterator{};
    }

    template<typename T, std::size_t N>
    inline typename fixed_array_list<T, N>::iterator fixed_array_list<T, N>::erase(const_iterator first, const_iterator last)
    {
        return iterator{};
    }

    template<typename T, std::size_t N>
    inline void fixed_array_list<T, N>::clear()
    {
    }

    template<typename T, std::size_t N>
    inline void fixed_array_list<T, N>::swap(fixed_array_list& other)
    {
    }

}

#endif