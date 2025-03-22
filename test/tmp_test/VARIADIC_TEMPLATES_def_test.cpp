// VARIADIC_TEMPLATES_def_test.cpp

#include <boost/config.hpp>


#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
#	error "aaa"
#endif // BOOST_NO_CXX11_VARIADIC_TEMPLATES


int main(int argc, char* argv[])
{
	return 0;
}