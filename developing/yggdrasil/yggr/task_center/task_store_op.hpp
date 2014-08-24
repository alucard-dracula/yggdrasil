//task_store_op.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_STORE_OP_HPP__
#define __YGGR_TASK_CENTER_TASK_STORE_OP_HPP__

#include <algorithm>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/bind/bind.hpp>

#include <yggr/move/move.hpp>


namespace yggr
{
namespace task_center
{

template<typename Store_Val, typename KeyGetter>
struct key_value_store
{

public:
	typedef Store_Val store_val_type;
	typedef KeyGetter key_getter_type;
	typedef typename store_val_type::first_type key_type;
	typedef typename store_val_type::second_type task_type;

private:
	typedef key_value_store this_type;

public:
	store_val_type get_store_data(BOOST_RV_REF(task_type) t) const
	{
		key_getter_type key_getter;
		return store_val_type(key_getter(t), boost::forward<task_type>(t));
	}

	store_val_type get_store_data(const task_type& t) const
	{
		key_getter_type key_getter;
		return store_val_type(key_getter(t), t);
	}

	template<typename Container>
	void remove(Container& cont, const key_type& key) const
	{
		cont.remove(key);
	}

	template<typename Container,
				template<typename _Key>
					class Key_Container>
	void remove(Container& cont, const Key_Container<key_type>& keys) const
	{
	    typedef Key_Container<key_type> key_cont_type;
	    typedef typename key_cont_type::const_iterator key_cont_citer_type;

		for(key_cont_citer_type i = keys.begin(), isize = keys.end(); i != isize; ++i)
		{
			cont.remove(*i);
		}
	}
};

template<typename Store_Val>
struct random_access_store
{
private:
	typedef random_access_store this_type;

public:
	typedef Store_Val store_val_type;
	typedef store_val_type task_type;

	//typedef Key_Tag key_tag_type;

	BOOST_RV_REF(store_val_type) get_store_data(BOOST_RV_REF(task_type) t) const
	{
		return boost::forward<task_type>(t);
	}

	const store_val_type& get_store_data(const task_type& t) const
	{
		return t;
	}

	template<typename Container, typename Key>
	void remove(Container& cont, const Key& key) const
	{
		std::remove_if(cont.begin(), cont.end(),
						boost::bind(&this_type::is_equal_eins<Key>, this, _1, boost::cref(key)));
	}

	template< typename Container, typename Key,
				template<typename _Key>
				class Key_Container>
	void remove(Container& cont, const Key_Container<Key>& keys) const
	{
        typedef Key key_type;
        typedef Key_Container<Key> key_cont_type;
        typedef typename key_cont_type::const_iterator key_cont_citer_type;
		for(key_cont_citer_type i = keys.begin(), isize = keys.end(); i != isize; ++i)
		{
			std::remove_if(cont.begin(), cont.end(),
							boost::bind(&this_type::is_equal_eins<key_type>, this, _1, boost::cref(*i)));
		}
	}

	template<typename Container, typename Key, typename KeyGetter>
	void remove(Container& cont, const Key& key, const KeyGetter& key_getter) const
	{
		std::remove_if(cont.begin(), cont.end(),
						boost::bind(&this_type::is_equal_zwei<Key, KeyGetter>,
										this,
										_1,
										boost::cref(key),
										boost::cref(key_getter)
										));
	}

	template< typename Container, typename Key, typename KeyGetter,
				template<typename _Key>
				class Key_Container>
	void remove(Container& cont, const Key_Container<Key>& keys, const KeyGetter& key_getter) const
	{
        typedef Key key_type;
        typedef Key_Container<Key> key_cont_type;
        typedef typename key_cont_type::const_iterator key_cont_citer_type;
		for(key_cont_citer_type i = keys.begin(), isize = keys.end(); i != isize; ++i)
		{
			std::remove_if(cont.begin(), cont.end(),
							boost::bind(&this_type::is_equal_zwei<key_type, KeyGetter>,
											this,
											_1,
											boost::cref(*i),
											boost::cref(key_getter)
											));
		}
	}

private:
	template<typename Key>
	bool is_equal_eins(const task_type& t, const Key& right) const
	{
		return t == right;
	}

	template<typename Key, typename KeyGetter>
	bool is_equal_zwei(const task_type& t, const Key& right,
							const KeyGetter& key_getter) const
	{
		return key_getter(t) == right;
	}

};


template<typename Store_Val>
struct sequenced_store
{
private:
	typedef sequenced_store this_type;

public:
	typedef Store_Val store_val_type;
	typedef store_val_type task_type;

	BOOST_RV_REF(store_val_type) get_store_data(BOOST_RV_REF(task_type) t) const
	{
		return boost::forward<task_type>(t);
	}

	const store_val_type& get_store_data(const task_type& t) const
	{
		return t;
	}

	template<typename Container, typename Key>
	void remove(Container& cont, const Key& key) const
	{
		cont.remove_if(boost::bind(&this_type::is_equal_eins<Key>, this, _1, boost::cref(key)));
	}

	template<typename Container, typename Key,
				template<typename _Key>
				class Key_Container>
	void remove(Container& cont, const Key_Container<Key>& keys) const
	{
        typedef Key key_type;
        typedef Key_Container<Key> key_cont_type;
        typedef typename key_cont_type::const_iterator key_cont_citer_type;
		for(key_cont_citer_type i = keys.begin(), isize = keys.end(); i != isize; ++i)
		{
			cont.remove_if(boost::bind(&this_type::is_equal_eins<key_type>, this, _1, boost::cref(*i)));
		}
	}

	template<typename Container, typename Key, typename KeyGetter>
	void remove(Container& cont, const Key& key, const KeyGetter& key_getter) const
	{
		cont.remove_if(boost::bind(&this_type::is_equal_zwei<Key, KeyGetter>,
										this,
										_1,
										boost::cref(key),
										boost::cref(key_getter)
										));
	}

	template<typename Container, typename Key, typename KeyGetter,
				template<typename _Key>
				class Key_Container>
	void remove(Container& cont, const Key_Container<Key>& keys, const KeyGetter& key_getter) const
	{
        typedef Key key_type;
        typedef Key_Container<Key> key_cont_type;
        typedef typename key_cont_type::const_iterator key_cont_citer_type;

		for(key_cont_citer_type i = keys.begin(), isize = keys.end(); i != isize; ++i)
		{
			cont.remove_if(boost::bind(&this_type::is_equal_zwei<key_type, KeyGetter>,
										this,
										_1,
										boost::cref(*i),
										boost::cref(key_getter)
										));
		}
	}

private:
	template<typename Key>
	bool is_equal_eins(const task_type& t, const Key& right) const
	{
		return t == right;
	}

	template<typename Key, typename KeyGetter>
	bool is_equal_zwei(const task_type& t, const Key& right,
						const KeyGetter& key_getter) const
	{
		return key_getter(t) == right;
	}
};

// non_remove_mode
template<typename Store_Val, typename KeyGetter>
struct nonremove_key_value_store
{

public:
	typedef Store_Val store_val_type;
	typedef KeyGetter key_getter_type;
	typedef typename store_val_type::first_type key_type;
	typedef typename store_val_type::second_type task_type;

private:
	typedef nonremove_key_value_store this_type;

public:
	store_val_type get_store_data(BOOST_RV_REF(task_type) t) const
	{
		key_getter_type key_getter;
		return store_val_type(key_getter(t), boost::forward<task_type>(t));
	}

	store_val_type get_store_data(const task_type& t) const
	{
		key_getter_type key_getter;
		return store_val_type(key_getter(t), t);
	}

	template<typename Container>
	inline void remove(Container& cont, const key_type& key) const
	{
		return;
	}

	template<typename Container,
				template<typename _Key>
					class Key_Container>
	inline void remove(Container& cont, const Key_Container<key_type>& keys) const
	{
		return;
	}
};


template<typename Store_Val>
struct nonremove_linner_store
{
private:
	typedef nonremove_linner_store this_type;

public:
	typedef Store_Val store_val_type;
	typedef store_val_type task_type;

	BOOST_RV_REF(store_val_type) get_store_data(BOOST_RV_REF(task_type) t) const
	{
		return boost::forward<task_type>(t);
	}

	const store_val_type& get_store_data(const task_type& t) const
	{
		return t;
	}

	template<typename Container, typename Key>
	inline void remove(Container& cont, const Key& key) const
	{
		return;
	}

	template<typename Container, typename Key,
				template<typename _Key>
				class Key_Container>
	inline void remove(Container& cont, const Key_Container<Key>& ids) const
	{
		return;
	}

	template<typename Container, typename Key, typename KeyGetter>
	inline void remove(Container& cont, const Key& key, const KeyGetter& key_getter) const
	{
		return;
	}

	template<typename Container, typename Key, typename KeyGetter,
				template<typename _Key>
				class Key_Container>
	inline void remove(Container& cont, const Key_Container<Key>& ids, const KeyGetter& key_getter) const
	{
		return;
	}
};

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_TASK_STORE_OP_HPP__
