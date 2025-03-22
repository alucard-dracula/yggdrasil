//xtime_basic_foo.hpp

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

#ifndef __YGGR_TIME_XTIME_BASIC_FOO_HPP__
#define __YGGR_TIME_XTIME_BASIC_FOO_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/math/modulus.hpp>
#include <yggr/mplex/limits.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/time/time_ratio_def.hpp>

#include <boost/thread/xtime.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>


namespace yggr
{
namespace time
{

namespace detail
{

template<typename T>
struct is_floating_point_time_second
	: public boost::is_floating_point<T>
{
};

template<typename T>
struct is_integer_time_second
	: public
		boost::mpl::and_
		<
			boost::mpl::not_< is_floating_point_time_second<T> >,
			boost::mpl::not_< boost::is_base_of<boost::xtime, T > >
		>
{
};

template<typename T>
struct is_time_second
	: public 
		boost::mpl::or_
		<
			is_floating_point_time_second<T>,
			is_integer_time_second<T>
		>
{
};

} // namespace detail

// decompose
inline bool xtime_decomposable(const boost::xtime& t)
{
	return 
		((t.sec < 0 && 0 < t.nsec) || (t.nsec < 0 && 0 < t.sec))
		&& (-time_ratio_def::E_S_AND_NS_RATIO < t.nsec
			&& t.nsec < time_ratio_def::E_S_AND_NS_RATIO);
}

inline boost::xtime& xtime_decompose(boost::xtime& t)
{
	if(t.sec < 0 && 0 < t.nsec)
	{
		assert(xtime_decomposable(t));
		t.sec = t.sec + 1;
		t.nsec = t.nsec - time_ratio_def::E_S_AND_NS_RATIO;
		return t;
	}
	else if(t.nsec < 0 && 0 < t.sec)
	{
		assert(xtime_decomposable(t));
		t.sec = t.sec - 1;
		t.nsec = t.nsec + time_ratio_def::E_S_AND_NS_RATIO;
		return t;
	}
	else
	{
		assert(!xtime_decomposable(t));
		return t;
	}
}

// format
inline bool xtime_formatable(const boost::xtime& t)
{
	return 
		(t.nsec <= -time_ratio_def::E_S_AND_NS_RATIO)
		|| (time_ratio_def::E_S_AND_NS_RATIO <= t.nsec);
}

inline boost::xtime& xtime_format(boost::xtime& t)
{
	t.sec += t.nsec / time_ratio_def::E_S_AND_NS_RATIO;
	t.nsec = math::modulus(t.nsec, time_ratio_def::E_S_AND_NS_RATIO);
	return t;
}

// from init

template<typename T>
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_from_second(boost::xtime& l, const T& r);

template<typename T>
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_from_second(boost::xtime& l, const T& r);

template<typename T> inline
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_from_second(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	if(0 == r - static_cast<int_type>(r))
	{
		return xtime_from_second<int_type>(l, static_cast<int_type>(r));
	}
	else
	{
		l.sec = static_cast<xtime_sec_type>(r);
		l.nsec = static_cast<xtime_nsec_type>((r - l.sec) * time_ratio_def::E_S_AND_NS_RATIO);
		return l;
	}
}

template<typename T> inline
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_from_second(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	l.sec = static_cast<xtime_sec_type>(r);
	assert((l.sec == r));
	l.nsec = 0;

	return l;
}

// validate
inline bool xtime_validate(const boost::xtime& t)
{
	return 
		!((t.sec < 0 && 0 < t.nsec)
			|| (t.nsec < 0 && 0 < t.sec)
			|| (t.nsec <= -time_ratio_def::E_S_AND_NS_RATIO)
			|| (time_ratio_def::E_S_AND_NS_RATIO <= t.nsec));

}

// to_second
template<typename T> inline
T xtime_to_second(const boost::xtime& t)
{
	assert((xtime_validate(t)));

	return 
		T(static_cast<f64>(t.sec) 
			+ static_cast<f64>(t.nsec) 
				/ static_cast<f64>(time_ratio_def::E_S_AND_NS_RATIO));
}

// compare

inline s32 xtime_compare(const boost::xtime& l, const boost::xtime& r)
{
    return 
		l.sec == r.sec?
			l.nsec == r.nsec? 
			0
			: l.nsec < r.nsec? -1 : 1
		: l.sec < r.sec? -1 : 1;
}

inline bool xtime_compare_eq(const boost::xtime& l, const boost::xtime& r)
{
    return l.sec == r.sec && l.nsec == r.nsec;
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, s32>::type
	xtime_compare(const boost::xtime& l, const T& r)
{
	boost::xtime t = {0, 0};
	return xtime_compare(l, xtime_from_second(t, r));
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	xtime_compare_eq(const boost::xtime& l, const T& r)
{
	boost::xtime t = {0, 0};
	return xtime_compare_eq(l, xtime_from_second(t, r));
}

// + - * / %
// +

template<typename T>
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_plus_set(boost::xtime& l, const T& r);

template<typename T>
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_plus_set(boost::xtime& l, const T& r);

template<typename T> inline
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_plus_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	assert((xtime_validate(l)));

	if(0 == r - static_cast<int_type>(r))
	{
		return xtime_plus_set<int_type>(l, static_cast<int_type>(r));
	}
	else
	{
		s64 b1b2 = l.nsec + (r - int_type(r)) * time_ratio_def::E_S_AND_NS_RATIO;

		l.sec = static_cast<xtime_sec_type>(
					(l.sec + static_cast<xtime_sec_type>(r)) + b1b2 / time_ratio_def::E_S_AND_NS_RATIO);
		l.nsec = static_cast<xtime_nsec_type>(math::modulus(b1b2, time_ratio_def::E_S_AND_NS_RATIO));

		return xtime_decompose(l);
	}
}

template<typename T> inline
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_plus_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l)));

	l.sec = static_cast<xtime_sec_type>(l.sec + r);
	// l.nsec = l.nsec; // only mark
	return xtime_decompose(l);
}

inline boost::xtime& xtime_plus_set(boost::xtime& l, const boost::xtime& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l) && xtime_validate(r)));

	s64 b1b2 = l.nsec + r.nsec;

	l.sec = static_cast<xtime_sec_type>((l.sec + r.sec) + b1b2 / time_ratio_def::E_S_AND_NS_RATIO);
	l.nsec = static_cast<xtime_nsec_type>(math::modulus(b1b2, time_ratio_def::E_S_AND_NS_RATIO));

	return xtime_decompose(l);
}

// -
template<typename T>
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_minus_set(boost::xtime& l, const T& r);

template<typename T>
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_minus_set(boost::xtime& l, const T& r);

template<typename T> inline
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_minus_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	assert((xtime_validate(l)));

	if(0 == r - static_cast<int_type>(r))
	{
		return xtime_minus_set<int_type>(l, static_cast<int_type>(r));
	}
	else
	{
		s64 b1b2 = l.nsec - (r - int_type(r)) * time_ratio_def::E_S_AND_NS_RATIO;

		l.sec = static_cast<xtime_sec_type>(
					(l.sec - static_cast<xtime_sec_type>(r)) + b1b2 / time_ratio_def::E_S_AND_NS_RATIO);
		l.nsec = static_cast<xtime_nsec_type>(math::modulus(b1b2, time_ratio_def::E_S_AND_NS_RATIO));

		return xtime_decompose(l);
	}
}

template<typename T> inline
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_minus_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l)));

	l.sec = static_cast<xtime_sec_type>(l.sec - r);
	// l.nsec = l.nsec;
	return xtime_decompose(l);
}

inline boost::xtime& xtime_minus_set(boost::xtime& l, const boost::xtime& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l) && xtime_validate(r)));

	s64 b1b2 = l.nsec - r.nsec;

	l.sec = static_cast<xtime_sec_type>((l.sec - r.sec) + b1b2 / time_ratio_def::E_S_AND_NS_RATIO);
	l.nsec = static_cast<xtime_nsec_type>(math::modulus(b1b2, time_ratio_def::E_S_AND_NS_RATIO));

	return xtime_decompose(l);
}

// *

template<typename T>
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_multiplies_set(boost::xtime& l, const T& r);

template<typename T>
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_multiplies_set(boost::xtime& l, const T& r);

template<typename T> inline
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_multiplies_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	assert((xtime_validate(l)));

	return 
		(0 == r - static_cast<int_type>(r))?
			xtime_multiplies_set<int_type>(l, static_cast<int_type>(r))
			: xtime_from_second(l, xtime_to_second<f64>(l) * r);
}

template<typename T> inline
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_multiplies_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l)));

	s64 b1 = l.nsec * r;

	l.sec = static_cast<xtime_sec_type>(l.sec * r + b1 / time_ratio_def::E_S_AND_NS_RATIO);
	l.nsec = static_cast<xtime_nsec_type>(math::modulus(b1, time_ratio_def::E_S_AND_NS_RATIO));

	assert(!xtime_decomposable(l));
	return l;
}

// t = a + b / c
// t1 * t1=> a1 * a2 + (a1 * b2 + a2 * b1 + b1 * b2 / c) / c 

inline boost::xtime& xtime_multiplies_set(boost::xtime& l, const boost::xtime& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l) && xtime_validate(r)));

	if(r.nsec == 0)
	{
		if(l.nsec == 0)
		{
			l.sec *= r.sec;
			return l;
		}
		else
		{
			return xtime_multiplies_set(l, r.sec);
		}
	}
	else
	{
		if(l.nsec == 0)
		{
			boost::xtime t = r;
			return l = xtime_multiplies_set(t, l.sec);
		}
		else
		{
			return xtime_from_second(l, xtime_to_second<f64>(l) * xtime_to_second<f64>(r));
		}
	}
}

// /

template<typename T>
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_divides_set(boost::xtime& l, const T& r);

template<typename T>
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_divides_set(boost::xtime& l, const T& r);

template<typename T> inline
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_divides_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	assert((xtime_validate(l)));
	assert(r);

	return 
		(0 == r - static_cast<int_type>(r))?
			xtime_divides_set<int_type>(l, static_cast<int_type>(r))
			: xtime_from_second(l, xtime_to_second<f64>(l) / r);
}

template<typename T> inline
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_divides_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	assert((xtime_validate(l)));
	assert(r);

	s64 b1 = (math::modulus(l.sec, r) * time_ratio_def::E_S_AND_NS_RATIO + l.nsec) / r;

	l.sec = static_cast<xtime_sec_type>(l.sec / r + b1 / time_ratio_def::E_S_AND_NS_RATIO);
	l.nsec = static_cast<xtime_nsec_type>(math::modulus(b1, time_ratio_def::E_S_AND_NS_RATIO));

	assert(!xtime_decomposable(l));
	return l;
}

template<typename T> inline
T xtime_divides(const boost::xtime& l, const boost::xtime& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l) && xtime_validate(r)));
	assert((r.sec || r.nsec));

	return 
		(l.nsec == 0 && r.nsec == 0)?
			T(l.sec) / T(r.sec)
			:  T(xtime_to_second<f64>(l) / xtime_to_second<f64>(r));
}

inline boost::xtime& xtime_divides_set(boost::xtime& l, const boost::xtime& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l) && xtime_validate(r)));
	assert((r.sec || r.nsec));

	return 
		(r.nsec == 0)?
			xtime_divides_set(l, r.sec)
			: xtime_from_second(l, xtime_to_second<f64>(l) / xtime_to_second<f64>(r));
}

// %

template<typename T>
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_modulus_set(boost::xtime& l, const T& r);

template<typename T>
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_modulus_set(boost::xtime& l, const T& r);

template<typename T> inline
typename boost::enable_if<detail::is_floating_point_time_second<T>, boost::xtime&>::type
	xtime_modulus_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;
	typedef typename upper_integer<T>::type int_type;

	assert((xtime_validate(l)));
	assert(r);

	return 
		(0 == r - static_cast<int_type>(r))?
			xtime_modulus_set<int_type>(l, static_cast<int_type>(r))
			: xtime_from_second(l, math::modulus(xtime_to_second<f64>(l), r));
}

template<typename T> inline
typename boost::enable_if<detail::is_integer_time_second<T>, boost::xtime&>::type
	xtime_modulus_set(boost::xtime& l, const T& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l)));
	assert(r);

	return 
		(l.nsec == 0)?
			(l.sec = math::modulus(l.sec, r), l)
			: xtime_from_second(l, math::modulus(xtime_to_second<f64>(l), r));
}

inline boost::xtime& xtime_modulus_set(boost::xtime& l, const boost::xtime& r)
{
	typedef boost::xtime::xtime_sec_t xtime_sec_type;
	typedef boost::xtime::xtime_nsec_t xtime_nsec_type;

	assert((xtime_validate(l) && xtime_validate(r)));
	assert((r.sec || r.nsec));

	return 
		(r.nsec == 0)?
			xtime_modulus_set(l, r.sec)
			: xtime_from_second(
				l, 
				math::modulus(
					xtime_to_second<f64>(l), 
					xtime_to_second<f64>(r) ) );
}

} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_XTIME_BASIC_FOO_HPP__
