//client_passive_udp_start_mode.hpp

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

#ifndef __YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_UDP_START_MODE_HPP__
#define __YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_UDP_START_MODE_HPP__

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/hn_conv.hpp>

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
		: _pak(boost::forward<recv_packet_type>(pak))
	{
	}

	client_passive_udp_start_mode(const recv_packet_type& pak)
		: _pak(pak)
	{
	}

	client_passive_udp_start_mode(BOOST_RV_REF(this_type) right)
		: _pak(boost::forward<recv_packet_type>(right._pak))
	{
	}

	client_passive_udp_start_mode(const this_type& right)
		: _pak(right._pak)
	{
	}

	~client_passive_udp_start_mode(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		_pak = boost::forward<recv_packet_type>(right._pak);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_pak = right._pak;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
		//_pak.swap(boost::forward<recv_packet_type>(right._pak));
		_pak.swap(right._pak);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_pak.swap(right._pak);
	}

	template<typename ConnPtr, typename Session_Mgr>
	void operator()(ConnPtr pconn, Session_Mgr& smgr)
	{
		this_type::prv_create_helper(pconn, smgr.shared_from_this(), _pak);
	}

private:

	template<typename ConnPtr, typename Session_Mgr_Ptr>
	static void prv_create_helper(ConnPtr pconn, Session_Mgr_Ptr psmgr, recv_packet_type& pak)
	{
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
		if(!(pconn && psmgr) || pak.empty())
		{
			return;
		}

		start_data_generator gener;
		start_data_type start_data(gener(pak));

		pak.owner_info().set_now_owner_id(start_data.id());

		psmgr->handler_create_and_fix_start_packet(boost::system::error_code(), session_creator_type(pconn, start_data), pak);
	}

private:
	recv_packet_type _pak;
};

} // namespce start_mode
} // namespace client
} // namespace yggr

#define _YGGR_PP_CLIENT_START_MODE_SWAP() \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
	void swap(yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, \
				yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) { \
		l.swap(r); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
	void swap(BOOST_RV_REF(yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) l, \
				yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) { \
		typedef yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)> now_this_type; \
		r.swap(l); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
	void swap(yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, \
				BOOST_RV_REF(yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) r) { \
		typedef yggr::client::start_mode::client_passive_udp_start_mode<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)> now_this_type; \
		l.swap(r); }


namespace std
{
	_YGGR_PP_CLIENT_START_MODE_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_CLIENT_START_MODE_SWAP()
} // namespace boost

#undef _YGGR_PP_CLIENT_START_MODE_SWAP

#endif //__YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_UDP_START_MODE_HPP__
