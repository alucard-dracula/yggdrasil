// typeids_def.hpp

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

#ifndef __YGGR_TYPE_AWARENESS_TYPEIDS_DEF_HPP__
#define __YGGR_TYPE_AWARENESS_TYPEIDS_DEF_HPP__

namespace yggr
{
namespace awareness
{

struct typeids
{
	enum
	{
		E_TYPE_IDS = 0,
		E_CHAR,
		E_S8,
		E_U8,
		E_S16,
		E_U16,
		E_S32,
		E_U32,
		E_S64,
		E_U64,
		E_F32,
		E_F64,
		E_STL_STRING,
		E_STL_WSTRING,
		E_BOOST_STRING,
		E_BOOST_WSTRING,
		E_UTF8_STRING,

		E_compile_u32 = 0xffffffff
	};
};

} // namespace awareness
} // namespace yggr

#endif // __YGGR_TYPE_AWARENESS_TYPEIDS_DEF_HPP__