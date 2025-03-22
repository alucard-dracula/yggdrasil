// c_mongo_client_encryption_datakey_opts_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.hpp>

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
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_datakey_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count);
		yggr_test_assert(opts_cref.var_keyaltnames().empty());
		yggr_test_assert(!opts_cref.var_keyaltnames_count());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len);
		yggr_test_assert(opts_cref.var_keymaterial().empty());
	}

	{
		const char* keyaltnames[] = {"mongoc_encryption_example_1", "mongoc_encryption_example_2"};

		opts_type opts(keyaltnames);
		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len);
		yggr_test_assert(opts_cref.var_keymaterial().empty());
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		const char* keyaltnames[] = {"mongoc_encryption_example_1", "mongoc_encryption_example_2"};

		opts_type opts(keyaltnames, keyaltnames + 2);
		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len);
		yggr_test_assert(opts_cref.var_keymaterial().empty());
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts(bs_master_key, keyaltnames, key_material);
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts(bs_master_key, keyaltnames, keyaltnames + 2, key_material);
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts(bs_master_key, keyaltnames, key_material.begin(), key_material.end());
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts(bs_master_key, keyaltnames, keyaltnames + 2, key_material.begin(), key_material.end());
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts(opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts(*opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts(boost::move(opts_sample));
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts(opts_sample);
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}
	
	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_datakey_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts;
		opts = (*opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts;
		opts = reinterpret_cast<base_opts_type&>(*opts_sample.org_pointer());
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts;
		opts = boost::move(opts_sample);
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
		const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
		yggr::string key_material = "abcdefgh";

		opts_type opts_sample(bs_master_key, keyaltnames, key_material);

		opts_type opts;
		opts = opts_sample;
		const opts_type& opts_cref = opts;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_datakey_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	bson_type bs_master_key_1("{ \"endpoint\" : \"127.0.0.1:5200\" }");
	bson_type bs_master_key_2("{ \"endpoint\" : \"127.0.0.1:5300\" }");

	const char* keyaltnames_1[] = { "mongoc_encryption_example_1_1", "mongoc_encryption_example_1_2" };
	const char* keyaltnames_2[] = { "mongoc_encryption_example_2_1" };

	yggr::string key_material_1 = "abcdefgh1";
	yggr::string key_material_2 = "abcdefgh2";

	opts_type opts_sample_1(bs_master_key_1, keyaltnames_1, key_material_1);
	opts_type opts_sample_2(bs_master_key_2, keyaltnames_2, key_material_2);

	const opts_type& opts_sample_cref_1 = opts_sample_1;
	const opts_type& opts_sample_cref_2 = opts_sample_2;

	{
		opts_type opts_1(opts_sample_1);
		opts_type opts_2(opts_sample_2);

		const opts_type& opts_cref1 = opts_1;
		const opts_type& opts_cref2 = opts_2;

		opts_1.swap(boost::move(opts_2));

		yggr_test_assert(opts_cref1.var_masterkey() == opts_sample_cref_2.var_masterkey());
		yggr_test_assert(opts_cref2.var_masterkey() == opts_sample_cref_1.var_masterkey());

		yggr_test_assert(opts_cref1.var_keyaltnames().size() == opts_sample_cref_2.var_keyaltnames().size());
		yggr_test_assert(opts_cref2.var_keyaltnames().size() == opts_sample_cref_1.var_keyaltnames().size());

		yggr_test_assert(opts_cref1.var_keyaltnames_count() == opts_sample_cref_2.var_keyaltnames_count());
		yggr_test_assert(opts_cref2.var_keyaltnames_count() == opts_sample_cref_1.var_keyaltnames_count());

		yggr_test_assert(opts_cref1.var_keymaterial() == opts_sample_cref_2.var_keymaterial());
		yggr_test_assert(opts_cref2.var_keymaterial() == opts_sample_cref_1.var_keymaterial());

		yggr_test_assert(opts_cref1.var_keymaterial_size() == opts_sample_cref_2.var_keymaterial_size());
		yggr_test_assert(opts_cref2.var_keymaterial_size() == opts_sample_cref_1.var_keymaterial_size());
	}

	{
		opts_type opts_1(opts_sample_1);
		opts_type opts_2(opts_sample_2);

		const opts_type& opts_cref1 = opts_1;
		const opts_type& opts_cref2 = opts_2;

		opts_1.swap(opts_2);

		yggr_test_assert(opts_cref1.var_masterkey() == opts_sample_cref_2.var_masterkey());
		yggr_test_assert(opts_cref2.var_masterkey() == opts_sample_cref_1.var_masterkey());

		yggr_test_assert(opts_cref1.var_keyaltnames().size() == opts_sample_cref_2.var_keyaltnames().size());
		yggr_test_assert(opts_cref2.var_keyaltnames().size() == opts_sample_cref_1.var_keyaltnames().size());

		yggr_test_assert(opts_cref1.var_keyaltnames_count() == opts_sample_cref_2.var_keyaltnames_count());
		yggr_test_assert(opts_cref2.var_keyaltnames_count() == opts_sample_cref_1.var_keyaltnames_count());

		yggr_test_assert(opts_cref1.var_keymaterial() == opts_sample_cref_2.var_keymaterial());
		yggr_test_assert(opts_cref2.var_keymaterial() == opts_sample_cref_1.var_keymaterial());

		yggr_test_assert(opts_cref1.var_keymaterial_size() == opts_sample_cref_2.var_keymaterial_size());
		yggr_test_assert(opts_cref2.var_keymaterial_size() == opts_sample_cref_1.var_keymaterial_size());
	}

	{
		opts_type opts_1(opts_sample_1);
		opts_type opts_2(opts_sample_2);

		const opts_type& opts_cref1 = opts_1;
		const opts_type& opts_cref2 = opts_2;

		std::swap(opts_1, boost::move(opts_2));
		std::swap(boost::move(opts_1), opts_2);
		std::swap(opts_1, opts_2);

		yggr_test_assert(opts_cref1.var_masterkey() == opts_sample_cref_2.var_masterkey());
		yggr_test_assert(opts_cref2.var_masterkey() == opts_sample_cref_1.var_masterkey());

		yggr_test_assert(opts_cref1.var_keyaltnames().size() == opts_sample_cref_2.var_keyaltnames().size());
		yggr_test_assert(opts_cref2.var_keyaltnames().size() == opts_sample_cref_1.var_keyaltnames().size());

		yggr_test_assert(opts_cref1.var_keyaltnames_count() == opts_sample_cref_2.var_keyaltnames_count());
		yggr_test_assert(opts_cref2.var_keyaltnames_count() == opts_sample_cref_1.var_keyaltnames_count());

		yggr_test_assert(opts_cref1.var_keymaterial() == opts_sample_cref_2.var_keymaterial());
		yggr_test_assert(opts_cref2.var_keymaterial() == opts_sample_cref_1.var_keymaterial());

		yggr_test_assert(opts_cref1.var_keymaterial_size() == opts_sample_cref_2.var_keymaterial_size());
		yggr_test_assert(opts_cref2.var_keymaterial_size() == opts_sample_cref_1.var_keymaterial_size());
	}

	{
		opts_type opts_1(opts_sample_1);
		opts_type opts_2(opts_sample_2);

		const opts_type& opts_cref1 = opts_1;
		const opts_type& opts_cref2 = opts_2;

		boost::swap(opts_1, boost::move(opts_2));
		boost::swap(boost::move(opts_1), opts_2);
		boost::swap(opts_1, opts_2);

		yggr_test_assert(opts_cref1.var_masterkey() == opts_sample_cref_2.var_masterkey());
		yggr_test_assert(opts_cref2.var_masterkey() == opts_sample_cref_1.var_masterkey());

		yggr_test_assert(opts_cref1.var_keyaltnames().size() == opts_sample_cref_2.var_keyaltnames().size());
		yggr_test_assert(opts_cref2.var_keyaltnames().size() == opts_sample_cref_1.var_keyaltnames().size());

		yggr_test_assert(opts_cref1.var_keyaltnames_count() == opts_sample_cref_2.var_keyaltnames_count());
		yggr_test_assert(opts_cref2.var_keyaltnames_count() == opts_sample_cref_1.var_keyaltnames_count());

		yggr_test_assert(opts_cref1.var_keymaterial() == opts_sample_cref_2.var_keymaterial());
		yggr_test_assert(opts_cref2.var_keymaterial() == opts_sample_cref_1.var_keymaterial());

		yggr_test_assert(opts_cref1.var_keymaterial_size() == opts_sample_cref_2.var_keymaterial_size());
		yggr_test_assert(opts_cref2.var_keymaterial_size() == opts_sample_cref_1.var_keymaterial_size());
	}

	{
		opts_type opts_1(opts_sample_1);
		opts_type opts_2(opts_sample_2);

		const opts_type& opts_cref1 = opts_1;
		const opts_type& opts_cref2 = opts_2;

		yggr::swap(opts_1, boost::move(opts_2));
		yggr::swap(boost::move(opts_1), opts_2);
		yggr::swap(opts_1, opts_2);

		yggr_test_assert(opts_cref1.var_masterkey() == opts_sample_cref_2.var_masterkey());
		yggr_test_assert(opts_cref2.var_masterkey() == opts_sample_cref_1.var_masterkey());

		yggr_test_assert(opts_cref1.var_keyaltnames().size() == opts_sample_cref_2.var_keyaltnames().size());
		yggr_test_assert(opts_cref2.var_keyaltnames().size() == opts_sample_cref_1.var_keyaltnames().size());

		yggr_test_assert(opts_cref1.var_keyaltnames_count() == opts_sample_cref_2.var_keyaltnames_count());
		yggr_test_assert(opts_cref2.var_keyaltnames_count() == opts_sample_cref_1.var_keyaltnames_count());

		yggr_test_assert(opts_cref1.var_keymaterial() == opts_sample_cref_2.var_keymaterial());
		yggr_test_assert(opts_cref2.var_keymaterial() == opts_sample_cref_1.var_keymaterial());

		yggr_test_assert(opts_cref1.var_keymaterial_size() == opts_sample_cref_2.var_keymaterial_size());
		yggr_test_assert(opts_cref2.var_keymaterial_size() == opts_sample_cref_1.var_keymaterial_size());
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_datakey_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	bson_type bs_master_key("{ \"endpoint\" : \"127.0.0.1:5200\" }");
	const char* keyaltnames[] = { "mongoc_encryption_example_1", "mongoc_encryption_example_2" };
	yggr::string key_material = "abcdefgh";

	// var_masterkey
	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.var_masterkey() = bs_master_key;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);
		yggr_test_assert(opts_cref.var_masterkey<bson_t>() == bs_master_key);
		yggr_test_assert(opts_cref.var_masterkey<bson_type>() == bs_master_key);
	}

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.var_masterkey() = *bs_master_key.org_pointer();

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->masterkey);
		yggr_test_assert(opts_cref.var_masterkey() == bs_master_key);
		yggr_test_assert(opts_cref.var_masterkey<bson_t>() == bs_master_key);
		yggr_test_assert(opts_cref.var_masterkey<bson_type>() == bs_master_key);
	}

	// var_keyaltnames
	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keyaltnames(keyaltnames, keyaltnames + 2);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);
	}

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keyaltnames(keyaltnames);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);
	}

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.append_keyaltname(*keyaltnames);
		opts.append_keyaltname(*(keyaltnames + 1));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 2);
		yggr_test_assert((opts_cref.var_keyaltnames())[0]);
		yggr_test_assert((opts_cref.var_keyaltnames())[1]);
		yggr_test_assert(*(opts_cref.var_keyaltnames())[0] == "mongoc_encryption_example_1");
		yggr_test_assert(*(opts_cref.var_keyaltnames())[1] == "mongoc_encryption_example_2");
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 2);

		opts.clear_keyaltnames();
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyaltnames_count == 0);
		yggr_test_assert(opts_cref.var_keyaltnames_count() == 0);

	}

	// var_keymaterial
	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.var_keymaterial() = key_material;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keymaterial(reinterpret_cast<yggr::u8*>(const_cast<char*>(key_material.data())), key_material.size());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keymaterial(key_material.begin(), key_material.end());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		yggr::string now_key_material = key_material;

		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keymaterial(boost::move(now_key_material));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keymaterial(key_material);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
	}

	{
		yggr::vector<char> key_material_cont;
		key_material_cont.assign(key_material.begin(), key_material.end());

		opts_type opts;
		const opts_type& opts_cref = opts;

		opts.set_keymaterial(key_material_cont);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial);
		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keymaterial_len == key_material.size());
		yggr_test_assert(opts_cref.var_keymaterial() == key_material);
		yggr_test_assert(opts_cref.var_keymaterial_size() == opts_cref.var_keymaterial().size());
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