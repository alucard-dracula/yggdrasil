//basic_local_file_operator.hpp

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

#ifndef __YGGR_FILE_SYSTEM_BASIC_LOCAL_FILE_OPERATOR_HPP__
#define __YGGR_FILE_SYSTEM_BASIC_LOCAL_FILE_OPERATOR_HPP__

#ifdef USE_OLD_BOOST_FILESYSTEM
#	define BOOST_FILESYSTEM_VERSION 2
#endif // USE_OLD_BOOST_FILESYSTEM

#include <iostream>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <memory>

#include <iterator>
#include <vector>
#include <list>

#ifdef __GNUC__
#   define BOOST_NO_CXX11_SCOPED_ENUMS
#endif // __MINGW32__

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <compatibility/stl_exception.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace file_system
{

class file_check
	: private nonable::noncreateable
{
public:
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static Basic_String<Char, Traits, Alloc>
				check_file_name(const Basic_String<Char, Traits, Alloc>& name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;
		//YGGR_PP_TEXT_DEF(Char, A);
		//YGGR_PP_TEXT_DEF(Char, Z);
		//YGGR_PP_TEXT_DEF(Char, a);
		//YGGR_PP_TEXT_DEF(Char, z);

		stringstream_type ss_azs;
		ss_azs << "AZaz";
		string_type str_azs(ss_azs.str());

		if(name.empty())
		{
			return string_type();
		}
		string_type str(name);
		stringstream_type ss_back_slant;
		stringstream_type ss_slant;
		ss_back_slant << "\\";
		ss_slant << "/";

		Char char_back_slant = (ss_back_slant.str())[0];
		Char char_slant = ss_slant.str()[0];

		std::replace(str.begin(), str.end(), char_back_slant, char_slant);

		stringstream_type ss_dslant;
		ss_dslant << "//";

		stringstream_type ss_failed_char;
		ss_failed_char << "*?\"<>|";

		if( (str.find(ss_dslant.str()) != string_type::npos)
			|| (str.find_first_of(ss_failed_char.str()) != string_type::npos))
		{
			return string_type();
		}

		stringstream_type ss_colon;
		ss_colon << ":";

		std::size_t i = str.find_last_of(ss_colon.str());
		if(i != string_type::npos)
		{
			if(!(
					(i == 1)
					&& ((str[0] >= str_azs[2] && str[0] <= str_azs[3])
							|| (str[0] >= str_azs[0] && str[0] <= str_azs[1]))
					&& (str.size() < 3 || str[i+1] == char_slant)
				))
			{
				return string_type();
			}
		}
		return str;
	}
#else
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static Basic_String<Char, Traits, Alloc>
				check_file_name(const Basic_String<Char, Traits, Alloc>& name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;
		if(name.empty())
		{
			return string_type();
		}
		string_type str(name);

		stringstream_type ss_back_slant;
		stringstream_type ss_slant;
		ss_back_slant << "\\";
		ss_slant << "/";

		Char char_back_slant = (ss_back_slant.str())[0];
		Char char_slant = ss_slant.str()[0];

		std::replace(str.begin(), str.end(), char_back_slant, char_slant);

		stringstream_type ss_dslant;
		ss_dslant << "//";

		stringstream_type ss_failed_char;
		ss_failed_char << "*?\"<>|:";

		if( (str.find(ss_dslant.str()) != string_type::npos)
			|| (str.find_first_of(ss_failed_char.str()) != string_type::npos))
		{
			return string_type();
		}

		return str;
	}
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static Basic_String<Char, Traits, Alloc>
		check_path_name(const Basic_String<Char, Traits, Alloc>& path)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;

		string_type real_path = check_file_name(path);

		stringstream_type ss_slant;
		ss_slant << "/";

		Char char_slant = ss_slant.str()[0];

		if(!real_path.empty() && (*(real_path.rbegin())) != char_slant)
		{
			real_path += char_slant;
		}
		return real_path;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void parsing_file_path_and_name(const Basic_String<Char, Traits, Alloc>& full_name,
											Basic_String<Char, Traits, Alloc>& path,
											Basic_String<Char, Traits, Alloc>& name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;

		string_type fname(check_file_name(full_name));

		if(fname.empty())
		{
			return;
		}

		stringstream_type ss_slant;
		ss_slant << "/";

		Char char_slant = ss_slant.str()[0];

		std::string::size_type i = fname.rfind(char_slant);
		if(i != std::string::npos)
		{
			path.assign(fname.begin(), fname.begin() + i + 1);
			name.assign(fname.begin() + i + 1, fname.end());
		}
		else
		{
			name = fname;
		}
	}

	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void parsing_file_path_and_name(const Basic_String<Char, Traits, Alloc>& full_name,
											Path_Container& path,
											Basic_String<Char, Traits, Alloc>& name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;
	    typedef typename string_type::iterator str_iter_type;

		string_type fname(check_file_name(full_name));

		if(fname.empty())
		{
			return;
		}

		stringstream_type ss_slant;
		ss_slant << "/";

		Char char_slant = ss_slant.str()[0];

		str_iter_type right = fname.end();
		str_iter_type left = fname.begin();
		for(;;)
		{
			str_iter_type iter = std::find(left, right, char_slant);
			if(iter != right)
			{
				std::inserter(path, path.end()) = string_type(left, iter);
				left = iter + 1;
				continue;
			}
			else
			{
				if(left != right)
				{
					name.assign(left, right);
				}
				break;
			}

		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void parsing_path_of_dir(const Basic_String<Char, Traits, Alloc>& full_name,
										Basic_String<Char, Traits, Alloc>& path,
										Basic_String<Char, Traits, Alloc>& name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;

		string_type full_path_name(check_path_name(full_name));

		if(full_path_name.empty())
		{
			return;
		}

		stringstream_type ss_slant;
		ss_slant << "/";

		Char char_slant = ss_slant.str()[0];

		if((*(full_path_name.rbegin())) == char_slant)
		{
			full_path_name.erase(full_path_name.end() - 1);
		}

		typename string_type::size_type i = full_path_name.rfind(char_slant);
		if(i != string_type::npos)
		{
			path.assign(full_path_name.begin(), full_path_name.begin() + i + 1);
			name.assign(full_path_name.begin() + i + 1, full_path_name.end());
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void parsing_path(const Basic_String<Char, Traits, Alloc>& full_name,
								Basic_String<Char, Traits, Alloc>& path)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;

		string_type fname(check_file_name(full_name));

		if(fname.empty())
		{
			return;
		}

		stringstream_type ss_slant;
		ss_slant << "/";

		Char char_slant = ss_slant.str()[0];

		typename string_type::size_type i = fname.rfind(char_slant);
		if(i != string_type::npos)
		{
			path.assign(fname.begin(), fname.begin() + i + 1);
		}
	}

	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void parsing_path(const Basic_String<Char, Traits, Alloc>& full_name,
								Path_Container& path)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;

	    typedef typename string_type::iterator str_iter_type;

		string_type path_name(check_path_name(full_name));

		if(path_name.empty())
		{
			return;
		}

		stringstream_type ss_slant;
		ss_slant << "/";

		Char char_slant = ss_slant.str()[0];

		str_iter_type right = path_name.end();
		str_iter_type left = path_name.begin();
		for(;;)
		{
			str_iter_type iter = std::find(left, right, char_slant);
			if(iter != right)
			{
				std::inserter(path, path.end()) = string_type(left, iter);
				left = iter + 1;
			}
			else
			{
				break;
			}
		}
	}
};

template<typename File_Stream_Config>
class basic_local_file_operator
	: public file_check
{

private:
	typedef File_Stream_Config fstream_cfg_type;
	typedef basic_local_file_operator this_type;

public:
	typedef std::list<std::string> path_list_type;
	typedef std::string::iterator str_iter_type;
	typedef size_type file_size_type;

	typedef typename fstream_cfg_type::ifstream_type ifstream_type;
	typedef typename fstream_cfg_type::ofstream_type ofstream_type;
	typedef typename fstream_cfg_type::fbuf_type fbuf_type;
	typedef typename fstream_cfg_type::path_type path_type;
	typedef typename fstream_cfg_type::char_type char_type;

	typedef file_check base_type;

private:

	ERROR_MAKER_BEGIN("basic_local_file_operator")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_6)
			ERROR_CODE_DEF(E_invalid_file)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_6)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_file, "invalid file")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

public:

	static void create_file_of_binary(const std::string& fname, const file_size_type& size)
	{
		assert(!fname.empty());
		if(fname.empty())
		{
			return;
		}

		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return;
			}
		}

		try
		{
			fbuf_type fbuf;
			fbuf.open((path_name + file_name).c_str(), std::ios_base::in | std::ios_base::out
								| std::ios_base::trunc | std::ios_base::binary);

			fbuf.pubseekoff(size - 1, std::ios_base::beg);
			fbuf.sputc(char_type());
			fbuf.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	static void write_file_of_binary(const std::string& fname, const char* buffer, file_size_type size)
	{
		assert(!fname.empty());

		if(!buffer || !size)
		{
			return;
		}

		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return;
			}
		}

		try
		{
			ofstream_type ofs;
			ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary);
			ofs.write(buffer, size);
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	//static void write_file_of_binary(const std::string& fname, const std::string& buffer);
	static void write_file_of_binary(const std::string& fname, const std::string& buffer)
	{
		assert(!fname.empty());
		if(buffer.empty())
		{
			return;
		}

		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return;
			}
		}

		try
		{
			ofstream_type ofs;
			ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary);
			ofs.write(buffer.data(), buffer.size());
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	//static void write_file_of_binary(ofstream_type& ofs, const char* buffer, file_size_type size, bool bflush = false);
	static void write_file_of_binary(ofstream_type& ofs, const char* buffer, file_size_type size, bool bflush = false)
	{
		assert(ofs.rdstate() & std::ios::binary);

		if(!buffer || !size)
		{
			return;
		}

		if(!ofs.good())
		{
			return;
		}
		try
		{
			ofs.write(buffer, size);
			//ofs.close();
			if(bflush)
			{
				ofs.flush();
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	//static void write_file_of_binary(ofstream_type& ofs, const std::string& buffer, bool bflush = false);
	static void write_file_of_binary(ofstream_type& ofs, const std::string& buffer, bool bflush = false)
	{
		assert(ofs.rdstate() & std::ios::binary);

		if(buffer.empty())
		{
			return;
		}

		if(!ofs.good())
		{
			return;
		}
		try
		{
			ofs.write(buffer.data(), buffer.size());
			if(bflush)
			{
				ofs.flush();
			}
			//ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	//static void append_file_of_binary(const std::string& fname, const char* buffer, file_size_type size);
	static void append_file_of_binary(const std::string& fname, const char* buffer, file_size_type size)
	{
		assert(!fname.empty());
		if(!buffer || !size)
		{
			return;
		}

		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return;
			}
		}

		try
		{
			ofstream_type ofs;
			ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary | std::ios::app);
			ofs.write(buffer, size);
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	//static void append_file_of_binary(const std::string& fname, const std::string& buffer);
	static void append_file_of_binary(const std::string& fname, const std::string& buffer)
	{
		assert(!fname.empty());
		if(buffer.empty())
		{
			return;
		}

		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return;
			}
		}

		try
		{
			ofstream_type ofs;
			ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary | std::ios::app);
			ofs.write(buffer.data(), buffer.size());
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	//static void read_file_of_binary(ifstream_type& ifs, std::string& buffer, file_size_type& size);
	static void read_file_of_binary(ifstream_type& ifs, std::string& buffer, file_size_type& size)
	{
		assert(ifs.rdstate() & std::ios::binary);

		if(!size)
		{
			return;
		}

		std::auto_ptr<char> pt(new char[size]);
		try
		{
			if(!ifs.good())
			{
				throw error_maker_type::make_error(error_maker_type::E_invalid_file);
			}
			ifs.read(pt.get(), size);
		}
		catch(const compatibility::stl_exception& e)
		{
			size = 0;
			exception::exception::throw_error(e);
			return;
		}
		catch(const error& e)
		{
			size = 0;
			exception::exception::throw_error(e);
			return;
		}

		buffer.resize(size);
		memcpy(&(buffer[0]), pt.get(), size);
	}

	//static void read_file_of_binary(const std::string& fname, std::string& buffer);
	static void read_file_of_binary(const std::string& fname, std::string& buffer)
	{
		basic_local_file_operator::file_size_type size = 0;
		read_file_of_binary(fname, buffer, size);
	}

	//static void read_file_of_binary(const std::string& fname, std::string& buffer, file_size_type& size);
	static void read_file_of_binary(const std::string& fname, std::string& buffer, file_size_type& size)
	{

		std::string file_name = base_type::check_file_name(fname);

		assert(!file_name.empty());
		if(file_name.empty())
		{
			return;
		}

		try
		{
#	ifdef _MSC_VER
			path_type fpath(fname.c_str(), boost::filesystem::native);
#	else
			path_type fpath(fname.c_str());
#	endif // _MSC_VER
			size = boost::filesystem::file_size(fpath);
		}
		catch(const compatibility::stl_exception& e)
		{
			size = 0;
			exception::exception::throw_error(e);
			return;
		}

		std::auto_ptr<char> pt(new char[size]);
		try
		{
			ifstream_type ifs;
			ifs.open(fname.c_str(), std::ios::in | std::ios::binary);
			if(!ifs.good())
			{
				throw error_maker_type::make_error(error_maker_type::E_invalid_file);
			}
			ifs.read(pt.get(), size);
			ifs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			size = 0;
			exception::exception::throw_error(e);
			return;
		}
		catch(const error& e)
		{
			size = 0;
			exception::exception::throw_error(e);
			return;
		}

		buffer.resize(size);
		memcpy(&(buffer[0]), pt.get(), size);

	}

	//static bool create_directory(const std::string& path, const std::string& dir);
	static bool create_directory(const std::string& path, const std::string& dir)
	{
		std::string path_name = base_type::check_path_name(path);
		std::string full_path_name = path_name + dir;


		if(path_name.empty())
		{
			path_name = "./";
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER
		if(!boost::filesystem::exists(now_path))
		{
			return false;
		}

#	ifdef _MSC_VER
		path_type full_path(full_path_name.c_str(), boost::filesystem::native);
#	else
		path_type full_path(full_path_name.c_str());
#	endif //_MSC_VER

		if(boost::filesystem::exists(full_path))
		{
			return true;
		}

		try
		{
			boost::filesystem::create_directory(full_path);
		}
		catch(const compatibility::stl_exception & e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	};

	//static bool create_path(const std::string& path);
	static bool is_exists_path(const std::string& full_name)
	{
		std::string path_name = base_type::check_path_name(full_name);

		if(path_name.empty())
		{
			return true;
		}
#	ifdef _MSC_VER
		path_type full_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type full_path(path_name.c_str());
#	endif //_MSC_VER

		return boost::filesystem::exists(full_path);
	}

	static bool create_path(const std::string& full_name)
	{
		std::string path_name = base_type::check_path_name(full_name);

		if(path_name.empty())
		{
			return true;
		}
#	ifdef _MSC_VER
		path_type full_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type full_path(path_name.c_str());
#	endif //_MSC_VER
		if(boost::filesystem::exists(full_path))
		{
			return true;
		}

		str_iter_type right = path_name.end();
		str_iter_type left = path_name.begin();
		str_iter_type begin = path_name.begin();
		std::string now_path_name;
		for(;;)
		{
			str_iter_type iter = std::find(left, right, '/');
			if(iter != right)
			{
				now_path_name = std::string(begin, iter + 1);
#			ifdef _MSC_VER
				path_type now_path(now_path_name.c_str(), boost::filesystem::native);
#			else
				path_type now_path(now_path_name.c_str());
#			endif // _MSC_VER
				if(boost::filesystem::exists(now_path))
				{
					left = iter + 1;
					continue;
				}

				try
				{
					boost::filesystem::create_directory(now_path);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e);
					return false;
				}
				left = iter + 1;
			}
			else
			{
				break;
			}
		}

		return true;
	}


	//static bool rename_directory(const std::string& oldname, const std::string newname);
	static bool rename_directory(const std::string& oldname, const std::string& newname)
	{
		std::string old_path_name;
		std::string old_dir_name;
		std::string new_path_name;
		std::string new_dir_name;

		base_type::parsing_path_of_dir(oldname, old_path_name, old_dir_name);
		base_type::parsing_path_of_dir(newname, new_path_name, new_dir_name);

		if(old_path_name != new_path_name || old_dir_name == new_dir_name || old_dir_name.empty() || new_dir_name.empty())
		{
			return false;
		}

#	ifdef _MSC_VER
		path_type old_path((old_path_name + old_dir_name).c_str(), boost::filesystem::native);
		path_type new_path((new_path_name + new_dir_name).c_str(), boost::filesystem::native);
#	else
		path_type old_path((old_path_name + old_dir_name).c_str());
		path_type new_path((new_path_name + new_dir_name).c_str());
#	endif // _MSC_VER

		if(!(boost::filesystem::is_directory(old_path)))
		{
			return false;
		}

		try
		{
			boost::filesystem::rename(old_path, new_path);
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	//static bool remove_directory(const std::string& full_name, bool b_compulsion = false);
	static bool remove_directory(const std::string& full_name, bool b_compulsion = false)
	{
		std::string path_name = base_type::check_path_name(full_name);

#	ifdef _MSC_VER
		path_type full_path(path_name, boost::filesystem::native);
#	else
		path_type full_path(path_name);
#	endif //_MSC_VER

		if(!boost::filesystem::exists(full_path))
		{
			return true;
		}

		if(!boost::filesystem::is_directory(full_path))
		{
			return false;
		}

		try
		{
			if(b_compulsion)
			{
				boost::filesystem::remove_all(full_path);
			}
			else
			{
				boost::filesystem::remove(full_path);
			}
		}
		catch(const boost::filesystem::filesystem_error& e)
		{
		    exception::exception::throw_error(e.code().value(), e.what());
		    return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	//static bool copy_file(const std::string& path1, const std::string& path2, bool b_compulsion = false);
	static bool copy_file(const std::string& path1, const std::string& path2, bool b_compulsion = false)
	{
		std::string src_path_name = base_type::check_file_name(path1);
		std::string dst_path_name = base_type::check_file_name(path2);

		if(src_path_name.empty())
		{
			return false;
		}

		if(dst_path_name.empty())
		{
			return false;
		}

#	ifdef _MSC_VER
		path_type src_path(src_path_name.c_str(), boost::filesystem::native);
#	else
		path_type src_path(src_path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(src_path) || !boost::filesystem::is_regular_file(src_path))
		{
			return false;
		}

		if(src_path_name == dst_path_name)
		{
			for(size_type i = 1; ;++i)
			{
				std::stringstream ss;
				ss << i;
				dst_path_name = src_path_name + ".bak" + ss.str();
#			ifdef _MSC_VER
				path_type tpath(dst_path_name.c_str(), boost::filesystem::native);
#			else
				path_type tpath(dst_path_name.c_str());
#			endif //_MSC_VER
				if(!boost::filesystem::exists(tpath))
				{
					break;
				}
			}
		}

#	ifdef _MSC_VER
		path_type dst_path(dst_path_name.c_str(), boost::filesystem::native);
#	else
		path_type dst_path(dst_path_name.c_str());
#	endif //_MSC_VER

		if(boost::filesystem::exists(dst_path))
		{
			if(!b_compulsion)
			{
				return false;
			}
			else
			{
				try
				{
					boost::filesystem::remove(dst_path);
				}
				catch(const compatibility::stl_exception& e)
				{
					std::cerr << e.what() << std::endl;
					return false;
				}
			}
		}

		try
		{
			boost::filesystem::copy_file(src_path, dst_path);
		}
		catch(const compatibility::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}

		return true;
	}

	//static bool remove_file(const std::string& full_name);
	static bool remove_file(const std::string& full_name)
	{
		std::string file_name = base_type::check_file_name(full_name);
		if(file_name.empty())
		{
			return false;
		}
#	ifdef _MSC_VER
		path_type file_path(file_name.c_str(), boost::filesystem::native);
#	else
		path_type file_path(file_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(file_path) || !boost::filesystem::is_regular_file(file_path))
		{
			return false;
		}

		try
		{
			boost::filesystem::remove(file_path);
		}
		catch(const compatibility::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}

		return true;
	}

	//static bool cut_file(const std::string& path1, const std::string& path2, bool b_compulsion = false);
	static bool cut_file(const std::string& path1, const std::string& path2, bool b_compulsion = false)
	{
		if(!copy_file(path1, path2, b_compulsion))
		{
			return false;
		}

		if(!remove_file(path1))
		{
			return false;
		}

		return true;
	}

	//static bool rename_file(const std::string& oldname, const std::string newname);
	static bool rename_file(const std::string& oldname, const std::string newname)
	{
		std::string old_path_name;
		std::string old_file_name;
		std::string new_path_name;
		std::string new_file_name;

		base_type::parsing_file_path_and_name(oldname, old_path_name, old_file_name);
		base_type::parsing_file_path_and_name(newname, new_path_name, new_file_name);

		if(old_path_name != new_path_name || old_file_name == new_file_name || old_file_name.empty() || new_file_name.empty())
		{
			return false;
		}

#	ifdef _MSC_VER
		path_type old_path((old_path_name + old_file_name).c_str(), boost::filesystem::native);
		path_type new_path((new_path_name + new_file_name).c_str(), boost::filesystem::native);
#	else
		path_type old_path((old_path_name + old_file_name).c_str());
		path_type new_path((new_path_name + new_file_name).c_str());
#	endif //_MSC_VER

		if(!(boost::filesystem::is_regular_file(old_path)))
		{
			return false;
		}

		try
		{
			boost::filesystem::rename(old_path, new_path);
		}
		catch(const compatibility::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}

		return true;
	}


	//static file_size_type file_size(const std::string& str_path);
	static file_size_type file_size(const std::string& str_path)
	{
		std::string file_name = base_type::check_file_name(str_path);
		if(file_name.empty())
		{
			return 0;
		}

		basic_local_file_operator::file_size_type size = 0;
		try
		{
	#ifdef _MSC_VER
			path_type fpath(str_path.c_str(), boost::filesystem::native);
	#else
			path_type fpath(str_path.c_str());
	#endif // _MSC_VER
			size = boost::filesystem::file_size(fpath);
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return 0;
		}

		return size;
	}


	template<typename Value>
	static bool write_file_of_text(const std::string& fname, const Value& val, const std::string& spr = "\r\n")
	{
		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return false;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return false;
			}
		}

		ofstream_type ofs;

		try
		{
			ofs.open(file_name.c_str(), std::ios::out);
			if(!ofs.good())
			{
				throw error_maker_type::make_error(error_maker_type::E_invalid_file);
				return false;
			}
			ofs << val << spr;
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	template<typename Value>
	static bool write_file_of_text(ofstream_type& ofs, const Value& val, const std::string& spr = "\r\n")
	{
		assert(!(ofs.rdstate() & std::ios::binary));
		if(!ofs.good())
		{
			throw error_maker_type::make_error(error_maker_type::E_invalid_file);
			return false;
		}

		try
		{
			ofs << val << spr;
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}


	template<typename Value>
	static bool append_file_of_text(const std::string& fname, const Value& val, const std::string& spr = "\r\n")
	{
		std::string path_name;
		std::string file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return false;
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return false;
			}
		}

		ofstream_type ofs;

		try
		{
			ofs.open(file_name.c_str(), std::ios::out | std::ios::app);
			if(!ofs.good())
			{
				throw error_maker_type::make_error(error_maker_type::E_invalid_file);
				return false;
			}
			ofs << val;
			ofs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	template<typename Value>
	static bool read_file_of_text(const std::string& fname, Value& val)
	{
		std::string file_name = base_type::check_file_name(fname);
		if(file_name.empty())
		{
			return false;
		}

		try
		{
			ifstream_type ifs;
			ifs.open(file_name.c_str(), std::ios::in);
			if(!ifs.good())
			{
				throw error_maker_type::make_error(error_maker_type::E_invalid_file);
			}
			ifs >> val;
			ifs.close();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	template<typename Path_Container>
	static void search_files(const std::string& path, Path_Container& file_list)
	{
		std::string path_name = base_type::check_path_name(path);

		if(path_name.empty())
		{
			path_name = "./";
		}

	#ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
	#else
		path_type now_path(path_name.c_str());
	#endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		try
		{
			boost::filesystem::directory_iterator end_iter;
			for(boost::filesystem::directory_iterator i(now_path); i != end_iter ; ++i)
			{
				if(boost::filesystem::is_regular_file(*i))
				{
					//file_list.push_back(i->string());
					file_list.push_back(i->path().string());
				}
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename Path_Container>
	static void search_directorys(const std::string& path, Path_Container& dir_list)
	{
		std::string path_name = base_type::check_path_name(path);

		if(path_name.empty())
		{
			path_name = "./";
		}

	#ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
	#else
		path_type now_path(path_name.c_str());
	#endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		try
		{
			boost::filesystem::directory_iterator end_iter;
			for(boost::filesystem::directory_iterator i(now_path); i != end_iter ; ++i)
			{
				if(boost::filesystem::is_directory(*i))
				{
					//dir_list.push_back(i->string());
					dir_list.push_back(i->path().string());
				}
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename Path_Container>
	static void search_all_type(const std::string& path, Path_Container& name_list)
	{
		std::string path_name = base_type::check_path_name(path);

		if(path_name.empty())
		{
			path_name = "./";
		}

	#ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
	#else
		path_type now_path(path_name.c_str());
	#endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		try
		{
			boost::filesystem::directory_iterator end_iter;
			for(boost::filesystem::directory_iterator i(now_path); i != end_iter ; ++i)
			{
				//name_list.push_back(i->string());
				name_list.push_back(i->path().string());
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename Path_Container>
	static void recursion_search_of_files(const std::string& path, Path_Container& file_list)
	{
		std::string path_name = base_type::check_path_name(path);

		if(path_name.empty())
		{
			path_name = "./";
		}

	#ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
	#else
		path_type now_path(path_name.c_str());
	#endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		r_recursion_search_of_files(now_path, file_list);
	}

	template<typename Path_Container>
	static void r_recursion_search_of_files(const path_type& path, Path_Container& file_list)
	{
		try
		{
			boost::filesystem::directory_iterator end_iter;
			for(boost::filesystem::directory_iterator i(path); i != end_iter ; ++i)
			{
				if(boost::filesystem::is_directory(*i))
				{
					r_recursion_search_of_files(*i, file_list);
					continue;
				}

				if(boost::filesystem::is_regular_file(*i))
				{
					//file_list.push_back(i->string());
					file_list.push_back(i->path().string());
				}
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename Path_Container>
	static void recursion_search_of_directorys(const std::string& path, Path_Container& dir_list)
	{
		std::string path_name = base_type::check_path_name(path);

		if(path_name.empty())
		{
			path_name = "./";
		}

	#ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
	#else
		path_type now_path(path_name.c_str());
	#endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		r_recursion_search_of_directorys(now_path, dir_list);
	}

	template<typename Path_Container>
	static void recursion_search_of_all_type(const std::string& path, Path_Container& name_list)
	{
		std::string path_name = base_type::check_path_name(path);

		if(path_name.empty())
		{
			path_name = "./";
		}

	#ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
	#else
		path_type now_path(path_name.c_str());
	#endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		r_recursion_search_of_all_type(now_path, name_list);
	}


private:

	template<typename Path_Container>
	static void r_recursion_search_of_directorys(const path_type& path, Path_Container& dir_list)
	{
		try
		{
			boost::filesystem::directory_iterator end_iter;
			for(boost::filesystem::directory_iterator i(path); i != end_iter ; ++i)
			{
				if(boost::filesystem::is_directory(*i))
				{
					r_recursion_search_of_directorys(*i, dir_list);
					//dir_list.push_back(i->string());
					dir_list.push_back(i->path().string());
				}
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename Path_Container>
	static void r_recursion_search_of_all_type(const path_type& path, Path_Container& name_list)
	{
		try
		{
			boost::filesystem::directory_iterator end_iter;
			for(boost::filesystem::directory_iterator i(path); i != end_iter ; ++i)
			{
				if(boost::filesystem::is_directory(*i))
				{
					r_recursion_search_of_all_type(*i, name_list);
				}

				//name_list.push_back(i->string());
				name_list.push_back(i->path().string());
			}
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}
};

} // namespace file_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif // __YGGR_FILE_SYSTEM_BASIC_LOCAL_FILE_OPERATOR_HPP__
