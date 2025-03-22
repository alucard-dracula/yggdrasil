//test_c3855.cpp

#include <iostream>
#include <vector>

#include <map>

template<typename T1, typename T2, typename T3,
            template<typename _T1, typename _T2> class Cont = std::vector>
class s_vector
    : public Cont<T1, T2>
{
public:
    s_vector() {}
    ~s_vector() {}
};

template<typename T>
struct helper
{
	typedef T type;
	void operator()(const type& t) const
	{
		std::cout << "helper_native" << std::endl;
	}
};

// 这个是 s_vector使用的helper 单独使用是没问题
template<typename T1, typename T2, typename T3,
			template<typename _T1, typename _T2> class IC,
			template<typename _T1, typename _T2, typename _T3,
						template<typename __T1, typename __T2> class _IC> class C >
struct helper< C<T1, T2, T3, IC> >
{
	typedef C<T1, T2, T3, IC> type;
	void operator()(const type& t) const
	{
		std::cout << "helper_s_vector" << std::endl;
	}
};

// vc必须在这里用上 s_vector的实际模板才能一起使用 GCC 就没有必要这样写
//template<typename T1, typename T2, typename T3,
//			template<typename _T1, typename _T2> class IC >
//struct helper< s_vector<T1, T2, T3, IC> >
//{
//	typedef s_vector<T1, T2, T3, IC> type;
//	void operator()(const type& t) const
//	{
//		std::cout << "helper2" << std::endl;
//	}
//};

// 这个是 map使用的helper 单独使用是没问题的
template<typename T1, typename T2, typename T3, typename T4,
			template<typename _T1, typename _T2, typename _T3, typename T4> class C>
struct helper< C<T1, T2, T3, T4> >
{
	typedef C<T1, T2, T3, T4> type;
	void operator()(const type& t) const
	{
		std::cout << "helper_map" << std::endl;
	}
};

// 这个是测试函数
template<typename T>
void test(const T& t)
{
    typedef helper<T> helper_type;
    helper_type h;
    h(t);
}

template<template<typename _T1, typename _T2, typename _T3,
							template<typename __T1, typename __T2> class __C > class IC,
				typename IT1, typename IT2, typename IT3,
				template<typename _T1, typename _T2> class IIC
				>
void test2(const IC<IT1, IT2, IT3, IIC>& val)
{
	std::cout << "test2 svt" << std::endl;
}

template<template<typename _T1, typename _T2, typename _T3, typename _T4> class IC,
			typename IT1, typename IT2, typename IT3, typename IT4
			>
void test2(const IC<IT1, IT2, IT3, IT4>& val)
{
	std::cout << "test2 map" << std::endl;
}

int main(int argc, char* argv[])
{
	typedef std::map<int, int> map_type;
	map_type map;
	test(map);

	typedef s_vector<int, std::allocator<int>, int> s_vt_type;
	s_vt_type s_vt;
	test(s_vt);

	char cc = 0;
	std::cin >> cc;
	return 0;
}
