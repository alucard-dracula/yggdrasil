// arrangement_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/math/arrangement.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_runtime(void)
{
	// 1
	yggr_test_assert((yggr::math::arrangement(0, 0) == 1 * yggr::math::factorial(0)));

	// 1 1
	yggr_test_assert((yggr::math::arrangement(1, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(1, 1) == 1 * yggr::math::factorial(1)));

	// 1 2 1
	yggr_test_assert((yggr::math::arrangement(2, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(2, 1) == 2 * yggr::math::factorial(1)));
	yggr_test_assert((yggr::math::arrangement(2, 2) == 1 * yggr::math::factorial(2)));

	// 1 3 3 1
	yggr_test_assert((yggr::math::arrangement(3, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(3, 1) == 3 * yggr::math::factorial(1)));
	yggr_test_assert((yggr::math::arrangement(3, 2) == 3 * yggr::math::factorial(2)));
	yggr_test_assert((yggr::math::arrangement(3, 3) == 1 * yggr::math::factorial(3)));

	// 1 4 6 4 1
	yggr_test_assert((yggr::math::arrangement(4, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(4, 1) == 4 * yggr::math::factorial(1)));
	yggr_test_assert((yggr::math::arrangement(4, 2) == 6 * yggr::math::factorial(2)));
	yggr_test_assert((yggr::math::arrangement(4, 3) == 4 * yggr::math::factorial(3)));
	yggr_test_assert((yggr::math::arrangement(4, 4) == 1 * yggr::math::factorial(4)));

	// 1 5 10 10 5 1
	yggr_test_assert((yggr::math::arrangement(5, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(5, 1) == 5 * yggr::math::factorial(1)));
	yggr_test_assert((yggr::math::arrangement(5, 2) == 10 * yggr::math::factorial(2)));
	yggr_test_assert((yggr::math::arrangement(5, 3) == 10 * yggr::math::factorial(3)));
	yggr_test_assert((yggr::math::arrangement(5, 4) == 5 * yggr::math::factorial(4)));
	yggr_test_assert((yggr::math::arrangement(5, 5) == 1 * yggr::math::factorial(5)));

	// 1 6 15 20 15 6 1
	yggr_test_assert((yggr::math::arrangement(6, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(6, 1) == 6 * yggr::math::factorial(1)));
	yggr_test_assert((yggr::math::arrangement(6, 2) == 15 * yggr::math::factorial(2)));
	yggr_test_assert((yggr::math::arrangement(6, 3) == 20 * yggr::math::factorial(3)));
	yggr_test_assert((yggr::math::arrangement(6, 4) == 15 * yggr::math::factorial(4)));
	yggr_test_assert((yggr::math::arrangement(6, 5) == 6 * yggr::math::factorial(5)));
	yggr_test_assert((yggr::math::arrangement(6, 6) == 1 * yggr::math::factorial(6)));

	// 1 7 21 35 35 21 7 1
	yggr_test_assert((yggr::math::arrangement(7, 0) == 1 * yggr::math::factorial(0)));
	yggr_test_assert((yggr::math::arrangement(7, 1) == 7 * yggr::math::factorial(1)));
	yggr_test_assert((yggr::math::arrangement(7, 2) == 21 * yggr::math::factorial(2)));
	yggr_test_assert((yggr::math::arrangement(7, 3) == 35 * yggr::math::factorial(3)));
	yggr_test_assert((yggr::math::arrangement(7, 4) == 35 * yggr::math::factorial(4)));
	yggr_test_assert((yggr::math::arrangement(7, 5) == 21 * yggr::math::factorial(5)));
	yggr_test_assert((yggr::math::arrangement(7, 6) == 7 * yggr::math::factorial(6)));
	yggr_test_assert((yggr::math::arrangement(7, 7) == 1 * yggr::math::factorial(7)));
}

void test_meta(void)
{
	// 1
	yggr_test_assert((yggr::math::arrangement_t<0, 0>::value == 1 * yggr::math::factorial_t<0>::value));

	// 1 1
	yggr_test_assert((yggr::math::arrangement_t<1, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<1, 1>::value == 1 * yggr::math::factorial_t<1>::value));

	// 1 2 1
	yggr_test_assert((yggr::math::arrangement_t<2, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<2, 1>::value == 2 * yggr::math::factorial_t<1>::value));
	yggr_test_assert((yggr::math::arrangement_t<2, 2>::value == 1 * yggr::math::factorial_t<2>::value));

	// 1 3 3 1
	yggr_test_assert((yggr::math::arrangement_t<3, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<3, 1>::value == 3 * yggr::math::factorial_t<1>::value));
	yggr_test_assert((yggr::math::arrangement_t<3, 2>::value == 3 * yggr::math::factorial_t<2>::value));
	yggr_test_assert((yggr::math::arrangement_t<3, 3>::value == 1 * yggr::math::factorial_t<3>::value));

	// 1 4 6 4 1
	yggr_test_assert((yggr::math::arrangement_t<4, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<4, 1>::value == 4 * yggr::math::factorial_t<1>::value));
	yggr_test_assert((yggr::math::arrangement_t<4, 2>::value == 6 * yggr::math::factorial_t<2>::value));
	yggr_test_assert((yggr::math::arrangement_t<4, 3>::value == 4 * yggr::math::factorial_t<3>::value));
	yggr_test_assert((yggr::math::arrangement_t<4, 4>::value == 1 * yggr::math::factorial_t<4>::value));

	// 1 5 10 10 5 1
	yggr_test_assert((yggr::math::arrangement_t<5, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<5, 1>::value == 5 * yggr::math::factorial_t<1>::value));
	yggr_test_assert((yggr::math::arrangement_t<5, 2>::value == 10 * yggr::math::factorial_t<2>::value));
	yggr_test_assert((yggr::math::arrangement_t<5, 3>::value == 10 * yggr::math::factorial_t<3>::value));
	yggr_test_assert((yggr::math::arrangement_t<5, 4>::value == 5 * yggr::math::factorial_t<4>::value));
	yggr_test_assert((yggr::math::arrangement_t<5, 5>::value == 1 * yggr::math::factorial_t<5>::value));

	// 1 6 15 20 15 6 1
	yggr_test_assert((yggr::math::arrangement_t<6, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<6, 1>::value == 6 * yggr::math::factorial_t<1>::value));
	yggr_test_assert((yggr::math::arrangement_t<6, 2>::value == 15 * yggr::math::factorial_t<2>::value));
	yggr_test_assert((yggr::math::arrangement_t<6, 3>::value == 20 * yggr::math::factorial_t<3>::value));
	yggr_test_assert((yggr::math::arrangement_t<6, 4>::value == 15 * yggr::math::factorial_t<4>::value));
	yggr_test_assert((yggr::math::arrangement_t<6, 5>::value == 6 * yggr::math::factorial_t<5>::value));
	yggr_test_assert((yggr::math::arrangement_t<6, 6>::value == 1 * yggr::math::factorial_t<6>::value));

	// 1 7 21 35 35 21 7 1
	yggr_test_assert((yggr::math::arrangement_t<7, 0>::value == 1 * yggr::math::factorial_t<0>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 1>::value == 7 * yggr::math::factorial_t<1>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 2>::value == 21 * yggr::math::factorial_t<2>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 3>::value == 35 * yggr::math::factorial_t<3>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 4>::value == 35 * yggr::math::factorial_t<4>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 5>::value == 21 * yggr::math::factorial_t<5>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 6>::value == 7 * yggr::math::factorial_t<6>::value));
	yggr_test_assert((yggr::math::arrangement_t<7, 7>::value == 1 * yggr::math::factorial_t<7>::value));

	// 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<0, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));

	// 1 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<1, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<1, 1>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<1>::value> >));

	// 1 2 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<2, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<2, 1>::type,
						boost::mpl::size_t<2 * yggr::math::factorial_t<1>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<2, 2>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<2>::value> >));

	// 1 3 3 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<3, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<3, 1>::type,
						boost::mpl::size_t<3 * yggr::math::factorial_t<1>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<3, 2>::type,
						boost::mpl::size_t<3 * yggr::math::factorial_t<2>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<3, 3>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<3>::value> >));

	// 1 4 6 4 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<4, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<4, 1>::type,
						boost::mpl::size_t<4 * yggr::math::factorial_t<1>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<4, 2>::type,
						boost::mpl::size_t<6 * yggr::math::factorial_t<2>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<4, 3>::type,
						boost::mpl::size_t<4 * yggr::math::factorial_t<3>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<4, 4>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<4>::value> >));

	// 1 5 10 10 5 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<5, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<5, 1>::type,
						boost::mpl::size_t<5 * yggr::math::factorial_t<1>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<5, 2>::type,
						boost::mpl::size_t<10 * yggr::math::factorial_t<2>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<5, 3>::type,
						boost::mpl::size_t<10 * yggr::math::factorial_t<3>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<5, 4>::type,
						boost::mpl::size_t<5 * yggr::math::factorial_t<4>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<5, 5>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<5>::value> >));

	// 1 6 15 20 15 6 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 1>::type,
						boost::mpl::size_t<6 * yggr::math::factorial_t<1>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 2>::type,
						boost::mpl::size_t<15 * yggr::math::factorial_t<2>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 3>::type,
						boost::mpl::size_t<20 * yggr::math::factorial_t<3>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 4>::type,
						boost::mpl::size_t<15 * yggr::math::factorial_t<4>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 5>::type,
						boost::mpl::size_t<6 * yggr::math::factorial_t<5>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<6, 6>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<6>::value> >));

	// 1 7 21 35 35 21 7 1
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 0>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<0>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 1>::type,
						boost::mpl::size_t<7 * yggr::math::factorial_t<1>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 2>::type,
						boost::mpl::size_t<21 * yggr::math::factorial_t<2>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 3>::type,
						boost::mpl::size_t<35 * yggr::math::factorial_t<3>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 4>::type,
						boost::mpl::size_t<35 * yggr::math::factorial_t<4>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 5>::type,
						boost::mpl::size_t<21 * yggr::math::factorial_t<5>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 6>::type,
						boost::mpl::size_t<7 * yggr::math::factorial_t<6>::value> >));
	BOOST_MPL_ASSERT((boost::is_same<yggr::math::arrangement_t<7, 7>::type,
						boost::mpl::size_t<1 * yggr::math::factorial_t<7>::value> >));
}

int main(int argc, char* argv[])
{
	test_runtime();
	test_meta();

	wait_any_key(argc, argv);
	return 0;
}
