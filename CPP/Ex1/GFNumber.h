//
// Created by Ron on 27-Aug-19.
//

#ifndef GFNumber_H
#define GFNumber_H

#include "GField.h"

/**
 * A class representing a number in a GField field.
 */
class GFNumber
{
public:

    /**
     * Default, parameter-less constructor.
     */
    GFNumber();

    /**
    * Initialize with n as number and default field.
    * @param n
    */
    GFNumber(long n);

    /**
     * Constructs object with given values.
     * @param n number value
     * @param field field value
     */
    GFNumber(long n, GField field);

    /**
    * A copy constructor.
    * @param init GFNumber to copy values from.
    */
    GFNumber(const GFNumber &init);

    /**
     * Destructor.
     */
    ~GFNumber();

    /**
     * Return the number value.
     * @return number value
     */
    long getNumber() const;

    /**
     * Return the field value.
     * @return field value, GField object
     */
    GField getField() const;

    /**
     * Returns a pointer to an array of prime factors of this GFNumber.
     * @param size pointer to size of prime factors array
     * @return pointer to an array of prime factors of this GFNumber
     */
    GFNumber *getPrimeFactors(int *size);

    /**
     * Prints the prime factors of the number.
     */
    void printFactors();


    /**
     * @return true if number is prime; false otherwise;
     */
    bool getIsPrime() const;

    /**
     * Overloads the "=" between 2 GFNumbers.
     * @param other GFNumber object
     * @return reference to the current GFNumber
     */
    GFNumber &operator=(const GFNumber &other);

    /**
     * Overloads the "+" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a new GFNumber which is the result of the operator
     */
    GFNumber operator+(const GFNumber &other) const;

    /**
     * Overloads the "+" between GFNumber and long.
     * @param rparam long number
     * @return a new GFNumber which is the result of the operator
     */
    GFNumber operator+(const long &rparam) const;

    /**
     * Overloads the "+=" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator+=(const GFNumber &other);

    /**
     * Overloads the "+=" between GFNumber and long.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator+=(const long &rparam);

    /**
     * Overloads the "-" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a new GFNumber which is the result of the operator
     */
    GFNumber operator-(const GFNumber &other) const;

    /**
     * Overloads the "-" between GFNumber and long.
     * @param rparam long number
     * @return a new GFNumber which is the result of the operator
    */
    GFNumber operator-(const long &rparam) const;

    /**
     * Overloads the "-=" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator-=(const GFNumber &other);

    /**
     * Overloads the "-=" between GFNumber and long.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator-=(const long &rparam);

    /**
     * Overloads the "*" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a new GFNumber which is the result of the operator
     */
    GFNumber operator*(const GFNumber &other) const;

    /**
     * Overloads the "*" between GFNumber and long.
     * @param rparam long number
     * @return a new GFNumber which is the result of the operator
    */
    GFNumber operator*(const long &rparam) const;

    /**
     * Overloads the "*=" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator*=(const GFNumber &other);

    /**
     * Overloads the "*=" between GFNumber and long.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator*=(const long &rparam);

    /**
     * Overloads the "%" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a new GFNumber which is the result of the operator
     */
    GFNumber operator%(const GFNumber &other) const;

    /**
     * Overloads the "%" between GFNumber and long.
     * @param rparam long number
     * @return a new GFNumber which is the result of the operator
    */
    GFNumber operator%(const long &rparam) const;

    /**
     * Overloads the "%=" between 2 GFNumbers.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator%=(const GFNumber &other);

    /**
     * Overloads the "%=" between GFNumber and long.
     * @param other GFNumber object
     * @return a reference to the current GFNumber
     */
    GFNumber &operator%=(const long &rparam);

    /**
     * Overloads the "==" operator between 2 GFNumbers.
     * @param other other GFNumber object
     * @return true if both are equal; false otherwise;
     */
    bool operator==(const GFNumber &other) const;

    /**
     * Overloads the "!=" operator between 2 GFNumbers.
     * @param other other GFNumber object
     * @return true if both are inequal; false otherwise;
     */
    bool operator!=(const GFNumber &other) const;

    /**
     * Overloads the "<" operator between 2 GFNumbers.
     * @param other other GFNumber object
     * @return true if left is < than right; false otherwise;
     */
    bool operator<(const GFNumber &other) const;

    /**
     * Overloads the "<=" operator between 2 GFNumbers.
     * @param other other GFNumber object
     * @return true if left is <= than right; false otherwise;
     */
    bool operator<=(const GFNumber &other) const;

    /**
     * Overloads the ">" operator between 2 GFNumbers.
     * @param other other GFNumber object
     * @return true if left is > than right; false otherwise;
     */
    bool operator>(const GFNumber &other) const;

    /**
    * Overloads the ">=" operator between 2 GFNumbers.
    * @param other other GFNumber object
    * @return true if left is >= than right; false otherwise;
    */
    bool operator>=(const GFNumber &other) const;

    /**
     * Overloads the output operator.
     * @param out ostream object
     * @param number GFNumber object
     * @return reference to ostream object for concatenation
     */
    friend std::ostream &operator<<(std::ostream &out, const GFNumber &number);

    /**
     * Overloads the input operator.
     * @param out istream object
     * @param number GFNumber object
     * @return reference to istream object for concatenation
     */
    friend std::istream &operator>>(std::istream &in, GFNumber &number);


private:
    long _n; // long value of number
    GField _field; // field of number


/**
 * Changes a negative number to a postive number in relation with the field of the number.
 * @param n a negative long
 * @return the positive, field-adjusted value of n;
 */
    long _negativeMod(long &n);

    /**
     * Performs the pollardRho algorithm on a GFNumber object.
     * @param num GFNumber object
     * @return long, a prime factor of GFNumber
     */
    long _pollardRho(GFNumber num);


    /**
     * Performs the Trail Division method on a GFNumber until all prime factors are found.
     * @param factors array of GFNumber object
     * @param factorsSize array size
     * @param num GFNumber to find divisors of
     */
    void _directSearchFactorization(GFNumber *&factors, int *factorsSize, GFNumber &num);

    /**
     * Adds a prime factor to the factors array and dynamically increase the array.
     * @param factors array of GFNumber object
     * @param factorsSize array size
     * @param newFactor new GFNumber object which is a prime factor of the number
     */
    void _addFactor(GFNumber *&factors, int *factorsSize, const GFNumber newFactor);


};

#endif //GFNumber_H
