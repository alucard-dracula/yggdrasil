//calculator.hpp


#include "calculator.hpp"

/*static*/
void Calculator::print_socket_info(session_mgr_type::smap_type::base_type& base,
									session_mgr_type::smap_iter_type& iter)
{
	try
	{
		std::cout << iter->second->str_local_socket_info() << std::endl;
		std::cout << iter->second->str_remote_socket_info() << std::endl;

#if 0
		iter->second->set_option(boost::asio::ip::tcp::socket::linger(1, 0));

		boost::asio::ip::tcp::socket::linger linger;
		iter->second->get_option(linger);
#endif // 0, 1
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
	}
}
