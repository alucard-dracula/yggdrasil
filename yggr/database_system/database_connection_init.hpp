//database_connection_init.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_INIT_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_INIT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/database_system/dtl_include_helper.hpp>
#include <yggr/database_system/database_string_helper.hpp>

namespace yggr
{
namespace database_system
{

class database_connection_init
{
public:
	typedef dtl::tstring org_string_type;
	typedef charset::utf8_string inner_string_type;

private:
	typedef database_connection_init this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	database_connection_init(void);

	template<typename String>
	database_connection_init(const String& str_dsn, 
								bool bprompt = false, 
								bool bauto_commit = false,
								typename 
									boost::enable_if
									<
										charset::utf8_string_constructable<String>,
										sfinae_type
									>::type sfinae = 0)
		: _dsn(database_string_helper::s_conv(
					charset::string_charset_helper_first(str_dsn), 
					charset::string_charset_helper_second(str_dsn))), 
			_bprompt(bprompt), _bauto_commit(bauto_commit)
	{
	}

	database_connection_init(BOOST_RV_REF(this_type) right)
		: _dsn(boost::move(right._dsn)),
			_bprompt(boost::move(right._bprompt)), 
			_bauto_commit(boost::move(right._bauto_commit))
	{
	}

	database_connection_init(const this_type& right);
	~database_connection_init(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_dsn, boost::move(right._dsn));
		copy_or_move_or_swap(_bprompt, boost::move(right._bprompt));
		copy_or_move_or_swap(_bauto_commit, boost::move(right._bauto_commit));
		return *this;
	}

	this_type& operator=(const this_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_string_type& dsn_string(void)
	{
		return _dsn;
	}

	inline const org_string_type& dsn_string(void) const
	{
		return _dsn;
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, org_string_type>::type
		dsn_string(const Alloc& alloc) const
	{
		return org_string_type(boost::begin(_dsn), boost::end(_dsn), alloc);
	}

	template<typename String> inline
	String dsn_string(const string& charset_name 
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		return charset::string_converter::s_conv<ret_string_type>(
				_dsn, YGGR_STR_DATABASE_CHARSET_NAME(), charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, org_string_type>::type
		dsn_string(const Alloc& alloc,
					const string& charset_name
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		return charset::string_converter::s_conv<ret_string_type>(
				_dsn, alloc, YGGR_STR_DATABASE_CHARSET_NAME(), charset_name);
	}

	inline bool& prompt(void)
	{
		return _bprompt;
	}

	inline bool prompt(void) const
	{
		return _bprompt;
	}

	inline bool& auto_commit(void)
	{
		return _bauto_commit;
	}

	inline bool auto_commit(void) const
	{
		return _bauto_commit;
	}

private:
	org_string_type _dsn;
	bool _bprompt;
	bool _bauto_commit;
};

} // database_system
} // namespace yggr

namespace yggr
{
namespace database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(database_connection_init)
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

#endif // __YGGR_DATABASE_SYSTEM_DATABASE_CONNECTION_INIT_HPP__
