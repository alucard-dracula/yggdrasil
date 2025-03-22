//limit_num_test.cpp

#include <iostream>
#include <guild/limited_num.hpp>
#include <map>
#include <stdlib.h>

#ifdef _WIN32
#   include <vld.h>
#endif //_WIN32

void test1(void)
{
    typedef std::map<int, int> imap_type;
    imap_type imap;
    for(int i = 0; i != 1000; ++i)
    {
        for(int j = 0; j != 3; ++j)
        {
            imap.insert(imap_type::value_type(i*3 + j, rand()% 3));
        }
    }

    for(imap_type::iterator i = imap.begin(), isize = imap.end(), rm; i != isize;)
    {
        if(i->second == 2)
        {
            rm = i;
            ++i;
            imap.erase(rm);
            std::cout << "erase" << std::endl;
            continue;
        }

        ++i;
    }

    std::cout << "end" << std::endl;
}

void test2(void)
{
    typedef game::guild::limited_num<int> lint_type;

    lint_type linit(10);
}

int main(int argc, char* argv[])
{
    test1();

    char cc = 0;
    std::cin >> cc;
    return 0;
}
