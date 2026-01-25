#include <stdads/math.h>;

namespace stdads {

    float fabs(float value)
    {
        if (value < 0.0f)
        {
            return -value;
        }
        return value;
    }

    float pow(float base, int exp)
    {
        float result = 1.0f;
        float abs_exp = fabs(exp);
        for (int i = 0; i < abs_exp; ++i)
        {
            result *= base;
        }

        if (exp < 0)
        {
            result = 1.0f / result;
        }

        return result;
    }
}
