//tuple_comparer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/tuple_ex/tuple.hpp>

#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


template<typename Tuple, typename HT, typename TT> inline
void test_cons_sub_tuple_t(const boost::tuples::cons<HT, TT>& cons)
{
	typedef Tuple tuple_type;
	typedef boost::tuples::cons<HT, TT> cons_type;

	typedef typename yggr::tuple_ex::sub_tuple_t<tuple_type, 1, 2>::type sub_tuple_type;
	typedef typename yggr::tuple_ex::sub_tuple_t<cons_type, 1, 2>::type chk_sub_tuple_type;

	BOOST_MPL_ASSERT((boost::is_same<chk_sub_tuple_type, sub_tuple_type>));
}

template<typename Tuple, typename HT, typename TT> inline
void test_cons_sub_tuple_t(yggr::tuples::cons<HT, TT>& cons)
{
	typedef Tuple tuple_type;
	typedef yggr::tuples::cons<HT, TT> cons_type;

	typedef typename yggr::tuple_ex::sub_tuple_t<tuple_type, 1, 2>::type sub_tuple_type;
	typedef typename yggr::tuple_ex::sub_tuple_t<cons_type, 1, 2>::type chk_sub_tuple_type;

	BOOST_MPL_ASSERT((boost::is_same<chk_sub_tuple_type, sub_tuple_type>));
}

template<typename HT, typename TT, typename ChkTuple> inline
void test_cons_sub_tuple(const boost::tuples::cons<HT, TT>& cons, const ChkTuple& chk_tuple)
{
	typedef ChkTuple check_tuple_type;
	typedef boost::tuples::cons<HT, TT> cons_type;
	typedef typename yggr::tuple_ex::sub_tuple_t<cons_type, 1, 2>::type sub_tuple_type;

	BOOST_MPL_ASSERT((boost::is_same<sub_tuple_type, check_tuple_type>));

	sub_tuple_type sub_tp = yggr::tuple_ex::sub_tuple<1, 2>(cons);

	yggr_test_assert((sub_tp == chk_tuple));
}

template<typename HT, typename TT, typename ChkTuple> inline
void test_cons_sub_tuple(const yggr::tuples::cons<HT, TT>& cons, const ChkTuple& chk_tuple)
{
	typedef ChkTuple check_tuple_type;
	typedef yggr::tuples::cons<HT, TT> cons_type;
	typedef typename yggr::tuple_ex::sub_tuple_t<cons_type, 1, 2>::type sub_tuple_type;

	BOOST_MPL_ASSERT((boost::is_same<sub_tuple_type, check_tuple_type>));

	sub_tuple_type sub_tp = yggr::tuple_ex::sub_tuple<1, 2>(cons);

	yggr_test_assert((sub_tp == chk_tuple));
}

void test_sub_tuple(void)
{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	{
		typedef std::tuple<short, int, float, double> tuple_type;
		typedef yggr::tuple_ex::sub_tuple_t<tuple_type, 1, 2>::type sub_tuple_type;

		BOOST_MPL_ASSERT((boost::is_same<std::tuple<int, float>, sub_tuple_type>));

		tuple_type tp = std::make_tuple<short, int, float, double>(1, 2, 3.0f, 4.0);
		sub_tuple_type sub_tp = yggr::tuple_ex::sub_tuple<1, 2>(tp);

		yggr_test_assert((sub_tp == std::make_tuple<int, float>(2, 3.0f)));
	}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	{
		typedef boost::tuple<short, int, float, double> tuple_type;
		typedef yggr::tuple_ex::sub_tuple_t<tuple_type, 1, 2>::type sub_tuple_type;

		BOOST_MPL_ASSERT((boost::is_same<boost::tuple<int, float>, sub_tuple_type>));

		tuple_type tp = boost::make_tuple<short, int, float, double>(1, 2, 3.0f, 4.0);
		sub_tuple_type sub_tp = yggr::tuple_ex::sub_tuple<1, 2>(tp);

		yggr_test_assert((sub_tp == boost::make_tuple<int, float>(2, 3.0f)));

		test_cons_sub_tuple_t<tuple_type>(tp);
		test_cons_sub_tuple(tp, sub_tp);
	}

	{
		typedef yggr::tuple<short, int, float, double> tuple_type;
		typedef yggr::tuple_ex::sub_tuple_t<tuple_type, 1, 2>::type sub_tuple_type;

		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple<int, float>, sub_tuple_type>));

		tuple_type tp = boost::make_tuple<short, int, float, double>(1, 2, 3.0f, 4.0);
		sub_tuple_type sub_tp = yggr::tuple_ex::sub_tuple<1, 2>(tp);

		yggr_test_assert((sub_tp == yggr::make_tuple<int, float>(2, 3.0f)));

		test_cons_sub_tuple_t<tuple_type>(tp);
		test_cons_sub_tuple(tp, sub_tp);
	}

	// test failed sub_tuple
	{
		typedef yggr::tuple<short, int, float, double> tuple_type;
		typedef yggr::tuple_ex::sub_tuple_t<tuple_type, 5, 2>::type sub_tuple_type;

		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple<>, sub_tuple_type>));

		tuple_type tp = boost::make_tuple<short, int, float, double>(1, 2, 3.0f, 4.0);
		sub_tuple_type sub_tp = yggr::tuple_ex::sub_tuple<5, 2>(tp);

		yggr_test_assert((sub_tp == yggr::make_tuple()));
	}
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_sub_tuple();

	wait_any_key(argc, argv);
	return 0;
}
