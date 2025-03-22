//bind.hpp

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

#ifndef __YGGR_BIND_EX_BIND_HPP__
#define __YGGR_BIND_EX_BIND_HPP__

#include <yggr/bind/bind.hpp>

#define _YGGR_PP_TMP_BIND_COMPARE() \
	template<typename R, typename F, typename L> inline \
	bool operator==(const ::boost::_bi::bind_t<R, F, L>& left, \
						const ::boost::_bi::bind_t<R, F, L>& right) { \
		return left.compare(right); } \
	\
	template<typename R, typename F, typename L> inline \
	bool operator!=(const ::boost::_bi::bind_t<R, F, L>& left, \
						const ::boost::_bi::bind_t<R, F, L>& right) { \
		return !(left.compare(right)); } \
	\
	template<typename R1, typename F1, typename L1, \
			typename R2, typename F2, typename L2> inline  \
	bool operator==(const ::boost::_bi::bind_t<R1, F1, L1>& left, \
						const ::boost::_bi::bind_t<R2, F2, L2>& right) { \
		return false; } \
	\
	template<typename R1, typename F1, typename L1, \
				typename R2, typename F2, typename L2> inline \
	bool operator!=(const ::boost::_bi::bind_t<R1, F1, L1>& left, \
						const ::boost::_bi::bind_t<R2, F2, L2>& right) { \
		return true; }

namespace boost
{
namespace _bi
{
	_YGGR_PP_TMP_BIND_COMPARE()

} // namespace _bi
} // namespace boost

namespace yggr
{
namespace bind_ex
{
} // namespace bind_ex
} // namespace yggr

#undef _YGGR_PP_TMP_BIND_COMPARE

#endif // __YGGR_BIND_EX_BIND_HPP__