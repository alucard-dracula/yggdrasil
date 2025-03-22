//binary_pak_warchive_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#ifdef __MINGW32__
//# error "this file don't work of mingw!!!"
//#endif // __MINGW32__

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <string>

#include <yggr/network/network_packet.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/vector.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_woarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_wiarchive_partner> ipak;

struct A
{
	A(void)
		:a(0), f(0.0f)
	{
	}

	A(int na)
		:a(na), f(123.5f), str("Hello")
	{
	}

	A(const A& right)
		: a(right.a), f(right.f), str(right.str)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & a;
		ar & f;
		ar & str;
	}

	int a;
	float f;
	std::string str;

};

struct B : public A
{
	B(void)
		:a(0)
	{
	}

	B(int na)
		: A(na), a(na)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		A::serialize(ar, version);
		ar & a;
	}
	int a;
};

struct C
{
	C(void)
		:a(0)
	{
	}

	C(int n)
		: na(n), a(n)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & na;
		ar & a;
	}

	A na;
	int a;
};

class D
{
public:
	D(void)
	{
	}

	D(int n)
		: utf8_str("ÄãºÃ, Hello")
	{
		avt.push_back(A(1000));
		avt.push_back(A(1000));
		avt.push_back(A(1000));
	}

	void out(void)
	{
		std::cout << utf8_str << std::endl;
		std::cout << avt.size() << std::endl;
		std::cout << avt[0].f << std::endl;
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & avt;
		ar & utf8_str;
	}

public:
	yggr::charset::utf8_string utf8_str;
	std::vector<A> avt;

};

void test_foo0(void)
{
	int arr[10] = {1, 2, 3, 4, 5, 6};
	boost::array<int, 10> arr1;
	memcpy(&arr1[0], arr, sizeof(int) * 10);
	opak o;
	o.save(arr1);

	ipak i(o);
	boost::array<int, 10> arr2;
	i.load(arr2);

	yggr_test_assert(memcmp(&arr1[0], &arr2[0], sizeof(int) * 10) == 0);
}

void test_foo1(void)
{
	C c1(100);
	opak o;
	o.save(c1);

	ipak i(o);
	C c2;
	i.load(c2);

	std::cout << c2.a << std::endl;

}

void test_foo2(void)
{
	int c1(100);
	opak o;
	o.save(c1);

	ipak i(o);
	int c2 = 0;
	i.load(c2);

	std::cout << c2 << std::endl;

}

void test_foo3(void)
{
	B c1(100);
	opak o;
	o.save(c1);

	ipak i(o);
	B c2 = 0;
	i.load(c2);

	std::cout << c2.a << std::endl;
}

void test_foo4(void)
{
	D c1(100);
	opak o;
	o.save(c1);

	ipak i(o);
	D c2;
	i.load(c2);

	c2.out();
}


int main(int argc, char* argv[])
{
	test_foo0();
	test_foo1();
	test_foo2();
	test_foo3();
	test_foo4();

	wait_any_key(argc, argv);
	return 0;
}
