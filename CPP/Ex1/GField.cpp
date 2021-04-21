//
// Created by Ron on 27-Aug-19.
//
#include "GField.h"
#include <cmath>
#include <cassert>

/**
* Init GField with p = 2, l = 1;
*/
GField::GField() : GField(2, 1)
{}

/**
 * Init GField with p and l = 1;
 * @param p char of field
 */
GField::GField(long p) : GField(p, 1)
{
}

/**
 * Initialize GField with p as char and order.
 * @param p char of field
 * @param l order of field
 */
GField::GField(long p, long l)
{
    assert(GField::isPrime(p));
    assert(l > 0);
    if (p < 0)
    {
        p *= -1;
    }
    _p = p;
    _l = l;
}

/**
 * A copy constructor.
 * @param init GField to copy values from.
 */
GField::GField(const GField &init) : _p(init._p), _l(init._l)
{}

/**
 * Destructor.
 */
GField::~GField() = default;

/**
 * Returns the char of the field.
 * @return char of field
 */
long GField::getChar() const
{
    return _p;
}

/**
 * Returns the degree of the field.
 * @return degree of field
 */
long GField::getDegree() const
{
    return _l;
}

/**
 * Returns the order of the field.
 * @return order of field
 */
long GField::getOrder() const
{
    return pow(_p, _l);
}

/**
 * A method to check if p is a prime number.
 * @param p number to check if prime.
 * @return true if p is prime; false otherwise.
 */
bool GField::isPrime(long p)
{
    if (p < 0) // negative number is prime if its absolute value is prime
    {
        return isPrime(-1 * p);
    }
    if (p == 1 || p == 0) // 0, 1 are not prime numbers
    {
        return false;
    }

    int i = 2;
    // O(sqrt(p))
    while (i <= sqrt(p))
    {
        // Check if i divides p
        if (p % i == 0)
        {
            // p has a factor in between 2 and sqrt(p), not a prime number
            return false;
        }
        i += 1;
    }
    // no factors, p is a prime number
    return true;
}

/**
 * Returns the GCD (greatest common divisor) of two GFNumber objects.
 * @param a GFNumber object
 * @param b GFNumber object
 * @return GCD of a,b
 */
GFNumber GField::gcd(GFNumber a, GFNumber b)
{
    assert(a.getField() == b.getField());
    if (b.getNumber() == 0)
    {
        return a;
    }
    GFNumber c(a.getNumber() % b.getNumber(), a.getField());
    return gcd(b, c);
}

/**
 * Creates a number with the given value from the current field.
 * @param k number to use as value for number
 * @return GFNumber object from the current field
 */
GFNumber GField::createNumber(long k)
{
    GFNumber num(k, *this);
    return num;
}

/**
 * Overloads the "=" operator for GField.
 * @param other GField to perform the operator on
 * @return reference to the result of the operator
 */
GField &GField::operator=(const GField &other)
{
    _p = other._p;
    _l = other._l;
    return *this;
}

/**
 * Overloads the "==" operator for GField.
 * @param other GField to perform the operator on
 * @return true if both field's orders are equal; false otherwise;
 */
bool GField::operator==(const GField &other) const
{
    return (this->getOrder() == other.getOrder());
}

/**
 * Overloads the "!=" operator for GField.
 * @param other GField to perform the operator on
 * @return true if both field's orders are inequal; false otherwise;
 */
bool GField::operator!=(const GField &other) const
{
    return (this->getOrder() != other.getOrder());
}

/**
 * Overloads the output operator for GField.
 * @param out ostream object
 * @param field field object
 * @return reference to ostream object for concatenation
 */
std::ostream &operator<<(std::ostream &out, const GField &field)
{
    out << "GF(" << field.getChar() << "**" << field.getDegree() << ")";
    return out;
}

/**
 * Overloads the input operator for GField.
 * Asserts input validation - p is prime, l is positive.
 * @param out istream object
 * @param field field object
 * @return reference to istream object for concatenation
 */
std::istream &operator>>(std::istream &in, GField &field)
{
    long p, l;
    in >> p >> l;
    assert(!in.fail());
    assert(l > 0);
    assert(GField::isPrime(p));
    if (p < 0)
    {
        p *= -1;
    }
    GField temp(p, l);
    field = temp;
    return in;
}










