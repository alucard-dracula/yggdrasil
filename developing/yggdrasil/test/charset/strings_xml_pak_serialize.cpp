//utf8_string_pak_serialize.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

//#define YGGR_USE_STL_STRING

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/string.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <cassert>

#include <yggr/serialization/nvp.hpp>
#include <yggr/archive/xml_archive_partner.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;


void test_stl_string(void)
{
	std::string a("100");

	std::cout << a << std::endl;
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("str", a));

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	std::string b;
	i.load(YGGR_SERIALIZE_NAME_NVP("str", b));

	std::cout << b << std::endl;

	assert(a.size() == b.size());

	std::cout << "--------test_stl_string end----------" << std::endl;
}

void test_boost_string(void)
{
	boost::container::string a("100");

	std::cout << a << std::endl;
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("str", a));

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	std::string b;
	i.load(YGGR_SERIALIZE_NAME_NVP("str", b));

	std::cout << b << std::endl;

	assert(a.size() == b.size());

	std::cout << "--------test_boost_string end----------" << std::endl;
}

void test_utf8_string(void)
{
	yggr::utf8_string a("100");

	std::cout << a << std::endl;
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("str", a));

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	yggr::utf8_string b;
	i.load(YGGR_SERIALIZE_NAME_NVP("str", b));

	std::cout << b << std::endl;

	assert(a.size() == b.size());

	std::cout << "--------test_utf8_string end----------" << std::endl;
}


int main(int argc, char* argv[])
{
	test_stl_string();
	test_boost_string();
	test_utf8_string();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
