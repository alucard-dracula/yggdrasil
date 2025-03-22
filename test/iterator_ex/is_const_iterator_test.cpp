//is_const_iterator_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/type_traits/is_same.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/iterator_ex/is_const_iterator.hpp>

#include <yggr/charset/utf8_string.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	{
		
		BOOST_MPL_ASSERT((yggr::is_const_iterator<const char*>));
		BOOST_MPL_ASSERT((yggr::is_const_iterator<char const*>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<char*>));

		BOOST_MPL_ASSERT((yggr::is_const_iterator<const int*>));
		BOOST_MPL_ASSERT((yggr::is_const_iterator<int const*>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<int*>));
	}

	{
		typedef std::vector<int> vt_type;

		BOOST_MPL_ASSERT((yggr::is_const_iterator<vt_type::const_iterator>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<vt_type::iterator>));
	}

	{
		typedef boost::container::vector<int> vt_type;
	
		BOOST_MPL_ASSERT((yggr::is_const_iterator<vt_type::const_iterator>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<vt_type::iterator>));
	}

	{
		typedef std::list<int> vt_type;
	
		BOOST_MPL_ASSERT((yggr::is_const_iterator<vt_type::const_iterator>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<vt_type::iterator>));
	}

	{
		typedef boost::container::list<int> vt_type;
	
		BOOST_MPL_ASSERT((yggr::is_const_iterator<vt_type::const_iterator>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<vt_type::iterator>));
	}

	{
		BOOST_MPL_ASSERT((yggr::is_const_iterator<yggr::utf8_string::const_iterator>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<yggr::utf8_string::iterator>));
		BOOST_MPL_ASSERT_NOT((yggr::is_const_iterator<yggr::utf8_string::non_const_iterator>));
	}

	wait_any_key(argc, argv);
	return 0;
}