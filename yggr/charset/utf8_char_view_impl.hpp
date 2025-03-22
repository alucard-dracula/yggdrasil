// utf8_char_view_impl.hpp

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

#ifndef __YGGR_CHARSET_UTF8_CHAR_VIEW_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_CHAR_VIEW_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>
#include <yggr/support/operator_t.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/bytes/byte_size_sp_tpl.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/strict_sizeof.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/charset/is_boost_string.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/convertible_traits.hpp>
#include <yggr/charset/charset_foo.hpp>
#include <yggr/charset/utf8_foo.hpp>

#include <yggr/charset/utf8_string_iterator_decl.hpp>
#include <yggr/charset/utf8_string_impl_decl.hpp>
#include <yggr/charset/utf8_string_view_decl.hpp>

#include <yggr/charset/utf8_char_view_decl.hpp>
#include <yggr/charset/utf8_char_impl_decl.hpp>
#include <yggr/charset/utf8_char_reference_decl.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/functional/hash/hash.hpp>

YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::basic_utf8_char_view, 3)

namespace yggr
{
namespace charset
{

// is_utf8_char_view
template<typename Char, typename Traits>
struct is_utf8_char_view_t< basic_utf8_char_view<Char, Traits> >
	: public boost::mpl::true_
{
};

//template<typename Char, typename Traits>
//struct charset_name_t< basic_utf8_char_view<Char, Traits> >
//	: private nonable::noncopyable,
//		private nonable::nonmoveable
//{
//public:
//	typedef typename basic_utf8_char_view<Char, Traits>::char_type char_type;
//	typedef typename boost::remove_cv<Char>::type native_char_type;
//	typedef
//		utf8_string_impl
//		<
//			typename char_type::value_type,
//			typename char_type::traits_type,
//			typename mplex::typename_expand_get<char_type, 2>::type
//		> string_type;
//
//	typedef
//		basic_utf8_string_view
//		<
//			typename char_type::value_type,
//			typename char_type::traits_type
//		> string_view_type;
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
struct charset_name_t_string_view_impl< basic_utf8_char_view<Char, Traits> >
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef typename basic_utf8_char_view<Char, Traits>::char_type char_type;
	typedef typename boost::remove_cv<Char>::type native_char_type;
	typedef
		utf8_string_impl
		<
			typename char_type::value_type,
			typename char_type::traits_type,
			typename mplex::typename_expand_get<char_type, 2>::type
		> string_type;

	typedef
		basic_utf8_string_view
		<
			typename char_type::value_type,
			typename char_type::traits_type
		> string_view_type;

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

template< typename TChar, typename TTraits>
class basic_utf8_char_view
{

private:
	template<typename Char, typename Traits>
	friend class basic_utf8_char_view;

public:
	typedef
		utf8_string_impl
		<
			TChar,
			TTraits,
			typename mplex::typename_expand_get<basic_string<TChar>, 2>::type
		> utf8_string_type;
	typedef
		utf8_char_impl
		<
			TChar,
			TTraits,
			typename mplex::typename_expand_get<basic_string<TChar>, 2>::type
		> char_type;
	typedef basic_utf8_char_view char_view_type;

	typedef typename char_type::base_type base_type;
	typedef
		basic_string
		<
			TChar,
			TTraits,
			typename mplex::typename_expand_get<basic_string<TChar>, 2>::type
		> base_string_type;

	BOOST_MPL_ASSERT((boost::is_same<base_type, base_string_type>));

	typedef typename char_type::size_type size_type;

	typedef typename char_type::value_type value_type;
	typedef typename char_type::traits_type traits_type;

	typedef value_type* pointer;
	typedef const value_type* const_pointer;

	typedef const_pointer iterator;
	typedef const_pointer const_iterator;

	typedef utf8_string_iterator<char_type, typename base_string_type::iterator> utf8_iterator;
	typedef utf8_string_iterator<char_type, typename base_string_type::const_iterator> utf8_const_iterator;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<value_type>::value == 1)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(mplex::strict_sizeof<typename traits_type::char_type>::value == 1)>));

private:
	typedef basic_utf8_char_view this_type;
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:
	// don't write basic_utf8_char_view(void),
	// because if _pstart is allowed to be null,
	// then the operator bool will produce ambiguity (*_pstart == 0 or _pstart == 0)
	//basic_utf8_char_view(void);

	template<typename T>
	basic_utf8_char_view(const T* ptr)
		: _pstart(reinterpret_cast<const char*>(ptr))
	{
		assert(_pstart);
		assert(utf8_foo::charlen(_pstart));
	}

	template<typename Iterator>
	basic_utf8_char_view(const Iterator& iter,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										is_iterator<Iterator>,
										boost::mpl::not_< is_reverse_iterator<Iterator> >
									>,
									mplex::sfinae_type
								>::type sfinae = 0)
		: _pstart(reinterpret_cast<const char*>(&(*iter)))
	{
		assert(_pstart);
		assert(utf8_foo::charlen(_pstart));
	}

	template<typename Char, typename Traits, typename Alloc>
	basic_utf8_char_view(const utf8_char_impl<Char, Traits, Alloc>& right)
		: _pstart(right.data())
	{
		assert(_pstart);
	}

	template<typename Char, typename Traits, typename Alloc>
	basic_utf8_char_view(const utf8_string_impl<Char, Traits, Alloc>& right,
							size_type pos)
		: _pstart(&(right.at(pos)))
	{
		assert(_pstart);
	}

	template<typename Utf8String>
	basic_utf8_char_view(const basic_utf8_char_reference<Utf8String>& right)
		: _pstart(right.data())
	{
		assert(_pstart);
	}

	template<typename Utf8String>
	basic_utf8_char_view(const basic_utf8_char_const_reference<Utf8String>& right)
		: _pstart(right.data())
	{
		assert(_pstart);
	}

	template<typename Char, typename Traits>
	basic_utf8_char_view(const basic_utf8_char_view<Char, Traits>& right)
		: _pstart(right._pstart)
	{
		assert(_pstart);
	}

	basic_utf8_char_view(BOOST_RV_REF(this_type) right)
		: _pstart(right._pstart)
	{
		assert(_pstart);
	}

	basic_utf8_char_view(const this_type& right)
		: _pstart(right._pstart)
	{
		assert(_pstart);
	}

	~basic_utf8_char_view(void)
	{
	}

public:
	template<typename Char, typename Traits> inline
	this_type& operator=(const basic_utf8_char_view<Char, Traits>& right)
	{
		_pstart = right._pstart;
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		_pstart = right._pstart;
		return *this;
	}

public:
	//operator type
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		assert(_pstart);
		return (1 < utf8_foo::charlen(_pstart)) || (*_pstart);
	}

	// operator basic_string
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		assert(_pstart);
		return
			char_type(
				_pstart, utf8_foo::charlen(_pstart),
				YGGR_STR_INNER_STRING_CHARSET_NAME()).template str<ret_type>();
	}

	// operator utf8_char_impl
	template<typename Char, typename Traits, typename Alloc> inline
	operator utf8_char_impl<Char, Traits, Alloc>(void) const
	{
		return utf8_char_impl<Char, Traits, Alloc>(*this);
	}

	inline operator char_type(void) const
	{
		return char_type(*this);
	}

	// other_type basic_utf8_char_view
	template<typename Char, typename Traits> inline
	operator basic_utf8_char_view<Char, Traits>(void) const
	{
		return basic_utf8_char_view<Char, Traits>(*this);
	}

#define YGGR_TMP_PP_OPERATOR_T( __type__ ) \
	inline operator __type__(void) const { \
		assert(_pstart); \
		return static_cast< __type__ >(utf8_foo::char_to_number(_pstart)); }

	YGGR_TMP_PP_OPERATOR_T(s8)
	YGGR_TMP_PP_OPERATOR_T(u8)

	YGGR_TMP_PP_OPERATOR_T(s16)
	YGGR_TMP_PP_OPERATOR_T(u16)

	YGGR_TMP_PP_OPERATOR_T(s32)
	YGGR_TMP_PP_OPERATOR_T(u32)

#if ((defined(__clang__) && (__clang__)) \
		&& ((defined(_ILP32) && (_ILP32)) \
			|| defined(YGGR_AT_DARWIN)))
	YGGR_TMP_PP_OPERATOR_T(long int)
	YGGR_TMP_PP_OPERATOR_T(unsigned long int)
#endif // __clang__ _ILP32 __APPLE__

	YGGR_TMP_PP_OPERATOR_T(s64)
	YGGR_TMP_PP_OPERATOR_T(u64)

	YGGR_TMP_PP_OPERATOR_T(f32)
	YGGR_TMP_PP_OPERATOR_T(f64)

#undef YGGR_TMP_PP_OPERATOR_T

public:
	inline const value_type* operator&(void) const
	{
		assert(_pstart);
		return _pstart;
	}

	inline const value_type* data(void) const
	{
		assert(_pstart);
		return _pstart;
	}

public:
	inline size_type size(void) const
	{
		assert(_pstart);
		return utf8_foo::charlen(_pstart);
	}

	inline size_type length(void) const
	{
		assert(this_type::is_validate());
		return 1;
	}

	inline bool is_validate(void) const
	{
		assert(_pstart);
		return utf8_foo::charlen(_pstart);
	}

public:
	inline const_iterator begin(void) const
	{
		assert(_pstart);
		return _pstart;
	}

	inline const_iterator cbegin(void) const
	{
		assert(_pstart);
		return _pstart;
	}

	inline const_iterator end(void) const
	{
		assert(_pstart);
		return _pstart + utf8_foo::charlen(_pstart);
	}

	inline const_iterator cend(void) const
	{
		assert(_pstart);
		return _pstart + utf8_foo::charlen(_pstart);
	}

public:
	template<typename OUtf8_String> inline
	bool is_same_address(const basic_utf8_char_reference<OUtf8_String>& right) const
	{
		return _pstart == right.data();
	}

	template<typename OUtf8_String> inline
	bool is_same_address(const basic_utf8_char_const_reference<OUtf8_String>& right) const
	{
		return _pstart == right.data();
	}

	template<typename Char, typename Traits> inline
	bool is_same_address(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return _pstart == right._pstart;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	bool is_same_address(const utf8_char_impl<Char, Traits, Alloc>& right) const
	{
		return _pstart == right.data();
	}

public:
	// compare
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			s32
		>::type
		compare(Char c) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::compare(
				char_type(
					static_cast<now_char_type>(c),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type)),
					YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			s32
		>::type
		compare(Char c, const string& src_charset_name) const
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::charcmp(_pstart, static_cast<now_char_type>(c))
				: this_type::compare(
					char_type(c, src_charset_name),
					YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(const Char* str, size_type n) const
	{
		return
			this_type::compare(
				char_type(str, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		compare(const Char* src, size_type n, const string& src_charset_name) const
	{
		assert(src);
		return src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(_pstart, src)
					: this_type::compare(char_type(src, n, src_charset_name),
											YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_native_char<Char>,
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
			is_native_char<Char>,
			s32
		>::type
		compare(const Char* src, const string& src_charset_name) const
	{
		assert(src);
#ifdef _DEBUG
		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(( ((*reinterpret_cast<const char*>(src)) == 0) || (1 == utf8_foo::strlen(src)) ));
		}
		else
		{
			base_string_type tmp_src;
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		//const_iterator s = this_type::begin();

		if(reinterpret_cast<const value_type*>(src) == _pstart)
		{
			//char_view("a") == "a"; // not support assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			//assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::charcmp(_pstart, src)
				: utf8_foo::charcmp(_pstart,
									charset_foo::s_xchg_diff_charset<base_string_type>(
										src, src_charset_name,
										YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			s32
		>::type
		compare(const Basic_String<Char, Traits, Alloc>& src) const
	{
		return this_type::compare(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			s32
		>::type
		compare(const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name) const
	{
#ifdef _DEBUG
		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(( src.empty()
						|| ((*reinterpret_cast<const char*>(src.data())) == 0)
						|| (1 == utf8_foo::strlen(src)) ));
		}
		else
		{
			base_string_type tmp_src;
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		//const_iterator s = this_type::begin();

		if(reinterpret_cast<const value_type*>(src.data()) == _pstart)
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}

		return src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(_pstart, src.data())
					: utf8_foo::charcmp(_pstart,
										charset_foo::s_xchg_diff_charset<base_string_type>(
											src, src_charset_name,
											YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other) const
	{
		return utf8_foo::charcmp(_pstart, other.data());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), other.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_reference<OUtf8_String>& right) const
	{
		return this_type::compare(right, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_reference<OUtf8_String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_pstart, right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_const_reference<OUtf8_String>& right) const
	{
		return utf8_foo::charcmp(_pstart, right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_const_reference<OUtf8_String>& right,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_pstart, right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return utf8_foo::charcmp(_pstart, right._pstart);
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right,
				const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_pstart, right._pstart);
	}

	template<typename T1, typename T2> inline
	s32 compare(const string_charset_helper<T1, T2>& val) const
	{
		return this_type::compare(val.first, val.second);
	}

	template<typename T1, typename T2> inline
	s32 compare(const string_charset_helper<T1, T2>& val, size_type n) const
	{
		return this_type::compare(val.first, n, val.second);
	}

	// org_str
	inline base_type org_str(void) const
	{
		return base_type(_pstart, utf8_foo::charlen(_pstart));
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			base_type
		>::type
		org_str(const Alloc& alloc) const
	{
		return base_type(_pstart, utf8_foo::charlen(_pstart), alloc);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_basic_string_t<String>,
			String
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		assert(this_type::size());

		size_type byte_size = this_type::size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		memcpy(&str[0], _pstart, byte_size);
		return str;
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
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		assert(this_type::size());

		size_type byte_size = this_type::size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		memcpy(&str[0], _pstart, byte_size);
		return str;
	}

	// str
	inline utf8_string_type str(void) const
	{
		assert(_pstart);
		return
			utf8_string_type(
				_pstart, utf8_foo::charlen(_pstart),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			utf8_string_type
		>::type
		str(const Alloc& alloc) const
	{
		assert(_pstart);
		return utf8_string_type(_pstart, utf8_foo::charlen(_pstart), alloc,
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const string& dst_charset_name) const
	{
		assert(_pstart);
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return utf8_string_type(_pstart, utf8_foo::charlen(_pstart), dst_charset_name);
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			utf8_string_type
		>::type
		str(const Alloc& alloc, const string& dst_charset_name) const
	{
		assert(_pstart);
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return
			utf8_string_type(
				_pstart, utf8_foo::charlen(_pstart), alloc,
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_utf8_string_t<String>,
			String
		>::type
		str(void) const
	{
		typedef String ret_string_type;

		assert(_pstart);

		return
			ret_string_type(
				_pstart, utf8_foo::charlen(_pstart),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename String, typename Alloc> inline
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

		assert(_pstart);

		return
			ret_string_type(
				_pstart, utf8_foo::charlen(_pstart), alloc,
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			is_utf8_string_t<String>,
			String
		>::type
		str(const string& dst_charset_name) const
	{
		typedef String ret_string_type;

		assert(_pstart);
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return
			ret_string_type(
				_pstart, utf8_foo::charlen(_pstart),
				YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename String, typename Alloc> inline
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
		str(const Alloc& alloc, const string& dst_charset_name) const
	{
		typedef String ret_string_type;

		assert(_pstart);
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return ret_string_type(_pstart, utf8_foo::charlen(_pstart), alloc,
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename String> inline
	typename
		boost::disable_if
		<
			is_utf8_string_t<String>,
			String
		>::type
		str(void) const
	{

		typedef String ret_string_type;

		assert(_pstart);

		return
			charset_foo::s_xchg<ret_string_type>(
				_pstart, utf8_foo::charlen(_pstart),
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type) );
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_utf8_string_t<String> >,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc) const
	{

		typedef String ret_string_type;

		assert(_pstart);

		return
			charset_foo::s_xchg<ret_string_type>(
				_pstart, utf8_foo::charlen(_pstart), alloc,
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type));
	}

	template<typename String> inline
	typename
		boost::disable_if
		<
			is_utf8_string_t<String>,
			String
		>::type
		str(const string& dst_charset_name) const
	{

		typedef String ret_string_type;

		assert(_pstart);

		return charset_foo::s_xchg<ret_string_type>(
				_pstart, utf8_foo::charlen(_pstart),
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_utf8_string_t<String> >,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc, const string& dst_charset_name) const
	{

		typedef String ret_string_type;

		assert(_pstart);

		return charset_foo::s_xchg<ret_string_type>(
				_pstart, utf8_foo::charlen(_pstart), alloc,
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				dst_charset_name);
	}

	// view
	inline this_type& view(void)
	{
		return *this;
	}

	inline const this_type& view(void) const
	{
		return *this;
	}

	template<typename View>
	inline View view(void) const
	{
		return View(*this);
	}

	// arithmetic
	inline s32 to_number(void) const
	{
		assert(_pstart);
		return utf8_foo::char_to_number(_pstart);
	}

	inline char_type operator+(void) const
	{
		return char_type(_pstart, utf8_foo::charlen(_pstart), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline char_type operator-(void) const
	{
		assert(_pstart);

		if(0 == *_pstart)
		{
			assert(utf8_foo::charlen(_pstart) == 1);
			return char_type();
		}
		else
		{
			value_type buf[utf8_foo::E_char_max_size] = {0};
			size_type next_size = 0;

			utf8_foo::number_to_char(
				buf, utf8_foo::negative_number(
						utf8_foo::char_to_number(_pstart, utf8_foo::charlen(_pstart))),
				utf8_foo::E_char_max_size, next_size);

			assert(next_size);
			return char_type(buf, next_size, YGGR_STR_INNER_STRING_CHARSET_NAME());
		}
	}

	inline char_type operator~(void) const
	{
		char_type ret(this_type::operator-());
		ret -= 1;
		return ret;
	}

	// hash

	inline std::size_t hash(void) const
	{
		assert(_pstart);
		return boost::hash_range(_pstart, _pstart + utf8_foo::charlen(_pstart));
	}

protected:
	const_pointer _pstart;
};

// io
template<typename Char, typename Traits, typename Char_uc, typename Traits_uc>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const basic_utf8_char_view<Char_uc, Traits_uc>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Char_uc, typename Traits_uc>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				BOOST_RV_REF_BEG basic_utf8_char_view<Char_uc, Traits_uc> BOOST_RV_REF_END right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}


// arithmetic

// num __op__ utf8_char_cref -> num, utf8_char_ref __op__ num -> utf8_char
// utf8_char_cref __op__ utf8_char_cref -> utf8_char
// utf8_char_cref __op__ basic_utf8_char_view, basic_utf8_char_view __op__ utf8_char_ref -> utf8_char

#define YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				boost::is_enum<T> >, \
			typename \
				boost::mpl::if_< \
					boost::is_enum<T>, \
					s32, \
					typename __result_deducer__<T, s32>::type >::type >::type \
		operator __op__(T l, const basic_utf8_char_view<Char, Traits>& r) { \
		typedef typename \
			boost::mpl::if_< \
				boost::is_enum<T>, \
				s32, \
				typename __result_deducer__<T, s32>::type >::type result_type; \
		return static_cast<result_type>(l) __op__ r.to_number(); }

#define YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( __op__, __use_op__ ) \
	template<typename T, typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				boost::is_enum<T> >, \
			typename basic_utf8_char_view<Char, Traits>::char_type >::type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, T r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename CharL, typename TraitsL, typename CharR, typename TraitsR> inline \
	typename basic_utf8_char_view<CharL, TraitsL>::char_type \
		operator __op__(const basic_utf8_char_view<CharL, TraitsL>& l, \
						const basic_utf8_char_view<CharR, TraitsR>& r) { \
		typedef typename basic_utf8_char_view<CharL, TraitsL>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; }

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( +, plus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( +, += )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( -, minus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( -, -= )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( *, multiplies_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( *, *= )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( /, divides_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( /, /= )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( %, modulus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( %, %= )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( <<, bitwise_left_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( <<, <<= )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL( >>, bitwise_right_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL( >>, >>= )

#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_OF_NUMBER_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_ARITHMETIC_GENERAL_IMPL

// += -= *= /= %= &= |= ^=

#define YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( __op__ ) \
	template<typename T, typename Char, typename Traits> inline \
	T& operator __op__(T& l, const basic_utf8_char_view<Char, Traits>& r) { \
		l __op__ r.to_number(); \
		return l; }

	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( += )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( -= )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( *= )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( /= )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( %= )

	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( &= )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( |= )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL( ^= )



#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_OPERATOR_SET_IMPL


#define YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, const basic_utf8_char_view<Char, Traits>& r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, T r) { \
		return l.to_number() __op__ r; } \
	\
	template<typename T, typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, T r) { \
		return l.to_number() __op__ r; }

#define YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( __op__, __self_ret__ ) \
	template<typename CharL, typename TraitsL, typename CharR, typename TraitsR> inline \
	s32 operator __op__(const basic_utf8_char_view<CharL, TraitsL>& l, \
						const basic_utf8_char_view<CharR, TraitsR>& r) { \
		return l.data() == r.data()? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); }

	// operator&
	YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_OF_NUMBER_IMPL( &, bitwise_and_operator_result)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( &, l.to_number() )

	// operator|
	YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_OF_NUMBER_IMPL( |, bitwise_or_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( |, l.to_number() )

	// operator^
	YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_OF_NUMBER_IMPL( ^, bitwise_xor_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( ^, 0 )

#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_BITWISE_OF_NUMBER_IMPL

// comp
// ==, !=

#if (BOOST_VERSION < 106400)
#	define YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1_BASIC_STRING(__op__) \
		template<typename Char_uc, typename Traits_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
		return r.compare(l) __op__ 0; }

#else

#	define YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1_BASIC_STRING(__op__) \
		template<typename Char_uc, typename Traits_uc, \
					typename Char, typename Traits, typename Alloc, \
					template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					is_boost_string< Basic_String<Char, Traits, Alloc> >, \
					boost::is_same<Char, Char_uc>, \
					boost::is_same<Traits, Traits_uc> >, \
				bool >::type \
			operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, \
								const Basic_String<Char, Traits, Alloc>& r) { \
			return l.compare(r) __op__ 0; } \
		\
		template<typename Char_uc, typename Traits_uc, \
					typename Char, typename Traits, typename Alloc, \
					template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					is_boost_string< Basic_String<Char, Traits, Alloc> >, \
					boost::is_same<Char, Char_uc>, \
					boost::is_same<Traits, Traits_uc> >, \
				bool >::type \
			operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
								const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
			return r.compare(l) __op__ 0; }

#endif // BOOST_VERSION < 106400

#define YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1(__op__) \
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Traits1, typename Traits2 > inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits1>& l, \
							const basic_utf8_char_view<Char, Traits2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename CharL, typename TraitsL, typename CharR, typename TraitsR> inline \
	bool operator __op__(const basic_utf8_char_view<CharL, TraitsL>& l, \
							const utf8_char_impl<CharR, TraitsR>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename CharL, typename TraitsL, typename CharR, typename TraitsR> inline \
	bool operator __op__(const utf8_char_impl<CharL, TraitsL>& l, \
							const basic_utf8_char_view<CharR, TraitsR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename T1, typename T2> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1_BASIC_STRING(__op__)

#define YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2(__op__, __rop__) \
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char_uc, typename Traits_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const basic_utf8_char_view<Char_uc, Traits_uc>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const basic_utf8_char_view<Char_uc, Traits_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Traits1, typename Traits2> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits1>& l, \
							const basic_utf8_char_view<Char, Traits2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename CharL, typename TraitsL, \
				typename CharR, typename TraitsR, typename AllocR> inline \
	bool operator __op__(const basic_utf8_char_view<CharL, TraitsL>& l, \
							const utf8_char_impl<CharR, TraitsR, AllocR>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename CharL, typename TraitsL, typename AllocL, \
				typename CharR, typename TraitsR> inline \
	bool operator __op__(const utf8_char_impl<CharL, TraitsL, AllocL>& l, \
							const basic_utf8_char_view<CharR, TraitsR>& r) { \
		return !(l.compare(r) __rop__ 0); } \
	\
	template<typename Char, typename Traits, typename T1, typename T2> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return !(r.compare(l) __rop__ 0); }

#if (BOOST_VERSION < 106400)
#	define YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH(__op__, __rop__)
#else
#	define YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH(__op__, __rop__) \
		template<typename Char, typename Traits, typename Alloc> inline \
		bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
								const boost::container::basic_string<Char, Traits, Alloc>& r) { \
			return l.compare(r) __op__ 0; } \
		\
		template<typename Char, typename Traits, typename Alloc> inline \
		bool operator __op__(const boost::container::basic_string<Char, Traits, Alloc>& l, \
								const basic_utf8_char_view<Char, Traits>& r) { \
			return !(r.compare(l) __rop__ 0); }

#endif // (BOOST_VERSION < 106400)

	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1(==)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1(!=)

	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2(<, <=)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2(<=, <)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2(>, >=)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2(>=, >)

	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH(<, <=)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH(<=, <)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH(>, >=)
	YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH(>=, >)

#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP1
#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2
#undef YGGR_TMP_PP_UTF8_CHAR_VIEW_CMP2_BOOST_BASIC_STRING_PATCH

} // namespace charset
} // namespace yggr


namespace yggr
{
namespace charset
{

template<typename Char, typename Traits> inline
string_charset_helper<const basic_utf8_char_view<Char, Traits>&, const string&>
	make_string_charset_helper(const basic_utf8_char_view<Char, Traits>& src,
								const string& src_charset_name
									= charset_name_t< basic_utf8_char_view<Char, Traits> >())
{
	typedef const basic_utf8_char_view<Char, Traits>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

	return ret_type(src, src_charset_name);
}

template<typename Char, typename Traits> inline
string_charset_helper<basic_utf8_char_view<Char, Traits>&, const string&>
	make_string_charset_helper(basic_utf8_char_view<Char, Traits>& src,
								const string& src_charset_name
									= charset_name_t< basic_utf8_char_view<Char, Traits> >())
{
	typedef basic_utf8_char_view<Char, Traits>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

	return ret_type(src, src_charset_name);
}

} // namespace charset
} // namespace yggr

// boost range_size support
namespace yggr
{
namespace charset
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, basic_utf8_char_view)

} // namespace charset
} // namespace yggr

// byte_size support
namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename Char, typename Traits>
struct byte_size_tpl_helper< charset::basic_utf8_char_view<Char, Traits> >
	: public boost::mpl::true_
{
public:
	typedef charset::basic_utf8_char_view<Char, Traits> value_type;

public:
	inline u64 operator()(const value_type& val) const
	{
		return val.size();
	}
};

} // namespace detail
} // namespace bytes
} // namespace yggr


// ---------------hash_value------------------
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace charset
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename Char, typename Traits> inline
std::size_t hash_value(const ::yggr::charset::basic_utf8_char_view<Char, Traits>& ch)
{
	return ch.hash();
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

template<typename Char, typename Traits>
struct hash< ::yggr::charset::basic_utf8_char_view<Char, Traits> >
{
	typedef ::yggr::charset::basic_utf8_char_view<Char, Traits> type;

	inline std::size_t operator()(const type& ch) const
	{
		return ch.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_CHARSET_UTF8_CHAR_VIEW_IMPL_HPP__
