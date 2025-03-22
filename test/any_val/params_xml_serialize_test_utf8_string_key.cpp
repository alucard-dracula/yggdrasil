//params_xml_serialization.hpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <string>

#include <boost/container/string.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/any_val/params.hpp>
#include <yggr/any_val/any_operator_mgr_basic_t_reg.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/any.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(any_val)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;

typedef yggr::any_val::params<yggr::utf8_string> params_type;

//YGGR_ANY_EXPORT(int)
YGGR_ANY_EXPORT_DECLEAR(int)
YGGR_ANY_EXPORT_IMPL(int)

YGGR_ANY_EXPORT(float)

YGGR_ANY_EXPORT(std::string)
YGGR_ANY_EXPORT(boost::container::string)
YGGR_ANY_EXPORT(yggr::utf8_string)

void test_foo1(void)
{
	
	{
		// check conflict
		std::stringstream ss;
		ss << boost::container::string("abc");
		std::cout << boost::container::string("aaa") << std::endl;
	}
	
	params_type params1;

	params1["val1"] = ANY_SET(int, 10);
	params1["val2"] = ANY_SET(float, 100.5f);
	params1["val3"] = std::string("val3");
	params1["val4"] = boost::container::string("val4");
	params1["val5"] = yggr::utf8_string("val5");

	opak o;

	boost::serialization::nvp<params_type> nvps("params", params1);
	o.save(nvps);

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	params_type params2;

	boost::serialization::nvp<params_type> nvpl("params", params2);
	i.load(nvpl);

	yggr_test_assert(params1 == params2);
}

int main(int argc, char* argv[])
{
	bool bchk = yggr::any_val::any_operator_mgr_basic_t_reg::s_init(true);
	yggr_test_assert(bchk);
	test_foo1();

	wait_any_key(argc, argv);
	return 0;
}
