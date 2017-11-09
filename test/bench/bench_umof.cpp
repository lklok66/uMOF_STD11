#include <celero/Celero.h>

#include <object.h>

class UmofTest : public Object
{
	U_OBJECT(UmofTest, Object)
	U_EXPOSE(
		U_METHOD(UmofTest::func),
	)

public:
	int func(int a, int b)
	{
		return a + b;
	}
};

class UmofFixture : public celero::TestFixture
{
public:
    UmofFixture() : m(nullptr) {}

    virtual void setUp(int64_t experimentValue)
    {
        int idx = t.api()->indexOfMethod("func(int,int)");
        m = t.api()->method(idx);
    }

    virtual void tearDown()
    {
    }

public:
    UmofTest t;
    Method m;

};

BENCHMARK_F(FuncDefs, Umof, UmofFixture, 100, 100000)
{
    int res;
    m.invoke(&t, res, { 1, 2 });
    celero::DoNotOptimizeAway(res);
}
