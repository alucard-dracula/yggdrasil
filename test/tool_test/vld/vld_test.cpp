// vld_test.cpp

#include <iostream>

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include <yggr/safe_container/safe_vector.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#define NO_TEST_MEMLEAK() 1

void test1()
{
	int* p = new int(10);
	int* parr = new int[10];

#if NO_TEST_MEMLEAK()
	delete p;
	delete []parr;
#endif // NO_TEST_MEMLEAK
}

typedef yggr::safe_container::safe_vector<int*> int_ptr_vt_type;
typedef int_ptr_vt_type::base_type unsafe_int_ptr_vt_type;

int g_loop_size = 2000;
int g_thread_size = 10;

void new_thread_run(int_ptr_vt_type& vt)
{
	for(int i = 0; i != g_loop_size; ++i)
	{
		vt.push_back(new int(i + 1));
	}
}

void test2(void)
{
	boost::thread_group trds;
	int_ptr_vt_type vt;

	for(int i = 0; i != g_thread_size; ++i)
	{
		trds.create_thread(boost::bind(&new_thread_run, boost::ref(vt)));
	}

	trds.join_all();

#if NO_TEST_MEMLEAK()
	{
		unsafe_int_ptr_vt_type tmp;
		vt.swap(tmp);

		for(unsafe_int_ptr_vt_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			if(*i)
			{
				delete (*i);
			}
		}
	}
#endif // NO_TEST_MEMLEAK
}

int main(int argc, char* argv[])
{
	test1();
	test2();

	std::cout << "all test end" << std::endl;

	char cc = 0;
	std::cin >> cc;

	return 0;
}