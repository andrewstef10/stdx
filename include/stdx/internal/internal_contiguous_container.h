#ifndef INTERNAL_CONTIGUOUS_CONTAINER_H
#define INTERNAL_CONTIGUOUS_CONTAINER_H

#include <cstddef>
#include <stdexcept>
#include <stdx/memory.h>
#include <utility>

namespace stdx { namespace internal { namespace contiguous_container {

/// @brief Returns a reference to the element at specified location `index`, with bounds checking.
/// @details If index is not within the range of the container, an exception of type std::out_of_range is thrown.
///          Time:  O(1)
///          Space: O(1)
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param container The contiguous container storing elements to index
/// @param index Position of the element to return.
/// @return Reference to the requested element.
/// @exception std::out_of_range if `index` is out of range (index >= size()).
template <typename Container>
auto at(Container& container, typename Container::size_type index) -> decltype(container[index]) {
    if (index >= container.size()) {
        throw std::out_of_range("Index is outside the bounds of the container");
    }
    return container[index];
}

/// @brief Compares two contiguous containers element-by-element using operator==, stopping at the first mismatch.
/// @details Time:  O(n)
///          Space: O(1)
///          Requires Container::value_type to be equality-comparable (operator== defined).
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param lhs lhs contiguous container to compare
/// @param rhs rhs contiguous container to compare
/// @return True if the elements of both containers compare equal.
template <typename Container>
bool equals(const Container& lhs, const Container& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (typename Container::size_type i = 0; i < lhs.size(); ++i) {
        if (!(lhs[i] == rhs[i])) {
            return false;
        }
    }
    return true;
}

/// @brief Performs a lexicographic less-than comparison using operator<.
/// @details Time:  O(n)
///          Space: O(1)
///          Requires Container::value_type to be less-than-comparable (operator< defined).
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param lhs Reference to the lhs contiguous container to compare
/// @param rhs Reference to the rhs contiguous container to compare
/// @return True if `lhs` is lexicographically less than `rhs`
template <typename Container>
bool less_than(const Container& lhs, const Container& rhs) {
    for (typename Container::size_type i = 0; i < lhs.size() && i < rhs.size(); ++i) {
        if (lhs[i] < rhs[i]) {
            return true;
        }
        if (rhs[i] < lhs[i]) {
            return false;
        }
    }
    return lhs.size() < rhs.size();
}

/// @brief Opens a one-slot gap at `index` by moving the elements [index, container.size()) up by one.
/// @details Time:  O(n), where n = container.size() - index
///          Space: O(1)
///          Precondition: index < container.size(), and the container holds raw storage for at least container.size()
///          + 1 elements. The slot container[container.size()] is raw storage, so the last element is move-constructed
///          into it through the allocator.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param alloc  Allocator used to construct elements.
/// @param Container The contiguous container storing elements to shift
/// @param index  Position of the gap to open.
template <typename Allocator, typename Container>
void shift_right(Allocator& alloc, Container& container, typename Container::size_type index) {
    using size_type = typename Container::size_type; // typically std::size_t
    const size_type LAST = container.size() - 1;

    // Move construct the last element in the container 1 past the end of the current buffer
    std::allocator_traits<Allocator>::construct(alloc, container.data() + container.size(), std::move(container[LAST]));

    // Shift the rest of the elements between `index` and the end of the container
    for (size_type i = LAST; i > index; --i) {
        container[i] = std::move(container[i - 1]);
    }
}

/// @brief Closes the gap [first, last) by moving the elements [last, container.size()) down onto it.
/// @details Time:  O(n), where n = container.size() - last
///          Space: O(1)
///          Precondition: first <= last <= container.size().
///          The elements [last, container.size()) are move-assigned into [first, first + container.size() - last). The
///          tail behind the returned count is left holding live, moved-from objects that the caller remains responsible
///          for destroying. Does nothing if the range is empty.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param container The contiguous container storing elements to shift
/// @param first Position of the first element of the gap to close.
/// @param last  Position one past the last element of the gap to close.
/// @return The container's new logical element count (the position where the moved-from tail begins).
template <typename Container>
std::size_t shift_left(Container& container, typename Container::size_type first, typename Container::size_type last) {
    using size_type = typename Container::size_type; // typically std::size_t

    if (first == last) {
        return container.size();
    }

    size_type left = first;
    for (size_type right = last; right < container.size(); ++left, ++right) {
        container[left] = std::move(container[right]);
    }
    return left;
}

/// @brief Resizes the container to contain count elements.
/// @details Time:  O(n), where n is the difference between container.size() and count
///          Space: O(1)
///          Precondition: the buffer holds raw storage for at least count elements.
///          If container.size() is equal to count, this function does nothing.
///          If container.size() is greater than count, the buffer is reduced to its first count elements.
///          If container.size() is less than count, additional elements constructed from `args` are appended.
///          The caller remains responsible for tracking the new element count.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param alloc  Allocator used to construct appended elements or destroy surplus elements.
/// @param container The contiguous container storing elements to resize
/// @param count  Number of desired elements in the buffer.
/// @param args   Arguments forwarded to Container::value_type's constructor if additional Container::value_type
///               constructions are needed.
template <typename Allocator, typename Container, typename... Args>
void resize(Allocator& alloc, Container& container, typename Container::size_type count, Args&&... args) {
    using size_type = typename Container::size_type; // typically std::size_t
    if (container.size() < count) {
        for (size_type i = container.size(); i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc, container.data() + i, std::forward<Args>(args)...);
        }
    } else {
        for (size_type i = container.size(); i > count; --i) {
            std::allocator_traits<Allocator>::destroy(alloc, container.data() + (i - 1));
        }
    }
}

/// @brief Constructs an element at `index`, opening a one-slot gap in [index, container.size()) if needed.
/// @details Time:  O(n), where n = container.size() - index
///          Space: O(1)
///          Precondition: index <= container.size(), and the buffer holds raw storage for at least container.size() + 1
///          elements. For a middle insert the value is built into a temporary before shifting, so it remains correct
///          even when `args` alias an element already in the buffer. For index == container.size() the destination is
///          raw storage and the element is constructed directly in place. The caller remains responsible for tracking
///          the new element count.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param alloc  Allocator used to construct appended elements.
/// @param container The contiguous container to emplace into.
/// @param index  Position at which the new element is constructed (0..container.size()).
/// @param args   Arguments forwarded to Container::value_type's constructor.
/// @return Pointer to the newly constructed element.
template <typename Allocator, typename Container, typename... Args>
typename Container::value_type* emplace_at(Allocator& alloc, Container& container, std::size_t index, Args&&... args) {
    if (index < container.size()) {
        // Middle: build the value first so it stays correct even when args alias an existing element,
        // open a one-slot gap, then move the value into the now-live slot.
        typename Container::value_type value(std::forward<Args>(args)...);
        shift_right(alloc, container, index);
        container[index] = std::move(value);
    } else {
        // End: the destination is raw storage, so construct the element directly in place.
        std::allocator_traits<Allocator>::construct(alloc, container.data() + index, std::forward<Args>(args)...);
    }
    return container.data() + index;
}

/// @brief Copy-constructs the elements of `src` into the raw storage at `dest`, in order.
/// @details Time:  O(n), where n = src.size
///          Space: O(1)
///          Precondition: dest is raw storage for at least src.size elements and does not overlap src.
///          The caller remains responsible for tracking the new element count.
/// @tparam Allocator std::allocator class or similar.
/// @tparam T Element type being constructed.
/// @tparam ForwardIt A forward iterator.
/// @param alloc Allocator used to construct appended elements.
/// @param dest  Pointer to the raw storage receiving the copies.
/// @param first Forward iterator pointing to the first element to copy construct to dest.
/// @param count Number of elements to copy starting with `first`
template <typename Allocator, typename T, typename ForwardIt>
void construct_copy(Allocator& alloc, T* dest, ForwardIt first, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i, ++first) {
        std::allocator_traits<Allocator>::construct(alloc, dest + i, *first);
    }
}

/// @brief Replaces the live elements of `dest` with `count` copies of `value`.
/// @details Time:  O(n), where n = max(dest.size, count)
///          Space: O(1)
///          Precondition: dest holds dest.size live elements backed by raw storage for at least count elements, and
///          `value` is not an element of dest.
///          The caller remains responsible for tracking the new element count.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param alloc Allocator used to construct appended elements or destroy surplus elements.
/// @param dest  The contiguous container to emplace into assign.
/// @param count Number of copies of `value` the container holds afterwards.
/// @param value Value to fill the container with.
template <typename Allocator, typename Container>
void assign_copy(Allocator& alloc, Container& dest, typename Container::size_type count,
                 const typename Container::value_type& value) {
    if (dest.size() < count) {
        // Copy assign over the live elements
        typename Container::size_type i = 0;
        for (; i < dest.size(); ++i) {
            dest[i] = value;
        }

        // Copy construct the remaining elements into raw storage
        for (; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc, dest.data() + i, value);
        }
    } else {
        // Copy assign over the first count live elements
        typename Container::size_type i = 0;
        for (; i < count; ++i) {
            dest[i] = value;
        }

        // Destroy the surplus elements
        for (; i < dest.size(); ++i) {
            std::allocator_traits<Allocator>::destroy(alloc, dest.data() + i);
        }
    }
}

/// @brief Replaces the live elements of `dest` with copies of the elements of the container defined by the forward
///        iterator `first` and `count` number of elements past `first`.
/// @details Time:  O(n), where n = max(dest.size, src.size)
///          Space: O(1)
///          Precondition: dest holds dest.size live elements backed by raw storage for at least src.size elements, and
///          does not overlap src. The caller remains responsible for tracking the new element count.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @tparam ForwardIt A forward iterator.
/// @param alloc Allocator used to construct appended elements or destroy surplus elements.
/// @param dest  Container of the live destination elements.
/// @param first Forward iterator pointing to the first element to copy assign to dest.
/// @param count Number of elements to copy starting with `first`
template <typename Allocator, typename Container, typename ForwardIt>
void assign_copy(Allocator& alloc, Container& dest, ForwardIt first, typename Container::size_type count) {
    typename Container::size_type i = 0;
    ForwardIt src = first;
    if (dest.size() < count) { // if dest contains less elements than src
        // Copy assign all elements from src up to dest's live count
        for (; i < dest.size(); ++i, ++src) {
            dest[i] = *src;
        }

        // Copy construct the remaining elements from src
        for (; i < count; ++i, ++src) {
            std::allocator_traits<Allocator>::construct(alloc, dest.data() + i, *src);
        }
    } else {
        // Copy assign all elements from src
        for (; i < count; ++i, ++src) {
            dest[i] = *src;
        }

        // Destroy the surplus elements in dest
        for (; i < dest.size(); ++i) {
            std::allocator_traits<Allocator>::destroy(alloc, dest.data() + i);
        }
    }
}

/// @brief Move-constructs the elements of `src` into the raw storage at `dest`, destroying each source element as it is
///        consumed.
/// @details Time:  O(n), where n = src.size
///          Space: O(1)
///          Precondition: dest is raw storage for at least src.size elements and does not overlap src.
///          After the call src holds no live elements. The caller remains responsible for tracking both
///          containers' element counts.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param destAlloc Allocator used to construct appended elements to `dest`.
/// @param dest      Pointer to the raw storage receiving the moved elements.
/// @param srcAlloc  Allocator used to destroy surplus elements from `src`.
/// @param src       Container of the elements to move from.
template <typename DestAllocator, typename SrcAllocator, typename Container>
void construct_move(DestAllocator& destAlloc, typename Container::value_type* dest, SrcAllocator& srcAlloc,
                    Container& src) {
    for (std::size_t i = 0; i < src.size(); ++i) {
        std::allocator_traits<DestAllocator>::construct(destAlloc, dest + i, std::move(src[i]));
        std::allocator_traits<SrcAllocator>::destroy(srcAlloc, src.data() + i); // Destroy src's moved-from elements
    }
}

/// @brief Replaces the live elements of `dest` by moving the elements of `src` into them, destroying each
///        source element as it is consumed.
/// @details Time:  O(n), where n = max(dest.size, src.size)
///          Space: O(1)
///          Precondition: dest holds dest.size live elements backed by raw storage for at least src.size elements, and
///          does not overlap src. After the call src holds no live elements. The caller remains responsible for
///          tracking both containers' element counts.
/// @tparam Allocator std::allocator class or similar.
/// @tparam Container Contiguous container with public data(), size() and operator[] functions, as well as size_type and
///         value_type type traits.
/// @param destAlloc Allocator used to construct appended elements to `dest`.
/// @param dest      Container of the live destination elements.
/// @param srcAlloc  Allocator used to destroy surplus elements from `src`.
/// @param src       Container of the elements to move from.
template <typename DestAllocator, typename SrcAllocator, typename Container>
void assign_move(DestAllocator& destAlloc, Container& dest, SrcAllocator& srcAlloc, Container& src) {
    if (dest.size() < src.size()) // if dest contains less elements than src
    {
        // Move assign all elements from src up to dest's live count
        std::size_t i = 0;
        for (; i < dest.size(); ++i) {
            dest[i] = std::move(src[i]);
            std::allocator_traits<SrcAllocator>::destroy(srcAlloc, src.data() + i); // Destroy src's moved-from elements
        }

        // Move construct the remaining elements from src
        for (; i < src.size(); ++i) {
            std::allocator_traits<DestAllocator>::construct(destAlloc, dest.data() + i, std::move(src[i]));
            std::allocator_traits<SrcAllocator>::destroy(srcAlloc, src.data() + i); // Destroy src's moved-from elements
        }
    } else {
        // Move assign all elements from src
        std::size_t i = 0;
        for (; i < src.size(); ++i) {
            dest[i] = std::move(src[i]);
            std::allocator_traits<SrcAllocator>::destroy(srcAlloc, src.data() + i); // Destroy src's moved-from elements
        }

        // Destroy the surplus elements in dest
        for (; i < dest.size(); ++i) {
            std::allocator_traits<DestAllocator>::destroy(destAlloc, dest.data() + i);
        }
    }
}
}}} // namespace stdx::internal::contiguous_container

#endif
