#include <celero/Celero.h>

//Force noinline
#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define NOINLINE __attribute__ ((noinline))
#else
#define NOINLINE
#endif

class Object {};

class NativeTest : public Object
{
public:
	NOINLINE int func(int a, int b)
	{
		return a + b;
	}
};

class NativeFixture : public celero::TestFixture
{
public:
	NativeTest t;
};

BASELINE_F(FuncDefs, Native, NativeFixture, 100, 100000)
{
    celero::DoNotOptimizeAway(t.func(1, 2));
}
