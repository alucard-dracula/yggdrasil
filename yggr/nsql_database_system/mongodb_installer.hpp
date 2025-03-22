//mongodb_installer.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_INSTALLER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_INSTALLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/basic_mongodb_config.hpp>

#include <boost/atomic.hpp>

namespace yggr
{
namespace nsql_database_system
{

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

class mongodb_installer 
	: private nonable::noncreateable
{
public:
	// init_flag
	YGGR_STATIC_CONSTANT(u32, E_init_flag = 0);
	YGGR_STATIC_CONSTANT(u32, E_init_flag_network = 0x00000001);
	YGGR_STATIC_CONSTANT(u32, E_init_flag_openssl = 0x00000002);
	
	// append other flag enabled
	
	YGGR_STATIC_CONSTANT(u32, E_init_flag_all = (E_init_flag_network | E_init_flag_openssl));

	// init operator flag
	YGGR_STATIC_CONSTANT(u32, E_chk_non_init = 0);
	YGGR_STATIC_CONSTANT(u32, E_chk_init = 1);
	YGGR_STATIC_CONSTANT(u32, E_chk_uninit = 2);

private:
	typedef boost::atomic<u32> only_once_init_chk_type;
	typedef mongodb_installer this_type;

protected:
	static void pro_s_install(u32 flag,
								const char* driver_name,
								const char* driver_version,
								const char* platform);
public:
	inline static void install(u32 flag = E_init_flag_all)
	{
		return this_type::pro_s_install(flag, 0, 0, 0);
	}

	template<typename String1, typename String2, typename String3>inline 
	static void install(u32 flag,
						const String1& str_driver_name,
						const String2& str_driver_version,
						const String3& str_platform)
	{

		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);
		
		utf8_string1_type utf8_driver_name((charset::string_charset_helper_data(str_driver_name)));
		utf8_string2_type utf8_driver_version((charset::string_charset_helper_data(str_driver_version)));
		utf8_string3_type utf8_platform((charset::string_charset_helper_data(str_platform)));

		return this_type::pro_s_install(flag, utf8_driver_name.data(), utf8_driver_version.data(), utf8_platform.data());
	}

public:
	static void uninstall(u32 flag = E_init_flag_all);
	static void this_thread_clear(void);

private:
	static bool create_network_context(void);
	static bool destroy_network_context(void);

#ifdef MONGOC_ENABLE_SSL
	static void create_openssl_context(void);
	static void destroy_openssl_context(void);
	static void clear_openssl_error(void);
#endif // MONGOC_ENABLE_SSL


private:
	static only_once_init_chk_type _chk; // check init
};

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

} // namespace nsql_database_system
} // namespace yggr


#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_INSTALLER_HPP__
