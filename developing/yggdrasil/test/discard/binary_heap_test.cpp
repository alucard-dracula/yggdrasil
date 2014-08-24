//binary_heap_test.cpp

#include <iostream>
#include <yggr/normal_struct/binary_heap.hpp>
#include <yggr/base/random.h>
#include <yggr/time/time.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_DEBUG


typedef yggr::normal_struct::binary_heap<int, std::greater<int> > heap_type;

void test1(void)
{
	heap_type heap;

	yggr::random::init();

	for(int i = 0; i != 5; ++i)
	{
		heap.push(yggr::random::gen_random_integer<int>(1, 1000));
	}

	/*heap.push(10);
	heap.push(5);
	heap.push(20);*/

	{
		yggr::size_type isize = heap.size();

		for(yggr::size_type i = 0; i != isize; ++i)
		{
			std::cout << heap[i] << std::endl;
		}
	}

	std::cout << heap.front() << std::endl;
	std::cout << heap.back() << std::endl;

	std::cout << "----------------" << std::endl;
	heap_type heap2(heap);

	{
		yggr::size_type isize = heap.size();

		for(yggr::size_type i = 0; i != isize; ++i)
		{
			std::cout << heap2[i] << std::endl;
		}
	}

	std::cout << heap2.front() << std::endl;
	std::cout << heap2.back() << std::endl;
}

void test2(void)
{
	heap_type heap;

	yggr::time::time push_tm(0, 0);

	yggr::time::time tm1;
	for(int i = 0; i != 100000; ++i)
	{
		heap.push(yggr::random::gen_random_integer<int>(1, 100000));
	}
	yggr::time::time tm2;

	push_tm = tm2 - tm1;

	std::cout << "end" << std::endl;

	std::cout << "count time" << push_tm.sec << "," << push_tm.nsec
				<< "(" << (double)push_tm.nsec / yggr::time::time::E_NS_TO_S<< ")" << std::endl;

	std::cout << "avg time" << (double)push_tm.sec / 100000  << "," << (double)push_tm.nsec / 100000
				<< "(" << ((double)push_tm.nsec / 100000) / yggr::time::time::E_NS_TO_S<< ")" << std::endl;

}

void test3(void)
{
	heap_type heap;

	yggr::random::init();

	//heap.push(0, 137);
	//heap.push(1, 332);
	//heap.push(2, 611);
	//heap.push(3, 920);
	//heap.push(4, 769);

	{
		for(int i = 0; i != 10; ++i)
		{
			heap.push(yggr::random::gen_random_integer<int>(1, 1000));
		}
	}


	{
		yggr::size_type isize = heap.size();

		for(yggr::size_type i = 0; i != isize; ++i)
		{
			std::cout << heap[i] << std::endl;
		}
	}

	std::cout << "--------------------------------" << std::endl;
	{
		for(;!heap.empty();)
		{
			std::cout << heap.front() << std::endl;
			heap.pop();
		}
	}

}

void test4(void)
{
	typedef yggr::normal_struct::binary_heap<int, std::greater<int> > heap_greater_type;
	heap_greater_type heap;

	yggr::random::init();

	{
		for(int i = 0; i != 100000; ++i)
		{
			heap.push(yggr::random::gen_random_integer<int>(1, 1000));
		}
	}

	std::cout << "--------------------------------" << std::endl;

	int now = 0;

	{
		for(;!heap.empty();)
		{
			if(!now)
			{
				now =  heap.front() ;
			}
			else
			{
				if(now < heap.front())
				{
					std::cout << "error" << std::endl;
				}
			}
			heap.pop();
		}

		std::cout << "end" << std::endl;
	}

}

void test5(void)
{
	typedef yggr::normal_struct::binary_heap<int, std::less<int> > heap_less_type;
	heap_less_type heap;

	yggr::random::init();

	{
		for(int i = 0; i != 100000; ++i)
		{
			heap.push(yggr::random::gen_random_integer<int>(1, 1000));
		}
	}

	std::cout << "--------------------------------" << std::endl;

	int now = 0;

	{
		for(;!heap.empty();)
		{
			if(!now)
			{
				now =  heap.front() ;
			}
			else
			{
				if(now > heap.front())
				{
					std::cout << "error" << std::endl;
				}
			}
			heap.pop();
		}

		std::cout << "end" << std::endl;
	}

}



int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();
	test4();
	test5();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
