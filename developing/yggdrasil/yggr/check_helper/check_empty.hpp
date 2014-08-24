//check_empty.hpp

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

#ifndef __YGGR_CHECK_HELPER_CHECK_EMPTY_HPP__
#define __YGGR_CHECK_HELPER_CHECK_EMPTY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/charset/string.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace check_helper
{

template<typename T>
class check_empty : private nonable::noncopyable
{
public:
	typedef T value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val.empty();
	}

private:
	const value_type& _val;
};

template<>
class check_empty<s8>
	: private nonable::noncopyable
{
public:
	typedef s8 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<u8>
	: private nonable::noncopyable
{
public:
	typedef u8 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<s16>
	: private nonable::noncopyable
{
public:
	typedef s16 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<u16>
	: private nonable::noncopyable
{
public:
	typedef u16 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<s32>
	: private nonable::noncopyable
{
public:
	typedef s32 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<u32>
	: private nonable::noncopyable
{
public:
	typedef u32 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<s64>
	: private nonable::noncopyable
{
public:
	typedef s64 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<u64>
	: private nonable::noncopyable
{
public:
	typedef u64 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<f32>
	: private nonable::noncopyable
{
public:
	typedef f32 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

template<>
class check_empty<f64>
	: private nonable::noncopyable
{
public:
	typedef f64 value_type;

private:
	typedef check_empty this_type;
public:

	check_empty(const value_type& val)
		: _val(val)
	{
	}

	~check_empty(void)
	{
	}

	inline operator bool(void) const
	{
		return _val;
	}

private:
	const value_type& _val;
};

} // namespace check_helper
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif // __YGGR_CHECK_HELPER_CHECK_EMPTY_HPP__