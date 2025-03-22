//c_mongo_file_stream.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_FILE_STREAM_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_FILE_STREAM_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string_converter.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/nsql_database_system/c_mongo_basic_stream.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_file_stream
	: public c_mongo_basic_stream
{
public:
	typedef c_mongo_basic_stream base_type;
	typedef mongoc_stream_file_t org_type;
	typedef base_type::org_type base_org_type;
	typedef base_type::stream_ptr_type stream_ptr_type;

private:
	typedef c_mongo_file_stream this_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(c_mongo_file_stream) // not using this_type cpp20 has issue

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_mongo_file_stream(void);

	c_mongo_file_stream(base_org_type* pstream);
	c_mongo_file_stream(base_org_type* pstream, bool need_del);

	c_mongo_file_stream(int fd);
	c_mongo_file_stream(const char* fpath, int flags = O_RDONLY, int mode = 0);

	template<typename Char>
	c_mongo_file_stream(const Char* fpath, int flags = O_RDONLY, int mode = 0,
						typename
							boost::enable_if
							<
								is_native_char<Char>,
								sfinae_type
							>::type sfinae = 0)
	{
		typedef yggr::charset::basic_string<Char> src_string_type;
		typedef yggr::string dst_string_type;

		dst_string_type dst_path;
		charset::string_converter::s_conv(dst_path, fpath);

		base_type::pro_init(mongoc_stream_file_new_for_path(dst_path.data(), flags, mode), true);
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	c_mongo_file_stream(const Basic_String<char, Traits, Alloc>& fpath, int flags = O_RDONLY, int mode = 0,
							typename
							boost::enable_if
							<
								charset::is_string_t< Basic_String<char, Traits, Alloc> >,
								sfinae_type
							>::type sfinae = 0)
		: base_type(mongoc_stream_file_new_for_path(fpath.data(), flags, mode))
	{
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	c_mongo_file_stream(const Basic_String<Char, Traits, Alloc>& fpath, int flags = O_RDONLY, int mode = 0,
						typename
							boost::enable_if
							<
								charset::is_string_t< Basic_String<Char, Traits, Alloc> >,
								sfinae_type
							>::type sfinae = 0)
	{
		typedef Basic_String<Char, Traits, Alloc> src_string_type;
		typedef yggr::string dst_string_type;

		dst_string_type dst_path;
		charset::string_converter::s_conv(dst_path, fpath);

		base_type::pro_init(mongoc_stream_file_new_for_path(dst_path.data(), flags, mode), true);
	}

	template<typename Char, typename Traits, typename Alloc >
	c_mongo_file_stream(const charset::utf8_string_impl<Char, Traits, Alloc>& fpath,
							int flags = O_RDONLY, int mode = 0)
		: base_type(
			mongoc_stream_file_new_for_path(
#	if defined(YGGR_AT_WINDOWS)
		fpath.template str<yggr::string>().data(),
#	else
		fpath.data(),
#	endif // YGGR_AT_WINDOWS
				flags,
				mode))

	{
	}

	c_mongo_file_stream(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	~c_mongo_file_stream(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::pro_move(right_ref);

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void) const
	{
		return reinterpret_cast<org_type*>(base_type::org_pointer());
	}

public:
	inline operator org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	inline int fd(void) const
	{
		org_type* ptr = this_type::org_pointer();
		return ptr? mongoc_stream_file_get_fd(ptr) : 0;
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_file_stream)
} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_FILE_STREAM_HPP__
