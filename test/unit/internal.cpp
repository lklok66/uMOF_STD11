#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <umof.h>

#include <cstdarg>
#include <iostream>

using namespace umof::detail;

bool func1Called = false;
bool func2Called = false;
bool func3Called = false;

bool staticFunc1Called = false;
bool staticFunc2Called = false;
bool staticFunc3Called = false;

void void_func_no_args()
{
    func1Called = true;
}

void void_func_with_args(int a, float b)
{
    func2Called = true;
}

double return_func_with_args(int a, float b)
{
    func3Called = true;
    return a + b;
}

std::string func_with_vararg(const char *format, ...)
{
    char buffer[64];
    va_list argptr;
    va_start(argptr, format);
    std::vsprintf(buffer, format, argptr);
    va_end(argptr);
    return buffer;
}

class Test
{
    float _value;
public:
    static void void_method_no_args()
    {
        staticFunc1Called = true;
    }

    static void void_method_with_args(int a, float b)
    {
        staticFunc2Called = true;
    }

    static double return_method_with_args(int a, float b)
    {
        staticFunc3Called = true;
        return a + b;
    }

    static std::string static_method_with_vararg(const char *format, ...) {}
    void method_with_vararg(const char *format, ...) {}

    void method() {}
    void method_const() const {}
    void method_volatile() volatile {}
    void method_const_volatile() const volatile {}

    float& method_lvalue() & { return _value;  } //MSVC workaround
    void method_lvalue_const() const & {}
    void method_lvalue_volatile() volatile & {}
    void method_lvalue_const_volatile() const volatile & {}

    void method_rvalue() && {}
    void method_rvalue_const() const && {}
    void method_rvalue_volatile() volatile && {}
    void method_rvalue_const_volatile() const volatile && {}
};

TEST_CASE("Method argument count", "[MethodProps]")
{
    //C functions
    /*REQUIRE(MethodArguments<decltype(&void_func_no_args)>::count == 0);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::count == 2);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::count == 2);

    //Static methods
    REQUIRE(MethodArguments<decltype(&Test::void_method_no_args)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::void_method_with_args)>::count == 2);
    REQUIRE(MethodArguments<decltype(&Test::return_method_with_args)>::count == 2);

    //Function with varargs
    REQUIRE(MethodArguments<decltype(&func_with_vararg)>::count == 1);
    REQUIRE(MethodArguments<decltype(&Test::static_method_with_vararg)>::count == 1);
    REQUIRE(MethodArguments<decltype(&Test::method_with_vararg)>::count == 1);

    //Member functions
    REQUIRE(MethodArguments<decltype(&Test::method)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_const)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_volatile)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_const_volatile)>::count == 0);

    //Member functions lvalue ref
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue_const)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue_volatile)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue_const_volatile)>::count == 0);

    //Member functions rvalue ref
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue_const)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue_volatile)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue_const_volatile)>::count == 0);*/

    //REQUIRE(MethodArguments<float>::count == 0);
}

TEST_CASE("Method argument types", "[MethodProps]")
{
    REQUIRE(MethodArguments<decltype(&void_func_no_args)>::types()[0] == 0);

    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[0] == 0);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[1] == &Type<int>::table);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[2] == &Type<float>::table);

    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[0] == &Type<double>::table);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[1] == &Type<int>::table);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[2] == &Type<float>::table);
}

TEST_CASE("Free function defs, no params, no return", "[internal]")
{
    //Check types
    //REQUIRE(MethodArguments<decltype(&void_func_no_args)>::count == 0);
    REQUIRE(MethodArguments<decltype(&void_func_no_args)>::types()[0] == 0);

    //Check function call
    MethodCall<decltype(&void_func_no_args), &void_func_no_args>::call(nullptr, nullptr, nullptr);
    REQUIRE(func1Called);
}

TEST_CASE("Free function defs, has params, no return", "[internal]")
{
    //Check types
    //REQUIRE(MethodArguments<decltype(&void_func_with_args)>::count == 2);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[0] == 0);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[1] == &Type<int>::table);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[2] == &Type<float>::table);

    //Check function call
    int arg1 = 1;
    float arg2 = 2.0f;
    const void *params[] = { &arg1, &arg2 };
    MethodCall<decltype(&void_func_with_args), &void_func_with_args>::call(nullptr, nullptr, params);
    REQUIRE(func2Called);
}

TEST_CASE("Free function defs, has params, return", "[internal]")
{
    //Check types
    //REQUIRE(MethodArguments<decltype(&return_func_with_args)>::count == 2);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[0] == &Type<double>::table);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[1] == &Type<int>::table);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[2] == &Type<float>::table);

    //Check function call
    int arg1 = 1;
    float arg2 = 2.0f;
    double ret;
    const void *params[] = { &arg1, &arg2 };
    MethodCall<decltype(&return_func_with_args), &return_func_with_args>::call(nullptr, &ret, params);
    REQUIRE(func3Called);

    //Check return value
    REQUIRE(ret == 3.0);
}

TEST_CASE("Static function defs, no params, no return", "[internal]")
{
    //Check types
    //REQUIRE(MethodArguments<decltype(&Test::void_method_no_args)>::count == 0);
    REQUIRE(MethodArguments<decltype(&Test::void_method_no_args)>::types()[0] == 0);

    //Check function call
    MethodCall<decltype(&Test::void_method_no_args), &Test::void_method_no_args>::call(nullptr, nullptr, nullptr);
    REQUIRE(func1Called);
}

TEST_CASE("Static function defs, has params, no return", "[internal]")
{
    //Check types
    //REQUIRE(MethodArguments<decltype(&Test::void_method_with_args)>::count == 2);
    REQUIRE(MethodArguments<decltype(&Test::void_method_with_args)>::types()[0] == 0);
    REQUIRE(MethodArguments<decltype(&Test::void_method_with_args)>::types()[1] == &Type<int>::table);
    REQUIRE(MethodArguments<decltype(&Test::void_method_with_args)>::types()[2] == &Type<float>::table);

    //Check function call
    int arg1 = 1;
    float arg2 = 2.0f;
    const void *params[] = { &arg1, &arg2 };
    MethodCall<decltype(&Test::void_method_with_args), &Test::void_method_with_args>::call(nullptr, nullptr, params);
    REQUIRE(func2Called);
}

TEST_CASE("Static function defs, has params, return", "[internal]")
{
    //Check types
    //REQUIRE(MethodArguments<decltype(&Test::return_method_with_args)>::count == 2);
    REQUIRE(MethodArguments<decltype(&Test::return_method_with_args)>::types()[0] == &Type<double>::table);
    REQUIRE(MethodArguments<decltype(&Test::return_method_with_args)>::types()[1] == &Type<int>::table);
    REQUIRE(MethodArguments<decltype(&Test::return_method_with_args)>::types()[2] == &Type<float>::table);

    //Check function call
    int arg1 = 1;
    float arg2 = 2.0f;
    double ret;
    const void *params[] = { &arg1, &arg2 };
    MethodCall<decltype(&Test::return_method_with_args), &Test::return_method_with_args>::call(nullptr, &ret, params);
    REQUIRE(staticFunc3Called);

    //Check return value
    REQUIRE(ret == 3.0);
}