//ref.hpp

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

#ifndef __YGGR_SUPPORT_REF_HPP__
#define __YGGR_SUPPORT_REF_HPP__

namespace yggr
{
namespace support
{

struct ref
{
	template<typename T>
	inline static const T& to_cref(const T& t)
	{
		return t;
	}

	template<typename T>
	inline static T& to_ref(const T& t)
	{
		return const_cast<T&>(t);
	}

	template<typename T>
	inline static const typename T::base_type& to_base_cref(const T& t)
	{
		typedef typename T::base_type base_type;
		const base_type& base = t;
		return base;
	}

	template<typename T>
	inline static typename T::base_type& to_base_ref(const T& t)
	{
		typedef typename T::base_type base_type;
		base_type& base = const_cast<T&>(t);
		return base;
	}
};

} // namespace support
} // namespace yggr

#endif // __YGGR_SUPPORT_REF_HPP__
