// c_mongo_find_and_modify_opts_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_find_and_modify_opts mongo_find_and_opts_type;
	typedef mongo_find_and_opts_type::bson_type bson_type;

	{
		mongo_find_and_opts_type opts;

		yggr_test_assert(opts.var_sort().empty());
		yggr_test_assert(opts.var_update().empty());
		yggr_test_assert(opts.var_fields().empty());

		yggr_test_assert(opts.var_flags() == 0);
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT));
		yggr_test_assert(opts.var_max_time_ms() == 0);
		yggr_test_assert(opts.var_extra().empty());
	}

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts(opts_arg.org_pointer());

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts(*(opts_arg.org_pointer()));

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts(boost::move(opts_arg));

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts(opts_arg);

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	std::cout << "-----------test_construct-------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_find_and_modify_opts mongo_find_and_opts_type;
	typedef mongo_find_and_opts_type::bson_type bson_type;

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts;
		opts = *(opts_arg.org_pointer());

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts;
		opts = (boost::move(opts_arg));

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	{
		mongo_find_and_opts_type opts_arg;

		bson_type bs_sort;
		bs_sort.save("need_sort_filed_name", 1);

		bson_type bs_update;
		bs_update.save("need_update_filed_name", "update_value");

		bson_type bs_filed;
		bs_filed.save("filed_name1", 1);
		bs_filed.save("filed_name2", 1);

		bson_type bs_extra;
		bs_extra.save("var_name", "var_value");

		opts_arg.var_sort() = bs_sort;
		opts_arg.var_update() = bs_update;
		opts_arg.var_fields() = bs_filed;

		opts_arg.var_flags() = 
			(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
				| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

		opts_arg.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_arg.var_max_time_ms() = 1000;
		opts_arg.var_extra() = bs_extra;

		mongo_find_and_opts_type opts;
		opts = (opts_arg);

		yggr_test_assert(opts.var_sort() == bs_sort);
		yggr_test_assert(opts.var_update() == bs_update);
		yggr_test_assert(opts.var_fields() == bs_filed);

		yggr_test_assert(opts.var_flags() ==
				(mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT 
					| mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW));
		yggr_test_assert((opts.var_bypass_document_validation() 
				== mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE));
		yggr_test_assert(opts.var_max_time_ms() == 1000);
		yggr_test_assert(opts.var_extra() == bs_extra);
	}

	std::cout << "-----------test_operator_set-------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_find_and_modify_opts mongo_find_and_opts_type;
	typedef mongo_find_and_opts_type::bson_type bson_type;

	bson_type bs_sort1, bs_sort2;
	bs_sort1.save("need_sort_filed_name1", 1);
	bs_sort2.save("need_sort_filed_name2", 1);

	bson_type bs_update1, bs_update2;
	bs_update1.save("need_update_filed_name1", "update_value1");
	bs_update2.save("need_update_filed_name2", "update_value2");

	bson_type bs_filed1, bs_filed2;
	bs_filed1.save("filed_name11", 1);
	bs_filed1.save("filed_name21", 1);
	bs_filed2.save("filed_name12", 1);
	bs_filed2.save("filed_name22", 1);

	bson_type bs_extra1, bs_extra2;
	bs_extra1.save("var_name1", "var_value1");
	bs_extra2.save("var_name2", "var_value2");

	// opts1
	mongo_find_and_opts_type opts1;

	opts1.var_sort() = bs_sort1;
	opts1.var_update() = bs_update1;
	opts1.var_fields() = bs_filed1;

	opts1.var_flags() = mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT;

	opts1.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
	opts1.var_max_time_ms() = 1000;
	opts1.var_extra() = bs_extra1;

	// opts2
	mongo_find_and_opts_type opts2;

	opts2.var_sort() = bs_sort2;
	opts2.var_update() = bs_update2;
	opts2.var_fields() = bs_filed2;

	opts2.var_flags() = mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW;

	opts2.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE;
	opts2.var_max_time_ms() = 2000;
	opts2.var_extra() = bs_extra2;

	mongo_find_and_opts_type opts1_chk = opts1;
	mongo_find_and_opts_type opts2_chk = opts2;

	{
		opts1.swap(boost::move(opts2));

		yggr_test_assert(opts1.var_sort() == opts2_chk.var_sort());
		yggr_test_assert(opts1.var_update() == opts2_chk.var_update());
		yggr_test_assert(opts1.var_fields() == opts2_chk.var_fields());

		yggr_test_assert(opts1.var_flags() == opts2_chk.var_flags());
		yggr_test_assert(opts1.var_bypass_document_validation() == opts2_chk.var_bypass_document_validation());
		yggr_test_assert(opts1.var_max_time_ms() == opts2_chk.var_max_time_ms());
		yggr_test_assert(opts1.var_extra() == opts2_chk.var_extra());
	}

	{
		opts1.swap(opts2);

		yggr_test_assert(opts1.var_sort() == opts1_chk.var_sort());
		yggr_test_assert(opts1.var_update() == opts1_chk.var_update());
		yggr_test_assert(opts1.var_fields() == opts1_chk.var_fields());

		yggr_test_assert(opts1.var_flags() == opts1_chk.var_flags());
		yggr_test_assert(opts1.var_bypass_document_validation() == opts1_chk.var_bypass_document_validation());
		yggr_test_assert(opts1.var_max_time_ms() == opts1_chk.var_max_time_ms());
		yggr_test_assert(opts1.var_extra() == opts1_chk.var_extra());
	}

	// std::swap
	{
		{
			std::swap(opts1, opts2);

			yggr_test_assert(opts1.var_sort() == opts2_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts2_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts2_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts2_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts2_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts2_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts2_chk.var_extra());
		}

		{
			std::swap(boost::move(opts1), opts2);

			yggr_test_assert(opts1.var_sort() == opts1_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts1_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts1_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts1_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts1_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts1_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts1_chk.var_extra());
		}

		{
			std::swap(opts1, boost::move(opts2));

			yggr_test_assert(opts1.var_sort() == opts2_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts2_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts2_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts2_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts2_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts2_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts2_chk.var_extra());
		}
	}

	// boost::swap
	{
		{
			boost::swap(opts1, opts2);

			yggr_test_assert(opts1.var_sort() == opts1_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts1_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts1_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts1_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts1_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts1_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts1_chk.var_extra());
		}

		{
			std::swap(boost::move(opts1), opts2);

			yggr_test_assert(opts1.var_sort() == opts2_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts2_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts2_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts2_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts2_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts2_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts2_chk.var_extra());
		}

		{
			std::swap(opts1, boost::move(opts2));

			yggr_test_assert(opts1.var_sort() == opts1_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts1_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts1_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts1_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts1_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts1_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts1_chk.var_extra());
		}
	}

	// yggr::swap
	{
		{
			yggr::swap(opts1, opts2);

			yggr_test_assert(opts1.var_sort() == opts2_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts2_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts2_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts2_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts2_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts2_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts2_chk.var_extra());
		}

		{
			yggr::swap(boost::move(opts1), opts2);

			yggr_test_assert(opts1.var_sort() == opts1_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts1_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts1_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts1_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts1_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts1_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts1_chk.var_extra());
		}

		{
			yggr::swap(opts1, boost::move(opts2));

			yggr_test_assert(opts1.var_sort() == opts2_chk.var_sort());
			yggr_test_assert(opts1.var_update() == opts2_chk.var_update());
			yggr_test_assert(opts1.var_fields() == opts2_chk.var_fields());

			yggr_test_assert(opts1.var_flags() == opts2_chk.var_flags());
			yggr_test_assert(opts1.var_bypass_document_validation() == opts2_chk.var_bypass_document_validation());
			yggr_test_assert(opts1.var_max_time_ms() == opts2_chk.var_max_time_ms());
			yggr_test_assert(opts1.var_extra() == opts2_chk.var_extra());
		}
	}
	

	std::cout << "-----------test_swap-------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_find_and_modify_opts mongo_find_and_opts_type;
	typedef mongo_find_and_opts_type::bson_type bson_type;

	bson_type bs_sort1, bs_sort2;
	bs_sort1.save("need_sort_filed_name1", 1);
	bs_sort2.save("need_sort_filed_name2", 1);

	bson_type bs_update1, bs_update2;
	bs_update1.save("need_update_filed_name1", "update_value1");
	bs_update2.save("need_update_filed_name2", "update_value2");

	bson_type bs_filed1, bs_filed2;
	bs_filed1.save("filed_name11", 1);
	bs_filed1.save("filed_name21", 1);
	bs_filed2.save("filed_name12", 1);
	bs_filed2.save("filed_name22", 1);

	bson_type bs_extra1, bs_extra2;
	bs_extra1.save("var_name1", "var_value1");
	bs_extra2.save("var_name2", "var_value2");

	{
		mongo_find_and_opts_type opts;
		mongo_find_and_opts_type& opts_ref = opts;
		const mongo_find_and_opts_type& opts_cref = opts;

		opts_ref.var_sort() = bs_sort1;
		opts_ref.var_update() = bs_update1;
		opts_ref.var_fields() = bs_filed1;

		opts_ref.var_flags() = mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT;

		opts_ref.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_ref.var_max_time_ms() = 1000;
		opts_ref.var_extra() = bs_extra1;

		yggr_test_assert(opts_cref.var_sort() == bs_sort1);
		yggr_test_assert(opts_cref.var_update() == bs_update1);
		yggr_test_assert(opts_cref.var_fields() == bs_filed1);

		yggr_test_assert(opts_cref.var_flags() == mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT);
		yggr_test_assert(opts_cref.var_bypass_document_validation() ==  mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE);
		yggr_test_assert(opts_cref.var_max_time_ms() == 1000);
		yggr_test_assert(opts_cref.var_extra() == bs_extra1);
	}

	{
		mongo_find_and_opts_type opts;
		mongo_find_and_opts_type& opts_ref = opts;
		const mongo_find_and_opts_type& opts_cref = opts;

		opts_ref.var_sort<bson_type>() = bs_sort1;
		opts_ref.var_update<bson_type>() = bs_update1;
		opts_ref.var_fields<bson_type>() = bs_filed1;

		opts_ref.var_flags() = mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT;

		opts_ref.var_bypass_document_validation() = mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE;
		opts_ref.var_max_time_ms() = 1000;
		opts_ref.var_extra<bson_type>() = bs_extra1;

		yggr_test_assert(opts_cref.var_sort<bson_t>() == bs_sort1);
		yggr_test_assert(opts_cref.var_update<bson_t>() == bs_update1);
		yggr_test_assert(opts_cref.var_fields<bson_t>() == bs_filed1);

		yggr_test_assert(opts_cref.var_flags() == mongo_find_and_opts_type::E_MONGOC_FIND_AND_MODIFY_UPSERT);
		yggr_test_assert(opts_cref.var_bypass_document_validation() ==  mongo_find_and_opts_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE);
		yggr_test_assert(opts_cref.var_max_time_ms() == 1000);
		yggr_test_assert(opts_cref.var_extra<bson_t>() == bs_extra1);
	}

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