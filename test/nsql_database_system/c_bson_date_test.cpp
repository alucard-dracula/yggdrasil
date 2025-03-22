// c_bson_date_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>

#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_timeval.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_time bson_time_type;
	typedef yggr::nsql_database_system::c_bson_date bson_date_type;
	typedef yggr::nsql_database_system::c_bson_timeval bson_timeval_type;

	{
		bson_date_type data;

		std::cout << data << std::endl;
		yggr_test_assert(data);
	}

	{
		bson_date_type data(1000);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_date_type org_data(1000);
		bson_value_type org_val(org_data);
		bson_date_type data(org_val);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_time_type org_data(1);
		bson_date_type data(org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_timeval_type org_data(1, 1000);
		bson_date_type data(org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1001);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_time_type org_data(1);
		bson_date_type data(org_data.bridge_time());

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_date_type org_data(1000);
		bson_date_type data(boost::move(org_data));

		std::cout << data << std::endl;
		yggr_test_assert(data == org_data);
	}

	{
		bson_date_type org_data(1000);
		bson_date_type data(org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data == org_data);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_time bson_time_type;
	typedef yggr::nsql_database_system::c_bson_date bson_date_type;
	typedef yggr::nsql_database_system::c_bson_timeval bson_timeval_type;

	{
		bson_date_type data;
		
		data = 1000;

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_date_type org_data(1000);
		bson_value_type org_val(org_data);
		bson_date_type data;

		data = (org_val);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_time_type org_data(1);
		bson_date_type data;
		
		data = (org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_timeval_type org_data(1, 1000);
		bson_date_type data;
		
		data = (org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1001);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_time_type org_data(1);
		bson_date_type data;
		
		data = (org_data.bridge_time());

		std::cout << data << std::endl;
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.time() == 1);
	}

	{
		bson_date_type org_data(1000);
		bson_date_type data;
		
		data = (boost::move(org_data));

		std::cout << data << std::endl;
		yggr_test_assert(data == org_data);
	}

	{
		bson_date_type org_data(1000);
		bson_date_type data;
		
		data = (org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data == org_data);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_time bson_time_type;
	typedef yggr::nsql_database_system::c_bson_date bson_date_type;
	typedef yggr::nsql_database_system::c_bson_timeval bson_timeval_type;

	{
		bson_date_type org_data1(1000);
		bson_date_type org_data2(2000);

		bson_date_type data1(org_data1);
		bson_date_type data2(org_data2);

		data1.swap(boost::move(data2));
		
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		data1.swap(data2);
		
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		data1.swap(boost::move(data2.date()));
		
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		data1.swap(data2.date());
		
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_date_type org_data1(1000);
		bson_date_type org_data2(2000);

		bson_date_type data1(org_data1);
		bson_date_type data2(org_data2);

		boost::swap(data1, boost::move(data2));
		
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		boost::swap(boost::move(data1), data2);
		
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(data1, data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);
	}

	{
		bson_date_type org_data1(1000);
		bson_date_type org_data2(2000);

		bson_date_type data1(org_data1);
		bson_date_type data2(org_data2);

		yggr::swap(data1, boost::move(data2));
		
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(boost::move(data1), data2);
		
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		yggr::swap(data1, data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

#include <yggr/time/time_basic_foo.hpp>

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_time bson_time_type;
	typedef yggr::nsql_database_system::c_bson_date bson_date_type;
	typedef yggr::nsql_database_system::c_bson_timeval bson_timeval_type;

	{
		yggr_test_assert(bson_date_type::s_value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
		yggr_test_assert(bson_date_type().value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
	}

	{
		bson_date_type data(1000);
		
		yggr_test_assert(data.time() == 1);
		yggr_test_assert(data.date() == 1000);
		yggr_test_assert(data.bridge_time().sec == 1);
		yggr_test_assert(data.bridge_time().nsec == 0);
	}

	{
		std::cout << "utc_calendar_time_diff:" << yggr::time::utc_calendar_time_diff() << std::endl;
		std::cout << "utc_local_time_diff:" << yggr::time::utc_local_time_diff() << std::endl;
		std::cout << "local_calendar_time_diff:" << yggr::time::local_calendar_time_diff() << std::endl;

		bson_time_type tm;
		bson_date_type date(tm);
		bson_timeval_type tmval(tm);

		yggr_test_assert(tm == date);
		yggr_test_assert(tm == tmval);
		yggr_test_assert(date == tmval);

		std::cout << date.to_string() << std::endl;
		std::cout << date.to_local_string() << std::endl;

		std::cout << "----------------" << std::endl;

		std::cout << tm.to_string() << std::endl;
		std::cout << tm.to_local_string() << std::endl;

		std::cout << "----------------" << std::endl;

		std::cout << tmval.to_string() << std::endl;
		std::cout << tmval.to_local_string() << std::endl;
	}

	{
		typedef yggr::nsql_database_system::bson_date_time_format bson_date_time_format_type;
		bson_date_type data(1001);

		yggr::utf8_string chk_str_utc_iso8601_date = "1970-01-01T00:00:01.001Z";
		yggr::utf8_string chk_str_utc_iso8601_time = "1970-01-01T00:00:01Z";

		char tmp_str[256] = {0};
		std::cout << yggr::nsql_database_system::bson_date_time_native_ex::s_bson_date_format_iso8601(data.date(), tmp_str, 255) << std::endl;

		std::cout << data.to_string() << std::endl;
		std::cout << data.to_string(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) << std::endl;

		std::cout << bson_time_type(1).to_string() << std::endl;
		std::cout << bson_time_type(1).to_string(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) << std::endl;

		yggr_test_assert(data.to_string() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string(yggr::utf8_string().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), yggr::utf8_string().get_allocator()) == chk_str_utc_iso8601_date);

		yggr_test_assert(data.to_string<std::string>() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<std::string>(std::string().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<std::string>(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<std::string>(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), std::string().get_allocator()) == chk_str_utc_iso8601_date);

		yggr_test_assert(data.to_string<boost::container::string>() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<boost::container::string>(boost::container::string().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<boost::container::string>(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<boost::container::string>(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), boost::container::string().get_allocator()) == chk_str_utc_iso8601_date);

		yggr_test_assert(data.to_string<yggr::string>() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<yggr::string>(yggr::string().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<yggr::string>(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<yggr::string>(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), yggr::string().get_allocator()) == chk_str_utc_iso8601_date);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(data.to_string<std::wstring>() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<std::wstring>(std::wstring().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<std::wstring>(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<std::wstring>(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), std::wstring().get_allocator()) == chk_str_utc_iso8601_date);

		yggr_test_assert(data.to_string<boost::container::wstring>() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<boost::container::wstring>(boost::container::wstring().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<boost::container::wstring>(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<boost::container::wstring>(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), boost::container::wstring().get_allocator()) == chk_str_utc_iso8601_date);

		yggr_test_assert(data.to_string<yggr::wstring>() == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<yggr::wstring>(yggr::wstring().get_allocator()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<yggr::wstring>(bson_date_time_format_type::tm_fmt_iso8601_utc_org()) == chk_str_utc_iso8601_date);
		yggr_test_assert(data.to_string<yggr::wstring>(bson_date_time_format_type::tm_fmt_iso8601_utc_org(), yggr::wstring().get_allocator()) == chk_str_utc_iso8601_date);
#endif // YGGR_NO_CWCHAR
	}

	{
		bson_date_type data(1001);

		yggr::utf8_string chk_str_local_default = "1970-01-01T08:00:01.001000";
		yggr::utf8_string chk_str_local_iso8601 = "1970-01-01T08:00:01.001000";

		std::cout << data.to_local_string() << std::endl;
		std::cout << data.to_local_string(yggr::time::time_format::tm_fmt_iso8601()) << std::endl;

		std::cout << bson_time_type(1).to_local_string() << std::endl;
		std::cout << bson_time_type(1).to_local_string(yggr::time::time_format::tm_fmt_iso8601()) << std::endl;

		yggr_test_assert(data.to_local_string() == chk_str_local_default);
		yggr_test_assert(data.to_local_string(yggr::utf8_string().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string(yggr::time::time_format::tm_fmt_iso8601(), yggr::utf8_string().get_allocator()) == chk_str_local_iso8601);

		yggr_test_assert(data.to_local_string<std::string>() == chk_str_local_default);
		yggr_test_assert(data.to_local_string<std::string>(std::string().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string<std::string>(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string<std::string>(yggr::time::time_format::tm_fmt_iso8601(), std::string().get_allocator()) == chk_str_local_iso8601);

		yggr_test_assert(data.to_local_string<boost::container::string>() == chk_str_local_default);
		yggr_test_assert(data.to_local_string<boost::container::string>(boost::container::string().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string<boost::container::string>(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string<boost::container::string>(yggr::time::time_format::tm_fmt_iso8601(), boost::container::string().get_allocator()) == chk_str_local_iso8601);

		yggr_test_assert(data.to_local_string<yggr::string>() == chk_str_local_default);
		yggr_test_assert(data.to_local_string<yggr::string>(yggr::string().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string<yggr::string>(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string<yggr::string>(yggr::time::time_format::tm_fmt_iso8601(), yggr::string().get_allocator()) == chk_str_local_iso8601);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(data.to_local_string<std::wstring>() == chk_str_local_default);
		yggr_test_assert(data.to_local_string<std::wstring>(std::wstring().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601(), std::wstring().get_allocator()) == chk_str_local_iso8601);

		yggr_test_assert(data.to_local_string<boost::container::wstring>() == chk_str_local_default);
		yggr_test_assert(data.to_local_string<boost::container::wstring>(boost::container::wstring().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string<boost::container::wstring>(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string<boost::container::wstring>(yggr::time::time_format::tm_fmt_iso8601(), boost::container::wstring().get_allocator()) == chk_str_local_iso8601);

		yggr_test_assert(data.to_local_string<yggr::wstring>() == chk_str_local_default);
		yggr_test_assert(data.to_local_string<yggr::wstring>(yggr::wstring().get_allocator()) == chk_str_local_default);
		yggr_test_assert(data.to_local_string<yggr::wstring>(yggr::time::time_format::tm_fmt_iso8601()) == chk_str_local_iso8601);
		yggr_test_assert(data.to_local_string<yggr::wstring>(yggr::time::time_format::tm_fmt_iso8601(), yggr::wstring().get_allocator()) == chk_str_local_iso8601);
#endif // YGGR_NO_CWCHAR
	}

	// cmper
	{
		bson_date_type tmval(1000);

		bson_date_type tmval_same(tmval);
		bson_date_type tmval_diff(2000);

		bson_value_type bs_val_same(tmval_same);
		bson_value_type bs_val_diff(tmval_diff);

		// date <=> date
		yggr_test_assert(tmval == tmval);
		yggr_test_assert(tmval == tmval_same);
		yggr_test_assert(tmval != tmval_diff);
		yggr_test_assert(tmval < tmval_diff);
		yggr_test_assert(tmval <= tmval_diff);
		yggr_test_assert(!(tmval > tmval_diff));
		yggr_test_assert(!(tmval >= tmval_diff));

		// date <=> time
		yggr_test_assert(tmval == bson_time_type(tmval));
		yggr_test_assert(tmval == bson_time_type(tmval_same));
		yggr_test_assert(tmval != bson_time_type(tmval_diff));
		yggr_test_assert(tmval < bson_time_type(tmval_diff));
		yggr_test_assert(tmval <= bson_time_type(tmval_diff));
		yggr_test_assert(!(tmval > bson_time_type(tmval_diff)));
		yggr_test_assert(!(tmval >= bson_time_type(tmval_diff)));

		// date <=> timeval
		yggr_test_assert(tmval == bson_timeval_type(tmval));
		yggr_test_assert(tmval == bson_timeval_type(tmval_same));
		yggr_test_assert(tmval != bson_timeval_type(tmval_diff)); // less ms
		yggr_test_assert(tmval < bson_timeval_type(tmval_diff));
		yggr_test_assert(tmval <= bson_timeval_type(tmval_diff));
		yggr_test_assert(!(tmval > bson_timeval_type(tmval_diff)));
		yggr_test_assert(!(tmval >= bson_timeval_type(tmval_diff)));

		// date <=> xtime
		yggr_test_assert(tmval == tmval.bridge_time());
		yggr_test_assert(tmval == tmval_same.bridge_time());
		yggr_test_assert(tmval != tmval_diff.bridge_time());
		yggr_test_assert(tmval < tmval_diff.bridge_time());
		yggr_test_assert(tmval <= tmval_diff.bridge_time());
		yggr_test_assert(!(tmval > tmval_diff.bridge_time()));
		yggr_test_assert(!(tmval >= tmval_diff.bridge_time()));

		// xtime <=> date
		yggr_test_assert(tmval.bridge_time() == tmval);
		yggr_test_assert(tmval.bridge_time() == tmval_same);
		yggr_test_assert(tmval.bridge_time() != tmval_diff);
		yggr_test_assert(tmval.bridge_time() < tmval_diff);
		yggr_test_assert(tmval.bridge_time() <= tmval_diff);
		yggr_test_assert(!(tmval.bridge_time() > tmval_diff));
		yggr_test_assert(!(tmval.bridge_time() >= tmval_diff));

		// date <=> bson_value
		yggr_test_assert(tmval == bs_val_same);
		yggr_test_assert(tmval != bs_val_diff);
		yggr_test_assert(tmval < bs_val_diff);
		yggr_test_assert(tmval <= bs_val_diff);
		yggr_test_assert(!(tmval > bs_val_diff));
		yggr_test_assert(!(tmval >= bs_val_diff));

		// bson_value <=> date 
		yggr_test_assert(bs_val_same == tmval_same);
		yggr_test_assert(bs_val_same != tmval_diff);
		yggr_test_assert(bs_val_same < tmval_diff);
		yggr_test_assert(bs_val_same <= tmval_diff);
		yggr_test_assert(!(bs_val_same > tmval_diff));
		yggr_test_assert(!(bs_val_same >= tmval_diff));
	}

	// output
	{
		bson_date_type data(1001);
		
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		bson_date_type data(1001);

		std::string chk_str1 = "{ \"$date\" : 1001 }";
		std::string chk_str2 = "{ \"$date\" : { \"$numberLong\" : \"1001\" } }";
		std::string chk_str3 = "{ \"$date\" : \"1970-01-01T00:00:01.001Z\" }";

		{
			std::stringstream ss;
			ss << data;
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data);
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data);
			yggr_test_assert(ss.str() == chk_str2);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data);
			yggr_test_assert(ss.str() == chk_str3);
		}
	}

	{
		bson_type bs;
		bson_date_type data(1001);
		bs.save("c_bson_date", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_date_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_date");
		bs_iter.load("c_bson_date", ld_data);

		yggr_test_assert(ld_data == data);
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