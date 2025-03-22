//path_opt.hpp

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

#ifndef __YGGR_FILE_SYSTEM_DETAIL_PATH_OPT_HPP__
#define __YGGR_FILE_SYSTEM_DETAIL_PATH_OPT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/filesystem/path.hpp>

namespace yggr
{
namespace file_system
{
namespace detail
{

inline boost::filesystem::path::string_type& 
	get_path_string(boost::filesystem::path& path)
{
	typedef boost::filesystem::path path_type;
	typedef path_type::string_type string_type;
	return const_cast<string_type&>(path.native());
}

inline const boost::filesystem::path::string_type& 
	get_path_string(const boost::filesystem::path& path)
{
	return path.native();
}

} //namespace detail

using detail::get_path_string;

} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_DETAIL_PATH_OPT_HPP__
