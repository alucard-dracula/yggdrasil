//params_xml_serialization.hpp

#include <iostream>
#include <vector>
#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/any_val/any_val_operator.hpp>
#include <yggr/any_val/params.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak;

typedef yggr::any_val::any_val_operator<yggr::any_val::base_any_type_operator> any_val_op_type;
typedef yggr::any_val::params<std::string, any_val_op_type> params_type;

YGGR_ANY_EXPORT(int)
YGGR_ANY_EXPORT(float)

void test_foo1(void)
{
	params_type params1;

	params1["val1"] = ANY_SET(int, 10);
	params1["val2"] = ANY_SET(float, 100.5f);

	opak o;

	o.save(params1);

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	params_type params2;

	i.load(params2);

	assert(params1 == params2);
}

int main(int argc, char* argv[])
{
	test_foo1();

	std::cout << "complete" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
