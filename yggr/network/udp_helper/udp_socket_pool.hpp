// udp_socket_pool.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_NETWORK_UDP_HELPER_UDP_SOCKET_POOL_HPP__
#define __YGGR_NETWORK_UDP_HELPER_UDP_SOCKET_POOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/cast_pointer_to_int.hpp>
#include <yggr/system_controller/system_code.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/type_traits/socket_type_checker.hpp>
#include <yggr/network/type_traits/is_socket_option.hpp>
#include <yggr/network/network_config/network_config_template.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_ordered_index.hpp>
#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/atomic.hpp>
#include <boost/type_traits/is_same.hpp>

#include <algorithm>

namespace yggr
{
namespace network
{
namespace udp_helper
{
namespace detail
{

template<
	typename Socket,
	typename EndPoint,
	typename Service >
class socket_unit
{
public:
	struct tag_id {};
	struct tag_count {};

public:
	typedef Socket socket_type;
	typedef EndPoint endpoint_type;
	typedef Service service_type;

	typedef yggr::mplex::pointer_int::type socket_id_type;
	typedef ::yggr::shared_ptr<socket_type> socket_ptr_type;

private:
	typedef socket_unit this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	socket_unit(service_type& service, const endpoint_type& edpt)
		: _psocket(construct_shared<socket_type>(yggr_nothrow_new socket_type(service, edpt))),
			_count(0),
			_id(utility::cast_pointer_to_int::cast<socket_id_type>(_psocket.get()))
	{
		assert(_psocket);
	}

	socket_unit(BOOST_RV_REF(this_type) right)
		: _psocket(boost::move(right._psocket)),
			_count(boost::move(right._count)),
			_id(boost::move(right._id))
	{
		assert(_psocket);
	}

	socket_unit(const this_type& right)
		: _psocket(right._psocket),
			_count(right._count),
			_id(right._id)
	{
		assert(_psocket);
	}

	~socket_unit(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		_psocket = boost::move(right._psocket);
		_count = boost::move(right._count);
		_id = boost::move(right._id);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_psocket = right._psocket;
		_count = right._count;
		_id = right._id;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		_psocket.swap(right._psocket);
		::yggr::swap(_count, right._count);
		::yggr::swap(_id, right._id);
	}

public:
	template<typename SettableSocketOption> inline
	typename
		boost::enable_if
		<
			::yggr::network::type_traits::is_settable_socket_option<SettableSocketOption>,
			bool
		>::type
		set_option(const SettableSocketOption& opt) const // mutable
	{
		try
		{
			return _psocket && (_psocket->set_option(opt), true);
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			return false;
		}
	}

	template<typename Handler> inline
	typename
		boost::disable_if
		<
			::yggr::network::type_traits::is_settable_socket_option<Handler>,
			bool
		>::type
		set_option(const Handler& handler)
	{
		assert(_psocket);
		return !!handler(*this);
	}

public:
	inline bool operator<(const socket_unit& right) const
	{
		return _count < right._count;
	}

	inline bool operator==(const socket_ptr_type& right) const
	{
		return _psocket == right;
	}

	inline socket_ptr_type get(void) const
	{
		++_count;
		return _psocket;
	}

	inline void back(void) const
	{
		assert(_count > 0);
		--_count;
	}

	inline socket_id_type id(void) const
	{
		return _id;
	}

	inline u64 count(void) const
	{
		return _count;
	}

	void close(void)
	{
		if(!_psocket)
		{
			return;
		}

		if(_psocket->is_open())
        {
			try
			{
				_psocket->shutdown(socket_type::shutdown_both);
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}

			try
			{
				_psocket->close();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
        }
	}
private:
	mutable socket_ptr_type _psocket;
	mutable u64 _count;
	socket_id_type _id;
};

} // namespace detail
} // namespace udp_helper
} // namespace network
} // namespace yggr

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
	u32 ver >
class udp_socket_pool
{
public:
	typedef Socket socket_type;
	typedef EndPoint endpoint_type;
	typedef Service service_type;

	typedef ::yggr::shared_ptr<socket_type> socket_ptr_type;
	typedef typename endpoint_type::protocol_type protocol_type;

	typedef yggr::mplex::pointer_int::type socket_id_type;

	typedef boost::atomic<size_type> limit_size_type;

public:
	YGGR_STATIC_CONSTANT(u32, E_IP_VER = ver);

	typedef boost::mpl::bool_<(E_IP_VER == yggr::network::network_config::ip_ver::E_IPV4)> is_v4_type;
	typedef boost::mpl::bool_<(E_IP_VER == yggr::network::network_config::ip_ver::E_IPV6)> is_v6_type;

private:
	typedef typename yggr::network::type_traits::socket_type_checker<socket_type>::tag tag_type;
	BOOST_MPL_ASSERT((boost::is_same<tag_type, yggr::network::type_traits::tag_udp>));

public:
	typedef
		detail::socket_unit
		<
			socket_type,
			endpoint_type,
			service_type
		> socket_unit_type;

private:
	typedef
		safe_container::safe_multi_index_container
		<
			socket_unit_type,
			boost::multi_index::indexed_by
			<
				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<typename socket_unit_type::tag_id>,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(socket_unit_type, socket_id_type, id)
				>,

				boost::multi_index::ordered_non_unique
				<
					boost::multi_index::tag<typename socket_unit_type::tag_count>,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(socket_unit_type, u64, count)
				>
			>
		> socket_unit_container_type;

private:
	typedef udp_socket_pool this_type;

public:
	udp_socket_pool(void)
		:_limit_size(1)
	{
	}

	udp_socket_pool(size_type lim_size)
		: _limit_size((std::max<size_type>)(lim_size, 1))
	{
	}

	~udp_socket_pool(void)
	{
	}

public:
	inline size_type size(void) const
	{
		return _socket_cont.size();
	}

	inline size_type limit_size(void) const
	{
		return _limit_size.load();
	}

	inline bool empty(void) const
	{
		return _socket_cont.empty();
	}

public:
	inline socket_ptr_type get(service_type& service)
	{
		if(_socket_cont.size() < _limit_size.load())
		{
			socket_unit_type su(service,
								endpoint_type(
									this_type::is_v4()?
										protocol_type::v4()
										: protocol_type::v6(),
									0));

			return
				_socket_cont.using_handler(
					boost::bind(
						&this_type::prv_handler_insert_socket_eins,
						this, _1, boost::ref(service), boost::addressof(su) ) );
		}
		else
		{
			return
				_socket_cont.using_handler(
					boost::bind(
						&this_type::prv_handler_insert_socket_eins,
						this, _1, boost::ref(service), static_cast<socket_unit_type*>(0) ) );
		}
	}

	template<typename OptionSetter> inline
	socket_ptr_type get(service_type& service, const OptionSetter& ops_setter)
	{
		typedef OptionSetter ops_setter_type;

		if(_socket_cont.size() < _limit_size.load())
		{
			socket_unit_type su(service,
								endpoint_type(
									this_type::is_v4()?
										protocol_type::v4()
										: protocol_type::v6(),
									0));

			return
				su.set_option(ops_setter)?
					_socket_cont.using_handler(
						boost::bind(
							&this_type::prv_handler_insert_socket_zwei<ops_setter_type>,
							this, _1, boost::ref(service), boost::addressof(su), boost::cref(ops_setter) ) )
					: socket_ptr_type();
		}
		else
		{
			return
				_socket_cont.using_handler(
					boost::bind(
						&this_type::prv_handler_insert_socket_zwei<ops_setter_type>,
						this, _1, boost::ref(service), static_cast<socket_unit_type*>(0), boost::cref(ops_setter) ) );
		}
	}

	inline void back(socket_ptr_type psocket)
	{
		_socket_cont.template get<0>().find(
			utility::cast_pointer_to_int::cast<socket_id_type>(psocket.get()),
			boost::bind(&this_type::prv_s_handler_back_socket, _1, _2));
	}

	template<typename OptionSetter> inline
	bool set_option(const OptionSetter& ops_setter)
	{
		typedef OptionSetter ops_setter_type;

		return
			_socket_cont.using_handler(
				boost::bind(
					&this_type::prv_s_handler_set_options<ops_setter_type>,
					this, _1, boost::cref(ops_setter) ) );
	}

	void close(void)
	{
		typedef typename socket_unit_container_type::base_type base_cont_type;
		typedef typename boost::multi_index::nth_index_iterator<base_cont_type, 0>::type iter_type;

		base_cont_type tmp;
		_socket_cont.swap(tmp);
		for(iter_type i = tmp.template get<0>().begin(),
					isize = tmp.template get<0>().end();
				i != isize; ++i)
		{
			const_cast<socket_unit_type&>(*i).close();
		}

	}

	inline void clear(void)
	{
		this_type::close();
	}

private:
	inline static bool is_v4(void)
	{
		return is_v4_type::value;
	}

	inline static bool is_v6(void)
	{
		return is_v6_type::value;
	}

private:
	socket_ptr_type prv_handler_insert_socket_eins(typename socket_unit_container_type::base_type& base,
													service_type& service,
													socket_unit_type* psu)
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 0>::type iter_0_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 1>::type iter_1_type;
		typedef std::pair<iter_0_type, bool> ins_rst_type;

		if((base.size() < _limit_size.load()) && psu)
		{
			socket_unit_type& su = *psu;
			socket_ptr_type psocket = su.get();
			ins_rst_type ins_rst = base.template get<0>().insert(boost::move(su));
			assert(ins_rst.second);
			return psocket;
		}
		else
		{
			assert(base.size());

			iter_1_type iter1 = base.template get<1>().begin();
			iter_0_type iter0 = base.template project<0>(iter1);

			socket_unit_type su(*(iter1));
			socket_ptr_type psocket = su.get();

			return
				base.template get<0>().replace(iter0, boost::move(su))?
					psocket : socket_ptr_type();
		}
	}

	template<typename OptionSetter> inline
	socket_ptr_type prv_handler_insert_socket_zwei(typename socket_unit_container_type::base_type& base,
													service_type& service,
													socket_unit_type* psu,
													const OptionSetter& ops_setter)
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 0>::type iter_0_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 1>::type iter_1_type;
		typedef std::pair<iter_0_type, bool> ins_rst_type;

		if((base.size() < _limit_size.load()) && psu)
		{
			socket_unit_type& su = *psu;
			socket_ptr_type psocket = su.get();
			ins_rst_type ins_rst = base.template get<0>().insert(boost::move(su));
			assert(ins_rst.second);
			return psocket;
		}
		else
		{
			assert(base.size());

			iter_1_type iter1 = base.template get<1>().begin();
			iter_0_type iter0 = base.template project<0>(iter1);

			socket_unit_type su(*(iter1));
			if(!su.set_option(ops_setter))
			{
				return socket_ptr_type();
			}

			socket_ptr_type psocket = su.get();

			return
				base.template get<0>().replace(iter0, boost::move(su))?
					psocket : socket_ptr_type();
		}
	}

	static void prv_s_handler_back_socket(typename socket_unit_container_type::base_type& base,
											boost::fusion::pair
											<
												typename
													safe_container::nth_index
													<
														socket_unit_container_type,
														0
													>::type::index_id_type,
												typename
													safe_container::nth_index
													<
														socket_unit_container_type,
														0
													>::type::iterator
											> rst)
	{
		if(rst.second == base.template get<0>().end())
		{
			return;
		}

		socket_unit_type su(*rst.second);
		su.back();

		base.template get<0>().replace(rst.second, boost::move(su));
	}

	template<typename OptionSetter>
	static bool prv_s_handler_set_options(typename socket_unit_container_type::base_type& base,
											const OptionSetter& ops_setter)
	{
		typedef typename socket_unit_container_type::base_type base_type;
		typedef typename boost::multi_index::nth_index_iterator<base_type, 0>::type iter_0_type;

		for(iter_0_type i = base.template get<0>().begin(), isize = base.template get<0>().end(); i != isize; ++i)
		{
			if(!(i->set_option(ops_setter)))
			{
				return false;
			}
		}

		return true;
	}

private:
	socket_unit_container_type _socket_cont;
	limit_size_type _limit_size;
};

} // namespace udp_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_UDP_HELPER_UDP_SOCKET_POOL_HPP__
