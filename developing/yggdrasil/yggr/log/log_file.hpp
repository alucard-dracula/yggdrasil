// log_file.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_FILE_HPP__
#define __YGGR_LOG_LOGOP_LOG_FILE_HPP__

#include <fstream>
#include <yggr/charset/string.hpp>
#include <boost/thread/mutex.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/time/time.hpp>
#include <yggr/file_system/local_file_operator.hpp>

namespace yggr
{
namespace log
{
namespace log_op
{
template<typename Stream>
class log_file : private nonable::noncopyable
{
public:
	typedef Stream stream_type;

private:
	typedef boost::mutex::scoped_lock lock_type;
public:
	log_file(const std::string& name)
		: _fname(name)
	{
		file_system::local_file_operator_type::parsing_path(name, _fpath);
	}

	//log_file(const log_file& right)
	//	: _fname(right._fname), _stream(right._stream)
	//{
	//}

	~log_file(void)
	{
		_stream.close();
	}

	const std::string& name(void) const
	{
		//lock_type lk(_mutex);
		return _fname;
	}

	bool operator ==(const std::string& name) const
	{
		return _fname == name;
	}

	template<typename Value>
	bool append(const Value& val, bool bclear = false)
	{
		if(_stream.rdstate() & std::ios::in)
		{
			return false;
		}

		if(!connect(bclear))
		{
			return false;
		}
	
		lock_type lk(_mutex);
		_stream << val << std::endl;
		_stream.flush();

		return true;
	}

	bool clear(void)
	{
		{
			lock_type lk(_mutex);
			if(_stream.is_open())
			{
				_stream.close();
			}
		}

		return connect(true);
		//connect();
		//return true;
	}

private:
	bool connect(bool bclear = false)
	{
		if(_fname.empty())
		{
			return false;
		}

		if(!_fpath.empty())
		{
			if(!file_system::local_file_operator_type::create_path(_fpath))
			{
				return false;
			}
		}

		std::string now_date = time::time().to_local_string(".%Y%m%d");
		if(now_date != _now_date)
		{
			if(_stream.is_open())
			{
				_stream.close();
			}
			_now_date = now_date;
		}

		if(!_stream.is_open())
		{
			lock_type lk(_mutex);
			_stream.open((_fname + _now_date).c_str(), std::ios::out | (bclear? 0 :  std::ios::app));
		}

		return _stream.good();
	}

private:
	std::string _fname;
	std::string _fpath;
	std::string _now_date;
	stream_type _stream;
	mutable boost::mutex _mutex;
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_FILE_HPP__