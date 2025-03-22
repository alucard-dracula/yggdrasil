//tuple_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

//#include <yggr/ppex/pp_debug.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/utility/swap.hpp>

#define YGGR_TUPLE_LIMIT_LENGTH_CFG 20

//#define BASIC_TEST 0

#ifndef BASIC_TEST
#	include <yggr/tuple/tuple.hpp>
#elif BASIC_TEST
#	include <yggr/tuple/detail/tuple_basic.hpp>
#else
// this test need cheange tuple_config.hpp
// "#include <boost/tuple/tuple.hpp>" to
// "#include <boost/tuple/detail/tuple_basic_no_partial_spec.hpp>"

#	include <yggr/tuple/detail/tuple_basic_no_partial_spec.hpp>
#define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#endif // BASIC_TEST

#include <yggr/tuple/tuple_comparison.hpp>
#include <yggr/tuple/tuple_io.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace yggr
{

using tuples::tuple;
using tuples::make_tuple;
using tuples::tie;
using tuples::get;

} // namespace boost

namespace boost
{
using tuples::tuple;
using tuples::make_tuple;
using tuples::tie;
using tuples::get;

} // namespace boost

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

void test_utility(void)
{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	typedef std::tuple<int, int> std_tuple_type;
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	typedef yggr::tuple<int, int> yggr_tuple_type;
	typedef boost::tuple<int, int> boost_tuple_type;

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			yggr::tuples::length<std_tuple_type>::value
			== std::tuple_size<std_tuple_type>::value
		>));
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	// boost::tuples::length
	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			boost::tuples::length<boost_tuple_type>::value
			== boost::tuples::length<yggr_tuple_type>::value
		>));

	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			yggr::tuples::length<boost_tuple_type>::value
			== yggr::tuples::length<yggr_tuple_type>::value
		>));

	//boost::tuples::element

	BOOST_MPL_ASSERT((
		boost::is_same
		<
			yggr::tuples::element<0, boost_tuple_type>::type,
			yggr::tuples::element<0, yggr_tuple_type>::type
		>));

	BOOST_MPL_ASSERT((
		boost::is_same
		<
			boost::tuples::element<0, boost_tuple_type>::type,
			boost::tuples::element<0, yggr_tuple_type>::type
		>));

	// tuple_size
	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			yggr::tuple_size<boost_tuple_type>::value
			== yggr::tuple_size<yggr_tuple_type>::value
		>));

	// tuple_element
	BOOST_MPL_ASSERT((
		boost::is_same
		<
			yggr::tuple_element<0, boost_tuple_type>::type,
			yggr::tuple_element<0, yggr_tuple_type>::type
		>));
}

void test_basic(void)
{
	typedef yggr::tuple<int, int> yggr_tuple_type;
	typedef boost::tuple<int, int> boost_tuple_type;

	yggr_tuple_type t1 = yggr::make_tuple(10, 20);
	boost_tuple_type t2 = boost::make_tuple(10, 20);
	boost_tuple_type t3 = boost::make_tuple(30, 40);

	yggr_tuple_type ybt1 = t2;
	yggr_test_assert(yggr::get<0>(ybt1) == boost::get<0>(t2));
	yggr_test_assert(ybt1.get<0>() == boost::get<0>(t2));
	yggr_test_assert(yggr::get<1>(ybt1) == boost::get<1>(t2));

	ybt1 = t1;
	yggr_test_assert(yggr::get<0>(ybt1) == boost::get<0>(t1));
	yggr_test_assert(yggr::get<1>(ybt1) == boost::get<1>(t1));

	yggr_test_assert(boost::get<0>(t1) == 10);
	yggr_test_assert(yggr::get<0>(t1) == boost::get<0>(t2));

	yggr_test_assert(yggr::get<0>(t1) == 10);
	yggr_test_assert(boost::get<0>(t1) == yggr::get<0>(t2));

	boost::swap(t1, t3);
	yggr_test_assert(boost::get<0>(t1) == 30);
	yggr_test_assert(boost::get<1>(t1) == 40);
	yggr_test_assert(boost::get<0>(t3) == 10);
	yggr_test_assert(boost::get<1>(t3) == 20);

	{
		boost_tuple_type bt = t1;
		yggr_tuple_type yt = t2;

		yggr_test_assert(yggr::get<0>(bt) == boost::get<0>(t1));
		yggr_test_assert(yggr::get<1>(bt) == boost::get<1>(t1));
		yggr_test_assert(boost::get<0>(yt) == yggr::get<0>(t2));
		yggr_test_assert(boost::get<1>(yt) == yggr::get<1>(t2));
	}

	{
		boost_tuple_type bt;
		yggr_tuple_type yt;

		bt = t1;
		yt = t3;

		yggr_test_assert(boost::get<0>(bt) == boost::get<0>(t1));
		yggr_test_assert(boost::get<1>(bt) == boost::get<1>(t1));
		yggr_test_assert(boost::get<0>(yt) == boost::get<0>(t2));
		yggr_test_assert(boost::get<1>(yt) == boost::get<1>(t2));

		int a = 0, b = 0;
		const boost_tuple_type& cbt = bt;

		boost::tie(a, b) = cbt;
		yggr_test_assert(a == yggr::get<0>(bt));
		yggr_test_assert(b == yggr::get<1>(bt));

		yggr::tie(a, b) = yt;
		yggr_test_assert(a == boost::get<0>(yt));
		yggr_test_assert(b == boost::get<1>(yt));

		yggr::tie(a, b) = bt;
		yggr_test_assert(a == yggr::get<0>(bt));
		yggr_test_assert(b == yggr::get<1>(bt));


#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
		// if use tuple_basic_no_partial_spec.hpp, throw a static_assert(false)
		boost::tie(a, b) = yt;
		yggr_test_assert(a == yggr::get<0>(yt));
		yggr_test_assert(b == yggr::get<1>(yt));
#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	}
}

namespace std
{

void swap(int& l, float& r)
{
	float t = l;
	l = r;
	r = t;
}

void swap(float& l, int& r)
{
	int t = l;
	l = r;
	r = t;
}

} // namespace std

namespace boost
{

void swap(int& l, float& r)
{
	float t = l;
	l = r;
	r = t;
}

void swap(float& l, int& r)
{
	int t = l;
	l = r;
	r = t;
}

} // namespace boost

void test_swap(void)
{
	typedef yggr::tuple<int, int> yggr_i_tuple_type;
	typedef yggr::tuple<float, float> yggr_f_tuple_type;

	typedef boost::tuple<int, int> boost_i_tuple_type;
	typedef boost::tuple<float, float> boost_f_tuple_type;

	yggr_i_tuple_type yit1 = yggr::make_tuple(10.0f, 10.0f);
	yggr_f_tuple_type yft1 = yggr::make_tuple(20, 20);

	yggr_test_assert(yit1 == yggr::make_tuple(10.0f, 10.0f));
	yggr_test_assert(yft1 == yggr::make_tuple(20, 20));

	boost_i_tuple_type bit1 = boost::make_tuple(30, 30);
	boost_i_tuple_type bit2 = boost::make_tuple(40.0f, 40.0f);

	boost_f_tuple_type bft1 = boost::make_tuple(30, 30);
	boost_f_tuple_type bft2 = boost::make_tuple(40.0f, 40.0f);

	yggr_test_assert(bit1 == bft1);
	yggr_test_assert(bit2 == bft2);

	{
		yggr_i_tuple_type l = yit1;
		boost_f_tuple_type r = bft1;

		yggr_test_assert(l == yit1);
		yggr_test_assert(r == bft1);

		yggr::swap(l, r);

		yggr_test_assert(r == yit1);
		yggr_test_assert(l == bft1);
	}

	{
		yggr_i_tuple_type l = yit1;
		yggr_f_tuple_type r = bft1;

		yggr_test_assert(l == yit1);
		yggr_test_assert(r == bft1);

		yggr::swap(l, r);

		yggr_test_assert(r == yit1);
		yggr_test_assert(l == bft1);
	}

	{
		boost_i_tuple_type l = yit1;
		boost_f_tuple_type r = bft1;

		yggr_test_assert(l == yit1);
		yggr_test_assert(r == bft1);

		yggr::swap(l, r);

		yggr_test_assert(r == yit1);
		yggr_test_assert(l == bft1);
	}

	{
		boost_f_tuple_type l = yit1;
		boost_f_tuple_type r = bft1;

		yggr_test_assert(l == yit1);
		yggr_test_assert(r == bft1);

		yggr::swap(l, r);

		yggr_test_assert(r == yit1);
		yggr_test_assert(l == bft1);
	}

	{
		boost_f_tuple_type bft = boost::make_tuple(100, 200);

		bft = yit1;

		yggr::swap(bft, yit1);

		yggr_test_assert(bft == yit1);
	}


	{
		yggr_i_tuple_type t1 = yggr::make_tuple(10, 20);
		yggr_i_tuple_type t2 = yggr::make_tuple(20, 20);

		yggr_i_tuple_type t3 = t1, t4 = t2;

		std::swap(t1, t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
		boost::swap(t1, t2);
		yggr_test_assert((t1 == t3 && t2 == t4));
		yggr::swap(t1, t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
	}

	{
		yggr_i_tuple_type t1 = yggr::make_tuple(10, 20);
		yggr_i_tuple_type t2 = yggr::make_tuple(20, 20);

		yggr_i_tuple_type t3 = t1, t4 = t2;

		std::swap(boost::move(t1), t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
		boost::swap(boost::move(t1), t2);
		yggr_test_assert((t1 == t3 && t2 == t4));
		yggr::swap(boost::move(t1), t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
	}

	{
		boost_i_tuple_type t1 = boost::make_tuple(10, 20);
		boost_i_tuple_type t2 = boost::make_tuple(20, 20);

		boost_i_tuple_type t3 = t1, t4 = t2;

		std::swap(t1, t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
		boost::swap(t1, t2);
		yggr_test_assert((t1 == t3 && t2 == t4));
		yggr::swap(t1, t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
	}

	{
		boost_i_tuple_type t1 = boost::make_tuple(10, 20);
		boost_i_tuple_type t2 = boost::make_tuple(20, 20);

		boost_i_tuple_type t3 = t1, t4 = t2;

		std::swap(boost::move(t1), t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
		boost::swap(boost::move(t1), t2);
		yggr_test_assert((t1 == t3 && t2 == t4));
		yggr::swap(boost::move(t1), t2);
		yggr_test_assert((t1 == t4 && t2 == t3));
	}
}

void test_comparison(void)
{
	typedef yggr::tuple<int, int> yggr_tuple_type;
	typedef boost::tuple<int, int> boost_tuple_type;
	typedef std::pair<int, int> pair_type;

	yggr_tuple_type t1 = yggr::make_tuple(10, 20);
	boost_tuple_type t2 = boost::make_tuple(10, 20);
	boost_tuple_type t3 = boost::make_tuple(30, 40);

	pair_type p2 = std::make_pair(10, 20);
	pair_type p3 = std::make_pair(30, 40);

	yggr_test_assert(t1 == t1);
	yggr_test_assert(t1 == t2);
	yggr_test_assert(t1 == p2);
	yggr_test_assert(p2 == t1);

	yggr_test_assert(t1 != t3);
	yggr_test_assert(t1 != p3);
	yggr_test_assert(p3 != t1);

	yggr_test_assert(t1 <= t1);
	yggr_test_assert(t1 <= t2);

	yggr_test_assert(t1 <= p2);
	yggr_test_assert(p2 <= t1);

	yggr_test_assert(t1 < t3);
	yggr_test_assert(!(t3 < t1));

	yggr_test_assert(t1 < p3);
	yggr_test_assert(!(p3 < t1));

	yggr_test_assert(t1 >= t1);
	yggr_test_assert(t1 >= t2);

	yggr_test_assert(t1 >= p2);
	yggr_test_assert(p2 >= t1);

	yggr_test_assert(t3 > t1);
	yggr_test_assert(!(t1 > t3));

	yggr_test_assert(p3 > t1);
	yggr_test_assert(!(t1 > p3));
}

void test_io(int argc, char* argv[])
{
	typedef yggr::tuple<int, int> yggr_tuple_type;
	typedef boost::tuple<int, int> boost_tuple_type;
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	typedef std::tuple<int, int> std_tuple_type;
#endif // YGGR_NO_CXX11_HDR_TUPLE

	yggr_tuple_type t1 = yggr::make_tuple(10, 20);
	boost_tuple_type t2 = boost::make_tuple(30, 40);
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	std_tuple_type t3 = std::make_tuple(50, 60);
#endif // YGGR_NO_CXX11_HDR_TUPLE

	std::cout << t1 << std::endl;
	std::cout << t2 << std::endl;
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	std::cout << t3 << std::endl;
#endif //#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	std::cout << "input tuple, example: (10 20)" << std::endl;

	if((argc > 1) && (yggr::string("--batch-test") == argv[argc - 1]))
	{
		std::stringstream ss;
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
		ss << "(70 80) (90 100) (110 120)";
#else
		ss << "(70 80) (90 100)";
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)
		ss >> t1 >> t2
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
			>> t3
#endif //#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
			;
	}
	else
	{
		std::cin >> t1 >> t2
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
			>> t3
#endif //#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
			;
	}

	std::cout << t1 << std::endl;
	std::cout << t2 << std::endl;
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	std::cout << t3 << std::endl;
#endif //#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
}

int main(int argc, char* argv[])
{
#define TEST_TUPLE_IO

	test_utility();

	test_basic();

	test_comparison();

	test_swap();

#ifdef TEST_TUPLE_IO
	test_io(argc, argv);
#endif // TEST_TUPLE_IO

	wait_any_key(argc, argv);
	return 0;
}
