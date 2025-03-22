// typename_swapper_test.hpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <utility>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/pair.hpp>

#include <yggr/mplex/typename_swapper.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename L, typename R> inline
void test(const L& l, const R& r)
{
	typedef L l_type;
	typedef R r_type;
	typedef typename yggr::mplex::typename_swapper<l_type>::type chk_type;
	BOOST_MPL_ASSERT((boost::is_same<r_type, chk_type>));
}


int main(int argc, char* argv[])
{

	typedef std::pair<int, float> l0_type;
	typedef std::pair<float, int> r0_type;

	typedef boost::mpl::pair<int, float> l1_type;
	typedef boost::mpl::pair<float, int> r1_type;

	test(l0_type(), r0_type());
	test(l1_type(), r1_type());

	wait_any_key(argc, argv);
	return 0;
}