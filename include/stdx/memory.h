#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>
#include <memory>
#include <new> // for std::

#include <stdx/equatable.h>

namespace stdx {

    /// @brief Growth policy that returns the exact requested capacity.
    /// Use this policy when you want the array_list to grow to exactly the size needed,
    /// without any overallocation. This minimizes memory usage but may cause more
    /// frequent reallocations as the array_list grows.
    /// @tparam size_type The unsigned integer type used for capacity values.
    ///                   Defaults to std::size_t.
    template<typename size_type = std::size_t>
    struct exact_growth
    {
        /// @brief Returns the required capacity without any growth factor.
        /// @param current_capacity The current capacity (ignored by this policy).
        /// @param required_capacity The minimum capacity needed to accommodate new elements.
        /// @return The required_capacity unchanged, ensuring no overallocation.
        size_type operator()(size_type /*current_capacity*/, size_type required_capacity) const
        {
            return required_capacity;
        }
    };

    /// @brief Growth policy that doubles capacity each time it needs to grow
    /// This policy provides good performance characteristics by reducing frequency
    /// of reallocations while maintaining reasonable memory usage. It doubles the
    /// current capacity until it meets or exceeds the required size.
    /// @tparam size_type size_type The unsigned integer type used for capacity values.
    ///                   Defaults to std::size_t.
    template<typename size_type = std::size_t>
    struct doubling_growth
    {
        /// @brief Computes new capacity by doubling until it meets or exceeds required
        /// @param current_capacity The current allocated capacity
        /// @param required_capacity The minimum capacity needed for new elements
        /// @return size_type The new capacity which will always be >= required_capacity
        size_type operator()(size_type current_capacity, size_type required_capacity) const
        {
            if (current_capacity < required_capacity)
            {
                current_capacity = current_capacity == 0 ? 1 : current_capacity * 2;
                while (current_capacity < required_capacity)
                {
                    current_capacity *= 2;
                }
            }
            
            return current_capacity;
        }
    };

    template<typename T>
    class allocator
    {
    public:

        using value_type = T;

        using is_always_equal = std::true_type;

        using propagate_on_container_copy_assignment = std::false_type;
        using propagate_on_container_move_assignment = std::false_type;
        using propagate_on_container_swap = std::false_type;

        // No longer required c++11+
        // pointer
        // const_pointer
        // reference
        // const_reference
        // size_type
        // difference_type

        // construct()
        // destroy()

        // rebind
        // address()
        // max_size()


        /// @brief Default constructor
        allocator() = default;

        /// @brief Copy constructor
        /// @param other allocator to copy.
        allocator(const allocator& other) = default;

        /// @brief Move constructor
        /// @param other allocator to move.
        allocator(allocator&& other) = default;

        /// @brief Destructor.
        ~allocator() = default;

        /// @brief Copy assignment operator 
        /// @param other allocator to copy
        /// @return Reference to this object
        allocator& operator=(const allocator& other) = default;

        /// @brief Move assignment operator
        /// @param other allocator to move
        /// @return Reference to this object
        allocator& operator=(allocator&& other) = default;

        /// @brief Copy constructor
        /// @tparam U Other allocator's type
        /// @param  
        template<typename U>
        allocator(const allocator<U>&) noexcept {}

        /// @brief Allocates n * sizeof(T) bytes of uninitialized storage by calling ::operator new(std::size_t) or ::operator new(std::size_t, std::align_val_t)(since C++17), but it is unspecified when and how this function is called.
        /// Then, this function creates an array of type T[n] in the storage and starts its lifetime, but does not start lifetime of any of its elements.
        /// Use of this function is ill-formed if T is an incomplete type.
        /// @param n The number of objects to allocate storage for
        /// @return Pointer to the first element of an array of n objects of type T whose elements have not been constructed yet.
        T* allocate(std::size_t n);

        /// @brief Deallocates the storage referenced by the pointer p, which must be a pointer obtained by an earlier call to allocate() or allocate_at_least().
        /// Calls ::operator delete(void*) or ::operator delete(void*, std::align_val_t)(since C++17), but it is unspecified when and how it is called.
        /// @param ptr 
        /// @param n 
        void deallocate(T* ptr, std::size_t n) noexcept;

        /// @brief Returns the maximum theoretically possible value of n, for which the call allocate(n) could succeed.
        /// @return The maximum supported allocation size.
        constexpr std::size_t max_size() const noexcept;

        /// @brief Compares two allocators
        /// @tparam U Other allocator's type
        /// @param other Other allocator
        /// @return true always because allocators of any type T are equal
        template<typename U>
        bool constexpr equals(const allocator<U>& other) const noexcept { return true; }

        /// @brief Equality operator
        /// @tparam U Other allocator's type
        /// @param lhs left hand side
        /// @param rhs right had side
        /// @return true always because allocators of any type T are equal
        template<typename U>
        friend constexpr bool operator==(const allocator<T>& lhs, const allocator<U>& rhs) noexcept { return lhs.equals(rhs); }

        /// @brief Inequality operator 
        /// @tparam U Other allocator's type
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return true always because allocators of any type T are equal
        template<typename U>
        friend constexpr bool operator!=(const allocator<T>& lhs, const allocator<U>& rhs) noexcept { return !lhs.equals(rhs); }
    };


    // ===== Inline allocator Implementation =====

    template<typename T>
    inline T* allocator<T>::allocate(std::size_t n)
    {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<typename T>
    inline void allocator<T>::deallocate(T* ptr, std::size_t n) noexcept
    {
    // #if __cpp_sized_deallocation
    //         ::operator delete(ptr, n * sizeof(T));
    // #else
    //         ::operator delete(ptr);
    // #endif
        ::operator delete(ptr);
    }

    template<typename T>
    inline constexpr std::size_t allocator<T>::max_size() const noexcept
    {
        return static_cast<std::size_t>(-1) / sizeof(T);
    }

}

#endif