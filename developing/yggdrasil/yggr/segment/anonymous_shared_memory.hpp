//anonymous_shared_memory.hpp

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

#ifndef __YGGR_SEGMENT_ANMONYMOUS_SHARED_MEMORY_HPP__
#define __YGGR_SEGMENT_ANMONYMOUS_SHARED_MEMORY_HPP__

#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <yggr/move/move.hpp>

namespace yggr
{
namespace segment
{

class anonymous_shared_memory
    : public boost::interprocess::mapped_region
{
private:
	typedef boost::interprocess::mapped_region base_type;

private:
	typedef anonymous_shared_memory this_type;

	BOOST_MOVABLE_BUT_NOT_COPYABLE(anonymous_shared_memory)

public:

	anonymous_shared_memory(void)
	{
	}

	anonymous_shared_memory(std::size_t size, void *address = 0)
		: base_type(boost::interprocess::anonymous_shared_memory(size, address))
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	anonymous_shared_memory(BOOST_RV_REF(base_type) base)
		: base_type(boost::forward<base_type>(base))
	{
	}

	anonymous_shared_memory(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}

#else
    anonymous_shared_memory(BOOST_RV_REF(base_type) base)
	{
	    base_type& base_ref = base;
	    base_type::swap(base_ref);
	}

	anonymous_shared_memory(BOOST_RV_REF(this_type) right)
	{
	    this_type& right_ref = right;
	    base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	~anonymous_shared_memory(void)
	{
	}

	this_type &operator=(BOOST_RV_REF(base_type) base)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(base));
#else
        base_type& base_ref = base;
        base_type::swap(base_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type &operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
        this_type& right_ref = right;
        base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	void swap(base_type &base)
	{
		base_type::swap(base);
	}

	void swap(this_type& right)
	{
		base_type::swap(right);
	}
};

} // namespace segment
} // namespace yggr

#endif // __YGGR_SEGMENT_ANMONYMOUS_SHARED_MEMORY_HPP__
