//raw_socket_test.cpp

#error "this test is now failed after fixer";
// this test is now failed after fixer
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <boost/bind.hpp>

typedef boost::asio::basic_raw_socket<boost::asio::ip::tcp> socket_type;
//typedef boost::asio::ip::tcp::socket socket_type;
typedef boost::asio::io_service service_type;
typedef boost::thread thread_type;
typedef yggr::shared_ptr<thread_type> thread_ptr_type;
typedef socket_type::endpoint_type endpoint_type;
//typedef boost::asio::ip::tcp::endpoint endpoint_type;
typedef boost::asio::ip::tcp protocol_type;

typedef protocol_type::resolver resolver_type;
typedef resolver_type::iterator resolver_iter_type;
typedef resolver_type::query resolver_query_type;

service_type g_service;

void handler_connect(const boost::system::error_code& e)
{
	if(!e)
	{
		std::cout << "sucess" << std::endl;
		return;
	}

	std::cout << e.message() << std::endl;

}

/*
boost::asio::ip::tcp::endpoint endpoint(
    boost::asio::ip::address::from_string("1.2.3.4"), 12345);
*/

int main(int argc, char* argv[])
{
	try
	{
		socket_type s(g_service);
		//thread_ptr_type ptrd(new thread_type(boost::bind(&service_type::run, &g_service)));

		resolver_type rslvr(g_service);
		resolver_query_type query("127.0.0.1", "9000");
		resolver_iter_type edpt_iter = rslvr.resolve(query);
		endpoint_type edpt = *edpt_iter;

		//endpoint_type edpt(boost::asio::ip::address::from_string("127.0.0.1"), 9000);
		s.open(protocol_type::v4());
		//s.async_connect(edpt, boost::bind(&::handler_connect, boost::asio::placeholders::error));
		s.async_connect(edpt, ::handler_connect);
		//ptrd->join();
		g_service.run();
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	char cc = 0;
	std::cin >> cc;
	return 0;
}