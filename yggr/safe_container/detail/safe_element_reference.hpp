//safe_element_reference

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_MODIFY_HELPER_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_MODIFY_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>
#include <yggr/any_val/any.hpp>
#include <yggr/any_val/operator_wrap.hpp>
#include <yggr/tuple/tuple.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <yggr/container/unwrap_container.hpp>
#include <yggr/container_ex/container_mapped_value_t.hpp>

#include <boost/function.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iostream>

namespace yggr
{
namespace safe_container
{
namespace detail
{

// 0 op_handler, 1 p1_handler 2 p2_handler
template<typename R, typename Container, typename Tuple3_Handler> inline
R combo_bridge(Container cont, const Tuple3_Handler& tuple_handler)
{
	return boost::get<0>(tuple_handler)(
				boost::get<1>(tuple_handler)(cont),
				boost::get<2>(tuple_handler)(cont) );
}

template<typename Container>
class basic_bridge
{
public:
	typedef Container container_type;
	typedef typename container_type::base_type base_container_type;

private:
	typedef ::yggr::container::unwrap_container<base_container_type> unwrap_type;
	typedef typename unwrap_type::type unwrap_base_container_type;

public:
	typedef typename container_mapped_value_t<unwrap_base_container_type>::type value_type;
	
private:
	typedef boost::reference_wrapper<container_type> store_type;

private:
	typedef basic_bridge this_type;


public:
	basic_bridge(container_type& cont)
		: _wrap_cont(cont)
	{
	}

	basic_bridge(const this_type& right)
		: _wrap_cont(right._wrap_cont)
	{
	}

	~basic_bridge(void)
	{
	}

public:

	inline const container_type& container(void) const
	{
		return _wrap_cont;
	}

public:
	template<typename Other_Container> inline
	bool compare_eq(const basic_bridge<Other_Container>&) const
	{
		return false;
	}

	inline bool compare_eq(const this_type& right) const
	{
		return _wrap_cont.get_pointer() == right._wrap_cont.get_pointer();
	}

public:
	template<typename Other_Container> inline
	bool link_enable(const basic_bridge<Other_Container>& other) const
	{
		return false;
	}

	inline bool link_enable(const this_type& other) const
	{
		return boost::addressof(this_type::container())
					== boost::addressof(other.container());
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		container_type& cont = _wrap_cont;
		return cont.using_handler(handler);
	}

protected:
	template<typename Op_Handler, typename Native_Handler, typename Other_Native_Handler> inline
	static
	boost::function1
	<
		typename func::foo_t_info<Op_Handler>::result_type,
		base_container_type&
	>
		pro_s_link_handler(const Op_Handler& op_handler,
							const Native_Handler& nhandler,
							const Other_Native_Handler& other_nhander)
	{
		typedef Op_Handler op_handler_type;
		typedef Native_Handler native_handler_type;
		typedef Other_Native_Handler other_native_handler_type;

		return 
			boost::bind(
				&detail::combo_bridge
				<
					typename op_handler_type::result_type,
					base_container_type&,
					yggr::tuple
					<
						op_handler_type,
						native_handler_type,
						other_native_handler_type
					>
				>,
				_1, yggr::make_tuple(op_handler, nhandler, other_nhander));
	}

protected:
	store_type _wrap_cont;
};

template<typename TL, typename TR> inline
bool operator==(const basic_bridge<TL>& l, const basic_bridge<TR>& r)
{
	return l.compare_eq(r);
}

template<typename T> inline
bool operator==(const basic_bridge<T>& l, const basic_bridge<T>& r)
{
	return l.compare_eq(r);
}

template<typename TL, typename TR> inline
bool operator!=(const basic_bridge<TL>& l, const basic_bridge<TR>& r)
{
	return !l.compare_eq(r);
}

template<typename T> inline
bool operator!=(const basic_bridge<T>& l, const basic_bridge<T>& r)
{
	return !l.compare_eq(r);
}

template<typename Bridge>
class safe_element_reference
{
public:
	typedef Bridge bridge_type;
	typedef typename bridge_type::container_type container_type;
	typedef typename bridge_type::base_container_type base_container_type;
	typedef typename bridge_type::value_type value_type;

private:
	typedef safe_element_reference this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	template<typename C, typename Val> inline
	static Val s_prv_self(C, Val val)
	{
		return val;
	}

	template<bool is_same_container_type, typename Nil = void>
	class link_helper;

	template<bool is_same_container_type, typename Nil>
	friend class link_helper;

	template<typename Nil>
	class link_helper<true, Nil>
	{
	public:
		typedef safe_element_reference outside_type;

		template<typename OBridge> inline
		const outside_type& operator()(const outside_type& self,
									const safe_element_reference<OBridge>& right) const
		{
			if(self._bridge.link_enable(right.bridge()))
			{
				self._bridge.using_handler(
					self._bridge.link(
							boost::bind(&any_val::operator_wrap::set<value_type, value_type>, _1, _2),
							right.bridge()));
			}
			else
			{

				value_type val = right; // copy it next using move
				self._bridge(
					boost::bind(
						&any_val::operator_wrap::set_move<value_type, value_type>,
						_1, boost::ref(val)));
			}
			return self;
		}

		template<typename Op_Handler, typename OBridge> inline
		typename func::foo_t_info<Op_Handler>::result_type
			operator()(const outside_type& self,
						const Op_Handler& op_handler,
						const safe_element_reference<OBridge>& other_ref) const
		{
			typedef safe_element_reference<OBridge> other_type;
			typedef typename other_type::value_type other_value_type;

			if(self._bridge.link_enable(other_ref.bridge()))
			{
				return self._bridge.using_handler(self._bridge.link(op_handler, other_ref.bridge()));
			}
			else
			{
				other_value_type val = other_ref;
				return
					self._bridge.using_handler(
							self._bridge.link_handler(
									op_handler,
									boost::bind(
										&outside_type::s_prv_self<const base_container_type&, other_value_type&>,
										_1, boost::ref(val))));
			}
		}
	};

	template<typename Nil>
	class link_helper<false, Nil>
	{
	public:
		typedef safe_element_reference outside_type;

		template<typename OBridge> inline
		const outside_type& operator()(const outside_type& self,
									const safe_element_reference<OBridge>& right) const
		{
			value_type val = right; // copy it next using move
			self._bridge(boost::bind(&any_val::operator_wrap::set_move<value_type, value_type>,
										_1, boost::ref(val)));
			return self;
		}

		template<typename Op_Handler, typename OBridge> inline
		typename func::foo_t_info<Op_Handler>::result_type
			operator()(const outside_type& self,
						const Op_Handler& op_handler,
						const safe_element_reference<OBridge>& other_ref) const
		{
			typedef safe_element_reference<OBridge> other_type;
			typedef typename other_type::value_type other_value_type;

			other_value_type val = other_ref;
			return
				self._bridge.using_handler(
					self._bridge.link_handler(
							op_handler,
							boost::bind(
								&outside_type::s_prv_self<const base_container_type&, other_value_type&>,
								_1, boost::ref(val))));
		}
	};

public:
	explicit safe_element_reference(const bridge_type& bridge)
		: _bridge(bridge)
	{
	}

	safe_element_reference(const this_type& right)
		: _bridge(right._bridge)
	{
	}

	~safe_element_reference(void)
	{
	}

public:
	inline bridge_type& bridge(void)
	{
		return _bridge;
	}

	inline const bridge_type& bridge(void) const
	{
		return _bridge;
	}

	template<typename OBridge> inline
	bool is_same(const safe_element_reference<OBridge>& right) const
	{
		return _bridge == right.bridge();
	}

	// element access

	// T = elem
	template<typename T> inline
	operator T(void) const
	{
		return T(_bridge());
	}

	// value = emlem
	inline operator value_type(void) const
	{
		return _bridge();
	}

public:
	// don't using move, because the data will be invalid after being moved
	// bridge = other_bridge
	template<typename OBridge> inline
	const this_type& operator=(const safe_element_reference<OBridge>& right) const
	{
		typedef safe_element_reference<OBridge> other_type;
		typedef typename other_type::container_type other_container_type;

		typedef
			link_helper
			<
				boost::is_same
				<
					container_type,
					other_container_type
				>::value
			> helper_type;

		helper_type h;
		return h(*this, right);
	}

	// bridge = bridge
	inline const this_type& operator=(const this_type& right) const
	{
		typedef link_helper<true> helper_type;

		if(this == &right)
		{
			return *this;
		}

		helper_type h;
		return h(*this, right);
	}

	// bridge = T
	template<typename T> inline
	const this_type& operator=(const T& t) const
	{
		value_type val = t;
		_bridge(boost::bind(&any_val::operator_wrap::set_move<value_type, value_type>,
								_1, boost::ref(val)));
		return *this;
	}

	// value_type
	inline const this_type& operator=(BOOST_RV_REF(value_type) t) const
	{
		value_type& val_ref = t;
		_bridge(boost::bind(&any_val::operator_wrap::set_move<value_type, value_type>,
								_1, boost::ref(val_ref)));
		return *this;
	}

	inline const this_type& operator=(const value_type& t) const
	{
		_bridge(boost::bind(&any_val::operator_wrap::set<value_type, value_type>,
								_1, boost::cref(t)));
		return *this;
	}

public:
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(const Handler& handler) const
	{
		return _bridge(handler);
	}

	template<typename Op_Handler, typename OBridge> inline
	typename func::foo_t_info<Op_Handler>::result_type
		operator()(const Op_Handler& op_handler,
						const safe_element_reference<OBridge>& other_ref) const
	{
		typedef safe_element_reference<OBridge> other_type;
		typedef typename other_type::container_type other_container_type;

		typedef
			link_helper
			<
				boost::is_same
				<
					container_type,
					other_container_type
				>::value
			> helper_type;

		helper_type h;
		return h(*this, op_handler, other_ref);
	}

private:
	bridge_type _bridge;
};

template<typename B1> inline
typename safe_element_reference<B1>::value_type
	operator+(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::positive_custom<value_type, value_type>, _1));
}

template<typename B1> inline
typename safe_element_reference<B1>::value_type
	operator-(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::negate_custom<value_type, value_type>, _1)) ;
}

template<typename B1> inline
typename
	mplex::pointer_to_value_t
	<
		typename safe_element_reference<B1>::value_type
	>::type
	operator*(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::dereference_readonly<value_type>, _1)) ;
}

template<typename B1> inline
const safe_element_reference<B1>&
	operator++(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	l(boost::bind(&any_val::operator_wrap::head_increment<value_type>, _1));
	return l;
}

template<typename B1> inline
typename safe_element_reference<B1>::value_type
	operator++(const safe_element_reference<B1>& l, int)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::tail_increment<value_type>, _1));
}

template<typename B1> inline
const safe_element_reference<B1>&
	operator--(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	l(boost::bind(&any_val::operator_wrap::head_decrement<value_type>, _1));
	return l;
}

template<typename B1> inline
typename safe_element_reference<B1>::value_type
	operator--(const safe_element_reference<B1>& l, int)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::tail_decrement<value_type>, _1));
}

template<typename B1, typename B2> inline
typename
	plus_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator+(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::plus<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	plus_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator+(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::plus<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	minus_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator+(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::plus<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator+=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::plus_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator+=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::plus_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator+=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::plus_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	minus_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator-(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::minus<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	minus_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator-(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::minus<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	minus_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator-(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::minus<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator-=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::minus_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator-=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::minus_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator-=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::minus_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	multiplies_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator*(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::multiplies<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	multiplies_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator*(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::multiplies<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	multiplies_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator*(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::multiplies<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator*=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::multiplies_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator*=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::multiplies_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator*=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::multiplies_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	divides_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator/(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::divides<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	multiplies_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator/(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::divides<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	multiplies_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator/(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::divides<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator/=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::divides_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator/=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::divides_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator/=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::divides_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	modulus_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator%(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::modulus<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	modulus_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator%(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::modulus<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	modulus_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator%(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::modulus<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator%=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::modulus_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator%=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::modulus_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator%=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::modulus_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
bool operator&&(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::logic_and<lval_type, rval_type>, _1, _2), r);
}

template<typename B1, typename B2> inline
bool operator||(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::logic_or<lval_type, rval_type>, _1, _2), r);
}

template<typename B1> inline
bool operator!(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::logic_not<value_type>, _1));
}

template<typename B1, typename B2> inline
typename
	bitwise_and_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator&(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::bitwise_and<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	bitwise_and_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator&(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_and<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	bitwise_and_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator&(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::bitwise_and<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator&=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_and_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator&=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_and_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator&=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_and_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	bitwise_or_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator|(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::bitwise_or<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	bitwise_or_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator|(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_or<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	bitwise_or_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator|(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::bitwise_or<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator|=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_or_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator|=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_or_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator|=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_or_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	bitwise_xor_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator^(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::bitwise_xor<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	bitwise_xor_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator^(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_xor<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	bitwise_xor_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator^(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::bitwise_xor<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator^=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_xor_set<lval_type, rval_type>, _1, _2), r);
	return l;
}


template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator^=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_xor_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator^=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_xor_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	bitwise_left_shift_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator<<(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::bitwise_left_shift<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	bitwise_left_shift_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator<<(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_left_shift<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	bitwise_left_shift_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator<<(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::bitwise_left_shift<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))


template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator<<=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_left_shift_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator<<=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_left_shift_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator<<=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_left_shift_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
typename
	bitwise_right_shift_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		typename safe_element_reference<B2>::value_type
	>::type
	operator>>(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l(boost::bind(&any_val::operator_wrap::bitwise_right_shift<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
typename
	bitwise_right_shift_operator_result
	<
		T,
		typename safe_element_reference<B2>::value_type
	>::type
	operator>>(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_right_shift<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
typename
	bitwise_right_shift_operator_result
	<
		typename safe_element_reference<B1>::value_type,
		T
	>::type
	operator>>(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::bitwise_right_shift<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
const safe_element_reference<B1>&
	operator>>=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_right_shift_set<lval_type, rval_type>, _1, _2), r);
	return l;
}

template<typename B1, typename T> inline
const safe_element_reference<B1>&
	operator>>=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	l(boost::bind(&any_val::operator_wrap::bitwise_right_shift_set<lval_type, rval_type>, _1, boost::cref(r)));
	return l;
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
T& operator>>=(T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::bitwise_right_shift_set<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1> inline
typename safe_element_reference<B1>::value_type
	operator~(const safe_element_reference<B1>& l)
{
	typedef typename safe_element_reference<B1>::value_type value_type;
	return l(boost::bind(&any_val::operator_wrap::bitwise_not<value_type>, _1));
}

template<typename B1, typename B2> inline
bool operator==(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l.is_same(r)
			|| l(boost::bind(&any_val::operator_wrap::equal_to<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
bool operator==(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::equal_to<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
bool operator==(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::equal_to<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
bool operator!=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	return !operator==(l, r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
bool operator!=(const T& l, const safe_element_reference<B2>& r)
{
	return !operator==(l, r);
}

template<typename B1, typename T> inline
bool operator!=(const safe_element_reference<B1>& l, const T& r)
{
	return !operator==(l, r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
bool operator<(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return (!l.is_same(r))
			&& l(boost::bind(&any_val::operator_wrap::less<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
bool operator<(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::less<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
bool operator<(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::less<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
bool operator<=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l.is_same(r)
			|| l(boost::bind(&any_val::operator_wrap::less_equal<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
bool operator<=(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::less_equal<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
bool operator<=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::less_equal<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
bool operator>(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return (!l.is_same(r))
			&& l(boost::bind(&any_val::operator_wrap::greater<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
bool operator>(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::greater<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
bool operator>(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::greater<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

template<typename B1, typename B2> inline
bool operator>=(const safe_element_reference<B1>& l,
				const safe_element_reference<B2>& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return l.is_same(r)
			|| l(boost::bind(&any_val::operator_wrap::greater_equal<lval_type, rval_type>, _1, _2), r);
}

#if (defined(__clang__) && (__clang__))

template<typename T, typename B2> inline
bool operator>=(const T& l, const safe_element_reference<B2>& r)
{
	typedef T lval_type;
	typedef typename safe_element_reference<B2>::value_type rval_type;

	return any_val::operator_wrap::greater_equal<lval_type, rval_type>(l, static_cast<rval_type>(r));
}

template<typename B1, typename T> inline
bool operator>=(const safe_element_reference<B1>& l, const T& r)
{
	typedef typename safe_element_reference<B1>::value_type lval_type;
	typedef T rval_type;

	return any_val::operator_wrap::greater_equal<lval_type, rval_type>(static_cast<lval_type>(l), r);
}

#endif // #if (defined(__clang__) && (__clang__))

// io
template<typename Char, typename Traits, typename B>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const safe_element_reference<B>& val)
{
	typedef typename safe_element_reference<B>::value_type val_type;
	os << static_cast<val_type>(val);
	return os;
}


} // namespace detail

using detail::safe_element_reference;

//template<typename Op_Handler, typename B1> inline
//typename func::foo_t_info<Op_Handler>::result_type
//	safe_element_reference_unary_operator(const Op_Handler& op_handler,
//											safe_element_reference<B1>& l )
//{
//	return l(op_handler);
//}

template<typename Op_Handler, typename B1> inline
typename func::foo_t_info<Op_Handler>::result_type
	safe_element_reference_unary_operator(const Op_Handler& op_handler,
											const safe_element_reference<B1>& l )
{
	return l(op_handler);
}

//template<typename Op_Handler, typename B1, typename B2> inline
//typename func::foo_t_info<Op_Handler>::result_type
//	safe_element_reference_binary_operator(const Op_Handler& op_handler,
//											safe_element_reference<B1>& l,
//											const safe_element_reference<B2>& r )
//{
//	return l(op_handler, r);
//}

template<typename Op_Handler, typename B1, typename B2> inline
typename func::foo_t_info<Op_Handler>::result_type
	safe_element_reference_binary_operator(const Op_Handler& op_handler,
											const safe_element_reference<B1>& l,
											const safe_element_reference<B2>& r )
{
	return l(op_handler, r);
}

} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_MODIFY_HELPER_HPP__
