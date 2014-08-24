// multi_index_method_of_random_access_index.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_MULTI_INDEX_METHOD_OF_RANDOM_ACCESS_INDEX_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_MULTI_INDEX_METHOD_OF_RANDOM_ACCESS_INDEX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <boost/bind.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <yggr/mplex/foo_type_info.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Container>
class random_access_index_method
{
private:
	typedef Container container_type;
	typedef container_type cont_type;

	typedef typename cont_type::allocator_type allocator_type;

	typedef typename cont_type::value_type value_type;
	typedef typename cont_type::size_type size_type;

	typedef typename cont_type::iterator iterator;
	typedef typename cont_type::const_iterator const_iterator;

private:
	typedef random_access_index_method this_type;

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

	static void assign(cont_type& cont, size_type n, const value_type& val)
	{
		cont.assign(n, val);
	}

	template<typename InputIter>
	static void assign(cont_type& cont, InputIter start, InputIter last)
	{
		cont.assign(start, last);
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

	static size_type capacity(const cont_type& cont)
	{
		return cont.capacity();
	}

	static void reserve(cont_type& cont, size_type m)
	{
		cont.reserve(m);
	}

	static void resize(cont_type& cont, size_type m)
	{
		cont.resize(m);
	}

	static void resize(cont_type& cont, size_type m, const value_type& val)
	{
		cont.resize(m, val);
	}

	// access:

	static bool at(cont_type& cont, size_type n, value_type& val)
	{
		if(cont.size() <= n)
		{
			return false;
		}

		val = cont.at(n);
		return true;
	}

	static bool front(const cont_type& cont, value_type& val)
	{
		if(cont.empty())
		{
			return false;
		}

		val = cont.front();
		return true;
	}

	static bool back(const cont_type& cont, value_type& val)
	{
		if(cont.empty())
		{
			return false;
		}

		val = cont.back();
		return true;
	}

	// modifiers:
	static bool push_front(cont_type& cont, const value_type& val)
	{
		return cont.push_front(val).second;
	}

	static bool pop_front(cont_type& cont)
	{
		if(cont.empty())
		{
			return false;
		}

		cont.pop_front();
		return true;
	}

	static bool pop_front(cont_type& cont, value_type& val)
	{
		if(cont.empty())
		{
			return false;
		}

		val = cont.front();
		cont.pop_front();
		return true;
	}

	static bool push_back(cont_type& cont, const value_type& val)
	{
		return cont.push_back(val).second;
	}

	static bool pop_back(cont_type& cont)
	{
		if(cont.empty())
		{
			return false;
		}

		cont.pop_back();
		return true;
	}

	static bool pop_back(cont_type& cont, value_type& val)
	{
		if(cont.empty())
		{
			return false;
		}

		val = cont.back();
		cont.pop_back();
		return true;
	}

	static void remove(cont_type& cont, const value_type& val)
	{
		cont.remove(val);
	}

	template<typename Cmp>
	static void remove_if(cont_type& cont, const Cmp& cmp)
	{
		cont.remove_if(cmp);
	}

	static void unique_of_default(cont_type& cont)
	{
		cont.unique();
	}

	template<typename Cmp>
	static void unique_of_cmp(cont_type& cont, const Cmp& cmp)
	{
		cont.unique(cmp);
	}

	static void merge_of_default(cont_type& l, cont_type& r)
	{
		l.merge(r);
	}

	template<typename Cmp>
	static void merge_of_cmp(cont_type& l, cont_type& r, const Cmp& cmp)
	{
		l.merge(r, cmp);
	}

	static void sort_of_default(cont_type& cont)
	{
		cont.sort();
	}

	template<typename Cmp>
	static void sort_of_cmp(cont_type& cont, const Cmp& cmp)
	{
		cont.sort(cmp);
	}

	static void reverse(cont_type& cont)
	{
		cont.reverse();
	}

	static void swap(cont_type& l, cont_type& r)
	{
		l.swap(r);
	}

	static void clear(cont_type& cont)
	{
		cont.clear();
	}

	// set operations:

//	template<typename Key>
//	static size_type count(const cont_type& cont, const Key& key)
//	{
//		return cont.count(key);
//	}
//
//	template<typename Key, typename Cmp >
//	static size_type count(const cont_type& cont, const Key& key, const Cmp& cmp)
//	{
//		return cont.count(key, hasher, cmp);
//	}

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

public:

	class use_method
	{
	private:
		typedef random_access_index_method parent_type;
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

		// assign
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2<void, cont_type&, const value_type&>::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper<value_type const> >::type>
			assign(const value_type& val) const
		{
			return boost::bind(&parent_type::assign, _1, boost::cref(val));
		}

		// assign
		template<typename InputIter>
		boost::_bi::bind_t< void, typename mplex::foo_type_info_3<void, cont_type&, InputIter, InputIter>::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, boost::reference_wrapper<value_type const> >::type>
			assign(InputIter start, InputIter last) const
		{
			return boost::bind(&parent_type::assign, _1, start, last);
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

		// capacity()
		boost::_bi::bind_t< size_type, typename mplex::foo_type_info_1<size_type, const cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type>
			capacity(void) const
		{
			return boost::bind(&parent_type::capacity, _1);
		}

		//reserve
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2<void, cont_type&, size_type>::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, size_type >::type>
			reserve(size_type m) const
		{
			return boost::bind(&parent_type::reserve, _1, m);
		}

		//resize
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2<void, cont_type&, size_type>::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>, size_type >::type>
			resize(size_type m) const
		{
			return boost::bind(&parent_type::resize, _1, m);
		}

		//resize
		boost::_bi::bind_t< void, typename mplex::foo_type_info_3<void, cont_type&, size_type, const value_type&>::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															size_type,
															boost::reference_wrapper< value_type const > >::type>
			resize(size_type m, const value_type& val) const
		{
			return boost::bind(&parent_type::resize, _1, m, boost::cref(val));
		}

		//at
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_3<bool, cont_type&, size_type, value_type& >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															size_type,
															boost::reference_wrapper< value_type > >::type>
			at(size_type n, value_type& val) const
		{
			return boost::bind(&parent_type::at, _1, n, boost::ref(val));
		}

		// front()
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type > >::type >
			front(value_type& val) const
		{
			return boost::bind(&parent_type::front, _1, boost::ref(val));
		}

		// back()
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, const cont_type&, value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type > >::type >
			back(value_type& val) const
		{
			return boost::bind(&parent_type::back, _1, boost::ref(val));
		}

		// push_front
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, cont_type&, const value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type const > >::type >
			push_front(const value_type& val) const
		{
			return boost::bind(&parent_type::push_front, _1, boost::cref(val));
		}

		// pop_front
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_1< bool, cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			pop_front(void) const
		{
			return boost::bind(&parent_type::pop_front, _1);
		}

		// pop_front
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, cont_type&, value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type > >::type >
			pop_front(value_type& val) const
		{
			return boost::bind(&parent_type::pop_front, _1, boost::ref(val));
		}

		// push_back
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, cont_type&, const value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type const > >::type >
			push_back(const value_type& val) const
		{
			return boost::bind(&parent_type::push_back, _1, boost::cref(val));
		}

		// pop_back
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_1< bool, cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			pop_back(void) const
		{
			return boost::bind(&parent_type::pop_back, _1);
		}

		// pop_back
		boost::_bi::bind_t< bool, typename mplex::foo_type_info_2< bool, cont_type&, value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type > >::type >
			pop_back(value_type& val) const
		{
			return boost::bind(&parent_type::pop_back, _1, boost::ref(val));
		}

		// remove
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2< void, cont_type&, const value_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															 boost::reference_wrapper< value_type const > >::type >
			remove(const value_type& val) const
		{
			return boost::bind(&parent_type::remove, _1, boost::cref(val));
		}

		// remove_if
		template< typename Cmp >
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2< void, cont_type&, const Cmp& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< Cmp const > >::type >
			remove_if(const Cmp& cmp) const
		{
			typedef Cmp cmp_type;
			return boost::bind(&parent_type::remove_if<cmp_type>, _1, boost::cref(cmp));
		}

		// unique()
		boost::_bi::bind_t< void, typename mplex::foo_type_info_1< void, cont_type& >::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			unique(void) const
		{
			return boost::bind(&parent_type::unique_of_default, _1);
		}

		// unique()
		template< typename Cmp >
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2< void, cont_type&, const Cmp& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< Cmp const > >::type >
			unique(const Cmp& cmp) const
		{
			typedef Cmp cmp_type;
			return boost::bind(&parent_type::unique_of_cmp<cmp_type>, _1, boost::cref(cmp));
		}

		// merge
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2< void, cont_type&, cont_type& >::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< cont_type > >::type >
			merge(cont_type& cont) const
		{
			return boost::bind(&parent_type::merge_of_default, _1, boost::ref(cont));
		}

		// merge
		template< typename Cmp >
		boost::_bi::bind_t< void, typename mplex::foo_type_info_3< void, cont_type&, cont_type&, Cmp >::foo_type,
							typename boost::_bi::list_av_3< boost::arg<1>,
															boost::reference_wrapper< cont_type >,
															boost::reference_wrapper< Cmp const > >::type >
			merge(cont_type& cont, const Cmp& cmp) const
		{
		    typedef Cmp cmp_type;
			return boost::bind(&parent_type::merge_of_cmp<cmp_type>, _1, boost::ref(cont), boost::cref(cmp));
		}

		// sort
		boost::_bi::bind_t< void, typename mplex::foo_type_info_1< void, cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			sort(void) const
		{
			return boost::bind(&parent_type::sort_of_default, _1);
		}

		// sort
		template< typename Cmp >
		boost::_bi::bind_t< void, typename mplex::foo_type_info_2< void, cont_type&, Cmp>::foo_type,
							typename boost::_bi::list_av_2< boost::arg<1>,
															boost::reference_wrapper< Cmp const > >::type >
			sort(const Cmp& cmp) const
		{
			typedef Cmp cmp_type;
			return boost::bind(&parent_type::sort_of_cmp<cmp_type>, _1, boost::cref(cmp));
		}

		// reverse
		boost::_bi::bind_t< void, typename mplex::foo_type_info_1< void, cont_type&>::foo_type,
							typename boost::_bi::list_av_1< boost::arg<1> >::type >
			reverse(void) const
		{
			return boost::bind(&parent_type::reverse, _1);
		}

		// swap()
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
	};
};

} // namspace detial
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_MULTI_INDEX_METHOD_OF_RANDOM_ACCESS_INDEX_HPP__
