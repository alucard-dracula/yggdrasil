//typename_caster_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/mplex/typename_caster.hpp>
#include <yggr/tuple/tuple.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <vector>
#include <list>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T>
struct tuple_elem_add_reference
	: public
		boost::mpl::if_
		<
			boost::is_same<yggr::tuples::null_type, T>,
			T,
			typename boost::add_reference<T>::type
		>
{
};

int main(int argc, char* argv[])
{
	{
		typedef std::vector<int> src_type;
		typedef std::list<int> chk_type;

		typedef yggr::mplex::typename_caster<src_type> caster_type;
		typedef caster_type::apply<std::list>::type dst_type;

		BOOST_MPL_ASSERT((boost::is_same<dst_type, chk_type>));
	}

	{
		typedef yggr::tuple<int, int> src_type;
		typedef yggr::tuple<int&, int&> chk_type;

		typedef yggr::mplex::typename_caster<src_type> caster_type;
		typedef caster_type::apply<yggr::tuple, tuple_elem_add_reference>::type dst_type;

		BOOST_MPL_ASSERT((boost::is_same<dst_type, chk_type>));
	}

	wait_any_key(argc, argv);
	return 0;
}