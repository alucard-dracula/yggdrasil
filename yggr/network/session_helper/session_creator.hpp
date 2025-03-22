//session_creator.hpp

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

#ifndef __YGGR_NETWOR_SESSION_HELPER_SESSION_CREATOR_HPP__
#define __YGGR_NETWOR_SESSION_HELPER_SESSION_CREATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#include <yggr/network/session_helper/session_id_creator.hpp>
#include <yggr/network/session_helper/session_checker_creator.hpp>
#include <yggr/network/session_helper/session_crypher_creator.hpp>
#include <yggr/network/session_helper/options_setter.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename Start_Data, typename Conn,
			typename ID_Creator = session_id_creator<Start_Data>,
			typename Checker_Creator = session_checker_creator<Start_Data>,
			typename Crypher_Creator = session_crypher_creator<Start_Data>,
			typename Options_Setter = default_options_setter_type >
class session_creator
{
public:
	typedef Start_Data start_data_type;
	typedef Conn conn_type;
	typedef ::yggr::shared_ptr<conn_type> conn_ptr_type;
	typedef ID_Creator id_creator_type;
	typedef Checker_Creator checker_creator_type;
	typedef Crypher_Creator crypher_creator_type;
	typedef Options_Setter options_setter_type;

private:
	typedef session_creator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	session_creator(const conn_ptr_type& pconn, BOOST_RV_REF(start_data_type) start_data)
		: _pconn(pconn), _start_data(boost::move(start_data))
	{
	}

	session_creator(const conn_ptr_type& pconn, const start_data_type& start_data)
		: _pconn(pconn), _start_data(start_data)
	{
	}

	session_creator(BOOST_RV_REF(this_type) right)
		: _pconn(boost::move(right._pconn)), 
			_start_data(boost::move(right._start_data))
	{
	}

	session_creator(const this_type& right)
		: _pconn(right._pconn), _start_data(right._start_data)
	{
	}

	~session_creator(void)
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
		
		copy_or_move_or_swap(_pconn, boost::move(right._pconn));
		copy_or_move_or_swap(_start_data, boost::move(right._start_data));
		
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_pconn = right._pconn;
		_start_data = right._start_data;
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

		_pconn.swap(right._pconn);
		::yggr::swap(_start_data, right._start_data);
	}

public:
	template<typename SessionPtr, typename AdapterMgrPtr>
	SessionPtr create(const AdapterMgrPtr& padp_mgr) const
	{
		typedef SessionPtr session_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_ptr_type>::type session_type;
		typedef typename session_type::conn_ptr_type now_conn_ptr_type;

		typedef typename id_creator_type::result_type id_result_type;

		BOOST_MPL_ASSERT((boost::is_same<conn_ptr_type, now_conn_ptr_type>));

		static checker_creator_type chc;
		static crypher_creator_type crc;
		static options_setter_type opt_setter;

		if(!_pconn)
		{
			return session_ptr_type();
		}

		id_creator_type idc;

		id_result_type sid = idc(_start_data);
		if(!sid)
		{
			_pconn->close();
			return session_ptr_type();
		}

		session_ptr_type sptr;
		try
		{
			sptr = 
				session_ptr_type(
					yggr_nothrow_new 
						session_type(
							sid, _pconn, padp_mgr,
							chc(_start_data), crc(_start_data),
							_pconn->local_endpoint(),
							_pconn->localhost_endpoint(),
							_pconn->remote_endpoint() ) );

		}
		catch(...)
		{
			_pconn->close();
			return session_ptr_type();
		}

		return sptr && opt_setter(*sptr)? sptr : (sptr->close(), session_ptr_type());
	}

	inline void cancel(void) const
	{
		if(!_pconn)
		{
			return;
		}

		_pconn->close();
	}

	inline conn_ptr_type get(void) const
	{
		return _pconn;
	}

	inline start_data_type& start_data(void)
	{
		return _start_data;
	}

	inline const start_data_type& start_data(void) const
	{
		return _start_data;
	}

private:
	mutable conn_ptr_type _pconn;
	start_data_type _start_data;
};

} // namespace session_helper
} // namespace network
} // namespace yggr


namespace yggr
{
namespace network
{
namespace session_helper
{
namespace swap_support
{

template<typename T1, typename T2, typename T3, typename T4, typename T5> inline
void swap(session_creator<T1, T2, T3, T4, T5>& l, 
			session_creator<T1, T2, T3, T4, T5>& r)
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace session_helper
} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::session_helper::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::session_helper::swap_support::swap;
} // namespace boost

#endif // __YGGR_NETWOR_SESSION_HELPER_SESSION_CREATOR_HPP__
