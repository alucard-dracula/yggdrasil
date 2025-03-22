//moving_avg_accumulator.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_ACCUMULATOR_HPP__
#define __YGGR_MATH_MOVING_AVG_ACCUMULATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/math/moving_avg/moving_elem_genner.hpp>
#include <yggr/math/moving_avg/moving_result_getter.hpp>

#include <yggr/math/moving_avg/sma_accumulator.hpp>
#include <yggr/math/moving_avg/cma_accumulator.hpp>
#include <yggr/math/moving_avg/mma_accumulator.hpp>
#include <yggr/math/moving_avg/ema_accumulator.hpp>
#include <yggr/math/moving_avg/wma_accumulator.hpp>

#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace yggr
{
namespace math
{
namespace moving_avg
{

template< typename Val,
			typename Collector,
			typename Cdt_Genner,
			typename Result_Getter,
			typename Calculator >
struct make_accumulator
{
	BOOST_MPL_ASSERT((
		boost::mpl::or_
		<
			boost::is_floating_point<Val>,
			boost::is_signed<Val>
		> ));

	typedef yggr::tuple<Val, Collector, Cdt_Genner, Result_Getter, Calculator> type;
};

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::tuples::null_type
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_TUPLE_LIMIT_LENGTH(), typename T ) >
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct make_accumlator_group
{
	typedef yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> type;
};

namespace detail
{

// maa_check_group
template<typename Group,
			std::size_t size = yggr::tuples::length<Group>::value >
struct maa_check_group
	: public
		boost::mpl::and_
		<
			boost::mpl::bool_
			<(
				boost::is_same
				<
					typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
					typename yggr::tuples::element< 1, typename yggr::tuples::element<0, Group>::type >::type::value_type
				>::value
				&&
				boost::is_same
				<
					typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
					typename yggr::tuples::element< 2, typename yggr::tuples::element<0, Group>::type >::type::result_type
				>::value
				&&
				boost::is_same
				<
					typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
					typename yggr::tuples::element< 3, typename yggr::tuples::element<0, Group>::type >::type::result_type
				>::value
				&&
				boost::is_same
				<
					typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
					typename yggr::tuples::element< 4, typename yggr::tuples::element<0, Group>::type >::type::value_type
				>::value
				&&
				is_matched_genner_and_getter
				<
					typename yggr::tuples::element< 2, typename yggr::tuples::element<0, Group>::type >::type,
					typename yggr::tuples::element< 3, typename yggr::tuples::element<0, Group>::type >::type
				>::value
			)>,
			typename maa_check_group<typename Group::tail_type, yggr::tuples::length<typename Group::tail_type>::value>::type
		>::type
{
};

template<typename Group >
struct maa_check_group<Group, 1 >
	: public
		boost::mpl::bool_
		<(
			boost::is_same
			<
				typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
				typename yggr::tuples::element< 1, typename yggr::tuples::element<0, Group>::type >::type::value_type
			>::value
			&&
			boost::is_same
			<
				typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
				typename yggr::tuples::element< 2, typename yggr::tuples::element<0, Group>::type >::type::result_type
			>::value
			&&
			boost::is_same
			<
				typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
				typename yggr::tuples::element< 3, typename yggr::tuples::element<0, Group>::type >::type::result_type
			>::value
			&&
			boost::is_same
			<
				typename yggr::tuples::element< 0, typename yggr::tuples::element<0, Group>::type >::type,
				typename yggr::tuples::element< 4, typename yggr::tuples::element<0, Group>::type >::type::value_type
			>::value
			&&
			is_matched_genner_and_getter
			<
				typename yggr::tuples::element< 2, typename yggr::tuples::element<0, Group>::type >::type,
				typename yggr::tuples::element< 3, typename yggr::tuples::element<0, Group>::type >::type
			>::value
		)>
{
};

template<typename Group >
struct maa_check_group<Group, 0 >
	: public boost::mpl::false_
{
};

template<typename Group,
			std::size_t size = yggr::tuples::length<Group>::value >
struct maa_caller
{
private:
	typedef maa_caller this_type;

public:
	typedef Group group_type;
	typedef typename yggr::tuples::element<0, group_type>::type now_acc_type;

	typedef typename yggr::tuples::element<0, now_acc_type>::type value_type;
	typedef typename yggr::tuples::element<1, now_acc_type>::type collector_type;
	typedef typename yggr::tuples::element<2, now_acc_type>::type cdt_genner_type;
	typedef typename yggr::tuples::element<4, now_acc_type>::type calculator_type;

public:
	template<typename T1, typename T2> inline
	const this_type& operator()(group_type& group, const T1& s, const T2& befor_ma) const
	{
		typedef typename group_type::tail_type next_group_type;
		typedef maa_caller<next_group_type> next_caller_type;
		typedef typename calculator_type::result_type result_type;

		now_acc_type& now_acc = yggr::get<0>(group);

		value_type& now_val = yggr::get<0>(now_acc);
		collector_type& collector = yggr::get<1>(now_acc);
		cdt_genner_type& genner = yggr::get<2>(now_acc);
		calculator_type& calculator = yggr::get<4>(now_acc);

		value_type s_val = genner(s, befor_ma);

		result_type rst = calculator(collector, now_val, s_val);

		now_val = yggr::get<0>(rst);

		next_caller_type caller;
		caller(group.get_tail(), s, befor_ma);
		return *this;
	}

};

template<typename Group >
struct maa_caller<Group, 1 >
{
private:
	typedef maa_caller this_type;

public:
	typedef Group group_type;
	typedef typename yggr::tuples::element<0, group_type>::type now_acc_type;

	typedef typename yggr::tuples::element<0, now_acc_type>::type value_type;
	typedef typename yggr::tuples::element<1, now_acc_type>::type collector_type;
	typedef typename yggr::tuples::element<2, now_acc_type>::type cdt_genner_type;
	typedef typename yggr::tuples::element<4, now_acc_type>::type calculator_type;

public:
	template<typename T1, typename T2> inline
	const this_type& operator()(group_type& group, const T1& s, const T2& befor_ma) const
	{
		typedef typename calculator_type::result_type result_type;

		now_acc_type& now_acc = yggr::get<0>(group);

		value_type& now_val = yggr::get<0>(now_acc);
		collector_type& collector = yggr::get<1>(now_acc);
		cdt_genner_type& genner = yggr::get<2>(now_acc);
		calculator_type& calculator = yggr::get<4>(now_acc);
		value_type s_val = genner(s, befor_ma);

		result_type rst = calculator(collector, now_val, s_val);

		now_val = yggr::get<0>(rst);
		return *this;
	}
};

template<typename Group >
struct maa_caller<Group, 0 >
{
private:
	typedef maa_caller this_type;

public:
	typedef Group group_type;

public:
	template<typename T1, typename T2> inline
	const this_type& operator()(group_type&, const T1&, const T2&) const
	{
		return *this;
	}
};

} // namespace detail

template<typename Accumlator_Group,
			template<typename _T> class Correct = correct >
class moving_avg_accumulator
{
public:
	typedef Accumlator_Group accumlator_group_type;

	YGGR_STATIC_CONSTANT(u32, E_idx_value = 0);
	YGGR_STATIC_CONSTANT(u32, E_idx_collector = 1);
	YGGR_STATIC_CONSTANT(u32, E_idx_cdt_genner = 2);
	YGGR_STATIC_CONSTANT(u32, E_idx_result_getter = 3);
	YGGR_STATIC_CONSTANT(u32, E_idx_calculator = 4);

	typedef typename
		yggr::tuples::element
		<
			0,
			typename yggr::tuples::element<0, accumlator_group_type>::type
		>::type value_type;

private:
	typedef Correct<value_type> correct_type;

private:
	BOOST_MPL_ASSERT((detail::maa_check_group<accumlator_group_type>));

private:
	typedef moving_avg_accumulator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	moving_avg_accumulator(void)
	{
	}

	moving_avg_accumulator(BOOST_RV_REF(this_type) right)
		: _acc_group(boost::move(right._acc_group))
	{
	}

	moving_avg_accumulator(const this_type& right)
		: _acc_group(right._acc_group)
	{
	}

	~moving_avg_accumulator(void)
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

		copy_or_move_or_swap(_acc_group, boost::move(right._acc_group));

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_acc_group = right._acc_group;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_acc_group, right._acc_group);
	}

public:
	template<typename T> inline
	this_type& operator<<(const T& s)
	{
		typedef typename yggr::tuples::element<0, accumlator_group_type>::type now_acc_type;

		typedef typename yggr::tuples::element<0, now_acc_type>::type value_type;
		typedef typename yggr::tuples::element<1, now_acc_type>::type collector_type;
		typedef typename yggr::tuples::element<2, now_acc_type>::type cdt_genner_type;
		typedef typename yggr::tuples::element<4, now_acc_type>::type calculator_type;

		typedef typename calculator_type::result_type result_type;

		typedef typename accumlator_group_type::tail_type next_group_type;
		typedef detail::maa_caller<next_group_type> caller_type;

		now_acc_type& now_acc = yggr::get<0>(_acc_group);

		value_type& now_val = yggr::get<0>(now_acc);

		collector_type& collector = yggr::get<1>(now_acc);
		cdt_genner_type& genner = yggr::get<2>(now_acc);
		calculator_type& calculator = yggr::get<4>(now_acc);

		value_type s_val = this_type::prv_s_expand_value(s);

		result_type rst = calculator(collector, now_val, genner(s_val));

		now_val = yggr::get<0>(rst);

		if(collector.size() > 1)
		{
			caller_type caller;
			caller(_acc_group.get_tail(), s_val, yggr::get<1>(rst));
		}

		return *this;
	}

	/*
	 * if using pair of genner and getter,
	 * example [pow2_genner, sqrt_getter] or [pow2_genner, fast_getter]
	 * the last result method same average's method, because,
	 * sqrt{ (1/n) * sum[ ( k * xi - k * x_avg )^2 ] }
	 * = sqrt{ (1/n) * sum[ k^2 * ( xi - x_avg )^2 ] }
	 * = sqrt{ (1/n) * k^2 * sum[ ( xi - x_avg )^2 ] }
	 * = k * sqrt{ (1/n) * sum[ ( xi - x_avg )^2 ] }
	 * so, the last result rounding is still using ( expanded_result + k / 2 ) / k
	 */
	template<std::size_t idx> inline
	typename
		yggr::tuples::element
		<
			static_cast<std::size_t>(E_idx_result_getter),
			typename yggr::tuples::element
						<
							idx,
							accumlator_group_type
						>::type
		>::type::result_type
		get_result(void) const
	{
		typedef typename yggr::tuples::element<idx, accumlator_group_type>::type now_acc_type;
		typedef typename
			yggr::tuples::element
			<
				static_cast<std::size_t>(E_idx_value),
				now_acc_type
			>::type value_type;

		typedef typename
			yggr::tuples::element
			<
				static_cast<std::size_t>(E_idx_result_getter),
				now_acc_type
			>::type result_getter_type;

		const now_acc_type& acc = yggr::get<idx>(_acc_group);
		const value_type& rst_val = yggr::get<static_cast<std::size_t>(E_idx_value)>(acc);
		const result_getter_type& gettter = yggr::get<static_cast<std::size_t>(E_idx_result_getter)>(acc);

		return (gettter(rst_val) + correct_type::s_round_value()) / correct_type::s_correct_value();
	}

	template<std::size_t idx> inline
	bool is_saturated(void) const
	{
		typedef typename yggr::tuples::element<idx, accumlator_group_type>::type now_acc_type;
		typedef typename
			yggr::tuples::element
			<
				static_cast<std::size_t>(E_idx_collector),
				now_acc_type
			>::type collector_type;

		const now_acc_type& acc = yggr::get<idx>(_acc_group);
		const collector_type& collector = yggr::get<static_cast<std::size_t>(E_idx_collector)>(acc);

		return collector.is_saturated();
	}

	template<std::size_t idx> inline
	std::pair
	<
		typename yggr::tuples::element
			<
				static_cast<std::size_t>(E_idx_result_getter),
				typename yggr::tuples::element
							<
								idx,
								accumlator_group_type
							>::type
			>::type::result_type,
		bool
	>
		get_result_pair(void) const
	{
		return std::make_pair(this_type::get_result<idx>(), this_type::is_saturated<idx>());
	}

	template<std::size_t idx1, std::size_t idx2> inline
	typename
		yggr::tuples::access_traits
		<
			typename
				yggr::tuples::element
				<
					idx2,
					typename
						yggr::tuples::element
						<
							idx1,
							accumlator_group_type
						>::type
				>::type
		>::non_const_type
		get(void)
	{
		return yggr::get<idx2>(yggr::get<idx1>(_acc_group));
	}

	template<std::size_t idx1, std::size_t idx2> inline
	typename yggr::tuples::access_traits
				<
					typename yggr::tuples::element
							<
								idx2,
								typename yggr::tuples::element
											<
												idx1,
												accumlator_group_type
											>::type
							>::type
				>::const_type
		get(void) const
	{
		return yggr::get<idx2>(yggr::get<idx1>(_acc_group));
	}

private:
	template<typename T> inline
	static value_type prv_s_expand_value(const T& val)
	{
		value_type tmp = val;
		return tmp * correct_type::s_correct_value();
	}

	template<typename T> inline
	static T prv_s_rounding_value(const T& val)
	{
		typedef T ret_type;
		value_type tmp_val = val;
		return ret_type((tmp_val + correct_type::s_round_value()) / correct_type::s_correct_value());
	}

private:
	accumlator_group_type _acc_group;
};

} // namespace moving_avg
} // namespace math
} // namespace yggr


namespace yggr
{
namespace math
{
namespace moving_avg
{
namespace swap_support
{

template<typename Accumlator_Group,
			template<typename _T> class Corrent> inline
void swap(moving_avg_accumulator<Accumlator_Group, Corrent>& l,
			moving_avg_accumulator<Accumlator_Group, Corrent>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace moving_avg
} // namespace math
} // namespace yggr

namespace std
{
	using ::yggr::math::moving_avg::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::math::moving_avg::swap_support::swap;
} // namespace boost

#endif // __YGGR_MATH_MOVING_AVG_ACCUMULATOR_HPP__

