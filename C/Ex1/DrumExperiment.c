//
// Created by Ron on 31-Jul-19.
//
// -------------------------------------- INCLUDES --------------------------------------
#include <stdio.h>
#include <math.h> // Imported for pow(), floor(), ceil()
#include <stdlib.h> // Imported for "EXIT_FAILURE" and "EXIT_SUCCESS"
#include <string.h> // Imported for "strtok"

// ---------------------------------- CONST DEFINITIONS ---------------------------------
/**
 * @brief Defines the maximum line length.
 */
#define LINE_MAX_SIZE 1024
/**
 * @brief The delimiters used to separate the input
 */
#define INPUT_DELIMS " \t\r\n"
/**
 * @brief the backslash-zero character
 */
#define BACKSLASH_0 '\0'
/**
 * @brief the newline character
 */
#define NEW_LINE '\n'
/**
 * @brief maximum length of array per exercise instructions
 */
#define MAX_ARRAY_LENGTH 100
/**
 * @brief first index in array of size 100
 */
#define FIRST_IDX 0
/**
 * @brief last index in array of size 100
 */
#define LAST_IDX 99
/**
 * @brief maximum length of number per exercise instructions
 */
#define MAX_NUM_LENGTH 9
/**
 * @brief the zero character
 */
#define ZERO_CHAR '0'
/**
 * @brief minus 1 for representing errors ( implementation dependent )
 */
#define MINUS -1
/**
 * @brief the nine character
 */
#define NINE_CHAR '9'
/**
 * @brief the single point character
 */
#define POINT_CHAR '.'
/**
 * @brief the minus sign character
 */
#define MINUS_CHAR '-'
/**
 * @brief the value used for rounding numbers
 */
#define ROUND 1000
/**
 * @brief maximum number of stars
 */
#define MAX 20
/**
 * @brief the limit of max{r}
 */
#define EPSILON 0.00000001

// -------------------------------------- FUNCTIONS -------------------------------------

/**
 * Converts a string representing a valid number into a double.
 * @param value string representing a valid number
 * @param length length of string
 * @return valid double representation of number
 */
double stringToDouble(const char value[], size_t length)
{
    double dbl = 0;
    size_t j = 0;
    while ((j < length) && (value[j] != '.')) // add the numbers in the integer part of numbers
    {
        dbl *= 10;
        int c = value[j] - ZERO_CHAR;
        dbl += c;
        j++;
    }
    size_t point_idx = j; // the point of first index, if exists
    j++;
    while (j < length) // add the numbers in the fraction part of number
    {
        int c = value[j] - ZERO_CHAR;
        double d = c / pow(10, j - point_idx);
        dbl += d;
        j++;
    }
    return dbl;
}

/**
 * Validates a line of input from user, and if valid, return the number of elements found.
 * @param line a string representing a line of input from user
 * @param array array of doubles to put the converted valid values
 * @return -1 is line is invalid, else the number of elements in line
 */
int checkLine(char line[], double array[])
{
    char *value = NULL;
    int counter = 0; // element count
    value = strtok(line, INPUT_DELIMS);

    while (value != NULL)
    {
        size_t length = strlen(value);
        if ((length > MAX_NUM_LENGTH) || (value[0] == POINT_CHAR) || (value[0] == MINUS_CHAR))
        {
            return MINUS;
        }
        int point_counter = 0;
        for (size_t i = 0; i < length; ++i)
        {
            if (value[i] == POINT_CHAR)
            {
                point_counter++;
            }
            if (i == 1 && value[0] == ZERO_CHAR && value[i] != POINT_CHAR)
            {
                return MINUS;
            }
            if ((value[i] > NINE_CHAR || value[i] < ZERO_CHAR) && value[i] != POINT_CHAR)
            {
                return MINUS;
            }
        }
        if (point_counter > 1) // more than 1 fraction point, invalid value
        {
            return MINUS;
        } // else, convert to double and add to array
        double dbl = stringToDouble(value, length);
        array[counter] = dbl;
        counter++; // increase element count
        if (counter > MAX_ARRAY_LENGTH) // more than 100 values, invalid array
        {
            return MINUS;
        }
        value = strtok(NULL, INPUT_DELIMS);
    }
    return counter;
}

/**
 * Checks if array of chars is representing a valid non-negative integer.
 * @param num array of chars
 * @param length length of num[]
 * @return -1 is the number is not a non-negative integer, else the number
 */
int checkInteger(char num[], size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        if (num[i] > NINE_CHAR || num[i] < ZERO_CHAR)
        {
            return MINUS;
        }
    }
    // number is definitely only digits and positive,
    // return the integer part of the double representation
    return (int) stringToDouble(num, length);
}

/**
 * Returns the sum of all elements in an array of double.
 * @param arr array of doubles
 * @param length length of arr[]
 * @return sum of all elements in array.
 */
double getArraySum(double const arr[], int length)
{
    double sum = 0;
    for (int i = 0; i < length; ++i)
    {
        sum += arr[i];
    }
    return sum;
}

/**
 * Normalizes an array of doubles using the L1 norm.
 * @param arr array of doubles
 * @param length length of arr[]
 */
void normalizeArray(double arr[], int length)
{
    double sum = getArraySum(arr, length);
    for (int i = 0; i < length; ++i)
    {
        arr[i] = arr[i] / sum;
    }
}

/**
 * Performs a single convolution.
 * @param g input array of doubles
 * @param h input array of double
 * @param r output array of doubles
 */
void convolution(double const g[], double const h[], double r[])
{
    int floor_value = floor(MAX_ARRAY_LENGTH / 2);
    int ceil_value = ceil(MAX_ARRAY_LENGTH / 2);
    for (int t = -(ceil_value + 1); t < (floor_value - 1); ++t)
    {
        double sum_t = 0;
        for (int m = -(ceil_value); m < (floor_value); ++m) //
        {
            if ((t - m + floor_value >= FIRST_IDX) && (t - m + floor_value <= LAST_IDX)
                && (m + floor_value >= FIRST_IDX) && (m + floor_value <= LAST_IDX))
            {
                sum_t += g[t - m + floor_value] * h[m + floor_value];
            }
        }
        r[t + (MAX_ARRAY_LENGTH / 2 + 1)] = sum_t;
    }
}

/**
 * Performs n-convolutions.
 * @param g input array of doubles
 * @param h input array of double
 * @param r output array of doubles
 * @param n number of convolutions to perform.
 */
void nConvolution(double g[], double h[], double r[], int n)
{
    if (n == 0)
    {
        for (int i = 0; i < MAX_ARRAY_LENGTH; ++i)
        {
            r[i] = g[i];
        }
    }
    else
    {
        for (int j = 0; j < n; ++j)
        {
            convolution(g, h, r);
            normalizeArray(r, MAX_ARRAY_LENGTH);
            for (int i = 0; i < MAX_ARRAY_LENGTH; ++i)
            {
                g[i] = r[i];
            }
        }
    }
}

/**
 * Returns the middle index of an array in relation to the maximum array length.
 * @param numOfElements number of elements in array
 * @return  middle index of array in relation to the maximum array length
 */
int getMiddleIdx(int numOfElements)
{
    return ((MAX_ARRAY_LENGTH - numOfElements) / 2);
}

/**
 * Centralizes the values of a given input array into the given output array.
 * @param arrayLength length of input array
 * @param middleIdx middle index of input array
 * @param inputArray input array of double to centralize
 * @param outputArray output array, the centralized array
 */
void centralizeArray(int arrayLength, int middleIdx, double const inputArray[],
                     double outputArray[])
{
    for (int i = 0; i < arrayLength; ++i)
    {
        outputArray[middleIdx + i] = inputArray[i];
    }
}

/**
 * Round all numbers in a given array to 3 digits after fraction point.
 * @param arr array of numbers to round
 */
void roundNumbers(double arr[])
{
    for (int i = 0; i < MAX_ARRAY_LENGTH; ++i)
    {
        arr[i] = (round(ROUND * arr[i])) / ROUND;
    }
}

/**
 * Gets the maximum value of a given array.
 * @param arr array to extract max out of
 * @return maximum value of array
 */
double getMax(double const arr[])
{
    double max = 0;
    for (int i = 0; i < MAX_ARRAY_LENGTH; ++i)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

/**
 * Prints the result of n-convolutions.
 * @param arr array containing the results of n-convolutions
 * @param max maximum number of stars
 */
void printResults(double arr[], double max)
{
    for (int i = 0; i < MAX_ARRAY_LENGTH; ++i)
    {
        int numOfStars = floor(MAX * (arr[i] / max));
        printf("%0.3f" "%s", arr[i], ": ");
        for (int j = 0; j < numOfStars; ++j)
        {
            printf("%c", '*');
        }
        printf("\n");
    }
}

/**
 * Returns the length of the 3rd input line, number of convolutions.
 * @param arr arr containing the input
 * @param arrLength strlen of input
 * @return the valid length of input excluding special characters
 */
size_t getLength(char arr[], size_t arrLength)
{
    size_t length = 0;
    if (arr[arrLength - 1] == NEW_LINE)
    {
        arr[arrLength] = BACKSLASH_0;
        length = arrLength - 1;
    }
    else
    {
        length = arrLength;
    }
    return length;
}

int main()
{
    char line_g[LINE_MAX_SIZE] = {'\0'};
    char line_h[LINE_MAX_SIZE] = {'\0'};
    char n[LINE_MAX_SIZE] = {'\0'}; // number of rooms

    // Initialize the g,h, arrays
    double array_g[MAX_ARRAY_LENGTH] = {0};
    double array_h[MAX_ARRAY_LENGTH] = {0};

    // Read lines
    if (fgets(line_g, LINE_MAX_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }
    if (fgets(line_h, LINE_MAX_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }
    // Initialize number of elements in each line
    int g_elements = 0;
    int h_elements = 0;
    // Get number of elements
    g_elements = checkLine(line_g, array_g);
    h_elements = checkLine(line_h, array_h);
    // Check validity of number of elements
    if (g_elements == MINUS || h_elements == MINUS || g_elements < h_elements)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }
    // Read 3rd line of input - number of convolutions
    if (fgets(n, LINE_MAX_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }
    // Extract the number from the line
    size_t length = getLength(n, strlen(n));
    int num = checkInteger(n, length);
    // Check for validity of number
    if (num == MINUS)
    {
        fprintf(stderr, "ERROR\n");
        return EXIT_FAILURE;
    }
    // Initialize the centered g,h, arrays
    double center_array_g[MAX_ARRAY_LENGTH] = {0};
    double center_array_h[MAX_ARRAY_LENGTH] = {0};
    // Get middle index of both arrays
    int g_mid_idx = getMiddleIdx(g_elements);
    int h_mid_idx = getMiddleIdx(h_elements);
    // Centralize arrays into the center arrays
    centralizeArray(g_elements, g_mid_idx, array_g, center_array_g);
    centralizeArray(h_elements, h_mid_idx, array_h, center_array_h);
    // Normalize both arrays
    normalizeArray(center_array_g, MAX_ARRAY_LENGTH);
    normalizeArray(center_array_h, MAX_ARRAY_LENGTH);
    // Initialize the results array
    double r[MAX_ARRAY_LENGTH] = {0};
    // Perform n-convolutions
    nConvolution(center_array_g, center_array_h, r, num);
    // Round number, get max and check for validity
    roundNumbers(r);
    double max = getMax(r);
    if (max <= EPSILON)
    {
        return EXIT_FAILURE;
    }
    // If max is valid, print results, exit program
    printResults(r, max);
    return 0;
}
