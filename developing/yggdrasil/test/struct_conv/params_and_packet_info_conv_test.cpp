//params_and_packet_info_conv_test.cpp

#include <cassert>
#include <iostream>
#include <utility>
#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/any_val/any_val_operator.hpp>
#include <yggr/struct_conv/conv_params_and_packet_info.hpp>

#include <yggr/ppex/pp_debug.hpp>

typedef yggr::any_val::any_val_operator<yggr::any_val::base_any_type_operator> any_val_op_type;
typedef yggr::any_val::params<std::string, any_val_op_type> params_type;

typedef yggr::struct_conv::conv_params_and_packet_info conv_type;

void test_params_to_packet_info(void)
{
	//std::cout << ( YGGR_PP_DEBUG((BOOST_PP_LOCAL_MACRO(1))) ) << std::endl;
	typedef yggr::packet::packet_info<
				std::pair<std::string, int>,
				std::pair<std::string, int>,
				std::pair<std::string, int> > packet_info_type;

	params_type params;
	params["val1"] = ANY_SET(int, 1);
	params["val2"] = ANY_SET(int, 2);
	params["val3"] = ANY_SET(int, 3);

	packet_info_type pak_info(
			conv_type::s_conv<int, int, int, std::pair>(
				std::string("val1"), std::string("val2"), std::string("val3"),
				params));

	assert(pak_info.get<0>().first == "val1" && pak_info.get<0>().second == 1);
	assert(pak_info.get<1>().first == "val2" && pak_info.get<1>().second == 2);
	assert(pak_info.get<2>().first == "val3" && pak_info.get<2>().second == 3);

}

void test_packet_info_to_params(void)
{
	//std::cout << ( YGGR_PP_DEBUG((BOOST_PP_LOCAL_MACRO(2))) ) << std::endl;

	typedef std::pair<std::string, int> pair_type;
	typedef yggr::packet::packet_info< pair_type, pair_type > packet_info_type;

	packet_info_type pak_info(pair_type("val1", 1), pair_type("val2", 2));

	params_type params(conv_type::s_conv<params_type>(pak_info));

	assert(params.get_val<int>("val1") == 1);
	assert(params.get_val<int>("val2") == 2);

}

int main(int argc, char* argv[])
{
	test_params_to_packet_info();
	test_packet_info_to_params();
	std::cout << "completed" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
