// c_mongo_connection_init_test.cpp

//mongo_accesser_test_of_connection.cpp

#include <test/wait_any_key/wait_any_key.hpp>
//#include <fstream>
//
//#include <boost/bind.hpp>
//
//#include <yggr/container/list.hpp>
//#include <yggr/container/map.hpp>
//#include <yggr/container/align_vector.hpp>
//#include <yggr/nsql_database_system/c_bson.hpp>
//#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_init.hpp>
//#include <yggr/nsql_database_system/c_mongo_index_description.hpp>
//#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>
//#include <yggr/nsql_database_system/c_mongo_index_model.hpp>
//#include <yggr/nsql_database_system/mongo_bulk_operation_helper.hpp>
//#include <yggr/nsql_database_system/mongo_utility.hpp>
//
//#include <yggr/nsql_database_system/mongo_accesser.hpp>

//#include <yggr/utility/to_pointer.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_init conn_init_type;
	typedef conn_init_type::uri_type uri_type;

#ifdef MONGOC_ENABLE_SSL
	typedef conn_init_type::ssl_opt_type ssl_opt_type;
	typedef conn_init_type::internal_tls_opts_type internal_tls_opts_type;
#endif // MONGOC_ENABLE_SSL

	{
		conn_init_type init;
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1/");
	}

	{
		conn_init_type init("mongodb://127.0.0.1:10098/?ssl=false");
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init(boost::move(uri));
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init(uri);
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

#ifdef MONGOC_ENABLE_SSL
	{
		conn_init_type init("mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
								"./nsql_database_system/cert/client.pem",		// pem_file
								"",										// pem_pwd
								"./nsql_database_system/cert/ca.pem",	// ca_file
								"./nsql_database_system/cert",				// ca_dir
								"",										// crl_file
								false, false, 
								internal_tls_opts_type(false, false)
							);
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=true");

		ssl_opt_type opt("./nsql_database_system/cert/client.pem",		// pem_file
							"",										// pem_pwd
							"./nsql_database_system/cert/ca.pem",	// ca_file
							"./nsql_database_system/cert",				// ca_dir
							"",										// crl_file
							false, false, 
							internal_tls_opts_type(false, false));
		conn_init_type init(boost::move(uri), boost::move(opt));
		yggr_test_assert(init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=true");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=true");

		ssl_opt_type opt("./nsql_database_system/cert/client.pem",		// pem_file
							"",										// pem_pwd
							"./nsql_database_system/cert/ca.pem",	// ca_file
							"./nsql_database_system/cert",				// ca_dir
							"",										// crl_file
							false, false, 
							internal_tls_opts_type(false, false));
		conn_init_type init(boost::move(uri), opt);
		yggr_test_assert(init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=true");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=true");

		ssl_opt_type opt("./nsql_database_system/cert/client.pem",		// pem_file
							"",										// pem_pwd
							"./nsql_database_system/cert/ca.pem",	// ca_file
							"./nsql_database_system/cert",				// ca_dir
							"",										// crl_file
							false, false, 
							internal_tls_opts_type(false, false));
		conn_init_type init(uri, boost::move(opt));
		yggr_test_assert(init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=true");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=true");

		ssl_opt_type opt("./nsql_database_system/cert/client.pem",		// pem_file
							"",										// pem_pwd
							"./nsql_database_system/cert/ca.pem",	// ca_file
							"./nsql_database_system/cert",				// ca_dir
							"",										// crl_file
							false, false, 
							internal_tls_opts_type(false, false));
		conn_init_type init(uri, opt);
		yggr_test_assert(init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=true");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		conn_init_type init_sample("mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);

		conn_init_type init(boost::move(init_sample));
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		conn_init_type init_sample("mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);

		conn_init_type init(init_sample);
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

#endif // MONGOC_ENABLE_SSL

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init_sample(uri);
		conn_init_type init(boost::move(init_sample));
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init_sample(uri);
		conn_init_type init(init_sample);
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

	std::cout << "---------test_construct end--------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_init conn_init_type;
	typedef conn_init_type::uri_type uri_type;

#ifdef MONGOC_ENABLE_SSL
	typedef conn_init_type::ssl_opt_type ssl_opt_type;
	typedef conn_init_type::internal_tls_opts_type internal_tls_opts_type;
#endif // MONGOC_ENABLE_SSL


#ifdef MONGOC_ENABLE_SSL

	{
		conn_init_type init_sample("mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);

		conn_init_type init;
		init = (boost::move(init_sample));
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

	{
		conn_init_type init_sample("mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);

		conn_init_type init;
		init = (init_sample);
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
		yggr_test_assert(init.ssl_opt().var_pem_file() == "./nsql_database_system/cert/client.pem");
	}

#endif // MONGOC_ENABLE_SSL

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init_sample(uri);
		conn_init_type init;
		init = (boost::move(init_sample));
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init_sample(uri);
		conn_init_type init;
		init = (init_sample);
		yggr_test_assert(!init.is_using_tls());
		yggr_test_assert(!init.empty());
		yggr_test_assert(init.uri().var_uri_string() == "mongodb://127.0.0.1:10098/?ssl=false");
	}

	std::cout << "---------test_operator_set end--------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_init conn_init_type;
	typedef conn_init_type::uri_type uri_type;

#ifdef MONGOC_ENABLE_SSL
	typedef conn_init_type::ssl_opt_type ssl_opt_type;
	typedef conn_init_type::internal_tls_opts_type internal_tls_opts_type;
#endif // MONGOC_ENABLE_SSL


#ifdef MONGOC_ENABLE_SSL

	{
		conn_init_type init_sample1("mongodb://127.0.0.1:10098/?ssl=false",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);

		conn_init_type init_sample2("mongodb://127.0.0.1:10098/?ssl=true",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);

		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			init1.swap(boost::move(init2));

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			init1.swap(init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		//std::swap
		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			std::swap(boost::move(init1), init2);
			std::swap(init1, boost::move(init2));
			std::swap(init1, init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		//boost::swap
		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			boost::swap(boost::move(init1), init2);
			boost::swap(init1, boost::move(init2));
			boost::swap(init1, init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		//yggr::swap
		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			yggr::swap(boost::move(init1), init2);
			yggr::swap(init1, boost::move(init2));
			yggr::swap(init1, init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}
	}

#endif // MONGOC_ENABLE_SSL

	{
		conn_init_type init_sample1("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init_sample2("mongodb://127.0.0.1:10098/?ssl=true");

		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			init1.swap(boost::move(init2));

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			init1.swap(init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		//std::swap
		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			std::swap(boost::move(init1), init2);
			std::swap(init1, boost::move(init2));
			std::swap(init1, init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		//boost::swap
		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			boost::swap(boost::move(init1), init2);
			boost::swap(init1, boost::move(init2));
			boost::swap(init1, init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}

		//yggr::swap
		{
			conn_init_type init1(init_sample1);
			conn_init_type init2(init_sample2);

			yggr::swap(boost::move(init1), init2);
			yggr::swap(init1, boost::move(init2));
			yggr::swap(init1, init2);

			yggr_test_assert(init1.uri().var_uri_string() == init_sample2.uri().var_uri_string());
			yggr_test_assert(init2.uri().var_uri_string() == init_sample1.uri().var_uri_string());

			yggr_test_assert(init1.ssl_opt().var_pem_file() == init_sample2.ssl_opt().var_pem_file());
			yggr_test_assert(init2.ssl_opt().var_pem_file() == init_sample1.ssl_opt().var_pem_file());
		}
	}

	std::cout << "---------test_swap end--------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_init conn_init_type;
	typedef conn_init_type::uri_type uri_type;

#ifdef MONGOC_ENABLE_SSL
	typedef conn_init_type::ssl_opt_type ssl_opt_type;
	typedef conn_init_type::internal_tls_opts_type internal_tls_opts_type;
#endif // MONGOC_ENABLE_SSL

	// empty
	{
		{
			conn_init_type init("");
			bool bchk = init.empty();
			yggr_test_assert(bchk);
		}

		{
			conn_init_type init;
			bool bchk = init.empty();
			yggr_test_assert(!bchk);
		}
	}

	// is_using_tls
	{
		{
			conn_init_type init;
			bool bchk = init.is_using_tls();
			yggr_test_assert(!bchk);
		}

#ifdef MONGOC_ENABLE_SSL
		{
			conn_init_type init("mongodb://127.0.0.1:10098/?ssl=true",	// str_uri
									"./nsql_database_system/cert/client.pem",		// pem_file
									"",										// pem_pwd
									"./nsql_database_system/cert/ca.pem",	// ca_file
									"./nsql_database_system/cert",				// ca_dir
									"",										// crl_file
									false, false, 
									internal_tls_opts_type(false, false)
								);
			bool bchk = init.is_using_tls();
			yggr_test_assert(bchk);
		}
#endif // MONGOC_ENABLE_SSL
	}

	// uri
	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=false");
		conn_init_type init;

		const conn_init_type& init_cref = init;

		init.uri() = uri;
		yggr_test_assert(init.uri().var_uri_string() == uri.var_uri_string());
		yggr_test_assert(init_cref.uri().var_uri_string() == uri.var_uri_string());
	}

#ifdef MONGOC_ENABLE_SSL
	{
		uri_type uri("mongodb://127.0.0.1:10098/?ssl=true");

		ssl_opt_type opt("./nsql_database_system/cert/client.pem",		// pem_file
							"",										// pem_pwd
							"./nsql_database_system/cert/ca.pem",	// ca_file
							"./nsql_database_system/cert",				// ca_dir
							"",										// crl_file
							false, false, 
							internal_tls_opts_type(false, false));
		
		conn_init_type init;

		const conn_init_type& init_cref = init;

		init.uri() = uri;
		init.ssl_opt() = opt;

		yggr_test_assert(init.ssl_opt().var_pem_file() == opt.var_pem_file());
		yggr_test_assert(init_cref.ssl_opt().var_pem_file() == opt.var_pem_file());
	}
#endif // MONGOC_ENABLE_SSL

	std::cout << "---------test_member_foo end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}