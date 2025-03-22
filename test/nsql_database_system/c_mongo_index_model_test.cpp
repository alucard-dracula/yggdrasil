// c_mongo_index_model_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_mongo_index_model.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_index_model c_mongo_index_model_type;
	typedef c_mongo_index_model_type::bson_type bson_type;

	{
		c_mongo_index_model_type model;

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		c_mongo_index_model_type model(boost::move(keys));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		c_mongo_index_model_type model(keys);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model(boost::move(keys), boost::move(opts));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	
	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model(boost::move(keys), opts);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model(keys, boost::move(opts));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model(keys, opts);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model = model_arg.org_pointer();

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model = boost::move(model_arg);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model = model_arg;

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	std::cout << "-----------test_construct-------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_index_model c_mongo_index_model_type;
	typedef c_mongo_index_model_type::bson_type bson_type;

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model;

		model = model_arg.org_pointer();

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model;
		
		model = boost::move(model_arg);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model;
		model = model_arg;

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	std::cout << "-----------test_operator_set-------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_index_model c_mongo_index_model_type;
	typedef c_mongo_index_model_type::bson_type bson_type;

	{
		bson_type keys1;
		keys1.save("key_name1", 1);
		bson_type opts1;
		opts1.save("name", "key_index_name1");

		bson_type keys2;
		keys2.save("key_name2", -1);
		bson_type opts2;
		opts2.save("name", "key_index_name1");

		c_mongo_index_model_type model1(keys1, opts1);
		c_mongo_index_model_type model2(keys2, opts2);

		{
			model1.swap(boost::move(model2));

			yggr_test_assert(model1.var_keys() == keys2);
			yggr_test_assert(model1.var_opts() == opts2);
		}

		{
			model1.swap(model2);

			yggr_test_assert(model1.var_keys() == keys1);
			yggr_test_assert(model1.var_opts() == opts1);
		}

		{
			std::swap(model1, model2);

			yggr_test_assert(model1.var_keys() == keys2);
			yggr_test_assert(model1.var_opts() == opts2);

			std::swap(boost::move(model1), model2);

			yggr_test_assert(model1.var_keys() == keys1);
			yggr_test_assert(model1.var_opts() == opts1);

			std::swap(model1, boost::move(model2));

			yggr_test_assert(model1.var_keys() == keys2);
			yggr_test_assert(model1.var_opts() == opts2);
		}

		{
			boost::swap(model1, model2);

			yggr_test_assert(model1.var_keys() == keys1);
			yggr_test_assert(model1.var_opts() == opts1);

			boost::swap(boost::move(model1), model2);

			yggr_test_assert(model1.var_keys() == keys2);
			yggr_test_assert(model1.var_opts() == opts2);

			boost::swap(model1, boost::move(model2));

			yggr_test_assert(model1.var_keys() == keys1);
			yggr_test_assert(model1.var_opts() == opts1);
		}

		{
			yggr::swap(model1, model2);

			yggr_test_assert(model1.var_keys() == keys2);
			yggr_test_assert(model1.var_opts() == opts2);

			yggr::swap(boost::move(model1), model2);

			yggr_test_assert(model1.var_keys() == keys1);
			yggr_test_assert(model1.var_opts() == opts1);

			yggr::swap(model1, boost::move(model2));

			yggr_test_assert(model1.var_keys() == keys2);
			yggr_test_assert(model1.var_opts() == opts2);
		}
	}

	{
		bson_type keys1;
		keys1.save("key_name1", 1);
		bson_type opts1;
		opts1.save("name", "key_index_name1");

		c_mongo_index_model_type model1(keys1, opts1);
		c_mongo_index_model_type model2;

		{
			model1.swap(boost::move(model2));

			yggr_test_assert(model1.empty());
			yggr_test_assert(!model2.empty());
		}

		{
			model1.swap(model2);

			yggr_test_assert(!model1.empty());
			yggr_test_assert(model2.empty());
		}

		{
			std::swap(model1, model2);

			yggr_test_assert(model1.empty());
			yggr_test_assert(!model2.empty());

			std::swap(boost::move(model1), model2);

			yggr_test_assert(!model1.empty());
			yggr_test_assert(model2.empty());

			std::swap(model1, boost::move(model2));

			yggr_test_assert(model1.empty());
			yggr_test_assert(!model2.empty());
		}

		{
			boost::swap(model1, model2);

			yggr_test_assert(!model1.empty());
			yggr_test_assert(model2.empty());

			boost::swap(boost::move(model1), model2);

			yggr_test_assert(model1.empty());
			yggr_test_assert(!model2.empty());

			boost::swap(model1, boost::move(model2));

			yggr_test_assert(!model1.empty());
			yggr_test_assert(model2.empty());
		}

		{
			yggr::swap(model1, model2);

			yggr_test_assert(model1.empty());
			yggr_test_assert(!model2.empty());

			yggr::swap(boost::move(model1), model2);

			yggr_test_assert(!model1.empty());
			yggr_test_assert(model2.empty());

			yggr::swap(model1, boost::move(model2));

			yggr_test_assert(model1.empty());
			yggr_test_assert(!model2.empty());
		}
	}

	std::cout << "-----------test_swap-------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_index_model c_mongo_index_model_type;
	typedef c_mongo_index_model_type::bson_type bson_type;

	// assign
	{
		bson_type keys;
		keys.save("key_name", 1);

		c_mongo_index_model_type model;
		model.assign(boost::move(keys));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		c_mongo_index_model_type model;
		model.assign(keys);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model;
		model.assign(boost::move(keys), boost::move(opts));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model;
		model.assign(boost::move(keys), opts);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model;
		model.assign(keys, boost::move(opts));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model;
		model.assign(keys, opts);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model;
		model.assign(model_arg.org_pointer());

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model;
		model.assign(boost::move(model_arg));

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type opts;
		opts.save("name", "key_index_name");

		c_mongo_index_model_type model_arg(keys, opts);
		c_mongo_index_model_type model;

		model.assign(model_arg);

		std::cout << model.var_keys() << std::endl;
		std::cout << model.var_opts() << std::endl;

		yggr_test_assert(!model.empty());
		yggr_test_assert(model.var_keys() == keys);
		yggr_test_assert(model.var_opts() == opts);
	}

	// keys opts get set
	{
		bson_type keys;
		keys.save("key_name", 1);

		bson_type keys2;

		bson_type opts;
		opts.save("name", "key_index_name");

		bson_type opts2;

		c_mongo_index_model_type model(keys, opts);
		c_mongo_index_model_type& model_ref = model;
		const c_mongo_index_model_type& model_cref = model;

		model_ref.var_keys() = keys2;
		yggr_test_assert(model.empty());
		yggr_test_assert(model_cref.var_keys().empty());

		model_ref.var_keys() = keys;
		yggr_test_assert(!model.empty());
		yggr_test_assert(!model_cref.var_keys().empty());

		model_ref.var_opts() = opts2;
		yggr_test_assert(model_cref.var_opts().empty());

		model_ref.var_opts() = opts;
		yggr_test_assert(!model_cref.var_opts().empty());
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