// c_bson_error_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>

#include <iostream>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include <yggr/compile_link/linker.hpp>

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson_error bson_error_type;

	{
		bson_error_type err;

		yggr_test_assert(!err);
		std::cout << err << std::endl;
	}

	{
		bson_error_type err;
		bson_set_error(err.org_pointer(), 1, 2, "this is a test error, id is %d", 10);

		yggr_test_assert(err);
		std::cout << err << std::endl;
	}

	{
		bson_error_type err(MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_SOCKET,
								"Failed to write rpc bytes.");

		yggr_test_assert(err);
		std::cout << err << std::endl;
	}

	{
		bson_error_type err(1, 2, "this is a test error, id is %d", 10);

		yggr_test_assert(err);
		std::cout << err << std::endl;
	}

	{
		bson_error_type::org_type org_err = {0};
		yggr::nsql_database_system::bson_error_native_ex::s_bson_error_init(
			boost::addressof(org_err), 1, 2, "this is a test error, id is %d", 10);

		bson_error_type err(boost::move(org_err));

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain
			&& err.code() == org_err.code);
		std::cout << err << std::endl;
	}

	{
		bson_error_type::org_type org_err = {0};
		yggr::nsql_database_system::bson_error_native_ex::s_bson_error_init(
			boost::addressof(org_err), 1, 2, "this is a test error, id is %d", 10);

		bson_error_type err(org_err);

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain
			&& err.code() == org_err.code);
		std::cout << err << std::endl;
	}

	{
		bson_error_type org_err(1, 2, "this is a test error, id is %d", 10);

		bson_error_type err(boost::move(org_err));

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain()
			&& err.code() == org_err.code());
		std::cout << err << std::endl;
	}

	{
		bson_error_type org_err(1, 2, "this is a test error, id is %d", 10);

		bson_error_type err(org_err);

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain()
			&& err.code() == org_err.code());
		std::cout << err << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson_error bson_error_type;

	{
		bson_error_type::org_type org_err = {0};
		yggr::nsql_database_system::bson_error_native_ex::s_bson_error_init(
			boost::addressof(org_err), 1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err = boost::move(org_err);

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain
			&& err.code() == org_err.code);
		std::cout << err << std::endl;
	}

	{
		bson_error_type::org_type org_err = {0};
		yggr::nsql_database_system::bson_error_native_ex::s_bson_error_init(
			boost::addressof(org_err), 1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err = org_err;

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain
			&& err.code() == org_err.code);
		std::cout << err << std::endl;
	}

	{
		bson_error_type org_err(1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err = boost::move(org_err);

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain()
			&& err.code() == org_err.code());
		std::cout << err << std::endl;
	}

	{
		bson_error_type org_err(1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err = org_err;

		yggr_test_assert(err);
		yggr_test_assert(
			err.domain() == org_err.domain()
			&& err.code() == org_err.code());
		std::cout << err << std::endl;
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson_error bson_error_type;

	{
		bson_error_type::org_type org_err = {0};
		yggr::nsql_database_system::bson_error_native_ex::s_bson_error_init(
			boost::addressof(org_err), 1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err.swap(boost::move(org_err));

		yggr_test_assert(err);
		yggr_test_assert(err.domain() == 1);
		yggr_test_assert(err.code() == 2);

		yggr_test_assert(org_err.domain == 0);
		yggr_test_assert(org_err.code == 0);

		std::cout << err << std::endl;
	}

	{
		bson_error_type::org_type org_err = {0};
		yggr::nsql_database_system::bson_error_native_ex::s_bson_error_init(
			boost::addressof(org_err), 1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err.swap(org_err);

		yggr_test_assert(err);
		yggr_test_assert(err.domain() == 1);
		yggr_test_assert(err.code() == 2);

		yggr_test_assert(org_err.domain == 0);
		yggr_test_assert(org_err.code == 0);

		std::cout << err << std::endl;
	}

	{
		bson_error_type org_err(1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err.swap(boost::move(org_err));

		yggr_test_assert(err);
		yggr_test_assert(err.domain() == 1);
		yggr_test_assert(err.code() == 2);

		yggr_test_assert(org_err.domain() == 0);
		yggr_test_assert(org_err.code() == 0);
		std::cout << err << std::endl;
	}

	{
		bson_error_type org_err(1, 2, "this is a test error, id is %d", 10);

		bson_error_type err;
		err.swap(org_err);

		yggr_test_assert(err);
		yggr_test_assert(err.domain() == 1);
		yggr_test_assert(err.code() == 2);

		yggr_test_assert(org_err.domain() == 0);
		yggr_test_assert(org_err.code() == 0);
		std::cout << err << std::endl;
	}

	{
		bson_error_type err1_chk(1, 1, "this is a test error, id is %d", 1);
		bson_error_type err2_chk(2, 2, "this is a test error, id is %d", 2);

		bson_error_type err1 = err1_chk;
		bson_error_type err2 = err2_chk;

		bson_error_type::base_type& base_err1 = err1;
		bson_error_type::base_type& base_err2 = err2;

		//swap org, org
		{
			{
				std::swap(base_err1, base_err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				std::swap(boost::move(base_err1), base_err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				std::swap(base_err1, boost::move(base_err2));
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}
		}

		{
			{
				boost::swap(base_err1, base_err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				boost::swap(boost::move(base_err1), base_err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				boost::swap(base_err1, boost::move(base_err2));
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}
		}

		{
			{
				yggr::swap(base_err1, base_err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				yggr::swap(boost::move(base_err1), base_err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				yggr::swap(base_err1, boost::move(base_err2));
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}
		}

		// swap org yggr
		{
			{
				std::swap(base_err1, err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				std::swap(boost::move(base_err1), err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				std::swap(base_err1, boost::move(err2));
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}
		}

		{
			{
				boost::swap(base_err1, err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				boost::swap(boost::move(base_err1), err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				boost::swap(base_err1, boost::move(err2));
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}
		}

		{
			{
				yggr::swap(base_err1, err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				yggr::swap(boost::move(base_err1), err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				yggr::swap(base_err1, boost::move(err2));
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}
		}

		//swap yggr, org
		{
			{
				std::swap(err1, base_err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				std::swap(boost::move(err1), base_err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				std::swap(err1, boost::move(base_err2));
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}
		}

		{
			{
				boost::swap(err1, base_err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				boost::swap(boost::move(err1), base_err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				boost::swap(err1, boost::move(base_err2));
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}
		}

		{
			{
				yggr::swap(err1, base_err2);
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}

			{
				yggr::swap(boost::move(err1), base_err2);
				yggr_test_assert(base_err1.domain == err1_chk.domain());
				yggr_test_assert(base_err2.domain == err2_chk.domain());
			}

			{
				yggr::swap(err1, boost::move(base_err2));
				yggr_test_assert(base_err1.domain == err2_chk.domain());
				yggr_test_assert(base_err2.domain == err1_chk.domain());
			}
		}

		//swap yggr, org
		{
			{
				std::swap(err1, err2);
				yggr_test_assert(err1.domain() == err1_chk.domain());
				yggr_test_assert(err2.domain() == err2_chk.domain());
			}

			{
				std::swap(boost::move(err1), base_err2);
				yggr_test_assert(err1.domain() == err2_chk.domain());
				yggr_test_assert(err2.domain() == err1_chk.domain());
			}

			{
				std::swap(err1, boost::move(base_err2));
				yggr_test_assert(err1.domain() == err1_chk.domain());
				yggr_test_assert(err2.domain() == err2_chk.domain());
			}
		}

		{
			{
				boost::swap(err1, err2);
				yggr_test_assert(err1.domain() == err2_chk.domain());
				yggr_test_assert(err2.domain() == err1_chk.domain());
			}

			{
				boost::swap(boost::move(err1), base_err2);
				yggr_test_assert(err1.domain() == err1_chk.domain());
				yggr_test_assert(err2.domain() == err2_chk.domain());
			}

			{
				boost::swap(err1, boost::move(base_err2));
				yggr_test_assert(err1.domain() == err2_chk.domain());
				yggr_test_assert(err2.domain() == err1_chk.domain());
			}
		}

		{
			{
				yggr::swap(err1, err2);
				yggr_test_assert(err1.domain() == err1_chk.domain());
				yggr_test_assert(err2.domain() == err2_chk.domain());
			}

			{
				yggr::swap(boost::move(err1), base_err2);
				yggr_test_assert(err1.domain() == err2_chk.domain());
				yggr_test_assert(err2.domain() == err1_chk.domain());
			}

			{
				yggr::swap(err1, boost::move(base_err2));
				yggr_test_assert(err1.domain() == err1_chk.domain());
				yggr_test_assert(err2.domain() == err2_chk.domain());
			}
		}
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_bson_error bson_error_type;

	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	{
		bson_error_type err(1, 2, "this is a test error, id is %d", 10);

		yggr_test_assert(err.domain() == 1);
		yggr_test_assert(err.code() == 2);
		yggr_test_assert(err.what() == yggr::string("this is a test error, id is 10"));
		yggr_test_assert(err.what() == err.message());

	}

	// output
	{
		bson_error_type err(1, 2, "this is a test error, id is %d", 10);

		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(err) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(err) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(err) << std::endl;
	}

	{
		bson_error_type err(1, 2, "this is a test error, id is %d", 10);

		std::string chk_str = "{ \"domain\" : 1, \"code\" : 2, \"msg\" : \"this is a test error, id is 10\" }";

		{
			std::stringstream ss;
			ss << err;
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(err);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(err);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(err);
			yggr_test_assert(ss.str() == chk_str);
		}
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
