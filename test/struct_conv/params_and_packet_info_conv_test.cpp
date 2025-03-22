//params_and_packet_info_conv_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <utility>

#include <yggr/any_val/params.hpp>
#include <yggr/any_val/any_operator_mgr_basic_t_reg.hpp>
#include <yggr/struct_conv/conv_params_and_packet_info.hpp>

//#include <yggr/ppex/pp_debug.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(any_val)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::any_val::params<std::string> params_type;

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
			conv_type::conv<std::pair, int, int, int>(
				params,
				std::string("val1"), std::string("val2"), std::string("val3") ));

	yggr_test_assert(pak_info.get<0>().first == "val1" && pak_info.get<0>().second == 1);
	yggr_test_assert(pak_info.get<1>().first == "val2" && pak_info.get<1>().second == 2);
	yggr_test_assert(pak_info.get<2>().first == "val3" && pak_info.get<2>().second == 3);

	std::cout << (pak_info.get<0>().first == "val1" && pak_info.get<0>().second == 1) << std::endl;
	std::cout << (pak_info.get<1>().first == "val2" && pak_info.get<1>().second == 2) << std::endl;
	std::cout << (pak_info.get<2>().first == "val3" && pak_info.get<2>().second == 3) << std::endl;

}

void test_packet_info_to_params(void)
{
	//std::cout << ( YGGR_PP_DEBUG((BOOST_PP_LOCAL_MACRO(2))) ) << std::endl;

	typedef std::pair<std::string, int> pair_type;
	typedef yggr::packet::packet_info< pair_type, pair_type > packet_info_type;

	packet_info_type pak_info(pair_type("val1", 1), pair_type("val2", 2));

	params_type params(conv_type::conv<params_type>(pak_info));

	yggr_test_assert(params.get_value<int>("val1") == 1);
	yggr_test_assert(params.get_value<int>("val2") == 2);

	std::cout << (params.get_value<int>("val1") == 1) << std::endl;
	std::cout << (params.get_value<int>("val2") == 2) << std::endl;

}

int main(int argc, char* argv[])
{
	test_params_to_packet_info();
	test_packet_info_to_params();

	wait_any_key(argc, argv);
	return 0;
}
