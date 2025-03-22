//boost_interlocked_test.cpp

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER
#include <iostream>
#include <boost/interprocess/detail/atomic.hpp>

int main(int argc, char* argv[])
{
	volatile boost::uint32_t a = 10;
    boost::uint32_t b = 11;

    //for(;boost::interprocess::detail::atomic_cas32(&a, b, a) == a;);
	std::cout << boost::interprocess::ipcdetail::atomic_cas32(&a, b, a) << std::endl;
	std::cout << boost::interprocess::ipcdetail::atomic_inc32(&a) << std::endl;

	std::cout <<a << std::endl;


	char cc = 0;
	std::cin >> cc;
	return 0;
}
