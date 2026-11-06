//tuple_to_params_call_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/caller/to_params_caller.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

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
	void nc_foo(int a, int b)
	{
		std::cout << "member nc_foo(int, int)" << std::endl;
		return;
	}

	void nc_foo1(int a)
	{
		std::cout << "member nc_foo1(int)" << std::endl;
		return;
	}

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

namespace yggr
{
namespace utility
{
namespace detail
{

template<>
struct to_pointer_helper<yggr::shared_ptr<Foo_obj>, false>
{
public:
	typedef yggr::shared_ptr<Foo_obj> type;
	typedef Foo_obj* pointer_type;

public:
	inline pointer_type conv(type& t) const
	{
		return t.get();
	}

	template<typename R> inline
	typename boost::enable_if<boost::is_same<type, R>, pointer_type>::type
		conv(type& t) const
	{
		return t.get();
	}

	template<typename R> inline
	typename boost::disable_if<boost::is_same<type, R>, R*>::type
		conv(type& t) const
	{
		return reinterpret_cast<R*>(t.get());
	}
};

} // namespace detail
} // namespace utility
} // namespace yggr

void test_packet_info(void)
{

	yggr::caller::to_params_call(&foo, ::yggr::packet::make_packet_info(10, 20));
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(&foo, ::yggr::packet::make_packet_info(10, 20))));

	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), ::yggr::packet::make_packet_info(10, 20));
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), ::yggr::packet::make_packet_info(10, 20))));

	yggr::shared_ptr<Foo_obj> pfoo_obj = yggr::make_shared<Foo_obj>();
	assert(pfoo_obj);
	Foo_obj& foo_obj_ref = *pfoo_obj;
	const Foo_obj& foo_obj_cref = foo_obj_ref;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), ::yggr::packet::make_packet_info(10, 20));
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), ::yggr::packet::make_packet_info(boost::ref(foo_obj_ref), 10, 20));

	yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, ::yggr::packet::make_packet_info(10, 20));
	yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, ::yggr::packet::make_packet_info(10, 20));

	yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, ::yggr::packet::make_packet_info(10, 20));
	yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_ref, ::yggr::packet::make_packet_info(10, 20));

	//yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_cref, ::yggr::packet::make_packet_info(10, 20));
	yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, ::yggr::packet::make_packet_info(10, 20));

}

void test_tuple(void)
{
	yggr::shared_ptr<Foo_obj> pfoo_obj = yggr::make_shared<Foo_obj>();
	assert(pfoo_obj);
	Foo_obj& foo_obj_ref = *pfoo_obj;
	const Foo_obj& foo_obj_cref = foo_obj_ref;

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		yggr::caller::to_params_call(&foo, std::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(foo, std::make_tuple(10, 20))));

		yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_tuple(10, 20))));

		std::tuple<int, int> tuple_val =  std::make_tuple(10, 20);
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), tuple_val)));

		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), std::make_tuple(10, 20));
		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), std::make_tuple(boost::ref(foo_obj_ref), 10, 20));

		yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, std::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, std::make_tuple(10, 20));

		yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, std::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_ref, std::make_tuple(10, 20));

		//yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_cref, std::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, std::make_tuple(10, 20));
	}

#endif // YGGR_NO_CXX11_HDR_TUPLE

	{
		yggr::caller::to_params_call(&foo, boost::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(foo, boost::make_tuple(10, 20))));

		yggr::caller::to_params_call(boost::bind(&foo, _1, _2), boost::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), boost::make_tuple(10, 20))));

		boost::tuple<int, int> tuple_val =  boost::make_tuple(10, 20);
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), tuple_val)));

		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), boost::make_tuple(10, 20));
		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), boost::make_tuple(boost::ref(foo_obj_ref), 10, 20));

		yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, boost::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, boost::make_tuple(10, 20));

		yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, boost::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_ref, boost::make_tuple(10, 20));

		//yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_cref, boost::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, boost::make_tuple(10, 20));
	}

	{
		yggr::caller::to_params_call(&foo, yggr::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(foo, yggr::make_tuple(10, 20))));

		yggr::caller::to_params_call(boost::bind(&foo, _1, _2), yggr::make_tuple(10, 20));
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), yggr::make_tuple(10, 20))));

		yggr::tuple<int, int> tuple_val =  yggr::make_tuple(10, 20);
		yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), tuple_val)));

		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), yggr::make_tuple(10, 20));
		yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, _1, _2, _3), yggr::make_tuple(boost::ref(foo_obj_ref), 10, 20));

		yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, yggr::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, yggr::make_tuple(10, 20));

		yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, yggr::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_ref, yggr::make_tuple(10, 20));

		//yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_cref, yggr::make_tuple(10, 20));
		yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, yggr::make_tuple(10, 20));
	}

}

void test_pair(void)
{
	yggr::caller::to_params_call(&foo, std::make_pair(10, 20));
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(&foo, std::make_pair(10, 20))));

	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_pair(10, 20));
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), std::make_pair(10, 20))));

	std::pair<int, int> pair_val =  std::make_pair(10, 20);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), pair_val)));

	yggr::shared_ptr<Foo_obj> pfoo_obj = yggr::make_shared<Foo_obj>();
	assert(pfoo_obj);
	Foo_obj& foo_obj_ref = *pfoo_obj;
	const Foo_obj& foo_obj_cref = foo_obj_ref;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::nc_foo, &foo_obj_ref, _1, _2), std::make_pair(10, 20));
	yggr::caller::to_params_call(boost::bind(&Foo_obj::nc_foo1, _1, _2), std::make_pair(boost::ref(foo_obj_ref), 10));

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_cref, _1, _2), std::make_pair(10, 20));
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo1, _1, _2), std::make_pair(boost::ref(foo_obj_cref), 10));

	yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, std::make_pair(10, 20));
	yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, std::make_pair(10, 20));

	yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, std::make_pair(10, 20));
	yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_ref, std::make_pair(10, 20));

	//yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_cref, std::make_pair(10, 20));
	yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, std::make_pair(10, 20));
}

#ifndef YGGR_NO_CXX11_HDR_ARRAY

void test_stl_array(void)
{
	std::array<int, 2> array = {10, 20};
	const std::array<int, 2>& array_cref = array;

	yggr::caller::to_params_call(&foo, array);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(&foo, array_cref)));

	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), array);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), array_cref)));

	yggr::shared_ptr<Foo_obj> pfoo_obj = yggr::make_shared<Foo_obj>();
	assert(pfoo_obj);
	Foo_obj& foo_obj_ref = *pfoo_obj;
	const Foo_obj& foo_obj_cref = foo_obj_ref;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), array);
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), array_cref);

	yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, array);
	yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, array_cref);

	yggr::caller::to_params_call(&Foo_obj::nc_foo, boost::addressof(foo_obj_ref), array);
	yggr::caller::to_params_call(&Foo_obj::foo, boost::addressof(foo_obj_cref), array_cref);

	yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, array_cref);
	yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, array);
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

void test_boost_array(void)
{
	boost::array<int, 2> array = {10, 20};
	const boost::array<int, 2>& array_cref = array;

	yggr::caller::to_params_call(&foo, array);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(&foo, array_cref)));

	yggr::caller::to_params_call(boost::bind(&foo, _1, _2), array);
	yggr_test_assert((10 + 20 == yggr::caller::to_params_call(boost::bind(&foo, _1, _2), array_cref)));

	yggr::shared_ptr<Foo_obj> pfoo_obj = yggr::make_shared<Foo_obj>();
	assert(pfoo_obj);
	Foo_obj& foo_obj_ref = *pfoo_obj;
	const Foo_obj& foo_obj_cref = foo_obj_ref;

	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), array);
	yggr::caller::to_params_call(boost::bind(&Foo_obj::foo, &foo_obj_ref, _1, _2), array_cref);

	yggr::caller::to_params_call(&Foo_obj::nc_foo, pfoo_obj, array);
	yggr::caller::to_params_call(&Foo_obj::foo, pfoo_obj, array_cref);

	yggr::caller::to_params_call(&Foo_obj::nc_foo, boost::addressof(foo_obj_ref), array);
	yggr::caller::to_params_call(&Foo_obj::foo, boost::addressof(foo_obj_cref), array_cref);

	yggr::caller::to_params_call(&Foo_obj::nc_foo, foo_obj_ref, array_cref);
	yggr::caller::to_params_call(&Foo_obj::foo, foo_obj_cref, array);
}

int main(int argc, char* argv[])
{
	test_tuple();
	test_pair();

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	test_stl_array();
#endif // YGGR_NO_CXX11_HDR_ARRAY

	test_boost_array();

	wait_any_key(argc, argv);
	return 0;
}
