// c_mongo_client_encryption_encrypt_opts_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/ids/uuid.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer.hpp>

#include <yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

// test
void test_construct(void)
{
	typedef yggr::nsql_database_system::bson_binary_typeid_def bson_binary_typeid_def_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<bson_binary_typeid_def_type::E_BSON_BIN_TYPE_UUID> key_id_type;
	typedef yggr::ids::uuid uuid_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;
	typedef opts_type::range_opts_type range_opts_type;

	{
		opts_type opts;
		const opts_type& opts_cref = opts;
		yggr_test_assert(opts.var_keyid().value_typeid() == BSON_TYPE_EOD);

		yggr_test_assert(!((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm));
		yggr_test_assert(opts_cref.var_algorithm() == "");

		yggr_test_assert(!((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname));
		yggr_test_assert(opts_cref.var_keyaltname() == "");

		yggr_test_assert(opts.var_contention_factor_value() == 0);
		yggr_test_assert(opts.var_contention_factor_set() == false);

		yggr_test_assert(!((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type));
		yggr_test_assert(opts_cref.var_query_mode() == "");

		yggr_test_assert(!((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 0);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		opts_type opts(key_id_val,
						opts_type::s_encrypt_algorithm_indexed(),
						"sample_keyvalut_name",
						1,
						opts_type::s_encrypt_query_mode_equality());


		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_indexed());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_equality());

		yggr_test_assert(!((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 0);
	}


	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts(key_id_val,
						opts_type::s_encrypt_algorithm_range_preview(),
						"sample_keyvalut_name",
						1,
						opts_type::s_encrypt_query_mode_range_preview(),
						range_opts);


		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts(opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts(*opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts(boost::move(opts_sample));
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts(opts_sample);
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::bson_binary_typeid_def bson_binary_typeid_def_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<bson_binary_typeid_def_type::E_BSON_BIN_TYPE_UUID> key_id_type;
	typedef yggr::ids::uuid uuid_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;
	typedef opts_type::range_opts_type range_opts_type;

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts;
		opts = (*opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts;
		opts = (boost::move(opts_sample));
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	{
		yggr::string id = "1000000000000000";

		uuid_type uuid_keyid(id);
		key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
		bson_value_type key_id_val = key_id;

		range_opts_type range_opts(1, 2, 3, 4);

		opts_type opts_sample(key_id_val,
								opts_type::s_encrypt_algorithm_range_preview(),
								"sample_keyvalut_name",
								1,
								opts_type::s_encrypt_query_mode_range_preview(),
								range_opts);

		//const opts_type& opts_sample_cref = opts_sample;

		opts_type opts;
		opts = (opts_sample);
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts.var_keyid() == key_id_val);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->algorithm) == opts_cref.var_algorithm().data());
		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->keyaltname) == opts_cref.var_keyaltname().data());
		yggr_test_assert(opts_cref.var_keyaltname() == "sample_keyvalut_name");

		yggr_test_assert(opts.var_contention_factor_value() == 1);
		yggr_test_assert(opts.var_contention_factor_set() == true);

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->query_type) == opts_cref.var_query_mode().data());
		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());

		yggr_test_assert(((reinterpret_cast<base_opts_type*>(opts.org_pointer()))->range_opts));
		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::bson_binary_typeid_def bson_binary_typeid_def_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<bson_binary_typeid_def_type::E_BSON_BIN_TYPE_UUID> key_id_type;
	typedef yggr::ids::uuid uuid_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;
	typedef opts_type::range_opts_type range_opts_type;

	yggr::string id_1 = "1000000000000000";
	uuid_type uuid_keyid_1(id_1);
	key_id_type key_id_1(uuid_keyid_1.begin(), uuid_keyid_1.end());
	bson_value_type key_id_val_1 = key_id_1;
	range_opts_type range_opts_1(1, 2, 3, 4);

	yggr::string id_2 = "2000000000000000";
	uuid_type uuid_keyid_2(id_2);
	key_id_type key_id_2(uuid_keyid_2.begin(), uuid_keyid_2.end());
	bson_value_type key_id_val_2 = key_id_2;
	range_opts_type range_opts_2(4, 5, 6, 7);

	opts_type opts_sample_1(key_id_val_1,
							opts_type::s_encrypt_algorithm_range_preview(),
							"sample_keyvalut_name_1",
							1,
							opts_type::s_encrypt_query_mode_range_preview(),
							range_opts_1);

	opts_type opts_sample_2(key_id_val_2,
							opts_type::s_encrypt_algorithm_range_preview(),
							"sample_keyvalut_name_2",
							2,
							opts_type::s_encrypt_query_mode_range_preview(),
							range_opts_2);

	opts_sample_2.var_contention_factor_set() = false;

	const opts_type& opts_sample_cref1 = opts_sample_1;
	const opts_type& opts_sample_cref2 = opts_sample_2;

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		const opts_type& opts_cref1 = opts1;
		const opts_type& opts_cref2 = opts2;

		opts1.swap(boost::move(opts2));

		yggr_test_assert(opts_cref1.var_keyid() == opts_sample_cref2.var_keyid());
		yggr_test_assert(opts_cref2.var_keyid() == opts_sample_cref1.var_keyid());

		yggr_test_assert(opts_cref1.var_algorithm() == opts_sample_cref2.var_algorithm());
		yggr_test_assert(opts_cref2.var_algorithm() == opts_sample_cref1.var_algorithm());

		yggr_test_assert(opts_cref1.var_keyaltname() == opts_sample_cref2.var_keyaltname());
		yggr_test_assert(opts_cref2.var_keyaltname() == opts_sample_cref1.var_keyaltname());

		yggr_test_assert(opts_cref1.var_contention_factor_value() == opts_sample_cref2.var_contention_factor_value());
		yggr_test_assert(opts_cref2.var_contention_factor_value() == opts_sample_cref1.var_contention_factor_value());

		yggr_test_assert(opts_cref1.var_contention_factor_set() == opts_sample_cref2.var_contention_factor_set());
		yggr_test_assert(opts_cref2.var_contention_factor_set() == opts_sample_cref1.var_contention_factor_set());

		yggr_test_assert(opts_cref1.var_query_mode() == opts_sample_cref2.var_query_mode());
		yggr_test_assert(opts_cref2.var_query_mode() == opts_sample_cref1.var_query_mode());

		yggr_test_assert(opts_cref1.var_range_opts().var_sparsity() == opts_sample_cref2.var_range_opts().var_sparsity());
		yggr_test_assert(opts_cref2.var_range_opts().var_sparsity() == opts_sample_cref1.var_range_opts().var_sparsity());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		const opts_type& opts_cref1 = opts1;
		const opts_type& opts_cref2 = opts2;

		opts1.swap(opts2);

		yggr_test_assert(opts_cref1.var_keyid() == opts_sample_cref2.var_keyid());
		yggr_test_assert(opts_cref2.var_keyid() == opts_sample_cref1.var_keyid());

		yggr_test_assert(opts_cref1.var_algorithm() == opts_sample_cref2.var_algorithm());
		yggr_test_assert(opts_cref2.var_algorithm() == opts_sample_cref1.var_algorithm());

		yggr_test_assert(opts_cref1.var_keyaltname() == opts_sample_cref2.var_keyaltname());
		yggr_test_assert(opts_cref2.var_keyaltname() == opts_sample_cref1.var_keyaltname());

		yggr_test_assert(opts_cref1.var_contention_factor_value() == opts_sample_cref2.var_contention_factor_value());
		yggr_test_assert(opts_cref2.var_contention_factor_value() == opts_sample_cref1.var_contention_factor_value());

		yggr_test_assert(opts_cref1.var_contention_factor_set() == opts_sample_cref2.var_contention_factor_set());
		yggr_test_assert(opts_cref2.var_contention_factor_set() == opts_sample_cref1.var_contention_factor_set());

		yggr_test_assert(opts_cref1.var_query_mode() == opts_sample_cref2.var_query_mode());
		yggr_test_assert(opts_cref2.var_query_mode() == opts_sample_cref1.var_query_mode());

		yggr_test_assert(opts_cref1.var_range_opts().var_sparsity() == opts_sample_cref2.var_range_opts().var_sparsity());
		yggr_test_assert(opts_cref2.var_range_opts().var_sparsity() == opts_sample_cref1.var_range_opts().var_sparsity());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		const opts_type& opts_cref1 = opts1;
		const opts_type& opts_cref2 = opts2;

		std::swap(boost::move(opts1), opts2);
		std::swap(opts1, boost::move(opts2));
		std::swap(opts1, opts2);

		yggr_test_assert(opts_cref1.var_keyid() == opts_sample_cref2.var_keyid());
		yggr_test_assert(opts_cref2.var_keyid() == opts_sample_cref1.var_keyid());

		yggr_test_assert(opts_cref1.var_algorithm() == opts_sample_cref2.var_algorithm());
		yggr_test_assert(opts_cref2.var_algorithm() == opts_sample_cref1.var_algorithm());

		yggr_test_assert(opts_cref1.var_keyaltname() == opts_sample_cref2.var_keyaltname());
		yggr_test_assert(opts_cref2.var_keyaltname() == opts_sample_cref1.var_keyaltname());

		yggr_test_assert(opts_cref1.var_contention_factor_value() == opts_sample_cref2.var_contention_factor_value());
		yggr_test_assert(opts_cref2.var_contention_factor_value() == opts_sample_cref1.var_contention_factor_value());

		yggr_test_assert(opts_cref1.var_contention_factor_set() == opts_sample_cref2.var_contention_factor_set());
		yggr_test_assert(opts_cref2.var_contention_factor_set() == opts_sample_cref1.var_contention_factor_set());

		yggr_test_assert(opts_cref1.var_query_mode() == opts_sample_cref2.var_query_mode());
		yggr_test_assert(opts_cref2.var_query_mode() == opts_sample_cref1.var_query_mode());

		yggr_test_assert(opts_cref1.var_range_opts().var_sparsity() == opts_sample_cref2.var_range_opts().var_sparsity());
		yggr_test_assert(opts_cref2.var_range_opts().var_sparsity() == opts_sample_cref1.var_range_opts().var_sparsity());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		const opts_type& opts_cref1 = opts1;
		const opts_type& opts_cref2 = opts2;

		boost::swap(boost::move(opts1), opts2);
		boost::swap(opts1, boost::move(opts2));
		boost::swap(opts1, opts2);

		yggr_test_assert(opts_cref1.var_keyid() == opts_sample_cref2.var_keyid());
		yggr_test_assert(opts_cref2.var_keyid() == opts_sample_cref1.var_keyid());

		yggr_test_assert(opts_cref1.var_algorithm() == opts_sample_cref2.var_algorithm());
		yggr_test_assert(opts_cref2.var_algorithm() == opts_sample_cref1.var_algorithm());

		yggr_test_assert(opts_cref1.var_keyaltname() == opts_sample_cref2.var_keyaltname());
		yggr_test_assert(opts_cref2.var_keyaltname() == opts_sample_cref1.var_keyaltname());

		yggr_test_assert(opts_cref1.var_contention_factor_value() == opts_sample_cref2.var_contention_factor_value());
		yggr_test_assert(opts_cref2.var_contention_factor_value() == opts_sample_cref1.var_contention_factor_value());

		yggr_test_assert(opts_cref1.var_contention_factor_set() == opts_sample_cref2.var_contention_factor_set());
		yggr_test_assert(opts_cref2.var_contention_factor_set() == opts_sample_cref1.var_contention_factor_set());

		yggr_test_assert(opts_cref1.var_query_mode() == opts_sample_cref2.var_query_mode());
		yggr_test_assert(opts_cref2.var_query_mode() == opts_sample_cref1.var_query_mode());

		yggr_test_assert(opts_cref1.var_range_opts().var_sparsity() == opts_sample_cref2.var_range_opts().var_sparsity());
		yggr_test_assert(opts_cref2.var_range_opts().var_sparsity() == opts_sample_cref1.var_range_opts().var_sparsity());
	}

	{
		opts_type opts1(opts_sample_1);
		opts_type opts2(opts_sample_2);

		const opts_type& opts_cref1 = opts1;
		const opts_type& opts_cref2 = opts2;

		yggr::swap(boost::move(opts1), opts2);
		yggr::swap(opts1, boost::move(opts2));
		yggr::swap(opts1, opts2);

		yggr_test_assert(opts_cref1.var_keyid() == opts_sample_cref2.var_keyid());
		yggr_test_assert(opts_cref2.var_keyid() == opts_sample_cref1.var_keyid());

		yggr_test_assert(opts_cref1.var_algorithm() == opts_sample_cref2.var_algorithm());
		yggr_test_assert(opts_cref2.var_algorithm() == opts_sample_cref1.var_algorithm());

		yggr_test_assert(opts_cref1.var_keyaltname() == opts_sample_cref2.var_keyaltname());
		yggr_test_assert(opts_cref2.var_keyaltname() == opts_sample_cref1.var_keyaltname());

		yggr_test_assert(opts_cref1.var_contention_factor_value() == opts_sample_cref2.var_contention_factor_value());
		yggr_test_assert(opts_cref2.var_contention_factor_value() == opts_sample_cref1.var_contention_factor_value());

		yggr_test_assert(opts_cref1.var_contention_factor_set() == opts_sample_cref2.var_contention_factor_set());
		yggr_test_assert(opts_cref2.var_contention_factor_set() == opts_sample_cref1.var_contention_factor_set());

		yggr_test_assert(opts_cref1.var_query_mode() == opts_sample_cref2.var_query_mode());
		yggr_test_assert(opts_cref2.var_query_mode() == opts_sample_cref1.var_query_mode());

		yggr_test_assert(opts_cref1.var_range_opts().var_sparsity() == opts_sample_cref2.var_range_opts().var_sparsity());
		yggr_test_assert(opts_cref2.var_range_opts().var_sparsity() == opts_sample_cref1.var_range_opts().var_sparsity());
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_binary_typeid_def bson_binary_typeid_def_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer<bson_binary_typeid_def_type::E_BSON_BIN_TYPE_UUID> key_id_type;
	typedef yggr::ids::uuid uuid_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;
	typedef opts_type::range_opts_type range_opts_type;

	yggr::string id = "1000000000000000";
	uuid_type uuid_keyid(id);
	key_id_type key_id(uuid_keyid.begin(), uuid_keyid.end());
	bson_value_type key_id_val = key_id;
	range_opts_type range_opts(1, 2, 3, 4);

	// var_keyid
	{
		opts_type opts;

		opts.var_keyid() = key_id_val;

		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_cref.var_keyid() == key_id_val);
	}

	// var_algorithm
	{
		opts_type opts;

		opts.var_algorithm() = opts_type::s_encrypt_algorithm_range_preview();

		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_cref.var_algorithm() == opts_type::s_encrypt_algorithm_range_preview());
		std::cout << opts_cref.var_algorithm<yggr::string>() << std::endl;
		std::cout << opts_cref.var_algorithm<yggr::string>(yggr::string().get_allocator()) << std::endl;
	}

	// var_keyaltname
	{
		opts_type opts;

		opts.var_keyaltname() = opts_type::s_encrypt_query_mode_range_preview();

		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_cref.var_keyaltname() == opts_type::s_encrypt_query_mode_range_preview());
		std::cout << opts_cref.var_keyaltname<yggr::string>() << std::endl;
		std::cout << opts_cref.var_keyaltname<yggr::string>(yggr::string().get_allocator()) << std::endl;
	}

	// var_contention_factor
	{
		opts_type opts;

		opts.var_contention_factor() = 1;

		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_cref.var_contention_factor_value() == 1);
		yggr_test_assert(opts_cref.var_contention_factor_set() == true);

		opts.var_contention_factor_set() = false;
		yggr_test_assert(!opts.contention_factor_is_seted());
	}

	// var_query_mode
	{
		opts_type opts;

		opts.var_query_mode() = opts_type::s_encrypt_query_mode_range_preview();

		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_cref.var_query_mode() == opts_type::s_encrypt_query_mode_range_preview());
		std::cout << opts_cref.var_query_mode<yggr::string>() << std::endl;
		std::cout << opts_cref.var_query_mode<yggr::string>(yggr::string().get_allocator()) << std::endl;
	}

	// var_range_opts
	{
		opts_type opts;

		opts.var_range_opts() = range_opts;

		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 0);

		opts.var_algorithm() = opts_type::s_encrypt_algorithm_range_preview();
		opts.var_range_opts() = range_opts;

		yggr_test_assert(opts_cref.var_range_opts().var_sparsity() == 3);

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
