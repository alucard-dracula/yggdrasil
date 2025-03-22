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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SAFE_INSERT_RETURN_TYPE_BASE_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SAFE_INSERT_RETURN_TYPE_BASE_HPP__

#include <yggr/container/detail/insert_return_type_base.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename NodeType>
struct safe_insert_return_type_base
{
public:
	typedef NodeType node_type;

private:
	typedef safe_insert_return_type_base this_type;

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
	BOOST_MOVABLE_BUT_NOT_COPYABLE(safe_insert_return_type_base) // not using this_type cpp20 has issue
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

public:
	safe_insert_return_type_base(void)
		: inserted(false), node()
	{
	}

	safe_insert_return_type_base(bool bins, BOOST_RV_REF(node_type) n)
		: inserted(false), node(boost::move(n))
	{
	}

	//template<typename RelatedIt, typename RelatedNode>
	//safe_insert_return_type_base(bool insert, BOOST_RV_REF(RelatedNode) node)
	//	: inserted(insert), node(boost::move(node))
	//{
	//}

	template<typename RelatedNode>
	safe_insert_return_type_base(bool insert, BOOST_RV_REF(RelatedNode) node)
		: inserted(insert), node(boost::move(node))
	{
	}

//	template<template<typename _I, typename _N> class InsRetTBase, typename I, typename N>
//	safe_insert_return_type_base(BOOST_RV_REF_BEG InsRetTBase<I, N> BOOST_RV_REF_END base)
//		: inserted(base.inserted), node(boost::move(base.node))
//	{
//	}

	template<typename InsRetTBase>
	safe_insert_return_type_base(BOOST_RV_REF(InsRetTBase) base)
		: inserted(base.inserted), node(boost::move(base.node))
	{
	}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
	template<typename InsRetTBase>
	safe_insert_return_type_base(const InsRetTBase& base)
		: inserted(base.inserted), node(boost::move(const_cast<InsRetTBase&>(base).node))
	{
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	safe_insert_return_type_base(BOOST_RV_REF(this_type) right)
		: inserted(right.inserted), node(boost::move(right.node))
	{
	}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
	safe_insert_return_type_base(const this_type& right)
		: inserted(right.inserted), node(boost::move(right.node))
	{
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	~safe_insert_return_type_base(void)
	{
	}

public:
	template<typename InsRetTBase> inline
	this_type& operator=(BOOST_RV_REF(InsRetTBase) base_right)
	{
		typedef InsRetTBase base_insert_return_type;

		base_insert_return_type& right_ref = base_right;
		inserted = right_ref.inserted;
		node = boost::move(right_ref.node);
		return *this;
	}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
	template<typename InsRetTBase> inline
	this_type& operator=(const InsRetTBase& base_right)
	{
		typedef InsRetTBase base_insert_return_type;

		base_insert_return_type& right_ref = const_cast<base_insert_return_type&>(base_right);
		inserted = right_ref.inserted;
		node = boost::move(right_ref.node);
		return *this;
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(boost::addressof(right_ref) == this)
		{
			return *this;
		}

		inserted = right_ref.inserted;
		node = boost::move(right_ref.node);
		return *this;
	}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
	inline this_type& operator=(const this_type& right)
	{
		if(boost::addressof(right) == this)
		{
			return *this;
		}

		inserted = right.inserted;
		node = boost::move(right.node);
		return *this;
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

public:
	bool inserted;
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
	NodeType node;
#else
	mutable NodeType node;
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SAFE_INSERT_RETURN_TYPE_BASE_HPP__
