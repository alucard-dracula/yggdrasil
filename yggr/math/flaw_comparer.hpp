//flaw_comparer.hpp

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

#ifndef __YGGR_MATH_FLAW_COMPARER_HPP__
#define __YGGR_MATH_FLAW_COMPARER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/support/operator_t.hpp>
#include <yggr/math/detail/flawcmp.hpp>

#include <complex>
#include <float.h>

#ifndef YGGR_MATH_F32_DEFAULT_PRECISION
//#	define YGGR_MATH_F32_DEFAULT_PRECISION() (0.000001f) // older
#	define YGGR_MATH_F32_DEFAULT_PRECISION() (FLT_EPSILON)
#endif // YGGR_MATH_F32_PRECISION

#ifndef YGGR_MATH_F64_DEFAULT_PRECISION
//#	define YGGR_MATH_F64_DEFAULT_PRECISION() (0.00000000000001) // older
#	define YGGR_MATH_F64_DEFAULT_PRECISION() (DBL_EPSILON)
#endif  //YGGR_MATH_F64_DEFAULT_PRECISION

#ifndef YGGR_MATH_FLDB_DEFAULT_PRECISION
#	define YGGR_MATH_FLDB_DEFAULT_PRECISION() (LDBL_EPSILON)
#endif  //YGGR_MATH_FLDB_DEFAULT_PRECISION

namespace yggr
{
namespace math
{

namespace detail
{
template<typename T>
struct default_flaw_genner
{
private:
	typedef default_flaw_genner this_type;

public:
	inline T operator()(void) const
	{
		return T();
	}
};

template<>
struct default_flaw_genner<f32>
{
private:
	typedef default_flaw_genner this_type;

public:
	inline f32 operator()(void) const
	{
		return YGGR_MATH_F32_DEFAULT_PRECISION();
	}
};

template<>
struct default_flaw_genner<f64>
{
private:
	typedef default_flaw_genner this_type;

public:
	inline f64 operator()(void) const
	{
		return YGGR_MATH_F64_DEFAULT_PRECISION();
	}
};

template<>
struct default_flaw_genner<long double>
{
private:
	typedef default_flaw_genner this_type;

public:
	inline long double operator()(void) const
	{
		return YGGR_MATH_FLDB_DEFAULT_PRECISION();
	}
};



template<typename T>
struct choose_default_flaw_genner
	: public 
		boost::mpl::if_
		<
			boost::is_floating_point<T>,
			default_flaw_genner<T>,
			mplex::null_type
		>
{
};

template<typename T> inline
const typename choose_default_flaw_genner<T>::type&
	default_genner(void)
{
	typedef typename choose_default_flaw_genner<T>::type genner_type;
	static genner_type gen;
	return gen;
}


} // namespace detail

template<typename T, 
			typename FlawGen = typename detail::choose_default_flaw_genner<T>::type> 
class flaw_comparer
	: protected detail::flaw_comparer_impl<T, FlawGen>
{
protected:
	typedef detail::flaw_comparer_impl<T, FlawGen> base_type;

public:
	typedef typename base_type::value_type value_type;
	typedef typename base_type::flaw_genner_type flaw_genner_type;

private:
	typedef flaw_comparer this_type;

public:
	flaw_comparer(const value_type& val)
		: base_type(val, detail::default_genner<value_type>())
	{
	}

	template<typename TFlawGen>
	flaw_comparer(const value_type& val, const TFlawGen& gen)
		: base_type(val, gen)
	{
	}
 
	flaw_comparer(const this_type& right)
		: base_type(right)
	{
	}

	~flaw_comparer(void)
	{
	}

public:
	
	// > >= < <= == != opbool

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return base_type::operator_bool();
	}

	// operator ==
	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_eq(const OVal& right) const
	{
		return base_type::pro_compare_eq(right);
	}

	template<typename OT> inline
	bool compare_eq(const flaw_comparer<OT, flaw_genner_type>& right) const
	{
		return base_type::pro_compare_eq(right);
	}

	//operator<
	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_less(const OVal& right) const
	{
		return base_type::pro_compare_less(right);
	}

	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_left_less(const OVal& left) const
	{
		return base_type::pro_compare_left_less(left);
	}

	template<typename OT> inline
	bool compare_less(const flaw_comparer<OT, flaw_genner_type>& right) const
	{
		return base_type::compare_less(right);
	}

	//operator<=
	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_less_eq(const OVal& right) const
	{
		return base_type::pro_compare_less_eq(right);
	}

	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_left_less_eq(const OVal& left) const
	{
		return base_type::pro_compare_left_less_eq(left);
	}

	template<typename OT> inline
	bool compare_less_eq(const flaw_comparer<OT, flaw_genner_type>& right) const
	{
		return base_type::pro_compare_less_eq(right);
	}

	// operator>
	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_greater(const OVal& right) const
	{
		return base_type::pro_compare_greater(right);
	}

	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_left_greater(const OVal& left) const
	{
		return base_type::pro_compare_left_greater(left);
	}

	template<typename OT> inline
	bool compare_greater(const flaw_comparer<OT, flaw_genner_type>& right) const
	{
		return base_type::pro_compare_greater(right);
	}

	// operator>=
	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_greater_eq(const OVal& right) const
	{
		return base_type::pro_compare_greater_eq(right);
	}

	template<typename OVal> inline
	typename boost::disable_if<detail::is_flaw_comparer<OVal>, bool>::type
		compare_left_greater_eq(const OVal& left) const
	{
		return base_type::pro_compare_left_greater_eq(left);
	}

	template<typename OT> inline
	bool compare_greater_eq(const flaw_comparer<OT, flaw_genner_type>& right) const
	{
		return base_type::pro_compare_greater_eq(right);
	}
};

// non-member foo
// == 
template<typename T1, typename T2, typename F> inline
bool operator==(const flaw_comparer<T1, F>& l, const T2& r)
{
	return l.compare_eq(r);
}

template<typename T1, typename T2, typename F> inline
bool operator==(const T1& l, const flaw_comparer<T2, F>& r)
{
	return r.compare_eq(l);
}

template<typename T1, typename T2, typename F> inline
bool operator==(const flaw_comparer<T1, F>& l, const flaw_comparer<T2, F>& r)
{
	return l.compare_eq(r);
}

// !=
template<typename T1, typename T2, typename F> inline
bool operator!=(const flaw_comparer<T1, F>& l, const T2& r)
{
	return !l.compare_eq(r);
}

template<typename T1, typename T2, typename F> inline
bool operator!=(const T1& l, const flaw_comparer<T2, F>& r)
{
	return !r.compare_eq(l);
}

template<typename T1, typename T2, typename F> inline
bool operator!=(const flaw_comparer<T1, F>& l, const flaw_comparer<T2, F>& r)
{
	return !l.compare_eq(r);
}

// <
template<typename T1, typename T2, typename F> inline
bool operator<(const flaw_comparer<T1, F>& l, const T2& r)
{
	return l.compare_less(r);
}

template<typename T1, typename T2, typename F> inline
bool operator<(const T1& l, const flaw_comparer<T2, F>& r)
{
	return r.compare_left_less(l);
}

template<typename T1, typename T2, typename F> inline
bool operator<(const flaw_comparer<T1, F>& l, const flaw_comparer<T2, F>& r)
{
	return l.compare_less(r);
}

// <= 
template<typename T1, typename T2, typename F> inline
bool operator<=(const flaw_comparer<T1, F>& l, const T2& r)
{
	return l.compare_less_eq(r);
}

template<typename T1, typename T2, typename F> inline
bool operator<=(const T1& l, const flaw_comparer<T2, F>& r)
{
	return r.compare_left_less_eq(l);
}

template<typename T1, typename T2, typename F> inline
bool operator<=(const flaw_comparer<T1, F>& l, const flaw_comparer<T2, F>& r)
{
	return l.compare_less_eq(r);
}

// >
template<typename T1, typename T2, typename F> inline
bool operator>(const flaw_comparer<T1, F>& l, const T2& r)
{
	return l.compare_greater(r);
}

template<typename T1, typename T2, typename F> inline
bool operator>(const T1& l, const flaw_comparer<T2, F>& r)
{
	return r.compare_left_greater(l);
}

template<typename T1, typename T2, typename F> inline
bool operator>(const flaw_comparer<T1, F>& l, const flaw_comparer<T2, F>& r)
{
	return l.compare_greater(r);
}

// >
template<typename T1, typename T2, typename F> inline
bool operator>=(const flaw_comparer<T1, F>& l, const T2& r)
{
	return l.compare_greater_eq(r);
}

template<typename T1, typename T2, typename F> inline
bool operator>=(const T1& l, const flaw_comparer<T2, F>& r)
{
	return r.compare_left_greater_eq(l);
}

template<typename T1, typename T2, typename F> inline
bool operator>=(const flaw_comparer<T1, F>& l, const flaw_comparer<T2, F>& r)
{
	return l.compare_greater_eq(r);
}

// flaw_comparer

#if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT(0x581) )
#	define YGGR_REF_CONST
#else
#	define YGGR_REF_CONST const
#endif // __BORLANDC__

template<typename T> inline
flaw_comparer<T> YGGR_REF_CONST 
	make_flaw_comparer(const T& t)
{
	return flaw_comparer<T>(t);
}

template<typename T, typename FlawGen>
flaw_comparer<T, FlawGen> YGGR_REF_CONST 
	make_flaw_comparer(const T& t, const FlawGen& gen)
{
	return flaw_comparer<T, FlawGen>(t, gen);
}

#undef YGGR_REF_CONST


} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_FLAW_COMPARER_HPP__