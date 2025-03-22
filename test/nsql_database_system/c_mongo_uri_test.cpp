// c_mongo_uri_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_uri.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

/*
https://mongoc.org/libmongoc/current/mongoc_uri_t.html
mongodb[+srv]://                             <1>
   [username:password@]                      <2>
   host1                                     <3>
   [:port1]                                  <4>
   [,host2[:port2],...[,hostN[:portN]]]      <5>
   [/[database]                              <6>
   [?options]]                               <7>

*/

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_uri c_mongo_uri_type;

	{
		c_mongo_uri_type uri;
		std::cout << uri << std::endl;
		yggr_test_assert(!uri.empty());
	}

	{
		c_mongo_uri_type::bson_error_type bs_err;
		c_mongo_uri_type uri(bs_err);
		std::cout << uri << std::endl;
		std::cout << bs_err << std::endl;
		yggr_test_assert(!uri.empty());
		yggr_test_assert(bs_err.code() == 0);
	}

	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type::bson_error_type bs_err;
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2",
								bs_err);
		const c_mongo_uri_type& uri_cref = uri;

		std::cout << bs_err << std::endl;
		yggr_test_assert(bs_err.code() == 0);
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type uri("127.0.0.1", 10098);
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view(""));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view(""));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type uri_tmp("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		c_mongo_uri_type uri(static_cast<c_mongo_uri_type::org_type&>(uri_tmp));
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type uri_tmp("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		c_mongo_uri_type uri(static_cast<c_mongo_uri_type::org_type*>(uri_tmp));
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type uri_tmp("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		c_mongo_uri_type uri(boost::move(uri_tmp));
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type uri_tmp("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		c_mongo_uri_type uri(uri_tmp);
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_uri c_mongo_uri_type;

	{
		
		c_mongo_uri_type uri;
		uri = "mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2";
		uri = std::string("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		uri = boost::container::string("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		uri = yggr::string("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type tmp_uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		c_mongo_uri_type uri;
		uri = static_cast<c_mongo_uri_type::org_type&>(tmp_uri);
		uri = static_cast<c_mongo_uri_type::org_type&>(tmp_uri);
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type tmp_uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		c_mongo_uri_type uri;
		uri = boost::move(tmp_uri);
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	{
		c_mongo_uri_type tmp_uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		c_mongo_uri_type uri;
		uri = tmp_uri;
		uri = tmp_uri;
		const c_mongo_uri_type& uri_cref = uri;
		
		yggr_test_assert(!uri.empty());
		std::cout << "uri: " << uri << std::endl;
		
		std::cout << "var_scheme:" << uri.var_scheme() << std::endl;
		yggr_test_assert(uri.var_scheme() == "mongodb://");

		c_mongo_uri_type::mongo_host_info_list_type hosts = uri.var_hosts();
		yggr_test_assert(hosts.size() == 1);
		std::size_t host_i = 0;
		for(c_mongo_uri_type::mongo_host_info_list_type::const_iterator i = hosts.begin(), isize = hosts.end(); 
			i != isize; ++i)
		{
			std::cout << "host_info " << host_i << ": " << *i << std::endl;
			yggr_test_assert((*i).var_host() == "127.0.0.1");
			yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
			yggr_test_assert((*i).var_port() == 10098);
			yggr_test_assert((*i).var_family() == 0);
		}
		
		yggr_test_assert( uri.var_srv_hostname().empty());
		std::cout << "var_srv_hostname: " << uri.var_srv_hostname() << std::endl;

		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		std::cout << "var_srv_service_name: " << uri.var_srv_service_name() << std::endl;

		yggr_test_assert(uri.var_database() == yggr::utf8_string_view(""));
		std::cout << "var_database: " << uri.var_database() << std::endl;

		std::cout << "var_options: " << uri.var_options() << std::endl;

		yggr_test_assert(uri.var_username() == yggr::utf8_string_view("username"));
		std::cout << "var_username: " << uri.var_username() << std::endl;

		yggr_test_assert(uri.var_passwd() == yggr::utf8_string_view("passwd"));
		std::cout << "var_passwd: " << uri.var_passwd() << std::endl;

		yggr_test_assert(uri_cref.auth_mechanism().empty());
		std::cout << "get_auth_mechanism: " << uri_cref.auth_mechanism() << std::endl;
		
		yggr_test_assert(uri.auth_source() == "admin");
		std::cout << "get_auth_source: " << uri.auth_source() << std::endl;

		yggr_test_assert(uri.var_compressors().empty());
		std::cout << "var_compressors: " << uri.var_compressors() << std::endl;
		
		yggr_test_assert(uri.get_mechanism_properties().empty());
		std::cout << "get_mechanism_properties: " << uri.get_mechanism_properties() << std::endl;

		std::cout << "var_read_concern: " << uri_cref.var_read_concern() << std::endl;

		std::cout << "var_read_prefs: " << uri_cref.var_read_prefs() << std::endl;

		yggr_test_assert(uri.get_replica_set().empty());
		std::cout << "get_replica_set: " << uri.get_replica_set() << std::endl;

		yggr_test_assert(!uri.get_tls());
		std::cout << "get_tls: " << uri.get_tls() << std::endl;

		std::cout << "var_write_concern: " << uri_cref.var_write_concern() << std::endl;
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_uri c_mongo_uri_type;

	{
		c_mongo_uri_type uri1("mongodb://127.0.0.1:10098");
		c_mongo_uri_type uri2("mongodb+srv://127.0.0.1");

		yggr_test_assert(uri1.var_scheme() == "mongodb://");
		yggr_test_assert(uri2.var_scheme() == "mongodb+srv://");

		uri1.swap(boost::move(uri2));
		yggr_test_assert(uri1.var_scheme() == "mongodb+srv://");
		yggr_test_assert(uri2.var_scheme() == "mongodb://");

		uri1.swap(uri2);
		yggr_test_assert(uri1.var_scheme() == "mongodb://");
		yggr_test_assert(uri2.var_scheme() == "mongodb+srv://");

		std::swap(uri1, boost::move(uri2));
		yggr_test_assert(uri1.var_scheme() == "mongodb+srv://");
		yggr_test_assert(uri2.var_scheme() == "mongodb://");

		std::swap(uri1, uri2);
		yggr_test_assert(uri1.var_scheme() == "mongodb://");
		yggr_test_assert(uri2.var_scheme() == "mongodb+srv://");

		boost::swap(uri1, boost::move(uri2));
		yggr_test_assert(uri1.var_scheme() == "mongodb+srv://");
		yggr_test_assert(uri2.var_scheme() == "mongodb://");

		boost::swap(uri1, uri2);
		yggr_test_assert(uri1.var_scheme() == "mongodb://");
		yggr_test_assert(uri2.var_scheme() == "mongodb+srv://");

		yggr::swap(uri1, boost::move(uri2));
		yggr_test_assert(uri1.var_scheme() == "mongodb+srv://");
		yggr_test_assert(uri2.var_scheme() == "mongodb://");

		yggr::swap(uri1, uri2);
		yggr_test_assert(uri1.var_scheme() == "mongodb://");
		yggr_test_assert(uri2.var_scheme() == "mongodb+srv://");
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_uri c_mongo_uri_type;

	// empty()
	{
		c_mongo_uri_type uri;
		yggr_test_assert(!uri.empty());
		yggr_test_assert(!uri.failed());

		c_mongo_uri_type uri2("mongodb+srv://127.0.0.1:10098");
		yggr_test_assert(uri2.empty());
		yggr_test_assert(uri2.failed());
	}

	// var_scheme
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		yggr_test_assert(uri.var_scheme() == "mongodb://");
		yggr_test_assert(uri.var_scheme(yggr::utf8_string::allocator_type()) == "mongodb://");
		yggr_test_assert(uri.var_scheme<std::string>() == "mongodb://");
		yggr_test_assert(uri.var_scheme<std::wstring>() == L"mongodb://");
		yggr_test_assert(uri.var_scheme<boost::container::string>() == "mongodb://");
		yggr_test_assert(uri.var_scheme<boost::container::wstring>() == L"mongodb://");
		yggr_test_assert(uri.var_scheme<yggr::string>() == "mongodb://");
		yggr_test_assert(uri.var_scheme<yggr::wstring>() == L"mongodb://");
	}

	// var_hosts
	{
		typedef c_mongo_uri_type::mongo_host_info_list_type mongo_host_info_list_type;
		typedef mongo_host_info_list_type::allocator_type mongo_host_info_list_aloc_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		
		{
			mongo_host_info_list_type host_list;
			uri.var_hosts(host_list);
			yggr_test_assert(host_list.size() == 1);

			for(mongo_host_info_list_type::const_iterator i = host_list.begin(), isize= host_list.end(); i != isize; ++i)
			{
				yggr_test_assert((*i).var_host() == "127.0.0.1");
				yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
				yggr_test_assert((*i).var_port() == 10098);
				yggr_test_assert((*i).var_family() == 0);
			}
		}

		{
			mongo_host_info_list_type host_list = uri.var_hosts<mongo_host_info_list_type>();
			yggr_test_assert(host_list.size() == 1);

			for(mongo_host_info_list_type::const_iterator i = host_list.begin(), isize= host_list.end(); i != isize; ++i)
			{
				yggr_test_assert((*i).var_host() == "127.0.0.1");
				yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
				yggr_test_assert((*i).var_port() == 10098);
				yggr_test_assert((*i).var_family() == 0);
			}
		}

		{
			mongo_host_info_list_type host_list = uri.var_hosts<mongo_host_info_list_type>(mongo_host_info_list_aloc_type());
			yggr_test_assert(host_list.size() == 1);

			for(mongo_host_info_list_type::const_iterator i = host_list.begin(), isize= host_list.end(); i != isize; ++i)
			{
				yggr_test_assert((*i).var_host() == "127.0.0.1");
				yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
				yggr_test_assert((*i).var_port() == 10098);
				yggr_test_assert((*i).var_family() == 0);
			}
		}

		{
			mongo_host_info_list_type host_list = uri.var_hosts();
			yggr_test_assert(host_list.size() == 1);

			for(mongo_host_info_list_type::const_iterator i = host_list.begin(), isize= host_list.end(); i != isize; ++i)
			{
				yggr_test_assert((*i).var_host() == "127.0.0.1");
				yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
				yggr_test_assert((*i).var_port() == 10098);
				yggr_test_assert((*i).var_family() == 0);
			}
		}

		{
			mongo_host_info_list_type host_list = uri.var_hosts(mongo_host_info_list_aloc_type());
			yggr_test_assert(host_list.size() == 1);

			for(mongo_host_info_list_type::const_iterator i = host_list.begin(), isize= host_list.end(); i != isize; ++i)
			{
				yggr_test_assert((*i).var_host() == "127.0.0.1");
				yggr_test_assert((*i).var_host_and_port() == "127.0.0.1:10098");
				yggr_test_assert((*i).var_port() == 10098);
				yggr_test_assert((*i).var_family() == 0);
			}
		}
	}

	// var_srv_hostname
	{
		{
			c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
			yggr_test_assert(uri.var_srv_hostname().empty());
		}

		{
			c_mongo_uri_type uri("mongodb+srv://127.0.0.1");
			yggr_test_assert(uri.var_srv_hostname() == "127.0.0.1");
		}

		c_mongo_uri_type uri("mongodb+srv://127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname() == "127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname(yggr::utf8_string::allocator_type()) == "127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname<std::string>() == "127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname<std::wstring>() == L"127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname<boost::container::string>() == "127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname<boost::container::wstring>() == L"127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname<yggr::string>() == "127.0.0.1");
		yggr_test_assert(uri.var_srv_hostname<yggr::wstring>() == L"127.0.0.1");
	}

	// var_srv_service_name
	{
		c_mongo_uri_type uri("mongodb+srv://127.0.0.1");
		yggr_test_assert(uri.var_srv_service_name() == "mongodb");
		yggr_test_assert(uri.var_srv_service_name(yggr::utf8_string::allocator_type()) == "mongodb");
		yggr_test_assert(uri.var_srv_service_name<std::string>() == "mongodb");
		yggr_test_assert(uri.var_srv_service_name<std::wstring>() == L"mongodb");
		yggr_test_assert(uri.var_srv_service_name<boost::container::string>() == "mongodb");
		yggr_test_assert(uri.var_srv_service_name<boost::container::wstring>() == L"mongodb");
		yggr_test_assert(uri.var_srv_service_name<yggr::string>() == "mongodb");
		yggr_test_assert(uri.var_srv_service_name<yggr::wstring>() == L"mongodb");
	}

	// var_database
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/db_name?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		c_mongo_uri_type& uri_ref = uri;
		const c_mongo_uri_type& uri_cref = uri;

		yggr_test_assert(uri.var_database() == "db_name");
		yggr_test_assert(uri.var_database(yggr::utf8_string::allocator_type()) == "db_name");
		yggr_test_assert(uri.var_database<std::string>() == "db_name");
		yggr_test_assert(uri.var_database<std::wstring>() == L"db_name");
		yggr_test_assert(uri.var_database<boost::container::string>() == "db_name");
		yggr_test_assert(uri.var_database<boost::container::wstring>() == L"db_name");
		yggr_test_assert(uri.var_database<yggr::string>() == "db_name");
		yggr_test_assert(uri.var_database<yggr::wstring>() == L"db_name");

		yggr_test_assert(uri_ref.var_database() == "db_name");
		yggr_test_assert(uri_cref.var_database(yggr::utf8_string::allocator_type()) == "db_name");
		yggr_test_assert(uri_cref.var_database<std::string>() == "db_name");
		yggr_test_assert(uri_cref.var_database<std::wstring>() == L"db_name");
		yggr_test_assert(uri_cref.var_database<boost::container::string>() == "db_name");
		yggr_test_assert(uri_cref.var_database<boost::container::wstring>() == L"db_name");
		yggr_test_assert(uri_cref.var_database<yggr::string>() == "db_name");
		yggr_test_assert(uri_cref.var_database<yggr::wstring>() == L"db_name");

		uri.var_database() = "db_name2";
		yggr_test_assert(uri.var_database() == "db_name2");

		uri.var_database() = L"db_name3";
		yggr_test_assert(uri.var_database() == "db_name3");

		uri.var_database() = std::string("db_name4");
		yggr_test_assert(uri.var_database() == "db_name4");

		uri.var_database() = std::wstring(L"db_name5");
		yggr_test_assert(uri.var_database() == "db_name5");

		uri.var_database() = boost::container::string("db_name6");
		yggr_test_assert(uri.var_database() == "db_name6");

		uri.var_database() = boost::container::wstring(L"db_name7");
		yggr_test_assert(uri.var_database() == "db_name7");

		uri.var_database() = yggr::string("db_name8");
		yggr_test_assert(uri.var_database() == "db_name8");

		uri.var_database() = yggr::wstring(L"db_name9");
		yggr_test_assert(uri.var_database() == "db_name9");

		uri.var_database() = yggr::charset::make_string_charset_helper("db_name10");
		yggr_test_assert(uri.var_database() == "db_name10");

		uri.var_database() = yggr::charset::make_string_charset_helper(L"db_name11");
		yggr_test_assert(uri.var_database() == "db_name11");
	}

	//var_username
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/db_name?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		yggr_test_assert(uri.var_username() == "username");
		yggr_test_assert(uri.var_username(yggr::utf8_string::allocator_type()) == "username");
		yggr_test_assert(uri.var_username<std::string>() == "username");
		yggr_test_assert(uri.var_username<std::wstring>() == L"username");
		yggr_test_assert(uri.var_username<boost::container::string>() == "username");
		yggr_test_assert(uri.var_username<boost::container::wstring>() == L"username");
		yggr_test_assert(uri.var_username<yggr::string>() == "username");
		yggr_test_assert(uri.var_username<yggr::wstring>() == L"username");

		uri.var_username() = "user_name2";
		yggr_test_assert(uri.var_username() == "user_name2");

		uri.var_username() = L"user_name3";
		yggr_test_assert(uri.var_username() == "user_name3");

		uri.var_username() = std::string("user_name4");
		yggr_test_assert(uri.var_username() == "user_name4");

		uri.var_username() = std::wstring(L"user_name5");
		yggr_test_assert(uri.var_username() == "user_name5");

		uri.var_username() = boost::container::string("user_name6");
		yggr_test_assert(uri.var_username() == "user_name6");

		uri.var_username() = boost::container::wstring(L"user_name7");
		yggr_test_assert(uri.var_username() == "user_name7");

		uri.var_username() = yggr::string("user_name8");
		yggr_test_assert(uri.var_username() == "user_name8");

		uri.var_username() = yggr::wstring(L"user_name9");
		yggr_test_assert(uri.var_username() == "user_name9");

		uri.var_username() = yggr::charset::make_string_charset_helper("user_name10");
		yggr_test_assert(uri.var_username() == "user_name10");

		uri.var_username() = yggr::charset::make_string_charset_helper(L"user_name11");
		yggr_test_assert(uri.var_username() == "user_name11");
	}

	//var_pwd
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/db_name?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		yggr_test_assert(uri.var_passwd() == "passwd");
		yggr_test_assert(uri.var_passwd(yggr::utf8_string::allocator_type()) == "passwd");
		yggr_test_assert(uri.var_passwd<std::string>() == "passwd");
		yggr_test_assert(uri.var_passwd<std::wstring>() == L"passwd");
		yggr_test_assert(uri.var_passwd<boost::container::string>() == "passwd");
		yggr_test_assert(uri.var_passwd<boost::container::wstring>() == L"passwd");
		yggr_test_assert(uri.var_passwd<yggr::string>() == "passwd");
		yggr_test_assert(uri.var_passwd<yggr::wstring>() == L"passwd");

		uri.var_passwd() = "passwd2";
		yggr_test_assert(uri.var_passwd() == "passwd2");

		uri.var_passwd() = L"passwd3";
		yggr_test_assert(uri.var_passwd() == "passwd3");

		uri.var_passwd() = std::string("passwd4");
		yggr_test_assert(uri.var_passwd() == "passwd4");

		uri.var_passwd() = std::wstring(L"passwd5");
		yggr_test_assert(uri.var_passwd() == "passwd5");

		uri.var_passwd() = boost::container::string("passwd6");
		yggr_test_assert(uri.var_passwd() == "passwd6");

		uri.var_passwd() = boost::container::wstring(L"passwd7");
		yggr_test_assert(uri.var_passwd() == "passwd7");

		uri.var_passwd() = yggr::string("passwd8");
		yggr_test_assert(uri.var_passwd() == "passwd8");

		uri.var_passwd() = yggr::wstring(L"passwd9");
		yggr_test_assert(uri.var_passwd() == "passwd9");

		uri.var_passwd() = yggr::charset::make_string_charset_helper("passwd10");
		yggr_test_assert(uri.var_passwd() == "passwd10");

		uri.var_passwd() = yggr::charset::make_string_charset_helper(L"passwd11");
		yggr_test_assert(uri.var_passwd() == "passwd11");
	}

	//var_options
	{
		typedef c_mongo_uri_type::bson_type bson_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/db_name?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		{
			//{ "directconnection" : true, "serverselectiontimeoutms" : 2000, "appname" : "mongosh+1.8.2" }
			bson_type opt = uri.var_options();

			std::cout << opt << std::endl;

			bool directconnection = opt.get_value<bool>("directconnection");
			yggr_test_assert(directconnection == true);

			yggr::s32 serverselectiontimeoutms = opt.get_value<yggr::s32>("serverselectiontimeoutms");
			yggr_test_assert(serverselectiontimeoutms == 2000);

			yggr::utf8_string appname = opt.get_value<yggr::utf8_string>("appname");
			yggr_test_assert(appname == "mongosh+1.8.2");
		}

		{
			bson_type::base_type base_opt = uri.var_options<bson_type::base_type>();

			std::cout << base_opt << std::endl;

			bson_type& opt = reinterpret_cast<bson_type&>(base_opt);

			bool directconnection = opt.get_value<bool>("directconnection");
			yggr_test_assert(directconnection == true);

			yggr::s32 serverselectiontimeoutms = opt.get_value<yggr::s32>("serverselectiontimeoutms");
			yggr_test_assert(serverselectiontimeoutms == 2000);

			yggr::utf8_string appname = opt.get_value<yggr::utf8_string>("appname");
			yggr_test_assert(appname == "mongosh+1.8.2");
		}

		{
			bson_type& opt_ref = uri.var_options();
			opt_ref.save(MONGOC_URI_RETRYREADS, true);
			yggr_test_assert(uri.has_option(MONGOC_URI_RETRYREADS));
		}
	}

	// has_options option_is
	{
		//{ "directconnection" : true, "serverselectiontimeoutms" : 2000, "appname" : "mongosh+1.8.2" }
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/db_name?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		
		yggr_test_assert(uri.has_option("directconnection"));
		yggr_test_assert(uri.has_option("serverselectiontimeoutms"));
		yggr_test_assert(uri.has_option("appname"));
		yggr_test_assert(!uri.has_option("not_exists_options"));

		yggr_test_assert(c_mongo_uri_type::option_is_bool("directconnection"));
		yggr_test_assert(c_mongo_uri_type::option_is_int32("serverselectiontimeoutms"));
		yggr_test_assert(c_mongo_uri_type::option_is_int64(MONGOC_URI_WTIMEOUTMS));
		yggr_test_assert(c_mongo_uri_type::option_is_utf8("appname"));

		yggr_test_assert(!c_mongo_uri_type::option_is_bool("not_exists_options"));
		yggr_test_assert(!c_mongo_uri_type::option_is_int32("not_exists_options"));
		yggr_test_assert(!c_mongo_uri_type::option_is_int64("not_exists_options"));
		yggr_test_assert(!c_mongo_uri_type::option_is_utf8("not_exists_options"));

		{
			yggr_test_assert(!uri.has_option(MONGOC_URI_WTIMEOUTMS));

			bool bchk = uri.set_option_as_int64(MONGOC_URI_WTIMEOUTMS, 1000);
			yggr_test_assert(bchk);

			yggr_test_assert(uri.has_option(MONGOC_URI_WTIMEOUTMS));

			yggr::s64 val = uri.get_option_as_int64(MONGOC_URI_WTIMEOUTMS, 0);
			yggr_test_assert(val == 1000);

		}
	}

	// set_options
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/db_name?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");

		bool bchk = false;
		
		// set_option_as_bool
		{
			bool val = false;
			val = uri.get_option_as_bool("directconnection", false);
			yggr_test_assert(val == true);

			bchk = uri.set_option_as_bool("directconnection", false);
			yggr_test_assert(bchk);

			val = uri.get_option_as_bool("directconnection", true);
			yggr_test_assert(val == false);
		}

		// set_option_as_int32
		{
			yggr::s32 val = 0;
			val = uri.get_option_as_int32("serverselectiontimeoutms", 0);
			yggr_test_assert(val == 2000);

			bchk = uri.set_option_as_int32("serverselectiontimeoutms", 3000);
			yggr_test_assert(bchk);

			val = uri.get_option_as_int32("serverselectiontimeoutms", 0);
			yggr_test_assert(val == 3000);
		}

		// set_option_as_int64
		{
			yggr::s64 val = 0;
			val = uri.get_option_as_int64(MONGOC_URI_WTIMEOUTMS, 0);
			yggr_test_assert(val == 0);

			bchk = uri.set_option_as_int64(MONGOC_URI_WTIMEOUTMS, 3000);
			yggr_test_assert(bchk);

			val = uri.get_option_as_int64(MONGOC_URI_WTIMEOUTMS, 0);
			yggr_test_assert(val == 3000);
		}

		// set_option_as_utf8
		{
			yggr::utf8_string val;
			val = uri.get_option_as_utf8("appname", "");
			yggr_test_assert(val == "mongosh+1.8.2");

			bchk = uri.set_option_as_utf8("appname", "other_name");
			yggr_test_assert(bchk);

			val = uri.get_option_as_utf8("appname", "");
			yggr_test_assert(val == "other_name");

			bchk = uri.set_option_as_utf8(
					L"appname", 
					yggr::charset::make_string_charset_helper(L"other_name2"));
			yggr_test_assert(bchk);

			val = uri.get_option_as_utf8<yggr::utf8_string>(
					yggr::charset::make_string_charset_helper(L"appname"), 
					yggr::charset::make_string_charset_helper(""));
			yggr_test_assert(val == "other_name2");
		}
	}

	// get_options
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?wtimeoutms=1000&directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		std::cout << uri.var_options() << std::endl;

		{
			bool val = false;
			val = uri.get_option_as_bool("directconnection", false);
			yggr_test_assert(val);

			val = uri.get_option_as_bool(L"directconnection", false);
			yggr_test_assert(val);

			val = uri.get_option_as_bool(yggr::charset::utf8_string("directconnection"), false);
			yggr_test_assert(val);

			val = uri.get_option_as_bool(yggr::charset::utf8_string_view("directconnection"), false);
			yggr_test_assert(val);

			val = uri.get_option_as_bool(yggr::charset::make_string_charset_helper("directconnection"), false);
			yggr_test_assert(val);
		}

		{
			yggr::s32 val = 0;
			val = uri.get_option_as_int32("serverselectiontimeoutms", 0);
			yggr_test_assert(val == 2000);

			val = uri.get_option_as_int32(L"serverselectiontimeoutms", 0);
			yggr_test_assert(val == 2000);

			val = uri.get_option_as_int32(yggr::charset::utf8_string("serverselectiontimeoutms"), 0);
			yggr_test_assert(val == 2000);

			val = uri.get_option_as_int32(yggr::charset::utf8_string_view("serverselectiontimeoutms"), 0);
			yggr_test_assert(val == 2000);

			val = uri.get_option_as_int32(yggr::charset::make_string_charset_helper("serverselectiontimeoutms"), 0);
			yggr_test_assert(val == 2000);
		}

		{
			yggr::s64 val = 0;
			val = uri.get_option_as_int64("wtimeoutms", 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int32(L"wtimeoutms", 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int32(yggr::charset::utf8_string("wtimeoutms"), 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int32(yggr::charset::utf8_string_view("wtimeoutms"), 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int32(yggr::charset::make_string_charset_helper("wtimeoutms"), 0);
			yggr_test_assert(val == 1000);
		}

		{
			yggr::s64 val = 0;
			val = uri.get_option_as_int64("wtimeoutms", 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int64(L"wtimeoutms", 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int64(yggr::charset::utf8_string("wtimeoutms"), 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int64(yggr::charset::utf8_string_view("wtimeoutms"), 0);
			yggr_test_assert(val == 1000);

			val = uri.get_option_as_int64(yggr::charset::make_string_charset_helper("wtimeoutms"), 0);
			yggr_test_assert(val == 1000);
		}

		{
			{
				yggr::utf8_string_view val = uri.get_option_as_utf8("appname", "");
				yggr_test_assert(val == "mongosh+1.8.2");
			}

			{
				yggr::utf8_string val = uri.get_option_as_utf8("appname", "", yggr::utf8_string::allocator_type());
				yggr_test_assert(val == "mongosh+1.8.2");
			}

			{
				yggr::string val = uri.get_option_as_utf8<yggr::string>("appname", "");
				yggr_test_assert(val == "mongosh+1.8.2");
			}

			{
				yggr::wstring val = uri.get_option_as_utf8<yggr::wstring>("appname", "", yggr::wstring::allocator_type());
				yggr_test_assert(val == L"mongosh+1.8.2");
			}
		}
	}

	// var_credentials
	{
		typedef c_mongo_uri_type::bson_type bson_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?authMechanism=SCRAM-SHA-256");
		std::cout << uri.var_credentials() << std::endl;
		std::cout << uri.var_credentials<bson_type::base_type>() << std::endl;
	}

	// auth_mechanism
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?authMechanism=SCRAM-SHA-256");

		yggr_test_assert(uri.auth_mechanism() == "SCRAM-SHA-256");

		uri.auth_mechanism() = "GSSAPI";

		yggr_test_assert(uri.auth_mechanism() == "GSSAPI");
		yggr_test_assert(uri.auth_mechanism(yggr::utf8_string::allocator_type()) == "GSSAPI");
		yggr_test_assert(uri.auth_mechanism<yggr::string>() == "GSSAPI");
		yggr_test_assert(uri.auth_mechanism<yggr::wstring>(yggr::wstring::allocator_type()) == L"GSSAPI");
	}

	// auth_mechanism
	{
		typedef c_mongo_uri_type::bson_type bson_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?authSource=mydb");

		std::cout << uri.var_credentials() << std::endl;
		std::cout << uri.var_credentials<bson_type::base_type>() << std::endl;

		yggr_test_assert(uri.auth_source() == "mydb");

		uri.auth_source() = "mydb2";

		yggr_test_assert(uri.auth_source() == "mydb2");
		yggr_test_assert(uri.auth_source(yggr::utf8_string::allocator_type()) == "mydb2");
		yggr_test_assert(uri.auth_source<yggr::string>() == "mydb2");
		yggr_test_assert(uri.auth_source<yggr::wstring>(yggr::wstring::allocator_type()) == L"mydb2");
	}

	// var_compressors
	{
		typedef c_mongo_uri_type::bson_type bson_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");
		std::cout << uri.var_compressors() << std::endl;

		yggr_test_assert(uri.var_compressors().get_value<yggr::utf8_string>("zlib") == "yes");
		yggr_test_assert(uri.var_compressors().find("snappy") == uri.var_compressors().end());
		yggr_test_assert(uri.var_compressors().find("zstd") == uri.var_compressors().end());

	}

	// set_mechanism_properties
	{
		typedef c_mongo_uri_type::bson_type bson_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?authMechanism=GSSAPI&authMechanismProperties=SERVICE_NAME:other,CANONICALIZE_HOST_NAME:true");
		std::cout << uri.var_credentials() << std::endl;

		bson_type bs;
		uri.get_mechanism_properties(bs);

		std::cout << bs << std::endl;

		yggr_test_assert(bs.get_value<yggr::utf8_string>("SERVICE_NAME") == "other");

		bson_type props;
		props.save("SERVICE_NAME", "my_service");
		uri.set_mechanism_properties(props);

		std::cout << uri.get_mechanism_properties(bs) << std::endl;

		yggr_test_assert(bs.get_value<yggr::utf8_string>("SERVICE_NAME") == "my_service");
	}

	// var_read_concern
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?wtimeoutms=1000&directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		c_mongo_uri_type& uri_ref(uri);
		const c_mongo_uri_type& uri_cref(uri);
		
		std::cout << uri.var_read_concern() << std::endl;
		std::cout << uri_ref.var_read_concern() << std::endl;
		std::cout << uri_cref.var_read_concern() << std::endl;
	}

	// var_read_prefs
	{
		typedef c_mongo_uri_type::bson_type bson_type;
		typedef c_mongo_uri_type::read_prefs_type read_prefs_type;

		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?wtimeoutms=1000&directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.8.2");
		c_mongo_uri_type& uri_ref(uri);
		const c_mongo_uri_type& uri_cref(uri);
		
		std::cout << uri.var_read_prefs() << std::endl;
		std::cout << uri_ref.var_read_prefs() << std::endl;
		std::cout << uri_cref.var_read_prefs() << std::endl;

		bson_type& bs = uri.var_read_prefs<bson_type>();
		std::cout << bs << std::endl;

		read_prefs_type& read_prefs_ref = uri.var_read_prefs<read_prefs_type>();
		std::cout << read_prefs_ref << std::endl;
	}

	// get_replica_set
	{
		{
			c_mongo_uri_type uri("mongodb://db1.example.com,db2.example.com:2500/?replicaSet=test");
		
			std::cout << uri.get_replica_set() << std::endl;
			yggr_test_assert(uri.get_replica_set() == "test");
			yggr_test_assert(uri.get_replica_set(yggr::utf8_string::allocator_type()) == "test");
			yggr_test_assert(uri.get_replica_set<yggr::string>() == "test");
			yggr_test_assert(uri.get_replica_set<yggr::wstring>(yggr::wstring::allocator_type()) == L"test");
		}

		{
			c_mongo_uri_type uri("mongodb+srv://server.example.com/?replicaSet=rs&appName=applicationName");
		
			std::cout << uri.get_replica_set() << std::endl;
			yggr_test_assert(uri.get_replica_set() == "rs");
			yggr_test_assert(uri.get_replica_set(yggr::utf8_string::allocator_type()) == "rs");
			yggr_test_assert(uri.get_replica_set<yggr::string>() == "rs");
			yggr_test_assert(uri.get_replica_set<yggr::wstring>(yggr::wstring::allocator_type()) == L"rs");
		}
	}

	// tsl ssl
	{
		{
			c_mongo_uri_type uri("mongodb://db1.example.com,db2.example.com:2500/?replicaSet=test");
		
			std::cout << uri.get_tls() << std::endl;

			yggr_test_assert(!uri.get_tls());
			yggr_test_assert(!uri.has_tls());
		}

		{
			c_mongo_uri_type uri("mongodb://db1.example.com,db2.example.com:2500/?tls=true");
		
			std::cout << uri.get_tls() << std::endl;
			
			yggr_test_assert(uri.get_tls());
			yggr_test_assert(uri.has_tls());
		}

		{
			c_mongo_uri_type uri("mongodb://db1.example.com,db2.example.com:2500/?ssl=true");
		
			std::cout << uri.get_tls() << std::endl;
			
			yggr_test_assert(uri.get_tls());
			yggr_test_assert(uri.has_tls());
		}
	}

	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");
		std::cout << uri.var_uri_string() << std::endl;

		yggr_test_assert(uri.var_uri_string() == "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");
		yggr_test_assert(uri.var_uri_string(yggr::utf8_string::allocator_type()) == "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");
		yggr_test_assert(uri.var_uri_string<yggr::string>() == "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");
		yggr_test_assert(uri.var_uri_string<yggr::wstring>(yggr::wstring::allocator_type()) == L"mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");

		uri.var_uri_string() = "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib";
		std::cout << uri.var_uri_string() << std::endl;

		yggr_test_assert(uri.var_uri_string() == "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib");
		yggr_test_assert(uri.var_uri_string(yggr::utf8_string::allocator_type()) == "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib");
		yggr_test_assert(uri.var_uri_string<yggr::string>() == "mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib");
		yggr_test_assert(uri.var_uri_string<yggr::wstring>(yggr::wstring::allocator_type()) == L"mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib");
	}

	// var_write_concern
	{
		c_mongo_uri_type uri("mongodb://username:passwd@127.0.0.1:10098/?compressors=snappy,zlib,zstd");

		c_mongo_uri_type& uri_ref(uri);
		const c_mongo_uri_type& uri_cref(uri);

		std::cout << uri.var_write_concern() << std::endl;
		std::cout << uri_ref.var_write_concern() << std::endl;
		std::cout << uri_cref.var_write_concern() << std::endl;
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	wait_any_key(argc, argv);
	return 0;
}
