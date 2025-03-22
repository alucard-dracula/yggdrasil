//database_connection_pool.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_POOL_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_POOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/support/operator_t.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_queue.hpp>

#include <yggr/database_system/dtl_include_helper.hpp>
#include <yggr/database_system/database_charset_config.hpp>
#include <yggr/database_system/database_connection.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/thread/shared_mutex.hpp>


namespace yggr
{
namespace database_system
{

class database_connection_pool
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef dtl::DBConnection conn_type;
	typedef yggr::utf8_string inner_string_type;
	typedef dtl::tstring org_string_type;

	typedef database_connection_init init_type;

	typedef ::yggr::shared_ptr<conn_type> org_conn_ptr_type;
	typedef database_connection connection_type;

private:
	typedef dtl::RootException org_root_err_type;
	typedef dtl::DBException org_err_type;

private:
	typedef safe_container::safe_wrap<init_type, boost::shared_mutex> init_wrap_type;

private:
	typedef safe_container::safe_queue<org_conn_ptr_type> conn_queue_type;

private:
	typedef database_connection_pool this_type;

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	database_connection_pool(void);

	template<typename String>
	database_connection_pool(const String& str_dsn, 
								bool bprompt = false, 
								bool bauto_commit = false,
								typename 
									boost::enable_if
									<
										charset::utf8_string_constructable<String>,
										sfinae_type
									>::type sfinae = 0)
		: _init_wrap(init_type(str_dsn, bprompt, bauto_commit))
	{
	}

	database_connection_pool(const init_type& init);

	~database_connection_pool(void);

public:
	//----------------------------------------------------------
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !_init_wrap.empty();
	}

	// don't use swap
	//void swap(this_type& right);

	inline void clear(void)
	{
		_conn_queue.clear();
	}

	inline bool empty(void) const
	{
		return _init_wrap.empty();
	}

	inline bool is_poped(void) const
	{
		return false;
	}

	inline bool push(const org_conn_ptr_type& ptr)
	{
		return this_type::prv_push_enable(ptr) && (_conn_queue.push(ptr), true);
	}

	org_conn_ptr_type pop(void);

	inline bool gen_connection(connection_type& conn) const
	{
		init_type init;
		try
		{
			init = _init_wrap.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return false;
		}

		conn.init(init);

		return true;
	}

	inline org_string_type dns_string(void) const
	{
		init_type init;
		try
		{
			init = _init_wrap.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return org_string_type();
		}

		return init.dsn_string();
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, org_string_type>::type
		dns_string(const Alloc& alloc) const
	{
		init_type init;
		try
		{
			init = _init_wrap.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return org_string_type();
		}

		return init.dsn_string(alloc);
	}

	template<typename String> inline
	String dns_string(const string& charset_name = charset::charset_name_t<String>()) const
	{
		typedef String now_string_type;

		init_type init;
		try
		{
			init = _init_wrap.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return now_string_type();
		}

		return 
			charset::string_converter::s_conv<now_string_type>(
				init.dsn_string(), YGGR_STR_DATABASE_CHARSET_NAME(), charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		dns_string(const Alloc& alloc,
					const string& charset_name
						= charset::charset_name_t<String>()) const
	{
		typedef String now_string_type;

		init_type init;
		try
		{
			init = _init_wrap.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return now_string_type(alloc);
		}

		return 
			charset::string_converter::s_conv<now_string_type>(
				init.dsn_string(), alloc, YGGR_STR_DATABASE_CHARSET_NAME(), charset_name);
	}

	inline init_type connect_info(void) const
	{
		try
		{
			return _init_wrap.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return init_type();
		}
	}

private:
	inline bool prv_push_enable(const org_conn_ptr_type& ptr) const
	{
		if(!ptr)
		{
			return false;
		}

		try
		{
			return _init_wrap.using_handler(boost::bind(&this_type::prv_handler_push_enable, this, _1, ptr));
		}
		catch(const init_wrap_type::error_type&)
		{
			return false;
		}
	}

	inline bool prv_handler_push_enable(const init_wrap_type::base_type& base, 
											const org_conn_ptr_type& ptr) const
	{
		assert(base);
		assert(ptr);

		const init_wrap_type::value_type& val_cref = *base;
		return 
			val_cref.dsn_string() == ptr->GetDSN()
				&& val_cref.auto_commit() == ptr->GetAutoCommit();
	}
		
private:
	init_wrap_type _init_wrap;
	conn_queue_type _conn_queue;
};

} // namespace database_system
} // namespace yggr

#endif // __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_POOL_HPP__
