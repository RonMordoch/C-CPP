//
// Created by Ron on 27-Aug-19.
//

#include <iostream>

#ifndef GField_H
#define GField_H

class GFNumber;

/**
* A class representing a galois field.
*/
class GField
{
public:
/**
* Init GField with p = 2, l = 1;
*/
    GField();

/**
 * Init GField with p and l = 1;
 * @param p char of field
 */
    GField(long p);

/**
 * Initialize GField with p as char and order.
 * @param p char of field
 * @param l order of field
 */
    GField(long p, long l);

/**
 * A copy constructor.
 * @param init GField to copy values from.
 */
    GField(const GField &init);

/**
 * Destructor.
 */
    ~GField();

/**
 * Returns the char of the field.
 * @return char of field
 */
    long getChar() const;

/**
 * Returns the degree of the field.
 * @return degree of field
 */
    long getDegree() const;

/**
 * Returns the order of the field.
 * @return order of field
 */
    long getOrder() const;

/**
 * A method to check if p is a prime number.
 * @param p number to check if prime.
 * @return true if p is prime; false otherwise.
 */
    static bool isPrime(long p);

/**
 * Returns the GCD (greatest common divisor) of two GFNumber objects.
 * @param a GFNumber object
 * @param b GFNumber object
 * @return GCD of a,b
 */
    GFNumber gcd(GFNumber a, GFNumber b);

/**
 * Creates a number with the given value from the current field.
 * @param k number to use as value for number
 * @return GFNumber object from the current field
 */
    GFNumber createNumber(long k);

/**
 * Overloads the "=" operator for GField.
 * @param other GField to perform the operator on
 * @return reference to the result of the operator
 */
    GField &operator=(const GField &other);

/**
 * Overloads the "==" operator for GField.
 * @param other GField to perform the operator on
 * @return true if both field's orders are equal; false otherwise;
 */
    bool operator==(const GField &other) const;

/**
 * Overloads the "!=" operator for GField.
 * @param other GField to perform the operator on
 * @return true if both field's orders are inequal; false otherwise;
 */
    bool operator!=(const GField &other) const;

/**
 * Overloads the output operator for GField.
 * @param out ostream object
 * @param field field object
 * @return reference to ostream object for concatenation
 */
    friend std::ostream &operator<<(std::ostream &out, const GField &field);

/**
 * Overloads the input operator for GField.
 * Asserts input validation - p is prime, l is positive.
 * @param out istream object
 * @param field field object
 * @return reference to istream object for concatenation
 */
    friend std::istream &operator>>(std::istream &in, GField &field);

private:
    long _p; // char of the field
    long _l; // degree of the field
};

#include "GFNumber.h"

#endif //GField_H
