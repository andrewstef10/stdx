#ifndef String_H
#define String_H

#include <cstddef>

namespace stdads {

    /**
     * @brief The default capacity of a String in bytes
     * 
     * Used by the String class to determine how many byes to allocate on the stack by default for SSO (Small String Optimization)
     */
    const static size_t DEFAULT_STRING_CAPACITY_BYTES = 24;

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
    const void* memchr(const void* ptr, int value, size_t num);

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
    int memcmp(const void* ptr1, const void* ptr2, size_t num);

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
    void* memcpy(void* destination, const void* source, size_t num);

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
    void* memset(void* ptr, int value, size_t num);

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
    size_t strlen(const char* str);


    /**
     * @brief String class
     *
     * The templated parameter N represents the initial capacity of the String allocated on the stack.
     * If the String outgrows its initial capacity, dynamic memory allocation will be used to grow the String.
     *
     * @param N The initial capacity of this string allocated on the stack (Default 23).
     */
    template<size_t N = DEFAULT_STRING_CAPACITY_BYTES>
    class String {
    public:

        // Constructors //

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


        // // Assignment operators //

        /**
         * @brief Assigns a new value to the String, replacing its current contents.
         *
         * @param other String object to copy if different from *this.
         * @return Reference to this String object
         */
        String& operator=(const String& other);

        /**
         * @brief Assigns a new value to the String, replacing its current contents.
         *
         * @param cstr Pointer to a null-terminated sequence of characters. The sequence is copied as the new value for the String.
         * @return Reference to this String object
         */
        String& operator=(const char* cstr);

        /**
         * @brief Assigns a new value to the String, replacing its current contents.
         *
         * @param c A character. The String value is set to a single copy of this character.
         * @return Reference to this String object
         */
        String& operator=(char c);


        // // Equality operators //

        // /**
        //  * @brief String Equality Operator
        //  *
        //  * @param other Other String object to compare *this to.
        //  * @return True if *this String has the same content as other
        //  */
        // bool operator==(const String& other) const;

        // /**
        //  * @brief C String Equality Operator
        //  *
        //  * @param cstr Other C String to compare *this to.
        //  * @return True if *this String has the same content as cstr
        //  */
        // bool operator==(const char* cstr) const;

        // /**
        //  * @brief Character Equality Operator
        //  *
        //  * @param c Other Character to compare *this to.
        //  * @return True if *this String is equal to Character c
        //  */
        // bool operator==(char c) const;

        // /**
        //  * @brief String Inequality Operator
        //  *
        //  * @param other Other String object to compare *this to.
        //  * @return True if *this String does not have the same content as other
        //  */
        // bool operator!=(const String& other) const { return !(*this == other); }

        // /**
        //  * @brief C String Inequality Operator
        //  *
        //  * @param cstr Other C String to compare *this to.
        //  * @return True if *this String does not have the same content as cstr
        //  */
        // bool operator!=(const char* cstr) const { return !(*this == cstr); }

        // /**
        //  * @brief Character Inequality Operator
        //  *
        //  * @param c Other Character to compare *this to.
        //  * @return True if *this String is not equal to Character c
        //  */
        // bool operator!=(char c) const { return !(*this == c); }


        // Capacity //

        /**
         * @brief Returns the length of the String, in terms of bytes (excluding the null terminator).
         * 
         * This is the number of actual bytes that conform the contents of the String, which is not necessarily equal to its storage capacity.
         *
         * @return The number of bytes in the String.
         */
        size_t Size() const { return size_; }

        /**
         * @brief Return size of allocated storage
         * 
         * Returns the size of the storage space currently allocated for the string, expressed in terms of bytes (excluding the null terminator).
         * This capacity is not necessarily equal to the string length. It can be equal or greater, with the extra space allowing the object to optimize its operations when new characters are added to the string.
         *
         * @return The size of the storage capacity currently allocated for the string.
         */
        size_t Capacity() const { return capacity_; }

        /**
         * @brief Returns whether the String is empty (i.e. whether its length is 0).
         *
         * @return true if the String length is 0, false otherwise.
         */
        bool Empty() const { return size_ == 0; }


        // Element access //

        /**
         * @brief Get contents as a C String.
         * 
         * Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-String) representing the current value of the String object.
         *
         * @return A pointer to the c-String representation of the String object's value.
         */
        const char* c_str() const { return data_; }

        // char& operator[](size_t i) { return data_[i]; }
        // const char& operator[](size_t i) const { return data_[i]; }
        // char& at(size_t i) { return data_[i]; }
        // const char& at(size_t i) const { return data_[i]; }
        // char& back() { return data_[size_ - 1]; }
        // const char& back() const { return data_[size_ - 1]; }
        // char& front() { return data_[0]; };
        // const char& front() const { return data_[0]; };


        // Modifiers //

        // /**
        //  * @brief Addition Assignment Operator.
        //  * 
        //  * Extends the String by appending additional characters at the end of its current value.
        //  *
        //  * @param other A String object, whose value is copied at the end.
        //  * @return Reference to this String object
        //  */
        // String& operator+=(const String& other);

        // /**
        //  * @brief Addition Assignment Operator.
        //  * 
        //  * Extends the String by appending additional characters at the end of its current value.
        //  *
        //  * @param cstr Pointer to a null-terminated sequence of characters. The sequence is copied at the end of the String.
        //  * @return Reference to this String object
        //  */
        // String& operator+=(const char* cstr);

        // /**
        //  * @brief Addition Assignment Operator.
        //  * 
        //  * Extends the String by appending additional character at the end of its current value.
        //  *
        //  * @param c A character, which is appended to the current value of the String.
        //  * @return Reference to this String object
        //  */
        // String& operator+=(char c);

        // /**
        //  * @brief Erases the contents of the String, which becomes an empty String (with a length of 0 characters).
        //  */
        // void Clear();

    private:
        size_t size_; // length of the string excluding the null terminator
        size_t capacity_; // number of bytes currently allocated for the string excluding the null terminator
        char stackBuffer_[N + 1]; // initial stack allocated memory for this string
        char* data_; // pointer to the Strings char buffer

        /**
         * @brief Delete any dynamically allocated memory for this class
         */
        void Deallocate();

        /**
         * @brief Grows (repeatedly doubles) the capacity of this String until enough space for minSize is achieved.
         * 
         * @param minSize
         */
        void GrowCapacity(size_t minSize);

        /**
         * @brief Set this String equal to a C string
         * 
         * This Function grows this String's capacity if the provided cstr is larger than the current capacity.
         * 
         * @param cstr C String to assign to this
         * @param cstrLength The length of the cstr
         */
        void Assign(const char* cstr, size_t cstrLength);
    };


    // Inline String implementation //

    template<size_t N>
    String<N>::String()
    : size_(0)
    , capacity_(N)
    , data_(stackBuffer_)
    {
        data_[0] = '\0';
    }

    template<size_t N>
    String<N>::String(const String<N>& other)
    : size_(other.size_)
    , capacity_(N)
    {
        Assign(other.c_str(), other.size_);
    }

    template<size_t N>
    String<N>::String(const char* cstr)
    : size_(strlen(cstr))
    , capacity_(N)
    {
        Assign(cstr, size_);
    }

    template<size_t N>
    String<N>::String(char c)
    : size_(1)
    , capacity_(N)
    {
        Assign("X", size_); // "X" is a dummy value
        data_[0] = c;
    }

    template<size_t N>
    String<N>::~String()
    {
        Deallocate();
    }

    template<size_t N>
    String<N>& String<N>::operator=(const String<N>& other)
    {
        if (this != &other)
        {
            Deallocate();
            Assign(other.c_str(), other.size_);
        }
        return *this;
    }

    template<size_t N>
    String<N>& String<N>::operator=(const char* cstr)
    {
        if (data_ != cstr)
        {
            Deallocate();
            Assign(cstr, strlen(cstr));
        }
        return *this;
    }

    template<size_t N>
    String<N>& String<N>::operator=(char c)
    {
        Deallocate();
        Assign("X", 1); // "X" is a dummy value
        data_[0] = c;
        return *this;
    }

    template<size_t N>
    void String<N>::Deallocate()
    {
        if (data_ != stackBuffer_)
        {
            delete[] data_;
            data_ = 0;
        }
    }

    template<size_t N>
    void String<N>::GrowCapacity(size_t minSize)
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

    template<size_t N>
    void String<N>::Assign(const char* cstr, size_t cstrLength)
    {
        if (cstrLength <= N)
        {
            // cstr fits in our allocated stack memory
            data_ = stackBuffer_;
            capacity_ = N;
        }
        else
        {
            // cstr is larger than our allocate stack memory, so use dynamic memory
            GrowCapacity(cstrLength);
            data_ = new char[capacity_ + 1];
        }

        size_ = cstrLength;
        memcpy(data_, cstr, cstrLength + 1);
    }
}

#endif