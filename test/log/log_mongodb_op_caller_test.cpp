//log_mongodb_op_caller_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <string>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

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

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
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
	void serialize(Archive& ar, yggr::u32 version)
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

#define MONGODB_USE_AUTH() 0

bool collector_foo(const bson_type& bs, std::vector<type_test>& test_vt)
{
	i_bson_packet_type pak(bs);

	type_test tmp;
	pak.load(tmp);

	test_vt.push_back(tmp);

	return true;
}

const bson_t* get_bson_ptr(const yggr::nsql_database_system::c_bson& right)
{
	return right.org_pointer();
}


template<typename Caller, typename Op>
void test_call_append(Caller& caller, Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;
	
	// test_append
	// append
	// args 6
	// append data
	{
		type_test test;
		
		bool bchk =
			caller.call_append(op, test, 0, 0, 0 );

		yggr_test_assert(bchk);
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk =
			caller.call_append(
				op,
				test, opts.org_pointer(), reply.org_pointer(), err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(
				op,
				test, opts, reply.org_pointer(), err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(
				op,
				test, opts, reply, err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(
				op,
				test, opts, reply, err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// append bson
	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bool bchk =
			caller.call_append(
				op,
				opak.org_buf(), static_cast<const bson_type*>(0), 0, 0 );

		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk =
			caller.call_append(
				op,
				opak.org_buf(), opts.org_pointer(), reply.org_pointer(), err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(
				op,
				opak.org_buf(), opts, reply.org_pointer(), err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(
				op,
				opak.org_buf(), opts, reply, err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(
				op,
				opak.org_buf(), opts, reply, err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// args 6 bson_container

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bson_type bs_arr[1] = {opak.org_buf()};

		bool bchk = 
			caller.call_append(op, 
						bs_arr, opts, 
						reply.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		yggr::vector<bson_type> data_vt;
		data_vt.push_back(opak.org_buf());

		bool bchk = 
			caller.call_append(op, 
						data_vt, opts, 
						reply, err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		yggr::vector<bson_type> data_vt;
		data_vt.push_back(opak.org_buf());

		bool bchk = 
			caller.call_append(op, 
						data_vt, opts, 
						reply.org_pointer(), err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		yggr::vector<bson_type> data_vt;
		data_vt.push_back(opak.org_buf());

		bool bchk = 
			caller.call_append(op, 
						data_vt, opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// args 7 bson**, n

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append(op, 
						&bs_arr[0], 1, opts, 
						reply.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append(op, 
						&bs_arr[0], 1, opts, 
						reply.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append(op, 
						&bs_arr[0], 1, opts, 
						reply, err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append(op, 
						&bs_arr[0], 1, opts, 
						reply.org_pointer(), err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append(op, 
						&bs_arr[0], 1, opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// args 7 container, handler

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply, err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply.org_pointer(), err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// test_append_sp
	{
		type_test test;

		bool bchk = caller.call_append(op, test);
		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bool bchk = caller.call_append(op, opak.org_buf());
		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = caller.call_append(op, &bs_arr[0], 1);
		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bool bchk = caller.call_append(
						op, 
						bson_vt, 
						boost::bind(&get_bson_ptr, _1));
		yggr_test_assert(bchk);
	}

	std::cout << "----------------test_call_append end----------------" << std::endl;
}

template<typename Caller, typename Op>
void test_call_append_any(Caller& caller, Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;
		{
		type_test test;
		
		bool bchk =
			caller.call_append_any(op, test, 0, 0, 0 );

		yggr_test_assert(bchk);
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk =
			caller.call_append_any(
				op,
				test, opts.org_pointer(), reply.org_pointer(), err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(
				op,
				test, opts, reply.org_pointer(), err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(
				op,
				test, opts, reply, err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(
				op,
				test, opts, reply, err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// append bson
	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bool bchk =
			caller.call_append_any(
				op,
				opak.org_buf(), static_cast<const bson_type*>(0), 0, 0 );

		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk =
			caller.call_append_any(
				op,
				opak.org_buf(), opts.org_pointer(), reply.org_pointer(), err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(
				op,
				opak.org_buf(), opts, reply.org_pointer(), err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(
				op,
				opak.org_buf(), opts, reply, err.org_pointer() );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(
				op,
				opak.org_buf(), opts, reply, err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// args 6 bson_container

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bson_type bs_arr[1] = {opak.org_buf()};

		bool bchk = 
			caller.call_append_any(op, 
						bs_arr, opts, 
						reply.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		yggr::vector<bson_type> data_vt;
		data_vt.push_back(opak.org_buf());

		bool bchk = 
			caller.call_append_any(op, 
						data_vt, opts, 
						reply, err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		yggr::vector<bson_type> data_vt;
		data_vt.push_back(opak.org_buf());

		bool bchk = 
			caller.call_append_any(op, 
						data_vt, opts, 
						reply.org_pointer(), err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		yggr::vector<bson_type> data_vt;
		data_vt.push_back(opak.org_buf());

		bool bchk = 
			caller.call_append_any(op, 
						data_vt, opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// args 7 bson**, n

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append_any(op, 
						&bs_arr[0], 1, opts, 
						reply.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append_any(op, 
						&bs_arr[0], 1, opts, 
						reply.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append_any(op, 
						&bs_arr[0], 1, opts, 
						reply, err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append_any(op, 
						&bs_arr[0], 1, opts, 
						reply.org_pointer(), err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type opts;
		bson_type reply;
		bson_err_type err;


		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = 
			caller.call_append_any(op, 
						&bs_arr[0], 1, opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// args 7 container, handler

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply, err.org_pointer());

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply.org_pointer(), err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk = 
			caller.call_append_any(op, 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// append_sp
	{
		type_test test;

		bool bchk = caller.call_append_any(op, test);
		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bool bchk = caller.call_append_any(op, opak.org_buf());
		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bson_type* bs_arr[1] = {&(opak.org_buf())};

		bool bchk = caller.call_append_any(op, &bs_arr[0], 1);
		yggr_test_assert(bchk);
	}

	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		yggr::vector<bson_type> bson_vt;

		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());
		bson_vt.push_back(opak.org_buf());

		bool bchk = caller.call_append_any(
						op, 
						bson_vt, 
						boost::bind(&get_bson_ptr, _1));
		yggr_test_assert(bchk);
	}

	std::cout << "----------------test_call_append_any end----------------" << std::endl;
}

template<typename Caller, typename Op>
void test_call_clear(Caller& caller, Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		caller.call_clear(op, 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply, err);

		std::cout << reply << std::endl;
	}

	{
		caller.call_clear(op, 0/*flag*/, bson_type(), 0, 0);
	}

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		caller.call_clear(op, 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply.org_pointer(), err.org_pointer());

		std::cout << reply << std::endl;
	}

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		caller.call_clear(op, 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply, err.org_pointer());

		std::cout << reply << std::endl;
	}

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		caller.call_clear(op, 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply.org_pointer(), err);

		std::cout << reply << std::endl;
	}

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		caller.call_clear(op, 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply, err);

		std::cout << reply << std::endl;
	}

	{
		bson_type cmd;
		caller.call_clear(op, cmd.org_pointer());
	}

	{
		bson_type cmd;
		caller.call_clear(op, cmd);
	}

	std::cout << "----------------test_call_clear end----------------" << std::endl;
}

template<typename Caller, typename Op>
void test_call_clear_any(Caller& caller, Op& op)
{
	{
		bson_type cmd;
		bool bchk = caller.call_clear_any(op, cmd);
		yggr_test_assert(!bchk);
	}
}

template<typename Caller, typename Op>
void test_call_search(Caller& caller, Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		caller.call_search(op,
					filter.org_pointer(),
					opts.org_pointer(),
					0,
					0,
					boost::bind(&collector_foo, _1, boost::ref(vt))
					);


		yggr_test_assert(!vt.empty());

		{
			for(std::size_t i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");
		bson_err_type err;

		std::vector<type_test> vt;

		caller.call_search(op,
					filter,
					opts,
					0,
					err.org_pointer(),
					boost::bind(&collector_foo, _1, boost::ref(vt))
					);


		yggr_test_assert(!vt.empty());

		{
			for(std::size_t i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");
		bson_err_type err;

		std::vector<type_test> vt;

		caller.call_search(op,
					filter,
					opts,
					0,
					err,
					boost::bind(&collector_foo, _1, boost::ref(vt))
					);


		yggr_test_assert(!vt.empty());

		{
			for(std::size_t i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		caller.call_search(op, 
							filter, opts,
							boost::bind(&collector_foo, _1, boost::ref(vt)));

		yggr_test_assert(!vt.empty());

		{
			for(int i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		caller.call_search(op, 
							filter, 
							boost::bind(&collector_foo, _1, boost::ref(vt)));

		yggr_test_assert(!vt.empty());

		{
			for(int i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	std::cout << "----------------test_call_search end----------------" << std::endl;
}

template<typename Caller, typename Op>
void test_call_search_any(Caller& caller, Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		caller.call_search_any(op,
					filter.org_pointer(),
					opts.org_pointer(),
					0,
					0,
					boost::bind(&collector_foo, _1, boost::ref(vt))
					);


		yggr_test_assert(!vt.empty());

		{
			for(std::size_t i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");
		bson_err_type err;

		std::vector<type_test> vt;

		caller.call_search_any(op,
					filter,
					opts,
					0,
					err.org_pointer(),
					boost::bind(&collector_foo, _1, boost::ref(vt))
					);


		yggr_test_assert(!vt.empty());

		{
			for(std::size_t i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");
		bson_err_type err;

		std::vector<type_test> vt;

		caller.call_search_any(op,
					filter,
					opts,
					0,
					err,
					boost::bind(&collector_foo, _1, boost::ref(vt))
					);


		yggr_test_assert(!vt.empty());

		{
			for(std::size_t i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		caller.call_search_any(op, 
							filter, opts,
							boost::bind(&collector_foo, _1, boost::ref(vt)));

		yggr_test_assert(!vt.empty());

		{
			for(int i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		caller.call_search_any(op, 
							filter, 
							boost::bind(&collector_foo, _1, boost::ref(vt)));

		yggr_test_assert(!vt.empty());

		{
			for(int i = 0, isize = vt.size(); i != isize; ++i)
			{
				std::cout << vt[i] << std::endl;
			}
		}
	}

	std::cout << "----------------test_call_search_any end----------------" << std::endl;
}


void test_mongodb_op_caller(void)
{
//#if MONGODB_USE_AUTH()
//	connection_type::init_type client_init(
//				"mongodb://xy:123456abc@127.0.0.1:10098/?ssl=true",	// str_uri
//				"./nsql_database_system/mongo.pem",		// pem_file
//				"",										// pem_pwd
//				"./nsql_database_system/camongodb.pem",	// ca_file
//				"./nsql_database_system",				// ca_dir
//				""										// crl_file
//			);
//#else
//	connection_type::init_type client_init(
//				"mongodb://127.0.0.1:10098/?ssl=true",	// str_uri
//				"./nsql_database_system/mongo.pem",		// pem_file
//				"",										// pem_pwd
//				"./nsql_database_system/camongodb.pem",	// ca_file
//				"./nsql_database_system",				// ca_dir
//				""										// crl_file
//			);
//#endif // MONGODB_USE_AUTH

	#if MONGODB_USE_AUTH()
	connection_type::init_type client_init(
				"mongodb://xy:123456abc@127.0.0.1:10098/?ssl=false"
			);
#else
	connection_type::init_type client_init(
				"mongodb://127.0.0.1:10098/?ssl=false"
			);
#endif // MONGODB_USE_AUTH

	log_mongodb_op_type op(client_init);

	log_mongodb_op_caller_type caller("test_log", "op_caller_test");

	std::cout << op.id() << std::endl;

	//test_call_clear_any(caller, op);
	test_call_clear(caller, op);

	test_call_append(caller, op);
	test_call_append_any(caller, op);

	test_call_search(caller, op);
	test_call_search_any(caller, op);
}

int main(int argc, char* argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();
	
	test_mongodb_op_caller();
	
	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	wait_any_key(argc, argv);
	return 0;
}
