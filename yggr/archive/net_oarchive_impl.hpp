//net_oarchive_impl.hpp

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

#ifndef YGGR_ARCHIVE_NET_OARCHIVE_IMPL_HPP
#define YGGR_ARCHIVE_NET_OARCHIVE_IMPL_HPP

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/serialization/pfto.hpp>
#include <boost/archive/basic_binary_oprimitive.hpp>
#include <yggr/archive/impl/basic_binary_oprimitive.hpp>
#include <boost/archive/basic_binary_oarchive.hpp>
#include <yggr/archive/impl/basic_binary_oarchive.hpp>
#include <yggr/serialization/collection_size_type.hpp>

#include <yggr/archive/impl/archive_serializer_map.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/network/hn_conv.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/utility/sao_buffer.hpp>

#include <boost/asio.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_same.hpp>

#include <ostream>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr
{
namespace archive
{

template<typename Archive, typename Elem, typename Tr>
class net_oarchive_impl :
	public boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>,
	public boost::archive::basic_binary_oarchive<Archive>
{
public:
	typedef boost::archive::basic_binary_oprimitive<Archive, Elem, Tr> base_type;

private:
	typedef net_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_oprimitive<
								Archive YGGR_PP_SYMBOL_COMMA() Elem YGGR_PP_SYMBOL_COMMA() Tr>);
	friend class boost::archive::save_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive

	template<class T> inline
	void save_override(T& t YGGR_PFTO_TYPE(int))
	{
		this->boost::archive::basic_binary_oarchive<Archive>::save_override(t YGGR_PFTO_VAR(0L));
	}

	inline void init(u32 flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
#	if !defined(__MWERKS__)
		this->boost::archive::basic_binary_oarchive<Archive>::init();
		this->boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>::init();
#	else
		boost::archive::basic_binary_oarchive<Archive>::init();
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>::init();
#	endif
	}

	net_oarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		u32 flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

	net_oarchive_impl(
		std::basic_ostream<Elem, Tr> & os,
		u32 flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			* os.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

public:
	inline void save(const boost::serialization::collection_size_type& t)
	{
		const std::size_t tmp1 = t;
		ser_size_type tmp = static_cast<const ser_size_type>(tmp1);
		this_type::save(tmp);
	}

	inline void save(const yggr::serialization::collection_size_type& t)
	{
		const ser_size_type tmp = t;
		this_type::save(tmp);
	}

	inline void save(const boost::archive::tracking_type& t)
	{
		bool tmp = t;
		this_type::save(tmp);
	}

	inline void save(const boost::archive::library_version_type& t)
	{
		u16 tmp = t; //gcc must use this style
		this_type::save(tmp);
	}

	inline void save(const boost::archive::version_type& t)
	{
		u32 tmp = t;
		this_type::save(tmp);
	}

	inline void save(const boost::archive::class_id_type& t)
	{
		s16 tmp = t;
		this_type::save(tmp);
	}

	inline void save(const boost::archive::object_id_type& t)
	{
		u32 tmp = t;
		this_type::save(tmp);
	}

	//note:add base type save S

protected:
	template<typename Char> inline
	void pro_save_const_char_pointer_1(const Char* s)
	{
		ser_size_type size = static_cast<ser_size_type>(std::strlen(s));
		assert(size == std::strlen(s));
		this_type::save(size);
		this_type::save_binary(s, size);
	}

public:
	inline void save(const char* s)
	{
		return pro_save_const_char_pointer_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline 
	typename
		boost::enable_if_c
		<
			sizeof(Char) == 1,
			void
		>::type
		save(const std::basic_string<Char, Traits, Alloc>& t)
	{
		ser_size_type size = static_cast<ser_size_type>(t.size());
		this_type::save(size);
		this_type::save_binary(t.data(), size);
	}

	template<typename Char, typename Traits, typename Alloc> inline 
	typename
		boost::enable_if_c
		<
			sizeof(Char) == 1,
			void
		>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& t)
	{
		ser_size_type size = static_cast<ser_size_type>(t.size());
		this_type::save(size);
		this_type::save_binary(t.data(), size);
	}

#ifndef BOOST_NO_CWCHAR

	void save(const wchar_t* ws)
	{
		typedef sao_buffer<wchar_t> sao_buf_type;

		ser_size_type size = static_cast<ser_size_type>(std::wcslen(ws));
		assert(size == std::wcslen(ws));

		sao_buf_type sao_buf(size);

		wchar_t* pstr = sao_buf.buffer_pointer();
		assert(pstr);
		if(!pstr)
		{
			throw boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error);
			return;
		}
	
		ser_size_type byte_size = sizeof(wchar_t) * size;

		if(byte_size)
		{
			memcpy(pstr, ws, byte_size);
			for(ser_size_type i = 0; i != size; ++i)
			{
				pstr[i] = yggr::network::hton(pstr[i]);
			}
		}
		this_type::save(size);
		this_type::save_binary(pstr, byte_size /*/ sizeof(char)*/);
	}

#endif // BOOST_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
	inline void save(const c8* s)
	{
		return pro_save_const_char_pointer_1(s);
	}
#endif // YGGR_NO_CHAR8_T

#if !(defined(YGGR_NO_CHAR16_T) && defined(YGGR_NO_CHAR32_T))
protected:
	template<typename Char> inline
	void pro_save_const_char_pointer_n(const Char* s)
	{
		typedef Char now_char_type;
		typedef std::char_traits<now_char_type> now_char_traits_type;
		typedef sao_buffer<now_char_type> sao_buf_type;

		ser_size_type size = static_cast<ser_size_type>(now_char_traits_type::length(s));
		assert(size == now_char_traits_type::length(s));

		sao_buf_type sao_buf(size);

		now_char_type* pstr = sao_buf.buffer_pointer();
		assert(pstr);
		if(!pstr)
		{
			throw boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error);
			return;
		}
	
		ser_size_type byte_size = sizeof(now_char_type) * size;

		if(byte_size)
		{
			memcpy(pstr, s, byte_size);
			for(ser_size_type i = 0; i != size; ++i)
			{
				pstr[i] = yggr::network::hton(pstr[i]);
			}
		}

		this_type::save(size);
		this_type::save_binary(pstr, byte_size /*/ sizeof(char)*/);
	}

#endif // (defined(YGGR_NO_CHAR16_T) && defined(YGGR_NO_CHAR32_T))

#if !defined(YGGR_NO_CHAR16_T)
	inline void save(const c16* s)
	{
		return pro_save_const_char_pointer_n(s);
	}
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void save(const c32* s)
	{
		return pro_save_const_char_pointer_n(s);
	}
#endif // YGGR_NO_CHAR16_T


#if !(defined(BOOST_NO_STD_WSTRING) && defined(YGGR_NO_U8STRING_T) \
		&& defined(YGGR_NO_U16STRING_T) && defined(YGGR_NO_U32STRING_T))

	template<typename Char, typename Traits, typename Alloc>
	typename
		boost::disable_if_c
		<
			sizeof(Char) == 1,
			void
		>::type
		save(const std::basic_string<Char, Traits, Alloc>& t)
	{
		typedef std::basic_string<Char, Traits, Alloc> now_string_type;
		now_string_type str(t);
		ser_size_type size = static_cast<ser_size_type>(str.size());

		for(ser_size_type i = 0; i != size; ++i)
		{
			str[i] = yggr::network::hton(str[i]);
		}
		this_type::save(size);
		this_type::save_binary(str.data(), size * sizeof(Char) / sizeof(char));
	}

	template<typename Char, typename Traits, typename Alloc>
	typename
		boost::disable_if_c
		<
			sizeof(Char) == 1,
			void
		>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& t)
	{
		typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
		now_string_type str(t);
		ser_size_type size = static_cast<ser_size_type>(str.size());

		for(ser_size_type i = 0; i != size; ++i)
		{
			str[i] = yggr::network::hton(str[i]);
		}
		this_type::save(size);
		this_type::save_binary(str.data(), size * sizeof(Char) / sizeof(char));
	}

#endif // BOOST_NO_STD_WSTRING

	inline void save(bool t)
	{
		this_type::save_binary(&t, sizeof(bool));
	}

	inline void save(s8 t)
	{
		this_type::save_binary(&t, sizeof(s8));
	}

	inline void save(char t)
	{
		this_type::save_binary(&t, sizeof(char));
	}

	inline void save(u8 t)
	{
		this_type::save_binary(&t, sizeof(u8));
	}

#if !defined(YGGR_NO_CWCHAR)

	inline void save(wchar_t t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(wchar_t));
	}

#endif // BOOST_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)

	inline void save(c8 t)
	{
		this_type::save_binary(&t, sizeof(c8));
	}

#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	
	inline void save(c16 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(c16));
	}

#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)

	inline void save(c32 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(c32));
	}

#endif // YGGR_NO_CHAR32_T

	inline void save(s16 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(s16));
	}

	inline void save(u16 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(u16));
	}

	inline void save(s32 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(s32));
	}

	inline void save(u32 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(u32));
	}

	inline void save(s64 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(s64));
	}

	inline void save(u64 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(u64));
	}

	inline void save(f32 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(f32));
	}

	inline void save(f64 t)
	{
		t = yggr::network::hton(t);
		this_type::save_binary(&t, sizeof(f64));
	}

	//note:add base type save E

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

#endif // YGGR_ARCHIVE_NET_OARCHIVE_IMPL_HPP
