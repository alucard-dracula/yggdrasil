//get_element_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/utility/get_element.hpp>
#include <yggr/tuple_ex/get_element.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

void test_object(void)
{
	int n = 10;
	yggr_test_assert(yggr::get_first_element(n) == n);
	yggr_test_assert(yggr::get_last_element(n) == n);
}

void pair_get_test(void)
{
	std::pair<int, int> pair(10, 20);

	yggr_test_assert(yggr::get_first_element(pair) == 10);
	yggr_test_assert(yggr::get_last_element(pair) == 20);

	const std::pair<int, int>& cref_pair = pair;

	yggr_test_assert(yggr::get_first_element(cref_pair) == 10);
	yggr_test_assert(yggr::get_last_element(cref_pair) == 20);

	yggr_test_assert(&yggr::get_first_element(pair) == &pair.first);
	yggr_test_assert(&yggr::get_last_element(pair) == &pair.second);
}

void tuple_get_test(void)
{
	typedef boost::tuple<int, int, int> tuple_type;
	tuple_type tuple = boost::make_tuple(1, 2, 3);
	const tuple_type& tuple_cref = tuple;

	yggr_test_assert(yggr::get_first_element(tuple) == yggr::get<0>(tuple));
	yggr_test_assert(yggr::get_last_element(tuple) == yggr::get<2>(tuple));

	yggr_test_assert(yggr::get_first_element(tuple_cref) == yggr::get<0>(tuple_cref));
	yggr_test_assert(yggr::get_last_element(tuple_cref) == yggr::get<2>(tuple_cref));

	yggr_test_assert(yggr::get_first_element(tuple) == yggr::get<0>(tuple_cref));
	yggr_test_assert(yggr::get_last_element(tuple) == yggr::get<2>(tuple_cref));

	//test address
	yggr_test_assert(&yggr::get_first_element(tuple) == &yggr::get<0>(tuple));
	yggr_test_assert(&yggr::get_last_element(tuple) == &yggr::get<2>(tuple));

	yggr_test_assert(&yggr::get_first_element(tuple_cref) == &yggr::get<0>(tuple_cref));
	yggr_test_assert(&yggr::get_last_element(tuple_cref) == &yggr::get<2>(tuple_cref));

	yggr_test_assert(&yggr::get_first_element(tuple) == &yggr::get<0>(tuple_cref));
	yggr_test_assert(&yggr::get_last_element(tuple) == &yggr::get<2>(tuple_cref));
}

int main(int argc, char* argv[])
{
	test_object();
	pair_get_test();
	tuple_get_test();

	wait_any_key(argc, argv);
	return 0;
}
