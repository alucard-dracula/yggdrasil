//mongodb_installer.cpp

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

#include <yggr/nsql_database_system/mongodb_installer.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

mongodb_installer::only_once_init_chk_type mongodb_installer::_chk(false);

/*static*/ 
void mongodb_installer::pro_s_install(u32 flag, 
										const char* driver_name,
										const char* driver_version,
										const char* platform)
{
	u32 old_state = this_type::E_chk_non_init;
	u32 new_state = this_type::E_chk_init;

	if(!this_type::_chk.compare_exchange_strong(old_state, new_state))
	{
		assert(false); // mongodb can only be called once init
		return;
	}

	if(flag & E_init_flag_network)
	{
		bool bsuccess = this_type::create_network_context();
		assert(bsuccess);
	}
#ifdef MONGOC_ENABLE_SSL
	if(flag & E_init_flag_openssl)
	{
		this_type::create_openssl_context();
	}
#endif // MONGOC_ENABLE_SSL

	if((driver_name || driver_version || platform))
	{
		bool bsuccess = mongoc_handshake_data_append(driver_name, driver_version, platform);
		assert(bsuccess);
	}

	::mongoc_init();
}

/*static*/ 
void mongodb_installer::uninstall(u32 flag)
{
	u32 old_state = this_type::E_chk_init;
	u32 new_state = this_type::E_chk_uninit;

	if(!this_type::_chk.compare_exchange_strong(old_state, new_state))
	{
		assert(false); // mongodb can only be called once init
		return;
	}

	::bson_mem_restore_vtable();
	::mongoc_cleanup();

#ifdef MONGOC_ENABLE_SSL
	if(flag & E_init_flag_openssl)
	{
		this_type::destroy_openssl_context();
	}
#endif // MONGOC_ENABLE_SSL

	if(flag & E_init_flag_network)
	{
		this_type::destroy_network_context();
	}
}

/*static*/ 
void mongodb_installer::this_thread_clear(void)
{
#ifdef MONGOC_ENABLE_SSL
	if(this_type::_chk.load())
	{
		clear_openssl_error();
	}
#endif // MONGOC_ENABLE_SSL
}

//private:

/*static*/ 
bool mongodb_installer::create_network_context(void)
{
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	WSADATA out;
    return (0 == ::WSAStartup(MAKEWORD(2,2), &out))
		    && (out.wVersion == 0x0202);
#else
	return true;
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
}

/*static*/ 
bool mongodb_installer::destroy_network_context(void)
{
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	::WSACleanup();
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	return true;
}

#ifdef MONGOC_ENABLE_SSL
/*static*/ 
void mongodb_installer::create_openssl_context(void)
{
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
}

/*static*/ 
void mongodb_installer::destroy_openssl_context(void)
{
	ENGINE_cleanup();
#if !defined(OPENSSL_VERSION_MAJOR)
	FIPS_mode_set(0);
#endif // OPENSSL_VERSION_MAJOR
	CONF_modules_free();
	CONF_modules_unload(1);
	EVP_cleanup();
	SSL_COMP_free_compression_methods();
	COMP_zlib_cleanup();

	// if you thread foo call openssl cxt series function, 
	// the befor end must call this twice foo
	ERR_free_strings();
	ERR_clear_error();
	ERR_remove_state(0);
	ERR_remove_thread_state(0);

	CRYPTO_cleanup_all_ex_data();
}

/*static*/ 
void mongodb_installer::clear_openssl_error(void)
{
	ERR_free_strings();
	ERR_clear_error();
	ERR_remove_state(0);
	ERR_remove_thread_state(0);
}

#endif // MONGOC_ENABLE_SSL

} // namespace nsql_database_system
} // namespace yggr


