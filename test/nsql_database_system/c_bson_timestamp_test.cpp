// c_bson_timestamp_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_timestamp.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson_timestamp bson_timestamp_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_timestamp_type bs_tmstamp;

		yggr_test_assert(bs_tmstamp.timestamp() == 0);
		std::cout << bs_tmstamp << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp(1, 1);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
		std::cout << bs_tmstamp << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_value_type bs_tmstamp_val(bs_tmstamp_smp);
		bson_timestamp_type bs_tmstamp(bs_tmstamp_val);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
		std::cout << bs_tmstamp << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type& bs_tmstamp_smp_ref = bs_tmstamp_smp;
		bson_timestamp_type bs_tmstamp(boost::move(bs_tmstamp_smp_ref));

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
		std::cout << bs_tmstamp << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		const bson_timestamp_type& bs_tmstamp_smp_ref = bs_tmstamp_smp;
		bson_timestamp_type bs_tmstamp(bs_tmstamp_smp_ref);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
		std::cout << bs_tmstamp << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type bs_tmstamp(boost::move(bs_tmstamp_smp));

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
		std::cout << bs_tmstamp << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type bs_tmstamp(bs_tmstamp_smp);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
		std::cout << bs_tmstamp << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson_timestamp bson_timestamp_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_value_type bs_tmstamp_val(bs_tmstamp_smp);
		bson_timestamp_type bs_tmstamp;
		bs_tmstamp = bs_tmstamp_val;

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type& bs_tmstamp_smp_ref = bs_tmstamp_smp;
		bson_timestamp_type bs_tmstamp;
		bs_tmstamp = boost::move(bs_tmstamp_smp_ref);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		const bson_timestamp_type& bs_tmstamp_smp_cref = bs_tmstamp_smp;
		bson_timestamp_type bs_tmstamp;
		bs_tmstamp = bs_tmstamp_smp_cref;

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type bs_tmstamp;
		bs_tmstamp = boost::move(bs_tmstamp_smp);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type bs_tmstamp;
		bs_tmstamp = bs_tmstamp_smp;

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}


void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson_timestamp bson_timestamp_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type bs_tmstamp;

		bs_tmstamp.swap(boost::move(bs_tmstamp_smp));

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	{
		bson_timestamp_type bs_tmstamp_smp(1, 1);
		bson_timestamp_type bs_tmstamp;

		bs_tmstamp.swap(bs_tmstamp_smp);

		yggr_test_assert(bs_tmstamp.timestamp() == UINT64_C(0x0000000100000001));
	}

	{
		bson_timestamp_type bs_tmstamp_chk1(1, 1);
		bson_timestamp_type bs_tmstamp_chk2(2, 2);

		bson_timestamp_type bs_tmstamp1 = bs_tmstamp_chk1;
		bson_timestamp_type bs_tmstamp2 = bs_tmstamp_chk2;

		{
			std::swap(boost::move(bs_tmstamp1), bs_tmstamp2);

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk2);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk1);

			std::swap(bs_tmstamp1, boost::move(bs_tmstamp2));

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk1);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk2);

			std::swap(bs_tmstamp1, bs_tmstamp2);

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk2);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk1);
		}

		{
			boost::swap(boost::move(bs_tmstamp1), bs_tmstamp2);

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk1);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk2);

			boost::swap(bs_tmstamp1, boost::move(bs_tmstamp2));

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk2);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk1);

			boost::swap(bs_tmstamp1, bs_tmstamp2);

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk1);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk2);
		}

		{
			yggr::swap(boost::move(bs_tmstamp1), bs_tmstamp2);

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk2);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk1);

			yggr::swap(bs_tmstamp1, boost::move(bs_tmstamp2));

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk1);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk2);

			yggr::swap(bs_tmstamp1, bs_tmstamp2);

			yggr_test_assert(bs_tmstamp1 == bs_tmstamp_chk2);
			yggr_test_assert(bs_tmstamp2 == bs_tmstamp_chk1);
		}
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}


void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_timestamp bson_timestamp_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// value_typeid
	{
		bson_timestamp_type bs_timestamp;

		yggr_test_assert(bson_timestamp_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_TIMESTAMP);
		yggr_test_assert(bson_timestamp_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_TIMESTAMP);
		yggr_test_assert(bs_timestamp.value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_TIMESTAMP);
		yggr_test_assert(bs_timestamp.s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_TIMESTAMP);
	}

	// compare_eq
	{
		bson_timestamp_type bs_tmstamp_chk1(1, 1);
		bson_timestamp_type bs_tmstamp_chk2(2, 2);

		bson_timestamp_type bs_tmstamp = bs_tmstamp_chk1;

		bool bchk = false;

		bchk = bs_tmstamp.compare_eq(bs_tmstamp_chk1);
		yggr_test_assert(bchk);

		bchk = bs_tmstamp.compare_eq(bs_tmstamp_chk2);
		yggr_test_assert(!bchk);

		bchk = (bs_tmstamp == bs_tmstamp_chk1);
		yggr_test_assert(bchk);

		bchk = (bs_tmstamp != bs_tmstamp_chk1);
		yggr_test_assert(!bchk);

		bchk = (bs_tmstamp == bs_tmstamp_chk2);
		yggr_test_assert(!bchk);

		bchk = (bs_tmstamp != bs_tmstamp_chk2);
		yggr_test_assert(bchk);
	}

	{
		bson_timestamp_type bs_tmstamp(1, 1);
		const bson_timestamp_type& bs_tmstamp_cref = bs_tmstamp;

		bool bchk = false;

		bchk = (bs_tmstamp.time() == 1);
		yggr_test_assert(bchk);

		bchk = (bs_tmstamp_cref.time() == 1);
		yggr_test_assert(bchk);

		bchk = (bs_tmstamp.inc() == 1);
		yggr_test_assert(bchk);

		bchk = (bs_tmstamp_cref.inc() == 1);
		yggr_test_assert(bchk);

		bs_tmstamp.grab();

		bchk = (bs_tmstamp.inc() == 2);
		yggr_test_assert(bchk);

		bchk = (bs_tmstamp_cref.inc() == 2);
		yggr_test_assert(bchk);

		bchk = (bs_tmstamp.timestamp() == UINT64_C(0x0000000200000001)); // need test other compiler
		yggr_test_assert(bchk);
	}

	// output
	{
		bson_timestamp_type bs_tmstamp(1, 1);

		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_tmstamp) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_tmstamp) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_tmstamp) << std::endl;
	}

	{
		bson_timestamp_type bs_tmstamp(1, 1);

		std::string chk_str = "{ \"$timestamp\" : { \"t\" : 1, \"i\" : 1 } }";

		{
			std::stringstream ss;
			ss << bs_tmstamp;
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_tmstamp);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_tmstamp);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_tmstamp);
			yggr_test_assert(ss.str() == chk_str);
		}
	}

	//save load
	{
		bson_timestamp_type bs_tmstamp(1, 1);

		bson_type bs;
		bs.save("c_bson_timestamp", bs_tmstamp);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_timestamp_type ld_bs_tmstamp;
		bson_type::iterator bs_iter(bs, "c_bson_timestamp");
		bs_iter.load("c_bson_timestamp", ld_bs_tmstamp);

		yggr_test_assert(bs_tmstamp == ld_bs_tmstamp);
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
