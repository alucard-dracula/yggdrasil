// c_mongo_client_encryption_encrypt_range_opts_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.hpp>

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
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_range_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	{
		opts_type opts;
		yggr_test_assert(opts.var_min_value().value_typeid() == 0);
		yggr_test_assert(!opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == 0);
		yggr_test_assert(!opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 0);

		yggr_test_assert(opts.var_precision_value() == 0);
		yggr_test_assert(!opts.var_precision_set());
	}

	{
		opts_type opts(1, 2, 3, 4);

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_value() == 1);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_value() == 2);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 3);

		yggr_test_assert(opts.var_precision_value() == 0);
		yggr_test_assert(!opts.var_precision_set());
	}

	{
		opts_type opts(1.0, 2.0, 3.0, 4.0);

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_DOUBLE);
		yggr_test_assert(opts.var_min_value() == 1.0);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_DOUBLE);
		yggr_test_assert(opts.var_max_value() == 2.0);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 3.0);

		yggr_test_assert(opts.var_precision_value() == 4.0);
		yggr_test_assert(opts.var_precision_set());
	}

	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts(opts_sample.org_pointer());

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}

	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts(*opts_sample.org_pointer());

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}
	
	
	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts(boost::move(opts_sample));

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}

	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts(opts_sample);

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}
	
	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_range_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts;
		opts = (*opts_sample.org_pointer());

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}

	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts;
		opts = (*reinterpret_cast<base_opts_type*>(opts_sample.org_pointer()));

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}
	
	
	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts;
		opts = (boost::move(opts_sample));

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}

	{
		bson_value_type val_min = 1;
		bson_value_type val_max = 10;

		opts_type opts_sample;
		opts_sample.var_min() = val_min;
		opts_sample.var_max() = val_max;
		opts_sample.var_sparsity() = 2;
		opts_sample.var_precision() = 3;

		opts_type opts;
		opts = (opts_sample);

		yggr_test_assert(opts.var_min_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_min_set());

		yggr_test_assert(opts.var_max_value().value_typeid() == BSON_TYPE_INT32);
		yggr_test_assert(opts.var_max_set());

		yggr_test_assert(opts.var_sparsity() == 2);

		yggr_test_assert(opts.var_precision_value() == 3);
		yggr_test_assert(opts.var_precision_set());
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_range_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	opts_type opts_sample_1;
	opts_sample_1.var_min() = 11;
	opts_sample_1.var_max() = 111;
	opts_sample_1.var_sparsity() = 1111;
	opts_sample_1.var_precision() = 11111;

	opts_type opts_sample_2;
	opts_sample_2.var_min() = 22;
	opts_sample_2.var_max() = 222;
	opts_sample_2.var_sparsity() = 2222;
	opts_sample_2.var_precision() = 22222;

	opts_sample_2.var_min_set() = false;
	opts_sample_2.var_max_set() = false;
	opts_sample_2.var_precision_set() = false;

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		opts1.swap(boost::move(opts2));

		yggr_test_assert(opts1.var_min_value() == opts_sample_2.var_min_value());
		yggr_test_assert(opts1.var_min_set() == opts_sample_2.var_min_set());

		yggr_test_assert(opts1.var_max_value() == opts_sample_2.var_max_value());
		yggr_test_assert(opts1.var_max_set() == opts_sample_2.var_max_set());

		yggr_test_assert(opts1.var_sparsity() == opts_sample_2.var_sparsity());

		yggr_test_assert(opts1.var_precision_value() == opts_sample_2.var_precision_value());
		yggr_test_assert(opts1.var_precision_set() == opts_sample_2.var_precision_set());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		opts1.swap(opts2);

		yggr_test_assert(opts1.var_min_value() == opts_sample_2.var_min_value());
		yggr_test_assert(opts1.var_min_set() == opts_sample_2.var_min_set());

		yggr_test_assert(opts1.var_max_value() == opts_sample_2.var_max_value());
		yggr_test_assert(opts1.var_max_set() == opts_sample_2.var_max_set());

		yggr_test_assert(opts1.var_sparsity() == opts_sample_2.var_sparsity());

		yggr_test_assert(opts1.var_precision_value() == opts_sample_2.var_precision_value());
		yggr_test_assert(opts1.var_precision_set() == opts_sample_2.var_precision_set());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		std::swap(boost::move(opts1), opts2);
		std::swap(opts1, boost::move(opts2));
		std::swap(opts1, opts2);

		yggr_test_assert(opts1.var_min_value() == opts_sample_2.var_min_value());
		yggr_test_assert(opts1.var_min_set() == opts_sample_2.var_min_set());

		yggr_test_assert(opts1.var_max_value() == opts_sample_2.var_max_value());
		yggr_test_assert(opts1.var_max_set() == opts_sample_2.var_max_set());

		yggr_test_assert(opts1.var_sparsity() == opts_sample_2.var_sparsity());

		yggr_test_assert(opts1.var_precision_value() == opts_sample_2.var_precision_value());
		yggr_test_assert(opts1.var_precision_set() == opts_sample_2.var_precision_set());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		boost::swap(boost::move(opts1), opts2);
		boost::swap(opts1, boost::move(opts2));
		boost::swap(opts1, opts2);

		yggr_test_assert(opts1.var_min_value() == opts_sample_2.var_min_value());
		yggr_test_assert(opts1.var_min_set() == opts_sample_2.var_min_set());

		yggr_test_assert(opts1.var_max_value() == opts_sample_2.var_max_value());
		yggr_test_assert(opts1.var_max_set() == opts_sample_2.var_max_set());

		yggr_test_assert(opts1.var_sparsity() == opts_sample_2.var_sparsity());

		yggr_test_assert(opts1.var_precision_value() == opts_sample_2.var_precision_value());
		yggr_test_assert(opts1.var_precision_set() == opts_sample_2.var_precision_set());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		yggr::swap(boost::move(opts1), opts2);
		yggr::swap(opts1, boost::move(opts2));
		yggr::swap(opts1, opts2);

		yggr_test_assert(opts1.var_min_value() == opts_sample_2.var_min_value());
		yggr_test_assert(opts1.var_min_set() == opts_sample_2.var_min_set());

		yggr_test_assert(opts1.var_max_value() == opts_sample_2.var_max_value());
		yggr_test_assert(opts1.var_max_set() == opts_sample_2.var_max_set());

		yggr_test_assert(opts1.var_sparsity() == opts_sample_2.var_sparsity());

		yggr_test_assert(opts1.var_precision_value() == opts_sample_2.var_precision_value());
		yggr_test_assert(opts1.var_precision_set() == opts_sample_2.var_precision_set());
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_range_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	// min
	{
		opts_type opts;

		opts.var_min() = 1;

		yggr_test_assert(opts.var_min_value() == 1);
		yggr_test_assert(opts.var_min_set());
	}

	// max
	{
		opts_type opts;

		opts.var_max() = 2;

		yggr_test_assert(opts.var_max_value() == 2);
		yggr_test_assert(opts.var_max_set());
	}

	// sparsity
	{
		opts_type opts;

		opts.var_sparsity() = 3;

		yggr_test_assert(opts.var_sparsity() == 3);
	}

	// precision
	{
		opts_type opts;

		opts.var_precision() = 4;

		yggr_test_assert(opts.var_precision_value() == 4);
		yggr_test_assert(opts.var_precision_set());
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