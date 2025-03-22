//string_converter.hpp

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

#ifndef __YGGR_CHARSET_STRING_CONVERTER_HPP__
#define __YGGR_CHARSET_STRING_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/container/get_allocator.hpp>
#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/container_ex/allocator_t.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename String_Src, typename String_Dst>
struct conv_helper
{
public:
	typedef String_Src src_type;
	typedef String_Dst dst_type;

	typedef dst_type result_type;
	typedef dst_type& ref_result_type;
	typedef const dst_type& cref_result_type;

	typedef typename allocator_t<dst_type>::type dst_allocator_type;

private:
	typedef conv_helper this_type;

public:
	inline result_type operator()(const src_type& src) const
	{
		return charset_foo::s_xchg<result_type>(src);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc) const
	{
		return charset_foo::s_xchg<result_type>(src, alloc);
	}

	inline result_type operator()(const src_type& src,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		return charset_foo::s_xchg<result_type>(src, charset_src, charset_dst);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		return charset_foo::s_xchg<result_type>(src, alloc, charset_src, charset_dst);
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src) const
	{
		return charset_foo::s_xchg(dst, src);
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		return charset_foo::s_xchg(dst, src, charset_src, charset_dst);
	}
};

template<typename String_Src>
struct conv_helper<String_Src, String_Src>
{
public:
	typedef String_Src src_type;
	typedef String_Src dst_type;

	typedef dst_type result_type;
	typedef dst_type& ref_result_type;
	typedef const dst_type& cref_result_type;

	typedef typename allocator_t<dst_type>::type dst_allocator_type;

private:
	typedef conv_helper this_type;

public:
	inline ref_result_type operator()(src_type& src) const
	{
		return src;
	}

	inline ref_result_type operator()(src_type& src, const dst_allocator_type&) const
	{
		return src;
	}

	inline cref_result_type operator()(const src_type& src) const
	{
		return src;
	}

	inline cref_result_type operator()(const src_type& src, const dst_allocator_type&) const
	{
		return src;
	}

	inline result_type operator()(const src_type& src,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		return charset_foo::s_xchg<result_type>(src, yggr::get_allocator(src), charset_src, charset_dst);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		return charset_foo::s_xchg<result_type>(src, alloc, charset_src, charset_dst);
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src) const
	{
		return charset_foo::s_xchg(dst, src);
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		return charset_foo::s_xchg(dst, src, charset_src, charset_dst);
	}
};

template<typename String_Src, typename Char, typename Traits, typename Alloc>
struct conv_helper<String_Src, utf8_string_impl<Char, Traits, Alloc> >
{
public:
	typedef String_Src src_type;
	typedef utf8_string_impl<Char, Traits, Alloc> dst_type;

	typedef dst_type result_type;
	typedef dst_type& ref_result_type;
	typedef const dst_type& cref_result_type;

	typedef typename container_ex::allocator_t<dst_type>::type dst_allocator_type;

private:
	typedef conv_helper this_type;

public:
	inline result_type operator()(const src_type& src) const
	{
		return result_type(src, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc) const
	{
		return result_type(src, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	inline result_type operator()(const src_type& src,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_dst == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return result_type(src, charset_src);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_dst == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return result_type(src, charset_src);
	}

	// src_type must not be equal dst_type, but compile need this foo, so assert(false)
	inline ref_result_type operator()(dst_type& dst, const src_type& src) const
	{
		return dst.assign(src, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		assert(charset_dst == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		dst.assign(src, charset_src);
		return dst;
	}
};

template<typename Char, typename Traits, typename Alloc,
			typename String_Dst >
struct conv_helper<utf8_string_impl<Char, Traits, Alloc>, String_Dst >
{
public:
	typedef utf8_string_impl<Char, Traits, Alloc> src_type;
	typedef String_Dst dst_type;

	typedef dst_type result_type;
	typedef dst_type& ref_result_type;
	typedef const dst_type& cref_result_type;

	typedef typename container_ex::allocator_t<dst_type>::type dst_allocator_type;

private:
	typedef conv_helper this_type;

public:
	inline result_type operator()(const src_type& src) const
	{
		return charset_foo::s_xchg<result_type>(src.org_str());
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc) const
	{
		return charset_foo::s_xchg<result_type>(src.org_str(), alloc);
	}

	inline result_type operator()(const src_type& src,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return src.template str<dst_type>(charset_dst);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return src.template str<dst_type>(alloc, charset_dst);
	}

	// src_type must not be equal dst_type, but compile need this foo, so assert(false)
	inline ref_result_type operator()(dst_type& dst, const src_type& src) const
	{
		return charset_foo::s_xchg<result_type>(dst, src.org_str());
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		dst = src.template str<dst_type>(dst.get_allocator(), charset_dst);
		return dst;
	}
};

template<typename Char1, typename Traits1, typename Alloc1,
			typename Char2, typename Traits2, typename Alloc2 >
struct conv_helper<utf8_string_impl<Char1, Traits1, Alloc1>,
					utf8_string_impl<Char2, Traits2, Alloc2> >
{
public:
	typedef utf8_string_impl<Char1, Traits1, Alloc1> src_type;
	typedef utf8_string_impl<Char2, Traits2, Alloc2> dst_type;
	BOOST_MPL_ASSERT_NOT((boost::is_same<src_type, dst_type>));

	typedef dst_type result_type;
	typedef dst_type& ref_result_type;
	typedef const dst_type& cref_result_type;

	typedef typename container_ex::allocator_t<dst_type>::type dst_allocator_type;

private:
	typedef conv_helper this_type;

public:
	inline result_type operator()(const src_type& src) const
	{
		return dst_type(src);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc) const
	{
		return dst_type(src, alloc);
	}

	inline result_type operator()(const src_type& src,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return dst_type(src);
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return dst_type(src, alloc);
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src) const
	{
		return dst.assign(src);
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		return dst.assign(src);
	}
};

template<typename Char, typename Traits, typename Alloc >
struct conv_helper<utf8_string_impl<Char, Traits, Alloc>,
					utf8_string_impl<Char, Traits, Alloc> >
{
public:
	typedef utf8_string_impl<Char, Traits, Alloc> src_type;
	typedef utf8_string_impl<Char, Traits, Alloc> dst_type;
	BOOST_MPL_ASSERT((boost::is_same<src_type, dst_type>));

	typedef const dst_type& result_type;
	typedef dst_type& ref_result_type;
	typedef const dst_type& cref_result_type;

	typedef typename container_ex::allocator_t<dst_type>::type dst_allocator_type;

private:
	typedef conv_helper this_type;

public:
	inline result_type operator()(src_type& src) const
	{
		return src;
	}

	inline result_type operator()(src_type& src, const dst_allocator_type&) const
	{
		return src;
	}

	inline result_type operator()(const src_type& src) const
	{
		return src;
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type&) const
	{
		return src;
	}

	inline result_type operator()(src_type& src,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return src;
	}

	inline result_type operator()(src_type& src, const dst_allocator_type& alloc,
									const yggr::string& charset_src,
									const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return src;
	}

	inline result_type operator()(const src_type& src,
											const yggr::string& charset_src,
											const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return src;
	}

	inline result_type operator()(const src_type& src, const dst_allocator_type& alloc,
											const yggr::string& charset_src,
											const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return src;
	}

	inline ref_result_type operator()(dst_type& dst, BOOST_RV_REF(src_type) src) const
	{
		return copy_or_move_or_swap(dst, boost::move(src));
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src) const
	{
		return dst.assign(src);
	}

	inline ref_result_type operator()(dst_type& dst, BOOST_RV_REF(src_type) src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		return copy_or_move_or_swap(dst, boost::move(src));
	}

	inline ref_result_type operator()(dst_type& dst, const src_type& src,
										const yggr::string& charset_src,
										const yggr::string& charset_dst) const
	{
		assert(charset_src == charset_dst);
		assert(charset_src == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		return dst.assign(src);
	}
};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

// this class is a wrapper for charset_foo for string

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! s_conv rule:																											!!!
!!! basic_string_src conv_to basic_string_dst and non-charset_name -> basic_string_dst (byte_copy_only)						!!!
!!! basic_string_src conv_to basic_string_dst and has-charset_name -> basic_string_dst (charset_conv)						!!!
!!!																															!!!
!!! basic_string_src conv_to utf8_string_dst  and non-charset_name -> utf8_string_dst  (byte_copy_only)						!!!
!!! basic_string_src conv_to utf8_string_dst  and has-charset_name -> utf8_string_dst  (charset_conv)						!!!
!!!																															!!!
!!! utf8_string_src  conv_to basic_string_dst and non-charset_name -> basic_string_dst (byte_copy_only)						!!!
!!! utf8_string_src  conv_to basic_string_dst and has-charset_name -> basic_string_dst (charset_conv)						!!!
!!!																															!!!
!!! utf8_string_src  conv_to utf8_string_src  and non-charset_name -> basic_string_dst (byte_copy_only | return reference)	!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

class string_converter
{
private:
	typedef string_converter this_type;

public:
	// same string_type
	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<String_Dst, String_Src>,
			const String_Src&
		>::type
		s_conv(const String_Src& src)
	{
		return src;
	}

	template<typename String_Dst, typename String_Src, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<String_Dst, String_Src>,
				container_ex::is_allocator<Alloc>
			>,
			const String_Dst&
		>::type
		s_conv(const String_Src& src, const Alloc&)
	{
		return src;
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<String_Dst, String_Src>,
			typename detail::conv_helper<String_Src, String_Src>::result_type
		>::type
		s_conv(const String_Src& src,
				const yggr::string& charset_src,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type) )
	{
		typedef String_Src dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst? h(src) : h(src, charset_src, charset_dst);
	}

	template<typename String_Dst, typename String_Src, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<String_Dst, String_Src>,
				container_ex::is_allocator<Alloc>
			>,
			typename detail::conv_helper<String_Src, String_Src>::result_type
		>::type
		s_conv(const String_Src& src, const Alloc& alloc,
				const yggr::string& charset_src,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type) )
	{
		typedef String_Src dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst? h(src, alloc) : h(src, alloc, charset_src, charset_dst);
	}

	// diff string_type of return dst_type
	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::disable_if
		<
			boost::is_same<String_Dst, String_Src>,
			typename detail::conv_helper<String_Src, String_Dst>::result_type
		>::type
		s_conv(const String_Src& src)
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return h(src);
	}

	template<typename String_Dst, typename String_Src, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<String_Dst, String_Src> >,
				container_ex::is_allocator<Alloc>
			>,
			typename detail::conv_helper<String_Src, String_Dst>::result_type
		>::type
		s_conv(const String_Src& src, const Alloc& alloc)
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return h(src, alloc);
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::disable_if
		<
			boost::is_same<String_Dst, String_Src>,
			typename detail::conv_helper<String_Src, String_Dst>::result_type
		>::type
		s_conv(const String_Src& src,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;

		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst? h(src) : h(src, charset_src, charset_dst);
	}

	template<typename String_Dst, typename String_Src, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, String_Src> >,
				container_ex::is_allocator<Alloc>
			>,
			typename detail::conv_helper<String_Src, String_Dst>::result_type
		>::type
		s_conv(const String_Src& src, const Alloc& alloc,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;

		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst? h(src, alloc) : h(src, alloc, charset_src, charset_dst);
	}

	// diff_string type of input_dst
	template<typename String_Dst, typename Char> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, const Char*> >,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<const Char*, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst, const Char* src)
	{
		typedef String_Dst dst_type;
		typedef const Char* src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return h(dst, src);
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, String_Src> >,
				boost::mpl::not_<boost::is_const<String_Dst> >,
				boost::mpl::not_<boost::is_pointer<String_Src> >
			>,
			typename detail::conv_helper<String_Src, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst, BOOST_RV_REF(String_Src) src)
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return h(dst, boost::forward<src_type>(src));
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, String_Src> >,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<String_Src, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst, const String_Src& src)
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return h(dst, src);
	}

	template<typename String_Dst, typename Char> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, const Char*> >,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<const Char*, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst,
				const Char* src,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef const Char* src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst?
				h(dst, src)
				: h(dst, src, charset_src, charset_dst);
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<String_Dst, String_Src>,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<String_Src, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst,
				BOOST_RV_REF(String_Src) src,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst?
				h(dst, boost::forward<src_type>(src))
				: h(dst, boost::forward<src_type>(src), charset_src, charset_dst);
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<String_Dst, String_Src>,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<String_Src, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst,
				const String_Src& src,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst?
				h(dst, src)
				: h(dst, src, charset_src, charset_dst);
	}


	/*
		// see test/charset/string_converter_test.cpp line 86
		code:
		boost_string_type src("abc");
		boost_string_type dst;
		conv_type::s_conv(dst, src, 
							YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type),
							YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type));

		Why is it that in MSVC the above code src and dst are the same, 
		but go into the following function?
		and GCC can go into the correct function.
	*/

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, String_Src> >,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<String_Src, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst,
				BOOST_RV_REF(String_Src) src,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst?
				h(dst, boost::forward<src_type>(src))
				: h(dst, boost::forward<src_type>(src), charset_src, charset_dst);
	}

	template<typename String_Dst, typename String_Src> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_<boost::is_same<String_Dst, String_Src> >,
				boost::mpl::not_<boost::is_const<String_Dst> >
			>,
			typename detail::conv_helper<String_Src, String_Dst>::ref_result_type
		>::type
		s_conv(String_Dst& dst,
				const String_Src& src,
				const yggr::string& charset_src
					/*= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Src>::type)*/,
				const yggr::string& charset_dst
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String_Dst>::type) )
	{
		typedef String_Dst dst_type;
		typedef String_Src src_type;
		typedef detail::conv_helper<src_type, dst_type> h_type;

		h_type h;
		return charset_src == charset_dst?
				h(dst, src)
				: h(dst, src, charset_src, charset_dst);
	}
};


} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_STRING_CONVERTER_HPP__
