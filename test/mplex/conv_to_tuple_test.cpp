// conv_to_tuple_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/mplex/conv_to_tuple.hpp>
#include <yggr/tuple/tuple.hpp>
#include <boost/tuple/tuple.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
	{
		typedef boost::mpl::vector<int, float, char>::type vt_type;
		typedef yggr::tuple<int, float, char> yggr_tuple_type;
		typedef boost::tuple<int, float, char> boost_tuple_type;

		typedef yggr::mplex::conv_to_tuple<yggr::tuple, vt_type>::type yggr_chk_type;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_type>::type yggr_chk2_type;

		typedef yggr::mplex::conv_to_tuple<boost::tuple, vt_type>::type boost_chk_type;
		typedef yggr::mplex::conv_to_boost_tuple<vt_type>::type boost_chk2_type;

		BOOST_MPL_ASSERT((boost::is_same<yggr_tuple_type, yggr_chk_type>));
		BOOST_MPL_ASSERT((boost::is_same<yggr_tuple_type, yggr_chk2_type>));

		BOOST_MPL_ASSERT((boost::is_same<boost_tuple_type, boost_chk_type>));
		BOOST_MPL_ASSERT((boost::is_same<boost_tuple_type, boost_chk2_type>));
	}

	wait_any_key(argc, argv);
	return 0;
}