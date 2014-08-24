//tmp_test.cpp

#include <iostream>

#include <yggr/ppex/concat.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/ppex/pp_debug.hpp>

//#include <yggr/log/yggr_exception_log_accesser.hpp>

//#define CONCAT(...) __VA_ARGS__

#define _YGGR_PP_TEMPLATE_TYPE( __class_name__, __params__ ) \
			__class_name__ < __params__ >

#define _YGGR_PP_TEMPLATE_PARAMS_TYPES( __count__, __class_name__ ) \
			 BOOST_PP_ENUM_PARAMS( __count__, __class_name__ )


int main(int argc, char* argv[])
{

    std::cout << YGGR_PP_DEBUG(YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T )) << std::endl;
    return 0;
}
