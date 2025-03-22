//size_container.hpp

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

#ifndef __YGGR_NETWOR_SIZE_CONTAINER_HPP__
#define __YGGR_NETWOR_SIZE_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/network/hn_conv.hpp>

#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_unsigned.hpp>

namespace yggr
{
namespace network
{

template<typename Size>
class size_container
{
	BOOST_MPL_ASSERT((boost::is_integral<Size>));
	BOOST_MPL_ASSERT((boost::is_unsigned<Size>));

public:
	typedef Size size_type;
	YGGR_STATIC_CONSTANT(u32, E_length = sizeof(size_type) * 3);

private:
	typedef size_container this_type;

public:
	size_container(void)
		: _size_t_size(sizeof(yggr::ser_size_type)),
			_info_size(size_type()),
			_data_size(size_type())
	{
	}

	size_container(const size_type& info_size, 
					const size_type& data_size)
		: _size_t_size(sizeof(yggr::ser_size_type)),
			_info_size(info_size),
			_data_size(data_size)
	{
	}

	template<typename Buffer>
	size_container(const Buffer& buf)
		: _size_t_size(size_type()),
			_info_size(size_type()),
			_data_size(size_type())
	{
		assert(buf.size() >= this_type::E_length);
		memcpy(&_buf[0], &buf[0], E_length);
	}

	template<typename InputIter>
	size_container(InputIter s, InputIter e)
	{
		assert((e - s >= E_length));
		memcpy(&_buf[0], &(*s), E_length);
	}

	size_container(const this_type& right)
		: _size_t_size(right._size_t_size),
			_info_size(right._info_size),
			_data_size(right._data_size)
	{
	}

private:
	size_container(const size_type& size_t_size,
					const size_type& info_size, 
					const size_type& data_size)
		: _size_t_size(size_t_size),
			_info_size(info_size),
			_data_size(data_size)
	{
	}

public:
	~size_container(void)
	{
	}

public:
	template<typename Buffer>
	this_type& operator=(const Buffer& buf)
	{
		assert(buf.size() >= this_type::E_length);
		memcpy(&_buf[0], &buf[0], E_length);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		// _size_t_size = right._size_t_size; // this is const var, don't copy it
		_info_size = right._info_size;
		_data_size = right._data_size;
		return *this;
	}
	
public:
	inline bool effective(void) const
	{
		return sizeof(yggr::ser_size_type) == _size_t_size;
	}

	inline size_type& info_size(void)
	{
		return _info_size;
	}

	inline size_type info_size(void) const
	{
		return _info_size;
	}

	inline size_type& data_size(void)
	{
		return _data_size;
	}

	inline size_type data_size(void) const
	{
		return _data_size;
	}

	inline size_type count_size(void) const
	{
		return _info_size + _data_size;
	}

	inline static size_type size(void)
	{
		return this_type::E_length;
	}

	inline static size_type size_size(void)
	{
		return sizeof(size_type);
	}

	inline u8* buf(void)
	{
		return _buf;
	}

	inline const u8* buf(void) const
	{
		return _buf;
	}
	
	inline this_type hton(void) const
	{
		return 
			this_type( 
				yggr::network::hton(_size_t_size),
				yggr::network::hton(_info_size),
				yggr::network::hton(_data_size));
	}

	inline this_type ntoh(void) const
	{
		return 
			this_type( 
				yggr::network::ntoh(_size_t_size),
				yggr::network::ntoh(_info_size), 
				yggr::network::ntoh(_data_size));
	}


private:
	union
	{
		struct
		{
			size_type _size_t_size;
			size_type _info_size;
			size_type _data_size;
		};

		u8 _buf[E_length];
	};
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWOR_SIZE_CONTAINER_HPP__
