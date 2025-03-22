//timer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;
	typedef thread_config_type::time_type time_type;

	time_type tms;

	std::cout << "wait some times (input any char end)" << std::endl; 
	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(5, 0));
	}

	time_type tme;

	time_type tmstep(tme - tms);

	std::cout << tmstep.to_string() << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
