#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <stdexcept>

#include <stdads/Iterator.h>

namespace stdads {

    /**
     * @brief Fixed size contiguous array class.
     */
    template<typename T, std::size_t N>
    struct Array {

        using Iterator = T*;
        using ConstIterator = const T*;
        using ReverseIterator = stdads::ReverseIterator<Iterator>;
        using ConstReverseIterator = stdads::ReverseIterator<ConstIterator>;


        // /// @brief Implicitly defined default constructor.
        // /// Creates an Array with the default compiler generated constructor for arrays.
        // /// If T is a class type, each element's default constructor will be called.
        // /// If T is a primitive type, the array will be initalized with garbage data.
        // Array() = default;

        // /// @brief Implicitly defined Copy constructor.
        // Array(const Array&) = default;

        // /// @brief Implicitly defined Move constructor.
        // Array(Array&&) = default;

        // /// @brief Implicitly defined destructor.
        // ~Array() = default;

        // /// @brief Implicitly defined assignment operator.
        // /// Overwrites every element of the array with a copy of the corresponding element of another array.
        // /// @return Reference to this object.
        // Array& operator=(const Array&) = default;

        // /// @brief Implicitly defined move assignment operator.
        // /// Overwrites every element of the array with the corresponding element of another array.
        // /// @return Reference to this object.
        // Array& operator=(Array&&) = default;


        // ===== Element Access =====

        /// @brief Returns a reference to the element at specified location pos, with bounds checking.
        /// If pos is not within the range of the container, an exception of type std::out_of_range is thrown.
        /// @param pos Position of the element to return.
        /// @return Reference to the requested element.
        /// @exception std::out_of_range if pos is out of range (pos >= Size()).
        T& At(std::size_t pos);
        const T& At(std::size_t pos) const;

        /// @brief Returns a reference to the element at specified location pos, without bounds checking.
        /// @param index Position of the element to return.
        /// @return Reference to the requested element.
        T& operator[](std::size_t index) { return data_[index]; }
        const T& operator[](std::size_t index) const { return data_[index]; }

        /// @brief Returns a pointer to the underlying array serving as element storage.
        /// The pointer is such that range [data(), data() + size()) is always a valid range.
        /// If *this is empty, data() is not dereferenceable.
        /// @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
        T* Data() { return data_; }
        const T* Data() const { return data_; }

        /// @brief Returns a reference to the last element in the container.
        /// @return Reference to the last element.
        T& Back() { return data_[N - 1]; }
        const T& Back() const { return data_[N - 1]; }

        /// @brief Returns a reference to the first element in the container.
        /// @return Reference to the first element.
        T& Front() { return data_[0]; }
        const T& Front() const { return data_[0]; }


        // ==== Iterators ====

        /// @brief Returns a contiguous iterator to the first element of *this.
        /// If *this is empty, the returned iterator will be equal to end().
        /// @return contiguous iterator to the first element.
        Iterator Begin() { return Iterator(data_); }
        ConstIterator Begin() const { return ConstIterator(data_); }
        ConstIterator CBegin() const { return ConstIterator(data_); }

        /// @brief Returns a contiguous iterator past the last element of *this.
        /// This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
        /// @return contiguous iterator past the last element.
        Iterator End() { return Iterator(data_ + N); }
        ConstIterator End() const { return ConstIterator(data_ + N); }
        ConstIterator CEnd() const { return ConstIterator(data_ + N); }

        /// @brief Returns a reverse contiguous iterator to the first element of the reversed *this. It corresponds to the last element of the non-reversed *this.
        /// If *this is empty, the returned iterator will be equal to end().
        /// @return Reverse contiguous iterator to the first element.
        ReverseIterator RBegin() { return ReverseIterator(data_ + N); }
        ConstReverseIterator RBegin() const { return ConstReverseIterator(data_ + N); }
        ConstReverseIterator CRBegin() const { return ConstReverseIterator(data_ + N); }

        /// @brief Returns a reverse contiguous iterator past the last element of the reversed *this. It corresponds to the element preceding the first element of the non-reversed *this.
        /// This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
        /// @return Reverse contiguous iterator to the element following the last element.
        ReverseIterator REnd() { return ReverseIterator(data_); }
        ConstReverseIterator REnd() const { return ConstReverseIterator(data_); }
        ConstReverseIterator CREnd() const { return ConstReverseIterator(data_); }


        // ===== Capacity =====

        /// @brief Checks if the container has no elements.
        /// @return true if the container is empty, false otherwise.
        bool Empty() const { return Size() == 0; }

        /// @brief Returns the number of elements in the container.
        /// @return The number of elements in the container.
        std::size_t Size() const { return N; }


        // ==== Operations ====

        /// @brief Assigns the value to all elements in the container.
        /// @param value The value to assign to the elements.
        void Fill(const T& value);

        /// @brief Exchanges the contents of the container with those of other.
        /// Does not cause iterators and references to associate with the other container.
        /// @param other container to exchange the contents with
        void Swap(Array& other);


        // ==== Comparison ====

        /// @brief Compare two arrays for equality.
        /// The equality comparison is performed by comparing the elements sequentially using operator==, stopping at the first mismatch.
        /// @param other Other array
        /// @return True if this is equal to other, false otherwise.
        bool Equals(const Array& other) const;

        /// @brief Get if this array is less than other
        /// The less than comparison is performed by comparing the elements sequentially using operator<, stopping at the first mismatch.
        /// @param other Other array
        /// @return True if this array is less thn other, false otherwise
        bool LessThan(const Array& other) const;

        /// @brief Equality operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is equal to rhs, false otherwise.
        friend bool operator==(const Array& lhs, const Array& rhs) { return lhs.Equals(rhs); }

        /// @brief Inequality operator 
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is not equal to rhs, false otherwise.
        friend bool operator!=(const Array& lhs, const Array& rhs) { return !lhs.Equals(rhs); }

        /// @brief Less than operator
        /// @param lhs left hand sid
        /// @param rhs right had side
        /// @return True if lhs is less than rhs, false otherwise.
        friend bool operator<(const Array& lhs, const Array& rhs) { return lhs.LessThan(rhs); }

        /// @brief Greater than operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is greater than rhs, false otherwise.
        friend bool operator>(const Array& lhs, const Array& rhs) { return rhs.LessThan(lhs); }

        /// @brief Less than or equal to operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is less than or equal to rhs, false otherwise.
        friend bool operator<=(const Array& lhs, const Array& rhs) { return !rhs.LessThan(lhs); }

        /// @brief Greater than or equal to operator
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return True if lhs is greater than or equal to rhs, false otherwise.
        friend bool operator>=(const Array& lhs, const Array& rhs) { return !lhs.LessThan(rhs); }

    
        T data_[N];
    };


    // ==== Array Non Member functions ====

    /// @brief Combines two arrays
    /// @tparam M Size of the fhs Array
    /// @param lhs Left hand side Array
    /// @param rhs Right hand side Array
    /// @return An Array size N + M with contents of lhs and then rhs
    template<typename T, std::size_t N, std::size_t M>
    Array<T, N + M> operator+(const Array<T, N>& lhs, const Array<T, M>& rhs);


    // ===== Inline Array Implementation =====

    template<typename T, std::size_t N>
    inline T& Array<T, N>::At(std::size_t pos)
    {
        if (pos >= N) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the array");
        }
        return data_[pos];
    }

    template<typename T, std::size_t N>
    inline const T& Array<T, N>::At(std::size_t pos) const
    {
        if (pos >= N) // size_t can't be negative
        {
            throw std::out_of_range("Index outside the bounds of the array");
        }
        return data_[pos];
    }

    template<typename T, std::size_t N>
    inline void Array<T, N>::Fill(const T& value)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            data_[i] = value;
        }
    }

    template<typename T, std::size_t N>
    inline void Array<T, N>::Swap(Array<T, N>& other)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            T temp = other.data_[i];
            other.data_[i] = data_[i];
            data_[i] = temp;
        }
    }

    template<typename T, std::size_t N>
    inline bool Array<T, N>::Equals(const Array& other) const
    {
        bool equals = true;
        for (std::size_t i = 0; equals && i < N; ++i)
        {
            equals = data_[i] == other.data_[i];
        }
        return equals;
    }

    template<typename T, std::size_t N>
    inline bool Array<T, N>::LessThan(const Array& other) const
    {
        bool lessThan = false;
        for (std::size_t i = 0; i < N; ++i)
        {
            if (!(data_[i] == other.data_[i]))
            {
                lessThan = data_[i] < other.data_[i];
                break;
            }
        }
        return lessThan;
    }

    template<typename T, std::size_t N, std::size_t M>
    inline Array<T, N + M> operator+(const Array<T, N>& lhs, const Array<T, M>& rhs)
    {
        static_assert(N + M > 0, "Adding two arrays of size 0 is not allowed");
        Array<T, N + M> combinedArray;
        for (std::size_t i = 0; i < N; ++i)
        {
            combinedArray[i] = lhs[i];
        }
        for (std::size_t i = 0; i < M; ++i)
        {
            combinedArray[i + N] = rhs[i];
        }

        return combinedArray;
    }

}

#endif