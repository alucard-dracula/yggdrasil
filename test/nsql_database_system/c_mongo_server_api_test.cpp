// c_mongo_server_api_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_server_api.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


// test
void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_server_api server_api_type;

	{
		server_api_type svr_api;

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(!svr_api.strict_value());
		yggr_test_assert(!svr_api.strict_is_set());

		yggr_test_assert(!svr_api.deprecation_errors_value());
		yggr_test_assert(!svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api(server_api_type::E_MONGOC_SERVER_API_V1, true, true);

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api(svr_api_sample.org_pointer());

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api(*svr_api_sample.org_pointer());

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api(boost::move(svr_api_sample));

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api(svr_api_sample);

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_server_api server_api_type;

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api;
		svr_api = (*svr_api_sample.org_pointer());

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api;
		svr_api = boost::move(svr_api_sample);

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	{
		server_api_type svr_api_sample(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api;
		svr_api = svr_api_sample;

		yggr_test_assert(svr_api.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(svr_api.strict_value());
		yggr_test_assert(svr_api.strict_is_set());

		yggr_test_assert(svr_api.deprecation_errors_value());
		yggr_test_assert(svr_api.deprecation_errors_is_set());
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_server_api server_api_type;

	{
		server_api_type svr_api_sample1(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api_sample12;

		server_api_type svr_api1(svr_api_sample1);
		server_api_type svr_api2(svr_api_sample12);

		svr_api1.swap(*svr_api2.org_pointer());

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		svr_api1.swap(boost::move(svr_api2));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		svr_api1.swap(svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
	}

	{
		server_api_type svr_api_sample1(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api_sample12;

		server_api_type svr_api1(svr_api_sample1);
		server_api_type svr_api2(svr_api_sample12);

		std::swap(svr_api1, boost::move(*svr_api2.org_pointer()));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		std::swap(boost::move(svr_api1), *svr_api2.org_pointer());

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		std::swap(boost::move(svr_api1), svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		std::swap(svr_api1, boost::move(svr_api2));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		std::swap(svr_api1, svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
	}

	{
		server_api_type svr_api_sample1(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api_sample12;

		server_api_type svr_api1(svr_api_sample1);
		server_api_type svr_api2(svr_api_sample12);

		boost::swap(svr_api1, boost::move(*svr_api2.org_pointer()));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		boost::swap(boost::move(svr_api1), *svr_api2.org_pointer());

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		boost::swap(boost::move(svr_api1), svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		boost::swap(svr_api1, boost::move(svr_api2));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		boost::swap(svr_api1, svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
	}

	{
		server_api_type svr_api_sample1(server_api_type::E_MONGOC_SERVER_API_V1, true, true);
		server_api_type svr_api_sample12;

		server_api_type svr_api1(svr_api_sample1);
		server_api_type svr_api2(svr_api_sample12);

		yggr::swap(svr_api1, boost::move(*svr_api2.org_pointer()));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		yggr::swap(boost::move(svr_api1), *svr_api2.org_pointer());

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		yggr::swap(boost::move(svr_api1), svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));

		yggr::swap(svr_api1, boost::move(svr_api2));

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));

		yggr::swap(svr_api1, svr_api2);

		yggr_test_assert(0 == memcmp(svr_api1.org_pointer(), svr_api_sample12.org_pointer(), sizeof(server_api_type)));
		yggr_test_assert(0 == memcmp(svr_api2.org_pointer(), svr_api_sample1.org_pointer(), sizeof(server_api_type)));
	}
	
	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_server_api server_api_type;

	{
		server_api_type svr_api;

		server_api_type& svr_api_ref = svr_api;
		const server_api_type& svr_api_cref = svr_api;

		svr_api_ref.var_version() = server_api_type::E_MONGOC_SERVER_API_V1;
		yggr_test_assert(svr_api_cref.var_version() == server_api_type::E_MONGOC_SERVER_API_V1);

		yggr_test_assert(!svr_api_cref.strict_is_set());
		svr_api_ref.strict_value() = true;
		yggr_test_assert(svr_api_cref.strict_value());
		yggr_test_assert(svr_api_cref.strict_is_set());

		yggr_test_assert(!svr_api_cref.deprecation_errors_is_set());
		svr_api_ref.deprecation_errors_value() = true;
		yggr_test_assert(svr_api_cref.deprecation_errors_value());
		yggr_test_assert(svr_api_cref.deprecation_errors_is_set());
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