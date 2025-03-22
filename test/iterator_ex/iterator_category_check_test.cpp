//iterator_category_check_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <vector>
#include <list>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test1(void)
{
	typedef std::vector<int> stl_vt_type;
	typedef std::list<int> stl_list_type;

	typedef boost::container::vector<int> boost_vt_type;
	typedef boost::container::list<int> boost_list_type;

	// stl test
	BOOST_MPL_ASSERT((yggr::is_random_access_iterator<stl_vt_type::iterator>));
	BOOST_MPL_ASSERT((yggr::is_bidirectional_iterator<stl_vt_type::iterator>));

	BOOST_MPL_ASSERT((yggr::is_bidirectional_iterator<stl_list_type::iterator>));
	BOOST_MPL_ASSERT_NOT((yggr::is_random_access_iterator<stl_list_type::iterator>));

	// boost test
	BOOST_MPL_ASSERT((yggr::is_random_access_iterator<boost_vt_type::iterator>));
	BOOST_MPL_ASSERT((yggr::is_bidirectional_iterator<boost_vt_type::iterator>));

	BOOST_MPL_ASSERT((yggr::is_bidirectional_iterator<boost_list_type::iterator>));
	BOOST_MPL_ASSERT_NOT((yggr::is_random_access_iterator<boost_list_type::iterator>));
}

void test_string(void)
{
	typedef std::string stl_string_type;
	typedef boost::container::string boost_string_type;

	//typedef boost::range_iterator<stl_string_type>::type iter_type;

	BOOST_MPL_ASSERT((yggr::is_random_access_iterator<boost::range_iterator<stl_string_type>::type >));
	BOOST_MPL_ASSERT((yggr::is_random_access_iterator<boost::range_iterator<boost_string_type>::type >));
}

int main(int argc, char* argv[])
{
	test1();
	test_string();

	wait_any_key(argc, argv);
	return 0;
}