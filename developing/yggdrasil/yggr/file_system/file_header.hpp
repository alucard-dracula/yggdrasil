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

#ifndef __YGGR_FILE_SYSTEM_FILE_HEADER_HPP__
#define __YGGR_FILE_SYSTEM_FILE_HEADER_HPP__

#include <algorithm>
#include <boost/serialization/access.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>

#include <yggr/serialization/nvp.hpp> 

namespace yggr
{
namespace file_system
{

class file_header
{
public:
	//typedef u64 file_size_type;
	typedef size_type file_size_type;
	typedef u32 file_type_id_type;

	enum
	{
		E_File_Unknow = 0,
		E_File_Network, 
		E_File_Text,
		E_File_Binary,
		E_File_XML,

		E_compile_u32 = 0xffffffff
	};

public:

	file_header(void)
		: _size(0), _spos(0), _type(E_File_Unknow)
	{
	}
	

	file_header(const std::string& name, const file_size_type& spos, const file_size_type& size, const file_type_id_type& type)
		: _file_name(name), _size(size), _spos(spos), _type(type)
	{
	}

	file_header(const file_header& right)
		: _file_name(right._file_name), _spos(right._spos), _size(right._size), _type(right._type)
	{
	}

	~file_header(void)
	{
	}

	void mend_offset(const file_size_type& offset)
	{
		_spos += offset;
	}

	file_size_type& set_seek_pos(file_size_type& spos)
	{
		_spos = spos;
		spos += _size;
		return spos;
	}

	file_size_type size(void) const
	{
		return _size;
	}

	file_size_type next_seek_pos(void) const
	{
		return _spos + _size;
	}

	const std::string& file_name(void) const
	{
		return _file_name;
	}

	file_size_type seek_pos(void) const
	{
		return _spos;
	}

	file_type_id_type file_type(void) const
	{
		return _type;
	}

	bool operator <(const file_header& right) const
	{
		return _size < right._size;
	}

private:

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_file_name);
		ar & YGGR_SERIALIZE_NVP(_spos);
		ar & YGGR_SERIALIZE_NVP(_size);
		ar & YGGR_SERIALIZE_NVP(_type);
	}

private:
	std::string _file_name;
	file_size_type _spos;
	file_size_type _size;
	file_type_id_type _type;  
};


} // namespace file_system
} // namespace yggr

#endif //__YGGR_FILE_SYSTEM_FILE_HEADER_HPP__