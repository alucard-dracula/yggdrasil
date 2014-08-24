//opids_def.hpp

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

#ifndef __YGGR_TYPE_AWARENESS_OPIDS_DEF_HPP__
#define __YGGR_TYPE_AWARENESS_OPIDS_DEF_HPP__

namespace yggr
{
namespace type_awareness
{

struct opids
{
	enum
	{
		E_OPIDS = 0,
		E_PLUS,
		E_MINUS,
		E_MUL,
		E_DIV,
		E_MOD,
		E_NEGATE,
		
		E_EQUAL_TO,
		E_NOT_EQUAL_TO,
		E_GREATER,
		LESS,
		GREATER_EQUAL,
		LESS_EQUAL,

		E_LOG_AND,
		E_LOG_OR,
		E_LOG_NOT,

		E_compile_u32 = 0xffffffff
	};
};

} // namespace type_awareness
} // namespace yggr

#endif // __YGGR_TYPE_AWARENESS_OPIDS_DEF_HPP__