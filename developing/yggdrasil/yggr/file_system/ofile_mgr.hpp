//ofile_mgr.hpp

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

#ifndef __YGGR_FILE_SYSTEM_OUTPUT_FILE_MANAGER_HPP__
#define __YGGR_FILE_SYSTEM_OUTPUT_FILE_MANAGER_HPP__


#include <boost/filesystem/fstream.hpp>
#include <yggr/file_system/base_file_mgr.hpp>

namespace yggr
{
namespace file_system
{

class ofile_mgr : public base_file_mgr<boost::filesystem::ofstream>
{
public:
	typedef base_file_mgr<boost::filesystem::ofstream> base_type;
	typedef base_type::file_type_id_type file_type_id_type;
	typedef base_type::buf_type buf_type;

	typedef base_type::file_list_type file_list_type;
	
public:
	ofile_mgr(void)
	{
	}

	ofile_mgr(const std::string& root_name)
	{
		base_type::open(root_name, std::ios::out | std::ios::binary);
	}

	~ofile_mgr(void)
	{
	}

	void open(const std::string& root_name)
	{
		base_type::open(root_name, std::ios::out | std::ios::binary);
	}

	bool add_file(const std::string& file_name, const file_type_id_type& type, const buf_type& buf)
	{
		return base_type::add_file(file_name, type, buf);
	}

	void save(void)
	{
		base_type::save();
	}

	void close(void)
	{
		base_type::close();
	}

	bool rm_file(const std::string& file_name)
	{
		return base_type::rm_file(file_name);
	}
};

} // namespace file_system
} //namesapce yggr
#endif //__YGGR_FILE_SYSTEM_OUTPUT_FILE_MANAGER_HPP__