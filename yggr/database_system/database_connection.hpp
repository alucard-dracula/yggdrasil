//database_connection.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/nonable.hpp>
#include <yggr/support/operator_t.hpp>

#include <yggr/utility/cast_pointer_to_int.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/mplex/pointer_int.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/database_system/dtl_include_helper.hpp>
#include <yggr/database_system/database_connection_init.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/atomic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace database_system
{
namespace detail
{

class database_connection_container
{
public:
	typedef database_connection_init init_type;
	typedef dtl::DBConnection conn_type;
	typedef ::yggr::shared_ptr<conn_type> org_conn_ptr_type;
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef dtl::RootException org_root_err_type;
	typedef dtl::DBException org_err_type;

private:
	typedef database_connection_container this_type;

public:
	database_connection_container(void);
	database_connection_container(const init_type& init);

	database_connection_container(BOOST_RV_REF(this_type) right)
		: _pconn(boost::move(right._pconn)),
			_bpoped(boost::move(right._bpoped))
	{
	}

	database_connection_container(const this_type& right);
	~database_connection_container(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_pconn, boost::move(right._pconn));
		copy_or_move_or_swap(_bpoped, boost::move(right._bpoped));
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !!_pconn;
	}

	inline bool empty(void) const
	{
		return _pconn && !_bpoped;
	}

	inline bool is_poped(void) const
	{
		return _bpoped;
	}

	inline bool push(const org_conn_ptr_type& ptr)
	{
		if(!_bpoped || ptr != _pconn)
		{
			return false;
		}

		assert(_bpoped == true);
		_bpoped = !_bpoped;
		return true;
	}

	inline org_conn_ptr_type pop(void)
	{
		if(_bpoped || !_pconn)
		{
			return org_conn_ptr_type();
		}

		assert(_bpoped == false);
		_bpoped = !_bpoped;
		return _pconn;
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler)
	{
		return handler(_pconn, _bpoped);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		return handler(_pconn, _bpoped);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_any_handler(const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		assert(phandler);

		return (*phandler)(_pconn, _bpoped);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_any_handler_const(const basic_args_holder_nv_type& any_handler) const
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		assert(phandler);

		return (*phandler)(_pconn, _bpoped);
	}

private:
	org_conn_ptr_type _pconn;
	bool _bpoped;
};

} // namespace detail
} // namespace database_system
} // namespace yggr

namespace yggr
{
namespace database_system
{

class database_connection
		: private nonable::noncopyable,
			private nonable::nonmoveable
{
public:
	typedef mplex::pointer_int::type id_type;

	typedef dtl::DBConnection conn_type;
	typedef yggr::utf8_string inner_string_type;
	typedef dtl::tstring org_string_type;

	typedef database_connection_init init_type;

	typedef ::yggr::shared_ptr<conn_type> org_conn_ptr_type;

private:
	typedef dtl::RootException org_root_err_type;
	typedef dtl::DBException org_err_type;

private:
	typedef safe_container::safe_wrap<init_type> init_wrap_type;

public:
	typedef init_wrap_type::error_type empty_conn_err_type;
	typedef org_root_err_type db_root_err_type_type;
	typedef org_err_type db_err_type;

private:
	typedef detail::database_connection_container connection_container_type;
	typedef safe_container::safe_wrap<connection_container_type> container_wrap_type;

private:
	typedef database_connection this_type;

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	database_connection(void);

	template<typename String>
	database_connection(const String& str_dsn,
							bool bprompt = false,
							bool bauto_commit = false,
							typename
								boost::enable_if
								<
									charset::utf8_string_constructable<String>,
									sfinae_type
								>::type sfinae = 0)
		: _pconn_cont_wrap(connection_container_type(init_type(str_dsn, bprompt, bauto_commit)))
	{
	}

	database_connection(const init_type& init);
	~database_connection(void);

public:
	void swap(this_type& right);

public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		try
		{
			return _pconn_cont_wrap.call_function(&connection_container_type::operator bool);
		}
		catch(const empty_conn_err_type&)
		{
			return false;
		}
	}

	inline bool empty(void) const
	{
		try
		{
			return _pconn_cont_wrap.call_function(&connection_container_type::empty);
		}
		catch(const empty_conn_err_type&)
		{
			return true;
		}
	}

	inline bool is_poped(void) const
	{
		try
		{
			return _pconn_cont_wrap.call_function(&connection_container_type::is_poped);
		}
		catch(const empty_conn_err_type&)
		{
			return false;
		}
	}

	inline id_type id(void) const
	{
		return utility::cast_pointer_to_int::cast<id_type>(this);
	}

public:
	inline void clear(void)
	{
		_pconn_cont_wrap.clear();
	}

	inline bool push(const org_conn_ptr_type& ptr)
	{
		try
		{
			return _pconn_cont_wrap.call_function(&connection_container_type::push, ptr);
		}
		catch(const empty_conn_err_type&)
		{
			return false;
		}
	}

	inline org_conn_ptr_type pop(void)
	{
		try
		{
			return _pconn_cont_wrap.call_function(&connection_container_type::pop);
		}
		catch(const empty_conn_err_type&)
		{
			return org_conn_ptr_type();
		}
	}

	inline bool init(const init_type& init)
	{
		return
			_pconn_cont_wrap.locked_unsafe_using_handler(
				boost::bind(&this_type::prv_s_handler_init, _1, boost::cref(init)));
	}

public:
	//throws empty_conn_err_type, db_err_type, db_root_err_type_type
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		handler_holder_type handler_holder(handler);
		return
			_pconn_cont_wrap.call_function(
				&connection_container_type::using_any_handler<handler_type>,
				utility::args_holder_trans(handler_holder) );
	}

	//throws empty_conn_err_type, db_err_type, db_root_err_type_type
	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv<const handler_type> handler_holder_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		handler_holder_type handler_holder(handler);
		return
			_pconn_cont_wrap.call_function(
				&connection_container_type::using_any_handler_const<handler_type>,
				utility::args_holder_trans(handler_holder));
	}

private:
	inline static bool prv_s_handler_init(container_wrap_type::base_type& base, const init_type& init)
	{
		container_wrap_type::base_type new_base =
			construct_shared<connection_container_type>(yggr_nothrow_new connection_container_type(init));
		return new_base && (base.swap(new_base), true);
	}

private:
	container_wrap_type _pconn_cont_wrap;
};

} // namespace database_system
} // namespace yggr

namespace yggr
{
namespace database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(database_connection)
} // namespace swap_support

using swap_support::swap;

} // namespace database_system
} // namespace yggr

namespace std
{
	using ::yggr::database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::database_system::swap_support::swap;
} // namespace boost

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_HPP__
