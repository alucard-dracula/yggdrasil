// string_view_impl.hpp

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

#ifndef __YGGR_CHARSET_STRING_VIEW_IMPL_HPP__
#define __YGGR_CHARSET_STRING_VIEW_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/mplex/limits.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/reserve.hpp>

#include <yggr/charset/const_strings.hpp>
#include <yggr/charset/string_view_decl.hpp>
#include <yggr/charset/charset_type_traits.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/min.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/and.hpp>

#include <boost/functional/hash/hash.hpp>
#include <boost/typeof/typeof.hpp>

#include <iostream>
#include <algorithm>
#include <stdexcept>

YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::basic_string_view, 2)

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename Traits, typename Value>
struct basic_string_view_eq_traits
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::binary_function< Value, Value, bool>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
public:
	typedef Traits traits_type;
	typedef Value value_type;

	typedef value_type first_argument_type;
	typedef value_type second_argument_type;
	typedef bool result_type;

private:
	typedef basic_string_view_eq_traits this_type;

public:
	YGGR_CXX17_CONSTEXPR_OR_INLINE result_type operator()(value_type x, value_type y) const
	{
		return traits_type::eq(x, y); 
	}
};

template<typename EqTraits, typename Value, typename ConstPointer>
struct basic_string_view_not_within_traits
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function<Value, bool>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
public:
	typedef EqTraits eq_traits_type;
	typedef Value value_type;
	typedef ConstPointer const_pointer;
	typedef bool result_type;
	typedef value_type argument_type;

private:
	typedef basic_string_view_not_within_traits this_type;

public:
	basic_string_view_not_within_traits(void)
		: _pfirst(), _plast()
	{
	}

	basic_string_view_not_within_traits(const_pointer pf, const_pointer pl)
		: _pfirst(pf), _plast(pl) 
	{
		assert(_pfirst && _plast);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE result_type operator()(value_type x) const
	{
		eq_traits_type eq_foo;
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
		return std::find_if(_pfirst, _plast, std::bind1st(eq_foo, x)) == _plast;
#else
		return std::find_if(_pfirst, _plast, boost::bind(eq_foo, boost::cref(x), _1)) == _plast;
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	}

private:
	const_pointer _pfirst;
	const_pointer _plast;
};

} // namespace detail
} // namespace charset
} // namespace yggr

YGGR_PP_CONST_STRING_GLOBAL_DEF(empty_string, "")

namespace yggr
{
namespace charset
{

template<typename CharT, typename Traits>
class basic_string_view
{
public:
	typedef Traits traits_type;
	typedef typename traits_type::char_type value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef const_pointer const_iterator;
	typedef const_iterator iterator;

	// must using std::reverse_iterator
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef const_reverse_iterator reverse_iterator;

	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

public:
	YGGR_STATIC_CONSTANT(size_type, npos = static_cast<size_type>(-1));

protected:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef basic_string_view this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	typedef typename
		boost::mpl::min
		<
			boost::mpl::integral_c
			<
				size_type, 
				static_cast<size_type>(
					mplex::numeric_limits<difference_type>::max_type::value)
			>,
			boost::mpl::integral_c
			<
				size_type,
				this_type::npos / sizeof(value_type)
			>
		>::type max_size_t;

public:
	// constructor
	// !!! warning MSVC allows string_view to use null pointers and GCC doesn't allow it, !!!
	// !!! so the GCC standard is used
	basic_string_view(void) YGGR_NOEXCEPT_OR_NOTHROW
		: _pdata(YGGR_PP_CONST_STRING_GLOBAL_GET(empty_string, value_type)), _size(0)
	{
	}

	basic_string_view(const_pointer pdata)
		: _pdata(pdata? pdata : YGGR_PP_CONST_STRING_GLOBAL_GET(empty_string, value_type)),
			 _size(pdata? traits_type::length(pdata) : 0)
	{
	}
	
	basic_string_view(const_pointer pdata, size_type n)
		: _pdata(pdata? pdata : YGGR_PP_CONST_STRING_GLOBAL_GET(empty_string, value_type)), 
			_size(pdata? n : 0)
	{
		assert(_size <= traits_type::length(_pdata));
	}

	template<typename IterS, typename IterE>
	basic_string_view(IterS s, IterE e,
						typename
							boost::enable_if
							<
								boost::mpl::and_
								<
									is_iterator<IterS>,
									is_iterator<IterE>
								>,
								sfinae_type
							>::type sfinae = 0)
		: _pdata(boost::addressof(*s)), _size(std::distance(s, e))
	{
	}

	basic_string_view(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: _pdata(boost::move(right._pdata)), _size(boost::move(right._size))
	{
	}

	basic_string_view(const this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
		: _pdata(right._pdata), _size(right._size)
	{
	}

	~basic_string_view(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		_pdata = boost::move(right._pdata);
		_size = boost::move(right._size);
		
		return *this;
	}

	this_type& operator=(const this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_pdata = right._pdata;
		_size = right._size;

		return *this;
	}

public:
	YGGR_CXX17_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX17_CONSTEXPR void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(_pdata, right._pdata);
		yggr::swap(_size, right._size);
	}

public:
	// Iterators
	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator begin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _pdata;
	}
	
	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator cbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _pdata;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator end(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return ::yggr::advance_copy(_pdata, _size);
	}
	
	YGGR_CXX17_CONSTEXPR_OR_INLINE const_iterator cend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return ::yggr::advance_copy(_pdata, _size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator rbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(::yggr::advance_copy(_pdata, _size));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator crbegin(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(::yggr::advance_copy(_pdata, _size));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator rend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(_pdata);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reverse_iterator crend(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return const_reverse_iterator(_pdata);
	}

public:
	// Element access
	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reference operator[](size_type pos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(pos < _size);
		return *::yggr::advance_copy(_pdata, pos);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reference at(size_type pos) const
	{
		if(!(pos < _size))
		{
			throw std::out_of_range("basic_string_view::at invalid subscript");
		}

		return *::yggr::advance_copy(_pdata, pos);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reference front(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(_pdata && _size);
		return *_pdata;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const_reference back(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		assert(_pdata && _size);
		return *::yggr::advance_copy(_pdata, _size - 1);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE const value_type* data(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _pdata;
	}

public:
	// Capacity
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type size(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _size;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type length(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _size;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type max_size(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return max_size_t::value;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool empty(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return (_size == 0) || (!_pdata);
	}

public:
	// Modifiers
	YGGR_CXX17_CONSTEXPR_OR_INLINE void remove_prefix(size_type n)
	{
		assert(n <= _size);
		_pdata += n;
		_size -= n;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE void remove_suffix(size_type n) 
	{
		assert(n <= _size);
		_size -= n;
	}

public:
	// Operations
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type copy(pointer dst, size_type n, size_type pos = 0) const
	{
		if(!(pos < _size))
		{
			throw std::out_of_range("basic_string_view::copy invalid subscript");
		}

		size_type count = (std::min)(_size - pos, n);
		traits_type::copy(dst, ::yggr::advance_copy(_pdata, pos), count);
		return count;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE this_type substr(size_type pos = 0, size_type n = this_type::npos ) const
	{
		if(!(pos < _size))
		{
			throw std::out_of_range("basic_string_view::substr invalid subscript");
		}

		return this_type(::yggr::advance_copy(_pdata, pos), (std::min)(_size - pos, n));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			_pdata == right._pdata?
				_size == right._size
				: ((_size == right._size) 
					&& (traits_type::compare(_pdata, right._pdata, _size) == 0));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE s32 compare(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		int cmp = traits_type::compare(_pdata, right._pdata, (std::min)(_size, right._size));
		return cmp != 0? cmp : (_size < right._size? -1 : (_size > right._size? 1 : 0));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE s32 compare(size_type pos1, size_type n1, const this_type& right) const
	{
		if(!(pos1 < _size))
		{
			throw std::out_of_range("basic_string_view::compare invalid subscript");
		}

		difference_type count1 = (std::min)(_size - pos1, n1);

		int cmp = traits_type::compare(
					::yggr::advance_copy(_pdata, pos1), 
					right._pdata, 
					(std::min)(count1, static_cast<difference_type>(right._size)));

		return 
			cmp != 0?
				cmp 
				: (count1 < static_cast<difference_type>(right._size)? 
					-1 
					: (count1 > static_cast<difference_type>(right._size)? 1 : 0));

	}
	
	YGGR_CXX17_CONSTEXPR_OR_INLINE s32 compare(size_type pos1, size_type n1,
						const this_type& right, size_type pos2, size_type n2) const
	{
		if(!(pos1 < _size && pos2 < right._size))
		{
			throw std::out_of_range("basic_string_view::compare invalid subscript");
		}

		difference_type count1 = (std::min)(_size - pos1, n1);
		difference_type count2 = (std::min)(right._size - pos2, n2);

		int cmp = traits_type::compare(
					::yggr::advance_copy(_pdata, pos1), 
					::yggr::advance_copy(right._pdata, pos2), 
					(std::min)(count1, count2));

		return cmp != 0? cmp : (count1 < count2? -1 : (count1 > count2? 1 : 0));

	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE s32 compare(const_pointer s) const
	{
		difference_type count2 = s? traits_type::length(s) : 0;
		int cmp = traits_type::compare(_pdata, s, (std::min)(static_cast<difference_type>(_size), count2));

		return 
			cmp != 0?
				cmp 
				: (static_cast<difference_type>(_size) < count2? 
					-1 
					: (static_cast<difference_type>(_size) > count2? 1 : 0));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE s32 compare(size_type pos1, size_type n1, const_pointer s) const
	{
		if(!(pos1 < _size))
		{
			throw std::out_of_range("basic_string_view::compare invalid subscript");
		}

		difference_type count1 = (std::min)(_size - pos1, n1);
		difference_type count2 = s? traits_type::length(s) : 0;

		int cmp = traits_type::compare(
					::yggr::advance_copy(_pdata, pos1), 
					s, 
					(std::min)(count1, count2));

		return cmp != 0? cmp : (count1 < count2? -1 : (count1 > count2? 1 : 0));

	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE s32 compare(size_type pos1, size_type n1,
												 const_pointer s, size_type n2) const
	{
		if(!(pos1 < _size))
		{
			throw std::out_of_range("basic_string_view::compare invalid subscript");
		}

		difference_type count1 = (std::min)(_size - pos1, n1);
		difference_type count2 = (std::min)((s? traits_type::length(s) : 0), n2);

		int cmp = traits_type::compare(
					::yggr::advance_copy(_pdata, pos1), 
					s, 
					(std::min)(count1, count2));

		return cmp != 0? cmp : (count1 < count2? -1 : (count1 > count2? 1 : 0));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool starts_with(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return 
			(!(_size < right._size))
			&& (traits_type::compare(_pdata, right._pdata, right._size) == 0);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool starts_with(value_type c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _pdata && _size && traits_type::eq(this_type::front(), c);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool starts_with(const_pointer s) const
	{
		return this_type::starts_with(this_type(s));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool ends_with(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
        return
			(!(_size < right._size))
				&& (traits_type::compare(
						::yggr::advance_copy(_pdata, (_size - right._size)), 
						right._pdata, 
						right._size) == 0);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool ends_with(value_type c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _pdata && _size && traits_type::eq(this_type::back(), c);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool ends_with(const_pointer s) const
	{
		return this_type::ends_with(this_type(s));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool contains(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(right) != this_type::npos;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool contains(value_type c) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c) != this_type::npos;
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE bool contains(const_pointer s) const
	{
		return this_type::find(s) != this_type::npos;
	}

protected:
	typedef detail::basic_string_view_eq_traits<traits_type, value_type> eq_traits_type;

public:
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(right._pdata, pos, right._size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		eq_traits_type eq_foo;
		const_pointer pe;
		const_pointer prst;

		return 
			(!(pos < _size))?
				this_type::npos
				:(pe = ::yggr::advance_copy(_pdata, _size),
					prst =
						std::find_if(
							::yggr::advance_copy(_pdata, pos), pe,
#						if YGGR_CPP_VERSION < YGGR_CPP_VER_17
							std::bind2nd(eq_foo, c)
#						else
							boost::bind(eq_foo, _1, boost::cref(c))
#						endif  // YGGR_CPP_VERSION < YGGR_CPP_VER_17
							),
					(prst == pe? this_type::npos : std::distance(_pdata, prst)));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find(const_pointer s, size_type pos, size_type n) const
	{
		eq_traits_type eq_foo;
		const_pointer pe;
		const_pointer prst;

		return 
			(_size < pos + n)?
				this_type::npos
				: (pe = ::yggr::advance_copy(_pdata, _size),
					prst = 
						std::search(
							::yggr::advance_copy(_pdata, pos), pe,
							s, ::yggr::advance_copy(s, n),
							eq_foo),
					(prst == pe? this_type::npos : std::distance(_pdata, prst)));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find(const_pointer s, size_type pos = 0) const
	{
		return this_type::find(s, pos, s? traits_type::length(s) : 0);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type rfind(const this_type& right, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(right._pdata, pos, right._size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type rfind(value_type c, size_type pos = this_type::npos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		eq_traits_type eq_foo;
		const_iterator last;
		const_reverse_iterator rrst;

		return 
			(_size < 1)?
				this_type::npos
				: (last = ::yggr::advance_copy(this_type::begin(), (std::min)(_size - 1, pos) + 1),
					rrst =
						std::find_if(
							const_reverse_iterator(last), 
							this_type::rend(),
#						if YGGR_CPP_VERSION < YGGR_CPP_VER_17
							std::bind2nd(eq_foo, c)
#						else
							boost::bind(eq_foo, _1, boost::cref(c))
#						endif // YGGR_CPP_VERSION < YGGR_CPP_VER_17
							),
					(rrst == this_type::rend()? 
						this_type::npos 
						: std::distance(this_type::begin(), ::yggr::advance_copy(rrst.base(), -1))));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type rfind(const_pointer s, size_type pos, size_type n) const
	{
		eq_traits_type eq_foo;
		const_pointer pe;
		const_pointer prst;

		return 
			(_size < n)?
				this_type::npos
				: (n == 0)?
					(std::min)(_size, pos)
					: (pe = ::yggr::advance_copy(_pdata, (std::min)(_size - n, pos) + n),
						prst = 
							std::find_end(
								_pdata, pe,
								s, ::yggr::advance_copy(s, n),
								eq_foo),
						(prst == pe? this_type::npos : std::distance(_pdata, prst)));
	}
	
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type rfind(const_pointer s, size_type pos = this_type::npos) const
	{
		return this_type::rfind(s, pos, s? traits_type::length(s) : 0);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_of(right._pdata, pos, right._size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find(c, pos);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_of(const_pointer s, size_type pos, size_type n) const
	{
		eq_traits_type eq_foo;
		const_pointer pe;
		const_pointer prst;

		return 
			(!(pos < _size))?
				this_type::npos
				: (pe = ::yggr::advance_copy(_pdata, _size),
					prst =
						std::find_first_of(
							::yggr::advance_copy(_pdata, pos), pe,
							s, ::yggr::advance_copy(s, n),
							eq_foo),
					(prst == pe? this_type::npos : std::distance(_pdata, prst)));
	}
	
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_of(const_pointer s, size_type pos = 0) const
	{
		return this_type::find_first_of(s, pos, s? traits_type::length(s) : 0);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_last_of(const this_type& right, size_type pos = npos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_of(right._pdata, pos, right._size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_last_of(value_type c, size_type pos = npos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::rfind(c, pos);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_last_of(const_pointer s, size_type pos, size_type n) const
	{
		eq_traits_type eq_foo;
		const_iterator last;
		const_reverse_iterator rrst;

		return 
			(_size < 1)?
				this_type::npos
				: (last = ::yggr::advance_copy(this_type::begin(), (std::min)(_size - 1, pos) + 1),
					 rrst =
						std::find_first_of(
							const_reverse_iterator(last), this_type::rend(),
							s, ::yggr::advance_copy(s, n),
							eq_foo),
					(rrst == this_type::rend()? 
						this_type::npos 
						: std::distance(this_type::begin(), ::yggr::advance_copy(rrst.base(), -1))));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_last_of(const_pointer s, size_type pos = npos) const
	{
		return this_type::find_last_of(s, pos, s? traits_type::length(s) : 0);
	}

protected:
	typedef 
		detail::basic_string_view_not_within_traits
		<
			eq_traits_type, 
			value_type, 
			const_pointer
		> not_within_traits_type;

public:
	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_not_of(const this_type& right, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_first_not_of(right._pdata, pos, right._size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_not_of(value_type c, size_type pos = 0) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		eq_traits_type eq_foo;
		const_pointer pe;
		const_pointer prst;

		return 
			(!(pos < _size))?
				this_type::npos
				: (pe = ::yggr::advance_copy(_pdata, _size), 
					prst = 
						std::find_if(
							::yggr::advance_copy(_pdata, pos),
							pe,
#						if YGGR_CPP_VERSION < YGGR_CPP_VER_17
							std::not1(std::bind2nd(eq_foo, c))
#						else
							std::not_fn(boost::bind(eq_foo, _1, boost::cref(c)))
#						endif // YGGR_CPP_VERSION < YGGR_CPP_VER_17
							),
					(prst == pe? this_type::npos : std::distance(_pdata, prst)) );
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const
	{
		const_pointer pe;
		const_pointer prst;

		return 
			(!(pos < _size))?
				this_type::npos
				: (pe = ::yggr::advance_copy(_pdata, _size),
					prst = 
						std::find_if(
							::yggr::advance_copy(_pdata, pos), 
							pe, 
							not_within_traits_type(s, ::yggr::advance_copy(s, n))),
					(prst == pe? this_type::npos : std::distance(_pdata, prst)));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_first_not_of(const_pointer s, size_type pos = 0) const
	{
		return this_type::find_first_not_of(s, pos, s? traits_type::length(s) : 0);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE 
	size_type find_last_not_of(const basic_string_view& right, size_type pos = npos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::find_last_not_of(right._pdata, pos, right._size);
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE 
	size_type find_last_not_of(value_type c, size_type pos = npos) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		eq_traits_type eq_foo;
		const_iterator last;
		const_reverse_iterator rrst;

		return 
			(_size < 1)?
				this_type::npos
				: (last = ::yggr::advance_copy(this_type::begin(), (std::min)(_size - 1, pos) + 1),
					rrst = 
						std::find_if(
							const_reverse_iterator(last), 
							this_type::rend(), 
#						if YGGR_CPP_VERSION < YGGR_CPP_VER_17
							std::not1(std::bind2nd(eq_foo, c))
#						else
							std::not_fn(boost::bind(eq_foo, _1, boost::cref(c)))
#						endif // YGGR_CPP_VERSION < YGGR_CPP_VER_17
							),
					(rrst == this_type::rend()?
						this_type::npos
						: std::distance(this_type::begin(), ::yggr::advance_copy(rrst.base(), -1))));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const
	{
		const_iterator last;
		const_reverse_iterator rrst;

		return 
			(_size < 1)?
				this_type::npos
				: (last = ::yggr::advance_copy(this_type::begin(), (std::min)(_size - 1, pos) + 1),
					rrst = 
						std::find_if(
							const_reverse_iterator(last), 
							this_type::rend(), 
							not_within_traits_type(s, ::yggr::advance_copy(s, n))),
					(rrst == this_type::rend()? 
						this_type::npos
						: std::distance(this_type::begin(), ::yggr::advance_copy(rrst.base(), -1))));
	}

	YGGR_CXX17_CONSTEXPR_OR_INLINE size_type find_last_not_of(const_pointer s, size_type pos = npos) const
	{
		return this_type::find_last_not_of(s, pos, s? traits_type::length(s) : 0);
	}

public:
	//Other
	inline std::size_t hash(void) const
	{
		return boost::hash_range(_pdata, ::yggr::advance_copy(_pdata, _size));
	}

protected:
	const_pointer _pdata;
	size_type _size;
};

// non-member foo
// compare basic_string_view

template<typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
bool operator==(const basic_string_view<C, T>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

template<typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
bool operator!=(const basic_string_view<C, T>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !l.compare_eq(r);
}

template<typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
bool operator<(const basic_string_view<C, T>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

template<typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
bool operator<=(const basic_string_view<C, T>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

template<typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
bool operator >(const basic_string_view<C, T>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < l.compare(r);
}

template<typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
bool operator >=(const basic_string_view<C, T>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 <= l.compare(r);
}

// compare different basic_string_view
template<template<typename _C, typename _T> class BasicStringViewL,
			template<typename _C, typename _T> class BasicStringViewR,
			typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_difference_basic_string_view_tpl
		< 
			BasicStringViewL<C, T>,
			BasicStringViewR<C, T>
		>,
		bool
	>::type
	operator==(const BasicStringViewL<C, T>& l, const BasicStringViewR<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(0, l.size(), r.data(), r.size()) == 0;
}

template<template<typename _C, typename _T> class BasicStringViewL,
			template<typename _C, typename _T> class BasicStringViewR,
			typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_difference_basic_string_view_tpl
		< 
			BasicStringViewL<C, T>,
			BasicStringViewR<C, T>
		>,
		bool
	>::type
	operator!=(const BasicStringViewL<C, T>& l, const BasicStringViewR<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(l.compare(0, l.size(), r.data(), r.size()) == 0);
}

template<template<typename _C, typename _T> class BasicStringViewL,
			template<typename _C, typename _T> class BasicStringViewR,
			typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_difference_basic_string_view_tpl
		< 
			BasicStringViewL<C, T>,
			BasicStringViewR<C, T>
		>,
		bool
	>::type
	operator<(const BasicStringViewL<C, T>& l, const BasicStringViewR<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(0, l.size(), r.data(), r.size()) < 0;
}

template<template<typename _C, typename _T> class BasicStringViewL,
			template<typename _C, typename _T> class BasicStringViewR,
			typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_difference_basic_string_view_tpl
		< 
			BasicStringViewL<C, T>,
			BasicStringViewR<C, T>
		>,
		bool
	>::type
	operator<=(const BasicStringViewL<C, T>& l, const BasicStringViewR<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(0, l.size(), r.data(), r.size()) <= 0;
}

template<template<typename _C, typename _T> class BasicStringViewL,
			template<typename _C, typename _T> class BasicStringViewR,
			typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_difference_basic_string_view_tpl
		< 
			BasicStringViewL<C, T>,
			BasicStringViewR<C, T>
		>,
		bool
	>::type
	operator>(const BasicStringViewL<C, T>& l, const BasicStringViewR<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(0, l.size(), r.data(), r.size()) > 0;
}

template<template<typename _C, typename _T> class BasicStringViewL,
			template<typename _C, typename _T> class BasicStringViewR,
			typename C, typename T > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_difference_basic_string_view_tpl
		< 
			BasicStringViewL<C, T>,
			BasicStringViewR<C, T>
		>,
		bool
	>::type
	operator>=(const BasicStringViewL<C, T>& l, const BasicStringViewR<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(0, l.size(), r.data(), r.size()) >= 0;
}

// compare basic_string

#define YGGR_TMP_PP_YGGR_BASIC_STRING_VIEW_CMP_SPECIAL( __basic_string__ ) \
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator==(const __basic_string__<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return 0 == l.compare(0, l.size(), r.data(), r.size()); } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator==(const basic_string_view<C, T>& l, const __basic_string__<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return 0 == r.compare(0, r.size(), l.data(), l.size()); } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator!=(const __basic_string__<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return 0 != l.compare(0, l.size(), r.data(), r.size()); } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator!=(const basic_string_view<C, T>& l, const __basic_string__<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return 0 != r.compare(0, r.size(), l.data(), l.size()); } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator<(const __basic_string__<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return l.compare(0, l.size(), r.data(), r.size()) < 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator<(const basic_string_view<C, T>& l, const __basic_string__<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return r.compare(0, r.size(), l.data(), l.size()) > 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator<=(const __basic_string__<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return l.compare(0, l.size(), r.data(), r.size()) <= 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator<=(const basic_string_view<C, T>& l, const __basic_string__<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return r.compare(0, r.size(), l.data(), l.size()) >= 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator>(const __basic_string__<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return l.compare(0, l.size(), r.data(), r.size()) > 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator>(const basic_string_view<C, T>& l, const __basic_string__<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return r.compare(0, r.size(), l.data(), l.size()) < 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator>=(const __basic_string__<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return l.compare(0, l.size(), r.data(), r.size()) >= 0; } \
	\
	template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE \
	bool operator>=(const basic_string_view<C, T>& l, const __basic_string__<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW { \
		return r.compare(0, r.size(), l.data(), l.size()) <= 0; }

//#if defined(BOOST_NO_CXX17_HDR_STRING_VIEW) // why write this check need some test

	YGGR_TMP_PP_YGGR_BASIC_STRING_VIEW_CMP_SPECIAL(std::basic_string)

//#endif // BOOST_NO_CXX17_HDR_STRING_VIEW

#if (BOOST_VERSION < 106400)

	YGGR_TMP_PP_YGGR_BASIC_STRING_VIEW_CMP_SPECIAL(boost::container::basic_string)

#endif // BOOST_VERSION < 106400

#undef YGGR_TMP_PP_YGGR_BASIC_STRING_VIEW_CMP_SPECIAL


template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator==(const C* l, const BasicStringView<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return r.compare(0, r.size(), l) == 0; 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator==(const BasicStringView<C, T>& l, const C* r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return l.compare(0, l.size(), r) == 0; 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator!=(const C* l, const BasicStringView<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return !(r.compare(0, r.size(), l) == 0); 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator!=(const BasicStringView<C, T>& l, const C* r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return !(l.compare(0, l.size(), r) == 0); 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator<(const C* l, const BasicStringView<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return 0 < r.compare(0, r.size(), l); 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator<(const BasicStringView<C, T>& l, const C* r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return l.compare(0, l.size(), r) < 0; 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator<=(const C* l, const BasicStringView<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return 0 <= r.compare(0, r.size(), l); 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator<=(const BasicStringView<C, T>& l, const C* r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return l.compare(0, l.size(), r) <= 0; 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator>(const C* l, const BasicStringView<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return 0 > r.compare(0, r.size(), l); 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator>(const BasicStringView<C, T>& l, const C* r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return l.compare(0, l.size(), r) > 0; 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator>=(const C* l, const BasicStringView<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return 0 >= r.compare(0, r.size(), l); 
}

template<template<typename _C, typename _T> class BasicStringView,
			typename C, typename T> YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_view_t< BasicStringView<C, T> >,
		bool
	>::type
	operator>=(const BasicStringView<C, T>& l, const C* r) YGGR_NOEXCEPT_OR_NOTHROW 
{
	return l.compare(0, l.size(), r) >= 0; 
}

// +
template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_t< BasicString<C, T, A> >,
		BasicString<C, T, A>
	>::type
	operator+(const BasicString<C, T, A>& l, const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef BasicString<C, T, A> ret_type;

	ret_type ret(l.get_allocator());
	container::reserve(ret, l.size() + r.size());
	ret.append(l);
	ret.append(r.data(), r.size());
	return ret;
}

template<template<typename _C, typename _T, typename _A> class BasicString,
			typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		is_basic_string_t< BasicString<C, T, A> >,
		BasicString<C, T, A>
	>::type
	operator+(const basic_string_view<C, T>& l, const BasicString<C, T, A>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef BasicString<C, T, A> ret_type;

	ret_type ret(r.get_allocator());
	container::reserve(ret, l.size() + r.size());
	ret.append(l.data(), l.size());
	ret.append(r);
	return ret;
}

// +=
//#if defined(BOOST_NO_CXX17_HDR_STRING_VIEW) // why write this check need some test

template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE
std::basic_string<C, T, A>&
	operator+=(std::basic_string<C, T, A>& l,
				const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	l.append(r.data(), r.size());
	return l;
}

//#endif // BOOST_NO_CXX17_HDR_STRING_VIEW

#if (BOOST_VERSION < 106400)

template<typename C, typename T, typename A > YGGR_CXX17_CONSTEXPR_OR_INLINE
boost::container::basic_string<C, T, A>&
	operator+=(boost::container::basic_string<C, T, A>& l,
				const basic_string_view<C, T>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	l.append(r.data(), r.size());
	return l;
}

#endif // BOOST_VERSION < 106400

namespace detail
{

template <class Char, class Traits> inline
bool string_fill(std::basic_ostream<Char, Traits>& os,
                  std::basic_streambuf<Char, Traits>* buf,
                  std::size_t n)
{
	typedef Char char_type;
	typedef Traits traits_type;

	char_type f = os.fill();
	bool ok = true;

	for(std::size_t i = 0; i != n; ++i)
	{
		ok = ok && !traits_type::eq_int_type(buf->sputc(f), traits_type::eof());
	}

	return ok;
}

} // namespace detail

// io
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const basic_string_view<Char, Traits>& s)
{
	typedef std::basic_ostream<Char, Traits> ostream_type;
	typedef typename ostream_type::sentry sentry_type;

	typedef basic_string_view<Char, Traits> string_view_type;
	typedef typename string_view_type::size_type size_type;
	
	sentry_type sentry(os);
	bool ok = false;

	if(sentry) 
	{
		ok = true;
		size_type pad_len = 0;
		bool left = (os.flags() & std::ios::left) != 0;
		std::size_t w = os.width(0);
		std::basic_streambuf<Char, Traits>* buf = os.rdbuf();

		if(w != 0 && s.size() < w)
		{
			pad_len = w - s.size();
		}

		if(!left)
		{
			ok = detail::string_fill(os, buf, pad_len);
		}

		ok = ok && (buf->sputn(s.data(), std::streamsize(s.size())) == std::streamsize(s.size()));

		if(left)
		{
			ok = ok && detail::string_fill(os, buf, pad_len);
		}
	}

	if(!ok)
	{
		os.setstate(std::ios_base::failbit);
	}

	return os;
}

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename Char, typename Traits>
struct is_basic_string_view_t< basic_string_view<Char, Traits> >
	: public boost::mpl::true_
{
};

//template<typename Char, typename Traits>
//struct charset_name_t< basic_string_view<Char, Traits> >
//	: private nonable::noncopyable,
//		private nonable::nonmoveable
//{
//public:
//	typedef Char char_type;
//	typedef typename boost::remove_cv<char_type>::type native_char_type;
//	typedef basic_string<Char, Traits> string_type;
//	typedef basic_string_view<Char, Traits> string_view_type;
//
//public:
//	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
//	{
//		return default_charset_config::get_default_string_charset_name<native_char_type>();
//	}
//
//	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
//	{
//		return default_charset_config::get_str_default_string_charset_name<native_char_type>();
//	}
//};

namespace detail
{

/*
!!!
mingw gcc

template<typename ...T, template<typename ...T> class Tpl>
struct tpl_t<Tpl<T...> > {};

template<typename ...T>
struct tpl_t<real_tpl<T...> > {};

is erro "ambiguous template instantiation"

!!!
*/

template<typename Char, typename Traits>
struct charset_name_t_string_view_impl< basic_string_view<Char, Traits> >
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Char char_type;
	typedef typename boost::remove_cv<char_type>::type native_char_type;
	typedef basic_string<Char, Traits> string_type;
	typedef basic_string_view<Char, Traits> string_view_type;

public:
	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_default_string_charset_name<native_char_type>();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_str_default_string_charset_name<native_char_type>();
	}
};

} // namespace detail

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{
namespace swap_support
{

template<typename C, typename T > inline
void swap(basic_string_view<C, T>& l, basic_string_view<C, T>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace charset
} // namespace yggr

namespace std
{
	using ::yggr::charset::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::charset::swap_support::swap;
} // namespace boost


#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace charset
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename C, typename T > inline
std::size_t hash_value(const basic_string_view<C, T>& val)
{
	return val.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace charset
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP


#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename C, typename T>
struct hash<yggr::charset::basic_string_view<C, T> >
{
	typedef yggr::charset::basic_string_view<C, T> type;

	inline std::size_t operator()(const type& val) const 
	{
		return val.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_CHARSET_STRING_VIEW_IMPL_HPP__
