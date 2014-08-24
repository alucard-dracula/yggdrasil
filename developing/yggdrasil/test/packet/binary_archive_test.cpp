//text_archive_test.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <boost/serialization/shared_ptr.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/vector.hpp>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#if 0

 //use local binaty packet
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak;

#else

 //use network binaty packet
typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak;

#endif // 0

struct C
{
	C(void)
		: n(200)
	{
	}

	C(int n)
		: n(0)
	{
	}

	~C(void)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZE_NVP(n);
	}

	int n;
};

BOOST_SERIALIZATION_SHARED_PTR(C)

struct A
{
	A(void)
		:a(100), f(123.5f), str("Hello"), ptr(new C())
	{
	}

	A(int n)
		: a(0), f(0.0), str(""), ptr(new C(10))
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZE_NVP(a);
		ar & YGGR_SERIALIZE_NVP(f);
		ar & YGGR_SERIALIZE_NVP(str);
		ar & YGGR_SERIALIZE_NVP(ptr);
	}

	int a;
	float f;
	std::string str;
	boost::shared_ptr<C> ptr;
};

void test_foo1(void)
{
	int a = 100;
	opak o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

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
	std::cout << o.org_buf() << std::endl;

	ipak i(o);

	A a2(10);
	i.load(a2);

	std::cout << a2.a << "," << a2.f << "," << a2.str << "," << (*(a2.ptr)).n << std::endl;
}

void test_foo3(void)
{
	std::vector<int> ivt;
	ivt.push_back(1);
	ivt.push_back(2);
	ivt.push_back(3);

	opak o;
	o.save(ivt);
	std::cout << o.org_buf() << std::endl;

	ipak i(o);

	std::vector<int> ivt2;
	i.load(ivt2);

	for(std::size_t i = 0, isize = ivt2.size(); i != isize; ++i)
	{
		std::cout << ivt2[i] << std::endl;
	}
}

void test_foo2_2(void)
{
	A a;
	opak o;
	o.save(a);
	std::cout << o.org_buf() << std::endl;

	ipak ti(o);
	ipak i;
	i.swap(o);

	A a2(10);
	i.load(a2);

	std::cout << a2.a << "," << a2.f << "," << a2.str << std::endl;
}

#include <fstream>

int main(int argc, char* argv[])
{
	boost::archive::tracking_type t;
	test_foo1();
	test_foo2();
	test_foo2_2();
	test_foo3();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
