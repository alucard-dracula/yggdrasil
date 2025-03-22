// has_functional_member_t_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/func/has_functional_member_t.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T1, typename T2>
struct A
{
	typedef T1 argument_type;
	typedef T2 result_type;
};

template<typename T1, typename T2, typename T3>
struct B
{
	typedef T1 first_argument_type;
	typedef T2 second_argument_type;
	typedef T3 result_type;
};


int main(int argc, char* argv[])
{
	BOOST_MPL_ASSERT((yggr::has_argument_t<A<const int&, const float&> >));
	BOOST_MPL_ASSERT((yggr::has_result_t<A<const int&, const float&> >));

	BOOST_MPL_ASSERT_NOT((yggr::has_argument_t<B<const int&, const int&, const float&> >));

	BOOST_MPL_ASSERT((yggr::has_first_argument_t<B<const int&, const int&, const float&> >));
	BOOST_MPL_ASSERT((yggr::has_second_argument_t<B<const int&, const int&, const float&> >));
	BOOST_MPL_ASSERT((yggr::has_result_t<B<const int&, const int&, const float&> >));
	
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	BOOST_MPL_ASSERT((yggr::has_argument_t<std::unary_function<const int&, const int&> >));
	BOOST_MPL_ASSERT((yggr::has_result_t<std::unary_function<const int&, const float&> >));

	BOOST_MPL_ASSERT((yggr::has_first_argument_t<std::binary_function<const int&, const int&, const float&> >));
	BOOST_MPL_ASSERT((yggr::has_second_argument_t<std::binary_function<const int&, const int&, const float&> >));
	BOOST_MPL_ASSERT((yggr::has_result_t<std::binary_function<const int&, const int&, const float&> >));

#endif // YGGR_CPP_VER_17

	wait_any_key(argc, argv);
	return 0;
}