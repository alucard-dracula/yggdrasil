// any_network_serialize_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <string>

#include <yggr/base/yggrdef.h>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/any_val/any.hpp>
#include <yggr/serialization/access.hpp>
#include <yggr/serialization/any.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak;

//YGGR_ANY_EXPORT(int)
YGGR_ANY_EXPORT_DECLEAR(int)
YGGR_ANY_EXPORT_IMPL(int)

void test_foo1(void)
{
	yggr::any any1(10);

	opak o;
	o.save(any1);

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	yggr::any any2;

	i.load(any2);

	yggr_test_assert(yggr::any_cast<int>(any2) == yggr::any_cast<int>(any1));
}

int main(int argc, char* argv[])
{
	test_foo1();

	wait_any_key(argc, argv);
	return 0;
}
