//tuple_to_params_call_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/caller/to_params_caller.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int foo(int a, int b)
{
	std::cout << "foo(int, int)" << std::endl;

	return a + b;
}

struct Foo_obj
{
	void foo(int a, int b) const
	{
		std::cout << "member foo(int, int)" << std::endl;
		return;
	}

	void foo1(int a) const
	{
		std::cout << "member foo1(int)" << std::endl;
		return;
	}
};

void test_packet_info(void)
{
	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), ::yggr::packet::make_packet_info(10, 20));
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), ::yggr::packet::make_packet_info(10, 20))));

	Foo_obj foo_obj;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), ::yggr::packet::make_packet_info(10, 20));
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), ::yggr::packet::make_packet_info(boost::ref(foo_obj), 10, 20));
}

void test_tuple(void)
{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_tuple(10, 20))));

		std::tuple<int, int> tuple_val =  std::make_tuple(10, 20);
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), tuple_val)));

		Foo_obj foo_obj;

		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), std::make_tuple(10, 20));
		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), std::make_tuple(boost::ref(foo_obj), 10, 20));
	}

#endif // YGGR_NO_CXX11_HDR_TUPLE

	{
		yggr::caller::to_params_call(boost::bind(&foo, _1, _2), boost::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), boost::make_tuple(10, 20))));

		boost::tuple<int, int> tuple_val =  boost::make_tuple(10, 20);
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), tuple_val)));

		Foo_obj foo_obj;

		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), boost::make_tuple(10, 20));
		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), boost::make_tuple(boost::ref(foo_obj), 10, 20));
	}

	{
		yggr::caller::to_params_call(boost::bind(&foo, _1, _2), yggr::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), yggr::make_tuple(10, 20))));

		yggr::tuple<int, int> tuple_val =  yggr::make_tuple(10, 20);
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), tuple_val)));

		Foo_obj foo_obj;

		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), yggr::make_tuple(10, 20));
		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), yggr::make_tuple(boost::ref(foo_obj), 10, 20));
	}
}

void test_pair(void)
{
	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_pair(10, 20));
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_pair(10, 20))));

	std::pair<int, int> pair_val =  std::make_pair(10, 20);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), pair_val)));

	Foo_obj foo_obj;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), std::make_pair(10, 20));
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo1, _1, _2), std::make_pair(boost::ref(foo_obj), 10));
}

void test_array(void)
{
	boost::array<int, 2> array = {10, 20};
	const boost::array<int, 2>& array_cref = array;

	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), array);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), array_cref)));

	Foo_obj foo_obj;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), array);
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj, _1, _2), array_cref);
}

int main(int argc, char* argv[])
{
	test_tuple();
	test_pair();
	test_array();

	wait_any_key(argc, argv);
	return 0;
}
