// safe_multiset_performance_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
////#define YGGR_SAFE_CONTAINER_NOT_USING_ALLOCATOR_OPT

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <functional>
#include <cstdlib>

#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/safe_container/safe_set.hpp>
#include <yggr/safe_container/safe_options_set.hpp>

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

typedef std::vector<std::string> vt_type;
typedef std::vector<vt_type> vt_vt_type;

typedef yggr::time::runtime_performance_tester<yggr::time::time> test_type;

template<typename SafeCont>
void safe_range_push(SafeCont& cont, const vt_type& vt, int s, int e)
{
	for(int i = s, isize = e; i != isize; ++i)
	{
		cont.insert(vt[i]);
	}
}

template<typename SafeCont>
void safe_range_pop(SafeCont& cont, int s, int e)
{
	typedef SafeCont safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;


#if defined(_MSC_VER)
    typedef typename unsafe_cont_type::iterator iter_type;
	typedef iter_type (unsafe_cont_type::*foo_type)(void);
#else
    typedef typename unsafe_cont_type::const_iterator iter_type;
	typedef iter_type (unsafe_cont_type::*foo_type)(void) const;
#endif // _MSC_VER

	for(int i = s, isize = e; i != isize; ++i)
	{
		cont.erase(boost::bind(static_cast<foo_type>(&unsafe_cont_type::begin), _1));
	}
}

template<typename SafeCont>
void gen_data(vt_vt_type& vtvt, int size)
{
	typedef SafeCont safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;

	vtvt.resize(size);
	std::string str(g_string_size, ',');
	unsafe_cont_type tmp;

	char sch[2] = {'a', 'A'};
	for(int i = 0; tmp.size() != g_test_length; ++i)
	{
		str[i % str.size()] = sch[rand() % 2] + rand() % 26;
		str[rand() % str.size()] = sch[rand() % 2] + rand() % 26;
		str[rand() % str.size()] = sch[rand() % 2] + rand() % 26;
		tmp.insert(str);
	}

	typename unsafe_cont_type::const_iterator s = tmp.begin();
	typename unsafe_cont_type::const_iterator e = s;

	int step = g_test_length / size;

	for(int i = 0; i != size; ++i)
	{
		std::advance(e, step);
		vtvt[i].assign(s, e);
		std::advance(s, step);
	}
}

template<typename SafeCont>
void test_safe_multi(void)
{
	typedef SafeCont safe_cont_type;

	std::cout << "-----------test_safe_multi s--------------" << std::endl;
	safe_cont_type cont;
	boost::thread_group trds;

	int step = g_test_length / g_thread_size;

	vt_vt_type datas;
	gen_data<safe_cont_type>(datas, g_thread_size);

	std::cout << "mulit safe push time: " << std::endl;
	{
		test_type test;
		for(int i = 0; i != g_thread_size; ++i)
		{
			trds.create_thread(boost::bind(&safe_range_push<safe_cont_type>,
												boost::ref(cont),
												boost::cref(datas[i]),
												0,
												step));
		}
		trds.join_all();
	}

	if(cont.size() != g_test_length)
	{
		std::cout << "!!!!!!!!!!!!!!!!less data !!!!!!!!!!!!!!!!!!!! " << g_test_length - cont.size() << std::endl;
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
			typedef yggr::safe_container::safe_multiset<std::string> set_type;
			test_safe_multi<set_type>();
		}

		{
			typedef
				yggr::safe_container::safe_multiset
				<
					std::string,
					boost::mutex,
					std::less<std::string>,
					std::allocator<std::string>,
					std::multiset
				> set_type;
			test_safe_multi<set_type>();
		}

		{
			typedef
				yggr::safe_container::safe_multiset
				<
					std::string,
					boost::mutex,
					std::less<std::string>,
					std::allocator<std::string>,
					std::multiset
				> set_type;
			test_safe_multi<set_type>();
		}

#	if BOOST_VERSION < 105600
		{
			typedef
				yggr::safe_container::safe_multiset
				<
					std::string,
					boost::mutex,
					std::less<std::string>,
					std::allocator<std::string>,
					boost::container::multiset
				> set_type;
			test_safe_multi<set_type>();
		}
#	endif // BOOST_VERSION < 105600

		{
			typedef
				yggr::safe_container::safe_multiset
				<
					std::string,
					boost::mutex,
					std::less<std::string>,
					std::allocator<std::string>,
					yggr::multiset
				> set_type;
			test_safe_multi<set_type>();
		}

#	if !(BOOST_VERSION < 105800)
		{
			typedef
				yggr::safe_container::safe_multiset
				<
					std::string,
					boost::mutex,
					std::less<std::string>,
					boost::container::new_allocator<std::string>,
					yggr::multiset
				> set_type;
			test_safe_multi<set_type>();
		}

#	endif // !(BOOST_VERSION < 105800)

	} while( THREAD_TEST_USING_MANUAL() );

	wait_any_key(argc, argv);
	return 0;
}
