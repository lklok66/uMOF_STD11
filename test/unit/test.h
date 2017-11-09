#pragma once

#include <umof.h>

class MTest
{
public:
    double func1(int a, float b)
    {
        return a + b;
    }

};

U_DECALRE_METHODS(MTest)
{
    U_METHOD(func1)
};
U_DECLARE_API(MTest)