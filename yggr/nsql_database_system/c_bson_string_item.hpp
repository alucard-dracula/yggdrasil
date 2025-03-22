//c_bson_string_item.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_STRING_ITEM_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_STRING_ITEM_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_string_item
{
public:
	typedef utf8_string string_type;

private:
	typedef c_bson_string_item this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

protected:
	c_bson_string_item(void);

	template<typename String>
	c_bson_string_item(const String& str,
						typename
							boost::enable_if
							<
								charset::is_convertible_utf8_string< String >,
								sfinae_type
							>::type sfinae = 0)
		: _str(str)
	{
	}

	c_bson_string_item(BOOST_RV_REF(string_type) str)
		: _str(boost::move(str))
	{
	}

	c_bson_string_item(const string_type& str)
		: _str(str)
	{
	}

	c_bson_string_item(BOOST_RV_REF(this_type) right)
		: _str(boost::move(right._str))
	{
	}

	c_bson_string_item(const this_type& right);
	~c_bson_string_item(void);

public:
	template<typename Char>
	typename boost::enable_if<is_native_char<Char>, this_type&>::type
		operator=(const Char* right)
	{
		_str = right;
		return *this;
	}

	template<typename String> inline
	typename boost::enable_if<charset::is_convertible_utf8_string< String >, this_type&>::type
		operator=(const String& right)
	{
		_str = right;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(string_type) right)
	{
		copy_or_move_or_swap(_str, boost::move(right));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_str, boost::move(right._str));

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
	inline bool compare_eq(const this_type& right) const
	{
		return this == boost::addressof(right) || _str == right._str;
	}

public:
	inline operator string_type&(void)
	{
		return _str;
	}

	inline operator const string_type&(void) const
	{
		return _str;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> ret_string_type;
		BOOST_MPL_ASSERT((charset::is_string_t<ret_string_type>));

		return _str.str<ret_string_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type));
	}

public:
	inline string_type& str(void)
	{
		return _str;
	}

	// inline const string_type& str(void) const;
	
	// template<typename ...Args> inline 
	// const string_type& str(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String str(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, _str, str, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, _str, str, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(string_type, _str, str)

protected:
	string_type _str;
};

// non member operator foo
inline bool operator==(const c_bson_string_item& l, const c_bson_string_item& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_bson_string_item& l, const c_bson_string_item& r)
{
	return !l.compare_eq(r);
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_bson_string_item)
} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_STRING_ITEM_HPP__
