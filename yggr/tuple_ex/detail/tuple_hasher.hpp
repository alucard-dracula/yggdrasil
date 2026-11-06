// tuple_hasher.hpp

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

#ifndef __YGGR_TUPLE_EX_DETAIL_TUPLE_HASHER_HPP__
#define __YGGR_TUPLE_EX_DETAIL_TUPLE_HASHER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/tuple_comparison.hpp>
#include <yggr/tuple/tuple_io.hpp>

#include <boost/functional/hash/hash.hpp>

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	include <tuple>
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{
namespace tuples
{

template<typename T>
struct cons_hash_helper;


template<typename HH, typename TT,
			template<typename _HH, typename _TT> class Cons>
struct cons_hash_helper< Cons< HH, TT> >
{
	typedef Cons< HH, TT > value_type;

	inline void operator()(std::size_t& seed, const value_type& val) const
	{
		typedef cons_hash_helper<typename value_type::tail_type> next_helper_type;

		boost::hash_combine(seed, val.get_head());
		next_helper_type h;
		h(seed, val.get_tail());
	}
};

template<typename HH,
			template<typename _HH, typename _TT> class Cons>
struct cons_hash_helper< Cons<HH, null_type> >
{
	typedef Cons<HH, null_type> value_type;

	inline void operator()(std::size_t& seed, const value_type& val) const
	{
		boost::hash_combine(seed, val.get_head());
	}
};

template<>
struct cons_hash_helper< null_type >
{
	typedef null_type value_type;

	inline void operator()(std::size_t& seed, const value_type& ) const
	{
		boost::hash<std::string> hasher;
		seed = hasher(std::string());
	}
};

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace detail
{

template<std::size_t I, std::size_t ISize>
struct std_tuple_hash_helper
{
	template<typename Tuple> inline
	void operator()(std::size_t& seed, const Tuple& tuple) const
	{
		typedef std_tuple_hash_helper<I + 1, ISize> next_helper_type;

		boost::hash_combine(seed, yggr::get<I>(tuple));
		next_helper_type h;
		h(seed, tuple);
	}
};

template<std::size_t ISize>
struct std_tuple_hash_helper<ISize, ISize>
{
	template<typename Tuple> inline
	void operator()(std::size_t& seed, const Tuple& tuple) const
	{
	}
};

} // namespace detail

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

} // namespace tuples
} // namespace yggr

namespace boost
{

// boost tuple hash
template<typename HH, typename TT>
struct hash< tuples::cons<HH, TT> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::cons<HH, TT>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::cons<HH, TT> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< tuples::tuple<Args...> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::tuple<Args...>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::tuple<Args...> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)


template<typename ...Args>
struct hash< std::tuple<Args...> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< std::tuple<Args...>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
public:
	typedef std::tuple<Args...> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

public:
	inline std::size_t operator()(const value_type& val) const
	{
		typedef
			::yggr::tuples::detail::std_tuple_hash_helper
			<
				0,
				::yggr::tuple_size<value_type>::value
			> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#endif // YGGR_NO_CXX11_HDR_TUPLE

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> value_type;
	typedef const value_type& argument_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<>
struct hash< tuples::null_type >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::null_type, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::null_type value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

// yggr tuple hash
template<typename HH, typename TT>
struct hash< yggr::tuples::cons<HH, TT> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< yggr::tuples::cons<HH, TT>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef yggr::tuples::cons<HH, TT> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< yggr::tuples::tuple<Args...> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< yggr::tuples::tuple<Args...>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef yggr::tuples::tuple<Args...> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace boost

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
template<typename ...Args>
struct hash< std::tuple<Args...> >
{
public:
	typedef std::tuple<Args...> value_type;

public:
	inline std::size_t operator()(const value_type& val) const
	{
		typedef
			::yggr::tuples::detail::std_tuple_hash_helper
			<
				0,
				::yggr::tuple_size<value_type>::value
			> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#	endif // #	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#endif // YGGR_NO_CXX11_HDR_TUPLE

// boost tuple hash
template<typename HH, typename TT>
struct hash< ::boost::tuples::cons<HH, TT> >
{
	typedef ::boost::tuples::cons<HH, TT> type;

	inline std::size_t operator()(const type& val) const
	{
		boost::hash<type> hasher;
		return hasher(val);
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< ::boost::tuples::tuple<Args...> >
{
	typedef ::boost::tuples::tuple<Args...> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> >
{
	typedef ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<>
struct hash< ::boost::tuples::null_type >
{
	typedef ::boost::tuples::null_type value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

// yggr tuple hash
template<typename HH, typename TT>
struct hash< ::yggr::tuples::cons<HH, TT> >
{
	typedef ::yggr::tuples::cons<HH, TT> type;

	inline std::size_t operator()(const type& val) const
	{
		boost::hash<type> hasher;
		return hasher(val);
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< yggr::tuples::tuple<Args...> >
{
	typedef yggr::tuples::tuple<Args...> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> >
{
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_TUPLE_EX_DETAIL_TUPLE_HASHER_HPP__
