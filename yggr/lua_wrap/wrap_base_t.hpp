//wrap_base_t.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_BASE_T_HPP__
#define __YGGR_LUA_WRAP_WRAP_BASE_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/type_traits/half_types.hpp>
#include <yggr/type_traits/operator_result.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/math/modulus.hpp>
#include <yggr/support/operator_t.hpp>

#include <yggr/lua_wrap/wrap_base_t_maker.hpp>
#include <yggr/lua_wrap/detail/unwrap_t.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/mpl/bool.hpp>

#include <ostream>
#include <sstream>

namespace yggr
{
namespace lua_wrap
{

template<typename Val>
class wrap_base_t;

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename T>
struct is_wrap_base_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_wrap_base_t< wrap_base_t<T> >
	: public boost::mpl::true_
{
};

} // namespace detail
} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{

template<typename Val>
class wrap_base_t
{
public:
	typedef Val value_type;
	typedef value_type base_type;

public:
	typedef typename half_t<value_type>::type half_type;

private:
	BOOST_MPL_ASSERT((boost::is_integral<value_type>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(value_type) == 8)>));
private:
	YGGR_STATIC_CONSTANT(u32, half_bit_size = (sizeof(half_type) << 3)); // sizeof(half_type) * 8

private:
	template<typename OT>
	friend class wrap_base_t;

private:
	typedef wrap_base_t this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	wrap_base_t(void)
		: _val()
	{
	}

	wrap_base_t(value_type val)
		: _val(val)
	{
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class BasicString>
	wrap_base_t(const BasicString<Char, Traits, Alloc>& val)
	{
		std::stringstream ss;
		ss << val;
		ss >> _val;
	}

	template<typename Char>
	wrap_base_t(const Char* val)
	{
		std::stringstream ss;
		ss << val;
		ss >> _val;
	}

	wrap_base_t(const std::string& val)
	{
		std::stringstream ss;
		ss << val;
		ss >> _val;
	}

	wrap_base_t(half_type h, half_type l)
	{
		value_type valh = h;
		value_type vall = l;
		_val = (valh << half_bit_size) | vall;
	}

	template<typename T>
	wrap_base_t(const wrap_base_t<T>& right)
		: _val(right._val)
	{
	}

	wrap_base_t(BOOST_RV_REF(this_type) right)
		: _val(boost::move(right._val))
	{
	}

	wrap_base_t(const this_type& right)
		: _val(right._val)
	{
	}

	~wrap_base_t(void)
	{
	}

public:
	template<typename T> inline
	this_type& operator=(const T& right)
	{
		_val = right;
		return *this;
	}

	template<typename T> inline
	this_type& operator=(const wrap_base_t<T>& right)
	{
		_val = right._val;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}
		_val = right._val;
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}
		_val = right._val;
		return *this;
	}

public:
	inline void swap(value_type& right)
	{
		::yggr::swap(_val, right);
	}

	template<typename T> inline
	void swap(wrap_base_t<T>& right)
	{
		typedef T right_value_type;

		value_type tmp = _val;
		_val = static_cast<right_value_type>(right._val);
		right._val = static_cast<right_value_type>(tmp);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(_val, right._val);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE value_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const value_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE operator bool(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!_val;
	}

protected:
	typedef typename boost::make_signed<value_type>::type signed_value_type;
	typedef typename boost::make_unsigned<value_type>::type unsigned_value_type;

public:
	YGGR_CONSTEXPR_OR_INLINE operator signed_value_type(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

	YGGR_CONSTEXPR_OR_INLINE operator unsigned_value_type(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

	//// dont't using operator T, luabind not supported
	//template<typename T> inline
	//operator T(void) const
	//{
	//	return T(_val);
	//}

	YGGR_CXX14_CONSTEXPR_OR_INLINE value_type& value(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE value_type value(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		set_value(T right)
	{
		_val = right;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		set_value(const T& right)
	{
		_val = right._val;
		return *this;
	}

	this_type& set_value(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}
		_val = right._val;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE value_type get_value(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _val;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool to_boolean(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!_val;
	}

	inline this_type copy(void) const
	{
		return *this;
	}

public:
	// math
	inline const this_type& positive(void) const
	{
		return *this;
	}

	inline this_type negate(void) const
	{
		return this_type(-_val);
	}

	inline this_type& head_increment(void)
	{
		_val += value_type(1);
		return *this;
	}

	inline this_type tail_increment(void)
	{
		this_type ret(_val);
		_val += value_type(1);
		return ret;
	}

	inline this_type& head_decrement(void)
	{
		_val -= value_type(1);
		return *this;
	}

	inline this_type tail_decrement(void)
	{
		this_type ret(_val);
		_val -= value_type(1);
		return ret;
	}

	// wrap_base_t op any, don't using operator_result make new result_type

	// don't using this foo type, because export lua conflict
	//template<typename T> inline
	//this_type plus(const T& right) const
	//{
	//	return this_type(_val + right);
	//}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		plus(T right) const
	{
		return this_type(_val + right);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename wrap_base_t_maker<typename plus_operator_result<value_type, typename T::value_type>::type >::type
		>::type
		plus(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename plus_operator_result<value_type, typename T::value_type>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type(_val + right._val);
	}

	inline this_type plus(const this_type& right) const
	{
		return this_type(_val + right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		plus_set(T right)
	{
		_val += right;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		plus_set(const T& right)
	{
		_val += right._val;
		return *this;
	}

	inline this_type& plus_set(const this_type& right)
	{
		_val += right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		minus(T right) const
	{
		return this_type(_val - right);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename
				wrap_base_t_maker
				<
					typename minus_operator_result<value_type, typename T::value_type>::type
				>::type
		>::type
		minus(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename minus_operator_result<value_type, typename T::value_type>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type(_val - right._val);
	}

	this_type minus(const this_type& right) const
	{
		return this_type(_val - right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		minus_set(T right)
	{
		_val -= right;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		minus_set(const T& right)
	{
		_val -= right._val;
		return *this;
	}

	inline this_type& minus_set(const this_type& right)
	{
		_val -= right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		multiplies(T right) const
	{
		return this_type(_val * right);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename
				wrap_base_t_maker
				<
					typename
						multiplies_operator_result
						<
							value_type,
							typename T::value_type
						>::type
				>::type
		>::type
		multiplies(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename
					multiplies_operator_result
					<
						value_type,
						typename T::value_type
					>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type(_val * right._val);
	}

	inline this_type multiplies(const this_type& right) const
	{
		return this_type(_val * right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		multiplies_set(T right)
	{
		_val *= right;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		multiplies_set(const T& right)
	{
		_val *= right._val;
		return *this;
	}

	inline this_type& multiplies_set(const this_type& right)
	{
		_val *= right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		divides(T right) const
	{
		return this_type(_val / right);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename
				wrap_base_t_maker
				<
					typename divides_operator_result<value_type, typename T::value_type>::type
				>::type
		>::type
		divides(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename divides_operator_result<value_type, typename T::value_type>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type(_val / right._val);
	}

	inline this_type divides(const this_type& right) const
	{
		return this_type(_val / right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		divides_set(T right)
	{
		_val /= right;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		divides_set(const T& right)
	{
		_val /= right._val;
		return *this;
	}

	inline this_type& divides_set(const this_type& right)
	{
		_val /= right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		modulus(T right) const
	{
		return this_type(math::modulus(_val, right));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename
				wrap_base_t_maker
				<
					typename modulus_operator_result<value_type, typename T::value_type>::type
				>::type
		>::type
		modulus(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename modulus_operator_result<value_type, typename T::value_type>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type(math::modulus(_val, right._val));
	}

	inline this_type modulus(const this_type& right) const
	{
		return this_type(math::modulus(_val, right._val));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		modulus_set(T right)
	{
		math::modulus_set(_val, right);
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		modulus_set(const T& right)
	{
		math::modulus_set(_val, right._val);
		return *this;
	}

	inline this_type& modulus_set(const this_type& right)
	{
		math::modulus_set(_val, right._val);
		return *this;
	}

	// logic
	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			bool
		>::type
		logic_and(T right) const
	{
		return _val && right;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			bool
		>::type
		logic_and(const T& right) const
	{
		return _val && right._val;
	}

	inline bool logic_and(const this_type& right) const
	{
		return _val && right._val;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			bool
		>::type
		logic_or(T right) const
	{
		return _val || right;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			bool
		>::type
		logic_or(const T& right) const
	{
		return _val || right._val;
	}

	inline bool logic_or(const this_type& right) const
	{
		return _val || right._val;
	}

	inline bool logic_not(void) const
	{
		return !_val;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			bool
		>::type
		compare_eq(T right) const
	{
		return _val == right;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			bool
		>::type
		compare_eq(const T& right) const
	{
		return _val == right._val;
	}

	inline bool compare_eq(const this_type& right) const
	{
		return _val == right._val;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			s32
		>::type
		compare(T right) const
	{
		return (_val == right)? 0 : _val < right? -1 : 1;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			s32
		>::type
		compare(const T& right) const
	{
		return (_val == right._val)? 0 : _val < right._val? -1 : 1;
	}

	inline s32 compare(const this_type& right) const
	{
		return (_val == right._val)? 0 : _val < right._val? -1 : 1;
	}

	// bitwise
	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		bitwise_left_shift(T right) const
	{
		value_type val = (_val << static_cast<value_type>(right));
		return this_type(val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename
				wrap_base_t_maker
				<
					typename
						bitwise_left_shift_operator_result
						<
							value_type,
							typename T::value_type
						>::type
				>::type
		>::type
		bitwise_left_shift(const T& right) const
	{
		typedef typename
			bitwise_left_shift_operator_result
			<
				value_type,
				typename T::value_type
			>::type ret_val_type;
		typedef typename wrap_base_t_maker<ret_val_type>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		ret_val_type val = (_val << right._val);
		return ret_type(val);
	}

	inline this_type bitwise_left_shift(const this_type& right) const
	{
		return this_type((_val << right._val));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		bitwise_left_shift_set(T right)
	{
		_val <<= static_cast<value_type>(right);
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		bitwise_left_shift_set(const T& right)
	{
		_val <<= right._val;
		return *this;
	}

	inline this_type& bitwise_left_shift_set(const this_type& right)
	{
		_val <<= right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		bitwise_right_shift(T right) const
	{
		value_type val = _val >> static_cast<value_type>(right);
		return this_type(val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			typename
				wrap_base_t_maker
				<
					typename
						bitwise_right_shift_operator_result
						<
							value_type,
							typename T::value_type
						>::type
				>::type
		>::type
		bitwise_right_shift(const T& right) const
	{
		typedef typename
			bitwise_right_shift_operator_result
			<
				value_type,
				typename T::value_type
			>::type ret_val_type;
		typedef typename wrap_base_t_maker<ret_val_type>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		value_type val = _val >> right._val;
		return ret_type(val);
	}

	inline this_type bitwise_right_shift(const this_type& right) const
	{
		return this_type((_val >> right));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		bitwise_right_shift_set(T right)
	{
		_val >>= static_cast<value_type>(right);
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		bitwise_right_shift_set(const T& right)
	{
		_val >>= right._val;
		return *this;
	}

	inline this_type& bitwise_right_shift_set(const this_type& right)
	{
		_val >>= right._val;
		return *this;
	}

	inline this_type bitwise_not(void) const
	{
		return this_type(~_val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		bitwise_and(T right) const
	{
		return this_type((_val & static_cast<value_type>(right)));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			wrap_base_t< typename bitwise_and_operator_result<value_type, typename T::value_type>::type >
		>::type
		bitwise_and(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename bitwise_and_operator_result<value_type, typename T::value_type>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type((_val & right._val));
	}

	inline this_type bitwise_and(const this_type& right) const
	{
		return this_type(_val & right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		bitwise_and_set(T right)
	{
		_val &= static_cast<value_type>(right);
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		bitwise_and_set(const T& right)
	{
		_val &= right._val;
		return *this;
	}

	inline this_type& bitwise_and_set(const this_type& right)
	{
		_val &= right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		bitwise_or(T right) const
	{
		return this_type((_val | static_cast<value_type>(right)));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			wrap_base_t< typename bitwise_or_operator_result<value_type, typename T::value_type>::type >
		>::type
		bitwise_or(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename bitwise_or_operator_result<value_type, typename T::value_type>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type((_val | right._val));
	}

	inline this_type bitwise_or(const this_type& right) const
	{
		return this_type(_val | right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		bitwise_or_set(T right)
	{
		_val |= static_cast<value_type>(right);
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		bitwise_or_set(const T& right)
	{
		_val |= right._val;
		return *this;
	}

	inline this_type& bitwise_or_set(const this_type& right)
	{
		_val |= right._val;
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type
		>::type
		bitwise_xor(T right) const
	{
		return this_type((_val ^ static_cast<value_type>(right)));
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			wrap_base_t< typename bitwise_xor_operator_result<value_type, typename T::value_type>::type >
		>::type
		bitwise_xor(const T& right) const
	{
		typedef typename
			wrap_base_t_maker
			<
				typename bitwise_xor_operator_result<value_type, T>::type
			>::type ret_type;

		BOOST_MPL_ASSERT((detail::is_wrap_base_t<ret_type>));

		return ret_type((_val ^ right._val));
	}

	inline this_type bitwise_xor(const this_type& right) const
	{
		return this_type(_val ^ right._val);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			this_type&
		>::type
		bitwise_xor_set(T right)
	{
		_val ^= static_cast<value_type>(right);
		return *this;
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			detail::is_wrap_base_t<T>,
			this_type&
		>::type
		bitwise_xor_set(const T& right)
	{
		_val ^= right._val;
		return *this;
	}

	inline this_type& bitwise_xor_set(const this_type& right)
	{
		_val ^= right._val;
		return *this;
	}

private:
	value_type _val;
};

// positive
template<typename L> inline
const wrap_base_t<L>& operator+(const wrap_base_t<L>& l)
{
	return l.positive();
}

// negate
template<typename L> inline
wrap_base_t<L> operator-(const wrap_base_t<L>& l)
{
	return l.negate();
}

// plus_plus
template<typename L> inline
wrap_base_t<L>& operator++(wrap_base_t<L>& l)
{
	return l.head_increment();
}

template<typename L> inline
wrap_base_t<L>& operator++(wrap_base_t<L>& l, int)
{
	return l.tail_increment();
}

// minus_minus
template<typename L> inline
wrap_base_t<L>& operator--(wrap_base_t<L>& l)
{
	return l.head_decrement();
}

template<typename L> inline
wrap_base_t<L>& operator--(wrap_base_t<L>& l, int)
{
	return l.tail_decrement();
}

// plus
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename plus_operator_result<L, R>::type
	>::type
	operator+(L l, const wrap_base_t<R>& r)
{
	return l + static_cast<R>(r.value());
}

// lua number is double, so redefined wrap_base_t<L> * R -> wrap_base_t<L>
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator+(const wrap_base_t<L>& l, R r)
{
	return l.plus(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename plus_operator_result<L, R>::type >::type
	operator+(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.plus(r);
}

// plus_set
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator+=(L& l, const wrap_base_t<R>& r)
{
	return l += r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator+=(wrap_base_t<L>& l, R r)
{
	return l.plus_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator+=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.plus_set(r);
}

// minus
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename minus_operator_result<L, R>::type
	>::type
	operator-(L l, const wrap_base_t<R>& r)
{
	return l - r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator-(const wrap_base_t<L>& l, R r)
{
	return l.minus(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename minus_operator_result<L, R>::type >::type
	operator-(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.minus(r);
}

// minus_set
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator-=(L& l, const wrap_base_t<R>& r)
{
	return l -= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator-=(wrap_base_t<L>& l, R r)
{
	return l.minus_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator-=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.minus_set(r);
}

// multiplies
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename multiplies_operator_result<L, R>::type
	>::type
	operator*(L l, const wrap_base_t<R>& r)
{
	return l * r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator*(const wrap_base_t<L>& l, R r)
{
	return l.multiplies(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename multiplies_operator_result<L, R>::type >::type
	operator*(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.multiplies(r);
}

// multiplies_set
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator*=(L& l, const wrap_base_t<R>& r)
{
	return l *= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator*=(wrap_base_t<L>& l, R r)
{
	return l.multiplies_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator*=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.multiplies_set(r);
}

// divides
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename divides_operator_result<L, R>::type
	>::type
	operator/(L l, const wrap_base_t<R>& r)
{
	return l / r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator/(const wrap_base_t<L>& l, R r)
{
	return l.divides(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename divides_operator_result<L, R>::type >::type
	operator/(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.divides(r);
}

// divides_set
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator/=(L& l, const wrap_base_t<R>& r)
{
	return l /= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator/=(wrap_base_t<L>& l, R r)
{
	return l.divides_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator/=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.divides_set(r);
}

// modulus
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename modulus_operator_result<L, R>::type
	>::type
	operator%(L l, const wrap_base_t<R>& r)
{
	return math::modulus(l, r.value());
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator%(const wrap_base_t<L>& l, R r)
{
	return l.modulus(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename modulus_operator_result<L, R>::type >::type
	operator%(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.modulus(r);
}

// modulus_set
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator%=(L& l, const wrap_base_t<R>& r)
{
	return math::modulus_set(l, r.value());
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator%=(wrap_base_t<L>& l, R r)
{
	return l.modulus_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator%=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.modulus_set(r);
}

// logic and or not using operator bool

// compare
// ==
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		bool
	>::type
	operator==(L l, const wrap_base_t<R>& r)
{
	return r.compare_eq(l);
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		bool
	>::type
	operator==(const wrap_base_t<L>& l, R r)
{
	return l.compare_eq(r);
}

template<typename L, typename R> inline
bool operator==(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.compare_eq(r);
}

// !=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		bool
	>::type
	operator!=(L l, const wrap_base_t<R>& r)
{
	return !r.compare_eq(l);
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		bool
	>::type
	operator!=(const wrap_base_t<L>& l, R r)
{
	return !l.compare_eq(r);
}

template<typename L, typename R> inline
bool operator!=(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return !l.compare_eq(r);
}

// <
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		bool
	>::type
	operator<(L l, const wrap_base_t<R>& r)
{
	return 0 < r.compare(l);
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		bool
	>::type
	operator<(const wrap_base_t<L>& l, R r)
{
	return l.compare(r) < 0;
}

template<typename L, typename R> inline
bool operator<(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.compare(r) < 0;
}

// <=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		bool
	>::type
	operator<=(L l, const wrap_base_t<R>& r)
{
	return r.compare(l) >= 0;
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		bool
	>::type
	operator<=(const wrap_base_t<L>& l, R r)
{
	return l.compare(r) <= 0;
}

template<typename L, typename R> inline
bool operator<=(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.compare(r) <= 0;
}

// >
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		bool
	>::type
	operator>(L l, const wrap_base_t<R>& r)
{
	return r.compare(l) < 0;
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		bool
	>::type
	operator>(const wrap_base_t<L>& l, R r)
{
	return l.compare(r) > 0;
}

template<typename L, typename R> inline
bool operator>(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.compare(r) > 0;
}

// >=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		bool
	>::type
	operator>=(L l, const wrap_base_t<R>& r)
{
	return r.compare(l) <= 0;
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		bool
	>::type
	operator>=(const wrap_base_t<L>& l, R r)
{
	return l.compare(r) >= 0;
}

template<typename L, typename R> inline
bool operator>=(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.compare(r) >= 0;
}

// <<
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename bitwise_left_shift_operator_result<L, R>::type
	>::type
	operator<<(L l, const wrap_base_t<R>& r)
{
	return l << r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator<<(const wrap_base_t<L>& l, R r)
{
	return l.bitwise_left_shift(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename bitwise_left_shift_operator_result<L, R>::type >::type
	operator<<(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_left_shift(r);
}

// <<=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator<<=(L& l, const wrap_base_t<R>& r)
{
	return l <<= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator<<=(wrap_base_t<L>& l, R r)
{
	return l.bitwise_left_shift_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator<<=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_left_shift_set(r);
}

// >>
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename bitwise_right_shift_operator_result<L, R>::type
	>::type
	operator>>(L l, const wrap_base_t<R>& r)
{
	return l >> r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator>>(const wrap_base_t<L>& l, R r)
{
	return l.bitwise_right_shift(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename bitwise_right_shift_operator_result<L, R>::type >::type
	operator>>(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_right_shift(r);
}

// >>=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator>>=(L& l, const wrap_base_t<R>& r)
{
	return l >>= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator>>=(wrap_base_t<L>& l, R r)
{
	return l.bitwise_right_shift_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator>>=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_right_shift_set(r);
}

// ~
template<typename L> inline
wrap_base_t<L> operator~(const wrap_base_t<L>& l)
{
	return l.bitwise_not();
}

// &
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename bitwise_and_operator_result<L, R>::type
	>::type
	operator&(L l, const wrap_base_t<R>& r)
{
	return l & r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator&(const wrap_base_t<L>& l, R r)
{
	return l.bitwise_and(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename bitwise_and_operator_result<L, R>::type >::type
	operator&(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_and(r);
}

// &=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator&=(L& l, const wrap_base_t<R>& r)
{
	return l &= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator&=(wrap_base_t<L>& l, R r)
{
	return l.bitwise_and_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator&=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_and_set(r);
}

// |
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename bitwise_or_operator_result<L, R>::type
	>::type
	operator|(L l, const wrap_base_t<R>& r)
{
	return l | r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator|(const wrap_base_t<L>& l, R r)
{
	return l.bitwise_or(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename bitwise_or_operator_result<L, R>::type >::type
	operator|(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_or(r);
}

// |=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator|=(L& l, const wrap_base_t<R>& r)
{
	return l |= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator|=(wrap_base_t<L>& l, R r)
{
	return l.bitwise_or_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator|=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_or_set(r);
}

// ^
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		typename bitwise_xor_operator_result<L, R>::type
	>::type
	operator^(L l, const wrap_base_t<R>& r)
{
	return l ^ r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>
	>::type
	operator^(const wrap_base_t<L>& l, R r)
{
	return l.bitwise_xor(r);
}

template<typename L, typename R> inline
typename wrap_base_t_maker<typename bitwise_xor_operator_result<L, R>::type >::type
	operator^(const wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_xor(r);
}

// ^=
template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		L&
	>::type
	operator^=(L& l, const wrap_base_t<R>& r)
{
	return l ^= r.value();
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		wrap_base_t<L>&
	>::type
	operator^=(wrap_base_t<L>& l, R r)
{
	return l.bitwise_xor_set(r);
}

template<typename L, typename R> inline
wrap_base_t<L>& operator^=(wrap_base_t<L>& l, const wrap_base_t<R>& r)
{
	return l.bitwise_xor_set(r);
}


} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{

template<typename Char, typename Traits, typename T>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::lua_wrap::wrap_base_t<T>& v)
{
	typedef T t_value_type;
	typedef yggr::lua_wrap::wrap_base_t<t_value_type> wrap_type;
	typedef typename wrap_type::value_type value_type;
	const value_type& val = v;
	os << val;
	return os;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits, typename T,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os, const yggr::lua_wrap::wrap_base_t<T>& v);

// fix binary << conflict
template<typename Char, typename Traits, typename Alloc, typename T>
std::basic_stringstream<Char, Traits, Alloc>&
	operator<<(std::basic_stringstream<Char, Traits, Alloc>& os,
				const yggr::lua_wrap::wrap_base_t<T>& v)
{
	typedef T t_value_type;
	typedef yggr::lua_wrap::wrap_base_t<t_value_type> wrap_type;
	typedef typename wrap_type::value_type value_type;
	const value_type& val = v;
	os << val;
	return os;
}

template<typename Char, typename Traits, typename T>
std::basic_fstream<Char, Traits>&
	operator<<(std::basic_fstream<Char, Traits>& os,
				const yggr::lua_wrap::wrap_base_t<T>& v)
{
	typedef T t_value_type;
	typedef yggr::lua_wrap::wrap_base_t<t_value_type> wrap_type;
	typedef typename wrap_type::value_type value_type;
	const value_type& val = v;
	os << val;
	return os;
}

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace swap_support
{

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<L>,
		void
	>::type
	swap(L& l, yggr::lua_wrap::wrap_base_t<R>& r)
{
	r.swap(l);
}

template<typename L, typename R> inline
typename
	boost::enable_if
	<
		boost::is_arithmetic<R>,
		void
	>::type
	swap(yggr::lua_wrap::wrap_base_t<L>& l, R& r)
{
	l.swap(r);
}

template<typename L, typename R> inline
void swap(yggr::lua_wrap::wrap_base_t<L>& l,
			yggr::lua_wrap::wrap_base_t<R>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace lua_wrap
} // namespace yggr

namespace std
{
	using ::yggr::lua_wrap::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::lua_wrap::swap_support::swap;
} // namespace boost

#endif //__YGGR_LUA_WRAP_WRAP_BASE_T_HPP__
