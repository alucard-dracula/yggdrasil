// insert_return_type_base.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_INSERT_RETURN_TYPE_BASE_HPP__
#define __YGGR_CONTAINER_DETAIL_INSERT_RETURN_TYPE_BASE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Iterator, typename NodeType>
struct insert_return_type_base
{
private:
	typedef insert_return_type_base this_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(insert_return_type_base) // not using this_type cpp20 has issue

public:
	insert_return_type_base(void)
		: inserted(false), position(), node()
	{
	}

	template<typename RelatedIt, typename RelatedNode>
	insert_return_type_base(bool insert, RelatedIt it, BOOST_RV_REF(RelatedNode) node)
		: inserted(insert), position(it), node(boost::move(node))
	{
	}

	// support boost::multi_index::insert_return_type
	template<typename RelatedIt, typename RelatedNode>
	insert_return_type_base(RelatedIt it, bool insert, BOOST_RV_REF(RelatedNode) node)
		: inserted(insert), position(it), node(boost::move(node))
	{
	}

	insert_return_type_base(BOOST_RV_REF(this_type) right)
		: inserted(right.inserted), position(right.position), node(boost::move(right.node))
	{
	}

	~insert_return_type_base(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(boost::addressof(right_ref) == this)
		{
			return *this;
		}

		inserted = right_ref.inserted;
		position = right_ref.position;
		node = boost::move(right_ref.node);
		return *this;
	}

public:
	bool inserted;
	Iterator position;
	NodeType node;
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_INSERT_RETURN_TYPE_BASE_HPP__
