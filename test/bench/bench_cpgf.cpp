#include <celero/Celero.h>

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gscopedinterface.h"

class Object {};

class CpgfTest : public Object
{
public:
	int func(int a, int b)
	{
		return a + b;
	}
};

void declare_cpgf()
{
	using namespace cpgf;
	GDefineMetaClass<CpgfTest>
		::define("CpgfTest")
		._method("func", &CpgfTest::func);
}

class CpgfFixture : public celero::TestFixture
{
public:
	virtual void setUp(int64_t experimentValue)
	{
		declare_cpgf();
		metaClass = cpgf::findMetaClass("CpgfTest");
		metaFunc = metaClass->getMethod("func");
	}

public:
	CpgfTest t;
	const cpgf::GMetaClass *metaClass;
	const cpgf::GMetaMethod *metaFunc;

};

BENCHMARK_F(FuncDefs, Cpgf, CpgfFixture, 100, 100000)
{
	celero::DoNotOptimizeAway(cpgf::fromVariant<int>(metaFunc->invoke(&t, 1, 2)));
}
