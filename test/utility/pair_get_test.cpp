//pair_get_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/utility/pair.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

void test_get(void)
{
	std::pair<int, int> pair(10, 20);

	yggr_test_assert(yggr::get<0>(pair) == 10);
	yggr_test_assert(yggr::get<1>(pair) == 20);

	const std::pair<int, int>& cref_pair = pair;

	yggr_test_assert(yggr::get<0>(cref_pair) == 10);
	yggr_test_assert(yggr::get<1>(cref_pair) == 20);

	yggr_test_assert(&yggr::get<0>(pair) == &pair.first);
	yggr_test_assert(&yggr::get<1>(pair) == &pair.second);
}

void test_tuple_utility(void)
{
	typedef std::pair<int, float> pair_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::tuple_size<pair_type>::value == 2)>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<0, pair_type>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<1, pair_type>::type, float>));

	//BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<2, pair_type>::type, int>));
}

int main(int argc, char* argv[])
{
	test_get();
	test_tuple_utility();

	wait_any_key(argc, argv);
	return 0;
}
