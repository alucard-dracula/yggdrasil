//rename_handlers.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_FILE_SYSTEM_RENAME_HANDLERS_HPP__
#define __YGGR_FILE_SYSTEM_RENAME_HANDLERS_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/charset/const_strings.hpp>
#include <yggr/container/reserve.hpp>

#include <yggr/file_system/path_check.hpp>
#include <yggr/file_system/detail/path_opt.hpp>
#include <yggr/file_system/local_fsys_native_operators.hpp>

#include <boost/ref.hpp>

#include <sstream>

namespace yggr
{
namespace file_system
{

// ext
struct rename_ext_empty
{
public:
	typedef native::path path_type;
	typedef path_type::value_type path_char_type;

private:
	typedef rename_ext_empty this_type;

private:
	inline static const path_char_type* prv_s_ext(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(ext, "");
		return YGGR_PP_CONST_STRING_LOCAL_GET(ext, path_char_type);
	}

public:
	inline const path_char_type* operator()(void) const
	{
		return this_type::prv_s_ext();
	}
};

struct rename_ext_bak
{
public:
	typedef native::path path_type;
	typedef path_type::value_type path_char_type;

private:
	typedef rename_ext_bak this_type;

private:
	inline static const path_char_type* prv_s_ext(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(ext, ".bak-");
		return YGGR_PP_CONST_STRING_LOCAL_GET(ext, path_char_type);
	}

public:
	inline const path_char_type* operator()(void) const
	{
		return this_type::prv_s_ext();
	}
};

// rename_handlers

template<typename Path>
struct not_rename
{
	typedef Path path_type;

	not_rename(void)
	{
	}

	template<typename T>
	not_rename(const T&)
	{
	}

	~not_rename(void)
	{
	}

public:
	// using lazy evaluation
	inline const path_type& operator()(void) const
	{
		return prv_s_empty_path();
	}

	inline const path_type& src_path(void) const
	{
		return prv_s_empty_path();
	}

private:
	inline static const path_type& prv_s_empty_path(void)
	{
		static const path_type p;
		return p;
	}
};

template<typename Ext>
struct rename_of_count
{
	typedef Ext ext_type;
	typedef typename ext_type::path_type path_type;
	typedef typename path_type::string_type path_string_type;
	typedef typename ext_type::path_char_type path_char_type;

public:
	rename_of_count(const path_type& path)
		: _src_path(path), _idx(0), _size(0)
	{
	}

	~rename_of_count(void)
	{
	}

public:
	// using lazy evaluation
	const path_type& operator()(void) const
	{
		typedef std::basic_stringstream<path_char_type> ss_type;

		prv_init();
		path_string_type& path_str = get_path_string(_dst_path);
		path_string_type& tail_str = get_path_string(_tail);

		native::error_code ec;
		native::file_status sat;

		for(++_idx; ; ++_idx)
		{
			path_str.erase(_size);
			ss_type ss;
			ss << _idx;
			ss >> tail_str;
			_dst_path += _tail;

			sat = native::call_symlink_status_ex(_dst_path, boost::addressof(ec));
			switch(sat.type())
			{
			case boost::filesystem::status_error:
				_dst_path.clear();
				_tail.clear();
			case boost::filesystem::file_not_found:
				return _dst_path;
			default:
				ec.clear();
				break;
			}
		}
	}

	inline const path_type& src_path(void) const
	{
		return _src_path;
	}

private:
	inline void prv_init(void) const
	{
		if(_tail.native().empty() && !_src_path.empty())
		{
			path_string_type& path_str = get_path_string(_dst_path);
			path_string_type& tail_str = get_path_string(_tail);

			ext_type ext;

			_dst_path = _src_path;

			if(path_check::is_slants(*(path_str.rbegin())))
			{
				path_str.erase(path_str.size() - 1);
			}

			_dst_path += ext();
			_size = path_str.size();
			container::reserve(tail_str, 5);
			container::reserve(path_str, _size + 5);
		}
	}

private:
	const path_type& _src_path;
	mutable std::size_t _idx;
	mutable std::size_t _size;
	mutable path_type _dst_path;
	mutable path_type _tail;
};

template<typename Ext>
struct rename_of_unique
{
	typedef Ext ext_type;
	typedef typename ext_type::path_type path_type;
	typedef typename path_type::string_type path_string_type;
	typedef typename ext_type::path_char_type path_char_type;

	typedef native::error_code error_code_type;

public:
	rename_of_unique(const path_type& path)
		: _src_path(path), _size(0)
	{
	}

	~rename_of_unique(void)
	{
	}

public:
	// using lazy evaluation
	const path_type& operator()(void) const
	{
		prv_init();
		path_string_type& path_str = get_path_string(_dst_path);
		//path_string_type& tail_str = get_path_string(_tail);

		native::error_code ec;
		native::file_status sat;

		for(;;)
		{
			path_str.erase(_size);
			_tail = native::unique_path("%%%%-%%%%-%%%%-%%%%", ec);
			if(!!ec)
			{
				_dst_path.clear();
				_tail.clear();
				return _dst_path;
			}
			_dst_path += _tail;

			sat = native::call_symlink_status_ex(_dst_path, boost::addressof(ec));
			switch(sat.type())
			{
			case boost::filesystem::status_error:
				_dst_path.clear();
				_tail.clear();
			case boost::filesystem::file_not_found:
				return _dst_path;
			default:
				ec.clear();
				break;
			}
		}
	}

	inline const path_type& src_path(void) const
	{
		return _src_path;
	}

private:

	inline void prv_init(void) const
	{
		if(_tail.native().empty() && !_src_path.empty())
		{
			path_string_type& path_str = get_path_string(_dst_path);
			path_string_type& tail_str = get_path_string(_tail);

			ext_type ext;
			_dst_path = _src_path;

			if(path_check::is_slants(*(path_str.rbegin())))
			{
				path_str.erase(path_str.size() - 1);
			}

			_dst_path += ext();
			_size = path_str.size();
			container::reserve(tail_str, 19); // 19 is "%%%%-%%%%-%%%%-%%%%" length
			container::reserve(path_str, _size + 19);
		}

	}

private:
	const path_type& _src_path;
	mutable std::size_t _size;
	mutable path_type _dst_path;
	mutable path_type _tail;
};

} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_RENAME_HANDLERS_HPP__
