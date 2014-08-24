//packet_info_test.cpp

#include <iostream>

#include <cassert>
#include <functional>

#include <yggr/mplex/conv_to_tuple.hpp>
#include <yggr/struct_support/tuple.hpp>
#include <yggr/packet/packet_info.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_packet_info_op1(void)
{
	typedef yggr::packet::packet_info<int, int, int> pak_info_type;

	pak_info_type info(1, 2, 3);

	assert((info == boost::make_tuple(1, 2, 3)));
	assert((boost::make_tuple(1, 2, 3) == info));
	assert( !(boost::make_tuple(1, 2, 3) != info));

	assert( (true == yggr::packet::packet_info_op::compare<0, 1>(info, 1, 2)) );
	assert( (false == yggr::packet::packet_info_op::compare<0, 2>(info, 1, 2)) );
	assert( (true == yggr::packet::packet_info_op::compare<0, 1, 2>(info, 1, 2, 3)) );
	assert( (true == yggr::packet::packet_info_op::compare<0, 1>(info, 1, 2)) );

	assert( (boost::make_tuple(1, 2) == yggr::packet::packet_info_op::get_sub_infos<0, 1>(info)) );
	assert( (boost::make_tuple(1, 2) != yggr::packet::packet_info_op::get_sub_infos<0, 2>(info)) );
	std::cout << yggr::packet::packet_info_op::get_sub_infos<0, 2>(info) << std::endl;
	std::cout << info << std::endl;
	std::cout << yggr::packet::packet_info_op::get_sub_info<0, 2>(info) << std::endl;

	std::equal_to<pak_info_type> eq;
	assert(eq(pak_info_type(1, 2, 3), pak_info_type(1, 2, 3)));

#ifdef _MSC_VER
	std::cout << pak_info_type(5, 6, 7) << std::endl;
	std::wcout.imbue(std::locale("chs"));
	std::wcout << pak_info_type(5, 6, 7) << std::endl;
#endif //_MSC_VER
}

void test_packet_info(void)
{
	typedef yggr::packet::packet_info<int> packet_info_type;
	packet_info_type pak_info;

	pak_info.get<0>() = 100;
	yggr::packet::packet_info<int> pak_info2(boost::move(pak_info));
	assert(pak_info2.get<0>() == 100);

	packet_info_type pak_info3;
	pak_info2.swap(pak_info3);

	assert(pak_info2.get<0>() == 0);
	assert(pak_info3.get<0>() == 100);

	boost::swap(pak_info2, pak_info3);
	assert(pak_info2.get<0>() == 100);
	assert(pak_info3.get<0>() == 0);

	//pak_info2.get<3>(); //compile assert error
}

void test_packet_info2(void)
{
	yggr::packet::packet_info<int, int> pak_info;

	pak_info.get<0>() = 100;
	pak_info.get<1>() = 200;
	yggr::packet::packet_info<int, int> pak_info2(boost::move(pak_info));
	assert(pak_info2.get<0>() == 100);

	yggr::packet::packet_info<int, int> pak_info3;

	pak_info2.swap(pak_info3);

	assert(pak_info2.get<0>() == 0);
	assert(pak_info2.t_get<int>() == 0);

	assert(pak_info3.get<0>() == 100);

	boost::swap(pak_info3, pak_info2);
	assert(pak_info3.get<0>() == 0);
	assert(pak_info2.get<0>() == 100);
}

void test_packet_info3(void)
{
	yggr::packet::packet_info<> pak_info;
	assert(yggr::packet::packet_info<>::E_length == 0);
}

void test_conv_to_tuple(void)
{
	// faile
	/*{
		typedef boost::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::mplex::conv_to_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
	}*/

	{
		typedef boost::tuple<> tuple_t;
		typedef boost::mpl::vector<> vt_t;
		typedef yggr::mplex::conv_to_tuple<vt_t>::type conv_t;
		BOOST_MPL_ASSERT((boost::is_same<tuple_t, conv_t>));
		conv_t tuple = boost::make_tuple();
	}

	{
		typedef boost::tuple<int> tuple_t;
		typedef boost::mpl::vector<int> vt_t;
		typedef yggr::mplex::conv_to_tuple<vt_t>::type conv_t;
		conv_t tuple = boost::make_tuple(10);
	}

	{
		typedef boost::tuple<int, int> tuple_t;
		typedef boost::mpl::vector<int, int> vt_t;
		typedef yggr::mplex::conv_to_tuple<vt_t>::type conv_t;
		conv_t tuple = boost::make_tuple(10, 20);
	}

	{
		typedef boost::tuple<int, int, int> tuple_t;
		typedef boost::mpl::vector<int, int, int> vt_t;
		typedef yggr::mplex::conv_to_tuple<vt_t>::type conv_t;

		conv_t tuple = boost::make_tuple(10, 20, 30);
	}

	typedef boost::mpl::vector<int, int, int, boost::tuples::null_type> org_vt1_type;
	typedef boost::mpl::vector<int, int, int> org_vt2_type;
	typedef boost::mpl::find<org_vt1_type, boost::tuples::null_type>::type iter1_type;
	typedef boost::mpl::find<org_vt2_type, boost::tuples::null_type>::type iter2_type;
	typedef boost::mpl::erase<org_vt1_type, iter1_type, boost::mpl::end<org_vt1_type>::type >::type vt1_type;
	typedef boost::mpl::erase<org_vt2_type, iter2_type, boost::mpl::end<org_vt2_type>::type >::type vt2_type;

	BOOST_MPL_ASSERT((boost::is_same<vt1_type, vt1_type>));
	std::cout << "test_conv_to_tuple sucess" << std::endl;
}

#include <yggr/ppex/pp_debug.hpp>
#include <boost/mpl/push_back.hpp>
int main(int argc, char* argv[])
{
	test_conv_to_tuple();
	test_packet_info();
	test_packet_info2();
	test_packet_info3();
	test_packet_info_op1();

	boost::tuple<int, int> t_type;

    typedef boost::mpl::vector0<> vt0_type;
    typedef boost::mpl::push_back<vt0_type, int>::type vt1_type;
    typedef boost::mpl::push_back<vt1_type, int>::type vt2_type;

	typedef boost::mpl::find<vt2_type, float>::type i; 
    typedef yggr::mplex::conv_to_tuple<vt2_type>::type vtt_type;

	typedef boost::mpl::erase<vt2_type, i, boost::mpl::end<vt2_type>::type>::type vt3_type;
	std::cout << (boost::mpl::size<vt3_type>::value) << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
