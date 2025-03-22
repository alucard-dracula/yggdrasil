//typename_caster_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <list>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/mplex/typename_caster.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	typedef std::vector<int> vt_type;
	typedef std::list<int> chk_type;

	typedef yggr::mplex::typename_caster<vt_type> caster_type;
	typedef caster_type::apply<std::list>::type list_type;

	BOOST_MPL_ASSERT((boost::is_same<list_type, chk_type>));

	wait_any_key(argc, argv);
	return 0;
}