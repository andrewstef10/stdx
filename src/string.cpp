#include <stdads/string.h>

namespace stdads {

    const void* memchr(const void* ptr, int value, size_t num)
    {
        const unsigned char* p = static_cast<const unsigned char*>(ptr);
        for (; num > 0; --num)
        {
            if (*p == static_cast<unsigned char>(value))
            {
                return p; // found value
            }
            ++p;
        }
        return nullptr; // value not found, return nullptr
    }

    int memcmp(const void* ptr1, const void* ptr2, size_t num)
    {
        if (ptr1 == ptr2) 
        {
            return 0; // If both pointers are to the same location, they are equal
        }

        const unsigned char* p1 = static_cast<const unsigned char*>(ptr1);
        const unsigned char* p2 = static_cast<const unsigned char*>(ptr2);
        for (; num > 0; --num) 
        {
            if (*p1 != *p2) 
            {
                // If *p1 < *p2, return value < 0, otherwise > 0
                return static_cast<int>(*p1) - static_cast<int>(*p2);
            }
            ++p1;
            ++p2;
        }
        return 0; // memory blocks are equal
    }

    void* memcpy(void* destination, const void* source, size_t num)
    {
        unsigned char* dest = static_cast<unsigned char*>(destination);
        const unsigned char* src  = static_cast<const unsigned char*>(source);
        for (; num > 0; --num)
        {
            *dest = *src;
            ++dest;
            ++src;
        }
        return destination;
    }

    void* memset(void* ptr, int value, size_t num)
    {
        unsigned char* dest = static_cast<unsigned char*>(ptr);
        for (; num > 0; --num)
        {
            *dest = static_cast<unsigned char>(value);
            ++dest;
        }
        return ptr;
    }

    char* strcat(char* destination, const char* source)
    {
        char* dest = destination; // save a copy so destination can be returned
        while (*dest != '\0') { ++dest; } // find the end of the dest string

        while (*source != '\0')
        {
            *dest = *source;
            ++source;
            ++dest;
        }
        *dest = '\0';

        return destination;
    }

    const char* strchr(const char* str, int character)
    {
        while (*str != '\0')
        {
            if (*str == static_cast<char>(character))
            {
                return str;
            }
            ++str;
        }

        // check for null term
        if (*str == static_cast<char>(character))
        {
            return str;
        }
        return nullptr; // char not found
    }

    int strcmp(const char* str1, const char* str2)
    {
        // If both pointers are to the same location, they are equal
        if (str1 == str2) 
        {
            return 0;
        }
        
        while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) 
        {
            ++str1;
            ++str2;
        }
        return static_cast<int>(*str1) - static_cast<int>(*str2);
    }

    char* strcpy(char* destination, const char* source)
    {
        // If both pointers are to the same location, they are equal
        if (destination == source)
        {
            return destination;
        }
        
        char* dest = destination; // save of copy of dest to interate
        while (*source != '\0')
        {
            *dest = *source;
            ++dest;
            ++source;
        }
        
        *dest = '\0'; // copy null term
        return destination;
    }

    size_t strlen(const char* str)
    {
        size_t len = 0;
        while (*str != '\0') 
        {
            ++len;
            ++str;
        }
        return len;
    }


    string::string()
    : size_(0)
    , data_(new char[1]{'\0'})
    {
    }

    string::string(const string& other)
    : size_(other.size_)
    , data_(new char[other.size_ + 1])
    {
        memcpy(data_, other.data_, size_ + 1);
    }

    string::string(const char* cstr)
    : size_(strlen(cstr))
    , data_(0)
    {
        data_ = new char[size_ + 1];
        memcpy(data_, cstr, size_ + 1);
    }

    string::string(char c)
    : size_(1)
    , data_(new char[2])
    {
        data_[0] = c;
        data_[1] = '\0';
    }

    string::~string()
    {
        delete[] data_;
    }

    string& string::operator=(const string& other)
    {
        if (this != &other)
        {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            memcpy(data_, other.data_, size_ + 1);
        }
        return *this;
    }

    string& string::operator=(const char* cstr)
    {
        if (data_ != cstr)
        {
            delete[] data_;
            size_ = strlen(cstr);
            data_ = new char[size_ + 1];
            memcpy(data_, cstr, size_ + 1);
        }
        return *this;
    }

    string& string::operator=(char c)
    {
        delete[] data_;
        size_ = 1;
        data_ = new char[size_ + 1];
        data_[0] = c;
        data_[1] = '\0';
        return *this;
    }

    bool string::operator==(const string& other) const
    {
        if (size_ != other.size_) 
        {
            return false;
        }
        return memcmp(data_, other.data_, size_ + 1) == 0;
    }

    bool string::operator==(const char* cstr) const
    {
        return memcmp(data_, cstr, size_ + 1) == 0;
    }

    bool string::operator==(char c) const
    {
        return size_ == 1 && data_[0] == c;
    }

    string& string::operator+=(const string& other)
    {
        char* tempData = new char[size_ + other.size_ + 1];
        memcpy(tempData, data_, size_); // copy current string
        memcpy(tempData + size_, other.data_, other.size_ + 1); // copy new string

        delete[] data_;
        size_ = size_ + other.size_;
        data_ = tempData;
        return *this;
    }

    string& string::operator+=(const char* cstr)
    {
        size_t cstrLength = strlen(cstr);
        char* tempData = new char[size_ + cstrLength + 1];
        memcpy(tempData, data_, size_); // copy current string
        memcpy(tempData + size_, cstr, cstrLength + 1); // copy new string

        delete[] data_;
        size_ = size_ + cstrLength;
        data_ = tempData;
        return *this;
    }

    string& string::operator+=(char c)
    {
        char* tempData = new char[size_ + 2]; // +1 for c and +1 for null term = +2
        memcpy(tempData, data_, size_); // copy current string
        tempData[size_] = c;
        tempData[size_ + 1] = '\0';

        delete[] data_;
        size_ = size_ + 1;
        data_ = tempData;
        return *this;
    }

    void string::clear()
    {
        *this = "";
    }
}