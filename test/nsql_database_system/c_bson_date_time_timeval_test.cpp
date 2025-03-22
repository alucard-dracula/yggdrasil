//c_bson_data_time_t_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/time/time_cast.hpp>
#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/c_bson_timeval.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	{
		yggr::nsql_database_system::c_bson_date tdate;

		yggr::nsql_database_system::c_bson_date date = tdate;
		std::cout << "date =\t\t" << date << std::endl;

		std::cout << "date =\t\t" << date.to_string() << std::endl;
		std::cout << "date =\t\t" << date.to_string<std::string>() << std::endl;

		std::cout << "date =\t\t" << date.to_string("%Y-%m-%d %H:%M:%S") << std::endl;

		std::cout << "date =\t\t" << date.to_string(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_string(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_string(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "date =\t\t" << date.to_string<std::string>(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_string<std::string>(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_string<std::string>(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "date =\t\t" << date.to_local_string() << std::endl;
		std::cout << "date =\t\t" << date.to_local_string<std::string>() << std::endl;

		std::cout << "date =\t\t" << date.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
		std::cout << "date =\t\t" << date.to_local_string(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_local_string(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "date =\t\t" << date.to_local_string<std::string>(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_local_string<std::string>(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "date =\t\t" << date.to_local_string<std::string>(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		yggr::nsql_database_system::c_bson_time time = tdate;
		std::cout << "time =\t\t" << time << std::endl;

		std::cout << "time =\t\t" << time << std::endl;

		std::cout << "time =\t\t" << time.to_string() << std::endl;
		std::cout << "time =\t\t" << time.to_string<std::string>() << std::endl;

		std::cout << "time =\t\t" << time.to_string("%Y-%m-%d %H:%M:%S") << std::endl;
		std::cout << "time =\t\t" << time.to_string(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "time =\t\t" << time.to_string(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "time =\t\t" << time.to_string(
										yggr::charset::make_string_charset_helper("%Y-%m-%d %H:%M:%S")) << std::endl;
		
		std::cout << "time =\t\t" << time.to_string<std::string>(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "time =\t\t" << time.to_string<std::string>(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "time =\t\t" << time.to_string<std::string>(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "time =\t\t" << time.to_string<std::string>(
										yggr::charset::make_string_charset_helper(std::string("%Y-%m-%d %H:%M:%S"))) << std::endl;

		std::cout << "time =\t\t" << time.to_local_string() << std::endl;
		std::cout << "time =\t\t" << time.to_local_string<std::string>() << std::endl;

		std::cout << "time =\t\t" << time.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
		std::cout << "time =\t\t" << time.to_local_string(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "time =\t\t" << time.to_local_string(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "time =\t\t" << time.to_local_string<std::string>(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;

		yggr::nsql_database_system::c_bson_timeval timeval = tdate;
		std::cout << "timeval =\t" << timeval << std::endl; 

		std::cout << "timeval =\t" << timeval << std::endl;

		std::cout << "timeval =\t" << timeval.to_string() << std::endl;
		std::cout << "timeval =\t" << timeval.to_string<std::string>() << std::endl;

		std::cout << "timeval =\t" << timeval.to_string("%Y-%m-%d %H:%M:%S") << std::endl;
		std::cout << "timeval =\t" << timeval.to_string(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "timeval =\t" << timeval.to_string(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "timeval =\t" << timeval.to_string<std::string>(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "timeval =\t" << timeval.to_string<std::string>(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "timeval =\t" << timeval.to_string<std::string>(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "timeval =\t" << timeval.to_local_string() << std::endl;
		std::cout << "timeval =\t" << timeval.to_local_string<std::string>() << std::endl;

		std::cout << "timeval =\t" << timeval.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
		std::cout << "timeval =\t" << timeval.to_local_string(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "timeval =\t" << timeval.to_local_string(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		std::cout << "timeval =\t" << timeval.to_local_string<std::string>(std::string("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "timeval =\t" << timeval.to_local_string<std::string>(yggr::string_view("%Y-%m-%d %H:%M:%S")) << std::endl;
		std::cout << "timeval =\t" << timeval.to_local_string<std::string>(yggr::utf8_string_view("%Y-%m-%d %H:%M:%S")) << std::endl;

		yggr::nsql_database_system::c_bson_date date2;
		date2 = tdate;
		std::cout << "date2 =\t\t" << date << std::endl;

		yggr::nsql_database_system::c_bson_time time2;
		time2 = tdate;
		std::cout << "time2 =\t\t" << time << std::endl;

		yggr::nsql_database_system::c_bson_timeval timeval2;
		timeval2 = tdate;

		std::cout << "timeval2 =\t" << timeval << std::endl; 

		std::cout << "var_tv_usec =\t" << timeval.var_tv_usec() << std::endl;
	}

	std::cout << "--------------------------------------" << std::endl;

	{
		yggr::nsql_database_system::c_bson_time ttime;

		yggr::nsql_database_system::c_bson_date date = ttime;
		std::cout << "date =\t\t" << date << std::endl;

		yggr::nsql_database_system::c_bson_time time = ttime;
		std::cout << "time =\t\t" << time << std::endl;

		yggr::nsql_database_system::c_bson_timeval timeval = ttime;

		std::cout << "timeval =\t" << timeval << std::endl; 

		yggr::nsql_database_system::c_bson_date date2;
		date2 = ttime;
		std::cout << "date2 =\t\t" << date << std::endl;

		yggr::nsql_database_system::c_bson_time time2;
		time2 = ttime;
		std::cout << "time2 =\t\t" << time << std::endl;

		yggr::nsql_database_system::c_bson_timeval timeval2;
		timeval2 = ttime;

		std::cout << "timeval2 =\t" << timeval << std::endl; 
	}

	std::cout << "--------------------------------------" << std::endl;

	{
		yggr::nsql_database_system::c_bson_timeval ttimeval;

		yggr::nsql_database_system::c_bson_date date = ttimeval;
		std::cout << "date =\t\t" << date << std::endl;

		yggr::nsql_database_system::c_bson_time time = ttimeval;
		std::cout << "time =\t\t" << time << std::endl;

		yggr::nsql_database_system::c_bson_timeval timeval = ttimeval;

		std::cout << "timeval =\t" << timeval << std::endl; 

		yggr::nsql_database_system::c_bson_date date2;
		date2 = ttimeval;
		std::cout << "date2 =\t\t" << date << std::endl;

		yggr::nsql_database_system::c_bson_time time2;
		time2 = ttimeval;
		std::cout << "time2 =\t\t" << time << std::endl;

		yggr::nsql_database_system::c_bson_timeval timeval2;
		timeval2 = ttimeval;

		std::cout << "timeval2 =\t" << timeval << std::endl; 
	}

	wait_any_key(argc, argv);
	return 0;
}
