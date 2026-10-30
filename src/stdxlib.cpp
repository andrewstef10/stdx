#include <stdx/stdxlib.h>

#include <stdx/ctype.h>
#include <stdx/string.h>

namespace stdx {

long strtol(const char* str, int base, char** endptr)
{
    // Set endptr to str. Will be overridden if parsing is successful.
    if (endptr != nullptr)
    {
        *endptr = const_cast<char*>(str); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    // Only support bases from 0 - 36
    if (base < 0 || base > 36) // NOLINT(readability-magic-numbers)
    {
        return 0L;
    }

    // skip white space
    str = stdx::trimFront(str);

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
            if (stdx::toLower(*str) == 'x')
            {
                // hex
                base = 16; // NOLINT(readability-magic-numbers)
                ++str;
            }
            else
            {
                // octal
                base = 8; // NOLINT(readability-magic-numbers)
            }
        }
        else
        {
            // else assume decimal
            base = 10; // NOLINT(readability-magic-numbers)
        }
    }
    else if (base == 16) // NOLINT(readability-magic-numbers)
    {
        // strip 0x prefix
        if (*str == '0' && stdx::toLower(*(str + 1)) == 'x')
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
        int digit = stdx::charToDigit(*str, base);
        if (digit < 0)
        {
            break;
        }
        result = (result * base) + digit;
        valid = true;
        ++str;
    }

    if (valid)
    {
        if (negative)
        {
            result = -result;
        }
        if (endptr != nullptr)
        {
            *endptr = const_cast<char*>(str); // NOLINT(cppcoreguidelines-pro-type-const-cast)
        }
    }
    return result;
}
} // namespace stdx
