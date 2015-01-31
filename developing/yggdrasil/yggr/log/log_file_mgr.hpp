//log_file_mgr.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_FILE_MGR_HPP__
#define __YGGR_LOG_LOGOP_LOG_FILE_MGR_HPP__

#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

namespace yggr
{
namespace log
{
namespace log_op
{
template<typename File>
class log_file_mgr
{
public:
	typedef File log_file_type;

	typedef log_file_mgr<log_file_type> this_type;

	typedef boost::shared_ptr<log_file_type> log_file_ptr_type;

	typedef std::map<const std::string, log_file_ptr_type> map_type;
	typedef typename map_type::iterator map_iter_type;
	typedef typename map_type::const_iterator map_c_iter_type;

	typedef std::list<std::string> file_name_list_type;
	typedef file_name_list_type::iterator file_name_list_iter_type;
	typedef file_name_list_type::const_iterator file_name_list_c_iter_type;

	typedef file_name_list_type init_type;

public:
	log_file_mgr(const init_type& init)
	{
		create_log_files(init);
	}

	log_file_mgr(const this_type& right)
		: _map(right._map)
	{
	}

	~log_file_mgr(void)
	{
		_map.clear();
	}

	template<typename Value>
	bool append(const boost::tuple<std::string, Value, bool>& val)
	{
		map_iter_type iter = _map.find(boost::get<0>(val));
		if(_map.end() == iter)
		{
			return false;
		}

		return iter->second? iter->second->append(boost::get<1>(val), boost::get<2>(val)) : false;
	}

	bool clear(const std::string& fname)
	{
		map_iter_type iter = _map.find(fname);
		if(_map.end() == iter)
		{
			return false;
		}

		return iter->second? iter->second->clear() : false;
	}

	bool clear(void) // clear_all
	{
		for(map_iter_type i = _map.begin(), isize = _map.end() ; i != isize; ++i)
		{
			if(i->second)
			{
				i->second->clear();
			}
		}

		return true;
	}

	template<typename Value>
	bool append(const Value& val) // append_all
	{
		for(map_iter_type i = _map.begin(), isize = _map.end() ; i != isize; ++i)
		{
			if(i->second)
			{
				i->second->append(val);
			}
		}

		return true;
	}


private:
	void create_log_files(const init_type& init)
	{
		if(init.empty())
		{
			return;
		}

		file_name_list_c_iter_type isize = init.end();
		for(file_name_list_c_iter_type i = init.begin(); i != isize; ++i)
		{
			if((*i).empty())
			{
				continue;
			}

			log_file_ptr_type ptr(new log_file_type(*i));
			_map[*i].swap(ptr);
		}
	}
private:
	map_type _map;
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_FILE_MGR_HPP__