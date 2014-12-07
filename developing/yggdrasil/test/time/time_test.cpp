//time_test.cpp


#include<iostream>
#include<ctime>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

#include <yggr/time/time.hpp>

int main()
{
	std::time_t t = std::time(0);
	yggr::time::time ttm;

	std::time_t nttm= ttm.sec;

	char tmp[64] = {0};
	std::strftime( tmp, sizeof(tmp), "%Y-%m-%d %X %A",localtime(&t) );
	std::cout << tmp << std::endl;

	yggr::time::time yggr_tm;

	std::tm tm = yggr_tm.to_tm();

	std::cout << tm.tm_year << ", " << tm.tm_mon << ", " << tm.tm_mday << ", "
		<< tm.tm_hour << ", " << tm.tm_min << ", " << tm.tm_sec << std::endl;

	std::tm tm2 = yggr_tm.to_local_tm();

	std::cout << tm2.tm_year << ", " << tm2.tm_mon << ", " << tm2.tm_mday << ", "
		<< tm2.tm_hour << ", " << tm2.tm_min << ", " << tm2.tm_sec << std::endl;


	std::cout << yggr_tm.to_string("") << std::endl;

	std::cout << yggr_tm.to_string("%Y-%m-%d-%H-%M-%S") << std::endl;
	std::cout << yggr_tm.to_local_string("%Y-%m-%d-%H-%M-%S") << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
