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
        
        *dest = *source; // copy null term
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
        for (size_t i = 0; i < size_; ++i) 
        {
            data_[i] = other.data_[i];
        }
        data_[size_] = '\0';
    }

    string::string(const char* cstr)
    : size_(strlen(cstr))
    {
        data_ = new char[size_ + 1];
        for (size_t i = 0; i < size_; ++i) 
        {
            data_[i] = cstr[i];
        }
        data_[size_] = '\0';
    }

    string::~string()
    {
        delete[] data_;
    }

    bool string::operator==(const string& other) const
    {
        if (size_ != other.size_) 
        {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) 
        {
            if (data_[i] != other.data_[i]) 
            {
                return false;
            }
        }
        return true;
    }

    bool string::operator==(const char* cstr) const
    {
        size_t cstr_len = strlen(cstr);
        if (size_ != cstr_len) 
        {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) 
        {
            if (data_[i] != cstr[i]) 
            {
                return false;
            }
        }
        return true;
    }

    string& string::operator=(const string& other)
    {
        if (this != &other && strcmp(data_, other.data_) != 0)
        {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            for (size_t i = 0; i < size_; ++i) 
            {
                data_[i] = other.data_[i];
            }
            data_[size_] = '\0';
        }
        return *this;
    }

    string& string::operator=(const char* cstr)
    {
        if (strcmp(data_, cstr) != 0)
        {
            delete[] data_;
            size_ = strlen(cstr);
            data_ = new char[size_ + 1];
            for (size_t i = 0; i < size_; ++i) 
            {
                data_[i] = cstr[i];
            }
            data_[size_] = '\0';
        }
        return *this;
    }
}