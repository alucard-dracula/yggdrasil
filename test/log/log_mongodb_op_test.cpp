//log_mongodb_op_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <yggr/container/vector.hpp>

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

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(system_controller)
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



const bson_t* get_bson_ptr(const yggr::nsql_database_system::c_bson& right)
{
	return right.org_pointer();
}

template<typename Op>
void test_append(Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	// append
	// args 6
	// append data
	{
		type_test test;
		
		bool bchk =
			op.append(
				std::string("test_log"), std::string("op_test_append"),
				test, 0, 0, 0 );

		yggr_test_assert(bchk);
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk =
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(
				std::string("test_log"), std::string("op_test_append"),
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
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
			op.append(std::string("test_log"), std::string("op_test_append"), 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	std::cout << "------------test_append end--------------" << std::endl;
}

template<typename Op>
void test_append_any(Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	// append_any
	// args 6
	// append_any data
	{
		type_test test;

		bool bchk =
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
				test, 0, 0, 0 );

		yggr_test_assert(bchk);
	}

	{
		type_test test;
		
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		bool bchk =
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
				test, opts, reply, err );
		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	// append_any bson
	{
		type_test test;
		o_bson_packet_type opak;
		opak.save(test);

		bool bchk =
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(
				std::string("test_log"), std::string("op_test_append_any"),
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
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
			op.append_any(std::string("test_log"), std::string("op_test_append_any"), 
						bson_vt,
						boost::bind(&get_bson_ptr, _1),
						opts, 
						reply, err);

		yggr_test_assert(bchk);

		std::cout << reply << std::endl;
	}

	std::cout << "------------test_append_any end--------------" << std::endl;
}

template<typename Op>
void test_clear(Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		op.clear(std::string("test_log"),  std::string("op_test_append"), 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply, err);

		std::cout << reply << std::endl;
	}

	{
		op.clear(std::string("test_log"),  std::string("op_test_append"), 0/*flag*/, bson_type(), 0, 0);
	}

	{
		bson_type sel;
		bson_type opts;
		bson_type reply;
		bson_err_type err;

		op.clear(std::string("test_log"),  std::string("op_test_append_any"), 
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

		op.clear(std::string("test_log"),  std::string("op_test_append_any"), 
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

		op.clear(std::string("test_log"),  std::string("op_test_append_any"), 
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

		op.clear(std::string("test_log"),  std::string("op_test_append_any"), 
					0/*flag*/, 
					sel.org_pointer(), opts.org_pointer(), 
					reply, err);

		std::cout << reply << std::endl;
	}

	std::cout << "------------test_clear end--------------" << std::endl;
}


bool collector_foo(const bson_type& bs, std::vector<type_test>& test_vt)
{
	i_bson_packet_type pak(bs);

	type_test tmp;
	pak.load(tmp);

	test_vt.push_back(tmp);

	return true;
}

template<typename Op>
void test_search(Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		op.search(std::string("test_log"), 
					std::string("op_test_append"),
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

		op.search(std::string("test_log"), 
					std::string("op_test_append"),
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

		op.search(std::string("test_log"), 
					std::string("op_test_append"),
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

	std::cout << "------------test_search end--------------" << std::endl;
}

template<typename Op>
void test_search_any(Op& op)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

	{
		bson_type filter;
		bson_type opts("{\"sort\" : { \"_id\" : 1 }}");

		std::vector<type_test> vt;

		op.search_any(std::string("test_log"), 
						std::string("op_test_append"),
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

		op.search_any(std::string("test_log"), 
						std::string("op_test_append"),
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

		op.search_any(std::string("test_log"), 
						std::string("op_test_append"),
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

	std::cout << "------------test_search_any end--------------" << std::endl;
}

void test_mongodb_op(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error bson_err_type;

//#if MONGODB_USE_AUTH()
//	connection_type::init_type client_init(
//				"mongodb://xy:123456abc@127.0.0.1:10098/?ssl=true",	// str_uri
//				"./nsql_database_system/cert/client.pem",		// pem_file
//				"",										// pem_pwd
//				"./nsql_database_system/cert/ca.pem",	// ca_file
//				"./nsql_database_system/cert",				// ca_dir
//				""										// crl_file
//			);
//#else
//	connection_type::init_type client_init(
//				"mongodb://127.0.0.1:10098/?ssl=true",	// str_uri
//				"./nsql_database_system/cert/client.pem",		// pem_file
//				"",										// pem_pwd
//				"./nsql_database_system/cert/ca.pem",	// ca_file
//				"./nsql_database_system/cert",				// ca_dir
//				""										// crl_file
//			);
//#endif // MONGODB_USE_AUTH

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE

#if MONGODB_USE_AUTH()
	connection_type::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10098/?ssl=false" //"mongodb://xy:123456abc@127.0.0.1:10098/?ssl=false"
			);
#else
	connection_type::init_type client_init(
				"mongodb://" + host_addr + ":10098/?ssl=false" // "mongodb://127.0.0.1:10098/?ssl=false"
			);
#endif // MONGODB_USE_AUTH

	log_mongodb_op_type op(client_init);

	std::cout << op.id() << std::endl;

	test_clear(op);

	test_append(op);
	test_append_any(op);

	test_search(op);
	test_search_any(op);
}

int main(int argc, char* argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();
	
	test_mongodb_op();
	
	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	wait_any_key(argc, argv);
	return 0;
}
