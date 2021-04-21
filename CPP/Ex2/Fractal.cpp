//
// Created by Ron on 04-Sep-19.
//
#include <iostream>
#include "Fractal.h"

/**
 * Overriding the what() exception.
 * @return exception message
 */
const char *InvalidInputException::what() const noexcept
{
    return "Invalid input";
}

/**
 * Overriding the what() exception.
 * @return exception message
 */
const char *InvalidMemoryException::what() const noexcept
{
    return "Memory allocation failed";

}

/**
 * Performs exponent function on ints.
 * @param base base of exponent
 * @param pow power to exponent
 * @return result
 */
int exponent(int base, int pow)
{
    int res = 1;
    for (int i = 0; i < pow; ++i)
    {
        res *= base;
    }
    return res;
}

/**
 * Draw function, pure virtual to enforce interface on abstract class.
 */
void Fractal::draw() const
{
    int size = exponent(_cellSize, _depth);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int tile = checkCell(i, j);
            if (tile == 0)
            {
                std::cout << ' ';
            }
            else
            {
                std::cout << '#';
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * Overrides the check cell function.
 * @param i row index
 * @param j column index
 * @return 1 if cell should be full; 0 otherwise.
 */
int SierpinskiFractal::checkCell(int i, int j) const
{
    while (i > 0 || j > 0)
    {
        if (i % _cellSize == 1 && j % _cellSize == 1)
        {
            return 0;
        }
        i /= _cellSize;
        j /= _cellSize;
    }
    return 1;
}

/**
 * Overrides the check cell function.
 * @param i row index
 * @param j column index
 * @return 1 if cell should be full; 0 otherwise.
 */
int CantorDust::checkCell(int i, int j) const
{
    while (i > 0 || j > 0)
    {
        if (i % _cellSize == 1 || j % _cellSize == 1)
        {
            return 0;
        }
        i /= _cellSize;
        j /= _cellSize;
    }
    return 1;
}

/**
 * static function implementing the factory design method, allocates memory and creates new
 * fractal.
 * @param fractalNum type of the fractal
 * @param fractalDepth depth of the fractal
 * @return pointer to allocated memory where fractal resides in
 */
Fractal *Fractal::createFractal(int fractalNum, int fractalDepth)
{
    try
    {
        switch (fractalNum)
        {
            case 1:
                return new SierpinskiCarpet(fractalDepth, 3);
            case 2:
                return new SierpinskiSieve(fractalDepth, 2);
            case 3:
                return new CantorDust(fractalDepth, 3);
            default:
                throw InvalidInputException{};
        }
    }
    catch (std::bad_alloc &exception)
    {
        throw InvalidMemoryException{};
    }
}
