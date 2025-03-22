//allocator_destroy_holder_unordered.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_ALLOCATOR_DESTROY_HOLDER_UNORDERED_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_ALLOCATOR_DESTROY_HOLDER_UNORDERED_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_ALLOCATOR_DESTROY_HOLDER_UNORDERED_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_ALLOCATOR_DESTROY_HOLDER_UNORDERED_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/array.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <boost/unordered/detail/allocate.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

#include <cassert>
#include <algorithm>
#include <iterator>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Allocator, std::size_t MS>
class unordered_allocator_destroy_holder
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Allocator allocator_type;
	typedef boost::unordered::detail::allocator_traits<allocator_type> alloc_traits_type;
	typedef typename alloc_traits_type::value_type value_type;
	typedef typename alloc_traits_type::pointer pointer;

private:
	typedef container::array<pointer, MS> pointer_array_type;
	typedef container::vector<pointer> pointer_vt_type;

private:
	typedef unordered_allocator_destroy_holder this_type;

public:
	unordered_allocator_destroy_holder(void)
		: _a(), _arr_size(0)
	{
		assert(_ptr_vt.capacity() == 0);
	}

	template<typename VAlloc> explicit
	unordered_allocator_destroy_holder(const VAlloc& valloc,
										typename
											boost::disable_if
											<
												boost::is_arithmetic<VAlloc>,
												mplex::sfinae_type
											>::type sfinae = 0)
		: _a(valloc), _arr_size(0)
	{
		assert(_ptr_vt.capacity() == 0);
	}

	unordered_allocator_destroy_holder(std::size_t size)
		: _a(), _arr_size(0)
	{
		this_type::reserve(size);
	}

	template<typename VAlloc>
	unordered_allocator_destroy_holder(std::size_t size, const VAlloc& valloc)
		: _a(valloc), _arr_size(0)
	{
		this_type::reserve(size);
	}

	~unordered_allocator_destroy_holder(void)
	{
		this_type::destroy();
		assert(!_arr_size);
		assert(_ptr_vt.empty());
	}

public:
	inline void operator()(const pointer& p)
	{
		if(_arr_size < _ptr_arr.size())
		{
			_ptr_arr[_arr_size++] = p;
		}
		else
		{
			_ptr_vt.push_back(p);
		}
	}

	template<typename InputIter> inline
	void operator()(InputIter s, InputIter e)
	{
		std::size_t input_size = std::distance(s, e);
		if(!input_size)
		{
			return;
		}
		std::size_t arr_empty_size = _ptr_arr.max_size() - _arr_size;
		
		InputIter s_end = s;
		if(arr_empty_size)
		{
			std::size_t input_less_size = 
				input_size > arr_empty_size? 
				input_size - arr_empty_size : 0; 
			std::size_t copy_size = input_size - input_less_size;
			std::advance(s_end, copy_size);
			std::copy(s, s_end, _ptr_arr.begin() + _arr_size);
			_arr_size += copy_size;
		}

		std::copy(s_end, e, std::back_inserter(_ptr_vt));
	}

	template<typename T> inline
	bool move_value(std::size_t idx, T& out)
	{
		std::size_t size = _arr_size + _ptr_vt.size();

		if(!(idx < size))
		{
			return false;
		}

		if(idx < _arr_size)
		{
			assert(_ptr_arr[idx]);
			copy_or_move_or_swap(out, boost::move(_ptr_arr[idx]->value()));
		}
		else
		{
			idx -= _arr_size;
			assert(_ptr_vt[idx]);
			copy_or_move_or_swap(out, boost::move(_ptr_vt[idx]->value()));
		}

		return true;
	}

	template<typename T> inline
	bool get_value(std::size_t idx, T& out)
	{
		std::size_t size = _arr_size + _ptr_vt.size();

		if(!(idx < size))
		{
			return false;
		}

		if(idx < _arr_size)
		{
			assert(_ptr_arr[idx]);
			out = _ptr_arr[idx]->value();
		}
		else
		{
			idx -= _arr_size;
			assert(_ptr_vt[idx]);
			out = _ptr_vt[idx]->value();
		}

		return true;
	}

	template<typename NodePtr> inline
	NodePtr get_pointer(std::size_t idx)
	{
		typedef NodePtr node_ptr_type;

		std::size_t size = _arr_size + _ptr_vt.size();

		if(!(idx < size))
		{
			return node_ptr_type();
		}

		if(idx < _arr_size)
		{
			return node_ptr_type(_ptr_arr[idx]);
		}

		idx -= _arr_size;
		return node_ptr_type(_ptr_vt[idx]);
	}


	inline void clear(void)
	{
		this_type::destroy();
	}

	inline void non_free_clear(void)
	{
		_arr_size = 0;
		_ptr_vt.clear();
	}

	inline void reserve(std::size_t size)
	{
		std::size_t space_size = MS - _arr_size;
		if(size <= space_size)
		{
			return;
		}

		_ptr_vt.reserve(size - space_size + _ptr_vt.size());
	}

	inline std::size_t size(void) const
	{
		return _arr_size + _ptr_vt.size();
	}

	inline bool empty(void) const
	{
		return this_type::size() == 0;
	}

	inline static std::size_t static_max_size(void)
	{
		return MS;
	}

	inline allocator_type& get_allocator(void)
	{
		return _a;
	}

	inline const allocator_type& get_allocator(void) const
	{
		return _a;
	}

	template<typename Container> inline
	void copy_to(Container& cont) const
	{
		if(_arr_size)
		{
			std::copy(_ptr_arr.begin(),
						_ptr_arr.begin() + _arr_size,
						std::back_inserter(cont));
		}

		if(!_ptr_vt.empty())
		{
			std::copy(_ptr_vt.begin(),
						_ptr_vt.begin() + _arr_size,
						std::back_inserter(cont));
		}
	}

private:
	void destroy(void)
	{
		//pointer ptr = 0;
		{
			for(std::size_t i = 0, isize = _arr_size; i != isize; ++i)
			{
				destroy_one(_ptr_arr[i]);
			}
		}

		{
			for(typename pointer_vt_type::iterator i = _ptr_vt.begin(), isize = _ptr_vt.end();
				 i != isize; ++i)
			{
				destroy_one(*i);
			}
		}

		_arr_size = 0;
		_ptr_vt.clear();
	}

	inline void destroy_one(pointer ptr)
	{
		assert(ptr);
		boost::unordered::detail::func::call_destroy(_a, ptr->value_ptr()); // 1.63 start
		boost::unordered::detail::func::destroy(boost::addressof(*ptr));
		alloc_traits_type::deallocate(_a, ptr, 1);
	}

private:
	allocator_type _a;
	pointer_array_type _ptr_arr;
	std::size_t _arr_size;
	pointer_vt_type _ptr_vt;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION


#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_ALLOCATOR_DESTROY_HOLDER_UNORDERED_HPP__
