//file_obj.hpp

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

#ifndef __YGGR_FILE_SYSTEM_FILE_OBJ_HPP__
#define __YGGR_FILE_SYSTEM_FILE_OBJ_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <algorithm>
#include <yggr/file_system/file_header.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/base/ptr_single.hpp>

namespace yggr
{
namespace file_system
{

class file_object : public file_header
{
public:

	typedef file_header base_type;
	typedef base_type::file_type_id_type file_type_id_type;
	typedef base_type::file_size_type file_size_type;
	typedef std::string buf_type;
	typedef buf_type::value_type buf_val_type;

private:
	template<typename T>
	struct array_deleter
	{
		void operator()(T* x) const
		{
			if(!x)
			{
				return;
			}

			delete []x;
		}
	};

public:

	file_object(void)
	{
	}

	file_object(const std::string& name, const file_size_type& spos, const file_type_id_type type, const buf_type& buf)
		: base_type(name, spos,buf.size(), type)
	{
		_buf.resize(base_type::size());
		memcpy(&(_buf[0]), &(buf[0]), _buf.size());
	}

	file_object(const file_object& right)
		: base_type(right)
	{
		if(right.empty())
		{
			return;
		}
		_buf.resize(right.size());
		memcpy(&(_buf[0]), &((right._buf)[0]), _buf.size());
	}

	~file_object(void)
	{
	}

	file_size_type real_size(void) const
	{
		return _buf.size();
	}

	bool empty(void) const
	{
		return _buf.empty();
	}

	const buf_type& buf(void) const
	{
		return _buf;
	}

	void clear_buf(void)
	{
		buf_type buf;
		_buf.swap(buf);
	}

	template<typename OutStream>
	void save(OutStream& os) const
	{	
		try
		{
			os.write(_buf.data(), base_type::size());
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename InputStream>
	void load(InputStream& is, const file_size_type& offset)
	{
		typedef array_deleter<char> array_deleter_type;

		file_size_type now_size = base_type::size();
		//std::auto_ptr<char> pt(new char[base_type::size()]);
		boost::shared_ptr<char> pt(new char[base_type::size()], array_deleter_type());

		try
		{
			is.seekg(offset + base_type::seek_pos(), std::ios::beg);
			is.read(pt.get(), base_type::size());
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}

		_buf.resize(base_type::size());
		memcpy(&(_buf[0]), pt.get(), now_size);
	}
private:
	buf_type _buf; 
};


} // namespace file_system
} // namespace yggr

#endif //__YGGR_FILE_SYSTEM_FILE_OBJ_HPP__