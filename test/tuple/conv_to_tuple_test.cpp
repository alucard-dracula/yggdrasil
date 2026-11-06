// conv_to_tuple_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/tuple_ex/conv_to_tuple.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test_conv_to_yggr_tuple(void)
{
	{
		typedef yggr::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT_NOT((boost::is_same<tuple_t, conv_t>));
	}

	{
		typedef yggr::tuple<> tuple_t;
		typedef boost::mpl::vector<> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple();
	}

	{
		typedef yggr::tuple<int> tuple_t;
		typedef boost::mpl::vector<int> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple(10);
	}

	{
		typedef yggr::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple(10, 20);
	}

	{
		typedef yggr::tuple<int, int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple(10, 20, 30);
	}

	std::cout << "test_conv_to_tuple sucess" << std::endl;
}

void test_conv_to_boost_tuple(void)
{
	{
		typedef boost::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT_NOT((boost::is_same<tuple_t, conv_t>));
	}

	{
		typedef boost::tuple<> tuple_t;
		typedef boost::mpl::vector<> vt_t;
		typedef yggr::tuple_ex::conv_to_boost_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple();
	}

	{
		typedef boost::tuple<int> tuple_t;
		typedef boost::mpl::vector<int> vt_t;
		typedef yggr::tuple_ex::conv_to_boost_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple(10);
	}

	{
		typedef boost::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_boost_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple(10, 20);
	}

	{
		typedef boost::tuple<int, int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_boost_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple(10, 20, 30);
	}

	typedef boost::mpl::vector<int, int, int, boost::tuples::null_type> org_vt1_type;
	typedef boost::mpl::vector<int, int, int> org_vt2_type;
	typedef boost::mpl::find<org_vt1_type, boost::tuples::null_type>::type iter1_type;
	typedef boost::mpl::find<org_vt2_type, boost::tuples::null_type>::type iter2_type;
	typedef boost::mpl::erase<org_vt1_type, iter1_type, boost::mpl::end<org_vt1_type>::type >::type vt1_type;
	typedef boost::mpl::erase<org_vt2_type, iter2_type, boost::mpl::end<org_vt2_type>::type >::type vt2_type;

	BOOST_MPL_ASSERT((boost::is_same<vt1_type, vt1_type>));
	std::cout << "test_conv_to_boost_tuple sucess" << std::endl;
}

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

void test_conv_to_stl_tuple(void)
{
	{
		typedef std::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT_NOT((boost::is_same<tuple_t, conv_t>));
	}

	{
		typedef std::tuple<> tuple_t;
		typedef boost::mpl::vector<> vt_t;
		typedef yggr::tuple_ex::conv_to_stl_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = std::make_tuple();
	}

	{
		typedef std::tuple<int> tuple_t;
		typedef boost::mpl::vector<int> vt_t;
		typedef yggr::tuple_ex::conv_to_stl_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = std::make_tuple(10);
	}

	{
		typedef std::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_stl_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = std::make_tuple(10, 20);
	}

	{
		typedef std::tuple<int, int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::tuple_ex::conv_to_stl_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = std::make_tuple(10, 20, 30);
	}

	std::cout << "test_conv_to_tuple sucess" << std::endl;
}

#endif // YGGR_NO_CXX11_HDR_TUPLE

void test_other(void)
{
	typedef boost::mpl::vector<int, float, char>::type vt_type;
	typedef yggr::tuple<int, float, char> yggr_tuple_type;
	typedef boost::tuple<int, float, char> boost_tuple_type;

	typedef yggr::tuple_ex::conv_to_tuple<yggr::tuple, vt_type>::type yggr_chk_type;
	typedef yggr::tuple_ex::conv_to_yggr_tuple<vt_type>::type yggr_chk2_type;

	typedef yggr::tuple_ex::conv_to_tuple<boost::tuple, vt_type>::type boost_chk_type;
	typedef yggr::tuple_ex::conv_to_boost_tuple<vt_type>::type boost_chk2_type;

	BOOST_MPL_ASSERT((boost::is_same<yggr_tuple_type, yggr_chk_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr_tuple_type, yggr_chk2_type>));

	BOOST_MPL_ASSERT((boost::is_same<boost_tuple_type, boost_chk_type>));
	BOOST_MPL_ASSERT((boost::is_same<boost_tuple_type, boost_chk2_type>));
}

int main(int argc, char* argv[])
{
	test_conv_to_yggr_tuple();
	test_conv_to_boost_tuple();

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	test_conv_to_stl_tuple();
#endif // YGGR_NO_CXX11_HDR_TUPLE

	test_other();

	wait_any_key(argc, argv);
	return 0;
}