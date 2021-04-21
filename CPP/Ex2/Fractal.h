//
// Created by Ron on 04-Sep-19.
//

#ifndef EX2_FRACTAL_H
#define EX2_FRACTAL_H

#include <exception>

/**
 * An abstract class for a general fractal, implementing the rule of five.
 */
class Fractal
{
protected:
    int _depth; // depth or height of fractal
    int _cellSize; // cell size of fractal
public:
    /**
     * Constructor which receives the depth and cell size of fractal.
     * @param depth depth of the fractal
     * @param cellSize size of each recursive cell
     */
    explicit Fractal(int depth, int cellSize) : _depth(depth), _cellSize(cellSize)
    {}

    /**
     * Draw function, pure virtual to enforce interface on abstract class.
     */
    virtual void draw() const = 0;

    /**
     * Checks if the current cell should be empty or full.
     * @param i row index
     * @param j column index
     * @return 1 if cell should be full; 0 otherwise.
     */
    virtual int checkCell(int i, int j) const = 0;

    /**
     * static function implementing the factory design method, allocates memory and creates new
     * fractal.
     * @param fractalNum type of the fractal
     * @param fractalDepth depth of the fractal
     * @return pointer to allocated memory where fractal resides in
     */
    static Fractal *createFractal(int fractalNum, int fractalDepth);

    /**
     * Virtual default destructor
     */
    virtual ~Fractal() = default;

    /**
     * Default copy constructor
     * @param other fractal to copy from
     */
    Fractal(const Fractal &other) = default;

    /**
     * Default copy assignment operator.
     * @param other fractal to copy from
     * @return reference to this
     */
    Fractal &operator=(const Fractal &other) = default;

    /**
     * Default move constructor.
     * @param other fractal to move from
    */
    Fractal(Fractal && other) noexcept = default;

    /**
     * Default move assignment operator.
     * @param other fractal to move from
     * @return reference to this
     */
    Fractal &operator=(Fractal && other) = default;
};

/**
 * Abstract class representing a Sierpinski sieve fractal.
 */
class SierpinskiFractal : public Fractal
{
public:
    /**
     * Constructor which receives the depth and cell size of fractal.
     * @param depth depth of the fractal
     * @param cellSize size of each recursive cell
     */
    SierpinskiFractal(int depth, int cellSize) : Fractal(depth, cellSize)
    {}

    /**
     * Overrides the check cell function.
     * @param i row index
     * @param j column index
     * @return 1 if cell should be full; 0 otherwise.
     */
    int checkCell(int i, int j) const override;
};

/**
 * A class representing Sierpinski Carpet.
 */
class SierpinskiCarpet : public SierpinskiFractal
{
public:
    /**
     * Constructor for Sierpinski Carpet fractal.
     * @param depth depth of fractal
     * @param cellSize cell size of fractal
     */
    explicit SierpinskiCarpet(int depth, int cellSize) : SierpinskiFractal(depth, cellSize)
    {}

    /**
     * Overrides the draw method for Sierpinski Carpet.
     */
    void draw() const override
    { SierpinskiFractal::draw(); }
};

/**
 * A class representing Sierpinski Sieve.
 */
class SierpinskiSieve : public SierpinskiFractal
{
public:
    /**
     * Constructor for Sierpinski Sieve fractal.
     * @param depth depth of fractal
     * @param cellSize cell size of fractal
     */
    explicit SierpinskiSieve(int depth, int cellSize) : SierpinskiFractal(depth, cellSize)
    {}

    /**
     * Overrides the draw method for Sierpinski Sieve.
    */
    void draw() const override
    { SierpinskiFractal::draw(); }
};

/**
 * A class representing Cantor Dust.
 */
class CantorDust : public Fractal
{
public:
    /**
     * Constructor for Cantor Dust fractal.
     * @param depth depth of fractal
     */
    explicit CantorDust(int depth, int cellSize) : Fractal(depth, cellSize)
    {}

    /**
    * Overrides the draw method for Cantor Dust.
     */
    void draw() const override
    {
        Fractal::draw();
    }

    /**
     * Overrides the check cell function.
     * @param i row index
     * @param j column index
     * @return 1 if cell should be full; 0 otherwise.
     */
    int checkCell(int i, int j) const override;
};

/**
 * A class representing exception thrown due to invalid input.
 */
class InvalidInputException : public std::exception
{
public:
    /**
     * Overriding the what() exception.
     * @return exception message
     */
    const char *what() const noexcept override;
};

/**
 * A class representing exception thrown due to memory error.
 */
class InvalidMemoryException : public std::exception
{
public:
    /**
     * Overriding the what() exception.
     * @return exception message
     */
    const char *what() const noexcept override;
};

#endif //EX2_FRACTAL_H

