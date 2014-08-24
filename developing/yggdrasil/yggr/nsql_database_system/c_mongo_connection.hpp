//c_mongo_connection.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECT_HPP__

#include <sstream>
#include <memory>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/base/inc_ver.h>
#include <yggr/base/error_make.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncopyable.hpp>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/regular_parse/regex_parse.hpp>

#include <yggr/network/socket_info.hpp>


#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_connection
	: public mongo,
		public inc_ver,
		private nonable::noncopyable
{
private:
	typedef c_mongo_connection this_type;

public:
	typedef mongo base_type;
	typedef boost::shared_ptr< this_type > this_ptr_type;

	typedef struct _init_t
	{
		friend class c_mongo_connection;

	private:
		BOOST_COPYABLE_AND_MOVABLE(_init_t)
	public:
		typedef std::string host_type;
		typedef u16 port_type;

		typedef network::socket_info<host_type, port_type> socket_info_type;

		typedef std::set<socket_info_type> socket_info_set_type;
		typedef socket_info_set_type::iterator socket_info_set_iter_type;
		typedef socket_info_set_type::const_iterator socket_info_set_citer_type;

		_init_t(void);
		_init_t(const std::string& set_name);
		_init_t(const std::string& set_name, const std::string& str_host, port_type nport);
		_init_t(const std::string& set_name, const std::string& str_host, const std::string& str_port);
		_init_t(const std::string& set_name, const std::string& str);

		_init_t(BOOST_RV_REF(_init_t) right)
		{
			_init_t::swap(right);
		}

		_init_t(const _init_t& right);
		~_init_t(void);

		_init_t& add_seed(const std::string& str_host, port_type& nport);
		_init_t& operator<<(const std::string& addr);

		//_init_t& operator=(BOOST_RV_REF(_init_t) right);
		_init_t& operator=(BOOST_RV_REF(_init_t) right)
		{
			_init_t::swap(right);
			return *this;
		}
		_init_t& operator=(const _init_t& right);

		bool empty(void) const;
		size_type size(void) const;
		const std::string& name(void) const;
		void swap(_init_t& right);

		template<typename Handler>
		typename Handler::result_type use_handler(const Handler& handler) const
		{
			return handler(_set_name, _si_set);
		}

	private:
		void parse_string(const std::string& str);

	private:
		std::string _set_name;
		socket_info_set_type _si_set;
	} init_type;

//---------------------------------------------------------

//typedef enum mongo_error_t {
//    MONGO_CONN_SUCCESS = 0,  /**< Connection success! */
//    MONGO_CONN_NO_SOCKET,    /**< Could not create a socket. */
//    MONGO_CONN_FAIL,         /**< An error occured while calling connect(). */
//    MONGO_CONN_ADDR_FAIL,    /**< An error occured while calling getaddrinfo(). */
//    MONGO_CONN_NOT_MASTER,   /**< Warning: connected to a non-master node (read-only). */
//    MONGO_CONN_BAD_SET_NAME, /**< Given rs name doesn't match this replica set. */
//    MONGO_CONN_NO_PRIMARY,   /**< Can't find primary in replica set. Connection closed. */
//
//    MONGO_IO_ERROR,          /**< An error occurred while reading or writing on the socket. */
//    MONGO_SOCKET_ERROR,      /**< Other socket error. */
//    MONGO_READ_SIZE_ERROR,   /**< The response is not the expected length. */
//    MONGO_COMMAND_FAILED,    /**< The command returned with 'ok' value of 0. */
//    MONGO_WRITE_ERROR,       /**< Write with given write_concern returned an error. */
//    MONGO_NS_INVALID,        /**< The name for the ns (database or collection) is invalid. */
//    MONGO_BSON_INVALID,      /**< BSON not valid for the specified op. */
//    MONGO_BSON_NOT_FINISHED, /**< BSON object has not been finished. */
//    MONGO_BSON_TOO_LARGE,    /**< BSON object exceeds max BSON size. */
//    MONGO_WRITE_CONCERN_INVALID /**< Supplied write concern object is invalid. */
//} mongo_error_t;

	ERROR_MAKER_BEGIN("c_mongo_connection")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_no_socket, MONGO_CONN_NO_SOCKET)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_fail, MONGO_CONN_FAIL)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_addr_fail, MONGO_CONN_ADDR_FAIL)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_not_master, MONGO_CONN_NOT_MASTER)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_bad_set_name, MONGO_CONN_BAD_SET_NAME)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_no_primary, MONGO_CONN_NO_PRIMARY)
			//ERROR_CODE_KEY_VAL_DEF(E_mongo_io_error, MONGO_IO_ERROR)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_socket_error, MONGO_SOCKET_ERROR)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_mongo_conn_no_socket, "mongo connection no socket")
			ERROR_CODE_MSG(E_mongo_conn_fail, "mongo connection fail")
			ERROR_CODE_MSG(E_mongo_conn_addr_fail, "mongo connection addr fail")
			ERROR_CODE_MSG(E_mongo_conn_not_master, "mongo connection not master")
			ERROR_CODE_MSG(E_mongo_conn_bad_set_name, "mongo connection bad set name")
			ERROR_CODE_MSG(E_mongo_conn_no_primary, "mongo connection no primary")
			//ERROR_CODE_MSG(E_mongo_io_error, "mongo io error")
			ERROR_CODE_MSG(E_mongo_socket_error, "mongo socket error")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

private:
	typedef inc_ver ver_type;

public:
	c_mongo_connection(void); //try catch
	explicit c_mongo_connection(const std::string& str); //try catch
	c_mongo_connection(const std::string& str_host, u16 port);

	c_mongo_connection(BOOST_RV_REF(init_type) init)
		: _init(boost::forward<init_type>(init))
	{
		base_type& conn = *this;
		memset(&conn, 0, sizeof(base_type));
		mongodb_init::init_network_context();//try catch
	}

	c_mongo_connection(const init_type& init);//try catch
	~c_mongo_connection(void);

	inline operator base_type*(void)
	{
		return this;
	}

	inline operator const base_type*(void) const
	{
		return this;
	}

	bool connect(void);

	bool is_connected(void) const;
	bool reset_connect(void);
	bool reset_connect_of_ver(u32 ver);
	bool reconnect(void);
	bool reconnect_of_ver(u32 ver);
	u64 id(void) const;
	bool close(void);

private:
	u32 prv_connect(const std::string& set_name, const init_type::socket_info_set_type& set);

private:
	init_type _init;
};


} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECT_HPP__
