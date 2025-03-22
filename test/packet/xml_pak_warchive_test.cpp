//xml_packet_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

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
		ar & YGGR_SERIALIZATION_NAME_NVP("bbb", b);
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
		ar &  YGGR_SERIALIZATION_NAME_NVP("aaa", a);
		ar &  YGGR_SERIALIZATION_NAME_NVP("fff", f);
		ar &  YGGR_SERIALIZATION_NAME_NVP("string", str);
		ar &  YGGR_SERIALIZATION_NAME_NVP("bbb", b);
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
		ar & YGGR_SERIALIZATION_NAME_NVP("str1", str1);
		ar & YGGR_SERIALIZATION_NAME_NVP("str2", str2);
	}

	yggr::charset::utf8_string str1;
	yggr::charset::utf8_string str2;

};

void test_foo1(void)
{
	int a = 100;
	opak o;

	o.save(YGGR_SERIALIZATION_NAME_NVP("INT", a));

#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20) //std::cout << wstring is deleted
	std::wcout << o.val_buf() << std::endl;
#else
	std::cout << o.val_buf() << std::endl;
#endif // #if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20)

	ipak i(o);
	int b = 0;

	i.load(YGGR_SERIALIZATION_NAME_NVP("INT", b));

	std::cout << b << std::endl;
}

void test_foo2(void)
{
	A a;
	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("A", a));

#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20) //std::cout << wstring is deleted
	std::wcout << o.val_buf() << std::endl;
#else
	std::cout << o.val_buf() << std::endl;
#endif // #if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20)

	ipak i(o);

	A a2;
	i.load(YGGR_SERIALIZATION_NAME_NVP("A", a2));

	std::cout << a2.a << "," << a2.f << "," << a2.str << std::endl;
}

void test_foo3(void)
{
	C c(10);
	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("C", c));

#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20) //std::cout << wstring is deleted
	std::wcout << o.val_buf() << std::endl;
#else
	std::cout << o.val_buf() << std::endl;
#endif // #if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20)

	ipak i(o);

	C c2;
	i.load(YGGR_SERIALIZATION_NAME_NVP("C", c2));

	std::cout << c2.str1 << "," << c2.str2 << std::endl;
}


int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_foo1();
	test_foo2();
	test_foo3();

	wait_any_key(argc, argv);
	return 0;
}
