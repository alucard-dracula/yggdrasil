//time_basic_foo_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

// #define YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME

#include <yggr/time/time_basic_foo.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable : 4996 )
#endif // _MSC_VER

void test_time_foo_call_warning(void)
{
	std::tm tmp_tm = {0};

	{	
		std::time_t cnd_tm = std::time(0);

		std::tm* putc_tm = boost::date_time::c_time::gmtime(&cnd_tm, &tmp_tm);
		std::tm* plcl_tm = boost::date_time::c_time::localtime(&cnd_tm, &tmp_tm);

		yggr_test_assert(putc_tm == plcl_tm);

		std::time_t utc_tm = mktime(putc_tm);
		std::time_t lcl_tm = mktime(plcl_tm);

		// yggr_test_assert(utc_tm != lcl_tm); // ok
		yggr_test_assert(utc_tm == lcl_tm); // failed
	}

	{
		std::time_t cnd_tm = std::time(0);

		std::tm* putc_tm = boost::date_time::c_time::gmtime(&cnd_tm, &tmp_tm);
		std::time_t utc_tm = mktime(putc_tm);

		std::tm* plcl_tm = boost::date_time::c_time::localtime(&cnd_tm, &tmp_tm);
		std::time_t lcl_tm = mktime(plcl_tm);

		yggr_test_assert(putc_tm == plcl_tm);

		yggr_test_assert(utc_tm != lcl_tm); // ok
		//yggr_test_assert(utc_tm == lcl_tm); // failed
	}

	
	{
		std::time_t cnd_tm = std::time(0);
		std::time_t cnd_tm_zwei = cnd_tm + 86400;

		std::tm* putc_tm = boost::date_time::c_time::gmtime(&cnd_tm, &tmp_tm);
		std::tm* plcl_tm = boost::date_time::c_time::localtime(&cnd_tm_zwei, &tmp_tm);

		yggr_test_assert(putc_tm == plcl_tm);

		std::time_t utc_tm = mktime(putc_tm);
		std::time_t lcl_tm = mktime(plcl_tm);

		//yggr_test_assert(utc_tm != lcl_tm); // ok
		yggr_test_assert(utc_tm == lcl_tm && utc_tm == cnd_tm_zwei); // failed
	}

}

void test(void)
{
	std::cout << "utc_calendar_time_diff:" << yggr::time::utc_calendar_time_diff() << std::endl;
	std::cout << "utc_local_time_diff:" << yggr::time::utc_local_time_diff() << std::endl;
	std::cout << "local_calendar_time_diff:" << yggr::time::local_calendar_time_diff() << std::endl;

	//std::time_t first_cnd_time = std::time(0);
	//std::time_t first_cnd_time = 1; // mktime(gmtime()) failed return -1
	std::time_t first_cnd_time = 28801;

	std::tm utc_tm = {0};
	std::tm* putc_tm = yggr::time::calendar_time_to_utc_tm(first_cnd_time, utc_tm);
	std::time_t utc_time = yggr::time::calendar_time_to_utc_time(first_cnd_time);

	std::tm cnd_tm = {0};
	std::tm* pcnd_tm = yggr::time::calendar_time_to_calendar_tm(first_cnd_time, cnd_tm);
	std::time_t cnd_time = first_cnd_time;

	std::tm lcl_tm = {0};
	std::tm* plcl_tm = yggr::time::calendar_time_to_local_tm(first_cnd_time, lcl_tm);
	std::time_t lcl_time = yggr::time::calendar_time_to_local_time(first_cnd_time);

	std::tm tmp_tm = {0};

	// utc_time
	yggr_test_assert(utc_time == yggr::time::calendar_time_to_utc_time(cnd_time));
	yggr_test_assert(utc_time == yggr::time::local_time_to_utc_time(lcl_time));

	yggr_test_assert(utc_time == yggr::time::utc_tm_to_utc_time(putc_tm));
	yggr_test_assert(utc_time == yggr::time::calendar_tm_to_utc_time(pcnd_tm));
	yggr_test_assert(utc_time == yggr::time::local_tm_to_utc_time(plcl_tm));

	yggr_test_assert(utc_time == yggr::time::utc_tm_to_utc_time(utc_tm));
	yggr_test_assert(utc_time == yggr::time::calendar_tm_to_utc_time(cnd_tm));
	yggr_test_assert(utc_time == yggr::time::local_tm_to_utc_time(lcl_tm));

	yggr_test_assert(utc_time == yggr::time::utc_tm_to_utc_time(yggr::time::calendar_tm_to_utc_tm(cnd_tm, tmp_tm)));
	yggr_test_assert(utc_time == yggr::time::utc_tm_to_utc_time(yggr::time::local_tm_to_utc_tm(lcl_tm, tmp_tm)));

	// cnd_time
	yggr_test_assert(cnd_time == yggr::time::utc_time_to_calendar_time(utc_time));
	yggr_test_assert(cnd_time == yggr::time::local_time_to_calendar_time(lcl_time));

	yggr_test_assert(cnd_time == yggr::time::utc_tm_to_calendar_time(putc_tm));
	yggr_test_assert(cnd_time == yggr::time::calendar_tm_to_calendar_time(pcnd_tm));
	yggr_test_assert(cnd_time == yggr::time::local_tm_to_calendar_time(plcl_tm));

	yggr_test_assert(cnd_time == yggr::time::utc_tm_to_calendar_time(utc_tm));
	yggr_test_assert(cnd_time == yggr::time::calendar_tm_to_calendar_time(cnd_tm));
	yggr_test_assert(cnd_time == yggr::time::local_tm_to_calendar_time(lcl_tm));

	yggr_test_assert(cnd_time == yggr::time::calendar_tm_to_calendar_time(yggr::time::utc_tm_to_calendar_tm(utc_tm, tmp_tm)));
	yggr_test_assert(cnd_time == yggr::time::calendar_tm_to_calendar_time(yggr::time::local_tm_to_calendar_tm(lcl_tm, tmp_tm)));

	// lcl_time
	yggr_test_assert(lcl_time == yggr::time::utc_time_to_local_time(utc_time));
	yggr_test_assert(lcl_time == yggr::time::calendar_time_to_local_time(cnd_time));

	yggr_test_assert(lcl_time == yggr::time::utc_tm_to_local_time(putc_tm));
	yggr_test_assert(lcl_time == yggr::time::calendar_tm_to_local_time(pcnd_tm));
	yggr_test_assert(lcl_time == yggr::time::local_tm_to_local_time(plcl_tm));

	yggr_test_assert(lcl_time == yggr::time::utc_tm_to_local_time(utc_tm));
	yggr_test_assert(lcl_time == yggr::time::calendar_tm_to_local_time(cnd_tm));
	yggr_test_assert(lcl_time == yggr::time::local_tm_to_local_time(lcl_tm));

	yggr_test_assert(lcl_time == yggr::time::local_tm_to_local_time(yggr::time::utc_tm_to_local_tm(utc_tm, tmp_tm)));
	yggr_test_assert(lcl_time == yggr::time::local_tm_to_local_time(yggr::time::calendar_tm_to_local_tm(lcl_tm, tmp_tm)));
}


#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER


int main(int argc, char* argv[])
{
	test_time_foo_call_warning();

	test();

	wait_any_key(argc, argv);
	return 0;
}
