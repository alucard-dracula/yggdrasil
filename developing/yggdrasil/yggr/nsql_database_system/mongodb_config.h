//mongodb_config.h

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_CONFIG_H__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_CONFIG_H__

#if	defined(WIN32) || defined(WIN64) || defined(WINDOS)
#	define MONGO_USE_LONG_LONG_INT
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOS)

#define BUILD_MONGODB_DRIVER_YGGR_VER

#include <bson.h>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/network/socket_conflict_fixer.hpp>
#include <mongo.h>


#if defined(WIN32) || defined(WIN64) || defined(WINDOS)
#	include <windows.h>
#   if defined(_MSC_VER)
#	    if (_MSC_VER == 1600)
#		    pragma comment(lib, "Ws2_32.lib")
#		    if defined(_DEBUG)
#		    	pragma comment(lib, "libbson-vc100-d.lib")
#			    pragma comment(lib, "libmongoc-vc100-d.lib")
#		    else
#			    pragma comment(lib, "libbson-vc100.lib")
#			    pragma comment(lib, "libmongoc-vc100.lib")
#		    endif //_DEBUG
#	    elif (_MSC_VER == 1500)
#		    pragma comment(lib, "Ws2_32.lib")
#		    if defined(_DEBUG)
#			    pragma comment(lib, "libbson-vc90-d.lib")
#			    pragma comment(lib, "libmongoc-vc90-d.lib")
#		    else
#			    pragma comment(lib, "libbson-vc90.lib")
#			    pragma comment(lib, "libmongoc-vc90.lib")
#		    endif //_DEBUG
#	    elif (_MSC_VER == 1400)
#		    pragma comment(lib, "Ws2_32.lib")
#		    if defined(_DEBUG)
#			    pragma comment(lib, "libbson-vc80-d.lib")
#			    pragma comment(lib, "libmongoc-vc80-d.lib")
#		    else
#			    pragma comment(lib, "libbson-vc80.lib")
#			    pragma comment(lib, "libmongoc-vc80.lib")
#		    endif //_DEBUG
#	    elif (_MSC_VER == 1310)
#		    pragma comment(lib, "Ws2_32.lib")
#		    if defined(_DEBUG)
#			    pragma comment(lib, "libbson-vc71-d.lib")
#			    pragma comment(lib, "libmongoc-vc71-d.lib")
#		    else
#			    pragma comment(lib, "libbson-vc71.lib")
#			    pragma comment(lib, "libmongoc-vc71.lib")
#		    endif //_DEBUG
#	    elif (_MSC_VER == 1300)
#		    pragma comment(lib, "Ws2_32.lib")
#		    if defined(_DEBUG)
#			    pragma comment(lib, "libbson-vc70-d.lib")
#			    pragma comment(lib, "libmongoc-vc70-d.lib")
#		    else
#			    pragma comment(lib, "libbson-vc70.lib")
#			    pragma comment(lib, "libmongoc-vc70.lib")
#		    endif //_DEBUG
#	    else
#		    error "please add other msc version library"
#	    endif // _MSC_VER
#   endif // _MSC_VER
#else
#   if defined(YGGR_AT_LINUX)
//	non def
#   else // defined(YGGR_AT_LINUX)
#	    error "please coding other system network init code"
#   endif //defined(YGGR_AT_LINUX)
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOS)

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#if !defined(MONGO_MAJOR) || !defined(MONGO_PATCH) || !defined(MONGO_MINOR)
#	error "please use high version mongodb c dirver!!!!"
#endif // !defined(MONGO_MAJOR) || !defined(MONGO_PATCH) || !defined(MONGO_MINOR)

//enum
//{
//	bson_eoo = BSON_EOO,
//    bson_double = BSON_DOUBLE,
//    bson_string = BSON_STRING,
//    bson_object = BSON_OBJECT,
//    bson_array = BSON_ARRAY,
//    bson_bindata = BSON_BINDATA,
//    bson_undefined = BSON_UNDEFINED,
//    bson_oid = BSON_OID,
//    bson_bool = BSON_BOOL,
//    bson_date = BSON_DATE,
//    bson_null = BSON_NULL,
//    bson_regex = BSON_REGEX,
//    bson_dbref = BSON_DBREF, /**< Deprecated. */
//    bson_code = BSON_CODE,
//    bson_symbol = BSON_SYMBOL,
//    bson_codewscope = BSON_CODEWSCOPE,
//    bson_int = BSON_INT,
//    bson_timestamp = BSON_TIMESTAMP,
//    bson_long = BSON_LONG
//};

namespace yggr
{
namespace nsql_database_system
{

struct bson_value_type : private nonable::noncreateable
{
public:
	enum
	{
		E_BSON_TYPE_EOO = BSON_EOO,
		E_BSON_TYPE_DOUBLE = BSON_DOUBLE,
		E_BSON_TYPE_STRING = BSON_STRING,
		E_BSON_TYPE_OBJECT = BSON_OBJECT,
		E_BSON_TYPE_ARRAY = BSON_ARRAY,
		E_BSON_TYPE_BINDATA = BSON_BINDATA,
		E_BSON_TYPE_UNDEFINED = BSON_UNDEFINED,
		E_BSON_TYPE_OID = BSON_OID,
		E_BSON_TYPE_BOOL = BSON_BOOL,
		E_BSON_TYPE_DATE = BSON_DATE,
		E_BSON_TYPE_NULL = BSON_NULL,
		E_BSON_TYPE_REGEX = BSON_REGEX,
		E_BSON_TYPE_DBREF = BSON_DBREF, /**< Deprecated. */
		E_BSON_TYPE_CODE = BSON_CODE,
		E_BSON_TYPE_SYMBOL = BSON_SYMBOL,
		E_BSON_TYPE_CODEWSCOPE = BSON_CODEWSCOPE,
		E_BSON_TYPE_INT = BSON_INT,
		E_BSON_TYPE_TIMESTAMP = BSON_TIMESTAMP,
		E_BSON_TYPE_LONG = BSON_LONG,

		E_BSON_TYPE_USER = 0x80,
		E_BSON_TYPE_SPLICE_OBJECT,
		// can append new
		E_id_compile_u8 = 0xff
	};
};

struct bson_binary_type_ex : private nonable::noncreateable
{
public:
	enum
	{
		E_BSON_BIN_TYPE_EX = BSON_BIN_USER + 2,
		E_BSON_BIN_TYPE_U8_BUFFER,
		E_BSON_BIN_TYPE_PACKET,

		E_id_compile_u8 = 0xff
	};
};

struct mongodb_init : private nonable::noncreateable
{
public:

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	static bool init_network_context(void)
	{
		WSADATA out;
        WSAStartup(MAKEWORD(2,2), &out);

		return out.wVersion == 0x0202;
	}
#else // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#   if defined (YGGR_AT_LINUX)
        static bool init_network_context(void)
        {
            return true;
        }
#   else // defined (YGGR_AT_LINUX)
#	    error "please coding other system network init code"
#   endif // defined(YGGR_AT_LINUX)
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

};

} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGODB_CONFIG_H__
