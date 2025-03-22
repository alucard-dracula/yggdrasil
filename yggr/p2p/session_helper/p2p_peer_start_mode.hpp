//p2p_peer_start_mode.hpp

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

#ifndef __YGGR_P2P_SESSION_HELPER_P2P_PEER_START_MODE_HPP__
#define __YGGR_P2P_SESSION_HELPER_P2P_PEER_START_MODE_HPP__

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/hn_conv.hpp>

#include <boost/type_traits/is_same.hpp>
#include <cassert>

namespace yggr
{
namespace p2p
{

template<
			typename Start_Data,
			typename Session_Creator
		>
struct p2p_peer_start_mode
{
public:
	typedef Start_Data start_data_type;
	typedef Session_Creator session_creator_type;

private:
	typedef p2p_peer_start_mode this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	p2p_peer_start_mode(void)
	{
		// compile need this function
		// if stop here please check you codes, and this class suport only p2p
		assert(false);
	}

	p2p_peer_start_mode(BOOST_RV_REF(start_data_type) start_data)
		: _start_data(boost::move(start_data))
	{
	}

	p2p_peer_start_mode(const start_data_type& start_data)
		: _start_data(start_data)
	{
	}

	p2p_peer_start_mode(BOOST_RV_REF(this_type) right)
		: _start_data(boost::move(right._start_data))
	{
	}

	p2p_peer_start_mode(const this_type& right)
		: _start_data(right._start_data)
	{
	}

	~p2p_peer_start_mode(void)
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

		copy_or_move_or_swap(_start_data, boost::move(right._start_data));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}
		_start_data = right._start_data;
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

		yggr::swap(_start_data, right._start_data);
	}

public:
	template<typename ConnPtr, typename Session_Mgr> inline
	void operator()(ConnPtr pconn, Session_Mgr& smgr)
	{
		//this_type::prv_s_create_helper(pconn, smgr.this_type::shared_from_this(), _start_data);
		this_type::prv_s_create_helper(pconn, smgr.shared_from_this(), _start_data);
	}

private:

	template<typename ConnPtr, typename Session_Mgr_Ptr> inline
	static void prv_s_create_helper(ConnPtr pconn, Session_Mgr_Ptr psmgr, start_data_type& start_data)
	{
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
		typedef typename session_mgr_type::session_ptr_type session_ptr_type;

		assert(psmgr);
		if(!(pconn && psmgr) || start_data.empty())
		{
			return;
		}

		session_ptr_type sptr = psmgr->create_session_only(boost::system::error_code(), session_creator_type(pconn, start_data));
		if(!sptr)
		{
			return;
		}

		sptr->start();
	}

private:
	start_data_type _start_data;
};

} // namespace p2p
} // namespace yggr

namespace yggr
{
namespace p2p
{
namespace swap_support
{

template< typename Start_Data, typename Session_Creator > inline 
void swap(p2p_peer_start_mode< Start_Data, Session_Creator >& l, 
			p2p_peer_start_mode< Start_Data, Session_Creator >& r) 
{ 
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace p2p
} // namespace yggr

namespace std
{
	using ::yggr::p2p::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::p2p::swap_support::swap;
} // namespace boost


#endif //__YGGR_P2P_SESSION_HELPER_P2P_PEER_START_MODE_HPP__
