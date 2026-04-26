#include <stdx/math.h>

namespace stdx {

    float abs(float value)              { return (value < 0.0f) ? -value : value; }
    double abs(double value)            { return (value < 0.0) ? -value : value; }
    long double abs(long double value)  { return (value < 0.0L) ? -value : value; }
    int abs(int value)                  { return (value < 0) ? -value : value; }
    long abs(long value)                { return (value < 0) ? -value : value; }
    long long abs(long long value)      { return (value < 0) ? -value : value; }

    float pow(float base, int exp)
    {
        float result = 1.0f;
        int abs_exp = abs(exp);
        while (abs_exp > 0) 
        {
            if (abs_exp & 1) 
            {
                result *= base;
            }
            base *= base;
            abs_exp >>= 1;
        }
        return (exp < 0) ? 1.0f / result : result;
    }

    double pow(double base, int exp)
    {
        double result = 1.0;
        int abs_exp = abs(exp);
        while (abs_exp > 0) 
        {
            if (abs_exp & 1) 
            {
                result *= base;
            }
            base *= base;
            abs_exp >>= 1;
        }
        return (exp < 0) ? 1.0 / result : result;
    }
}
