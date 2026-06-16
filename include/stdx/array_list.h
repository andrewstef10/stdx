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
     * @brief Dynamic size array class.
     */
    template<typename T, typename allocator = stdx::allocator<T>, typename growth_policy = exact_growth<>>
    class array_list
        : public container<array_list<T, allocator, growth_policy>>
        , public comparable<array_list<T, allocator, growth_policy>>
    {
    public:

        using iterator = T*;
        using const_iterator = const T*;
        using reverse_iterator = stdx::reverse_iterator<iterator>;
        using const_reverse_iterator = stdx::reverse_iterator<const_iterator>;

        
        /// @brief Default constructor.
        /// Constructs an empty container, with no elements.
        array_list();

        /// @brief Copy constructor.
        /// Constructs a container with a copy of each of the elements in `other`, in the same order.
        /// `other` is left in an unspecified but valid state.
        /// @param other container to copy.
        array_list(const array_list& other);

        /// @brief Move constructor
        /// Constructs a container that acquires the elements of `other`.
        /// @param  
        array_list(array_list&& other);

        /// @brief Destructor.
        ~array_list();

        /// @brief Copy assignment operator 
        /// Copies new contents to the container, replacing its current contents, and modifying its size accordingly.
        /// @param other array_list to copy
        /// @return Reference to this container
        array_list& operator=(const array_list& other);

        /// @brief Move assignment operator
        /// Moves new contents to the container, replacing its current contents, and modifying its size accordingly.
        /// @param  
        /// @return Reference to this container
        array_list& operator=(array_list&&);

        /// @brief Returns the allocator associated with the container.
        /// @details Complexity: Constant
        /// @return The associated allocator.
        allocator get_allocator() const noexcept { return m_alloc; }

        /// @brief Returns the growth policy associated with the container.
        /// @details Complexity: Constant
        /// @return The associated growth policy.
        growth_policy get_growth_policy() const noexcept { return m_growth_policy; }


        // ===== Element Access =====

        /// @brief Returns a reference to the element at specified location `index`, with bounds checking.
        /// If `index` is not within the range of the container, an exception of type std::out_of_range is thrown.
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        /// @exception std::out_of_range if `index` is out of range (index >= size()).
        T& at(std::size_t index);
        const T& at(std::size_t index) const;

        /// @brief Returns a reference to the element at specified location `index`, without bounds checking.
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        T& operator[](std::size_t index) { return m_data[index]; }
        const T& operator[](std::size_t index) const { return m_data[index]; }

        /// @brief Returns a direct pointer to the memory array used internally by the array_list to store its owned elements.
        /// The pointer is such that range [data(), data() + size()) is always a valid range.
        /// If *this is empty, data() is not dereferenceable.
        /// @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
        T* data() { return m_data; }
        const T* data() const { return m_data; }

        /// @brief Returns a reference to the last element in the container.
        /// @return Reference to the last element.
        T& back() { return m_data[this->m_size - 1]; }
        const T& back() const { return m_data[this->m_size - 1]; }

        /// @brief Returns a reference to the first element in the container.
        /// @return Reference to the first element.
        T& front() { return m_data[0]; }
        const T& front() const { return m_data[0]; }


        // ==== Iterators ====

        /// @brief Returns a contiguous iterator to the first element of *this.
        /// If *this is empty, the returned iterator will be equal to end().
        /// @return contiguous iterator to the first element.
        iterator begin() { return iterator(m_data); }
        const_iterator begin() const { return const_iterator(m_data); }
        const_iterator cbegin() const { return const_iterator(m_data); }

        /// @brief Returns a contiguous iterator past the last element of *this.
        /// This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
        /// @return contiguous iterator past the last element.
        iterator end() { return iterator(m_data + this->m_size); }
        const_iterator end() const { return const_iterator(m_data + this->m_size); }
        const_iterator cend() const { return const_iterator(m_data + this->m_size); }

        /// @brief Returns a reverse contiguous iterator to the first element of the reversed *this. It corresponds to the last element of the non-reversed *this.
        /// If *this is empty, the returned iterator will be equal to end().
        /// @return Reverse contiguous iterator to the first element.
        reverse_iterator rbegin() { return reverse_iterator(m_data + this->m_size); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(m_data + this->m_size); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(m_data + this->m_size); }

        /// @brief Returns a reverse contiguous iterator past the last element of the reversed *this. It corresponds to the element preceding the first element of the non-reversed *this.
        /// This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
        /// @return Reverse contiguous iterator to the element following the last element.
        reverse_iterator rend() { return reverse_iterator(m_data); }
        const_reverse_iterator rend() const { return const_reverse_iterator(m_data); }
        const_reverse_iterator crend() const { return const_reverse_iterator(m_data); }


        // ===== Capacity =====

        /// @brief Returns the size of the storage space currently allocated for the array_list, expressed in terms of elements.
        /// @return The size of the currently allocated storage capacity in the array_list, measured in terms of the number elements it can hold.
        std::size_t capacity() const { return m_capacity; }

        /// @brief Requests that the array_list capacity be at least enough to contain `n` elements.
        /// If n is greater than the current array_list capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
        /// In all other cases, the function call does not cause a reallocation and the array_list capacity is not affected.
        /// @param n Minimum capacity for the array_list. Note that the resulting array_list capacity may be equal or greater than n.
        void reserve(std::size_t n);

        /// @brief Requests the removal of unused capacity.
        /// If reallocation occurs, all iterators (including the end() iterator) and all references to the elements are invalidated.
        /// If no reallocation occurs, no iterators or references are invalidated.
        void shrink_to_fit();


        // ==== Modifiers ====

        /// @brief Adds a new element at the end of the array_list, after its current last element.
        /// The content of val is copied to the new element.
        /// This effectively increases the container size by one, which causes an automatic reallocation of the allocated storage space if -and only if- the new array_list size surpasses the current array_list capacity.
        /// @param val Value to be copied to the new element.
        void push_back(const T& val);

        /// @brief Adds a new element at the end of the array_list, after its current last element.
        /// The content of val is moved to the new element.
        /// This effectively increases the container size by one, which causes an automatic reallocation of the allocated storage space if -and only if- the new array_list size surpasses the current array_list capacity.
        /// @param val Value to be moved to the new element.
        void push_back(T&& val);

        /// @brief Removes the last element of the container.
        /// If empty() is true, the behavior is undefined.
        void pop_back();

        /// @brief Inserts an element at the specified location in the container.
        /// @param pos iterator before which the content will be inserted (pos may be the end() iterator)
        /// @param value Element value to insert. Copy of the value will be made
        /// @return Iterator pointing to the inserted value.
        iterator insert(const_iterator pos, const T& value);

        /// @brief Inserts an element at the specified location in the container.
        /// @param pos iterator before which the content will be inserted (pos may be the end() iterator)
        /// @param value Element value to insert. Value will be moved.
        /// @return Iterator pointing to the inserted value.
        iterator insert(const_iterator pos, T&& value);

        /// @brief Erases the specified element from the container.
        /// @param pos iterator to the element to remove
        /// @return Iterator following the last removed element. If pos refers to the last element, then the end() iterator is returned.
        iterator erase(iterator pos);
        iterator erase(const_iterator pos);

        /// @brief Erases the specified elements from the container.
        /// Removes elements in the range [first, last)
        /// @param first the first of two iterators defining the range of elements to remove
        /// @param last the last of two iterators defining the range of elements to remove
        /// @return Iterator following the last removed element. If [first, last) is an empty range, then last is returned.
        iterator erase(iterator first, iterator last);
        iterator erase(const_iterator first, const_iterator last);

        /// @brief Erases all elements from the container. After this call, size() returns zero.
        /// @details Invalidates any references, pointers, and iterators referring to contained elements. Any past-the-end iterators are also invalidated.
        ///          Complexity:
        ///             - Time: Linear (O(n)) where n is the size of the container, i.e., the number of elements
        ///             - Space: Constant (O(1))
        /// Leaves the capacity() of the vector unchanged.
        void clear();

        /// @brief Exchanges the contents of the container with those of `other`. Sizes may differ.
        /// Does not cause iterators and references to associate with the other container.
        /// @param other container to exchange the contents with
        void swap(array_list& other);


        // ==== Comparison ====

        /// @brief Compare two array_list's for equality.
        /// The equality comparison is performed by comparing the elements sequentially using operator==, stopping at the first mismatch.
        /// NOTE: T must be equality comparable and implement operator==
        /// @param other Other array_list
        /// @return True if this is equal to `other`, false otherwise.
        bool equals(const array_list& other) const;

        /// @brief Get if this array_list is less than `other`
        /// NOTE: T must be less than comparable and implement operator<
        /// @param other Other array_list
        /// @return True if this array_list is less than other, false otherwise
        bool less_than(const array_list& other) const;

    private:

        /// @brief Pointer to underlying array of elements
        T* m_data;

        /// @brief Size of the allocated storage capacity
        std::size_t m_capacity;

        /// @brief Allocator for this array_list
        allocator m_alloc;

        /// @brief Growth policy for this data structure
        growth_policy m_growth_policy;

        void grow(std::size_t required_capacity);
    };


    // ==== array_list Non Member functions ====


    // ===== Inline array_list Implementation =====

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::array_list()
    : container<array_list>()
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc()
    , m_growth_policy()
    {
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::array_list(const array_list<T, allocator, growth_policy>& other)
    : container<array_list>(other)
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
        for (; this->m_size < other.m_size; ++this->m_size)
        {
            std::allocator_traits<allocator>::construct(m_alloc, m_data + this->m_size, other.m_data[this->m_size]);
        }
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::array_list(array_list<T, allocator, growth_policy>&& other)
    : container<array_list>(std::move(other))
    , m_capacity(other.m_capacity)
    , m_data(other.m_data)
    , m_alloc(std::move(other.m_alloc))
    , m_growth_policy(std::move(other.m_growth_policy))
    {
        other.m_data = nullptr;
        other.m_capacity = 0;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::~array_list()
    {
        clear();
        std::allocator_traits<allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>& array_list<T, allocator, growth_policy>::operator=(const array_list<T, allocator, growth_policy>& other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (std::allocator_traits<allocator>::propagate_on_container_copy_assignment::value)
        {
            // allocator should be copied. Need to deallocate our own memory and reallocate with a copy of other's allocator.
            std::allocator_traits<allocator>::deallocate(m_alloc, m_data, m_capacity);
            m_capacity = 0;
            m_alloc = other.m_alloc;
        }

        // Grow capacity to at least the size of other if needed
        if (m_capacity < other.m_size)
        {
            grow(other.m_size);
        }

        // Copy assign existing elements in the container
        std::size_t smallerSize = this->m_size < other.m_size ? this->m_size : other.m_size;
        for (std::size_t i = 0; i < smallerSize; ++i)
        {
            m_data[i] = other.m_data[i];
        }

        // Copy construct the remaining elements in other
        for (; this->m_size < other.m_size; ++this->m_size)
        {
            std::allocator_traits<allocator>::construct(m_alloc, m_data + this->m_size, other.m_data[this->m_size]);
        }

        return *this;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>& array_list<T, allocator, growth_policy>::operator=(array_list<T, allocator, growth_policy>&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        clear(); // clear the current elements

        constexpr bool POCMA = std::allocator_traits<allocator>::propagate_on_container_move_assignment::value;
        if (POCMA || m_alloc == other.m_alloc)
        {
            // Either our allocator and the memory our allocator allocates need to stay together (if POCMA) OR allocators are equal
            // Its safe to steal other's buffer and deallocate our own O(n)
            std::allocator_traits<allocator>::deallocate(m_alloc, m_data, m_capacity);

            if (POCMA)
            {
                m_alloc = std::move(other.m_alloc);
            }

            container<array_list>::operator=(std::move(other));
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            other.m_data = nullptr;
            other.m_capacity = 0;
        }
        else
        {
            // POCMA false AND allocators are not equal
            // We must perform element-wise move into own storage, O(n)
            if (m_capacity < other.m_size)
            {
                grow(other.m_size);
            }

            for (; this->m_size < other.m_size; ++this->m_size)
            {
                std::allocator_traits<allocator>::construct(m_alloc, m_data + this->m_size, std::move(other.m_data[this->m_size]));
            }
        }

        return *this;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline T& array_list<T, allocator, growth_policy>::at(std::size_t index)
    {
        if (index >= this->m_size) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the array_list");
        }
        return m_data[index];
    }

    template<typename T, typename allocator, typename growth_policy>
    inline const T& array_list<T, allocator, growth_policy>::at(std::size_t index) const
    {
        if (index >= this->m_size) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the array_list");
        }
        return m_data[index];
    }

    template<typename T, typename allocator, typename growth_policy>
    inline bool array_list<T, allocator, growth_policy>::equals(const array_list<T, allocator, growth_policy>& other) const
    {
        for (size_t i = 0; i < this->m_size; ++i)
        {
            if (!(m_data[i] == other.m_data[i]))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline bool array_list<T, allocator, growth_policy>::less_than(const array_list<T, allocator, growth_policy>& other) const
    {
        return false;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::reserve(std::size_t n)
    {

    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::shrink_to_fit()
    {

    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::push_back(const T& val)
    {
        if (this->m_size == m_capacity)
        {
            grow(this->m_size + 1);
        }

        m_data[this->m_size] = val;
        ++this->m_size;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::push_back(T&& val)
    {
        if (this->m_size == m_capacity)
        {
            grow(this->m_size + 1);
        }

        m_data[this->m_size] = std::move(val);
        ++this->m_size;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::pop_back()
    {
        --this->m_size;
        std::allocator_traits<allocator>::destroy(m_alloc, m_data + this->m_size);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::iterator array_list<T, allocator, growth_policy>::insert(const_iterator pos, const T& value)
    {
        return iterator(m_data);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::iterator array_list<T, allocator, growth_policy>::insert(const_iterator pos, T&& value)
    {
        return iterator(m_data);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::iterator array_list<T, allocator, growth_policy>::erase(iterator pos)
    {
        return iterator(m_data);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::iterator array_list<T, allocator, growth_policy>::erase(const_iterator pos)
    {
        return iterator(m_data);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::iterator array_list<T, allocator, growth_policy>::erase(iterator first, iterator last)
    {
        return iterator(m_data);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline array_list<T, allocator, growth_policy>::iterator array_list<T, allocator, growth_policy>::erase(const_iterator first, const_iterator last)
    {
        return iterator(m_data);
    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::clear()
    {
        while (this->m_size > 0)
        {
            --this->m_size;
            std::allocator_traits<allocator>::destroy(m_alloc, m_data + this->m_size);
        }
    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::swap(array_list<T, allocator, growth_policy>& other)
    {
        T* tempData = other.m_data;
        std::size_t tempSize = other.m_size;
        std::size_t tempCapacity = other.m_capacity;

        other.m_data = m_data;
        other.m_size = this->m_size;
        other.m_capacity = m_capacity;
        
        m_data = tempData;
        this->m_size = tempSize;
        m_capacity = tempCapacity;
    }

    template<typename T, typename allocator, typename growth_policy>
    inline void array_list<T, allocator, growth_policy>::grow(std::size_t required_capacity)
    {
        // Use growth policy to calculate the new capacity then allocate
        std::size_t newCapacity = m_growth_policy(m_capacity, required_capacity);
        T* tempData = std::allocator_traits<allocator>::allocate(m_alloc, newCapacity);

        // Copy contents to new memory
        for (size_t i = 0; i < this->m_size; ++i)
        {
            std::allocator_traits<allocator>::construct(m_alloc, tempData + i, std::move_if_noexcept(m_data[i]));
            std::allocator_traits<allocator>::destroy(m_alloc, m_data + i);
        }

        // Deallocate and assign members to temp vars
        std::allocator_traits<allocator>::deallocate(m_alloc, m_data, m_capacity);
        m_capacity = newCapacity;
        m_data = tempData;
    }

}

#endif