// packet_info_caster.hpp

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

#ifndef __YGGR_PACKET_PACKET_INFO_CASTER_HPP__
#define __YGGR_PACKET_PACKET_INFO_CASTER_HPP__

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#	include <yggr/packet/detail/packet_info_caster_impl_vtpl.hpp>
#else
#	include <yggr/packet/detail/packet_info_caster_impl_vmacro.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace packet
{

template<typename T, typename Trans = mplex::identity_meta_foo >
struct packet_info_caster
	: public detail::packet_info_caster_impl<T, Trans>
{
};

} // namespace packet
} // namespace yggr

#endif // __YGGR_PACKET_PACKET_INFO_CASTER_HPP__
