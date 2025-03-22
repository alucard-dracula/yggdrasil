//utf8_string_impl.hpp

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

#ifndef __YGGR_CHARSET_UTF8_STRING_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_STRING_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/strict_sizeof.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/unwrap_reverse_iterator.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/get_allocator.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/bytes/byte_size_sp_tpl.hpp>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/string_charset_helper.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/string_view.hpp>
#include <yggr/charset/basic_string_adp_helper.hpp>
#include <yggr/charset/utf8_foo.hpp>

#include <yggr/charset/utf8_string_impl_decl.hpp>
#include <yggr/charset/utf8_string_view_decl.hpp>
#include <yggr/charset/utf8_string_iterator_decl.hpp>

#include <yggr/charset/utf8_char_impl_decl.hpp>
#include <yggr/charset/utf8_char_view_decl.hpp>
#include <yggr/charset/utf8_char_reference_decl.hpp>

#include <boost/range/functions.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/functional/hash/hash.hpp>

#include <utility>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <cassert>
#include <stdexcept>

// (const char* str, size_type n) n -> sizeof(char) * size;
// (any_string, size_type subpos, size_type sublen) subpos, sublen -> string.length()

#ifdef _DEBUG
#	define YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() dst_charset_name
#else
#	define YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()
#endif // _DEBUG

#if !(BOOST_VERSION < 106600)

namespace boost
{
namespace container
{

// fix mingw gcc ambiguous overload for 'operator>' 'operator>='  'operator=='  'operator<=' 'operator<'
// yggr: utf8_string_impl<...>() >= basic_string<...> or boost: BasicStringView<...> >= basic_sting()
template<typename Char, typename Traits, typename Alloc>
struct is_string< ::yggr::charset::utf8_string_impl<Char, Traits, Alloc> >
{
	static const bool value = true;
};

} // namespace container
} // namspace boost

#endif // #if !(BOOST_VERSION < 106600)

namespace yggr
{
namespace charset
{

// is_utf8_string_impl
template<typename Char, typename Traits, typename Alloc>
struct is_utf8_string_t< utf8_string_impl<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
};

//template<typename Char, typename Traits, typename Alloc>
//struct charset_name_t< utf8_string_impl<Char, Traits, Alloc> >
//	:	private nonable::noncopyable,
//		private nonable::nonmoveable
//{
//public:
//	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
//	typedef typename boost::remove_cv<Char>::type native_char_type;
//	typedef typename string_type::char_type char_type;
//	typedef basic_utf8_string_view<Char, Traits> string_view_type;
//
//public:
//
//	YGGR_CXX23_CONSTEXPR_OR_INLINE explicit operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
//	{
//		return default_charset_config::get_utf8_string_charset_name();
//	}
//
//	YGGR_CXX23_CONSTEXPR_OR_INLINE explicit operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
//	{
//		return default_charset_config::get_str_utf8_string_charset_name();
//	}
//};

namespace detail
{

/*
!!!
mingw gcc

template<typename ...T, template<typename ...T> class Tpl>
struct tpl_t<Tpl<T...> > {};

template<typename ...T>
struct tpl_t<real_tpl<T...> > {};

is erro "ambiguous template instantiation"

!!!
*/

template<typename Char, typename Traits, typename Alloc>
struct charset_name_t_string_impl< utf8_string_impl<Char, Traits, Alloc> >
	:	private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	typedef typename boost::remove_cv<Char>::type native_char_type;
	typedef typename string_type::char_type char_type;
	typedef basic_utf8_string_view<Char, Traits> string_view_type;

public:

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_utf8_string_charset_name();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return charset::default_charset_config::get_str_utf8_string_charset_name();
	}
};

} // namespace detail

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc, typename C1, typename C2>
struct other_string<utf8_string_impl<Char, Traits, Alloc >, C1, C2 >
{
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef Char value_type;
	typedef Traits traits_type;
	typedef Alloc tpl_arg_allocator_type;

	typedef utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type > string_type;
	typedef typename string_type::char_type char_type;
	typedef typename string_type::allocator_type allocator_type;

	typedef value_type other_value_type;
	typedef traits_type other_traits_type;
	typedef tpl_arg_allocator_type other_tpl_arg_allocator_type;
	typedef allocator_type other_allocator_type;

	typedef string_type other_string_type;
	typedef char_type other_char_type;

	typedef other_string_type type;
};

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{
namespace detail
{

// iterator_append_helper
template<typename Iter,
			bool is_seq_random_access_iter =
				boost::mpl::and_
				<
					is_random_access_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>::value >
struct iterator_append_helper;

template<typename Iter>
struct iterator_append_helper<Iter, true>
{
public:
	typedef Iter iter_type;

private:
	typedef iterator_append_helper this_type;

public:
	template<typename Utf8String> inline
	Utf8String& operator()(Utf8String& dst, iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		assert(std::distance(s, e) >= 0);
		size_type size = static_cast<size_type>(std::distance(s, e));
		return size? prv_operator(dst, &(*s), size, src_charset_name) : dst;
	}

private:
	template<typename Char, typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_integral<Char>,
			Utf8String&
		>::type
		prv_operator(Utf8String& dst, const Char* s, size_type size,
						const string& src_charset_name) const
	{
		assert(s && size);
		return dst.append(s, size, src_charset_name);
	}

	template<typename Utf8String, typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::disable_if
		<
			boost::is_integral<Char>,
			Utf8String&
		>::type
		prv_operator(Utf8String& dst, const Char* s, size_type size,
						const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		assert(s && size);

		src_type src(s, s + size);
		return dst.append(&src[0], size, src_charset_name);
	}
};

template<typename Iter>
struct iterator_append_helper<Iter, false>
{
public:
	typedef Iter iter_type;

private:
	typedef iterator_append_helper this_type;

public:
	template<typename Utf8String> inline
	Utf8String& operator()(Utf8String& dst, iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		src_type src(s, e);

		return src.empty()? dst : dst.append(&src[0], src.size(), src_charset_name);
	}
};

// iterator_assign_helper
template<typename Iter,
			bool is_seq_random_access_iter =
				boost::mpl::and_
				<
					is_random_access_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>::value >
struct iterator_assign_helper;

template<typename Iter>
struct iterator_assign_helper<Iter, true>
{
public:
	typedef Iter iter_type;

private:
	typedef iterator_assign_helper this_type;

public:
	template<typename Utf8String> inline
	Utf8String& operator()(Utf8String& dst, iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		assert(std::distance(s, e) >= 0);
		size_type size = static_cast<size_type>(std::distance(s, e));
		return size? prv_operator(dst, &(*s), size, src_charset_name) : (dst.clear(), dst);
	}

private:
	template<typename Utf8String, typename Char> inline
	typename
		boost::enable_if
		<
			boost::is_integral<Char>,
			Utf8String&
		>::type
		prv_operator(Utf8String& dst, const Char* s, size_type size,
						const string& src_charset_name) const
	{
		assert(s && size);
		return dst.assign(s, size, src_charset_name);
	}

	template<typename Utf8String, typename Char> inline
	typename
		boost::disable_if
		<
			boost::is_integral<Char>,
			Utf8String&
		>::type
		prv_operator(Utf8String& dst, const Char* s, size_type size,
						const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		assert(s && size);

		src_type src(s, s + size);
		return dst.assign(&src[0], size, src_charset_name);
	}
};

template<typename Iter>
struct iterator_assign_helper<Iter, false >
{
public:
	typedef Iter iter_type;

private:
	typedef iterator_assign_helper this_type;

public:
	template<typename Utf8String> inline
	Utf8String& operator()(Utf8String& dst, iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		src_type src(s, e);
		return src.empty()? (dst.clear(), dst) : dst.assign(&src[0], src.size(), src_charset_name);
	}
};

// iterator_insert_helper
template<typename RetIter, typename Iter, typename ParamIter, typename ParamBaseIter,
			bool is_seq_random_access_iter =
				boost::mpl::and_
				<
					is_random_access_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>::value >
struct iterator_insert_helper;

template<typename RetIter, typename Iter, typename ParamIter, typename ParamBaseIter >
struct iterator_insert_helper<RetIter, Iter, ParamIter, ParamBaseIter, true >
{
public:
	typedef RetIter iterator;
	typedef Iter iter_type;
	typedef ParamIter param_iterator_type;
	typedef ParamBaseIter param_base_iterator_type;

private:
	typedef iterator_insert_helper this_type;

public:
	template<typename Utf8String> inline
	iterator operator()(Utf8String& dst,
						param_iterator_type p,
						iter_type s, iter_type e,
						const string& src_charset_name) const
	{
		assert(std::distance(s, e) >= 0);
		size_type size = static_cast<size_type>(std::distance(s, e));
		return size?
				prv_operator(dst, p, &(*s), size, src_charset_name)
				: iterator(dst, dst.begin().org() + std::distance<param_base_iterator_type>(dst.begin().org(), p.org()));
	}

private:
	template<typename Utf8String, typename Char> inline
	typename
		boost::enable_if
		<
			boost::is_integral<Char>,
			iterator
		>::type
		prv_operator(Utf8String& dst, param_iterator_type p,
						const Char* s, size_type size,
						const string& src_charset_name) const
	{
		assert(s && size);
		size_type old_size = std::distance<param_base_iterator_type>(dst.begin().org(), p.org());
		size_type pos = std::distance<param_iterator_type>(dst.begin(), p);

		dst.insert(pos, s, size, src_charset_name);
		return iterator(dst, dst.begin().org() + old_size);
	}

	template<typename Utf8String, typename Char> inline
	typename
		boost::disable_if
		<
			boost::is_integral<Char>,
			iterator
		>::type
		prv_operator(Utf8String& dst, param_iterator_type p,
						const Char* s, size_type size,
						const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		assert(s && size);

		size_type old_size = std::distance<param_base_iterator_type>(dst.begin().org(), p.org());
		size_type pos = std::distance(dst.begin(), p);
		src_type src(s, s + size);

		dst.insert(pos, &src[0], size, src_charset_name);
		return iterator(dst, dst.begin().org() + old_size);
	}
};

template<typename RetIter, typename Iter, typename ParamIter, typename ParamBaseIter >
struct iterator_insert_helper<RetIter, Iter, ParamIter, ParamBaseIter, false >
{
public:
	typedef RetIter iterator;
	typedef Iter iter_type;
	typedef ParamIter param_iterator_type;
	typedef ParamBaseIter param_base_iterator_type;

private:
	typedef iterator_insert_helper this_type;

public:
	template<typename Utf8String> inline
	iterator operator()(Utf8String& dst,
							param_iterator_type p,
							iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		size_type old_size = std::distance<param_base_iterator_type>(dst.begin().org(), p.org());
		src_type src(s, e);
		if(!src.empty())
		{
			size_type pos = std::distance<param_iterator_type>(dst.begin(), p);
			dst.insert(pos, &src[0], src.size(), src_charset_name);
		}

		return iterator(dst, dst.begin().org() + old_size);
	}
};

// iterator_replace_helper
template<typename Iter, typename ParamIter,
			bool is_seq_random_access_iter =
				boost::mpl::and_
				<
					is_random_access_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>::value >
struct iterator_replace_helper;

template<typename Iter, typename ParamIter>
struct iterator_replace_helper<Iter, ParamIter, true>
{
public:
	typedef Iter iter_type;
	typedef ParamIter param_iterator_type;

private:
	typedef iterator_replace_helper this_type;

public:
	template<typename Utf8String> inline
	Utf8String& operator()(Utf8String& dst,
							param_iterator_type i1, param_iterator_type i2,
							iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		typedef Utf8String utf8_string_type;
		typedef typename utf8_string_type::value_type value_type;

		assert(std::distance(s, e) >= 0);
		size_type size = static_cast<size_type>(std::distance(s, e));

		return size? prv_operator(dst, i1, i2, &(*s), size, src_charset_name)
						: dst.replace(i1, i2, 0, value_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

private:
	template<typename Utf8String, typename Char> inline
	typename
		boost::enable_if
		<
			boost::is_integral<Char>,
			Utf8String&
		>::type
		prv_operator(Utf8String& dst,
						param_iterator_type i1, param_iterator_type i2,
						const Char* s, size_type size,
						const string& src_charset_name) const
	{
		assert(s && size);
		return dst.replace(i1, i2, s, size, src_charset_name);
	}

	template<typename Utf8String, typename Char> inline
	typename
		boost::disable_if
		<
			boost::is_integral<Char>,
			Utf8String&
		>::type
		prv_operator(Utf8String& dst,
						param_iterator_type i1, param_iterator_type i2,
						const Char* s, size_type size,
						const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		assert(s && size);

		src_type src(s, s + size);
		return dst.replace(i1, i2, &src[0], size, src_charset_name);
	}
};

template<typename Iter, typename ParamIter>
struct iterator_replace_helper<Iter, ParamIter, false>
{
public:
	typedef Iter iter_type;

private:
	typedef ParamIter param_iterator_type;

public:
	template<typename Utf8String> inline
	Utf8String& operator()(Utf8String& dst,
							param_iterator_type i1, param_iterator_type i2,
							iter_type s, iter_type e,
							const string& src_charset_name) const
	{
		typedef Utf8String utf8_string_type;
		typedef typename utf8_string_type::value_type value_type;
		typedef typename boost::iterator_value<iter_type>::type org_iter_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(org_iter_val_type) iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		src_type src(s, e);

		return src.empty()?
				dst.replace(i1, i2, 0, value_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME())
				: dst.replace(i1, i2, &src[0], src.size(), src_charset_name);
	}
};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename TChar, typename TTraits, typename TAlloc>
class utf8_string_impl
{
public:
	typedef basic_string<TChar, TTraits, TAlloc> base_type;

public:
	typedef TAlloc tpl_arg_allocator_type;

public:
	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::traits_type traits_type;
	//typedef typename base_type::reference reference; // discard base_type::reference
	//typedef typename base_type::const_reference const_reference; // discard base_type::const_reference
	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;
	//typedef typename base_type::iterator iterator; // discard base_type::iterator
	//typedef typename base_type::const_iterator const_iterator; // discard base_type::const_iterator
	//typedef typename base_type::reverse_iterator reverse_iterator; // discard base_type::reverse_iterator
	//typedef typename base_type::const_reverse_iterator const_reverse_iterator; // discard base_type::const_reverse_iterator
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::size_type size_type;

	YGGR_STATIC_CONSTANT(size_type, npos = static_cast<size_type>(-1));

public:
	typedef utf8_char_impl<value_type, traits_type, tpl_arg_allocator_type> char_type;
	typedef basic_utf8_char_view<value_type, traits_type> char_view_type;

protected:
	typedef typename base_type::iterator base_iterator;
	typedef typename base_type::const_iterator base_const_iterator;

public:
	typedef utf8_string_modify_iterator<char_type, base_iterator> iterator;
	typedef utf8_string_const_iterator<char_type, base_const_iterator> const_iterator;
	typedef utf8_string_iterator<char_type, base_iterator> non_const_iterator;

	// must using std::reverse_iterator
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator<non_const_iterator> non_const_reverse_iterator;

	typedef basic_utf8_char_reference<utf8_string_impl> reference;
	typedef basic_utf8_char_const_reference<utf8_string_impl> const_reference;

protected:

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
	typedef iterator param_iterator_type;
	typedef base_iterator param_base_iterator_type;
#else
	typedef const_iterator param_iterator_type;
	typedef base_const_iterator param_base_iterator_type;
#endif // YGGR_USEING_CXX11

// sfinae
protected:
	typedef mplex::sfinae_type sfinae_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<value_type>::value == 1)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<typename traits_type::char_type>::value == 1)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<typename allocator_type::value_type>::value == 1)>));

private:
	typedef utf8_string_impl this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// default
	utf8_string_impl(void)
		: _utf8_size(0)
	{
	}

	explicit utf8_string_impl(const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
	}

	// (n, char)

	template<typename Char>
	utf8_string_impl(size_type n, Char c,
						typename boost::enable_if< is_can_be_conv_into_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(n, static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char>
	utf8_string_impl(size_type n, Char c, const allocator_type& alloc,
						typename boost::enable_if< is_can_be_conv_into_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(n, static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char>
	utf8_string_impl(size_type n, Char c, const string& src_charset_name,
						typename boost::enable_if< is_can_be_conv_into_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(n, static_cast<now_char_type>(c), src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(size_type n, Char c,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename boost::enable_if< is_can_be_conv_into_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(n, static_cast<now_char_type>(c), src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(size_type n, const Char* c,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(size_type n, const Char* c, const allocator_type& alloc,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(size_type n, const Char* c, const string& src_charset_name,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(size_type n, const Char* c,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	utf8_string_impl(size_type n, const Basic_String<Char, Traits, Alloc>& c,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	utf8_string_impl(size_type n, const Basic_String<Char, Traits, allocator_type>& c,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, allocator_type> >,
								sfinae_type
							>::type nil = 0)
		: _base(container::get_allocator(c)), _utf8_size(0)
	{
		this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	utf8_string_impl(size_type n, const Basic_String<Char, Traits, Alloc>& c,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	utf8_string_impl(size_type n, const Basic_String<Char, Traits, Alloc>& c,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	utf8_string_impl(size_type n, const Basic_String<Char, Traits, allocator_type>& c,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, allocator_type> >,
								sfinae_type
							>::type nil = 0)
		: _base(container::get_allocator(c)), _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	utf8_string_impl(size_type n, const Basic_String<Char, Traits, Alloc>& c,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
		: _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Char, typename Traits>
	utf8_string_impl(size_type n, const utf8_char_impl<Char, Traits, allocator_type>& c)
		: _base(container::get_allocator(c)), _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(size_type n,
						const utf8_char_impl<Char, Traits, Alloc>& c,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(size_type n,
						const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
		: _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Char, typename Traits>
	utf8_string_impl(size_type n,
						const utf8_char_impl<Char, Traits, allocator_type>& c,
						const string& src_charset_name)
		: _base(container::get_allocator(c)), _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(size_type n,
						const utf8_char_impl<Char, Traits, Alloc>& c,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Char, typename Traits>
	utf8_string_impl(size_type n, const basic_utf8_char_view<Char, Traits>& c)
		: _utf8_size(0)
	{
		this_type::assign(n, c);
	}

	template<typename Char, typename Traits>
	utf8_string_impl(size_type n,
						const basic_utf8_char_view<Char, Traits>& c,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c);
	}

	template<typename Char, typename Traits>
	utf8_string_impl(size_type n,
						const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
		: _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Char, typename Traits>
	utf8_string_impl(size_type n,
						const basic_utf8_char_view<Char, Traits>& c,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c, src_charset_name);
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(c.get_allocator()), _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(c.get_allocator()), _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_const_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(c.get_allocator()), _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_const_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c.view());
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_const_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(c.get_allocator()), _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									typename basic_utf8_char_const_reference<Utf8String>::allocator_type,
									allocator_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Utf8String>
	utf8_string_impl(size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(n, c.view(), src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(const Char* src)
		: _utf8_size(0)
	{
		BOOST_MPL_ASSERT((is_convertible_char<Char>));

		// !!! this comment is a remark don't delete it !!!
		// if src_charset_name has a default value, a C1001 error occurs in MSVC
		// error C1001: An internal error has occurred in the compiler.
		// so use twines function fixed this issue.
//#	if 0
//		this_type::assign(src);
//#	else
//		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
//#	endif // 0, 1

		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(const Char* src, const allocator_type& alloc,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(const Char* src, const string& src_charset_name,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(const Char* src,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	// const char*, size_type
	template<typename Char>
	utf8_string_impl(const Char* src, size_type n,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(const Char* src, size_type n, const allocator_type& alloc,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(const Char* src, size_type n,
						const string& src_charset_name,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(src, n, src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(const Char* src, size_type n,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename boost::enable_if< is_convertible_char<Char>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, n, src_charset_name);
	}

	// string
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src)
		: _utf8_size(0)
	{
		BOOST_MPL_ASSERT((is_basic_string_t< Basic_String<Char, Traits, Alloc> >));
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, allocator_type>& src)
		: _base(container::get_allocator(src)), _utf8_size(0)
	{
		BOOST_MPL_ASSERT((is_basic_string_t< Basic_String<Char, Traits, allocator_type> >));
		this_type::assign<Char, Traits, allocator_type, Basic_String>(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, src_charset_name);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, allocator_type>& src,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, allocator_type> >,
								sfinae_type
							>::type nil = 0)
		: _base(container::get_allocator(src)), _utf8_size(0)
	{
		this_type::assign<Char, Traits, allocator_type, Basic_String>(src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, src_charset_name);
	}

	// string_type, size_type
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, src_charset_name);
	}

	// string_type, size_type, size_type

	template<typename Char>
	utf8_string_impl(const Char* src, size_type subpos, size_type sublen,
						typename
							boost::enable_if
							<
								is_convertible_char<Char>,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(const Char* src, size_type subpos, size_type sublen, const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_convertible_char<Char>,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_string_impl(const Char* src, size_type subpos, size_type sublen,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_convertible_char<Char>,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

		template<typename Char>
	utf8_string_impl(const Char* src, size_type subpos, size_type sublen, const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_convertible_char<Char>,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen, const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen, const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, sublen, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, subpos, sublen, src_charset_name);
	}


	// iterator
	template<typename InputIter>
	utf8_string_impl(InputIter start, InputIter last,
						typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(start, last);
	}

	template<typename InputIter>
	utf8_string_impl(InputIter start, InputIter last,
					const allocator_type& alloc,
					typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(start, last);
	}

	template<typename InputIter>
	utf8_string_impl(InputIter start, InputIter last,
						const string& src_charset_name,
						typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _utf8_size(0)
	{
		this_type::assign(start, last, src_charset_name);
	}

	template<typename InputIter>
	utf8_string_impl(InputIter start, InputIter last,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(start, last, src_charset_name);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// initializer_list
	template<typename Char>
	utf8_string_impl(std::initializer_list<Char> il)
		: _utf8_size(0)
	{
		BOOST_MPL_ASSERT((is_can_be_conv_into_char<Char>));
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char>
	utf8_string_impl(std::initializer_list<Char> il, const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_can_be_conv_into_char<Char>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char>
	utf8_string_impl(std::initializer_list<Char> il, const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_can_be_conv_into_char<Char>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(il, src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(std::initializer_list<Char> il,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_can_be_conv_into_char<Char>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(il, src_charset_name);
	}

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// utf8_string_impl
	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other)
		: _utf8_size(0)
	{
		this_type::assign(other);
	}

	template<typename Char, typename Traits>
	utf8_string_impl(const utf8_string_impl<Char, Traits, allocator_type>& other)
		: _base(other.get_allocator()), _utf8_size(0)
	{
		this_type::assign(other);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(other);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
		: _utf8_size(0)
	{
		this_type::assign(other, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(other, src_charset_name);
	}

	// utf8_string_impl, size_type
	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos)
		: _utf8_size(0)
	{
		this_type::assign(other, subpos);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(other, subpos);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos,
						const string& src_charset_name)
		: _utf8_size(0)
	{
		this_type::assign(other, subpos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(other, subpos, src_charset_name);
	}

	// utf8_string_impl, size_type, size_type
	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
		: _utf8_size(0)
	{
		this_type::assign(other, subpos, sublen);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(other, subpos, sublen);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
		: _utf8_size(0)
	{
		this_type::assign(other, subpos, sublen, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(other, subpos, sublen, src_charset_name);
	}

private:

// disable this utf8_char_impl auto conv to base_type

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

    template< typename Char, typename Traits, typename Alloc, typename ...Args >
    utf8_string_impl(const utf8_char_impl<Char, Traits, Alloc>&, Args ...args );

    template< typename Char, typename Traits, typename ...Args >
    utf8_string_impl(const basic_utf8_char_view<Char, Traits>&, Args ...args );

    template< typename Utf8String, typename ...Args >
    utf8_string_impl(const basic_utf8_char_reference<Utf8String>&, Args ...args );

    template< typename Utf8String, typename ...Args >
    utf8_string_impl(const basic_utf8_char_const_reference<Utf8String>&, Args ...args );

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Char, typename Traits, typename Alloc \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	utf8_string_impl(const utf8_char_impl<Char, Traits, Alloc>& \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Char, typename Traits \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	utf8_string_impl(const basic_utf8_char_view<Char, Traits>& \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	utf8_string_impl(const basic_utf8_char_reference<Utf8String>& \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	utf8_string_impl(const basic_utf8_char_const_reference<Utf8String>& \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) );

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	//base_type
	utf8_string_impl(BOOST_RV_REF(base_type) src)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(const base_type& src)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src, const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(const base_type& src, const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src, const string& src_charset_name)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), src_charset_name);
	}

	utf8_string_impl(const base_type& src, const string& src_charset_name)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	utf8_string_impl(const base_type& src,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, src_charset_name);
	}

	// base_type, size_type
	utf8_string_impl(BOOST_RV_REF(base_type) src, size_type subpos)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(const base_type& src, size_type subpos)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src, size_type subpos, const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(const base_type& src, size_type subpos, const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src, size_type subpos, const string& src_charset_name)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, src_charset_name);
	}

	utf8_string_impl(const base_type& src, size_type subpos, const string& src_charset_name)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, src_charset_name);
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src, size_type subpos,
						const allocator_type& alloc, const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, src_charset_name);
	}

	utf8_string_impl(const base_type& src, size_type subpos,
						const allocator_type& alloc, const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
					size_type subpos,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							sfinae_type
						>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, src_charset_name);
	}

	// base_type, size_type, size_type
	utf8_string_impl(BOOST_RV_REF(base_type) src, size_type subpos, size_type sublen)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(const base_type& src, size_type subpos, size_type sublen)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(const base_type& src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, sublen, src_charset_name);
	}

	utf8_string_impl(const base_type& src,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	utf8_string_impl(BOOST_RV_REF(base_type) src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	utf8_string_impl(const base_type& src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos,
						size_type sublen,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos,
						size_type sublen,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(value_type));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos, size_type sublen,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(src), subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos, size_type sublen,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(src.get_allocator()), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							basic_string<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const basic_string<value_type, traits_type, Alloc2>& src,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									basic_string<value_type, traits_type, Alloc2>,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(src, subpos, sublen, src_charset_name);
	}

	// this_type
	utf8_string_impl(BOOST_RV_REF(this_type) right)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right));
	}

	utf8_string_impl(const this_type& right)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right));
	}

	utf8_string_impl(const this_type& right,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						const string& src_charset_name)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), src_charset_name);
	}

	utf8_string_impl(const this_type& right,
						const string& src_charset_name)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, src_charset_name);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), src_charset_name);
	}

	utf8_string_impl(const this_type& right,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right));
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right));
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, src_charset_name);
	}

	// this_type, size_type
	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos);
	}

	utf8_string_impl(const this_type& right, size_type subpos)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos);
	}

	utf8_string_impl(const this_type& right,
						size_type subpos,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos,
						const string& src_charset_name)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, src_charset_name);
	}

	utf8_string_impl(const this_type& right,
						size_type subpos,
						const string& src_charset_name)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos, src_charset_name);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, src_charset_name);
	}

	utf8_string_impl(const this_type& right,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos, src_charset_name);
	}

	// this_type, size_type, size_type
	utf8_string_impl(BOOST_RV_REF(this_type) right, size_type subpos, size_type sublen)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen);
	}

	utf8_string_impl(const this_type& right, size_type subpos, size_type sublen)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen);
	}

	utf8_string_impl(const this_type& right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen, src_charset_name);
	}

	utf8_string_impl(const this_type& right,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen, src_charset_name);
	}

	utf8_string_impl(BOOST_RV_REF(this_type) right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen, src_charset_name);
	}

	utf8_string_impl(const this_type& right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos,
						size_type sublen,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos,
						size_type sublen,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos, size_type sublen,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos, size_type sublen,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator()), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(BOOST_RV_REF_BEG
							utf8_string_impl<value_type, traits_type, Alloc2>
						BOOST_RV_REF_END right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(boost::move(right), subpos, sublen, src_charset_name);
	}

	template<typename Alloc2>
	utf8_string_impl(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
						size_type subpos, size_type sublen,
						const allocator_type& alloc,
						const string& src_charset_name,
						typename
							boost::enable_if
							<
								is_isomeric_same_string
								<
									typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
									base_type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(right, subpos, sublen, src_charset_name);
	}

	// string_view
	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(str_view);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								const string& src_charset_name,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(str_view, src_charset_name);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								const allocator_type& alloc,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(str_view);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								const allocator_type& alloc,
								const string& src_charset_name,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(str_view, src_charset_name);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								size_type subpos, size_type sublen,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(str_view, subpos, sublen);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								size_type subpos, size_type sublen,
								const string& src_charset_name,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(str_view, subpos, sublen, src_charset_name);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								size_type subpos, size_type sublen,
								const allocator_type& alloc,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(str_view, subpos, sublen);
	}

	template<typename StringView>
	explicit utf8_string_impl(const StringView& str_view,
								size_type subpos, size_type sublen,
								const allocator_type& alloc,
								const string& src_charset_name,
								typename
									boost::enable_if
									<
										is_string_view_t< StringView >,
										sfinae_type
									>::type sfinae = 0)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(str_view, subpos, sublen, src_charset_name);
	}

	// string_charset_helper

	template<typename T1, typename T2>
	utf8_string_impl(size_type n,
						const string_charset_helper<T1, T2>& val,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(container::get_allocator(val.first)), _utf8_size(0)
	{
		this_type::assign(n, val);
	}

	template<typename T1, typename T2>
	utf8_string_impl(size_type n,
						const string_charset_helper<T1, T2>& val,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(n, val);
	}

	template<typename T1, typename T2>
	utf8_string_impl(size_type n,
						const string_charset_helper<T1, T2>& val,
						const allocator_type& alloc)
		: _base(alloc)
	{
		this_type::assign(n, val);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(container::get_allocator(val.first)), _utf8_size(0)
	{
		this_type::assign(val);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(val);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(val);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						size_type n_or_subpos,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(container::get_allocator(val.first)), _utf8_size(0)
	{
		this_type::assign(val, n_or_subpos);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						size_type n_or_subpos,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(val, n_or_subpos);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						size_type n_or_subpos,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(val, n_or_subpos);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						size_type subpos, size_type sublen,
						typename
							boost::enable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _base(container::get_allocator(val.first)), _utf8_size(0)
	{
		this_type::assign(val, subpos, sublen);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						size_type subpos, size_type sublen,
						typename
							boost::disable_if
							<
								boost::is_same
								<
									allocator_type,
									typename
										container_ex::allocator_t
										<
											typename native_t<T1>::type
										>::type
								>,
								sfinae_type
							>::type sfinae = 0)
		: _utf8_size(0)
	{
		this_type::assign(val, subpos, sublen);
	}

	template<typename T1, typename T2>
	utf8_string_impl(const string_charset_helper<T1, T2>& val,
						size_type subpos, size_type sublen,
						const allocator_type& alloc)
		: _base(alloc), _utf8_size(0)
	{
		this_type::assign(val, subpos, sublen);
	}

	~utf8_string_impl(void)
	{
	}

// don't support operator utf8_string_view_impl<value_type, triats_type>(void) const,
// because utf8_string_view_impl support utf8_string_impl in construct operator=
//public:
//	operator utf8_string_view_impl<value_type, triats_type>(void) const;

public:
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		operator=(const Char* str)
	{
		return this_type::assign(str);
	}


	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		operator=(const Basic_String<Char, Traits, Alloc>& str)
	{
		return this_type::assign(str);
	}


	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator=(const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		return this_type::assign(other);
	}

private:
	// disable this utf8_char_impl auto conv to base_type
	template<typename Char, typename Traits, typename Alloc> inline
	void operator=(const utf8_char_impl<Char, Traits, Alloc>&);

	template<typename Char, typename Traits> inline
	void operator=(const basic_utf8_char_view<Char, Traits>&);

	template<typename Utf8String> inline
	void operator=(const basic_utf8_char_reference<Utf8String>&);

	template<typename Utf8String> inline
	void operator=(const basic_utf8_char_const_reference<Utf8String>&);

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		return this_type::assign(boost::move(right));
	}

	inline this_type& operator=(const base_type& right)
	{
		return this_type::assign(right);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						basic_string<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right)
	{
		return this_type::assign(boost::move(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const basic_string<value_type, traits_type, Alloc2>& right)
	{
		return this_type::assign(right);
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		return this_type::assign(boost::move(right));
	}

	inline this_type& operator=(const this_type& right)
	{
		return this_type::assign(right);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						utf8_string_impl<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right)
	{
		return this_type::assign(boost::move(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::assign(right);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t< StringView >,
			this_type&
		>::type
		operator=(const StringView& right)
	{
		return this_type::assign(right);
	}

	template<typename T1, typename T2> inline
	this_type& operator=(const string_charset_helper<T1, T2> val)
	{
		return this_type::assign(val);
	}

public:
	// swap
private:
	//using base_type::swap;

	// disable this utf8_char_impl auto conv to base_type
	template<typename Char, typename Traits, typename Alloc> inline
	void swap(const utf8_char_impl<Char, Traits, Alloc>&) const;

	template<typename Char, typename Traits> inline
	void swap(const basic_utf8_char_view<Char, Traits>&) const;

	template<typename Utf8String> inline
	void swap(const basic_utf8_char_reference<Utf8String>&) const;

	template<typename Utf8String> inline
	void swap(const basic_utf8_char_const_reference<Utf8String>&) const;

public:
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		swap(BOOST_RV_REF_BEG Basic_String<Char, Traits, Alloc> BOOST_RV_REF_END right)
	{
		typedef Basic_String<Char, Traits, Alloc> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		swap(BOOST_RV_REF_BEG Basic_String<Char, Traits, Alloc> BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef Basic_String<Char, Traits, Alloc> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		swap(Basic_String<Char, Traits, Alloc>& right)
	{
		this_type::swap(right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		swap(Basic_String<Char, Traits, Alloc>& right, const string& src_charset_name)
	{
		typedef Basic_String<Char, Traits, Alloc> right_type;

		this_type tmp(right, _base.get_allocator(), src_charset_name);
		right = this_type::str<right_type>(right.get_allocator(), src_charset_name);

		_base.swap(tmp._base);
		_utf8_size = tmp._utf8_size;
	}

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline void swap(base_type& right)
	{
		this_type::swap(right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef basic_string<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		this_type::swap(
			reinterpret_cast<base_type&>(right_ref),
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(basic_string<value_type, traits_type, Alloc2>& right)
	{
		this_type::swap(
			reinterpret_cast<base_type&>(right),
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline void swap(BOOST_RV_REF(base_type) right, const string& src_charset_name)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref, src_charset_name);
	}

	inline void swap(base_type& right, const string& src_charset_name)
	{
		if(_base.data() == right.data())
		{
			return;
		}
		else if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			_base.swap(right);
			_utf8_size = utf8_foo::strlen(right);
		}
		else
		{
			this_type tmp(right, _base.get_allocator(), src_charset_name);
			right = this_type::str<base_type>(_base.get_allocator(), src_charset_name);

			_base.swap(tmp._base);
			_utf8_size = tmp._utf8_size;
		}
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		this_type::swap(reinterpret_cast<base_type&>(right_ref), src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(basic_string<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		this_type::swap(reinterpret_cast<base_type&>(right), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void swap(BOOST_RV_REF_BEG utf8_string_impl<Char, Traits, Alloc> BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<Char, Traits, Alloc> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Char, typename Traits, typename Alloc>  inline
	void swap(BOOST_RV_REF_BEG utf8_string_impl<Char, Traits, Alloc> BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<Char, Traits, Alloc> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void swap(utf8_string_impl<Char, Traits, Alloc>& right)
	{
		size_type utf8_size = right.length();
		base_type tmp(right.data(), right.size(), _base.get_allocator());

		right.assign(*this);

		_base.swap(tmp);
		_utf8_size = utf8_size;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void swap(utf8_string_impl<Char, Traits, Alloc>& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::swap(right);
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

		_base.swap(right._base);
		yggr::swap(_utf8_size, right._utf8_size);
	}


	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		this_type::swap(reinterpret_cast<this_type&>(right_ref));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		this_type::swap(reinterpret_cast<this_type&>(right));
	}

	inline void swap(BOOST_RV_REF(this_type) right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::swap(right);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		this_type::swap(reinterpret_cast<this_type&>(right_ref));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(utf8_string_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::swap(reinterpret_cast<this_type&>(right));
	}

	template<typename T1, typename T2> inline
	void swap(BOOST_RV_REF_BEG string_charset_helper<T1, T2> BOOST_RV_REF_END right)
	{
		this_type::swap(right.first, right.second);
	}

	template<typename T1, typename T2> inline
	void swap(string_charset_helper<T1, T2>& right)
	{
		this_type::swap(right.first, right.second);
	}

public:
	// operator string
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_type));
	}

	// iterators:

	// begins
	inline non_const_iterator ncbegin(void)
	{
		return non_const_iterator(_base.begin());
	}

	inline iterator begin(void)
	{
		return iterator(*this, 0);
	}

	inline const_iterator begin(void) const
	{
		return const_iterator(_base.begin());
	}

	inline const_iterator cbegin(void) const
	{
		return const_iterator(_base.begin());
	}

	//ends
	inline non_const_iterator ncend(void)
	{
		return non_const_iterator(_base.end());
	}

	inline iterator end(void)
	{
		return iterator(*this, _utf8_size);
	}

	inline const_iterator end(void) const
	{
		return const_iterator(_base.end());
	}

	inline const_iterator cend(void) const
	{
		return const_iterator(_base.end());
	}

	// rbegins
	inline non_const_reverse_iterator ncrbegin(void)
	{
		return non_const_reverse_iterator(this_type::ncend());
	}

	inline reverse_iterator rbegin(void)
	{
		return reverse_iterator(this_type::end());
	}

	inline const_reverse_iterator rbegin(void) const
	{
		return const_reverse_iterator(this_type::end());
	}

	inline const_reverse_iterator crbegin(void) const
	{
		return const_reverse_iterator(this_type::cend());
	}

	// rends
	inline non_const_reverse_iterator ncrend(void)
	{
		return non_const_reverse_iterator(this_type::ncbegin());
	}

	inline reverse_iterator rend(void)
	{
		return reverse_iterator(this_type::begin());
	}

	inline const_reverse_iterator rend(void) const
	{
		return const_reverse_iterator(this_type::begin());
	}

	inline const_reverse_iterator crend(void) const
	{
		return const_reverse_iterator(this_type::cbegin());
	}

	// capacity:

	inline size_type size(void) const
	{
		return _base.size();
	}

	inline size_type length(void) const
	{
		return _utf8_size;
	}

	inline size_type max_size(void) const
	{
		return _base.max_size();
	}

	inline void resize(size_type n) // n is byte_size
	{
		this_type::resize(n, value_type(0));
	}

	void resize(size_type n, value_type c)
	{
		if(n == 0)
		{
			this_type::clear();
		}
		else if(n < _base.size())
		{
			iterator iter = this_type::end();
			size_type rm_len = 0;
			size_type rm_size = 0;

			size_type add_len = 0;

			for(;n < (rm_size = std::distance(_base.begin(), iter.org())); --iter, ++rm_len);

			assert(rm_size <= n);
			assert((*(_base.data() + rm_size) & 0xc0) != 0x80);
			assert(n - rm_size <= utf8_foo::char_max_size());

			if(rm_size < n)
			{
				add_len = n - rm_size;
				memset(&_base.at(rm_size), c, add_len);
			}

			_base.erase(n);
			_utf8_size = _utf8_size + add_len - rm_len;
		}
		else if(n > _base.size())
		{
			size_type add_size = n - _base.size();
			_base.append(add_size, c);
			_utf8_size += add_size;
		}
	}

	template<typename Operation> inline
	void resize_and_overwrite(size_type count, const Operation& op)
	{
		if(_base.size() < count)
		{
			_base.resize(count, 0);
		}

		size_type new_resize = op(const_cast<char*>(_base.data()), count);
		_base.resize(new_resize);
		this_type::recount_length();
	}

	inline void relength(size_type len) // len is utf8_length
	{
		if(len == 0)
		{
			this_type::clear();
		}
		else if(len < _utf8_size) // <
		{
			iterator iter = this_type::end() - (_utf8_size - len);
			_base.erase(iter.org(), _base.end());
			_utf8_size = len;
		}
		else if(len > _utf8_size) // >
		{
			this_type::append(len - _utf8_size, value_type(0));
		}
	}

	template<typename T> inline
	void relength(size_type len, const T& c)
	{
		return this_type::relength(len, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(T));
	}

	template<typename T> inline
	void relength(size_type len, const T& c,
					const string& src_charset_name)
	{
		if(len == 0)
		{
			this_type::clear();
		}
		else if(len < _utf8_size) // <
		{
			iterator iter = this_type::end() - (_utf8_size - len);
			_base.erase(iter.org(), _base.end());
			_utf8_size = len;
		}
		else if(len > _utf8_size) // >
		{
			this_type::append(len - _utf8_size, c, src_charset_name);
		}
	}

	template<typename T1, typename T2> inline
	void relength(size_type len, const string_charset_helper<T1, T2>& c)
	{
		if(len == 0)
		{
			this_type::clear();
		}
		else if(len < _utf8_size) // <
		{
			iterator iter = this_type::end() - (_utf8_size - len);
			_base.erase(iter.org(), _base.end());
			_utf8_size = len;
		}
		else if(len > _utf8_size) // >
		{
			this_type::append(len - _utf8_size, c.first, c.second);
		}
	}

	template<typename Operation> inline
	void relength_and_overwrite(size_type len, const Operation& op)
	{
		if(_utf8_size < len)
		{
			_base.resize(_base.size() + ((len - _utf8_size) *  utf8_foo::E_char_max_size / sizeof(value_type)));
		}

		size_type new_size = op(const_cast<char*>(_base.data()), _base.size(), len);

		assert(new_size <= _base.size());

		if(new_size < _base.size())
		{
			_base.resize(new_size);
		}

		this_type::recount_length();
	}

	inline size_type capacity(void) const
	{
		return _base.capacity();
	}

	inline void reserve(size_type n = 0)
	{
		_base.reserve(n);
	}

	inline void clear(void)
	{
		_base.clear();
		_utf8_size = 0;
	}

	inline bool empty(void) const
	{
		return !_utf8_size;
	}

	inline void shrink_to_fit(void)
	{
		::yggr::charset::shrink_to_fit(_base);
	}

	inline bool is_validate(void) const
	{
		return _utf8_size == utf8_foo::strlen(_base);
	}

	// element access:
	inline reference operator[](size_type idx) YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(idx < _utf8_size);
		return reference(*this, idx);
	}

	inline const_reference operator[](size_type idx) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(idx < _utf8_size);
		return const_reference(*this, idx);
	}

	inline reference at(size_type idx)
	{
		if(!(idx < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::at invalid subscript");
		}
		return reference(*this, idx);
	}

	inline const_reference at(size_type idx) const
	{
		if(!(idx < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::at invalid subscript");
		}
		return const_reference(*this, idx);
	}

	inline reference front(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(_utf8_size);
		return reference(*this, 0);
	}

	inline const_reference front(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(_utf8_size);
		return const_reference(*this, 0);
	}

	inline reference back(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(_utf8_size);
		return reference(*this, _utf8_size -1);
	}

	inline const_reference back(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(_utf8_size);
		return const_reference(*this, _utf8_size -1);
	}

	//modifiers:
	// +=
	// use non member foo

	// append
	// append n, char

	// if using src_charset_name default value, C1001 will likely occur,
	// so need write twins function

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		append(size_type n, Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::append(n, static_cast<now_char_type>(c),
									YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		append(size_type n, Char c, const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(n)
			{
				_base.append(n, static_cast<value_type>(c));
				_utf8_size += n;
			}

			return *this;
		}
		else
		{
			return
				this_type::append(
					n, char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(size_type n, const Char* c)
	{
		return this_type::append(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(size_type n, const Char* c, const string& src_charset_name)
	{
		assert(c);
		if(!(c && c[0]))
		{
			if(n)
			{
				_base.append(n, value_type(0));
				_utf8_size += n;
			}
			return *this;
		}
		else
		{
			return
				this_type::append(
					n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(size_type n, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::append(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(size_type n, const Basic_String<Char, Traits, Alloc>& c,
				const string& src_charset_name)
	{
		if(c.empty() || 0 == c[0])
		{
			if(n)
			{
				_base.append(n, value_type(0));
				_utf8_size += n;
			}
			return *this;
		}
		else
		{
			return
				this_type::append(
					n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::append(n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(size_type n, const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(n, c.view());
	}

	template<typename Char, typename Traits>
	this_type& append(size_type n, const basic_utf8_char_view<Char, Traits>& c)
	{
		if(n)
		{
			const value_type* ptr = c.data();
			size_type step = c.size();
			assert(step);

			if(step == 1)
			{
				_base.append(n, *ptr);
			}
			else
			{
				base_type tmp(n * step, value_type(0), _base.get_allocator());
				for(size_type i = 0, spos = 0; i != n; ++i, spos += step)
				{
					memcpy(&tmp[spos], ptr, step);
				}

				if(_utf8_size)
				{
					_base.append(tmp);
				}
				else
				{
					_base.swap(tmp);
				}
			}

			_utf8_size += n;
		}
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& append(size_type n, const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(n, c);
	}

	template<typename Utf8String> inline
	this_type& append(size_type n, const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::append(n, c.view());
	}

	template<typename Utf8String> inline
	this_type& append(size_type n, const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(n, c.view());
	}

	template<typename Utf8String> inline
	this_type& append(size_type n, const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::append(n, c.view());
	}

	template<typename Utf8String> inline
	this_type& append(size_type n, const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(n, c.view());
	}

	// append const char*

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(const Char* src)
	{
		return this_type::append(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(const Char* src, const string& src_charset_name)
	{
		assert(src);
		if(!(src && src[0]))
		{
			return *this;
		}

		size_type add_size = 0;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			const char* utf8_src = reinterpret_cast<const char*>(src);

			assert(utf8_foo::is_utf8_string(utf8_src));
			add_size = utf8_foo::strlen(utf8_src);
			_base.append(utf8_src);
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size += add_size;
		return *this;
	}

	// append const char*, n

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(const Char* src, size_type n)
	{
		return this_type::append(src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(const Char* src, size_type n,
				const string& src_charset_name)
	{
		if(!n)
		{
			return *this;
		}

		assert(src);
		if(!(src))
		{
			return *this;
		}

		base_type tmp(_base.get_allocator());
		charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(tmp);

		if(_utf8_size)
		{
			_base.append(tmp);
		}
		else
		{
			_base.swap(tmp);
		}

		_utf8_size += add_size;
		return *this;
	}

	// append string

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::append(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<value_type, Traits, Alloc>& src,
				const string& src_charset_name)
	{
		if(src.empty())
		{
			return *this;
		}

		if(src.data() == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			_base.append(src.begin(), src.end());
			_utf8_size = _utf8_size << 1;
			return *this;
		}

		size_type add_size = 0;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(utf8_foo::is_utf8_string(src));
			add_size = utf8_foo::strlen(src);
			_base.append(src.begin(), src.end());
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<Char, Traits, Alloc>& src,
				const string& src_charset_name)
	{
		if(src.empty())
		{
			return *this;
		}

		base_type tmp(_base.get_allocator());
		charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(tmp);

		if(_utf8_size)
		{
			_base.append(tmp);
		}
		else
		{
			_base.swap(tmp);
		}

		_utf8_size += add_size;
		return *this;
	}

	// append (string, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<Char, Traits, Alloc>& src, size_type subpos)
	{
		return this_type::append(src, subpos, src.size() - subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos,
				const string& src_charset_name)
	{
		return this_type::append(src, subpos, src.size() - subpos, src_charset_name);
	}

	// append (string, pos, len)

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(const Char* src, size_type subpos, size_type sublen)
	{
		return this_type::append(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(const Char* src, size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if(!sublen)
		{
			return *this;
		}

		assert(src);

		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src + subpos, sublen, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(right);

		if(_utf8_size)
		{
			_base.append(right);
		}
		else
		{
			_base.swap(right);
		}

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen)
	{
		return this_type::append(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if((subpos == src.size()) || (!sublen))
		{
			return *this;
		}

		if(!(subpos < src.size()))
		{
			throw std::out_of_range("basic_utf8_string_view::append invalid subscript");
		}
		assert(sublen);
		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(right);

		if(_utf8_size)
		{
			_base.append(right);
		}
		else
		{
			_base.swap(right);
		}

		_utf8_size += add_size;
		return *this;
	}

	// append utf8_string
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		if(!other.empty())
		{
			_base.append(other.org_str().begin(), other.org_str().end());
			_utf8_size += other.length();
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(other);
	}

	// append (utf8_string, subpos)
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos)
	{
		return this_type::append(other, subpos, other.length() - subpos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(other, subpos, other.length() - subpos);
	}

	// append (utf8_string, subpos, sublen)
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
	{
		typedef utf8_string_impl<Char, Traits, Alloc> other_type;
		typedef typename other_type::const_iterator other_citer_type;

		if(!(subpos == other.length() || !sublen))
		{
			if(!(subpos < other.length()))
			{
				throw std::out_of_range("basic_utf8_string_view::append invalid subscript");
			}
			assert(sublen);

			sublen = (std::min)(other.length() - subpos, sublen);

			other_citer_type start = other.begin() + subpos;
			other_citer_type last = start + sublen;

			_base.append(start.org(), last.org());
			_utf8_size += sublen;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(other, subpos, sublen);
	}

public:
	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			this_type&
		>::type
		append(InputIter start, InputIter last)
	{
		typedef InputIter iter_type;
		typedef detail::iterator_append_helper<iter_type> h_type;
		typedef typename boost::iterator_value<InputIter>::type cv_now_char_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_now_char_type) now_char_type;

		h_type h;
		return h(*this, start, last, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) );
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			this_type&
		>::type
		append(InputIter start, InputIter last,
				const string& src_charset_name)
	{
		typedef InputIter iter_type;
		typedef detail::iterator_append_helper<iter_type> h_type;

		h_type h;
		return h(*this, start, last, src_charset_name);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& append(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& append(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& append(utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& append(utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& append(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& append(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		append(ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		append(ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		append(ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		append(ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		append(ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		append(ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append_of_this_riterator(start, last);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		append(std::initializer_list<Char> il)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::append(
				boost::begin(il),
				boost::end(il),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		append(std::initializer_list<Char> il, const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::append(boost::begin(il), boost::end(il), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(std::initializer_list<Char*> il)
	{
		return this_type::append(il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		append(std::initializer_list<Char*> il, const string& src_charset_name)
	{
		typedef std::initializer_list<Char*> il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(this_type::get_allocator());
			tmp.reserve(add_size << 2); // add_size * 4

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					assert(*i);
					assert(1 == utf8_foo::strlen(*i));
					tmp.append(reinterpret_cast<const value_type*>(*i),
								utf8_foo::charlen(reinterpret_cast<const value_type*>(*i)));
				}
			}
			else
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					assert(*i);
					tmp.append(char_type(*i, this_type::get_allocator(), src_charset_name).org_str());
				}
			}

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(std::initializer_list< Basic_String<Char, Traits, Alloc> > il)
	{
		return this_type::append(il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		append(std::initializer_list< Basic_String<Char, Traits, Alloc> > il,
				const string& src_charset_name)
	{
		typedef std::initializer_list< Basic_String<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(this_type::get_allocator());
			tmp.reserve(add_size << 2); // add_size * 4

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					if((*i).empty())
					{
						tmp.append(1, value_type(0));
					}
					else
					{
						assert(1 == utf8_foo::strlen(*i));
						tmp.append(reinterpret_cast<const value_type*>((*i).data()),
									utf8_foo::charlen(reinterpret_cast<const value_type*>((*i).data())));
					}
				}
			}
			else
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					tmp.append(char_type(*i, this_type::get_allocator(), src_charset_name).org_str());
				}
			}

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il)
	{
		typedef std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& append(std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(il);
	}

	template<typename Char, typename Traits>
	this_type& append(std::initializer_list< basic_utf8_char_view<Char, Traits> > il)
	{
		typedef std::initializer_list< basic_utf8_char_view<Char, Traits> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& append(std::initializer_list< basic_utf8_char_view<Char, Traits> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(il);
	}

	template<typename Utf8String>
	this_type& append(std::initializer_list< basic_utf8_char_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return *this;
	}

	template<typename Utf8String> inline
	this_type& append(std::initializer_list< basic_utf8_char_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(il);
	}

	template<typename Utf8String>
	this_type& append(std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return *this;
	}

	template<typename Utf8String> inline
	this_type& append(std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(il);
	}

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

private:

// disable this utf8_char_impl auto conv to base_type
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Char, typename Traits, typename Alloc \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void append(const utf8_char_impl<Char, Traits, Alloc>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Char, typename Traits \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void append(const basic_utf8_char_view<Char, Traits>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void append(const basic_utf8_char_reference<Utf8String>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void append(const basic_utf8_char_const_reference<Utf8String>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) );

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

public:

#if !(defined(_DEBUG) && defined(_YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY))
	inline this_type& append(BOOST_RV_REF(base_type) src)
	{
		return this_type::append(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& append(BOOST_RV_REF(base_type) src, const string& src_charset_name)
	{
		if(src.empty())
		{
			return *this;
		}

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			base_type& src_ref = src;
			if(&_base == &src_ref)
			{
				_base.append(src);
				_utf8_size <<= 1; // _utf8_size * 2
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				size_type add_size = utf8_foo::strlen(src);
				if(_utf8_size)
				{
					_base.append(src);
				}
				else
				{
					copy_or_move_or_swap(_base, boost::move(src));
				}
				_utf8_size += add_size;
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, boost::move(src), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}
		return *this;
	}

	inline this_type& append(const base_type& src)
	{
		return this_type::append(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& append(const base_type& src, const string& src_charset_name)
	{
		if(src.empty())
		{
			return *this;
		}

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			_base.append(src);

			if(&_base == &src)
			{
				_utf8_size <<= 1; // _utf8_size * 2
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				_utf8_size += utf8_foo::strlen(src);
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				_base.append(tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}
		return *this;
	}

	// append (isomeric_same_string, pos)
	inline this_type& append(const base_type& src, size_type subpos)
	{
		return this_type::append(src, subpos, src.size() - subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& append(const base_type& src,
								size_type subpos,
								const string& src_charset_name)
	{
		return this_type::append(src, subpos, src.size() - subpos, src_charset_name);
	}

	// append (isomeric_same_string, pos, len)
	inline this_type& append(const base_type& src,
								size_type subpos, size_type sublen)
	{
		return this_type::append(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& append(const base_type& src,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		if((subpos == src.size()) || (!sublen))
		{
			return *this;
		}

		if(!(subpos < src.size()))
		{
			throw std::out_of_range("basic_utf8_string_view::append invalid subscript");
		}
		assert(sublen);
		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(right);

		if(_utf8_size)
		{
			_base.append(right);
		}
		else
		{
			_base.swap(right);
		}

		_utf8_size += add_size;
		return *this;
	}

	// append (isomeric_same_base_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::append(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::append(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(const basic_string<value_type, traits_type, Alloc2>& src)
	{
		return
			this_type::append(
				reinterpret_cast<const base_type&>(src),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(const basic_string<value_type, traits_type, Alloc2>& src,
				const string& src_charset_name)
	{
		return
			this_type::append(
				reinterpret_cast<const base_type&>(src),
				src_charset_name);
	}

	// append (isomeric_same_string, pos)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos)
	{
		return
			this_type::append(
				reinterpret_cast<const base_type&>(src),
				subpos, src.size() - subpos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos,
				const string& src_charset_name)
	{
		return
			this_type::append(
				reinterpret_cast<const base_type&>(src),
				subpos, src.size() - subpos,
				src_charset_name);
	}

	// append (isomeric_same_string, pos, len)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen)
	{
		return
			this_type::append(
				reinterpret_cast<const base_type&>(src),
				subpos, sublen,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		append(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		return
			this_type::append(
				reinterpret_cast<const base_type&>(src),
				subpos, sublen, src_charset_name);
	}

	// append(this_type)
	inline this_type& append(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			_base.append(const_cast<const base_type&>(right._base));
			_utf8_size <<= 1; // _utf8_size *= 2;
		}
		else
		{
			if(_utf8_size)
			{
				_base.append(const_cast<const base_type&>(right._base));
			}
			else
			{
				copy_or_move_or_swap(_base, boost::move(right._base));
			}

			_utf8_size += boost::move(right._utf8_size);
		}

		return *this;
	}

	inline this_type& append(BOOST_RV_REF(this_type) right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(boost::move(right));
	}

	inline this_type& append(const this_type& right)
	{
		_base.append(right._base);
		_utf8_size += right._utf8_size;
		return *this;
	}

	inline this_type& append(const this_type& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(right);
	}

	// append(this_type, pos)
	inline this_type& append(const this_type& right, size_type subpos)
	{
		return this_type::append(right, subpos, right._utf8_size - subpos);
	}

	inline this_type& append(const this_type& right, size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(right, subpos, right._utf8_size - subpos);
	}

	// append(this_type, pos, len)
	inline this_type& append(const this_type& right, size_type subpos, size_type sublen)
	{
		if(subpos == right._utf8_size || !sublen)
		{
			return *this;
		}

		//assert(subpos < right._utf8_size);
		if(!(subpos < right._utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::append invalid subscript");
		}
		assert(sublen);

		sublen = (std::min)(right._utf8_size - subpos, sublen);

		const_iterator start = right.begin() + subpos;
		const_iterator last = start + sublen;

		_base.append(start.org(), last.org());
		_utf8_size += sublen;
		return *this;
	}

	inline this_type& append(const this_type& right, size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(right, subpos, sublen);
	}

	// append(isomeric_same_this_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;
		right_type& right_ref = right;
		return this_type::append(boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		return this_type::append(boost::move(reinterpret_cast<this_type&>(right)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(const utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::append(reinterpret_cast<const this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(reinterpret_cast<const this_type&>(right));
	}

	// append(isomeric_same_this_type, pos)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(const utf8_string_impl<value_type, traits_type, Alloc2>& right, size_type subpos)
	{
		return
			this_type::append(
				reinterpret_cast<const this_type&>(right),
				subpos, right.length() - subpos);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::append(
				reinterpret_cast<const this_type&>(right),
				subpos, right.length() - subpos);
	}

	// append(isomeric_same_this_type, pos, len)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen)
	{
		return
			this_type::append(
				reinterpret_cast<const this_type&>(right),
				subpos, sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		append(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::append(reinterpret_cast<const this_type&>(right), subpos, sublen);
	}

#endif // _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

	// string_view

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::append(
				str_view.data(), str_view.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, const string& src_charset_name)
	{
		return
			this_type::append(
				str_view.data(), str_view.size(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos, size_type sublen)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos, size_type sublen, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view)
	{
		return
			this_type::append(
				str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return
			this_type::append(
				str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos, size_type sublen)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			this_type&
		>::type
		append(const StringView& str_view, size_type subpos, size_type sublen, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::append of range position");
		}

		return
			this_type::append(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				src_charset_name);
	}

	// append string_charset_helper
	template<typename T1, typename T2> inline
	this_type& append(size_type n, const string_charset_helper<T1, T2>& val)
	{
		return this_type::append(n, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& append(const string_charset_helper<T1, T2>& val)
	{
		return this_type::append(val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& append(const string_charset_helper<T1, T2>& val, size_type n_or_subpos)
	{
		return this_type::append(val.first, n_or_subpos, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& append(const string_charset_helper<T1, T2>& val, size_type subpos, size_type sublen)
	{
		return this_type::append(val.first, subpos, sublen, val.second);
	}

	//operator +=

	// += char
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		operator+=(Char r)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;
		return this_type::append(1, r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator+=(const utf8_char_impl<Char, Traits, Alloc>& r)
	{
		return this_type::append(1, r);
	}

	template<typename Char, typename Traits> inline
	this_type& operator+=(const basic_utf8_char_view<Char, Traits>& r)
	{
		return this_type::append(1, r);
	}

	template<typename Utf8String> inline
	this_type& operator+=(const basic_utf8_char_const_reference<Utf8String>& r)
	{
		return this_type::append(1, r);
	}

	// += const char*
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		operator+=(const Char* r)
	{
		return this_type::append(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename T> inline
	this_type& operator+=(std::initializer_list<T> il)
	{
		return this_type::append(il);
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// += string

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		operator+=(const Basic_String<Char, Traits, Alloc>& r)
	{
		return this_type::append(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	// += this

	template<typename Char, typename Traits, typename Alloc > inline
	this_type& operator+=(const utf8_string_impl<Char, Traits, Alloc>& r)
	{
		return this_type::append(r);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			this_type&
		>::type
		operator+=(const StringView& r)
	{
		return this_type::append(r);
	}

	// += string_charset_helper
	template<typename T1, typename T2> inline
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				is_can_be_conv_into_char<T1>,
				is_utf8_char_t< typename native_t<T1>::type >
			>,
			this_type&
		>::type
		operator+=(const string_charset_helper<T1, T2>& r)
	{
		return this_type::append(r);
	}

	template<typename T1, typename T2> inline
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_can_be_conv_into_char<T1>,
				is_utf8_char_t< typename native_t<T1>::type >
			>,
			this_type&
		>::type
		operator+=(const string_charset_helper<T1, T2>& r)
	{
		return this_type::append(1, r);
	}

	// push_back

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			void
		>::type
	push_back(Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::append(1, static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			void
		>::type
	push_back(Char c, const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::append(1, static_cast<now_char_type>(c), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			void
		>::type
	push_back(const Char* c)
	{
		this_type::append(1, char_type(c, this_type::get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			void
		>::type
	push_back(const Char* c, const string& src_charset_name)
	{
		this_type::append(1, char_type(c, this_type::get_allocator(), src_charset_name));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		push_back(const Basic_String<Char, Traits, Alloc>& c)
	{
		this_type::append(1, char_type(c, this_type::get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		push_back(const Basic_String<Char, Traits, Alloc>& c,
					const string& src_charset_name)
	{
		this_type::append(1, char_type(c, this_type::get_allocator(), src_charset_name));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void push_back(const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		this_type::append(1, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void push_back(const utf8_char_impl<Char, Traits, Alloc>& c,
					const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::append(1, c.view());
	}

	template<typename Char, typename Traits> inline
	void push_back(const basic_utf8_char_view<Char, Traits>& c)
	{
		this_type::append(1, c);
	}

	template<typename Char, typename Traits> inline
	void push_back(const basic_utf8_char_view<Char, Traits>& c,
					const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::append(1, c);
	}

	template<typename Utf8String> inline
	void push_back(const basic_utf8_char_reference<Utf8String>& c)
	{
		this_type::append(1, c.view());
	}

	template<typename Utf8String> inline
	void push_back(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::append(1, c.view());
	}

	template<typename Utf8String> inline
	void push_back(const basic_utf8_char_const_reference<Utf8String>& c)
	{
		this_type::append(1, c.view());
	}

	template<typename Utf8String> inline
	void push_back(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::append(1, c.view());
	}

	// push_back string_charset_helper
	template<typename T1, typename T2> inline
	void push_back(const string_charset_helper<T1, T2>& val)
	{
		return this_type::push_back(val.first, val.second);
	}

	inline void pop_back(void)
	{
		if(_utf8_size)
		{
			this_type::erase(this_type::end() - 1);
		}
	}

	// assign
	// assign n, char

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		assign(size_type n, Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::assign(n, static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		assign(size_type n, Char c, const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			basic_string_assign(_base, n, static_cast<value_type>(c));
			_utf8_size = n;
			return *this;
		}
		else
		{
			return
				this_type::assign(
					n, char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(size_type n, const Char* c)
	{
		return this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(size_type n, const Char* c, const string& src_charset_name)
	{
		assert(c);
		if(!(c && c[0]))
		{
			if(n)
			{
				basic_string_assign(_base, n, value_type(0));
			}
			else
			{
				_base.clear();
			}
			_utf8_size = n;
			return *this;

		}
		else
		{
			return this_type::assign(n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(size_type n, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::assign(n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(size_type n, const Basic_String<Char, Traits, Alloc>& c,
				const string& src_charset_name)
	{
		if(c.empty() || !c[0])
		{
			if(n)
			{
				basic_string_assign(_base, n, value_type(0));
			}
			else
			{
				_base.clear();
			}

			_utf8_size = n;
			return *this;
		}
		else
		{
			return this_type::assign(n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::assign(n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(size_type n, const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(n, c.view());
	}

	template<typename Char, typename Traits> inline
	this_type& assign(size_type n, const basic_utf8_char_view<Char, Traits>& c)
	{
		if(n)
		{
			const value_type* ptr = c.data();
			size_type step = c.size();
			assert(step);

			if(step == 1)
			{
				basic_string_assign(_base, n, *ptr);
			}
			else
			{
				base_type tmp(n * step, value_type(0), _base.get_allocator());
				for(size_type i = 0, spos = 0; i != n; ++i, spos += step)
				{
					memcpy(&tmp[spos], ptr, step);
				}

				_base.swap(tmp);
			}
		}
		else
		{
			_base.clear();
		}

		_utf8_size = n;
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& assign(size_type n, const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(n, c);
	}

	template<typename Utf8String> inline
	this_type& assign(size_type n, const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::assign(n, c.view());
	}

	template<typename Utf8String> inline
	this_type& assign(size_type n, const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(n, c.view());
	}

	template<typename Utf8String> inline
	this_type& assign(size_type n, const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::assign(n, c.view());
	}

	template<typename Utf8String> inline
	this_type& assign(size_type n, const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(n, c.view());
	}

	// assign const char*
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src)
	{
		return this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src, const string& src_charset_name)
	{
		assert(src);
		if(!(src && src[0]))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				const char* utf8_src = reinterpret_cast<const char*>(src);

				assert(utf8_foo::is_utf8_string(utf8_src));

				basic_string_assign(_base, utf8_src);
			}
			else
			{
				base_type right(_base.get_allocator());
				charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				_base.swap(right);
			}

			_utf8_size = utf8_foo::strlen(_base);
		}

		return *this;
	}

	// assign const char*, n

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src, size_type n)
	{
		return this_type::assign(src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src, size_type n,
				const string& src_charset_name )
	{
		if(!n)
		{
			_base.clear();
			_utf8_size = 0;
			return *this;
		}

		assert(src);
		if(!(src))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			_base.swap(right);
			_utf8_size = utf8_foo::strlen(_base);
		}

		return *this;
	}

	// assign const string&
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<value_type, Traits, Alloc>& src,
				const string& src_charset_name)
	{
		if(_base.data() == src.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return *this;
		}

		if(src.empty())
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));

				basic_string_assign(_base, src.begin(), src.end());
			}
			else
			{
				base_type right(_base.get_allocator());
				charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				_base.swap(right);
			}
			_utf8_size = utf8_foo::strlen(_base);
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src,
				const string& src_charset_name)
	{
		if(src.empty())
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			_base.swap(right);
			_utf8_size = utf8_foo::strlen(_base);
		}

		return *this;
	}

	// assign const string&, pos
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src, size_type subpos)
	{
		return this_type::assign(src, subpos, src.size() - subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src, size_type subpos,
				const string& src_charset_name)
	{
		return this_type::assign(src, subpos, src.size() - subpos, src_charset_name);
	}

	// assign const string&, pos, len
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src, size_type subpos, size_type sublen)
	{
		return this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src, size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if(!sublen)
		{
			_base.clear();
			_utf8_size = 0;
		}

		assert(src);
		if(!src)
		{
			_base.clear();
			_utf8_size = 0;
			return *this;
		}

		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src + subpos, sublen, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		_base.swap(right);
		_utf8_size = utf8_foo::strlen(_base);

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos, size_type sublen)
	{
		return this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if((subpos == src.size()) || (!sublen))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			//assert(subpos < src.size());
			if(!(subpos < src.size()))
			{
				throw std::out_of_range("basic_utf8_string_view::assign invalid subscript");
			}
			assert(sublen);
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			_base.swap(right);
			_utf8_size = utf8_foo::strlen(_base);
			
             if(!_utf8_size)
             {
                throw charset_error_def::error_maker_type::make_error(charset_error_def::error_maker_type::E_charset_conver_failed);
             }
		}

		return *this;
	}

	// assign const utf8_string
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		basic_string_assign(_base, other.org_str().begin(), other.org_str().end());
		_utf8_size = other.length();
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(other);
	}

	// assign utf8_string, pos
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos)
	{
		if(subpos == other.length())
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			//assert(subpos < other.length());
			if(!(subpos < other.length()))
			{
				throw std::out_of_range("basic_utf8_string_view::assign invalid subscript");
			}
			basic_string_assign(_base, (other.begin() + subpos).org(), other.end().org());
			_utf8_size = other.length() - subpos;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(other, subpos);
	}

	// assign utf8_string, pos, len
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
	{
		typedef utf8_string_impl<Char, Traits, Alloc> other_string_type;
		typedef typename other_string_type::const_iterator other_string_citer_type;

		if((subpos == other.length()) || (!sublen))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			//assert(subpos < other.length());
			if(!(subpos < other.length()))
			{
				throw std::out_of_range("basic_utf8_string_view::assign invalid subscript");
			}
			assert(sublen);

			sublen = (std::min)(other.length() - subpos, sublen);

			other_string_citer_type start = other.begin() + subpos;
			other_string_citer_type last = start + sublen;

			basic_string_assign(_base, start.org(), last.org());
			_utf8_size = sublen;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(other, subpos, sublen);
	}

	// assign iter, iter
public:
	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			this_type&
		>::type
		assign(InputIter start, InputIter last)
	{
		typedef typename boost::iterator_value<InputIter>::type cv_now_char_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_now_char_type) now_char_type;
		typedef InputIter iter_type;
		typedef detail::iterator_assign_helper<iter_type> h_type;

		h_type h;
		return h(*this, start, last, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			this_type&
		>::type
		assign(InputIter start, InputIter last,
				const string& src_charset_name)
	{
		typedef InputIter iter_type;
		typedef detail::iterator_assign_helper<iter_type> h_type;

		h_type h;
		return h(*this, start, last, src_charset_name);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		assign(ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last)
	{
		return this_type::assign_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		assign(ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		assign(ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last)
	{
		return this_type::assign_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		assign(ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_this_riterator(start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		assign(ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last)
	{
		return this_type::assign_of_this_riterator(start, last);
	}


	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		assign(ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_this_riterator(start, last);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		assign(std::initializer_list<Char> il)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::assign(
				boost::begin(il),
				boost::end(il),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		assign(std::initializer_list<Char> il, const string& src_charset_name)
	{
		return this_type::assign(boost::begin(il), boost::end(il), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(std::initializer_list<Char*> il)
	{
		return this_type::assign(il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(std::initializer_list<Char*> il, const string& src_charset_name)
	{
		typedef std::initializer_list<Char*> il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type il_size = il.size();

		if(il_size)
		{
			base_type tmp(this_type::get_allocator());
			tmp.reserve(il_size << 2); // add_size * 4

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					assert(*i);
					assert(1 == utf8_foo::strlen(*i));
					tmp.append(reinterpret_cast<const value_type*>(*i),
								utf8_foo::charlen(reinterpret_cast<const value_type*>(*i)));
				}
			}
			else
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					assert(*i);
					tmp.append(char_type(*i, this_type::get_allocator(), src_charset_name).org_str());
				}
			}

			_base.swap(tmp);
		}
		else
		{
			_base.clear();
		}

		_utf8_size = il_size;

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(std::initializer_list< Basic_String<Char, Traits, Alloc> > il)
	{
		return this_type::assign(il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(std::initializer_list< Basic_String<Char, Traits, Alloc> > il,
				const string& src_charset_name)
	{
		typedef std::initializer_list< Basic_String<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type il_size = il.size();

		if(il_size)
		{
			base_type tmp(this_type::get_allocator());
			tmp.reserve(il_size << 2); // add_size * 4

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					if((*i).empty())
					{
						tmp.append(1, value_type(0));
					}
					else
					{
						assert(1 == utf8_foo::strlen(*i));
						tmp.append(reinterpret_cast<const value_type*>((*i).data()),
									utf8_foo::charlen(reinterpret_cast<const value_type*>((*i).data())));
					}
				}
			}
			else
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					tmp.append(char_type(*i, this_type::get_allocator(), src_charset_name).org_str());
				}
			}

			_base.swap(tmp);
		}
		else
		{
			_base.clear();
		}

		_utf8_size = il_size;

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il)
	{
		typedef std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type il_size = il.size();

		if(il_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve(il_size << 2); // il_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			_base.swap(tmp);
		}
		else
		{
			_base.clear();
		}

		_utf8_size = il_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(il);
	}

	template<typename Char, typename Traits>
	this_type& assign(std::initializer_list< basic_utf8_char_view<Char, Traits> > il)
	{
		typedef std::initializer_list< basic_utf8_char_view<Char, Traits> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type il_size = il.size();

		if(il_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve(il_size << 2); // il_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			_base.swap(tmp);
		}
		else
		{
			_base.clear();
		}

		_utf8_size = il_size;
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& assign(std::initializer_list< basic_utf8_char_view<Char, Traits> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(il);
	}

	template<typename Utf8String>
	this_type& assign(std::initializer_list< basic_utf8_char_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type il_size = il.size();

		if(il_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve(il_size << 2); // il_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			_base.swap(tmp);
		}
		else
		{
			_base.clear();
		}

		_utf8_size = il_size;
		return *this;
	}

	template<typename Utf8String> inline
	this_type& assign(std::initializer_list< basic_utf8_char_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(il);
	}

	template<typename Utf8String>
	this_type& assign(std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type il_size = il.size();

		if(il_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve(il_size << 2); // il_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			_base.swap(tmp);
		}
		else
		{
			_base.clear();
		}

		_utf8_size = il_size;
		return *this;
	}

	template<typename Utf8String> inline
	this_type& assign(std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(il);
	}


#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

private:

// disable this utf8_char_impl auto conv to base_type
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Char, typename Traits, typename Alloc \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void assign(const utf8_char_impl<Char, Traits, Alloc>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Char, typename Traits \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void assign(const basic_utf8_char_view<Char, Traits>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void assign(const basic_utf8_char_reference<Utf8String>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void assign(const basic_utf8_char_const_reference<Utf8String>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) );

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

public:

#if !(defined(_DEBUG) && defined(_YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY))
	// assign const base_type&
	inline this_type& assign(BOOST_RV_REF(base_type) src)
	{
		return this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& assign(BOOST_RV_REF(base_type) src, const string& src_charset_name)
	{
		base_type& src_ref = src;
		if(&_base == &src_ref)
		{
			return *this;
		}

		if(src.empty())
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));
				copy_or_move_or_swap(_base, boost::move(src));
			}
			else
			{
				base_type right(_base.get_allocator());
				charset_foo::s_xchg(right, boost::move(src), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				_base.swap(right);
			}

			_utf8_size = utf8_foo::strlen(_base);
		}

		return *this;
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_type;

		src_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_type;

		src_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				src_charset_name);
	}

	inline this_type& assign(const base_type& src)
	{
		return this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& assign(const base_type& src, const string& src_charset_name)
	{
		if(&_base == &src)
		{
			return *this;
		}

		if(src.empty())
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));

				basic_string_assign(_base, src);
			}
			else
			{
				base_type right(_base.get_allocator());
				charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				_base.swap(right);
			}
			_utf8_size = utf8_foo::strlen(_base);
		}

		return *this;
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const basic_string<value_type, traits_type, Alloc2>& src)
	{
		return
			this_type::assign(
				reinterpret_cast<const base_type&>(src),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const basic_string<value_type, traits_type, Alloc2>& src,
				const string& src_charset_name)
	{
		return
			this_type::assign(
				reinterpret_cast<const base_type&>(src),
				src_charset_name);
	}

	// assign base, pos
	inline this_type& assign(BOOST_RV_REF(base_type) src, size_type subpos)
	{
		return
			this_type::assign(
				boost::move(src), subpos, src.size() - subpos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& assign(BOOST_RV_REF(base_type) src, size_type subpos, const string& src_charset_name)
	{
		return this_type::assign(boost::move(src), subpos, src.size() - subpos, src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				size_type subpos)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_type;

		src_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				subpos,
				src_ref.size() - subpos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				size_type subpos,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_type;

		src_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				subpos,
				src_ref.size() - subpos,
				src_charset_name);
	}

	inline this_type& assign(const base_type& src, size_type subpos)
	{
		return this_type::assign(src, subpos, src.size() - subpos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& assign(const base_type& src, size_type subpos, const string& src_charset_name)
	{
		return this_type::assign(src, subpos, src.size() - subpos, src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos)
	{
		return
			this_type::assign(
				reinterpret_cast<const base_type&>(src),
				subpos,
				src.size() - subpos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos,
				const string& src_charset_name)
	{
		return
			this_type::assign(
				reinterpret_cast<const base_type&>(src),
				subpos,
				src.size() - subpos,
				src_charset_name);
	}

	// assign base, pos, len
	inline this_type& assign(BOOST_RV_REF(base_type) src,
								size_type subpos, size_type sublen)
	{
		return this_type::assign(boost::move(src), subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& assign(BOOST_RV_REF(base_type) src,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		if((subpos == src.size()) || (!sublen))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			//assert(subpos < src.size());
			if(!(subpos < src.size()))
			{
				throw std::out_of_range("basic_utf8_string_view::assign invalid subscript");
			}
			assert(sublen);

			sublen = (std::min)(src.size() - subpos, sublen);

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()
				&& sublen == src.size())
			{
				base_type& src_ref = src;
				if(!(&_base == &src_ref))
				{
					assert(utf8_foo::is_utf8_string(src));
					copy_or_move_or_swap(_base, boost::move(src));
					_utf8_size = utf8_foo::strlen(_base);
				}
			}
			else
			{
				base_type right(_base.get_allocator());
				charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				_base.swap(right);
				_utf8_size = utf8_foo::strlen(_base);
			}
		}

		return *this;
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				size_type subpos,
				size_type sublen)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_type;

		src_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				subpos,
				sublen,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				size_type subpos,
				size_type sublen,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_type;

		src_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				subpos,
				sublen,
				src_charset_name);
	}

	// the pos and len is base_type's pos and len, not utf8's pos and len
	inline this_type& assign(const base_type& src,
								size_type subpos, size_type sublen)
	{
		return this_type::assign(src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& assign(const base_type& src,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		if((subpos == src.size()) || (!sublen))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			if(!(subpos < src.size()))
			{
				throw std::out_of_range("basic_utf8_string_view::assign invalid subscript");
			}
			assert(sublen);

			sublen = (std::min)(src.size() - subpos, sublen);

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()
				&& sublen == src.size())
			{
				if(!(&_base == &src))
				{
					assert(utf8_foo::is_utf8_string(src));
					basic_string_assign(_base, src);
					_utf8_size = utf8_foo::strlen(_base);
				}
			}
			else
			{
				base_type right(_base.get_allocator());
				charset_foo::s_xchg(right, src.substr(subpos, sublen),
										src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				_base.swap(right);
				_utf8_size = utf8_foo::strlen(_base);
			}
		}

		return *this;
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen)
	{
		return
			this_type::assign(
				reinterpret_cast<const base_type&>(src),
				subpos,
				sublen,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		return
			this_type::assign(
				reinterpret_cast<const base_type&>(src),
				subpos,
				sublen,
				src_charset_name);
	}

	// assign this_type
	inline this_type& assign(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right._base));
		_utf8_size = boost::move(right._utf8_size);
		return *this;
	}

	inline this_type& assign(BOOST_RV_REF(this_type) right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(boost::move(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		return this_type::assign(boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		return this_type::assign(boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	inline this_type& assign(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		basic_string_assign(_base, right._base);
		_utf8_size = right._utf8_size;
		return *this;
	}

	inline this_type& assign(const this_type& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::assign(reinterpret_cast<const this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(reinterpret_cast<const this_type&>(right));
	}

	// assign this_type, pos

	inline this_type& assign(BOOST_RV_REF(this_type) right, size_type subpos)
	{
		return
			this_type::assign_of_utf8_base(
				boost::move(right._base), right._utf8_size,
				subpos, right._utf8_size - subpos);
	}

	inline this_type& assign(BOOST_RV_REF(this_type) right,
								size_type subpos,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::assign_of_utf8_base(
				boost::move(right._base), right._utf8_size,
				subpos, right._utf8_size - subpos);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				size_type subpos)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		return
			this_type::assign_of_utf8_base(
				boost::move(reinterpret_cast<base_type&>(right_ref.org_str())),
				right_ref.length(),
				subpos,
				right_ref.length() - subpos);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				size_type subpos,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		return
			this_type::assign_of_utf8_base(
				boost::move(reinterpret_cast<base_type&>(right_ref.org_str())),
				right_ref.length(),
				subpos,
				right_ref.length() - subpos);
	}

	inline this_type& assign(const this_type& right, size_type subpos)
	{
		return this_type::assign_of_utf8_base(
					right._base, right._utf8_size,
					subpos, right._utf8_size - subpos);
	}

	inline this_type& assign(const this_type& right,
								size_type subpos,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_utf8_base(
					right._base, right._utf8_size,
					subpos, right._utf8_size - subpos);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos)
	{
		return
			this_type::assign_of_utf8_base(
				reinterpret_cast<const base_type&>(right.org_str()),
				right.length(),
				subpos,
				right.length() - subpos);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::assign_of_utf8_base(
				reinterpret_cast<const base_type&>(right.org_str()),
				right.length(),
				subpos,
				right.length() - subpos);
	}

	// assign this_type, subpos, sublen

	inline this_type& assign(BOOST_RV_REF(this_type) right,
								size_type subpos, size_type sublen)
	{
		return this_type::assign_of_utf8_base(
					boost::move(right._base), right._utf8_size,
					subpos, sublen);
	}

	inline this_type& assign(BOOST_RV_REF(this_type) right,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_utf8_base(
					boost::move(right._base), right._utf8_size,
					subpos, sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				size_type subpos, size_type sublen)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		return
			this_type::assign_of_utf8_base(
				boost::move(reinterpret_cast<base_type&>(right_ref.org_str())),
				right_ref.length(),
				subpos, sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				size_type subpos,
				size_type sublen,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		return
			this_type::assign_of_utf8_base(
				boost::move(reinterpret_cast<base_type&>(right_ref.org_str())),
				right_ref.length(),
				subpos, sublen);
	}

	inline this_type& assign(const this_type& right,
								size_type subpos, size_type sublen)
	{
		return this_type::assign_of_utf8_base(right._base, right._utf8_size, subpos, sublen);
	}

	inline this_type& assign(const this_type& right,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign_of_utf8_base(right._base, right._utf8_size, subpos, sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen)
	{
		return
			this_type::assign_of_utf8_base(
				reinterpret_cast<const base_type&>(right.org_str()),
				right.length(),
				subpos,
				sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::assign_of_utf8_base(
				reinterpret_cast<const base_type&>(right.org_str()),
				right.length(),
				subpos, sublen);
	}

#endif // _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::assign(
				str_view.data(), str_view.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, const string& src_charset_name)
	{
		return this_type::assign(str_view.data(), str_view.size(), src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view)
	{
		return
			this_type::assign(
				str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return
			this_type::assign(
				str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	// string_view, pos
	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;


		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));

	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());

	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	// pos, len
	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos, size_type sublen)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos, size_type sublen, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos, size_type sublen)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		assign(const StringView& str_view, size_type subpos, size_type sublen, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::assign of range position");
		}

		return
			this_type::assign(
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				src_charset_name);
	}

	// assign string_charset_helper
	template<typename T1, typename T2> inline
	this_type& assign(size_type n, const string_charset_helper<T1, T2>& val)
	{
		return this_type::assign(n, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& assign(const string_charset_helper<T1, T2>& val)
	{
		return this_type::assign(val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& assign(const string_charset_helper<T1, T2>& val, size_type n_or_subpos)
	{
		return this_type::assign(val.first, n_or_subpos, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& assign(const string_charset_helper<T1, T2>& val, size_type subpos, size_type sublen)
	{
		return this_type::assign(val.first, subpos, sublen, val.second);
	}

	// insert

	// insert(pos, n, char)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		insert(size_type pos, size_type n, Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::insert(pos, n, static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		insert(size_type pos, size_type n, Char c, const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(n)
			{
				param_base_iterator_type iter = param_base_iterator_type((this_type::begin() + pos).org());
				basic_string_insert(_base, iter, n, static_cast<value_type>(c));
				_utf8_size += n;
			}
			return *this;
		}
		else
		{
			return
				this_type::insert(
					pos, n, char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, size_type n, const Char* c)
	{
		return this_type::insert(pos, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, size_type n, const Char* c, const string& src_charset_name)
	{
		assert(c);

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(!(c && c[0]))
		{
			if(n)
			{
				param_base_iterator_type iter((this_type::begin() + pos).org());
				basic_string_insert(_base, iter, n, value_type(0));
				_utf8_size += n;
			}

			return *this;
		}
		else
		{
			return this_type::insert(pos, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos, size_type n, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::insert(pos, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos, size_type n, const Basic_String<Char, Traits, Alloc>& c,
				const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(c.empty() || !c[0])
		{
			if(n)
			{
				param_base_iterator_type iter((this_type::begin() + pos).org());
				basic_string_insert(_base, iter, n, value_type(0));
				_utf8_size += n;
			}

			return *this;
		}
		else
		{
			return this_type::insert(pos, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos, size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::insert(pos, n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos, size_type n, const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, n, c.view());
	}

	template<typename Char, typename Traits>
	this_type& insert(size_type pos, size_type n, const basic_utf8_char_view<Char, Traits>& c)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(n)
		{
			const value_type* ptr = c.data();
			size_type step = c.size();
			assert(step);

			param_base_iterator_type iter = param_base_iterator_type((this_type::begin() + pos).org());

			if(step == 1)
			{
				basic_string_insert(_base, iter, n, *ptr);
			}
			else
			{
				base_type tmp(n * step, value_type(0), _base.get_allocator());
				for(size_type i = 0, spos = 0; i != n; ++i, spos += step)
				{
					memcpy(&tmp[spos], ptr, step);
				}

				if(_utf8_size)
				{
					basic_string_insert(_base, iter, tmp.begin(), tmp.end());
				}
				else
				{
					_base.swap(tmp);
				}
			}

			_utf8_size += n;
		}

		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& insert(size_type pos, size_type n, const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, n, c);
	}

	template<typename Utf8String> inline
	this_type& insert(size_type pos, size_type n, const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::insert(pos, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& insert(size_type pos, size_type n, const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& insert(size_type pos, size_type n, const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::insert(pos, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& insert(size_type pos, size_type n, const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, n, c.view());
	}

	// insert(iter, n, char)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, size_type n, Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::insert(p, n, static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, size_type n, Char c,
				const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		assert(( static_cast<size_type>(std::distance(param_iterator_type(this_type::begin()), p)) <= _utf8_size));

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			// can only use the "begin () + old_pos" way to get the return of the iterator,
			// because the CPP98 insert return value is void
			size_type old_pos = std::distance(param_iterator_type(this_type::begin()).org(), p.org());

			if(n)
			{
				param_base_iterator_type iter = param_base_iterator_type(p.org());
				basic_string_insert(_base, iter, n, static_cast<value_type>(c));
				_utf8_size += n;
			}

			return iterator(*this, _base.begin() + old_pos);
		}
		else
		{
			return
				this_type::insert(
					p, n, char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, size_type n, const Char* c)
	{
		return this_type::insert(p, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, size_type n, const Char* c, const string& src_charset_name)
	{
		assert(( static_cast<size_type>(std::distance(param_iterator_type(this_type::begin()), p)) <= _utf8_size));

		assert(c);
		if(!(c && c[0]))
		{
			size_type old_pos = std::distance(param_iterator_type(this_type::begin()).org(), p.org());
			if(n)
			{
				param_base_iterator_type iter = param_base_iterator_type(p.org());
				basic_string_insert(_base, iter, n, value_type(0));
				_utf8_size += n;
			}

			return iterator(*this, _base.begin() + old_pos);
		}
		else
		{
			return this_type::insert(p, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			iterator
		>::type
		insert(param_iterator_type p, size_type n, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::insert(p, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			iterator
		>::type
		insert(param_iterator_type p, size_type n, const Basic_String<Char, Traits, Alloc>& c,
				const string& src_charset_name)
	{
		assert(( static_cast<size_type>(std::distance(param_iterator_type(this_type::begin()), p)) <= _utf8_size));

		if(c.empty() || !c[0])
		{
			size_type old_pos = std::distance(param_iterator_type(this_type::begin()).org(), p.org());
			if(n)
			{
				param_base_iterator_type iter = param_base_iterator_type(p.org());
				basic_string_insert(_base, iter, n, value_type(0));
				_utf8_size += n;
			}

			return iterator(*this, _base.begin() + old_pos);
		}
		else
		{
			return this_type::insert(p, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	iterator insert(param_iterator_type p, size_type n,
						const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::insert(p, n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	iterator insert(param_iterator_type p, size_type n,
						const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, n, c.view());
	}

	template<typename Char, typename Traits>
	iterator insert(param_iterator_type p, size_type n,
						const basic_utf8_char_view<Char, Traits>& c)
	{
		assert(( static_cast<size_type>(std::distance(param_iterator_type(this_type::begin()), p)) <= _utf8_size));

		param_base_iterator_type iter = p.org();
		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), iter);

		if(n)
		{
			const value_type* ptr = c.data();
			size_type step = c.size();
			assert(step);

			if(step == 1)
			{
				basic_string_insert(_base, iter, n, *ptr);
			}
			else
			{
				base_type tmp(n * step, value_type(0), _base.get_allocator());

				for(size_type i = 0, spos = 0; i != n; ++i, spos += step)
				{
					memcpy(&tmp[spos], ptr, step);
				}

				if(_utf8_size)
				{
					basic_string_insert(_base, iter, tmp.begin(), tmp.end());
				}
				else
				{
					_base.swap(tmp);
				}
			}

			_utf8_size += n;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Char, typename Traits> inline
	iterator insert(param_iterator_type p, size_type n,
						const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, n, c);
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p, size_type n,
						const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::insert(p, n, c.view());
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p, size_type n,
						const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, n, c.view());
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p, size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::insert(p, n, c.view());
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p, size_type n,
						const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, n, c.view());
	}

	// insert(iterp, char)

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	don't support this function,															!!!
	!!!	because																					!!!
	!!!	insert(Param_Iterator p, size_type n, const Char*) and									!!!
	!!!	insert(Param_Iterator p, const Char c, const string& src_charset_name)					!!!
	!!!	conflict point (size_type Char ambiguous), there is no way to deal with this conflict,	!!!
	!!!	so remove the insert (iterp, char) style of the function								!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, Char* c)
	{
		return this_type::insert(p, size_type(1), c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, Char* c, const string& src_charset_name)
	{
		return this_type::insert(p, size_type(1), c, src_charset_name);
	}


	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			iterator
		>::type
		insert(param_iterator_type p, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::insert(p, size_type(1), c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			iterator
		>::type
		insert(param_iterator_type p, const Basic_String<Char, Traits, Alloc>& c,
				const string& src_charset_name)
	{
		return this_type::insert(p, size_type(1), c, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	iterator insert(param_iterator_type p, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::insert(p, size_type(1), c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	iterator insert(param_iterator_type p,
						const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, size_type(1), c.view());
	}

	template<typename Char, typename Traits> inline
	iterator insert(param_iterator_type p,
						const basic_utf8_char_view<Char, Traits>& c)
	{
		return this_type::insert(p, size_type(1), c);
	}

	template<typename Char, typename Traits> inline
	iterator insert(param_iterator_type p,
						const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, size_type(1), c);
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p,
						const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::insert(p, size_type(1), c.view());
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p,
						const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, size_type(1), c.view());
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p,
						const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::insert(p, size_type(1), c.view());
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p,
						const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, size_type(1), c.view());
	}

	// insert( pos, const char*)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, const Char* src)
	{
		return this_type::insert(pos, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, const Char* src, const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		assert(src);

		if(!(src && src[0]))
		{
			return *this;
		}

		size_type add_size = 0;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			const char* utf8_src = reinterpret_cast<const char*>(src);

			assert(utf8_foo::is_utf8_string(utf8_src));

			size_type base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
			add_size = utf8_foo::strlen(utf8_src);

			if(add_size)
			{
				basic_string_insert(_base, base_pos, utf8_src);
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_insert(_base, (this_type::begin() + pos).org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size += add_size;
		return *this;
	}

	// insert(pos, const char* n)

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, const Char* src, size_type n)
	{
		return this_type::insert(pos, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, const Char* src, size_type n, const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(!n)
		{
			return *this;
		}

		assert(src);
		if(!(src))
		{
			return *this;
		}

		base_type tmp(_base.get_allocator());
		charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(tmp);

		if(_utf8_size)
		{
			basic_string_insert(_base, (this_type::begin() + pos).org(), tmp.begin(), tmp.end());
		}
		else
		{
			_base.swap(tmp);
		}

		_utf8_size += add_size;
		return *this;
	}

	// insert(pos, const string&)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos, const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::insert(pos, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos, const Basic_String<value_type, Traits, Alloc>& src,
						const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(src.empty())
		{
			return *this;
		}

		if(src.data() == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			basic_string_insert(_base, (this_type::begin() + pos).org(), src.begin(), src.end());
			_utf8_size = _utf8_size << 1;
			return *this;
		}

		size_type add_size = 0;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(utf8_foo::is_utf8_string(src));

			basic_string_insert(_base, (this_type::begin() + pos).org(), src.begin(), src.end());
			add_size = utf8_foo::strlen(src);
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_insert(_base, (this_type::begin() + pos).org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos, const Basic_String<Char, Traits, Alloc>& src,
				const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(src.empty())
		{
			return *this;
		}

		base_type tmp(_base.get_allocator());
		charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(tmp);

		if(_utf8_size)
		{
			basic_string_insert(_base, (this_type::begin() + pos).org(), tmp.begin(), tmp.end());
		}
		else
		{
			_base.swap(tmp);
		}

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos,
				const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos)
	{
		return this_type::insert(pos, src, subpos, src.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos,
				const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos, const string& src_charset_name)
	{
		return this_type::insert(pos, src, subpos, src.size(), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, const Char* src, size_type subpos, size_type sublen)
	{
		return this_type::insert(pos, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		insert(size_type pos, const Char* src, size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(!sublen)
		{
			return *this;
		}

		assert(src);
		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src + subpos, sublen, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(right);

		if(_utf8_size)
		{
			basic_string_insert(_base, (this_type::begin() + pos).org(), right.begin(), right.end());
		}
		else
		{
			_base.swap(right);
		}

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos,
				const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos, size_type sublen)
	{
		return this_type::insert(pos, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		insert(size_type pos,
				const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if(!(pos <= _utf8_size && subpos < src.size()))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if((subpos == src.size()) || (!sublen))
		{
			return *this;
		}

		assert(sublen);
		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(right);

		if(_utf8_size)
		{
			basic_string_insert(_base, (this_type::begin() + pos).org(), right.begin(), right.end());
		}
		else
		{
			_base.swap(right);
		}

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos, const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		basic_string_insert(_base, (this_type::begin() + pos).org(),
								other.org_str().begin(), other.org_str().end());
		_utf8_size += other.length();
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos, const utf8_string_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, other);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos)
	{
		return this_type::insert(pos, other, subpos, other.length());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, other, subpos, other.length());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
	{
		typedef utf8_string_impl<Char, Traits, Alloc> other_type;
		typedef typename other_type::const_iterator other_citer_type;

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(!(subpos == other.length() || !sublen))
		{
			if(!(subpos < other.length()))
			{
				throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
			}
			assert(sublen);

			sublen = (std::min)(other.length() - subpos, sublen);

			other_citer_type start = other.begin() + subpos;
			other_citer_type last = start + sublen;

			basic_string_insert(_base, (this_type::begin() + pos).org(), start.org(), last.org());
			_utf8_size += sublen;
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& insert(size_type pos,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, other, subpos, sublen);
	}

public:
	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			iterator
		>::type
		insert(param_iterator_type p, InputIter start, InputIter last)
	{
		typedef typename boost::iterator_value<InputIter>::type cv_now_char_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_now_char_type) now_char_type;

		return this_type::insert(p, start, last, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			iterator
		>::type
		insert(param_iterator_type p, InputIter start, InputIter last,
				const string& src_charset_name)
	{
		typedef
			detail::iterator_insert_helper
			<
				iterator,
				InputIter,
				param_iterator_type,
				param_base_iterator_type
			> h_type;

		h_type h;
		return h(*this, p, start, last, src_charset_name);
	}

	template<typename Char, typename Basic_Iter> inline
	iterator insert(param_iterator_type p,
						utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	iterator insert(param_iterator_type p,
						utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	iterator insert(param_iterator_type p,
						utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	iterator insert(param_iterator_type p,
						utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	iterator insert(param_iterator_type p,
						utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	iterator insert(param_iterator_type p,
						utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			iterator
		>::type
		insert(param_iterator_type p,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			iterator
		>::type
		insert(param_iterator_type p,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			iterator
		>::type
		insert(param_iterator_type p,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			iterator
		>::type
		insert(param_iterator_type p,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			iterator
		>::type
		insert(param_iterator_type p,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			iterator
		>::type
		insert(param_iterator_type p,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert_of_this_riterator(p, start, last);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, std::initializer_list<Char> il)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(
			p, boost::begin(il), boost::end(il),
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p,
				std::initializer_list<Char> il,
				const string& src_charset_name)
	{
		this_type::assign(p, boost::begin(il), boost::end(il), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, std::initializer_list<Char*> il)
	{
		return this_type::insert(p, il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			iterator
		>::type
		insert(param_iterator_type p, std::initializer_list<Char*> il, const string& src_charset_name)
	{
		typedef std::initializer_list<Char*> il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(this_type::get_allocator());
			tmp.reserve(add_size << 2); // add_size * 4

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					assert(*i);
					assert(1 == utf8_foo::strlen(*i));
					tmp.append(reinterpret_cast<const value_type*>(*i),
								utf8_foo::charlen(reinterpret_cast<const value_type*>(*i)));
				}
			}
			else
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					assert(*i);
					tmp.append(char_type(*i, this_type::get_allocator(), src_charset_name).org_str());
				}
			}

			if(_utf8_size)
			{
				basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			iterator
		>::type
		insert(param_iterator_type p, std::initializer_list< Basic_String<Char, Traits, Alloc> > il)
	{
		return this_type::insert(p, il, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			iterator
		>::type
		insert(param_iterator_type p,
                std::initializer_list< Basic_String<Char, Traits, Alloc> > il,
				const string& src_charset_name)
	{
		typedef std::initializer_list< Basic_String<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(this_type::get_allocator());
			tmp.reserve(il.size() << 2); // add_size * 4

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					if((*i).empty())
					{
						tmp.append(1, value_type(0));
					}
					else
					{
						assert(1 == utf8_foo::strlen(*i));
						tmp.append(reinterpret_cast<const value_type*>((*i).data()),
									utf8_foo::charlen(reinterpret_cast<const value_type*>((*i).data())));
					}
				}
			}
			else
			{
				for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
				{
					tmp.append(char_type(*i, this_type::get_allocator(), src_charset_name).org_str());
				}
			}

			if(_utf8_size)
			{
				basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	iterator insert(param_iterator_type p,
						std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il)
	{
		typedef std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			if(_utf8_size)
			{
				basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	iterator insert(param_iterator_type p,
						std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, il);
	}

	template<typename Char, typename Traits>
	iterator insert(param_iterator_type p,
						std::initializer_list< basic_utf8_char_view<Char, Traits> > il)
	{
		typedef std::initializer_list< basic_utf8_char_view<Char, Traits> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			if(_utf8_size)
			{
				basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Char, typename Traits> inline
	iterator insert(param_iterator_type p,
						std::initializer_list< basic_utf8_char_view<Char, Traits> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, il);
	}

	template<typename Utf8String>
	iterator insert(param_iterator_type p,
						std::initializer_list< basic_utf8_char_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			if(_utf8_size)
			{
				basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p,
						std::initializer_list< basic_utf8_char_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, il);
	}

	template<typename Utf8String>
	iterator insert(param_iterator_type p,
						std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			if(_utf8_size)
			{
				basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}

		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename Utf8String> inline
	iterator insert(param_iterator_type p,
						std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(p, il);
	}

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

private:

// disable this utf8_char_impl auto conv to base_type
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Char, typename Traits, typename Alloc \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void insert(size_type, const utf8_char_impl<Char, Traits, Alloc>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Char, typename Traits \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void insert(size_type, const basic_utf8_char_view<Char, Traits>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void insert(size_type, const basic_utf8_char_reference<Utf8String>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void insert(size_type, const basic_utf8_char_const_reference<Utf8String>& \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) );

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

public:

#if !(defined(_DEBUG) && defined(_YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY))
	// insert(pos, base_type)
	inline this_type& insert(size_type pos, BOOST_RV_REF(base_type) src)
	{
		return this_type::insert(pos, boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& insert(size_type pos, BOOST_RV_REF(base_type) src, const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(src.empty())
		{
			return *this;
		}

		param_base_iterator_type iter = (this_type::begin() + pos).org();

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			base_type& src_ref = src;
			if(&_base == &src_ref)
			{
				basic_string_insert(_base, iter, src.begin(), src.end());
				_utf8_size <<= 1; // _utf8_size * 2
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				size_type add_size = utf8_foo::strlen(src);
				if(_utf8_size)
				{
					basic_string_insert(_base, iter, src.begin(), src.end());
				}
				else
				{
					copy_or_move_or_swap(_base, boost::move(src));
				}

				_utf8_size += add_size;
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, boost::move(src), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_insert(_base, iter, tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}
		return *this;
	}

	inline this_type& insert(size_type pos, const base_type& src)
	{
		return this_type::insert(pos, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& insert(size_type pos, const base_type& src, const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(src.empty())
		{
			return *this;
		}

		param_base_iterator_type iter = (this_type::begin() + pos).org();

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			basic_string_insert(_base, iter, src.begin(), src.end());

			if(&_base == &src)
			{
				_utf8_size <<= 1; // _utf8_size * 2
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				_utf8_size += utf8_foo::strlen(src);
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_insert(_base, iter, tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size += add_size;
		}
		return *this;
	}

	// insert (pos, string, subpos)
	inline this_type& insert(size_type pos, const base_type& src, size_type subpos)
	{
		return this_type::insert(pos, src, subpos, src.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& insert(size_type pos, const base_type& src,
								size_type subpos,
								const string& src_charset_name)
	{
		return this_type::insert(pos, src, subpos, src.size(), src_charset_name);
	}

	// insert (pos, string, subpos, sublen)
	inline this_type& insert(size_type pos, const base_type& src,
								size_type subpos, size_type sublen)
	{
		return this_type::insert(pos, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& insert(size_type pos, const base_type& src,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		if(!(pos <= _utf8_size && subpos < src.size()))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if((subpos == src.size()) || (!sublen))
		{
			return *this;
		}

		assert(sublen);
		base_type right(_base.get_allocator());
		charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type add_size = utf8_foo::strlen(right);

		if(_utf8_size)
		{
			basic_string_insert(_base, (this_type::begin() + pos).org(), right.begin(), right.end());
		}
		else
		{
			_base.swap(right);
		}

		_utf8_size += add_size;
		return *this;
	}

	//isomeric_same_base_type

	// insert(pos, base_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::insert(
				pos, boost::move(reinterpret_cast<base_type&>(src_ref)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::insert(
				pos,
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const basic_string<value_type, traits_type, Alloc2>& src)
	{
		return
			this_type::insert(
				pos,
				reinterpret_cast<const base_type&>(src),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				const basic_string<value_type, traits_type, Alloc2>& src,
				const string& src_charset_name)
	{
		return
			this_type::insert(
				pos,
				reinterpret_cast<const base_type&>(src),
				src_charset_name);
	}

	// insert (pos, string, subpos)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const basic_string<value_type, traits_type, Alloc2>& src, size_type subpos)
	{
		return
			this_type::insert(
				pos,
				reinterpret_cast<const base_type&>(src),
				subpos, src.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos,
				const string& src_charset_name)
	{
		return
			this_type::insert(
				pos,
				reinterpret_cast<const base_type&>(src),
				subpos, src.size(),
				src_charset_name);
	}

	// insert (pos, string, subpos, sublen)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen)
	{
		return
			this_type::insert(
				pos,
				reinterpret_cast<const base_type&>(src),
				subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		return
			this_type::insert(
				pos,
				reinterpret_cast<const base_type&>(src),
				subpos, sublen, src_charset_name);
	}

	// insert(pos, this_type)
	inline this_type& insert(size_type pos, BOOST_RV_REF(this_type) right)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		param_base_iterator_type iter = (this_type::begin() + pos).org();

		this_type& right_ref = right;
		if(this == &right_ref)
		{
			basic_string_insert(_base, iter, right_ref.org_str().begin(), right_ref.org_str().end());
			_utf8_size <<= 1; // _utf8_size *= 2;
		}
		else
		{
			if(_utf8_size)
			{
				basic_string_insert(_base, iter, right_ref.org_str().begin(), right_ref.org_str().end());
			}
			else
			{
				copy_or_move_or_swap(_base, boost::move(right._base));
			}

			_utf8_size += boost::move(right._utf8_size);
		}

		return *this;
	}

	inline this_type& insert(size_type pos, BOOST_RV_REF(this_type) right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, boost::move(right));
	}

	inline this_type& insert(size_type pos, const this_type& right)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		basic_string_insert(_base, (this_type::begin() + pos).org(), right.org_str().begin(), right.org_str().end());
		_utf8_size += right._utf8_size;
		return *this;
	}

	inline this_type& insert(size_type pos, const this_type& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, right);
	}

	// insert(pos, this_type, pos)
	inline this_type& insert(size_type pos, const this_type& right, size_type subpos)
	{
		return this_type::insert(pos, right, subpos, right._utf8_size);
	}

	inline this_type& insert(size_type pos, const this_type& right, size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, right, subpos, right._utf8_size);
	}

	// insert(pos, this_type, pos, len)
	inline this_type& insert(size_type pos, const this_type& right, size_type subpos, size_type sublen)
	{
		if(!(pos <= _utf8_size && subpos < right._utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::insert invalid subscript");
		}

		if(subpos == right._utf8_size || !sublen)
		{
			return *this;
		}

		assert(sublen);

		sublen = (std::min)(right._utf8_size - subpos, sublen);

		const_iterator start = right.begin() + subpos;
		const_iterator last = start + sublen;

		basic_string_insert(_base, (this_type::begin() + pos).org(), start.org(), last.org());
		_utf8_size += sublen;
		return *this;
	}

	inline this_type& insert(size_type pos, const this_type& right, size_type subpos, size_type sublen,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, right, subpos, sublen);
	}

	// isomeric_same_this_type
	// insert(pos, this_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;
		right_type& right_ref = right;
		return this_type::insert(pos, boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		right_type& right_ref = right;
		return this_type::insert(pos, boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		this_type::insert(pos, reinterpret_cast<const this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, reinterpret_cast<const this_type&>(right));
	}

	// insert(pos, this_type, pos)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const utf8_string_impl<value_type, traits_type, Alloc2>& right, size_type subpos)
	{
		return this_type::insert(pos, reinterpret_cast<const this_type&>(right), subpos, right.length());
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, reinterpret_cast<const this_type&>(right), subpos, right.length());
	}

	// insert(pos, this_type, pos, len)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen)
	{
		return
			this_type::insert(
				pos, reinterpret_cast<const this_type&>(right),
				subpos, sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		insert(size_type pos, const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::insert(pos, reinterpret_cast<const this_type&>(right), subpos, sublen);
	}

#endif // _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

	// string_view
	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::insert(
				pos, str_view.data(), str_view.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, const string& src_charset_name)
	{
		return
			this_type::insert(
				pos, str_view.data(), str_view.size(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, size_type subpos)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, size_type subpos, const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				src_charset_name), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, size_type subpos, size_type sublen)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				src_charset_name), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view)
	{
		return
			this_type::insert(
				pos, str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, const string& src_charset_name)
	{
		return
			this_type::insert(
				pos, str_view.data(), str_view.size(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, size_type subpos)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_INNER_STRING_CHARSET_NAME()), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, size_type subpos, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				str_view.end(),
				YGGR_STR_INNER_STRING_CHARSET_NAME()), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view, size_type subpos, size_type sublen)
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				YGGR_STR_INNER_STRING_CHARSET_NAME()), *this);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< StringView >,
			this_type&
		>::type
		insert(size_type pos, const StringView& str_view,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::insert of range position");
		}

		return
			(this_type::insert(
				advance_copy(this_type::begin(), pos),
				advance_copy(str_view.begin(), subpos),
				advance_copy(str_view.begin(), subpos + (std::min<size_type>)(sublen, str_view.length() - subpos)),
				src_charset_name), *this);
	}

	// string_charset_helper
	template<typename T1, typename T2> inline
	this_type& insert(size_type pos, size_type n, const string_charset_helper<T1, T2>& val)
	{
		return this_type::insert(pos, n, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& insert(size_type pos, const string_charset_helper<T1, T2>& val)
	{
		return this_type::insert(pos, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& insert(size_type pos, const string_charset_helper<T1, T2>& val,
						size_type n_or_subpos)
	{
		return this_type::insert(pos, val.first, n_or_subpos, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& insert(size_type pos, const string_charset_helper<T1, T2>& val,
						size_type subpos, size_type sublen)
	{
		return this_type::insert(pos, val.first, subpos, sublen, val.second);
	}

	template<typename T1, typename T2> inline
	iterator insert(param_iterator_type p, size_type n, const string_charset_helper<T1, T2>& val)
	{
		return this_type::insert(p, n, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	iterator insert(param_iterator_type p, const string_charset_helper<T1, T2>& val)
	{
		return this_type::insert(p, val.first, val.second);
	}

	// erase
	inline this_type& erase(size_type pos = 0, size_type len = base_type::npos)
	{
		if(!(pos < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::erase invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);

		param_iterator_type first = this_type::begin() + pos;
		param_iterator_type last = first + len;

		_base.erase(first.org(), last.org());
		_utf8_size -= len;
		return *this;
	}

	inline iterator erase(param_iterator_type p)
	{
		size_type rm_pos = std::distance<param_iterator_type>(this_type::begin(), p);
		if(!(rm_pos < _utf8_size))
		{
			return this_type::end();
		}

		size_type old_size = _base.size();
		base_iterator iter = _base.erase(p.org(), (p + 1).org());

		if(old_size != _base.size())
		{
			--_utf8_size;
		}
		return this_type::iterator(*this, iter);
	}

	inline iterator erase(param_iterator_type start, param_iterator_type last)
	{
		size_type rm_pos = std::distance<param_iterator_type>(this_type::begin(), start);
		if(!(rm_pos < _utf8_size))
		{
			return this_type::end();
		}

		size_type rm_size = std::distance(start, last);
		assert(rm_size <= _utf8_size);

		size_type old_size = _base.size();
		base_iterator iter = _base.erase(start.org(), last.org());

		if(old_size != _base.size())
		{
			_utf8_size -= rm_size;
		}

		return this_type::iterator(*this, iter);
	}

	// replace

	// replace(pos, len, n, char)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, size_type n, Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::replace(
				pos, len, n, static_cast<now_char_type>(c),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, size_type n, Char c,
					const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			len = (std::min)(_utf8_size - pos, len);
			param_iterator_type i1 = this_type::begin() + pos;
			param_iterator_type i2 = i1 + len;

			basic_string_replace(_base, i1.org(), i2.org(), n, static_cast<value_type>(c));
			_utf8_size = _utf8_size - len + n;

			return *this;
		}
		else
		{
			return
				this_type::replace(
					pos, len, n,
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, size_type n, const Char* c)
	{
		return this_type::replace(pos, len, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_native_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, size_type n, const Char* c,
				const string& src_charset_name)
	{
		assert(c);

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		if(!(c && c[0]))
		{
			len = (std::min)(_utf8_size - pos, len);
			param_iterator_type i1 = this_type::begin() + pos;
			param_iterator_type i2 = i1 + len;

			basic_string_replace(_base, i1.org(), i2.org(), n, value_type(0));
			_utf8_size = _utf8_size - len + n;
			return *this;
		}
		else
		{
			return this_type::replace(pos, len, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len, size_type n, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::replace(pos, len, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len, size_type n, const Basic_String<Char, Traits, Alloc>& c,
					const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		if(c.empty() || 0 == c[0])
		{
			len = (std::min)(_utf8_size - pos, len);
			param_iterator_type i1 = this_type::begin() + pos;
			param_iterator_type i2 = i1 + len;

			basic_string_replace(_base, i1.org(), i2.org(), n, value_type(0));
			_utf8_size = _utf8_size - len + n;
			return *this;
		}
		else
		{
			return this_type::replace(pos, len, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::replace(pos, len, n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, n, c.view());
	}

	template<typename Char, typename Traits>
	this_type& replace(size_type pos, size_type len,
						size_type n, const basic_utf8_char_view<Char, Traits>& c)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;

		size_type step = c.size();
		const value_type* ptr = c.data();

		if(step == 1)
		{
			basic_string_replace(_base, i1.org(), i2.org(), n, *ptr);
		}
		else
		{

			base_type tmp(n * step, value_type(0), _base.get_allocator());
			for(size_type i = 0, spos = 0; i != n; ++i, spos += step)
			{
				memcpy(&tmp[spos], ptr, step);
			}

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size = _utf8_size - len + n;
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, n, c);
	}

	template<typename Utf8String> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::replace(pos, len, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::replace(pos, len, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, n, c.view());
	}

	// replace(iters, itere, n, char)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2, size_type n, Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::replace(
				i1, i2, n, static_cast<now_char_type>(c),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2, size_type n, Char c,
					const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			size_type len = distance(i1, i2);
			basic_string_replace(_base, i1.org(), i2.org(), n, static_cast<value_type>(c));
			_utf8_size = _utf8_size - len + n;

			return *this;
		}
		else
		{
			return
				this_type::replace(
					i1, i2, n,
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					size_type n, const Char* c)
	{
		return this_type::replace(i1, i2, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					size_type n, const Char* c,
					const string& src_charset_name)
	{
		assert(c);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		if(!(c && c[0]))
		{
			size_type len = distance(i1, i2);

			basic_string_replace(_base, i1.org(), i2.org(), n, value_type(0));
			_utf8_size = _utf8_size - len + n;
			return *this;
		}
		else
		{
			return this_type::replace(i1, i2, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					size_type n, const Basic_String<Char, Traits, Alloc>& c)
	{
		return this_type::replace(i1, i2, n, c, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					size_type n, const Basic_String<Char, Traits, Alloc>& c,
					const string& src_charset_name)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		if(c.empty() || 0 == c[0])
		{
			size_type len = distance(i1, i2);

			basic_string_replace(_base, i1.org(), i2.org(), n, value_type(0));
			_utf8_size = _utf8_size - len + n;
			return *this;
		}
		else
		{
			return this_type::replace(i1, i2, n, char_type(c, this_type::get_allocator(), src_charset_name));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		return this_type::replace(i1, i2, n, c.view());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, n, c.view());
	}

	template<typename Char, typename Traits>
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const basic_utf8_char_view<Char, Traits>& c)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);

		size_type step = c.size();
		const value_type* ptr = c.data();

		if(step == 1)
		{
			basic_string_replace(_base, i1.org(), i2.org(), n, *ptr);
		}
		else
		{
			base_type tmp(n * step, value_type(0), _base.get_allocator());
			for(size_type i = 0, spos = 0; i != n; ++i, spos += step)
			{
				memcpy(&tmp[spos], ptr, step);
			}

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp.begin(), tmp.end());
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size = _utf8_size - len + n;
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const basic_utf8_char_view<Char, Traits>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, n, c);
	}

	template<typename Utf8String> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const basic_utf8_char_reference<Utf8String>& c)
	{
		return this_type::replace(i1, i2, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const basic_utf8_char_const_reference<Utf8String>& c)
	{
		return this_type::replace(i1, i2, n, c.view());
	}

	template<typename Utf8String> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, n, c.view());
	}

	// replace(pos, len, const char*)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, const Char* src)
	{
		return this_type::replace(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, const Char* src, const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		assert(src);

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if(!(src && src[0]))
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				const char* utf8_src = reinterpret_cast<const char*>(src);

				assert(utf8_foo::is_utf8_string(utf8_src));

				add_size = utf8_foo::strlen(utf8_src);
				basic_string_replace(_base, i1.org(), i2.org(), utf8_src);
			}
			else
			{
				base_type tmp(_base.get_allocator());
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				add_size = utf8_foo::strlen(tmp);

				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), tmp);
				}
				else
				{
					_base.swap(tmp);
				}
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace(pos, len, const char*, n)

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, const Char* src, size_type n)
	{
		return this_type::replace(pos, len, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len, const Char* src, size_type n,
					const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if(!n)
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			assert(src);
			if(!src)
			{
				basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
			}
			else
			{
				base_type tmp(_base.get_allocator());
				charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				add_size = utf8_foo::strlen(tmp);

				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), tmp);
				}
				else
				{
					_base.swap(tmp);
				}
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace(iters, itere, const char*)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2, const Char* src)
	{
		return this_type::replace(i1, i2, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					const Char* src, const string& src_charset_name)
	{
		assert(src);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = 0;

		if(!(src && src[0]))
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace(iters, itere, const char*, n)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2, const Char* src, size_type n)
	{
		return this_type::replace(i1, i2, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					const Char* src, size_type n,
					const string& src_charset_name)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = 0;

		if(!n)
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			assert(src);
			if(!src)
			{
				basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
			}
			else
			{
				base_type tmp(_base.get_allocator());
				charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				add_size = utf8_foo::strlen(tmp);

				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), tmp);
				}
				else
				{
					_base.swap(tmp);
				}
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace(pos, len, string)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len, const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::replace(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Basic_String<value_type, Traits, Alloc>& src,
					const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if(src.data() == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			basic_string_replace(_base, i1.org(), i2.org(), src.begin(), src.end());
			_utf8_size = (_utf8_size << 1) - len;
			return *this;
		}

		if(src.empty())
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));

				add_size = utf8_foo::strlen(src);
				basic_string_replace(_base, i1.org(), i2.org(), src.begin(), src.end());
			}
			else
			{
				base_type tmp(_base.get_allocator());
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				add_size = utf8_foo::strlen(tmp);

				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), tmp);
				}
				else
				{
					_base.swap(tmp);
				}
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if(src.empty())
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace (pos, len, string, subpos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const Basic_String<Char, Traits, Alloc>& src,
				size_type subpos)
	{
		return this_type::replace(pos, len, src, subpos, src.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos,
					const string& src_charset_name)
	{
		return this_type::replace(pos, len, src, subpos, src.size(), src_charset_name);
	}

	// replace (pos, len, string, subpos, sublen)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Char* src, size_type subpos, size_type sublen)
	{
		return this_type::replace(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Char* src, size_type subpos, size_type sublen,
					const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if(!sublen)
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			assert(sublen);
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src + subpos, sublen, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(right);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), right);
			}
			else
			{
				_base.swap(right);
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen)
	{
		return this_type::replace(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen,
					const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if((subpos == src.size()) || (!sublen))
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(!(subpos < src.size()))
			{
				throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
			}
			assert(sublen);
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(right);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), right);
			}
			else
			{
				_base.swap(right);
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace(pos, len, utf8_string)
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;

		basic_string_replace(_base, i1.org(), i2.org(), other.org_str().begin(), other.org_str().end());
		_utf8_size = _utf8_size - len + other.length();
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, other);
	}

	// replace (pos, len, utf8_string, pos)
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos)
	{
		return this_type::replace(pos, len, other, subpos, other.length());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, other, subpos, other.length());
	}

	// replace (pos, len, utf8_string, pos, len)
	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(size_type pos, size_type len,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
	{
		typedef utf8_string_impl<Char, Traits, Alloc> other_type;
		typedef typename other_type::const_iterator other_citer_type;

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if(subpos == other.length() || !sublen)
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(!(subpos < other.length()))
			{
				throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
			}

			assert(sublen);

			add_size = (std::min)(other.length() - subpos, sublen);

			other_citer_type start = other.begin() + subpos;
			other_citer_type last = start + add_size;

			basic_string_replace(_base, i1.org(), i2.org(), start.org(), last.org());
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(size_type pos, size_type len,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, other, subpos, sublen);
	}

	// replace(iters, itere, string)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::replace(i1, i2, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					const Basic_String<value_type, Traits, Alloc>& src,
					const string& src_charset_name)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = 0;

		if(src.data() == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			basic_string_replace(_base, i1.org(), i2.org(), src.begin(), src.end());
			_utf8_size = (_utf8_size << 1) - len;
			return *this;
		}

		if(src.empty())
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));

				add_size = utf8_foo::strlen(src);
				basic_string_replace(_base, i1.org(), i2.org(), src.begin(), src.end());
			}
			else
			{
				base_type tmp(_base.get_allocator());
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				add_size = utf8_foo::strlen(tmp);

				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), tmp);
				}
				else
				{
					_base.swap(tmp);
				}
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = 0;

		if(src.empty())
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// replace(iters, itere, utf8_string)
	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);

		basic_string_replace(_base, i1.org(), i2.org(), other.org_str().begin(), other.org_str().end());
		_utf8_size = _utf8_size - len + other.length();
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						const utf8_string_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, other);
	}

	// replace(iters, itere, oiters, oitere)

public:
	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					InputIter start, InputIter last)
	{
		typedef InputIter iter_type;
		typedef detail::iterator_replace_helper<iter_type, param_iterator_type> h_type;
		typedef typename boost::iterator_value<iter_type>::type cv_now_char_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_now_char_type) now_char_type;

		h_type h;
		return h(*this, i1, i2, start, last, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			is_iterator<InputIter>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				InputIter start, InputIter last,
				const string& src_charset_name)
	{
		typedef InputIter iter_type;
		typedef detail::iterator_replace_helper<iter_type, param_iterator_type> h_type;

		h_type h;
		return h(*this, i1, i2, start, last, src_charset_name);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_modify_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Basic_Iter,
				template<typename _T> class ReverseIter > inline
	typename
		boost::enable_if
		<
			is_reverse_iterator< ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > >,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > start,
				ReverseIter< utf8_string_const_iterator<Char, Basic_Iter> > last,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					std::initializer_list<Char> ilst)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::replace(
				i1, i2, boost::begin(ilst), boost::end(ilst),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
					std::initializer_list<Char> ilst,
					const string& src_charset_name)
	{
		return this_type::replace(i1, i2, boost::begin(ilst), boost::end(ilst), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il)
	{
		typedef std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}
		else
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< utf8_char_impl<Char, Traits, Alloc> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, il);
	}

	template<typename Char, typename Traits>
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< basic_utf8_char_view<Char, Traits> > il)
	{
		typedef std::initializer_list< basic_utf8_char_view<Char, Traits> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				tmp.append((*i).data(), (*i).size());
			}

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}
		else
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< basic_utf8_char_view<Char, Traits> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, il);
	}

	template<typename Utf8String>
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< basic_utf8_char_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}
		else
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Utf8String> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< basic_utf8_char_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, il);
	}

	template<typename Utf8String>
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il)
	{
		typedef std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il_type;
		typedef typename boost::range_iterator<il_type>::type il_iter_type;

		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = il.size();

		if(add_size)
		{
			base_type tmp(_base.get_allocator());
			tmp.reserve((add_size << 2)); // add_size * 4
			const value_type* ptr = 0;
			for(il_iter_type i = boost::begin(il), isize = boost::end(il); i != isize; ++i)
			{
				ptr = (*i).data();
				tmp.append(ptr, utf8_foo::charlen(ptr));
			}

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}
		}
		else
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename Utf8String> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						std::initializer_list< basic_utf8_char_const_reference<Utf8String> > il,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, il);
	}

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

private:
// disable this utf8_char_impl auto conv to base_type
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Char, typename Traits, typename Alloc \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void replace(size_type, size_type, \
					const utf8_char_impl<Char, Traits, Alloc>& \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Char, typename Traits \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void replace(size_type, size_type, \
					const basic_utf8_char_view<Char, Traits>& \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void replace(size_type, size_type, \
					const basic_utf8_char_reference<Utf8String>& \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ); \
	\
	template< typename Utf8String \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void replace(size_type, size_type, \
					const basic_utf8_char_const_reference<Utf8String>& \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) );


#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

public:

#if !(defined(_DEBUG) && defined(_YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY))

	// replace(pos, len, base_type)
	inline this_type& replace(size_type pos, size_type len, BOOST_RV_REF(base_type) src)
	{
		return this_type::replace(pos, len, boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& replace(size_type pos, size_type len,
						BOOST_RV_REF(base_type) src,
						const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			base_type& src_ref = src;
			if(&_base == &src_ref)
			{
				basic_string_replace(_base, i1.org(), i2.org(), src);
				_utf8_size = (_utf8_size  << 1) - len; // _utf8_size - len + _utf8_size
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				size_type add_size = utf8_foo::strlen(src);
				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), src);
				}
				else
				{
					copy_or_move_or_swap(_base, boost::move(src));
				}
				_utf8_size = _utf8_size - len + add_size;
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, boost::move(src), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size = _utf8_size - len + add_size;
		}

		return *this;
	}

	inline this_type& replace(size_type pos, size_type len, const base_type& src)
	{
		return this_type::replace(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& replace(size_type pos, size_type len,
						const base_type& src,
						const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			basic_string_replace(_base, i1.org(), i2.org(), src);

			if(&_base == &src)
			{
				_utf8_size = (_utf8_size  << 1) - len; // _utf8_size - len + _utf8_size
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				_utf8_size = _utf8_size - len + utf8_foo::strlen(src);
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size = _utf8_size - len + add_size;
		}

		return *this;
	}

	// replace (pos, len, string, subpos)
	inline this_type& replace(size_type pos, size_type len,
								const base_type& src,
								size_type subpos)
	{
		return this_type::replace(pos, len, src, subpos, src.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline this_type& replace(size_type pos, size_type len,
								const base_type& src,
								size_type subpos,
								const string& src_charset_name)
	{
		return this_type::replace(pos, len, src, subpos, src.size(), src_charset_name);
	}

	// replace(string, pos, len)
	inline this_type& replace(size_type pos, size_type len,
								const base_type& src,
								size_type subpos, size_type sublen)
	{
		return this_type::replace(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& replace(size_type pos, size_type len,
						const base_type& src,
						size_type subpos, size_type sublen,
						const string& src_charset_name)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		assert(subpos <= src.size());

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if((subpos == src.size()) || (!sublen))
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(!(subpos < src.size()))
			{
				throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
			}
			assert(sublen);

			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src.substr(subpos, sublen), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			add_size = utf8_foo::strlen(right);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), right);
			}
			else
			{
				_base.swap(right);
			}
		}
		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	// isomeric_same_base_type
	// replace(pos, len, base_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::replace(
				pos, len,
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::replace(
				pos, len,
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len, const basic_string<value_type, traits_type, Alloc2>& src)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const base_type&>(src),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const basic_string<value_type, traits_type, Alloc2>& src,
				const string& src_charset_name)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const base_type&>(src),
				src_charset_name);
	}

	// replace (pos, len, string, subpos)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const base_type&>(src),
				subpos, src.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos,
				const string& src_charset_name)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const base_type&>(src),
				subpos, src.size(),
				src_charset_name);
	}

	// replace(string, pos, len)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const base_type&>(src),
				subpos, sublen,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const basic_string<value_type, traits_type, Alloc2>& src,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const base_type&>(src),
				subpos, sublen,
				src_charset_name);
	}

	// replace(pos, len, this_type)
	this_type& replace(size_type pos, size_type len, BOOST_RV_REF(this_type) right)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;

		this_type& right_ref = right;
		if(this == &right_ref)
		{
			basic_string_replace(_base, i1.org(), i2.org(), right._base);
			_utf8_size = (_utf8_size << 1) - len; // _utf8_size = _utf8_size - len + _utf8_size;
		}
		else
		{
			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), right._base);
			}
			else
			{
				copy_or_move_or_swap(_base, boost::move(right._base));
			}

			_utf8_size = _utf8_size - len + boost::move(right._utf8_size);
		}

		return *this;
	}

	inline this_type& replace(size_type pos, size_type len, BOOST_RV_REF(this_type) right,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, boost::move(right));
	}

	inline this_type& replace(size_type pos, size_type len, const this_type& right)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;

		basic_string_replace(_base, i1.org(), i2.org(), right._base);
		_utf8_size = _utf8_size - len + right._utf8_size;
		return *this;
	}

	inline this_type& replace(size_type pos, size_type len,
								const this_type& right,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, right);
	}

	// replace(pos, len, this_type, subpos)
	inline this_type& replace(size_type pos, size_type len,
								const this_type& right,
								size_type subpos)
	{
		return this_type::replace(pos, len, right, subpos, right._utf8_size);
	}

	inline this_type& replace(size_type pos, size_type len,
								const this_type& right,
								size_type subpos,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, right, subpos, right._utf8_size);
	}

	// replace(pos, len, this_type, subpos, sublen)
	this_type& replace(size_type pos, size_type len,
						const this_type& right,
						size_type subpos, size_type sublen)
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		param_iterator_type i1 = this_type::begin() + pos;
		param_iterator_type i2 = i1 + len;
		size_type add_size = 0;

		if((subpos == right._utf8_size) || (!sublen) )
		{
			basic_string_replace(_base, i1.org(), i2.org(), 0, value_type(0));
		}
		else
		{
			if(!(subpos < right._utf8_size))
			{
				throw std::out_of_range("basic_utf8_string_view::replace invalid subscript");
			}
			assert(sublen);

			add_size = (std::min)(right._utf8_size - subpos, sublen);
			const_iterator start = right.begin() + subpos;
			const_iterator last = start + add_size;

			basic_string_replace(_base, i1.org(), i2.org(), start.org(), last.org());
		}

		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	inline this_type& replace(size_type pos, size_type len,
								const this_type& right,
								size_type subpos, size_type sublen,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, right, subpos, sublen);
	}

	// isomeric_same_this_type
	// replace(pos, len, this_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;
		right_type& right_ref = right;
		return
			this_type::replace(
				pos, len,
				boost::move(reinterpret_cast<this_type&>(right_ref)) );
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;
		right_type& right_ref = right;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::replace(pos, len, reinterpret_cast<const this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, reinterpret_cast<const this_type&>(right));
	}

	// replace(pos, len, this_type, subpos)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos)
	{
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const this_type&>(right),
				subpos, right.length());
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::replace(
				pos, len,
				reinterpret_cast<const this_type&>(right),
				subpos, right.length());
	}

	// replace(pos, len, this_type, subpos, sublen)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen)
	{
		return this_type::replace(pos, len, reinterpret_cast<const this_type&>(right), subpos, sublen);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(size_type pos, size_type len,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				size_type subpos, size_type sublen,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(pos, len, reinterpret_cast<const this_type&>(right), subpos, sublen);
	}

	// replace(iters, itere, base_type)
	inline this_type& replace(param_iterator_type i1, param_iterator_type i2,
								BOOST_RV_REF(base_type) src)
	{
		return this_type::replace(i1, i2, boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						BOOST_RV_REF(base_type) src,
						const string& src_charset_name)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			base_type& src_ref = src;
			if(&_base == &src_ref)
			{
				basic_string_replace(_base, i1.org(), i2.org(), src);
				_utf8_size = (_utf8_size  << 1) - len; // _utf8_size - len + _utf8_size
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				size_type add_size = utf8_foo::strlen(src);
				if(_utf8_size)
				{
					basic_string_replace(_base, i1.org(), i2.org(), src);
				}
				else
				{
					copy_or_move_or_swap(_base, boost::move(src));
				}
				_utf8_size = _utf8_size - len + add_size;
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, boost::move(src), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size = _utf8_size - len + add_size;
		}

		return *this;
	}

	inline this_type& replace(param_iterator_type i1, param_iterator_type i2, const base_type& src)
	{
		return this_type::replace(i1, i2, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						const base_type& src,
						const string& src_charset_name)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			basic_string_replace(_base, i1.org(), i2.org(), src);

			if(&_base == &src)
			{
				_utf8_size = (_utf8_size  << 1) - len; // _utf8_size - len + _utf8_size
			}
			else
			{
				assert(utf8_foo::is_utf8_string(src));
				_utf8_size = _utf8_size - len + utf8_foo::strlen(src);
			}
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type add_size = utf8_foo::strlen(tmp);

			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), tmp);
			}
			else
			{
				_base.swap(tmp);
			}

			_utf8_size = _utf8_size - len + add_size;
		}

		return *this;
	}

	// isomeric_same_base_type

	// replace(iters, itere, base_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::replace(
				i1, i2,
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				BOOST_RV_REF_BEG
					basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				const string& src_charset_name)
	{
		typedef basic_string<value_type, traits_type, Alloc2> src_string_type;
		src_string_type& src_ref = src;
		return
			this_type::replace(
				i1, i2,
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				const basic_string<value_type, traits_type, Alloc2>& src)
	{
		return
			this_type::replace(
				i1, i2,
				reinterpret_cast<const base_type&>(src),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				const basic_string<value_type, traits_type, Alloc2>& src,
				const string& src_charset_name)
	{
		return
			this_type::replace(
				i1, i2,
				reinterpret_cast<const base_type&>(src),
				src_charset_name);
	}

	// replace(iters, itere, this_type)
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						BOOST_RV_REF(this_type) right)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);

		this_type& right_ref = right;
		if(this == &right_ref)
		{
			basic_string_replace(_base, i1.org(), i2.org(), right._base);
			_utf8_size = (_utf8_size << 1) - len; // _utf8_size = _utf8_size - len + _utf8_size;
		}
		else
		{
			if(_utf8_size)
			{
				basic_string_replace(_base, i1.org(), i2.org(), right._base);
			}
			else
			{
				copy_or_move_or_swap(_base, boost::move(right._base));
			}

			_utf8_size = _utf8_size - len + boost::move(right._utf8_size);
		}

		return *this;
	}

	inline this_type& replace(param_iterator_type i1, param_iterator_type i2,
								BOOST_RV_REF(this_type) right,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, boost::move(right));
	}

	inline this_type& replace(param_iterator_type i1, param_iterator_type i2,
								const this_type& right)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);

		basic_string_replace(_base, i1.org(), i2.org(), right._base);
		_utf8_size = _utf8_size - len + right._utf8_size;
		return *this;
	}

	inline this_type& replace(param_iterator_type i1, param_iterator_type i2,
								const this_type& right,
								const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, right);
	}

	// isomeric_same_this_type
	// replace(iters, itere, this_type)
	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		return this_type::replace(i1, i2, boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				BOOST_RV_REF_BEG
					utf8_string_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_string_impl<value_type, traits_type, Alloc2> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		right_type& right_ref = right;
		return this_type::replace(i1, i2, boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::replace(i1, i2, reinterpret_cast<const this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_string_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		replace(param_iterator_type i1, param_iterator_type i2,
				const utf8_string_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::replace(i1, i2, reinterpret_cast<const this_type&>(right));
	}

#endif // _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

	// replace string_charset_helper
	template<typename T1, typename T2> inline
	this_type& replace(size_type pos, size_type len,
						size_type n, const string_charset_helper<T1, T2>& val)
	{
		return this_type::replace(pos, len, n, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& replace(size_type pos, size_type len, const string_charset_helper<T1, T2>& val)
	{
		return this_type::replace(pos, len, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& replace(size_type pos, size_type len,
						const string_charset_helper<T1, T2>& val,
						size_type n_or_subpos)
	{
		return this_type::replace(pos, len, val.first, n_or_subpos, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& replace(size_type pos, size_type len,
						const string_charset_helper<T1, T2>& val,
						size_type subpos, size_type sublen)
	{
		return this_type::replace(pos, len, val.first, subpos, sublen, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						size_type n, const string_charset_helper<T1, T2>& val)
	{
		return this_type::replace(i1, i2, n, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						const string_charset_helper<T1, T2>& val)
	{
		return this_type::replace(i1, i2, val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& replace(param_iterator_type i1, param_iterator_type i2,
						const string_charset_helper<T1, T2>& val,
						size_type n_or_subpos)
	{
		return this_type::replace(i1, i2, val.first, n_or_subpos, val.second);
	}

	//string operations:

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* c_str(void) const YGGR_NOEXCEPT
	{
		return _base.data();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE value_type* data(void) YGGR_NOEXCEPT
	{
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17 )
		return const_cast<value_type*>(_base.data());
#else
		return _base.data();
#endif //(YGGR_CPP_VERSION < YGGR_CPP_VER_17 )
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* data(void) const YGGR_NOEXCEPT
	{
		return _base.data();
	}

	inline allocator_type get_allocator(void) const
	{
		return _base.get_allocator();
	}

	// the return value represents how many T is copied,
	// if sizeof(T) == 1 then this return value is equivalent to byte_size.
	// when the T size is not 1 byte and "charset_name_t" is "utf8",
	// that is, "sizeof(T) * n != byte_size(utf8_str)" then the missing part will be filled with 0.
	// "len" and "pos" is utf8_length
	// other rule same to string::copy

	template<typename T> inline
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len) const
	{
		return this_type::copy(out, len, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(T));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len, const string& src_charset_name) const
	{
		return this_type::copy(out, len, size_type(0), src_charset_name);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len, size_type pos) const
	{
		return this_type::copy(out, len, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(T));
	}

	template<typename T>
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len, size_type pos,
				const string& dst_charset_name) const
	{
		assert(out);
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::copy invalid subscript");
		}

		len = (std::min)(_utf8_size - pos, len);
		if(!len)
		{
			return 0;
		}

		const_iterator s = this_type::begin() + pos;
		const_iterator e = s + len;

		size_type byte_size = std::distance(s.org(), e.org());

		if(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			size_type filled_size = byte_size % sizeof(T);
			if(byte_size) memcpy(out, &(*(s.org())), byte_size);
			if(filled_size) memset(out + byte_size, 0, filled_size);

			assert((0 == (byte_size + filled_size) % sizeof(T)));
			return (byte_size + filled_size) / sizeof(T);
		}
		else
		{
			charset_converter conv(YGGR_STR_INNER_STRING_CHARSET_NAME(), dst_charset_name);

			std::size_t ilen_byte = byte_size;
			std::size_t xchglen = charset_base_foo::xchglen(len);
			std::size_t olen_byte = xchglen;

			if(conv(&(*(s.org())), ilen_byte, reinterpret_cast<char*>(out), olen_byte))
			{
				std::size_t use_byte = xchglen - olen_byte;
				size_type filled_size = use_byte % sizeof(T);
				memset(out + use_byte, 0, filled_size);

				assert((0 == (use_byte + filled_size) % sizeof(T)));
				return (use_byte + filled_size) / sizeof(T);
			}
			else
			{
				return 0;
			}
		}
	}

	//find

	// find(char, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), size_type(0),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::find(static_cast<now_char_type>(c), size_type(0), src_charset_name);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find(Char c, size_type pos) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), pos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find(Char c, size_type pos, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(!(pos < _utf8_size))
			{
				return base_type::npos;
			}

			value_type cmp = static_cast<value_type>(c);
			const_iterator start = (this_type::begin() + pos);
			size_type dpos = utf8_foo::find(start.org(), _base.end(), &cmp);

			return dpos == base_type::npos? base_type::npos : pos + dpos;
		}
		else
		{
			return
				this_type::find(
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name), pos);
		}
	}

	//!!! utf8_char represented by const Char* is equivalent to the find operation utf8_string represented by const Char*,
	//!!! and "size_type find (const charT* s, size_type pos = 0) const" has conflict
	//!!! so don't write function find(const Char*(a char), pos)
	//!!! if you want to find utf8_char(0), please using "find(utf8_char(0));" or "find(const Char*, pos, 1);" to fixit

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c.view(), pos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c,
					size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos);
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		const_iterator start = this_type::begin() + pos;
		size_type dpos = utf8_foo::find(start.org(), _base.end(), c.data());

		return dpos == base_type::npos? base_type::npos : pos + dpos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c, size_type pos,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, pos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c.view(), pos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c, size_type pos,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c.view(), pos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos);
	}

	// find(const char*, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src) const
	{
		return this_type::find(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, const string& src_charset_name) const
	{
		return this_type::find(src, size_type(0), src_charset_name);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos) const
	{
		return this_type::find(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos, const string& src_charset_name) const
	{
		assert(src);

		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
		size_type ret_pos = base_type::npos;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src)));

			ret_pos = utf8_foo::search(_base.data() + base_pos,
										_base.size() - base_pos,
										src,
										charset_base_foo::strlen(src) * mplex::strict_sizeof<Char>::value);
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			ret_pos = utf8_foo::search(_base.data() + base_pos,
										_base.data() + _base.size(),
										tmp);
		}

		return ret_pos == base_type::npos? base_type::npos : pos + ret_pos;
	}

	// find(const char*, pos, n);
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos, size_type n) const
	{
		return this_type::find(src, pos, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos, size_type n, const string& src_charset_name) const
	{
		if(!n)
		{
			return pos < _utf8_size? pos : base_type::npos;
		}

		assert(src);

		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
		size_type ret_pos = base_type::npos;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(
				utf8_foo::is_utf8_string(
					reinterpret_cast<const value_type*>(src),
					n * mplex::strict_sizeof<Char>::value));

			ret_pos = utf8_foo::search(
				_base.data() + base_pos,
				_base.size() - base_pos,
				src,
				n * mplex::strict_sizeof<Char>::value);
		}
		else
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			ret_pos =
				utf8_foo::search(
					_base.data() + base_pos,
					_base.data() + _base.size(),
					tmp);
		}

		return ret_pos == base_type::npos? base_type::npos : pos + ret_pos;
	}

	// find(const strin&, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::find(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return this_type::find(src, size_type(0), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find(const Basic_String<Char, Traits, Alloc>& src, size_type pos) const
	{
		return this_type::find(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			size_type
		>::type
		find(const Basic_String<value_type, Traits, Alloc>& src, size_type pos,
				const string& src_charset_name) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
		size_type ret_pos = base_type::npos;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(utf8_foo::is_utf8_string(src));
			ret_pos = utf8_foo::search(_base.data() + base_pos,
										_base.data() + _base.size(),
										src);
		}
		else
		{
			assert((_base.data() != src.data()));
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			ret_pos = utf8_foo::search(_base.data() + base_pos,
										_base.data() + _base.size(),
										tmp);
		}

		return ret_pos == base_type::npos? base_type::npos : pos + ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find(const Basic_String<Char, Traits, Alloc>& src, size_type pos,
				const string& src_charset_name) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());

		base_type tmp(_base.get_allocator());
		charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type ret_pos = utf8_foo::search(_base.data() + base_pos,
												_base.data() + _base.size(),
												tmp);

		return ret_pos == base_type::npos? base_type::npos : pos + ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_string_impl<Char, Traits, Alloc>& src) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(src, size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_string_impl<Char, Traits, Alloc>& src,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(src, size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
		size_type ret_pos =
			utf8_foo::search(
				_base.data() + base_pos,
				_base.data() + _base.size(),
				src.data(),
				src.data() + src.size());

		return ret_pos == base_type::npos? base_type::npos : pos + ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos,
					const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(src, pos);
	}

	//string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find(str_view.data(), 0, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(str_view.data(), 0, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find(str_view.data(), pos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(str_view.data(), 0, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(str_view.data(), 0, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(str_view.data(), pos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// find string_charest_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const string_charset_helper<T1, T2>& val, size_type pos = 0) const
	{
		return this_type::find(val.first, pos, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const string_charset_helper<T1, T2>& val, size_type pos, size_type n) const
	{
		return this_type::find(val.first, pos, n, val.second);
	}

	// rfind
	// rfind(char, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		rfind(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::rfind(
				static_cast<now_char_type>(c), base_type::npos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		rfind(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::rfind(static_cast<now_char_type>(c), base_type::npos, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		rfind(Char c, size_type pos) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::rfind(
				static_cast<now_char_type>(c), pos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		rfind(Char c, size_type pos, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		size_type ret_pos = base_type::npos;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(_utf8_size)
			{
				value_type cmp = static_cast<value_type>(c);
				size_type base_pos = base_type::npos;

				if(pos < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos = _utf8_size - 1;
				}

				const value_type* i = _base.data() + base_pos;
				const value_type* isize = _base.data() - 1;

				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(*i == cmp) break;
						else --pos;
					}
				}

				ret_pos = (i == isize? base_type::npos : pos);
			}
		}
		else
		{
			ret_pos =
				this_type::rfind(
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name), pos);
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos) const
	{
		return this_type::rfind(c.view(), pos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos,
						const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos);
	}

	template<typename Char, typename Traits> inline
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> inline
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c, size_type pos) const
	{
		assert(c.size());

		size_type ret_pos = base_type::npos;

		if(_utf8_size && (!(_base.size() < c.size())))
		{
			size_type base_pos = base_type::npos;

			if(pos < _utf8_size)
			{
				base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
			}
			else
			{
				base_pos = _base.size() - 1;
				pos = _utf8_size - 1;
			}

			const value_type* i = _base.data() + base_pos;
			const value_type* isize = _base.data() - 1;
			const value_type* cmp = c.data();
			const value_type* iend = _base.data() + _base.size();
			size_type cmp_size = c.size();

			// align
			for(;i != isize && std::distance(i, iend) < cmp_size; --i)
			{
				if((*i & 0xc0) != 0x80)
				{
					--pos;
				}
			}

			// cmp
			for(; i != isize; --i)
			{
				if((*i & 0xc0) != 0x80)
				{
					if(0 == std::memcmp(i, cmp, cmp_size)) break;
					else --pos;
				}
			}

			ret_pos = i == isize? base_type::npos : pos;
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> inline
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c, size_type pos,
						const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, pos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::rfind(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c, size_type pos,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::rfind(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos);
	}

	// rfind(const char* pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src) const
	{
		return this_type::rfind(src, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, const string& src_charset_name) const
	{
		return this_type::rfind(src, base_type::npos, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos) const
	{
		return this_type::rfind(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			base_type tmp(_base.get_allocator());

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src)));

				cmp_size = charset_base_foo::strlen(reinterpret_cast<const value_type*>(src));
				cmp = reinterpret_cast<const value_type*>(src);
			}
			else
			{
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				cmp_size = tmp.size();
				cmp = tmp.data();
			}

			if(0 == cmp_size)
			{
				ret_pos = (pos < _utf8_size? pos : _utf8_size);
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				assert(cmp_size <= _base.size());
				size_type base_pos = base_type::npos;

				if(pos < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos = _utf8_size - 1;
				}

				const value_type* i = _base.data() + base_pos;
				const value_type* isize = _base.data() - 1;
				const value_type* iend = _base.data() + _base.size();

				// align
				for(;i != isize && std::distance(i, iend) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == std::memcmp(i, cmp, cmp_size)) break;
						else --pos;
					}
				}

				ret_pos = i == isize? base_type::npos : pos;
			}
		}

		return ret_pos;
	}

	// rfind(const char*, pos, n)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos, size_type n) const
	{
		return this_type::rfind(src, pos, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos, size_type n, const string& src_charset_name) const
	{
		if(!n)
		{
			return pos < _utf8_size? pos : _utf8_size;
		}

		assert(src);
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			cmp_size = tmp.size();
			cmp = tmp.data();

			if(0 == cmp_size)
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos = _utf8_size - 1;
				}

				const value_type* i = _base.data() + base_pos;
				const value_type* isize = _base.data() - 1;
				const value_type* iend = _base.data() + _base.size();

				// align
				for(;i != isize && std::distance(i, iend) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == std::memcmp(i, cmp, cmp_size)) break;
						else --pos;
					}
				}

				ret_pos = i == isize? base_type::npos : pos;
			}
		}

		return ret_pos;
	}

	// rfind(const string&, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		rfind(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::rfind(src, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		rfind(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return this_type::rfind(src, base_type::npos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		rfind(const Basic_String<Char, Traits, Alloc>& src, size_type pos) const
	{
		return this_type::rfind(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			size_type
		>::type
		rfind(const Basic_String<value_type, Traits, Alloc>& src, size_type pos,
				const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			base_type tmp(_base.get_allocator());

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));
				cmp_size = src.size();
				cmp = src.data();
			}
			else
			{
				assert((_base.data() != src.data()));
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				cmp_size = tmp.size();
				cmp = tmp.data();
			}

			if(0 == cmp_size)
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos = _utf8_size - 1;
				}

				const value_type* i = _base.data() + base_pos;
				const value_type* isize = _base.data() - 1;
				const value_type* iend = _base.data() + _base.size();

				// align
				for(;i != isize && std::distance(i, iend) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == std::memcmp(i, cmp, cmp_size)) break;
						else --pos;
					}
				}

				ret_pos = i == isize? base_type::npos : pos;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		rfind(const Basic_String<Char, Traits, Alloc>& src, size_type pos,
				const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			cmp_size = tmp.size();
			cmp = tmp.data();

			if(0 == cmp_size)
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos = _utf8_size - 1;
				}

				const value_type* i = _base.data() + base_pos;
				const value_type* isize = _base.data() - 1;
				const value_type* iend = _base.data() + _base.size();

				// align
				for(;i != isize && std::distance(i, iend) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == std::memcmp(i, cmp, cmp_size)) break;
						else --pos;
					}
				}

				ret_pos = i == isize? base_type::npos : pos;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_string_impl<Char, Traits, Alloc>& src) const
	{
		return this_type::rfind(src, base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_string_impl<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(src, base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type rfind(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			cmp_size = src.size();
			cmp = src.data();

			if(0 == cmp_size)
			{
				ret_pos =  pos < _utf8_size? pos : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos = _utf8_size - 1;
				}

				const value_type* i = _base.data() + base_pos;
				const value_type* isize = _base.data() - 1;
				const value_type* iend = _base.data() + _base.size();

				// align
				for(;i != isize && std::distance(i, iend) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == std::memcmp(i, cmp, cmp_size)) break;
						else --pos;
					}
				}

				ret_pos = i == isize? base_type::npos : pos;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type rfind(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos,
						const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(src, pos);
	}

	//string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::rfind(str_view.data(), base_type::npos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(str_view.data(), base_type::npos, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::rfind(str_view.data(), pos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(str_view.data(), base_type::npos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(str_view.data(), base_type::npos, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(str_view.data(), pos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		rfind(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// rfind string_charset_helper
	template<typename T1, typename T2> inline
	size_type rfind(const string_charset_helper<T1, T2>& val, size_type pos = base_type::npos) const
	{
		return this_type::rfind(val.first, pos, val.second);
	}

	template<typename T1, typename T2> inline
	size_type rfind(const string_charset_helper<T1, T2>& val, size_type pos, size_type n) const
	{
		return this_type::rfind(val.first, pos, n, val.second);
	}

	// find_first_of

	// find_first_of(char, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_of(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), size_type(0),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_of(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::find(static_cast<now_char_type>(c), size_type(0), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_of(Char c, size_type pos) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), pos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_of(Char c, size_type pos, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::find(static_cast<now_char_type>(c), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos) const
	{
		return this_type::find(c.view(), pos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos);
	}

	template<typename Char, typename Traits> inline
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> inline
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> inline
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos) const
	{
		return this_type::find(c, pos);
	}

	template<typename Char, typename Traits> inline
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, pos);
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::find(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::find(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos);
	}

	// find_first_of(const char*, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src) const
	{
		return this_type::find_first_of(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, const string& src_charset_name) const
	{
		return this_type::find_first_of(src, size_type(0), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos) const
	{
		return this_type::find_first_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src)));
				src_len = utf8_foo::strlen(src);
				pbuf = reinterpret_cast<const value_type*>(src);
			}
			else
			{
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	// find_first_of(const char*, pos, n);
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos, size_type n) const
	{
		return this_type::find_first_of(src, pos, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos, size_type n, const string& src_charset_name) const
	{
		if(!n)
		{
			return base_type::npos;
		}

		assert(src);

		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	// find_first_of(const strin&, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_of(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::find_first_of(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_of(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return this_type::find_first_of(src, size_type(0), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos) const
	{
		return this_type::find_first_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			size_type
		>::type
		find_first_of(const Basic_String<value_type, Traits, Alloc>& src, size_type pos,
						const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size && src.size())
		{

			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));
				src_len = utf8_foo::strlen(src);
				pbuf = src.data();
			}
			else
			{
				assert((_base.data() != src.data()));
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos,
						const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		base_type tmp(_base.get_allocator());
		charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
		size_type src_len = utf8_foo::strlen(tmp);

		if(src_len)
		{
			const value_type* pbuf = tmp.data();
			const value_type* pstart = &(*((this_type::begin() + pos).org()));
			for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
			{
				if(utf8_foo::memchr(pbuf, pstart, src_len))
				{
					ret_pos = i;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_string_impl<Char, Traits, Alloc>& src) const
	{
		return this_type::find_first_of(src, size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_string_impl<Char, Traits, Alloc>& src,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_of(src, size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_first_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos) const
	{
		size_type ret_pos = base_type::npos;
		size_type src_len = src.length();

		if(pos < _utf8_size && src_len)
		{
			const value_type* pbuf = src.data();
			const value_type* pstart = _base.data() + std::distance(_base.begin(), (this_type::begin() + pos).org());
			for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
			{
				if(utf8_foo::memchr(pbuf, pstart, src_len))
				{
					ret_pos = i;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_of(src, pos);
	}

	//string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_first_of(str_view.data(), 0, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(str_view.data(), 0, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_first_of(str_view.data(), pos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(str_view.data(), 0, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_of(str_view.data(), 0, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(str_view.data(), pos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// find_firset_of string_charest_helper
	template<typename T1, typename T2> inline
	size_type find_first_of(const string_charset_helper<T1, T2>& val, size_type pos = 0) const
	{
		return this_type::find_first_of(val.first, pos, val.second);
	}

	template<typename T1, typename T2> inline
	size_type find_first_of(const string_charset_helper<T1, T2>& val, size_type pos, size_type n) const
	{
		return this_type::find_first_of(val.first, pos, n, val.second);
	}

	// find_first_not_of

	// find_first_not_of(char, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_not_of(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find_first_not_of(
				static_cast<now_char_type>(c), size_type(0),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_not_of(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::find_first_not_of(static_cast<now_char_type>(c), size_type(0), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_not_of(Char c, size_type pos) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find_first_not_of(
				static_cast<now_char_type>(c), pos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_not_of(Char c, size_type pos, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		size_type ret_pos = base_type::npos;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(pos < _utf8_size)
			{
				value_type buf[2] = { static_cast<value_type>(c) };

				const value_type* pbuf = &buf[0];
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, 1))
					{
						ret_pos = i;
						break;
					}
				}
			}
		}
		else
		{
			ret_pos =
				this_type::find_first_not_of(
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name), pos);
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos) const
	{
		return this_type::find_first_not_of(c.view(), pos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), pos);
	}

	template<typename Char, typename Traits> inline
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find_first_not_of(c, size_type(0));
	}

	template<typename Char, typename Traits> inline
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c, size_type(0));
	}

	template<typename Char, typename Traits>
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos) const
	{
		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			const value_type* pbuf = c.data();
			const value_type* pstart = &(*((this_type::begin() + pos).org()));
			for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
			{
				if(!utf8_foo::memchr(pbuf, pstart, 1))
				{
					ret_pos = i;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> inline
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c, pos);
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::find_first_not_of(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::find_first_not_of(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos,
									const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), pos);
	}

	// find_first_not_of(const char*, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src) const
	{
		return this_type::find_first_not_of(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, const string& src_charset_name) const
	{
		return this_type::find_first_not_of(src, size_type(0), src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos) const
	{
		return this_type::find_first_not_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src)));
				src_len = utf8_foo::strlen(src);
				pbuf = reinterpret_cast<const value_type*>(src);
			}
			else
			{
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos;
			}
		}

		return ret_pos;
	}

	// find_first_not_of(const char*, pos, n);
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos, size_type n) const
	{
		return this_type::find_first_not_of(src, pos, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos, size_type n, const string& src_charset_name) const
	{
		if(!n)
		{
			return pos < _utf8_size? pos : base_type::npos;
		}

		assert(src);

		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos;
			}
		}

		return ret_pos;
	}

	// find_first_not_of(const strin&, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_not_of(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::find_first_not_of(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_not_of(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return this_type::find_first_not_of(src, size_type(0), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_not_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos) const
	{
		return this_type::find_first_not_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			size_type
		>::type
		find_first_not_of(const Basic_String<value_type, Traits, Alloc>& src, size_type pos,
							const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));
				src_len = utf8_foo::strlen(src);
				pbuf = src.data();
			}
			else
			{
				assert((_base.data() != src.data()));
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_first_not_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos,
				const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_string_impl<Char, Traits, Alloc>& src) const
	{
		return this_type::find_first_not_of(src, size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(src, size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_first_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos) const
	{
		size_type ret_pos = base_type::npos;

		if(pos < _utf8_size)
		{
			size_type src_len = src.length();

			if(src_len)
			{
				const value_type* pbuf = src.data();
				const value_type* pstart = &(*((this_type::begin() + pos).org()));
				for(size_type i = pos, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, src_len))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_first_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(src, pos);
	}

	//string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_first_not_of(str_view.data(), 0, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(str_view.data(), 0, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_first_not_of(str_view.data(), pos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(str_view.data(), 0, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(str_view.data(), 0, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(str_view.data(), pos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_first_not_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// find_first_not_of string_charest_helper
	template<typename T1, typename T2> inline
	size_type find_first_not_of(const string_charset_helper<T1, T2>& val, size_type pos = 0) const
	{
		return this_type::find_first_not_of(val.first, pos, val.second);
	}

	template<typename T1, typename T2> inline
	size_type find_first_not_of(const string_charset_helper<T1, T2>& val, size_type pos, size_type n) const
	{
		return this_type::find_first_not_of(val.first, pos, n, val.second);
	}

	// find_last_of

	// find_last_of(char, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_of(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::rfind(
				static_cast<now_char_type>(c), base_type::npos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_of(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::rfind(static_cast<now_char_type>(c), base_type::npos, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_of(Char c, size_type pos) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::rfind(
				static_cast<now_char_type>(c), pos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_of(Char c, size_type pos, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::rfind(static_cast<now_char_type>(c), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos) const
	{
		return this_type::rfind(c.view(), pos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos);
	}

	template<typename Char, typename Traits> inline
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> inline
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> inline
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos) const
	{
		return this_type::rfind(c, pos);
	}

	template<typename Char, typename Traits> inline
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, pos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::rfind(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::rfind(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos);
	}

	// find_last_of(const char*, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src) const
	{
		return this_type::find_last_of(src, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, const string& src_charset_name) const
	{
		return this_type::find_last_of(src, base_type::npos, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos) const
	{
		return this_type::find_last_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos, const string& src_charset_name) const
	{
		assert(src);
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src)));
				src_len = utf8_foo::strlen(src);
				pbuf = reinterpret_cast<const value_type*>(src);
			}
			else
			{
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	// find_last_of(const char*, pos, n);
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos, size_type n) const
	{
		return this_type::find_last_of(src, pos, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos, size_type n, const string& src_charset_name) const
	{
		if(!n)
		{
			return base_type::npos;
		}

		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	// find_last_of(const strin&, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_of(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::find_last_of(src, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_of(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return this_type::find_last_of(src, base_type::npos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos) const
	{
		return this_type::find_last_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			size_type
		>::type
		find_last_of(const Basic_String<value_type, Traits, Alloc>& src, size_type pos,
						const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));
				src_len = utf8_foo::strlen(src);
				pbuf = src.data();
			}
			else
			{
				assert((_base.data() != src.data()));
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos,
						const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if((_utf8_size && src.size()))
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_string_impl<Char, Traits, Alloc>& src) const
	{
		return this_type::find_last_of(src, base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_string_impl<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(src, base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_last_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos) const
	{
		size_type ret_pos = base_type::npos;
		size_type src_len = src.length();

		if((_utf8_size && src_len))
		{
			const value_type* pbuf = src.data();
			const_reverse_iterator i;
			if(pos < _utf8_size)
			{
				i = const_reverse_iterator(this_type::begin() + (pos + 1));
			}
			else
			{
				pos = _utf8_size - 1;
				i = this_type::rbegin();
			}

			for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
			{
				if(utf8_foo::memchr(pbuf, &(*i), src_len))
				{
					ret_pos = pos;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(src, pos);
	}

	//string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_of(str_view.data(), base_type::npos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(str_view.data(), base_type::npos, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_of(str_view.data(), pos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(str_view.data(), base_type::npos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(str_view.data(), base_type::npos, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(str_view.data(), pos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// find_last_of string_charest_helper
	template<typename T1, typename T2> inline
	size_type find_last_of(const string_charset_helper<T1, T2>& val, size_type pos = base_type::npos) const
	{
		return this_type::find_last_of(val.first, pos, val.second);
	}

	template<typename T1, typename T2> inline
	size_type find_last_of(const string_charset_helper<T1, T2>& val, size_type pos, size_type n) const
	{
		return this_type::find_last_of(val.first, pos, n, val.second);
	}

	// find_last_not_of

	// find_last_not_of(char, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_not_of(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find_last_not_of(
				static_cast<now_char_type>(c), base_type::npos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_not_of(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::find_last_not_of(static_cast<now_char_type>(c), base_type::npos, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_not_of(Char c, size_type pos) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find_last_not_of(
				static_cast<now_char_type>(c), pos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_not_of(Char c, size_type pos, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		size_type ret_pos = base_type::npos;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(_utf8_size)
			{

				value_type buf[2] = { static_cast<value_type>(c) };

				const value_type* pbuf = &buf[0];
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), 1))
					{
						ret_pos = pos;
						break;
					}
				}
			}
		}
		else
		{
			ret_pos =
				this_type::find_last_not_of(
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name), pos);
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos) const
	{
		return this_type::find_last_not_of(c.view(), pos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), pos);
	}

	template<typename Char, typename Traits> inline
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find_last_not_of(c, base_type::npos);
	}

	template<typename Char, typename Traits> inline
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c, base_type::npos);
	}

	template<typename Char, typename Traits>
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			const value_type* pbuf = c.data();
			const_reverse_iterator i;
			if(pos < _utf8_size)
			{
				i = const_reverse_iterator(this_type::begin() + (pos + 1));
			}
			else
			{
				pos = _utf8_size - 1;
				i = this_type::rbegin();
			}

			for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
			{
				if(!utf8_foo::memchr(pbuf, &(*i), 1))
				{
					ret_pos = pos;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> inline
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c, pos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::find_last_not_of(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos) const
	{
		return this_type::find_last_not_of(c.view(), pos);
	}

	template<typename Utf8String> inline
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), pos);
	}

	// find_last_not_of(const char*, pos)
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src) const
	{
		return this_type::find_last_not_of(src, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, const string& src_charset_name) const
	{
		return this_type::find_last_not_of(src, base_type::npos, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos) const
	{
		return this_type::find_last_not_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src)));
				src_len = utf8_foo::strlen(src);
				pbuf = reinterpret_cast<const value_type*>(src);
			}
			else
			{
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	// find_last_not_of(const char*, pos, n);
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos, size_type n) const
	{
		return this_type::find_last_not_of(src, pos, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos, size_type n, const string& src_charset_name) const
	{
		if(!n)
		{
			return pos < _utf8_size? pos : _utf8_size - 1;
		}

		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	// find_last_not_of(const strin&, pos)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_not_of(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::find_last_not_of(src, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_not_of(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return this_type::find_last_not_of(src, base_type::npos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_not_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos) const
	{
		return this_type::find_last_not_of(src, pos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<value_type, Traits, Alloc> >,
			size_type
		>::type
		find_last_not_of(const Basic_String<value_type, Traits, Alloc>& src, size_type pos,
							const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			size_type src_len = 0;
			const value_type* pbuf = 0;

			if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				assert(utf8_foo::is_utf8_string(src));
				src_len = utf8_foo::strlen(src);
				pbuf = src.data();
			}
			else
			{
				assert((_base.data() != src.data()));
				charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
				src_len = utf8_foo::strlen(tmp);
				pbuf = tmp.data();
			}

			if(src_len)
			{
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		find_last_not_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos,
				const string& src_charset_name) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			base_type tmp(_base.get_allocator());
			charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_string_impl<Char, Traits, Alloc>& src) const
	{
		return this_type::find_last_not_of(src, base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_string_impl<Char, Traits, Alloc>& src,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(src, base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_last_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type src_len = src.length();

			if(src_len)
			{
				const value_type* pbuf = src.data();
				const_reverse_iterator i;
				if(pos < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos + 1));
				}
				else
				{
					pos = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos < _utf8_size? pos : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	size_type find_last_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(src, pos);
	}

	//string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_not_of(str_view.data(), base_type::npos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(str_view.data(), base_type::npos, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_not_of(str_view.data(), pos, str_view.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(str_view.data(), base_type::npos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(str_view.data(), base_type::npos, str_view.size(), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view, size_type pos) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(str_view.data(), pos, str_view.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			size_type
		>::type
		find_last_not_of(const StringView& str_view, size_type pos, const string& src_charset_name) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(str_view.data(), pos, str_view.size(), src_charset_name);
	}

	// find_first_not_of string_charest_helper
	template<typename T1, typename T2> inline
	size_type find_last_not_of(const string_charset_helper<T1, T2>& val, size_type pos = base_type::npos) const
	{
		return this_type::find_last_not_of(val.first, pos, val.second);
	}

	template<typename T1, typename T2> inline
	size_type find_last_not_of(const string_charset_helper<T1, T2>& val, size_type pos, size_type n) const
	{
		return this_type::find_last_not_of(val.first, pos, n, val.second);
	}

	// substr
	inline this_type substr(size_type pos = 0, size_type len = base_type::npos) const
	{
		// why this place using ==,
		// because std::string using it, if pos > _utf8_size using assert(false)
		if((pos == _utf8_size) || (!len))
		{
			return this_type(_base.get_allocator());
		}
		else
		{
			if(!(pos < _utf8_size))
			{
				throw std::out_of_range("basic_utf8_string_view::substr invalid subscript");
			}
			assert(len);

			len = (std::min)(len, _utf8_size - pos);

			const_iterator start(this_type::begin() + pos);
			const_iterator last(pos + len == _utf8_size? this_type::end() : start + len);

			return this_type(start, last, _base.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
		}
	}

	template<typename String> inline
	String substr(size_type pos = 0, size_type len = base_type::npos,
					const string& dst_charset_name = charset_name_t<String>()) const
	{
		return this_type::substr(pos, len).template str<String>(dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(const Alloc& alloc) const
	{
		return this_type::substr(0, base_type::npos).template str<String>(alloc, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(const Alloc& alloc, const string& dst_charset_name) const
	{
		return this_type::substr(0, base_type::npos).template str<String>(alloc, dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(size_type pos, const Alloc& alloc) const
	{
		return this_type::substr(pos, base_type::npos).template str<String>(alloc, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(size_type pos, size_type len,
					const Alloc& alloc,
					const string& dst_charset_name = charset_name_t<String>()) const
	{
		return this_type::substr(pos, len).template str<String>(alloc, dst_charset_name);
	}

	// compare_eq
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(const Char* src) const
	{
		return 0 == this_type::compare(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(const Char* src, const string& src_charset_name) const
	{
		return 0 == this_type::compare(src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, bool>::type
		compare_eq(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return 0 == this_type::compare(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, bool>::type
		compare_eq(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		return 0 == this_type::compare(src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(const utf8_string_impl<Char, Traits, Alloc>& other) const
	{
		return
			(_base.data() == other.data() && _base.size() == other.size() && _utf8_size == other.length())
			|| (0 == utf8_foo::strcmp(_base.data(), other.data(), _base.size(), other.size()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(const utf8_string_impl<Char, Traits, Alloc>& other,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			(_base.data() == other.data() && _base.size() == other.size() && _utf8_size == other.length())
			|| (0 == utf8_foo::strcmp(_base.data(), other.data(), _base.size(), other.size()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (0 == utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(const this_type& right, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare_eq(right);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const Char* src) const
	{
		return 0 == this_type::compare(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const Char* src,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, src, src_charset_name);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const Char* src, size_type n) const
	{
		return 0 == this_type::compare(pos, len, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const Char* src, size_type n,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, src, n, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	bool compare_eq(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src) const
	{
		return 0 == this_type::compare(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, bool>::type
		compare_eq(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other) const
	{
		return 0 == this_type::compare(pos, len, other);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return 0 == this_type::compare<Char, Traits, Alloc>(pos, len, other);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos, size_type len,
					const Char* src, size_type subpos, size_type sublen) const
	{
		return 0 == this_type::compare(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos, size_type len,
					const Char* src, size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, src, subpos, sublen, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	bool compare_eq(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen) const
	{
		return 0 == this_type::compare(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, bool>::type
		compare_eq(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, src, subpos, sublen, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other,
					size_type subpos, size_type sublen) const
	{
		return this_type::compare_eq(pos, len, other, subpos, sublen);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool compare_eq(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other,
					size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare(pos, len, other, subpos, sublen);
	}

	// string_view
	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(const StringView& str_view) const
	{
		return 0 == this_type::compare(str_view);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(const StringView& str_view, const string& src_charset_name) const
	{
		return 0 == this_type::compare(str_view, src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const StringView& str_view) const
	{
		return 0 == this_type::compare(pos, len, str_view);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const StringView& str_view, const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, str_view, src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const StringView& str_view, size_type subpos) const
	{
		return 0 == this_type::compare(pos, len, str_view, subpos);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const StringView& str_view, size_type subpos,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, str_view, subpos, src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			s32
		>::type
		compare_eq(size_type pos, size_type len, const StringView& str_view,
					size_type subpos, size_type sublen) const
	{
		return 0 == this_type::compare(pos, len, str_view, subpos, sublen);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_string_view_t<StringView>,
			bool
		>::type
		compare_eq(size_type pos, size_type len, const StringView& str_view,
					size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos, len, str_view, subpos, sublen, src_charset_name);
	}

	template<typename T1, typename T2> inline
	bool compare_eq(const string_charset_helper<T1, T2>& right) const
	{
		return 0 == this_type::compare(right.first, right.second);
	}

	template<typename T1, typename T2> inline
	bool compare_eq(size_type pos, size_type len, const string_charset_helper<T1, T2>& src, size_type n) const
	{
		return 0 == this_type::compare(pos, len, src.first, n, src.second);
	}

	template<typename T1, typename T2> inline
	bool compare_eq(size_type pos, size_type len, const string_charset_helper<T1, T2>& right,
					size_type subpos, size_type sublen) const
	{
		return 0 == this_type::compare(pos, len, right.first, subpos, sublen, right.second);
	}

	// compare
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(const Char* src) const
	{
		return this_type::compare(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(const Char* src, const string& src_charset_name) const
	{
		assert(src);
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(_base, src)
				: utf8_foo::strcmp(
					_base,
					charset_foo::s_xchg_diff_charset<base_type>(
						src, _base.get_allocator(),
						src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, s32>::type
		compare(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::compare(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, s32>::type
		compare(const Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name) const
	{
		typedef Basic_String<Char, Traits, Alloc> now_string_type;
		typedef utf8_string_impl<Char, Traits, Alloc> chk_string_type;

		BOOST_MPL_ASSERT_NOT((boost::is_same<now_string_type, chk_string_type>));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(_base, src)
				: utf8_foo::strcmp(
					_base,
					charset_foo::s_xchg_diff_charset<base_type>(
					src, _base.get_allocator(),
					src_charset_name,
					YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_string_impl<Char, Traits, Alloc>& other) const
	{
		return utf8_foo::strcmp(_base.data(), other.data(), _base.size(), other.size());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_string_impl<Char, Traits, Alloc>& other,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::strcmp(_base.data(), other.data(), _base.size(), other.size());
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos, size_type len, const Char* src) const
	{
		return this_type::compare(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos, size_type len, const Char* src,
					const string& src_charset_name) const
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + (std::min)(_utf8_size - pos, len));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(
					&(*(start.org())), src,
					std::distance(start.org(), last.org()))
				: utf8_foo::strcmp(
					&(*start.org()),
					charset_foo::s_xchg_diff_charset<base_type>(
						src, _base.get_allocator(),
						src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					std::distance(start.org(), last.org()));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos, size_type len, const Char* src, size_type n) const
	{
		return this_type::compare(pos, len, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos, size_type len, const Char* src, size_type n,
				const string& src_charset_name) const
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + (std::min)(_utf8_size - pos, len));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(&(*start.org()), src,
									std::distance(start.org(), last.org()), n * sizeof(Char))
				: utf8_foo::strcmp(&(*start.org()),
										charset_foo::s_xchg_diff_charset<base_type>(
											src, n, _base.get_allocator(),
											src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
										std::distance(start.org(), last.org()));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	s32 compare(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::compare(pos, len, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, s32>::type
		compare(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name) const
	{
		typedef Basic_String<Char, Traits, Alloc> now_string_type;
		typedef utf8_string_impl<Char, Traits, Alloc> chk_string_type;

		BOOST_MPL_ASSERT_NOT((boost::is_same<now_string_type, chk_string_type>));

		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + (std::min)(_utf8_size - pos, len));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(&(*(start.org())), src, std::distance(start.org(), last.org()))
				: utf8_foo::strcmp(&(*(start.org())),
									charset_foo::s_xchg_diff_charset<base_type>(
										src, _base.get_allocator(),
										src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
									std::distance(start.org(), last.org()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other) const
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + (std::min)(_utf8_size - pos, len));

		return utf8_foo::strcmp(&(*(start.org())), other.data(),
								std::distance(start.org(), last.org()),
								other.size());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare<Char, Traits, Alloc>(pos, len, other);
	}

	// compare(const string&, subpos, sublen)
	// why is the compare(const Char* src, subpos, sublen) version available,
	// because the third argument to "compare(pos, len, const string&, subpos, sublen)"
	//	in "std::string" uses const "std::string&", so "const char*" is automatically converted to "std::string",
	// but in "utf8_string",
	// because "utf8_string" need to support "const char*", "const wchat_t*"
	//	together with some other data can be expressed as string,
	//	so lost the conversion ability

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos, size_type len,
				const Char* src, size_type subpos, size_type sublen) const
	{
		return this_type::compare(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos, size_type len,
				const Char* src, size_type subpos, size_type sublen,
				const string& src_charset_name) const
	{
		if(!(pos <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + (std::min)(_utf8_size - pos, len));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(
					&(*(start.org())), src + subpos,
					std::distance(start.org(), last.org()), sublen * sizeof(Char))
				: utf8_foo::strcmp(
					&(*(start.org())),
					charset_foo::s_xchg_diff_charset<base_type>(
						src + subpos, sublen, _base.get_allocator(),
						src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	s32 compare(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen) const
	{
		return this_type::compare(pos, len, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<is_native_char<Char>, s32>::type
		compare(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		typedef Basic_String<Char, Traits, Alloc> now_string_type;
		typedef utf8_string_impl<Char, Traits, Alloc> chk_string_type;

		BOOST_MPL_ASSERT_NOT((boost::is_same<now_string_type, chk_string_type>));

		if(!(pos <= _utf8_size && subpos <= src.size()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + (std::min)(_utf8_size - pos, len));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(
					&(*(start.org())),
					src.data() + subpos,
					std::distance(start.org(), last.org()),
					((std::min)(src.size() - subpos, sublen)) * sizeof(Char))
				: utf8_foo::strcmp(
					&(*(start.org())),
					charset_foo::s_xchg_diff_charset<base_type>(
						src.data() + subpos,
						(std::min)(src.size() - subpos, sublen),
						_base.get_allocator(),
						src_charset_name,
						YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(size_type pos, size_type len,
				const utf8_string_impl<Char, Traits, Alloc>& other,
				size_type subpos, size_type sublen) const
	{
		typedef utf8_string_impl<Char, Traits, Alloc> right_type;
		typedef typename right_type::const_iterator right_iter_type;

		if(!(pos <= _utf8_size && subpos <= other.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator sl = this_type::begin() + pos;
		const_iterator el = sl + (std::min)(_utf8_size - pos, len);

		right_iter_type sr = other.begin() + subpos;
		right_iter_type er = sr + (std::min)(other.length() - subpos, sublen);

		return
			utf8_foo::strcmp(
				&(*(sl.org())),
				&(*(sr.org)),
				std::distance(sl.org(), el.org()),
				std::distance(sr.org(), er.org()) );
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other,
					size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare(pos, len, other, subpos, sublen);
	}

	// string_view
	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(const StringView& str_view) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::compare(
				0, _utf8_size, str_view.data(), str_view.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(const StringView& str_view, const string& src_charset_name) const
	{
		return
			this_type::compare(
				0, _utf8_size, str_view.data(), str_view.size(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::compare(
				pos, len, str_view.data(), str_view.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view, const string& src_charset_name) const
	{
		return
			this_type::compare(
				pos, len, str_view.data(), str_view.size(),
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view, size_type subpos) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		return
			this_type::compare(
				pos, len,
				advance_copy(str_view.data(), subpos),
				str_view.size() - subpos,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view, size_type subpos,
					const string& src_charset_name) const
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		return
			this_type::compare(
				pos, len,
				advance_copy(str_view.data(), subpos),
				str_view.length() - subpos,
				src_charset_name);
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view,
				size_type subpos, size_type sublen) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		return
			this_type::compare(
				pos, len,
				advance_copy(str_view.data(), subpos),
				(std::min)(sublen, str_view.length() - subpos),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view,
				size_type subpos, size_type sublen,
				const string& src_charset_name) const
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		return
			this_type::compare(
				pos, len,
				advance_copy(str_view.data(), subpos),
				(std::min)(sublen, str_view.size() - subpos),
				src_charset_name);
	}

	// utf8_string_view
	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(const StringView& str_view) const
	{
		return
			this_type::compare(
				0, _utf8_size, str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(const StringView& str_view, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return
			this_type::compare(
				0, _utf8_size, str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view) const
	{
		return
			this_type::compare(
				pos, len, str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::compare(
				pos, len, str_view.data(), str_view.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view, size_type subpos) const
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		size_type byte_subpos = utf8_foo::utf8_length_to_byte_size(str_view.data(), subpos);
		size_type byte_sublen =
			utf8_foo::utf8_length_to_byte_size(
				reinterpret_cast<const char*>(str_view.data()) + byte_subpos, str_view.length());

		return
			this_type::compare(
				pos, len,
				advance_copy(reinterpret_cast<const char*>(str_view.data()), byte_subpos),
				byte_sublen,
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view, size_type subpos,
					const string& src_charset_name) const
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		size_type byte_subpos = utf8_foo::utf8_length_to_byte_size(str_view.data(), subpos);
		size_type byte_sublen =
			utf8_foo::utf8_length_to_byte_size(
				reinterpret_cast<const char*>(str_view.data()) + byte_subpos, str_view.length());

		return
			this_type::compare(
				pos, len,
				advance_copy(reinterpret_cast<const char*>(str_view.data()), byte_subpos),
				byte_sublen,
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view,
				size_type subpos, size_type sublen) const
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		size_type byte_subpos = utf8_foo::utf8_length_to_byte_size(str_view.data(), subpos);
		size_type byte_sublen =
			utf8_foo::utf8_length_to_byte_size(
				reinterpret_cast<const char*>(str_view.data()) + byte_subpos, sublen);

		return
			this_type::compare(
				pos, len,
				advance_copy(reinterpret_cast<const char*>(str_view.data()), byte_subpos),
				byte_sublen,
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			is_utf8_string_view_t<StringView>,
			s32
		>::type
		compare(size_type pos, size_type len, const StringView& str_view,
				size_type subpos, size_type sublen,
				const string& src_charset_name) const
	{
		if(subpos > str_view.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare of range position");
		}

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		size_type byte_subpos = utf8_foo::utf8_length_to_byte_size(str_view.data(), subpos);
		size_type byte_sublen =
			utf8_foo::utf8_length_to_byte_size(
				reinterpret_cast<const char*>(str_view.data()) + byte_subpos, str_view.length());

		return
			this_type::compare(
				pos, len,
				advance_copy(reinterpret_cast<const char*>(str_view.data()), byte_subpos),
				byte_sublen,
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename T1, typename T2> inline
	s32 compare(const string_charset_helper<T1, T2>& right) const
	{
		return this_type::compare(right.first, right.second);
	}

	template<typename T1, typename T2> inline
	s32 compare(size_type pos, size_type len, const string_charset_helper<T1, T2>& src, size_type n) const
	{
		return this_type::compare(pos, len, src.first, n, src.second);
	}

	template<typename T1, typename T2> inline
	s32 compare(size_type pos, size_type len, const string_charset_helper<T1, T2>& right,
					size_type subpos, size_type sublen) const
	{
		return this_type::compare(pos, len, right.first, subpos, sublen, right.second);
	}

	// xxx_with
	//starts_with
	template<typename Utf8StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<Utf8StringView>, bool>::type
		starts_with(const Utf8StringView& right) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return
			(!(_utf8_size < right.length()))
			&& (utf8_foo::strcmp(_base.data(), right.data(), right.size()) == 0);
	}

	template<typename Utf8StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<Utf8StringView>, bool>::type
		starts_with(const Utf8StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::starts_with(right);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		starts_with(const StringView& right,
					const string& src_charset_name
						= charset_name_t<typename range_ex::range_value_ex<StringView>::type >()) const YGGR_NOEXCEPT_OR_NOTHROW
	{

		base_type tmp(_base.get_allocator());
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				(!(_base.size() < right.size()))
					&& (utf8_foo::strcmp(_base.data(), right.data(), right.size()) == 0)
				: (tmp = charset_foo::s_xchg_diff_charset<base_type>(
							right.data(), right.size(), _base.get_allocator(),
							src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					(!(_base.size() < tmp.size()))
						&& (utf8_foo::strcmp(_base.data(), tmp.data(), tmp.size()) == 0));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::not_< is_string_view_t<Char> >,
			bool
		>::type
		starts_with(const Char& c,
						const string& src_charset_name = charset_name_t<Char>()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			_base.data() && _base.size() && _utf8_size
			&& 0 == this_type::front().compare(c, src_charset_name);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		starts_with(const Char* src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		base_type tmp(_base.get_allocator());
		size_type rbyte_size = 0;
		bool bret = false;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			rbyte_size = charset_base_foo::strlen(src) * mplex::strict_sizeof<Char>::value;
			bret = (!(_base.size() < rbyte_size)) && (utf8_foo::strcmp(_base.data(), src, rbyte_size) == 0);
		}
		else
		{
			tmp = charset_foo::s_xchg_diff_charset<base_type>(
					src, _base.get_allocator(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			bret =
				(!(src && *src && tmp.size() == 0))
				&& (!(_base.size() < tmp.size()))
				&& (utf8_foo::strcmp(_base.data(), tmp.data(), tmp.size()) == 0);
		}

		return bret;
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const string_charset_helper<T1, T2>& val) const
	{
		return this_type::starts_with(val.first, val.second);
	}

	// ends_with
	template<typename Utf8StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<Utf8StringView>, bool>::type
		ends_with(const Utf8StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			(!(_utf8_size < right.length()))
			&& (utf8_foo::strcmp(
				::yggr::advance_copy(this_type::begin(), _utf8_size - right.length()),
				right.data(),
				right.size()) == 0);
	}

	template<typename Utf8StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<Utf8StringView>, bool>::type
		ends_with(const Utf8StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::ends_with(right);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		ends_with(const StringView& right,
					const string& src_charset_name
						= charset_name_t<typename range_ex::range_value_ex<StringView>::type >()) const YGGR_NOEXCEPT_OR_NOTHROW
	{

		base_type tmp(_base.get_allocator());

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				(!(_base.size() < right.size()))
					&& (utf8_foo::strcmp(
						::yggr::advance_copy(_base.begin(), _base.size() - right.size()),
						right.data(),
						right.size()) == 0)
				: (tmp = charset_foo::s_xchg_diff_charset<base_type>(
							right.data(), right.size(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					(!(_base.size() < tmp.size()))
						&& (utf8_foo::strcmp(
								::yggr::advance_copy(_base.begin(), _base.size() - tmp.size()),
								tmp.data(),
								tmp.size()) == 0));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::not_< is_string_view_t<Char> >,
			bool
		>::type
		ends_with(const Char& c,
					const string& src_charset_name = charset_name_t<Char>()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			_base.data() && _base.size() && _utf8_size
			&& 0 == this_type::back().compare(c, src_charset_name);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		ends_with(const Char* src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		base_type tmp(_base.get_allocator());
		size_type rbyte_size = 0;
		bool bret = false;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			rbyte_size = charset_base_foo::strlen(src) * mplex::strict_sizeof<Char>::value;
			bret =
				(!(_base.size() < rbyte_size))
				&& (utf8_foo::strcmp(
						::yggr::advance_copy(this_type::begin().org(), _base.size() - rbyte_size),
						src,
						rbyte_size) == 0);
		}
		else
		{
			tmp = charset_foo::s_xchg_diff_charset<base_type>(
					src, _base.get_allocator(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			bret =
				(!(src && *src && tmp.size() == 0))
				&& (!(_base.size() < tmp.size()))
				&& (utf8_foo::strcmp(
					::yggr::advance_copy(this_type::begin().org(), _base.size() - tmp.size()),
					tmp.data(),
					tmp.size()) == 0);
		}

		return bret;
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const string_charset_helper<T1, T2>& val) const
	{
		return this_type::ends_with(val.first, val.second);
	}

	// contains
	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		contains(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			this_type::find(
				right.data(), size_type(0), right.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME()) != this_type::npos;
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		contains(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::find(right.data(), size_type(0), right.size(), src_charset_name)
			!= this_type::npos;
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		contains(const StringView& right) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::find(
				right.data(), size_type(0), right.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)) != this_type::npos;
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		contains(const StringView& right, const string& src_charset_name) const
	{
		return this_type::find(right.data(), size_type(0), right.size(), src_charset_name) != this_type::npos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			bool
		>::type
		contains(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), size_type(0),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)) != this_type::npos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			bool
		>::type
		contains(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(static_cast<now_char_type>(c), size_type(0), src_charset_name)
			!= this_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find(c.view(), size_type(0)) != this_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const utf8_char_impl<Char, Traits, Alloc>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0)) != this_type::npos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find(c, size_type(0)) != this_type::npos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_view<Char, Traits>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, size_type(0)) != this_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0)) != this_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0)) != this_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0)) != this_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0)) != this_type::npos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		contains(const Char* src) const
	{
		return this_type::find(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) != this_type::npos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		contains(const Char* src, const string& src_charset_name) const
	{
		return this_type::find(src, size_type(0), src_charset_name) != this_type::npos;
	}

	// find string_charest_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const string_charset_helper<T1, T2>& val) const
	{
		return this_type::contains(val.first, val.second);
	}

	// org_str
	inline base_type& org_str(void)
	{
		return _base;
	}

	inline base_type& org_str(const allocator_type&)
	{
		return _base;
	}

	inline const base_type& org_str(void) const
	{
		return _base;
	}

	inline const base_type& org_str(const allocator_type&) const
	{
		return _base;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_type>,
			base_type&
		>::type
		org_str(void)
	{
		return _base;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, base_type>,
			String&
		>::type
		org_str(void)
	{
		return reinterpret_cast<String&>(_base);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_type>,
			base_type&
		>::type
		org_str(const allocator_type&)
	{
		return _base;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, base_type>,
			String&
		>::type
		org_str(const allocator_type&)
	{
		return reinterpret_cast<String&>(_base);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_type>,
			const base_type&
		>::type
		org_str(void) const
	{
		return _base;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, base_type>,
			const String&
		>::type
		org_str(void) const
	{
		return reinterpret_cast<const String&>(_base);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_type>,
			const base_type&
		>::type
		org_str(const allocator_type&) const
	{
		return _base;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, base_type>,
			const base_type&
		>::type
		org_str(const allocator_type&) const
	{
		return reinterpret_cast<const String&>(_base);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_basic_string_t<String>,
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_type>
				>
			>,
			String
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		size_type byte_size = _base.size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		if(byte_size) memcpy(&str[0], _base.data(), byte_size);
		return str;
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_basic_string_t<String>,
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_type>
				>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		size_type byte_size = _base.size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		if(byte_size) memcpy(&str[0], _base.data(), byte_size);
		return str;
	}

	// str
public:
	inline this_type& str(void)
	{
		return *this;
	}

	inline this_type& str(const allocator_type&)
	{
		return *this;
	}

	inline this_type& str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME())
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	inline this_type& str(const allocator_type&, const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME())
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	inline const this_type& str(void) const
	{
		return *this;
	}

	inline const this_type& str(const allocator_type&) const
	{
		return *this;
	}

	inline const this_type& str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	inline const this_type& str(const allocator_type&, const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const
	{
#if defined(_DEBUG)
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			this_type&
		>::type
		str(void)
	{
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			String&
		>::type
		str(void)
	{
		return reinterpret_cast<String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			this_type&
		>::type
		str(const allocator_type&)
	{
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			String&
		>::type
		str(const allocator_type&)
	{
		return reinterpret_cast<String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			this_type&
		>::type
		str(const string& dst_charset_name)
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			String&
		>::type
		str(const string& dst_charset_name)
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return reinterpret_cast<String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			this_type&
		>::type
		str(const allocator_type&, const string& dst_charset_name)
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			String&
		>::type
		str(const allocator_type&, const string& dst_charset_name)
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return reinterpret_cast<String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			const this_type&
		>::type
		str(void) const
	{
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			const String&
		>::type
		str(void) const
	{
		return reinterpret_cast<const String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			const this_type&
		>::type
		str(const allocator_type&) const
	{
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			const String&
		>::type
		str(const allocator_type&) const
	{
		return reinterpret_cast<const String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			const this_type&
		>::type
		str(const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			const String&
		>::type
		str(const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return reinterpret_cast<const String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, this_type>,
			const this_type&
		>::type
		str(const allocator_type&, const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return *this;
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string<String, this_type>,
			const String&
		>::type
		str(const allocator_type&, const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return reinterpret_cast<const String&>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_utf8_string_t<String>,
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, this_type>
				>
			>,
			String
		>::type
		str(void) const
	{
		return String(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_utf8_string_t<String>,
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, this_type>
				>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc) const
	{
		return String(*this, alloc);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_utf8_string_t<String>,
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, this_type>
				>
			>,
			String
		>::type
		str(const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return String(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_utf8_string_t<String>,
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, this_type>
				>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc, const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return String(*this, alloc);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t<String>,
			String
		>::type
		str(void) const
	{
		typedef String ret_string_type;

		return
			YGGR_STR_INNER_STRING_CHARSET_NAME() == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type)?
				charset_foo::s_xchg<ret_string_type>(_base)
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type));
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		return
			YGGR_STR_INNER_STRING_CHARSET_NAME() == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type)?
				charset_foo::s_xchg<ret_string_type>(_base, alloc)
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base, alloc,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type));
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t<String>,
			String
		>::type
		str(const string& dst_charset_name) const
	{
		typedef String ret_string_type;

		return YGGR_STR_INNER_STRING_CHARSET_NAME() == dst_charset_name?
				charset_foo::s_xchg<ret_string_type>(_base)
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc, const string& dst_charset_name) const
	{
		typedef String ret_string_type;

		return
			YGGR_STR_INNER_STRING_CHARSET_NAME() == dst_charset_name?
				charset_foo::s_xchg<ret_string_type>(_base, alloc)
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base, alloc,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					dst_charset_name);
	}

	// other function

	// affix_length
	//when using org_str() to manipulate utf8_string and utf8_size is known during the operation,
	//	utf8_len can be written to the _utf8_size variable by affix_length()

	inline void affix_length(size_type utf8_len)
	{
		assert(utf8_foo::strlen(_base) == utf8_len);
		_utf8_size = utf8_len;
	}

	inline size_type recount_length(void)
	{
		return (_utf8_size = utf8_foo::strlen(_base));
	}

	inline std::size_t hash(void) const
	{
		boost::hash<base_type> hasher;
		return hasher(_base);
	}

private:
	//append this iterator
	template<typename InputIter> inline
	this_type& append_of_this_iterator(const InputIter& start, const InputIter last)
	{
		_base.append(start.org(), last.org());
		_utf8_size += std::distance(start, last);
		return *this;
	}

	template<typename InputIter>
	this_type& append_of_this_riterator(const InputIter& start, const InputIter& last)
	{
		typedef InputIter r_iter_type;

		_base.reserve(_base.size() + std::distance(last.base().org(), start.base().org()));
		const value_type* ptr = 0;
		for(r_iter_type i = start; i != last; ++i)
		{
			ptr = &(*i);
			_base.append(ptr, ptr + utf8_foo::charlen(ptr));
		}
		_utf8_size += std::distance(start, last);
		return *this;
	}

	this_type& assign_of_utf8_base(BOOST_RV_REF(base_type) src, size_type src_utf8_size, size_type pos, size_type len)
	{
		assert(utf8_foo::is_utf8_string(src));

		if((pos == src_utf8_size) || (!len))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			// using assert replace throw
			assert(pos < src.size());
			assert(len);

			len = (std::min)(len, src_utf8_size - pos);

			// not need check is_this, because base_type is fixed
			//!!!if(this == &src){} else{} !!!

			if((len - pos) == src_utf8_size)
			{
				copy_or_move_or_swap(_base, boost::move(src));
			}
			else
			{
				const_iterator start(src.begin());
				start += pos;
				const_iterator last = start + len;

				basic_string_assign(_base, start.org(), last.org());
			}

			_utf8_size = len;
		}

		return *this;
	}

	this_type& assign_of_utf8_base(const base_type& src, size_type src_utf8_size, size_type pos, size_type len)
	{
		assert(utf8_foo::is_utf8_string(src));

		if((pos == src_utf8_size) || (!len))
		{
			_base.clear();
			_utf8_size = 0;
		}
		else
		{
			// using assert replace throw
			assert(pos < src_utf8_size);
			assert(len);

			len = (std::min)(len, src_utf8_size - pos);

			// not need check is_this, because base_type is fixed
			// !!!if(this == &src){} else{}!!!

			const_iterator start(src.begin());
			start += pos;
			const_iterator last = start + len;

			basic_string_assign(_base, start.org(), last.org());
			_utf8_size = len;
		}

		return *this;
	}

	// assign this iterator
	template<typename InputIter> inline
	this_type& assign_of_this_iterator(InputIter start, InputIter last)
	{
		size_type rst_size = std::distance(start, last);
		basic_string_assign(_base, start.org(), last.org());
		_utf8_size = rst_size;
		return *this;
	}

	template<typename InputIter>
	this_type& assign_of_this_riterator(InputIter start, InputIter last)
	{
		typedef InputIter r_iter_type;

		_base.clear();
		_base.reserve(std::distance(last.base().org(), start.base().org()));
		const value_type* ptr = 0;
		size_type rst_size = std::distance(start, last);

		for(r_iter_type i = start; i != last; ++i)
		{
			ptr = &(*i);
			_base.append(ptr, ptr + utf8_foo::charlen(ptr));
		}
		_utf8_size = rst_size;
		return *this;
	}

	// insert this iterator
	template<typename InputIter> inline
	iterator insert_of_this_iterator(param_iterator_type p, InputIter start, InputIter last)
	{
		assert(( static_cast<size_type>(std::distance(param_iterator_type(this_type::begin()), p)) <= _utf8_size));

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());

		basic_string_insert(_base, p.org(), start.org(), last.org());
		_utf8_size += std::distance(start, last);
		return iterator(*this, _base.begin() + old_pos);
	}

	template<typename InputIter>
	iterator insert_of_this_riterator(param_iterator_type p, InputIter start, InputIter last)
	{
		typedef InputIter r_iter_type;
		typedef typename iterator_ex::unwrap_reverse_iterator<r_iter_type>::type iter_type;
		typedef typename iter_type::org_type org_iter_type;

		assert(( static_cast<size_type>(std::distance(param_iterator_type(this_type::begin()), p)) <= _utf8_size));

		size_type old_pos = std::distance<param_base_iterator_type>(_base.begin(), p.org());

		base_type tmp(_base.get_allocator());
		tmp.reserve(std::distance(last.base().org(), start.base().org()));

		const value_type* ptr = 0;
		for(r_iter_type i = start; i != last; ++i)
		{
			ptr = &(*i);
			tmp.append(ptr, ptr + utf8_foo::charlen(ptr));
		}

		if(_utf8_size)
		{
			basic_string_insert(_base, p.org(), tmp.begin(), tmp.end());
		}
		else
		{
			_base.swap(tmp);
		}
		_utf8_size += std::distance(start, last);
		return iterator(*this, _base.begin() + old_pos);
	}

	//replace this iterator
	template<typename InputIter> inline
	this_type& replace_of_this_iterator(param_iterator_type i1, param_iterator_type i2,
											const InputIter& start, const InputIter& last)
	{
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = std::distance(start, last);

		basic_string_replace(_base, i1.org(), i2.org(), start.org(), last.org());
		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

	template<typename InputIter>
	this_type& replace_of_this_riterator(param_iterator_type i1, param_iterator_type i2,
											const InputIter& start, const InputIter& last)
	{
		typedef InputIter r_iter_type;

		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i1)) <= _utf8_size);
		assert(static_cast<size_type>(std::distance<param_iterator_type>(this_type::begin(), i2)) <= _utf8_size);
		assert(i1 <= i2);

		size_type len = distance(i1, i2);
		size_type add_size = std::distance(start, last);

		base_type tmp(_base.get_allocator());
		tmp.reserve(std::distance(last.base().org(), start.base().org()));

		const value_type* ptr = 0;
		for(r_iter_type i = start; i != last; ++i)
		{
			ptr = &(*i);
			tmp.append(ptr, utf8_foo::charlen(ptr));
		}

		basic_string_replace(_base, i1.org(), i2.org(), tmp);
		_utf8_size = _utf8_size - len + add_size;
		return *this;
	}

private:
	base_type _base;
	size_type _utf8_size;
};

} // namespace charset
} // namespace yggr

YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::utf8_string_impl, 3)

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace charset
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename Char, typename Traits, typename Alloc> inline
std::size_t hash_value(const yggr::charset::utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace charset
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename Char, typename Traits, typename Alloc>
struct hash< yggr::charset::utf8_string_impl<Char, Traits, Alloc> >
{
	typedef yggr::charset::utf8_string_impl<Char, Traits, Alloc> type;

	inline std::size_t operator()(const type& ch) const
	{
		return ch.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

namespace yggr
{
namespace charset
{

// io
template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				BOOST_RV_REF_BEG utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> BOOST_RV_REF_END right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
std::basic_istream<Char, Traits>&
	operator>>(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& right)
{
	::yggr::charset::basic_string<Char> tmp;

	is >> tmp;
	right.assign(tmp, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return is;
}

// getline
template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
std::basic_istream<Char, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst, Char dim)
{
	typedef std::basic_istream<Char, Traits> stream_type;

	::yggr::charset::basic_string<Char> tmp;
	::yggr::charset::getline(static_cast<stream_type&>(is), tmp, dim);
	dst.assign(tmp, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return is;
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
std::basic_istream<Char, Traits>&
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst, Char dim)
{
	::yggr::charset::basic_string<Char> tmp;
	::yggr::charset::getline(is, tmp, dim);
	dst.assign(tmp, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return is;
}

// general getline function

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
std::basic_istream<Char, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst,
				const ::yggr::charset::basic_string<Char>& dim)
{
	typedef std::basic_istream<Char, Traits> stream_type;

	stream_type& is_ref = is;
	::yggr::charset::getline(is_ref, dst, dim);
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
std::basic_istream<Char, Traits>&
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst,
				const ::yggr::charset::basic_string<Char>& dim)
{
	typedef std::basic_istream<Char, Traits> stream_type;
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> now_string_type;
	typedef typename now_string_type::base_type now_base_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename stream_type::sentry sentry_type;
	typedef ::yggr::charset::basic_string<Char> dim_string_type;
	typedef dim_string_type input_string_type;

	assert((
		utf8_char_impl<Char_utf8, Traits_utf8, Alloc_utf8>(
			dim, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(dim_string_type)).length()));

	size_type dim_size = dim.size();
	assert(dim_size);

	if(dim_size == 1)
	{
		return ::yggr::charset::getline(is, dst, dim[0]);
	}
	else
	{
		now_size_type nread = 0;
		sentry_type sentry(is, true);

		if(sentry)
		{
			input_string_type s;
			std::basic_streambuf<Char, Traits>* buf = is.rdbuf();
			dst.clear();

			const Char* cmp = dim.data();
			size_type dim_byte_size = dim_size * sizeof(Char);

			while(nread < s.max_size())
			{
				int c1 = buf->sbumpc();
				if(Traits::eq_int_type(c1, Traits::eof()))
				{
					is.setstate(std::ios_base::eofbit);
					break;
				}
				else
				{
					++nread;
					Char c = Traits::to_char_type(c1);

					s.push_back(c);

					if((s.size() < dim.size())
						|| (0 != memcmp(s.data() + (s.size() - dim_size), cmp, dim_byte_size)))
					{
						continue;
					}
					else
					{
						s.erase(s.size() - dim_size);
						break;              // Character is extracted but not appended.
					}
				}
			}

			if(nread == 0 || nread >= s.max_size())
			{
				is.setstate(std::ios_base::failbit);
			}

			dst.assign(s, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(input_string_type));
		}

		return is;
	}
}

template<typename Char, typename Traits, typename Dim_Char,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Dim_Char>,
		std::basic_istream<Char, Traits>&
	>::type
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst, Dim_Char ndim)
{
	typedef typename
		boost::mpl::if_
		<
			::yggr::is_enum<Dim_Char>,
			s32,
			Dim_Char
		>::type now_dim_char_type;

	typedef std::basic_istream<Char, Traits> stream_type;
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	now_dim_char_type dim[2] = {static_cast<now_dim_char_type>(ndim), 0};
	stream_type& is_ref = is;
	return
		::yggr::charset::getline(is_ref, dst,
			charset_foo::s_xchg<dim_string_type>(
				dim,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Dim_Char),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)));
}

template<typename Char, typename Traits, typename Dim_Char,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Dim_Char>,
		std::basic_istream<Char, Traits>&
	>::type
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst, Dim_Char ndim)
{
	typedef typename
		boost::mpl::if_
		<
			::yggr::is_enum<Dim_Char>,
			s32,
			Dim_Char
		>::type now_dim_char_type;

	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	now_dim_char_type dim[2] = {static_cast<now_dim_char_type>(ndim), 0};

	return
		::yggr::charset::getline(is, dst,
			charset_foo::s_xchg<dim_string_type>(
				dim,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Dim_Char),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)));
}

template<typename Char, typename Traits, typename Dim_Char,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_convertible_char<Dim_Char>,
		std::basic_istream<Char, Traits>&
	>::type
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst, const Dim_Char* ndim)
{
	typedef std::basic_istream<Char, Traits> stream_type;
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	stream_type& is_ref = is;
	return
		::yggr::charset::getline(is_ref, dst,
			charset_foo::s_xchg<dim_string_type>(
				ndim,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Dim_Char),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)));
}

template<typename Char, typename Traits, typename Dim_Char,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_convertible_char<Dim_Char>,
		std::basic_istream<Char, Traits>&
	>::type
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst, const Dim_Char* ndim)
{
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	return
		::yggr::charset::getline(is, dst,
			charset_foo::s_xchg<dim_string_type>(
				ndim,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Dim_Char),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)));
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			typename Dim_Char, typename Dim_Traits, typename Dim_Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Dim_Char, Dim_Traits, Dim_Alloc> >,
		std::basic_istream<Char, Traits>&
	>::type
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst,
				const Basic_String<Dim_Char, Dim_Traits, Dim_Alloc>& ndim)
{
	typedef std::basic_istream<Char, Traits> stream_type;
	typedef Basic_String<Dim_Char, Dim_Traits, Dim_Alloc> ndim_string_type;
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	stream_type& is_ref = is;

	return
		::yggr::charset::getline(is_ref, dst,
			charset_foo::s_xchg<dim_string_type>(
				ndim,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ndim_string_type),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(dim_string_type) ));
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			typename Dim_Char, typename Dim_Traits, typename Dim_Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Dim_Char, Dim_Traits, Dim_Alloc> >,
		std::basic_istream<Char, Traits>&
	>::type
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst,
				const Basic_String<Dim_Char, Dim_Traits, Dim_Alloc>& ndim)
{
	typedef Basic_String<Dim_Char, Dim_Traits, Dim_Alloc> ndim_string_type;
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	return
		::yggr::charset::getline(is, dst,
			charset_foo::s_xchg<dim_string_type>(
				ndim,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ndim_string_type),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(dim_string_type) ));
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			typename Dim_Char, typename Dim_Traits, typename Dim_Alloc> inline
std::basic_istream<Char, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst,
				const utf8_char_impl<Dim_Char, Dim_Traits, Dim_Alloc>& ndim)
{
	typedef std::basic_istream<Char, Traits> stream_type;
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	stream_type& is_ref = is;
	return ::yggr::charset::getline(is_ref, dst, ndim.template str<dim_string_type>());
}


template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			typename Dim_Char, typename Dim_Traits, typename Dim_Alloc> inline
std::basic_istream<Char, Traits>&
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst,
				const utf8_char_impl<Dim_Char, Dim_Traits, Dim_Alloc>& ndim)
{
	typedef ::yggr::charset::basic_string<Char> dim_string_type;

	return ::yggr::charset::getline(is, dst, ndim.template str<dim_string_type>());
}


// dim is '\n' version
template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
std::basic_istream<Char, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<Char, Traits> BOOST_RV_REF_END is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst)
{
	typedef std::basic_istream<Char, Traits> stream_type;

	stream_type& is_ref = is;
	::yggr::charset::basic_string<Char> tmp;
	::yggr::charset::getline(is_ref, tmp, is.widen('\n'));
	dst.assign(tmp, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return is;
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
std::basic_istream<Char, Traits>&
	getline(std::basic_istream<Char, Traits>& is,
				utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& dst)
{
	::yggr::charset::basic_string<Char> tmp;
	::yggr::charset::getline(is, tmp, is.widen('\n'));
	dst.assign(tmp, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return is;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
//			template<typename _Char, typename _Traits> class Basic_OStream>
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& right);
//
//template<typename Char, typename Traits,
//			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
//			template<typename _Char, typename _Traits> class Basic_IStream>
//Basic_IStream<Char, Traits>&
//	operator>>(Basic_IStream<Char, Traits>& is,
//				yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& right);

// operator +
template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	>::type
	operator+(Char l, const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(r.get_allocator());
	str.reserve(r.size() + utf8_foo::E_char_max_size);
	str.append(1, l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)).append(r);
	return str;
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	>::type
	operator+(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, Char r)
{
	typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + utf8_foo::E_char_max_size);
	str.append(l).append(1, r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	return str;
}

// utf8_char + utf8_string, utf8_string + utf8_char -> utf8_string
template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const utf8_char_impl<Char, Traits, Alloc>& l,
				const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(r.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(1, l).append(r);
	return str;
}

// utf8_string + utf8_char
template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+( const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const utf8_char_impl<Char, Traits, Alloc>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(1, r);
	return str;
}

// utf8_char_view + utf8_string, utf8_string + utf8_char_view -> utf8_string
template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const basic_utf8_char_view<Char, Traits>& l,
				const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(r.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(1, l).append(r);
	return str;
}

// utr8_string + utf8_char_view
template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+( const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const basic_utf8_char_view<Char, Traits>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(1, r);
	return str;
}

// basic_utf8_char_reference + utf8_string, utf8_string + basic_utf8_char_reference -> utf8_string
template<typename Utf8String, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const basic_utf8_char_reference<Utf8String>& l,
				const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(r.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(1, l).append(r);
	return str;
}

// utr8_string + basic_utf8_char_reference

template<typename Utf8String, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+( const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const basic_utf8_char_reference<Utf8String>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(1, r);
	return str;
}

// basic_utf8_char_const_reference + utf8_string, utf8_string + basic_utf8_char_const_reference -> utf8_string
template<typename Utf8String, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const basic_utf8_char_const_reference<Utf8String>& l,
				const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(r.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(1, l).append(r);
	return str;
}

// utr8_string + basic_utf8_char_const_reference
template<typename Utf8String, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+( const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const basic_utf8_char_const_reference<Utf8String>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(1, r);
	return str;
}

// const Char* + utf8_string, utf8_string + const Char* -> utf8_string

// const Char* + utf8_string

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	>::type
	operator+(const Char* l, const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l, r.get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	str.append(r);
	return str;
}

// utf8_string + const char*

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	>::type
	operator+(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, const Char* r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

	string_type str(l);
	str.append(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return str;
}

// basic_string + utf8_string -> basic_string, utf8_string + basic_string -> utf8_string

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		Basic_String<Char, Traits, Alloc>
	>::type
	operator+(const Basic_String<Char, Traits, Alloc>& l,
				const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef Basic_String<Char, Traits, Alloc> string_type;

	return l + r.template str<string_type>(l.get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(string_type));
}

// utf8_string + basic_string

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	>::type
	operator+(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const Basic_String<Char, Traits, Alloc>& r)
{
	typedef utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;
	string_type ret(l);
	ret.append(r.data(), r.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	return ret;
}

// utf8_string + utf8_string -> utf8_string

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R> inline
utf8_string_impl<Char, Traits_L, Alloc_L>
	operator+(const utf8_string_impl<Char, Traits_L, Alloc_L>& l,
				const utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	typedef utf8_string_impl<Char, Traits_L, Alloc_L> string_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(r.data(), r.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	return str;
}

template<typename StringView, typename Char, typename Traits, typename Alloc> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t<StringView>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const StringView& l, const utf8_string_impl<Char, Traits, Alloc>& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	typedef StringView string_view_type;

	string_type str(r.get_allocator());
	str.reserve(l.size() * utf8_foo::E_char_max_size + r.size());
	str.append(yggr::charset::make_string_charset_helper(l)).append(r);
	return str;
}

template<typename Char, typename Traits, typename Alloc, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t<StringView>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const utf8_string_impl<Char, Traits, Alloc>& l, const StringView& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	typedef StringView string_view_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size() * utf8_foo::E_char_max_size);
	str.append(l).append(yggr::charset::make_string_charset_helper(r));
	return str;
}

template<typename StringView, typename Char, typename Traits, typename Alloc> inline
typename
	boost::enable_if
	<
		is_utf8_string_view_t<StringView>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const StringView& l, const utf8_string_impl<Char, Traits, Alloc>& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	typedef StringView string_view_type;

	string_type str(r.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(r);
	return str;
}

template<typename Char, typename Traits, typename Alloc, typename StringView> inline
typename
	boost::enable_if
	<
		is_utf8_string_view_t<StringView>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const utf8_string_impl<Char, Traits, Alloc>& l, const StringView& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	typedef StringView string_view_type;

	string_type str(l.get_allocator());
	str.reserve(l.size() + r.size());
	str.append(l).append(r);
	return str;
}

// string_charset_helper + utf8_string, utf8_string + string_charset_helper -> utf8_string

//string_charset_helper + utf8_string
template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			is_can_be_conv_into_char<T1>,
			is_utf8_char_t< typename native_t<T1>::type >
		>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const string_charset_helper<T1, T2>& l,
				const utf8_string_impl<Char, Traits, Alloc>& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	return string_type(l, r.get_allocator()) + r;
}

template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			is_can_be_conv_into_char<T1>,
			is_utf8_char_t< typename native_t<T1>::type >
		>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const string_charset_helper<T1, T2>& l,
				const utf8_string_impl<Char, Traits, Alloc>& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	return string_type(1, l, r.get_allocator()) + r;
}


// utf8_string + string_charset_helper

template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			is_can_be_conv_into_char<T1>,
			is_utf8_char_t< typename native_t<T1>::type >
		>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const utf8_string_impl<Char, Traits, Alloc>& l,
				const string_charset_helper<T1, T2>& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	string_type str(l);
	str.append(r);
	return str;
}

template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			is_can_be_conv_into_char<T1>,
			is_utf8_char_t< typename native_t<T1>::type >
		>,
		utf8_string_impl<Char, Traits, Alloc>
	>::type
	operator+(const utf8_string_impl<Char, Traits, Alloc>& l,
				const string_charset_helper<T1, T2>& r)
{
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
	string_type str(l);
	str.append(1, r);
	return str;
}

//operator +=
// string +=

#if 0

// conflict mingw gcc boost::container::basic_string operator+=
//template<typename Char, typename Traits, typename Alloc,
//			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >  inline
//typename
//	boost::enable_if
//	<
//		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
//		Basic_String<Char, Traits, Alloc>&
//	>::type
//	operator+=(Basic_String<Char, Traits, Alloc>& l,
//				const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
//{
//	typedef Basic_String<Char, Traits, Alloc> string_type;
//
//	l += r.template str<string_type>(l.get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(string_type));
//	return l;
//}

#else

template<typename BasicString, typename Utf8String>  inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString >,
			is_utf8_string_t<Utf8String>
		>,
		BasicString&
	>::type
	operator+=(BasicString& l, const Utf8String& r)
{
	typedef BasicString string_type;

	l += r.template str<string_type>(l.get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(string_type));
	return l;
}

#endif // 0, 1

// ==. !=
#define YGGR_TMP_PP_UTF8_STRING_CMP1( __op__ ) \
	template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline \
	bool operator __op__(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, \
							const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline \
	bool operator __op__(const Char* l, \
							const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, \
				typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, \
				typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R> inline \
	bool operator __op__(const utf8_string_impl<Char, Traits_L, Alloc_L>& l, \
							const utf8_string_impl<Char, Traits_R, Alloc_R>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename StringView> inline \
	typename boost::enable_if<is_string_view_t<StringView>, bool>::type \
		operator __op__(const utf8_string_impl<Char, Traits, Alloc>& l, const StringView& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename StringView> inline \
	typename boost::enable_if<is_string_view_t<StringView>, bool>::type \
		operator __op__(const StringView& l, const utf8_string_impl<Char, Traits, Alloc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, typename T1, typename T2> inline \
	bool operator __op__(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r) { \
		return r.compare(l) __op__ 0; }

// <, <=, >, >=
#define YGGR_TMP_PP_UTF8_STRING_CMP2( __op__, __rop__ ) \
	template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline \
	bool operator __op__(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, \
										const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8> inline \
	bool operator __op__(const Char* l, \
										const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Traits, typename Alloc, \
				typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, \
										const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, \
				typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
										const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R> inline \
	bool operator __op__(const utf8_string_impl<Char, Traits_L, Alloc_L>& l, \
							const utf8_string_impl<Char, Traits_R, Alloc_R>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename StringView> inline \
	typename boost::enable_if<is_string_view_t<StringView>, bool>::type \
		operator __op__(const utf8_string_impl<Char, Traits, Alloc>& l, const StringView& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename StringView> inline \
	typename boost::enable_if<is_string_view_t<StringView>, bool>::type \
		operator __op__(const StringView& l, const utf8_string_impl<Char, Traits, Alloc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, typename T1, typename T2> inline \
	bool operator __op__(const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_utf8, typename Traits_utf8, typename Alloc_utf8, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r) { \
		return !(r.compare(l) __rop__ 0); }


	YGGR_TMP_PP_UTF8_STRING_CMP1(==)
	YGGR_TMP_PP_UTF8_STRING_CMP1(!=)

	YGGR_TMP_PP_UTF8_STRING_CMP2(<, <=)
	YGGR_TMP_PP_UTF8_STRING_CMP2(<=, <)
	YGGR_TMP_PP_UTF8_STRING_CMP2(>, >=)
	YGGR_TMP_PP_UTF8_STRING_CMP2(>=, >)

#undef YGGR_TMP_PP_UTF8_STRING_CMP1
#undef YGGR_TMP_PP_UTF8_STRING_CMP2

} // namesapce charset
} // namespace yggr

//swap

namespace yggr
{
namespace charset
{
namespace swap_support
{

template<typename CharL, typename TraitsL, typename AllocL,
			typename CharR, typename TraitsR, typename AllocR,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename boost::enable_if< is_basic_string_t< Basic_String<CharR, TraitsR, AllocR> >, void >::type
	swap(utf8_string_impl<CharL, TraitsL, AllocL>& l, Basic_String<CharR, TraitsR, AllocR>& r)
{
	l.swap(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharR));
}

template<typename CharL, typename TraitsL, typename AllocL,
			typename CharR, typename TraitsR, typename AllocR,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename boost::enable_if< yggr::charset::is_basic_string_t< Basic_String<CharR, TraitsR, AllocR> >, void >::type
	swap(Basic_String<CharL, TraitsL, AllocL>& l, utf8_string_impl<CharR, TraitsR, AllocR>& r)
{
	r.swap(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharL));
}

template<typename CharL, typename TraitsL, typename AllocL,
			typename CharR, typename TraitsR, typename AllocR> inline
void swap(utf8_string_impl<CharL, TraitsL, AllocL>& l,
			utf8_string_impl<CharR, TraitsR, AllocR>& r)
{
	l.swap(r);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename boost::enable_if< is_basic_string_t< Basic_String<Char, Traits, Alloc> >, void >::type
	swap(utf8_string_impl<Char, Traits, Alloc>& l, Basic_String<Char, Traits, Alloc>& r)
{
	l.swap(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename boost::enable_if< yggr::charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >, void >::type
	swap(Basic_String<Char, Traits, Alloc>& l, utf8_string_impl<Char, Traits, Alloc>& r)
{
	r.swap(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename Char, typename Traits, typename Alloc> inline
void swap(utf8_string_impl<Char, Traits, Alloc>& l,
			utf8_string_impl<Char, Traits, Alloc>& r)
{
	l.swap(r);
}

template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline
void swap(yggr::charset::utf8_string_impl<Char, Traits, Alloc>& l,
			yggr::charset::string_charset_helper<T1, T2>& r)
{
	l.swap(r.first, r.second);
}

template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline
void swap(string_charset_helper<T1, T2>& l,
			utf8_string_impl<Char, Traits, Alloc>& r)
{
	r.swap(l.first, l.second);
}

} // namespace swap_support

using swap_support::swap;

} // namespace charset
} // namespace yggr

namespace std
{
	using ::yggr::charset::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::charset::swap_support::swap;
} // namespace boost


// is_validate_char, is_validate_string
namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc > inline
bool is_validate_char(const utf8_string_impl<Char, Traits, Alloc>& /*str*/)
{
	return false;
}

template<typename Char, typename Traits, typename Alloc > inline
bool is_validate_string(const utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.is_validate();
}

} // namespace charset
} // namespace yggr


namespace yggr
{
namespace charset
{
namespace detail
{

template<typename Char, typename Traits, typename Alloc>
struct is_inner_string_traits< charset::utf8_string_impl<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
};

template<typename Char, typename Traits, typename Alloc>
struct basic_string_traits<charset::utf8_string_impl<Char, Traits, Alloc>,
							tag_inner_string_traits>
{
	typedef charset::utf8_string_impl<Char, Traits, Alloc> string_type;
	typedef Alloc tpl_arg_allocator_type;
	typedef typename string_type::char_type char_type;
	typedef typename string_type::value_type value_type;
	typedef typename string_type::traits_type traits_type;
	typedef typename string_type::allocator_type allocator_type;
	typedef typename string_type::reference reference;
	typedef typename string_type::const_reference const_reference;
	typedef typename string_type::pointer pointer;
	typedef typename string_type::const_pointer const_pointer;
	typedef typename string_type::iterator iterator;
	typedef typename string_type::const_iterator const_iterator;
	typedef typename string_type::reverse_iterator reverse_iterator;
	typedef typename string_type::const_reverse_iterator const_reverse_iterator;
	typedef typename string_type::difference_type difference_type;
	typedef typename string_type::size_type size_type;

	typedef typename string_type::base_type base_class_string_type;
	typedef string_type class_string_type;

	typedef tag_inner_string_traits is_inner_decl_type;
};

} // namespace detail
} // namespace charset
} // namespace yggr

// string_charset_helper
namespace yggr
{
namespace charset
{

// string_charset_helper_data
template<typename Char, typename Traits, typename Alloc> inline
const utf8_string_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str;
}

template<typename Char, typename Traits, typename Alloc> inline
const utf8_string_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const utf8_string_impl<Char, Traits, Alloc>& str, const string& charset_name)
{
	assert(charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return str;
}

template<typename Char, typename Traits, typename Alloc, typename Charset_Name > inline
const utf8_string_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const string_charset_helper<utf8_string_impl<Char, Traits, Alloc>&, Charset_Name>& val)
{
	assert(YGGR_STR_UTF8_STRING_CHARSET_NAME() == val.second);
	return val.first;
}

template<typename Char, typename Traits, typename Alloc, typename Charset_Name > inline
const utf8_string_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const string_charset_helper<const utf8_string_impl<Char, Traits, Alloc>&, Charset_Name>& val)
{
	assert(YGGR_STR_UTF8_STRING_CHARSET_NAME() == val.second);
	return val.first;
}

} // namespace charset
} // namespace yggr

// boost range_size support
namespace yggr
{
namespace charset
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, utf8_string_impl)

} // namespace charset
} // namespace yggr

//byte_size support
namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename Char, typename Traits, typename Alloc>
struct byte_size_tpl_container_helper< charset::utf8_string_impl<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
public:
	typedef charset::utf8_string_impl<Char, Traits, Alloc> value_type;

public:
	inline u64 operator()(const value_type& val) const
	{
		return val.size();
	}
};

} // namespace detail
} // namespace bytes
} // namespace yggr


// adapter_function
namespace yggr
{
namespace charset
{

// org_str
template<typename Char, typename Traits, typename Alloc> inline
typename utf8_string_impl<Char, Traits, Alloc>::base_type&
	org_str(utf8_string_impl<Char, Traits, Alloc>& src)
{
	return src.org_str();
}

template<typename Char, typename Traits, typename Alloc> inline
const typename utf8_string_impl<Char, Traits, Alloc>::base_type&
	org_str(const utf8_string_impl<Char, Traits, Alloc>& src)
{
	return src.org_str();
}

// recount_length
template<typename Char, typename Traits, typename Alloc> inline
typename utf8_string_impl<Char, Traits, Alloc>::size_type
	recount_length(utf8_string_impl<Char, Traits, Alloc>& src)
{
	return src.recount_length();
}

// affix_length
template<typename Char, typename Traits, typename Alloc> inline
void affix_length(utf8_string_impl<Char, Traits, Alloc>& src,
					typename utf8_string_impl<Char, Traits, Alloc>::size_type n)
{
	src.affix_length(n);
}

} // namespace charset
} // namespace yggr

// utf8_string_constructable
namespace yggr
{
namespace charset
{

namespace detail
{

// utf8_string_constructable_helper
template<typename T>
struct utf8_string_constructable_helper
	: public
		boost::mpl::or_
		<
			is_can_be_conv_into_char<T>,
			is_string_t<T>,
			is_string_view_t<T>
		>::type
{
};

template<typename T, std::size_t N>
struct utf8_string_constructable_helper<T[N]>
	: public boost::mpl::true_
{
};

template<typename T>
struct utf8_string_constructable_helper<T*>
	: public is_native_char<typename boost::remove_cv<typename boost::remove_pointer<T>::type>::type>::type
{
};

template<typename T1, typename T2>
struct utf8_string_constructable_helper<string_charset_helper< T1, T2 > >
	: public boost::mpl::true_
{
};

//utf8_string_loadable_helper
template<typename T>
struct utf8_string_loadable_helper
	: public is_string_t<T>
{
};

template<typename T1, typename T2>
struct utf8_string_loadable_helper< string_charset_helper< T1, T2 > >
	: public
		boost::mpl::and_
		<
			boost::mpl::not_< boost::is_const<T1> >,
			utf8_string_loadable_helper< typename boost::remove_reference<T1>::type >
		>::type
{
};

} // namespace detail

template<typename T>
struct utf8_string_constructable
	: public
		detail::utf8_string_constructable_helper
		<
			typename
				boost::remove_cv
				<
					typename boost::remove_reference<T>::type
				>::type
		>::type
{
};

template<typename T>
struct utf8_string_loadable
	: public
		detail::utf8_string_loadable_helper
		<
			typename
				boost::remove_cv
				<
					typename boost::remove_reference<T>::type
				>::type
		>::type
{
};

template<typename Char, typename Traits, typename Alloc>
struct basic_string_getter< utf8_string_impl<Char, Traits, Alloc> >
{
private:
	typedef utf8_string_impl<Char, Traits, Alloc> now_string_type;

public:
	typedef typename now_string_type::base_type type;

public:
	inline type& operator()(now_string_type& src) const
	{
		return src.org_str();
	}

	inline const type& operator()(const now_string_type& src) const
	{
		return src.org_str();
	}
};

} // namespace charset
} // namesapce yggr

#undef YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME

#endif //__YGGR_CHARSET_UTF8_STRING_IMPL_HPP__
