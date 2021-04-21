//
// Created by Ron on 17-Aug-19.
//
// -------------------------------------- INCLUDES --------------------------------------

#include "infi.h"
#include <assert.h>

// -------------------------------------- FUNCTIONS -------------------------------------

/**
 * Performs integration.
 * @param func function to integrate
 * @param a beginning of interval [a,b]
 * @param b ending of interval [a,b]
 * @param n number of partitions
 * @return integration
 */
double integration(RealFunction func, double a, double b, unsigned int n)
{
    assert(a < b);
    assert(n > 0);
    double deltaX = (b - a) / n;
    double res = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        double xi = ((a + i * deltaX) + a + (i + 1) * deltaX) / 2;
        res += func(xi) * deltaX;
    }
    return res;

}

/**
 *
 * @param func function to differentiate
 * @param x0 point of differentiation
 * @param h epsilon
 * @return value of derivative in point
 */
double derivative(RealFunction func, double x0, double h)
{
    assert(h > 0);
    return ((func(x0 + h) - func(x0 - h)) / (2 * h));
}
