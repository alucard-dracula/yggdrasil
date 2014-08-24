//cpu_timer_test.cpp

#include <iostream>
#include <boost/timer/timer.hpp>
#include <cmath>

static const int test_len = 1000;

void test1(void)
{
	boost::timer::auto_cpu_timer t;
	for (long i = 0; i < test_len; ++i)
		std::sqrt(123.456L); // burn some time
}

void fun(void)  
{  
    for (int i = 0;i < 1000;i++);  
} 

void test2(void)
{
	{
		boost::timer::cpu_timer timer;  
		for (int i = 0;i < test_len;i++)  
			fun();  
		std::cout << timer.format(5,"%ws wall time,%ts totle time\n") << std::endl;  
      
		boost::timer::cpu_times time = timer.elapsed();  
		std::cout << "wall time is :" << time.wall << "ns("   
				  << time.wall/1000000000.0L << "s)" << std::endl;  
	}
}

int main()
{
	test1();
	test2();

	char cc = 0;
	std::cin >> cc;
	return 0;
}

//#define BOOST_ALL_DYN_LINK  