//io_service_pool_test.cpp

#include <iostream>

#include <boost/asio.hpp>

#include <yggr/network/balance_io_service.hpp>
#include <yggr/network/io_service_pool.hpp>
#include <yggr/network/balance_io_service_selector.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#ifdef _MSC_VER
#pragma comment(lib, "vld.lib")
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	yggr::network::io_service_pool<boost::asio::io_service> pool1(10);
	pool1.get_service();
	//pool1.run();
	pool1.stop();

	typedef yggr::network::balance_io_service<boost::asio::io_service> balance_io_service_type;
	typedef yggr::network::io_service_pool<balance_io_service_type,
											yggr::thread::boost_thread_config_type,
											std::allocator, std::vector,
											yggr::network::balance_io_service_selector> pool2_type;

	pool2_type pool2(10);
	pool2.get_service();
	//pool2.run();
	pool2.stop();

	std::cout << "end" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
