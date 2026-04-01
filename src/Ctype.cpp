#include <stdads/Ctype.h>
#include <stdads/NumUtils.h>

namespace stdads {

    bool IsAlpha(char c)
    {
        return stdads::Within(c, 'A', 'Z') || stdads::Within(c, 'a', 'z');
    }

    bool IsDigit(char c)
    {
        return stdads::Within(c, '0', '9');
    }

    bool IsDigitOfBase(char c, int base)
    {
        return CharToDigit(c, base) >= 0;
    }

    int CharToDigit(char c, int base)
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
            if (stdads::Within(c, '0', static_cast<char>('0' + (base - 1))))
            {
                result = c - '0';
            }
        }
        else if (base <= 36) // bases 11 - 36
        {
            // bases 11 - 36 contain digits and letters in range a - (base - 11)
            if (stdads::Within(c, '0', '9'))
            {
                result = c - '0';
            }
            else if (stdads::Within(c, 'a', static_cast<char>('a' + (base - 11))))
            {
                result = (c - 'a') + 10;
            }
            else if (stdads::Within(c, 'A', static_cast<char>('A' + (base - 11))))
            {
                result = (c - 'A') + 10;
            }
        }
        return result;
    }

    char ToLower(char c)
    {
        if (stdads::Within(c, 'A', 'Z'))
        {
            c += 32;
        }
        return c;
    }

    char ToUpper(char c)
    {
        if (stdads::Within(c, 'a', 'z'))
        {
            c -= 32;
        }
        return c;
    }

}