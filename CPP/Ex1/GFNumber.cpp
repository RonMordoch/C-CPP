//
// Created by Ron on 27-Aug-19.
//

#include "GFNumber.h"
#include <cassert>
#include <cmath>

/**
 * Default, parameter-less constructor.
 * Initialize with n = 0, field = 2^1;
 */
GFNumber::GFNumber() : _n(0), _field(2)
{
}

/**
* Initialize with n as number and default field 2^1;
* @param n number to initalize
*/
GFNumber::GFNumber(long n) : _field(2)
{
    if (n < 0)
    {
        _negativeMod(n);
    }
    _n = n % _field.getOrder();
}

/**
 * Constructs object with given values.
 * @param n number value
 * @param field field value
 */
GFNumber::GFNumber(long n, const GField field)
{
    _field = field;
    if (n < 0)
    {
        _negativeMod(n);
    }
    _n = n % _field.getOrder();
}

/**
 * A copy constructor.
 * @param init GFNumber to copy values from.
 */
GFNumber::GFNumber(const GFNumber &init) : _n(init._n), _field(init._field)
{}


/**
 * Destructor.
 */
GFNumber::~GFNumber() = default;

/**
 * Changes a negative number to a postive number in relation with the field of the number.
 * @param n a negative long
 * @return the positive, field-adjusted value of n;
 */
long GFNumber::_negativeMod(long &n)
{
    while (n < 0)
    {
        n += _field.getOrder();
    }
    return n;
}


/**
 * Adds a prime factor to the factors array and dynamically increase the array.
 * @param factors array of GFNumber object
 * @param factorsSize array size
 * @param newFactor new GFNumber object which is a prime factor of the number
 */
void GFNumber::_addFactor(GFNumber *&factors, int *factorsSize, const GFNumber newFactor)
{
    int newSize = *factorsSize + 1;
    GFNumber *tempArray = new GFNumber[newSize];
    for (long j = 0; j < *factorsSize; ++j)
    {
        tempArray[j] = factors[j];
    }
    tempArray[*factorsSize] = newFactor;
    ++(*factorsSize);
    delete[] factors;
    factors = tempArray;
}


/**
 * Performs the Trail Division method on a GFNumber until all prime factors are found.
 * @param factors array of GFNumber object
 * @param factorsSize array size
 * @param num GFNumber to find divisors of
 */
void GFNumber::_directSearchFactorization(GFNumber *&factors, int *factorsSize, GFNumber &num)
{
    long i = 2;
    while (num._n >= i * i)
    {
        if (num._n % i == 0)
        {
            GFNumber newFactor(i, num._field);
            _addFactor(factors, factorsSize, newFactor);
            num._n = num._n / i;
        }
        else
        { i++; }
    }
    if (num._n > 1)
    {
        factors[*factorsSize] = num;
        ++(*factorsSize);
    }
}


/**
 * Performs the pollardRho algorithm on a GFNumber object.
 * @param num GFNumber object
 * @return long, a prime factor of GFNumber
 */
long GFNumber::_pollardRho(GFNumber num)
{
    GFNumber x((std::rand() % (num._n - 1) + 1), num._field);
    GFNumber y = x;
    GFNumber p(1, num._field);
    while (p._n == 1)
    {
        x._n = x._n * x._n + 1;
        y._n = x._n * x._n + 1;
        x._n %= num._field.getOrder();
        y._n %= num._field.getOrder();
        p = num._field.gcd((x - y), num);
    }
    if (p == num)
    {
        return -1;
    }
    return p._n;

}

/**
 * Returns a pointer to an array of prime factors of this GFNumber.
 * @param size pointer to size of prime factors array
 * @return pointer to an array of prime factors of this GFNumber
 */
GFNumber *GFNumber::getPrimeFactors(int *factorsSize)
{
    GFNumber *factors = new GFNumber[*factorsSize];
    if (this->getIsPrime())
    {
        return factors;
    }
    else
    {
        GFNumber temp = *this;
        while (temp._n % 2 == 0)
        {
            GFNumber newFactor(2, _field);
            _addFactor(factors, factorsSize, newFactor);
            temp._n /= 2;
        }
        if (temp.getIsPrime())
        {
            _addFactor(factors, factorsSize, temp);
            return factors;

        }
        if (temp._n > 1)
        {
            long factor = _pollardRho(temp);
            while (factor != -1)
            {
                GFNumber newFactor(factor, _field);
                _addFactor(factors, factorsSize, newFactor);
                temp._n = temp._n / factor;
                if (temp.getIsPrime())
                {
                    _addFactor(factors, factorsSize, temp);
                    return factors;
                }
                factor = _pollardRho(temp);

            }
            _directSearchFactorization(factors, factorsSize, temp);
        }

    }
    return factors;
}


/**
 * Prints the prime factors of the number.
 */
void GFNumber::printFactors()
{
    std::cout << this->getNumber() << "=";
    if (this->getIsPrime() || this->getNumber() == 0 || this->getNumber() == 1)
    {
        std::cout << this->getNumber() << "*" << 1 << std::endl;
        return;
    }

    int factorsSize = 0;
    GFNumber *factors = getPrimeFactors(&factorsSize);
    for (long i = 0; i < factorsSize; ++i)
    {
        if (i == factorsSize - 1) // last factor in array
        {
            std::cout << factors[i].getNumber() << std::endl;
            return;
        }
        std::cout << factors[i].getNumber() << "*";
    }
    delete[] factors;
}

/**
 * Return the number value.
 * @return number value
 */
long GFNumber::getNumber() const
{
    return _n;
}

/**
 * Return the field value.
 * @return field value, GField object
 */
GField GFNumber::getField() const
{
    return _field;
}

/**
 * @return true if number is prime; false otherwise;
 */
bool GFNumber::getIsPrime() const
{
    return GField::isPrime(_n);
}

/**
 * Overloads the "=" between 2 GFNumbers.
 * @param other GFNumber object
 * @return reference to the current GFNumber
 */
GFNumber &GFNumber::operator=(const GFNumber &other)
{
    if (this != &other)
    {
        _field = other._field;
        _n = other._n;
    }
    return *this;
}

/**
 * Overloads the "+" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a new GFNumber which is the result of the operator
 */
GFNumber GFNumber::operator+(const GFNumber &other) const
{
    assert(_field == other._field);
    GFNumber res(((_n + other._n) % _field.getOrder()), _field);
    return res;
}

/**
 * Overloads the "+" between GFNumber and long.
 * @param rparam long number
 * @return a new GFNumber which is the result of the operator
 */
GFNumber GFNumber::operator+(const long &rparam) const
{
    GFNumber res(((this->_n + rparam) % this->_field.getOrder()), this->_field);
    return res;
}


/**
 * Overloads the "+=" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
  */
GFNumber &GFNumber::operator+=(const GFNumber &other)
{
    assert(_field == other._field);
    _n = (_n + other._n) % _field.getOrder();

    return *this;
}

/**
 * Overloads the "+=" between GFNumber and long.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator+=(const long &rparam)
{
    this->_n = (this->_n + rparam) % this->_field.getOrder();
    if (this->_n < 0)
    {
        this->_negativeMod(this->_n);
    }
    return *this;
}

/**
 * Overloads the "-" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a new GFNumber which is the result of the operator
 */
GFNumber GFNumber::operator-(const GFNumber &other) const
{
    assert(_field == other._field);
    GFNumber res(((_n - other._n) % _field.getOrder()), _field);
    return res;

}

/**
 * Overloads the "-" between GFNumber and long.
 * @param rparam long number
 * @return a new GFNumber which is the result of the operator
 */
GFNumber GFNumber::operator-(const long &rparam) const
{
    GFNumber res(((this->_n - rparam) % this->_field.getOrder()), this->_field);
    return res;
}

/**
 * Overloads the "-=" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator-=(const GFNumber &other)
{
    assert(_field == other._field);
    _n = (_n - other._n) % _field.getOrder();
    return *this;
}

/**
 * Overloads the "-=" between GFNumber and long.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator-=(const long &rparam)
{
    this->_n = (this->_n - rparam) % this->_field.getOrder();
    if (this->_n < 0)
    {
        this->_negativeMod(this->_n);
    }
    return *this;
}

/**
 * Overloads the "*" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a new GFNumber which is the result of the operator
 */
GFNumber GFNumber::operator*(const GFNumber &other) const
{
    assert(_field == other._field);
    GFNumber res(((_n * other._n) % _field.getOrder()), _field);
    return res;
}

/**
 * Overloads the "*" between GFNumber and long.
 * @param rparam long number
 * @return a new GFNumber which is the result of the operator
*/
GFNumber GFNumber::operator*(const long &rparam) const
{
    GFNumber res(((this->_n * rparam) % this->_field.getOrder()), this->_field);
    return res;
}

/**
 * Overloads the "*=" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator*=(const GFNumber &other)
{
    assert(_field == other._field);
    _n = (_n * other._n) % _field.getOrder();
    return *this;
}

/**
 * Overloads the "*=" between GFNumber and long.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator*=(const long &rparam)
{
    this->_n = (this->_n * rparam) % this->_field.getOrder();
    if (this->_n < 0)
    {
        this->_negativeMod(this->_n);
    }
    return *this;
}


/**
 * Overloads the "%" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a new GFNumber which is the result of the operator
 */
GFNumber GFNumber::operator%(const GFNumber &other) const
{
    assert(other._n != 0);
    assert(_field == other._field);
    GFNumber res(((this->_n % other._n) % this->_field.getOrder()), this->_field);
    return *this;

}

/**
 * Overloads the "%" between GFNumber and long.
 * @param rparam long number
 * @return a new GFNumber which is the result of the operator
*/
GFNumber GFNumber::operator%(const long &rparam) const
{
    assert(rparam != 0);
    GFNumber res(((this->_n % rparam) % this->_field.getOrder()), this->_field);
    return res;
}

/**
 * Overloads the "%=" between 2 GFNumbers.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator%=(const GFNumber &other)
{
    assert(other._n != 0);
    assert(_field == other._field);
    _n = (_n % other._n) % _field.getOrder();
    return *this;
}

/**
 * Overloads the "%=" between GFNumber and long.
 * @param other GFNumber object
 * @return a reference to the current GFNumber
 */
GFNumber &GFNumber::operator%=(const long &rparam)
{
    assert(rparam != 0);
    this->_n = (this->_n % rparam) % this->_field.getOrder();
    if (this->_n < 0)
    {
        this->_negativeMod(this->_n);
    }
    return *this;
}

/**
 * Overloads the "==" operator between 2 GFNumbers.
 * @param other other GFNumber object
 * @return true if both are equal; false otherwise;
 */
bool GFNumber::operator==(const GFNumber &other) const
{
    return ((_n == other._n) && (_field.getChar() == other._field.getChar()));
}

/**
 * Overloads the "!=" operator between 2 GFNumbers.
 * @param other other GFNumber object
 * @return true if both are inequal; false otherwise;
 */
bool GFNumber::operator!=(const GFNumber &other) const
{
    return ((_n != other._n) || (_field.getChar() != other._field.getChar()));
}

/**
 * Overloads the "<" operator between 2 GFNumbers.
 * @param other other GFNumber object
 * @return true if left is < than right; false otherwise;
 */
bool GFNumber::operator<(const GFNumber &other) const
{
    assert(_field == other._field);
    return (_n < other._n);
}

/**
 * Overloads the "<=" operator between 2 GFNumbers.
 * @param other other GFNumber object
 * @return true if left is <= than right; false otherwise;
 */
bool GFNumber::operator<=(const GFNumber &other) const
{
    assert(_field == other._field);
    return (_n <= other._n);

}

/**
 * Overloads the ">" operator between 2 GFNumbers.
 * @param other other GFNumber object
 * @return true if left is > than right; false otherwise;
 */
bool GFNumber::operator>(const GFNumber &other) const
{
    assert(_field == other._field);
    return (_n > other._n);

}

/**
* Overloads the ">=" operator between 2 GFNumbers.
* @param other other GFNumber object
* @return true if left is >= than right; false otherwise;
*/
bool GFNumber::operator>=(const GFNumber &other) const
{
    assert(_field == other._field);
    return (_n >= other._n);


}

/**
 * Overloads the output operator.
 * @param out ostream object
 * @param number GFNumber object
 * @return reference to ostream object for concatenation
 */
std::ostream &operator<<(std::ostream &out, const GFNumber &number)
{
    out << number._n << " GF(" << number._field.getChar() << "**"
        << number._field.getDegree()
        << ")";
    return out;
}

/**
 * Overloads the input operator.
 * @param out istream object
 * @param number GFNumber object
 * @return reference to istream object for concatenation
 */
std::istream &operator>>(std::istream &in, GFNumber &number)
{
    long n;
    GField field;
    in >> n >> field;
    (assert(!in.fail()));
    GFNumber temp(n, field);
    number = temp;
    return in;
}
