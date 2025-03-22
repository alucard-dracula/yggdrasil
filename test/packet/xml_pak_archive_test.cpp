//xml_packet_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/array.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>

// warning: mingw use libboost_serialization-mgw47-mt-1_53.a //fix xml_gamma_error

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

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak;

struct D
{
	D(void)
		: n(0)
	{
	}

	D(int nn)
		: n(nn)
	{
	}

	~D(void)
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(n);
	}

	int n;
};

BOOST_SERIALIZATION_SHARED_PTR(D)

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
		dptr = yggr::shared_ptr<D>(new D(700));
	}

	A(int n)
		: a(0), f(0.0)
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
		ar &  YGGR_SERIALIZATION_NVP(dptr);
	}

	int a;
	float f;
	std::string str;
	B b;
	yggr::shared_ptr<D> dptr;
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

	std::cout << o.val_buf() << std::endl;

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
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	A a2(10);
	i.load(YGGR_SERIALIZATION_NAME_NVP("A", a2));

	std::cout << a2.a << "," << a2.f << "," << a2.str << "," << a2.dptr->n << std::endl;
}

void test_foo3(void)
{
	C c(10);
	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("C", c));
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	C c2;
	i.load(YGGR_SERIALIZATION_NAME_NVP("C", c2));

	std::cout << c2.str1 << "," << c2.str2 << std::endl;
}

void test_foo4(void)
{
	C c(10);
	std::cout << c.str1 << "," << c.str2 << std::endl;
	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("C", c));
	std::cout << o.val_buf() << std::endl;

	
	yggr::file_system::local_fsys_operators::write_file_of_binary(
		yggr::file_system::local_fsys_operators::make_path_object("../test_out/database.xml"),
		o.org_buf(),
		yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	
	

	opak::buf_type buf;

	yggr::file_system::local_fsys_operators::read_file_of_binary(
		yggr::file_system::local_fsys_operators::make_path_object("../test_out/database.xml"),
		buf);

	ipak i(buf);

	C c2;
	i.load(YGGR_SERIALIZATION_NAME_NVP("C", c2));

	std::cout << c2.str1 << "," << c2.str2 << std::endl;
}

void test_foo5(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	yggr::vector<int> vt(arr + 0, arr + 10);

	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("vt", vt));
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	yggr::vector<int> vt2;

	i.load(YGGR_SERIALIZATION_NAME_NVP("vt", vt2));

	yggr_test_assert(0 == memcmp(&arr[0], &vt2[0], sizeof(arr)));
}

void test_foo6(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	yggr::array<int, 10> vt;
	memcpy(&vt[0], arr, sizeof(arr));

	opak o;
	o.save(YGGR_SERIALIZATION_NAME_NVP("vt", vt));
	std::cout << o.val_buf() << std::endl;

	ipak i(o);

	yggr::array<int, 10> vt2 = {0};

	i.load(YGGR_SERIALIZATION_NAME_NVP("vt", vt2));

	yggr_test_assert(0 == memcmp(&arr[0], &vt2[0], sizeof(arr)));
}

int main(int argc, char* argv[])
{
	test_foo1();
	test_foo2();
	test_foo3();
	test_foo4();
	test_foo5();
	test_foo6();

	wait_any_key(argc, argv);
	return 0;
}
