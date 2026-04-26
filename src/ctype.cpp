#include <stdx/ctype.h>
#include <stdx/num_utils.h>

namespace stdx {

    bool is_alpha(char c)
    {
        return stdx::within(c, 'A', 'Z') || stdx::within(c, 'a', 'z');
    }

    bool is_digit(char c)
    {
        return stdx::within(c, '0', '9');
    }

    bool is_digit_of_base(char c, int base)
    {
        return char_to_digit(c, base) >= 0;
    }

    int char_to_digit(char c, int base)
    {
        if (base <= 0)
        {
            return -1; // positive bases non zero bases only
        }

        int result = -1;
        if (base == 1) // base 1
        {
            if (c == '1') // base 1 only contains 1s
            {
                result = 1;
            }
        }
        else if (base <= 10) // bases 2 - 10
        {
            // bases 2 - 10 only contain digits in range 0 - (base - 1)
            if (stdx::within(c, '0', static_cast<char>('0' + (base - 1))))
            {
                result = c - '0';
            }
        }
        else if (base <= 36) // bases 11 - 36
        {
            // bases 11 - 36 contain digits and letters in range a - (base - 11)
            if (stdx::within(c, '0', '9'))
            {
                result = c - '0';
            }
            else if (stdx::within(c, 'a', static_cast<char>('a' + (base - 11))))
            {
                result = (c - 'a') + 10;
            }
            else if (stdx::within(c, 'A', static_cast<char>('A' + (base - 11))))
            {
                result = (c - 'A') + 10;
            }
        }
        return result;
    }

    char to_lower(char c)
    {
        if (stdx::within(c, 'A', 'Z'))
        {
            c += 32;
        }
        return c;
    }

    char to_upper(char c)
    {
        if (stdx::within(c, 'a', 'z'))
        {
            c -= 32;
        }
        return c;
    }

}