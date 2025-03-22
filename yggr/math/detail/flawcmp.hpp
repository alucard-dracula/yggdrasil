// flawcmp.hpp

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

#ifndef __YGGR_MATH_DETAIL_FLAWCMP_HPP__
#define __YGGR_MATH_DETAIL_FLAWCMP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/math/make_zero_value.hpp>
#include <yggr/type_traits/has_value_t.hpp>
#include <yggr/mplex/null_t.hpp>

#include <boost/ref.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/if.hpp>

#include <complex>


namespace yggr
{
namespace math
{
namespace detail
{

// basic_flawcmp_foo

template<typename L, typename R, typename FlawGen> inline
bool flawcmp_equal(const L& l, const R& r, const FlawGen& gen)
{
	return std::abs(l - r) < gen();
}

template<typename L, typename R, typename FlawGen> inline
bool flawcmp_not_equal(const L& l, const R& r, const FlawGen& gen)
{
	return !flawcmp_equal(l, r, gen);
}

template<typename L, typename R, typename FlawGen> inline
bool flawcmp_less(const L& l, const R& r, const FlawGen& gen)
{
	return flawcmp_not_equal(l, r, gen) && (l < r);
}

template<typename L, typename R, typename FlawGen> inline
bool flawcmp_less_equal(const L& l, const R& r, const FlawGen& gen)
{
	return flawcmp_equal(l, r, gen) || (l < r);
}

template<typename L, typename R, typename FlawGen> inline
bool flawcmp_greater(const L& l, const R& r, const FlawGen& gen)
{
	return flawcmp_not_equal(l, r, gen) && (l > r);
}

template<typename L, typename R, typename FlawGen> inline
bool flawcmp_greater_equal(const L& l, const R& r, const FlawGen& gen)
{
	return flawcmp_equal(l, r, gen) || (l > r);
}

} // namespace detail
} // namespace math
} // namespace yggr

namespace yggr
{
namespace math
{
namespace detail
{

template<typename T, typename FlawGen> 
class flaw_comparer_impl;

} // namespace detail
} // namespace math
} // namespace yggr

namespace yggr
{
namespace math
{
namespace detail
{

template<typename Container>
struct _has_flaw_genner_t 
{
private:
	typedef Container cont_type;

	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::flaw_genner_type>::type*); // If T's parent defines the value_type then it is true
		
	template<typename T>
	static false_type chk_foo(...);
		
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type;
};

template<typename Container>
struct has_flaw_genner_t
	: public _has_flaw_genner_t<Container>::type
{
};

template<typename T, bool chk>
struct make_flaw_comparer_impl
	: public ::yggr::mplex::null_type
{
};

template<typename T>
struct make_flaw_comparer_impl<T, true>
{
	typedef 
		flaw_comparer_impl
		<
			typename T::value_type,
			typename T::flaw_genner_type
		> type;
};

template<typename T>
struct is_flaw_comparer
	: public 
		boost::mpl::if_
		<
			boost::is_base_of<mplex::null_type, T>,
			boost::mpl::false_,
			typename 
				boost::is_base_of
				<
					typename 
						make_flaw_comparer_impl
						<
							T, 
							boost::mpl::and_
							<
								::yggr::has_value_t<T>,
								has_flaw_genner_t<T>
							>::type::value
						>::type,
					T
				>::type
		>::type
{
};

} // namespace detail
} // namespace math
} // namespace yggr

namespace yggr
{
namespace math
{
namespace detail
{

// class flaw_comparer_impl

template<typename T, typename FlawGen> 
class flaw_comparer_impl
{
protected:
	typedef T value_type;
	typedef FlawGen flaw_genner_type;

protected:
	template<typename OT, typename OFlawGen>
	friend class flaw_comparer_impl;

private:
	typedef boost::reference_wrapper<value_type const> value_cref_type;
	typedef boost::reference_wrapper<flaw_genner_type const> flaw_genner_cref_type;

private:
	typedef flaw_comparer_impl this_type;

protected:
	template<typename Gen>
	flaw_comparer_impl(const value_type& val, const Gen& gen)
		: _val(val), _flaw_gen(gen)
	{
	}

	flaw_comparer_impl(const this_type& right)
		: _val(right._val), _flaw_gen(right._flaw_gen)
	{
	}

	~flaw_comparer_impl(void)
	{
	}

protected:
	
	// > >= < <= == != opbool

	//operator bool
	
	inline bool pro_operator_bool(void) const
	{
		static const value_type zero = make_zero_value<value_type>();
		return !flawcmp_equal(this_type::pro_value_cref(), zero, this_type::pro_flaw_gen_cref());
	}

	// operator ==
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_eq(const OVal& right) const
	{
		return flawcmp_equal(this_type::pro_value_cref(), right, this_type::pro_flaw_gen_cref());
	}

	template<typename OT> inline
	bool pro_compare_eq(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		//assert(this_type::pro_flaw_gen_cref()() == right.this_type::pro_flaw_gen_cref()());
		assert((this_type::pro_flaw_gen_cref())() == (right.pro_flaw_gen_cref())());
		return flawcmp_equal(this_type::pro_value_cref(), right.pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	//operator<
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_less(const OVal& right) const
	{
		return flawcmp_less(this_type::pro_value_cref(), right, this_type::pro_flaw_gen_cref());
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_less(const OVal& left) const
	{
		return flawcmp_less(left, this_type::pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	template<typename OT> inline
	bool pro_compare_less(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		//assert(this_type::pro_flaw_gen_cref()() == right.this_type::pro_flaw_gen_cref()());
		assert((this_type::pro_flaw_gen_cref())() == (right.pro_flaw_gen_cref())());
		return flawcmp_less(this_type::pro_value_cref(), right.pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	//operator<=
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_less_eq(const OVal& right) const
	{
		return flawcmp_less_equal(this_type::pro_value_cref(), right, this_type::pro_flaw_gen_cref());
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_less_eq(const OVal& left) const
	{
		return flawcmp_less_equal(left, this_type::pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	template<typename OT> inline
	bool pro_compare_less_eq(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		//assert(this_type::pro_flaw_gen_cref()() == right.this_type::pro_flaw_gen_cref()());
		assert((this_type::pro_flaw_gen_cref())() == (right.pro_flaw_gen_cref())());
		return flawcmp_less_equal(this_type::pro_value_cref(), right.pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	// operator>
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_greater(const OVal& right) const
	{
		return flawcmp_greater(this_type::pro_value_cref(), right, this_type::pro_flaw_gen_cref());
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_greater(const OVal& left) const
	{
		return flawcmp_greater(left, this_type::pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	template<typename OT> inline
	bool pro_compare_greater(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		//assert(this_type::pro_flaw_gen_cref()() == right.this_type::pro_flaw_gen_cref()());
		assert((this_type::pro_flaw_gen_cref())() == (right.pro_flaw_gen_cref())());
		return flawcmp_greater(this_type::pro_value_cref(), right.pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	// operator>=
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_greater_eq(const OVal& right) const
	{
		return flawcmp_greater_equal(this_type::pro_value_cref(), right, this_type::pro_flaw_gen_cref());
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_greater_eq(const OVal& left) const
	{
		return flawcmp_greater_equal(left, this_type::pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

	template<typename OT> inline
	bool pro_compare_greater_eq(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		//assert(this_type::pro_flaw_gen_cref()() == right.this_type::pro_flaw_gen_cref()());
		assert((this_type::pro_flaw_gen_cref())() == (right.pro_flaw_gen_cref())());
		return flawcmp_greater_equal(this_type::pro_value_cref(), right.pro_value_cref(), this_type::pro_flaw_gen_cref());
	}

protected:
	inline const value_type& pro_value_cref(void) const
	{
		return _val;
	}

	inline const flaw_genner_type& pro_flaw_gen_cref(void) const
	{
		return _flaw_gen;
	}

protected:
	value_cref_type _val; 
	flaw_genner_cref_type _flaw_gen;
};

// class flaw_comparer_impl<T, mplex::null_type>

template<typename T> 
class flaw_comparer_impl<T, mplex::null_type>
{
protected:
	typedef T value_type;
	typedef mplex::null_type flaw_genner_type;

protected:
	template<typename OT, typename OFlawGen>
	friend class flaw_comparer_impl;

private:
	typedef boost::reference_wrapper<value_type const> value_cref_type;

private:
	typedef flaw_comparer_impl this_type;

protected:
	template<typename FakeGen>
	flaw_comparer_impl(const value_type& val, const FakeGen&)
		:_val(val)
	{
	}

	flaw_comparer_impl(const this_type& right)
		: _val(right._val)
	{
	}

	~flaw_comparer_impl(void)
	{
	}

protected:
	
	// > >= < <= == != opbool

	//operator bool
	
	inline bool pro_operator_bool(void) const
	{
		return !!this_type::pro_value_cref();
	}

	// operator ==
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_eq(const OVal& right) const
	{
		return this_type::pro_value_cref() == right;
	}

	template<typename OT> inline
	bool pro_compare_eq(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		return this_type::pro_value_cref() == right.pro_value_cref();
	}

	//operator<
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_less(const OVal& right) const
	{
		return this_type::pro_value_cref() < right;
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_less(const OVal& left) const
	{
		return left < this_type::pro_value_cref();
	}

	template<typename OT> inline
	bool pro_compare_less(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		return this_type::pro_value_cref() < right.pro_value_cref();
	}

	//operator<=
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_less_eq(const OVal& right) const
	{
		return this_type::pro_value_cref() <= right;
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_less_eq(const OVal& left) const
	{
		return this_type::pro_value_cref() <= this_type::pro_value_cref();
	}

	template<typename OT> inline
	bool pro_compare_less_eq(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		return this_type::pro_value_cref() <= right.pro_value_cref();
	}

	// operator>
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_greater(const OVal& right) const
	{
		return this_type::pro_value_cref() > right;
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_greater(const OVal& left) const
	{
		return left > this_type::pro_value_cref();
	}

	template<typename OT> inline
	bool pro_compare_greater(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		return this_type::pro_value_cref() > right.pro_value_cref();
	}

	// operator>=
	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_greater_eq(const OVal& right) const
	{
		return this_type::pro_value_cref() >= right;
	}

	template<typename OVal> inline
	typename boost::disable_if<is_flaw_comparer<OVal>, bool>::type
		pro_compare_left_greater_eq(const OVal& left) const
	{
		return left >= this_type::pro_value_cref();
	}

	template<typename OT> inline
	bool pro_compare_greater_eq(const flaw_comparer_impl<OT, flaw_genner_type>& right) const
	{
		return this_type::pro_value_cref() >= right.pro_value_cref();
	}

protected:
	inline const value_type& pro_value_cref(void) const
	{
		return _val;
	}

protected:
	value_cref_type _val; 
};

} // namespace detail
} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_DETAIL_FLAWCMP_HPP__