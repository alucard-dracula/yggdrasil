// geo_bezier_curve_t.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_BEZIER_CURVE_T_HPP__
#define __YGGR_GEOMETRY_GEO_BEZIER_CURVE_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/reference_to_value_t.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/geometry/detail/bezier_curvel.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/ref.hpp>

#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <cassert>

namespace yggr
{
namespace geometry
{

template<typename P, u32 N,
			template<typename _P, std::size_t> class Array>
class geo_bezier_curve;

} // namespace geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{
namespace detail
{

template<typename PointArray, std::size_t I, std::size_t ISize>
struct geo_bezier_curve_init_helper_t
	: public geo_bezier_curve_init_helper_t<PointArray, I + 1, ISize>
{
public:
	typedef PointArray point_array_type;
	typedef typename range_ex::range_value_ex<point_array_type>::type point_type;
	typedef geo_bezier_curve_init_helper_t<point_array_type, I + 1, ISize> base_type;
	typedef boost::mpl::size_t<I> index_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_< (I < ISize) >));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(index_type::value < ISize)>));

private:
	typedef geo_bezier_curve_init_helper_t this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename T, typename ...Args> inline
	void init(point_array_type& arr, BOOST_FWD_REF(T) val, BOOST_FWD_REF(Args)... args) const
	{
		base_type::init(arr, boost::forward<Args>(args)...);
		this_type::prv_init_detail(arr, boost::forward<T>(val));
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename T \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	void init( point_array_type& arr, BOOST_FWD_REF(T) val \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		base_type::init(arr YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
		this_type::prv_init_detail(arr, boost::forward<T>(val)); } 

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	inline void prv_init_detail(point_array_type& arr, BOOST_RV_REF(point_type) val) const
	{
		arr[index_type::value] = boost::move(val);
	}

	template<typename T> inline
	void prv_init_detail(point_array_type& arr, const T& val) const
	{
		arr[index_type::value] = val;
	}
};

template<typename PointArray, std::size_t ISize>
struct geo_bezier_curve_init_helper_t<PointArray, ISize, ISize>
{
	typedef PointArray point_array_type;

	inline void init(point_array_type& arr) const {}
};

} // namespace detail
} // namespace geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{

template<typename P, u32 N,
			template<typename _P, std::size_t> class Array = yggr::array>
class geo_bezier_curve
{
public:
	typedef boost::mpl::integral_c<u32, N> order_type;
	YGGR_STATIC_CONSTANT(u32, E_order = static_cast<yggr::u32>(order_type::value));

	typedef boost::mpl::size_t<static_cast<std::size_t>(N + 1)> point_size_type;
	YGGR_STATIC_CONSTANT(std::size_t, E_point_size = point_size_type::value);

#ifdef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	BOOST_MPL_ASSERT((boost::mpl::bool_<(point_size_type::value < YGGR_PP_FOO_DEFAULT_PARAMS_LEN )>));
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	typedef P pos_type;
	typedef pos_type point_type;
	typedef pos_type value_type;

public:
	typedef typename point_type::value_type pos_value_type;
	typedef pos_value_type point_value_type; 

public:
	typedef typename
		boost::mpl::if_
		<
			boost::mpl::bool_<!!order_type::value>,
			typename upper_float<point_type>::type,
			point_type
		>::type result_point_type;

	typedef yggr::array<point_type, point_size_type::value> point_array_type;
	typedef point_array_type base_type;

private:
	typedef geo_bezier_curve this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	geo_bezier_curve(void)
	{
		this_type::prv_init();
	}

	template<typename Iter>
	geo_bezier_curve(Iter s, Iter e, 
						typename
							boost::enable_if
							<
								is_iterator<Iter>,
								void*
							>::type chk = 0)
	{
		this_type::prv_init(s, e);
	}

	template<typename Container> explicit
	geo_bezier_curve(const Container& cont, 
						typename
							boost::enable_if
							<
								container_ex::is_container<Container>,
								void*
							>::type chk = 0)
	{
		this_type::prv_init(cont);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename T, typename ...Args>
	geo_bezier_curve(BOOST_FWD_REF(T) fst, BOOST_FWD_REF(Args)... args)
	{
		this_type::prv_init(boost::forward<T>(fst), boost::forward<Args>(args)...);
	}
#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename T \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		geo_bezier_curve( BOOST_FWD_REF(T) fst \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			this_type::prv_init( \
				boost::forward<T>(fst) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } 

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename OPos,
			template<typename _P, std::size_t> class OArray>
	geo_bezier_curve(const OArray<OPos, order_type::value>& right)
		: ctrl_points(boost::begin(right), boost::end(right) )
	{
	}

	geo_bezier_curve(BOOST_RV_REF(base_type) right)
		: ctrl_points(boost::move(right))
	{
	}

	geo_bezier_curve(const base_type& right)
		: ctrl_points(right)
	{
	}

	template<typename OPos,
			template<typename _P, std::size_t> class OArray>
	geo_bezier_curve(const geo_bezier_curve<OPos, order_type::value, OArray>& right)
		: ctrl_points(
			boost::begin(right.ctrl_points),
			boost::end(right.ctrl_points) )
	{
	}

	geo_bezier_curve(BOOST_RV_REF(this_type) right)
		: ctrl_points(boost::move(right.ctrl_points))
	{
	}

	geo_bezier_curve(const this_type& right)
		: ctrl_points(right.ctrl_points)
	{
	}

	~geo_bezier_curve(void)
	{
	}

public:
	template<typename Container> inline
	typename
		boost::enable_if
		<
			container_ex::is_container<Container>,
			this_type&
		>::type
		operator=(const Container& right)
	{
		this_type::prv_init(right);
		return *this;
	}

	template<typename OPos,
			template<typename _P, std::size_t> class OArray>
	this_type& operator=(const OArray<OPos, order_type::value>& right)
	{
		std::copy(boost::begin(right), boost::end(right), boost::begin(ctrl_points));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		if(&ctrl_points == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(ctrl_points, boost::move(right_ref));
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		if(&ctrl_points == &right)
		{
			return *this;
		}

		ctrl_points = right.ctrl_points;
		return *this;
	}

	template<typename OPos,
			template<typename _P, std::size_t> class OArray>
	this_type& operator=(const geo_bezier_curve<OPos, order_type::value, OArray>& right)
	{
		std::copy(boost::begin(right.ctrl_points), boost::end(right.ctrl_points), boost::begin(ctrl_points));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		::yggr::copy_or_move_or_swap(ctrl_points, boost::move(right_ref.ctrl_points));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		ctrl_points = right.ctrl_points;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right)
	{
		if(&ctrl_points == &right)
		{
			return;
		}

		::yggr::swap(ctrl_points, right);
	}

	void swap(BOOST_RV_REF(this_type) right)
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

		::yggr::swap(ctrl_points, right.ctrl_points);
	}

public:
	inline this_type reverse_curve(void) const
	{
		point_array_type rarr;
		std::copy(boost::rbegin(ctrl_points), boost::rend(ctrl_points), boost::begin(rarr));
		return this_type(rarr);
	}

public:
	inline static u32 order(void)
	{
		return order_type::value;
	}

	inline static std::size_t size(void)
	{
		return point_size_type::value;
	}

public:
	template<typename T> inline
	result_point_type value(T t) const
	{
		return detail::bezier_curve_value(ctrl_points, t);
	}

	template<typename T1, typename T2> inline
	result_point_type value(T1 t1, T2 t2) const
	{
		return detail::bezier_curve_value(ctrl_points, t1, t2);
	}

	template<typename T> inline
	result_point_type operator[](T t) const
	{
		return detail::bezier_curve_value(ctrl_points, t);
	}

	template<typename T1, typename T2> inline
	result_point_type operator[](const std::pair<T1, T2> t) const
	{
		return detail::bezier_curve_value(ctrl_points, t.first, t.second);
	}

	template<typename T> inline
	result_point_type derivation(T t) const
	{
		return detail::bezier_curve_derivation(ctrl_points, t);
	}

	template<typename T1, typename T2> inline
	result_point_type derivation(T1 t1, T2 t2) const
	{
		return detail::bezier_curve_derivation(ctrl_points, t1, t2);
	}

	template<typename T> inline
	result_point_type operator()(T t) const
	{
		return detail::bezier_curve_derivation(ctrl_points, t);
	}

	template<typename T1, typename T2> inline
	result_point_type operator()(const std::pair<T1, T2>& t) const
	{
		return detail::bezier_curve_derivation(ctrl_points, t.first, t.second);
	}
public:
	inline void reverse(void)
	{
		::std::reverse(boost::begin(ctrl_points), boost::end(ctrl_points));
	}

public:
	template<
		typename Oth_P, 
		template<typename _P, std::size_t> class Oth_Array >
	bool equal_to(const geo_bezier_curve<Oth_P, order_type::value, Oth_Array>& right) const
	{
		typedef typename range_ex::range_const_iterator_ex<point_array_type>::type pos_citer_type;
		typedef geo_bezier_curve<Oth_P, order_type::value, Oth_Array> other_type;
		typedef typename other_type::point_array_type other_point_array_type;
		typedef typename range_ex::range_const_iterator_ex<other_point_array_type>::type other_pos_citer_type;

		
		pos_citer_type li = boost::begin(ctrl_points), lisize = boost::end(ctrl_points);
		other_pos_citer_type ri = boost::begin(right.ctrl_points), risize = boost::end(right.ctrl_points);

		assert(std::distance(li, lisize) == std::distance(ri, risize));
		for(;li != lisize && ri != risize; ++li, ++ri)
		{
			if(*li != *ri)
			{
				return false;
			}
		}

		return true;
	}

	template<typename Oth_P, u32 Oth_N,
				template<typename _P, std::size_t> class Oth_Array > inline
	bool equal_to(const geo_bezier_curve<Oth_P, Oth_N, Oth_Array>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<(Oth_N != order_type::value)>));
		return false;
	}

	bool equal_to(const this_type& right) const
	{
		typedef typename range_ex::range_const_iterator_ex<point_array_type>::type pos_citer_type;

		pos_citer_type li = boost::begin(ctrl_points), lisize = boost::end(ctrl_points);
		pos_citer_type ri = boost::begin(right.ctrl_points), risize = boost::end(right.ctrl_points);

		if(li == ri) // this == this
		{
			return true;
		}

		assert(std::distance(li, lisize) == std::distance(ri, risize));
		for(;li != lisize && ri != risize; ++li, ++ri)
		{
			if(*li != *ri)
			{
				return false;
			}
		}

		return true;
	}

	template<typename Oth_P, u32 Oth_N,
				template<typename _P, std::size_t> class Oth_Array > inline
	bool not_equal_to(const geo_bezier_curve<Oth_P, Oth_N, Oth_Array>& right) const
	{
		return !this_type::equal_to(right);
	}

private:
	inline void prv_init(void)
	{
		std::fill(boost::begin(ctrl_points), boost::end(ctrl_points), point_type());
	}

	template<typename Iter> inline
	typename 
		boost::enable_if
		<
			is_iterator<Iter>,
			void
		>::type
		prv_init(Iter s, Iter e)
	{
		assert(std::distance(s, e) == point_size_type::value);
		std::copy(s, e, boost::begin(ctrl_points));
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			container_ex::is_container<Container>,
			void
		>::type
		prv_init(const Container& cont)
	{
		assert((container::size(cont) == point_size_type::value));
		std::copy(boost::begin(cont), boost::end(cont), boost::begin(ctrl_points));
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename T, typename ...Args> inline
	typename 
		boost::disable_if
		<
			boost::mpl::or_
			<
				is_iterator<typename mplex::reference_to_value_t<T>::type>,
				container_ex::is_container<typename mplex::reference_to_value_t<T>::type>,
				boost::is_same<this_type, typename mplex::reference_to_value_t<T>::type >
			>,
			void 
		>::type 
		prv_init(BOOST_FWD_REF(T) fst, BOOST_FWD_REF(Args)... args)
	{
		typedef 
			detail::geo_bezier_curve_init_helper_t
			<
				point_array_type, 
				0, 
				point_size_type::value
			> type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<T, point_array_type>));
		type t;
		t.init(ctrl_points, boost::forward<T>(fst), boost::forward<Args>(args)...);
	}
#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename T \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename \
			boost::disable_if< \
				boost::mpl::or_< \
					is_iterator<typename mplex::reference_to_value_t<T>::type>, \
					container_ex::is_container<typename mplex::reference_to_value_t<T>::type>, \
					boost::is_same<this_type, typename mplex::reference_to_value_t<T>::type> >, \
				void >::type \
			prv_init( BOOST_FWD_REF(T) fst \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef detail::geo_bezier_curve_init_helper_t<point_array_type, 0, point_size_type::value> type; \
			BOOST_MPL_ASSERT_NOT((boost::is_same<T, point_array_type>)); \
			type t; \
			t.init(ctrl_points, boost::forward<T>(fst) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } 

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline void prv_init(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return;
		}

		yggr::copy_or_move_or_swap(ctrl_points, boost::move(right_ref.ctrl_points));
	}

	inline void prv_init(const this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		ctrl_points = right.ctrl_points;
	}

protected:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(ctrl_points);
	}

public:
	point_array_type ctrl_points;
};

template<typename P, yggr::u32 N, template<typename _P, std::size_t> class Array> inline
yggr::geometry::geo_bezier_curve<P, N, Array>
	operator~(BOOST_RV_REF_BEG yggr::geometry::geo_bezier_curve<P, N, Array> BOOST_RV_REF_END val)
{
	typedef yggr::geometry::geo_bezier_curve<P, N, Array> value_type;
	value_type tmp(boost::move(val));
	tmp.reverse();
	return tmp;
}

template<typename P, yggr::u32 N, template<typename _P, std::size_t> class Array> inline
yggr::geometry::geo_bezier_curve<P, N, Array>
	operator~(const yggr::geometry::geo_bezier_curve<P, N, Array>& val)
{
	typedef yggr::geometry::geo_bezier_curve<P, N, Array> value_type;
	value_type tmp(val);
	tmp.reverse();
	return tmp;
}

template<typename P1, yggr::u32 N1, template<typename _P, std::size_t> class Array1,
			typename P2, yggr::u32 N2, template<typename _P, std::size_t> class Array2 > inline
bool operator==(const yggr::geometry::geo_bezier_curve<P1, N1, Array1>& l,
					const yggr::geometry::geo_bezier_curve<P2, N2, Array2>& r)
{
	return l.equal_to(r);
}

template<typename P1, yggr::u32 N1, template<typename _P, std::size_t> class Array1,
			typename P2, yggr::u32 N2, template<typename _P, std::size_t> class Array2 > inline
bool operator!=(const yggr::geometry::geo_bezier_curve<P1, N1, Array1>& l,
					const yggr::geometry::geo_bezier_curve<P2, N2, Array2>& r)
{
	return !l.equal_to(r);
}

} // namespace geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{
namespace swap_support
{

template<typename P, yggr::u32 N, template<typename _P, std::size_t> class Array> inline
void swap(geo_bezier_curve<P, N, Array>& l, geo_bezier_curve<P, N, Array>& r) 
{
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace geometry
} // namespace yggr

namespace std
{
	using ::yggr::geometry::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::geometry::swap_support::swap;
} // namespace boost

#endif // __YGGR_GEOMETRY_GEO_BEZIER_CURVE_T_HPP__
