//text_archive_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#define YGGR_SERIALIZE_SIZE32 //add this test 64bit ser_size_type is 32bit

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/archive/binary_oarchive.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <string>

#include <yggr/network/network_packet.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/vector.hpp>

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

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak;

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

#include <boost/serialization/export.hpp>
//#include <yggr/serialization/bson_inner_data.hpp>
struct in_val_t
{
	in_val_t(void)
		: n(0)
	{
	}

	in_val_t(int nn)
		: n(nn)
	{
	}

	in_val_t(const in_val_t& right)
		: n(right.n)
	{
	}
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(n);
	}
public:
	int n;
};

struct E
{
public:
	E(void)
	{
	}

	~E(void)
	{
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & YGGR_SERIALIZATION_NVP(pval);
	}

public:
	yggr::shared_ptr<in_val_t> pval;
};

void test_shared_ptr(void)
{
	E e1;

	opak o;
	o.save(e1);

	std::cout << o.org_buf() << std::endl;

	E e3;

	ipak i(o);

	i.load(e3);

	yggr_test_assert(!e3.pval);
	
}

void test_foo0(void)
{
	int arr[10] = {1, 2, 3, 4, 5, 6};
	boost::array<int, 10> arr1;
	memcpy(&arr1[0], arr, sizeof(int) * 10);
	opak o;
	o.save(arr1);

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo0"), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::string buf;

	try
	{
		yggr::file_system::local_fsys_operators::read_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo0"), 
			buf);
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	ipak i(buf);
	boost::array<int, 10> arr2;
	i.load(arr2);

	yggr_test_assert(memcmp(&arr1[0], &arr2[0], sizeof(int) * 10) == 0);
}

void test_foo1(void)
{
	C c1(100);
	opak o;
	o.save(c1);

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo1"), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::string buf;

	try
	{
		yggr::file_system::local_fsys_operators::read_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo1"), 
			buf);
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	ipak i(buf);
	C c2;
	i.load(c2);

	std::cout << c2.a << std::endl;

}

void test_foo2(void)
{
	int c1(100);
	opak o;
	o.save(c1);

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo2"), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::string buf;
	
	try
	{
		yggr::file_system::local_fsys_operators::read_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo2"), 
			buf);
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	ipak i(buf);
	int c2 = 0;
	i.load(c2);

	std::cout << c2 << std::endl;

}

void test_foo3(void)
{
	B c1(100);
	opak o;
	o.save(c1);

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo3"), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::string buf;

	try
	{
		yggr::file_system::local_fsys_operators::read_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo3"), 
			buf);
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	ipak i(buf);
	B c2 = 0;
	i.load(c2);

	std::cout << c2.a << std::endl;
}

void test_foo4(void)
{
	D c1(100);
	opak o;
	o.save(c1);

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo4"), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::string buf;

	try
	{
		yggr::file_system::local_fsys_operators::read_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object("../test_out/packet/test_foo4"), 
			buf);
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	ipak i(buf);
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

	test_shared_ptr();

	wait_any_key(argc, argv);
	return 0;
}
