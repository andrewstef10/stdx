#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <stdexcept>

#include <stdads/IteratorBase.h>
#include <stdads/String.h>

namespace stdads {

    /**
     * @brief Fixed size contiguous array class.
     */
    template<typename T, std::size_t N>
    class Array {
    public:
        using Iterator = stdads::PtrIterator<T>;
        using ConstIterator = stdads::PtrIterator<const T>;
        using ReverseIterator = stdads::ReverseIterator<Iterator>;
        using ConstReverseIterator = stdads::ReverseIterator<ConstIterator>;

        /**
         * @brief Implicitly defined default constructor.
         *
         * Creates an Array with the default compiler generated constructor for arrays.
         * If T is a class type, each element's default constructor will be called.
         * If T is a primitive type, the array will be initalized with garbage data.
         */
        Array() = default;

        /**
         * @brief Copy constructor.
         *
         * Creates a copy of another array.
         */
        Array(const Array& other) = default;

        /**
         * @brief Aggregate initialization constructor.
         *
         * Creates an array with {T1, T2 ...} syntax
         */
        Array(std::initializer_list<T> init);

        /**
         * @brief Implicitly defined destructor.
         */
        ~Array() = default;

        /**
         * @brief Implicitly defined assignment operator.
         * 
         * Overwrites every element of the array with the corresponding element of another array.
         *
         * @param other Array to assign this to.
         * @return Reference to this object.
         */
        Array& operator=(const Array& other) = default;


        // ===== Element Access =====

        /**
         * @brief Returns a reference to the element at specified location pos, with bounds checking.
         * 
         * If pos is not within the range of the container, an exception of type std::out_of_range is thrown.
         *
         * @param pos Position of the element to return.
         * @return Reference to the requested element.
         * @exception std::out_of_range if pos is out of range (pos >= Size()).
         */
        T& At(std::size_t pos);
        const T& At(std::size_t pos) const;

        /**
         * @brief Returns a reference to the element at specified location pos, without bounds checking.
         *
         * @param pos Position of the element to return.
         * @return Reference to the requested element.
         */
        T& operator[](std::size_t index) { return data_[index]; }
        const T& operator[](std::size_t index) const { return data_[index]; }

        /**
         * @brief Returns a pointer to the underlying array serving as element storage.
         * 
         * The pointer is such that range [data(), data() + size()) is always a valid range.
         * If *this is empty, data() is not dereferenceable.
         * 
         * @return Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
         */
        T* Data() { return data_; }
        const T* Data() const { return data_; }


        // ==== Iterators ====

        /**
         * @brief Returns an iterator to the first element of *this.
         * 
         * If *this is empty, the returned iterator will be equal to end().
         * 
         * @return Iterator to the first element.
         */
        Iterator Begin() { return Iterator(data_); }
        ConstIterator Begin() const { return ConstIterator(data_); }
        ConstIterator CBegin() const { return ConstIterator(data_); }

        /**
         * @brief Returns an iterator past the last element of *this.
         * 
         * This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
         * 
         * @return Iterator past the last element.
         */
        Iterator End() { return Iterator(data_ + N); }
        ConstIterator End() const { return ConstIterator(data_ + N); }
        ConstIterator CEnd() const { return ConstIterator(data_ + N); }

        /**
         * @brief Returns a reverse iterator to the first element of the reversed *this. It corresponds to the last element of the non-reversed *this.
         * 
         * If *this is empty, the returned iterator will be equal to end().
         * 
         * @return Reverse iterator to the first element.
         */
        ReverseIterator RBegin() { return ReverseIterator(data_ + N); }
        ConstReverseIterator RBegin() const { return ConstReverseIterator(data_ + N); }
        ConstReverseIterator CRBegin() const { return ConstReverseIterator(data_ + N); }

         /**
         * @brief Returns a reverse iterator past the last element of the reversed *this. It corresponds to the element preceding the first element of the non-reversed *this.
         * 
         * This returned iterator only acts as a sentinel. It is not guaranteed to be dereferenceable.
         * 
         * @return Reverse iterator to the element following the last element.
         */
        ReverseIterator REnd() { return ReverseIterator(data_); }
        ConstReverseIterator REnd() const { return ConstReverseIterator(data_); }
        ConstReverseIterator CREnd() const { return ConstReverseIterator(data_); }


        // ===== Capacity =====

        /**
         * @brief Checks if the container has no elements.
         *
         * @return true if the container is empty, false otherwise.
         */
        bool Empty() const { return Size() == 0; }

        /**
         * @brief Returns the number of elements in the container.
         *
         * @return The number of elements in the container.
         */
        std::size_t Size() const { return N; }

    private:
        T data_[N];
    };


    // ===== Inline Array Implementation =====

    template<typename T, std::size_t N>
    inline Array<T, N>::Array(std::initializer_list<T> init)
    {
        if (init.size() > N)
        {
            throw std::out_of_range("Too many initializers");
        }

        // Copy provided values
        size_t i = 0;
        for (const T& value : init)
        {
            data_[i] = value;
            ++i;
        }
        
        // Default init the rest
        for (; i < N; ++i)
        {
            data_[i] = T();
        }
    }

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

}

#endif