// c_mongo_write_concern_test.cpp


#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_write_concern write_concern_type;

	{
		write_concern_type write_concern;
		yggr_test_assert(write_concern.is_default());
	}

	{
		write_concern_type write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern("tag_sample");

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern.var_wtag() == "tag_sample");
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern(write_concern_sample.org_pointer());

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern(*(write_concern_sample.org_pointer()));

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern(boost::move(write_concern_sample));

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern(write_concern_sample);

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_write_concern write_concern_type;

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern;
		write_concern = (*(write_concern_sample.org_pointer()));

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern;
		write_concern = (boost::move(write_concern_sample));

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern_sample(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		write_concern_type write_concern;
		write_concern = (write_concern_sample);

		yggr_test_assert(write_concern.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_write_concern write_concern_type;

	{
		write_concern_type write_concern1(write_concern_type::E_MONGOC_WRITE_CONCERN_W_DEFAULT);
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		write_concern1.swap(*write_concern2.org_pointer());
		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_DEFAULT);
	}

	{
		write_concern_type write_concern1(write_concern_type::E_MONGOC_WRITE_CONCERN_W_DEFAULT);
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		write_concern1.swap(boost::move(write_concern2));
		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_DEFAULT);
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		write_concern1.swap(boost::move(write_concern2));

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	// std::swap
	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		std::swap(write_concern1, write_concern2);

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		std::swap(boost::move(write_concern1), write_concern2);

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		std::swap(write_concern1, boost::move(write_concern2));

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	// boost::swap
	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		boost::swap(write_concern1, write_concern2);

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		boost::swap(boost::move(write_concern1), write_concern2);

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		boost::swap(write_concern1, boost::move(write_concern2));

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}


	// yggr::swap
	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		yggr::swap(write_concern1, write_concern2);

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		yggr::swap(boost::move(write_concern1), write_concern2);

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	{
		write_concern_type write_concern1("tag_sample");
		write_concern_type write_concern2(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		yggr::swap(write_concern1, boost::move(write_concern2));

		yggr_test_assert(write_concern1.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern1.var_wtag() == "");

		yggr_test_assert(write_concern2.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_TAG);
		yggr_test_assert(write_concern2.var_wtag() == "tag_sample");
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_write_concern write_concern_type;
	typedef yggr::nsql_database_system::c_bson bson_type;

	// var_level
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		//const write_concern_type& write_concern_cref(write_concern);

		yggr_test_assert(write_concern_ref.validate());
	}

	{
		write_concern_type write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED);
		write_concern.var_fsync() = 1;
		//write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		yggr_test_assert(!write_concern_cref.validate());
	}

	// var_fsync
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		write_concern_ref.var_fsync() = 3;
		yggr_test_assert(write_concern_cref.var_fsync() == 1);
		yggr_test_assert(!write_concern_cref.is_default());
	}

	// var_journal
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		write_concern_ref.var_journal() = true;
		yggr_test_assert(write_concern_cref.var_journal());
		yggr_test_assert(!write_concern_cref.is_default());
	}

	// var_w
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		write_concern_ref.var_w() = write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY;
		yggr_test_assert(write_concern_cref.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern_cref.var_wtag() == "");
	}

	// var_wtimeout
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		write_concern_ref.var_wtimeout() = 1000;
		yggr_test_assert(write_concern_cref.var_wtimeout() == 1000);
	}

	// var_wmajority
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		write_concern_ref.var_wmajority() = 2000;
		yggr_test_assert(write_concern_cref.var_w() == write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		yggr_test_assert(write_concern_cref.var_wtimeout() == 2000);
		yggr_test_assert(write_concern_cref.var_wmajority());
	}

	// var_wtag
	{
		write_concern_type write_concern("tag_sample");
		write_concern_type& write_concern_ref(write_concern);
		const write_concern_type& write_concern_cref(write_concern);

		write_concern_ref.var_wtag() = "tag_sample2";
		yggr_test_assert(write_concern_cref.var_wtag() == "tag_sample2");

		std::cout << write_concern_ref.var_wtag() << std::endl;
		std::cout << write_concern_cref.var_wtag() << std::endl;
		std::cout << write_concern_cref.var_wtag(yggr::utf8_string().get_allocator()) << std::endl;

		std::cout << write_concern_ref.var_wtag<std::string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<std::string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<std::string>(std::string().get_allocator()) << std::endl;

		std::cout << write_concern_ref.var_wtag<boost::container::string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<boost::container::string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<boost::container::string>(boost::container::string().get_allocator()) << std::endl;

		std::cout << write_concern_ref.var_wtag<yggr::string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<yggr::string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<yggr::string>(yggr::string().get_allocator()) << std::endl;

		std::cout << write_concern_ref.var_wtag<yggr::utf8_string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<yggr::utf8_string>() << std::endl;
		std::cout << write_concern_cref.var_wtag<yggr::utf8_string>(yggr::utf8_string().get_allocator()) << std::endl;
	}

	// var_compiled
	{
		bson_type bson_sample("{\"w\" : \"tag_sample\"}");
		write_concern_type write_concern("tag_sample");

		bool bchk = write_concern.freeze();
		yggr_test_assert(bchk);
		yggr_test_assert(write_concern.var_compiled<bson_t>() == bson_sample);

		std::cout << write_concern.var_compiled() << std::endl;
		std::cout << write_concern.var_compiled<bson_t>() << std::endl;
	}

	// append_to
	{
		bson_type bs;
		write_concern_type write_concern("tag_sample");
		write_concern.append_to(bs);
		std::cout << bs << std::endl;
	}

	{
		bson_type bs;
		write_concern_type write_concern("tag_sample");
		bool bchk = write_concern.freeze();
		yggr_test_assert(bchk);
		write_concern.append_to(*(bs.org_pointer()));
		std::cout << bs << std::endl;
	}

	// freeze_compiled
	{
		bson_type bson_sample("{\"w\" : \"tag_sample\"}");
		write_concern_type write_concern("tag_sample");

		yggr_test_assert(write_concern.freeze_compiled<bson_t>() == bson_sample);

		std::cout << write_concern.freeze_compiled() << std::endl;
		std::cout << write_concern.freeze_compiled<bson_t>() << std::endl;

		yggr_test_assert(write_concern.is_frozen());
	}

	// freeze
	{
		bson_type bson_sample("{\"w\" : \"tag_sample\"}");
		write_concern_type write_concern("tag_sample");

		write_concern.freeze();

		yggr_test_assert(write_concern.is_frozen());
		yggr_test_assert(write_concern.var_compiled() == bson_sample);

		write_concern.unfreeze();

		yggr_test_assert(!write_concern.is_frozen());
		yggr_test_assert(write_concern.var_compiled().empty());
	}

	// is_default
	{
		write_concern_type write_concern;
		yggr_test_assert(write_concern.is_default());
	}

	{
		write_concern_type write_concern("tag_sample");
		yggr_test_assert(!write_concern.is_default());
	}

	// journal_is_set
	{
		write_concern_type write_concern;
		yggr_test_assert(!write_concern.journal_is_set());

		write_concern.var_journal() = true;
		yggr_test_assert(write_concern.journal_is_set());

		write_concern.var_journal() = false;
		yggr_test_assert(write_concern.journal_is_set());
	}

	// is_acknowledged
	{
		write_concern_type write_concern;
		yggr_test_assert(write_concern.is_acknowledged());

		write_concern.var_journal() = true;
		yggr_test_assert(write_concern.is_acknowledged());

		write_concern.var_w() = write_concern_type::E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED;
		write_concern.var_journal() = false;
		yggr_test_assert(!write_concern.is_acknowledged());
	}

	// output
	{
		typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

		write_concern_type write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
		std::cout << write_concern << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(write_concern) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(write_concern) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(write_concern) << std::endl;
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
