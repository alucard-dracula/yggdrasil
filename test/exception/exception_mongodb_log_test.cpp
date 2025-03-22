//exception_mongodb_log_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/archive/bson_archive_partner.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/serialization/bson_inner_data.hpp>

#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/log/log_of_mongodb.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/exception/serialization/error_code.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

#define MONGODB_USE_CA() 0
#define MONGODB_USE_AUTH() 0

void test(void)
{
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


	typedef yggr::shared_ptr<log_acc_type> log_acc_ptr_type;

	typedef yggr::exception::exception exception_type;
	typedef yggr::ptr_single<exception_type> exception_single_type;
	typedef exception_single_type::obj_ptr_type exception_ptr_type;

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

	log_acc_ptr_type pacc(new log_acc_type(client_init));
	yggr_test_assert(pacc);

	log_mongodb_op_caller_type caller("test_log", "exception_test");

	pacc->register_msg(1, caller);
	pacc->register_msg(2, caller);

	exception_ptr_type pe = exception_single_type::init_ins();
	yggr_test_assert(pe);

	pe->bind_log_accesser(pacc);

	exception_type::throw_error(1, "test_err1");
	exception_type::throw_error(2, "test_err2");
	exception_type::throw_error(3, "test_err3");

	pe->stop();
	pe->join();

	exception_single_type::uninstall();
}


int main(int argc, char* argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();
	test();
	yggr::nsql_database_system::mongodb_installer::uninstall();

	wait_any_key(argc, argv);
	return 0;
}