//text_iarchive.hpp

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

#ifndef __YGGR_ARCHIVE_TEXT_IARCHIVE_HPP__
#define __YGGR_ARCHIVE_TEXT_IARCHIVE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/archive/text_iarchive.hpp>
#include <yggr/archive/impl/basic_text_iprimitive.hpp>
#include <yggr/archive/impl/basic_text_iarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/text_iarchive_impl.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/serialization/collection_size_type.hpp>

#include <yggr/ppex/friend.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_same.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr 
{ 
namespace archive 
{

template<class Archive>
class text_iarchive_impl
	: public boost::archive::text_iarchive_impl<Archive>
{
public:
	typedef boost::archive::text_iarchive_impl<Archive> base_type;

private:
	typedef text_iarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_iarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_text_iarchive<Archive>);
	friend class boost::archive::basic_text_iprimitive<std::istream>;
	friend class boost::archive::load_access;
protected:
#endif

	text_iarchive_impl(std::istream & is, unsigned int flags)
		: base_type(is, flags)
	{
	}

	~text_iarchive_impl(void)
	{
	}

public:
	using base_type::load;

protected:
	template<typename Char> inline
	void pro_load_const_char_pointer_1n(Char* s)
	{
		typedef Char now_char_type;
		ser_size_type size = 0;
		*base_type::This() >> size;
		// skip separating space
		base_type::is.get();
		base_type::is.read(reinterpret_cast<char *>(s), size * sizeof(now_char_type) /*/ sizeof(char)*/);
		s[size] = static_cast<now_char_type>(0);
	}

public:
	inline void load(char* s)
	{
		return pro_load_const_char_pointer_1n(s);
	}

	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<char, Traits, Alloc>& s)
	{
		ser_size_type size = 0;
		*base_type::This() >> size;
		// skip separating space
		base_type::is.get();
		// borland de-allocator fixup
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != s.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			s.resize(size);
		if(0 < size)
			base_type::is.read(boost::addressof(*s.begin()), size);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<char, Traits, Alloc>& s)
	{
		ser_size_type size = 0;
		*base_type::This() >> size;
		// skip separating space
		base_type::is.get();
		// borland de-allocator fixup
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != s.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			s.resize(size);
		if(0 < size)
			base_type::is.read(boost::addressof(*s.begin()), size);
	}

#ifndef BOOST_NO_CWCHAR
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
	inline void load(wchar_t* ws)
	{
		return pro_load_const_char_pointer_1n(ws);
	}
#endif // BOOST_NO_INTRINSIC_WCHAR_T
#endif // BOOST_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
	// !!!oarchive not need, because oarchive using save_array_type!!!
	inline void load(c8& t)
    {
        s16 i;
        base_type::load(i);
        t = i;
    }

	inline void load(c8* s)
	{
		return pro_load_const_char_pointer_1n(s);
	}
#endif //YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
    inline void load(c16& t)
    {
        s32 i;
        load(i);
        t = i;
    }

	inline void load(c16* s)
	{
		return pro_load_const_char_pointer_1n(s);
	}
#endif //YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void load(c32& t)
    {
        s32 i;
        load(i);
        t = i;
    }

	inline void load(c32* s)
	{
		return pro_load_const_char_pointer_1n(s);
	}
#endif //YGGR_NO_CHAR32_T

#if !(defined(BOOST_NO_STD_WSTRING) && defined(YGGR_NO_U8STRING_T) \
		&& defined(YGGR_NO_U16STRING_T) && defined(YGGR_NO_U32STRING_T))

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::is_same< Char, char >,
			void
		>::type
		load(std::basic_string<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;

		ser_size_type size = 0;
		*base_type::This() >> size;
		// borland de-allocator fixup
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != ws.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			ws.resize(size);
		// skip separating space
		base_type::is.get();
		base_type::is.read(reinterpret_cast<char *>(const_cast<now_char_type*>(ws.data())), 
					size * sizeof(now_char_type) /*/ sizeof(char)*/);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::is_same< Char, char >,
			void
		>::type
		load(boost::container::basic_string<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;

		ser_size_type size = 0;
		*base_type::This() >> size;
		// borland de-allocator fixup
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != ws.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			ws.resize(size);
		// skip separating space
		base_type::is.get();
		base_type::is.read(reinterpret_cast<char *>(const_cast<now_char_type*>(ws.data())), 
					size * sizeof(now_char_type)/* / sizeof(char)*/);
	}

#endif // BOOST_NO_STD_WSTRING

	inline void load(boost::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		base_type::load(tmp);
		t = boost::serialization::collection_size_type(tmp);
	}

	inline void load(yggr::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		base_type::load(tmp);
		t = yggr::serialization::collection_size_type(tmp);
	}
};

class naked_text_iarchive 
	: public yggr::archive::text_iarchive_impl<yggr::archive::naked_text_iarchive>
{
public:
	typedef std::istream::char_type char_type;
	typedef std::istream::traits_type traits_type;
	
	typedef yggr::archive::text_iarchive_impl<yggr::archive::naked_text_iarchive> base_type;
private:
	typedef naked_text_iarchive this_type;
public:
	naked_text_iarchive(std::istream & is_, unsigned int flags = 0) 
		: base_type(is_, flags)
	{
	}

	~naked_text_iarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC


#if BOOST_VERSION < 105600
#	include <boost/archive/shared_ptr_helper.hpp>
#endif // BOOST_VERSION < 105600

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr 
{ 
namespace archive 
{

class text_iarchive 
	: public yggr::archive::text_iarchive_impl<yggr::archive::text_iarchive>
#if BOOST_VERSION < 105600
		, public boost::archive::detail::shared_ptr_helper
#endif // BOOST_VERSION < 105600
{
public:
	typedef std::istream::char_type char_type;
	typedef std::istream::traits_type traits_type;
	
	typedef yggr::archive::text_iarchive_impl<yggr::archive::text_iarchive> base_type;
private:
	typedef text_iarchive this_type;

public:
	text_iarchive(std::istream & is_, unsigned int flags = 0)
		: base_type(is_, flags)
	{
	}

	~text_iarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::text_iarchive)

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC


// boost::archive::detail::load_non_pointer_type patch
#include <yggr/archive/detail/iserializer_patch.hpp>

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::naked_text_iarchive
	YGGR_PP_BOOST_ISERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::text_iarchive
	YGGR_PP_BOOST_ISERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#endif // __YGGR_ARCHIVE_TEXT_IARCHIVE_HPP__
