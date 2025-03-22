//log_file_op_caller.hpp

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

#ifndef __YGGR_LOG_LOG_FILE_OP_CALLER_HPP__
#define __YGGR_LOG_LOG_FILE_OP_CALLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/ref.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <algorithm>

namespace yggr
{
namespace log
{

class log_file_op_caller
{
public:

#ifdef YGGR_AT_WINDOWS
	typedef yggr::string fname_string_type;
#else
	typedef yggr::utf8_string fname_string_type;
#endif // YGGR_AT_WINDOWS

	typedef fname_string_type id_type;

private:
	typedef safe_container::safe_wrap<fname_string_type> fname_string_wrap_type;

private:
	typedef log_file_op_caller this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	log_file_op_caller(void);

	template<typename Char>
	log_file_op_caller(const Char* fname)
		: _fname_wrap(charset::string_converter::s_conv<fname_string_type>(fname))
	{
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	log_file_op_caller(const Basic_String<Char, Traits, Alloc>& fname)
		: _fname_wrap(charset::string_converter::s_conv<fname_string_type>(fname))
	{
	}

	log_file_op_caller(const fname_string_type& fname);

	log_file_op_caller(BOOST_RV_REF(this_type) right)
		: _fname_wrap(boost::move(right._fname_wrap))
	{
	}

	log_file_op_caller(const this_type& right);
	~log_file_op_caller(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_fname_wrap, boost::move(right._fname_wrap));
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
	inline id_type id(void) const
	{
		return this_type::prv_get_fname();
	}

	inline bool empty(void) const
	{
		return _fname_wrap.empty();
	}

	//call_append

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args> inline
	bool call_append( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.append(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Op \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool call_append( Op& op \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return op.append( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// call_append_any

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args> inline
	bool call_append_any( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.append_any(boost::forward<Args>(args)...);
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Op \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	bool call_append_any( Op& op \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return op.append_any( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// call_clear
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args> inline
	bool call_clear( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.clear(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Op \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool call_clear( Op& op \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return op.clear( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args > inline
	bool call_clear_any( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.clear_any(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Op \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool call_clear_any( Op& op \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return op.clear_any( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args> inline
	bool call_search( Op& op, BOOST_FWD_REF(Args)... args) const
	{
		return op.search(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Op \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool call_search( Op& op \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return op.search( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Op, typename ...Args > inline
	bool call_search_any( Op& op , BOOST_FWD_REF(Args)... args) const
	{
		return op.search(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Op \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool call_search_any( Op& op \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return op.search( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	inline fname_string_type prv_get_fname(void) const
	{
		try
		{
			return _fname_wrap.load();
		}
		catch(const fname_string_wrap_type::error_type&)
		{
			return fname_string_type();
		}
	}

private:
	fname_string_wrap_type _fname_wrap;
};

// non_member_foo

// operator==
inline bool operator==(const log_file_op_caller::id_type& l,
						const log_file_op_caller& r)
{
	return l == r.id();
}

inline bool operator==(const log_file_op_caller& l,
						const log_file_op_caller::id_type& r)
{
	return l.id() == r;
}

inline bool operator==(const log_file_op_caller& l,
						const log_file_op_caller& r)
{
	return (boost::addressof(l) == boost::addressof(r)) || (l.id() == r.id());
}

// operator!=
inline bool operator!=(const log_file_op_caller::id_type& l,
						const log_file_op_caller& r)
{
	return !(l == r.id());
}

inline bool operator!=(const log_file_op_caller& l,
						const log_file_op_caller::id_type& r)
{
	return !(l.id() == r);
}

inline bool operator!=(const log_file_op_caller& l,
						const log_file_op_caller& r)
{
	return !((boost::addressof(l) == boost::addressof(r)) || (l.id() == r.id()));
}

} // namespace log
} // namespace yggr

namespace yggr
{
namespace log
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(log_file_op_caller)
} // namespace swap_support

using swap_support::swap;

} // namespace log
} // namespace yggr

namespace std
{
	using ::yggr::log::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::log::swap_support::swap;
} // namespace boost

#endif // __YGGR_LOG_LOG_FILE_OP_CALLER_HPP__
