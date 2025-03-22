// c_mongo_client_encryption_rewrap_many_datakey_result_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.hpp>

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

	typedef yggr::nsql_database_system::c_mongo_client_encryption_rewrap_many_datakey_result result_type;
	typedef result_type::base_type base_result_type;
	typedef result_type::org_type org_result_type;

	{
		result_type result;
		yggr_test_assert(result.empty());
	}

	{
		bson_type bs_sample("{\"aaa\" : 1}");
		bson_type bs(bs_sample);
		result_type result(*(bs.org_pointer()));
		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == bs_sample);
	}

	{
		bson_type bs_sample("{\"aaa\" : 1}");
		bson_type bs(bs_sample);
		result_type result(bs);
		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == bs_sample);
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));

		result_type result(result_sample.org_pointer());

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));

		result_type result(*(result_sample.org_pointer()));

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));
		result_type result_tmp(result_sample);

		result_type result(boost::move(result_tmp));

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));

		result_type result(result_sample);

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_rewrap_many_datakey_result result_type;
	typedef result_type::base_type base_result_type;
	typedef result_type::org_type org_result_type;

	{
		bson_type bs_sample("{\"aaa\" : 1}");
		bson_type bs(bs_sample);
		result_type result;
		result = (*(bs.org_pointer()));
		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == bs_sample);
	}

	{
		bson_type bs_sample("{\"aaa\" : 1}");
		bson_type bs(bs_sample);
		result_type result;
		result = (bs);
		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == bs_sample);
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));

		result_type result;
		result = (*(result_sample.org_pointer()));

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));
		result_type result_tmp(result_sample);

		result_type result;
		result = (boost::move(result_tmp));

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	{
		result_type result_sample(bson_type("{\"aaa\" : 1}"));

		result_type result;
		result = (result_sample);

		yggr_test_assert(!result.empty());
		yggr_test_assert(result.result() == result_sample.result());
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}


void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_rewrap_many_datakey_result result_type;
	typedef result_type::base_type base_result_type;
	typedef result_type::org_type org_result_type;

	result_type result_sample_1(bson_type("{\"aaa\" : 1}"));
	result_type result_sample_2(bson_type("{\"bbb\" : 2}"));

	{
		result_type result1 = result_sample_1;
		result_type result2 = result_sample_2;

		result1.swap(boost::move(result2));

		yggr_test_assert(result1.result() == result_sample_2.result());
		yggr_test_assert(result2.result() == result_sample_1.result());
	}

	{
		result_type result1 = result_sample_1;
		result_type result2 = result_sample_2;

		result1.swap(result2);

		yggr_test_assert(result1.result() == result_sample_2.result());
		yggr_test_assert(result2.result() == result_sample_1.result());
	}

	// std::swap
	{
		result_type result1 = result_sample_1;
		result_type result2 = result_sample_2;

		std::swap(boost::move(result1), result2);
		std::swap(result1, boost::move(result2));
		std::swap(result1, result2);

		yggr_test_assert(result1.result() == result_sample_2.result());
		yggr_test_assert(result2.result() == result_sample_1.result());
	}

	// boost::swap
	{
		result_type result1 = result_sample_1;
		result_type result2 = result_sample_2;

		boost::swap(boost::move(result1), result2);
		boost::swap(result1, boost::move(result2));
		boost::swap(result1, result2);

		yggr_test_assert(result1.result() == result_sample_2.result());
		yggr_test_assert(result2.result() == result_sample_1.result());
	}

	// yggr::swap
	{
		result_type result1 = result_sample_1;
		result_type result2 = result_sample_2;

		boost::swap(boost::move(result1), result2);
		boost::swap(result1, boost::move(result2));
		boost::swap(result1, result2);

		yggr_test_assert(result1.result() == result_sample_2.result());
		yggr_test_assert(result2.result() == result_sample_1.result());
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_client_encryption_rewrap_many_datakey_result result_type;
	typedef result_type::base_type base_result_type;
	typedef result_type::org_type org_result_type;

	bson_type bs_rst("{\"aaa\" : 1}");

	// result
	{
		result_type rst;
		const result_type& rst_cref = rst;

		yggr_test_assert(rst.empty());

		rst.result() = bs_rst;

		yggr_test_assert(!rst.empty());
		yggr_test_assert(rst.result() == bs_rst);

		std::cout << rst.result() << std::endl;
		std::cout << rst.result<bson_t>() << std::endl;
		std::cout << rst.result<bson_type>() << std::endl;

		std::cout << rst_cref.result() << std::endl;
		std::cout << rst_cref.result<bson_t>() << std::endl;
		std::cout << rst_cref.result<bson_type>() << std::endl;
	}

	// clear
	{
		result_type rst(bs_rst);
		yggr_test_assert(!rst.empty());

		rst.clear();
		
		yggr_test_assert(rst.empty());
	}

	// empty
	{
		result_type rst;
		yggr_test_assert(rst.empty());

		rst = bs_rst;
		
		yggr_test_assert(!rst.empty());
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