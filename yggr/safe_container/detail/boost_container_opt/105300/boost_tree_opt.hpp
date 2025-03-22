// boost_tree_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105300_BOOST_TREE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105300_BOOST_TREE_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_TREE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_TREE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/safe_container/detail/allocator_destroy_holder.hpp>
#include <yggr/safe_container/detail/boost_container_create_node.hpp>

#include <boost/container/detail/tree.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Key, typename Value, typename KeyOfValue,
			typename KeyCompare, typename A>
class boost_tree_opt;

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename K, typename V, typename KFV,
			typename C, typename A>
struct cast_to_tree_opt<boost::container::container_detail::rbtree<K, V, KFV, C, A> >
{
	typedef boost_tree_opt<K, V, KFV, C, A> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

template <typename Key, typename Value, typename KeyOfValue,
			typename KeyCompare, typename A>
class boost_tree_opt
	: protected
		boost::container::container_detail::node_alloc_holder
		<
			A,
			typename
				boost::container::container_detail::intrusive_rbtree_type
				<
					A,
					boost
					::container
					::container_detail
					::tree_value_compare<Key, Value, KeyCompare, KeyOfValue>
				>::type,
			KeyCompare
		>
{
	typedef typename
		boost::container::container_detail::intrusive_rbtree_type
		<
			A,
			boost
			::container
			::container_detail
			::tree_value_compare<Key, Value, KeyCompare, KeyOfValue>
		>::type																	Icont;

	typedef
		boost::container::container_detail::node_alloc_holder
		<
			A, Icont, KeyCompare
		>																		AllocHolder;

	typedef typename AllocHolder::NodePtr					                    NodePtr;

	typedef boost_tree_opt														ThisType;
	typedef typename AllocHolder::NodeAlloc										NodeAlloc;
	typedef typename AllocHolder::ValAlloc										ValAlloc;
	typedef typename AllocHolder::Node											Node;
	typedef typename Icont::iterator											iiterator;
	typedef typename Icont::const_iterator										iconst_iterator;
	typedef boost::container::container_detail::allocator_destroyer<NodeAlloc>	Destroyer;
	typedef typename AllocHolder::allocator_v1									allocator_v1;
	typedef typename AllocHolder::allocator_v2									allocator_v2;
	typedef typename AllocHolder::alloc_version									alloc_version;

	// opt_support
	typedef typename AllocHolder::AllocVersionTraits alloc_version_traits_type;
	typedef typename AllocHolder::Deallocator deallocator_type;

	class RecyclingCloner;
	friend class RecyclingCloner;

	class RecyclingCloner
	{
	public:
		RecyclingCloner(AllocHolder &holder, Icont &irbtree)
			:  m_holder(holder), m_icont(irbtree)
		{}

		inline NodePtr operator()(const Node &other) const
		{
			if(NodePtr p = m_icont.unlink_leftmost_without_rebalance()){
			//First recycle a node (this can't throw)
			BOOST_TRY{
				//This can throw
				p->do_assign(other.m_data);
				return p;
			}
			BOOST_CATCH(...){
				//If there is an exception destroy the whole source
				m_holder.destroy_node(p);
				while((p = m_icont.unlink_leftmost_without_rebalance())){
					m_holder.destroy_node(p);
				}
				BOOST_RETHROW
			}
			BOOST_CATCH_END
			}
			else{
			return m_holder.create_node(other.m_data);
			}
		}

		AllocHolder &m_holder;
		Icont &m_icont;
	};

	class RecyclingMoveCloner;
	friend class RecyclingMoveCloner;

	class RecyclingMoveCloner
	{
	public:
		RecyclingMoveCloner(AllocHolder &holder, Icont &irbtree)
			:  m_holder(holder), m_icont(irbtree)
		{}

		inline NodePtr operator()(const Node &other) const
		{
			if(NodePtr p = m_icont.unlink_leftmost_without_rebalance()){
			//First recycle a node (this can't throw)
			BOOST_TRY{
				//This can throw
				p->do_move_assign(const_cast<Node &>(other).m_data);
				return p;
			}
			BOOST_CATCH(...){
				//If there is an exception destroy the whole source
				m_holder.destroy_node(p);
				while((p = m_icont.unlink_leftmost_without_rebalance())){
					m_holder.destroy_node(p);
				}
				BOOST_RETHROW
			}
			BOOST_CATCH_END
			}
			else{
			return m_holder.create_node(other.m_data);
			}
		}

		AllocHolder &m_holder;
		Icont &m_icont;
	};

public:
	typedef
		boost::container::container_detail::rbtree
		<
			Key, Value, KeyOfValue, KeyCompare, A
		> org_type;

	typedef typename org_type::key_type key_type;
	typedef typename org_type::value_type value_type;
	typedef typename org_type::allocator_type allocator_type;
	typedef typename org_type::key_compare key_compare;
	typedef typename org_type::value_compare value_compare;
	typedef typename org_type::pointer pointer;
	typedef typename org_type::const_pointer const_pointer;
	typedef typename org_type::reference reference;
	typedef typename org_type::const_reference const_reference;
	typedef typename org_type::size_type size_type;
	typedef typename org_type::difference_type difference_type;
	typedef typename org_type::rbtree_difference_type rbtree_difference_type;
	typedef typename org_type::rbtree_pointer rbtree_pointer;
	typedef typename org_type::rbtree_const_pointer rbtree_const_pointer;
	typedef typename org_type::rbtree_reference rbtree_reference;
	typedef typename org_type::rbtree_const_reference rbtree_const_reference;
	typedef typename org_type::stored_allocator_type stored_allocator_type;

public:
	// opt_support
	typedef NodePtr node_ptr_type;
	typedef allocator_destroy_holder<NodeAlloc, 1> destroy_holder_type;
	typedef allocator_destroy_holder<NodeAlloc, 64> multi_destroy_holder_type;

private:

	template<typename KeyValueCompare>
	struct key_node_compare
		:  private KeyValueCompare
	{
		key_node_compare(const KeyValueCompare &comp)
			:  KeyValueCompare(comp)
		{
		}

		template<typename T>
		struct is_node
		{
			static const bool value = boost::is_same<T, Node>::value;
		};

		template<typename T> inline
		typename boost::enable_if_c<is_node<T>::value, const value_type &>::type
			key_forward(const T &node) const
		{
			return node.get_data();
		}

		template<typename T> inline
		typename boost::enable_if_c<!is_node<T>::value, const T &>::type
			key_forward(const T &key) const
		{
			return key;
		}

		template<typename KeyType, typename KeyType2> inline
		bool operator()(const KeyType &key1, const KeyType2 &key2) const
		{
			return KeyValueCompare::operator()(this->key_forward(key1), this->key_forward(key2));
		}
	};

	typedef key_node_compare<value_compare>  KeyNodeCompare;

public:
	typedef typename org_type::const_iterator const_iterator;
	typedef typename org_type::iterator iterator;
	typedef typename org_type::reverse_iterator reverse_iterator;
	typedef typename org_type::const_reverse_iterator const_reverse_iterator;

private:
	class const_iterator_opt
		: public
			std::iterator
			<
				std::bidirectional_iterator_tag,
				value_type,
				rbtree_difference_type,
				rbtree_const_pointer,
				rbtree_const_reference
			>
	{
	protected:
		typedef typename Icont::iterator  iiterator;
		iiterator m_it;

		explicit const_iterator_opt(iiterator it)
			: m_it(it)
		{
		}

		inline void prot_incr(void)
		{
			++m_it;
		}

		inline void prot_decr(void)
		{
			--m_it;
		}

	private:
		inline iiterator get(void)
		{
			return this->m_it;
		}

	public:
		YGGR_PP_FRIEND_TYPENAME(boost_tree_opt<
									Key YGGR_PP_SYMBOL_COMMA() 
									Value YGGR_PP_SYMBOL_COMMA() 
									KeyOfValue YGGR_PP_SYMBOL_COMMA()
									KeyCompare YGGR_PP_SYMBOL_COMMA()
									A>);

		typedef rbtree_difference_type        difference_type;

		//Constructors
		const_iterator_opt()
			:  m_it()
		{
		}

		//Pointer like operators
		inline const_reference operator*(void) const
		{
			return m_it->get_data();
		}

		inline const_pointer operator->(void) const
		{
			return const_pointer(&m_it->get_data());
		}

		//Increment / Decrement
		inline const_iterator_opt& operator++(void)
		{
			prot_incr();
			return *this;
		}

		inline const_iterator_opt operator++(int)
		{
			iiterator tmp = m_it;
			++*this;
			return const_iterator_opt(tmp);
		}

		inline const_iterator_opt& operator--(void)
		{
			prot_decr();
			return *this;
		}

		inline const_iterator_opt operator--(int)
		{
			iiterator tmp = m_it;
			--*this;
			return const_iterator_opt(tmp);
		}

		//Comparison operators
		inline bool operator==(const const_iterator_opt& r) const
		{
			return m_it == r.m_it;
		}

		inline bool operator!=(const const_iterator_opt& r)  const
		{
			return m_it != r.m_it;
		}
	};

	class iterator_opt
		: public const_iterator_opt
	{
	private:
		explicit iterator_opt(iiterator it)
			: const_iterator_opt(it)
		{
		}

		inline iiterator get(void)
		{
			return this->m_it;
		}

	public:
		YGGR_PP_FRIEND_TYPENAME(boost_tree_opt<
									Key YGGR_PP_SYMBOL_COMMA() 
									Value YGGR_PP_SYMBOL_COMMA() 
									KeyOfValue YGGR_PP_SYMBOL_COMMA() 
									KeyCompare YGGR_PP_SYMBOL_COMMA() 
									A>);
		typedef rbtree_pointer       pointer;
		typedef rbtree_reference     reference;

		//Constructors
		iterator_opt(void)
		{
		}

		//Pointer like operators
		inline reference operator*(void) const
		{
			return this->m_it->get_data();
		}

		inline pointer operator->(void) const
		{
			return boost::intrusive::pointer_traits<pointer>::pointer_to(this->m_it->get_data());
		}

		//Increment / Decrement
		inline iterator_opt& operator++()
		{
			this->prot_incr();
			return *this;
		}

		inline iterator_opt operator++(int)
		{
			iiterator tmp = this->m_it;
			++*this;
			return iterator_opt(tmp);
		}

		inline iterator_opt& operator--()
		{
			this->prot_decr();
			return *this;
		}

		inline iterator_opt operator--(int)
		{
			iterator_opt tmp = *this;
			--*this;
			return tmp;
		}
	};

private:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

	// org_iterator_cast
	inline iterator& org_iterator_cast(iterator_opt& src) const
	{
		return reinterpret_cast<iterator&>(src);
	}

	inline const iterator& org_iterator_cast(const iterator_opt& src) const
	{
		return reinterpret_cast<const iterator&>(src);
	}

	// org_const_iterator_cast
	inline const_iterator& org_const_iterator_cast(const_iterator_opt& src) const
	{
		return reinterpret_cast<const_iterator&>(src);
	}

	inline const const_iterator& org_const_iterator_cast(const const_iterator_opt& src) const
	{
		return reinterpret_cast<const const_iterator&>(src);
	}

	// opt_iterator_cast
	inline iterator_opt& opt_iterator_cast(iterator& src) const
	{
		return reinterpret_cast<iterator_opt&>(src);
	}

	inline const iterator_opt& opt_iterator_cast(const iterator& src) const
	{
		return reinterpret_cast<const iterator_opt&>(src);
	}

	// opt_const_iterator_cast
	inline const_iterator_opt& opt_const_iterator_cast(const_iterator& src) const
	{
		return reinterpret_cast<const_iterator_opt&>(src);
	}

	inline const const_iterator_opt& opt_const_iterator_cast(const const_iterator& src) const
	{
		return reinterpret_cast<const const_iterator_opt&>(src);
	}


private:
	boost_tree_opt(void);
	boost_tree_opt(BOOST_RV_REF(boost_tree_opt));
	boost_tree_opt(const boost_tree_opt&);
	~boost_tree_opt(void);
	const boost_tree_opt& operator=( const boost_tree_opt& );
	const boost_tree_opt& operator=( BOOST_RV_REF(boost_tree_opt) );

public:

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	template<typename VAlloc, typename ...Args> inline
	static node_ptr_type create_node(const VAlloc& valloc, BOOST_FWD_REF(Args)... args)
	{
		return 
			__create_node<node_ptr_type, alloc_version_traits_type, deallocator_type>(
				valloc, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename VAlloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static node_ptr_type create_node(const VAlloc& valloc \
											YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )){ \
			return \
				__create_node<node_ptr_type, alloc_version_traits_type, deallocator_type>( \
					valloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

public:
	typedef typename Icont::insert_commit_data insert_commit_data;

public:
	// insert/erase
	inline iterator insert_unique_commit(node_ptr_type pnode, insert_commit_data &data)
	{
		assert(pnode);
		iiterator it(this->icont().insert_unique_commit(*pnode, data));
		return org_iterator_cast(iterator_opt(it));
	}

	template<typename Destroy_Holder> inline
	std::pair<iterator, bool>
		insert_unique(Destroy_Holder holder, const key_type& key, node_ptr_type pnode)
	{
		insert_commit_data data;
		std::pair<iterator, bool> ret =
			org_cast().insert_unique_check(key, data);
		if(!ret.second)
		{
			holder(pnode);
			return ret;
		}
		return std::pair<iterator, bool>(
				this->insert_unique_commit(pnode, data), true);
	}

public:
	template<typename Destroy_Holder> inline
	iterator insert_unique(Destroy_Holder holder, const_iterator hint,
							const key_type& key, node_ptr_type pnode)
	{
		insert_commit_data data;
		std::pair<iterator,bool> ret =
			org_cast().insert_unique_check(hint, key, data);
		if(!ret.second)
		{
			holder(pnode);
			return ret.first;
		}
		return this->insert_unique_commit(pnode, data);
	}

	// boost::move_detail::identity not support operator() const, so using "KFV& key_of_value"
	template <typename Destroy_Holder, typename KFV, typename InputIterator>
	void insert_unique(Destroy_Holder holder, /*const*/ KFV& key_of_value,
						InputIterator first, InputIterator last)
	{
		org_type& org = org_cast();
		if(org.empty())
		{
			//Insert with end hint, to achieve linear
			//complexity if [first, last) is ordered
			const_iterator hint(org.cend());
			for( ; first != last; ++first)
			{
				assert(*first);
				hint = this->insert_unique(holder, hint, key_of_value((*first)->get_data()), *first);
			}
		}
		else
		{
			for( ; first != last; ++first)
			{
				assert(*first);
				this->insert_unique(holder, key_of_value((*first)->get_data()), *first);
			}
		}
	}

	// multi support
	inline iterator insert_equal(node_ptr_type pnode)
	{
		assert(pnode);
		return org_iterator_cast(iterator_opt(this->icont().insert_equal(this->icont().end(), *pnode)));
	}

	inline iterator insert_equal(const_iterator hint, node_ptr_type pnode)
	{
		assert(pnode);
		return org_iterator_cast(iterator_opt(this->icont().insert_equal(opt_const_iterator_cast(hint).get(), *pnode)));
	}

	template <class InputIterator>
	void insert_equal(InputIterator first, InputIterator last)
	{
		//Insert with end hint, to achieve linear
		//complexity if [first, last) is ordered
		const_iterator hint(org_cast().cend());
		for( ; first != last; ++first)
		{
			hint = this->insert_equal(hint, *first);
		}
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator position)
	{
		return org_iterator_cast(
				iterator_opt(this->icont().erase_and_dispose(
								opt_const_iterator_cast(position).get(), holder)));
	}

	template<typename Destroy_Holder> inline
	size_type erase(Destroy_Holder holder, const key_type& k)
	{
		return this->erase_key(holder, k, KeyNodeCompare(org_cast().value_comp()), alloc_version());
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator first, const_iterator last)
	{
		return org_iterator_cast(
					iterator_opt(
						this->erase_range(
							holder,
							opt_const_iterator_cast(first).get(),
							opt_const_iterator_cast(last).get(),
							alloc_version())));
	}

	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder holder)
	{
		this->clear_of_holder(holder, alloc_version());
	}

private:
	template<typename Destroy_Holder, typename Comparator, typename AllocVer> inline
	size_type erase_key(Destroy_Holder holder, const key_type& k, const Comparator &comp, AllocVer)
	{
		return this->icont().erase_and_dispose(k, comp, holder);
	}

	template<typename Destroy_Holder, typename Icont_Iter, typename AllocVer> inline
	Icont_Iter erase_range(Destroy_Holder holder,
								const Icont_Iter &first, const Icont_Iter &last, AllocVer)
	{
		return this->icont().erase_and_dispose(first, last, holder);
	}

	template<typename Destroy_Holder, typename AllocVer> inline
	void clear_of_holder(Destroy_Holder holder, AllocVer)
	{
		this->icont().clear_and_dispose(holder);
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105300_BOOST_TREE_OPT_HPP__
