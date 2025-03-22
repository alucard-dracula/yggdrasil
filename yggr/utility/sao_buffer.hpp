// sao_buffer.hpp

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

#ifndef __YGGR_UTILITY_SAO_BUFFER_HPP__
#define __YGGR_UTILITY_SAO_BUFFER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/container/array.hpp>
#include <yggr/bytes/byte_size.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/ref.hpp>

#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_const.hpp>

#include <memory>

namespace yggr
{
namespace utility
{

template<typename T, std::size_t MinBufSize = 1024>
struct sao_buffer
	: private nonable::noncopyable, 
		private nonable::nonmoveable 
{
public:
	typedef boost::mpl::size_t<MinBufSize> min_buffer_size_type;
	YGGR_STATIC_CONSTANT(u32, E_MIN_BUFFER_SIZE = min_buffer_size_type::value);

public:
	typedef T value_type;
	typedef typename boost::add_reference<value_type>::type reference;
	typedef typename boost::add_const<reference> const_reference;

	typedef typename boost::add_pointer<value_type>::type pointer;
	typedef typename boost::add_const<pointer>::type const_pointer;

public:
	typedef pointer iterator;
	typedef const_pointer const_iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

public:
	typedef typename std::iterator_traits<iterator>::difference_type difference_type;
	typedef difference_type size_type;


private:
	typedef container::array<T, min_buffer_size_type::value> min_buffer_type;

private:
	typedef sao_buffer this_type;

public:
	sao_buffer(std::size_t req_size)
		: _req_size(req_size), _pbuf(0)
	{
		assert(req_size);
		this_type::prv_init();
	}

	template<typename Val>
	sao_buffer(std::size_t req_size, const Val& val)
		: _req_size(req_size), _pbuf(0)
	{
		assert(req_size);
		this_type::prv_init();
		std::fill(_pbuf, advance_copy(_pbuf, _req_size), value_type(val));
	}

	~sao_buffer(void)
	{
		if(_pbuf != boost::addressof(_min_buf[0]))
		{
			delete []_pbuf;
		}
	}

public:
	// begin
	inline iterator begin(void)
	{
		return _pbuf;
	}

	inline const_iterator begin(void) const
	{
		return _pbuf;
	}

	inline const_iterator cbegin(void) const
	{
		return _pbuf;
	}

	// end
	inline iterator end(void)
	{
		return advance_copy(_pbuf, _req_size);
	}

	inline const_iterator end(void) const
	{
		return advance_copy(_pbuf, _req_size);
	}

	inline const_iterator cend(void) const
	{
		return advance_copy(_pbuf, _req_size);
	}

	// rbegin
	inline reverse_iterator rbegin(void)
	{
		return reverse_iterator(this_type::end());
	}

	inline const const_reverse_iterator rbegin(void) const
	{
		return const_reverse_iterator(this_type::cend());
	}

	inline const const_reverse_iterator crbegin(void) const
	{
		return const_reverse_iterator(this_type::cend());
	}

	// rend
	inline reverse_iterator rend(void)
	{
		return reverse_iterator(this_type::begin());
	}

	inline const const_reverse_iterator rend(void) const
	{
		return const_reverse_iterator(this_type::cbegin());
	}

	inline const const_reverse_iterator crend(void) const
	{
		return const_reverse_iterator(this_type::cbegin());
	}
	
public:
	inline pointer buffer_pointer(void)
	{
		return _pbuf;
	}

	inline const_pointer buffer_pointer(void) const
	{
		return _pbuf;
	}

	inline size_type buffer_size(void) const
	{
		return _req_size;
	}

	inline size_type buffer_byte_size(void) const
	{
		return bytes::byte_size(_pbuf, advance_copy(_pbuf, _req_size));
	}

public:
	inline bool is_optimized(void) const
	{
		return _pbuf == boost::addressof(_min_buf[0]);
	}

public:
	inline static size_type min_buffer_size(void)
	{
		return min_buffer_size_type::value;
	}

private:
	inline void prv_init(void)
	{
		if(_pbuf)
		{
			return;
		}

		assert(_req_size);
		_req_size = _req_size? _req_size : 1;
		_pbuf = 
			_req_size <= min_buffer_size_type::value?
				boost::addressof(_min_buf[0])
				: yggr_nothrow_new T[_req_size];
	}

private:
	min_buffer_type _min_buf;
	size_type _req_size;
	pointer _pbuf;
};

} // namespace utility

using utility::sao_buffer;

} // namespace yggr

#endif // __YGGR_UTILITY_SAO_BUFFER_HPP__
