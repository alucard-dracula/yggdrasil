//mongo_accesser.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_SAVER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_SAVER_HPP__

#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>

#include <boost/thread/mutex.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>

#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/nsql_database_system/c_bson_err.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{

template<typename Mongo_Conn, typename Mutex = boost::mutex>
class mongo_accesser
{
	typedef Mutex mutex_type;
	typedef typename helper::mutex_def_helper<mutex_type>::lock_type lock_type;

public:

	ERROR_MAKER_BEGIN("mongo_accesser")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_no_socket, MONGO_CONN_NO_SOCKET)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_conn_fail, MONGO_CONN_FAIL)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_io_error, MONGO_IO_ERROR)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_socket_error, MONGO_SOCKET_ERROR)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_read_size_error, MONGO_READ_SIZE_ERROR)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_command_failed, MONGO_COMMAND_FAILED)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_write_error, MONGO_WRITE_ERROR)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_ns_invalid, MONGO_NS_INVALID)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_bson_invalid, MONGO_BSON_INVALID)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_bson_not_finished, MONGO_BSON_NOT_FINISHED)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_bson_too_large, MONGO_BSON_TOO_LARGE)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_write_concern_invalid, MONGO_WRITE_CONCERN_INVALID)
			ERROR_CODE_KEY_VAL_DEF(E_mongo_access_error_start, exception::exception_sort::E_code_start_8)
			ERROR_CODE_DEF(E_mongo_datas_empty)
			ERROR_CODE_DEF(E_mongo_find_empty)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_mongo_conn_no_socket, "mongo connection no socket")
			ERROR_CODE_MSG(E_mongo_conn_fail, "mongo connection fail")
			ERROR_CODE_MSG(E_mongo_io_error, "mongo io error")
			ERROR_CODE_MSG(E_mongo_socket_error, "mongo socket error")
			ERROR_CODE_MSG(E_mongo_read_size_error, "mongo read size error")
			ERROR_CODE_MSG(E_mongo_command_failed, "mongo command failed")
			ERROR_CODE_MSG(E_mongo_write_error, "mongo write error")
			ERROR_CODE_MSG(E_mongo_ns_invalid, "mongo ns invalid")
			ERROR_CODE_MSG(E_mongo_bson_invalid, "mongo bson invalid")
			ERROR_CODE_MSG(E_mongo_bson_not_finished, "mongo bson not finished")
			ERROR_CODE_MSG(E_mongo_bson_too_large, "mongo bson too large")
			ERROR_CODE_MSG(E_mongo_write_concern_invalid, "mongo write concern invalid")
			ERROR_CODE_MSG(E_mongo_datas_empty, "mongo datas empty")
			ERROR_CODE_MSG(E_mongo_find_empty, "mongo find empty")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

private:
	typedef std::vector<bson*> batch_insert_type;

public:
	typedef Mongo_Conn conn_type;
	typedef typename conn_type::this_ptr_type conn_ptr_type;

	enum
	{
		E_MONGO_SORT_DESC = -1,
		E_MONGO_SORT_ASC = 1,

		E_mongo_sort_op_compile_s32 = 0xffffffff
	};

	enum
	{
		E_MONGO_INDEX_UNIQUE = MONGO_INDEX_UNIQUE,
		E_MONGO_INDEX_DROP_DUPS = MONGO_INDEX_DROP_DUPS,
		E_MONGO_INDEX_BOTH = MONGO_INDEX_UNIQUE | MONGO_INDEX_DROP_DUPS,
		E_mongo_create_index_op_compile_s32 = 0xffffffff
	};

	typedef std::pair<std::string, s32> mongo_index_item_type;
	typedef std::vector<mongo_index_item_type> mongo_index_vt_type;

private:
	typedef mongo_accesser this_type;

	typedef boost::tuple<u32, u32> params_type;

	typedef mongo mongo_connection;

public:
	mongo_accesser(void)
		: _pconn(new conn_type())
	{
	}

	explicit mongo_accesser(const std::string& conn_str)
		: _pconn(new conn_type(conn_str))
	{
	}

	mongo_accesser(const std::string& conn_str,
						const std::string& user,
						const std::string& pwd)
		: _pconn(new conn_type(conn_str)), _auth_db("admin"), _user(user), _pwd(pwd)
	{
	}

	mongo_accesser(const std::string& conn_str,
					const std::string& auth_db,
					const std::string& user,
					const std::string& pwd)
		: _pconn(new conn_type(conn_str)), _auth_db(auth_db), _user(user), _pwd(pwd)
	{
	}

	template<typename Init>
	explicit mongo_accesser(const Init& init)
		: _pconn(new conn_type(init.host(), init.port())),
			_auth_db(init.auth_db()), _user(init.user()), _pwd(init.pwd())
	{
	}


	mongo_accesser(const std::string& str_host, u16 port)
		: _pconn(new conn_type(str_host, port))
	{
	}

	mongo_accesser(const std::string& str_host, u16 port,
					const std::string& user, const std::string& pwd)
		: _pconn(new conn_type(str_host, port)), _auth_db("admin"), _user(user), _pwd(pwd)
	{
	}

	mongo_accesser(const std::string& str_host, u16 port,
					const std::string& auth_db, const std::string& user, const std::string& pwd)
		: _pconn(new conn_type(str_host, port)), _auth_db(auth_db), _user(user), _pwd(pwd)
	{
	}

	explicit mongo_accesser(const conn_ptr_type& pconn)
		: _pconn(pconn)/*, _bauth(false)*/
	{
		assert(_pconn);
	}

	mongo_accesser(const conn_ptr_type& pconn, const std::string& user, const std::string& pwd)
		: _pconn(pconn), /*_bauth(true),*/ _auth_db("admin"), _user(user), _pwd(pwd)
	{
		assert(_pconn);
		cmd_authenticate(_auth_db, _user, _pwd);
	}

	mongo_accesser(const conn_ptr_type& pconn,
					const std::string& auth_db, const std::string& user, const std::string& pwd)
		: _pconn(pconn), /*_bauth(true),*/ _auth_db(auth_db), _user(user), _pwd(pwd)
	{
		assert(_pconn);
		cmd_authenticate(_auth_db, _user, _pwd);
	}

	mongo_accesser(const mongo_accesser& right)
		: _pconn(right._pconn), /*_bauth(right._bauth),*/ _auth_db(right._auth_db), _user(right._user), _pwd(right._pwd)
	{
	}

	~mongo_accesser(void)
	{
	}

	bool connect(void)
	{
		bool bconn = false;
		{
			lock_type lk(_mutex);

			if(!_pconn)
			{
				return false;
			}

			if(_pconn->is_connected())
			{
				return true;
			}

			bconn = _pconn->connect();
		}
		return bconn && !cmd_authenticate(_auth_db, _user, _pwd);
	}

	bool reconnect(void)
	{
		return prv_reconnect(_pconn->now_ver());
	}

	template<typename Packet>
	error_type insert(const std::string& db, const std::string& coln, const Packet& pak)
	{
		return insert(db, coln, pak.org_buf());
	}

	template<typename Packet>
	error_type insert(const std::string& db, const std::string& coln, const Packet& pak, c_mongo_write_concern& write_concern)
	{
		return insert(db, coln, pak.org_buf(), write_concern);
	}

	error_type insert(const std::string& db, const std::string& coln, const c_bson& b)
	{
		const c_bson::base_type& tb = b;
		return insert(db, coln,  const_cast<c_bson::base_type&>(tb), 0);
	}

	error_type insert(const std::string& db, const std::string& coln, const c_bson& b, c_mongo_write_concern& write_concern)
	{
		const c_bson::base_type& tb = b;
		return insert(db, coln,  const_cast<c_bson::base_type&>(tb), write_concern);
	}

	error_type insert(const std::string& db, const std::string& coln, bson& b, c_mongo_write_concern& write_concern)
	{
		return insert(db, coln, b, &write_concern);
	}

	error_type insert(const std::string& db, const std::string& coln, bson& b, mongo_write_concern* write_concern = 0)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		std::stringstream ss;
		ss << db << "." << coln;

		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);

			s32 rst = mongo_insert(pconn, ss.str().c_str(), &b, write_concern);
			if(rst)
			{
				error_type err(pconn->err, "[insert]");
				exception::exception::throw_error(err);
				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}
				prv_reset_connect(pconn->err, _pconn->now_ver());

				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	template<typename Key, typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multimap<Key, Packet, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multimap<Key, Packet, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_map<Key, Packet, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_map<Key, Packet, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multimap<Key, Packet, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multimap<Key, Packet, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::map<Key, Packet, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::map<Key, Packet, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multiset<Packet, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multiset<Packet, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_set<Packet, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_set<Packet, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multiset<Packet, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multiset<Packet, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::set<Packet, Cmp, Alloc>& bsons,
						s32 flag)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::set<Packet, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::list<Packet, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::list<Packet, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::vector<Packet, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Packet, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::vector<Packet, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_packet_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multimap<Key, c_bson, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multimap<Key, c_bson, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_map<Key, c_bson, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_map<Key, c_bson, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multimap<Key, c_bson, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multimap<Key, c_bson, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
							const std::map<Key, c_bson, Cmp, Alloc>& bsons,
							s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Key, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
							const std::map<Key, c_bson, Cmp, Alloc>& bsons,
							c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_key_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multiset<c_bson, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_multiset<c_bson, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_set<c_bson, Hash, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Hash, typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const boost::unordered_set<c_bson, Hash, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multiset<c_bson, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::multiset<c_bson, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::set<c_bson, Cmp, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Cmp, typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::set<c_bson, Cmp, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::list<c_bson, Alloc>& bsons,
						s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();

		return err;
	}

	template<typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::list<c_bson, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();

		return err;
	}

	template<typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::vector<c_bson, Alloc>& bsons, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, 0, flag);
		vt.clear();
		return err;
	}

	template<typename Alloc>
	error_type insert(const std::string& db, const std::string& coln,
						const std::vector<c_bson, Alloc>& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		batch_insert_type vt;
		make_bson_container_of_val(bsons, vt);
		error_type err = insert(db, coln, vt, write_concern, flag);
		vt.clear();
		return err;
	}

	error_type insert(const std::string& db, const std::string& coln, const batch_insert_type& bsons,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		return insert(db, coln, bsons, &write_concern, flag);
	}

	error_type insert(const std::string& db, const std::string& coln, const batch_insert_type& bsons,
						mongo_write_concern* write_concern = 0, s32 flag = 0)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		if(bsons.empty())
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_datas_empty));
			exception::exception::throw_error(err);
			return err;
		}

		std::stringstream ss;
		ss << db << "." << coln;

		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);
			s32 rst = mongo_insert_batch(pconn, ss.str().c_str(), (const bson**)(&bsons[0]), bsons.size(),
											write_concern, flag);

			if(rst)
			{
				error_type err(pconn->err, "[insert batch]");
				exception::exception::throw_error(err);
				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}
				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	template<typename Packet>
	error_type remove(const std::string& db, const std::string& coln, const Packet& pak)
	{
		return remove(db, coln, pak.org_buf());
	}

	template<typename Packet>
	error_type remove(const std::string& db, const std::string& coln, const Packet& pak, c_mongo_write_concern& write_concern)
	{
		return remove(db, coln, pak.org_buf(), write_concern);
	}

	error_type remove(const std::string& db, const std::string& coln, const c_bson& b)
	{
		return remove(db, coln, static_cast<const c_bson::base_type&>(b));
	}

	error_type remove(const std::string& db, const std::string& coln, const c_bson& b, c_mongo_write_concern& write_concern)
	{
		return remove(db, coln, static_cast<const c_bson::base_type&>(b), write_concern);
	}

	error_type remove_all(const std::string& db, const std::string& coln)
	{
		c_bson b;
		b.finish();
		return remove(db, coln, static_cast<const c_bson::base_type&>(b));
	}

	error_type remove_all(const std::string& db, const std::string& coln, c_mongo_write_concern& write_concern)
	{
		c_bson b;
		b.finish();
		return remove(db, coln, static_cast<const c_bson::base_type&>(b), write_concern);
	}

	error_type remove(const std::string& db, const std::string& coln, const bson& b, c_mongo_write_concern& write_concern)
	{
		return remove(db, coln, b, &write_concern);
	}

	error_type remove(const std::string& db, const std::string& coln, const bson& b, mongo_write_concern* write_concern = 0)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		std::stringstream ss;
		ss << db << "." << coln;

		mongo_connection* pconn(_pconn.get());
		{
			lock_type lk(_mutex);
			s32 rst = mongo_remove(pconn, ss.str().c_str(), &b, write_concern);
			if(rst)
			{
				error_type err(pconn->err, "[remove]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	template<typename Packet>
	error_type update(const std::string& db, const std::string& coln, const Packet& opak, const Packet& npak, s32 flag = 0)
	{
		return update(db, coln, opak.org_buf(), npak.org_buf(), flag);
	}

	template<typename Packet>
	error_type update(const std::string& db, const std::string& coln, const Packet& opak, const Packet& npak,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		return update(db, coln, opak.org_buf(), npak.org_buf(), write_concern, flag);
	}

	error_type update(const std::string& db, const std::string& coln, const c_bson& ob, const c_bson& nb, s32 flag = 0)
	{
		return update(db, coln, static_cast<const c_bson::base_type&>(ob),
						static_cast<const c_bson::base_type&>(nb), 0, flag);
	}

	error_type update(const std::string& db, const std::string& coln, const c_bson& ob, const c_bson& nb,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		return update(db, coln, static_cast<const c_bson::base_type&>(ob), static_cast<const c_bson::base_type&>(nb),
						write_concern, flag);
	}

	error_type update(const std::string& db, const std::string& coln, const bson& ob, const bson& nb,
						c_mongo_write_concern& write_concern, s32 flag = 0)
	{
		return update(db, coln, ob, &write_concern, flag);
	}

	error_type update(const std::string& db, const std::string& coln, const bson& ob, const bson& nb,
						mongo_write_concern* write_concern = 0, s32 flag = 0)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		std::stringstream ss;
		ss << db << "." << coln;
		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);

			s32 rst = mongo_update(pconn, ss.str().c_str(), &ob, &nb, flag, write_concern);

			if(rst)
			{
				error_type err(pconn->err, "[update]");
				exception::exception::throw_error(err);
				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());

				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	template<typename Saver>
	error_type find(Saver& saver, const std::string& db, const std::string& coln, const c_bson& query,
				const c_bson& fields, s32 n_to_ret = 0, s32 n_to_skip = 0, s32 options = 0) /*const*/
	{
		const bson &tquery = query;
		const bson &tfields = fields;
		return find(saver, db, coln, const_cast<bson&>(tquery), const_cast<bson&>(tfields), n_to_ret, n_to_skip, options);
	}

	template<typename Saver>
	error_type find(Saver& saver, const std::string& db, const std::string& coln, bson& query, bson& fields,
				s32 n_to_ret = 0, s32 n_to_skip = 0, s32 options = 0) /*const*/
	{
		typedef Saver saver_type;
		typedef typename saver_type::buf_type buf_type;

		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		std::stringstream ss;
		ss << db << "." << coln;

		mongo_connection* pconn(_pconn.get());
		mongo_cursor* p_cursor = 0;

		{
			lock_type lk(_mutex);
			p_cursor = mongo_find(pconn, ss.str().c_str(), &query, &fields, n_to_ret, n_to_skip, options);

			if(!p_cursor)
			{
				if(!pconn->err)
				{
					return error_maker_type::make_error(error_maker_type::E_mongo_find_empty);
				}

				error_type err(pconn->err, "[find]");
				exception::exception::throw_error(err);


				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());

				return err;
			}
		}

		if(n_to_ret)
		{
			saver << n_to_ret;
			for(s32 i = 0; !mongo_cursor_next(p_cursor) && i < n_to_ret; ++i)
			{
				c_bson tb(p_cursor->current);
				buf_type buf(boost::move(tb));
				saver << buf; //limited
			}
		}
		else
		{
			for(; !mongo_cursor_next(p_cursor); )
			{
				c_bson tb(p_cursor->current);
				buf_type buf(boost::move(tb));
				saver += buf; //non limited
			}
		}

		mongo_cursor_destroy(p_cursor);

		return error_maker_type::make_error(0);
	}


	template<typename T>
	error_type find_one(T& obj, const std::string& db, const std::string& coln, const c_bson& query, const c_bson& fields) /*const*/
	{
		const bson &tquery = query;
		const bson &tfields = fields;
		return find_one(obj, db, coln, const_cast<bson&>(tquery), const_cast<bson&>(tfields));
	}


	template<typename T>
	error_type find_one(T& obj, const std::string& db, const std::string& coln, bson& query, bson& fields) /*const*/
	{
		typedef T obj_type;

		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		std::stringstream ss;
		ss << db << "." << coln;

		mongo_connection* pconn(_pconn.get());

		{
			c_bson b(true);
			lock_type lk(_mutex);
			s32 rst = mongo_find_one(pconn, ss.str().c_str(), &query, &fields, &b);

			if(rst)
			{
				if(!pconn->err)
				{
					return error_maker_type::make_error(error_maker_type::E_mongo_find_empty);
				}

				error_type err(pconn->err, "[find one]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}
				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}

			obj = b;
			//bson_destroy(&b);
		}

		return error_maker_type::make_error(0);
	}

	error_type create_simple_indexes(const std::string& db, const std::string& coln, const mongo_index_vt_type& vt, s32 options = 0)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		std::vector<c_bson> outs;
		return create_simple_indexes(ss.str(), vt, options, outs);
	}

	error_type create_simple_indexes(const std::string& db, const std::string& coln, const mongo_index_vt_type& vt, s32 options,
									std::vector<c_bson>& outs)
	{
		std::stringstream ss;
		ss << db << "." << coln;

		return create_simple_indexes(ss.str(), vt, options, outs);
	}

	error_type create_simple_indexes(const std::string& db_coln, const mongo_index_vt_type& vt, s32 options = 0)
	{
		 std::vector<c_bson> outs;
		 return create_simple_indexes(db_coln, vt, options, outs);
	}

	error_type create_simple_indexes(const std::string& db_coln, const mongo_index_vt_type& vt, s32 options, std::vector<c_bson>& outs)
	{
		typedef mongo_index_vt_type::const_iterator c_idx_iter_type;
		typedef std::vector<c_bson>::iterator out_iter_type;

		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		outs.resize(vt.size());

		c_idx_iter_type isize = vt.end();
		out_iter_type out_i = outs.begin();
		//bool b_all_right = true;
		error_type err(error_maker_type::make_error(0));
		for(c_idx_iter_type i = vt.begin(); i != isize; ++i, ++out_i)
		{
			err = create_index(db_coln, *i, options, *out_i);
			if(err.code())
			{
				break;
			}
		}

		return err;
	}

	error_type create_index(const std::string& db, const std::string& coln, const std::string& key, s32 ord, s32 options = 0)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		c_bson out;
		return create_index(ss.str(), mongo_index_item_type(key, ord), options, out);
	}

	error_type create_index(const std::string& db, const std::string& coln, const std::string& key, s32 ord, s32 options, c_bson& out)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		return create_index(ss.str(), mongo_index_item_type(key, ord), options, out);
	}

	error_type create_index(const std::string& db_coln, const std::string& key, s32 ord, s32 options = 0)
	{
		c_bson out;
		return create_index(db_coln, mongo_index_item_type(key, ord), options, out);
	}

	error_type create_index(const std::string& db_coln, const std::string& key, s32 ord, s32 options, c_bson& out)
	{
		return create_index(db_coln, mongo_index_item_type(key, ord), options, out);
	}

	error_type create_index(const std::string& db, const std::string& coln, const mongo_index_item_type& item, s32 options = 0)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		c_bson out;
		return create_index(ss.str(), item, options, out);
	}

	error_type create_index(const std::string& db, const std::string& coln, const mongo_index_item_type& item, s32 options, c_bson& out)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		return create_index(ss.str(), item, options, out);
	}

	error_type create_index(const std::string& db_coln, const mongo_index_item_type& item, s32 options = 0)
	{
		c_bson out;
		return create_index(db_coln, item, options);
	}

	error_type create_index(const std::string& db_coln, const mongo_index_item_type& item, s32 options, c_bson& out)
	{
		c_bson bs_idx;
		bs_idx.save(item.first, item.second);
		bs_idx.finish();

		return create_index(db_coln, bs_idx, options, out);
	}

	error_type create_index(const std::string& db, const std::string& coln, const mongo_index_vt_type& index_vt, s32 options, c_bson& out)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		return create_index(ss.str(), index_vt, options, out);
	}

	error_type create_index(const std::string& db, const std::string& coln, const mongo_index_vt_type& index_vt, s32 options = 0)
	{
		std::stringstream ss;
		ss << db << "." << coln;
		c_bson out;
		return create_index(ss.str(), index_vt, options, out);
	}

	error_type create_index(const std::string& db_coln, const mongo_index_vt_type& index_vt, s32 options = 0)
	{
		c_bson out;
		return create_index(db_coln, index_vt, options, out);
	}


	error_type create_index(const std::string& db_coln, const mongo_index_vt_type& index_vt, s32 options, c_bson& out)
	{
		typedef mongo_index_vt_type::const_iterator c_iter_type;

		c_bson bs_idx;

		c_iter_type isize = index_vt.end();
		for(c_iter_type i = index_vt.begin();i != isize; ++i)
		{
			bs_idx.save((*i).first, (*i).second);
		}

		bs_idx.finish();

		return create_index(db_coln, bs_idx, options, out);
	}

	error_type create_index(const std::string& db_coln, const c_bson& key, c_bson_err& bson_err, s32 options = 0)
	{
		return create_index(db_coln, key, &bson_err, options);
	}

	error_type create_index(const std::string& db_coln, const c_bson& key, bson* bson_err = 0, s32 options = 0)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		const c_bson::base_type* pkey(key);

		{
			lock_type lk(_mutex);

			s32 rst = mongo_create_index(pconn, db_coln.c_str(), const_cast<c_bson::base_type*>(pkey), options, 0);

			if(rst)
			{
				error_type err(pconn->err, "[create index]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type create_index(const std::string& db_coln, const c_bson& key, s32 options, c_bson& out)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		const c_bson::base_type* pkey(key);

		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);
			s32 rst = mongo_create_index(pconn, db_coln.c_str(), const_cast<c_bson::base_type*>(pkey), options, &bs);

			out=bs;
			bson_destroy(&bs);

			if(rst)
			{
				error_type err(pconn->err, "[create index]");
				exception::exception::throw_error(err);
				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());

				return err;
			}
		}


		return error_maker_type::make_error(0);
	}

	error_type cmd_add_user(const std::string& db, const std::string& user, const std::string& pwd)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		{
			lock_type lk(_mutex);

			s32 rst = mongo_cmd_add_user(pconn, db.c_str(), user.c_str(), pwd.c_str());

			if(rst)
			{
				error_type err(pconn->err, "[cmd add user]");
				exception::exception::throw_error(err);
				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type cmd_drop_collection(const std::string& db, const std::string& coln)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);
			s32 rst = mongo_cmd_drop_collection(pconn, db.c_str(), coln.c_str(), 0);

			if(rst)
			{
				error_type err(pconn->err, "[cmd drop collection]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type cmd_drop_collection(const std::string& db, const std::string& coln, c_bson& out)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);

			s32 rst = mongo_cmd_drop_collection(pconn, db.c_str(), coln.c_str(), &bs);
			out = bs;
			bson_destroy(&bs);

			if(rst)
			{
				error_type err(pconn->err, "[cmd drop collection]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type cmd_drop_db(const std::string& db)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);

			s32 rst = mongo_cmd_drop_db(pconn, db.c_str());

			if(rst)
			{
				error_type err(pconn->err, "[cmd drop db]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	bool cmd_is_master(void) /*const*/
	{
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return false;
			}
		}

		mongo_connection* pconn(_pconn.get());

		lock_type lk(_mutex);
		return mongo_cmd_ismaster(pconn, 0);
	}

	bool cmd_is_master(c_bson& out) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return false;
			}
		}

		mongo_connection* pconn(_pconn.get());

		bool bret = false;
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);
			bret = mongo_cmd_ismaster(pconn, &bs);
		}

		out = bs;
		bson_destroy(&bs);

		return bret;

	}

	error_type run_command(const std::string& db, const c_bson& cmd, c_bson& out)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		const c_bson::base_type* pcmd(cmd);

		c_bson::base_type bs;

		{
			lock_type lk(_mutex);
			s32 rst = mongo_run_command(pconn, db.c_str(), const_cast<c_bson::base_type*>(pcmd), &bs); //bson error bs discard

			if(rst)
			{
				out.finish();
				error_type err(pconn->err, "[run command]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		out = bs;
		bson_destroy(&bs);

		return error_maker_type::make_error(0);
	}

	error_type run_command(const std::string& db, const std::string& cmd, s32 arg, c_bson& out)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);

			s32 rst = mongo_simple_int_command(pconn, db.c_str(), cmd.c_str(), arg, &bs);

			if(rst)
			{
				out.finish();
				error_type err(pconn->err, "[simple int command]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		out = bs;
		bson_destroy(&bs);

		return error_maker_type::make_error(0);
	}

	error_type run_command(const std::string& db, const std::string& cmd, const std::string& arg, c_bson& out)
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);

			s32 rst = mongo_simple_str_command(pconn, db.c_str(), cmd.c_str(), arg.c_str(), &bs);

			if(rst)
			{
				out.finish();
				error_type err(pconn->err, "[simple str command]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		out = bs;
		bson_destroy(&bs);
		return error_maker_type::make_error(0);
	}

	error_type drop_index(const std::string& db, const std::string& coln, const std::string& idx, s32 ord)
	{
		c_bson out;
		return drop_index(db, coln, idx, ord, out);
	}

	error_type drop_index(const std::string& db, const std::string& coln, const std::string& idx, s32 ord, c_bson& out)
	{
		c_bson bs_arg;
		bs_arg.save(idx, ord);
		bs_arg.finish();

		c_bson bs_cmd;
		bs_cmd.save(std::string("dropIndexes"), coln);
		bs_cmd.save(std::string("index"), bs_arg);
		bs_cmd.finish();

		return run_command(db, bs_cmd, out);
	}

	error_type drop_all_indexes(const std::string& db, const std::string& coln)
	{
		c_bson bs_cmd, out;
		bs_cmd.save(std::string("dropIndexes"), coln);
		bs_cmd.save(std::string("index"), std::string("*"));
		bs_cmd.finish();

		return run_command(db, bs_cmd, out);
	}

	error_type drop_all_indexes(const std::string& db, const std::string& coln, c_bson& out)
	{
		c_bson bs_cmd;
		bs_cmd.save(std::string("dropIndexes"), coln);
		bs_cmd.save(std::string("index"), std::string("*"));
		bs_cmd.finish();

		return run_command(db, bs_cmd, out);
	}

	error_type cmd_get_last_error(const std::string& db, c_bson& out) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);

			s32 rst = mongo_cmd_get_last_error(pconn, db.c_str(), &bs);
			out = bs;
			bson_destroy(&bs);
			if(rst)
			{
				error_type err(pconn->err, "[cmd get last error]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type cmd_get_prev_error(const std::string& db, c_bson& out) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);

			s32 rst = mongo_cmd_get_prev_error(pconn, db.c_str(), &bs);
			out = bs;
			bson_destroy(&bs);

			if(rst)
			{
				error_type err(pconn->err, "[cmd get prev error]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());

				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type cmd_reset_error(const std::string& db) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);
			//s32 rst = mongo_cmd_reset_error(pconn, db.c_str(), 0);
			s32 rst = mongo_cmd_reset_error(pconn, db.c_str());

			if(rst)
			{
				error_type err(pconn->err, "[cmd reset error]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	error_type cmd_reset_error(const std::string& db, c_bson& out) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return err;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			lock_type lk(_mutex);
			//s32 rst = mongo_cmd_reset_error(pconn, db.c_str(), &bs);
			s32 rst = mongo_cmd_reset_error(pconn, db.c_str());
			out = bs;
			bson_destroy(&bs);

			if(rst)
			{
				error_type err(pconn->err, "[cmd reset error]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	s64 count(const std::string& db, const std::string& coll) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return -1;
			}
		}

		mongo_connection* pconn(_pconn.get());

		c_bson query;
		query.finish();
		c_bson::base_type* pquery(query);

		f64 rst = 0.0;

		{
			lock_type lk(_mutex);
			rst = mongo_count(pconn, db.c_str(), coll.c_str(), pquery);

			if(rst == MONGO_ERROR)
			{
				error_type err(pconn->err, "[count]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
			}
		}
		return rst;
	}

	s64 count(const std::string& db, const std::string& coll, c_bson& query) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			if(!prv_reconnect(_pconn->now_ver()))
			{
				return -1;
			}
		}

		mongo_connection* pconn(_pconn.get());
		c_bson::base_type* pquery(query);

		f64 rst = 0.0;

		{
			lock_type lk(_mutex);
			rst = mongo_count(pconn, db.c_str(), coll.c_str(), pquery);

			if(rst == MONGO_ERROR)
			{
				error_type err(pconn->err, "[count]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
			}
		}

		return rst;
	}

private:
	template<typename Val_Container>
	void make_bsons_container_of_val(const Val_Container& container, std::vector<bson*>& vt) const
	{
		typedef Val_Container container_type;
		typedef typename container_type::const_iterator container_citer_type;

		vt.resize(container.size());
		size_type idx = 0;
		for(container_citer_type i = container.begin(), isize = container.end(); i != isize; ++i, ++idx)
		{
			vt[idx] = (bson*)(boost::cref(*i).get_pointer());
		}
	}

	template<typename Key_Val_Container>
	void make_bsons_container_of_key_val(const Key_Val_Container& container, std::vector<bson*>& vt) const
	{
		typedef Key_Val_Container container_type;
		typedef typename container_type::const_iterator container_citer_type;

		vt.resize(container.size());
		size_type idx = 0;
		for(container_citer_type i = container.begin(), isize = container.end(); i != isize; ++i, ++idx)
		{
			vt[idx] = (bson*)(boost::cref(i->second).get_pointer());
		}
	}

	template<typename Val_Container>
	void make_packet_container_of_val(const Val_Container& container, std::vector<bson*>& vt) const
	{
		typedef Val_Container container_type;
		typedef typename container_type::const_iterator container_citer_type;

		vt.resize(container.size());
		size_type idx = 0;
		for(container_citer_type i = container.begin(), isize = container.end(); i != isize; ++i, ++idx)
		{
			vt[idx] = (bson*)(boost::cref(i->org_buf()).get_pointer());
		}
	}

	template<typename Key_Val_Container>
	void make_packet_container_of_key_val(const Key_Val_Container& container, std::vector<bson*>& vt) const
	{
		typedef Key_Val_Container container_type;
		typedef typename container_type::const_iterator container_citer_type;

		vt.resize(container.size());
		size_type idx = 0;
		for(container_citer_type i = container.begin(), isize = container.end(); i != isize; ++i, ++idx)
		{
			vt[idx] = (bson*)(boost::cref(i->second.org_buf()).get_pointer());
		}
	}

private:
	error_type cmd_authenticate(const std::string& db, const std::string& user, const std::string& pwd) /*const*/
	{
		assert(_pconn);
		if(!(_pconn && _pconn->is_connected()))
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_mongo_conn_fail));
			exception::exception::throw_error(err);
			prv_reconnect(_pconn->now_ver());
			return err;
		}

		//if(!_bauth)
		if(_auth_db.empty())
		{
			return error_maker_type::make_error(0);
		}

		mongo_connection* pconn(_pconn.get());

		{
			lock_type lk(_mutex);

			s32 rst = mongo_cmd_authenticate(pconn, db.c_str(), user.c_str(), pwd.c_str());

			if(rst)
			{
				error_type err(pconn->err, "[cmd authenticate]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

	bool prv_reconnect(u32 ver)
	{
		bool bconn = false;
		{
			lock_type lk(_mutex);
			if(!_pconn)
			{
				return false;
			}
			bconn = _pconn->reconnect_of_ver(ver);
		}

		return bconn && !cmd_authenticate(_auth_db, _user, _pwd);
	}

	void prv_reset_connect(u32 err, u32 ver)
	{
		if(!_pconn || _pconn->err)
		{
			return;
		}

		switch(err)
		{
		case error_maker_type::E_mongo_conn_no_socket:
		case error_maker_type::E_mongo_conn_fail:
		case error_maker_type::E_mongo_io_error:
		case error_maker_type::E_mongo_socket_error:
		case error_maker_type::E_mongo_write_error:
		case error_maker_type::E_mongo_ns_invalid:
		case error_maker_type::E_mongo_bson_invalid:
		case error_maker_type::E_mongo_write_concern_invalid:
			_pconn->reset_connect_of_ver(ver);
			break;
		default:
			break;
		};
	}

	error_type unlocked_cmd_get_last_error(const std::string& db, c_bson& out) /*const*/
	{
		mongo_connection* pconn(_pconn.get());
		c_bson::base_type bs = {0, 0};

		{
			s32 rst = mongo_cmd_get_last_error(pconn, db.c_str(), &bs);
			out = bs;
			bson_destroy(&bs);

			if(rst)
			{
				error_type err(pconn->err, "[unlock cmd get last error]");
				exception::exception::throw_error(err);

				if(pconn->errcode)
				{
					exception::exception::throw_error(pconn->errcode, pconn->errstr);
				}

				prv_reset_connect(pconn->err, _pconn->now_ver());
				return err;
			}
		}

		return error_maker_type::make_error(0);
	}

private:
	conn_ptr_type _pconn;
	//bool _bauth;

	std::string _auth_db;
	std::string _user;
	std::string _pwd;
	mutable mutex_type _mutex;
};

} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_SAVER_HPP__
