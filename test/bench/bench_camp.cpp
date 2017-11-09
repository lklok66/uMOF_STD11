#include <celero/Celero.h>

#include <camp/camptype.hpp>
#include <camp/class.hpp>

class Object {};

class CampTest : public Object
{
public:
	int func(int a, int b)
	{
		return a + b;
	}
};

void declare_camp()
{
	camp::Class::declare<CampTest>("CampTest")
		.function("func", &CampTest::func);
}

CAMP_TYPE(CampTest)

class CampFixture : public celero::TestFixture
{
public:
	virtual void setUp(int64_t experimentValue)
	{
		declare_camp();
		metaClass = &camp::classByName("Test");
		metaFunc = &metaClass->function("func");
	}

public:
	CampTest t;
	const camp::Class *metaClass;
	const camp::Function *metaFunc;

};

BENCHMARK_F(FuncDefs, Camp, CampFixture, 100, 100000)
{
	celero::DoNotOptimizeAway(metaFunc->call(t, camp::Args(1, 2)).to<int>());
}
