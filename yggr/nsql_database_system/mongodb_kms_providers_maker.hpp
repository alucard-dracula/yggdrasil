// mongodb_kms_providers_maker.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_KMS_PROVIDERS_MAKER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_KMS_PROVIDERS_MAKER_HPP__

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongodb_kms_providers_maker
{
public:
	typedef c_bson bson_type;

	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

private:
	typedef mongodb_kms_providers_maker this_type;

public:
	inline static const inner_string_type& s_kms_providers_mode_name_local(void)
	{
		static const charset::utf8_string s_str_kms_providers_mode("local", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_str_kms_providers_mode;
	}

	inline static const inner_string_type& s_kms_providers_mode_name_aws(void)
	{
		static const charset::utf8_string s_str_kms_providers_mode("aws", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_str_kms_providers_mode;
	}

	inline static const inner_string_type& s_kms_providers_mode_name_azure(void)
	{
		static const charset::utf8_string s_str_kms_providers_mode("azure", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_str_kms_providers_mode;
	}

	inline static const inner_string_type& s_kms_providers_mode_name_gcp(void)
	{
		static const charset::utf8_string s_str_kms_providers_mode("gcp", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_str_kms_providers_mode;
	}
	
	inline static const inner_string_type& s_kms_providers_mode_name_kmip(void)
	{
		static const charset::utf8_string s_str_kms_providers_mode("kmip", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_str_kms_providers_mode;
	}

public:
	template<typename Bson, typename BinaryBuffer> inline
	static 
		typename
			boost::enable_if
			<
				boost::is_base_of<bson_t, Bson>,
				Bson&
			>::type
			s_make_local_kms_providers(Bson& bs_kms_providers, const BinaryBuffer& binary_buffer)
	{
		static const charset::utf8_string s_str_key("key", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		
		bson_type& out_ref = reinterpret_cast<bson_type&>(bs_kms_providers);

		bson_mark_splice sp;
		out_ref.save_document_start(this_type::s_kms_providers_mode_name_local(), sp);
		out_ref.save(s_str_key, binary_buffer);
		out_ref.save_document_end(sp);

		return bs_kms_providers;
	}

	template<typename BinaryBuffer> inline
	static bson_type s_make_local_kms_providers(const BinaryBuffer& binary_buffer)
	{
		bson_type out;
		this_type::s_make_local_kms_providers(out, binary_buffer);
		return out;
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_KMS_PROVIDERS_MAKER_HPP__
