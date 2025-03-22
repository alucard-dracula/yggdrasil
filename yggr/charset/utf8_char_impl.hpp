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

#ifndef __YGGR_CHARSET_UTF8_CHAR_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_CHAR_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/support/operator_t.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/ppex/symbols.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>

#include <yggr/type_traits/is_enum.hpp>
#include <yggr/type_traits/operator_result.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/is_convertible_size_t.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/get_allocator.hpp>
#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/bytes/byte_size_sp_container.hpp>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/assert_char.hpp>
#include <yggr/charset/string_charset_helper.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/basic_string_adp_helper.hpp>
#include <yggr/charset/charset_foo.hpp>
#include <yggr/charset/utf8_foo.hpp>

#include <yggr/charset/utf8_string_impl_decl.hpp>
#include <yggr/charset/utf8_string_view_decl.hpp>
#include <yggr/charset/utf8_string_iterator_decl.hpp>

#include <yggr/charset/utf8_char_impl_decl.hpp>
#include <yggr/charset/utf8_char_view_decl.hpp>
#include <yggr/charset/utf8_char_reference_decl.hpp>

#include <boost/ref.hpp>

#include <boost/preprocessor/cat.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/functional/hash/hash.hpp>
#include <boost/utility/enable_if.hpp>

#include <iostream>
#include <cassert>
#include <complex>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#if !(BOOST_VERSION < 106600)

namespace boost
{
namespace container
{

// fix mingw gcc ambiguous overload for 'operator>' 'operator>='  'operator=='  'operator<=' 'operator<'  
// yggr: utf8_string_impl<...>() >= basic_string<...> or boost: BasicStringView<...> >= basic_sting()
template<typename Char, typename Traits, typename Alloc>
struct is_string< ::yggr::charset::utf8_char_impl<Char, Traits, Alloc> >
{
	static const bool value = true;
};

} // namespace container
} // namspace boost

#endif // #if !(BOOST_VERSION < 106600)

// charset_name_t
namespace yggr
{
namespace charset
{

// is_utf8_char_impl
template<typename Char, typename Traits, typename Alloc>
struct is_utf8_char_t< utf8_char_impl<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
};

//template<typename Char, typename Traits, typename Alloc>
//struct charset_name_t< utf8_char_impl<Char, Traits, Alloc> >
//	:	private nonable::noncopyable,
//		private nonable::nonmoveable
//{
//public:
//	typedef utf8_char_impl<Char, Traits, Alloc> char_type;
//	typedef typename boost::remove_cv<Char>::type native_char_type;
//	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
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

template<typename Char, typename Traits, typename Alloc>
struct charset_name_t_string_impl< utf8_char_impl<Char, Traits, Alloc> >
	:	private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef utf8_char_impl<Char, Traits, Alloc> char_type;
	typedef typename boost::remove_cv<Char>::type native_char_type;
	typedef utf8_string_impl<Char, Traits, Alloc> string_type;
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

// other_char
namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc, typename C1, typename C2 >
struct other_char<utf8_char_impl<Char, Traits, Alloc >, C1, C2 >
{
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef utf8_char_impl<Char, Traits, Alloc > type;
};

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename Iter,
			bool is_seq_random_access_iter =
				boost::mpl::and_
				<
					is_random_access_iterator<Iter>,
					boost::mpl::not_< is_reverse_iterator<Iter> >
				>::value>
struct utf8_char_impl_iterator_assign_helper;

template<typename Iter>
struct utf8_char_impl_iterator_assign_helper<Iter, true>
{
public:
	typedef Iter iter_type;
private:
	typedef utf8_char_impl_iterator_assign_helper this_type;

public:
	template<typename Utf8Char>
	inline Utf8Char& operator()(Utf8Char& dst, iter_type s, iter_type e,
									const string& src_charset_name) const
	{
		dst.assign(&(*s), std::distance(s, e), src_charset_name);
		return dst;
	}
};

template<typename Iter>
struct utf8_char_impl_iterator_assign_helper<Iter, false>
{
public:
	typedef Iter iter_type;

private:
	typedef utf8_char_impl_iterator_assign_helper this_type;

public:

	template<typename Utf8Char>
	inline Utf8Char& operator()(Utf8Char& dst, iter_type s, iter_type e,
								const string& src_charset_name) const
	{
		typedef typename boost::iterator_value<iter_type>::type iter_val_type;

		typedef typename
			boost::mpl::if_
			<
				is_native_char<iter_val_type>,
				basic_string<iter_val_type>,
				vector<iter_val_type>
			>::type src_type;

		src_type src(s, e);

		if(src.empty())
        {
            dst.assign(iter_val_type(0));
        }
        else
        {
            dst.assign(&src[0], src.size(), src_charset_name);
        }

        return dst;
	}
};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! utf8_char arithmetic rule:																						!!!
!!!	if result is utf8_char the result must in utf8_char's range														!!!
!!!		in utf8_default_mode the range is 0x00000000 - 0x0010ffff (the value defined in utf8_foo::E_char_max_num)	!!!
!!!		in utf8_ex_mode the range is 0x00000000 - 0x7fffffff														!!!
!!!																													!!!
!!!	if result is number, the result is number + utf8_char.to_number(),												!!!
!!!		range is std::numeric_limits<result_type>::min() - std::numeric_limits<result_type>::max()					!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/


template<typename TChar, typename TTraits, typename TAlloc>
class utf8_char_impl
{
public:
	typedef ::yggr::charset::basic_string<TChar, TTraits, TAlloc> base_type;
	typedef base_type base_string_type;

public:
	typedef TAlloc tpl_arg_allocator_type;

public:
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;
	typedef typename base_type::difference_type difference_type;

protected:
	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

public:
	typedef typename base_type::size_type size_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::traits_type traits_type;
	typedef typename base_type::allocator_type allocator_type;

	typedef utf8_char_impl char_type;
	typedef basic_utf8_char_view<value_type, traits_type> char_view_type;
	typedef basic_utf8_char_reference<char_type> char_reference_type;
	typedef basic_utf8_char_const_reference<char_type> char_const_reference_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<value_type, char>));

protected:
	YGGR_STATIC_CONSTANT(u32, E_max_size = 5);

protected:
	typedef mplex::sfinae_type sfinae_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<value_type, char>));
	BOOST_MPL_ASSERT((boost::is_same<typename traits_type::char_type, char>));
	BOOST_MPL_ASSERT((boost::is_same<typename allocator_type::value_type, char>));

private:
	typedef utf8_char_impl this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	//default
	utf8_char_impl(void)
		: _base(1, value_type(0))
	{
	}

	explicit utf8_char_impl(const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
	}

	// char
	template<typename Char>
	utf8_char_impl(Char c)
		: _base(1, value_type(0))
	{
		BOOST_MPL_ASSERT((is_can_be_conv_into_char<Char>));
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(static_cast<now_char_type>(c));
	}

	template<typename Char>
	utf8_char_impl(Char c, const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_can_be_conv_into_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(static_cast<now_char_type>(c));
	}

	template<typename Char>
	utf8_char_impl(Char c, const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_can_be_conv_into_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0))
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(static_cast<now_char_type>(c), src_charset_name);
	}

	template<typename Char>
	utf8_char_impl(Char c, const allocator_type& alloc, const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_can_be_conv_into_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		this_type::assign(static_cast<now_char_type>(c));
	}


	// const char*
	template<typename Char>
	utf8_char_impl(const Char* src)
		: _base(1, value_type(0))
	{
		BOOST_MPL_ASSERT((is_native_char<Char>));
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_char_impl(const Char* src, const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_char_impl(const Char* src, const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0))
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Char>
	utf8_char_impl(const Char* src,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, src_charset_name);
	}

	// const char* n
	template<typename Char>
	utf8_char_impl(const Char* src, size_type n,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0))
	{
		BOOST_MPL_ASSERT((is_convertible_char<Char>));
		this_type::assign(src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_char_impl(const Char* src, size_type n,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, n, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	utf8_char_impl(const Char* src, size_type n,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0))
	{
		this_type::assign(src, n, src_charset_name);
	}

	template<typename Char>
	utf8_char_impl(const Char* src, size_type n,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_convertible_char<Char>,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, n, src_charset_name);
	}

	// string
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_char_impl(const Basic_String<Char, Traits, Alloc>& src)
		: _base(1, value_type(0))
	{
		typedef Basic_String<Char, Traits, Alloc> src_type;
		BOOST_MPL_ASSERT((is_basic_string_t<src_type>));
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_char_impl(const Basic_String<Char, Traits, Alloc>& src,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_char_impl(const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0))
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	utf8_char_impl(const Basic_String<Char, Traits, Alloc>& src,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, src_charset_name);
	}

	// string_view
	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView >
	utf8_char_impl(const BasicStringView<Char, Traits>& src)
		: _base(1, value_type(0))
	{
		typedef BasicStringView<Char, Traits> src_type;
		BOOST_MPL_ASSERT((is_basic_string_view_t<src_type>));
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView >
	utf8_char_impl(const BasicStringView<Char, Traits>& src,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_basic_string_view_t< BasicStringView<Char, Traits> >,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView >
	utf8_char_impl(const BasicStringView<Char, Traits>& src,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_basic_string_view_t< BasicStringView<Char, Traits> >,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0))
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView >
	utf8_char_impl(const BasicStringView<Char, Traits>& src,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_basic_string_view_t< BasicStringView<Char, Traits> >,
							sfinae_type
						>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, src_charset_name);
	}

	// iterator (start, end)
	template<typename InputIter>
	utf8_char_impl(InputIter start, InputIter last,
					typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _base(1, value_type(0))
	{
		this_type::assign(start, last);
	}

	template<typename InputIter>
	utf8_char_impl(InputIter start, InputIter last,
					const allocator_type& alloc,
					typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(start, last);
	}

	template<typename InputIter>
	utf8_char_impl(InputIter start, InputIter last, const string& src_charset_name,
					typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _base(1, value_type(0))
	{
		this_type::assign(start, last, src_charset_name);
	}

	template<typename InputIter>
	utf8_char_impl(InputIter start, InputIter last,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename boost::enable_if<is_iterator<InputIter>, sfinae_type>::type nil = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(start, last, src_charset_name);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char>
	utf8_char_impl(std::initializer_list<Char> il)
		: _base(1, value_type(0))
	{
		this_type::assign(il);
	}

	template<typename Char>
	utf8_char_impl(std::initializer_list<Char> il, const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(il);
	}

	template<typename Char>
	utf8_char_impl(std::initializer_list<Char> il, const string& src_charset_name)
		: _base(1, value_type(0))
	{
		this_type::assign(il, src_charset_name);
	}

	template<typename Char>
	utf8_char_impl(std::initializer_list<Char> il,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(il, src_charset_name);
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// utf8_char_impl
	template<typename Char, typename Traits, typename Alloc>
	utf8_char_impl(const utf8_char_impl<Char, Traits, Alloc>& other)
		: _base(other.begin(), other.end())
	{
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_char_impl(const utf8_char_impl<Char, Traits, Alloc>& other,
					const allocator_type& alloc)
		: _base(other.begin(), other.end(), alloc)
	{
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_char_impl(const utf8_char_impl<Char, Traits, Alloc>& other,
					const string& src_charset_name)
		: _base(other.begin(), other.end())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_char_impl(const utf8_char_impl<Char, Traits, Alloc>& other,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(other.begin(), other.end(), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	// base_type
	utf8_char_impl(BOOST_RV_REF(base_type) src)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	utf8_char_impl(const base_type& src)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	utf8_char_impl(BOOST_RV_REF(base_type) src, const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	utf8_char_impl(const base_type& src, const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	utf8_char_impl(BOOST_RV_REF(base_type) src, const string& src_charset_name)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(boost::move(src), src_charset_name);
	}

	utf8_char_impl(const base_type& src, const string& src_charset_name)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(src, src_charset_name);
	}

	utf8_char_impl(BOOST_RV_REF(base_type) src,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(boost::move(src), src_charset_name);
	}

	utf8_char_impl(const base_type& src,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, src_charset_name);
	}

	// isomeric_same_base_type
	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						::yggr::charset::basic_string<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END src,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2>
	utf8_char_impl(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& src,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						::yggr::charset::basic_string<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END src,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2>
	utf8_char_impl(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& src,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						::yggr::charset::basic_string<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END src,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(boost::move(src), src_charset_name);
	}

	template<typename Alloc2>
	utf8_char_impl(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& src,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), src.get_allocator())
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						::yggr::charset::basic_string<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END src,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(boost::move(src), src_charset_name);
	}

	template<typename Alloc2>
	utf8_char_impl(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& src,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(src, src_charset_name);
	}

	// this_type
	utf8_char_impl(BOOST_RV_REF(this_type) right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(boost::move(right));
	}

	utf8_char_impl(const this_type& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}

	utf8_char_impl(BOOST_RV_REF(this_type) right, const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(boost::move(right));
	}

	utf8_char_impl(const this_type& right, const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	utf8_char_impl(BOOST_RV_REF(this_type) right, const string& src_charset_name)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(boost::move(right));
	}

	utf8_char_impl(const this_type& right, const string& src_charset_name)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right, src_charset_name);
	}

	utf8_char_impl(BOOST_RV_REF(this_type) right,
					const allocator_type& alloc, const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(boost::move(right));
	}

	utf8_char_impl(const this_type& right,
					const allocator_type& alloc, const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	// is_isomeric_same_string this_type
	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						utf8_char_impl<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(boost::move(right));
	}

	template<typename Alloc2>
	utf8_char_impl(const utf8_char_impl<value_type, traits_type, Alloc2>& right,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}

	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						utf8_char_impl<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(boost::move(right));
	}

	template<typename Alloc2>
	utf8_char_impl(const utf8_char_impl<value_type, traits_type, Alloc2>& right,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						utf8_char_impl<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(boost::move(right));
	}

	template<typename Alloc2>
	utf8_char_impl(const utf8_char_impl<value_type, traits_type, Alloc2>& right,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	template<typename Alloc2>
	utf8_char_impl(BOOST_RV_REF_BEG
						utf8_char_impl<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(boost::move(right));
	}

	template<typename Alloc2>
	utf8_char_impl(const utf8_char_impl<value_type, traits_type, Alloc2>& right,
					const allocator_type& alloc,
					const string& src_charset_name,
					typename
						boost::enable_if
						<
							is_isomeric_same_string
							<
								typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	// basic_utf8_char_reference
	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_reference<Utf8String>& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}

	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_reference<Utf8String>& right,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_reference<Utf8String>& right,
					const string& src_charset_name)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_reference<Utf8String>& right,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

#if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
	utf8_char_impl(char_reference_type& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}
#endif // #	if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

	utf8_char_impl(const char_reference_type& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}

	utf8_char_impl(const char_reference_type& right,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	utf8_char_impl(const char_reference_type& right,
					const string& src_charset_name)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	utf8_char_impl(const char_reference_type& right,
					const allocator_type& alloc,
					const string& src_charset_name )
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	// basic_utf8_char_const_reference
	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_const_reference<Utf8String>& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}

	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_const_reference<Utf8String>& right,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_const_reference<Utf8String>& right,
					const string& src_charset_name)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	template<typename Utf8String>
	utf8_char_impl(const basic_utf8_char_const_reference<Utf8String>& right,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

#if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
	utf8_char_impl(char_const_reference_type& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}
#endif // #	if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

	utf8_char_impl(const char_const_reference_type& right)
		: _base(1, value_type(0), right.get_allocator())
	{
		this_type::assign(right);
	}

	utf8_char_impl(const char_const_reference_type& right,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	utf8_char_impl(const char_const_reference_type& right,
					const string& src_charset_name)
		: _base(1, value_type(0), right.get_allocator())
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	utf8_char_impl(const char_const_reference_type& right,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	// utf8_char_view
	template<typename Char, typename Traits>
	utf8_char_impl(const basic_utf8_char_view<Char, Traits>& right)
		: _base(1, value_type(0))
	{
		this_type::assign(right);
	}

	template<typename Char, typename Traits>
	utf8_char_impl(const basic_utf8_char_view<Char, Traits>& right,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(right);
	}

	template<typename Char, typename Traits>
	utf8_char_impl(const basic_utf8_char_view<Char, Traits>& right,
					const string& src_charset_name)
		: _base(1, value_type(0))
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	template<typename Char, typename Traits>
	utf8_char_impl(const basic_utf8_char_view<Char, Traits>& right,
					const allocator_type& alloc,
					const string& src_charset_name)
		: _base(1, value_type(0), alloc)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::assign(right);
	}

	// string_charset_helper
	template<typename T1, typename T2>
	utf8_char_impl(const string_charset_helper<T1, T2>& val,
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
		: _base(1, value_type(0), container::get_allocator(val.first))
	{
		this_type::assign(val.first, val.second);
	}

	template<typename T1, typename T2>
	utf8_char_impl(const string_charset_helper<T1, T2>& val,
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
		: _base(1, value_type(0))
	{
		this_type::assign(val.first, val.second);
	}

	template<typename T1, typename T2>
	utf8_char_impl(const string_charset_helper<T1, T2>& val,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(val.first, val.second);
	}

	template<typename T1, typename T2>
	utf8_char_impl(const string_charset_helper<T1, T2>& val,
					size_type n,
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
		: _base(1, value_type(0), container::get_allocator(val.first))
	{
		this_type::assign(val.first, n, val.second);
	}

	template<typename T1, typename T2>
	utf8_char_impl(const string_charset_helper<T1, T2>& val,
					size_type n,
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
		: _base(1, value_type(0))
	{
		this_type::assign(val.first, n, val.second);
	}

	template<typename T1, typename T2>
	utf8_char_impl(const string_charset_helper<T1, T2>& val,
					size_type n,
					const allocator_type& alloc)
		: _base(1, value_type(0), alloc)
	{
		this_type::assign(val.first, n, val.second);
	}

	~utf8_char_impl(void)
	{
	}

public:
	// =
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		operator=(Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return this_type::assign(static_cast<now_char_type>(c), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_native_char<Char>,
			this_type&
		>::type
		operator=(const Char* src)
	{
		return this_type::assign(src);
	}

	// this foo don't write rvref_version,
	// because Basic_string can be std::basic_string and boost::container::string and some different declarations
	// if base_type is differnt Basic_string can use copy only
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

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			this_type&
		>::type
		operator=(const BasicStringView<Char, Traits>& str)
	{
		return this_type::assign(str);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator=(const utf8_char_impl<Char, Traits, Alloc>& other)
	{
		return this_type::assign(other, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	// operator= base_type
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
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						::yggr::charset::basic_string<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right)
	{
		typedef ::yggr::charset::basic_string<value_type, traits_type, Alloc2> now_string_type;

		now_string_type& right_ref = right;
		return this_type::assign(boost::move(reinterpret_cast<base_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& right)
	{
		return this_type::assign(reinterpret_cast<const base_type&>(right));
	}

	// operator= this_type
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
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						utf8_char_impl<value_type, traits_type, Alloc2>
					BOOST_RV_REF_END right)
	{
		return this_type::assign(boost::move(reinterpret_cast<this_type&>(right)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const utf8_char_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::assign(reinterpret_cast<const this_type&>(right));
	}

	template<typename Utf8String> inline
	this_type& operator=(const basic_utf8_char_reference<Utf8String>& right)
	{
		return this_type::assign(right);
	}

	template<typename Utf8String> inline
	this_type& operator=(const basic_utf8_char_const_reference<Utf8String>& right)
	{
		return this_type::assign(right);
	}

	template<typename Char, typename Traits> inline
	this_type& operator=(const basic_utf8_char_view<Char, Traits>& right)
	{
		return this_type::assign(right);
	}

	template<typename T1, typename T2> inline
	this_type& operator=(const string_charset_helper<T1, T2> val)
	{
		return this_type::assign(val.first, val.second);
	}

public:
//swap

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
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
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
		swap(BOOST_RV_REF_BEG Basic_String<Char, Traits, Alloc> BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef Basic_String<Char, Traits, Alloc> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			void
		>::type
		swap(Basic_String<Char, Traits, Alloc>& right, const string& src_charset_name)
	{
		typedef Basic_String<Char, Traits, Alloc> right_type;

		this_type tmp(right, src_charset_name);
		right = this_type::str<right_type>(right.get_allocator(), src_charset_name);

		_base.swap(tmp._base);
	}

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	void swap(base_type& right)
	{
		this_type::swap(right, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	inline void swap(BOOST_RV_REF(base_type) right, const string& src_charset_name)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref, src_charset_name);
	}

	void swap(base_type& right, const string& src_charset_name)
	{
		if(_base.data() == right.data())
		{
			return;
		}
		else if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert(1 == utf8_foo::strlen(right.data(), right.size()));
			_base.swap(right);
		}
		else
		{
			this_type tmp(right, src_charset_name);
			right = this_type::str<base_type>(right.get_allocator(), src_charset_name);

			_base.swap(tmp._base);
		}
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					::yggr::charset::basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef ::yggr::charset::basic_string<value_type, traits_type, Alloc2> now_string_type;

		now_string_type& right_ref = right;
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
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(::yggr::charset::basic_string<value_type, traits_type, Alloc2>& right)
	{
		this_type::swap(
			reinterpret_cast<base_type&>(right),
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					::yggr::charset::basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef ::yggr::charset::basic_string<value_type, traits_type, Alloc2> now_string_type;

		now_string_type& right_ref = right;
		this_type::swap(
			reinterpret_cast<base_type&>(right_ref),
			src_charset_name);
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			void
		>::type
		swap(::yggr::charset::basic_string<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		this_type::swap(reinterpret_cast<base_type&>(right), src_charset_name);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(_base.data() == right.data())
		{
			return;
		}

		_base.swap(right._base);
	}

	inline void swap(BOOST_RV_REF(this_type) right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right, const string& src_charset_name)
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
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					utf8_char_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_char_impl<value_type, traits_type, Alloc2> right_type;

		right_type& right_ref = right;
		this_type::swap(reinterpret_cast<this_type&>(right_ref));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(utf8_char_impl<value_type, traits_type, Alloc2>& right)
	{
		this_type::swap(reinterpret_cast<this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					utf8_char_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right, const string& src_charset_name)
	{
		typedef utf8_char_impl<value_type, traits_type, Alloc2> right_type;

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
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			void
		>::type
		swap(utf8_char_impl<value_type, traits_type, Alloc2>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::swap(reinterpret_cast<this_type&>(right));
	}

	template<typename Utf8String>
	inline void swap(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END right)
	{
		typedef basic_utf8_char_reference<Utf8String> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Utf8String>
	void swap(basic_utf8_char_reference<Utf8String>& right)
	{
		assert(_base.data() != right.data());
		right.swap(*this);
	}

	template<typename Utf8String>
	inline void swap(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END right,
						const string& src_charset_name)
	{
		typedef basic_utf8_char_reference<Utf8String> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Utf8String>
	void swap(basic_utf8_char_reference<Utf8String>& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::swap(right);
	}

	inline void swap(BOOST_RV_REF_BEG basic_utf8_char_reference<char_type> BOOST_RV_REF_END right)
	{
		typedef basic_utf8_char_reference<char_type> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(basic_utf8_char_reference<char_type>& right)
	{
		if(_base.data() == right.data())
		{
			return;
		}

		_base.swap(right._str_ref._base);
	}

	inline void swap(BOOST_RV_REF_BEG basic_utf8_char_reference<char_type> BOOST_RV_REF_END right,
						const string& src_charset_name)
	{
		typedef basic_utf8_char_reference<char_type> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(basic_utf8_char_reference<char_type>& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		this_type::swap(right);
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
	//operator type
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		assert(!_base.empty());
		return (1 < _base.size()) || (_base.at(0));
	}

#define YGGR_TMP_PP_OPERATOR_T( __type__ ) \
	inline operator __type__(void) const { \
		return this_type::to_number< __type__ >(); }

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

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>();
	}

	// don't write these function, because gcc conflict construct {aka construct is ambiguous}
	// because utf8_char_reference construct params is utf8_string_type&
	//operator utf8_char_reference<this_type>(void);

	template<typename Utf8String> inline
	operator basic_utf8_char_reference<Utf8String>(void)
	{
		BOOST_MPL_ASSERT((boost::is_same<Utf8String, this_type>));
		return basic_utf8_char_reference<Utf8String>(*this, 0);
	}

#if !(defined(__clang__) && (__clang__))
	inline operator basic_utf8_char_const_reference<this_type>(void) const
	{
		return basic_utf8_char_const_reference<this_type>(*this, 0);
	}
#endif // #if !(defined(__clang__) && (__clang__))

	template<typename Char, typename Traits> inline
	operator basic_utf8_char_view<Char, Traits>(void) const
	{
		return basic_utf8_char_view<Char, Traits>(_base.data());
	}

	inline operator char_view_type(void) const
	{
		return char_view_type(_base.data());
	}

	// must using value_type* not using const_pointer, because segemnt string const_pointer not a raw pointer
	inline value_type* operator&(void)
	{
		assert(!(_base.empty()));
		return boost::addressof(_base.at(0));
	}

	inline const value_type* operator&(void) const
	{
		assert(!(_base.empty()));
		return _base.data();
	}

	// begin end
	inline iterator begin(void)
	{
		return _base.begin();
	}

	inline const_iterator begin(void) const
	{
		return _base.begin();
	}

	inline const_iterator cbegin(void) const
	{
		return _base.begin();
	}

	inline iterator end(void)
	{
		return _base.end();
	}

	inline const_iterator end(void) const
	{
		return _base.end();
	}

	inline const_iterator cend(void) const
	{
		return _base.end();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE value_type* data(void)
	{
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17 )
		return const_cast<value_type*>(_base.data());
#else
		return _base.data();
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17 )
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* data(void) const
	{
		return _base.data();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* c_str(void) const
	{
		return _base.data();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type size(void) const
	{
		return _base.size();
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type length(void) const
	{
		return 1;
	}

	inline bool empty(void) const
	{
		assert(!(_base.empty()));
		return false;
	}

	inline bool is_validate(void) const
	{
		assert(!(_base.empty()));
		return (!(_base.empty()))
				&& (_base.size() == utf8_foo::charlen(_base.data()));
	}

	inline void shrink_to_fit(void)
	{
		::yggr::charset::shrink_to_fit(_base);
	}

	// assign
	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		assign(Char c)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		return
			this_type::assign(
				static_cast<now_char_type>(c),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			this_type&
		>::type
		assign(Char c, const string& src_charset_name)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) now_char_type;

		if(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))
		{
			basic_string_assign(_base, 1, static_cast<value_type>(c));
		}
		else
		{
			now_char_type tmp[2] = {static_cast<now_char_type>(c), 0};
			this_type::assign(tmp, 1, src_charset_name);
		}

		return *this;
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_native_char<Char>,
			this_type&
		>::type
		assign(const Char* src)
	{
		return this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char>
	typename
		boost::enable_if
		<
			is_native_char<Char>,
			this_type&
		>::type
		assign(const Char* src, const string& src_charset_name)
	{
		assert(src);

		if(reinterpret_cast<const value_type*>(src) == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return *this;
		}

		if(!(src && src[0]))
		{
			basic_string_assign(_base, 1, value_type(0));
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			_base.swap(right);
		}

		return *this;
	}

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

	template<typename Char>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			this_type&
		>::type
		assign(const Char* src, size_type n, const string& src_charset_name)
	{
		assert(src);
		if(src && n)
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			_base.swap(right);
		}

		return *this;
	}

	// don't write
	// assign(vector), assign(string, pos, n),
	// because utf8_char_impl is a char, not a string

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

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			this_type&
		>::type
		assign(const Basic_String<Char, Traits, Alloc>& src,
				const string& src_charset_name)
	{
		if(reinterpret_cast<const value_type*>(src.data()) == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return *this;
		}

		if(src.empty() || src[0] == 0)
		{
			basic_string_assign(_base, 1, value_type(0));
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			_base.swap(right);
		}
		return *this;
	}

public:
	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			this_type&
		>::type
		assign(const BasicStringView<Char, Traits>& src)
	{
		return this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			this_type&
		>::type
		assign(const BasicStringView<Char, Traits>& src,
				const string& src_charset_name)
	{
		if(reinterpret_cast<const value_type*>(src.data()) == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return *this;
		}

		if(src.empty() || src[0] == 0)
		{
			basic_string_assign(_base, 1, value_type(0));
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			_base.swap(right);
		}
		return *this;
	}

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
		typedef InputIter iter_type;
		typedef detail::utf8_char_impl_iterator_assign_helper<iter_type> h_type;
		typedef typename boost::iterator_value<iter_type>::type now_char_type;

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
		assign(InputIter start, InputIter last, const string& src_charset_name)
	{
		typedef InputIter iter_type;
		typedef detail::utf8_char_impl_iterator_assign_helper<iter_type> h_type;

		h_type h;
		return h(*this, start, last, src_charset_name);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last)
	{
		assert((std::distance(start, last) == 1));
		basic_string_assign(_base, start.org(), last.org());
		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_modify_iterator<Char, Basic_Iter> start,
						utf8_string_modify_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert((std::distance(start, last) == 1));
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, start.org(), last.org());
		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last)
	{
		assert((std::distance(start, last) == 1));
		basic_string_assign(_base, start.org(), last.org());
		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert((std::distance(start, last) == 1));
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, start.org(), last.org());
		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last)
	{
		assert((std::distance(start, last) == 1));
		basic_string_assign(_base, start.org(), last.org());
		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert((std::distance(start, last) == 1));
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, start.org(), last.org());
		return *this;
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
		assert((std::distance(start, last) == 1));
		basic_string_assign(_base, &(*start), utf8_foo::charlen(&(*start)));
		return *this;
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
		assert((std::distance(start, last) == 1));
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, &(*start), utf8_foo::charlen(&(*start)));
		return *this;
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
		assert((std::distance(start, last) == 1));
		basic_string_assign(_base, &(*start), utf8_foo::charlen(&(*start)));
		return *this;
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
		assert((std::distance(start, last) == 1));
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, &(*start), utf8_foo::charlen(&(*start)));
		return *this;
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
		assert((std::distance(start, last) == 1));
		basic_string_assign(_base, &(*start), utf8_foo::charlen(&(*start)));
		return *this;
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
		assert((std::distance(start, last) == 1));
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, &(*start), utf8_foo::charlen(&(*start)));
		return *this;
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
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) );
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

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_char_impl<Char, Traits, Alloc>& other)
	{
		basic_string_assign(_base, other.data(), other.size());
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_char_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_assign(_base, other.data(), other.size());
		return *this;
	}

#if !(defined(_DEBUG) && defined(_YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY))

	inline this_type& assign(BOOST_RV_REF(base_type) src)
	{
		return this_type::assign(boost::move(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& assign(BOOST_RV_REF(base_type) src, const string& src_charset_name)
	{
		if(_base.data() == src.data())
		{
			return *this;
		}

		if(src.empty() || src[0] == 0)
		{
			basic_string_assign(_base, 1, value_type(0));
			return *this;
		}

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert( 1 == utf8_foo::strlen(src));
			copy_or_move_or_swap(_base, boost::move(src));
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			_base.swap(right);
		}

		return *this;
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					::yggr::charset::basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src)
	{
		typedef ::yggr::charset::basic_string<value_type, traits_type, Alloc2> now_string_type;
		now_string_type& src_ref = src;
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
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					::yggr::charset::basic_string<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END src,
				const string& src_charset_name)
	{
		typedef ::yggr::charset::basic_string<value_type, traits_type, Alloc2> now_string_type;
		now_string_type& src_ref = src;
		return
			this_type::assign(
				boost::move(reinterpret_cast<base_type&>(src_ref)),
				src_charset_name);
	}

	inline this_type& assign(const base_type& src)
	{
		return this_type::assign(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	this_type& assign(const base_type& src, const string& src_charset_name)
	{
		if(_base.data() == src.data())
		{
			return *this;
		}

		if(src.empty() || src[0] == 0)
		{
			basic_string_assign(_base, 1, value_type(0));
			return *this;
		}

		if(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME())
		{
			assert( 1 == utf8_foo::strlen(src));
			basic_string_assign(_base, src);
		}
		else
		{
			base_type right(_base.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			_base.swap(right);
		}

		return *this;
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& src)
	{
		return
			this_type::assign(
				boost::move(reinterpret_cast<const base_type&>(src)),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(base_type));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				::yggr::charset::basic_string<value_type, traits_type, Alloc2>,
				base_type
			>,
			this_type&
		>::type
		assign(const ::yggr::charset::basic_string<value_type, traits_type, Alloc2>& src,
				const string& src_charset_name)
	{
		return
			this_type::assign(
				boost::move(reinterpret_cast<const base_type&>(src)),
				src_charset_name);
	}

	// assign this_type

	inline this_type& assign(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;

		if(_base.data() == right_chk.data())
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right._base));

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
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_char_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right)
	{
		typedef utf8_char_impl<value_type, traits_type, Alloc2> now_char_type;
		now_char_type& right_ref = right;
		return this_type::assign(boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(BOOST_RV_REF_BEG
					utf8_char_impl<value_type, traits_type, Alloc2>
				BOOST_RV_REF_END right,
				 const string& src_charset_name)
	{
		typedef utf8_char_impl<value_type, traits_type, Alloc2> now_char_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		now_char_type& right_ref = right;
		return this_type::assign(boost::move(reinterpret_cast<this_type&>(right_ref)));
	}

	inline this_type& assign(const this_type& right)
	{
		if(_base.data() == right.data())
		{
			return *this;
		}

		basic_string_assign(_base, right.data(), right.size());
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
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_char_impl<value_type, traits_type, Alloc2>& right)
	{
		return this_type::assign(reinterpret_cast<const this_type&>(right));
	}

	template<typename Alloc2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_string
			<
				typename utf8_char_impl<value_type, traits_type, Alloc2>::base_type,
				base_type
			>,
			this_type&
		>::type
		assign(const utf8_char_impl<value_type, traits_type, Alloc2>& right,
				 const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(reinterpret_cast<const this_type&>(right));
	}

#endif // _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

	// basic_utf8_char_reference
	template<typename Utf8String> inline
	this_type& assign(const basic_utf8_char_reference<Utf8String>& right)
	{
		if(_base.data() == right.data())
		{
			return *this;
		}

		const value_type* ptr = right.data();
		assert(ptr);
		return ptr? (basic_string_assign(_base, ptr, utf8_foo::charlen(ptr)), *this) : *this;
	}

	template<typename Utf8String> inline
	this_type& assign(const basic_utf8_char_reference<Utf8String>& right,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

	// basic_utf8_char_const_reference
	template<typename Utf8String> inline
	this_type& assign(const basic_utf8_char_const_reference<Utf8String>& right)
	{
		if(_base.data() == right.data())
		{
			return *this;
		}

		const value_type* ptr = right.data();
		assert(ptr);
		return ptr?(basic_string_assign(_base, ptr, utf8_foo::charlen(ptr)), *this) : *this;
	}

	template<typename Utf8String> inline
	this_type& assign(const basic_utf8_char_const_reference<Utf8String>& right,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

	// utf8_char_view
	template<typename Char, typename Traits> inline
	this_type& assign(const basic_utf8_char_view<Char, Traits>& right)
	{
		if(_base.data() == right.data())
		{
			return *this;
		}

		const value_type* ptr = right.data();
		assert(ptr);
		return ptr?(basic_string_assign(_base, ptr, utf8_foo::charlen(ptr)), *this) : *this;
	}

	template<typename Char, typename Traits> inline
	this_type& assign(const basic_utf8_char_view<Char, Traits>& right,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

	// string_charset_helper
	template<typename T1, typename T2> inline
	this_type& assign(const string_charset_helper<T1, T2>& val)
	{
		return this_type::assign(val.first, val.second);
	}

	template<typename T1, typename T2> inline
	this_type& assign(const string_charset_helper<T1, T2>& val, size_type n)
	{
		return this_type::assign(val.first, n, val.second);
	}

	// compare
	// !!!compare don't wrtie isomeric_same version, because other version is include!!!
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
			(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))?
				(_base.size() == 1)?
					(_base[0] == static_cast<value_type>(c)? 0 : _base[0] < static_cast<value_type>(c)? -1 : 1)
					: -1
				: this_type::compare(
					char_type(
						static_cast<now_char_type>(c),
						_base.get_allocator(),
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
			(0 == (static_cast<now_char_type>(c) & (now_char_type(-1) << 7)))?
				(_base.size() == 1)?
					(_base[0] == static_cast<value_type>(c)? 0 : _base[0] < static_cast<value_type>(c)? -1 : 1)
					: -1
				: (src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(_base.data(), static_cast<now_char_type>(c))
					: this_type::compare(
						char_type(static_cast<now_char_type>(c), _base.get_allocator(), src_charset_name),
						YGGR_STR_INNER_STRING_CHARSET_NAME()) );
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
				char_type(str, n, _base.get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)),
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

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::charcmp(_base.data(), src)
				: this_type::compare(
					char_type(src, n, _base.get_allocator(), src_charset_name),
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
			base_type tmp_src(_base.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		if(reinterpret_cast<const value_type*>(src) == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}
		else
		{
			return
				src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(_base.data(), src)
					: utf8_foo::charcmp(
						_base.data(),
						charset_foo::s_xchg_diff_charset<base_type>(
							src, _base.get_allocator(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
		}
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
			base_type tmp_src(_base.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		if(reinterpret_cast<const value_type*>(src.data()) == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}
		else
		{
			return
				src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(_base.data(), src.data())
					: utf8_foo::charcmp(
						_base.data(),
						charset_foo::s_xchg_diff_charset<base_type>(
							src, _base.get_allocator(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
		}
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			s32
		>::type
		compare(const BasicStringView<Char, Traits>& src) const
	{
		return this_type::compare(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			s32
		>::type
		compare(const BasicStringView<Char, Traits>& src,
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
			base_type tmp_src(_base.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		if(reinterpret_cast<const value_type*>(src.data()) == _base.data())
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}
		else
		{
			return
				src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(_base.data(), src.data())
					: utf8_foo::charcmp(
						_base.data(),
						charset_foo::s_xchg_diff_charset<base_type>(
							src, _base.get_allocator(), src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other) const
	{
		return utf8_foo::charcmp(_base.data(), other.data());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_base.data(), other.data());
	}

	inline s32 compare(const this_type& right) const
	{
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	inline s32 compare(const this_type& right,
						const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	template<typename Utf8String> inline
	s32 compare(const basic_utf8_char_reference<Utf8String>& right) const
	{
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	template<typename Utf8String> inline
	s32 compare(const basic_utf8_char_reference<Utf8String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	template<typename Utf8String> inline
	s32 compare(const basic_utf8_char_const_reference<Utf8String>& right) const
	{
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	template<typename Utf8String> inline
	s32 compare(const basic_utf8_char_const_reference<Utf8String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return utf8_foo::charcmp(_base.data(), right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(_base.data(), right.data());
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

	inline allocator_type get_allocator(void) const
	{
		return _base.get_allocator();
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
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					boost::mpl::or_
					<
						boost::is_same<String, base_type>,
						is_isomeric_same_string<String, base_type>
					>
				>,
				is_basic_string_t<String>
			>,
			String
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		assert(_base.size());

		size_type byte_size = _base.size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		if(byte_size) memcpy(_base.data(), &str[0], byte_size);

		return str;
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

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					boost::mpl::or_
					<
						boost::is_same<String, base_type>,
						is_isomeric_same_string<String, base_type>
					>
				>,
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		assert(_base.size());

		size_type byte_size = _base.size();
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		if(byte_size) memcpy(_base.data(), &str[0], byte_size);

		return str;
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
			const String&
		>::type
		org_str(const allocator_type&) const
	{
		return reinterpret_cast<const String&>(_base);
	}

	// str
	inline utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type>
		str(void) const
	{
		typedef utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type> ret_string_type;

		return ret_string_type(_base.data(), _base.size(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type>
		str(const allocator_type& alloc) const
	{
		typedef utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type> ret_string_type;

		return ret_string_type(_base.data(), _base.size(), alloc,
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type>
		str(const string& dst_charset_name) const
	{
		typedef utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type> ret_string_type;

		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return ret_string_type(_base.data(), _base.size(),
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type>
		str(const allocator_type& alloc, const string& dst_charset_name) const
	{
		typedef utf8_string_impl<value_type, traits_type, tpl_arg_allocator_type> ret_string_type;

		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return ret_string_type(_base.data(), _base.size(), alloc,
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

		return ret_string_type(_base.data(), _base.size(),
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

		return ret_string_type(_base.data(), _base.size(), alloc,
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

		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return ret_string_type(_base.data(), _base.size(),
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
		str(const Alloc& alloc,
			const string& dst_charset_name) const
	{
		typedef String ret_string_type;

		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		return ret_string_type(_base.data(), _base.size(), alloc,
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
				boost::mpl::not_<is_utf8_string_t<String> >,
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
                    YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_string_type) );
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

		return 
            YGGR_STR_INNER_STRING_CHARSET_NAME() == dst_charset_name?
                 charset_foo::s_xchg<ret_string_type>(_base)
                 :  charset_foo::s_xchg_diff_charset<ret_string_type>(
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
				boost::mpl::not_<is_utf8_string_t<String> >,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		str(const Alloc& alloc,
				const string& dst_charset_name) const
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

	// view

	inline char_view_type view(void) const
	{
		return char_view_type(_base.data());
	}

	template<typename View> inline
	View view(void) const
	{
		return View(_base.data());
	}

	//arithmetic

	inline s32 to_number(void) const
	{
		return utf8_foo::char_to_number(_base.data());
	}

	template<typename T> inline
	typename boost::enable_if<boost::is_arithmetic<T>, T>::type
		to_number(void) const
	{
		typedef T number_type;
		return static_cast<number_type>(utf8_foo::char_to_number(_base.data()));
	}

	inline const this_type& operator+(void) const
	{
		return *this;
	}

	this_type operator-(void) const
	{
		if(0 == _base[0])
		{
			assert(_base.size() == 1);
			return *this;
		}
		else
		{
			value_type buf[utf8_foo::E_char_max_size] = {0};
			size_type next_size = 0;

			utf8_foo::number_to_char(
				buf, utf8_foo::negative_number(
						utf8_foo::char_to_number(_base.data(), _base.size())),
				utf8_foo::E_char_max_size, next_size);

			assert(next_size);
			return this_type(buf, next_size, _base.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
		}
	}

	inline this_type operator~(void) const
	{
		this_type ret(-(*this));
		ret -= 1;
		return ret;
	}

	// operator+=
	template<typename T> inline
	typename
		boost::enable_if
		<
			is_convertible_size_t<T>,
			this_type&
		>::type
		operator+=(T step)
	{
		return this_type::prv_plus_set(step);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator+=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			s32 num = utf8_foo::char_to_number(ptr);
			return num? this_type::prv_plus_set(num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator+=(const basic_utf8_char_view<Char, Traits>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			s32 num = utf8_foo::char_to_number(ptr);
			return num? this_type::prv_plus_set(num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator+=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::prv_plus_set(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator+=(const basic_utf8_char_reference<this_type>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			s32 num = utf8_foo::char_to_number(ptr);
			return num? this_type::prv_plus_set(num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator+=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::prv_plus_set(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator+=(const basic_utf8_char_const_reference<this_type>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			s32 num = utf8_foo::char_to_number(ptr);
			return num? this_type::prv_plus_set(num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(utf8_foo::char_to_number(ptr));
		}
	}

	// operator-=
	template<typename T> inline
	typename
		boost::enable_if
		<
			is_convertible_size_t<T>,
			this_type&
		>::type
		operator-=(T step)
	{
		return this_type::prv_plus_set(-step);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator-=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(-utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator-=(const basic_utf8_char_view<Char, Traits>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(-utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator-=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::prv_plus_set(-utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator-=(const basic_utf8_char_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(-utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator-=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::prv_plus_set(-utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator-=(const basic_utf8_char_const_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(-utf8_foo::char_to_number(ptr));
		}
	}

	// operator++
	inline this_type& operator++(void)
	{
		return this_type::prv_plus_set(s32(1));
	}

	inline this_type operator++(int)
	{
		this_type ret(*this);
		this_type::prv_plus_set(s32(1));
		return ret;
	}

	//operator--
	inline this_type& operator--(void)
	{
		return this_type::prv_plus_set(s32(-1));
	}

	inline this_type operator--(int)
	{
		this_type ret(*this);
		this_type::prv_plus_set(s32(-1));
		return ret;
	}

	// operator*=
	template<typename Step>
	typename
		boost::enable_if
		<
			is_convertible_size_t<Step>,
			this_type&
		>::type
		operator*=(Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		step_type nstep = static_cast<step_type>(tstep);
		step_type chk_step = std::abs(nstep);

		if(step_type(0) < chk_step && chk_step < step_type(1))
		{
			assert(std::abs(step_type(1) / nstep) > 1);
			return this_type::operator/=(step_type(1) / nstep);
		}
		else if(_base[0] == 0 || nstep == step_type(1))
		{
			return *this;
		}
		else if(nstep == step_type(0))
		{
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			s32 ch_value = static_cast<s32>(static_cast<step_type>(_base[0]) * nstep);

			if((_base.size() == 1) && (0 == (ch_value & 0xffffff80)))
			{
				_base[0] = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
				s32 num = utf8_foo::char_to_number(_base.data(), _base.size());
				return this_type::prv_plus_set(
							num, utf8_foo::format_number_step(
									static_cast<step_type>(num) * (nstep - step_type(1))));

			}
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator*=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			return this_type::prv_mul_set_opt_of_self(utf8_foo::char_to_number(ptr));
		}
		else
		{
			return this_type::prv_mul_set_opt_of_integer(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator*=(const basic_utf8_char_view<Char, Traits>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			return this_type::prv_mul_set_opt_of_self(utf8_foo::char_to_number(ptr));
		}
		else
		{
			return this_type::prv_mul_set_opt_of_integer(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator*=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::prv_mul_set_opt_of_integer(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator*=(const basic_utf8_char_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);

		return
			_base.data() == ptr?
				this_type::prv_mul_set_opt_of_self(utf8_foo::char_to_number(ptr))
				: this_type::prv_mul_set_opt_of_integer(utf8_foo::char_to_number(ptr));
	}

	template<typename Utf8String> inline
	this_type& operator*=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::prv_mul_set_opt_of_integer(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator*=(const basic_utf8_char_const_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);

		return
			_base.data() == ptr?
				this_type::prv_mul_set_opt_of_self(utf8_foo::char_to_number(ptr))
				: this_type::prv_mul_set_opt_of_integer(utf8_foo::char_to_number(ptr));
	}

	// operator/=
	template<typename Step>
	typename
		boost::enable_if
		<
			is_convertible_size_t<Step>,
			this_type&
		>::type
		operator/=(Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;

		step_type nstep = static_cast<step_type>(tstep);
		assert(nstep);
		step_type chk_step = std::abs(nstep);

		if(step_type(0) < chk_step && chk_step < step_type(1))
		{
			assert(std::abs(step_type(1) / nstep) > 1);
			return this_type::operator*=(step_type(1) / nstep);
		}
		else if(_base[0] == 0 || nstep == step_type(1))
		{
			return *this;
		}
		else
		{
			if(_base.size() == 1 && nstep > static_cast<step_type>(1))
			{
				_base[0] = static_cast<value_type>(static_cast<step_type>(_base[0]) / nstep);
				return *this;
			}
			else
			{
				assert(nstep > step_type(1) || nstep < step_type(-1));

				s32 num = nstep > 0?
							utf8_foo::char_to_number(_base.data(), _base.size())
							: utf8_foo::negative_number(utf8_foo::char_to_number(_base.data(), _base.size()));

				if(num < chk_step)
				{
					_base.assign(1, value_type(0));
					return *this;
				}
				else
				{
					return this_type::prv_plus_set(
								num, utf8_foo::format_number_step(
									-((static_cast<step_type>(num) * (chk_step - step_type(1))) / chk_step)));
				}
			}
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator/=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator/=(const basic_utf8_char_view<Char, Traits>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator/=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::prv_div_set_opt_of_integer(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator/=(const basic_utf8_char_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator/=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::prv_div_set_opt_of_integer(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator/=(const basic_utf8_char_const_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(utf8_foo::char_to_number(ptr));
		}
	}

	// operator%=
	template<typename Step>
	typename
		boost::enable_if
		<
			is_convertible_size_t<Step>,
			this_type&
		>::type
		operator%=(Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT_NOT((boost::is_floating_point<Step>));

		step_type step = static_cast<step_type>(tstep);

		assert(step);

		if(_base.size() == 1)
		{
			_base[0] = static_cast<value_type>(static_cast<step_type>(_base[0]) % step);
		}
		else
		{
			s32 num = utf8_foo::char_to_number(_base.data(), _base.size());

			if(!(static_cast<step_type>(num) < std::abs(step)))
			{
				value_type buf[utf8_foo::E_char_max_size] = {0};
				size_type next_size = 0;

				utf8_foo::number_to_char(buf, static_cast<s32>(static_cast<step_type>(num) % step),
												utf8_foo::E_char_max_size, next_size);
				assert(next_size);
				_base.assign(buf, next_size);
			}
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator%=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator%=(const basic_utf8_char_view<Char, Traits>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator%=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::operator%=(utf8_foo::char_to_number(step.data()));
	}

	this_type& operator%=(const basic_utf8_char_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Utf8String> inline
	this_type& operator%=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::operator%=(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator%=(const basic_utf8_char_const_reference<this_type>& step)
	{
		const value_type* ptr = step.data();
		assert(ptr);
		if(_base.data() == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(ptr));
		}
	}

	// operator<<=

	inline this_type& operator<<=(u32 n)
	{
		assert((u32(0x00000001) << n) + 1);
		return n? this_type::prv_mul_set_opt_of_integer((s32(1) << n)) : *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator<<=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		return this_type::operator<<=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename Char, typename Traits> inline
	this_type& operator<<=(const basic_utf8_char_view<Char, Traits>& step)
	{
		return this_type::operator<<=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename Utf8String> inline
	this_type& operator<<=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::operator<<=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename Utf8String> inline
	this_type& operator<<=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::operator<<=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	//	operator>>=

	inline this_type& operator>>=(u32 n)
	{
		assert((u32(0x80000000) >> n) + 1);
		return n? this_type::prv_div_set_opt_of_integer((s32(1) << n)) : *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator>>=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		return this_type::operator>>=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename Char, typename Traits> inline
	this_type& operator>>=(const basic_utf8_char_view<Char, Traits>& step)
	{
		return this_type::operator>>=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename Utf8String> inline
	this_type& operator>>=(const basic_utf8_char_reference<Utf8String>& step)
	{
		return this_type::operator>>=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename Utf8String> inline
	this_type& operator>>=(const basic_utf8_char_const_reference<Utf8String>& step)
	{
		return this_type::operator>>=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	Since bit operations & | ^ require the same range of values,		!!!
	!!!		only numerical calculations can be made.						!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	//hash

	inline std::size_t hash(void) const
	{
		return boost::hash_range(_base.begin(), _base.end());
	}

private:
	// prv_plus_set
	template<typename Step>
	this_type& prv_plus_set(s32 num, Step step)
	{
		typedef Step step_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(utf8_foo::E_char_max_range) <= sizeof(step_type))>));

		assert(step);
		assert(step < step_type(utf8_foo::E_char_max_range));
		assert(num == utf8_foo::char_to_number(_base.data(), _base.size())
				|| num == utf8_foo::negative_number(utf8_foo::char_to_number(_base.data(), _base.size())));

		value_type buf[utf8_foo::E_char_max_size] = {0};
       
        size_type next_size = 0;

		if(step > 0)
		{
			if(step < step_type(utf8_foo::E_char_max_range) - num)
			{
				utf8_foo::number_to_char(
                    buf, static_cast<s32>(static_cast<step_type>(num) + step),
                    utf8_foo::E_char_max_size, next_size);
			}
			else
			{
				step -= static_cast<step_type>(step_type(utf8_foo::E_char_max_range) - num);
				utf8_foo::number_to_char(buf, static_cast<s32>(step), utf8_foo::E_char_max_size, next_size);
			}
		}
		else
		{
			step = -step;
			if(step <= num)
			{
				utf8_foo::number_to_char(buf, static_cast<s32>(static_cast<step_type>(num) - step),
											utf8_foo::E_char_max_size, next_size);
			}
			else
			{
				utf8_foo::number_to_char(buf, static_cast<s32>(step_type(utf8_foo::E_char_max_range)
																- (step - static_cast<step_type>(num))),
											utf8_foo::E_char_max_size, next_size);
			}
		}

		assert(next_size);
		_base.assign(buf, next_size);
		return *this;
	}

	template<typename Step>
	this_type& prv_plus_set(Step nstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(utf8_foo::E_char_max_range) <= sizeof(step_type))>));

		step_type step = utf8_foo::format_number_step(static_cast<step_type>(nstep));

		if(step == step_type(0))
		{
			return *this;
		}
		else
		{
			s32 ch_value = static_cast<s32>(static_cast<step_type>(_base[0]) + step);

			if((_base.size() == 1) && (0 == (ch_value & 0xffffff80)))
			{
				_base[0] = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
				return this_type::prv_plus_set(utf8_foo::char_to_number(_base.data(), _base.size()), step);
			}
		}
	}

	// prv_mul_set_opt_
	template<typename Step>
	this_type& prv_mul_set_opt_of_integer(Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT_NOT((boost::is_floating_point<Step>));

		step_type nstep = static_cast<step_type>(tstep);

		if(_base[0] == 0 || nstep == step_type(1))
		{
			return *this;
		}
		else if(nstep == step_type(0))
		{
			_base.assign(1, value_type(0));
			return *this;
		}
		else
		{
			s32 ch_value = static_cast<s32>(static_cast<step_type>(_base[0]) * nstep);

			if((_base.size() == 1) && (0 == (ch_value & 0xffffff80)))
			{
				_base[0] = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
				s32 num = utf8_foo::char_to_number(_base.data(), _base.size());
				return this_type::prv_plus_set(
							num, utf8_foo::format_number_step(
									static_cast<step_type>(num) * (nstep - step_type(1))));

			}
		}
	}

	this_type& prv_mul_set_opt_of_self(s32 num)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		assert(num >= 0);
		assert(static_cast<step_type>(num) < step_type(utf8_foo::E_char_max_range));

		if(num == 1 || num == 0)
		{
			return *this;
		}
		else
		{
			s32 ch_value = static_cast<s32>(static_cast<step_type>(num) * static_cast<step_type>(num));

			if((_base.size() == 1) && (0 == (ch_value & 0xffffff80)))
			{
				_base[0] = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
				return this_type::prv_plus_set(
							num, utf8_foo::format_number_step(
									static_cast<step_type>(num) * static_cast<step_type>(num - 1)));
			}
		}
	}

	// prv_div_set_opt
	template<typename Step>
	this_type& prv_div_set_opt_of_integer(Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT_NOT((boost::is_floating_point<Step>));

		step_type nstep = static_cast<step_type>(tstep);
		assert(nstep);

		if(_base[0] == 0 || nstep == step_type(1))
		{
			return *this;
		}
		else
		{
			if(_base.size() == 1 && nstep > static_cast<step_type>(1))
			{
				_base[0] = static_cast<value_type>(static_cast<step_type>(_base[0]) / nstep);
				return *this;
			}
			else
			{
				assert(nstep > step_type(1) || nstep < step_type(-1));
				step_type chk_step = std::abs(nstep);

				s32 num = nstep > 0?
							utf8_foo::char_to_number(_base.data(), _base.size())
							: utf8_foo::negative_number(utf8_foo::char_to_number(_base.data(), _base.size()));

				if(num < chk_step)
				{
					_base.assign(1, value_type(0));
					return *this;
				}
				else
				{
					return this_type::prv_plus_set(
								num, utf8_foo::format_number_step(
									-((static_cast<step_type>(num) * (chk_step - step_type(1))) / chk_step)));
				}
			}
		}
	}

protected:
	base_type _base;

};

} // namespace charset
} // namespace yggr

YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::utf8_char_impl, 3)

namespace yggr
{
namespace charset
{

//arithmetic

// num __op__ utf8_char -> num, utf8_char __op__ num -> utf8_char
// utf8_char __op__ utf8_char -> utf8_char
// utf8_char __op__ utf8_char, utf8_char __op__ utf8_char -> utf8_char
// utf8_char __op__ basic_utf8_char_reference, basic_utf8_char_reference __op__ utf8_char->utf8_char
// utf8_char __op__ basic_utf8_char_const_reference, basic_utf8_char_const_reference __op__ utf8_char ->utf8_char

namespace detail
{

template<typename T,
			bool is_arithmetic_or_enum = boost::mpl::or_<boost::is_arithmetic<T>, ::yggr::is_enum<T> >::value >
struct arithmetic_of_number_concept
	: public boost::mpl::bool_<is_arithmetic_or_enum>
{
};

template<template<typename _T1, typename _T2> class ResultCal,
			typename T,
			bool is_arithmetic_or_enum = arithmetic_of_number_concept<T>::value >
struct arithmetic_of_number_result_of
{
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<is_arithmetic_or_enum>));
public:
	typedef mplex::null_type type;
};

template<template<typename _T1, typename _T2> class ResultCal, typename T>
struct arithmetic_of_number_result_of<ResultCal, T, true>
{
public:
	typedef typename
		boost::mpl::if_
		<
			::yggr::is_enum<T>,
			s32,
			typename ResultCal<T, s32>::type
		>::type type;
};

} // namespace detail

#define YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			detail::arithmetic_of_number_concept< T >, \
			typename detail::arithmetic_of_number_result_of< __result_deducer__, T >::type >::type \
		operator __op__(T l, const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef typename detail::arithmetic_of_number_result_of< __result_deducer__, T >::type result_type; \
		return static_cast<result_type>(l) __op__ r.to_number(); }

#define YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( __op__, __use_op__ ) \
	template<typename T, typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_ < \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			utf8_char_impl<Char, Traits, Alloc> >::type \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, T r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename CharL, typename TraitsL, typename AllocL, \
				typename CharR, typename TraitsR, typename AllocR> inline \
	utf8_char_impl<CharL, TraitsL, AllocL> \
		operator __op__(const utf8_char_impl<CharL, TraitsL, AllocL>& l, \
						const utf8_char_impl<CharR, TraitsR, AllocR>& r) { \
		typedef utf8_char_impl<CharL, TraitsL, AllocL> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename CharL, typename TraitsL, typename AllocL, \
				typename CharR, typename TraitsR> inline \
	utf8_char_impl<CharL, TraitsL, AllocL> \
		operator __op__(const utf8_char_impl<CharL, TraitsL, AllocL>& l, \
						const basic_utf8_char_view<CharR, TraitsR>& r) { \
		typedef utf8_char_impl<CharL, TraitsL, AllocL> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename CharL, typename TraitsL, \
				typename CharR, typename TraitsR, typename AllocR> inline \
	typename basic_utf8_char_view<CharL, TraitsL>::char_type \
		operator __op__(const basic_utf8_char_view<CharL, TraitsL>& l, \
						const utf8_char_impl<CharR, TraitsR, AllocR>& r) { \
		typedef typename basic_utf8_char_view<CharL, TraitsL>::char_type ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	utf8_char_impl<Char, Traits, Alloc> \
		operator __op__(const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		typedef utf8_char_impl<Char, Traits, Alloc> ret_type; \
		ret_type ret(l); \
		ret __use_op__ r; \
		return ret; }

	// operator +
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( +, plus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( +, += )

	// operator-
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( -, minus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( -, -= )

	// operator*
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( *, multiplies_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( *, *= )

	// operator/
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( /, divides_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( /, /= )

	// operator%
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( %, modulus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( %, %= )

	// operator<<
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( <<, bitwise_left_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( <<, <<= )

	// operator>>
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL( >>, bitwise_right_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL( >>, >>= )

#undef YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_OF_NUMBER_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_ARITHMETIC_GENERAL_IMPL

// += -= *= /= %= &= |= ^=

#define YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( __op__ ) \
	template<typename T, typename Char, typename Traits, typename Alloc> inline \
	typename boost::disable_if<yggr::charset::is_string_t<T>, T&>::type \
		operator __op__(T& l, const utf8_char_impl<Char, Traits, Alloc>& r) { \
		l __op__ r.to_number(); \
		return l; }

	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( += )
	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( -= )
	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( *= )
	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( /= )
	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( %= )

	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( &= )
	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( |= )
	YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL( ^= )

#undef YGGR_TMP_PP_UTF8_CHAR_OPERATOR_SET_IMPL

// bitwise
// num __bitwise_op__ utf8_char -> num, utf8_char __bitwise_op__ num -> num
// utf8_char __bitwise_op__ utf8_char -> s32
// utf8_char __bitwise_op__ utf8_char_view_, utf8_char_view __bitwise_op__ utf8_char -> s32
// utf8_char __bitwise_op__ basic_utf8_char_reference, basic_utf8_char_reference __bitwise_op__ utf8_char->s32
// utf8_char __bitwise_op__ basic_utf8_char_const_reference, basic_utf8_char_const_reference __bitwise_op__ utf8_char ->s32


#define YGGR_TMP_PP_UTF8_CHAR_BITWISE_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, T r) { \
		return l.to_number() __op__ r; }

#define YGGR_TMP_PP_UTF8_CHAR_BITWISE_GENERAL_OPERATOR_IMPL( __op__ ) \
	template<typename CharL, typename TraitsL, typename AllocL, \
				typename CharR, typename TraitsR, typename AllocR> inline \
	s32 operator __op__(const utf8_char_impl<CharL, TraitsL, AllocL>& l, \
						const utf8_char_impl<CharR, TraitsR, AllocR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename CharL, typename TraitsL, typename AllocL, \
				typename CharR, typename TraitsR> inline \
	s32 operator __op__(const utf8_char_impl<CharL, TraitsL, AllocL>& l, \
						const basic_utf8_char_view<CharR, TraitsR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename CharL, typename TraitsL, \
				typename CharR, typename TraitsR, typename AllocR> inline \
	s32 operator __op__(const basic_utf8_char_view<CharL, TraitsL>& l, \
						const utf8_char_impl<CharR, TraitsR, AllocR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	s32 operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	s32 operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Alloc, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.to_number() __op__ r.to_number(); }

#define YGGR_TMP_PP_UTF8_CHAR_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( __op__, __self_ret_l__, __self_ret_r__ ) \
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.data() == r.data()? ( __self_ret_l__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		return l.data() == r.data()? ( __self_ret_l__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.data() == r.data()? ( __self_ret_l__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		return l.data() == r.data()? ( __self_ret_l__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.data() == r.data()? ( __self_ret_r__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
						const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		return l.data() == r.data()? ( __self_ret_l__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	s32 operator __op__(const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
						const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.data() == r.data()? ( __self_ret_r__ ) : ( l.to_number() __op__ r.to_number() ); }

	// operator&
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_OF_NUMBER_IMPL( &, bitwise_and_operator_result)
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_GENERAL_OPERATOR_IMPL( & )
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( &, l.to_number(), r.to_number() )

	// operator|
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_OF_NUMBER_IMPL( |, bitwise_or_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_GENERAL_OPERATOR_IMPL( | )
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( |, l.to_number(), r.to_number() )

	// operator^
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_OF_NUMBER_IMPL( ^, bitwise_xor_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_GENERAL_OPERATOR_IMPL( ^ )
	YGGR_TMP_PP_UTF8_CHAR_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( ^, 0, 0 )


#undef YGGR_TMP_PP_UTF8_CHAR_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_BITWISE_GENERAL_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_BITWISE_OF_NUMBER_IMPL

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

// ==, !=
#define YGGR_TMP_PP_UTF8_CHAR_CMP1( __op__ ) \
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, \
				template<typename _Char, typename _Traits> class BasicStringView > inline \
	bool operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, \
							const BasicStringView<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, \
				template<typename _Char, typename _Traits> class BasicStringView > inline \
	bool operator __op__(const BasicStringView<Char, Traits>& l, \
							const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, \
				typename Traits1, typename Alloc1, \
				typename Traits2, typename Alloc2 > inline \
	bool operator __op__(const utf8_char_impl<Char, Traits1, Alloc1>& l, \
							const utf8_char_impl<Char, Traits2, Alloc2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return r.compare(l) __op__ 0; }

// < <= > >=
#define YGGR_TMP_PP_UTF8_CHAR_CMP2(__op__, __rop__) \
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, \
				template<typename _Char, typename _Traits> class BasicStringView > inline \
	bool operator __op__(const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& l, \
							const BasicStringView<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char_uc, typename Traits_uc, typename Alloc_uc, \
				typename Char, typename Traits, \
				template<typename _Char, typename _Traits> class BasicStringView > inline \
	bool operator __op__(const BasicStringView<Char, Traits>& l, \
							const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, \
				typename Traits1, typename Alloc1, \
				typename Traits2, typename Alloc2 > inline \
	bool operator __op__(const utf8_char_impl<Char, Traits1, Alloc1>& l, \
							const utf8_char_impl<Char, Traits2, Alloc2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_const_reference< utf8_char_impl<Char, Traits, Alloc> >& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Traits, typename Alloc, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return !(r.compare(l) __rop__ 0); }

// ==, !=
	YGGR_TMP_PP_UTF8_CHAR_CMP1(==)
	YGGR_TMP_PP_UTF8_CHAR_CMP1(!=)

// <, <=, >, >=
	YGGR_TMP_PP_UTF8_CHAR_CMP2(<, <=)
	YGGR_TMP_PP_UTF8_CHAR_CMP2(<=, <)
	YGGR_TMP_PP_UTF8_CHAR_CMP2(>, >=)
	YGGR_TMP_PP_UTF8_CHAR_CMP2(>=, >)


#undef YGGR_TMP_PP_UTF8_CHAR_CMP1
#undef YGGR_TMP_PP_UTF8_CHAR_CMP2


} // namespace charset
} // naemspace yggr

// swap

namespace yggr
{
namespace charset
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline
void swap(typename utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>::base_type& l,
			utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r)
{
	r.swap(l);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline
void swap(utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l,
			typename utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>::base_type& r)
{
	l.swap(r);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline
void swap(utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l,
			utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r)
{
	l.swap(r);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T), typename A1, typename A2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			::yggr::charset::basic_string<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A1>,
			typename utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A2>::base_type
		>,
		void
	>::type
	swap(::yggr::charset::basic_string<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A1>& l,
			utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A2>& r)
{
	r.swap(l);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T), typename A1, typename A2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			typename utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A1>::base_type,
			::yggr::charset::basic_string<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A2>
		>,
		void
	>::type
	swap(utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A1>& l,
			::yggr::charset::basic_string<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A2>& r)
{
	l.swap(r);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T), typename A1, typename A2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			typename utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A1>::base_type,
			typename utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A2>::base_type
		>,
		void
	>::type
	swap(utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A1>& l,
			utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T), A2>& r)
{
	l.swap(r);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline
void swap(basic_utf8_char_reference< yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)> >& l,
			utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r)
{
	r.swap(l);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline
void swap(utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l,
			basic_utf8_char_reference< yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)> >& r)
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

// io

namespace yggr
{
namespace charset
{

template<typename Char, typename Traits,
			typename Char_uc, typename Traits_uc, typename Alloc_uc>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
					const utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits,
			typename Char_uc, typename Traits_uc, typename Alloc_uc>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
					BOOST_RV_REF_BEG utf8_char_impl<Char_uc, Traits_uc, Alloc_uc> BOOST_RV_REF_END right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Char_uc, typename Traits_uc, typename Alloc_uc>
std::basic_istream<Char, Traits>&
	operator>>(std::basic_istream<Char, Traits>& is,
				utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	yggr::charset::basic_string<Char> str;
	is >> str;
	right.assign(str);
	return is;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			typename Char_uc, typename Traits_uc, typename Alloc_uc,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right);
//
//template<typename Char, typename Traits,
//			typename Char_uc, typename Traits_uc, typename Alloc_uc,
//			template<typename _Char, typename _Traits> class Basic_IStream>
//Basic_IStream<Char, Traits>&
//	operator>>(Basic_IStream<Char, Traits>& is,
//				yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right);

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc > inline
bool is_validate_char(const utf8_char_impl<Char, Traits, Alloc>& c)
{
	return c.is_validate();
}

template<typename Char, typename Traits, typename Alloc > inline
bool is_validate_string(const utf8_char_impl<Char, Traits, Alloc>& c)
{
	return false;
}

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

// string_charset_helper_data
template<typename Char, typename Traits, typename Alloc> inline
const utf8_char_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str;
}

template<typename Char, typename Traits, typename Alloc> inline
const utf8_char_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const utf8_char_impl<Char, Traits, Alloc>& str, const string& charset_name)
{
	assert(charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return str;
}

template<typename Char, typename Traits, typename Alloc, typename Charset_Name > inline
const utf8_char_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const string_charset_helper<utf8_char_impl<Char, Traits, Alloc>&, Charset_Name>& val)
{
	assert(val.second == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return val.first;
}

template<typename Char, typename Traits, typename Alloc, typename Charset_Name > inline
const utf8_char_impl<Char, Traits, Alloc>&
	string_charset_helper_data(const string_charset_helper<const utf8_char_impl<Char, Traits, Alloc>&, Charset_Name>& val)
{
	assert(val.second == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return val.first;
}

} // namespace charset
} // namespace yggr

// boost range_size support
namespace yggr
{
namespace charset
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, utf8_char_impl)

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
struct byte_size_tpl_container_helper< charset::utf8_char_impl<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
public:
	typedef charset::utf8_char_impl<Char, Traits, Alloc> value_type;

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

template<typename Char, typename Traits, typename Alloc> inline
std::size_t hash_value(const ::yggr::charset::utf8_char_impl<Char, Traits, Alloc>& ch)
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

template<typename Char, typename Traits, typename Alloc>
struct hash< ::yggr::charset::utf8_char_impl<Char, Traits, Alloc> >
{
	typedef ::yggr::charset::utf8_char_impl<Char, Traits, Alloc> type;

	inline std::size_t operator()(const type& ch) const
	{
		return ch.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

// utf8_string_constructable
namespace yggr
{
namespace charset
{

namespace detail
{

//utf8_char_constructable_helper
template<typename T>
struct utf8_char_constructable_helper
	: public
		boost::mpl::or_
		<
			is_can_be_conv_into_char<T>,
			is_basic_string_t<T>
		>::type
{
};

template<typename T, std::size_t N>
struct utf8_char_constructable_helper<T[N]>
	: public boost::mpl::true_
{
};

template<typename T>
struct utf8_char_constructable_helper<T*>
	: public is_native_char<typename boost::remove_cv<typename boost::remove_pointer<T>::type>::type>::type
{
};

template<typename T1, typename T2>
struct utf8_char_constructable_helper<string_charset_helper< T1, T2 > >
	: public boost::mpl::true_
{
};

// utf8_char_loadable_helper
template<typename T>
struct utf8_char_loadable_helper
	: public  is_basic_string_t<T>::type
{
};

template<typename T1, typename T2>
struct utf8_char_loadable_helper<string_charset_helper< T1, T2 > >
	: public
		boost::mpl::and_
		<
			boost::mpl::not_< boost::is_const<T1> >,
			utf8_char_loadable_helper<typename boost::remove_reference<T1>::type>
		>::type
{
};

} // namespace detail

template<typename T>
struct utf8_char_constructable
	: public
		boost::mpl::if_
		<
			is_string_charset_helper_ex<T>,
			typename
				detail::utf8_char_constructable_helper
				<
					typename
						boost::remove_cv
						<
							typename boost::remove_reference<T>::type
						>::type
				>::type,
			typename detail::utf8_char_constructable_helper<T>::type
		>::type

{
};

template<typename T>
struct utf8_char_loadable
	: public
		boost::mpl::if_
		<
			is_string_charset_helper_ex<T>,
			typename
				detail::utf8_char_loadable_helper
				<
					typename
						boost::remove_cv
						<
							typename boost::remove_reference<T>::type
						>::type
				>::type,
			typename detail::utf8_char_loadable_helper<T>::type
		>::type
{
};

} // namespace charset
} // namesapce yggr

#endif // __YGGR_CHARSET_UTF8_CHAR_IMPL_HPP__

