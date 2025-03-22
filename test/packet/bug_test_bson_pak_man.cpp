// bug_test_bson_pak_man.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"
#endif // _MSC_VER

// This file must be created using MSVC using GBK encoding because VS2010 cannot handle the Chinese "man" character

#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <vector>

#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/serialization/utf8_string.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/serialization/shared_ptr.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;


struct type_test
{
	type_test(void)
		: _u8_str("ÄÐ")
	{
	}

	type_test(int n)
	{
	}

	inline bool equal_to(const type_test& right) const
	{
		return _u8_str == right._u8_str;
	}

	inline bool not_equal_to(const type_test& right) const
	{
		return !(_u8_str == right._u8_str);
	}

private :
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
    {
		ar & YGGR_SERIALIZATION_NVP(_u8_str);
	}

public:
	yggr::utf8_string _u8_str;

};

inline bool operator==(const type_test& l, const type_test& r)
{
	return l.equal_to(r);
}

inline bool operator!=(const type_test& l, const type_test& r)
{
	return !l.equal_to(r);
}


std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << "_u8_str = " << val._u8_str;
	return os;
}


void test1_1(void)
{
	type_test test; //create unknow object
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("test_ttt", test));

	std::cout << opak.org_buf() << std::endl;

	type_test test2(10);
	i_bson_packet_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("test_ttt", test2));
	std::cout << test2 << std::endl;

	yggr_test_assert(test == test2);

}

void test1_2(void)
{
	yggr::utf8_string u8_str("??");
	std::cout << u8_str.org_str() << std::endl;
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("u8_str", u8_str));

	std::cout << opak.org_buf() << std::endl;

	yggr::utf8_string u8_str2;
	i_bson_packet_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("u8_str", u8_str2));
	std::cout << u8_str2 << std::endl;

	yggr_test_assert(u8_str == u8_str2);
}

void test2_1(void)
{
	type_test test; //create unknow object
	opacket_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("test_ttt", test));

	std::cout << opak.org_buf() << std::endl;

	type_test test2(10);
	ipacket_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("test_ttt", test2));
	std::cout << test2 << std::endl;

}

void test2_2(void)
{
	yggr::utf8_string u8_str("??");
	std::cout << u8_str.org_str() << std::endl;
	opacket_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("u8_str", u8_str));

	std::cout << opak.org_buf() << std::endl;

	yggr::utf8_string u8_str2;
	ipacket_type ipak(opak);

	ipak.load(YGGR_SERIALIZATION_NAME_NVP("u8_str", u8_str2));
	std::cout << u8_str2 << std::endl;

}

int main(int argc, char* argv[])
{
	test1_1();
	test1_2();

	test2_1();
	test2_2();

	wait_any_key(argc, argv);
	return 0;
}
