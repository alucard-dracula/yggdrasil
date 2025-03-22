//charset_foo.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_FOO_HPP__
#define __YGGR_CHARSET_CHARSET_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/charset_error_def.hpp>
#include <yggr/charset/string_decl.hpp>
#include <yggr/charset/charset_converter.hpp>
#include <yggr/charset/string_charset_helper.hpp>
#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/is_isomeric_same_string.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/get_allocator.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/range/functions.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <algorithm>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename TSrc, typename TDst, bool is_isomeric_same>
struct charset_foo_call_t_allocator_patch;

template<typename TSrc, typename TDst>
struct charset_foo_call_t_allocator_patch<TSrc, TDst, false>
{
	typedef TSrc src_type;
	typedef TDst dst_type;

	typedef dst_type result_type;
	typedef result_type ref_result_type;
	typedef result_type cref_result_type;

	inline result_type operator()(const src_type& src) const
	{
		typedef typename src_type::value_type src_char_type;
		typedef typename dst_type::value_type dst_char_type;
		typedef typename dst_type::size_type dst_size_type;

		size_type byte_size = boost::size(src) * sizeof(src_char_type);
		size_type dst_size = (byte_size + sizeof(dst_char_type) - 1) / sizeof(dst_char_type);
		dst_type dst(dst_size, src_char_type(0));
		if(byte_size) memcpy(&dst[0], &src[0], byte_size);

		return dst;
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, result_type>::type
		operator()(const src_type& src, const Alloc& alloc) const
	{
		typedef typename src_type::value_type src_char_type;
		typedef typename dst_type::value_type dst_char_type;
		typedef typename dst_type::size_type dst_size_type;

		size_type byte_size = boost::size(src) * sizeof(src_char_type);
		size_type dst_size = (byte_size + sizeof(dst_char_type) - 1) / sizeof(dst_char_type);
		dst_type dst(dst_size, 0, alloc);
		if(byte_size) memcpy(&dst[0], &src[0], byte_size);

		return dst;
	}

	inline result_type operator()(const src_type& src, size_type pos, size_type len) const
	{
		typedef typename src_type::value_type src_char_type;
		typedef typename dst_type::value_type dst_char_type;
		typedef typename dst_type::size_type dst_size_type;

		size_type str_size = boost::size(src);
		if(!(pos < str_size && len))
		{
			return result_type();
		}
		else
		{
			len = (std::min)(str_size - pos, len);
			size_type byte_size = len * sizeof(src_char_type);
			size_type dst_size = (byte_size + sizeof(dst_char_type) - 1) / sizeof(dst_char_type);
			dst_type dst(dst_size, 0);
			if(byte_size) memcpy(&dst[pos], &src[0], byte_size);

			return dst;
		}
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, result_type>::type
		operator()(const src_type& src, size_type pos, size_type len, const Alloc& alloc) const
	{
		typedef typename src_type::value_type src_char_type;
		typedef typename dst_type::value_type dst_char_type;
		typedef typename dst_type::size_type dst_size_type;

		size_type str_size = boost::size(src);
		if(!(pos < str_size && len))
		{
			return result_type(alloc);
		}
		else
		{
			len = (std::min)(str_size - pos, len);
			size_type byte_size = len * sizeof(src_char_type);
			size_type dst_size = (byte_size + sizeof(dst_char_type) - 1) / sizeof(dst_char_type);
			dst_type dst(dst_size, 0, alloc);
			if(byte_size) memcpy(&dst[pos], &src[0], byte_size);

			return dst;
		}
	}
};

template<typename TSrc, typename TDst>
struct charset_foo_call_t_allocator_patch<TSrc, TDst, true>
{
	typedef TSrc src_type;
	typedef TDst dst_type;

	typedef const dst_type& result_type;
	typedef dst_type& ref_result_type;
	typedef result_type cref_result_type;

	inline ref_result_type operator()(src_type& src) const
	{
		return reinterpret_cast<ref_result_type>(src);
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, ref_result_type>::type
		operator()(src_type& src, const Alloc&) const
	{
		return reinterpret_cast<ref_result_type>(src);
	}

	inline result_type operator()(const src_type& src) const
	{
		return reinterpret_cast<result_type>(src);
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, result_type>::type
		operator()(const src_type& src, const Alloc&) const
	{
		return reinterpret_cast<result_type>(src);
	}

	inline dst_type operator()(const src_type& src, size_type pos, size_type len) const
	{
		size_type str_size = boost::size(src);
		return 
			(pos < str_size && len)?
				dst_type(
					boost::begin(src) + pos, 
					boost::begin(src) + (pos + (std::min)(str_size - pos, len)),
					src.get_allocator())
				: dst_type(src.get_allocator());

	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, dst_type>::type
		operator()(const src_type& src, size_type pos, size_type len, const Alloc& alloc) const
	{
		size_type str_size = boost::size(src);

		return 
			(pos < str_size && len)?
				dst_type(
					boost::begin(src) + pos, 
					boost::begin(src) + (pos + (std::min)(str_size - pos, len)),
					alloc)
				: dst_type(alloc);
	}
};

template<typename TSrc, typename TDst>
struct charset_foo_call_t
	: public 
		charset_foo_call_t_allocator_patch
		<
			TSrc,
			TDst,
			is_isomeric_same_string<TSrc, TDst>::value
		>
{
protected:
	typedef 
		charset_foo_call_t_allocator_patch
		<
			TSrc,
			TDst,
			is_isomeric_same_string<TSrc, TDst>::value
		> base_type;

public:
	typedef typename base_type::src_type src_type;
	typedef typename base_type::dst_type dst_type;

	typedef typename base_type::result_type result_type;
	typedef typename base_type::ref_result_type ref_result_type;
	typedef typename base_type::cref_result_type cref_result_type;

public:
	using base_type::operator();

};

template<typename TSrc>
struct charset_foo_call_t<TSrc, TSrc>
{
	typedef TSrc src_type;
	typedef src_type dst_type;

	typedef const dst_type& result_type;
	typedef dst_type& ref_result_type;
	typedef result_type cref_result_type;

	inline ref_result_type operator()(src_type& src) const
	{
		return src;
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, ref_result_type>::type
		operator()(src_type& src, const Alloc&) const
	{
		return src;
	}

	inline result_type operator()(const src_type& src) const
	{
		return src;
	}


	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, result_type>::type
		operator()(const src_type& src, const Alloc&) const
	{
		return src;
	}

	inline dst_type operator()(const src_type& src, size_type pos, size_type len) const
	{
		size_type str_size = boost::size(src);
		return 
			(pos < str_size && len)?
				dst_type(
					boost::begin(src) + pos, 
					boost::begin(src) + (pos + (std::min)(str_size - pos, len)),
					src.get_allocator())
				: dst_type(src.get_allocator());

	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, dst_type>::type
		operator()(const src_type& src, size_type pos, size_type len, const Alloc& alloc) const
	{
		size_type str_size = boost::size(src);

		return 
			(pos < str_size && len)?
				dst_type(
					boost::begin(src) + pos, 
					boost::begin(src) + (pos + (std::min)(str_size - pos, len)),
					alloc)
				: dst_type(alloc);
	}
};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

class charset_foo
	: private nonable::noncreateable
{
private:
	typedef charset_foo this_type;

protected:
	typedef charset_error_def charset_error_def_type;
	typedef charset_error_def_type::error_maker_type error_maker_type;

public:
	// this foos is same charset xhg

	template<typename String, typename Char> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>
				>,
				String
			>::type
		s_xchg(const Char* src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef basic_string<Char> src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return (src && src[0])? cal(src_type(src)) : dst_type();
	}

	template<typename String, typename Char, typename Alloc> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>,
					container_ex::is_allocator<Alloc>
				>,
				String
			>::type
		s_xchg(const Char* src, const Alloc& alloc)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef basic_string<Char> src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return (src && src[0])? cal(src_type(src), alloc) : dst_type(alloc);
	}

	template<typename String, typename Char> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>
				>,
				String
			>::type
		s_xchg(const Char* src, size_type n)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef typename
			boost::mpl::if_
			<
				is_native_char<Char>,
				basic_string<Char>,
				vector<Char>
			>::type src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return (src && n)? cal(src_type(src, src + n)) : dst_type();
	}

	template<typename String, typename Char, typename Alloc> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>,
					container_ex::is_allocator<Alloc>
				>,
				String
			>::type
		s_xchg(const Char* src, size_type n, const Alloc& alloc)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef typename
			boost::mpl::if_
			<
				is_native_char<Char>,
				basic_string<Char>,
				vector<Char>
			>::type src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;

		return (src && n)? cal(src_type(src, src + n), alloc) : dst_type(alloc);
	}

	template<typename String, typename Src_String> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				typename detail::charset_foo_call_t< Src_String, String >::ref_result_type
			>::type
		s_xchg(Src_String& src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return cal(src);
	}

	template<typename String, typename Src_String, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					container_ex::is_allocator<Alloc>
				>,
				typename detail::charset_foo_call_t< Src_String, String >::ref_result_type
			>::type
		s_xchg(Src_String& src, const Alloc& alloc)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return cal(src, alloc);
	}

	template<typename String, typename Src_String> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				String
			>::type
		s_xchg(Src_String& src, size_type pos, size_type len)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return cal(src, pos, len);
	}

	template<typename String, typename Src_String, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					container_ex::is_allocator<Alloc>
				>,
				String
			>::type
		s_xchg(Src_String& src, size_type pos, size_type len, const Alloc& alloc)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return cal(src, pos, len, alloc);
	}

	template<typename String, typename Src_String > inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				typename detail::charset_foo_call_t< Src_String, String >::cref_result_type
			>::type
		s_xchg(const Src_String& src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return cal(src);
	}

	template<typename String, typename Src_String, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					container_ex::is_allocator<Alloc>
				>,
				typename detail::charset_foo_call_t< Src_String, String >::cref_result_type
			>::type
		s_xchg(const Src_String& src, const Alloc& alloc)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return cal(src, alloc);
	}

	template<typename String, typename Char> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>
				>,
				String&
			>::type
		s_xchg(String& dst, const Char* src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef basic_string<Char> src_type;
		return this_type::s_xchg(dst, src_type(src));
	}

	template<typename String, typename Char> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>
				>,
				String&
			>::type
		s_xchg(String& dst, const Char* src, size_type n)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef typename
			boost::mpl::if_
			<
				is_native_char<Char>,
				basic_string<Char>,
				vector<Char>
			>::type src_type;

		return src && n? this_type::s_xchg(dst, src_type(src, src + n)) : dst;
	}

	template<typename String> inline
	static
		typename 
			boost::enable_if
			<
				container_ex::is_container<String>,
				String&
			>::type
		s_xchg(String& dst, BOOST_RV_REF(String) src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		src_type& src_ref = static_cast<src_type&>(src);
		return boost::empty(src)? dst : (copy_or_move_or_swap(dst, boost::move(cal(src_ref))), dst);
	}

	template<typename String, typename Src_String > inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				String&
			>::type
		s_xchg(String& dst, const Src_String& src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		return boost::empty(src)? dst : (dst = cal(src, ::yggr::get_allocator(dst)), dst);
	}

	template<typename String, typename Src_String > inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				String&
			>::type
		s_xchg(String& dst, const Src_String& src, size_type pos, size_type len)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef Src_String src_type;
		typedef String dst_type;
		typedef detail::charset_foo_call_t<src_type, dst_type> call_type;

		call_type cal;
		dst = cal(src, pos, len, ::yggr::get_allocator(dst));
		return dst;
	}

	// this foos is unknown is same charset xhg
	template<typename String, typename Char> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>
				>,
				String
			>::type
		s_xchg(const Char* src, 
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;
		typedef basic_string<Char> src_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(!src || 0 == src[0])
		{
			return dst_type();
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg<dst_type>(src);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Char, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>
				>,
				String
			>::type
		s_xchg(const Char* src, const Alloc& alloc,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;
		typedef basic_string<Char> src_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(!src || 0 == src[0])
		{
			return dst_type(alloc);
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg<dst_type>(src, alloc);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Char> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>
				>,
				String
			>::type
		s_xchg(const Char* src, size_type n,
				const string& src_charset_name, 
				const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;

		typedef typename 
			boost::mpl::if_
			<
				is_native_char<Char>,
				basic_string<Char>,
				vector<Char>
			>::type src_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(!(src && n))
		{
			return dst_type();
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg<dst_type>(src, n);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;

			if(!conv(src, n, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Char, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>
				>,
				String
			>::type
		s_xchg(const Char* src, size_type n, 
				const Alloc& alloc,
				const string& src_charset_name, 
				const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;

		typedef typename 
			boost::mpl::if_
			<
				is_native_char<Char>,
				basic_string<Char>,
				vector<Char>
			>::type src_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(!(src && n))
		{
			return dst_type(alloc);
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg<dst_type>(src, n, alloc);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);
			
			if(!conv(src, n, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Src_String > inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					boost::mpl::not_< boost::is_same<String, Src_String> >
				>,
				String
			>::type
		s_xchg(const Src_String& src,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(boost::empty(src))
		{
			return dst_type();
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg<dst_type>(src);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;
			
			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Src_String, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					boost::mpl::not_< boost::is_same<String, Src_String> >
				>,
				String
			>::type
		s_xchg(const Src_String& src, const Alloc& alloc,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(boost::empty(src))
		{
			return dst_type(alloc);
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg<dst_type>(src, alloc);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);
			
			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Src_String > inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					boost::is_same<String, Src_String>
				>,
				String
			>::type
		s_xchg(const Src_String& src,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(boost::empty(src))
		{
			return dst_type();
		}
		else if(src_charset_name == dst_charset_name)
		{
			YGGR_MSG_PCRE_WARNING("please call only a parameter of the overloaded s_xchg foo to fix the same character string conversion")
			return src;
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;
			
			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Src_String, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
					boost::is_same<String, Src_String>
				>,
				String
			>::type
		s_xchg(const Src_String& src, const Alloc& alloc,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(boost::empty(src))
		{
			return dst_type(alloc);
		}
		else if(src_charset_name == dst_charset_name)
		{
			YGGR_MSG_PCRE_WARNING("please call only a parameter of the overloaded s_xchg foo to fix the same character string conversion")
			return src;
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	// this foos is different charset xhg
	template<typename String, typename Char> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>
				>,
				String
			>::type
		s_xchg_diff_charset(const Char* src, const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());
		assert(src_charset_name != dst_charset_name);

		if(!(src && src[0]))
		{
			return dst_type();
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Char, typename Alloc> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>,
                    container_ex::is_allocator<Alloc> // apple_cc need
				>,
				String
			>::type
		s_xchg_diff_charset(const Char* src, const Alloc& alloc,
							const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());
		assert(src_charset_name != dst_charset_name);

		if(!(src && src[0]))
		{
			return dst_type(alloc);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Char> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>
				>,
				String
			>::type
		s_xchg_diff_charset(const Char* src, size_type n, 
								const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());
		assert(src_charset_name != dst_charset_name);

		if(!(src && n))
		{
			return dst_type();
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;

			if(!conv(src, n, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Char, typename Alloc> inline
	static
		typename
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>,
                    container_ex::is_allocator<Alloc> // apple_cc need
				>,
				String
			>::type
		s_xchg_diff_charset(const Char* src, size_type n, const Alloc& alloc,
							const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());
		assert(src_charset_name != dst_charset_name);

		if(!(src && n))
		{
			return dst_type(alloc);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);

			if(!conv(src, n, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Src_String> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				String
			>::type
		s_xchg_diff_charset(const Src_String& src,
							const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());
		assert(src_charset_name != dst_charset_name);

		if(boost::empty(src))
		{
			return dst_type();
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst;

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	template<typename String, typename Src_String, typename Alloc> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>,
                    container_ex::is_allocator<Alloc> // apple_cc need
				>,
				String
			>::type
		s_xchg_diff_charset(const Src_String& src, const Alloc& alloc,
								const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());
		assert(src_charset_name != dst_charset_name);

		if(boost::empty(src))
		{
			return dst_type(alloc);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(alloc);

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return dst;
		}
	}

	// this foos is unknown is same charset xhg
	template<typename String, typename Char> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_native_char<Char>
				>,
				String&
			>::type
		s_xchg(String& o_dst, const Char* src,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));

		typedef String dst_type;
		typedef basic_string<Char> src_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(!src || 0 == src[0])
		{
			return o_dst;
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg(o_dst, src_type(src));
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(o_dst.get_allocator());

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return (o_dst.swap(dst), o_dst);
		}
	}

	template<typename String, typename Char> inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					is_convertible_char<Char>
				>,
				String&
			>::type
		s_xchg(String& o_dst, const Char* src, size_type n, 
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		typedef String dst_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<Char>,
				basic_string<Char>,
				vector<Char>
			>::type src_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(!(src && n))
		{
			return o_dst;
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg(o_dst, src, n);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(o_dst.get_allocator());

			if(!conv(src, n, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return (o_dst.swap(dst), o_dst);
		}
	}

	template<typename String> inline
	static
		typename 
			boost::enable_if
			<
				container_ex::is_container<String>,
				String&
			>::type
		s_xchg(String& o_dst, BOOST_RV_REF(String) src,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(boost::empty(src))
		{
			return o_dst;
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg(o_dst, boost::move(src));
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(o_dst.get_allocator());

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return (o_dst.swap(dst), o_dst);
		}	
	}

	template<typename String, typename Src_String > inline
	static
		typename 
			boost::enable_if
			<
				boost::mpl::and_
				<
					container_ex::is_container<String>,
					boost::mpl::or_
					<
						container_ex::is_container<Src_String>,
						is_string_view_t<Src_String>
					>
				>,
				String&
			>::type
		s_xchg(String& o_dst, const Src_String& src,
				const string& src_charset_name, const string& dst_charset_name)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<String>::type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<typename boost::range_iterator<Src_String>::type>));

		typedef String dst_type;

		assert(!src_charset_name.empty());
		assert(!dst_charset_name.empty());

		if(boost::empty(src))
		{
			return o_dst;
		}
		else if(src_charset_name == dst_charset_name)
		{
			return this_type::s_xchg(o_dst, src);
		}
		else
		{
			charset_converter conv(src_charset_name, dst_charset_name);
			dst_type dst(o_dst.get_allocator());

			if(!conv(src, dst))
			{
				throw error_maker_type::make_error(error_maker_type::E_charset_conver_failed);
			}
		
			return (o_dst.swap(dst), o_dst);
		}
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_CHARSET_FOO_HPP__
