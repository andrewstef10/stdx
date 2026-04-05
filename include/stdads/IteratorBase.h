#ifndef ITERATORBASE_H
#define ITERATORBASE_H

#include <cstddef>
#include <iterator>

namespace stdads {

    template<
        typename Derived,
        typename T,
        typename Category,
        typename Difference = std::ptrdiff_t,
        typename Pointer = T*,
        typename Reference = T&
    >
    class IteratorBase {
    public:
        // ===== Iterator Traits =====
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Difference;
        using pointer           = Pointer;
        using reference         = Reference;

        // ===== Core Operators =====
        reference operator*() const { return GetDerived().Dereference(); }
        pointer operator->() const { return &GetDerived().Dereference(); }

        // ===== Increment / Decrement =====
        Derived& operator++()        // pre-increment
        {
            GetDerived().Increment();
            return GetDerived();
        }

        Derived operator++(int)     // post-increment
        {
            Derived temp = GetDerived();
            GetDerived().Increment();
            return temp;
        }

        Derived& operator--()        // pre-decrement
        {
            GetDerived().Decrement();
            return GetDerived();
        }

        Derived operator--(int)     // post-decrement
        {
            Derived temp = GetDerived();
            GetDerived().Decrement();
            return temp;
        }

        // ===== Arithmetic =====
        Derived& operator+=(difference_type n)
        {
            GetDerived().Advance(n);
            return GetDerived();
        }

        Derived& operator-=(difference_type n)
        {
            GetDerived().Advance(-n);
            return GetDerived();
        }

        Derived operator+(difference_type n) const
        {
            Derived temp = GetDerived();
            temp.Advance(n);
            return temp;
        }

        Derived operator-(difference_type n) const
        {
            Derived temp = GetDerived();
            temp.Advance(-n);
            return temp;
        }

        friend Derived operator+(difference_type n, const Derived& it) { return it + n; }

        difference_type operator-(const Derived& other) const { return GetDerived().DistanceTo(other); }

        // ===== Element Access =====
        reference operator[](difference_type n) const { return *(*this + n); }

        // ===== Comparisons =====
        bool operator==(const Derived& other) const { return GetDerived().Equals(other); }
        bool operator!=(const Derived& other) const { return !GetDerived().Equals(other); }
        bool operator<(const Derived& other) const { return GetDerived().LessThan(other); }
        bool operator>(const Derived& other) const { return other < GetDerived(); }
        bool operator<=(const Derived& other) const { return !(GetDerived() > other); }
        bool operator>=(const Derived& other) const { return !(GetDerived() < other); }

        template <typename OtherDerived>
        bool operator==(const OtherDerived& other) const { return GetDerived().Equals(other); }
        template <typename OtherDerived>
        bool operator!=(const OtherDerived& other) const { return !GetDerived().Equals(other); }
        template <typename OtherDerived>
        bool operator<(const OtherDerived& other) const { return GetDerived().LessThan(other); }
        template <typename OtherDerived>
        bool operator>(const OtherDerived& other) const { return other < GetDerived(); }
        template <typename OtherDerived>
        bool operator<=(const OtherDerived& other) const { return !(GetDerived() > other); }
        template <typename OtherDerived>
        bool operator>=(const OtherDerived& other) const { return !(GetDerived() < other); }

    protected:
        // Constructors for Derived classes only
        IteratorBase() = default;
        IteratorBase(const IteratorBase&) = default;
        //IteratorBase(const IteratorBase&&) = default;
        ~IteratorBase() = default;
        IteratorBase& operator=(const IteratorBase&) = default;
        //IteratorBase& operator=(const IteratorBase&&) = default;

    private:
        // Helper to cast to derived
        Derived&       GetDerived() { return static_cast<Derived&>(*this); }
        const Derived& GetDerived() const { return static_cast<const Derived&>(*this); }
    };


    /**
     * @brief Reverse Iterator class
     * 
     * Reverses an Iterator
     */
    template <typename Iterator>
    class ReverseIterator {
    public:
        // ===== Iterator Traits =====
        using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
        using value_type        = typename std::iterator_traits<Iterator>::value_type;
        using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
        using pointer           = typename std::iterator_traits<Iterator>::pointer;
        using reference         = typename std::iterator_traits<Iterator>::reference;

        // ===== Constructors =====
        ReverseIterator() : base_() {}
        ReverseIterator(Iterator it) : base_(it) {}
        ReverseIterator(const ReverseIterator&) = default;
        //ReverseIterator(const ReverseIterator&&) = default;
        ~ReverseIterator() = default;
        ReverseIterator& operator=(const ReverseIterator&) = default;
        //ReverseIterator& operator=(const ReverseIterator&&) = default;

        template <typename OtherIterator,
                  typename = std::enable_if_t<std::is_convertible_v<typename OtherIterator::pointer, pointer>>>
        ReverseIterator(const ReverseIterator<OtherIterator>& other) : base_(other.Base()) {} // Const conversion constructor

        // ===== Get base Iterator =====
        Iterator Base() const { return base_; }

        // ===== Core Operators =====
        reference operator*() const 
        {
            Iterator tmp = base_;
            return *--tmp;
        }

        pointer operator->() const
        {
            Iterator tmp = base_;
            return &(*--tmp);
        }

        // ===== Increment / Decrement =====
        ReverseIterator& operator++()       // pre-increment
        {
            --base_;
            return *this;
        }

        ReverseIterator operator++(int)     // post-increment
        { 
            ReverseIterator tmp = *this;
            --base_;
            return tmp;
        }

        ReverseIterator& operator--()       // pre-decrement
        { 
            ++base_;
            return *this;
        }

        ReverseIterator operator--(int)     // post-decrement
        { 
            ReverseIterator tmp = *this;
            ++base_;
            return tmp;
        }

        // ===== Arithmetic =====
        ReverseIterator& operator+=(difference_type n)
        { 
            base_ -= n;
            return *this;
        }

        ReverseIterator& operator-=(difference_type n)
        { 
            base_ += n;
            return *this;
        }

        ReverseIterator operator+(difference_type n) const { return ReverseIterator(base_ - n); }
        ReverseIterator operator-(difference_type n) const { return ReverseIterator(base_ + n); }
        friend ReverseIterator operator+(difference_type n, const ReverseIterator& it) { return it + n; }

        difference_type operator-(const ReverseIterator& other) const { return other.base_ - base_; }

        // ===== Element Access =====
        reference operator[](difference_type n) const { return *(*this + n); }

        // ===== Comparisons =====
        bool operator==(const ReverseIterator& other) const { return base_ == other.base_; }
        bool operator!=(const ReverseIterator& other) const { return base_ != other.base_; }
        bool operator<(const ReverseIterator& other) const { return other.base_ < base_; }
        bool operator>(const ReverseIterator& other) const { return other.base_ > base_ ; }
        bool operator<=(const ReverseIterator& other) const { return other.base_ <= base_; }
        bool operator>=(const ReverseIterator& other) const { return other.base_ >= base_; }

        template <typename OtherIterator>
        bool operator==(const ReverseIterator<OtherIterator>& other) const { return base_ == other.Base(); }
        template <typename OtherIterator>
        bool operator!=(const ReverseIterator<OtherIterator>& other) const { return base_ != other.Base(); }
        template <typename OtherIterator>
        bool operator<(const ReverseIterator<OtherIterator>& other) const { return other.Base() < base_; }
        template <typename OtherIterator>
        bool operator>(const ReverseIterator<OtherIterator>& other) const { return other.Base() > base_; }
        template <typename OtherIterator>
        bool operator<=(const ReverseIterator<OtherIterator>& other) const { return other.Base() <= base_; }
        template <typename OtherIterator>
        bool operator>=(const ReverseIterator<OtherIterator>& other) const { return other.Base() >= base_; }

    private:
        Iterator base_;
    };


    /**
     * @brief Pointer Iterator class
     */
    template <typename T>
    class PtrIterator : public IteratorBase<PtrIterator<T>, T, std::random_access_iterator_tag> {
    public:
        using difference_type = typename IteratorBase<PtrIterator<T>, T, std::random_access_iterator_tag>::difference_type;

        // Friend declaration for all PtrIterators of different type T
        template <typename> friend class PtrIterator;

        // ===== Constructors =====
        PtrIterator() : ptr_(0) {}
        PtrIterator(T* ptr) : ptr_(ptr) {}
        PtrIterator(const PtrIterator&) = default;
        //PtrIterator(const PtrIterator&&) = default;
        ~PtrIterator() = default;
        PtrIterator& operator=(const PtrIterator&) = default;
        //PtrIterator& operator=(const PtrIterator&&) = default;

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        PtrIterator(const PtrIterator<U>& other) : ptr_(other.ptr_) {} // Const conversion constructor

        // ===== Required by IteratorBase =====
        T& Dereference() const { return *ptr_; }

        void Increment() { ++ptr_; }
        void Decrement() { --ptr_; }

        void Advance(difference_type n) { ptr_ += n; }
        difference_type DistanceTo(const PtrIterator& other) const { return ptr_ - other.ptr_; }

        bool Equals(const PtrIterator& other) const { return ptr_ == other.ptr_; }
        bool LessThan(const PtrIterator& other) const { return ptr_ < other.ptr_; }

        template <typename U>
        bool Equals(const PtrIterator<U>& other) const { return ptr_ == other.ptr_; }
        template <typename U>
        bool LessThan(const PtrIterator<U>& other) const { return ptr_ < other.ptr_; }

    private:
        T* ptr_;
    };

}

#endif