//mongo_utility.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_UTILITY_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_UTILITY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_utility
{
public:
	typedef c_bson bson_type;

private:
	typedef mongo_utility this_type;

protected:
	template<typename Utf8String> inline
	static Utf8String& pro_s_key_to_index_string(Utf8String& ret, const bson_t& keys)
	{
		typedef Utf8String ret_type;

		char* pstr = mongoc_collection_keys_to_index_string(&keys);

		if(pstr)
		{
			ret.assign(pstr, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}

		bson_free(pstr);
		return ret;
	}

public:
	// keys_to_index_string return utf8_string
	template<typename Bson> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, utf8_string>::type
		keys_to_index_string(const Bson& keys)
	{
		typedef charset::utf8_string ret_type;

		ret_type ret;
		this_type::pro_s_key_to_index_string(ret, keys);
		return ret;
	}

	template<typename Bson> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, utf8_string>::type
		keys_to_index_string(const Bson& keys, const string& /*ret_charset_name*/)
	{
		typedef charset::utf8_string ret_type;

		ret_type ret;
		this_type::pro_s_key_to_index_string(ret, keys);
		return ret;
	}

	template<typename Bson, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			charset::utf8_string_impl
			<
				charset::utf8_string::value_type,
				charset::utf8_string::traits_type,
				Alloc
			>
		>::type
		keys_to_index_string(const Bson& keys, const Alloc& alloc)
	{
		typedef
			charset::utf8_string_impl
			<
				charset::utf8_string::value_type,
				charset::utf8_string::traits_type,
				Alloc
			> ret_type;

		ret_type ret(alloc);
		this_type::pro_s_key_to_index_string(ret, keys);
		return ret;
	}

	template<typename Bson, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			charset::utf8_string_impl
			<
				charset::utf8_string::value_type,
				charset::utf8_string::traits_type,
				Alloc
			>
		>::type
		keys_to_index_string(const Bson& keys, const Alloc& alloc, const string& /*ret_charset_name*/)
	{
		typedef
			charset::utf8_string_impl
			<
				charset::utf8_string::value_type,
				charset::utf8_string::traits_type,
				Alloc
			> ret_type;

		ret_type ret(alloc);
		this_type::pro_s_key_to_index_string(ret, keys);
		return ret;
	}

	// keys_to_index_string return String
	template<typename String, typename Bson> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
		keys_to_index_string(const Bson& keys)
	{
		typedef String ret_type;

		utf8_string ret;
		return (this_type::pro_s_key_to_index_string(ret, keys)).template str<ret_type>();
	}

	template<typename String, typename Bson> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
		keys_to_index_string(const Bson& keys, const string& ret_charset_name)
	{
		typedef String ret_type;

		utf8_string ret;
		return this_type::pro_s_key_to_index_string(ret, keys).template str<ret_type>(ret_charset_name);
	}

	template<typename String, typename Bson, typename Alloc> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
		keys_to_index_string(const Bson& keys, const Alloc& alloc)
	{
		typedef String ret_type;

		utf8_string ret;
		return this_type::pro_s_key_to_index_string(ret, keys).template str<ret_type>(alloc);
	}

	template<typename String, typename Bson, typename Alloc> inline
	static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
		keys_to_index_string(const Bson& keys, const Alloc& alloc, const string& ret_charset_name)
	{
		typedef String ret_type;

		utf8_string ret;
		return this_type::pro_s_key_to_index_string(ret, keys).template str<ret_type>(alloc, ret_charset_name);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGO_UTILITY_HPP__
