//packet_info_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <functional>

#include <yggr/packet/packet_info.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_packet_info_op1(void)
{
	typedef yggr::packet::packet_info<int, int, int> pak_info_type;
	typedef yggr::tuple<int, int> cmp2_value_type;
	typedef yggr::tuple<int, int, int> cmp3_value_type;

	pak_info_type info(1, 2, 3);

	yggr_test_assert((info == boost::make_tuple(1, 2, 3)));
	yggr_test_assert((boost::make_tuple(1, 2, 3) == info));
	yggr_test_assert( !(boost::make_tuple(1, 2, 3) != info));

	yggr_test_assert( (true == yggr::packet::packet_info_op::compare<0, 1>(info, 1, 2)) );
	yggr_test_assert( (false == yggr::packet::packet_info_op::compare<0, 2>(info, 1, 2)) );
	yggr_test_assert( (true == yggr::packet::packet_info_op::compare<0, 1, 2>(info, 1, 2, 3)) );
	yggr_test_assert( (true == yggr::packet::packet_info_op::compare<0, 1>(info, 1, 2)) );

	yggr_test_assert( (true == yggr::packet::packet_info_op::compare<0, 1>(std::equal_to<cmp2_value_type>(), info, 1, 2)) );
	yggr_test_assert( (false == yggr::packet::packet_info_op::compare<0, 2>(std::equal_to<cmp2_value_type>(), info, 1, 2)) );
	yggr_test_assert( (true == yggr::packet::packet_info_op::compare<0, 1, 2>(std::equal_to<cmp3_value_type>(), info, 1, 2, 3)) );
	yggr_test_assert( (true == yggr::packet::packet_info_op::compare<0, 1>(std::equal_to<cmp2_value_type>(), info, 1, 2)) );

	yggr_test_assert( (boost::make_tuple(1, 2) == yggr::packet::packet_info_op::get_sub_infos<0, 1>(info)) );
	yggr_test_assert( (yggr::make_tuple(1, 2) == yggr::packet::packet_info_op::get_sub_infos<0, 1>(info)) );

	yggr_test_assert( (boost::make_tuple(1, 2) != yggr::packet::packet_info_op::get_sub_infos<0, 2>(info)) );
	yggr_test_assert( (yggr::make_tuple(1, 2) != yggr::packet::packet_info_op::get_sub_infos<0, 2>(info)) );

	std::cout << yggr::packet::packet_info_op::get_sub_infos<0, 2>(info) << std::endl;
	std::cout << info << std::endl;
	std::cout << yggr::packet::packet_info_op::get_sub_packet_info<0, 2>(info) << std::endl;

	{
		std::equal_to<pak_info_type> eq;
		bool bchk = eq(pak_info_type(1, 2, 3), pak_info_type(1, 2, 3));
		yggr_test_assert(bchk);
	}

	std::cout << pak_info_type(5, 6, 7) << std::endl;
	
#ifndef YGGR_NO_CWCHAR
	std::wcout << pak_info_type(5, 6, 7) << std::endl;
#endif // #ifndef YGGR_NO_CWCHAR
}

void test_packet_info(void)
{
	typedef yggr::packet::packet_info<int> packet_info_type;
	packet_info_type pak_info;

	pak_info.get<0>() = 100;
	yggr::packet::packet_info<int> pak_info2(boost::move(pak_info));
	yggr_test_assert(pak_info2.get<0>() == 100);

	packet_info_type pak_info3;
	pak_info2.swap(pak_info3);

	yggr_test_assert(pak_info2.get<0>() == 0);
	yggr_test_assert(pak_info3.get<0>() == 100);

	std::swap(static_cast<packet_info_type::base_type&>(pak_info2), 
					static_cast<packet_info_type::base_type&>(pak_info3));
	boost::swap(static_cast<packet_info_type::base_type&>(pak_info2), 
					static_cast<packet_info_type::base_type&>(pak_info3));
	yggr::swap(static_cast<packet_info_type::base_type&>(pak_info2), 
					static_cast<packet_info_type::base_type&>(pak_info3));
	yggr_test_assert(pak_info2.get<0>() == 100);
	yggr_test_assert(pak_info3.get<0>() == 0);


	//pak_info2.get<3>(); //compile assert error
}

void test_packet_info2(void)
{
	yggr::packet::packet_info<int, int> pak_info;

	pak_info.get<0>() = 100;
	pak_info.get<1>() = 200;
	yggr::packet::packet_info<int, int> pak_info2(boost::move(pak_info));
	yggr_test_assert(pak_info2.get<0>() == 100);

	yggr::packet::packet_info<int, int> pak_info3;

	pak_info2.swap(pak_info3);

	yggr_test_assert(pak_info2.get<0>() == 0);
	yggr_test_assert(pak_info2.get<int>() == 0);

	yggr_test_assert(pak_info3.get<0>() == 100);

	yggr::swap(pak_info3, pak_info2);
	yggr_test_assert(pak_info3.get<0>() == 0);
	yggr_test_assert(pak_info2.get<0>() == 100);
}

void test_packet_info3(void)
{
	yggr::packet::packet_info<> pak_info;
	yggr_test_assert(yggr::packet::packet_info<>::E_length == 0);
}

void test_conv_to_tuple(void)
{
	{
		typedef yggr::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT_NOT((boost::is_same<tuple_t, conv_t>));
	}

	{
		typedef yggr::tuple<> tuple_t;
		typedef boost::mpl::vector<> vt_t;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple();
	}

	{
		typedef yggr::tuple<int> tuple_t;
		typedef boost::mpl::vector<int> vt_t;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_t>::type conv_t;
		conv_t tuple = boost::make_tuple(10);
	}

	{
		typedef yggr::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int> vt_t;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_t>::type conv_t;
		conv_t tuple = boost::make_tuple(10, 20);
	}

	{
		typedef yggr::tuple<int, int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_t>::type conv_t;

		conv_t tuple = boost::make_tuple(10, 20, 30);
	}

	std::cout << "test_conv_to_tuple sucess" << std::endl;
}

void test_conv_to_boost_tuple(void)
{
	{
		typedef boost::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::mplex::conv_to_yggr_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT_NOT((boost::is_same<tuple_t, conv_t>));
	}

	{
		typedef boost::tuple<> tuple_t;
		typedef boost::mpl::vector<> vt_t;
		typedef yggr::mplex::conv_to_boost_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple();
	}

	{
		typedef boost::tuple<int> tuple_t;
		typedef boost::mpl::vector<int> vt_t;
		typedef yggr::mplex::conv_to_boost_tuple<vt_t>::type conv_t;
		conv_t tuple = boost::make_tuple(10);
	}

	{
		typedef boost::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int> vt_t;
		typedef yggr::mplex::conv_to_boost_tuple<vt_t>::type conv_t;
		conv_t tuple = boost::make_tuple(10, 20);
	}

	{
		typedef boost::tuple<int, int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::mplex::conv_to_boost_tuple<vt_t>::type conv_t;

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

//#include <yggr/ppex/pp_debug.hpp>
#include <boost/mpl/push_back.hpp>
int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_conv_to_tuple();
	test_conv_to_boost_tuple();

	test_packet_info();
	test_packet_info2();
	test_packet_info3();
	test_packet_info_op1();

	boost::tuple<int, int> t_type;

	typedef boost::mpl::vector0<> vt0_type;
	typedef boost::mpl::push_back<vt0_type, int>::type vt1_type;
	typedef boost::mpl::push_back<vt1_type, int>::type vt2_type;

	typedef boost::mpl::find<vt2_type, float>::type i; 
	typedef yggr::mplex::conv_to_yggr_tuple<vt2_type>::type vtt_type;

	//typedef boost::mpl::erase<vt2_type, i, boost::mpl::end<vt2_type>::type>::type vt3_type;
	//std::cout << (boost::mpl::size<vt3_type>::value) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
