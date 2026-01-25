#ifndef MATH_H
#define MATH_H

namespace stdads {

    /**
     * @brief Gets the absolute value of a float
     *
     * @param value Value to get absolute value of
     * @return absolute value of the input
     */
    float fabs(float value);

    /**
     * @brief Raises a number to a power
     *
     * This function rasies the base to the exponent exp.
     *
     * @param base Base value
     * @param exp Exponent value
     * @return base raised to the power of exp
     */
    float pow(float base, int exp);

}

#endif