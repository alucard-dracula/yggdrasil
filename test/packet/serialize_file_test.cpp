//serialize_file_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/xml_archive_partner.hpp> //xml archive
#include <yggr/serialization/nvp.hpp> //nvp

#include <yggr/file_system/local_fsys_operators.hpp>

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

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

struct type_test
{
	type_test(void)
	{
		_s8 = 100;
		_u8 = 200;
		_s16 = 10000;
		_u16 = 20000;
		_s32 = 1000000;
		_u32 = 2000000;

		_s64 = 1000000000;
		_u64 = 2000000000;

		_f32 = 100.12345f;
		_f64 = 200.1234567;

		_str = "abcdefg";
	}

	type_test(int n)
	{
		_s8 = 0;
		_u8 = 0;
		_s16 = 0;
		_u16 = 0;
		_s32 = 0;
		_u32 = 0;

		_s64 = 0;
		_u64 = 0;

		_f32 = 0;
		_f64 = 0;
	}

private :
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
    {
		ar & YGGR_SERIALIZATION_NAME_NVP("s8", _s8);
		ar & YGGR_SERIALIZATION_NAME_NVP("u8", _u8);
		ar & YGGR_SERIALIZATION_NAME_NVP("s16", _s16);
		ar & YGGR_SERIALIZATION_NAME_NVP("u16", _u16);
		ar & YGGR_SERIALIZATION_NAME_NVP("s32", _s32);
		ar & YGGR_SERIALIZATION_NAME_NVP("u32", _u32);
		ar & YGGR_SERIALIZATION_NAME_NVP("s64", _s64);
		ar & YGGR_SERIALIZATION_NAME_NVP("u64", _u64);

		ar & YGGR_SERIALIZATION_NAME_NVP("f32", _f32);
		ar & YGGR_SERIALIZATION_NAME_NVP("f64", _f64);

		ar & YGGR_SERIALIZATION_NAME_NVP("str", _str);
	}

public:
	yggr::s8 _s8;
	yggr::u8 _u8;
	yggr::s16 _s16;
	yggr::u16 _u16;
	yggr::s32 _s32;
	yggr::u32 _u32;
	yggr::s64 _s64;
	yggr::u64 _u64;

	yggr::f32 _f32;
	yggr::f64 _f64;

	std::string _str;

};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << "s8 = " << val._s8 << '\n';
	os << "u8 = " << (int)val._u8 << '\n';
	os << "s16 = " << val._s16 << '\n';
	os << "u16 = " << val._u16 << '\n';
	os << "s32 = " << val._s32 << '\n';
	os << "u32 = " << val._u32 << '\n';
	os << "s64 = " << val._s64 << '\n';
	os << "u64 = " << val._u64 << '\n';

	os << "f32 = " << val._f32 << '\n';
	os << "f64 = " << val._f64 << '\n';

	os << "string = " << val._str << '\n';

	return os;
}

int main(int argc, char* argv[])
{
	type_test test1;
	std::cout << test1 << std::endl;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("test", test1));

	std::cout << opak.org_buf() << std::endl;

	//file i/o
	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary("../test_out/packet/test_out.xml", opak.org_buf());
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch(const ::yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	yggr::file_system::local_fsys_operators::file_size_type fsize = 0;
	std::string in_buf;

	try
	{
		fsize = yggr::file_system::local_fsys_operators::read_file_of_binary("../test_out/packet/test_out.xml", in_buf);
	}
	catch(const ::yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	if(fsize == 0)
	{
		return 1;
	}

	type_test test2(10);

	ipak_type ipak(in_buf);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("test", test2));
	std::cout << test2 <<std::endl;

	wait_any_key(argc, argv);
	return 0;
}
