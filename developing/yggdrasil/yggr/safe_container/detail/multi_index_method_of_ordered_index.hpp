// multi_index_method_of_ordered_index.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_MULTI_INDEX_METHOD_OF_ORDERED_INDEX_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_MULTI_INDEX_METHOD_OF_ORDERED_INDEX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <boost/bind.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <yggr/mplex/foo_type_info.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Container>
class ordered_index_method
{
private:
	typedef Container container_type;
	typedef container_type cont_type;

	typedef typename cont_type::allocator_type allocator_type;

	typedef typename cont_type::key_type key_type;
	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::key_from_value key_from_value;
	typedef typename cont_type::value_compare value_compare;
	typedef typename cont_type::key_compare key_compare;

	typedef typename cont_type::size_type size_type;

	typedef typename cont_type::iterator iterator;
	typedef typename cont_type::const_iterator const_iterator;

private:
	typedef ordered_index_method this_type;

private:
	//allocator :
	static allocator_type get_allocator(const cont_type& cont)
	{
		return cont.get_allocator();
	}

	//copy :
	static void operator_set(cont_type& l, const cont_type& r)
	{
		l = r;
	}

	// capacity:
	static bool empty(const cont_type& cont)
	{
		return cont.empty();
	}

	static size_type size(const cont_type& cont)
	{
		return cont.size();
	}

	static size_type max_size(const cont_type& cont)
	{
		return cont.max_size();
	}

	// modifiers:

	static bool insert_of_val(cont_type& cont, const value_type& val)
	{
		return cont.insert(val).second;
	}

	template< typename InputIter >
	static bool insert_of_iter(cont_type& cont, InputIter start, InputIter last)
	{
		size_type old_size = cont.size();
		cont.insert(start, last);
		return old_size < cont.size();
	}

	static size_type erase(cont_type& cont, const key_type& key)
	{
		return cont.erase(key);
	}

	static bool replace(cont_type& cont, const key_type& key, const value_type& val)
	{
		iterator iter = cont.find(key);
		if(iter == cont.end())
		{
			return false;
		}

		return cont.replace(iter, val);
	}

	template< typename Modifier >
	static bool modify(cont_type& cont, const key_type& key, const Modifier& modifier)
	{
		iterator iter = cont.find(key);
		if(iter == cont.end())
		{
			return false;
		}

		return cont.modify(iter, modifier);
	}

	template< typename Modifier, typename Rollback >
	static bool modify(cont_type& cont, const key_type& key, const Modifier& modifier, const Rollback& rollback)
	{
		iterator iter = cont.find(key);
		if(iter == cont.end())
		{
			return false;
		}

		return cont.modify(iter, modifier, rollback);
	}

	template<typename Modifier>
	static bool modify_key(cont_type& cont, const key_type& key, const Modifier& modifier)
	{
		iterator iter = cont.find(key);
		if(iter == cont.end())
		{
			return false;
		}

		return cont.modify_key(iter, modifier);
	}

	template<typename Modifier, typename Rollback>
	static bool modify_key(cont_type& cont, const key_type& key, const Modifier& modifier, const Rollback& rollback)
	{
		iterator iter = cont.find(key);

		if(iter == cont.end())
		{
			return false;
		}

		return cont.modify_key(iter, modifier, rollback);
	}

	static void swap(cont_type& l, cont_type& r)
	{
		l.swap(r);
	}

	static void clear(cont_type& cont)
	{
		cont.clear();
	}

	// observers:
	static key_from_value key_extractor(const cont_type& cont)
	{
		return cont.key_extractor();
	}

	static key_compare key_comp(const cont_type& cont)
	{
		return cont.key_comp();
	}

	static value_compare value_comp(const cont_type& cont)
	{
		return cont.value_comp();
	}

	// set operations:

	template<typename Key>
	static size_type count(const cont_type& cont, const Key& key)
	{
		return cont.count(key);
	}

	template<typename Key, typename Cmp >
	static size_type count(const cont_type& cont, const Key& key, const Cmp& cmp)
	{
		return cont.count(key, cmp);
	}

	// index comparison:
	static bool operator_equal_to(const cont_type& l, const cont_type& r)
	{
		return l == r;
	}

	static bool operator_not_equal_to(const cont_type& l, const cont_type& r)
	{
		return l != r;
	}

	static bool operator_less(const cont_type& l, const cont_type& r)
	{
		return l < r;
	}

	static bool operator_less_equal(const cont_type& l, const cont_type& r)
	{
		return l <= r;
	}

	static bool operator_greater(const cont_type& l, const cont_type& r)
	{
		return l > r;
	}

	static bool operator_greater_equal(const cont_type& l, const cont_type& r)
	{
		return l >= r;
	}

	// other
	template<typename Key, typename Val>
	static bool get_value(const cont_type& cont, const key_type& key, Val& val)
	{
		typedef typename cont_type::const_iterator citer_type;
		citer_type iter = cont.find(key);
		if(iter == cont.end())
		{
			return false;
		}

		val = *iter;
		return true;
	}

public:

	class use_method
	{
	private:
		typedef ordered_index_method parent_type;
	public:
		use_method(void) {}
		~use_method(void) {}

		//get_allocator
		boost::_bi::bind_t< allocator_type, typename mplex::foo_type_info_1< allocator_type, const cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type>
			get_allocator(void) const
		{
			return boost::bind(&parent_type::get_allocator, _1);
		}

		// operator=
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2<void, cont_type&, const cont_type&>::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper<cont_type const> >::type>
			operator_set(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_set, _1, boost::cref(right));
		}

		// empty()
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_1<bool, const cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type>
			empty(void) const
		{
			return boost::bind(&parent_type::empty, _1);
		}

		// size()
		boost::_bi::bind_t< size_type, typename mplex::foo_type_info_1<size_type, const cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type>
			size(void) const
		{
			return boost::bind(&parent_type::size, _1);
		}

		// max_size()
		boost::_bi::bind_t< size_type, typename mplex::foo_type_info_1<size_type, const cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type>
			max_size(void) const
		{
			return boost::bind(&parent_type::max_size, _1);
		}

		// insert
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, cont_type&, const value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper< value_type const > >::type >
			insert(const value_type& val) const
		{
			return boost::bind(&parent_type::insert_of_val, _1, boost::cref(val));
		}

		//insert
		template< typename InputIter >
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_3< bool, cont_type&, InputIter, InputIter >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>, InputIter, InputIter >::type >
			insert(InputIter start, InputIter last)
		{
			typedef InputIter iter_type;
			return boost::bind(&parent_type::insert_of_iter< iter_type >, _1, start, last);
		}

		// erase
		boost::_bi::bind_t< size_type, typename mplex::foo_type_info_2< size_type, cont_type&, const key_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper< key_type const > >::type >
			erase(const key_type& key) const
		{
			return boost::bind(&parent_type::erase, _1, boost::cref(key));
		}

		// replace
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_3< bool, cont_type&, const key_type&, const value_type& >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															boost::reference_wrapper< key_type const >,
															boost::reference_wrapper< value_type const > >::type >
			replace(const key_type& key, const value_type& val) const
		{
			return boost::bind(&parent_type::replace, _1, boost::cref(key), boost::cref(val));
		}

		// modify
		template<typename Modifier>
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_3< bool, cont_type&, const key_type&, const Modifier& >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															boost::reference_wrapper< key_type const >,
															boost::reference_wrapper< Modifier const > >::type >
			modify(const key_type& key, const Modifier& modifier) const
		{
			typedef Modifier modifier_type;
			return boost::bind(&parent_type::modify<modifier_type>,
									_1, boost::cref(key), boost::cref(modifier));
		}

		// modify
		template<typename Modifier, typename Rollback>
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_4< bool, cont_type&, const key_type&,
																	const Modifier&, const Rollback& >::foo_type,
							typename boost::_bi::list_av_4< boost::arg<1>,
															boost::reference_wrapper< key_type const >,
															boost::reference_wrapper< Modifier const >,
															boost::reference_wrapper< Rollback const > >::type >
			modify(const key_type& key, const Modifier& modifier, const Rollback& rollback) const
		{
			typedef Modifier modifier_type;
			typedef Rollback rollback_type;
			return boost::bind(&parent_type::modify<modifier_type, rollback_type>,
									_1, boost::cref(key), boost::cref(modifier), boost::cref(rollback));
		}

		// modify_key
		template<typename Modifier>
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_3< bool, cont_type&, const key_type&, const Modifier& >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															boost::reference_wrapper< key_type const >,
															boost::reference_wrapper< Modifier const > >::type >
			modify_key(const key_type& key, const Modifier& modifier) const
		{
			typedef Modifier modifier_type;
			return boost::bind(&parent_type::modify_key<modifier_type>,
									_1, boost::cref(key), boost::cref(modifier));
		}

		// modify_key
		template<typename Modifier, typename Rollback>
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_4< bool, cont_type&, const key_type&,
																	const Modifier&, const Rollback& >::foo_type,
							typename boost::_bi::list_av_4< boost::arg<1>,
															boost::reference_wrapper< key_type const >,
															boost::reference_wrapper< Modifier const >,
															boost::reference_wrapper< Rollback const > >::type >
			modify_key(const key_type& key, const Modifier& modifier, const Rollback& rollback) const
		{
			typedef Modifier modifier_type;
			typedef Rollback rollback_type;
			return boost::bind(&parent_type::modify_key<modifier_type, rollback_type>,
									_1, boost::cref(key), boost::cref(modifier), boost::cref(rollback));
		}

		//swap
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2< void, cont_type&, cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper< cont_type > >::type >
			swap(cont_type& other) const
		{
			return boost::bind(&parent_type::swap, _1, boost::ref(other));
		}

		// clear()
		boost::_bi::bind_t< void, typename mplex::foo_type_info_1< void, cont_type& >::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			clear(void) const
		{
			return boost::bind(&parent_type::clear, _1);
		}

		// count
		template< typename Key >
		boost::_bi::bind_t< size_type, typename mplex::foo_type_info_2< size_type, const cont_type&, const Key& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper< Key const > >::type >
			count(const Key& key) const
		{
			return boost::bind(&parent_type::count<Key>, _1, boost::cref(key));
		}

		// count
		template< typename Key, typename Cmp >
		boost::_bi::bind_t< size_type, typename mplex::foo_type_info_3< size_type, const cont_type&,
																			const Key&, const Cmp& >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															boost::reference_wrapper< Key const >,
															boost::reference_wrapper< Cmp const > >::type >
			count(const Key& key, const Cmp& cmp) const
		{
			return boost::bind(&parent_type::count<Key, Cmp>, _1, boost::cref(key), boost::cref(cmp));
		}

		// key_extractor
		boost::_bi::bind_t< key_from_value, typename mplex::foo_type_info_1< key_from_value, const cont_type& >::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			key_extractor(void) const
		{
			return boost::bind(&parent_type::key_extractor, _1);
		}

		// key_comp
		boost::_bi::bind_t< key_compare, typename mplex::foo_type_info_1< key_compare, const cont_type& >::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			key_comp(void) const
		{
			return boost::bind(&parent_type::key_comp, _1);
		}

		// value_comp
		boost::_bi::bind_t< value_compare, typename mplex::foo_type_info_1< value_compare, const cont_type& >::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			value_comp(void) const
		{
			return boost::bind(&parent_type::value_comp, _1);
		}

		// index comparison:
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, const cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type const > >::type >
			operator_equal_to(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_equal_to, _1, boost::cref(right));
		}

		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, const cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type const > >::type >
			operator_not_equal_to(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_not_equal_to, _1, boost::cref(right));
		}

		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, const cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type const > >::type >
			operator_less(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_less, _1, boost::cref(right));
		}

		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, const cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type const > >::type >
			operator_less_equal(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_less_equal, _1, boost::cref(right));
		}

		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, const cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type const > >::type >
			operator_greater(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_greater, _1, boost::cref(right));
		}

		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, const cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type const > >::type >
			operator_greater_equal(const cont_type& right) const
		{
			return boost::bind(&parent_type::operator_greater_equal, _1, boost::cref(right));
		}

		//get_value
		template<typename Key, typename Val>
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_3< bool, const cont_type&, const Key&, Val&>::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															boost::reference_wrapper< Key const >,
															boost::reference_wrapper< Val > >::type >
			get_value(const Key& key, Val& val) const
		{
			typedef Key now_key_type;
			typedef Val now_val_type;
			return boost::bind(&parent_type::get_value<now_key_type, now_val_type>, _1, boost::cref(key), boost::ref(val));
		}
	};
};

} // namspace detial
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_MULTI_INDEX_METHOD_OF_ORDERED_INDEX_HPP__
