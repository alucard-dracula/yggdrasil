//client_passive_udp_start_mode.hpp

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

#ifndef __YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_UDP_START_MODE_HPP__
#define __YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_UDP_START_MODE_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/hn_conv.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#include <boost/asio.hpp>
#include <boost/type_traits/is_same.hpp>

#include <cassert>

namespace yggr
{
namespace client
{
namespace start_mode
{

template<
			typename Recv_Packet,
			typename Start_Data_Generator,
			typename Session_Creator
		>
struct client_passive_udp_start_mode
{
public:
	typedef Recv_Packet recv_packet_type;
	typedef Start_Data_Generator start_data_generator;
	typedef Session_Creator session_creator_type;

private:
	typedef typename start_data_generator::start_data_type start_data_type;

private:
	typedef client_passive_udp_start_mode this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	client_passive_udp_start_mode(void)
	{
		// compile need this function
		// if stop here please check you codes, and this class suport only udp
		assert(false);
	}

	client_passive_udp_start_mode(BOOST_RV_REF(recv_packet_type) pak)
		: _pak(boost::move(pak))
	{
	}

	client_passive_udp_start_mode(const recv_packet_type& pak)
		: _pak(pak)
	{
	}

	client_passive_udp_start_mode(BOOST_RV_REF(this_type) right)
		: _pak(boost::move(right._pak))
	{
	}

	client_passive_udp_start_mode(const this_type& right)
		: _pak(right._pak)
	{
	}

	~client_passive_udp_start_mode(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_pak, boost::move(right._pak));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}
		_pak = right._pak;
		return *this;
	}

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
		_pak.swap(right._pak);
	}

public:
	template<typename ConnPtr, typename Session_Mgr> inline
	void operator()(ConnPtr pconn, Session_Mgr& smgr) const
	{
		//this_type::prv_create_helper(pconn, smgr.this_type::shared_from_this(), _pak);
		this_type::prv_create_helper(pconn, smgr.shared_from_this(), _pak);
	}

private:

	template<typename ConnPtr, typename Session_Mgr_Ptr> inline
	static void prv_create_helper(ConnPtr pconn, Session_Mgr_Ptr psmgr, recv_packet_type& pak)
	{
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::tag_type session_mgr_tag_type;
		typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
		typedef typename session_mgr_type::session_ptr_type session_ptr_type;


		BOOST_MPL_ASSERT((
							boost::is_same
							<
								session_mgr_tag_type,
								yggr::network::type_traits::tag_client
							>
						));
		
		assert(psmgr);
		if(!(pconn && psmgr) || (pak.empty()))
		{
			return;
		}

		start_data_generator gener;
		start_data_type start_data(gener(pak));

		pak.owner_info().set_now_owner_id(start_data.id());

		session_ptr_type sptr = psmgr->create_session_only(boost::system::error_code(), session_creator_type(pconn, start_data));

		if(!sptr)
		{
			return;
		}

		sptr->start();
		psmgr->direct_send_to_adapter(pak);
	}

private:
	mutable recv_packet_type _pak;
};

} // namespce start_mode
} // namespace client
} // namespace yggr

namespace yggr
{
namespace client
{
namespace start_mode
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline 
void swap(client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, 
			client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) 
{ 
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap; 

} // namespace start_mode
} // namespace client
} // namespace yggr

namespace std
{
	using ::yggr::client::start_mode::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::client::start_mode::swap_support::swap;
} // namespace boost

#endif //__YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_UDP_START_MODE_HPP__
