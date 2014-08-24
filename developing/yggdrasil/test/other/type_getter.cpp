//type_getter.cpp

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>

struct type_defs
{
	enum
	{
		E_int = 1,
		E_long = 3,
		E_long_long = 6
	};
};

template<int>
struct type_getter;

template<typename T>
struct type_val_getter;


#define TYPE_MAKE(__type__, __val__) \
	template<> \
	struct type_getter<__val__> { \
		enum { value = __val__ }; \
		typedef __type__ type;  }; \
	\
	template<> \
	struct type_val_getter<__type__> { \
		enum { value = __val__ }; \
		typedef __type__ type; };



#include <iostream>

TYPE_MAKE(int, type_defs::E_int)
TYPE_MAKE(long, type_defs::E_long)
TYPE_MAKE(long long, type_defs::E_long_long)

int main(int argc, char* argv[])
{
	BOOST_MPL_ASSERT((boost::is_same<int, type_getter<1>::type>));
	BOOST_MPL_ASSERT((boost::is_same<long, type_getter<3>::type>));
	BOOST_MPL_ASSERT((boost::is_same<long long, type_getter<6>::type>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<(1 == type_val_getter<int>::value)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(3 == type_val_getter<long>::value)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(6 == type_val_getter<long long>::value)>));

	BOOST_MPL_ASSERT((boost::is_same<int, type_getter<type_val_getter<int>::value>::type>));
	BOOST_MPL_ASSERT((boost::is_same<long, type_getter<type_val_getter<long>::value>::type>));
	BOOST_MPL_ASSERT((boost::is_same<long long, type_getter<type_val_getter<long long>::value>::type>));
	char cc= 0;
	std::cin >> cc;
	return 0;
}