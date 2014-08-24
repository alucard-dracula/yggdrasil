//fstream_config.hpp

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

#ifndef __YGGR_FILE_SYSTEM_FSTREM_CONFIG_HPP__
#define __YGGR_FILE_SYSTEM_FSTREM_CONFIG_HPP__

#include <boost/mpl/assert.hpp>
#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace file_system
{

template<typename Input_Stream, typename Output_Stream, typename Buffer, typename Path>
class fstream_config : private nonable::noncreateable
{
public:
	typedef Buffer fbuf_type;
	typedef Input_Stream ifstream_type;
	typedef Output_Stream ofstream_type;
	typedef Path path_type;
	typedef typename fbuf_type::char_type char_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<char_type, typename ifstream_type::char_type >));
	BOOST_MPL_ASSERT((boost::is_same<char_type, typename ofstream_type::char_type >));
};

} // namespace file_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif // __YGGR_FILE_SYSTEM_FSTREM_CONFIG_HPP__