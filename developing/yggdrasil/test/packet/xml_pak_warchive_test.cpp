//xml_packet_test.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_woarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_wiarchive_partner> ipak;

struct B
{
	B(void)
		: b(10)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("bbb", b);
	}

	int b;
};

struct A
{
	A(void)
		:a(100), f(123.5f), str("Hello")
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar &  YGGR_SERIALIZE_NAME_NVP("aaa", a);
		ar &  YGGR_SERIALIZE_NAME_NVP("fff", f);
		ar &  YGGR_SERIALIZE_NAME_NVP("string", str);
		ar &  YGGR_SERIALIZE_NAME_NVP("bbb", b);
	}

	int a;
	float f;
	std::string str;
	B b;

};

struct C
{
	C(void)
	{
	}

	C(int n)
		: str1("str1"), str2("str2")
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("str1", str1);
		ar & YGGR_SERIALIZE_NAME_NVP("str2", str2);
	}

	yggr::charset::utf8_string str1;
	yggr::charset::utf8_string str2;

};

void test_foo1(void)
{
	int a = 100;
	opak o;

	boost::serialization::nvp<int> nvps("INT", a);
	o.save(nvps);

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	int b = 0;

	boost::serialization::nvp<int> nvpl("INT", b);
	i.load(nvpl);

	std::cout << b << std::endl;
}

void test_foo2(void)
{
	A a;
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("A", a));
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	A a2;
	i.load(YGGR_SERIALIZE_NAME_NVP("A", a2));

	std::cout << a2.a << "," << a2.f << "," << a2.str << std::endl;
}

void test_foo3(void)
{
	C c(10);
	opak o;
	o.save(YGGR_SERIALIZE_NAME_NVP("C", c));
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	C c2;
	i.load(YGGR_SERIALIZE_NAME_NVP("C", c2));

	std::cout << c2.str1 << "," << c2.str2 << std::endl;
}


int main(int argc, char* argv[])
{
	test_foo1();
	test_foo2();
	test_foo3();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
