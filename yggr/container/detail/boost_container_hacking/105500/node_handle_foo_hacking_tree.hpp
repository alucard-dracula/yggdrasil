// node_handle_foo_hacking_tree.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105500_NODE_HANDLE_FOO_HACKING_TREE_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105500_NODE_HANDLE_FOO_HACKING_TREE_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_TREE_HPP__
#	error "this file is inner file, can't include it!"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_TREE_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/container/detail/tree.hpp>

#include <yggr/move/detail/move_return.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_node_alloc_holder.hpp>
#include <yggr/container/detail/node_handle.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Tree>
class node_handle_foo_hacking_tree;

template<typename K, typename V, typename KOV, typename KC, typename A,
			template<typename _K, typename _V, typename _KOV, typename _KC, typename _A> class Tree>
class node_handle_foo_hacking_tree< Tree<K, V, KOV, KC, A> >
	: public
		node_handle_foo_hacking_node_alloc_holder
		<
			boost::container::container_detail::node_alloc_holder
			< 
				A, 
				typename 
					boost::container::container_detail::intrusive_rbtree_type
					<
						A, 
						boost::container::container_detail::tree_value_compare
						<
							K, 
							V, 
							KC, 
							KOV
						> 
					>::type,
				KC
			>
		>
{
private:
	typedef node_handle_foo_hacking_tree this_type;

public:
	typedef Tree<K, V, KOV, KC, A> org_tree_type;
	typedef node_handle_foo_hacking_tree hacking_tree_type;

public:
	// org
	typedef typename org_tree_type::key_type key_type;
	typedef typename org_tree_type::value_type value_type;

	typedef typename org_tree_type::iterator iterator;
	typedef typename org_tree_type::const_iterator const_iterator;

public:
	// hacking
	typedef 
		boost::container::container_detail::node_alloc_holder
		< 
			A, 
			typename 
				boost::container::container_detail::intrusive_rbtree_type
				<
					A, 
					boost::container::container_detail::tree_value_compare
					<
						K, 
						V, 
						KC, 
						KOV
					> 
				>::type,
			KC
		> org_node_alloc_holder_type;

	typedef node_handle_foo_hacking_node_alloc_holder<org_node_alloc_holder_type> hacking_node_alloc_holder_type;
	typedef hacking_node_alloc_holder_type base_type;

	typedef typename hacking_node_alloc_holder_type::ICont Icont;
	typedef typename Icont::iterator iiterator;
	typedef typename Icont::insert_commit_data insert_commit_data;
	typedef typename hacking_node_alloc_holder_type::NodeAlloc NodeAlloc;
	typedef typename hacking_node_alloc_holder_type::NodePtr NodePtr;

protected:
	typedef typename Icont::node_traits node_traits;

public:
	typedef node_handle<NodeAlloc, void> node_type;
	typedef insert_return_type_base<iterator, node_type> insert_return_type;

public:
	inline org_tree_type& org_cast(void) YGGR_NOEXCEPT
	{
		return reinterpret_cast<org_tree_type&>(*this);
	}

	inline const org_tree_type& org_cast(void) const YGGR_NOEXCEPT
	{
		return reinterpret_cast<const org_tree_type&>(*this);
	}

protected:
	template<typename TreeIter, typename Nil = void>
	struct tree_iterator_traits;

	template<
		template<typename _T1, bool _B> class TreeIterTpl,
		typename ValueTraits, bool IsConst, typename Nil>
	struct tree_iterator_traits<TreeIterTpl<ValueTraits, IsConst>, Nil>
		: public TreeIterTpl<ValueTraits, IsConst>
	{
	public:
		typedef TreeIterTpl<ValueTraits, IsConst> base_type;
		
	public:
		typedef typename base_type::node_traits node_traits;
	};

private:
	inline static bool inited(const typename node_traits::const_node_ptr& node)
	{
		return 
			!node_traits::get_parent(node) 
			&& !node_traits::get_left(node)   
			&& !node_traits::get_right(node);
	};

	inline static bool is_header(const typename node_traits::const_node_ptr& p)
	{
		typename node_traits::node_ptr p_left (node_traits::get_left(p));
		typename node_traits::node_ptr p_right(node_traits::get_right(p));

		return 
			(!node_traits::get_parent(p) //Header condition when empty tree
				|| (p_left && p_right //Header always has leftmost and rightmost
				&& (p_left == p_right //Header condition when only node
					|| (node_traits::get_parent(p_left) != p 
						|| node_traits::get_parent(p_right) != p ))
				//When tree size > 1 headers can't be leftmost's
				//and rightmost's parent
				));
	}

	static typename node_traits::node_ptr 
		get_root(const typename node_traits::node_ptr& node)
	{
		BOOST_INTRUSIVE_INVARIANT_ASSERT((!this_type::inited(node)));
		typename node_traits::node_ptr x = node_traits::get_parent(node);
		if(x)
		{
			while(!this_type::is_header(x))
			{
				x = node_traits::get_parent(x);
			}
			return x;
		}
		else
		{
			return node;
		}
	}

protected:
	inline bool priv_is_linked(const_iterator const position) const
	{
		typedef tree_iterator_traits<iiterator, void> tree_iterator_traits_type;
		typedef typename tree_iterator_traits_type::node_traits node_traits_type;
		typedef typename node_traits_type::ptr_bit ptr_bit_type;

		iiterator const cur(hacking_iiterator_get(position));

		return   
			cur == base_type::icont().end() 
			|| node_traits_type::get_parent(cur.pointed_node()) ==
				this_type::get_root(base_type::icont().begin().pointed_node())
			|| (cur.pointed_node() == 
					node_traits_type::get_left(node_traits_type::get_parent(cur.pointed_node())))
			|| (cur.pointed_node() == 
					node_traits_type::get_right(node_traits_type::get_parent(cur.pointed_node())));
	}


protected:
	class hacking_const_iterator
	{
	public:
		hacking_const_iterator(iiterator iiter)
			: m_it(iiter)
		{
		}

	public:
		inline const const_iterator& to_org(void) const YGGR_NOEXCEPT
		{
			return reinterpret_cast<const const_iterator&>(*this);
		}

	public:
		iiterator m_it;
	};

	class hacking_iterator
		: public hacking_const_iterator
	{
	public:
		hacking_iterator(iiterator iiter)
			: hacking_const_iterator(iiter)
		{
		}

	public:
		inline const iterator& to_org(void) const YGGR_NOEXCEPT
		{
			return reinterpret_cast<const iterator&>(*this);
		}
	};

	inline static iiterator hacking_iiterator_get(iterator iter) YGGR_NOEXCEPT
	{
		return reinterpret_cast<hacking_iterator&>(iter).m_it;
	}

	inline static iiterator hacking_iiterator_get(const_iterator iter) YGGR_NOEXCEPT
	{
		return reinterpret_cast<hacking_const_iterator&>(iter).m_it;
	}

public:
	template<typename Key>inline
	node_type extract(BOOST_RV_REF(Key) k)
	{
		iterator const it = org_cast().find(boost::forward<Key>(k));
		return (org_cast().end() != it)? this_type::extract(it) : node_type();
	}

	inline node_type extract(const key_type& k)
	{
		iterator const it = org_cast().find(k);
		return (org_cast().end() != it)? this_type::extract(it) : node_type();
	}

	inline node_type extract(iterator position)
	{
		const_iterator iter = position;
		return this_type::extract(iter);
	}

	inline node_type extract(const_iterator position)
	{
		BOOST_ASSERT(((position != org_cast().cend()) && (this_type::priv_is_linked(position))));
		iiterator const iit(this_type::hacking_iiterator_get(position));
		base_type::icont().erase(iit);
		return node_type(iit.operator->(), base_type::node_alloc());
	}

public:
	inline insert_return_type insert_unique_node(BOOST_RV_REF(node_type) nh)
	{
		return this_type::insert_unique_node(org_cast().end(), boost::move(nh));
	}

	inline insert_return_type insert_unique_node(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		insert_return_type irt; //inserted == false, node.empty()
		if(!nh.empty())
		{
			insert_commit_data data;
			std::pair<iterator, bool> ret =
				org_cast().insert_unique_check(hint, KOV()(nh.value()), data);
			if(ret.second)
			{
				irt.inserted = true;
				hacking_iterator hacked_iter = base_type::icont().insert_unique_commit(*nh.get(), data);
				irt.position = hacked_iter.to_org();
				nh.release();
			}
			else
			{
				irt.position = ret.first;
				irt.node = boost::move(nh);
			}
		}
		else
		{
			irt.position = org_cast().end();
		}

		return YGGR_MOVE_RET(insert_return_type, irt);
	}

	inline iterator insert_equal_node(BOOST_RV_REF(node_type) nh)
	{
		if(nh.empty())
		{
			return org_cast().end();
		}
		else
		{
			NodePtr const p(nh.release());
			hacking_iterator hacked_iter = base_type::icont().insert_equal(*p);
			return hacked_iter.to_org();
		}
	}

	inline iterator insert_equal_node(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		if(nh.empty())
		{
			return org_cast().end();
		}
		else
		{
			NodePtr const p(nh.release());
			hacking_iterator hacked_iter = 
				base_type::icont().insert_equal(hacking_iiterator_get(hint), *p);
			return hacked_iter.to_org();
		}
	}

};



} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105500_NODE_HANDLE_FOO_HACKING_TREE_HPP__
