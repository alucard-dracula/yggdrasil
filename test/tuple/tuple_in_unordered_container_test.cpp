//tuple_in_unordered_container_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/unordered_set.hpp>
#include <yggr/tuple_ex/tuple.hpp> // must befor unordered before

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


void test_unordered_container(void)
{

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
#	if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		typedef std::tuple<int, int> tuple_type;
		typedef std::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

	{
		typedef std::tuple<int, int> tuple_type;
		typedef boost::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}

		{
			ins_ret_type rst = set.insert(std::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

#	endif // YGGR_NO_CXX11_HDR_TUPLE

	{
		typedef boost::tuple<int, int> tuple_type;
		typedef std::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

	{
		typedef yggr::tuple<int, int> tuple_type;
		typedef std::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

	{
		typedef boost::tuple<int, int> tuple_type;
		typedef boost::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}

		{
			ins_ret_type rst = set.insert(boost::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}

	{
		typedef yggr::tuple<int, int> tuple_type;
		typedef boost::unordered_set<tuple_type> tuple_set_type;

		typedef std::pair<tuple_set_type::iterator, bool> ins_ret_type;

		tuple_set_type set;

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(rst.second);
		}

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 10));
			yggr_test_assert(!rst.second);
		}

		{
			ins_ret_type rst = set.insert(yggr::make_tuple(10, 1));
			yggr_test_assert(rst.second);
		}
	}
}



int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_unordered_container();

	wait_any_key(argc, argv);
	return 0;
}
