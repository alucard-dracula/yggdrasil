//anonymous_shared_memory.hpp

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

#ifndef __YGGR_SEGMENT_ANMONYMOUS_SHARED_MEMORY_HPP__
#define __YGGR_SEGMENT_ANMONYMOUS_SHARED_MEMORY_HPP__

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include
#include <yggr/move/move.hpp>

#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace yggr
{
namespace segment
{

class anonymous_shared_memory
    : public boost::interprocess::mapped_region
{
public:
	typedef boost::interprocess::mapped_region base_type;

private:
	typedef anonymous_shared_memory this_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(anonymous_shared_memory) // not using this_type cpp20 has issue


public:
	anonymous_shared_memory(void);
	anonymous_shared_memory(std::size_t size, void* address = 0);

	anonymous_shared_memory(BOOST_RV_REF(base_type) base)
		: base_type(boost::move(base))
	{
	}

	anonymous_shared_memory(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	~anonymous_shared_memory(void);

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::operator=(boost::move(right));

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right)));

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type &base);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	using base_type::get_size;
	using base_type::get_address;
	using base_type::get_mode;
	using base_type::flush;
	using base_type::shrink_by;
	using base_type::advise;
};

} // namespace segment
} // namespace yggr

namespace yggr
{
namespace segment
{
namespace swap_support
{

inline void swap(anonymous_shared_memory::base_type& l, anonymous_shared_memory& r)
{ 
	r.swap(l); 
} 

inline void swap(anonymous_shared_memory& l, anonymous_shared_memory::base_type& r) 
{ 
	l.swap(r); 
} 

inline void swap(anonymous_shared_memory& l, anonymous_shared_memory& r) 
{ 
	l.swap(r); 
}


} // namespace swap_support

using swap_support::swap;

} // namespace segment
} // namespace yggr

namespace std
{
	using ::yggr::segment::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::segment::swap_support::swap;
} // namespace boost

#endif // __YGGR_SEGMENT_ANMONYMOUS_SHARED_MEMORY_HPP__

