//fedora_linux_64_test.cpp

#ifndef LINUX
#   error "this file use linux only!!"
#endif //LINUX


#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << sizeof(int*) << std::endl;
    std::cout << sizeof(char*) << std::endl;

    std::cout << sizeof(int) << std::endl;

    std::cout << sizeof(long) << std::endl;
    std::cout << sizeof(long long) << std::endl;
    return 0;
}
