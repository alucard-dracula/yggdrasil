// utf8_char_reference_impl.hpp

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

#ifndef __YGGR_CHARSET_UTF8_CHAR_REFERENCE_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_CHAR_REFERENCE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/support/operator_t.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/type_traits/is_convertible_size_t.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <yggr/bytes/detail/byte_size_detail_basic.hpp>
#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/is_isomeric_same_string.hpp>
#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/utf8_foo.hpp>

#include <yggr/charset/utf8_string_impl_decl.hpp>
#include <yggr/charset/utf8_string_view_decl.hpp>
#include <yggr/charset/utf8_string_iterator_decl.hpp>

#include <yggr/charset/utf8_char_impl_decl.hpp>
#include <yggr/charset/utf8_char_view_decl.hpp>
#include <yggr/charset/utf8_char_reference_decl.hpp>

#include <boost/ref.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>

#include <boost/functional/hash/hash.hpp>

#include <iostream>

YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::basic_utf8_char_reference, 1)
YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::basic_utf8_char_const_reference, 1)

// charset_name_t
namespace yggr
{
namespace charset
{

// is_utf8_char_reference
template<typename Utf8String>
struct is_utf8_char_reference_t< basic_utf8_char_reference<Utf8String> >
	: public boost::mpl::true_
{
};

// is_utf8_char_const_reference
template<typename Utf8String>
struct is_utf8_char_const_reference_t< basic_utf8_char_const_reference<Utf8String> >
	: public boost::mpl::true_
{
};

template<typename Utf8String>
struct charset_name_t< basic_utf8_char_reference< Utf8String > >
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef typename Utf8String::char_type char_type;
	typedef typename boost::remove_cv<typename Utf8String::value_type>::type native_char_type;

	typedef
		utf8_string_impl
		<
			typename Utf8String::value_type,
			typename Utf8String::traits_type,
			typename mplex::typename_expand_get<Utf8String, 2>::type
		> string_type;

	typedef
		basic_utf8_string_view
		<
			typename Utf8String::value_type,
			typename Utf8String::traits_type
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

template<typename Utf8String>
struct charset_name_t< basic_utf8_char_const_reference< Utf8String > >
	:	private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef typename Utf8String::char_type char_type;
	typedef typename boost::remove_cv<typename Utf8String::value_type>::type native_char_type;
	typedef
		utf8_string_impl
		<
			typename Utf8String::value_type,
			typename Utf8String::traits_type,
			typename mplex::typename_expand_get<Utf8String, 2>::type
		> string_type;

	typedef
		basic_utf8_string_view
		<
			typename Utf8String::value_type,
			typename Utf8String::traits_type
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
				>::value >
struct utf8_char_ref_iterator_assign_helper;

template<typename Iter >
struct utf8_char_ref_iterator_assign_helper<Iter, true >
{
public:
	typedef Iter iter_type;

private:
	typedef utf8_char_ref_iterator_assign_helper this_type;

public:
	template<typename Utf8CharRef> inline
	Utf8CharRef& operator()(Utf8CharRef& dst, iter_type s, iter_type e,
								const string& src_charset_name) const
	{
		dst.assign(&(*s), std::distance(s, e), src_charset_name);
		return dst;
	}
};

template<typename Iter >
struct utf8_char_ref_iterator_assign_helper<Iter, false >
{
public:
	typedef Iter iter_type;

private:
	typedef utf8_char_ref_iterator_assign_helper this_type;

public:
	template<typename Utf8CharRef> inline
	Utf8CharRef& operator()(Utf8CharRef& dst, iter_type s, iter_type e,
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

		return
			src.empty()?
				dst.assign(1, static_cast<char>(0))
				: dst.assign(&src[0], src.size(), src_charset_name);
	}
};

// utf8_char_ref_const_cast_helper
template<typename T >
class utf8_char_ref_const_cast_helper;

template<typename Utf8String >
class utf8_char_ref_const_cast_helper< basic_utf8_char_const_reference<Utf8String> >
{
public:
	typedef Utf8String utf8_string_type;
	typedef basic_utf8_char_reference<utf8_string_type> utf8_char_reference_type;
	typedef basic_utf8_char_const_reference<utf8_string_type> result_type;

private:
	typedef utf8_char_ref_const_cast_helper this_type;

public:
	inline result_type operator()(const utf8_char_reference_type& val) const
	{
		return result_type(val._str_ref, val._pos);
	}
};

template<typename Utf8String >
class utf8_char_ref_const_cast_helper< basic_utf8_char_reference<Utf8String> >
{
public:
	typedef Utf8String utf8_string_type;
	typedef basic_utf8_char_reference<utf8_string_type> result_type;

private:
	typedef utf8_char_ref_const_cast_helper this_type;

public:
	inline result_type operator()(const result_type& val) const
	{
		return result_type(val);
	}
};

template<typename Char, typename Traits >
class utf8_char_ref_const_cast_helper< basic_utf8_char_view<Char, Traits> >
{
public:
	typedef basic_utf8_char_view<Char, Traits> result_type;

private:
	typedef utf8_char_ref_const_cast_helper this_type;

public:
	template<typename Utf8String> inline
	result_type operator()(const basic_utf8_char_reference<Utf8String>& val) const
	{
		return result_type(val.data());
	}
};

// org_str_helper
template<typename BasicString, bool is_utf8_char>
class utf8_char_ref_org_str_helper;

template<typename BasicString>
class utf8_char_ref_org_str_helper<BasicString, true>
	: public boost::mpl::bool_<true>
{
public:
	typedef BasicString base_string_type;
	typedef base_string_type& nonconst_result_type;
	typedef const base_string_type& const_result_type;

private:
	typedef utf8_char_ref_org_str_helper this_type;

public:
	// non const version
	template<typename Utf8String> inline
	nonconst_result_type org_str(basic_utf8_char_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_string_type>,
			nonconst_result_type
		>::type
		org_str(basic_utf8_char_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String> inline
	typename
		boost::enable_if
		<
			::yggr::charset::is_isomeric_same_string<String, base_string_type>,
			String&
		>::type
		org_str(basic_utf8_char_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return reinterpret_cast<String&>(obj._str_ref.org_str());
	}

	template<typename String, typename Utf8String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_string_type>,
			nonconst_result_type
		>::type
		org_str(basic_utf8_char_reference<Utf8String>& obj, const Alloc&) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String, typename Alloc> inline
	typename
		boost::enable_if
		<
			::yggr::charset::is_isomeric_same_string<String, base_string_type>,
			String&
		>::type
		org_str(basic_utf8_char_reference<Utf8String>& obj, const Alloc&) const
	{
		assert(0 == obj._pos);
		return reinterpret_cast<String&>(obj._str_ref.org_str());
	}

	// const version
	template<typename Utf8String> inline
	const_result_type org_str(const basic_utf8_char_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_string_type>,
			const_result_type
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String> inline
	typename
		boost::enable_if
		<
			::yggr::charset::is_isomeric_same_string<String, base_string_type>,
			const String&
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return reinterpret_cast<const String&>(obj._str_ref.org_str());
	}

	template<typename String, typename Utf8String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_string_type>,
			const_result_type
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj, const Alloc&) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String, typename Alloc> inline
	typename
		boost::enable_if
		<
			::yggr::charset::is_isomeric_same_string<String, base_string_type>,
			const String&
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj, const Alloc&) const
	{
		assert(0 == obj._pos);
		return reinterpret_cast<const String&>(obj._str_ref.org_str());
	}

	template<typename String, typename Utf8String>
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_string_type>
				>,
				is_basic_string_t<String>
			>,
			String
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj) const
	{
		typedef Utf8String now_utf8_string_type;
		typedef typename boost::range_value<now_utf8_string_type>::type now_utf8_string_value_type;
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;
		assert(0 == obj._pos);

		const now_utf8_string_value_type* p = obj.data();
		size_type byte_size = utf8_foo::charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		memcpy(&str[0], p, byte_size);

		return str;
	}

	template<typename String, typename Utf8String, typename Alloc>
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_string_type>
				>,
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj, const Alloc& alloc) const
	{
		typedef Utf8String now_utf8_string_type;
		typedef typename boost::range_value<now_utf8_string_type>::type now_utf8_string_value_type;
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;
		assert(0 == obj._pos);

		const now_utf8_string_value_type* p = obj.data();
		size_type byte_size = utf8_foo::charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		memcpy(&str[0], p, byte_size);
	}

};

template<typename BasicString>
class utf8_char_ref_org_str_helper<BasicString, false>
	: public boost::mpl::bool_<false>
{
public:
	typedef BasicString base_string_type;
	typedef base_string_type nonconst_result_type;
	typedef base_string_type const_result_type;

private:
	typedef utf8_char_ref_org_str_helper this_type;

public:
	template<typename Utf8String> inline
	const_result_type org_str(const basic_utf8_char_reference<Utf8String>& obj)
	{
		typedef Utf8String now_utf8_string_type;
		typedef typename boost::range_value<now_utf8_string_type>::type now_utf8_string_value_type;

		const now_utf8_string_value_type* p = obj.data();
		return const_result_type(p, utf8_foo::charlen(p), obj.get_allocator());
	}

	template<typename String, typename Utf8String>
	typename
		boost::enable_if
		<
			is_basic_string_t<String>,
			String
		>::type
		org_str(const basic_utf8_char_reference<Utf8String>& obj) const
	{
		typedef Utf8String now_utf8_string_type;
		typedef typename boost::range_value<now_utf8_string_type>::type now_utf8_string_value_type;

		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		const now_utf8_string_value_type* p = obj.data();
		size_type byte_size = utf8_charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		memcpy(&str[0], p, byte_size);

		return str;
	}

	template<typename String, typename Utf8String, typename Alloc>
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
		org_str(const basic_utf8_char_reference<Utf8String>& obj, const Alloc& alloc) const
	{
		typedef Utf8String now_utf8_string_type;
		typedef typename boost::range_value<now_utf8_string_type>::type now_utf8_string_value_type;

		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		const now_utf8_string_value_type* p = obj.data();
		size_type byte_size = utf8_charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		memcpy(&str[0], p, byte_size);

		return str;
	}

};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

//------------------basic_utf8_char_reference----------------

template<typename Utf8String>
class basic_utf8_char_reference
{
private:
	template<typename OUtf8_String>
	friend class basic_utf8_char_reference;

	template<typename OUtf8_String>
	friend class basic_utf8_char_const_reference;

	template<typename Char, typename Traits, typename Alloc>
	friend class utf8_string_impl;

	template<typename Char, typename Traits, typename Alloc>
	friend class utf8_char_impl;

	YGGR_PP_FRIEND_TYPENAME(Utf8String);

	BOOST_MPL_ASSERT((
		boost::mpl::or_
		<
			is_utf8_char_t<Utf8String>,
			is_utf8_string_t<Utf8String>
		> ));

public:
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type base_type;

	typedef typename utf8_string_type::char_type char_type;
	typedef typename utf8_string_type::value_type value_type;
	typedef typename utf8_string_type::size_type size_type;

	typedef typename utf8_string_type::base_type base_string_type;
	typedef typename base_string_type::iterator iterator;
	typedef typename base_string_type::const_iterator const_iterator;

	typedef typename base_string_type::pointer pointer;
	typedef typename base_string_type::const_pointer const_pointer;

	typedef typename base_string_type::traits_type traits_type;
	typedef typename base_string_type::allocator_type allocator_type;

	typedef utf8_string_iterator<char_type, iterator> utf8_iterator;
	typedef utf8_string_const_iterator<char_type, const_iterator> utf8_const_iterator;

	typedef
		basic_utf8_char_view
		<
			typename char_type::value_type,
			typename char_type::traits_type
		> char_view_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef basic_utf8_char_reference this_type;
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:
	basic_utf8_char_reference(BOOST_RV_REF(utf8_string_type) str_ref)
		: _str_ref(str_ref), _pos(size_type(0))
	{
		assert(0 < str_ref.length());
	}

	basic_utf8_char_reference(utf8_string_type& str_ref)
		: _str_ref(str_ref), _pos(size_type(0))
	{
		assert(0 < str_ref.length());
	}

	template<typename Size> // fix conflict
	basic_utf8_char_reference(utf8_string_type& str_ref, Size pos,
								typename
									boost::enable_if
									<
										is_convertible_size_t<Size>,
										sfinae_type
									>::type sfinae = 0)
		: _str_ref(str_ref), _pos(pos)
	{
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_reference(utf8_string_type& str_ref, utf8_iterator iter)
		: _str_ref(str_ref), _pos(std::distance(utf8_iterator(str_ref.begin()), iter))
	{
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_reference(utf8_string_type& str_ref, utf8_const_iterator iter)
		: _str_ref(str_ref), _pos(std::distance(utf8_const_iterator(str_ref.begin()), iter))
	{
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_reference(utf8_string_type& str_ref, iterator iter)
		: _str_ref(str_ref),
			_pos(std::distance(utf8_iterator(str_ref.begin()),
								utf8_iterator(iter)))
	{
		assert(utf8_foo::charlen(iter));
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_reference(utf8_string_type& str_ref, const_iterator iter)
		: _str_ref(str_ref),
			_pos(std::distance(utf8_const_iterator(str_ref.base_type::begin()),
								utf8_const_iterator(iter)))
	{
		assert(utf8_foo::charlen(iter));
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_reference(BOOST_RV_REF(this_type) right)
		: _str_ref(static_cast<utf8_string_type&>(right._str_ref)),
			_pos(right._pos)
	{
	}

	basic_utf8_char_reference(const this_type& right)
		: _str_ref(right._str_ref), _pos(right._pos)
	{
	}

	~basic_utf8_char_reference(void)
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

		return
			this_type::assign(
				static_cast<now_char_type>(c),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) );
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

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator=(const utf8_char_impl<Char, Traits, Alloc>& other)
	{
		return this_type::assign(other);
	}

	template<typename OUtf8_String> inline
	this_type& operator=(const basic_utf8_char_reference<OUtf8_String>& right)
	{
		return this_type::assign(right);
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		return this_type::assign(right_cref);
	}

	inline this_type& operator=(const this_type& right)
	{
		return this_type::assign(right);
	}

	template<typename OUtf8_String> inline
	this_type& operator=(const basic_utf8_char_const_reference<OUtf8_String>& right)
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
		swap(Basic_String<Char, Traits, Alloc>& right, const string& src_charset_name)
	{
		typedef Basic_String<Char, Traits, Alloc> right_type;

		char_type ltmp(right, src_charset_name);

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		right_type rtmp = charset_foo::s_xchg<right_type>(
							s, e, right.get_allocator(),
							YGGR_STR_INNER_STRING_CHARSET_NAME(),
							src_charset_name);



		basic_string_replace(_str_ref.org_str(), s, e, ltmp.data(), ltmp.size());
		right.swap(rtmp);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void swap(BOOST_RV_REF_BEG utf8_char_impl<Char, Traits, Alloc> BOOST_RV_REF_END right)
	{
		typedef utf8_char_impl<Char, Traits, Alloc> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Char, typename Traits, typename Alloc>
	void swap(utf8_char_impl<Char, Traits, Alloc>& right)
	{
		typedef utf8_char_impl<Char, Traits, Alloc> right_type;
		if(_str_ref.data() == right.data())
		{
			return;
		}

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		right_type tmp(s, e, right.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());

		basic_string_replace(_str_ref.org_str(),s, e, right.data(), right.size());

		right.swap(tmp);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void swap(BOOST_RV_REF_BEG utf8_char_impl<Char, Traits, Alloc> BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef utf8_char_impl<Char, Traits, Alloc> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	void swap(utf8_char_impl<Char, Traits, Alloc>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::swap(right);
	}

	template<typename OUtf8_String> inline
	void swap(BOOST_RV_REF_BEG basic_utf8_char_reference<OUtf8_String> BOOST_RV_REF_END right)
	{
		typedef basic_utf8_char_reference<OUtf8_String> right_type;

		right_type& right_ref = right;
		this_type::swap<OUtf8_String>(right_ref);
	}

	template<typename OUtf8_String>
	void swap(basic_utf8_char_reference<OUtf8_String>& right)
	{
		typedef basic_utf8_char_reference<OUtf8_String> right_type;
		typedef typename right_type::iterator right_iter_type;

		if(_str_ref.data() == right._str_ref.data()
			&& _pos == right._pos)
		{
			return;
		}

		iterator ls = this_type::begin();
		iterator le = ls + utf8_foo::charlen(ls);

		iterator rs = right.begin();
		iterator re = rs + utf8_foo::charlen(rs);

		char_type tmp(ls, le, YGGR_STR_INNER_STRING_CHARSET_NAME());
		basic_string_replace(_str_ref.org_str(), ls, le, rs, re);

		try
		{
			basic_string_replace(right._str_ref.org_str(), rs, re, tmp.data(), tmp.size());
		}
		catch(const stl_exception& e)
		{
			ls = this_type::begin();
			le = ls + utf8_foo::charlen(ls);
			basic_string_replace(_str_ref.org_str(), ls, le, tmp.data(), tmp.size());
			throw e;
		}
	}

	template<typename OUtf8_String> inline
	void swap(BOOST_RV_REF_BEG basic_utf8_char_reference<OUtf8_String> BOOST_RV_REF_END right,
				const string& src_charset_name)
	{
		typedef basic_utf8_char_reference<OUtf8_String> right_type;

		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		right_type& right_ref = right;
		this_type::swap<OUtf8_String>(right_ref);
	}

	template<typename OUtf8_String> inline
	void swap(basic_utf8_char_reference<OUtf8_String>& right,
				const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		this_type::swap<OUtf8_String>(right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap<utf8_string_type>(right_ref);
	}

	inline void swap(this_type& right)
	{
		this_type::swap<utf8_string_type>(right);
	}

	inline void swap(BOOST_RV_REF(this_type) right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		this_type& right_ref = right;
		this_type::swap<utf8_string_type>(right_ref);
	}

	template<typename OUtf8_String> inline
	void swap(this_type& right, const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		this_type::swap<utf8_string_type>(right);
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
	inline value_type* operator&(void)
	{
		return &(*(this_type::begin()));
	}

	inline const value_type* operator&(void) const
	{
		return &(*(this_type::cbegin()));
	}

	//operator type
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		assert(_pos < _str_ref.length());
		const_iterator s = this_type::begin();
		return (1 < utf8_foo::charlen(s)) || (*s);
	}

#if !(defined(__clang__) && (__clang__))

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_type));
	}

#else

	template<typename Char, typename Traits, typename Alloc> inline
	operator std::basic_string<Char, Traits, Alloc>(void) const
	{
		typedef std::basic_string<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_type));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	operator boost::container::basic_string<Char, Traits, Alloc>(void) const
	{
		typedef boost::container::basic_string<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_type));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	operator utf8_string_impl<Char, Traits, Alloc>(void) const
	{
		typedef utf8_string_impl<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_type));
	}

#endif // #if !(defined(__clang__) && (__clang__))

#if !(defined(__clang__) && (__clang__))
	inline operator char_type(void) const
	{
		const_iterator s = this_type::begin();
		const_iterator e = s + utf8_foo::charlen(s);
		return char_type(s, e, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}
#endif // #if !(defined(__clang__) && (__clang__))

#if !(defined(__clang__) && (__clang__))
	inline operator basic_utf8_char_const_reference<utf8_string_type>(void) const
	{
		return basic_utf8_char_reference<utf8_string_type>(_str_ref, _pos);
	}
#endif // #if !(defined(__clang__) && (__clang__))

	template<typename Char, typename Traits> inline
	operator basic_utf8_char_view<Char, Traits>(void) const
	{
		return basic_utf8_char_view<Char, Traits>(this_type::data());
	}

	inline operator char_view_type(void) const
	{
		return char_view_type(this_type::data());
	}

#define YGGR_TMP_PP_OPERATOR_T( __type__ ) \
	inline operator __type__(void) const { \
		return static_cast< __type__ >(utf8_foo::char_to_number(this_type::data())); }

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

private:
	template<typename T>
	friend class detail::utf8_char_ref_const_cast_helper;

public:
	inline basic_utf8_char_const_reference<utf8_string_type>
		const_cast_(void) const
	{
		typedef basic_utf8_char_const_reference<utf8_string_type> ret_type;
		return ret_type(_str_ref, _pos);
	}

	template<typename ConstRef> inline
	typename detail::utf8_char_ref_const_cast_helper<ConstRef>::result_type
		const_cast_(void) const
	{
		typedef detail::utf8_char_ref_const_cast_helper<ConstRef> h_type;

		h_type h;
		return h(*this);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE value_type* data(void)
	{
		return const_cast<value_type*>(&(*(this_type::cbegin())));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* data(void) const
	{
		return &(*(this_type::cbegin()));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* c_str(void) const
	{
		return &(*(this_type::cbegin()));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type size(void) const
	{
		return utf8_foo::charlen(this_type::begin());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type length(void) const
	{
		assert(this_type::is_validate());
		return 1;
	}

	inline bool is_validate(void) const
	{
		return utf8_foo::charlen(this_type::begin());
	}

	inline iterator begin(void)
	{
		return (utf8_iterator(_str_ref.begin()) + _pos).org();
	}

	inline const_iterator begin(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + _pos).org();
	}

	inline const_iterator cbegin(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + _pos).org();
	}

	inline iterator end(void)
	{
		return (utf8_iterator(_str_ref.begin()) + (_pos + 1)).org();
	}

	inline const_iterator end(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + (_pos + 1)).org();
	}

	inline const_iterator cend(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + (_pos + 1)).org();
	}

	inline size_type index(void) const
	{
		return _pos;
	}

	template<typename OUtf8_String> inline
	bool is_same_address(const basic_utf8_char_reference<OUtf8_String>& right) const
	{
		return false;
	}

	inline bool is_same_address(const this_type& right) const
	{
		return boost::addressof(_str_ref) == boost::addressof(right._str_ref)
				&& _pos == right._pos;
	}

	template<typename OUtf8_String> inline
	bool is_same_address(const basic_utf8_char_const_reference<OUtf8_String>& right) const
	{
		return false;
	}

	inline bool is_same_address(const basic_utf8_char_const_reference<utf8_string_type>& right) const
	{
		return boost::addressof(_str_ref) == boost::addressof(right._str_ref)
				&& _pos == right._pos;
	}

	template<typename Char, typename Traits> inline
	bool is_same_address(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return this_type::data() == right.data();
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::is_same<utf8_char_impl<Char, Traits, Alloc>, utf8_string_type>,
			bool
		>::type
		is_same_address(const utf8_char_impl<Char, Traits, Alloc>& right) const
	{
		return false;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::is_same<utf8_char_impl<Char, Traits, Alloc>, utf8_string_type>,
			bool
		>::type
		is_same_address(const utf8_char_impl<Char, Traits, Alloc>& right) const
	{
		assert(_pos == 0);
		return _str_ref.data() == right.data();
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
			iterator s = this_type::begin();
			iterator e = s + utf8_foo::charlen(s);

			basic_string_replace(_str_ref.org_str(), s, e, 1, static_cast<value_type>(c));
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

	template<typename Char> inline
	typename
		boost::enable_if
		<
			is_native_char<Char>,
			this_type&
		>::type
		assign(const Char* src, const string& src_charset_name)
	{
		assert(src);

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		if(reinterpret_cast<const value_type*>(src) == &(*s))
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return *this;
		}

		if(!(src && src[0]))
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
		}
		else
		{
			base_string_type right(_str_ref.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			basic_string_replace(_str_ref.org_str(), s, e, right.begin(), right.end());
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

	template<typename Char> inline
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
			iterator s = this_type::begin();
			iterator e = s + utf8_foo::charlen(s);

			base_string_type right(_str_ref.get_allocator());
			charset_foo::s_xchg(right, src, n, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			basic_string_replace(_str_ref.org_str(), s, e, right.begin(), right.end());
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
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		if(reinterpret_cast<const value_type*>(src.data()) == &(*s))
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return *this;
		}

		if(src.empty() || src[0] == 0)
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
		}
		else
		{
			base_string_type right(_str_ref.get_allocator());
			charset_foo::s_xchg(right, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert( 1 == utf8_foo::strlen(right));
			basic_string_replace(_str_ref.org_str(), s, e, right.begin(), right.end());
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
		typedef detail::utf8_char_ref_iterator_assign_helper<iter_type> h_type;
		typedef typename boost::iterator_value<InputIter>::type cv_now_char_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_now_char_type) now_char_type;

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
		typedef detail::utf8_char_ref_iterator_assign_helper<iter_type> h_type;

		h_type h;
		return h(*this, start, last, src_charset_name);
	}

	// using "src_charset_name" version call non-src_charset_name version
	// because in utf8_string iterator the "src_charset_name" version is used more frequently
	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last)
	{
		assert((std::distance(start, last) == 1));

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		basic_string_replace(_str_ref.org_str(), s, e, start.org(), last.org());

		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_iterator<Char, Basic_Iter> start,
						utf8_string_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(start, last);
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last)
	{
		assert((std::distance(start, last) == 1));

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		basic_string_replace(_str_ref.org_str(), s, e, start.org(), last.org());
		return *this;
	}

	template<typename Char, typename Basic_Iter> inline
	this_type& assign(utf8_string_const_iterator<Char, Basic_Iter> start,
						utf8_string_const_iterator<Char, Basic_Iter> last,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(start, last);
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

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		basic_string_replace(_str_ref.org_str(), s, e, &(*start), utf8_foo::charlen(&(*start)));
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
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(start, last);
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

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		basic_string_replace(_str_ref.org_str(), s, e, &(*start), utf8_foo::charlen(&(*start)));
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
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(start, last);
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

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_char_impl<Char, Traits, Alloc>& other)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		basic_string_replace(_str_ref.org_str(), s, e, other.begin(), other.end());
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& assign(const utf8_char_impl<Char, Traits, Alloc>& other,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(other);
	}

	template<typename OUtf8_String> inline
	this_type& assign(const basic_utf8_char_reference<OUtf8_String>& right)
	{
		typedef basic_utf8_char_reference<OUtf8_String> right_type;
		typedef typename right_type::const_iterator right_citer_type;

		if(_str_ref.data() == right._str_ref.data()
			&& _pos == right._pos)
		{
			return *this;
		}

		iterator ls = this_type::begin();
		iterator le = ls + utf8_foo::charlen(ls);

		right_citer_type rs = right.begin();
		right_citer_type re = rs + utf8_foo::charlen(rs);

		basic_string_replace(_str_ref.org_str(), ls, le, rs, re);

		return *this;
	}

	template<typename OUtf8_String> inline
	this_type& assign(const basic_utf8_char_reference<OUtf8_String>& right,
						const string& src_chaset_name)
	{
		assert(src_chaset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

	template<typename OUtf8_String> inline
	this_type& assign(const basic_utf8_char_const_reference<OUtf8_String>& right)
	{
		typedef basic_utf8_char_const_reference<OUtf8_String> right_type;
		typedef typename right_type::const_iterator right_citer_type;

		if(_str_ref.data() == right._str_ref.data()
			&& _pos == right._pos)
		{
			return *this;
		}

		iterator ls = this_type::begin();
		iterator le = ls + utf8_foo::charlen(ls);

		right_citer_type rs = right.begin();
		right_citer_type re = rs + utf8_foo::charlen(rs);

		basic_string_replace(_str_ref.org_str(), ls, le, rs, re);
		return *this;
	}

	template<typename OUtf8_String> inline
	this_type& assign(const basic_utf8_char_const_reference<OUtf8_String>& right,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

	template<typename Char, typename Traits> inline
	this_type& assign(const basic_utf8_char_view<Char, Traits>& right)
	{
		typedef basic_utf8_char_view<Char, Traits> right_type;
		typedef typename right_type::const_iterator right_citer_type;


		iterator ls = this_type::begin();
		if(&(*ls) == right.data())
		{
			return *this;
		}

		iterator le = ls + utf8_foo::charlen(ls);

		right_citer_type rs = right.data();
		right_citer_type re = rs + utf8_foo::charlen(rs);

		basic_string_replace(_str_ref.org_str(), ls, le, rs, re);
		return *this;
	}

	template<typename Char, typename Traits> inline
	this_type& assign(const basic_utf8_char_view<Char, Traits>& right,
						const string& src_charset_name)
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return this_type::assign(right);
	}

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
					this_type::get_allocator(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) ),
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
				utf8_foo::charcmp(this_type::data(), static_cast<now_char_type>(c))
				: this_type::compare(
					char_type(static_cast<now_char_type>(c), this_type::get_allocator(), src_charset_name),
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
				char_type(
					str, n,
					this_type::get_allocator(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char) ),
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
				utf8_foo::charcmp(this_type::data(), src)
				: this_type::compare(
					char_type(src, n, this_type::get_allocator(), src_charset_name),
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
			base_string_type tmp_src(_str_ref.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		const_iterator s = this_type::begin();

		if(reinterpret_cast<const value_type*>(src) == &(*s))
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}
		else
		{
			return src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
						utf8_foo::charcmp(s, src)
						: utf8_foo::charcmp(s,
											charset_foo::s_xchg_diff_charset<base_string_type>(
												src, src_charset_name,
												YGGR_STR_INNER_STRING_CHARSET_NAME()));
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
			base_string_type tmp_src(_str_ref.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		const_iterator s = this_type::begin();

		if(reinterpret_cast<const value_type*>(src.data()) == &(*s))
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}
		else
		{
			return src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
						utf8_foo::charcmp(s, src.data())
						: utf8_foo::charcmp(s,
											charset_foo::s_xchg_diff_charset<base_string_type>(
												src, src_charset_name,
												YGGR_STR_INNER_STRING_CHARSET_NAME()));
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other) const
	{
		return utf8_foo::charcmp(this_type::data(), other.data());
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
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_reference<OUtf8_String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_const_reference<OUtf8_String>& right) const
	{
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_const_reference<OUtf8_String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), right.data());
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
		return _str_ref.get_allocator();
	}

	// org_str
private:
	template<typename BasicString, bool is_utf8_char>
	friend class detail::utf8_char_ref_org_str_helper;

	typedef
		detail::utf8_char_ref_org_str_helper
		<
			base_string_type,
			is_utf8_char_t<utf8_string_type>::value
		> org_str_helper_type;

public:
	inline typename org_str_helper_type::nonconst_result_type
		org_str(void)
	{
		org_str_helper_type h;
		return h.org_str(*this);
	}

	inline typename org_str_helper_type::nonconst_result_type
		org_str(const allocator_type&)
	{
		org_str_helper_type h;
		return h.org_str(*this);
	}

	inline typename org_str_helper_type::const_result_type
		org_str(void) const
	{
		org_str_helper_type h;
		return h.org_str(*this);
	}

	inline typename org_str_helper_type::const_result_type
		org_str(const allocator_type&) const
	{
		org_str_helper_type h;
		return h.org_str(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<base_string_type, String>,
			typename org_str_helper_type::nonconst_result_type
		>::type
		org_str(void)
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_isomeric_same_string<base_string_type, String>,
				org_str_helper_type
			>,
			String&
		>::type
		org_str(void)
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<base_string_type, String>,
				container_ex::is_allocator<Alloc>
			>,
			typename org_str_helper_type::nonconst_result_type
		>::type
		org_str(const Alloc& alloc)
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_isomeric_same_string<base_string_type, String>,
				org_str_helper_type,
				container_ex::is_allocator<Alloc>
			>,
			String&
		>::type
		org_str(const Alloc& alloc)
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<base_string_type, String>,
			typename org_str_helper_type::const_result_type
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_isomeric_same_string<base_string_type, String>,
				org_str_helper_type
			>,
			const String&
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<base_string_type, String>,
				container_ex::is_allocator<Alloc>
			>,
			typename org_str_helper_type::const_result_type
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_isomeric_same_string<base_string_type, String>,
				org_str_helper_type,
				container_ex::is_allocator<Alloc>
			>,
			const String&
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_string_type>
				>,
				is_basic_string_t<String>
			>,
			String
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_string_type>
				>,
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	// str

	inline utf8_string_type str(void) const
	{
		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								_str_ref.get_allocator(),
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const allocator_type& alloc) const
	{
		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								_str_ref.get_allocator(),
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const allocator_type& alloc, const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return
			charset_foo::s_xchg<ret_string_type>(
				p, utf8_foo::charlen(p),
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String) );
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

		const value_type* p = this_type::data();
		assert(p);
		return
			charset_foo::s_xchg<ret_string_type>(
				p, utf8_foo::charlen(p), alloc,
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

		const value_type* p = this_type::data();
		assert(p);
		return
			charset_foo::s_xchg<ret_string_type>(
				p, utf8_foo::charlen(p),
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

		const value_type* p = this_type::data();
		assert(p);
		return charset_foo::s_xchg<ret_string_type>(
					p, utf8_foo::charlen(p), alloc,
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					dst_charset_name);
	}

	// view
	inline char_view_type view(void) const
	{
		return char_view_type(this_type::data());
	}

	template<typename View> inline
	View view(void) const
	{
		return View(this_type::data());
	}

	// arithmetic
	inline s32 to_number(void) const
	{
		return utf8_foo::char_to_number(this_type::data());
	}

	inline char_type operator+(void) const
	{
		const_iterator s = this_type::begin();
		const_iterator e = s + utf8_foo::charlen(s);
		return char_type(s, e, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline char_type operator-(void) const
	{
		const value_type* ptr = this_type::data();
		assert(ptr);

		if(0 == *ptr)
		{
			assert(utf8_foo::charlen(ptr) == 1);
			return char_type(this_type::get_allocator());
		}
		else
		{
			value_type buf[utf8_foo::E_char_max_size] = {0};
			size_type next_size = 0;

			utf8_foo::number_to_char(
				buf, utf8_foo::negative_number(
						utf8_foo::char_to_number(ptr, utf8_foo::charlen(ptr))),
				utf8_foo::E_char_max_size, next_size);

			assert(next_size);
			return char_type(buf, next_size, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
		}
	}

	inline char_type operator~(void) const
	{
		char_type ret(this_type::operator-());
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
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);
		return this_type::prv_plus_set(s, e, step);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator+=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		const value_type* lptr = &(*s);
		const value_type* rptr = step.data();

		assert(lptr && rptr);
		assert(ch_size);

		if(lptr == rptr)
		{
			s32 num = utf8_foo::char_to_number(rptr, ch_size);
			return num? this_type::prv_plus_set(s, e, num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(rptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator+=(const basic_utf8_char_view<Char, Traits>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		const value_type* lptr = &(*s);
		const value_type* rptr = step.data();

		assert(lptr && rptr);
		assert(ch_size);

		if(lptr == rptr)
		{
			s32 num = utf8_foo::char_to_number(rptr, ch_size);
			return num? this_type::prv_plus_set(s, e, num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(rptr));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator+=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(step.data()));

	}

	inline this_type& operator+=(const this_type& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		assert(std::distance(s, e) > 0);

		if(_str_ref.data() == step._str_ref.data())
		{
			s32 num = utf8_foo::char_to_number(s, ch_size);
			return num? this_type::prv_plus_set(s, e, num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(step.data()));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator+=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);
		return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator+=(const basic_utf8_char_const_reference<utf8_string_type>& step)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		assert(std::distance(s, e) > 0);

		if(_str_ref.data() == step._str_ref.data())
		{
			s32 num = utf8_foo::char_to_number(s, ch_size);
			return num? this_type::prv_plus_set(s, e, num, step_type(num)) : *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(step.data()));
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
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);
		return this_type::prv_plus_set(s, e, -step);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator-=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		const value_type* lptr = &(*s);
		const value_type* rptr = step.data();
		assert(lptr && rptr);

		if(lptr == rptr)
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, -utf8_foo::char_to_number(rptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator-=(const basic_utf8_char_view<Char, Traits>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		const value_type* lptr = &(*s);
		const value_type* rptr = step.data();
		assert(lptr && rptr);

		if(lptr == rptr)
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, -utf8_foo::char_to_number(rptr));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator-=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_plus_set(s, e, -utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator-=(const this_type& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		if(_str_ref.data() == step._str_ref.data())
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, -utf8_foo::char_to_number(step.data()));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator-=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_plus_set(s, e, -utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator-=(const basic_utf8_char_const_reference<utf8_string_type>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		if(_str_ref.data() == step._str_ref.data())
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::prv_plus_set(s, e, -utf8_foo::char_to_number(step.data()));
		}
	}

	// operator++
	inline this_type& operator++(void)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_plus_set(s, e, s32(1));
	}

	inline char_type operator++(int)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		char_type ret(s, e, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::prv_plus_set(s, e, s32(1));
		return ret;
	}

	// operator--
	inline this_type& operator--(void)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_plus_set(s, e, s32(-1));
	}

	inline char_type operator--(int)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		char_type ret(s, e, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
		this_type::prv_plus_set(s, e, s32(-1));
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
		else if(nstep == step_type(1))
		{
			return *this;
		}
		else
		{
			iterator s = this_type::begin();
			size_type ch_size = utf8_foo::charlen(s);
			iterator e = s + ch_size;

			assert(ch_size);

			if(*s == 0)
			{
				return *this;
			}
			else if(nstep == step_type(0))
			{
				basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
				return *this;
			}
			else
			{
				s32 ch_value = static_cast<s32>(static_cast<step_type>(*s) * nstep);

				if((ch_size == 1) && (0 == (ch_value & 0xffffff80)))
				{
					*s = static_cast<value_type>(ch_value);
					return *this;
				}
				else
				{
					s32 num = utf8_foo::char_to_number(s, ch_size);
					return this_type::prv_plus_set(s, e,
								num, utf8_foo::format_number_step(
										static_cast<step_type>(num) * (nstep - step_type(1))));
				}
			}
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator*=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;
		const value_type* ptr = step.data();

		assert(ch_size);
		assert(ptr);
		assert(std::distance(s, e) > 0);

		return
			(&(*s) == ptr)?
				this_type::prv_mul_set_opt_of_self(s, e, utf8_foo::char_to_number(s, ch_size))
				: this_type::prv_mul_set_opt_of_integer(s, e, utf8_foo::char_to_number(ptr));
	}

	template<typename Char, typename Traits> inline
	this_type& operator*=(const basic_utf8_char_view<Char, Traits>& step)
	{
		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;
		const value_type* ptr = step.data();

		assert(ch_size);
		assert(ptr);
		assert(std::distance(s, e) > 0);

		return
			(&(*s) == ptr)?
				this_type::prv_mul_set_opt_of_self(s, e, utf8_foo::char_to_number(s, ch_size))
				: this_type::prv_mul_set_opt_of_integer(s, e, utf8_foo::char_to_number(ptr));
	}

	template<typename OUtf8_String> inline
	this_type& operator*=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_mul_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator*=(const this_type& step)
	{
		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		assert(std::distance(s, e) > 0);

		return
			(_str_ref.data() == step._str_ref.data())?
				this_type::prv_mul_set_opt_of_self(s, e, utf8_foo::char_to_number(s, ch_size))
				: this_type::prv_mul_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
	}

	template<typename OUtf8_String> inline
	this_type& operator*=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_mul_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator*=(const basic_utf8_char_const_reference<utf8_string_type>& step)
	{
		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		assert(std::distance(s, e) > 0);

		return
			(_str_ref.data() == step._str_ref.data())?
				this_type::prv_mul_set_opt_of_self(s, e, utf8_foo::char_to_number(s, ch_size))
				: this_type::prv_mul_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));

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
		else if(nstep == step_type(1))
		{
			return *this;
		}
		else
		{
			iterator s = this_type::begin();
			size_type ch_size = utf8_foo::charlen(s);
			iterator e = s + ch_size;

			assert(ch_size);

			if(*s == 0)
			{
				return *this;
			}
			else if(ch_size == 1 && nstep > static_cast<step_type>(1))
			{
				*s = static_cast<value_type>(static_cast<step_type>(*s) / nstep);
				return *this;
			}
			else
			{
				assert(nstep > step_type(1) || nstep < step_type(-1));

				s32 num = nstep > 0?
							utf8_foo::char_to_number(s, ch_size)
							: utf8_foo::negative_number(utf8_foo::char_to_number(s, ch_size));

				if(num < chk_step)
				{
					basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
					return *this;
				}
				else
				{
					return this_type::prv_plus_set(s, e,
								num, utf8_foo::format_number_step(
									-((static_cast<step_type>(num) * (chk_step - step_type(1))) / chk_step)));
				}
			}
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator/=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);
		const value_type* ptr = step.data();

		assert(ptr);
		assert(std::distance(s, e) > 0);

		if(&(*s) == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(s, e, utf8_foo::char_to_number(ptr));
		}
	}

	template<typename Char, typename Traits> inline
	this_type& operator/=(const basic_utf8_char_view<Char, Traits>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);
		const value_type* ptr = step.data();

		assert(ptr);
		assert(std::distance(s, e) > 0);

		if(&(*s) == ptr)
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(s, e, utf8_foo::char_to_number(ptr));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator/=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_div_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator/=(const this_type& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		if(_str_ref.data() == step._str_ref.data())
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator/=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		return this_type::prv_div_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator/=(const basic_utf8_char_const_reference<utf8_string_type>& step)
	{
		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		assert(std::distance(s, e) > 0);

		if(_str_ref.data() == step._str_ref.data())
		{
			assert((step.to_number() / step.to_number())); // div 0 chk
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(1));
			return *this;
		}
		else
		{
			return this_type::prv_div_set_opt_of_integer(s, e, utf8_foo::char_to_number(step.data()));
		}
	}

	// operator%=
	template<typename Step> inline
	typename
		boost::enable_if
		<
			boost::is_integral<Step>,
			this_type&
		>::type
		operator%=(Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT_NOT((boost::is_floating_point<Step>));

		step_type step = static_cast<step_type>(tstep);
		iterator s = this_type::begin();
		size_type ch_size = utf8_foo::charlen(s);
		iterator e = s + ch_size;

		assert(ch_size);
		assert(step);

		if(ch_size == 1)
		{
			*s = static_cast<value_type>(static_cast<step_type>(*s) % step);
		}
		else
		{
			s32 num = utf8_foo::char_to_number(s, ch_size);

			if(!(static_cast<step_type>(num) < std::abs(step)))
			{
				value_type buf[utf8_foo::E_char_max_size] = {0};
				size_type next_size = 0;

				utf8_foo::number_to_char(buf, static_cast<s32>(static_cast<step_type>(num) % step),
												utf8_foo::E_char_max_size, next_size);
				assert(next_size);
				basic_string_replace(_str_ref.org_str(), s, e, buf, next_size);
			}
		}

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	this_type& operator%=(const utf8_char_impl<Char, Traits, Alloc>& step)
	{
		iterator s = this_type::begin();
		const value_type* ptr = step.data();

		assert(ptr);

		if(&(*s) == ptr)
		{
			iterator e = s + utf8_foo::charlen(s);

			assert((step.to_number() / step.to_number())); // div 0 chk
			assert(std::distance(s, e) > 0);

			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
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
		iterator s = this_type::begin();
		const value_type* ptr = step.data();

		assert(ptr);

		if(&(*s) == ptr)
		{
			iterator e = s + utf8_foo::charlen(s);

			assert((step.to_number() / step.to_number())); // div 0 chk
			assert(std::distance(s, e) > 0);

			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator%=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		return this_type::operator%=(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator%=(const this_type& step)
	{
		iterator s = this_type::begin();
		const value_type* ptr = step.data();

		assert(ptr);

		if(&(*s) == ptr)
		{
			iterator e = s + utf8_foo::charlen(s);

			assert((step.to_number() / step.to_number())); // div 0 chk
			assert(std::distance(s, e) > 0);

			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(ptr));
		}
	}

	template<typename OUtf8_String> inline
	this_type& operator%=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		return this_type::operator%=(utf8_foo::char_to_number(step.data()));
	}

	inline this_type& operator%=(const basic_utf8_char_const_reference<utf8_string_type>& step)
	{
		iterator s = this_type::begin();

		if(_str_ref.data() == step._str_ref.data())
		{
			iterator e = s + utf8_foo::charlen(s);

			assert((step.to_number() / step.to_number())); // div 0 chk
			assert(std::distance(s, e) > 0);

			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			return this_type::operator%=(utf8_foo::char_to_number(step.data()));
		}
	}

	// operator<<=

	inline this_type& operator<<=(u32 n)
	{
		assert((u32(0x00000001) << n) + 1);

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		return n? this_type::prv_mul_set_opt_of_integer(s, e, (s32(1) << n)) : *this;
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

	template<typename OUtf8_String> inline
	this_type& operator<<=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		return this_type::operator<<=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename OUtf8_String> inline
	this_type& operator<<=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		return this_type::operator<<=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	//	operator>>=

	inline this_type& operator>>=(u32 n)
	{
		assert((u32(0x80000000) >> n) + 1);

		iterator s = this_type::begin();
		iterator e = s + utf8_foo::charlen(s);

		return n? this_type::prv_div_set_opt_of_integer(s, e, (s32(1) << n)) : *this;
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

	template<typename OUtf8_String> inline
	this_type& operator>>=(const basic_utf8_char_reference<OUtf8_String>& step)
	{
		return this_type::operator>>=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	template<typename OUtf8_String> inline
	this_type& operator>>=(const basic_utf8_char_const_reference<OUtf8_String>& step)
	{
		return this_type::operator>>=(static_cast<u32>(utf8_foo::char_to_number(step.data())));
	}

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	Since bit operations & | ^ require the same range of values,		!!!
	!!!		only numerical calculations can be made.						!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	// hash
	inline std::size_t hash(void) const
	{
		const value_type* pval = this_type::data();
		assert(pval);
		return boost::hash_range(pval, pval + utf8_foo::charlen(pval));
	}

private:

	template<typename Step>
	this_type& prv_plus_set(iterator s, iterator e, s32 num, Step step)
	{
		typedef Step step_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(utf8_foo::E_char_max_range) <= sizeof(step_type))>));

		assert(std::distance(s, e) == this_type::size());
		assert(step);
		assert(step < step_type(utf8_foo::E_char_max_range));
		assert(num == utf8_foo::char_to_number(s, this_type::size())
				|| num == utf8_foo::negative_number(utf8_foo::char_to_number(s, this_type::size())));

		value_type buf[utf8_foo::E_char_max_size] = {0};
		size_type next_size = 0;

		if(step > 0)
		{
			if(step < step_type(utf8_foo::E_char_max_range) - num)
			{
				utf8_foo::number_to_char(buf, static_cast<s32>(static_cast<step_type>(num) + step),
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
		basic_string_replace(_str_ref.org_str(), s, e, buf, next_size);

		return *this;
	}

	template<typename Step>
	this_type& prv_plus_set(iterator s, iterator e, Step nstep)
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
			assert(s == this_type::begin());
			assert(e == s + utf8_foo::charlen(s));
			assert(std::distance(s, e) == this_type::size());

			size_type ch_size = static_cast<size_type>(std::distance(s, e));
			s32 ch_value = static_cast<s32>(static_cast<step_type>(*s) + step);

			assert(ch_size);

			if((ch_size == 1) && (0 == (ch_value & 0xffffff80)))
			{
				*s = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
				return this_type::prv_plus_set(s, e, utf8_foo::char_to_number(s, ch_size), step);
			}
		}
	}

	// prv_mul_set_opt
	template<typename Step>
	this_type& prv_mul_set_opt_of_integer(iterator s, iterator e, Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT_NOT((boost::is_floating_point<Step>));

		assert(std::distance(s, e) > 0);

		step_type nstep = static_cast<step_type>(tstep);

		if(*s == 0 || nstep == step_type(1))
		{
			return *this;
		}
		else if(nstep == step_type(0))
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			size_type ch_size = static_cast<size_type>(std::distance(s, e));
			s32 ch_value = static_cast<s32>(static_cast<step_type>(*s) * nstep);

			assert(ch_size);

			if((ch_size == 1) && (0 == (ch_value & 0xffffff80)))
			{
				*s = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
			s32 num = utf8_foo::char_to_number(s, ch_size);
				return this_type::prv_plus_set(s, e,
							num, utf8_foo::format_number_step(
									static_cast<step_type>(num) * (nstep - step_type(1))));
			}
		}
	}

	this_type& prv_mul_set_opt_of_self(iterator s, iterator e, s32 num)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		assert(std::distance(s, e) > 0);
		assert(num >= 0);
		assert(static_cast<step_type>(num) < step_type(utf8_foo::E_char_max_range));
		assert(num == utf8_foo::char_to_number(s, std::distance(s, e)));

		if(*s == 0 || num == 1)
		{
			return *this;
		}
		else if(num == 0)
		{
			basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
			return *this;
		}
		else
		{
			size_type ch_size = static_cast<size_type>(std::distance(s, e));
			s32 ch_value = static_cast<s32>(static_cast<step_type>(num) * static_cast<step_type>(num));

			assert(ch_size);

			if((ch_size == 1) && (0 == (ch_value & 0xffffff80)))
			{
				*s = static_cast<value_type>(ch_value);
				return *this;
			}
			else
			{
				return this_type::prv_plus_set(s, e,
							num, utf8_foo::format_number_step(
									static_cast<step_type>(num) * static_cast<step_type>(num - 1)));
			}
		}
	}

	// prv_div_set_opt
	template<typename Step>
	this_type& prv_div_set_opt_of_integer(iterator s, iterator e, Step tstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(utf8_foo::E_char_max_range), Step) step_type;
		BOOST_MPL_ASSERT_NOT((boost::is_floating_point<Step>));

		assert(std::distance(s, e) > 0);

		step_type nstep = static_cast<step_type>(tstep);
		assert(nstep);

		if(*s == 0 || nstep == step_type(1))
		{
			return *this;
		}
		else
		{
			size_type ch_size = static_cast<size_type>(std::distance(s, e));

			assert(ch_size);

			if(ch_size == 1 && nstep > static_cast<step_type>(1))
			{
				*s = static_cast<value_type>(static_cast<step_type>(*s) / nstep);
				return *this;
			}
			else
			{
				assert(nstep > step_type(1) || nstep < step_type(-1));
				step_type chk_step = std::abs(nstep);

				s32 num = nstep > 0?
							utf8_foo::char_to_number(s, ch_size)
							: utf8_foo::negative_number(utf8_foo::char_to_number(s, ch_size));

				if(num < chk_step)
				{
					basic_string_replace(_str_ref.org_str(), s, e, 1, value_type(0));
					return *this;
				}
				else
				{
					return this_type::prv_plus_set(s, e,
								num, utf8_foo::format_number_step(
									-((static_cast<step_type>(num) * (chk_step - step_type(1))) / chk_step)));
				}
			}
		}
	}

private:
	utf8_string_type& _str_ref;
	const size_type _pos;
};


// const_cast

template<typename Utf8String> inline
basic_utf8_char_const_reference<Utf8String>
	const_cast_(const basic_utf8_char_reference<Utf8String>& right)
{
	return right.const_cast_();
}

template<typename CRef, typename Utf8String> inline
CRef const_cast_(const basic_utf8_char_reference<Utf8String>& right)
{
	return right.template const_cast_<CRef>();
}

// io

template<typename Char, typename Traits, typename Utf8String>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const basic_utf8_char_reference<Utf8String>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Utf8String>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Utf8String>
std::basic_istream<Char, Traits>&
	operator>>(std::basic_istream<Char, Traits>& is,
				basic_utf8_char_reference<Utf8String> right)
{
	yggr::charset::basic_string<Char> str;
	is >> str;
	right.assign(str);
	return is;
}

// arithmetic

// num __op__ utf8_char_ref -> num, utf8_char_ref __op__ num -> utf8_char
// utf8_char_ref __op__ utf8_char_ref -> utf8_char
// utf8_char_ref __op__ utf8_char_view, utf8_char_view __op__ utf8_char_ref -> utf8_char
// utf8_char_ref __op__ basic_utf8_char_const_reference, basic_utf8_char_const_reference __op__ utf8_char_ref -> utf8_char

#define YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename \
				boost::mpl::if_< \
					::yggr::is_enum<T>, \
					s32, \
					typename __result_deducer__<T, s32>::type >::type >::type \
		operator __op__(T l, BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename \
			boost::mpl::if_< \
				::yggr::is_enum<T>, \
				s32, \
				typename __result_deducer__<T, s32>::type >::type result_type; \
		return static_cast<result_type>(l) __op__ r.to_number(); } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename \
				boost::mpl::if_< \
					::yggr::is_enum<T>, \
					s32, \
					typename __result_deducer__<T, s32>::type >::type >::type \
		operator __op__(T l, const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename \
			boost::mpl::if_< \
				::yggr::is_enum<T>, \
				s32, \
				typename __result_deducer__<T, s32>::type >::type result_type; \
		return static_cast<result_type>(l) __op__ r.to_number(); }

#define YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( __op__, __use_op__ ) \
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename basic_utf8_char_reference<Utf8String>::char_type >::type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, T r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename basic_utf8_char_reference<Utf8String>::char_type >::type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, T r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference< Utf8String > BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; }

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( +, plus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( +, += )

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( -, minus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( -, -= )

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( *, multiplies_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( *, *= )

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( /, divides_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( /, /= )

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( %, modulus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( %, %= )

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( <<, bitwise_left_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( <<, <<= )

	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL( >>, bitwise_right_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL( >>, >>= )


#undef YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_OF_NUMBER_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_REF_ARITHMETIC_GENERAL_IMPL

// &= |= ^=

#define YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OPERATOR_SET_IMPL( __op__ ) \
	template<typename T, typename Utf8String> inline \
	T& operator __op__(T& l, const basic_utf8_char_reference<Utf8String>& r) { \
		l __op__ r.to_number(); \
		return l; }

	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OPERATOR_SET_IMPL( &= )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OPERATOR_SET_IMPL( |= )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OPERATOR_SET_IMPL( ^= )

#undef YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OPERATOR_SET_IMPL

#define YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, const basic_utf8_char_reference<Utf8String>& r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, T r) { \
		return l.to_number() __op__ r; } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, T r) { \
		return l.to_number() __op__ r; }

#define YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_GENERAL_OPERATOR_IMPL( __op__ ) \
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); }


#define YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( __op__, __self_ret__ ) \
	template<typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference< Utf8String > BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference< Utf8String >& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference< Utf8String > BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
						const basic_utf8_char_const_reference< Utf8String >& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference< Utf8String > BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference< Utf8String > BOOST_RV_REF_END l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_const_reference< Utf8String >& l, \
						BOOST_RV_REF_BEG basic_utf8_char_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_const_reference< Utf8String >& l, \
						const basic_utf8_char_reference<Utf8String>& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); }

	// operator&
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OF_NUMBER_IMPL( &, bitwise_and_operator_result)
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_GENERAL_OPERATOR_IMPL( & )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( &, l.to_number() )

	// operator|
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OF_NUMBER_IMPL( |, bitwise_or_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_GENERAL_OPERATOR_IMPL( | )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( |, l.to_number() )

	// operator^
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OF_NUMBER_IMPL( ^, bitwise_xor_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_GENERAL_OPERATOR_IMPL( ^ )
	YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( ^, 0 )


#undef YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_GENERAL_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_REF_BITWISE_OF_NUMBER_IMPL

// compare

#define YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP1( __op__ ) \
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const basic_utf8_char_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_reference<Utf8String>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const basic_utf8_char_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
							const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __op__ 0); }

#define YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP2( __op__, __rop__ ) \
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		 operator __op__(const basic_utf8_char_reference<Utf8String>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		 operator __op__(Char l, const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		 operator __op__(const basic_utf8_char_reference<Utf8String>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		 operator __op__(const Char* l, const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
							const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const basic_utf8_char_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); }

// ==, !=
	YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP1(==)
	YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP1(!=)

// <, <=, >, >=
	YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP2(<, <=)
	YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP2(<=, <)
	YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP2(>, >=)
	YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP2(>=, >)

#undef YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP1
#undef YGGR_TMP_PP_UTF8_CHAR_REFERENCE_CMP2

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename Utf8String> inline
string_charset_helper<basic_utf8_char_reference<Utf8String>&, const string&>
	make_string_charset_helper(basic_utf8_char_reference<Utf8String>& src,
								const string& src_charset_name
									= charset_name_t< basic_utf8_char_reference<Utf8String> >())
{
	typedef basic_utf8_char_reference<Utf8String>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

	return ret_type(src, src_charset_name);
}


template<typename Utf8String> inline
string_charset_helper<const basic_utf8_char_reference<Utf8String>&, const string&>
	make_string_charset_helper(const basic_utf8_char_reference<Utf8String>& src,
								const string& src_charset_name
									= charset_name_t< basic_utf8_char_reference<Utf8String> >())
{
	typedef const basic_utf8_char_reference<Utf8String>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

	return ret_type(src, src_charset_name);
}

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{
namespace swap_support
{
template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline
void swap(basic_utf8_char_reference<Utf8String>& l, utf8_char_impl<Char, Traits, Alloc>& r)
{
	l.swap(r);
}

template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline
void swap(utf8_char_impl<Char, Traits, Alloc>& l, basic_utf8_char_reference<Utf8String>& r)
{
	r.swap(l);
}

template<typename Utf8_StringL, typename Utf8_StringR> inline
void swap(basic_utf8_char_reference<Utf8_StringL>& l, basic_utf8_char_reference<Utf8_StringR>& r)
{
	l.swap(r);
}

template<typename Utf8String> inline
void swap(basic_utf8_char_reference<Utf8String>& l, basic_utf8_char_reference<Utf8String>& r)
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

// boost range_size support
namespace yggr
{
namespace charset
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(1, basic_utf8_char_reference)

} // namespace charset
} // namespace yggr

//byte_size support
namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename Utf8String>
struct byte_size_tpl_helper< charset::basic_utf8_char_reference<Utf8String> >
	: public boost::mpl::true_
{
public:
	typedef charset::basic_utf8_char_reference<Utf8String> value_type;

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

template<typename Utf8String> inline
std::size_t hash_value(const ::yggr::charset::basic_utf8_char_reference<Utf8String>& ch)
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

template<typename Utf8String>
struct hash< ::yggr::charset::basic_utf8_char_reference<Utf8String> >
{
	typedef ::yggr::charset::basic_utf8_char_reference<Utf8String> type;

	inline std::size_t operator()(const type& ch) const
	{
		return ch.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

//------------basic_utf8_char_const_reference----------------

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename T, typename Nil = void>
class utf8_char_cref_const_cast_helper;

template<typename Utf8String, typename Nil>
class utf8_char_cref_const_cast_helper< basic_utf8_char_const_reference<Utf8String>, Nil >
{
public:
	typedef Utf8String utf8_string_type;
	typedef basic_utf8_char_const_reference<utf8_string_type> result_type;

private:
	typedef utf8_char_cref_const_cast_helper this_type;

public:
	inline result_type operator()(const result_type& val) const
	{
		return result_type(val._str_ref, val._pos);
	}
};

template<typename Utf8String, typename Nil>
class utf8_char_cref_const_cast_helper< basic_utf8_char_reference<Utf8String>, Nil >
{
public:
	typedef Utf8String utf8_string_type;
	typedef basic_utf8_char_const_reference<utf8_string_type> utf8_char_const_reference_type;
	typedef basic_utf8_char_reference<utf8_string_type> result_type;

private:
	typedef utf8_char_cref_const_cast_helper this_type;

public:
	inline result_type operator()(const utf8_char_const_reference_type& val) const
	{
		return result_type(const_cast<utf8_string_type&>(val._str_ref), val._pos);
	}
};

template<typename Char, typename Traits, typename Nil>
class utf8_char_cref_const_cast_helper< basic_utf8_char_view<Char, Traits>, Nil >
{
public:
	typedef basic_utf8_char_view<Char, Traits> result_type;

private:
	typedef utf8_char_cref_const_cast_helper this_type;

public:
	template<typename Utf8String> inline
	result_type operator()(const basic_utf8_char_const_reference<Utf8String>& val) const
	{
		return result_type(val.data());
	}
};

// utf8_char_cref_org_str_helper
template<typename BasicString, bool is_utf8_char>
class utf8_char_cref_org_str_helper;

template<typename BasicString>
class utf8_char_cref_org_str_helper<BasicString, true>
	: public boost::mpl::bool_<true>
{
public:
	typedef BasicString base_string_type;
	typedef const base_string_type& const_result_type;

private:
	typedef utf8_char_cref_org_str_helper this_type;

public:
	// const version
	template<typename Utf8String> inline
	const_result_type org_str(const basic_utf8_char_const_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String> inline
	typename
		boost::enable_if
		<
			boost::is_same<String, base_string_type>,
			const_result_type
		>::type
		org_str(const basic_utf8_char_const_reference<Utf8String>& obj) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<String, base_string_type>,
				container_ex::is_allocator<Alloc>
			>,
			const_result_type
		>::type
		org_str(const basic_utf8_char_const_reference<Utf8String>& obj, const Alloc&) const
	{
		assert(0 == obj._pos);
		return obj._str_ref.org_str();
	}

	template<typename String, typename Utf8String>
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<String, base_string_type> >,
				is_basic_string_t<String>
			>,
			String
		>::type
		org_str(const basic_utf8_char_const_reference<Utf8String>& obj) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;
		assert(0 == obj._pos);

		const value_type* p = obj.data();
		size_type byte_size = utf8_foo::charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		memcpy(&str[0], p, byte_size);

		return str;
	}

	template<typename String, typename Utf8String, typename Alloc>
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<String, base_string_type> >,
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const basic_utf8_char_const_reference<Utf8String>& obj, const Alloc& alloc) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;
		assert(0 == obj._pos);

		const value_type* p = obj.data();
		size_type byte_size = utf8_foo::charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		memcpy(&str[0], p, byte_size);

		return str;
	}

};

template<typename BasicString>
class utf8_char_cref_org_str_helper<BasicString, false>
	: public boost::mpl::bool_<false>
{
public:
	typedef BasicString base_string_type;
	typedef base_string_type const_result_type;

private:
	typedef utf8_char_cref_org_str_helper this_type;

public:
	template<typename Utf8String> inline
	const_result_type org_str(const basic_utf8_char_const_reference<Utf8String>& obj)
	{
		const value_type* p = obj.data();
		return const_result_type(p, utf8_foo::charlen(p), obj.get_allocator());
	}

	template<typename String, typename Utf8String>
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<String, base_string_type> >,
				is_basic_string_t<String>
			>,
			String
		>::type
		org_str(const basic_utf8_char_const_reference<Utf8String>& obj) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		const value_type* p = obj.data();
		size_type byte_size = utf8_foo::charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0));
		memcpy(&str[0], p, byte_size);

		return str;
	}

	template<typename String, typename Utf8String, typename Alloc>
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<String, base_string_type> >,
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const basic_utf8_char_const_reference<Utf8String>& obj, const Alloc& alloc) const
	{
		typedef String ret_string_type;
		typedef typename boost::range_value<ret_string_type>::type ret_value_type;

		const value_type* p = obj.data();
		size_type byte_size = utf8_foo::charlen(p);
		assert(p && byte_size);
		size_type ret_size = (byte_size + sizeof(ret_value_type) - 1) / sizeof(ret_value_type);

		ret_string_type str(ret_size, ret_value_type(0), alloc);
		memcpy(&str[0], p, byte_size);

		return str;
	}

};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename Utf8String>
class basic_utf8_char_const_reference
{
private:
	template<typename OUtf8_String>
	friend class basic_utf8_char_reference;

	template<typename OUtf8_String>
	friend class basic_utf8_char_const_reference;

	template<typename Char, typename Traits, typename Alloc>
	friend class utf8_string_impl;

	template<typename Char, typename Traits, typename Alloc>
	friend class utf8_char_impl;

	YGGR_PP_FRIEND_TYPENAME(Utf8String);

	BOOST_MPL_ASSERT((
		boost::mpl::or_
		<
			is_utf8_char_t<Utf8String>,
			is_utf8_string_t<Utf8String>
		> ));

public:
	typedef Utf8String utf8_string_type;

	typedef typename utf8_string_type::char_type char_type;
	typedef typename utf8_string_type::value_type value_type;
	typedef typename utf8_string_type::size_type size_type;

	typedef typename utf8_string_type::base_type base_string_type;
	typedef typename base_string_type::iterator iterator;
	typedef typename base_string_type::const_iterator const_iterator;

	typedef typename base_string_type::pointer pointer;
	typedef typename base_string_type::const_pointer const_pointer;

	typedef typename base_string_type::traits_type traits_type;
	typedef typename base_string_type::allocator_type allocator_type;

	typedef utf8_string_iterator<char_type, iterator> utf8_iterator;
	typedef utf8_string_const_iterator<char_type, const_iterator> utf8_const_iterator;

	typedef
		basic_utf8_char_view
		<
			typename char_type::value_type,
			typename char_type::traits_type
		> char_view_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef basic_utf8_char_const_reference this_type;
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:
	// msvc cpp20 not support
	//basic_utf8_char_const_reference(BOOST_RV_REF(utf8_string_type) str_ref)
	//	: _str_ref(str_ref), _pos(size_type(0))
	//{
	//	assert(0 < str_ref.length());
	//}

#if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
	basic_utf8_char_const_reference(utf8_string_type& str_ref)
		: _str_ref(str_ref), _pos(size_type(0))
	{
		assert(0 < str_ref.length());
	}
#endif // #	if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

	basic_utf8_char_const_reference(const utf8_string_type& str_ref)
		: _str_ref(str_ref), _pos(size_type(0))
	{
		assert(0 < str_ref.length());
	}

	template<typename Size> // fix conflict
	basic_utf8_char_const_reference(const utf8_string_type& str_ref, Size pos,
									typename
										boost::enable_if
										<
											is_convertible_size_t<Size>,
											sfinae_type
										>::type sfinae = 0)
		: _str_ref(str_ref), _pos(pos)
	{
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_const_reference(const utf8_string_type& str_ref, utf8_iterator iter)
		: _str_ref(str_ref), _pos(std::distance(utf8_const_iterator(str_ref.begin()), utf8_const_iterator(iter)))
	{
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_const_reference(const utf8_string_type& str_ref, utf8_const_iterator iter)
		: _str_ref(str_ref), _pos(std::distance(utf8_const_iterator(str_ref.begin()), iter))
	{
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_const_reference(const utf8_string_type& str_ref, iterator iter)
		: _str_ref(str_ref),
			_pos(std::distance(utf8_const_iterator(str_ref.begin()),
								utf8_const_iterator(iter)))
	{
		assert(utf8_foo::charlen(iter));
		assert(_pos < str_ref.length());
	}

	basic_utf8_char_const_reference(const utf8_string_type& str_ref, const_iterator iter)
		: _str_ref(str_ref),
			_pos(std::distance(utf8_const_iterator(str_ref.begin()),
								utf8_const_iterator(iter)))
	{
		assert(utf8_foo::charlen(iter));
		assert(_pos < str_ref.length());
	}

#if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
	basic_utf8_char_const_reference(basic_utf8_char_reference<utf8_string_type>& right)
		: _str_ref(right._str_ref), _pos(right._pos)
	{
	}
#endif // #	if defined(_MSC_VER) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

	basic_utf8_char_const_reference(const basic_utf8_char_reference<utf8_string_type>& right)
		: _str_ref(right._str_ref), _pos(right._pos)
	{
	}

	basic_utf8_char_const_reference(BOOST_RV_REF(this_type) right)
		: _str_ref(const_cast<const utf8_string_type&>(right._str_ref)),
			_pos(right._pos)
	{
	}

	basic_utf8_char_const_reference(const this_type& right)
		: _str_ref(right._str_ref), _pos(right._pos)
	{
	}

	~basic_utf8_char_const_reference(void)
	{
	}

public:
	inline const value_type* operator&(void) const
	{
		return &(*(this_type::cbegin()));
	}

	//operator type
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		assert(_pos < _str_ref.length());
		const_iterator s = this_type::begin();
		return (1 < utf8_foo::charlen(s)) || (*s);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	operator Basic_String<Char, Traits, Alloc>(void) const
	{
		typedef Basic_String<Char, Traits, Alloc> ret_type;
		YGGR_ASSERT_CHAR_TYPE(Char);

		return this_type::str<ret_type>(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(ret_type));
	}

#if !(defined(__clang__) && (__clang__))
	inline operator char_type(void) const
	{
		const_iterator s = this_type::begin();
		const_iterator e = s + utf8_foo::charlen(s);
		return char_type(s, e, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}
#endif // #if !(defined(__clang__) && (__clang__))

	template<typename Char, typename Traits> inline
	operator basic_utf8_char_view<Char, Traits>(void) const
	{
		return basic_utf8_char_view<Char, Traits>(this_type::data());
	}

	inline operator char_view_type(void) const
	{
		return char_view_type(this_type::data());
	}

#define YGGR_TMP_PP_OPERATOR_T( __type__ ) \
	inline operator __type__(void) const { \
		return static_cast< __type__ >(utf8_foo::char_to_number(this_type::data())); }

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

private:
	template<typename T, typename Nil>
	friend class detail::utf8_char_cref_const_cast_helper;

public:
	basic_utf8_char_reference<utf8_string_type> inline
		const_cast_(void) const
	{
		typedef basic_utf8_char_reference<utf8_string_type> ret_type;
		return ret_type(const_cast<utf8_string_type&>(_str_ref), _pos);
	}

	template<typename NON_ConstRef> inline
	typename detail::utf8_char_cref_const_cast_helper<NON_ConstRef>::result_type
		const_cast_(void) const
	{
		typedef detail::utf8_char_cref_const_cast_helper<NON_ConstRef> h_type;
		h_type h;
		return h(*this);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* data(void) const
	{
		return &(*(this_type::cbegin()));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* c_str(void) const
	{
		return &(*(this_type::cbegin()));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type size(void) const
	{
		return utf8_foo::charlen(this_type::begin());
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type length(void) const
	{
		assert(this_type::is_validate());
		return 1;
	}

	inline bool is_validate(void) const
	{
		return utf8_foo::charlen(this_type::begin());
	}

	inline const_iterator begin(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + _pos).org();
	}

	inline const_iterator cbegin(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + _pos).org();
	}

	inline const_iterator end(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + (_pos + 1)).org();
	}

	inline const_iterator cend(void) const
	{
		return (utf8_const_iterator(_str_ref.begin()) + (_pos + 1)).org();
	}

	inline size_type index(void) const
	{
		return _pos;
	}

	template<typename OUtf8_String> inline
	bool is_same_address(const basic_utf8_char_reference<OUtf8_String>& right) const
	{
		return false;
	}

	inline bool is_same_address(const basic_utf8_char_reference<utf8_string_type>& right) const
	{
		return boost::addressof(_str_ref) == boost::addressof(right._str_ref)
				&& _pos == right._pos;
	}

	template<typename OUtf8_String> inline
	bool is_same_address(const basic_utf8_char_const_reference<OUtf8_String>& right) const
	{
		return false;
	}

	inline bool is_same_address(const this_type& right) const
	{
		return boost::addressof(_str_ref) == boost::addressof(right._str_ref)
				&& _pos == right._pos;
	}

	template<typename Char, typename Traits> inline
	bool is_same_address(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return this_type::data() == right.data();
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::is_same<utf8_char_impl<Char, Traits, Alloc>, utf8_string_type>,
			bool
		>::type
		is_same_address(const utf8_char_impl<Char, Traits, Alloc>& right) const
	{
		return false;
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::is_same<utf8_char_impl<Char, Traits, Alloc>, utf8_string_type>,
			bool
		>::type
		is_same_address(const utf8_char_impl<Char, Traits, Alloc>& right) const
	{
		assert(_pos == 0);
		return _str_ref.data() == right.data();
	}

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
					this_type::get_allocator(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) ),
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
				utf8_foo::charcmp(this_type::data(), static_cast<now_char_type>(c))
				: this_type::compare(
					char_type(
						static_cast<now_char_type>(c),
						this_type::get_allocator(),
						src_charset_name),
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
				char_type(str, n, this_type::get_allocator(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)),
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
				utf8_foo::charcmp(this_type::data(), src)
				: this_type::compare(
					char_type(src, n, this_type::get_allocator(), src_charset_name),
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
			base_string_type tmp_src(_str_ref.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		const_iterator s = this_type::begin();

		if(reinterpret_cast<const value_type*>(src) == &(*s))
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}

		return
			src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
				utf8_foo::charcmp(s, src)
				: utf8_foo::charcmp(
					s,
					charset_foo::s_xchg_diff_charset<base_string_type>(
						src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME()));
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
			base_string_type tmp_src(_str_ref.get_allocator());
			charset_foo::s_xchg(tmp_src, src, src_charset_name, YGGR_STR_INNER_STRING_CHARSET_NAME());
			assert((tmp_src.empty() || tmp_src[0] == 0 || 1 == utf8_foo::strlen(tmp_src)));
		}
#endif // _DEBUG

		const_iterator s = this_type::begin();

		if(reinterpret_cast<const value_type*>(src.data()) == &(*s))
		{
			assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
			return 0;
		}

		return src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME()?
					utf8_foo::charcmp(s, src.data())
					: utf8_foo::charcmp(s,
										charset_foo::s_xchg_diff_charset<base_string_type>(
											src, src_charset_name,
											YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	template<typename Char, typename Traits, typename Alloc> inline
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other) const
	{
		return utf8_foo::charcmp(this_type::data(), other.data());
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
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_reference<OUtf8_String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_const_reference<OUtf8_String>& right) const
	{
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename OUtf8_String> inline
	s32 compare(const basic_utf8_char_const_reference<OUtf8_String>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right) const
	{
		return utf8_foo::charcmp(this_type::data(), right.data());
	}

	template<typename Char, typename Traits> inline
	s32 compare(const basic_utf8_char_view<Char, Traits>& right,
					const string& src_charset_name) const
	{
		assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());
		return utf8_foo::charcmp(this_type::data(), right.data());
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
		return _str_ref.get_allocator();
	}

	// org_str
private:
	template<typename BasicString, bool is_utf8_char>
	friend class detail::utf8_char_cref_org_str_helper;

	typedef
		detail::utf8_char_cref_org_str_helper
		<
			base_string_type,
			is_utf8_char_t<utf8_string_type>::value
		> org_str_helper_type;

public:
	inline typename org_str_helper_type::const_result_type
		org_str(void) const
	{
		org_str_helper_type h;
		return h.org_str(*this);
	}

	inline typename org_str_helper_type::const_result_type
		org_str(const allocator_type&) const
	{
		org_str_helper_type h;
		return h.org_str(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::is_same<base_string_type, String>,
			typename org_str_helper_type::const_result_type
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_isomeric_same_string<base_string_type, String>,
				org_str_helper_type
			>,
			const String&
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<base_string_type, String>,
				container_ex::is_allocator<Alloc>
			>,
			typename org_str_helper_type::const_result_type
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_isomeric_same_string<base_string_type, String>,
				org_str_helper_type,
				container_ex::is_allocator<Alloc>
			>,
			const String&
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_string_type>
				>,
				is_basic_string_t<String>
			>,
			String
		>::type
		org_str(void) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					is_same_or_isomeric_same_string<String, base_string_type>
				>,
				is_basic_string_t<String>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		org_str(const Alloc& alloc) const
	{
		typedef String ret_string_type;

		org_str_helper_type h;
		return h.template org_str<ret_string_type>(*this, alloc);
	}

	// str
	inline utf8_string_type str(void) const
	{
		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								_str_ref.get_allocator(),
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const allocator_type& alloc) const
	{
		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								_str_ref.get_allocator(),
								YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline utf8_string_type str(const allocator_type& alloc, const string& dst_charset_name) const
	{
		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		const value_type* p = this_type::data();
		assert(p);
		return utf8_string_type(p, utf8_foo::charlen(p),
								alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		assert(dst_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

		const value_type* p = this_type::data();
		assert(p);
		return ret_string_type(p, utf8_foo::charlen(p), alloc, YGGR_STR_INNER_STRING_CHARSET_NAME());
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

		const value_type* p = this_type::data();
		assert(p);
		return charset_foo::s_xchg<ret_string_type>(
					p, utf8_foo::charlen(p),
					YGGR_STR_INNER_STRING_CHARSET_NAME(),
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));
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

		const value_type* p = this_type::data();
		assert(p);
		return
			charset_foo::s_xchg<ret_string_type>(
				p, utf8_foo::charlen(p), alloc,
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));
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

		const value_type* p = this_type::data();
		assert(p);
		return
			charset_foo::s_xchg<ret_string_type>(
				p, utf8_foo::charlen(p),
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

		const value_type* p = this_type::data();
		assert(p);
		return
			charset_foo::s_xchg<ret_string_type>(
				p, utf8_foo::charlen(p), alloc,
				YGGR_STR_INNER_STRING_CHARSET_NAME(),
				dst_charset_name);
	}

	// view
	inline char_view_type view(void) const
	{
		return char_view_type(this_type::data());
	}

	template<typename View> inline
	View view(void) const
	{
		return View(this_type::data());
	}

	// arithmetic
	inline s32 to_number(void) const
	{
		return utf8_foo::char_to_number(this_type::data());
	}

	inline char_type operator+(void) const
	{
		const_iterator s = this_type::begin();
		const_iterator e = s + utf8_foo::charlen(s);
		return char_type(s, e, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
	}

	inline char_type operator-(void) const
	{
		const value_type* ptr = this_type::data();
		assert(ptr);

		if(0 == *ptr)
		{
			assert(utf8_foo::charlen(ptr) == 1);
			return char_type(this_type::get_allocator());
		}
		else
		{
			value_type buf[utf8_foo::E_char_max_size] = {0};
			size_type next_size = 0;

			utf8_foo::number_to_char(
				buf, utf8_foo::negative_number(
						utf8_foo::char_to_number(ptr, utf8_foo::charlen(ptr))),
				utf8_foo::E_char_max_size, next_size);

			assert(next_size);
			return char_type(buf, next_size, _str_ref.get_allocator(), YGGR_STR_INNER_STRING_CHARSET_NAME());
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
		const value_type* pval = this_type::data();
		assert(pval);
		return boost::hash_range(pval, pval + utf8_foo::charlen(pval));
	}

private:
	const utf8_string_type& _str_ref;
	size_type _pos;
};


// const_cast
template<typename Utf8String> inline
basic_utf8_char_reference<Utf8String>
	const_cast_(const basic_utf8_char_const_reference<Utf8String>& right)
{
	return right.const_cast_();
}

template<typename CRef, typename Utf8String> inline
CRef const_cast_(const basic_utf8_char_const_reference<Utf8String>& right)
{
	return right.template const_cast_<CRef>();
}

// io

template<typename Char, typename Traits, typename Utf8String>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const basic_utf8_char_const_reference<Utf8String>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Utf8String>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

// arithmetic

// num __op__ utf8_char_cref -> num, utf8_char_ref __op__ num -> utf8_char
// utf8_char_cref __op__ utf8_char_cref -> utf8_char
// utf8_char_cref __op__ utf8_char_view, utf8_char_view __op__ utf8_char_ref -> utf8_char

#define YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename \
				boost::mpl::if_< \
					::yggr::is_enum<T>, \
					s32, \
					typename __result_deducer__<T, s32>::type >::type >::type \
		operator __op__(T l, BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename \
			boost::mpl::if_< \
				::yggr::is_enum<T>, \
				s32, \
				typename __result_deducer__<T, s32>::type >::type result_type; \
		return static_cast<result_type>(l) __op__ r.to_number(); } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename \
				boost::mpl::if_< \
					::yggr::is_enum<T>, \
					s32, \
					typename __result_deducer__<T, s32>::type >::type >::type \
		operator __op__(T l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename \
			boost::mpl::if_< \
				::yggr::is_enum<T>, \
				s32, \
				typename __result_deducer__<T, s32>::type >::type result_type; \
		return static_cast<result_type>(l) __op__ r.to_number(); }

#define YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( __op__, __use_op__ ) \
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename basic_utf8_char_const_reference<Utf8String>::char_type >::type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, T r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::mpl::or_< \
				boost::is_arithmetic<T>, \
				::yggr::is_enum<T> >, \
			typename basic_utf8_char_const_reference<Utf8String>::char_type >::type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, T r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_const_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_const_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret __use_op__ r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_const_reference<Utf8_StringL>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_const_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_const_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	typename basic_utf8_char_const_reference<Utf8_StringL>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8_StringL>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_const_reference<Utf8String>::char_type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		typedef typename basic_utf8_char_const_reference<Utf8String>::char_type ret_type; \
		ret_type ret(l, l.get_allocator()); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	typename basic_utf8_char_view<Char, Traits>::char_type \
		operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		typedef typename basic_utf8_char_view<Char, Traits>::char_type ret_type; \
		ret_type ret(l); \
		ret  __use_op__  r; \
		return ret; }


	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( +, plus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( +, += )

	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( -, minus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( -, -= )

	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( *, multiplies_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( *, *= )

	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( /, divides_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( /, /= )

	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( %, modulus_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( %, %= )

	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( <<, bitwise_left_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( <<, <<= )

	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL( >>, bitwise_right_shift_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL( >>, >>= )


#undef YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_OF_NUMBER_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_CREF_ARITHMETIC_GENERAL_IMPL

// &= |= ^=

#define YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OPERATOR_SET_IMPL( __op__ ) \
	template<typename T, typename Utf8String> inline \
	T& operator __op__(T& l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		l __op__ r.to_number(); \
		return l; }

	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OPERATOR_SET_IMPL( &= )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OPERATOR_SET_IMPL( |= )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OPERATOR_SET_IMPL( ^= )

#undef YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OPERATOR_SET_IMPL

#define YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OF_NUMBER_IMPL( __op__, __result_deducer__ ) \
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<T, s32>::type >::type \
		operator __op__(T l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l __op__ r.to_number(); } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, T r) { \
		return l.to_number() __op__ r; } \
	\
	template<typename T, typename Utf8String> inline \
	typename \
		boost::enable_if< \
			boost::is_integral<T>, \
			typename __result_deducer__<s32, T>::type >::type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, T r) { \
		return l.to_number() __op__ r; }

#define YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_GENERAL_OPERATOR_IMPL( __op__ ) \
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringL> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8_StringR> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
						const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
						const basic_utf8_char_view<Char, Traits>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(BOOST_RV_REF_BEG basic_utf8_char_view<Char, Traits> BOOST_RV_REF_END l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.to_number() __op__ r.to_number(); } \
	\
	template<typename Char, typename Traits, typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
						const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.to_number() __op__ r.to_number(); }

#define YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( __op__, __self_ret__ ) \
	template<typename Utf8String> inline \
	s32 operator __op__( \
			BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
			BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__( \
			BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END l, \
			const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__( \
			const basic_utf8_char_const_reference<Utf8String>& l, \
			BOOST_RV_REF_BEG basic_utf8_char_const_reference<Utf8String> BOOST_RV_REF_END r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); } \
	\
	template<typename Utf8String> inline \
	s32 operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
										const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.is_same_address(r)? ( __self_ret__ ) : ( l.to_number() __op__ r.to_number() ); }

	// operator&
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OF_NUMBER_IMPL( &, bitwise_and_operator_result)
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_GENERAL_OPERATOR_IMPL( & )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( &, l.to_number() )

	// operator|
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OF_NUMBER_IMPL( |, bitwise_or_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_GENERAL_OPERATOR_IMPL( | )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( |, l.to_number() )

	// operator^
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OF_NUMBER_IMPL( ^, bitwise_xor_operator_result )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_GENERAL_OPERATOR_IMPL( ^ )
	YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL( ^, 0 )

#undef YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_MAY_BE_SAME_ADDRESS_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_GENERAL_OPERATOR_IMPL
#undef YGGR_TMP_PP_UTF8_CHAR_CREF_BITWISE_OF_NUMBER_IMPL

// compare

#define YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP1( __op__ ) \
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
							const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
										const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __op__ 0); }

#define YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP2( __op__, __rop__ ) \
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, Char r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_can_be_conv_into_char<Char>, bool>::type \
		operator __op__(Char l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, const Char* r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char> inline \
	typename boost::enable_if< is_convertible_char<Char>, bool>::type \
		operator __op__(const Char* l, const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const Basic_String<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline \
	bool operator __op__(const Basic_String<Char, Traits, Alloc>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const utf8_char_impl<Char, Traits, Alloc>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits, typename Alloc> inline \
	bool operator __op__(const utf8_char_impl<Char, Traits, Alloc>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const basic_utf8_char_view<Char, Traits>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename Char, typename Traits> inline \
	bool operator __op__(const basic_utf8_char_view<Char, Traits>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
							const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
							const string_charset_helper<T1, T2>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String, typename T1, typename T2> inline \
	bool operator __op__(const string_charset_helper<T1, T2>& l, \
							const basic_utf8_char_const_reference<Utf8String>& r) { \
		return !(r.compare(l) __rop__ 0); }

	YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP1(==)
	YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP1(!=)

	YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP2(<, <=)
	YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP2(<=, <)
	YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP2(>, >=)
	YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP2(>=, >)

#undef YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP1
#undef YGGR_TMP_PP_UTF8_CHAR_CONST_REFEREMCE_CMP2

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename Utf8String> inline
string_charset_helper<const basic_utf8_char_const_reference<Utf8String>&, const string&>
	make_string_charset_helper(const basic_utf8_char_const_reference<Utf8String>& src,
								const string& src_charset_name
									= charset_name_t< basic_utf8_char_const_reference<Utf8String> >())
{
	typedef const basic_utf8_char_const_reference<Utf8String>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

	return ret_type(src, src_charset_name);
}

template<typename Utf8String> inline
string_charset_helper<basic_utf8_char_const_reference<Utf8String>&, const string&>
	make_string_charset_helper(basic_utf8_char_const_reference<Utf8String>& src,
								const string& src_charset_name
									= charset_name_t< basic_utf8_char_const_reference<Utf8String> >())
{
	typedef basic_utf8_char_const_reference<Utf8String>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	assert(src_charset_name == YGGR_STR_INNER_STRING_CHARSET_NAME());

	return ret_type(src, src_charset_name);
}

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

#define YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP( __op__ ) \
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8_StringL>& l, \
										const basic_utf8_char_const_reference<Utf8_StringR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8_StringL, typename Utf8_StringR> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8_StringL>& l, \
										const basic_utf8_char_reference<Utf8_StringR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String> inline \
	bool operator __op__(const basic_utf8_char_reference<Utf8String>& l, \
										const basic_utf8_char_const_reference<Utf8String>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Utf8String> inline \
	bool operator __op__(const basic_utf8_char_const_reference<Utf8String>& l, \
										const basic_utf8_char_reference<Utf8String>& r) { \
		return l.compare(r) __op__ 0; }

// ==, !=, <, <=, >, >=
	YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP(==)
	YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP(!=)
	YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP(<)
	YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP(<=)
	YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP(>)
	YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP(>=)

#undef YGGR_TMP_PP_UTF8_CHAR_C_AND_NC_REFERENCE_CMP

} // namespace charset
} // namespace yggr

// boost range_size support
namespace yggr
{
namespace charset
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(1, basic_utf8_char_const_reference)

} // namespace charset
} // namespace yggr

//byte_size support
namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename Utf8String>
struct byte_size_tpl_helper< charset::basic_utf8_char_const_reference<Utf8String> >
	: public boost::mpl::true_
{
public:
	typedef charset::basic_utf8_char_const_reference<Utf8String> value_type;

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

template<typename Utf8String> inline
std::size_t hash_value(const ::yggr::charset::basic_utf8_char_const_reference<Utf8String>& ch)
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

template<typename Utf8String>
struct hash< ::yggr::charset::basic_utf8_char_const_reference<Utf8String> >
{
	typedef ::yggr::charset::basic_utf8_char_const_reference<Utf8String> type;

	inline std::size_t operator()(const type& ch) const
	{
		return ch.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_CHARSET_UTF8_CHAR_REFERENCE_IMPL_HPP__
