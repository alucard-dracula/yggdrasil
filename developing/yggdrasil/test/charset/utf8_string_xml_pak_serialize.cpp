//utf8_string_pak_serialize.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

//#define YGGR_USE_STL_STRING

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <cassert>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;


void test_foo1(void)
{
	//yggr::utf8_string a("100");
	boost::container::string a("100");
	//std::string a("100");

	std::cout << a << std::endl;
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("str", a));

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	//yggr::utf8_string b;
	boost::container::string b;
	//std::string b;
	i.load(YGGR_SERIALIZE_NAME_NVP("str", b));

	std::cout << b << std::endl;

	assert(a.size() == b.size());
}

struct A
{
	A(void)
		: a(0), f(0.0f)
	{
	}

	A(int n)
		:a(n), f(123.5f), str("���Hello")
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, unsigned int version)
	{
		ar & YGGR_SERIALIZE_NVP(a);
		ar & YGGR_SERIALIZE_NVP(f);
		ar & YGGR_SERIALIZE_NVP(str);
	}

	int a;
	float f;
	yggr::charset::utf8_string str;
};

void test_foo2(void)
{
	A a(100);

	std::cout << a.str << std::endl;
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("a", a));

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	A b;
	i.load(YGGR_SERIALIZE_NAME_NVP("a", b));

	std::cout << b.str << std::endl;
	assert(b.str.size() == a.str.size());
}

int main(int argc, char* argv[])
{
	test_foo1();
	test_foo2();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
