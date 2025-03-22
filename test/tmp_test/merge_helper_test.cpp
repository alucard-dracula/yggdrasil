// merge_helper_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <iostream>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace b
{

template<typename T>
struct cal
{
	typedef T type;
	
	inline type operator()(T& a, T& b) const
	{
#if 1
		using ::a::add;
		return add(a, b);
#else
		return a::add(a, b);
#endif // 0,1 
	}
};

template<typename T>
void call_add(void)
{
	typedef cal<T> cal_type;
	cal_type cal;
	T a = 10, b = 20;
	std::cout << cal(a, b) << std::endl;
}


} // namespace b

namespace a
{

template<typename T>
typename
	boost::enable_if
	<
		boost::is_same<T, int>,
		T
	>::type
	add(T& a, T& b)
{
	return a + b;
}

} // namespace a

//namespace b
//{
//
//template<typename T>
//struct cal
//{
//	typedef T type;
//	
//	inline type operator()(T& a, T& b) const
//	{
//#if 1
//		using ::a::add;
//		return add(a, b);
//#else
//		return a::add(a, b);
//#endif // 0,1 
//	}
//};
//
//template<typename T>
//void call_add(void)
//{
//	typedef cal<T> cal_type;
//	cal_type cal;
//	T a = 10, b = 20;
//	std::cout << cal(a, b) << std::endl;
//}
//
//
//} // namespace b


namespace a
{

template<typename T>
typename
	boost::disable_if
	<
		boost::is_same<T, int>,
		T
	>::type
	add(T& a, T& b)
{
	return a + b;
}

} // namespace a



int main(int argc, char* argv[])
{
	b::call_add<float>();

	wait_any_key(argc, argv);
	return 0;
}