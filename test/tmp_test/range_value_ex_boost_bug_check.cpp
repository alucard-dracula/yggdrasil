//time_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <boost/range/as_literal.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
	typedef yggr::range_ex::range_value_ex<const char*>::type fmt_str_char_cv_type;

	BOOST_MPL_ASSERT((boost::is_same<char, fmt_str_char_cv_type>));

	wait_any_key(argc, argv);
	return 0;
}
