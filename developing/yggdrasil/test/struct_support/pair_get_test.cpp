//pair_get_test.cpp

#include <iostream>
#include <cassert>

#include <yggr/struct_support/pair.hpp>

int main(int argc, char* argv[])
{
	std::pair<int, int> pair(10, 20);

	assert(yggr::get<0>(pair) == 10);
	assert(yggr::get<1>(pair) == 20);

	const std::pair<int, int>& cref_pair = pair;

	assert(yggr::get<0>(cref_pair) == 10);
	assert(yggr::get<1>(cref_pair) == 20);

	assert(&yggr::get<0>(pair) == &pair.first);
	assert(&yggr::get<1>(pair) == &pair.second);

	char cc = 0;
	std::cin >> cc;

	return 0;
}