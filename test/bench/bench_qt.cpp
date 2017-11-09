#include <celero/Celero.h>

#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>

class QtTest : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE int func(int a, int b)
	{
		return a + b;
	}
};

#include "bench_qt.moc"

class QtFixture : public celero::TestFixture
{
public:
    virtual void setUp(int64_t experimentValue)
    {
        const QMetaObject *meta = t.metaObject();
        m = meta->method(meta->indexOfMethod("func(int,int)"));
    }

public:
    QtTest t;
    QMetaMethod m;

};

BENCHMARK_F(FuncDefs, Qt, QtFixture, 100, 100000)
{
    int res;
    celero::DoNotOptimizeAway(m.invoke(&t, Q_RETURN_ARG(int, res), Q_ARG(int, 1), Q_ARG(int, 2)));
}
