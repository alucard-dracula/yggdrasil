//idn_test.cpp

#include <iostream>

#include <yggr/ids/id_n.hpp>
#include <yggr/ids/id_generator.hpp>

#define LEN 8
typedef yggr::ids::id_n<LEN> id_type;
typedef yggr::ids::id_generator<id_type> id_gen_type;
int main(int argc, char* argv[])
{
	id_gen_type gen;
	id_type id1 = gen();
	std::cout << id1 << std::endl;
	assert(id1.size() == LEN);

	char cc = 0;
	std::cin >> cc;
	return 0;
}
