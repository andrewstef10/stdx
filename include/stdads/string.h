#ifndef STRING_H
#define STRING_H

#include <cstddef>

namespace stdads {

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
     * @brief Concatenate strings
     * 
     * Appends a copy of the source string to the destination string.
     * The terminating null character in destination is overwritten by the first character of source, and a null-character is included at the end of the new string formed by the concatenation of both in destination.
     * destination and source shall not overlap.
     *
     * @param destination Pointer to the destination array, which should contain a C string, and be large enough to contain the concatenated resulting string.
     * @param source C string to be appended. This should not overlap destination.
     * @return destination is returned.
     */
    char* strcat(char* destination, const char* source);

    /**
     * @brief Locate first occurrence of character in string
     * 
     * Returns a pointer to the first occurrence of character in the C string str.
     * The terminating null-character is considered part of the C string. Therefore, it can also be located in order to retrieve a pointer to the end of a string.
     *
     * @param str C string.
     * @param character Character to be located. It is passed as its int promotion, but it is internally converted back to char for the comparison.
     * @return A pointer to the first occurrence of character in str. If the character is not found, the function returns a null pointer.
     */
    const char* strchr(const char* str, int character);

    /**
     * @brief Compares the C string str1 to the C string str2.
     *
     * This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs until the characters differ or until a terminating null-character is reached.
     * This function performs a binary comparison of the characters.
     * Passing str1 or str2 as nullptr results in undefined behavior.
     *
     * @param str1 C string to be compared.
     * @param str2 C string to be compared.
     * @return Zero if the contents of both strings are equal.
     *         Negative if the first character that does not match has a lower value in ptr1 than in ptr2.
     *         Positive if the first character that does not match has a greater value in ptr1 than in ptr2
     */
    int strcmp(const char* str1, const char* str2);

    /**
     * @brief Copies the C string pointed by source into the array pointed by destination, including the terminating null character (and stopping at that point).
     * 
     * To avoid overflows, the size of the array pointed by destination shall be long enough to contain the same C string as source (including the terminating null character), and should not overlap in memory with source.
     * Passing destination or source as nullptr results in undefined behavior.
     *
     * @param destination Pointer to the destination array where the content is to be copied.
     * @param source C string to be copied.
     * @return destination is returned.
     */
    char* strcpy(char* destination, const char* source);

    /**
     * @brief Returns the length of the C string str.
     *
     * The length of a C string is determined by the terminating null-character:
     * A C string is as long as the number of characters between the beginning of the string and the terminating null character (without including the terminating null character itself).
     * Passing str as nullptr results in undefined behavior.
     *
     * @param str C string.
     * @return The length of str.
     */
    size_t strlen(const char* str);


    class string {
    public:
        // Constructors //

        /**
         * @brief Default constructor.
         *
         * Creates an empty string.
         */
        string();

        /**
         * @brief Copy constructor.
         *
         * @param other String to copy.
         */
        string(const string& other);

        /**
         * @brief C string constructor.
         *
         * @param cstr C string.
         */
        string(const char* cstr);

        /**
         * @brief Character constructor.
         *
         * @param c A character.
         */
        string(char c);

        /**
         * @brief Destructor.
         */
        ~string();


        // Assignment operators //

        /**
         * @brief Assigns a new value to the string, replacing its current contents.
         *
         * @param other String object to copy if different from *this.
         * @return Reference to this string object
         */
        string& operator=(const string& other);

        /**
         * @brief Assigns a new value to the string, replacing its current contents.
         *
         * @param cstr Pointer to a null-terminated sequence of characters. The sequence is copied as the new value for the string.
         * @return Reference to this string object
         */
        string& operator=(const char* cstr);

        /**
         * @brief Assigns a new value to the string, replacing its current contents.
         *
         * @param c A character. The string value is set to a single copy of this character.
         * @return Reference to this string object
         */
        string& operator=(char c);


        // Equality operators //

        /**
         * @brief String Equality Operator
         *
         * @param other Other string object to compare *this to.
         * @return True if *this string has the same content as other
         */
        bool operator==(const string& other) const;

        /**
         * @brief C String Equality Operator
         *
         * @param cstr Other C String to compare *this to.
         * @return True if *this string has the same content as cstr
         */
        bool operator==(const char* cstr) const;

        /**
         * @brief Character Equality Operator
         *
         * @param c Other Character to compare *this to.
         * @return True if *this string is equal to Character c
         */
        bool operator==(char c) const;

        /**
         * @brief String Inequality Operator
         *
         * @param other Other string object to compare *this to.
         * @return True if *this string does not have the same content as other
         */
        bool operator!=(const string& other) const { return !(*this == other); }

        /**
         * @brief C String Inequality Operator
         *
         * @param cstr Other C String to compare *this to.
         * @return True if *this string does not have the same content as cstr
         */
        bool operator!=(const char* cstr) const { return !(*this == cstr); }

        /**
         * @brief Character Inequality Operator
         *
         * @param c Other Character to compare *this to.
         * @return True if *this string is not equal to Character c
         */
        bool operator!=(char c) const { return !(*this == c); }


        // Capacity //

        /**
         * @brief Returns the length of the string, in terms of bytes.
         * 
         * This is the number of actual bytes that conform the contents of the string, which is not necessarily equal to its storage capacity.
         *
         * @return The number of bytes in the string.
         */
        size_t size() const { return size_; }

        /**
         * @brief Returns whether the string is empty (i.e. whether its length is 0).
         *
         * @return true if the string length is 0, false otherwise.
         */
        bool empty() const { return size_ == 0; }


        // Element access //

        /**
         * @brief Get contents as a C String.
         * 
         * Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string) representing the current value of the string object.
         *
         * @return A pointer to the c-string representation of the string object's value.
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

        /**
         * @brief Addition Assignment Operator.
         * 
         * Extends the string by appending additional characters at the end of its current value.
         *
         * @param other A string object, whose value is copied at the end.
         * @return Reference to this string object
         */
        string& operator+=(const string& other);

        /**
         * @brief Addition Assignment Operator.
         * 
         * Extends the string by appending additional characters at the end of its current value.
         *
         * @param cstr Pointer to a null-terminated sequence of characters. The sequence is copied at the end of the string.
         * @return Reference to this string object
         */
        string& operator+=(const char* cstr);

        /**
         * @brief Addition Assignment Operator.
         * 
         * Extends the string by appending additional character at the end of its current value.
         *
         * @param c A character, which is appended to the current value of the string.
         * @return Reference to this string object
         */
        string& operator+=(char c);

        /**
         * @brief Erases the contents of the string, which becomes an empty string (with a length of 0 characters).
         */
        void clear();

    private:
        char* data_;
        size_t size_;
    };
}

#endif