//runtime_speed_test_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::time::time time_type;
typedef yggr::time::runtime_performance_tester<time_type, yggr::f64, std::stringstream> tester_type;

void test(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	std::stringstream ss;
	std::cout << "input any test waitting time" << std::endl;
	{
		tester_type tester(1.0f, ss);
		if(!detail::wait_any_key_detail(argc, argv))
		{
			this_thread_type::sleep(thread_config_type::time_type(10, 0));
		}
	}

	std::cout << ss.str() << std::endl;
}


int main(int argc, char* argv[])
{
	test(argc, argv);
	
	wait_any_key(argc, argv);
	return 0;
}
