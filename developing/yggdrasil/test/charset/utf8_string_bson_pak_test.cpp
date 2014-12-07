//bson_pak_test.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

#include <iostream>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/utf8_string.hpp>


#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "libmongoc-vc100-d.lib")
#pragma comment(lib, "libmongoc-vc100-d.lib")

#else
#pragma comment(lib, "libmongoc-vc100.lib")
#pragma comment(lib, "libbson-vc100.lib")

#endif // _DEBUG
#endif // _MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;


struct type_test
{
	type_test(void)
		: _str("Hello, 世界")
	{
	}

	type_test(int n)
	{
	}

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_str);
	}

public:
	//std::string _str;
	yggr::charset::utf8_string _str;
};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	//os << "string = " << val._str << '\n';
#ifdef WIN32
	os << "string = " << val._str;
#else
    os << "string = " << val._str.str("UTF-8");
#endif // WIN32
	return os;
}

int main(int argc, char* argv[])
{
	type_test test;
	std::cout << test << std::endl;
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZE_NAME_NVP("test_tt", test));

	type_test test2(10);

	i_bson_packet_type ipak(opak);


	ipak.load(YGGR_SERIALIZE_NAME_NVP("test_tt", test2));

	std::cout << test2 << std::endl;

	std::cout << test2._str << std::endl;

	yggr::charset::utf8_string str("Hello, 世界");
	std::cout << str << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
