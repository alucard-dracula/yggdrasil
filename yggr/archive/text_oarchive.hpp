//text_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_TEXT_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_TEXT_OARCHIVE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/archive/text_oarchive.hpp>

#include <yggr/archive/impl/basic_text_oprimitive.hpp>
#include <yggr/archive/impl/basic_text_oarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/text_oarchive_impl.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/serialization/collection_size_type.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/charset/string.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr
{
namespace archive
{

template<typename Archive>
class text_oarchive_impl
	: public boost::archive::text_oarchive_impl<Archive>
{

public:
	typedef boost::archive::text_oarchive_impl<Archive> base_type;

private:
	typedef text_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_text_oarchive<Archive>);
	friend class boost::archive::basic_text_oprimitive<std::ostream>;
	friend class boost::archive::save_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	text_oarchive_impl(std::ostream & os, unsigned int flags)
		: base_type(os, flags)
	{
	}

	~text_oarchive_impl(void)
	{
	}

	using base_type::save;

	inline void save(const char* s)
	{
		const ser_size_type len = static_cast<ser_size_type>(std::ostream::traits_type::length(s));
		*base_type::This() << len;
		base_type::This()->newtoken();
		base_type::os << s;
	}

	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<char, Traits, Alloc>& s)
	{
		const ser_size_type size = static_cast<ser_size_type>(s.size());
		*base_type::This() << size;
		base_type::This()->newtoken();
		base_type::os << s;
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<char, Traits, Alloc>& s)
	{
		const ser_size_type size = static_cast<ser_size_type>(s.size());
		*base_type::This() << size;
		base_type::This()->newtoken();
		base_type::os << s;
	}

#if !(defined(YGGR_NO_CWCHAR) \
		&& defined(BOOST_NO_INTRINSIC_WCHAR_T) \
		&& defined(YGGR_NO_CHAR8_T) \
		&& defined(YGGR_NO_CHAR16_T) \
		&& defined(YGGR_NO_CHAR32_T))

protected:
	template<typename Char> inline
	void pro_save_const_char_pointer_1n(const Char* s)
	{
		typedef Char now_char_type;
		typedef std::char_traits<now_char_type> now_char_tratis_type;
		const ser_size_type l = static_cast<ser_size_type>(now_char_tratis_type::length(s));
		*base_type::This() << l;
		base_type::This()->newtoken();
		base_type::os.write(reinterpret_cast<const char*>(s), l * sizeof(now_char_type)/* / sizeof(char)*/);
	}

#endif // #if !(defined(YGGR_NO_CWCHAR) && defined(BOOST_NO_INTRINSIC_WCHAR_T) && defined(YGGR_NO_CHAR8_T) && defined(YGGR_NO_CHAR16_T) && defined(YGGR_NO_CHAR32_T))

public:

#ifndef BOOST_NO_CWCHAR
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
	inline void save(const wchar_t* ws)
	{
		return pro_save_const_char_pointer_1n(ws);
	}
#endif // BOOST_NO_INTRINSIC_WCHAR_T
#endif // BOOST_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
	inline void save(const c8& t)
	{
		base_type::save( static_cast<s16>(t));
	}

	inline void save(const c8* s)
	{
		return pro_save_const_char_pointer_1n(s);
	}
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	inline void save(const c16& t)
	{
		base_type::save(static_cast<s32>(t));
	}

	inline void save(const c16* s)
	{
		return pro_save_const_char_pointer_1n(s);
	}
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void save(const c32& t)
	{
		base_type::save(static_cast<s32>(t));
	}

	inline void save(const c32* s)
	{
		return pro_save_const_char_pointer_1n(s);
	}
#endif // YGGR_NO_CHAR32_T

#if !(defined(BOOST_NO_STD_WSTRING) && defined(YGGR_NO_U8STRING_T) \
		&& defined(YGGR_NO_U16STRING_T) && defined(YGGR_NO_U32STRING_T))

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::is_same<Char, char>,
			void
		>::type
		save(const std::basic_string<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;

		const ser_size_type l = static_cast<ser_size_type>(ws.size());
		*base_type::This() << l;
		base_type::This()->newtoken();
		base_type::os.write(reinterpret_cast<const char *>(const_cast<const Char*>(ws.data())), 
					l * sizeof(now_char_type) /*/ sizeof(char)*/);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::is_same<Char, char>,
			void
		>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;

		const ser_size_type l = static_cast<ser_size_type>(ws.size());
		*base_type::This() << l;
		base_type::This()->newtoken();
		base_type::os.write(reinterpret_cast<const char *>(const_cast<const now_char_type*>(ws.data())), 
					l * sizeof(now_char_type)/* / sizeof(char)*/);
	}

#endif // BOOST_NO_STD_WSTRING

	inline void save(const boost::serialization::collection_size_type& t)
	{
		const std::size_t tmp1 = t;
		ser_size_type tmp = static_cast<const ser_size_type>(tmp1);
		base_type::save(tmp);
	}

	inline void save(const yggr::serialization::collection_size_type& t)
	{
		const ser_size_type tmp = t;
		base_type::save(tmp);
	}
};

class text_oarchive
	: public yggr::archive::text_oarchive_impl<yggr::archive::text_oarchive>
{
public:
	typedef std::ostream::char_type char_type;
	typedef std::ostream::traits_type traits_type;

	typedef yggr::archive::text_oarchive_impl<yggr::archive::text_oarchive> base_type;

private:
	typedef text_oarchive this_type;

public:
	text_oarchive(std::ostream & os_, unsigned int flags = 0)
		: base_type(os_, flags)
	{
	}

	~text_oarchive(void)
	{
	}
};

typedef text_oarchive naked_text_oarchive;

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::text_oarchive)

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

// boost::archive::detail::save_non_pointer_type patch
#include <yggr/archive/detail/oserializer_patch.hpp>

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::text_oarchive
	YGGR_PP_BOOST_OSERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#endif // __YGGR_ARCHIVE_TEXT_OARCHIVE_HPP__
