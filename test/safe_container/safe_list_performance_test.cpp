//safe_list_performance_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <functional>
#include <cstdlib>

#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/safe_container/safe_list.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)


yggr::u32 g_test_length = 1000;
yggr::u32 g_thread_size = 4;
yggr::u32 g_string_size = 1024;

#define THREAD_TEST_USING_MANUAL() 0

typedef yggr::time::runtime_performance_tester<yggr::time::time> test_type;

template<typename SafeCont>
void safe_range_push(SafeCont& cont, std::string str, int s, int e )
{
	for(int i = s, isize = e; i != isize; ++i)
	{
		str[rand() % str.size()] = 'a' + rand() % 26;
		cont.push_back(str);
	}
}

template<typename SafeCont>
void safe_range_pop(SafeCont& cont, int s, int e)
{
	for(int i = s, isize = e; i != isize; ++i)
	{
		cont.pop_back();
	}
}

template<typename SafeCont>
void test_safe_multi(void)
{
	typedef SafeCont safe_cont_type;

	std::cout << "-----------test_safe_multi s--------------" << std::endl;
	safe_cont_type cont;
	boost::thread_group trds;

	std::string str(g_string_size, 'A');

	int step = g_test_length / g_thread_size;

	std::cout << "mulit safe push time: " << std::endl;
	{
		test_type test;
		for(int i = 0; i != g_thread_size; ++i)
		{
			trds.create_thread(boost::bind(&safe_range_push<safe_cont_type>,
												boost::ref(cont),
												str,
												step * i,
												step * (i + 1)));
		}
		trds.join_all();
	}

	std::cout << "-------------------------" << std::endl;
	std::cout << "multi safe pop time: " << std::endl;
	{
		test_type test;
		for(int i = 0; i != g_thread_size; ++i)
		{
			trds.create_thread(boost::bind(&safe_range_pop<safe_cont_type>,
												boost::ref(cont),
												step * i,
												step * (i + 1)));
		}
		trds.join_all();
	}

	if(!cont.empty())
	{
		std::cout << cont.size() << std::endl;
		std::cout << "!!!!!non empty!!!!!" << std::endl;
	}
	std::cout << "-----------test_safe_multi e--------------" << std::endl;
}

int main(int argc, char* argv[])
{
	srand(std::time(0));

#	if THREAD_TEST_USING_MANUAL()
	yggr::u32 now_test_length = 1000000,
				now_thread_size = 4,
				now_string_size = 4096;
#	else
	yggr::u32 now_test_length = 0,
				now_thread_size = 0,
				now_string_size = 0;
#	endif // THREAD_TEST_USING_MANUAL()

	do
	{

#	if THREAD_TEST_USING_MANUAL()
		std::cout << "input test size and thread size and string size (0 is end): " << std::endl;
		std::cin >> now_test_length;
		std::cin >> now_thread_size;
		std::cin >> now_string_size;

		if(!now_string_size)
		{
			break;
		}
#endif // THREAD_TEST_USING_MANUAL()

		g_test_length = now_test_length? now_test_length : g_test_length;
		g_thread_size = now_thread_size? now_thread_size : g_thread_size;
		g_string_size = now_string_size? now_string_size : g_string_size;


		{
			typedef yggr::safe_container::safe_list<std::string> list_type;
			test_safe_multi<list_type>();
		}

		{
			typedef
				yggr::safe_container::safe_list
				<
					std::string, 
					boost::mutex, 
					std::allocator<std::string> 
				> list_type;
			test_safe_multi<list_type>();
		}
	
#	if !(BOOST_VERSION < 105800)
		{
			typedef 
				yggr::safe_container::safe_list
				<
					std::string,
					boost::mutex, 
					boost::container::new_allocator<std::string> 
				> list_type;
			test_safe_multi<list_type>();
		}
#	endif // (BOOST_VERSION < 105800)

		{
			typedef
				yggr::safe_container::safe_list
				<
					std::string, 
					boost::mutex, 
					std::allocator<std::string>, 
					std::list
				> list_type;
			test_safe_multi<list_type>();
		}

	} while( THREAD_TEST_USING_MANUAL() );

	wait_any_key(argc, argv);
	return 0;
}
