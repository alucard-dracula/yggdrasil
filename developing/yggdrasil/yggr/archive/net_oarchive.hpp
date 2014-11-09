//net_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_NET_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_NET_OARCHIVE_HPP__

#include <ostream>
#include <boost/config.hpp>
#include <yggr/archive/net_oarchive_impl.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

namespace yggr  
{ 
namespace archive 
{

class net_oarchive 
	: public net_oarchive_impl<
				yggr::archive::net_oarchive, 
				std::ostream::char_type, 
				std::ostream::traits_type
			>
{
public:
	typedef std::ostream::char_type char_type;
	typedef std::ostream::traits_type traits_type;

	typedef net_oarchive_impl<
				yggr::archive::net_oarchive, 
				std::ostream::char_type, 
				std::ostream::traits_type
			> base_type;

private:
	typedef net_oarchive this_type;

public:
	net_oarchive(std::ostream & os, unsigned int flags = 0) 
		: base_type(os, flags)
	{}

	net_oarchive(std::streambuf & bsb, unsigned int flags = 0) 
		: base_type(bsb, flags)
	{}
};

typedef net_oarchive naked_net_oarchive;

} // namespace archive
} // namespace yggr 

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::net_oarchive)
BOOST_SERIALIZATION_USE_ARRAY_OPTIMIZATION(yggr::archive::net_oarchive)

#endif // __YGGR_ARCHIVE_NET_OARCHIVE_HPP__
