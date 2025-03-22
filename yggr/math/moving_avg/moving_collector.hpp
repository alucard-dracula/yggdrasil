//moving_collector.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_MOVEING_COLLECTOR_HPP__
#define __YGGR_MATH_MOVING_AVG_MOVEING_COLLECTOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/container/list.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#include <utility>

namespace yggr
{
namespace math
{
namespace moving_avg
{

// -----------non_moving_collector------------------
template<s32 NStep, typename Val>
struct non_moving_collector
{
public:
	typedef Val value_type;

	YGGR_STATIC_CONSTANT(s32, E_step = NStep);
	typedef std::pair<value_type, bool> result_type;

private:
	BOOST_MPL_ASSERT((
		boost::mpl::and_
		<
			boost::mpl::or_
			<
				typename boost::is_floating_point<value_type>::type,
				typename boost::is_integral<value_type>::type
			>,
			boost::mpl::bool_< (sizeof(value_type) >= sizeof(u32)) >
		>));

	typedef yggr::list<value_type> value_list_type;

private:
	typedef non_moving_collector this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	non_moving_collector(void)
		: _sample_size(0)
	{
	}

	non_moving_collector(BOOST_RV_REF(this_type) right)
		: _sample_size(right._sample_size)
	{
	}

	non_moving_collector(const this_type& right)
		: _sample_size(right._sample_size)
	{
	}

	~non_moving_collector(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		_sample_size = right._sample_size;

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_sample_size = right._sample_size;

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

		yggr::swap(_sample_size, right._sample_size);
		return;
	}

public:
	inline std::pair<value_type, bool> operator<<(const value_type& val)
	{
		if(_sample_size < E_step)
		{
			++_sample_size;
		}
		return std::make_pair(value_type(0), _sample_size > 1);
	}

	inline bool is_saturated(void) const
	{
		return _sample_size == E_step;
	}

	inline u64 size(void) const
	{
		return _sample_size;
	}

	inline u64 step(void) const
	{
		return static_cast<yggr::u64>(E_step);
	}

	inline static u64 s_step(void)
	{
		return static_cast<yggr::u64>(E_step);
	}

private:
	u64 _sample_size;
};

template<typename T>
struct is_non_moving_collector
	: public boost::mpl::false_
{
};

template<s32 step, typename Val>
struct is_non_moving_collector< non_moving_collector<step, Val> >
	: public boost::mpl::true_
{
};

} // namespace moving_avg
} // namespace math
} // namespace yggr

#define YGGR_IS_NON_MOVING_COLLECTOR_DEF( __type__ ) \
	namespace yggr{ namespace math{ namespace moving_avg { \
	template<> \
	struct is_non_moving_collector< __type__ > : public boost::mpl::true_ { }; }}}


namespace yggr
{
namespace math
{
namespace moving_avg
{
namespace swap_support
{

template<yggr::s32 step, typename Val> inline
void swap(non_moving_collector< step, Val >& l, non_moving_collector< step, Val >& r)
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

// ----------moving_collector-----------------------
namespace yggr
{
namespace math
{
namespace moving_avg
{

template<s32 NStep, typename Val>
struct moving_collector
{
public:
	typedef Val value_type;

	YGGR_STATIC_CONSTANT(s32, E_step = NStep);

	typedef std::pair<value_type, bool> result_type;

private:
	BOOST_MPL_ASSERT((
		boost::mpl::and_
		<
			boost::mpl::or_
			<
				typename boost::is_floating_point<value_type>::type,
				typename boost::is_integral<value_type>::type
			>,
			boost::mpl::bool_< (sizeof(value_type) >= sizeof(u32)) >
		>));

	typedef yggr::list<value_type> value_list_type;

private:
	typedef moving_collector this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	moving_collector(void)
	{
	}

	moving_collector(BOOST_RV_REF(this_type) right)
		: _range_list(boost::move(right._range_list))
	{
	}

	moving_collector(const this_type& right)
		: _range_list(right._range_list)
	{
	}

	~moving_collector(void)
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

		copy_or_move_or_swap(_range_list, boost::move(right._range_list));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_range_list = right._range_list;

		return *this;
	}

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

		_range_list.swap(right._range_list);
	}

public:
	inline std::pair<value_type, bool> operator<<(const value_type& val)
	{
		return this_type::prv_append_sample(val);
	}

	inline bool is_saturated(void) const
	{
		return _range_list.size() == E_step;
	}

	inline u64 size(void) const
	{
		return _range_list.size();
	}

	inline u64 step(void) const
	{
		return static_cast<yggr::u64>(E_step);
	}

	inline static u64 s_step(void)
	{
		return static_cast<yggr::u64>(E_step);
	}

private:
	std::pair<value_type, bool> prv_append_sample(value_type pn)
	{
		value_type p1 = _range_list.empty()? 0 : _range_list.front();

		if(_range_list.size() < E_step)
		{
			_range_list.push_back(pn);
			return std::make_pair(p1, false);
		}

		assert(_range_list.size() == E_step);

		value_list_type tmp_list;
		typename value_list_type::iterator its = _range_list.begin();
		typename value_list_type::iterator ite = its;
		++ite;

		tmp_list.splice(tmp_list.end(), _range_list, its, ite);

		tmp_list.front() = pn;

		_range_list.splice(_range_list.end(), tmp_list);

		assert(tmp_list.empty());
		assert(_range_list.size() == E_step);

		return std::make_pair(p1, true);
	}

private:
	value_list_type _range_list;
};

template<typename T>
struct is_moving_collector
	: public boost::mpl::false_
{
};

template<s32 step, typename Val>
struct is_moving_collector< moving_collector<step, Val> >
	: public boost::mpl::true_
{
};

} // namespace moving_avg
} // namespace math
} // namespace yggr

#define YGGR_IS_MOVING_COLLECTOR_DEF( __type__ ) \
	namespace yggr{ namespace math{ namespace moving_avg { \
	template<> \
	struct is_moving_collector< __type__ > : public boost::mpl::true_ { }; }}}


namespace yggr
{
namespace math
{
namespace moving_avg
{
namespace swap_support
{

template<yggr::s32 step, typename Val> inline
void swap(moving_collector< step, Val >& l, moving_collector< step, Val >& r)
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

#endif // __YGGR_MATH_MOVING_AVG_MOVEING_COLLECTOR_HPP__
