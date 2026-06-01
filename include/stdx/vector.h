#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>

#include <stdx/comparable.h>
#include <stdx/iterator.h>

namespace stdx {

    /**
     * @brief Dynamic size array class.
     */
    template<typename T>
    class vector
        : public comparable<vector<T>> {
    public:

        using iterator = T*;
        using const_iterator = const T*;
        using reverse_iterator = stdx::reverse_iterator<iterator>;
        using const_reverse_iterator = stdx::reverse_iterator<const_iterator>;

        
        /// @brief Default constructor.
        /// Constructs an empty container, with no elements.
        vector();

        /// @brief Copy constructor.
        /// Constructs a container with a copy of each of the elements in `other`, in the same order.
        /// `other` is left in an unspecified but valid state.
        /// @param other container to copy.
        vector(const vector& other);

        /// @brief Move constructor
        /// Constructs a container that acquires the elements of `other`.
        /// @param  
        vector(vector&& other);

        /// @brief Destructor.
        ~vector();

        /// @brief Copy assignment operator 
        /// Copies new contents to the container, replacing its current contents, and modifying its size accordingly.
        /// @param other vector to copy
        /// @return Reference to this container
        vector& operator=(const vector& other);

        /// @brief Move assignment operator
        /// Moves new contents to the container, replacing its current contents, and modifying its size accordingly.
        /// @param  
        /// @return Reference to this container
        vector& operator=(vector&&);


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

        /// @brief Returns a direct pointer to the memory array used internally by the vector to store its owned elements.
        /// The pointer is such that range [data(), data() + size()) is always a valid range.
        /// If *this is empty, data() is not dereferenceable.
        /// @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
        T* data() { return m_data; }
        const T* data() const { return m_data; }

        /// @brief Returns a reference to the last element in the container.
        /// @return Reference to the last element.
        T& back() { return m_data[m_size - 1]; }
        const T& back() const { return m_data[m_size - 1]; }

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
        iterator end() { return iterator(m_data + m_size); }
        const_iterator end() const { return const_iterator(m_data + m_size); }
        const_iterator cend() const { return const_iterator(m_data + m_size); }

        /// @brief Returns a reverse contiguous iterator to the first element of the reversed *this. It corresponds to the last element of the non-reversed *this.
        /// If *this is empty, the returned iterator will be equal to end().
        /// @return Reverse contiguous iterator to the first element.
        reverse_iterator rbegin() { return reverse_iterator(m_data + m_size); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(m_data + m_size); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(m_data + m_size); }

        /// @brief Returns a reverse contiguous iterator past the last element of the reversed *this. It corresponds to the element preceding the first element of the non-reversed *this.
        /// This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
        /// @return Reverse contiguous iterator to the element following the last element.
        reverse_iterator rend() { return reverse_iterator(m_data); }
        const_reverse_iterator rend() const { return const_reverse_iterator(m_data); }
        const_reverse_iterator crend() const { return const_reverse_iterator(m_data); }


        // ===== Capacity =====

        /// @brief Checks if the container has no elements.
        /// @return true if the container is empty, false otherwise.
        bool empty() const { return size() == 0; }

        /// @brief Returns the number of elements in the container.
        /// @return The number of elements in the container.
        std::size_t size() const { return m_size; }

        /// @brief Returns the size of the storage space currently allocated for the vector, expressed in terms of elements.
        /// @return The size of the currently allocated storage capacity in the vector, measured in terms of the number elements it can hold.
        std::size_t capacity() const { return m_capacity; }

        /// @brief Requests that the vector capacity be at least enough to contain `n` elements.
        /// If n is greater than the current vector capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
        /// In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
        /// @param n Minimum capacity for the vector. Note that the resulting vector capacity may be equal or greater than n.
        void reserve(std::size_t n);


        // ==== Modifiers ====

        /// @brief Adds a new element at the end of the vector, after its current last element.
        /// The content of val is copied to the new element.
        /// This effectively increases the container size by one, which causes an automatic reallocation of the allocated storage space if -and only if- the new vector size surpasses the current vector capacity.
        /// @param val Value to be copied to the new element.
        void push_back(const T& val);

        /// @brief Adds a new element at the end of the vector, after its current last element.
        /// The content of val is moved to the new element.
        /// This effectively increases the container size by one, which causes an automatic reallocation of the allocated storage space if -and only if- the new vector size surpasses the current vector capacity.
        /// @param val Value to be moved to the new element.
        void push_back(T&& val);

        /// @brief Exchanges the contents of the container with those of `other`. Sizes may differ.
        /// Does not cause iterators and references to associate with the other container.
        /// @param other container to exchange the contents with
        void swap(vector& other);


        // ==== Comparison ====

        /// @brief Compare two vector's for equality.
        /// The equality comparison is performed by comparing the elements sequentially using operator==, stopping at the first mismatch.
        /// NOTE: T must be equality comparable and implement operator==
        /// @param other Other vector
        /// @return True if this is equal to `other`, false otherwise.
        bool equals(const vector& other) const;

        /// @brief Get if this vector is less than `other`
        /// NOTE: T must be less than comparable and implement operator<
        /// @param other Other vector
        /// @return True if this vector is less thn other, false otherwise
        bool less_than(const vector& other) const;

    private:

        /// @brief Pointer to underlying array of elements
        T* m_data;

        /// @brief Current number of elements stored in the container
        std::size_t m_size;

        /// @brief Size of the allocated storage capacity
        std::size_t m_capacity;
    };


    // ==== vector Non Member functions ====


    // ===== Inline vector Implementation =====

    template<typename T>
    inline vector<T>::vector()
    : m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    {
    }

    template<typename T>
    inline vector<T>::vector(const vector<T>& other)
    : m_size(other.m_size)
    , m_capacity(other.m_capacity)
    , m_data(nullptr)
    {

    }

    template<typename T>
    inline vector<T>::vector(vector<T>&& other)
    {

    }

    template<typename T>
    inline vector<T>::~vector()
    {

    }

    template<typename T>
    inline vector<T>& vector<T>::operator=(const vector<T>& other)
    {
        return *this;
    }

    template<typename T>
    inline vector<T>& vector<T>::operator=(vector<T>&& other)
    {
        return *this;
    }

    template<typename T>
    inline T& vector<T>::at(std::size_t index)
    {
        if (index >= m_size) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the vector");
        }
        return m_data[index];
    }

    template<typename T>
    inline const T& vector<T>::at(std::size_t index) const
    {
        if (index >= m_size) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the vector");
        }
        return m_data[index];
    }

    template<typename T>
    inline bool vector<T>::equals(const vector& other) const
    {
        
    }

    template<typename T>
    inline bool vector<T>::less_than(const vector& other) const
    {
        
    }

    template<typename T>
    void vector<T>::reserve(std::size_t n)
    {

    }

    template<typename T>
    void vector<T>::push_back(const T& val)
    {

    }

    template<typename T>
    void vector<T>::push_back(T&& val)
    {

    }

    template<typename T>
    inline void vector<T>::swap(vector<T>& other)
    {
        
    }

}

#endif