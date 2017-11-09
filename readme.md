The uMOF library is a cross platform C++ meta system. It can be used for reflection and script binding. It's written in C++11 and require modern compiler.

## Build
[![Build Status](https://semaphoreci.com/api/v1/occash/umof/branches/master/badge.svg)](https://semaphoreci.com/occash/umof)

The library uses [premake4](http://industriousone.com/premake-quick-start) as the build system. To build uMOF, in command line navigate into uMOF folder, then:

	premake4 action

See premake4 documentation for full list of supported actions. The project files will be created in ```uMOF/build``` folder. After compiling the project, the library
file will be in ```uMOF/lib```.

### Options
 * ```--static``` Build static library instead of shared
 * ```--tests``` Build unit tests
 * ```--benchmark``` Build benchmark
 * ```--docs```  Build doxygen documentation

## Usage
To use meta system you should declare method and properties of class using macros. 

```
class Test
{
public:
    Test() = default;

    int func(int a, int b)
    {
        return a + b;
    }

    float func(float a, float b)
    {
        return a + b;
	}

    int null()
    {
        return 0;
    }

    void test()
    {
        std::cout << "test" << std::endl;
    }
	
    int getVal() const
    {
        return _val;
    }

    void setVal(int v)
    {
        _val = v;
    }

    double prop;
    
private:
    int _val;

};

U_DECALRE_METHODS(Test)
{
    U_OVERLOAD(func, Test, int, int, int),
    U_OVERLOAD(func, Test, float, float, float),
    U_METHOD(null),
    U_METHOD(test)
};
U_PROPERTIES(Test)
{
    U_PROPERTY(val, READ(getVal), WRITE(setVal))
    U_PROPERTY(prop, MEMBER(prop))
}
U_DECLARE_API(Test)
```
Now you can use exposed api of the class.
```
const Api api = Api::from<Test>();
Method method = api.method(0);

Test t;
int ret;
m.call(&t, ret, {1, 2});

```

## Benchmark

Configuration: Windows, Visual Studio 2013, Release x86

Framework | Compile/Link time, ms | Executable size, KB | Call time spent*, ms
-----|------|-----|-----
Native | 371/63 | 12 | 2 (45**)
uMOF | 406/78 | 18 | 349
Camp | 4492/116 | 66 | 6889
Qt | 1040/80 (+129***) | 15 | 498
cpgf | 2514/166 | 71 | 1184

\* 10000000 calls

\*\* Force noinline

\*\*\* Meta object compiler