//binary_woarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BINARY_WOARCHIVE_HPP__
#define __YGGR_ARCHIVE_BINARY_WOARCHIVE_HPP__


#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <ostream>
#include <yggr/archive/binary_oarchive_impl.hpp>
#include <yggr/archive/impl/codecvt_null.hpp>
#include <yggr/archive/impl/utf8_codecvt_facet.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

namespace yggr 
{ 
namespace archive 
{

class binary_woarchive
	: public binary_oarchive_impl<
				yggr::archive::binary_woarchive,
				std::wostream::char_type, 
				std::wostream::traits_type
			>
{
public:
	typedef std::wostream::char_type char_type;
	typedef std::wostream::traits_type traits_type;

	typedef binary_oarchive_impl<
				yggr::archive::binary_woarchive,
				std::wostream::char_type, 
				std::wostream::traits_type
			> base_type;
private:
	typedef binary_woarchive this_type;
public:
	binary_woarchive(std::wostream & os, unsigned int flags = 0)
		: base_type(os, flags)
	{
	}

	binary_woarchive(std::wstreambuf & bsb, unsigned int flags = 0) 
		: base_type(bsb, flags)
	{
	}
};

typedef binary_woarchive naked_binary_woarchive;

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::binary_woarchive)

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // __YGGR_ARCHIVE_BINARY_WOARCHIVE_HPP__
