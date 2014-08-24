//connect_backup.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_CONNECT_BACKUP_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_CONNECT_BACKUP_HPP__


#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename ConnPtr>
class connect_backup
{
protected:
	typedef ConnPtr conn_ptr_type;
	typedef ids::id64_type id_type;

private:
	typedef ids::pointer_id_generator<id_type> id_gen_type;
	typedef safe_container::safe_unordered_map<id_type, conn_ptr_type> conn_map_type;
	typedef typename conn_map_type::iterator conn_map_iter_type;

private:
	typedef connect_backup this_type;

protected:
	static bool backup_empty(void)
	{
		return _conn_ptr_map.empty();
	}

	static void clear_backup(void)
	{
		conn_map_type::base_type container;
		this_type::_conn_ptr_map.use_handler(
			boost::bind(&this_type::handler_clear_backup, _1, boost::ref(container)));

		for(conn_map_iter_type i = container.begin(), isize = container.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->close(); // last close connect must use this system
			}
		}

		container.clear();
	}

	static id_type backup(const conn_ptr_type& pconn)
	{
		id_type id = id_gen_type::gen(pconn);
		if(this_type::_conn_ptr_map.insert(id, pconn))
		{
			return id;
		}

		return id_type();
	}

	static conn_ptr_type drop_backup(const id_type& id)
	{
		conn_ptr_type pconn 
			= this_type::_conn_ptr_map.use_handler(boost::bind(&this_type::handler_drop_backup, _1, boost::cref(id)));

		return pconn;
	}

	static conn_ptr_type get(const id_type& id)
	{
		conn_ptr_type pconn;
		_conn_ptr_map.get_value(id, pconn);
		return pconn;
	}

private:
	static conn_ptr_type handler_drop_backup(typename conn_map_type::base_type& base,
												const id_type& id)
	{
		conn_map_iter_type iter = base.find(id);
		if(iter == base.end())
		{
			return conn_ptr_type();
		}

		conn_ptr_type ptr = iter->second;
		base.erase(iter);
		return ptr;
	}

	static void handler_clear_backup(typename conn_map_type::base_type& base,
										typename conn_map_type::base_type& back)
	{
		back.swap(base);
	}

private:
	static conn_map_type _conn_ptr_map;	
};

template<typename ConnPtr>
typename connect_backup<ConnPtr>::conn_map_type connect_backup<ConnPtr>::_conn_ptr_map;

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_HELPER_CONNECT_BACKUP_HPP__