#ifndef ITERATORBASE_H
#define ITERATORBASE_H

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace stdads {

    // Forward declarations
    template<typename Derived, typename T, typename Difference, typename Pointer, typename Reference, typename Category>
    class ForwardIterator;

    template<typename Derived, typename T, typename Difference, typename Pointer, typename Reference, typename Category>
    class BidirectionalIterator;

    template<typename Derived, typename T, typename Difference, typename Pointer, typename Reference, typename Category>
    class RandomAccessIterator;


    /// @brief Iterator base class selector tags
    ///
    /// Struct that provides the correct iterator base class to implement based on the provided iterator category.
    ///
    /// @tparam Category Iterator category
    /// @tparam Derived CRTP Derived class
    /// @tparam T Iterator value type
    /// @tparam Difference Iterator difference type
    /// @tparam Pointer Iterator pointer type
    /// @tparam Reference Iterator reference
    template<typename Category, typename Derived, typename T,
        typename Difference = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&>
    struct IteratorBaseSelector;

    template<typename Derived, typename T, typename Difference, typename Pointer, typename Reference>
    struct IteratorBaseSelector<std::forward_iterator_tag, Derived, T, Difference, Pointer, Reference> {
        using type = ForwardIterator<Derived, T, Difference, Pointer, Reference, std::forward_iterator_tag>;
    };

    template<typename Derived, typename T, typename Difference, typename Pointer, typename Reference>
    struct IteratorBaseSelector<std::bidirectional_iterator_tag, Derived, T, Difference, Pointer, Reference> {
        using type = BidirectionalIterator<Derived, T, Difference, Pointer, Reference, std::bidirectional_iterator_tag>;
    };

    template<typename Derived, typename T, typename Difference, typename Pointer, typename Reference>
    struct IteratorBaseSelector<std::random_access_iterator_tag, Derived, T, Difference, Pointer, Reference> {
        using type = RandomAccessIterator<Derived, T, Difference, Pointer, Reference, std::random_access_iterator_tag>;
    };


    /// @brief Base iterator class to define iterator traits.
    /// @tparam Category The type of iterator
    /// @tparam T The iterator's underlying value type
    /// @tparam Difference The type to define the difference between two iterators
    /// @tparam Pointer Pointer to the iterator's value type
    /// @tparam Reference Reference to the iterators value type
    template<typename Category, typename T,
        typename Difference = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&>
    struct Iterator {
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Difference;
        using pointer           = Pointer;
        using reference         = Reference;
    };


    /// @brief Base CRTP forward iterator class.
    ///
    /// This class implements all forward iterator requirements, and requires that Derived to implement the following:
    ///     - void Increment()
    ///     - bool Equals(const MyIterator& other) const
    ///     - Reference Dereference() const
    ///
    /// @tparam Derived CRTP Derived class
    /// @tparam T The iterator's underlying value type
    /// @tparam Difference The type to define the difference between two iterators
    /// @tparam Pointer Pointer to the iterator's value type
    /// @tparam Reference Reference to the iterators value type
    /// @tparam Category Category The type of iterator
    template<typename Derived, typename T,
        typename Difference = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&, typename Category = std::forward_iterator_tag>
    class ForwardIterator : public Iterator<Category, T, Difference, Pointer, Reference> {
    public:

        /// @brief Pre-increment operator
        /// @return A reference to this iterator after the increment
        Derived& operator++()
        {
            this->GetDerived().Increment();
            return this->GetDerived();
        }

        /// @brief Post-increment operator
        /// @param  
        /// @return Copy of this iterator before the increment
        Derived operator++(int)
        {
            Derived temp = this->GetDerived();
            this->GetDerived().Increment();
            return temp;
        }

        /// @brief Equality operator
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is equal to rhs, false otherwise.
        friend bool operator==(const Derived& lhs, const Derived& rhs) { return lhs.Equals(rhs); }

        /// @brief Inequality operator 
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is not equal to rhs, false otherwise.
        friend bool operator!=(const Derived& lhs, const Derived& rhs) { return !lhs.Equals(rhs); }

        /// @brief Dereference operator
        /// @return A reference to this iterator object
        Reference operator*() const { return this->GetDerived().Dereference(); }

        /// @brief Class member access (arrow) operator
        /// @return A pointer to this iterator object
        Pointer operator->() const { return &(this->GetDerived().Dereference()); }

    protected:
        // ==== ForwardIterator should not be constructed directly ====
        ForwardIterator() = default;
        ForwardIterator(const ForwardIterator&) = default;
        ForwardIterator(ForwardIterator&&) = default;
        ~ForwardIterator() = default;
        ForwardIterator& operator=(const ForwardIterator&) = default;
        ForwardIterator& operator=(ForwardIterator&&) = default;

        /// @brief Helper function to cast this to a Derived reference
        /// @return Derived reference
        Derived&       GetDerived()       { return static_cast<Derived&>(*this); }
        const Derived& GetDerived() const { return static_cast<const Derived&>(*this); }
    };


    /// @brief Base CRTP bidirectional iterator class.
    ///
    /// This class implements all bidirectional iterator requirements, and requires that Derived to implement the following:
    ///     - void Increment()
    ///     - bool Equals(const MyIterator& other) const
    ///     - void Decrement()
    ///
    /// @tparam Derived CRTP Derived class
    /// @tparam T The iterator's underlying value type
    /// @tparam Difference The type to define the difference between two iterators
    /// @tparam Pointer Pointer to the iterator's value type
    /// @tparam Reference Reference to the iterators value type
    /// @tparam Category Category The type of iterator
    template<typename Derived, typename T,
        typename Difference = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&, typename Category = std::bidirectional_iterator_tag>
    class BidirectionalIterator : public ForwardIterator<Derived, T, Difference, Pointer, Reference, Category> {
    public:

        /// @brief Pre-decrement operator
        /// @return A reference to this iterator after the decrement
        Derived& operator--()
        {
            this->GetDerived().Decrement();
            return this->GetDerived();
        }

        /// @brief Post-decrement operator
        /// @param  
        /// @return Copy of this iterator before the decrement
        Derived operator--(int)
        {
            Derived temp = this->GetDerived();
            this->GetDerived().Decrement();
            return temp;
        }

    protected:
        // ==== BidirectionalIterator should not be constructed directly ====
        BidirectionalIterator() = default;
        BidirectionalIterator(const BidirectionalIterator&) = default;
        BidirectionalIterator(BidirectionalIterator&&) = default;
        ~BidirectionalIterator() = default;
        BidirectionalIterator& operator=(const BidirectionalIterator&) = default;
        BidirectionalIterator& operator=(BidirectionalIterator&&) = default;
    };


    /// @brief Base CRTP random access iterator class.
    ///
    /// This class implements all random access iterator requirements, and requires that Derived to implement the following:
    ///     - void Increment()
    ///     - bool Equals(const MyIterator& other) const
    ///     - void Decrement()
    ///     - void Advance(Difference n)
    ///     - Difference DistanceTo(const MyIterator& other) const
    ///     - bool LessThan(const MyIterator& other) const
    ///
    /// @tparam Derived CRTP Derived class
    /// @tparam T The iterator's underlying value type
    /// @tparam Difference The type to define the difference between two iterators
    /// @tparam Pointer Pointer to the iterator's value type
    /// @tparam Reference Reference to the iterators value type
    /// @tparam Category Category The type of iterator
    template<typename Derived, typename T,
        typename Difference = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&, typename Category = std::random_access_iterator_tag>
    class RandomAccessIterator : public BidirectionalIterator<Derived, T, Difference, Pointer, Reference, Category> {
    public:

        /// @brief Addition assignment operator. Advances this iterator by n.
        /// @param n Amount to advance this iterator
        /// @return A reference to this iterator
        Derived& operator+=(Difference n)
        {
            this->GetDerived().Advance(n);
            return this->GetDerived();
        }

        /// @brief Subtraction assignment operator. Retreats this iterator by n.
        /// @param n Amount to retreats this iterator
        /// @return A reference to this iterator
        Derived& operator-=(Difference n)
        {
            this->GetDerived().Advance(-n);
            return this->GetDerived();
        }

        /// @brief Addition operator
        /// @param n Amount to add to this iterator
        /// @return Iterator advanced by n
        Derived operator+(Difference n) const
        {
            Derived temp = this->GetDerived();
            temp += n;
            return temp;
        }

        /// @brief Addition operator
        /// @param n Amount to add to this iterator
        /// @param it The iterator to add to
        /// @return Iterator advanced by n
        friend Derived operator+(Difference n, const Derived& it) { return it + n; }

        /// @brief Subtraction operator
        /// @param n Amount to subtract to this iterator
        /// @return An Iterator retreated by n
        Derived operator-(Difference n) const
        {
            Derived temp = this->GetDerived();
            temp -= n;
            return temp;
        }

        /// @brief Subtraction operator
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return The difference between two iterators
        friend Difference operator-(const Derived& lhs, const Derived& rhs) { return lhs.DistanceTo(rhs); }
        
        /// @brief Subscript operator
        /// @param n Number of elements to offset from the current iterator position
        /// @return A reference to the iterator at *this + n
        Reference operator[](Difference n) const { return *(this->GetDerived() + n); }

        /// @brief Less than operator
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is less than rhs, false otherwise.
        friend bool operator<(const Derived& lhs, const Derived& rhs) { return lhs.LessThan(rhs); }

        /// @brief Greater than operator
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is greater than rhs, false otherwise.
        friend bool operator>(const Derived& lhs, const Derived& rhs) { return rhs.LessThan(lhs); }

        /// @brief Less than or equal to operator
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is less than or equal to rhs, false otherwise.
        friend bool operator<=(const Derived& lhs, const Derived& rhs) { return !(lhs > rhs); }

        /// @brief Greater than or equal to operator
        /// @param lhs left hand side iterator
        /// @param rhs right had side iterator
        /// @return True if lhs is greater than or equal to rhs, false otherwise.
        friend bool operator>=(const Derived& lhs, const Derived& rhs) { return !(lhs < rhs); }

    protected:
        // ==== RandomAccessIterator should not be constructed directly ====
        RandomAccessIterator() = default;
        RandomAccessIterator(const RandomAccessIterator&) = default;
        RandomAccessIterator(RandomAccessIterator&&) = default;
        ~RandomAccessIterator() = default;
        RandomAccessIterator& operator=(const RandomAccessIterator&) = default;
        RandomAccessIterator& operator=(RandomAccessIterator&&) = default;
    };


    /// @brief Class to reverse an iterator
    /// @tparam Iterator The base iterator class to reverse
    template <typename Iterator>
    class ReverseIterator
        : public IteratorBaseSelector<typename std::iterator_traits<Iterator>::iterator_category, ReverseIterator<Iterator>
        , typename std::iterator_traits<Iterator>::value_type
        , typename std::iterator_traits<Iterator>::difference_type
        , typename std::iterator_traits<Iterator>::pointer
        , typename std::iterator_traits<Iterator>::reference
        >::type
    {
    public:
        // ===== Iterator Traits (full set defined in base class) =====
        using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
        using reference         = typename std::iterator_traits<Iterator>::reference;
        using pointer           = typename std::iterator_traits<Iterator>::pointer;

        // ===== Constructors =====
        ReverseIterator() : base_() {}
        ReverseIterator(Iterator it) : base_(it) {}
        ReverseIterator(const ReverseIterator&) = default;
        ReverseIterator(ReverseIterator&&) = default;
        ~ReverseIterator() = default;
        ReverseIterator& operator=(const ReverseIterator&) = default;
        ReverseIterator& operator=(ReverseIterator&&) = default;

        template <typename OtherIterator,
                  typename = std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<OtherIterator>::pointer, pointer>>>
        ReverseIterator(const ReverseIterator<OtherIterator>& other) : base_(other.Base()) {} // Const conversion constructor

        /// @brief Get the reverse iterator's base iterator
        /// @return Base iterator object
        Iterator Base() const { return base_; }

        // ==== Required by ForwardIterator ====
        void Increment() { --base_; }
        bool Equals(const ReverseIterator& other) const { return base_ == other.base_; }
        reference Dereference() const
        { 
            Iterator tmp = base_;
            return *--tmp;
        }

        // ==== Required by Bidirectional Iterator ====
        void Decrement() { ++base_; };

        // ==== Required by Random Access Iterator ====
        void Advance(difference_type n) { base_ -= n; };
        difference_type DistanceTo(const ReverseIterator& other) const { return other.base_ - base_; }
        bool LessThan(const ReverseIterator& other) const { return other.base_ < base_; }

    private:
        Iterator base_;
    };


    /**
     * @brief Pointer Iterator class
     */
    template <typename T>
    class PtrIterator : public RandomAccessIterator<PtrIterator<T>, T> {
    public:
        // ===== Constructors =====
        PtrIterator() : ptr_(0) {}
        PtrIterator(T* ptr) : ptr_(ptr) {}
        PtrIterator(const PtrIterator&) = default;
        PtrIterator(PtrIterator&&) = default;
        ~PtrIterator() = default;
        PtrIterator& operator=(const PtrIterator&) = default;
        PtrIterator& operator=(PtrIterator&&) = default;

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        PtrIterator(const PtrIterator<U>& other) : ptr_(&(*other)) {} // Const conversion constructor


        // ==== Required by ForwardIterator ====
        void Increment() { ++ptr_; }
        bool Equals(const PtrIterator& other) const { return ptr_ == other.ptr_; }
        T& Dereference() const { return *ptr_; }

        // ==== Required by Bidirectional Iterator ====
        void Decrement() { --ptr_; }

        // ==== Required by Random Access Iterator ====
        void Advance(std::ptrdiff_t n) { ptr_ += n; }
        std::ptrdiff_t DistanceTo(const PtrIterator& other) const { return ptr_ - other.ptr_; }
        bool LessThan(const PtrIterator& other) const { return ptr_ < other.ptr_; }

    private:
        T* ptr_;
    };

}

#endif