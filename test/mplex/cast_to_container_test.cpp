//cast_to_container_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/bind.hpp>

#include <yggr/mplex/cast_to_t_container.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
};

template<typename T1, typename T2>
struct B
{
};

void test_vector(void)
{
	{
		typedef yggr::mplex::cast_to_vector<>::type cast_type;
		typedef boost::mpl::vector<>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef int val_type;
		typedef yggr::mplex::cast_to_vector<val_type>::type cast_type;
		typedef boost::mpl::vector<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef A val_type;
		typedef yggr::mplex::cast_to_vector<val_type>::type cast_type;
		typedef boost::mpl::vector<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef B<int, int> val_type;
		typedef yggr::mplex::cast_to_vector<val_type>::type cast_type;
		typedef boost::mpl::vector<int, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef boost::_bi::list_av_2< boost::arg<1>, int> val_type;
		typedef yggr::mplex::cast_to_vector<val_type>::type cast_type;
		typedef boost::mpl::vector<boost::arg<1>, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}
}

void test_list(void)
{
	{
		typedef yggr::mplex::cast_to_list<>::type cast_type;
		typedef boost::mpl::list<>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef int val_type;
		typedef yggr::mplex::cast_to_list<val_type>::type cast_type;
		typedef boost::mpl::list<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef A val_type;
		typedef yggr::mplex::cast_to_list<val_type>::type cast_type;
		typedef boost::mpl::list<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef B<int, int> val_type;
		typedef yggr::mplex::cast_to_list<val_type>::type cast_type;
		typedef boost::mpl::list<int, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef boost::_bi::list_av_2< boost::arg<1>, int> val_type;
		typedef yggr::mplex::cast_to_list<val_type>::type cast_type;
		typedef boost::mpl::list<boost::arg<1>, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}
}

void test_deque(void)
{
	{
		typedef yggr::mplex::cast_to_deque<>::type cast_type;
		typedef boost::mpl::deque<>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef int val_type;
		typedef yggr::mplex::cast_to_deque<val_type>::type cast_type;
		typedef boost::mpl::deque<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef A val_type;
		typedef yggr::mplex::cast_to_deque<val_type>::type cast_type;
		typedef boost::mpl::deque<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef B<int, int> val_type;
		typedef yggr::mplex::cast_to_deque<val_type>::type cast_type;
		typedef boost::mpl::deque<int, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef boost::_bi::list_av_2< boost::arg<1>, int> val_type;
		typedef yggr::mplex::cast_to_deque<val_type>::type cast_type;
		typedef boost::mpl::deque<boost::arg<1>, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}
}

void test_set(void)
{
	{
		typedef yggr::mplex::cast_to_set<>::type cast_type;
		typedef boost::mpl::set<>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef int val_type;
		typedef yggr::mplex::cast_to_set<val_type>::type cast_type;
		typedef boost::mpl::set<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef A val_type;
		typedef yggr::mplex::cast_to_set<val_type>::type cast_type;
		typedef boost::mpl::set<val_type>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef B<int, int> val_type;
		typedef yggr::mplex::cast_to_set<val_type>::type cast_type;
		typedef boost::mpl::set<int, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}

	{
		typedef boost::_bi::list_av_2< boost::arg<1>, int> val_type;
		typedef yggr::mplex::cast_to_set<val_type>::type cast_type;
		typedef boost::mpl::set<boost::arg<1>, int>::type org_type;
		BOOST_MPL_ASSERT((boost::is_same<cast_type, org_type>));
	}
}

int main(int argc, char* argv[])
{
	test_vector();
	test_list();
	test_deque();
	test_set();

	wait_any_key(argc, argv);
	return 0;
}