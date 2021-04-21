//
// Created by Ron on 04-Sep-19.
//
#include <iostream>
#include "Fractal.h"
#include <fstream>
#include <deque>
#include <sstream>

/**
 * Frees all memory allocated by the fractals in the deque.
 * @param fractals deque of pointers to fractals
 */
void freeDeque(std::deque<Fractal *> &fractals)
{
    for (auto fractal: fractals)
    {
        delete fractal;
    }
}

/**
 * Drives the FractalDrawer program.
 * @param argc number of arguments
 * @param argv arguments vector
 * @return EXIT_FAILURE if error was encountered; 0 otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: FractalDrawer <file path>" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream inputFile(argv[1]);
    if (inputFile.fail())
    {
        inputFile.close();
        std::cerr << "Invalid input" << std::endl;
        return EXIT_FAILURE;
    }
    std::deque<Fractal *> fractals;
    std::string line;
    while (getline(inputFile, line))
    {
        if (line.length() != 3)
        {
            inputFile.close();
            freeDeque(fractals);
            std::cerr << "Invalid input" << std::endl;
            return EXIT_FAILURE;
        }
        for (char c : line)
        {
            if (c == ' ')
            {
                inputFile.close();
                freeDeque(fractals);
                std::cerr << "Invalid input" << std::endl;
                return EXIT_FAILURE;
            }
        }
        std::istringstream iss(line);
        int type, depth;
        char sep;
        if (!(iss >> type >> sep >> depth))
        {
            inputFile.close();
            freeDeque(fractals);
            std::cerr << "Invalid input" << std::endl;
            return EXIT_FAILURE;
        }
        if (type < 1 || type > 3 || sep != ',' || depth <= 0 || depth > 6)
        {
            inputFile.close();
            freeDeque(fractals);
            std::cerr << "Invalid input" << std::endl;
            return EXIT_FAILURE;
        }
        try
        {
            fractals.push_front(Fractal::createFractal(type, depth));
        }
        catch (InvalidMemoryException &exception)
        {
            std::cerr << exception.what() << std::endl;
        }
        catch (InvalidInputException &exception)
        {
            std::cerr << exception.what() << std::endl;
        }
    }
    for (auto fractal: fractals)
    {
        fractal->draw();
    }
    freeDeque(fractals);
    return 0;
}