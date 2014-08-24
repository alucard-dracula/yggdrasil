//data_info_def.hpp

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

#ifndef __YGGR_NETWORK_SUPPORT_DATA_INFO_DEF_HPP__
#define __YGGR_NETWORK_SUPPORT_DATA_INFO_DEF_HPP__

#include <yggr/packet/packet_info.hpp>

namespace yggr
{
namespace network
{
namespace support
{

template<typename ID, typename Ver>
class network_data_info_def
{
public:
	typedef ID data_id_type;
	typedef Ver data_ver_type;
	typedef yggr::packet::packet_info<ID, Ver> data_info_type;
};

} // namespace support
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SUPPORT_DATA_INFO_DEF_HPP__