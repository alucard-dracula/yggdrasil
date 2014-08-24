//mongo_connection.cpp

#include <iostream>
#include <string>
#include <vector>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/string.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "libmongoc-vc100-d.lib")
#pragma comment(lib, "libbson-vc100-d.lib")
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
	{
		_s8 = 97;
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
		_buf = "ABCDEFG";

		opacket_type opak;
		opak.save(100);
		_pak_buf = opak.org_buf();

		for(int i = 0; i != 3; ++i)
		{
			ivt.push_back(i + 1);
		}

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

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("s8", _s8);
		ar & YGGR_SERIALIZE_NAME_NVP("u8", _u8);
		ar & YGGR_SERIALIZE_NAME_NVP("s16", _s16);
		ar & YGGR_SERIALIZE_NAME_NVP("u16", _u16);
		ar & YGGR_SERIALIZE_NAME_NVP("s32", _s32);
		ar & YGGR_SERIALIZE_NAME_NVP("u32", _u32);
		ar & YGGR_SERIALIZE_NAME_NVP("s64", _s64);
		ar & YGGR_SERIALIZE_NAME_NVP("u64", _u64);

		ar & YGGR_SERIALIZE_NAME_NVP("f32", _f32);
		ar & YGGR_SERIALIZE_NAME_NVP("f64", _f64);

		ar & YGGR_SERIALIZE_NAME_NVP("str", _str);

		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_u8_binary_buffer, _buf);
		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_bpak_binary_buffer, _pak_buf);

		ar & YGGR_SERIALIZE_NAME_NVP("ivt", ivt);
	}

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

	yggr::nsql_database_system::c_bson_date _date;
	yggr::nsql_database_system::c_bson_time _time;
	yggr::nsql_database_system::c_bson_oid _oid;

	std::string _buf;

	std::string _pak_buf;

	std::vector<yggr::u32> ivt;

};

std::ostream& operator<<(std::ostream& os, const type_test& val)
{
	os << "s8 = " << val._s8 << '\n';
	os << "u8 = " << val._u8 << '\n';
	os << "s16 = " << val._s16 << '\n';
	os << "u16 = " << val._u16 << '\n';
	os << "s32 = " << val._s32 << '\n';
	os << "u32 = " << val._u32 << '\n';
	os << "s64 = " << val._s64 << '\n';
	os << "u64 = " << val._u64 << '\n';

	os << "f32 = " << val._f32 << '\n';
	os << "f64 = " << val._f64 << '\n';

	os << "string = " << val._str << '\n';
	os << "date = " << val._date << '\n';
	os << "time = " << val._time << '\n';
	os << "oid = " << val._oid << '\n';
	os << "buf = " << val._buf	<< '\n';

	int a = 0;
	ipacket_type ipak(val._pak_buf);
	ipak.load(a);

	os << "pak_buf = " << a << '\n';

	os << "ivt = ";
	for(int i= 0; i != val.ivt.size(); ++i)
	{
		os << val.ivt[i] << " ";
	}

	os << "\n";
	return os;
}

struct query_test
{
	query_test(void)
        : _less(100)
	{
	}

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
	    ar & YGGR_SERIALIZE_NAME_NVP("$lt", _less);
	}

	yggr::s8 _less;

};


struct find_op
{
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("s8", q);
	}
	query_test q;
};

typedef yggr::nsql_database_system::c_mongo_connection connection_type;
typedef yggr::nsql_database_system::mongo_accesser<connection_type> accesser_type;

typedef std::vector<type_test> test_vt_type;
class Saver
{
public:
	typedef i_bson_packet_type buf_type;

public:

	Saver(test_vt_type& rvt)
		: now_idx(0), vt(rvt)
	{
	}

	Saver& operator<<(yggr::s32 size)
	{
		vt.resize(size);
		return *this;
	}

	Saver& operator<<(buf_type& buf)
	{
		if(now_idx >= vt.size())
		{
			return *this;
		}

		buf.load(vt[now_idx]);
		++now_idx;

		return *this;
	}

	Saver& operator+=(buf_type& buf)
	{
		test_vt_type::value_type val;
		buf.load(val);
		vt.push_back(val);
		++now_idx;

		return *this;
	}

private:
	yggr::u32 now_idx;
	test_vt_type& vt;
};


void test(void)
{
	connection_type::this_ptr_type pconn(new connection_type("127.0.0.1:10098"));
	pconn->connect();

	//accesser_type accesser(pconn, "xy", "123456");
	accesser_type accesser(pconn);

	accesser.remove_all("test", "foo2");

	{
		for(int i = 0; i < 10; ++i)
		{
			o_bson_packet_type opak;
			type_test test;
			test._s8 = 97 + i;
			opak.save(test);
			accesser.insert(std::string("test"), std::string("foo2"), opak);
		}
	}


	test_vt_type vt;

	o_bson_packet_type opak_level;
	opak_level.save(find_op());
	std::cout << opak_level.org_buf() << std::endl;

	yggr::nsql_database_system::c_bson b1(opak_level.org_buf()),b2(true);

    yggr::s64 level = accesser.count(std::string("test"), std::string("foo2"), b1) + 1;
    std::cout << "level = " << level << std::endl;
}

int main(int argc, char* argv[])
{

	test();

	char cc = 0;
	std::cin >> cc;


	return 0;
}
