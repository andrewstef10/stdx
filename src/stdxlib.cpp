#include <stdx/stdxlib.h>

#include <cstddef>
#include <stdx/ctype.h>

namespace stdx {

    long strtol(const char* str, int base, char** endptr)
    {
        // Set endptr to str. Will be overridden if parsing is successful.
        if (endptr)
        {
            *endptr = const_cast<char*>(str);
        }

        // Only support bases from 0 - 36
        if (base < 0 || base > 36)
        {
            return 0L;
        }

        // skip white space
        while (*str == ' ')
        {
            ++str;
        }

        // Handle sign
        bool negative = false;
        if (*str == '+')
        {
            ++str;
        }
        else if (*str == '-')
        {
            negative = true;
            ++str;
        }

        // Handle base 0 (auto detect base)
        if (base == 0)
        {
            if (*str == '0')
            {
                ++str;
                if (*str == 'x' || *str == 'X')
                {
                    // hex
                    base = 16;
                    ++str;
                }
                else
                {
                    // octal
                    base = 8;
                }
            }
            else
            {
                // else assume decimal
                base = 10;
            }
        }
        else if (base == 16)
        {
            // strip 0x prefix
            if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X'))
            {
                // hex
                str += 2;
            }
        }

        // Parse result
        long result = 0L;
        bool valid = false;
        while (*str != '\0')
        {
            int digit = stdx::char_to_digit(*str, base);
            if (digit < 0)
            {
                break;
            }
            result = result * base + digit;
            valid = true;
            ++str;
        }

        if (valid)
        {
            if (negative)
            {
                result = -result;
            }
            if (endptr)
            {
                *endptr = const_cast<char*>(str);
            }
        }
        return result;
    }
}