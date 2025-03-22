// c_mongo_host_info_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_mongo_host_info.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


#if !defined(AF_INET)
#	define AF_INET 2
#endif // AF_INET

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_host_info mongo_host_info_type;
	typedef mongo_host_info_type::base_type base_mongo_host_info_type;
	typedef mongo_host_info_type::org_type org_mongo_host_info_type;

	{
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(std::strlen(base_host_info_ref.host) == 0);
		yggr_test_assert(std::strlen(base_host_info_ref.host_and_port) == 0);
		yggr_test_assert(base_host_info_ref.port == 0);
		yggr_test_assert(base_host_info_ref.family == 0);
	}

	{
		mongo_host_info_type host_info("127.0.0.1", 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info(std::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info(boost::container::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info(yggr::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info(yggr::utf8_string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info(yggr::string_view("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info(yggr::charset::make_string_charset_helper("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		org_mongo_host_info_type& org_host_info_sample_ref = reinterpret_cast<org_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info(boost::move(org_host_info_sample_ref));
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		org_mongo_host_info_type& org_host_info_sample_ref = reinterpret_cast<org_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info(org_host_info_sample_ref);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		org_mongo_host_info_type& org_host_info_sample_ref = reinterpret_cast<org_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info(boost::addressof(org_host_info_sample_ref));
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_sample_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info(boost::move(base_host_info_sample_ref));
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_sample_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info(base_host_info_sample_ref);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		mongo_host_info_type host_info(boost::move(host_info_sample));
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		mongo_host_info_type host_info(host_info_sample);
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	std::cout << "-----------test_construct-------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_host_info mongo_host_info_type;
	typedef mongo_host_info_type::base_type base_mongo_host_info_type;
	typedef mongo_host_info_type::org_type org_mongo_host_info_type;

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		org_mongo_host_info_type& org_host_info_sample_ref = reinterpret_cast<org_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		host_info = boost::move(org_host_info_sample_ref);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		org_mongo_host_info_type& org_host_info_sample_ref = reinterpret_cast<org_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		host_info = org_host_info_sample_ref;

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_sample_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		host_info = boost::move(base_host_info_sample_ref);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		base_mongo_host_info_type& base_host_info_sample_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info_sample);
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		host_info = base_host_info_sample_ref;

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		host_info = boost::move(host_info_sample);

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	{
		mongo_host_info_type host_info_sample(yggr::string("127.0.0.1"), 10098, AF_INET);
		mongo_host_info_type host_info;
		base_mongo_host_info_type& base_host_info_ref = reinterpret_cast<base_mongo_host_info_type&>(host_info);

		host_info = host_info_sample;

		yggr_test_assert(base_host_info_ref.next == 0);
		yggr_test_assert(base_host_info_ref.host == yggr::utf8_string("127.0.0.1"));
		yggr_test_assert(base_host_info_ref.host_and_port == yggr::utf8_string("127.0.0.1:10098"));
		yggr_test_assert(base_host_info_ref.port == 10098);
		yggr_test_assert(base_host_info_ref.family == AF_INET);
	}

	std::cout << "-----------test_operator_set-------------" << std::endl;
}

#if !defined(AF_INET2)
#	define AF_INET2 3
#endif // AF_INET2

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_host_info mongo_host_info_type;
	typedef mongo_host_info_type::base_type base_mongo_host_info_type;
	typedef mongo_host_info_type::org_type org_mongo_host_info_type;

	mongo_host_info_type host_info1_chk(yggr::string("127.0.0.1"), 10098, AF_INET);
	mongo_host_info_type host_info2_chk(yggr::string("127.0.0.2"), 10099, AF_INET2);

	mongo_host_info_type host_info1(host_info1_chk);
	mongo_host_info_type host_info2(host_info2_chk);

	org_mongo_host_info_type& org_host_info2 = reinterpret_cast<org_mongo_host_info_type&>(host_info2);
	base_mongo_host_info_type& base_host_info2 = reinterpret_cast<base_mongo_host_info_type&>(host_info2);
		
	{
		host_info1.swap(boost::move(org_host_info2));

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	{
		host_info1.swap(org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	{
		host_info1.swap(boost::move(base_host_info2));

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	{
		host_info1.swap(base_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	{
		host_info1.swap(boost::move(host_info2));

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	{
		host_info1.swap(host_info2);

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	// non-member swap
	// std::swap
	{
		std::swap(host_info1, boost::move(org_host_info2));

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	{
		std::swap(boost::move(host_info1), org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	{
		std::swap(host_info1, org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	// boost::swap
	{
		boost::swap(host_info1, boost::move(org_host_info2));

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	{
		boost::swap(boost::move(host_info1), org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	{
		boost::swap(host_info1, org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	// yggr::swap
	{
		yggr::swap(host_info1, boost::move(org_host_info2));

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}

	{
		yggr::swap(boost::move(host_info1), org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info1_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info1_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info1_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info1_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info1_chk.var_next());
	}

	{
		yggr::swap(host_info1, org_host_info2);

		yggr_test_assert(host_info1.var_host() == host_info2_chk.var_host());
		yggr_test_assert(host_info1.var_host_and_port() == host_info2_chk.var_host_and_port());
		yggr_test_assert(host_info1.var_port() == host_info2_chk.var_port());
		yggr_test_assert(host_info1.var_family() == host_info2_chk.var_family());
		yggr_test_assert(host_info1.var_next() == host_info2_chk.var_next());
	}
	
	std::cout << "-----------test_swap-------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_host_info mongo_host_info_type;
	typedef mongo_host_info_type::base_type base_mongo_host_info_type;
	typedef mongo_host_info_type::org_type org_mongo_host_info_type;

	mongo_host_info_type host_info(yggr::string("127.0.0.1"), 10098, AF_INET);

	// var_host
	host_info.var_host() = "127.0.0.2";

	yggr_test_assert(host_info.var_host() == "127.0.0.2");

	yggr_test_assert(host_info.var_host<std::string>() == "127.0.0.2");
	yggr_test_assert(host_info.var_host<std::string>(std::string().get_allocator()) == "127.0.0.2");

	yggr_test_assert(host_info.var_host<boost::container::string>() == "127.0.0.2");
	yggr_test_assert(host_info.var_host<boost::container::string>(boost::container::string().get_allocator()) == "127.0.0.2");

	yggr_test_assert(host_info.var_host<yggr::string>() == "127.0.0.2");
	yggr_test_assert(host_info.var_host<yggr::string>(yggr::string().get_allocator()) == "127.0.0.2");

	yggr_test_assert(host_info.var_host<yggr::utf8_string>() == "127.0.0.2");
	yggr_test_assert(host_info.var_host<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "127.0.0.2");

#ifndef YGGR_NO_CWCHAR
	yggr_test_assert(host_info.var_host<std::wstring>() == L"127.0.0.2");
	yggr_test_assert(host_info.var_host<std::wstring>(std::wstring().get_allocator()) == L"127.0.0.2");

	yggr_test_assert(host_info.var_host<boost::container::wstring>() == L"127.0.0.2");
	yggr_test_assert(host_info.var_host<boost::container::wstring>(boost::container::wstring().get_allocator()) == L"127.0.0.2");

	yggr_test_assert(host_info.var_host<yggr::wstring>() == L"127.0.0.2");
	yggr_test_assert(host_info.var_host<yggr::wstring>(yggr::wstring().get_allocator()) == L"127.0.0.2");
#endif // YGGR_NO_CWCHAR

	// var_host_and_port
	yggr_test_assert(host_info.var_host_and_port() == "127.0.0.2:10098");

	yggr_test_assert(host_info.var_host_and_port<std::string>() == "127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<std::string>(std::string().get_allocator()) == "127.0.0.2:10098");

	yggr_test_assert(host_info.var_host_and_port<boost::container::string>() == "127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<boost::container::string>(boost::container::string().get_allocator()) == "127.0.0.2:10098");

	yggr_test_assert(host_info.var_host_and_port<yggr::string>() == "127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<yggr::string>(yggr::string().get_allocator()) == "127.0.0.2:10098");

	yggr_test_assert(host_info.var_host_and_port<yggr::utf8_string>() == "127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "127.0.0.2:10098");

#ifndef YGGR_NO_CWCHAR
	yggr_test_assert(host_info.var_host_and_port<std::wstring>() == L"127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<std::wstring>(std::wstring().get_allocator()) == L"127.0.0.2:10098");

	yggr_test_assert(host_info.var_host_and_port<boost::container::wstring>() == L"127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<boost::container::wstring>(boost::container::wstring().get_allocator()) == L"127.0.0.2:10098");

	yggr_test_assert(host_info.var_host_and_port<yggr::wstring>() == L"127.0.0.2:10098");
	yggr_test_assert(host_info.var_host_and_port<yggr::wstring>(yggr::wstring().get_allocator()) == L"127.0.0.2:10098");
#endif // YGGR_NO_CWCHAR

	// var_port
	host_info.var_port() = 10099;

	yggr_test_assert(host_info.var_host_and_port() == "127.0.0.2:10099");
	yggr_test_assert(host_info.var_port() == 10099);

	// var_family
	host_info.var_family() = AF_INET2;
	yggr_test_assert(host_info.var_family() == AF_INET2);

	// var_next
	mongo_host_info_type* phost_info = boost::addressof(host_info);
	host_info.var_next() = phost_info;

	yggr_test_assert(host_info.var_next() == phost_info);
	yggr_test_assert(host_info.var_next<mongo_host_info_type>() == phost_info);
	yggr_test_assert(host_info.var_next<base_mongo_host_info_type>() == reinterpret_cast<base_mongo_host_info_type*>(phost_info));
	yggr_test_assert(host_info.var_next<org_mongo_host_info_type>() == reinterpret_cast<org_mongo_host_info_type*>(phost_info));

	std::cout << "-----------test_member_foo-------------" << std::endl;
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