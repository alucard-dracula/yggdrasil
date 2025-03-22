//array.hpp

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

#ifndef __YGGR_CONTAINER_EX_ARRAY_HPP__
#define __YGGR_CONTAINER_EX_ARRAY_HPP__

#include <yggr/base/static_constant.hpp>
#include <yggr/container/array.hpp>

#include <yggr/move/move.hpp>
#include <yggr/type_traits/traits.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/index_t_value.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/bytes/byte_size.hpp>
#include <yggr/tuple/tuple_utility_decl.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/remove_reference.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/range/functions.hpp>


#include <algorithm>

namespace yggr
{
namespace container_ex
{

// !!! gcc not support direct BOOST_MPL_ASSERT((false)) and static_assert(false), so not impl it !!!

#if defined(_MSC_VER)

template<typename T>
class native_array_ref
{
	BOOST_MPL_ASSERT((boost::mpl::false_));
};

template<typename T>
class native_array_cref
{
	BOOST_MPL_ASSERT((boost::mpl::false_));
};

#else

template<typename T>
class native_array_ref;

template<typename T>
class native_array_cref;

#endif // _MSC_VER

} // namespace container_ex
} // namespace yggr

namespace yggr
{
namespace container_ex
{

//-----------------native_array_ref----------------------

template<typename Tx, std::size_t N>
class native_array_ref<Tx[N]>
{
public:
	YGGR_STATIC_CONSTANT(std::size_t, E_size = N);

	typedef Tx type[N];
	typedef Tx value_type;

private:
	typedef native_array_ref this_type;

public:
	native_array_ref(type& arr)
		: _arr(arr)
	{
	}

	~native_array_ref(void)
	{
	}

public:
	YGGR_CONSTEXPR_OR_INLINE operator type&(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _arr;
	}

	YGGR_CONSTEXPR_OR_INLINE operator const type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _arr;
	}

	YGGR_CONSTEXPR_OR_INLINE operator value_type*(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return boost::addressof(_arr[0]);
	}

	YGGR_CONSTEXPR_OR_INLINE operator const value_type*(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return boost::addressof(_arr[0]);
	}

	YGGR_CONSTEXPR_OR_INLINE operator void*(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<void*>(boost::addressof(_arr[0]));
	}

	YGGR_CONSTEXPR_OR_INLINE operator const void*(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const void*>(boost::addressof(_arr[0]));
	}

	template<typename T> YGGR_CONSTEXPR_OR_INLINE
	operator T*(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<T*>(boost::addressof(_arr[0]));
	}

	template<typename T> YGGR_CONSTEXPR_OR_INLINE
	operator const T*(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const T*>(boost::addressof(_arr[0]));
	}

	YGGR_CONSTEXPR_OR_INLINE static std::size_t size(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return E_size;
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return (boost::begin(_arr) == boost::begin(right))
				|| std::equal(boost::begin(_arr), boost::end(_arr), boost::begin(right));
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare_eq(right._arr);
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const native_array_cref<value_type[E_size]>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		const type& arr_cref = right;
		return this_type::compare_eq(arr_cref);
	}

	YGGR_CONSTEXPR_OR_INLINE s32 compare(const type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			(boost::begin(_arr) == boost::begin(right))?
				0
				: (std::equal(boost::begin(_arr), boost::end(_arr), boost::begin(right))?
					0
					: std::lexicographical_compare(
						boost::begin(_arr), boost::end(_arr),
						boost::begin(right), boost::end(right))? -1 : 1);
	}

	YGGR_CONSTEXPR_OR_INLINE s32 compare(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right._arr);
	}

	YGGR_CONSTEXPR_OR_INLINE s32 compare(const native_array_cref<value_type[E_size]>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		const type& arr_cref = right;
		return this_type::compare(arr_cref);
	}

private:
	type& _arr;
};

// operator==
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const native_array_ref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(T(&l)[N], const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return r.compare_eq(l);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const native_array_ref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

// operator!=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const native_array_ref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(l.compare_eq(r));
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(T(&l)[N], const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare_eq(l));
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const native_array_ref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(l.compare_eq(r));
}

// operator <
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(const native_array_ref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(T(&l)[N], const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return 0 < r.compare(l);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(const native_array_ref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

//operator<=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(const native_array_ref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(T(&l)[N], const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) < 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(const native_array_ref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

// operator>
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(const native_array_ref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(T(&l)[N], const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) >= 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(const native_array_ref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

// operator>=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(const native_array_ref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(T(&l)[N], const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) > 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(const native_array_ref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

//-----------------native_array_cref----------------------

template<typename Tx, std::size_t N>
class native_array_cref<Tx[N]>
{
public:
	YGGR_STATIC_CONSTANT(std::size_t, E_size = N);

	typedef Tx type[N];
	typedef Tx value_type;

private:
	typedef native_array_cref this_type;

public:
	native_array_cref(const type& arr)
		: _arr(arr)
	{
	}

	~native_array_cref(void)
	{
	}

public:
	YGGR_CONSTEXPR_OR_INLINE operator const type&(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return _arr;
	}

	YGGR_CONSTEXPR_OR_INLINE operator const value_type*(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return boost::addressof(_arr[0]);
	}

	YGGR_CONSTEXPR_OR_INLINE operator const void*(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const void*>(boost::addressof(_arr[0]));
	}

	template<typename T> YGGR_CONSTEXPR_OR_INLINE
	operator const T*(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const T*>(boost::addressof(_arr[0]));
	}

	YGGR_CONSTEXPR_OR_INLINE static std::size_t size(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return E_size;
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			(boost::begin(_arr) == boost::begin(right))
			|| std::equal(boost::begin(_arr), boost::end(_arr), boost::begin(right));
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare_eq(right._arr);
	}

	YGGR_CONSTEXPR_OR_INLINE bool compare_eq(const native_array_ref<value_type[E_size]>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		const type& arr_cref = right;
		return this_type::compare_eq(arr_cref);
	}

	YGGR_CONSTEXPR_OR_INLINE s32 compare(const type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return
			(boost::begin(_arr) == boost::begin(right))? 0
			: (std::equal(boost::begin(_arr), boost::end(_arr), boost::begin(right))? 0
				: std::lexicographical_compare(
					boost::begin(_arr), boost::end(_arr),
					boost::begin(right), boost::end(right))? -1 : 1);
	}

	YGGR_CONSTEXPR_OR_INLINE s32 compare(const this_type& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right._arr);
	}

	YGGR_CONSTEXPR_OR_INLINE s32 compare(const native_array_ref<value_type[E_size]>& right) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		const type& arr_cref = right;
		return this_type::compare(arr_cref);
	}

private:
	const type& _arr;
};

// operator==
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const native_array_cref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(T(&l)[N], const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return r.compare_eq(l);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const native_array_cref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare_eq(r);
}

// operator!=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const native_array_cref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(l.compare_eq(r));
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(T(&l)[N], const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare_eq(l));
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const native_array_cref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(l.compare_eq(r));
}

// operator <
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(const native_array_cref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(T(&l)[N], const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) <= 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(const native_array_cref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

//operator<=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(const native_array_cref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(T(&l)[N], const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) < 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(const native_array_cref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

// operator>
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(const native_array_cref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(T(&l)[N], const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) >= 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(const native_array_cref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

// operator>=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(const native_array_cref<T[N]>& l, T(&r)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(T(&l)[N], const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return !(r.compare(l) > 0);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(const native_array_cref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

} // namespace container_ex
} // namesapce yggr

//--------------fusion_compare---------------

namespace yggr
{
namespace container_ex
{

// operator==
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const native_array_ref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) == 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator==(const native_array_cref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) == 0;
}

//operator !=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const native_array_ref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) != 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator!=(const native_array_cref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) != 0;
}

// operator<
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(const native_array_ref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<(const native_array_cref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) < 0;
}

// operator<=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(const native_array_ref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator<=(const native_array_cref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) <= 0;
}

// operator>
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(const native_array_ref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>(const native_array_cref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) > 0;
}

// operator>=
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(const native_array_ref<T[N]>& l, const native_array_cref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
bool operator>=(const native_array_cref<T[N]>& l, const native_array_ref<T[N]>& r) YGGR_NOEXCEPT_OR_NOTHROW
{
	return l.compare(r) >= 0;
}

} // namespace container_ex
} // namesapce yggr

//--------------ref_make_function-----------------
namespace yggr
{
namespace container_ex
{

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
native_array_ref<T[N]> make_array_ref(T(&arr)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return native_array_ref<T[N]>(arr);
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
native_array_ref<T[N]> make_array_cref(const T(&arr)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	return native_array_cref<T[N]>(arr);
}

#ifndef YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
std::array<T, N>& make_array_ref(std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const std::array<T, N>& make_array_cref(const std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr;
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
boost::array<T, N>& make_array_ref(boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr;
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const boost::array<T, N>& make_array_cref(const boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr;
}

} // namespace container_ex
} // namesapce yggr

//--------- yggr::get support--------------
namespace yggr
{
namespace container_ex
{

// get native array
template<yggr::size_type I, typename Val, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
Val& get(Val(&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return a[I];
}

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#	if !defined(_MSC_VER) || !(_MSC_VER < 1700)

template<yggr::size_type I, typename Val, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
BOOST_RV_REF(Val) get(Val(&&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Val(array_type)[N];
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return YGGR_MOVE_RET(Val, (static_cast<array_type&>(a))[I]);
}

#	endif // !defined(_MSC_VER) || !(_MSC_VER < 1700)
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

template<yggr::size_type I, typename Val, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const Val& get(const Val(&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return a[I];
}

template<typename Index, typename Val, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
Val& get(Val(&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, Index) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, Index)];
}

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#	if !defined(_MSC_VER) || !(_MSC_VER < 1700)

template<typename Index, typename Val, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
BOOST_RV_REF(Val) get(Val(&&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Val(array_type)[N];

	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, Index) < N)>));
	return YGGR_MOVE_RET(Val, (static_cast<array_type&>(a))[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, Index)]);
}

#	endif // !defined(_MSC_VER) || !(_MSC_VER < 1700)
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

template<typename Index, typename Val, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const Val& get(const Val(&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, Index) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, Index)];
}

// yggr::get<I>(std::array)
#if !defined(YGGR_NO_CXX11_HDR_ARRAY)

//using std::get; // don't add using std::get, because boost is fixed

// size_t<I>
template<typename IDX, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get(std::array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)];
}

//template<typename IDX, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
//BOOST_RV_REF(T) get(BOOST_RV_REF_BEG std::array<T, N> BOOST_RV_REF_END  a) YGGR_NOEXCEPT_OR_NOTHROW
//{
//	typedef std::array<T, N> array_type;
//
//	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
//	return YGGR_MOVE_RET(T, (static_cast<array_type&>(a))[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)]);
//}

template<typename IDX, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get(const std::array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)];
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

// yggr::get<I>(boost::array)
template<yggr::size_type I, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get(boost::array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return a[I];
}

//template<yggr::size_type I, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
//BOOST_RV_REF(T) get(BOOST_RV_REF_BEG boost::array<T, N> BOOST_RV_REF_END  a) YGGR_NOEXCEPT_OR_NOTHROW
//{
//	typedef boost::array<T, N> array_type;
//
//	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
//	return YGGR_MOVE_RET(T, (static_cast<array_type&>(a))[I]);
//}

template<yggr::size_type I, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
 const T& get(const boost::array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return a[I];
}

template<typename IDX, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get(boost::array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)];
}

//template<typename IDX, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
//BOOST_RV_REF(T) get(BOOST_RV_REF_BEG boost::array<T, N> BOOST_RV_REF_END  a) YGGR_NOEXCEPT_OR_NOTHROW
//{
//	typedef boost::array<T, N> array_type;
//
//	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
//	return YGGR_MOVE_RET(T, (static_cast<array_type&>(a))[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)]);
//}

template<typename IDX, typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get(const boost::array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)];
}

// yggr::get<I>(other_array)

template<yggr::size_type I,
			template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get(Array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return a[I];
}

//template<yggr::size_type I,
//			template<typename _T, std::size_t _N> class Array,
//			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
//BOOST_RV_REF(T) get(BOOST_RV_REF_BEG Array<T, N> BOOST_RV_REF_END  a) YGGR_NOEXCEPT_OR_NOTHROW
//{
//	typedef Array<T, N> array_type;
//
//	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
//	return YGGR_MOVE_RET(T, (static_cast<array_type&>(a))[I]);
//}

template<yggr::size_type I,
			template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get(const Array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
	return a[I];
}

template<typename IDX,
			template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get(Array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)];
}

//template<typename IDX,
//			template<typename _T, std::size_t _N> class Array,
//			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
//BOOST_RV_REF(T) get(BOOST_RV_REF_BEG Array<T, N> BOOST_RV_REF_END  a) YGGR_NOEXCEPT_OR_NOTHROW
//{
//	typedef Array<T, N> array_type;
//
//	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
//	return YGGR_MOVE_RET(T, (static_cast<array_type&>(a))[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)]);
//}

template<typename IDX,
			template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get(const Array<T, N>& a) YGGR_NOEXCEPT_OR_NOTHROW
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX) < N)>));
	return a[YGGR_INDEX_T_VALUE_GET_CAST(yggr::size_type, IDX)];
}

} // namespace container_ex

using container_ex::get;

} // namespace yggr

namespace yggr
{
namespace container_ex
{
namespace detail
{

// to_array_impl

template<std::size_t I, typename LArr, typename RArr>
struct array_copyer_impl;

template<std::size_t I,
			template<typename _T, std::size_t _N> class Array,
			typename LT, std::size_t LN,
			typename RT, std::size_t RN>
struct array_copyer_impl<I, Array<LT, LN>, RT[RN] >
	: public array_copyer_impl<I + 1, Array<LT, LN>, RT[RN] >
{
public:
	typedef array_copyer_impl<I + 1, Array<LT, LN>, RT[RN] > base_type;

public:
	typedef boost::mpl::size_t<I> now_index_type;

	typedef Array<LT, LN> left_array_type;
	typedef boost::mpl::size_t<LN> left_array_size_type;

	typedef RT(right_array_type)[RN];
	typedef boost::mpl::size_t<RN> right_array_size_type;

public:

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) )
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, const right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		l[now_index_type::value] = boost::move(r[now_index_type::value]);
		return (static_cast<const base_type&>(*this))(l, r);
	}
#else
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, BOOST_FWD_REF(right_array_type) r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		l[now_index_type::value] = boost::move(r[now_index_type::value]);
		return (static_cast<const base_type&>(*this))(l, boost::move(r));
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		l[now_index_type::value] = r[now_index_type::value];
		return (static_cast<const base_type&>(*this))(l, r);
	}
};

template<template<typename _T, std::size_t _N> class Array,
			typename LT, std::size_t LN,
			typename RT, std::size_t RN>
struct array_copyer_impl<LN, Array<LT, LN>, RT[RN] >
	: public mplex::null_type
{
public:
	typedef mplex::null_type base_type;

public:
	typedef boost::mpl::size_t<LN> now_index_type;

	typedef Array<LT, LN> left_array_type;
	typedef boost::mpl::size_t<LN> left_array_size_type;

	typedef RT(right_array_type)[RN];
	typedef boost::mpl::size_t<RN> right_array_size_type;

public:

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) )
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, const right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
#else
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, BOOST_FWD_REF(right_array_type) r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
};

template<template<typename _T, std::size_t _N> class Array,
			typename LT, std::size_t LN,
			typename RT, std::size_t RN>
struct array_copyer_impl<RN, Array<LT, LN>, RT[RN] >
	: public mplex::null_type
{
public:
	typedef mplex::null_type base_type;

public:
	typedef boost::mpl::size_t<RN> now_index_type;

	typedef Array<LT, LN> left_array_type;
	typedef boost::mpl::size_t<LN> left_array_size_type;

	typedef RT(right_array_type)[RN];
	typedef boost::mpl::size_t<RN> right_array_size_type;

public:

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) )
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, const right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
#else
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, BOOST_FWD_REF(right_array_type) r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
};

template<template<typename _T, std::size_t _N> class Array,
			typename LT, /*std::size_t LN,*/
			typename RT, std::size_t RN>
struct array_copyer_impl<RN, Array<LT, RN>, RT[RN] >
	: public mplex::null_type
{
public:
	typedef mplex::null_type base_type;

public:
	typedef boost::mpl::size_t<RN> now_index_type;

	typedef Array<LT, RN> left_array_type;
	typedef boost::mpl::size_t<RN> left_array_size_type;

	typedef RT(right_array_type)[RN];
	typedef boost::mpl::size_t<RN> right_array_size_type;

public:

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) )
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, const right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
#else
	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, BOOST_FWD_REF(right_array_type) r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES


	YGGR_CONSTEXPR_OR_INLINE
	left_array_type& operator()(left_array_type& l, right_array_type& r) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return l;
	}
};

template<typename LArr, typename RArr>
struct array_copyer;

template<template<typename _T, std::size_t _N> class Array,
			typename LT, std::size_t LN,
			typename RT, std::size_t RN>
struct array_copyer<Array<LT, LN>, RT[RN] >
	: public array_copyer_impl<0, Array<LT, LN>, RT[RN] >
{
};

} // namespace detail

// to_array

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
yggr::array<typename boost::remove_cv<T>::type, N>
	to_array(T (&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef yggr::array<typename boost::remove_cv<T>::type, N> larr_type;
	typedef T(rarr_type)[N];
	typedef detail::array_copyer<larr_type, rarr_type> arr_setter_type;

	arr_setter_type setter;
	larr_type larr;
	return setter(larr, a);
}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) )

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
yggr::array<typename boost::remove_cv<T>::type, N>
	to_array(const T (&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef yggr::array<typename boost::remove_cv<T>::type, N> larr_type;
	typedef T(rarr_type)[N];
	typedef detail::array_copyer<larr_type, rarr_type> arr_setter_type;

	arr_setter_type setter;
	larr_type larr;
	return setter(larr, a);
}

#else

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
yggr::array<typename boost::remove_cv<T>::type, N>
	to_array(T (&&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef yggr::array<typename boost::remove_cv<T>::type, N> larr_type;
	typedef T(rarr_type)[N];
	typedef detail::array_copyer<larr_type, rarr_type> arr_setter_type;

	arr_setter_type setter;
	larr_type larr;
	return setter(larr, boost::move(a));
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

// to_array ex

template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
Array<typename boost::remove_cv<T>::type, N>
	to_array(T (&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Array<typename boost::remove_cv<T>::type, N> larr_type;
	typedef T(rarr_type)[N];
	typedef detail::array_copyer<larr_type, rarr_type> arr_setter_type;

	arr_setter_type setter;
	larr_type larr;
	return setter(larr, a);
}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) )

template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
Array<typename boost::remove_cv<T>::type, N>
	to_array(const T (&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Array<typename boost::remove_cv<T>::type, N> larr_type;
	typedef T(rarr_type)[N];
	typedef detail::array_copyer<larr_type, rarr_type> arr_setter_type;

	arr_setter_type setter;
	larr_type larr;
	return setter(larr, a);
}

#else

template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
Array<typename boost::remove_cv<T>::type, N>
	to_array(T (&&a)[N]) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Array<typename boost::remove_cv<T>::type, N> larr_type;
	typedef T(rarr_type)[N];
	typedef detail::array_copyer<larr_type, rarr_type> arr_setter_type;

	arr_setter_type setter;
	larr_type larr;
	return setter(larr, boost::move(a));
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

} // namespace container_ex

using container_ex::to_array;

} // namespace yggr

// fill

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<std::size_t I, typename Array>
struct array_filler_impl;

// native array
template<std::size_t I, typename T, std::size_t N>
struct array_filler_impl<I, T[N] >
	: public array_filler_impl<I + 1, T[N] >
{
public:
	typedef array_filler_impl<I + 1, T[N] > base_type;

public:
	typedef boost::mpl::size_t<I> now_index_type;
	typedef T(array_type)[N];

public:
	template<typename Val> YGGR_CONSTEXPR_OR_INLINE
	array_type& operator()(array_type& a, const Val& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		a[now_index_type::value] = val;
		return (static_cast<const base_type&>(*this))(a, val);
	}
};

template<typename T, std::size_t N>
struct array_filler_impl<N, T[N] >
	: public mplex::null_t
{
public:
	typedef mplex::null_t base_type;

public:
	typedef boost::mpl::size_t<N> now_index_type;
	typedef T(array_type)[N];

public:
	template<typename Val> YGGR_CONSTEXPR_OR_INLINE
	array_type& operator()(array_type& a, const Val& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return a;
	}
};

// class array
template<std::size_t I,
			template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N>
struct array_filler_impl<I, Array<T, N> >
	: public array_filler_impl<I + 1, Array<T, N> >
{
public:
	typedef array_filler_impl<I + 1, Array<T, N> > base_type;

public:
	typedef boost::mpl::size_t<I> now_index_type;
	typedef Array<T, N> array_type;

public:
	template<typename Val> YGGR_CONSTEXPR_OR_INLINE
	array_type& operator()(array_type& a, const Val& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		a[now_index_type::value] = val;
		return (static_cast<const base_type&>(*this))(a, val);
	}
};

template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N>
struct array_filler_impl<N, Array<T, N> >
	: public mplex::null_type
{
public:
	typedef mplex::null_type base_type;

public:
	typedef boost::mpl::size_t<N> now_index_type;
	typedef Array<T, N> array_type;

public:
	template<typename Val> YGGR_CONSTEXPR_OR_INLINE
	array_type& operator()(array_type& a, const Val& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return a;
	}
};

template<typename Array>
struct array_filler
	: public array_filler_impl<0, Array>
{
};

} // namespace detail

// native_array
template<typename T, std::size_t N, typename Val> YGGR_CONSTEXPR_OR_INLINE
T(&fill(T (&a)[N], const Val& val))[N]
{
	typedef T(array_type)[N];
	typedef detail::array_filler<array_type> array_filler_type;

	array_filler_type filler;
	return filler(a, val);
}

// class array
template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N, typename Val> YGGR_CONSTEXPR_OR_INLINE
Array<T, N>& fill(Array<T, N>& a, const Val& val)
{
	typedef Array<T, N> array_type;
	typedef detail::array_filler<array_type> array_filler_type;

	array_filler_type filler;
	return filler(a, val);
}

} // namespace container_ex

using container_ex::fill;

} // namespace yggr

// tuple_utility
namespace yggr
{

// tuple_size
template<typename T, std::size_t N>
struct tuple_size< T[N] >
	: public boost::mpl::size_t<N>
{
};

template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N>
struct tuple_size<Array<T, N> >
	: public boost::mpl::size_t<N>
{
};

// tuple_element
template<std::size_t I, typename T, std::size_t N>
struct tuple_element<I, T[N] >
	: public traits<T>
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
};

template<std::size_t I,
			template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N>
struct tuple_element<I, Array<T, N> >
	: public traits<T>
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < N)>));
};

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_ARRAY_HPP__
