//mongo_connection.cpp

#define YGGR_SERIALIZE_SIZE32

#include <iostream>
#include <vector>
#include <string>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/vector.hpp>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_DEBUG

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

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_s8);
		ar & YGGR_SERIALIZE_NVP(_u8);
		ar & YGGR_SERIALIZE_NVP(_s16);
		ar & YGGR_SERIALIZE_NVP(_u16);
		ar & YGGR_SERIALIZE_NVP(_s32);
		ar & YGGR_SERIALIZE_NVP(_u32);
		ar & YGGR_SERIALIZE_NVP(_s64);
		ar & YGGR_SERIALIZE_NVP(_u64);

		ar & YGGR_SERIALIZE_NVP(_f32);
		ar & YGGR_SERIALIZE_NVP(_f64);

		ar & YGGR_SERIALIZE_NVP(_str);

		ar & YGGR_SERIALIZE_NVP(_date);
		ar & YGGR_SERIALIZE_NVP(_time);
		ar & YGGR_SERIALIZE_NVP(_oid);

		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_u8_binary_buffer, _buf);
		YGGR_SERIALIZE_WRAP_NVP_AR(ar, yggr::nsql_database_system::c_bson_bpak_binary_buffer, _pak_buf);

		ar & YGGR_SERIALIZE_NVP(ivt);
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

template<typename Accesser>
class Deleter
{
public:
	typedef Accesser accesser_type;
	typedef i_bson_packet_type buf_type;
	typedef o_bson_packet_type rm_pak_type;

public:

	Deleter(const std::string& str_db, const std::string& str_tab,
				const std::string& bak_str_db, const std::string& bak_str_tab,
				accesser_type& accesser)
		: _str_db(str_db)
		, _str_tab(str_tab)
		, _bak_str_db(bak_str_db)
		, _bak_str_tab(bak_str_tab)
		, _acc(accesser)
	{
	}

	Deleter& operator<<(const yggr::size_type& size)
	{
		return *this;
	}

	Deleter& operator<<(buf_type& buf) //limited
	{

		rm_pak_type pak(buf);

		_acc.insert(_bak_str_db, _bak_str_tab, pak);
		_acc.remove(_str_db, _str_tab, pak);
		return *this;
	}

	Deleter& operator+=(buf_type& buf) //non limited
	{

		rm_pak_type pak(buf);
		_acc.insert(_bak_str_db, _bak_str_tab, pak);
		_acc.remove(_str_db, _str_tab, pak);
		return *this;
	}

private:
	const std::string& _str_db;
	const std::string& _str_tab;
	const std::string& _bak_str_db;
	const std::string& _bak_str_tab;
	accesser_type& _acc;
};


typedef yggr::nsql_database_system::c_mongo_connection connection_type;
typedef yggr::nsql_database_system::mongo_accesser<connection_type> accesser_type;

int main(int argc, char* argv[])
{
	typedef connection_type::init_type init_type;
	init_type init("rs0");
	init << "127.0.0.1:10099"
		<< "127.0.0.1:10100"
		<< "127.0.0.1:10101";

	connection_type::this_ptr_type pconn(new connection_type(init));
	pconn->connect();

	accesser_type accesser(pconn);

	type_test test;
	o_bson_packet_type opak, opak2;
	opak.save(test);
	test._s8 = 127;
	test.ivt[0] = 100;
	opak2.save(test);

    accesser.remove_all(std::string("test"), std::string("foo"));
	accesser.remove_all(std::string("test"), std::string("bak_foo"));
	accesser.insert(std::string("test"), std::string("foo"), opak);
	accesser.insert(std::string("test"), std::string("foo"), opak2);
	accesser.insert(std::string("test"), std::string("bak_foo"), opak);
	accesser.insert(std::string("test"), std::string("bak_foo"), opak2);

	std::string db("test"), tab("foo"), bak_tab("bak_foo");
	Deleter<accesser_type> deleter(db, tab, db, bak_tab, accesser);

	yggr::nsql_database_system::c_bson b1, b2;
	b1.finish();
	b2.finish();

	accesser.find(deleter, db, tab, b1, b2); // this find is remove records

	char cc = 0;
	std::cin >> cc;

	return 0;
}
