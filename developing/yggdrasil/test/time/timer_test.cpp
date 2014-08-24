//timer_test.cpp

#include <iostream>
#include <yggr/thread/boost_thread_config.hpp>

int main(int argc, char* argv[])
{
	typedef yggr::thread::boost_thread_config_type trd_cfg_type;
	typedef trd_cfg_type::time_type time_type;

	time_type tms;
	char ccc =0;
	std::cin >> ccc;

	time_type tme;

	time_type tmstep(tme - tms);

	std::cout << tmstep.to_string() << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}