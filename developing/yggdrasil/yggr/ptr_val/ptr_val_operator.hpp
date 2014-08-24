//ptr_val_operator.hpp

#ifndef __YGGR_PTR_VAL_PTR_VAL_OPERATOR_HPP__
#define __YGGR_PTR_VAL_PTR_VAL_OPERATOR_HPP__

#include <functional>
#include <cassert>
#include <boost/mpl/assert.hpp>
#include <yggr/mplex/pointer_checker.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

namespace yggr
{
namespace ptr_val
{

template<typename T>
struct ptr_val_negate
	: public std::unary_function<T, typename mplex::pointer_to_value_t<T>::type >
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T>));
	typedef typename mplex::pointer_to_value_t<T>::type result_type;
	typedef result_type l_value_type;

	result_type operator()(const T& l) const
	{
		assert(l);
		return std::negate<result_type>()(*l);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_plus
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	result_type operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) + (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_minus
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	result_type operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) - (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_multiplies
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	result_type operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) * (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_divides
	: public std::binary_function<T1, T2, typename mplex::pointer_to_value_t<T1>::type >
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef typename mplex::pointer_to_value_t<T1>::type result_type;
	typedef result_type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	result_type operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) / (*r);
	}
};

template<typename T>
struct ptr_val_modulus
	: public std::binary_function<T, T, typename mplex::pointer_to_value_t<T>::type >
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T>));
	typedef typename mplex::pointer_to_value_t<T>::type result_type;
	typedef result_type l_value_type;
	typedef result_type r_value_type;

	result_type operator()(const T& l, const T& r) const
	{
		assert(l && r);
		return std::modulus<result_type>()(*l, *r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_less
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) < (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_less_equal
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) <= (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_greater
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) > (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_greater_equal
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) >= (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_equal_to
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) == (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_not_equal_to
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) != (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_logical_and
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) && (*r);
	}
};

template<typename T1, typename T2 = T1>
struct ptr_val_logical_or
	: public std::binary_function<T1, T2, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T1>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T2>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T1>::type l_value_type;
	typedef typename mplex::pointer_to_value_t<T2>::type r_value_type;

	bool operator()(const T1& l, const T2& r) const
	{
		assert(l && r);
		return (*l) || (*r);
	}
};

template<typename T>
struct ptr_val_logical_not
	: public std::unary_function<T, bool>
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<T>));
	typedef bool result_type;
	typedef typename mplex::pointer_to_value_t<T>::type l_value_type;

	bool operator()(const T& l) const
	{
		assert(l);
		return !(*l);
	}
};

} // namespace ptr_val
} // namespace yggr

#endif //__YGGR_PTR_VAL_OPERATOR_PTR_VAL_OPERATOR_HPP__
