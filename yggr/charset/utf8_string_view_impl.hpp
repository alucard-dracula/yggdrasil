// utf8_string_view_impl.hpp

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

#ifndef __YGGR_CHARSET_UTF8_STRING_VIEW_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_STRING_VIEW_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/strict_sizeof.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/range_ex/range_value_ex.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/string_charset_helper.hpp>
#include <yggr/charset/convertible_traits.hpp>

#include <yggr/charset/utf8_foo.hpp>
#include <yggr/charset/string_view.hpp>

#include <yggr/charset/utf8_string_impl_decl.hpp>
#include <yggr/charset/utf8_string_view_decl.hpp>
#include <yggr/charset/utf8_string_iterator_decl.hpp>

#include <yggr/charset/utf8_char_impl_decl.hpp>
#include <yggr/charset/utf8_char_view_decl.hpp>
#include <yggr/charset/utf8_char_reference_decl.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/typeof/typeof.hpp>

#include <stdexcept>


#ifdef _DEBUG
#	define YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() dst_charset_name
#else
#	define YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()
#endif // _DEBUG


YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::basic_utf8_string_view, 2)

namespace yggr
{
namespace charset
{

template<typename CharT, typename TraitsT>
struct is_utf8_string_view_t< basic_utf8_string_view<CharT, TraitsT> >
	: public boost::mpl::true_
{
};

//template<typename Char, typename Traits>
//struct charset_name_t< basic_utf8_string_view<Char, Traits> >
//	: private nonable::noncopyable,
//		private nonable::nonmoveable
//{
//public:
//	typedef Char char_type;
//	typedef typename boost::remove_cv<char_type>::type native_char_type;
//	typedef utf8_string_impl<Char, Traits> string_type;
//	typedef basic_utf8_string_view<Char, Traits> string_view_type;
//
//public:
//	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
//	{
//		return default_charset_config::get_utf8_string_charset_name();
//	}
//
//	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
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

template<typename Char, typename Traits>
struct charset_name_t_string_view_impl< basic_utf8_string_view<Char, Traits> >
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Char char_type;
	typedef typename boost::remove_cv<char_type>::type native_char_type;
	typedef utf8_string_impl<Char, Traits> string_type;
	typedef basic_utf8_string_view<Char, Traits> string_view_type;

public:
	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_utf8_string_charset_name();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_str_utf8_string_charset_name();
	}
};

} // namespace detail

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename CharT, typename TraitsT>
class basic_utf8_string_view
{
private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<CharT>::value == 1)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<typename TraitsT::char_type>::value == 1)>));

public:
	typedef basic_string_view<CharT, TraitsT> base_type;

public:
	typedef typename base_type::traits_type traits_type;
	typedef typename base_type::value_type value_type;
	//typedef typename base_type::reference reference; // discard base_type::reference
	//typedef typename base_type::const_reference const_reference; // discard base_type::const_reference
	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;
	//typedef typename base_type::iterator iterator; // discard base_type::iterator
	//typedef typename base_type::const_iterator const_iterator; // discard base_type::const_iterator
	//typedef typename base_type::reverse_iterator reverse_iterator; // discard base_type::reverse_iterator
	//typedef typename base_type::const_reverse_iterator const_reverse_iterator; // discard base_type::const_reverse_iterator

	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference_type;

protected:
	typedef typename base_type::iterator base_iterator;
	typedef typename base_type::const_iterator base_const_iterator;

public:
	typedef utf8_char_impl<value_type, traits_type> char_type;
	typedef basic_utf8_char_view<value_type, traits_type> char_view_type;

public:
	typedef utf8_string_const_iterator<char_type, base_const_iterator> iterator;
	typedef iterator const_iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef reverse_iterator const_reverse_iterator;

	typedef basic_utf8_char_reference<basic_utf8_string_view> reference;
	typedef basic_utf8_char_const_reference<basic_utf8_string_view> const_reference;

	typedef mplex::null_type allocator_type;

public:
	YGGR_STATIC_CONSTANT(size_type, npos = base_type::npos);

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef basic_utf8_string_view this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	typedef typename base_type::max_size_t max_size_t;

public:
	// constructor
	basic_utf8_string_view(void) YGGR_NOEXCEPT_OR_NOTHROW
		: _utf8_size(0)
	{
	}

	basic_utf8_string_view(const_pointer pdata)
		: _base(pdata), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	// compatibility linke basic_utf8_string_view(pdata, YGGR_STR_UTF8_STRING_CHARSET_NAME())
	basic_utf8_string_view(const_pointer pdata, const string& src_charset_name)
		: _base(pdata), _utf8_size(0)
	{
		assert(src_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		this_type::pro_init_utf8_size();
	}

	basic_utf8_string_view(const_pointer pdata, size_type n)
		: _base(pdata, n), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	// compatibility linke basic_utf8_string_view(pdata, len, YGGR_STR_UTF8_STRING_CHARSET_NAME())
	basic_utf8_string_view(const_pointer pdata, size_type n, const string& src_charset_name)
		: _base(pdata, n), _utf8_size(0)
	{
		assert(src_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		this_type::pro_init_utf8_size();
	}

	template<typename IterS, typename IterE>
	basic_utf8_string_view(IterS s, IterE e,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										is_iterator<IterS>,
										is_iterator<IterE>
									>,
									sfinae_type
								>::type sfinae = 0)
		: _base(s, e), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	template<typename Char, typename BaseIter>
	basic_utf8_string_view(utf8_string_iterator<Char, BaseIter> s,
							utf8_string_iterator<Char, BaseIter> e)
		: _base(s.org(), e.org()),
			_utf8_size(static_cast<size_type>(std::distance(s, e)))
	{
	}

	template<typename Char, typename BaseIter>
	basic_utf8_string_view(utf8_string_modify_iterator<Char, BaseIter> s,
							utf8_string_modify_iterator<Char, BaseIter> e)
		: _base(s.org(), e.org()),
			_utf8_size(static_cast<size_type>(std::distance(s, e)))
	{
	}

	template<typename Char, typename BaseIter>
	basic_utf8_string_view(utf8_string_const_iterator<Char, BaseIter> s,
							utf8_string_const_iterator<Char, BaseIter> e)
		: _base(s.org(), e.org()),
			_utf8_size(static_cast<size_type>(std::distance(s, e)))
	{
	}

	template<typename Char, typename Traits,
				template<typename _T1, typename _T2> class Basic_String_View>
	basic_utf8_string_view(const Basic_String_View<Char, Traits>& right)
		: _base(reinterpret_cast<const_pointer>(right.data()),
				right.size() * mplex::strict_sizeof<Char>::value),
			_utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	template<typename Char, typename Traits,
				template<typename _T1, typename _T2> class Basic_String_View>
	basic_utf8_string_view(const Basic_String_View<Char, Traits>& right, size_type n)
		: _base(reinterpret_cast<const_pointer>(right.data()),
				n * mplex::strict_sizeof<Char>::value),
			_utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	template<typename Char, typename Traits, typename Alloc>
	basic_utf8_string_view(const utf8_string_impl<Char, Traits, Alloc>& right)
		: _base(right.data(), right.size()), _utf8_size(right.length())
	{
	}

	basic_utf8_string_view(BOOST_RV_REF(base_type) right)
		: _base(boost::move(right)), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	basic_utf8_string_view(BOOST_RV_REF(base_type) right, size_type n)
		: _base(right.data(), n), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	basic_utf8_string_view(const base_type& right)
		: _base(right), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	basic_utf8_string_view(const base_type& right, size_type n)
		: _base(right.data(), n), _utf8_size(0)
	{
		this_type::pro_init_utf8_size();
	}

	basic_utf8_string_view(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(boost::move(right._base)),
			_utf8_size(boost::move(right._utf8_size))
	{
	}

	basic_utf8_string_view(const this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(right._base), _utf8_size(right._utf8_size)
	{
	}

	template<typename Char, typename Traits>
	basic_utf8_string_view(const basic_utf8_string_view<Char, Traits>& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _base(reinterpret_cast<const_pointer>(right.data()), right.size()),
			_utf8_size(right.length())
	{
		this_type::pro_init_utf8_size();
	}

protected:
	basic_utf8_string_view(base_const_iterator s, base_const_iterator e, size_type size8)
		: _base(s, e), _utf8_size(size8)
	{
		assert(_utf8_size == utf8_foo::strlen(s, e));
	}

	basic_utf8_string_view(const_pointer s, size_type sizeb, size_type size8)
		: _base(s, sizeb), _utf8_size(size8)
	{
		assert(_utf8_size == utf8_foo::strlen(s, s + sizeb));
	}

public:

	~basic_utf8_string_view(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

public:
	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	this_type& operator=(const utf8_string_impl<Char, Traits, Alloc>& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type tmp(right.data(), right.size());
		_base = tmp;
		_utf8_size = right.length();

		return *this;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	this_type& operator=(const basic_utf8_string_view<Char, Traits>& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type tmp(reinterpret_cast<const_pointer>(right.data()), right.size());
		_base = tmp;
		_utf8_size = right.length();

		return *this;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		_base = boost::move(right._base);
		_utf8_size = boost::move(right._utf8_size);

		return *this;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE this_type& operator=(const this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;
		_utf8_size = right._utf8_size;

		return *this;
	}

public:
	YGGR_CXX17_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX17_CONSTEXPR void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(_base, right._base);
		yggr::swap(_utf8_size, right._utf8_size);
	}

public:
	// Iterators
	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator begin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_iterator(_base.begin());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator cbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_iterator(_base.cbegin());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator end(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_iterator(_base.end());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator cend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_iterator(_base.cend());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator rbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(this_type::end());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator crbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(this_type::cend());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator rend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(this_type::begin());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator crend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(this_type::cbegin());
	}

public:
	// Element access
	YGGR_CXX17_CONSTEXPR_OR_INLINE char_view_type operator[](size_type idx8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(idx8 < _utf8_size);
		return char_view_type(::yggr::advance_copy(this_type::begin(), idx8));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE char_view_type at(size_type idx8) const
	{
		if(!(idx8 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::at invalid subscript");
		}

		return char_view_type(::yggr::advance_copy(this_type::begin(), idx8));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE char_view_type front(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(!this_type::empty());
		return char_view_type(this_type::begin());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE char_view_type back(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(!this_type::empty());
		return char_view_type(::yggr::advance_copy(this_type::end(), -1));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* data(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.data();
	}

public:
	// Capacity
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type size(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.size();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type length(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _utf8_size;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type max_size(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base.max_size();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool empty(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return (_utf8_size == 0) || _base.empty();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool is_validate(void) const
	{
		return _utf8_size == utf8_foo::strlen(_base.data(), _base.size());
	}

public:
	// Modifiers
	YGGR_CXX17_CONSTEXPR_OR_INLINE void remove_prefix(size_type n8)
	{
		assert(n8 <= _utf8_size);

		if(n8 == _utf8_size)
		{
			_base.remove_prefix(_base.size());
			_utf8_size = 0;
		}
		else
		{
			const_iterator now_iter = ::yggr::advance_copy(this_type::begin(), n8);
			_base.remove_prefix(static_cast<size_type>(std::distance(this_type::begin().org(), now_iter.org)));
			_utf8_size -= n8;
		}
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE void remove_suffix(size_type n8)
	{
		assert(n8 <= _utf8_size);

		if(n8 == _utf8_size)
		{
			_base.remove_suffix(_base.size());
			_utf8_size = 0;
		}
		else
		{
			const_iterator now_iter = ::yggr::advance_copy(this_type::begin(), _utf8_size - n8);
			_base.remove_suffix(static_cast<size_type>(std::distance(now_iter.org(), this_type::end().org())));
			_utf8_size -= n8;
		}
	}

public:
	// Operations

	// the return value represents how many T is copied,
	// if sizeof(T) == 1 then this return value is equivalent to byte_size.
	// when the T size is not 1 byte and "charset_name_t" is "utf8",
	// that is, "sizeof(T) * n != byte_size(utf8_str)" then the missing part will be filled with 0.
	// "len8" and "pos8" is utf8_length
	// other rule same to string::copy

	template<typename T> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len8) const
	{
		return this_type::copy(out, len8, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(T));
	}

	template<typename T> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len8, const string& src_charset_name) const
	{
		return this_type::copy(out, len8, size_type(0), src_charset_name);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type copy(BOOST_RV_REF_BEG string_charset_helper<T1, T2> BOOST_RV_REF_END out,
					size_type len8) const
	{
		return this_type::copy(out.first, len8, out.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type copy(string_charset_helper<T1, T2>& out, size_type len8) const
	{
		return this_type::copy(out.first, len8, out.second);
	}

	template<typename T> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len8, size_type pos8) const
	{
		return this_type::copy(out, len8, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(T));
	}

	template<typename T> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			size_type
		>::type
		copy(T* out, size_type len8, size_type pos8,
				const string& dst_charset_name) const
	{
		assert(out && pos8 <= _utf8_size);

		len8 = (std::min)(_utf8_size - pos8, len8);
		size_type ret_size = 0;

		if(len8)
		{
			const_iterator s = ::yggr::advance_copy(this_type::begin(), pos8);
			const_iterator e = ::yggr::advance_copy(s, len8);

			size_type byte_size = std::distance(s.org(), e.org());

			if(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
			{
				size_type filled_size = byte_size % sizeof(T);
				if(byte_size) memcpy(out, &(*(s.org())), byte_size);
				if(filled_size) memset(out + byte_size, 0, filled_size);

				assert((0 == (byte_size + filled_size) % sizeof(T)));
				ret_size = (byte_size + filled_size) / sizeof(T);
			}
			else
			{
				charset_converter conv(YGGR_STR_INNER_STRING_CHARSET_NAME(), dst_charset_name);

				std::size_t ilen_byte = byte_size;
				std::size_t xchglen = charset_base_foo::xchglen(len8);
				std::size_t olen_byte = xchglen;

				if(conv(&(*(s.org())), ilen_byte, reinterpret_cast<char*>(out), olen_byte))
				{
					std::size_t use_byte = xchglen - olen_byte;
					size_type filled_size = use_byte % sizeof(T);
					memset(out + use_byte, 0, filled_size);

					assert((0 == (use_byte + filled_size) % sizeof(T)));
					ret_size = (use_byte + filled_size) / sizeof(T);
				}
			}
		}

		return ret_size;
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type copy(BOOST_RV_REF_BEG string_charset_helper<T1, T2> BOOST_RV_REF_END out,
				   size_type len8, size_type pos8) const
	{
		return this_type::copy(out.first, len8, pos8, out.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type copy(string_charset_helper<T1, T2>& out, size_type len8, size_type pos8) const
	{
		return this_type::copy(out.first, len8, pos8, out.second);
	}

	// utf8_string_view

	YGGR_CXX17_CONSTEXPR_OR_INLINE
		this_type substr(size_type pos8 = 0, size_type len8 = base_type::npos,
							const string& dst_charset_name = YGGR_STR_INNER_STRING_CHARSET_NAME()) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		if(!(pos8 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::substr invalid subscript");
		}

		const_iterator siter = ::yggr::advance_copy(this_type::begin(), pos8);

		return
			_utf8_size - pos8 < len8?
				this_type(siter.org(), this_type::end().org(), _utf8_size - pos8)
				: this_type(siter.org(), ::yggr::advance_copy(siter, len8).org(), len8);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<boost::is_same<StringView, this_type>, StringView>::type
		substr(size_type pos8 = 0, size_type len8 = base_type::npos,
				const string& dst_charset_name = YGGR_STR_INNER_STRING_CHARSET_NAME()) const
	{
		return this_type::substr(pos8, len8);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_string_view_t<StringView>,
				boost::mpl::not_
				<
					boost::is_same<StringView, this_type>
				>
			>,
			StringView
		>::type
		substr(size_type pos8 = 0, size_type len8 = base_type::npos,
				const string& dst_charset_name = YGGR_STR_INNER_STRING_CHARSET_NAME()) const
	{
		typedef StringView ret_type;

		if(!(pos8 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::substr invalid subscript");
		}

		const_iterator siter = ::yggr::advance_copy(this_type::begin(), pos8);

		return
			_utf8_size - pos8 < len8?
				ret_type(siter.org(), std::distance(siter.org(), this_type::end().org()))
				: ret_type(siter.org(), std::distance(siter.org(), ::yggr::advance_copy(siter, len8).org()));
	}

	template<typename String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_not_string_view_t<String>, String>::type
		substr(size_type pos8 = 0, size_type len8 = base_type::npos,
				const string& dst_charset_name = charset_name_t<String>()) const
	{
		return this_type::substr(pos8, len8).template str<String>(dst_charset_name);
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(const Alloc& alloc) const
	{
		return this_type::substr(0, base_type::npos).template str<String>(alloc, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(const Alloc& alloc, const string& dst_charset_name) const
	{
		return this_type::substr(0, base_type::npos).template str<String>(alloc, dst_charset_name);
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(size_type pos8, const Alloc& alloc) const
	{
		return this_type::substr(pos8, base_type::npos).template str<String>(alloc, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		substr(size_type pos8, size_type len8,
				const Alloc& alloc,
				const string& dst_charset_name = charset_name_t<String>()) const
	{
		return this_type::substr(pos8, len8).template str<String>(alloc, dst_charset_name);
	}

	// compare_eq
	// basic_string_view
	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			bool
		>::type
		compare_eq(const BasicString<C, T, A>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			bool
		>::type
		compare_eq(const BasicString<C, T, A>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right, src_charset_name) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8, const BasicString<C, T, A>& right) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8,
					const BasicString<C, T, A>& right,
					const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, src_charset_name) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8,
					const BasicString<C, T, A>& right,
					size_type pos, size_type n) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, pos, n) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8,
					const BasicString<C, T, A>& right,
					size_type pos, size_type n,
					const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, pos, n, src_charset_name) == 0;
	}

	// is_utf8_string
	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			bool
		>::type
		compare_eq(const Utf8String<C, T, A>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right, YGGR_STR_INNER_STRING_CHARSET_NAME()) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			bool
		>::type
		compare_eq(const Utf8String<C, T, A>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right, src_charset_name) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8, const Utf8String<C, T, A>& right) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, YGGR_STR_INNER_STRING_CHARSET_NAME()) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8,
					const Utf8String<C, T, A>& right,
					const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, src_charset_name) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos81, size_type n81,
					const Utf8String<C, T, A>& right,
					size_type pos82, size_type n82) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos81, n81, right, pos82, n82, YGGR_STR_INNER_STRING_CHARSET_NAME()) == 0;
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			bool
		>::type
		compare_eq(size_type pos81, size_type n81,
					const Utf8String<C, T, A>& right,
					size_type pos82, size_type n82,
					const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos81, n81, right, pos82, n82, src_charset_name) == 0;
	}

	//basic_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			bool
		>::type
		compare_eq(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right) == 0;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			bool
		>::type
		compare_eq(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right, src_charset_name) == 0;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8, const StringView& right) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right) == 0;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8, const StringView& right, const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, src_charset_name) == 0;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8, const StringView& right,
					size_type pos, size_type n) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, pos, n) == 0;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			bool
		>::type
		compare_eq(size_type pos8, size_type n8, const StringView& right,
					size_type pos, size_type n, const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos8, n8, right, pos, n, src_charset_name);
	}

	// utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return 0 == utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return 0 == utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81, const basic_utf8_string_view<Char, Traits>& right) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare_eq(right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const basic_utf8_string_view<Char, Traits>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare_eq(pos81, n81, right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const basic_utf8_string_view<Char, Traits>& right,
					size_type pos82, size_type n82) const
	{
		if(!(pos81 < _utf8_size && pos82 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos82, n82));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const basic_utf8_string_view<Char, Traits>& right,
					size_type pos82, size_type n82, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare_eq(pos81, n81, right, pos82, n82);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			(this == boost::addressof(right))
			|| (_utf8_size == right.length() && _base.size() == right.size()
					&& (_base.data() == right.data()
						|| 0 == memcmp(_base.data(), right.data(), _base.size())));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(const this_type& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare_eq(right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81, const this_type& right) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare_eq(right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const this_type& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare_eq(pos81, n81, right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const this_type& right,
					size_type pos82, size_type n82) const
	{
		if(!(pos81 < _utf8_size && pos82 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos82, n82));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const this_type& right,
					size_type pos82, size_type n82, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::compare_eq(pos81, n81, right, pos82, n82);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		compare_eq(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return 0 == utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		compare_eq(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return 0 == utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81, const StringView& right) const
	{
		return 0 == this_type::compare(pos81, n81, right);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81,
					const StringView& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return 0 == this_type::compare(pos81, n81, right);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81,
					const StringView& right,
					size_type pos82, size_type n82) const
	{
		return 0 == this_type::compare(pos81, n81, right, pos82, n82);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81,
					const StringView& right,
					size_type pos82, size_type n82,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos81, n81, right, pos82, n82, src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		compare_eq(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return 0 == this_type::compare(right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		compare_eq(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return 0 == this_type::compare(right, src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81, const StringView& right) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return 0 == this_type::compare(pos81, n81, right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81,
				const StringView& right,
				const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos81, n81, right, src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81,
					const StringView& right,
					size_type pos2, size_type n2) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return 0 == this_type::compare(pos81, n81, right, pos2, n2, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		compare_eq(size_type pos81, size_type n81,
					const StringView& right,
					size_type pos2, size_type n2,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos81, n81, right, pos2, n2, src_charset_name);
	}

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos81, size_type len81, const Char* src) const
	{
		return 0 == this_type::compare(pos81, len81, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos81, size_type len81,
					const Char* src,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos81, len81, src, src_charset_name);
	}

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos81, size_type len81,
					const Char* src, size_type n) const
	{
		return 0 == this_type::compare(pos81, len81, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		compare_eq(size_type pos81, size_type len81,
					const Char* src, size_type n,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos81, len81, src, n, src_charset_name);
	}

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare_eq(size_type pos8, size_type len8,
					const Char* src, size_type subpos, size_type sublen) const
	{
		return 0 == this_type::compare(pos8, len8, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare_eq(size_type pos8, size_type len8,
					const Char* src, size_type subpos, size_type sublen,
					const string& src_charset_name) const
	{
		return 0 == this_type::compare(pos8, len8, src, subpos, sublen, src_charset_name);
	}

	template<typename T1, typename T2> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(const string_charset_helper<T1, T2>& right) const
	{
		return this_type::compare_eq(right.first, right.second);
	}

	template<typename T1, typename T2> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const string_charset_helper<T1, T2>& right) const
	{
		return this_type::compare_eq(pos81, n81, right.first, right.second);
	}

	template<typename T1, typename T2> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const string_charset_helper<T1, T2>& right,
					size_type n) const
	{
		return this_type::compare_eq(pos81, n81, right.first, n, right.second);
	}

	template<typename T1, typename T2> YGGR_CXX17_CONSTEXPR_OR_INLINE
	bool compare_eq(size_type pos81, size_type n81,
					const string_charset_helper<T1, T2>& right,
					size_type pos2, size_type n2) const
	{
		return this_type::compare_eq(pos81, n81, right.first, pos2, n2, right.second);
	}

	// compare
	// basic_string
	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			s32
		>::type
		compare(const BasicString<C, T, A>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			this_type::compare(
				0, _utf8_size,
				right.data(), right.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			s32
		>::type
		compare(const BasicString<C, T, A>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			this_type::compare(
				0, _utf8_size,
				right.data(), right.size(),
				src_charset_name);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			s32
		>::type
		compare(size_type pos8, size_type n8, const BasicString<C, T, A>& right) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return
			this_type::compare(
				pos8, n8,
				right.data(), right.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			s32
		>::type
		compare(size_type pos8, size_type n8,
				const BasicString<C, T, A>& right,
				const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return
			this_type::compare(
				pos8, n8,
				right.data(), right.size(),
				src_charset_name);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			s32
		>::type
		compare(size_type pos8, size_type n8,
				const BasicString<C, T, A>& right,
				size_type pos, size_type n) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		if((pos > right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return
			this_type::compare(
				pos8, n8,
				advance_copy(right.data(), pos), (std::min)(n, right.length() - pos),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< BasicString<C, T, A> >,
			s32
		>::type
		compare(size_type pos8, size_type n8,
				const BasicString<C, T, A>& right,
				size_type pos, size_type n,
				const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		if((pos > right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return
			this_type::compare(
				pos8, n8,
				advance_copy(right.data(), pos), (std::min)(n, right.length() - pos),
				src_charset_name);
	}

	// is_utf8_string
	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			s32
		>::type
		compare(const Utf8String<C, T, A>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			this_type::compare(
				0, _utf8_size,
				right.data(), right.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			s32
		>::type
		compare(const Utf8String<C, T, A>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::compare(
				0, _utf8_size,
				right.data(), right.size(),
				src_charset_name);
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			s32
		>::type
		compare(size_type pos8, size_type n8, const Utf8String<C, T, A>& right) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return
			this_type::compare(
				pos8, n8,
				right.data(), right.size(),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			s32
		>::type
		compare(size_type pos8, size_type n8,
				const Utf8String<C, T, A>& right,
				const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return
			this_type::compare(
				pos8, n8,
				right.data(), right.size(),
				src_charset_name);
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			s32
		>::type
		compare(size_type pos81, size_type n81,
				const Utf8String<C, T, A>& right,
				size_type pos82, size_type n82) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return this_type::compare(pos81, n81, right, pos82, n82, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<template<typename _C, typename _T, typename _A> class Utf8String,
				typename C, typename T, typename A> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t< Utf8String<C, T, A> >,
			s32
		>::type
		compare(size_type pos81, size_type n81,
				const Utf8String<C, T, A>& right,
				size_type pos82, size_type n82,
				const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		typedef Utf8String<C, T, A> now_utf8_string_type;
		typedef typename now_utf8_string_type::const_iterator now_iter_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(pos82 > right.length())
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		now_iter_type s = advance_copy(right.begin(), pos82);
		now_iter_type e = pos82 + n82 < right.length()? advance_copy(s, n82) : right.end();

		return
			this_type::compare(
				pos81, n81,
				&(*(s.org())), distance(s.org(), e.org()),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	//basic_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			s32
		>::type
		compare(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::compare(
				0, _utf8_size,
				right.data(), right.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			s32
		>::type
		compare(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			this_type::compare(
				0, _utf8_size,
				right.data(), right.size(),
				src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			s32
		>::type
		compare(size_type pos8, size_type n8, const StringView& right) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::compare(
				pos8, n8,
				right.data(), right.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			s32
		>::type
		compare(size_type pos8, size_type n8, const StringView& right, const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		return
			this_type::compare(
				pos8, n8,
				right.data(), right.size(),
				src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			s32
		>::type
		compare(size_type pos8, size_type n8, const StringView& right,
					size_type pos, size_type n) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if((pos > right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return
			this_type::compare(
				pos8, n8,
				advance_copy(right.data(), pos), (std::min)(n, right.length() - pos),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t< StringView >,
			s32
		>::type
		compare(size_type pos8, size_type n8, const StringView& right,
					size_type pos, size_type n, const string& src_charset_name) const /*YGGR_NOEXCEPT_OR_NOTHROW*/
	{
		if((pos > right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return
			this_type::compare(
				pos8, n8,
				advance_copy(right.data(), pos), (std::min)(n, right.length() - pos),
				src_charset_name);
	}

	// basic_utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	s32 compare(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	s32 compare(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type n81, const basic_utf8_string_view<Char, Traits>& right) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare(right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type n81,
				const basic_utf8_string_view<Char, Traits>& right,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare(right);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type n81,
				const basic_utf8_string_view<Char, Traits>& right,
				size_type pos82, size_type n82) const
	{
		if(!(pos81 < _utf8_size && pos82 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos82, n82));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type n81,
				const basic_utf8_string_view<Char, Traits>& right,
				size_type pos82, size_type n82, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(!(pos81 < _utf8_size && pos82 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos82, n82));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, s32>::type
		compare(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, s32>::type
		compare(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81, const StringView& right) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare(right);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81,
				const StringView& right,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare(right);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81,
				const StringView& right,
				size_type pos82, size_type n82) const
	{
		if(!(pos81 < _utf8_size && pos82 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos82, n82));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81,
				const StringView& right,
				size_type pos82, size_type n82,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		if(!(pos81 < _utf8_size && pos82 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos82, n82));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, s32>::type
		compare(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::compare(right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, s32>::type
		compare(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(_base.data(), right.data(), _base.size(), right.size())
				: utf8_foo::strcmp(
					_base.data(),
					charset_foo::s_xchg_diff_charset< ::yggr::string>(
						right.data(), right.size(),
						src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					_base.size());
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81, const StringView& right) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return
			this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81))
			.compare(right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81,
				const StringView& right,
				const string& src_charset_name) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, (std::min)(_utf8_size - pos81, n81)).compare(right, src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81,
				const StringView& right,
				size_type pos2, size_type n2) const
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		if(!(pos81 < _utf8_size && pos2 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return
			this_type::substr(pos81, n81)
			.compare(right.substr(pos2, n2), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, s32>::type
		compare(size_type pos81, size_type n81,
				const StringView& right,
				size_type pos2, size_type n2,
				const string& src_charset_name) const
	{
		if(!(pos81 < _utf8_size && pos2 < right.length()))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		return this_type::substr(pos81, n81).compare(right.substr(pos2, n2), src_charset_name);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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
				utf8_foo::strcmp(_base.data(), src, _base.size())
				: utf8_foo::strcmp(
					_base.data(),
					charset_foo::s_xchg_diff_charset< ::yggr::string>(
						src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					_base.size());
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos81, size_type len81, const Char* src) const
	{
		return this_type::compare(pos81, len81, src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos81, size_type len81,
				const Char* src,
				const string& src_charset_name) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		assert(src);

		const_iterator start = ::yggr::advance_copy(this_type::begin(), pos81);
		const_iterator last = ::yggr::advance_copy(start, (std::min)(_utf8_size - pos81, len81));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(
					&(*(start.org())), src,
					std::distance(start.org(), last.org()))
				: utf8_foo::strcmp(
					&(*start.org()),
					charset_foo::s_xchg_diff_charset< ::yggr::string>(
						src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					std::distance(start.org(), last.org()));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos81, size_type len81,
				const Char* src, size_type n) const
	{
		return this_type::compare(pos81, len81, src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos81, size_type len81,
				const Char* src, size_type n,
				const string& src_charset_name) const
	{
		if(!(pos81 < _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		assert(src);

		const_iterator start = ::yggr::advance_copy(this_type::begin(), pos81);
		const_iterator last = ::yggr::advance_copy(start, (std::min)(_utf8_size - pos81, len81));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(
					&(*start.org()), src,
					std::distance(start.org(), last.org()), n * sizeof(Char))
				: utf8_foo::strcmp(
					&(*start.org()),
					charset_foo::s_xchg_diff_charset< ::yggr::string>(
						src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					std::distance(start.org(), last.org()));
	}

	// compare(const string&, subpos, sublen)
	// why is the compare(const Char* src, subpos, sublen) version available,
	// because the third argument to "compare(pos, len, const string&, subpos, sublen)"
	//	in "std::string" uses const "std::string&", so "const char*" is automatically converted to "std::string",
	// but in "utf8_string",
	// because "utf8_string" need to support "const char*", "const wchat_t*"
	//	together with some other data can be expressed as string,
	//	so lost the conversion ability

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos8, size_type len8,
				const Char* src, size_type subpos, size_type sublen) const
	{
		return this_type::compare(pos8, len8, src, subpos, sublen, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(size_type pos81, size_type len81,
				const Char* src, size_type subpos, size_type sublen,
				const string& src_charset_name) const
	{
		if(!(pos81 <= _utf8_size))
		{
			throw std::out_of_range("basic_utf8_string_view::compare invalid subscript");
		}

		const_iterator start(this_type::begin() + pos81);
		const_iterator last(start + (std::min)(_utf8_size - pos81, len81));

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::strcmp(
					&(*(start.org())), src + subpos,
					std::distance(start.org(), last.org()), sublen * sizeof(Char))
				: utf8_foo::strcmp(
					&(*(start.org())),
					charset_foo::s_xchg_diff_charset< ::yggr::string>(
						src + subpos, sublen,
						src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	s32 compare(const string_charset_helper<T1, T2>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right.first, right.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type n81,
				const string_charset_helper<T1, T2>& right) const
	{
		return this_type::compare(pos81, n81, right.first, right.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type n81,
				const string_charset_helper<T1, T2>& right,
				size_type pos2, size_type n2) const
	{
		return this_type::compare(pos81, n81, right.first, pos2, n2, right.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	s32 compare(size_type pos81, size_type len81,
				const string_charset_helper<T1, T2>& right,
				size_type n) const
	{
		return this_type::compare(pos81, len81, right.first, n, right.second);
	}

	// xxx_with
	//starts_with
	template<typename Utf8StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<Utf8StringView>, bool>::type
		starts_with(const Utf8StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			(!(_utf8_size < right.length()))
			&& (utf8_foo::strcmp(_base.data(), right.data(), right.size()) == 0);
	}

	template<typename Utf8StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
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

		::yggr::string tmp;
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				(!(_base.size() < right.size()))
					&& (utf8_foo::strcmp(_base.data(), right.data(), right.size()) == 0)
				: (tmp = charset_foo::s_xchg_diff_charset< ::yggr::string>(
							right.data(), right.size(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					(!(_base.size() < tmp.size()))
						&& (utf8_foo::strcmp(_base.data(), tmp.data(), tmp.size()) == 0));
	}

	// compatibilityable utf8_char utf8_char_view basic_utf8_char_reference
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			bool
		>::type
		starts_with(Char c, const string& src_charset_name = charset_name_t<Char>()) const
	{
		return
			_base.data() && _base.size() && _utf8_size
			&& 0 == this_type::front().compare(c, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const utf8_char_impl<Char, Traits, Alloc>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::starts_with(c.view());
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::starts_with(c.view());
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const basic_utf8_char_view<Char, Traits>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			_base.data() && _base.size() && _utf8_size
			&& 0 == this_type::front().compare(c);
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const basic_utf8_char_view<Char, Traits>& c,
					const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::starts_with(c);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const basic_utf8_char_reference<Utf8String>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::starts_with(c.view());
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const basic_utf8_char_reference<Utf8String>& c,
						const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::starts_with(c.view());
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const basic_utf8_char_const_reference<Utf8String>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::starts_with(c.view());
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool starts_with(const basic_utf8_char_const_reference<Utf8String>& c,
						const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::starts_with(c.view());
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
		::yggr::string tmp;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			return this_type::starts_with(this_type(reinterpret_cast<const_pointer>(src)));
		}
		else
		{
			tmp = charset_foo::s_xchg_diff_charset< ::yggr::string>(
					src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			return
				(!(src && *src && tmp.size() == 0))
				&& (!(_base.size() < tmp.size()))
				&& (utf8_foo::strcmp(_base.data(), tmp.data(), tmp.size()) == 0);
		}
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

		::yggr::string tmp;

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				(!(_base.size() < right.size()))
					&& (utf8_foo::strcmp(
						::yggr::advance_copy(_base.begin(), _base.size() - right.size()),
						right.data(),
						right.size()) == 0)
				: (tmp = charset_foo::s_xchg_diff_charset< ::yggr::string>(
							right.data(), right.size(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
					(!(_base.size() < tmp.size()))
						&& (utf8_foo::strcmp(
								::yggr::advance_copy(_base.begin(), _base.size() - tmp.size()),
								tmp.data(),
								tmp.size()) == 0));
	}

	// compatibilityable utf8_char utf8_char_view basic_utf8_char_reference
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			bool
		>::type
		ends_with(Char c, const string& src_charset_name = charset_name_t<Char>()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			_base.data() && _base.size() && _utf8_size
			&& 0 == this_type::back().compare(c, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const utf8_char_impl<Char, Traits, Alloc>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::ends_with(c.view());
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const utf8_char_impl<Char, Traits, Alloc>& c,
						const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::ends_with(c.view());
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const basic_utf8_char_view<Char, Traits>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			_base.data() && _base.size() && _utf8_size
			&& 0 == this_type::back().compare(c);
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const basic_utf8_char_view<Char, Traits>& c,
					const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::ends_with(c);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const basic_utf8_char_reference<Utf8String>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::ends_with(c.view());
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::ends_with(c.view());
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const basic_utf8_char_const_reference<Utf8String>& c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::ends_with(c.view());
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::ends_with(c.view());
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
		::yggr::string tmp;

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			return this_type::ends_with(this_type(reinterpret_cast<const_pointer>(src)));
		}
		else
		{
			tmp = charset_foo::s_xchg_diff_charset< ::yggr::string>(
					src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			return
				(!(src && *src && tmp.size() == 0))
				&& (!(_base.size() < tmp.size()))
				&& (utf8_foo::strcmp(
					::yggr::advance_copy(this_type::begin().org(), _base.size() - tmp.size()),
					tmp.data(),
					tmp.size()) == 0);
		}
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool ends_with(const string_charset_helper<T1, T2>& val) const
	{
		return this_type::ends_with(val.first, val.second);
	}

	// contains
	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(right, size_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME()) != base_type::npos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_string_view<Char, Traits>& right,
					const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(right, size_type(0), src_charset_name) != base_type::npos;
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		contains(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(right, size_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME()) != base_type::npos;
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, bool>::type
		contains(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(right, size_type(0), src_charset_name) != base_type::npos;
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		contains(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return
			this_type::find(
				right, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type))
			!= base_type::npos;
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, bool>::type
		contains(const StringView& right, const string& src_charset_name)
	{
		return this_type::find(right, size_type(0), src_charset_name) != base_type::npos;
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
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)) != base_type::npos;
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
			!= base_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find(c.view(), size_type(0)) != base_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const utf8_char_impl<Char, Traits, Alloc>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0)) != base_type::npos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find(c, size_type(0)) != base_type::npos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_view<Char, Traits>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, size_type(0)) != base_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0)) != base_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0)) != base_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0)) != base_type::npos;
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0)) != base_type::npos;
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
		return this_type::find(src, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) != base_type::npos;
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
		return this_type::find(src, size_type(0), src_charset_name) != base_type::npos;
	}

	// find string_charest_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	bool contains(const string_charset_helper<T1, T2>& val) const
	{
		return this_type::contains(val.first, val.second);
	}

public:

	//find e-----------------------------------------------------
	//basic_utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(right, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(right, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_string_view<Char, Traits>& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		return
			(_utf8_size < pos8 + right.length())?
				base_type::npos
				: (ret_pos =
					utf8_foo::search(
						_base.data() + std::distance(_base.begin(), (this_type::begin() + pos8).org()),
						_base.data() + _base.size(),
						right.data(),
						right.data() + right.size()),
					ret_pos == base_type::npos? base_type::npos : pos8 + ret_pos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_string_view<Char, Traits>& right,
					size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(right, pos8);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(right, size_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find(const StringView& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		return
			(_utf8_size < pos8 + right.length())?
				base_type::npos
				: (ret_pos =
					utf8_foo::search(
						_base.data() + std::distance(_base.begin(), (this_type::begin() + pos8).org()),
						_base.data() + _base.size(),
						right.data(),
						right.data() + right.size()),
					ret_pos == base_type::npos? base_type::npos : pos8 + ret_pos);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find(const StringView& right, size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(right, pos8);
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find(right, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find(const StringView& right, const string& src_charset_name)
	{
		return this_type::find(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find(const StringView& right, size_type pos8)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find(right, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find(const StringView& right, size_type pos8, const string& src_charset_name)
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				this_type::find(this_type(right.data(), right.size()), pos8)
				: this_type::find(right.data(), pos8, right.size(), src_charset_name);
	}

	// !!! why not using YGGR_NOEXCEPT_OR_NOTHROW, because using utf8_char construct !!!
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
		find(Char c, size_type pos8) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), pos8,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find(Char c, size_type pos8, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		value_type cmp = 0;
		size_type dpos = base_type::npos;

		return
			(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))?
				(!(pos8 < _utf8_size))?
					base_type::npos
					: (cmp = static_cast<value_type>(c),
						 dpos = utf8_foo::find(
									::yggr::advance_copy(this_type::begin(), pos8).org(),
									_base.end(), &cmp),
						 dpos == base_type::npos? base_type::npos : pos8 + dpos)
				: this_type::find(char_type(static_cast<now_char_type>(c), src_charset_name), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8) const
	{
		return this_type::find(c.view(), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c, size_type pos8) const
	{
		size_type dpos = base_type::npos;

		return
			(!(pos8 < _utf8_size))?
				base_type::npos
				: (dpos =
					utf8_foo::find(
						::yggr::advance_copy(this_type::begin(), pos8).org(),
						_base.end(), c.data()),
					dpos == base_type::npos? base_type::npos : pos8 + dpos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_view<Char, Traits>& c, size_type pos8,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_reference<Utf8String>& c, size_type pos8,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos8);
	}

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
		find(const Char* src, size_type pos8) const
	{
		return this_type::find(src, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos8, const string& src_charset_name) const
	{
		assert(src);
		size_type base_pos = 0;
		size_type ret_pos = base_type::npos;
		::yggr::string tmp;

		return
			(!(pos8 < _utf8_size))?
				base_type::npos
				: (base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org()),
					((src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())?
						(assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src))),
							ret_pos =
								utf8_foo::search(
									_base.data() + base_pos,
									_base.size() - base_pos,
									src,
									charset_base_foo::strlen(src) * mplex::strict_sizeof<Char>::value))
						: (charset_foo::s_xchg(tmp, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
							ret_pos =
								utf8_foo::search(
									_base.data() + base_pos,
									_base.data() + _base.size(),
									tmp)) ),
						(ret_pos == base_type::npos? base_type::npos : pos8 + ret_pos));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos8, size_type n) const
	{
		return this_type::find(src, pos8, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find(const Char* src, size_type pos8, size_type n, const string& src_charset_name) const
	{
		assert(src);
		size_type base_pos = 0;
		size_type ret_pos = base_type::npos;
		::yggr::string tmp;

		return
			(!(pos8 < _utf8_size))?
				base_type::npos
				: (base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org()),
					((src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())?
						(assert(utf8_foo::is_utf8_string(reinterpret_cast<const value_type*>(src))),
							ret_pos =
								utf8_foo::search(
									_base.data() + base_pos,
									_base.size() - base_pos,
									src,
									n * mplex::strict_sizeof<Char>::value))
						: (charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()),
							ret_pos =
								utf8_foo::search(
									_base.data() + base_pos,
									_base.data() + _base.size(),
									tmp)) ),
						(ret_pos == base_type::npos? base_type::npos : pos8 + ret_pos));
	}

	// find string_charest_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const string_charset_helper<T1, T2>& val, size_type pos8 = 0) const
	{
		return this_type::find(val.first, pos8, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find(const string_charset_helper<T1, T2>& val, size_type pos8, size_type n) const
	{
		return this_type::find(val.first, pos8, n, val.second);
	}

	//find e-----------------------------------------------------

	//rfind s----------------------------------------------------

	//basic_utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(right, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(right, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR
	size_type rfind(const basic_utf8_string_view<Char, Traits>& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			cmp_size = right.size();
			cmp = right.data();

			if(0 == cmp_size)
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos8 < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos8 = _utf8_size - 1;
				}

				const_pointer i = _base.data() + base_pos;
				const_pointer isize = _base.data() - 1;

				// align
				for(;i != isize && std::distance(i, _base.end()) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos8;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == memcmp(i, cmp, cmp_size)) break;
						else --pos8;
					}
				}

				ret_pos = i == isize? base_type::npos : pos8;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_string_view<Char, Traits>& right,
					size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(right, pos8);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(right, base_type::npos, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(right, base_type::npos, src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			cmp_size = right.size() * mplex::strict_sizeof<now_char_type>::value;
			cmp = right.data();

			if(0 == cmp_size)
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos8 < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos8 = _utf8_size - 1;
				}

				const_pointer i = _base.data() + base_pos;
				const_pointer isize = _base.data() - 1;

				// align
				for(;i != isize && std::distance(i, _base.end()) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos8;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == memcmp(i, cmp, cmp_size)) break;
						else --pos8;
					}
				}

				ret_pos = i == isize? base_type::npos : pos8;
			}
		}
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right, size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(right, pos8);
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::rfind(right, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right, const string& src_charset_name)
	{
		return this_type::rfind(right, base_type::npos, src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right, size_type pos8)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::rfind(right, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		rfind(const StringView& right, size_type pos8, const string& src_charset_name)
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				this_type::rfind(this_type(right.data(), right.size()), pos8)
				: this_type::rfind(right.data(), pos8, right.size(), src_charset_name);
	}

	// rfind
	// rfind(char, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		rfind(Char c, size_type pos8) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::rfind(
				static_cast<now_char_type>(c), pos8,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		rfind(Char c, size_type pos8, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		size_type ret_pos = base_type::npos;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(_utf8_size)
			{
				value_type cmp = static_cast<value_type>(c);
				size_type base_pos = base_type::npos;

				if(pos8 < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos8 = _utf8_size - 1;
				}

				const_pointer i = _base.data() + base_pos;
				const_pointer isize = _base.data() - 1;

				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(*i == cmp) break;
						else --pos8;
					}
				}

				ret_pos = i == isize? base_type::npos : pos8;
			}
		}
		else
		{
			ret_pos = this_type::rfind(char_type(static_cast<now_char_type>(c), src_charset_name), pos8);
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8) const
	{
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8,
						const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c, size_type pos8) const
	{
		assert(c.size());

		size_type ret_pos = base_type::npos;

		if(_utf8_size && (!(_base.size() < c.size())))
		{
			size_type base_pos = base_type::npos;

			if(pos8 < _utf8_size)
			{
				base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org());
			}
			else
			{
				base_pos = _base.size() - 1;
				pos8 = _utf8_size - 1;
			}

			const_pointer i = _base.data() + base_pos;
			const_pointer isize = _base.data() - 1;
			const value_type* cmp = c.data();
			size_type cmp_size = c.size();

			// align
			for(;i != isize && std::distance(i, _base.end()) < cmp_size; --i)
			{
				if((*i & 0xc0) != 0x80)
				{
					--pos8;
				}
			}

			// cmp
			for(; i != isize; --i)
			{
				if((*i & 0xc0) != 0x80)
				{
					if(0 == memcmp(i, cmp, cmp_size)) break;
					else --pos8;
				}
			}

			ret_pos = i == isize? base_type::npos : pos8;
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_view<Char, Traits>& c, size_type pos8,
						const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_reference<Utf8String>& c, size_type pos8,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type rfind(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos8) const
	{
		return this_type::rfind(src, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos8, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			::yggr::string tmp;

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
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos8 < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos8 = _utf8_size - 1;
				}

				const_pointer i = _base.data() + base_pos;
				const_pointer isize = _base.data() - 1;

				// align
				for(;i != isize && std::distance(i, _base.end()) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos8;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == memcmp(i, cmp, cmp_size)) break;
						else --pos8;
					}
				}

				ret_pos = i == isize? base_type::npos : pos8;
			}
		}

		return ret_pos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos8, size_type n) const
	{
		return this_type::rfind(src, pos8, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		rfind(const Char* src, size_type pos8, size_type n, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type cmp_size = 0;
			const value_type* cmp = 0;

			::yggr::string tmp;
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());

			cmp_size = tmp.size();
			cmp = tmp.data();

			if(0 == cmp_size)
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size ;
			}
			else if(_base.size() < cmp_size)
			{
				ret_pos = base_type::npos;
			}
			else
			{
				size_type base_pos = base_type::npos;

				if(pos8 < _utf8_size)
				{
					base_pos = std::distance(_base.begin(), (this_type::begin() + pos8).org());
				}
				else
				{
					base_pos = _base.size() - 1;
					pos8 = _utf8_size - 1;
				}

				const_pointer i = _base.data() + base_pos;
				const_pointer isize = _base.data() - 1;

				// align
				for(;i != isize && std::distance(i, _base.end()) < cmp_size; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						--pos8;
					}
				}

				// cmp
				for(; i != isize; --i)
				{
					if((*i & 0xc0) != 0x80)
					{
						if(0 == memcmp(i, cmp, cmp_size)) break;
						else --pos8;
					}
				}

				ret_pos = i == isize? base_type::npos : pos8;
			}
		}

		return ret_pos;
	}

	// rfind string_charset_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type rfind(const string_charset_helper<T1, T2>& val, size_type pos8 = base_type::npos) const
	{
		return this_type::rfind(val.first, pos8, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type rfind(const string_charset_helper<T1, T2>& val, size_type pos8, size_type n) const
	{
		return this_type::rfind(val.first, pos8, n, val.second);
	}

	//rfind e----------------------------------------------------

	//find_first_of s----------------------------------------------------
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(right, size_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_of(right, size_type(0), src_charset_name);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR
	size_type find_first_of(const basic_utf8_string_view<Char, Traits>& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if((pos8 < _utf8_size && right.length()))
		{
			const value_type* pstart = &(*((this_type::begin() + pos8).org()));
			size_type rlen = right.length();
			for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
			{
				if(utf8_foo::memchr(right.data(), pstart, rlen))
				{
					ret_pos = i;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_string_view<Char, Traits>& right,
								size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert((src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()));
		return this_type::find_first_of(right, pos8);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(right, size_type(0), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_of(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if((pos8 < _utf8_size && right.length()))
		{
			const value_type* pstart = &(*((this_type::begin() + pos8).org()));
			size_type rlen = right.length();
			for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
			{
				if(utf8_foo::memchr(right.data(), pstart, rlen))
				{
					ret_pos = i;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right, size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert((src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()));
		return this_type::find_first_of(right, pos8);
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find(right, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right, const string& src_charset_name)
	{
		return this_type::find(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right, size_type pos8)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find(right, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_of(const StringView& right, size_type pos8, const string& src_charset_name)
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				this_type::find_first_of(this_type(right.data(), right.size()), pos8)
				: this_type::find_first_of(right.data(), pos8, right.size(), src_charset_name);
	}

	// find_first_of
	// find_first_of(char, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_of(Char c, size_type pos8) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find(
				static_cast<now_char_type>(c), pos8,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_of(Char c, size_type pos8, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::find(static_cast<now_char_type>(c), pos8, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8) const
	{
		return this_type::find(c.view(), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8) const
	{
		return this_type::find(c, pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c, pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find(c.view(), pos8);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos8) const
	{
		return this_type::find_first_of(src, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos8, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if((pos8 < _utf8_size))
		{
			::yggr::string tmp;
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
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos8, size_type n) const
	{
		return this_type::find_first_of(src, pos8, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_of(const Char* src, size_type pos8, size_type n, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if((pos8 < _utf8_size && n))
		{
			::yggr::string tmp;
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
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

	// rfind string_charset_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_first_of(const string_charset_helper<T1, T2>& val, size_type pos8 = 0) const
	{
		return this_type::find_first_of(val.first, pos8, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_first_of(const string_charset_helper<T1, T2>& val, size_type pos8, size_type n) const
	{
		return this_type::find_first_of(val.first, pos8, n, val.second);
	}

	//find_first_of e----------------------------------------------------

	//find_last_of s----------------------------------------------------
	//basic_utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(right, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(right, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR
	size_type find_last_of(const basic_utf8_string_view<Char, Traits>& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type rlen = right.length();
		size_type ret_pos = base_type::npos;

		if(_utf8_size && rlen)
		{
			const value_type* pbuf = right.data();
			const_reverse_iterator i;
			if(pos8 < _utf8_size)
			{
				i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
			}
			else
			{
				pos8 = _utf8_size - 1;
				i = this_type::rbegin();
			}

			for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
			{
				if(utf8_foo::memchr(pbuf, &(*i), rlen))
				{
					ret_pos = pos8;
					break;
				}
			}

			//return base_type::npos;
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_string_view<Char, Traits>& right,
							size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(right, pos8);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(right, base_type::npos, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(right, base_type::npos, src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type rlen = right.length();
		size_type ret_pos = base_type::npos;

		if((_utf8_size && rlen))
		{
			const_reverse_iterator i;
			if(pos8 < _utf8_size)
			{
				i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
			}
			else
			{
				pos8 = _utf8_size - 1;
				i = this_type::rbegin();
			}

			for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
			{
				if(utf8_foo::memchr(right.data(), &(*i), rlen))
				{
					ret_pos = pos8;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right, size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_of(right, pos8);
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_of(right, base_type::npos, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right, const string& src_charset_name)
	{
		return this_type::find_last_of(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right, size_type pos8)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_of(right, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_of(const StringView& right, size_type pos8, const string& src_charset_name)
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				this_type::find_last_of(this_type(right.data(), right.size()), pos8)
				: this_type::find_last_of(right.data(), pos8, right.size(), src_charset_name);
	}

	// find_last_of
	// find_last_of(char, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_of(Char c, size_type pos8) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::rfind(
				static_cast<now_char_type>(c), pos8,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_of(Char c, size_type pos8, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;
		return this_type::rfind(static_cast<now_char_type>(c), pos8, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8) const
	{
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8) const
	{
		return this_type::rfind(c, pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c, pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::rfind(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::rfind(c.view(), pos8);
	}

	// const char*

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos8) const
	{
		return this_type::find_last_of(src, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos8, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			::yggr::string tmp;
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
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos8, size_type n) const
	{
		return this_type::find_last_of(src, pos8, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_of(const Char* src, size_type pos8, size_type n, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			::yggr::string tmp;
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const_reverse_iterator i;
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
		}

		return ret_pos;
	}

	// find_last_of string_charset_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_of(const string_charset_helper<T1, T2>& val, size_type pos8 = base_type::npos) const
	{
		return this_type::find_last_of(val.first, pos8, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_of(const string_charset_helper<T1, T2>& val, size_type pos8, size_type n) const
	{
		return this_type::find_last_of(val.first, pos8, n, val.second);
	}

	//find_last_of e----------------------------------------------------

	//find_first_not_of s----------------------------------------------------

	//basic_utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(right, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_string_view<Char, Traits>& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(right, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR
	size_type find_first_not_of(const basic_utf8_string_view<Char, Traits>& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if(pos8 < _utf8_size)
		{
			size_type rlen = right.length();

			if(rlen)
			{
				const value_type* pbuf = right.data();
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(pbuf, pstart, rlen))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos8;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_string_view<Char, Traits>& right,
					size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(right, pos8);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(right, size_type(0));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(right, size_type(0));
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if(pos8 < _utf8_size)
		{

			size_type rlen = right.length();

			if(rlen)
			{
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
				{
					if(!utf8_foo::memchr(right.data(), pstart, rlen))
					{
						ret_pos = i;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos8;
			}
		}

		return ret_pos;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right, size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(right, pos8);
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_first_not_of(right, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right, const string& src_charset_name)
	{
		return this_type::find_first_not_of(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right, size_type pos8)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_first_not_of(right, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_first_not_of(const StringView& right, size_type pos8, const string& src_charset_name)
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				this_type::find_first_not_of(this_type(right.data(), right.size()), pos8)
				: this_type::find_first_not_of(right.data(), pos8, right.size(), src_charset_name);
	}

	// find_first_of
	// find_first_of(char, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_not_of(Char c, size_type pos8) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find_first_not_of(
				static_cast<now_char_type>(c), pos8,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_first_not_of(Char c, size_type pos8, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		size_type ret_pos = base_type::npos;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			if(pos8 < _utf8_size)
			{

				value_type buf[2] = { static_cast<value_type>(c) };

				const value_type* pbuf = &buf[0];
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
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
			ret_pos = this_type::find_first_not_of(char_type(static_cast<now_char_type>(c), src_charset_name), pos8);
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8) const
	{
		return this_type::find_first_not_of(c.view(), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find_first_not_of(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c, size_type(0));
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8) const
	{
		size_type ret_pos = base_type::npos;

		if(pos8 < _utf8_size)
		{

			const value_type* pbuf = c.data();
			const value_type* pstart = &(*((this_type::begin() + pos8).org()));
			for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
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

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c, pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find_first_not_of(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), size_type(0));
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find_first_not_of(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8,
							const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_first_not_of(c.view(), pos8);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos8) const
	{
		return this_type::find_first_not_of(src, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos8, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(pos8 < _utf8_size)
		{
			::yggr::string tmp;
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
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
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
				ret_pos = pos8;
			}
		}

		return ret_pos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos8, size_type n) const
	{
		return this_type::find_first_not_of(src, pos8, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_first_not_of(const Char* src, size_type pos8, size_type n, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(pos8 < _utf8_size)
		{
			::yggr::string tmp;
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const value_type* pstart = &(*((this_type::begin() + pos8).org()));
				for(size_type i = pos8, isize = _utf8_size; i != isize; ++i, pstart += utf8_foo::charlen(pstart))
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
				ret_pos = pos8;
			}
		}

		return ret_pos;
	}

	// find_first_not_of string_charset_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const string_charset_helper<T1, T2>& val, size_type pos8 = 0) const
	{
		return this_type::find_first_not_of(val.first, pos8, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_first_not_of(const string_charset_helper<T1, T2>& val, size_type pos8, size_type n) const
	{
		return this_type::find_first_not_of(val.first, pos8, n, val.second);
	}

	//find_first_not_of e----------------------------------------------------

	//find_last_not_of s----------------------------------------------------

	//basic_utf8_string_view
	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_string_view<Char, Traits>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(right, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_string_view<Char, Traits>& right,
								const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(right, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR
	size_type find_last_not_of(const basic_utf8_string_view<Char, Traits>& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type rlen = right.length();

			if(rlen)
			{
				const value_type* pbuf = right.data();
				const_reverse_iterator i;
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), rlen))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX17_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_string_view<Char, Traits>& right,
								size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(right, pos8);
	}

	// utf8_string_view
	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(right, base_type::npos);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(right, base_type::npos, src_charset_name);
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right, size_type pos8) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			size_type rlen = right.length();

			if(rlen)
			{
				const_reverse_iterator i;
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(!utf8_foo::memchr(right.data(), &(*i), rlen))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_utf8_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right, size_type pos8, const string& src_charset_name) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(right, pos8);
	}

	// basic_string_view
	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_not_of(right, size_type(0), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right, const string& src_charset_name)
	{
		return this_type::find_last_not_of(right, size_type(0), src_charset_name);
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right, size_type pos8)
	{
		typedef StringView now_string_view_type;
		typedef typename range_ex::range_value_ex<now_string_view_type>::type now_char_type;

		return this_type::find_last_not_of(right, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename StringView> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_basic_string_view_t<StringView>, size_type>::type
		find_last_not_of(const StringView& right, size_type pos8, const string& src_charset_name)
	{
		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				this_type::find_last_not_of(this_type(right.data(), right.size()), pos8)
				: this_type::find_last_not_of(right.data(), pos8, right.size(), src_charset_name);
	}

	// find_last_not_of
	// find_last_not_of(char, pos)
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_not_of(Char c, size_type pos8) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::find_last_not_of(
				static_cast<now_char_type>(c), pos8,
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			size_type
		>::type
		find_last_not_of(Char c, size_type pos8, const string& src_charset_name) const
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
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), 1))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
		}
		else
		{
			ret_pos = this_type::find_last_not_of(char_type(static_cast<now_char_type>(c), src_charset_name), pos8);
		}

		return ret_pos;
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c) const
	{
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8) const
	{
		return this_type::find_last_not_of(c.view(), pos8);
	}

	template<typename Char, typename Traits, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), pos8);
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c) const
	{
		return this_type::find_last_not_of(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c, const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c, base_type::npos);
	}

	template<typename Char, typename Traits> YGGR_CXX20_CONSTEXPR
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8) const
	{
		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			const value_type* pbuf = c.data();
			const_reverse_iterator i;
			if(pos8 < _utf8_size)
			{
				i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
			}
			else
			{
				pos8 = _utf8_size - 1;
				i = this_type::rbegin();
			}

			for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
			{
				if(!utf8_foo::memchr(pbuf, &(*i), 1))
				{
					ret_pos = pos8;
					break;
				}
			}
		}

		return ret_pos;
	}

	template<typename Char, typename Traits> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_view<Char, Traits>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c, pos8);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c) const
	{
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find_last_not_of(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_reference<Utf8String>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c) const
	{
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), base_type::npos);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8) const
	{
		return this_type::find_last_not_of(c.view(), pos8);
	}

	template<typename Utf8String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const basic_utf8_char_const_reference<Utf8String>& c, size_type pos8,
								const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::find_last_not_of(c.view(), pos8);
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
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

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos8) const
	{
		return this_type::find_last_not_of(src, pos8, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos8, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			::yggr::string tmp;
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
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos8, size_type n) const
	{
		return this_type::find_last_not_of(src, pos8, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char> YGGR_CXX20_CONSTEXPR
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		find_last_not_of(const Char* src, size_type pos8, size_type n, const string& src_charset_name) const
	{
		assert(src);

		size_type ret_pos = base_type::npos;

		if(_utf8_size)
		{
			::yggr::string tmp;
			charset_foo::s_xchg(tmp, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			size_type src_len = utf8_foo::strlen(tmp);

			if(src_len)
			{
				const value_type* pbuf = tmp.data();
				const_reverse_iterator i;
				if(pos8 < _utf8_size)
				{
					i = const_reverse_iterator(this_type::begin() + (pos8 + 1));
				}
				else
				{
					pos8 = _utf8_size - 1;
					i = this_type::rbegin();
				}

				for(const_reverse_iterator isize = this_type::rend(); i != isize; ++i, --pos8)
				{
					if(!utf8_foo::memchr(pbuf, &(*i), src_len))
					{
						ret_pos = pos8;
						break;
					}
				}
			}
			else
			{
				ret_pos = pos8 < _utf8_size? pos8 : _utf8_size - 1;
			}
		}

		return ret_pos;
	}

	// rfind string_charset_helper
	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const string_charset_helper<T1, T2>& val, size_type pos8 = base_type::npos) const
	{
		return this_type::find_last_not_of(val.first, pos8, val.second);
	}

	template<typename T1, typename T2> YGGR_CXX23_CONSTEXPR_OR_INLINE
	size_type find_last_not_of(const string_charset_helper<T1, T2>& val, size_type pos8, size_type n) const
	{
		return this_type::find_last_not_of(val.first, pos8, n, val.second);
	}

	//find_last_not_of e----------------------------------------------------

public:
	//Other
	YGGR_CXX17_CONSTEXPR_OR_INLINE
	operator base_type&(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	operator const base_type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	base_type& org_str(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	const base_type& org_str(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	inline std::size_t hash(void) const
	{
		return _base.hash();
	}

	// str
	YGGR_CXX17_CONSTEXPR_OR_INLINE
	this_type& str(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	this_type& str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) YGGR_NOEXCEPT_OR_NOTHROW
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	const this_type& str(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	const this_type& str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_same<StringView, this_type>,
			this_type&
		>::type
		str(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_same<StringView, this_type>,
			this_type&
		>::type
		str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) YGGR_NOEXCEPT_OR_NOTHROW
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_same<StringView, this_type>,
			const this_type&
		>::type
		str(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return *this;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_same<StringView, this_type>,
			const this_type&
		>::type
		str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return *this;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<StringView, this_type> >,
				is_utf8_string_view_t<StringView>
			>,
			StringView
		>::type
		str(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return StringView(this_type::begin(), this_type::end());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<StringView, this_type> >,
				is_utf8_string_view_t<StringView>
			>,
			StringView
		>::type
		str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return StringView(this_type::begin(), this_type::end());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			StringView
		>::type
		str(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return StringView(this_type::begin().org(), this_type::end().org());
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_view_t<StringView>,
			StringView
		>::type
		str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const YGGR_NOEXCEPT_OR_NOTHROW
	{
#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return StringView(this_type::begin().org(), this_type::end().org());
	}

	template<typename String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t<String>,
			String
		>::type
		str(void) const
	{
		typedef String ret_string_type;

		return ret_string_type(this_type::begin(), this_type::end());
	}

	template<typename String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_utf8_string_t<String>,
			String
		>::type
		str(const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const
	{
		typedef String ret_string_type;

#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return ret_string_type(this_type::begin(), this_type::end());
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_utf8_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		return ret_string_type(this_type::begin(), this_type::end(), alloc);
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_utf8_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc, const string& YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME()) const
	{
		typedef String ret_string_type;

#if defined(_DEBUG) // clang release check this assert
		assert(YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME() == YGGR_STR_INNER_STRING_CHARSET_NAME());
#endif // _DEBUG
		return ret_string_type(this_type::begin(), this_type::end(), alloc);
	}

	template<typename String> YGGR_CXX23_CONSTEXPR_OR_INLINE
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
			(YGGR_STR_INNER_STRING_CHARSET_NAME() == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type))?
				charset_foo::s_xchg<ret_string_type>(_base.data(), _base.size())
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base.data(), _base.size(),
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type));
	}

	template<typename String> YGGR_CXX23_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_basic_string_t<String>,
			String
		>::type
		str(const string& dst_charset_name) const
	{
		typedef String ret_string_type;

		return
			(YGGR_STR_INNER_STRING_CHARSET_NAME() == dst_charset_name)?
				charset_foo::s_xchg<ret_string_type>(_base.data(), _base.size())
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base.data(), _base.size(),
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					dst_charset_name);
	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
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
			(YGGR_STR_INNER_STRING_CHARSET_NAME() == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type))?
				charset_foo::s_xchg<ret_string_type>(_base.data(), _base.size(), alloc)
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base.data(), _base.size(), alloc,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type));

	}

	template<typename String, typename Alloc> YGGR_CXX23_CONSTEXPR_OR_INLINE
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
			(YGGR_STR_INNER_STRING_CHARSET_NAME() == dst_charset_name)?
				charset_foo::s_xchg<ret_string_type>(_base.data(), _base.size(), alloc)
				: charset_foo::s_xchg_diff_charset<ret_string_type>(
					_base.data(), _base.size(), alloc,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					dst_charset_name);
	}

public:
	// org_view
	YGGR_CXX17_CONSTEXPR_OR_INLINE
	base_type& org_view(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	const base_type& org_view(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	template<typename StringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			boost::is_same<StringView, base_type>,
			base_type&
		>::type
		org_view(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _base;
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			boost::is_same<StringView, base_type>,
			const base_type&
		>::type
		org_view(void) const
	{
		return _base;
	}

	template<typename StringView> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_basic_string_view_t<StringView>,
				boost::mpl::not_< boost::is_same<StringView, base_type> >
			>,
			StringView
		>::type
		org_view(void) const
	{
		typedef StringView ret_string_view_type;
		typedef typename ::yggr::range_ex::range_value_ex<ret_string_view_type>::type ret_value_type;

		size_type byte_size = _base.size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		return ret_string_type(reinterpret_cast<const ret_value_type*>(_base.data()), ret_size);
	}

public:
	// affix_length
	//when using org_str() to manipulate utf8_string and utf8_size is known during the operation,
	//	utf8_len can be written to the _utf8_size variable by affix_length()

	inline void affix_length(size_type utf8_len)
	{
		assert(utf8_foo::strlen(_base) == utf8_len);
		return _utf8_size = utf8_len;
	}

	inline size_type recount_length(void)
	{
		return (_utf8_size = utf8_foo::strlen(_base));
	}

protected:
	inline void pro_clear(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		{
			base_type tmp;
			tmp.swap(_base);
		}

		_utf8_size = 0;
	}

	inline void pro_init_utf8_size(void)
	{
		if(_base.empty())
		{
			_utf8_size = 0;
		}
		else
		{
			_utf8_size = utf8_foo::strlen(_base.data(), _base.size());
			assert(_utf8_size);
			if(!_utf8_size)
			{
				this_type::pro_clear();
			}
		}
	}

protected:
	base_type _base;
	size_type _utf8_size;
};

// non-member foo
// ==
template<typename C, typename T, typename T1, typename T2> inline
bool operator==(const basic_utf8_string_view<C, T>& l, const string_charset_helper<T1, T2>& r)
{
	return l.compare_eq(r);
}

template<typename C, typename T, typename T1, typename T2> inline
bool operator==(const string_charset_helper<T1, T2>& l, const basic_utf8_string_view<C, T>& r)
{
	return r.compare_eq(l);
}

// const char*
template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator==(const Char* l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return r.compare_eq(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator==(const basic_utf8_string_view<C, T>& l, const Char* r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

// basic_string
// fix utf8_string_view, boost::container::basic_string conflict
template<typename C, typename T, typename A> inline
bool operator==(const basic_utf8_string_view<C, T>& l, const boost::container::basic_string<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<typename C, typename T, typename A> inline
bool operator==(const boost::container::basic_string<C, T, A>& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return r.compare_eq(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator==(const basic_utf8_string_view<C, T>& l, const BasicString<C2, T2, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator==(const BasicString<C, T, A>& l, const basic_utf8_string_view<C2, T2>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return r.compare_eq(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

// string_view
template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator==(const basic_utf8_string_view<C, T>& l, const StringView& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator==(const StringView& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return r.compare_eq(l);
}

template<typename CL, typename TL, typename CR, typename TR > inline
bool operator==(const basic_utf8_string_view<CL, TL>& l, const basic_utf8_string_view<CR, TR>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

// !=
template<typename C, typename T, typename T1, typename T2> inline
bool operator!=(const basic_utf8_string_view<C, T>& l, const string_charset_helper<T1, T2>& r)
{
	return !l.compare_eq(r);
}

template<typename C, typename T, typename T1, typename T2> inline
bool operator!=(const string_charset_helper<T1, T2>& l, const basic_utf8_string_view<C, T>& r)
{
	return !r.compare_eq(l);
}

// const char*
template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator!=(const Char* l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !r.compare_eq(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator!=(const basic_utf8_string_view<C, T>& l, const Char* r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !l.compare_eq(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

// basic_string
// fix utf8_string_view, boost::container::basic_string conflict
template<typename C, typename T, typename A> inline
bool operator!=(const basic_utf8_string_view<C, T>& l, const boost::container::basic_string<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !l.compare_eq(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<typename C, typename T, typename A> inline
bool operator!=(const boost::container::basic_string<C, T, A>& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !r.compare_eq(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator!=(const basic_utf8_string_view<C, T>& l, const BasicString<C2, T2, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !l.compare_eq(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator!=(const BasicString<C, T, A>& l, const basic_utf8_string_view<C2, T2>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !r.compare_eq(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

// string_view
template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator!=(const basic_utf8_string_view<C, T>& l, const StringView& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !l.compare_eq(r);
}

template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator!=(const StringView& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !r.compare_eq(l);
}

template<typename CL, typename TL, typename CR, typename TR > inline
bool operator!=(const basic_utf8_string_view<CL, TL>& l, const basic_utf8_string_view<CR, TR>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !l.compare_eq(r);
}

// <
template<typename C, typename T, typename T1, typename T2> inline
bool operator<(const basic_utf8_string_view<C, T>& l, const string_charset_helper<T1, T2>& r)
{
	return l.compare(r) < 0;
}

template<typename C, typename T, typename T1, typename T2> inline
bool operator<(const string_charset_helper<T1, T2>& l, const basic_utf8_string_view<C, T>& r)
{
	return 0 < r.compare(l);
}

// const char*
template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator<(const Char* l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator<(const basic_utf8_string_view<C, T>& l, const Char* r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) < 0;
}

// basic_string
// fix utf8_string_view, boost::container::basic_string conflict
template<typename C, typename T, typename A> inline
bool operator<(const basic_utf8_string_view<C, T>& l, const boost::container::basic_string<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C)) < 0;
}

template<typename C, typename T, typename A> inline
bool operator<(const boost::container::basic_string<C, T, A>& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator<(const basic_utf8_string_view<C, T>& l, const BasicString<C2, T2, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2)) < 0;
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator<(const BasicString<C, T, A>& l, const basic_utf8_string_view<C2, T2>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

// string_view
template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator<(const basic_utf8_string_view<C, T>& l, const StringView& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator<(const StringView& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < r.compare(l);
}

template<typename CL, typename TL, typename CR, typename TR > inline
bool operator<(const basic_utf8_string_view<CL, TL>& l, const basic_utf8_string_view<CR, TR>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

// <=
template<typename C, typename T, typename T1, typename T2> inline
bool operator<=(const basic_utf8_string_view<C, T>& l, const string_charset_helper<T1, T2>& r)
{
	return l.compare(r) <= 0;
}

template<typename C, typename T, typename T1, typename T2> inline
bool operator<=(const string_charset_helper<T1, T2>& l, const basic_utf8_string_view<C, T>& r)
{
	return 0 <= r.compare(l);
}

// const char*
template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator<=(const Char* l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 <= r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator<=(const basic_utf8_string_view<C, T>& l, const Char* r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) <= 0;
}

// basic_string
// fix utf8_string_view, boost::container::basic_string conflict
template<typename C, typename T, typename A> inline
bool operator<=(const basic_utf8_string_view<C, T>& l, const boost::container::basic_string<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C)) <= 0;
}

template<typename C, typename T, typename A> inline
bool operator<=(const boost::container::basic_string<C, T, A>& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 <= r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator<=(const basic_utf8_string_view<C, T>& l, const BasicString<C2, T2, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2)) <= 0;
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator<=(const BasicString<C, T, A>& l, const basic_utf8_string_view<C2, T2>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 <= r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

// string_view
template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator<=(const basic_utf8_string_view<C, T>& l, const StringView& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator<=(const StringView& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 <= r.compare(l);
}

template<typename CL, typename TL, typename CR, typename TR > inline
bool operator<=(const basic_utf8_string_view<CL, TL>& l, const basic_utf8_string_view<CR, TR>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

// >
template<typename C, typename T, typename T1, typename T2> inline
bool operator>(const basic_utf8_string_view<C, T>& l, const string_charset_helper<T1, T2>& r)
{
	return 0 < l.compare(r);
}

template<typename C, typename T, typename T1, typename T2> inline
bool operator>(const string_charset_helper<T1, T2>& l, const basic_utf8_string_view<C, T>& r)
{
	return r.compare(l) < 0;
}

// const char*
template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator>(const Char* l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 > r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator>(const basic_utf8_string_view<C, T>& l, const Char* r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) > 0;
}

// basic_string
// fix utf8_string_view, boost::container::basic_string conflict
template<typename C, typename T, typename A> inline
bool operator>(const basic_utf8_string_view<C, T>& l, const boost::container::basic_string<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C)) > 0;
}

template<typename C, typename T, typename A> inline
bool operator>(const boost::container::basic_string<C, T, A>& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 > r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator>(const basic_utf8_string_view<C, T>& l, const BasicString<C2, T2, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2)) > 0;
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator>(const BasicString<C, T, A>& l, const basic_utf8_string_view<C2, T2>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 > r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

// string_view
template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator>(const basic_utf8_string_view<C, T>& l, const StringView& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator>(const StringView& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 > r.compare(l);
}

template<typename CL, typename TL, typename CR, typename TR > inline
bool operator >(const basic_utf8_string_view<CL, TL>& l, const basic_utf8_string_view<CR, TR>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < l.compare(r);
}

// >=
template<typename C, typename T, typename T1, typename T2> inline
bool operator>=(const basic_utf8_string_view<C, T>& l, const string_charset_helper<T1, T2>& r)
{
	return 0 <= l.compare(r);
}

template<typename C, typename T, typename T1, typename T2> inline
bool operator>=(const string_charset_helper<T1, T2>& l, const basic_utf8_string_view<C, T>& r)
{
	return r.compare(l) <= 0;
}

// const char*
template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator>=(const Char* l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 >= r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename C, typename T, typename Char>
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		bool
	>::type
	operator>=(const basic_utf8_string_view<C, T>& l, const Char* r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) >= 0;
}

// basic_string
// fix utf8_string_view, boost::container::basic_string conflict
template<typename C, typename T, typename A> inline
bool operator>=(const basic_utf8_string_view<C, T>& l, const boost::container::basic_string<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C)) >= 0;
}

template<typename C, typename T, typename A> inline
bool operator>=(const boost::container::basic_string<C, T, A>& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 >= r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C));
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator>=(const basic_utf8_string_view<C, T>& l, const BasicString<C2, T2, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2)) >= 0;
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename C2, typename T2, typename A> inline
typename
	boost::enable_if
	<
		is_basic_string_t< BasicString<C2, T2, A> >,
		bool
	>::type
	operator>=(const BasicString<C, T, A>& l, const basic_utf8_string_view<C2, T2>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 >= r.compare(l, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(C2));
}

// string_view
template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator>=(const basic_utf8_string_view<C, T>& l, const StringView& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

template<typename C, typename T, typename StringView> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< StringView >,
		bool
	>::type
	operator>=(const StringView& l, const basic_utf8_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 >= r.compare(l);
}

template<typename CL, typename TL, typename CR, typename TR > inline
bool operator >=(const basic_utf8_string_view<CL, TL>& l, const basic_utf8_string_view<CR, TR>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 <= l.compare(r);
}

// io

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const basic_utf8_string_view<Char_utf8, Traits_utf8>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits,
			typename Char_utf8, typename Traits_utf8>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				BOOST_RV_REF_BEG basic_utf8_string_view<Char_utf8, Traits_utf8> BOOST_RV_REF_END right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

} // namespace charset
} // namespace yggr


namespace yggr
{
namespace charset
{
namespace swap_support
{

template<typename C, typename T > inline
void swap(basic_utf8_string_view<C, T>& l, basic_utf8_string_view<C, T>& r)
{
	l.swap(r);
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

template<typename Char, typename Traits > inline
bool is_validate_char(const basic_utf8_string_view<Char, Traits>& /*str*/)
{
	return false;
}

template<typename Char, typename Traits > inline
bool is_validate_string(const basic_utf8_string_view<Char, Traits>& str)
{
	return str.is_validate();
}

} // namespace charset
} // namespace yggr

// string_charset_helper
namespace yggr
{
namespace charset
{

// string_charset_helper_data
template<typename Char, typename Traits> YGGR_CONSTEXPR_OR_INLINE
const basic_utf8_string_view<Char, Traits>&
	string_charset_helper_data(const basic_utf8_string_view<Char, Traits>& str)
{
	return str;
}

template<typename Char, typename Traits> YGGR_CONSTEXPR_OR_INLINE
const basic_utf8_string_view<Char, Traits>&
	string_charset_helper_data(const basic_utf8_string_view<Char, Traits>& str, const string& charset_name)
{
	assert(charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return str;
}

template<typename Char, typename Traits, typename Charset_Name > YGGR_CONSTEXPR_OR_INLINE
const basic_utf8_string_view<Char, Traits>&
	string_charset_helper_data(const string_charset_helper<basic_utf8_string_view<Char, Traits>&, Charset_Name>& val)
{
	assert(YGGR_STR_UTF8_STRING_CHARSET_NAME() == val.second);
	return val.first;
}

template<typename Char, typename Traits, typename Charset_Name > YGGR_CONSTEXPR_OR_INLINE
const basic_utf8_string_view<Char, Traits>&
	string_charset_helper_data(const string_charset_helper<const basic_utf8_string_view<Char, Traits>&, Charset_Name>& val)
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

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, basic_utf8_string_view)

} // namespace charset
} // namespace yggr

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace charset
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename C, typename T > inline
std::size_t hash_value(const basic_utf8_string_view<C, T>& val)
{
	return val.hash();
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

template<typename C, typename T >
struct hash< ::yggr::charset::basic_utf8_string_view<C, T> >
{
	typedef ::yggr::charset::basic_utf8_string_view<C, T> type;

	inline std::size_t operator()(const type& val) const
	{
		return val.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#undef YGGR_PP_UTF8_STRING_DEBUG_DST_CHAR_SET_NAME

namespace yggr
{
namespace charset
{
namespace detail
{

//utf8_string_view_loadable_helper
template<typename T>
struct utf8_string_view_loadable_helper
	: public is_utf8_string_view_t<T>
{
};

template<typename T1, typename T2>
struct utf8_string_view_loadable_helper< string_charset_helper< T1, T2 > >
	: public
		boost::mpl::and_
		<
			boost::mpl::not_< boost::is_const<T1> >,
			utf8_string_view_loadable_helper< typename boost::remove_reference<T1>::type >
		>::type
{
};

} // namespace detail

template<typename T>
struct utf8_string_view_loadable
	: public
		detail::utf8_string_view_loadable_helper
		<
			typename
				boost::remove_cv
				<
					typename boost::remove_reference<T>::type
				>::type
		>::type
{
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_UTF8_STRING_VIEW_IMPL_HPP__
