#include <stdads/Stdadslib.h>

namespace stdads {

    long Strtol(const char* str, int base, char** endptr)
    {
        // Algo:
        //   multiply each number by base^numPosition
        //   add all together

        // Example: "965" base 10
        //    result = (9*10^2) + (6*10^1) + (5*10^0) -> 900 + 60 + 5 = 965

        // Helpers needed:
        //   Convert a char to a digit supporting letters up to 'z' for base 36

        // Edge cases:
        //  - if base is 10 no math needed. Just convert the char to a digit
        // "962"

        // For now, only support bases from 0 - 36
        if (base < 0 || base > 36) return 0L;


        return 0L;
    }
}