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
#endif // __GNUC__

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <compatibility/stl_exception.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/ptr_single.hpp>
#include <yggr/charset/const_strings.hpp>

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
private:
	typedef file_check this_type;

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

		stringstream_type ss_azs;
		ss_azs << "AZaz";
		string_type str_azs(ss_azs.str().c_str());

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

		if( (str.find(ss_dslant.str().c_str()) != string_type::npos)
			|| (str.find_first_of(ss_failed_char.str().c_str()) != string_type::npos))
		{
			return string_type();
		}

		stringstream_type ss_colon;
		ss_colon << ":";

		std::size_t i = str.find_last_of(ss_colon.str().c_str());
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

	YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_newline, "\r\n");

public:
	typedef std::list<std::string> path_list_type;
	typedef size_type file_size_type;

	typedef typename fstream_cfg_type::ifstream_type ifstream_type;
	typedef typename fstream_cfg_type::ofstream_type ofstream_type;
	typedef typename fstream_cfg_type::fbuf_type fbuf_type;
	typedef typename fstream_cfg_type::path_type path_type;
	typedef typename fstream_cfg_type::char_type char_type;
	typedef typename fstream_cfg_type::char_type fstream_char_type;

	typedef file_check base_type;

public:

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char>
	static file_size_type create_file_of_binary(const Char* fname, const file_size_type& size)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname?this_type::create_file_of_binary(string_type(fname), size) : 0;
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type create_file_of_binary(const Basic_String<Char, Traits, Alloc>& fname, const file_size_type& size)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		assert(!fname.empty());
		if(fname.empty())
		{
			return file_size_type(0);
		}

		string_type path_name;
		string_type file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return file_size_type(0);
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return file_size_type(0);
			}
		}

		fbuf_type fbuf;
		fbuf.open((path_name + file_name).c_str(), std::ios_base::in | std::ios_base::out
							| std::ios_base::trunc | std::ios_base::binary);

		fbuf.pubseekoff(size - 1, std::ios_base::beg);
		fbuf.sputc(char_type());
		fbuf.close();

		return size;
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char>
	static file_size_type write_file_of_binary(const Char* fname, const char* buffer, file_size_type size)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname? this_type::write_file_of_binary(string_type(fname), buffer, size) : 0;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type write_file_of_binary(const Basic_String<Char, Traits, Alloc>& fname, const char* buffer, file_size_type size)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		assert(!fname.empty());

		if(!buffer || !size)
		{
			return file_size_type(0);
		}

		string_type path_name;
		string_type file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return file_size_type(0);
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return file_size_type(0);
			}
		}

		ofstream_type ofs;
		ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary);
		ofs.write(buffer, size);
		ofs.close();

		return size;
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type write_file_of_binary(const Char* fname,
												const Basic_String<char, Buf_Traits, Buf_Alloc>& buffer)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname? this_type::write_file_of_binary(string_type(fname), buffer) : 0;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_FName,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_Buf>
	static file_size_type write_file_of_binary(const Basic_String_FName<Char, Traits, Alloc>& fname,
													const Basic_String_Buf<char, Buf_Traits, Buf_Alloc>& buffer)
	{
		typedef Basic_String_FName<Char, Traits, Alloc> string_type;

		assert(!fname.empty());
		if(buffer.empty())
		{
			return file_size_type(0);
		}

		string_type path_name;
		string_type file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return file_size_type(0);
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return file_size_type(0);
			}
		}

		ofstream_type ofs;
		ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary);
		ofs.write(buffer.data(), buffer.size());
		ofs.close();

		return buffer.size();
	}

	// throw compatibility::stl_exception
	static file_size_type write_file_of_binary(ofstream_type& ofs, const char* buffer, file_size_type size, bool bflush = false)
	{
		assert(ofs.rdstate() & std::ios::binary);

		if(!buffer || !size)
		{
			return file_size_type(0);
		}

		if(!ofs.good())
		{
			return file_size_type(0);
		}

		ofs.write(buffer, size);
		//ofs.close();
		if(bflush)
		{
			ofs.flush();
		}

		return size;
	}

	// throw compatibility::stl_exception
	template<typename Buf_Traits, typename Buf_Alloc,
				template<typename Char, typename Traits, typename Alloc>
				class Basic_String >
	static file_size_type write_file_of_binary(ofstream_type& ofs, const Basic_String<char, Buf_Traits, Buf_Alloc>& buffer, bool bflush = false)
	{
		assert(ofs.rdstate() & std::ios::binary);

		if(buffer.empty())
		{
			return file_size_type(0);
		}

		if(!ofs.good())
		{
			return file_size_type(0);
		}

		ofs.write(buffer.data(), buffer.size());
		if(bflush)
		{
			ofs.flush();
		}
		//ofs.close();

		return buffer.size();
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char>
	static file_size_type append_file_of_binary(const Char* fname, const char* buffer, file_size_type size)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname? this_type::append_file_of_binary(string_type(fname), buffer, size) : 0;
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Tratis, typename _Alloc>
				class Basic_String >
	static file_size_type append_file_of_binary(const Basic_String<Char, Traits, Alloc>& fname, const char* buffer, file_size_type size)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		assert(!fname.empty());
		if(!buffer || !size)
		{
			return file_size_type(0);
		}

		string_type path_name;
		string_type file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return file_size_type(0);
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return file_size_type(0);
			}
		}

		ofstream_type ofs;
		ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary | std::ios::app);
		ofs.write(buffer, size);
		ofs.close();

		return size;
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type append_file_of_binary(const Char* fname,
												const Basic_String<char, Buf_Traits, Buf_Alloc>& buffer)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname? this_type::append_file_of_binary(string_type(fname), buffer) : 0;
	}

	// throw compatibility::stl_exception throw boost::filesystem::filesystem_error
	template<typename Char, typename Traits, typename Alloc, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_FName,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_Buf >
	static file_size_type append_file_of_binary(const Basic_String_FName<Char, Traits, Alloc>& fname,
													const Basic_String_Buf<char, Buf_Traits, Buf_Alloc>& buffer)
	{
		typedef Basic_String_FName<Char, Traits, Alloc> string_type;

		assert(!fname.empty());
		if(buffer.empty())
		{
			return file_size_type(0);
		}

		string_type path_name;
		string_type file_name;

		base_type::parsing_file_path_and_name(fname, path_name, file_name);

		if(file_name.empty())
		{
			return file_size_type(0);
		}

		if(!path_name.empty())
		{
			if(!create_path(path_name))
			{
				return file_size_type(0);
			}
		}

		ofstream_type ofs;
		ofs.open((path_name + file_name).c_str(), std::ios::out | std::ios::binary | std::ios::app);
		ofs.write(buffer.data(), buffer.size());
		ofs.close();

		return buffer.size();
	}

	// throw compatibility::stl_exception
	template<typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type read_file_of_binary(ifstream_type& ifs, Basic_String<char, Buf_Traits, Buf_Alloc>& buffer, file_size_type& size)
	{
		assert(ifs.rdstate() & std::ios::binary);

		if(!size)
		{
			size = 0;
			return file_size_type(0);
		}

		std::string buf(size, 0);

		if(!ifs.good())
		{
			size = 0;
			return file_size_type(0);
		}
		ifs.read(&buf[0], size);

		buffer.swap(buf);
		size = buffer.size();
		return size;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type read_file_of_binary(const Char* fname,
												Basic_String<char, Buf_Traits, Buf_Alloc>& buffer)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		file_size_type size = 0;
		return fname? this_type::read_file_of_binary(string_type(fname), buffer, size) : 0;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_FName,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_Buf>
	static file_size_type read_file_of_binary(const Basic_String_FName<Char, Traits, Alloc>& fname,
												Basic_String_Buf<char, Buf_Traits, Buf_Alloc>& buffer)
	{
		file_size_type size = 0;
		return this_type::read_file_of_binary(fname, buffer, size);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type read_file_of_binary(const Char* fname,
												Basic_String<char, Buf_Traits, Buf_Alloc>& buffer,
												file_size_type& size)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname? this_type::read_file_of_binary(string_type(fname), buffer, size) : 0;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Buf_Traits, typename Buf_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_FName,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_Buf>
	static file_size_type read_file_of_binary(const Basic_String_FName<Char, Traits, Alloc>& fname,
												Basic_String_Buf<char, Buf_Traits, Buf_Alloc>& buffer,
												file_size_type& size)
	{
		typedef Basic_String_FName<Char, Traits, Alloc> string_type;
		typedef Basic_String_Buf<char, Buf_Traits, Buf_Alloc> buf_type;
		string_type file_name = base_type::check_file_name(fname);

		assert(!file_name.empty());
		if(file_name.empty())
		{
			return file_size_type(0);
		}

#	ifdef _MSC_VER
		path_type fpath(fname.c_str(), boost::filesystem::native);
#	else
		path_type fpath(fname.c_str());
#	endif // _MSC_VER
		size = boost::filesystem::file_size(fpath);
		if(!size)
		{
			size = 0;
			return file_size_type(0);
		}

		buf_type buf(size, 0);

		ifstream_type ifs;
		ifs.open(fname.c_str(), std::ios::in | std::ios::binary);
		if(!ifs.good())
		{
			size = 0;
			return size;
		}
		ifs.read(&buf[0], size);
		ifs.close();

		buffer.swap(buf);
		return size;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool create_directory(const Char* path,  const Char* dir)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return path && dir && this_type::create_directory(string_type(path), string_type(dir));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_directory(const Char* path,
									const Basic_String<Char, Traits, Alloc>& dir)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return path && this_type::create_directory(string_type(path), dir);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_directory(const Basic_String<Char, Traits, Alloc>& path,
									const Char* dir)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return dir && this_type::create_directory(path, string_type(dir));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_directory(const Basic_String<Char, Traits, Alloc>& path,
									const Basic_String<Char, Traits, Alloc>& dir)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;

		string_type path_name = base_type::check_path_name(path);
		string_type full_path_name = path_name + dir;

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_current_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_current_dir, Char);
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

		boost::filesystem::create_directory(full_path);

		return true;
	};

	// non throw
	template<typename Char>
	static bool is_directory(const Char* str)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return str && this_type::is_directory(string_type(str));
	}

	// non throw
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool is_directory(const Basic_String<Char, Traits, Alloc>& str)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(str);
		if(path_name.empty())
		{
			return false;
		}

		path_type path(this_type::make_path_object(path_name));

		return boost::filesystem::is_directory(path);
	}

	// non throw
	template<typename Char>
	static bool is_exists_path(const Char* full_name)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return full_name && this_type::is_exists_path(string_type(full_name));
	}

	// non throw
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool is_exists_path(const Basic_String<Char, Traits, Alloc>& full_name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_file_name(full_name);

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

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool create_path(const Char* full_name)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return full_name && this_type::create_path(string_type(full_name));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_path(const Basic_String<Char, Traits, Alloc>& full_name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef typename string_type::iterator str_iter_type;

		string_type path_name = base_type::check_path_name(full_name);

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
		string_type now_path_name;

		YGGR_PP_CONST_CHAR_LOCAL_DEF(symbol_slash, '/');

		for(;;)
		{
			str_iter_type iter = std::find(left, right, YGGR_PP_CONST_CHAR_LOCAL_GET(symbol_slash, Char));
			if(iter != right)
			{
				now_path_name = string_type(begin, iter + 1);
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

				boost::filesystem::create_directory(now_path);

				left = iter + 1;
			}
			else
			{
				break;
			}
		}

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool rename_directory(const Char* oldname,
									const Char* newname)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return oldname && newname && this_type::rename_directory(string_type(oldname), string_type(newname));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool rename_directory(const Char* oldname,
									const Basic_String<Char, Traits, Alloc>& newname)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return oldname && this_type::rename_directory(string_type(oldname), newname);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool rename_directory(const Basic_String<Char, Traits, Alloc>& oldname,
									const Char* newname)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return newname && this_type::rename_directory(oldname, string_type(newname));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool rename_directory(const Basic_String<Char, Traits, Alloc>& oldname,
									const Basic_String<Char, Traits, Alloc>& newname)
	{
		typedef  Basic_String<Char, Traits, Alloc> string_type;
		string_type old_path_name;
		string_type old_dir_name;
		string_type new_path_name;
		string_type new_dir_name;

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

		boost::filesystem::rename(old_path, new_path);

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool copy_directory(const Char* src,
								const Char* dst)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return src && dst && this_type::copy_directory(string_type(src), string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_directory(const Char* src,
								const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return src && this_type::copy_directory(string_type(src), dst);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_directory(const Basic_String<Char, Traits, Alloc>& src,
								const Char* dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return dst && this_type::copy_directory(src, string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_directory(const Basic_String<Char, Traits, Alloc>& src,
								const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;
		string_type src_path_name = base_type::check_path_name(src);
		string_type dst_path_name = base_type::check_path_name(dst);

		if(src_path_name.empty() || dst_path_name.empty())
		{
			return false;
		}

#	ifdef _MSC_VER
		path_type src_path(src_path_name.c_str(), boost::filesystem::native);
#	else
		path_type src_path(src_path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(src_path) || !boost::filesystem::is_directory(src_path))
		{
			return false;
		}

		YGGR_PP_CONST_STRING_LOCAL_DEF(ext_bak, ".bak");
		if(src_path_name == dst_path_name)
		{
			for(size_type i = 1; ;++i)
			{
				stringstream_type ss;
				ss << i;
				//dst_path_name = (src_path_name + ".bak") + ss.str().c_str();
				dst_path_name = (src_path_name + YGGR_PP_CONST_STRING_LOCAL_GET(ext_bak, Char)) + ss.str().c_str();
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

		boost::filesystem::copy_directory(src_path, dst_path);

		return true;
	}

	// throw  boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool remove_directory(const Char* full_name, bool b_recursion = false)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return full_name && this_type::remove_directory(string_type(full_name), b_recursion);
	}

	// throw  boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool remove_directory(const Basic_String<Char, Traits, Alloc>& full_name, bool b_recursion = false)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(full_name);

#	ifdef _MSC_VER
		path_type full_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type full_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(full_path))
		{
			return true;
		}

		if(!boost::filesystem::is_directory(full_path))
		{
			return false;
		}

		if(b_recursion)
		{
			boost::filesystem::remove_all(full_path);
		}
		else
		{
			boost::filesystem::remove(full_path);
		}

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool copy_file(const Char* path1, const Char* path2,
							bool b_overwrite = false)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return path1 && path2 && this_type::copy_file(string_type(path1), string_type(path2), b_overwrite);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_file(const Char* path1,
							const Basic_String<Char, Traits, Alloc>& path2,
							bool b_overwrite = false)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return path1 && this_type::copy_file(string_type(path1), path2, b_overwrite);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_file(const Basic_String<Char, Traits, Alloc>& path1,
							const Char* path2,
							bool b_overwrite = false)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return path2 && this_type::copy_file(path1, string_type(path2), b_overwrite);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_file(const Basic_String<Char, Traits, Alloc>& path1,
							const Basic_String<Char, Traits, Alloc>& path2,
							bool b_overwrite = false)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		typedef std::basic_stringstream<Char> stringstream_type;
		string_type src_path_name = base_type::check_file_name(path1);
		string_type dst_path_name = base_type::check_file_name(path2);

		if(src_path_name.empty() || dst_path_name.empty())
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

		YGGR_PP_CONST_STRING_LOCAL_DEF(ext_bak, ".bak");
		if(src_path_name == dst_path_name)
		{
			for(size_type i = 1; ;++i)
			{
				stringstream_type ss;
				ss << i;
				//dst_path_name = (src_path_name + ".bak") + ss.str().c_str();
				dst_path_name = (src_path_name + YGGR_PP_CONST_STRING_LOCAL_GET(ext_bak, Char)) + ss.str().c_str();
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

		if(b_overwrite)
		{
			boost::filesystem::copy_file(src_path, dst_path, boost::filesystem::copy_option::overwrite_if_exists);
		}
		else
		{
			boost::filesystem::copy_file(src_path, dst_path, boost::filesystem::copy_option::fail_if_exists);
		}

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool remove_file(const Char* full_name)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return full_name && this_type::remove_file(string_type(full_name));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool remove_file(const Basic_String<Char, Traits, Alloc>& full_name)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type file_name = base_type::check_file_name(full_name);
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

		boost::filesystem::remove(file_path);

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool cut_file(const Char* path1, const Char* path2, bool b_compulsion = false)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return path1 && path2 && this_type::cut_file(string_type(path1), string_type(path2), b_compulsion);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool cut_file(const Char* path1,
							const Basic_String<Char, Traits, Alloc>& path2,
							bool b_compulsion = false)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return path1 && this_type::cut_file(string_type(path1), path2, b_compulsion);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool cut_file(const Basic_String<Char, Traits, Alloc>& path1,
							const Char* path2,
							bool b_compulsion = false)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return path2 && this_type::cut_file(path1, string_type(path2), b_compulsion);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool cut_file(const Basic_String<Char, Traits, Alloc>& path1,
							const Basic_String<Char, Traits, Alloc>& path2,
							bool b_compulsion = false)
	{
		return this_type::copy_file(path1, path2, b_compulsion) && this_type::remove_file(path1);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool rename_file(const Char* oldname,
								const Char* newname)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return oldname && newname && this_type::rename_file(string_type(oldname), string_type(newname));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool rename_file(const Char* oldname,
								const Basic_String<Char, Traits, Alloc>& newname)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return oldname && this_type::rename_file(string_type(oldname), newname);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool rename_file(const Basic_String<Char, Traits, Alloc>& oldname,
								const Char* newname)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return newname && this_type::rename_file(oldname, string_type(newname));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool rename_file(const Basic_String<Char, Traits, Alloc>& oldname,
								const Basic_String<Char, Traits, Alloc>& newname)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type old_path_name;
		string_type old_file_name;
		string_type new_path_name;
		string_type new_file_name;

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

		boost::filesystem::rename(old_path, new_path);

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static file_size_type file_size(const Char* str_path)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return str_path? this_type::file_size(string_type(str_path)) : 0;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static file_size_type file_size(const Basic_String<Char, Traits, Alloc>& str_path)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type file_name = base_type::check_file_name(str_path);
		if(file_name.empty())
		{
			return 0;
		}

		file_size_type size = 0;

#	ifdef _MSC_VER
		path_type fpath(str_path.c_str(), boost::filesystem::native);
#	else
		path_type fpath(str_path.c_str());
#	endif // _MSC_VER
		size = boost::filesystem::file_size(fpath);

		return size;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Value>
	static bool write_file_of_text(const Char* fname, const Value& val,
									const char_type* spr = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type))
	{
		typedef yggr::charset::basic_string<Char> string_type;
		typedef yggr::charset::basic_string<fstream_char_type> spr_type;
		return fname && this_type::write_file_of_text(string_type(fname), val, spr_type(spr));
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Value,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static bool write_file_of_text(const Basic_String<Char, Traits, Alloc>& fname,
										const Value& val,
										const fstream_char_type* spr = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type))
	{
		typedef yggr::charset::basic_string<fstream_char_type> spr_type;
		return this_type::write_file_of_text(fname, spr_type(spr));
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Value,
				typename SPR_Traits, typename SPR_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool write_file_of_text(const Char* fname,
										const Value& val,
										const Basic_String<fstream_char_type, SPR_Traits, SPR_Alloc>& spr
												= yggr::charset::basic_string<fstream_char_type>(
													YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type)))
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname && this_type::write_file_of_text(string_type(fname), spr);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Value,
				typename SPR_Traits, typename SPR_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_FName,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_SPR>
	static bool write_file_of_text(const Basic_String_FName<Char, Traits, Alloc>& fname,
										const Value& val,
										const Basic_String_SPR<fstream_char_type, SPR_Traits, SPR_Alloc>& spr
												= yggr::charset::basic_string<char>(
													YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type)))
	{
		typedef Basic_String_FName<Char, Traits, Alloc> string_type;
		string_type path_name;
		string_type file_name;

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

		ofs.open(file_name.c_str(), std::ios::out);
		if(!ofs.good())
		{
			return false;
		}
		ofs << val << spr;
		ofs.close();

		return true;
	}

	// throw compatibility::stl_exception
	template<typename Value>
	static bool write_file_of_text(ofstream_type& ofs, const Value& val,
										const fstream_char_type* spr
											= YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type))
	{
		typedef yggr::charset::basic_string<fstream_char_type> spr_type;
		return this_type::write_file_of_text(ofs, val, spr_type(spr));
	}

	// throw compatibility::stl_exception
	template<typename Value, typename SPR_Traits, typename SPR_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_SPR>
	static bool write_file_of_text(ofstream_type& ofs, const Value& val,
										const Basic_String_SPR<char, SPR_Traits, SPR_Alloc>& spr
												= yggr::charset::basic_string<fstream_char_type>(
													YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type)))
	{
		assert(!(ofs.rdstate() & std::ios::binary));
		if(!ofs.good())
		{
			return false;
		}

		ofs << val << spr;
		ofs.close();

		return true;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Value>
	static bool append_file_of_text(const Char* fname, const Value& val,
										const fstream_char_type* spr
												= YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type))
	{
		typedef yggr::charset::basic_string<Char> string_type;
		typedef yggr::charset::basic_string<fstream_char_type> spr_type;
		return fname && this_type::append_file_of_text(string_type(fname), val, spr_type(spr));
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Value,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static bool append_file_of_text(const Basic_String<Char, Traits, Alloc>& fname, const Value& val,
										const fstream_char_type* spr
												= YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type))
	{
		typedef yggr::charset::basic_string<fstream_char_type> spr_type;
		return this_type::append_file_of_text(fname, val, spr_type(spr));
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Value,
				typename SPR_Traits, typename SPR_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static bool append_file_of_text(const Char* fname, const Value& val,
										const Basic_String<fstream_char_type, SPR_Traits, SPR_Alloc>& spr
												= yggr::charset::basic_string<fstream_char_type>(
													YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type)))
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname && this_type::append_file_of_text(string_tpye(fname), val, spr);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Value,
				typename SPR_Traits, typename SPR_Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_FName,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String_SPR>
	static bool append_file_of_text(const Basic_String_FName<Char, Traits, Alloc>& fname, const Value& val,
										const Basic_String_SPR<char, SPR_Traits, SPR_Alloc>& spr
												= yggr::charset::basic_string<fstream_char_type>(
														YGGR_PP_CONST_STRING_LOCAL_GET(symbol_newline, fstream_char_type)))
	{
		typedef Basic_String_FName<Char, Traits, Alloc> string_type;
		string_type path_name;
		string_type file_name;

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

		ofs.open(file_name.c_str(), std::ios::out | std::ios::app);
		if(!ofs.good())
		{
			return false;
		}
		ofs << val;
		ofs.close();

		return true;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Value >
	static bool read_file_of_text(const Char* fname, Value& val)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return fname && this_type::read_file_of_text(string_type(fname), val);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Value,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool read_file_of_text(const Basic_String<Char, Traits, Alloc>& fname, Value& val)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type file_name = base_type::check_file_name(fname);
		if(file_name.empty())
		{
			return false;
		}

		ifstream_type ifs;
		ifs.open(file_name.c_str(), std::ios::in);
		if(!ifs.good())
		{
			return false;
		}
		ifs >> val;
		ifs.close();

		return true;
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Path_Container>
	static void search_files(const Char* path, Path_Container& file_list)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		if(!path)
		{
			return;
		}
		this_type::search_files(string_type(path), file_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void search_files(const Basic_String<Char, Traits, Alloc>& path, Path_Container& file_list)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(path);

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_root_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_root_dir, Char);
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		boost::filesystem::directory_iterator end_iter;
		for(boost::filesystem::directory_iterator i(now_path); i != end_iter; ++i)
		{
			if(boost::filesystem::is_regular_file(*i))
			{
				std::inserter(file_list, file_list.end()) = i->path().string();
				//file_list.push_back(i->path().string());
			}
		}
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Path_Container>
	static void search_directorys(const Char* path, Path_Container& dir_list)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		if(!path)
		{
			return;
		}
		this_type::search_directorys(string_type(path), dir_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void search_directorys(const Basic_String<Char, Traits, Alloc>& path, Path_Container& dir_list)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(path);

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_root_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_root_dir, Char);
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		boost::filesystem::directory_iterator end_iter;
		for(boost::filesystem::directory_iterator i(now_path); i != end_iter; ++i)
		{
			if(boost::filesystem::is_directory(*i))
			{
				std::inserter(dir_list, dir_list.end()) = i->path().string();
				//dir_list.push_back(i->path().string());
			}
		}
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Path_Container>
	static void search_all_type(const Char* path, Path_Container& name_list)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		if(!path)
		{
			return;
		}

		this_type::search_all_type(string_type(path), name_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void search_all_type(const Basic_String<Char, Traits, Alloc>& path, Path_Container& name_list)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(path);

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_root_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_root_dir, Char);
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || !boost::filesystem::is_directory(now_path))
		{
			return;
		}

		boost::filesystem::directory_iterator end_iter;
		for(boost::filesystem::directory_iterator i(now_path); i != end_iter; ++i)
		{
			std::inserter(name_list, name_list.end()) = i->path().string();
			//name_list.push_back(i->path().string());
		}
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Path_Container>
	static void recursion_search_of_files(const Char* path, Path_Container& file_list)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		if(!path)
		{
			return;
		}

		this_type::recursion_search_of_files(string_type(path), file_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void recursion_search_of_files(const Basic_String<Char, Traits, Alloc>& path, Path_Container& file_list)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(path);

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_root_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_root_dir, Char);
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		r_recursion_search_of_files(now_path, file_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Path_Container >
	static void recursion_search_of_directorys(const Char* path, Path_Container& dir_list)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		if(!path)
		{
			return;
		}

		this_type::recursion_search_of_directorys(string_type(path), dir_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void recursion_search_of_directorys(const Basic_String<Char, Traits, Alloc>& path, Path_Container& dir_list)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(path);

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_root_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_root_dir, Char);
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		r_recursion_search_of_directorys(now_path, dir_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Path_Container>
	static void recursion_search_of_all_type(const Char* path, Path_Container& name_list)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		if(!path)
		{
			return;
		}

		return recursion_search_of_all_type(string_type(path), name_list);
	}

	// throw compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc, typename Path_Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static void recursion_search_of_all_type(const Basic_String<Char, Traits, Alloc>& path, Path_Container& name_list)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_path_name(path);

		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_root_dir, "./");
		if(path_name.empty())
		{
			//path_name = "./";
			path_name = YGGR_PP_CONST_STRING_LOCAL_GET(symbol_root_dir, Char);
		}

#	ifdef _MSC_VER
		path_type now_path(path_name.c_str(), boost::filesystem::native);
#	else
		path_type now_path(path_name.c_str());
#	endif //_MSC_VER

		if(!boost::filesystem::exists(now_path) || ! boost::filesystem::is_directory(now_path))
		{
			return;
		}

		r_recursion_search_of_all_type(now_path, name_list);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename String>
	static String current_path(void)
	{
		typedef String string_type;
		return string_type(boost::filesystem::current_path().string());
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool create_hard_link(const Char* src, const Char* dst)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return src && dst && this_type::create_hard_link(string_type(src), string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_hard_link(const Char* src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return src && this_type::create_hard_link(string_type(src), dst);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_hard_link(const Basic_String<Char, Traits, Alloc>& src,
									const Char* dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return dst && this_type::create_hard_link(src, string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_hard_link(const Basic_String<Char, Traits, Alloc>& src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;

		string_type src_path_name = base_type::check_file_name(src);
		string_type dst_path_name = base_type::check_file_name(dst);

		path_type src_path(this_type::make_path_object(src_path_name));
		path_type dst_path(this_type::make_path_object(dst_path_name));

		if(src_path_name.empty() || dst_path_name.empty()
			|| !boost::filesystem::exists(src_path) || boost::filesystem::exists(dst_path))
		{
			return false;
		}

		boost::filesystem::create_hard_link(src_path, dst_path);
		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static yggr::u64 hard_link_count(const Char* str)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return str? this_type::hard_link_count(string_type(str)) : 0;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static yggr::u64 hard_link_count(const Basic_String<Char, Traits, Alloc>& str)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type path_name = base_type::check_file_name(str);
		if(path_name.empty())
		{
			return 0;
		}

		path_type path = this_type::make_path_object(path_name);
		return boost::filesystem::hard_link_count(path);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool create_symlink(const Char* src, const Char* dst)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return src && dst && this_type::create_symlink(string_type(src), string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_symlink(const Char* src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return src && this_type::create_symlink(string_type(src), dst);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_symlink(const Basic_String<Char, Traits, Alloc>& src,
									const Char* dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return dst && this_type::create_symlink(src, string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_symlink(const Basic_String<Char, Traits, Alloc>& src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;

		string_type src_path_name = base_type::check_file_name(src);
		string_type dst_path_name = base_type::check_file_name(dst);

		path_type src_path(this_type::make_path_object(src_path_name));
		path_type dst_path(this_type::make_path_object(dst_path_name));

		if(src_path_name.empty() || dst_path_name.empty()
			|| !boost::filesystem::exists(src_path) || boost::filesystem::exists(dst_path))
		{
			return false;
		}

		boost::filesystem::create_symlink(src_path, dst_path);

		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool copy_symlink(const Char* src, const Char* dst)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return src && dst && this_type::copy_symlink(string_type(src), string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_symlink(const Char* src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return src && this_type::copy_symlink(string_type(src), dst);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_symlink(const Basic_String<Char, Traits, Alloc>& src,
									const Char* dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return dst && this_type::copy_symlink(src, string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool copy_symlink(const Basic_String<Char, Traits, Alloc>& src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;

		string_type src_path_name = base_type::check_file_name(src);
		string_type dst_path_name = base_type::check_file_name(dst);

		path_type src_path(this_type::make_path_object(src_path_name));
		path_type dst_path(this_type::make_path_object(dst_path_name));

		if(src_path_name.empty() || dst_path_name.empty()
			|| !boost::filesystem::exists(src_path) || boost::filesystem::exists(dst_path))
		{
			return false;
		}

		boost::filesystem::copy_symlink(src_path, dst_path);
		return true;
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char>
	static bool create_directory_symlink(const Char* src, const Char* dst)
	{
		typedef yggr::charset::basic_string<Char> string_type;
		return src && dst && this_type::create_directory_symlink(string_type(src), string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_directory_symlink(const Char* src,
									const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return src && this_type::create_directory_symlink(string_type(src), dst);
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_directory_symlink(const Basic_String<Char, Traits, Alloc>& src,
									const Char* dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		return dst && this_type::create_directory_symlink(src, string_type(dst));
	}

	// throw boost::filesystem::filesystem_error compatibility::stl_exception
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool create_directory_symlink(const Basic_String<Char, Traits, Alloc>& src,
											const Basic_String<Char, Traits, Alloc>& dst)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;

		string_type src_path_name = base_type::check_path_name(src);
		string_type dst_path_name = base_type::check_file_name(dst);

		path_type src_path(this_type::make_path_object(src_path_name));
		path_type dst_path(this_type::make_path_object(dst_path_name));

		if(src_path_name.empty() || dst_path_name.empty()
			|| !boost::filesystem::exists(src_path) || boost::filesystem::exists(dst_path))
		{
			return false;
		}

		boost::filesystem::create_directory_symlink(src_path, dst_path);
		return true;
	}

private:
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	inline static path_type make_path_object(const Basic_String<Char, Traits, Alloc>& str_path)
	{
#	ifdef _MSC_VER
		return path_type(str_path.c_str(), boost::filesystem::native);
#	else
		return path_type(str_path.c_str());
#	endif //_MSC_VER
	}

	// throw compatibility::stl_exception
	template<typename Path_Container>
	static void r_recursion_search_of_files(const path_type& path, Path_Container& file_list)
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
				file_list.push_back(i->path().string());
			}
		}
	}

	// throw compatibility::stl_exception
	template<typename Path_Container>
	static void r_recursion_search_of_directorys(const path_type& path, Path_Container& dir_list)
	{
		boost::filesystem::directory_iterator end_iter;
		for(boost::filesystem::directory_iterator i(path); i != end_iter ; ++i)
		{
			if(boost::filesystem::is_directory(*i))
			{
				r_recursion_search_of_directorys(*i, dir_list);
				dir_list.push_back(i->path().string());
			}
		}
	}

	// throw compatibility::stl_exception
	template<typename Path_Container>
	static void r_recursion_search_of_all_type(const path_type& path, Path_Container& name_list)
	{
		boost::filesystem::directory_iterator end_iter;
		for(boost::filesystem::directory_iterator i(path); i != end_iter ; ++i)
		{
			if(boost::filesystem::is_directory(*i))
			{
				r_recursion_search_of_all_type(*i, name_list);
			}
			name_list.push_back(i->path().string());
		}
	}
};

} // namespace file_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif // __YGGR_FILE_SYSTEM_BASIC_LOCAL_FILE_OPERATOR_HPP__
