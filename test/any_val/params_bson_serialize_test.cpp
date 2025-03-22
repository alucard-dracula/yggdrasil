//params_bson_serialization.hpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/archive/bson_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/any_val/params.hpp>
#include <yggr/any_val/any_operator_mgr_basic_t_reg.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(any_val)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> ipak;


typedef yggr::any_val::params<std::string> params_type;

YGGR_ANY_EXPORT(int)
YGGR_ANY_EXPORT(float)

void test_foo1(void)
{
	params_type params1;

	params1["val1"] = ANY_SET(int, 10);
	params1["val2"] = ANY_SET(float, 100.5f);

	opak o;

	o.save(params1);

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	params_type params2;

	i.load(params2);

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
