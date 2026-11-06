//tuple_maker_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/tuple_ex/tuple.hpp> 
#include <yggr/tuple_ex/tuple_maker.hpp> 

#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_tuple_maker(void)
{
	// std tuple
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		typedef std::tuple<int, int, int> sample_tuple_type;
		typedef yggr::tuple_ex::tuple_maker<sample_tuple_type> tuple_maker_type;

		tuple_maker_type maker;

		yggr_test_assert((std::make_tuple(1.5f, 2.5) == maker.make_tuple(1.5f, 2.5)));
		yggr_test_assert((std::make_tuple(1, 1.5f, 2.5) == maker.make_tuple(1, 1.5f, 2.5)));
		yggr_test_assert((std::make_tuple(1, 1.5f, 2.5, 3) == maker.make_tuple(1, 1.5f, 2.5, 3)));

	}
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	{
		typedef boost::tuple<int, int, int> sample_tuple_type;
		typedef yggr::tuple_ex::tuple_maker<sample_tuple_type> tuple_maker_type;

		tuple_maker_type maker;

		yggr_test_assert((boost::make_tuple(1.5f, 2.5) == maker.make_tuple(1.5f, 2.5)));
		yggr_test_assert((boost::make_tuple(1, 1.5f, 2.5) == maker.make_tuple(1, 1.5f, 2.5)));
		yggr_test_assert((boost::make_tuple(1, 1.5f, 2.5, 3) == maker.make_tuple(1, 1.5f, 2.5, 3)));

	}

	{
		typedef yggr::tuple<int, int, int> sample_tuple_type;
		typedef yggr::tuple_ex::tuple_maker<sample_tuple_type> tuple_maker_type;

		tuple_maker_type maker;

		yggr_test_assert((yggr::make_tuple(1.5f, 2.5) == maker.make_tuple(1.5f, 2.5)));
		yggr_test_assert((yggr::make_tuple(1, 1.5f, 2.5) == maker.make_tuple(1, 1.5f, 2.5)));
		yggr_test_assert((yggr::make_tuple(1, 1.5f, 2.5, 3) == maker.make_tuple(1, 1.5f, 2.5, 3)));

	}
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_tuple_maker();

	wait_any_key(argc, argv);
	return 0;
}
