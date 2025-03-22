//gcc_marco_test.cpp

#include <iostream>

#define MAR(__a__, __b__) \
    void \
    __a__##__b__ \
    (void) { \
        std::cout << "hello" << std::endl; }

#define OP(__a__, __b__) \
    std::ostream& operator __a__ (std::ostream& os, const __b__ & na) { \
        return os; }



MAR(foo,_test)

struct A
{
};

OP(<<, A)


int main(int argc, char* argv[])
{
    foo_test();

    A a;
    std::cout << a << std::endl;
    return 0;
}
