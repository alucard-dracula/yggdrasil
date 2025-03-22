//lua_wrap_boost_size_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/range/functions.hpp>

#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_wrap_vector(void)
{
	int arr[] = {1, 2, 3};
	yggr::lua_wrap::wrap_vector<int> cont(arr + 0, arr + 3);

	yggr_test_assert((boost::size(cont) == cont.size()));

	std::cout << "test_wrap_vector success" << std::endl;
}

void test_wrap_list(void)
{
	int arr[] = {1, 2, 3};
	yggr::lua_wrap::wrap_list<int> cont(arr + 0, arr + 3);

	yggr_test_assert((boost::size(cont) == cont.size()));

	std::cout << "test_wrap_list success" << std::endl;
}

int main(int argc, char* argv[])
{
	test_wrap_vector();

	test_wrap_list();

	wait_any_key(argc, argv);
	return 0;
}