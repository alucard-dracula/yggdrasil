//mongo_connection.cpp

#include <iostream>
#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <string>

#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/nvp.hpp>

//#include <yggr/charset/utf8_string.h>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_nsql_db_op.hpp>

#include <vector>

#ifdef _MSC_VER
#   include <vld.h>
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

//typedef yggr::nsql_database_system::c_mongo_connection<yggr::nsql_database_system::E_CONN_SINGLE> connection_type;
typedef yggr::nsql_database_system::c_mongo_connection connection_type;
typedef yggr::nsql_database_system::mongo_accesser<connection_type> accesser_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;

typedef yggr::log::log_op::log_op_def<yggr::u32> log_op_def_type;

//typedef yggr::log::log_op::log_nsql_db_op<log_op_def_type, yggr::nsql_database_system::E_CONN_SINGLE> log_nsql_db_op_type;
typedef yggr::log::log_op::log_nsql_db_op<log_op_def_type> log_nsql_db_op_type;

typedef log_nsql_db_op_type::conn_type conn_type;
typedef log_nsql_db_op_type::conn_ptr_type conn_ptr_type;


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

		ar & YGGR_SERIALIZE_NVP(ivt);
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
	{
	}

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
	}

};

struct sort_test
{
	sort_test(void)
		: _sort(1)
	{
	}

	sort_test(yggr::s8 s)
		: _sort(s)
	{
	}

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_sort);
	}

	yggr::s8 _sort;
};


struct find_op
{
	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("$query", q);
		ar & YGGR_SERIALIZE_NAME_NVP("$orderby", s);
	}
	query_test q;
	sort_test s;
};

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

		//std::cout << buf.org_buf() << std::endl;
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


void test_nsql_op(void)
{
	//yggr::exception::exception::init_type init(1, 0);
	yggr::ptr_single<yggr::exception::exception>::init_ins();

	conn_ptr_type pconn(new conn_type("127.0.0.1:10098"));
	log_nsql_db_op_type op(pconn);

	std::cout << op.id() << std::endl;

	type_test test;
	op.append(boost::make_tuple(std::string("test"), std::string("foo3"), test));

	test_vt_type vt;

	Saver saver(vt);

	log_nsql_db_op_type::opak_type opak_sort;
	opak_sort.save(find_op());

	yggr::nsql_database_system::c_bson b1(opak_sort.org_buf()), b2(true);

	op.search(boost::make_tuple(saver, std::string("test"), std::string("foo3"), b1, b2, 0, 0, 0));

	{
		std::cout << vt.size() << std::endl;
		for(int i = 0; i < vt.size(); ++i)
		{
			std::cout << vt[i] << std::endl;
		}
	}


	//op.clear(boost::make_tuple(std::string("test"), std::string("foo3")));

	vt.clear();

    yggr::ptr_single<yggr::exception::exception>::uninstall();
}


int main(int argc, char* argv[])
{

	test_nsql_op();

	char cc = 0;
	std::cin >> cc;


	return 0;
}
