#ifndef STRING_H
#define STRING_H

#include <cstddef>

namespace stdads {

    /**
     * @brief The default capacity of a String in bytes
     * 
     * Used by the String class to determine how many byes to allocate on the stack by default for SSO (Small String Optimization)
     */
    const static std::size_t DEFAULT_STRING_CAPACITY_BYTES = 24;

    /**
     * @brief Searches within the first num bytes of the block of memory pointed by ptr for the first occurrence of value (interpreted as an unsigned char), and returns a pointer to it.
     * 
     * Both value and each of the bytes checked on the the ptr array are interpreted as unsigned char for the comparison.
     * Passing ptr as nullptr results in undefined behavior.
     * 
     * @param ptr Pointer to the block of memory to be searched
     * @param value Value to be located. The value is passed as an int, but the function performs a byte per byte search using the unsigned char conversion of this value.
     * @param num Number of bytes to be analyzed
     * @return A pointer to the first occurrence of value in the block of memory pointed by ptr.
     *         If the value is not found, the function returns a null pointer.
     */
    const void* memchr(const void* ptr, int value, std::size_t num);

    /**
     * @brief Compare two blocks of memory
     *
     * Compares the first num bytes of the block of memory pointed by ptr1 to the first num bytes pointed by ptr2.
     * Returns zero if they all match or a value different from zero representing which is greater if they do not.
     * Passing ptr1 or ptr2 as nullptr results in undefined behavior.
     *
     * @param ptr1 Pointer to a memory block
     * @param ptr2 Pointer to a memory block
     * @param num Number of bytes to compare
     * @return Zero if both memory blocks are equal.
     *         Negative if the first byte that does not match in both memory blocks has a lower value in ptr1 than in ptr2 (if evaluated as unsigned char values).
     *         Positive if the first byte that does not match in both memory blocks has a greater value in ptr1 than in ptr2 (if evaluated as unsigned char values).
     */
    int memcmp(const void* ptr1, const void* ptr2, std::size_t num);

    /**
     * @brief Copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination.
     *
     * The underlying type of the objects pointed to by both the source and destination pointers are irrelevant for this function; The result is a binary copy of the data.
     * The function does not check for any terminating null character in source - it always copies exactly num bytes.
     * To avoid overflows, the size of the arrays pointed to by both the destination and source parameters, shall be at least num bytes, and should not overlap (for overlapping memory blocks, memmove is a safer approach).
     * Passing destination or source as nullptr results in undefined behavior.
     *
     * @param destination Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
     * @param source Pointer to the source of data to be copied, type-casted to a pointer of type const void*.
     * @param num Number of bytes to copy.
     * @return destination is returned.
     */
    void* memcpy(void* destination, const void* source, std::size_t num);

    /**
     * @brief Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).
     * 
     * Passing ptr as nullptr results in undefined behavior.
     *
     * @param ptr Pointer to the block of memory to fill.
     * @param value Value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value.
     * @param num Number of bytes to be set to the value.
     * @return ptr is returned.
     */
    void* memset(void* ptr, int value, std::size_t num);

    /**
     * @brief Concatenate Strings
     * 
     * Appends a copy of the source String to the destination String.
     * The terminating null character in destination is overwritten by the first character of source, and a null-character is included at the end of the new String formed by the concatenation of both in destination.
     * destination and source shall not overlap.
     *
     * @param destination Pointer to the destination array, which should contain a C String, and be large enough to contain the concatenated resulting String.
     * @param source C String to be appended. This should not overlap destination.
     * @return destination is returned.
     */
    char* strcat(char* destination, const char* source);

    /**
     * @brief Locate first occurrence of character in String
     * 
     * Returns a pointer to the first occurrence of character in the C String str.
     * The terminating null-character is considered part of the C String. Therefore, it can also be located in order to retrieve a pointer to the end of a String.
     *
     * @param str C String.
     * @param character Character to be located. It is passed as its int promotion, but it is internally converted back to char for the comparison.
     * @return A pointer to the first occurrence of character in str. If the character is not found, the function returns a null pointer.
     */
    const char* strchr(const char* str, int character);

    /**
     * @brief Compares the C String str1 to the C String str2.
     *
     * This function starts comparing the first character of each String. If they are equal to each other, it continues with the following pairs until the characters differ or until a terminating null-character is reached.
     * This function performs a binary comparison of the characters.
     * Passing str1 or str2 as nullptr results in undefined behavior.
     *
     * @param str1 C String to be compared.
     * @param str2 C String to be compared.
     * @return Zero if the contents of both Strings are equal.
     *         Negative if the first character that does not match has a lower value in ptr1 than in ptr2.
     *         Positive if the first character that does not match has a greater value in ptr1 than in ptr2
     */
    int strcmp(const char* str1, const char* str2);

    /**
     * @brief Copies the C String pointed by source into the array pointed by destination, including the terminating null character (and stopping at that point).
     * 
     * To avoid overflows, the size of the array pointed by destination shall be long enough to contain the same C String as source (including the terminating null character), and should not overlap in memory with source.
     * Passing destination or source as nullptr results in undefined behavior.
     *
     * @param destination Pointer to the destination array where the content is to be copied.
     * @param source C String to be copied.
     * @return destination is returned.
     */
    char* strcpy(char* destination, const char* source);

    /**
     * @brief Returns the length of the C String str.
     *
     * The length of a C String is determined by the terminating null-character:
     * A C String is as long as the number of characters between the beginning of the String and the terminating null character (without including the terminating null character itself).
     * Passing str as nullptr results in undefined behavior.
     *
     * @param str C String.
     * @return The length of str.
     */
    std::size_t strlen(const char* str);


    /**
     * @brief String class
     *
     * The templated parameter N represents the initial capacity of the String allocated on the stack.
     * If the String outgrows its initial capacity, dynamic memory allocation will be used to grow the String.
     *
     * @param N The initial capacity of this string allocated on the stack (Default 23).
     */
    template<std::size_t N = DEFAULT_STRING_CAPACITY_BYTES>
    class String {
    public:

        /**
         * @brief Default constructor.
         *
         * Creates an empty String.
         */
        String();

        /**
         * @brief Copy constructor.
         *
         * @param other String to copy.
         */
        String(const String& other);
        template<std::size_t M>
        String(const String<M>& other); 

        /**
         * @brief C String constructor.
         *
         * @param cstr C String.
         */
        String(const char* cstr);

        /**
         * @brief Character constructor.
         *
         * @param c A character.
         */
        String(char c);

        /**
         * @brief Destructor.
         */
        ~String();

        /**
         * @brief Assigns a new value to the String, replacing its current contents.
         *
         * @param other String object to copy if different from *this.
         * @return Reference to this String object
         */
        String& operator=(const String& other) { return Assign(other.c_str(), other.Size()); }
        template<std::size_t M>
        String& operator=(const String<M>& other) { return Assign(other.c_str(), other.Size()); }

        /**
         * @brief Assigns a new value to the String, replacing its current contents.
         *
         * @param cstr Pointer to a null-terminated sequence of characters. The sequence is copied as the new value for the String.
         * @return Reference to this String object
         */
        String& operator=(const char* cstr) { return Assign(cstr, strlen(cstr)); }

        /**
         * @brief Assigns a new value to the String, replacing its current contents.
         *
         * @param c A character. The String value is set to a single copy of this character.
         * @return Reference to this String object
         */
        String& operator=(char c);


        //////////////////////// Capacity ////////////////////////

        /**
         * @brief Returns the length of the String, in terms of bytes (excluding the null terminator).
         * 
         * This is the number of actual bytes that conform the contents of the String, which is not necessarily equal to its storage capacity.
         *
         * @return The number of bytes in the String.
         */
        std::size_t Size() const { return size_; }

        /**
         * @brief Return size of allocated storage
         * 
         * Returns the size of the storage space currently allocated for the string, expressed in terms of bytes (excluding the null terminator).
         * This capacity is not necessarily equal to the string length. It can be equal or greater, with the extra space allowing the object to optimize its operations when new characters are added to the string.
         *
         * @return The size of the storage capacity currently allocated for the string.
         */
        std::size_t Capacity() const { return capacity_; }

        /**
         * @brief Returns whether the String is empty (i.e. whether its length is 0).
         *
         * @return true if the String length is 0, false otherwise.
         */
        bool Empty() const { return size_ == 0; }


        //////////////////////// Element Access ////////////////////////

        /**
         * @brief Get contents as a C String.
         * 
         * Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-String) representing the current value of the String object.
         *
         * @return A pointer to the c-String representation of the String object's value.
         */
        const char* c_str() const { return GetData(); }

        /**
         * @brief Returns a reference to the character at specified location pos if pos < size(), or if pos == size()
         * 
         * If pos > Size(), the behavior is undefined. Bounds checking is not performed.
         *
         * @param pos Position of the character to return
         * @return Reference to char at position pos
         */
        char& operator[](std::size_t pos) { return GetData()[pos]; }
        const char& operator[](std::size_t pos) const { return GetData()[pos]; }


        //////////////////////// Modifiers ////////////////////////

        /**
         * @brief Addition Assignment Operator.
         * 
         * Extends the String by appending additional characters at the end of its current value.
         *
         * @param other A String object, whose value is copied at the end.
         * @return Reference to this String object
         */
        String& operator+=(const String<N>& other) { return Append(other.c_str(), other.Size()); }
        template<std::size_t M>
        String& operator+=(const String<M>& other) { return Append(other.c_str(), other.Size()); }

        /**
         * @brief Addition Assignment Operator.
         * 
         * Extends the String by appending additional characters at the end of its current value.
         *
         * @param cstr Pointer to a null-terminated sequence of characters. The sequence is copied at the end of the String.
         * @return Reference to this String object
         */
        String& operator+=(const char* cstr) { return Append(cstr, strlen(cstr)); }

        /**
         * @brief Addition Assignment Operator.
         * 
         * Extends the String by appending additional character at the end of its current value.
         *
         * @param c A character, which is appended to the current value of the String.
         * @return Reference to this String object
         */
        String& operator+=(char c);

        /**
         * @brief Erases the contents of the String, which becomes an empty String (with a length of 0 characters).
         */
        void Clear();

    private:
        std::size_t size_; // length of the string excluding the null terminator
        std::size_t capacity_; // number of bytes currently allocated for the string excluding the null terminator

        union StringData
        {
            char stackBuffer[N + 1]; // initial stack allocated memory for this string
            char* heapPtr; // pointer to Strings memory after capacity has grown
        } data_;


        /**
         * @brief Helper to get a pointer to the correct buffer (stack or heap)
         * 
         * @returns Pointer to the String's character buffer
         */
        char* GetData() { return capacity_ > N ? data_.heapPtr : data_.stackBuffer; }
        const char* GetData() const { return capacity_ > N ? data_.heapPtr : data_.stackBuffer; }

        /**
         * @brief Delete any dynamically allocated memory for this class
         */
        void Deallocate();

        /**
         * @brief Grows (repeatedly doubles) the capacity of this String until enough space for minSize is achieved.
         * 
         * @param minSize
         */
        void GrowCapacity(std::size_t minSize);

        /**
         * @brief Set this String equal to a C string
         * 
         * This Function grows this String's capacity if the provided cstr is larger than the current capacity.
         * 
         * @param cstr C String to assign to this
         * @param cstrLength The length of the cstr
         * @returns Reference to this String object
         */
        String& Assign(const char* cstr, std::size_t cstrLength);

        /**
         * @brief Append a C string to the end of this String
         * 
         * This Function grows this String's capacity if the provided cstr is larger than the current capacity.
         * 
         * @param cstr C String to append to this
         * @param cstrLength The length of the cstr
         * @returns Reference to this String object
         */
        String& Append(const char* cstr, std::size_t cstrLength);
    };


    ////////////////////////String Non Members ////////////////////////

    /**
     * @brief String Equality Operator
     *
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return True if lhs and rhs are equal, false otherwise.
     */
    template<std::size_t N>
    bool operator==(const String<N>& lhs, const String<N>& rhs);
    template<std::size_t N, std::size_t M>
    bool operator==(const String<N>& lhs, const String<M>& rhs);

    /**
     * @brief C String Equality Operator
     *
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return True if lhs and rhs are equal, false otherwise.
     */
    template<std::size_t N>
    bool operator==(const String<N>& lhs, const char* rhs);
    template<std::size_t N>
    bool operator==(const char* lhs, const String<N>& rhs) { return rhs == lhs; }

    /**
     * @brief Character Equality Operator
     *
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return True if lhs and rhs are equal, false otherwise.
     */
    template<std::size_t N>
    bool operator==(const String<N>& lhs, char rhs);
    template<std::size_t N>
    bool operator==(char lhs, const String<N>& rhs) { return rhs == lhs; }

    /**
     * @brief String Inequality Operator
     *
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return True if lhs and rhs are equal, false otherwise.
     */
    template<std::size_t N>
    bool operator!=(const String<N>& lhs, const String<N>& rhs) { return !(lhs == rhs); }
    template<std::size_t N, std::size_t M>
    bool operator!=(const String<N>& lhs, const String<M>& rhs) { return !(lhs == rhs); }

    /**
     * @brief C String Inequality Operator
     *
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return True if lhs and rhs are equal, false otherwise.
     */
    template<std::size_t N>
    bool operator!=(const String<N>& lhs, const char* rhs) { return !(lhs == rhs); }
    template<std::size_t N>
    bool operator!=(const char* lhs, const String<N>& rhs) { return !(rhs == lhs); }

    /**
     * @brief Character Inequality Operator
     *
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return True if lhs and rhs are equal, false otherwise.
     */
    template<std::size_t N>
    bool operator!=(const String<N>& lhs, char rhs) { return !(lhs == rhs); }
    template<std::size_t N>
    bool operator!=(char lhs, const String<N>& rhs) { return !(rhs == lhs); }


    //////////////////////// Inline String Implementation ////////////////////////

    template<std::size_t N>
    String<N>::String()
    : size_(0)
    , capacity_(N)
    , data_{'\0'}
    {
    }

    template<std::size_t N>
    String<N>::String(const String<N>& other)
    : size_(0)
    , capacity_(N)
    , data_{'\0'}
    {
        Assign(other.c_str(), other.Size());
    }

    template<std::size_t N>
    template<std::size_t M>
    String<N>::String(const String<M>& other)
    : size_(0)
    , capacity_(N)
    , data_{'\0'}
    {
        Assign(other.c_str(), other.Size());
    }

    template<std::size_t N>
    String<N>::String(const char* cstr)
    : size_(0)
    , capacity_(N)
    , data_{'\0'}
    {
        Assign(cstr, strlen(cstr));
    }

    template<std::size_t N>
    String<N>::String(char c)
    : size_(0)
    , capacity_(N)
    , data_{'\0'}
    {
        char buff[] = {c, '\0'};
        Assign(buff, 1);
    }

    template<std::size_t N>
    String<N>::~String()
    {
        Deallocate();
    }

    template<std::size_t N>
    String<N>& String<N>::operator=(char c)
    {
        char buff[] = {c, '\0'};
        return Assign(buff, 1);
    }

    template<std::size_t N>
    String<N>& String<N>::operator+=(char c)
    {
        char buff[] = {c, '\0'};
        return Append(buff, 1);
    }

    template<std::size_t N>
    void String<N>::Clear()
    {
        size_ = 0;
        GetData()[0] = '\0';
    }

    template<std::size_t N>
    void String<N>::Deallocate()
    {
        if (capacity_ > N)
        {
            delete[] data_.heapPtr;
            data_.heapPtr = 0;
        }
    }

    template<std::size_t N>
    void String<N>::GrowCapacity(std::size_t minSize)
    {
        while (capacity_ < minSize)
        {
            if (capacity_ == 0)
            {
                capacity_ = 1;
            }
            else
            {
                capacity_ *= 2;
            }
        }
    }

    template<std::size_t N>
    String<N>& String<N>::Assign(const char* cstr, std::size_t cstrLength)
    {
        if (cstrLength > capacity_)
        {
            // need to allocate more memory
            Deallocate();
            GrowCapacity(cstrLength);
            data_.heapPtr = new char[capacity_ + 1];
        }

        size_ = cstrLength;
        char* data = GetData();
        memcpy(data, cstr, cstrLength); // assign all but the null term
        data[size_] = '\0';
        return *this;
    }

    template<std::size_t N>
    String<N>& String<N>::Append(const char* cstr, std::size_t cstrLength)
    {
        if (size_ + cstrLength > capacity_)
        {
            // Need to allocate more memory.
            char* tempData = new char[capacity_ + 1];
            memcpy(tempData, GetData(), size_); // copy current String. Not plus 1 because theres no need to copy null term (we are appending)

            Deallocate();
            GrowCapacity(size_ + cstrLength);
            data_.heapPtr = tempData;
        }

        char* data = GetData();
        memcpy(data + size_, cstr, cstrLength); // append other String excluding the null term
        size_ += cstrLength;
        data[size_] = '\0';
        return *this;
    }

    template<std::size_t N>
    bool operator==(const String<N>& lhs, const String<N>& rhs)
    {
        if (lhs.Size() != rhs.Size())
        {
            return false;
        }
        return memcmp(lhs.c_str(), rhs.c_str(), lhs.Size() + 1) == 0;
    }

    template<std::size_t N, std::size_t M>
    bool operator==(const String<N>& lhs, const String<M>& rhs)
    {
        if (lhs.Size() != rhs.Size())
        {
            return false;
        }
        return memcmp(lhs.c_str(), rhs.c_str(), lhs.Size() + 1) == 0;
    }

    template<std::size_t N>
    bool operator==(const String<N>& lhs, const char* rhs)
    {
        return memcmp(lhs.c_str(), rhs, lhs.Size() + 1) == 0;
    }

    template<std::size_t N>
    bool operator==(const String<N>& lhs, char rhs)
    {
        return lhs.Size() == 1 && lhs.c_str()[0] == rhs;
    }
}

#endif