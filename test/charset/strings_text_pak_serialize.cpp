//utf8_string_pak_serialize.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

#define YGGR_USE_STL_STRING

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/archive/text_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::text_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::text_iarchive_partner> ipak;


void test_stl_string(void)
{
	std::string a("你好abc");

	std::cout << a << std::endl;
	opak o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	std::string b;
	i.load(b);

	std::cout << b << std::endl;

	yggr_test_assert(a.size() == b.size());

	std::cout << "--------test_stl_string end----------" << std::endl;
}

void test_boost_string(void)
{
	boost::container::string a("你好");

	std::cout << a << std::endl;
	opak o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	boost::container::string b;
	i.load(b);

	std::cout << b << std::endl;

	yggr_test_assert(a.size() == b.size());

	std::cout << "--------test_boost_string end----------" << std::endl;
}

void test_utf8_string(void)
{
	yggr::utf8_string a("你好");

	std::cout << a << std::endl;
	opak o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	yggr::utf8_string b;
	i.load(b);

	std::cout << b << std::endl;

	yggr_test_assert(a.size() == b.size());

	std::cout << "--------test_utf8_string end----------" << std::endl;
}


int main(int argc, char* argv[])
{
	test_stl_string();
	test_boost_string();
	test_utf8_string();

	wait_any_key(argc, argv);
	return 0;
}
