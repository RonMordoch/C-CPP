//
// Created by Ron on 29-Aug-19.
//
#include <iostream>
#include "GField.h"
#include "GFNumber.h"
#include <cassert>

/**
 * Receives 2 lines of input into GFNumber objects, prints their sum, difference between them,
 * their product and their prime factors.
 * @return 0
 */
int main()
{
    GFNumber a, b;
    std::cin >> a;
    assert(!std::cin.fail());
    std::cin >> b;
    assert(!std::cin.fail());
    assert(a.getField() == b.getField());
    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
    std::cout << b - a << std::endl;
    std::cout << a * b << std::endl;
    a.printFactors();
    b.printFactors();

    return 0;


}


