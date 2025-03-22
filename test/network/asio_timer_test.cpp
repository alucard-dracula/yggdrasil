//asio_timer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void print(const boost::system::error_code& e,
			boost::asio::deadline_timer& t,
			boost::asio::io_service& io,
			yggr::u32& i, const yggr::u32& count)
{
	std::cout << i <<  ": Hello, world! " << std::endl;
	t.expires_at(t.expires_at() + boost::posix_time::seconds(1));
	++i;
	if(i < count)
	{
		t.async_wait(
			boost::bind(
				&print,
				_1,
				boost::ref(t),
				boost::ref(io),
				boost::ref(i),
				boost::cref(count)));
	}
	else
	{
		io.stop();
		io.reset();
	}
}

int main(int argc, char* argv[])
{
	yggr::u32 i = 0, count = 10;
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
	t.async_wait(
		boost::bind(
			&print,
			_1,
			boost::ref(t),
			boost::ref(io),
			boost::ref(i),
			boost::cref(count)));
	io.run();

	wait_any_key(argc, argv);
    return 0;
}
