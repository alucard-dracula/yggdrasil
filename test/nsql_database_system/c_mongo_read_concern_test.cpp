// c_mongo_read_concern_test.cpp


#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_read_concern.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_concern read_concern_type;

	{
		read_concern_type read_concern;
		yggr_test_assert(read_concern.is_default());
	}

	{
		read_concern_type read_concern(read_concern_type::s_level_majority());
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern(read_concern_sample.org_pointer());
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern(boost::move(*read_concern_sample.org_pointer()));
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern(*read_concern_sample.org_pointer());
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern(boost::move(read_concern_sample));
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}
	
	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern(read_concern_sample);
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_concern read_concern_type;

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern;
		read_concern = (boost::move(*read_concern_sample.org_pointer()));
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern;
		read_concern = (*read_concern_sample.org_pointer());
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern;
		read_concern = (boost::move(read_concern_sample));
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}
	
	{
		read_concern_type read_concern_sample(read_concern_type::s_level_majority());
		read_concern_type read_concern;
		read_concern = (read_concern_sample);
		yggr_test_assert(!read_concern.is_default());
		yggr_test_assert(read_concern.var_level() == read_concern_type::s_level_majority());
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_concern read_concern_type;

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		read_concern1.swap(boost::move(read_concern2));
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		read_concern1.swap(read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	// std::swap
	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		std::swap(read_concern1, read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		std::swap(boost::move(read_concern1), read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		std::swap(read_concern1, boost::move(read_concern2));
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	// boost::swap
	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		boost::swap(read_concern1, read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		boost::swap(boost::move(read_concern1), read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		boost::swap(read_concern1, boost::move(read_concern2));
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	// yggr::swap
	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		yggr::swap(read_concern1, read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		yggr::swap(boost::move(read_concern1), read_concern2);
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	{
		read_concern_type read_concern1(read_concern_type::s_level_majority());
		read_concern_type read_concern2(read_concern_type::s_level_available());
		
		yggr::swap(read_concern1, boost::move(read_concern2));
		yggr_test_assert(read_concern1.var_level() == read_concern_type::s_level_available());
		yggr_test_assert(read_concern2.var_level() == read_concern_type::s_level_majority());
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_read_concern read_concern_type;
	typedef yggr::nsql_database_system::c_bson bson_type;

	// var_level
	{
		read_concern_type read_concern(read_concern_type::s_level_majority());
		read_concern_type& read_concern_ref(read_concern);
		const read_concern_type& read_concern_cref(read_concern);

		read_concern_ref.var_level() = read_concern_type::s_level_available();
		yggr_test_assert(read_concern_cref.var_level() == read_concern_type::s_level_available());
		
		std::cout << read_concern_ref.var_level() << std::endl;
		std::cout << read_concern_ref.var_level<std::string>() << std::endl;
		std::cout << read_concern_cref.var_level<boost::container::string>() << std::endl;
		std::cout << read_concern_ref.var_level<yggr::string>(yggr::charset::charset_name_t<yggr::string>()) << std::endl;
		std::cout << read_concern_cref.var_level<yggr::utf8_string>() << std::endl;
		std::cout << read_concern_ref.var_level<yggr::utf8_string>(yggr::utf8_string().get_allocator()) << std::endl;
	}

	// var_compiled
	{
		read_concern_type read_concern(read_concern_type::s_level_majority());
		
		std::cout << read_concern.var_compiled() << std::endl;

		read_concern.freeze();

		std::cout << read_concern.var_compiled<bson_t>() << std::endl;
		std::cout << read_concern.var_compiled<bson_type>() << std::endl;
	}

	// append_to
	{
		bson_type cmd;
		read_concern_type read_concern(read_concern_type::s_level_majority());
		read_concern.append_to(cmd);

		std::cout << cmd << std::endl;
	}

	{
		bson_type cmd;
		read_concern_type read_concern(read_concern_type::s_level_majority());
		read_concern.freeze();
		read_concern.append_to(*(cmd.org_pointer()));

		std::cout << cmd << std::endl;
	}

	// frozen
	{
		read_concern_type read_concern(read_concern_type::s_level_majority());

		yggr_test_assert(!read_concern.is_frozen());
		yggr_test_assert(read_concern.var_compiled().empty());

		bool bchk = false;
		bchk = read_concern.freeze();
		yggr_test_assert(bchk);

		yggr_test_assert(read_concern.is_frozen());
		yggr_test_assert(!read_concern.var_compiled().empty());

		std::cout << read_concern.var_compiled() << std::endl;

		bchk = read_concern.unfreeze();
		yggr_test_assert(!read_concern.is_frozen());
		yggr_test_assert(read_concern.var_compiled().empty());
	}

	// is_default
	{
		read_concern_type read_concern;
		yggr_test_assert(read_concern.is_default());
	}

	{
		read_concern_type read_concern(read_concern_type::s_level_majority());
		yggr_test_assert(!read_concern.is_default());
	}

	// output
	{
		typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

		read_concern_type read_concern(read_concern_type::s_level_majority());
		std::cout << read_concern << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(read_concern) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(read_concern) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(read_concern) << std::endl;
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