//asio_timer_test.cpp

#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void print(const boost::system::error_code& e, boost::asio::deadline_timer& t)
{
	std::cout << "Hello, world! " << std::endl;
	t.expires_at(t.expires_at() + boost::posix_time::seconds(1));
	t.async_wait(boost::bind(&print, _1, boost::ref(t)));
}
int main()
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
    t.async_wait(boost::bind(&print, _1, boost::ref(t)));
    io.run();

	char cc = 0;
	std::cin >> cc;
    return 0;
}
