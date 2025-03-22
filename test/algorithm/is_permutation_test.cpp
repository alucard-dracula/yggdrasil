//is_permutation_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/array.hpp>
#include <yggr/algorithm/is_permutation.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	boost::array<int, 5> foo = {1, 2, 3, 4, 5};
	boost::array<int, 5> bar = {3, 1, 4, 5, 2};
	boost::array<int, 5> diff_bar = {3, 1, 4, 5, 8};

	yggr_test_assert(yggr::is_permutation(foo.begin(), foo.end(), bar.begin()));
	if(yggr::is_permutation(foo.begin(), foo.end(), bar.begin()))
	{
		std::cout << "foo and bar contain the same elements.\n";
	}

	yggr_test_assert(!yggr::is_permutation(foo.begin(), foo.end(), diff_bar.begin()));
	if(!yggr::is_permutation(foo.begin(), foo.end(), diff_bar.begin()))
	{
		std::cout << "foo and bar contain the different elements.\n";
	}

	wait_any_key(argc, argv);

	return 0;
}