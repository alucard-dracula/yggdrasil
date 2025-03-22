// c_mongo_read_prefs_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_read_prefs.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_prefs read_prefs_type;

	{
		read_prefs_type read_prefs;
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs(read_prefs_sample.org_pointer());
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs(*(read_prefs_sample.org_pointer()));
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs(boost::move(read_prefs_sample));
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs(read_prefs_sample);
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_prefs read_prefs_type;

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs;
		read_prefs = (*(read_prefs_sample.org_pointer()));
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs;
		read_prefs = (boost::move(read_prefs_sample));
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	{
		read_prefs_type read_prefs_sample(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type read_prefs;
		read_prefs = (read_prefs_sample);
		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_prefs read_prefs_type;

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		read_prefs1.swap(boost::move(read_prefs2));
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		read_prefs1.swap(read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	// std::swap
	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		std::swap(read_prefs1, read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		std::swap(boost::move(read_prefs1), read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		std::swap(read_prefs1, boost::move(read_prefs2));
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	// boost::swap
	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		boost::swap(read_prefs1, read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		boost::swap(boost::move(read_prefs1), read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		boost::swap(read_prefs1, boost::move(read_prefs2));
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	// yggr::swap
	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		yggr::swap(read_prefs1, read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		yggr::swap(boost::move(read_prefs1), read_prefs2);
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs1(read_prefs_type::E_MONGOC_READ_PRIMARY);
		read_prefs_type read_prefs2(read_prefs_type::E_MONGOC_READ_SECONDARY);
		
		yggr::swap(read_prefs1, boost::move(read_prefs2));
		yggr_test_assert(read_prefs1.var_mode() == read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs2.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_prefs read_prefs_type;
	typedef yggr::nsql_database_system::c_bson bson_type;

	// validate
	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		yggr_test_assert(read_prefs.validate());
	}

	// var_mode
	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type& read_prefs_ref(read_prefs);
		const read_prefs_type& read_prefs_cref(read_prefs);

		read_prefs_ref.var_mode() = read_prefs_type::E_MONGOC_READ_PRIMARY;
		yggr_test_assert(read_prefs_cref.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	// var_tags
	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type& read_prefs_ref(read_prefs);
		const read_prefs_type& read_prefs_cref(read_prefs);

		bson_type bson_sample("[{\"tag1\" : 1}]");
		std::cout << bson_sample << std::endl;

		read_prefs_ref.var_tags() = bson_sample;
		yggr_test_assert(read_prefs_cref.var_tags() == bson_sample);

		std::cout << read_prefs_ref.var_tags() << std::endl;
		std::cout << read_prefs_cref.var_tags() << std::endl;

		std::cout << read_prefs_ref.var_tags<bson_t>() << std::endl;
		std::cout << read_prefs_cref.var_tags<bson_t>() << std::endl;

		std::cout << read_prefs_ref.var_tags<bson_type>() << std::endl;
		std::cout << read_prefs_cref.var_tags<bson_type>() << std::endl;
	}

	// add_tag
	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type& read_prefs_ref(read_prefs);
		const read_prefs_type& read_prefs_cref(read_prefs);

		bson_type bson_sample("[{\"tag1\" : 1}]");
		read_prefs_ref.add_tag(bson_type("{\"tag1\" : 1}"));
		yggr_test_assert(read_prefs_cref.var_tags() == bson_sample);
	}

	// var_max_staleness_seconds
	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type& read_prefs_ref(read_prefs);
		const read_prefs_type& read_prefs_cref(read_prefs);

		read_prefs_ref.var_max_staleness_seconds() = 100;
		yggr_test_assert(read_prefs_cref.var_max_staleness_seconds() == 100);
	}

	// var_hedge
	{
		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		read_prefs_type& read_prefs_ref(read_prefs);
		const read_prefs_type& read_prefs_cref(read_prefs);

		bson_type bson_sample("{\"enable\" : true}");
		bool bval = bson_sample.get_value<bool>("enable");
		yggr_test_assert(bval);
		std::cout << bson_sample << std::endl;

		read_prefs_ref.var_hedge() = bson_sample;
		yggr_test_assert(read_prefs_cref.var_hedge() == bson_sample);

		std::cout << read_prefs_ref.var_hedge() << std::endl;
		std::cout << read_prefs_cref.var_hedge() << std::endl;

		std::cout << read_prefs_ref.var_hedge<bson_t>() << std::endl;
		std::cout << read_prefs_cref.var_hedge<bson_t>() << std::endl;

		std::cout << read_prefs_ref.var_hedge<bson_type>() << std::endl;
		std::cout << read_prefs_cref.var_hedge<bson_type>() << std::endl;
	}

	// output
	{
		typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

		read_prefs_type read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);
		std::cout << read_prefs << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(read_prefs) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(read_prefs) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(read_prefs) << std::endl;
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