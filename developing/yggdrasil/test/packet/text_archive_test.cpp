//text_archive_test.cpp


#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/text_archive_partner.hpp>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::text_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::text_iarchive_partner> ipak;

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
		ar & YGGR_SERIALIZE_NVP(a);
		ar & YGGR_SERIALIZE_NVP(f);
		ar & YGGR_SERIALIZE_NVP(str);
	}

	int a;
	float f;
	std::string str;

};

void test_foo1(void)
{
	int a = 100;
	opak o;
	o.save(a);

	std::cout << o.val_buf() << std::endl;

	ipak i(o);
	int b = 0;
	i.load(b);

	std::cout << b << std::endl;

}

void test_foo2(void)
{
	A a;
	opak o;
	o.save(a);
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	A a2;
	i.load(a2);

	std::cout << a2.a << "," << a2.f << "," << a2.str << std::endl;
}
int main(int argc, char* argv[])
{
	test_foo1();
	test_foo2();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
