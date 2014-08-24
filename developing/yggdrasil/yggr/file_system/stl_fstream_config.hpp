//stl_fstream_config.hpp

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

//stl boost only switch one
#ifndef __YGGR_FILE_SYSTEM_FSTREAM_CONFIG_HPP__ 
#define __YGGR_FILE_SYSTEM_FSTREAM_CONFIG_HPP__

#include <yggr/file_system/fstream_config.hpp>
#include <fstream>
#include <boost/filesystem/path.hpp>
namespace yggr
{
namespace file_system
{

typedef fstream_config<std::ifstream, std::ofstrem, std::filebuf, boost::filesystem::path> fstream_config_type;

} // namespace file_system
} // namespace yggr

#endif //__YGGR_FILE_SYSTEM_FSTREAM_CONFIG_HPP__