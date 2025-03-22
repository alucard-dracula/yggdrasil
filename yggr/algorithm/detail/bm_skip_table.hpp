//bm_skip_table.hpp

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

#ifndef __YGGR_ALGORITHM_DETAIL_BM_SKIP_TABLE_HPP__
#define __YGGR_ALGORITHM_DETAIL_BM_SKIP_TABLE_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/container/array.hpp>
#include <yggr/container/unordered_map.hpp>
#include <yggr/container/index_generator.hpp>

#include <yggr/algorithm/detail/bm_traits.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/type_traits/is_integral.hpp>

#include <climits>      // for CHAR_BIT
#include <utility>
#include <functional>
#include <memory>

namespace yggr
{
namespace algorithm
{
namespace detail
{

template<typename BM_Traits, typename Key_Extractor, std::size_t array_size = 0>
class bm_skip_table;

} // detail
} // namespace algorithm
} // namespaces yggr

namespace yggr
{
namespace algorithm
{
namespace detail
{

template<typename BM_Traits, typename Key_Extractor, std::size_t array_size>
class bm_skip_table
{
public:
	typedef BM_Traits bm_traits_type;
	typedef typename bm_traits_type::key_type key_type;
	typedef typename bm_traits_type::value_type pat_value_type;
	typedef typename bm_traits_type::difference_type difference_type;
	typedef Key_Extractor key_extor_type;

#ifdef _DEBUG
public:
#else
private:
#endif // _DEBUG
	typedef ::yggr::array<difference_type, array_size> skip_map_type;

public:
	typedef typename skip_map_type::size_type size_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(array_size > 0)>));
	BOOST_MPL_ASSERT((boost::is_integral<key_type>));

private:
	typedef bm_skip_table this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	bm_skip_table(size_type default_size, difference_type default_val)
	{
		assert(default_size < _skip.max_size());
		std::fill_n(&_skip[0], _skip.max_size(), default_val);
	}

	bm_skip_table(size_type default_size, difference_type default_val,
					const key_extor_type& key_extor)
		: _key_extor(key_extor)
	{
		assert(default_size < _skip.max_size());
		std::fill_n(&_skip[0], _skip.max_size(), default_val);
	}

	bm_skip_table(BOOST_RV_REF(this_type) right)
		: _key_extor(boost::move(right._key_extor)),
			_skip(boost::move(right._skip))
	{
	}

	bm_skip_table(const this_type& right)
		: _key_extor(right._key_extor),
			_skip(right._skip)
	{
	}

	~bm_skip_table(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		_key_extor = boost::move(right._key_extor);
		copy_or_move_or_swap(_skip, boost::move(right._skip));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_key_extor = right._key_extor;
		_skip = right._skip;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_key_extor, right._key_extor);
		_skip.swap(right._skip);
	}

public:
	inline void insert(const pat_value_type& val_key, difference_type step)
	{
		assert(_key_extor(val_key) < _skip.max_size());
		_skip[_key_extor(val_key)] = step;
	}

	inline difference_type operator[](const pat_value_type& val_key) const
	{
		assert(_key_extor(val_key) < _skip.max_size());
		return _skip[_key_extor(val_key)];
	}

	inline void reinit(size_type default_size, difference_type default_val)
	{
		assert(default_size < _skip.max_size());
		std::fill_n(&_skip[0], _skip.max_size(), default_val);
	}

	inline void reinit(size_type default_size, difference_type default_val, const key_extor_type& key_extor)
	{
		assert(default_size < _skip.max_size());
		_key_extor = key_extor;
		std::fill_n(&_skip[0], _skip.max_size(), default_val);
	}

	inline const key_extor_type& key_extor(void) const
	{
		return _key_extor;
	}

#ifdef _DEBUG

	inline const skip_map_type skip_table(void) const
	{
		return _skip;
	}

#endif // _DEBUG

private:
	key_extor_type _key_extor;
	skip_map_type _skip;
};

//template<typename PatIter, typename Key, typename Key_Extractor>
template<typename BM_Traits, typename Key_Extractor>
class bm_skip_table<BM_Traits, Key_Extractor, 0>
{

public:
	typedef BM_Traits bm_traits_type;
	typedef typename bm_traits_type::key_type key_type;
	typedef typename bm_traits_type::value_type pat_value_type;
	typedef typename bm_traits_type::difference_type difference_type;
	typedef Key_Extractor key_extor_type;

#ifdef _DEBUG
public:
#else
private:
#endif // _DEBUG
	typedef ::yggr::unordered_map<key_type, difference_type> skip_map_type;

public:
	typedef typename skip_map_type::size_type size_type;

private:
	typedef bm_skip_table this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	bm_skip_table(size_type default_size, difference_type default_val)
		: _default_val(default_val), _skip(default_size)
	{
	}

	bm_skip_table(size_type default_size, difference_type default_val,
					const key_extor_type& key_extor)
		: _default_val(default_val), _key_extor(key_extor),
			_skip(default_size)
	{
	}

	bm_skip_table(BOOST_RV_REF(this_type) right)
		: _default_val(boost::move(right._default_val)),
			_key_extor(boost::move(right._key_extor)),
			_skip(boost::move(right._skip))
	{
	}

	bm_skip_table(const this_type& right)
		: _default_val(right._default_val),
			_key_extor(right._key_extor),
			_skip(right._skip)
	{
	}

	~bm_skip_table(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		_default_val = boost::move(right._default_val);
		_key_extor = boost::move(right._key_extor);
		copy_or_move_or_swap(_skip, boost::move(right._skip));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_default_val = right._default_val;
		_key_extor = right._key_extor;
		_skip = right._skip;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_default_val, right._default_val);
		::yggr::swap(_key_extor, right._key_extor);
		_skip.swap(right._skip);
	}

public:
	inline void insert(const pat_value_type& val_key, difference_type step)
	{
		_skip[_key_extor(val_key)] = step;
	}

	inline difference_type operator[](const pat_value_type& val_key) const
	{
		typename skip_map_type::const_iterator iter = _skip.find(_key_extor(val_key));

		return iter == _skip.end()? _default_val : iter->second;
	}

	inline void reinit(size_type default_size, difference_type default_val)
	{
		_default_val = default_val;
		skip_map_type tmp(default_size);
		_skip.swap(tmp);
	}

	inline void reinit(size_type default_size, difference_type default_val, const key_extor_type& key_extor)
	{
		_default_val = default_val;
		_key_extor = key_extor;
		skip_map_type tmp(default_size);
		_skip.swap(tmp);
	}

	inline const key_extor_type& key_extor(void) const
	{
		return _key_extor;
	}

#ifdef _DEBUG

	inline const skip_map_type skip_table(void) const
	{
		return _skip;
	}

#endif // _DEBUG

private:
	difference_type _default_val;
	key_extor_type _key_extor;
	skip_map_type _skip;
};

} // detail
} // namespace algorithm
} // namespaces yggr

namespace yggr
{
namespace algorithm
{
namespace detail
{
namespace swap_support
{

template<typename BM_Traits, typename Key_Extractor, std::size_t array_size> inline
void swap(bm_skip_table<BM_Traits, Key_Extractor, array_size>& l,
			bm_skip_table<BM_Traits, Key_Extractor, array_size>& r) 
{
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace algorithm
} // namespace yggr

namespace std
{
	using ::yggr::algorithm::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::algorithm::detail::swap_support::swap;
} // namespace boost

#endif  //  __YGGR_ALGORITHM_DETAIL_BM_SKIP_TABLE_HPP__

