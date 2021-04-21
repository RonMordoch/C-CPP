//
// Created by Ron on 17-Aug-19.
//
// -------------------------------------- INCLUDES --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "infi.h"

// ---------------------------------- CONST DEFINITIONS ---------------------------------

#define MAX_ARGS_NUM 7
#define INVALID_ARGS_NUM "Usage: NumericalAnalyzer <function number> <a> <b> <N> <x0> <h>\n"
#define INVALID_INPUT "Invalid input\n"

// -------------------------------------- FUNCTIONS -------------------------------------

// ---------- 6 MATHEMATICAL FUNCTIONS ----------
/**
 *
 * @param x
 * @return f1 value of x
 */
double f1(double x)
{
    return ((pow(x, 2)) / 4);
}

/**
 *
 * @param x
 * @return f2 value of x
 */
double f2(double x)
{
    return (-pow(x, 3) + 3 * pow(x, 2) + x - 4 * sqrt(x)) / (2 * x * sqrt(x));
}

/**
 *
 * @param x
 * @return f3 value of x
 */
double f3(double x)
{
    return (sin(sin(x)) - cos(cos(x)));
}

/**
 *
 * @param x
 * @return f4 value of x
 */
double f4(double x)
{
    return (sin(x)) / (1 + cos(2 * x));
}

/**
 *
 * @param x
 * @return f5 value of x
 */
double f5(double x)
{
    return pow(M_E, x);
}

/**
 *
 * @param x
 * @return f6 value of x
 */
double f6(double x)
{
    return (sinh(2 * x)) / (M_E * pow(x, 2));
}

// ---------- INPUT VALIDATION ----------

/**
 * Choose a function, receives valid input after validation.
 * @param funcNum
 * @return RealFunction function to perform on
 */
RealFunction chooseFunction(int funcNum)
{
    switch (funcNum)
    {
        case 1:
            return &f1;
        case 2:
            return &f2;
        case 3:
            return &f3;
        case 4:
            return &f4;
        case 5:
            return &f5;
        case 6:
            return &f6;
        default:
            return NULL;
    }
}

/**
 * Checks if a string is a valid positive integer
 * @param string string to validate
 * @return integer representation of string if valid; -1 otherwise;
 */
int stringToInteger(char const *string, size_t length)
{
    int num = 0;
    for (size_t i = 0; i < length; ++i)
    {
        if (string[i] > '9' || string[i] < '0')
        {
            return -1;
        }
        num *= 10;
        int c = string[i] - '0';
        num += c;
    }
    // number is definitely only digits and positive, return the integer type
    return num;
}

/**
 * Converts a string representing a valid number into a double.
 * @param value string representing a valid number
 * @param length length of string
 */
void stringToDouble(const char *value, size_t length, double *dbl)
{
    double sgn = 1;
    size_t j = 0;
    if (value[0] == '-')
    {
        sgn = -1;
        ++j;
    }
    while ((j < length) && (value[j] != '.')) // add the numbers in the integer part of numbers
    {
        *dbl *= 10;
        int c = value[j] - '0';
        *dbl += c;
        j++;
    }
    size_t point_idx = j; // the point of first index, if exists
    j++;
    while (j < length) // add the numbers in the fraction part of number
    {
        int c = value[j] - '0';
        double d = c / pow(10, j - point_idx);
        *dbl += d;
        j++;
    }
    *dbl *= sgn;
}

/**
 *
 * @param value string representing a possible double number
 * @param length length of value
 * @return 0 if string representation is valid; -1 otherwise;
 */
int checkDouble(const char *value, size_t length)
{
    if (value[0] == '.')
    {
        return -1;
    }
    int point_counter = 0;
    int negative_counter = 0;
    for (size_t i = 0; i < length; ++i)
    {
        if (value[i] == '.')
        {
            point_counter++;
        }
        if (value[i] == '-')
        {
            negative_counter++;
        }
        if (i == 1 && value[0] == '.' && value[i] != '.')
        {
            return -1;
        }
        if ((value[i] > '9' || value[i] < '0') && value[i] != '.' && value[i] != '-')
        {
            return -1;
        }
    }
    if (point_counter > 1 || negative_counter > 1) // more than 1 decimal point, invalid signs
    {
        return -1;
    } // else valid double
    return 0;
}

// ---------- MAIN ----------

/**
 *
 * @param argc num of arguments
 * @param argv arguments vector
 * @return 0 if successful calculations were performed; EXIT_FAILURE value otherwise;
 */
int main(int argc, char *argv[])
{
    if (argc != MAX_ARGS_NUM)
    {
        fprintf(stderr, INVALID_ARGS_NUM);
        return EXIT_FAILURE;
    }
    int funcNum = stringToInteger(argv[1], strlen(argv[1]));
    if (funcNum < 1 || 6 < funcNum)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }

    RealFunction func = chooseFunction(funcNum);
    double a, b, x0, h;
    if (checkDouble(argv[2], strlen(argv[2])) == -1)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    stringToDouble(argv[2], strlen(argv[2]), &a);
    if (checkDouble(argv[3], strlen(argv[3])) == -1)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    stringToDouble(argv[3], strlen(argv[3]), &b);
    if (a >= b)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    // else, a<b, continue

    int n = stringToInteger(argv[4], strlen(argv[4]));
    if (n == -1)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    if (checkDouble(argv[5], strlen(argv[5])) == -1)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    stringToDouble(argv[5], strlen(argv[5]), &x0);

    if (checkDouble(argv[6], strlen(argv[6])) == -1)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }
    stringToDouble(argv[6], strlen(argv[6]), &h);
    if (h <= 0)
    {
        fprintf(stderr, INVALID_INPUT);
        return EXIT_FAILURE;
    }

    // by that point, all variables are valid
    printf("Integral: %0.5f\nDerivative: %0.5f",
           integration(func, a, b, n),
           derivative(func, x0, h));
}
