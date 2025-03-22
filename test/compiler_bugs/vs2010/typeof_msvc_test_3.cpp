//tmp_test_3.cpp

#include <iostream>

#ifdef _MSC_VER

#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>

struct default_param {};

template<typename ID, typename T = default_param>
struct extract;

template<typename ID>
struct extract<ID, default_param>
{
	template<bool>
	struct imp_t;

	typedef imp_t<true> type;
};

template<typename ID, typename T>
struct extract : extract<ID, default_param>
{
	template<>
	struct imp_t<true>
	{
		typedef T type;
	};

	template<bool>
	struct imp_t;

	typedef imp_t<true> type;

};

template<typename ID, typename T>
struct register_t : public extract<ID, T>
{
};

template<typename T>
struct encode_t
{
	enum
	{
		value = 5
	};
	typedef typename register_t<boost::mpl::int_<value>, T>::type type;
};

template<typename T>
struct sizer
{
	typedef char (*type)[encode_t<T>::value];
};

template<typename T>
typename sizer<T>::type start_encode(const T&);

template<int ID>
struct type_of_wrap
{
	typedef extract<boost::mpl::int_<ID> > ex_type;
	typedef typename ex_type::type impl_type;
	typedef typename impl_type::type type;
};

//-------------------test------------------
struct A
{
};

A foo(void)
{
	return A();
}

int main()
{

#define TEST_NUM 0

	int n = 10;
	float f = 10;
	A a = {};

	// 由于例题里encode_t::value 定死是5 所以这里不同类型不能同时测试，只能每次测试一个 否则会出现extract重复定义
#if TEST_NUM == 0
	std::cout << typeid(type_of_wrap<sizeof(*start_encode(n))>::type).name() << std::endl;
#elif TEST_NUM == 1
	std::cout << typeid(type_of_wrap<sizeof(*start_encode(f))>::type).name() << std::endl;
#elif TEST_NUM == 2
	std::cout << typeid(type_of_wrap<sizeof(*start_encode(a))>::type).name() << std::endl;
#else
	std::cout << typeid(type_of_wrap<sizeof(*start_encode(foo()))>::type).name() << std::endl;
#endif // 1

	char cc = 0;
	std::cin >> cc;
	return 0;
}

#else

int main(void)
{
	 std::cout << "now test only using in msvc" << std::endl;
}

#endif // _MSC_VER