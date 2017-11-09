#include "../catch.hpp"
#include "test.h"

using namespace umof;

TEST_CASE("Method interface", "[method]")
{
    const Api api = Api::from<MTest>();
    Method method = api.method(0);

    REQUIRE(method.valid());
    REQUIRE(method.name() == "func1");
    REQUIRE(method.parameterCount() == 2);
    REQUIRE(method.parmaeterType(0) == Type::from<int>());
    REQUIRE(method.parmaeterType(1) == Type::from<float>());
    REQUIRE(method.returnType() == Type::from<double>());
    REQUIRE(method.signature() == "func1(int,float)");

    MTest test;
    double ret;
    REQUIRE(method.call(&test, ret, { 1, 2.0f }));
    REQUIRE(ret == 3.0);
}