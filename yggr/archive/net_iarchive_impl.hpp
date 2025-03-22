//net_iarchive_impl.hpp
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

#ifndef __YGGR_ARCHIVE_NET_IARCHIVE_IMPL_HPP__
#define __YGGR_ARCHIVE_NET_IARCHIVE_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/serialization/pfto.hpp>
#include <boost/archive/basic_binary_iprimitive.hpp>
#include <yggr/archive/impl/basic_binary_iprimitive.hpp>
#include <boost/archive/basic_binary_iarchive.hpp>
#include <yggr/archive/impl/basic_binary_iarchive.hpp>
#include <yggr/serialization/collection_size_type.hpp>

#include <yggr/archive/impl/archive_serializer_map.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/network/hn_conv.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/symbols.hpp>

#include <boost/asio.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_same.hpp>

#include <istream>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr 
{
namespace archive
{

template<typename Archive, typename Elem, typename Tr>
class net_iarchive_impl :
	public boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>,
	public boost::archive::basic_binary_iarchive<Archive>
{
private:
	typedef net_iarchive_impl this_type;
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_iarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_iarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_iprimitive<
								Archive YGGR_PP_SYMBOL_COMMA() Elem YGGR_PP_SYMBOL_COMMA() Tr>);
	friend class boost::archive::load_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive

	template<class T> inline
	void load_override(T& t YGGR_PFTO_TYPE(int))
	{
		this->boost::archive::basic_binary_iarchive<Archive>::load_override(t YGGR_PFTO_VAR(0L));
	}

	inline void init(u32 flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
#	if !defined(__MWERKS__)
		this->boost::archive::basic_binary_iarchive<Archive>::init();
		this->boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>::init();
#	else
		boost::basic_binary_iarchive<Archive>::init();
		boost::basic_binary_iprimitive<Archive, Elem, Tr>::init();
#	endif
	}

	net_iarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		u32 flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}

	net_iarchive_impl(
		std::basic_istream<Elem, Tr> & is,
		u32 flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			* is.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}

public:
	inline void load(boost::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		this_type::load(tmp);
		t = boost::serialization::collection_size_type(tmp);
	}

	inline void load(yggr::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		this_type::load(tmp);
		t = yggr::serialization::collection_size_type(tmp);
	}

	inline void load(boost::archive::tracking_type& t)
	{
		bool tmp = false;
		this_type::load(tmp);
		t = boost::archive::tracking_type(tmp);
	}

	inline void load(boost::archive::library_version_type& t)
	{
		u16 tmp = 0;
		this_type::load(tmp);
		t = boost::archive::library_version_type(tmp);
	}

	inline void load(boost::archive::version_type& t)
	{
		u32 tmp = 0;
		this_type::load(tmp);
		t = boost::archive::version_type(tmp);
	}

	inline void load(boost::archive::class_id_type& t)
	{
		s16 tmp = 0;
		this_type::load(tmp);
		t = boost::archive::class_id_type(tmp);
	}

	inline void load(boost::archive::object_id_type& t)
	{
		u32 tmp = t;
		this_type::load(tmp);
		t = boost::archive::object_id_type(tmp);
	}

	//note: add base type load S

	inline void load(char* s)
	{
		ser_size_type size = 0;
		this_type::load(size);
		this_type::load_binary(s, size);
		s[size] = '\0';
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >,
			void
		>::type
		load(std::basic_string<Char, Traits, Alloc>& t)
	{
		ser_size_type size = 0;
		this_type::load(size);

#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != t.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			t.resize(size);

		if(0 < size)
			this_type::load_binary(static_cast<void*>(const_cast<Char*>(t.data())), size);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >,
			void
		>::type
		load(boost::container::basic_string<Char, Traits, Alloc>& t)
	{
		ser_size_type size = 0;
		this_type::load(size);

#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != t.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			t.resize(size);

		if(0 < size)
			this_type::load_binary(static_cast<void*>(const_cast<Char*>(t.data())), size);
	}

#ifndef BOOST_NO_CWCHAR
	void load(wchar_t* ws)
	{
		ser_size_type size = 0; // number of wchar_t !!!
		this_type::load(size);
		this_type::load_binary(static_cast<void*>(ws), size * sizeof(wchar_t) /*/ sizeof(char)*/);
		for(ser_size_type i = 0; i != size; ++i)
		{
			ws[i] = yggr::network::ntoh(ws[i]);
		}

		ws[size] = L'\0';
	}
#endif // BOOST_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
	inline void load(c8* s)
	{
		ser_size_type size = 0;
		this_type::load(size);
		this_type::load_binary(s, size);
		s[size] = (u8'\0');
	}
#endif //YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	inline void load(c16* s)
	{
		ser_size_type size = 0; // number of c16 !!!
		this_type::load(size);
		this_type::load_binary(static_cast<void*>(s), size * sizeof(c16) /*/ sizeof(char)*/);
		for(ser_size_type i = 0; i != size; ++i)
		{
			s[i] = yggr::network::ntoh(s[i]);
		}

		s[size] = u'\0';
	}
#endif //YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void load(c32* s)
	{
		ser_size_type size = 0; // number of c32 !!!
		this_type::load(size);
		this_type::load_binary(static_cast<void*>(s), size * sizeof(c32) /*/ sizeof(char)*/);
		for(ser_size_type i = 0; i != size; ++i)
		{
			s[i] = yggr::network::ntoh(s[i]);
		}

		s[size] = U'\0';
	}
#endif //YGGR_NO_CHAR32_T


#if !(defined(BOOST_NO_STD_WSTRING) && defined(YGGR_NO_U8STRING_T) \
		&& defined(YGGR_NO_U16STRING_T) && defined(YGGR_NO_U32STRING_T))

	// why using
	// sizeof(Char) == 1,
	// !!!because boost::disable_if_c< boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >, void >
	// msvc is failed !!!

	template<typename Char, typename Traits, typename Alloc> 
	typename
		boost::disable_if_c
		<
			sizeof(Char) == 1,
			void
		>::type
		load(std::basic_string<Char, Traits, Alloc>& t)
	{
		typedef Char now_char_type;

		ser_size_type size = 0;
		this_type::load(size);

#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != t.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			t.resize(size);

		this_type::load_binary(const_cast<now_char_type *>(t.data()), size * sizeof(now_char_type) /*/ sizeof(char)*/);

		for(ser_size_type i = 0; i < size; ++i)
		{
			t[i] = yggr::network::ntoh(t[i]);
		}
	}

	template<typename Char, typename Traits, typename Alloc>
	typename
		boost::disable_if_c
		<
			sizeof(Char) == 1,
			void
		>::type
		load(boost::container::basic_string<Char, Traits, Alloc>& t)
	{
		typedef Char now_char_type;

		ser_size_type size = 0;
		this_type::load(size);

#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != t.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			t.resize(size);

		this_type::load_binary(const_cast<now_char_type *>(t.data()), size * sizeof(now_char_type) /*/ sizeof(char)*/);

		for(ser_size_type i = 0; i < size; ++i)
		{
			t[i] = yggr::network::ntoh(t[i]);
		}
	}

#endif // BOOST_NO_STD_WSTRING

	inline void load(bool& t)
	{
		this_type::load_binary(&t, sizeof(bool));
	}

	inline void load(s8& t)
	{
		this_type::load_binary(&t, sizeof(s8));
	}

	inline void load(char& t)
	{
		this_type::load_binary(&t, sizeof(char));
	}

	inline void load(u8& t)
	{
		this_type::load_binary(&t, sizeof(u8));
	}

#if !defined(YGGR_NO_CWCHAR)

	inline void load(wchar_t& t)
	{
		this_type::load_binary(&t, sizeof(wchar_t));
		t = yggr::network::ntoh(t);
	}

#endif // BOOST_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)

	inline void load(c8& t)
	{
		this_type::load_binary(&t, sizeof(c8));
	}

#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	
	inline void load(c16& t)
	{
		this_type::load_binary(&t, sizeof(c16));
		t = yggr::network::ntoh(t);
	}

#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void load(c32& t)
	{
		this_type::load_binary(&t, sizeof(c32));
		t = yggr::network::ntoh(t);
	}
#endif // YGGR_NO_CHAR32_T

	inline void load(s16& t)
	{
		this_type::load_binary(&t, sizeof(s16));
		t = yggr::network::ntoh(t);
	}

	inline void load(u16& t)
	{
		this_type::load_binary(&t, sizeof(u16));
		t = yggr::network::ntoh(t);
	}

	inline void load(s32& t)
	{
		this_type::load_binary(&t, sizeof(s32));
		t = yggr::network::ntoh(t);
	}

	inline void load(u32& t)
	{
		this_type::load_binary(&t, sizeof(u32));
		t = yggr::network::ntoh(t);
	}

	inline void load(s64& t)
	{
		this_type::load_binary(&t, sizeof(s64));
		t = yggr::network::ntoh(t);
	}

	inline void load(u64& t)
	{
		this_type::load_binary(&t, sizeof(u64));
		t = yggr::network::ntoh(t);
	}

	inline void load(f32& t)
	{
		this_type::load_binary(&t, sizeof(f32));
		t = yggr::network::ntoh(t);
	}

	inline void load(f64& t)
	{
		this_type::load_binary(&t, sizeof(f64));
		t = yggr::network::ntoh(t);
	}
	//note: add base type load E

public:
	struct use_array_optimization 
	{
		template <typename T>  
#		if defined(BOOST_NO_DEPENDENT_NESTED_DERIVATIONS)  
			struct apply 
			{  
				typedef boost::mpl::bool_<sizeof(T) == 1> type;  
			};
#		else
			struct apply : public boost::mpl::bool_<sizeof(T) == 1> {};  
#		endif // BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
	};
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#endif // __YGGR_ARCHIVE_NET_IARCHIVE_IMPL_HPP__
