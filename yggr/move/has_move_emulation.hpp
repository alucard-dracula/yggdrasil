// has_move_emulation.hpp

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

#ifndef __YGGR_MOVE_HAS_MOVE_EMULATION_HPP__
#define __YGGR_MOVE_HAS_MOVE_EMULATION_HPP__

#include <yggr/move/detail/has_move_emulation_enabled_impl.hpp>

namespace yggr
{
namespace move
{

using detail::has_move_emulation_enabled;
using detail::has_move_emulation_disabled;

} // namespace move

using move::detail::has_move_emulation_enabled;
using move::detail::has_move_emulation_disabled;

} // namespace yggr

#endif // __YGGR_MOVE_HAS_MOVE_EMULATION_HPP__
