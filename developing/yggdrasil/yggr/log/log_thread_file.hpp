// log_thread_file.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_THREAD_FILE_HPP__
#define __YGGR_LOG_LOGOP_LOG_THREAD_FILE_HPP__

#include <fstream>
#include <sstream>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/time/time.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/file_system/local_file_operator.hpp>


namespace yggr
{
namespace log
{
namespace log_op
{
template<typename Stream, typename Thread_ID>
class log_thread_file : private nonable::noncopyable
{
private:
    typedef Stream stream_type;
    typedef boost::shared_ptr<stream_type> stream_ptr_type;
	typedef Thread_ID trd_id_type;
	typedef safe_container::safe_unordered_map<trd_id_type, stream_ptr_type> stream_map_type;
	typedef typename stream_map_type::iterator stream_map_iter_type;

	typedef log_thread_file this_type;

public:
	log_thread_file(const std::string& full_name)
		: _fname(full_name)
	{
		file_system::local_file_operator_type::parsing_path(full_name, _fpath);
	}

	~log_thread_file(void)
	{
		_stream_map.use_handler_of_all(boost::bind(&this_type::handler_clear, this, _1));
		_stream_map.clear();
	}

	const std::string& name(void) const
	{
		return _fname;
	}

	bool operator==(const std::string& name) const
	{
		return _fname == name;
	}

	template<typename Value>
	bool append(const Value& val, bool tnil = false)
	{
		typedef Value now_val_type;

		if(!connect())
		{
			return false;
		}

		return _stream_map.use_handler(boost::bind(&this_type::handler_append<now_val_type>, this, _1, boost::cref(val)));
	}

	bool clear(void)
	{
		assert(false);
		return false;
	}

private:

	bool prv_clear(void)
	{
		_stream_map.use_handler_of_all(boost::bind(&this_type::handler_clear, this, _1));
		_stream_map.clear();

		return connect();
	}

	bool connect(void)
	{
		if(_fname.empty())
		{
			return false;
		}

		if(!_fpath.empty())
		{
			bool bright = false;
			try
			{
				bright = file_system::local_file_operator_type::create_path(_fpath);
			}
			catch(const boost::filesystem::filesystem_error&)
			{
				return false;
			}
			catch(const compatibility::stl_exception&)
			{
				return false;
			}

			if(!bright)
			{
				return false;
			}
		}

		std::string now_date = time::time().to_local_string(".%Y%m%d");
		if(now_date != _now_date)
		{
			_now_date = now_date;
			return prv_clear();
		}

		return true;
	}

	template<typename Value>
	bool handler_append(typename stream_map_type::base_type& base, const Value& val)
	{
		trd_id_type tid = val.thread_id();
		stream_map_iter_type iter = base.find(tid);
		std::pair<stream_map_iter_type, bool> ins_rst;

		bool b = false;

		if(iter == base.end())
		{
			ins_rst = base.insert(typename stream_map_type::value_type(tid, stream_ptr_type(new stream_type())));
			if(ins_rst.second)
			{
				iter = ins_rst.first;
				assert(iter->second);
				std::stringstream ss;
				ss << _fname << _now_date << ".thread-" << tid;
				iter->second->open(ss.str().c_str(), std::ios::out | std::ios::app);
				b = iter->second->good();

				if(!b)
				{
					iter->second->close();
					base.erase(iter);
				}
			}
			else
			{
				b = ins_rst.second;
			}
		}
		else
		{
		    assert(iter->second);
			b = iter->second->good() && (!(iter->second->rdstate() & std::ios::in));
		}

		if(b)
		{
			(*(iter->second)) << val << std::endl;
			iter->second->flush();
		}

		return b;
	}

	void handler_clear(stream_map_iter_type& iter)
	{
	    assert(iter->second);
		iter->second->close();
	}

private:
	std::string _fname;
	std::string _fpath;
	std::string _now_date;
	stream_map_type _stream_map;
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_THREAD_FILE_HPP__
