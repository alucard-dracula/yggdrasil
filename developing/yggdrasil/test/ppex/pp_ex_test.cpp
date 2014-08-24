//pp_ex_test.cpp

#include <iostream>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/pp_debug.hpp>

#include <boost/preprocessor/punctuation/comma_if.hpp>

struct A
{
	const A& operator()(void) const
	{
		return *this;
	}


#	define BOOST_PP_LOCAL_MACRO(n)  \
	template< YGGR_PP_FOO_TYPES_DEF(n) > \
	const A& operator()(YGGR_PP_FOO_PARAMS_DEF( n, YGGR_PP_FOO_CREF_PARAMS )) const { \
							std::cout << (YGGR_PP_FOO_PARAMS_OP(n, YGGR_PP_SYMBOL_PLUS)) << std::endl; \
							return *this; }

#	define YGGR_PP_FOO_ARG_NAME() aaa
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

};


struct B
{
	int operator+(int n) const
	{
		return n;
	}

	int operator+(const B& right) const
	{
		return 0;
	}
};

int operator+(int n, const B& b)
{
	return n;
}

int main(int argc, char* argv[])
{
	A a;
	a(1)
		()
		(1, 2)
		(1, 2, 3)
		(1, 2, 3, B())
		(1, 2, B(), 4, B())
		(1, 2, 3, 4, 5);


	char cc = 0;
	std::cin >> cc;
	return 0;
}
