// exception_serialization_error_code.hpp

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

#ifndef __YGGR_EXCEPTION_SERIALIZATION_ERROR_CODE_HPP__
#define __YGGR_EXCEPTION_SERIALIZATION_ERROR_CODE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/exception/error_code.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/utf8_string.hpp>

namespace boost
{
namespace serialization
{

template<typename Archive> inline
void serialize(Archive& ar,
				yggr::exception::error_code& t,
				const unsigned int ver) 
{
	ar & YGGR_SERIALIZATION_NAME_NVP("time", t.time());
	ar & YGGR_SERIALIZATION_NAME_NVP("thread_id", t.thread_id());
	ar & YGGR_SERIALIZATION_NAME_NVP("code", t.code());
	ar & YGGR_SERIALIZATION_NAME_NVP("message", t.message());
}

} // namespace serialization
} // namespace boost

#endif // __YGGR_EXCEPTION_SERIALIZATION_ERROR_CODE_HPP__




