// deduce_result_test.cpp
#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/bind.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/typeof/typeof.hpp>
#include <yggr/typeof/deduce_result.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


void foo_void(void)
{
}

int foo_int(void)
{
	return 0;
}

void test_foo(void)
{
	//{
	//	typedef YGGR_TYPEOF(foo_void()) resut_void_type; // error C2784
	//}

	{
		typedef YGGR_TYPEOF(yggr::deduce_result(&foo_void)) resut_void_type;
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::void_, resut_void_type>));

		typedef YGGR_TYPEOF(yggr::deduce_result(&foo_int)) resut_int_type;
		BOOST_MPL_ASSERT((boost::is_same<int, resut_int_type>));
	}

	{
		typedef YGGR_TYPEOF(yggr::deduce_result(boost::bind(&foo_void))) resut_void_type;
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::void_, resut_void_type>));

		typedef YGGR_TYPEOF(yggr::deduce_result(boost::bind(&foo_int))) resut_int_type;
		BOOST_MPL_ASSERT((boost::is_same<int, resut_int_type>));
	}
}

struct A
{
	void mem_foo_void(void)
	{
	}

	int mem_foo_int(void)
	{
		return 0;
	}
};

void test_mem_foo(void)
{
	{
		typedef YGGR_TYPEOF(yggr::deduce_result(&A::mem_foo_void)) resut_void_type;
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::void_, resut_void_type>));

		typedef YGGR_TYPEOF(yggr::deduce_result(&A::mem_foo_int)) resut_int_type;
		BOOST_MPL_ASSERT((boost::is_same<int, resut_int_type>));
	}

	{
		typedef YGGR_TYPEOF(yggr::deduce_result(boost::bind(&A::mem_foo_void, _1))) resut_void_type;
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::void_, resut_void_type>));

		typedef YGGR_TYPEOF(yggr::deduce_result(boost::bind(&A::mem_foo_int, _1))) resut_int_type;
		BOOST_MPL_ASSERT((boost::is_same<int, resut_int_type>));
	}
}

int main(int argc, char* argv[])
{
	test_foo();
	test_mem_foo();

	wait_any_key(argc, argv);
	return 0;
}