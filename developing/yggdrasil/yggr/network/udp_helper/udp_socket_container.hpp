// udp_socket_container.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_NETWORK_UDP_HELPER_UDP_SOCKET_CONTAINER_HPP__
#define __YGGR_NETWORK_UDP_HELPER_UDP_SOCKET_CONTAINER_HPP__

#include <algorithm> 

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/system_code.hpp>
#include <yggr/base/exception.hpp>
//#include <yggr/container/data_range.hpp>

#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/type_traits/socket_type_checker.hpp>
#include <yggr/network/network_config/network_config_template.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>


namespace yggr
{
namespace network
{
namespace udp_helper
{
template<
			typename Socket,
			typename EndPoint,
			typename Service,
			u32 ver
		>
class udp_socket_container
{

public:
	typedef Socket socket_type;
	typedef EndPoint endpoint_type;
	typedef Service service_type;
	
	typedef boost::shared_ptr<socket_type> socket_ptr_type;
	//typedef yggr::error error_type;
	typedef typename endpoint_type::protocol_type protocol_type;
	//typedef container::data_range<u16> ports_type;

private:

	enum
	{
		E_IP_VER = ver,
		E_compile_u32 = 0xffffffff
	};

private:
	typedef typename yggr::network::type_traits::socket_type_checker<socket_type>::tag tag_type;

	BOOST_MPL_ASSERT((boost::is_same<tag_type, yggr::network::type_traits::tag_udp>));

public:
	class socket_unit
	{
	public:
		struct tag_id {};
		struct tag_port {};

	public:
		socket_unit(service_type& service, const endpoint_type& edpt)
			: _psocket(new socket_type(service, edpt)), _count(1),
				_id((u64)_psocket.get()), _port(edpt.port())
		{
			//_psocket->bind(edpt);
		}

		socket_unit(const socket_unit& right)
			: _psocket(right._psocket), _count(right._count.load()),
				_id(right._id)
		{
		}

		~socket_unit(void)
		{
		}

		bool operator<(const socket_unit& right) const
		{
			return _count < right._count;
		}

		bool operator==(const socket_ptr_type& right) const
		{
			return _psocket == right;
		}

		socket_ptr_type get(void) const
		{
			++_count;
			return _psocket;
		}

		void back(void) const
		{
			--_count;
		}

		const u64& id(void) const
		{
			return _id;
		}

		const u16& port(void) const
		{
			return _port;
		}

		void close(void)
		{
			if(_psocket->is_open())
            {
				try
				{
					_psocket->shutdown(socket_type::shutdown_both);
				}
				catch(const boost::system::system_error& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
						
				try
				{
					_psocket->close();
				}
				catch(const boost::system::system_error& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
            }
		}
	private:
		socket_ptr_type _psocket;
		mutable boost::atomic<u64> _count;
		u64 _id;
		u16 _port;
	};

	typedef socket_unit socket_unit_type;
	//typedef typename socket_unit_type::tag_id tag_id_type;

private:
	typedef safe_container::safe_multi_index_container
			<
				socket_unit_type,
				boost::multi_index::indexed_by
				<
					boost::multi_index::hashed_unique
					<
						boost::multi_index::tag<typename socket_unit_type::tag_id>,
						BOOST_MULTI_INDEX_CONST_MEM_FUN(socket_unit_type, const u64&, id) 
					>,
					
					boost::multi_index::hashed_non_unique
					<
						boost::multi_index::tag<typename socket_unit_type::tag_port>,
						BOOST_MULTI_INDEX_CONST_MEM_FUN(socket_unit_type, const u16&, port) 
					>,

					boost::multi_index::random_access< >
				>
			> socket_unit_container_type;
	
	typedef udp_socket_container this_type;

public:
	//udp_socket_container(const u16& sport, const u16& eport)
	//	//: _ports(sport, eport)
	//{
	//}

	//udp_socket_container(const ports_type& ports)
	//	//: _ports(ports)
	//{
	//}

	udp_socket_container(void)
	{
	}

	~udp_socket_container(void)
	{
	}

	size_type size(void) const
	{
		return _socket_cont.size();
	}

	//socket_ptr_type tmp_get(service_type& service)
	//{

	//	return _socket_cont.use_handler(
	//							boost::bind(
	//											&this_type::tmp_handler_insert_socket, 
	//											this, _1, boost::ref(service), boost::ref(port)
	//										)
	//									);
	//}

	socket_ptr_type get(service_type& service, u16 port = 0)
	{
		if(!port)
		{
			return _socket_cont.use_handler(
									boost::bind(
													&this_type::handler_get_socket,
													this, _1
												)
											);
		}

		return _socket_cont.use_handler(
								boost::bind(
												&this_type::handler_insert_socket, 
												this, _1, boost::ref(service), boost::ref(port)
											)
										);
	}

	void back(const socket_ptr_type& back)
	{
		_socket_cont.use_handler(
									boost::bind(
													&this_type::handler_get_socket,
													this, _1
												)
								);
	}

	void close(void)
	{
		_socket_cont.use_handler(boost::bind(&this_type::handler_close, this, _1));
	}

	bool empty(void) const
	{
		return _socket_cont.empty();
	}

	void clear(void)
	{
		close();
	}

private:
	inline bool is_v4(void) const
	{
		return E_IP_VER == yggr::network::network_config::ip_ver::v4();
	}

	inline bool is_v6(void) const
	{
		return E_IP_VER == yggr::network::network_config::ip_ver::v6();
	}

	static inline bool s_is_v4(void)
	{
		return E_IP_VER == yggr::network::network_config::ip_ver::v4();
	}

	static inline bool s_is_v6(void)
	{
		return E_IP_VER == yggr::network::network_config::ip_ver::v6();
	}

private:
	socket_ptr_type handler_get_socket(typename socket_unit_container_type::base_type& base) const
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_const_iterator<base_type, 2>::type citer_type;

		if(base.empty())
		{
			return socket_ptr_type();
		}

		citer_type iter = std::min_element(base.get<2>().begin(), base.get<2>().end());
		assert((iter != base.get<2>().end()));
		return (*iter).get();
	}

	//socket_ptr_type tmp_handler_insert_socket(typename socket_unit_container_type::base_type& base, 
	//										service_type& service, const u16& port)
	//{
	//	typedef typename socket_unit_container_type::base_type base_type;
	//	typedef typename boost::multi_index::nth_index_iterator<base_type, 1>::type iter_type;
	//	typedef std::pair<iter_type, bool> ins_rst_type;

	//	/*iter_type iter = base.get<1>().find(port);

	//	if(iter != base.get<1>().end())
	//	{
	//		return (*iter).get();
	//	}*/

	//	socket_unit_type su(service, 
	//							endpoint_type(this_type::is_v4()?
	//											protocol_type::v4()
	//											: protocol_type::v6(), 
	//											0));

	//	ins_rst_type ins_rst = base.get<1>().insert(su);

	//	assert(ins_rst.second);

	//	return (*(ins_rst.first)).get();

	//}

	socket_ptr_type handler_insert_socket(typename socket_unit_container_type::base_type& base, 
											service_type& service, const u16& port)
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 1>::type iter_type;
		typedef std::pair<iter_type, bool> ins_rst_type;

		iter_type iter = base.get<1>().find(port);

		if(iter != base.get<1>().end())
		{
			return (*iter).get();
		}

		socket_unit_type su(service, 
								endpoint_type(this_type::is_v4()?
												protocol_type::v4()
												: protocol_type::v6(), 
												port));

		ins_rst_type ins_rst = base.get<1>().insert(su);

		assert(ins_rst.second);

		return (*(ins_rst.first)).get();

	}

	void handler_back_socket(typename socket_unit_container_type::base_type& base, const u64 id) const
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_const_iterator<base_type, 0>::type citer_type;
		
		if(base.empty())
		{
			return;
		}

		citer_type iter = base.get<0>().find(id);
		assert((iter != base.get<0>().end()));
		return (*iter).back();
	}

	void handler_close(typename socket_unit_container_type::base_type& base)
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 0>::type iter_type;

		for(iter_type i = base.get<0>().begin(), isize = base.get<0>().end(); i != isize; ++i)
		{
			base.modify(i, boost::bind(&this_type::handler_close_op, this, _1));
		}

		base.clear();
	}

	void handler_close_op(socket_unit_type& val)
	{
		val.close();
	}

private:
	socket_unit_container_type _socket_cont;
};

} // namespace udp_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_UDP_HELPER_UDP_SOCKET_CONTAINER_HPP__