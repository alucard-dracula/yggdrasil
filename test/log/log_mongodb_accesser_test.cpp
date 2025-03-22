//log_mongodb_op_caller_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <vector>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/bson_inner_data.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_mongodb_op.hpp>
#include <yggr/log/log_mongodb_op_caller.hpp>

#include <yggr/log/log_accesser.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::nsql_database_system::c_mongo_connection connection_type;
typedef yggr::nsql_database_system::mongo_accesser accesser_type;

typedef accesser_type::bson_type bson_type;
typedef accesser_type::org_bson_error_type org_bson_err_type;
typedef accesser_type::org_write_concern_type org_write_concern_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
//typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;
typedef yggr::packet::ref_packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_nld_oarchive_partner> cmd_bson_packet_type;

typedef yggr::log::log_op_def log_op_def_type;

typedef yggr::log::log_mongodb_op<log_op_def_type, connection_type> log_mongodb_op_type;
typedef yggr::log::log_mongodb_op_caller log_mongodb_op_caller_type;

typedef yggr::log::log_accesser<int, log_mongodb_op_type, log_mongodb_op_caller_type> log_acc_type;

struct type_test
{
	type_test(void)
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

	type_test(int nid)
	{
		_s8 = 97;
		_u8 = 200;
		_s16 = 10000;
		_u16 = 20000;
		_s32 = nid;
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

	inline yggr::s32 code(void) const
	{
		return _s32;
	}

	template<typename Archive>
	void serialize(Archive & ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_s8);
		ar & YGGR_SERIALIZATION_NVP(_u8);
		ar & YGGR_SERIALIZATION_NVP(_s16);
		ar & YGGR_SERIALIZATION_NVP(_u16);
		ar & YGGR_SERIALIZATION_NVP(_s32);
		ar & YGGR_SERIALIZATION_NVP(_u32);
		ar & YGGR_SERIALIZATION_NVP(_s64);
		ar & YGGR_SERIALIZATION_NVP(_u64);

		ar & YGGR_SERIALIZATION_NVP(_f32);
		ar & YGGR_SERIALIZATION_NVP(_f64);

		ar & YGGR_SERIALIZATION_NVP(_str);

		ar & YGGR_SERIALIZATION_NVP(ivt);
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

#define MONGODB_USE_CA() 0
#define MONGODB_USE_AUTH() 0

bool collector_foo(const bson_type& bs, std::vector<type_test>& test_vt)
{
	i_bson_packet_type pak(bs);

	type_test tmp;
	pak.load(tmp);

	test_vt.push_back(tmp);

	return true;
}

void test_mongodb_accesser(void)
{

#if MONGODB_USE_CA()

#	if MONGODB_USE_AUTH()
		connection_type::init_type client_init(
					"mongodb://xy:123456abc@127.0.0.1:10098/?ssl=true",	// str_uri
					"./nsql_database_system/mongo.pem",		// pem_file
					"",										// pem_pwd
					"./nsql_database_system/camongodb.pem",	// ca_file
					"./nsql_database_system",				// ca_dir
					""										// crl_file
				);
#	else
		connection_type::init_type client_init(
					"mongodb://127.0.0.1:10098/?ssl=true",	// str_uri
					"./nsql_database_system/mongo.pem",		// pem_file
					"",										// pem_pwd
					"./nsql_database_system/camongodb.pem",	// ca_file
					"./nsql_database_system",				// ca_dir
					""										// crl_file
				);

#	endif // MONGODB_USE_AUTH

#else

#	if MONGODB_USE_AUTH()
		connection_type::init_type client_init(
					"mongodb://xy:123456abc@127.0.0.1:10098/?ssl=false",	// str_uri
					"./nsql_database_system/mongo.pem",		// pem_file
					"",										// pem_pwd
					"./nsql_database_system/camongodb.pem",	// ca_file
					"./nsql_database_system",				// ca_dir
					""										// crl_file
				);
#	else
		connection_type::init_type client_init(
					"mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
					"./nsql_database_system/mongo.pem",		// pem_file
					"",										// pem_pwd
					"./nsql_database_system/camongodb.pem",	// ca_file
					"./nsql_database_system",				// ca_dir
					""										// crl_file
				);
#	endif // MONGODB_USE_AUTH

#endif // MONGODB_USE_CA

	log_acc_type acc(client_init);
	log_mongodb_op_caller_type caller("test_log", "acc_test");

	acc.register_msg(100, caller);
	acc.register_msg(101, caller);

	type_test t1(100);
	type_test t2(101);
	type_test t3(102);

	acc.append(t1.code(), t1);
	acc.append(t2.code(), t2);
	acc.append(t3.code(), t3);

	std::vector<type_test> vt;

	bson_type filter;

	acc.search<log_acc_type::tag_op_caller_id>(caller.id(), filter, boost::bind(&collector_foo, _1, boost::ref(vt)));

	yggr_test_assert(!vt.empty());
	
	{
		for(int i = 0, isize = vt.size(); i != isize; ++i)
		{
			std::cout << vt[i] << std::endl;
		}
	}

	acc.clear<log_acc_type::tag_op_caller_id>(caller.id(), bson_type());
}

int main(int argc, char* argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();
	
	test_mongodb_accesser();
	
	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	wait_any_key(argc, argv);
	return 0;
}
