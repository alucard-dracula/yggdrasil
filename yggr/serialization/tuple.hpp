//tuple.hpp

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

#ifndef __YGGR_SERIALIZATION_TUPLE_HPP__
#define __YGGR_SERIALIZATION_TUPLE_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/tuple_ex/tuple.hpp>

#include <boost/serialization/split_free.hpp>

namespace yggr
{
namespace serialization
{

template<typename T>
struct cons_io_helper;

#define YGGR_CONS_IO_HELPER_CONS_DEF( __cons__ ) \
	template<typename HH, typename TT> \
	struct cons_io_helper< __cons__<HH, TT> > { \
		typedef __cons__<HH, TT> cons_type; \
		template<typename Archive> inline \
		Archive& operator()(Archive& ar, const cons_type& t) const { \
			typedef cons_io_helper<typename cons_type::tail_type> next_helper_type; \
			next_helper_type next_h; \
			ar << YGGR_SERIALIZATION_NAME_NVP("item", t.get_head()); \
			return next_h(ar, t.get_tail()); } \
		\
		template<typename Archive> inline \
		Archive& operator()(Archive& ar, cons_type& t) const { \
			typedef cons_io_helper<typename cons_type::tail_type> next_helper_type; \
			next_helper_type next_h; \
			ar >> YGGR_SERIALIZATION_NAME_NVP("item", t.get_head()); \
			return next_h(ar, t.get_tail()); } }; \
	\
	template<typename HH> \
	struct cons_io_helper< __cons__<HH, yggr::tuples::null_type> > { \
		typedef __cons__<HH, yggr::tuples::null_type> cons_type; \
		template<typename Archive> inline \
		Archive& operator()(Archive& ar, const cons_type& t) const { \
			ar << YGGR_SERIALIZATION_NAME_NVP("item", t.get_head()); \
			return ar; } \
		\
		template<typename Archive> inline \
		Archive& operator()(Archive& ar, cons_type& t) const { \
			ar >> YGGR_SERIALIZATION_NAME_NVP("item", t.get_head()); \
			return ar; } };

	YGGR_CONS_IO_HELPER_CONS_DEF(boost::tuples::cons);
	YGGR_CONS_IO_HELPER_CONS_DEF(yggr::tuples::cons);

#undef YGGR_CONS_IO_HELPER_CONS_DEF

//template<typename HH, typename TT,
//			template<typename _HH, typename _TT> class Cons>
//struct cons_io_helper< Cons<HH, TT> >
//{
//	typedef Cons<HH, TT> cons_type;
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, const cons_type& t) const
//	{
//		typedef cons_io_helper<typename cons_type::tail_type> next_helper_type;
//
//		next_helper_type next_h;
//
//		ar << YGGR_SERIALIZATION_NAME_NVP("item", t.get_head());
//		return next_h(ar, t.get_tail());
//	}
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, cons_type& t) const
//	{
//		typedef cons_io_helper<typename cons_type::tail_type> next_helper_type;
//
//		next_helper_type next_h;
//
//		ar >> YGGR_SERIALIZATION_NAME_NVP("item", t.get_head());
//		return next_h(ar, t.get_tail());
//	}
//};
//
//template<typename HH,
//			template<typename _HH, typename _TT> class Cons>
//struct cons_io_helper< Cons<HH, yggr::tuples::null_type> >
//{
//	typedef Cons<HH, yggr::tuples::null_type> cons_type;
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, const cons_type& t) const
//	{
//		ar << YGGR_SERIALIZATION_NAME_NVP("item", t.get_head());
//		return ar;
//	}
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, cons_type& t) const
//	{
//		ar >> YGGR_SERIALIZATION_NAME_NVP("item", t.get_head());
//		return ar;
//	}
//};

#define YGGR_CONS_IO_HELPER_TUPLES_DEF( __tuple__, __limit_length__ ) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__limit_length__, typename T)>  \
	struct cons_io_helper< __tuple__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__limit_length__, T) > > { \
		typedef __tuple__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__limit_length__, T) > tuple_type; \
		typedef typename tuple_type::inherited cons_type; \
		template<typename Archive> inline \
		Archive& operator()(Archive& ar, const tuple_type& t) const { \
			ar << YGGR_SERIALIZATION_NAME_NVP( \
					"tuple", boost::serialization::base_object< cons_type >(t)); \
			return ar; } \
		\
		template<typename Archive> inline \
		Archive& operator()(Archive& ar, tuple_type& t) const { \
			ar >> YGGR_SERIALIZATION_NAME_NVP( \
					"tuple", boost::serialization::base_object< cons_type >(t)); \
			return ar; } };


	YGGR_CONS_IO_HELPER_TUPLES_DEF(boost::tuples::tuple, BOOST_TUPLE_LIMIT_LENGTH());
	YGGR_CONS_IO_HELPER_TUPLES_DEF(yggr::tuples::tuple, YGGR_TUPLE_LIMIT_LENGTH());

#undef YGGR_CONS_IO_HELPER_TUPLES_DEF

//template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)>
//struct cons_io_helper< boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> >
//{
//	typedef ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> tuple_type;
//	typedef typename tuple_type::inherited cons_type;
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, const tuple_type& t) const
//	{
//		ar << YGGR_SERIALIZATION_NAME_NVP("tuple",
//										boost::serialization::base_object< cons_type >(t));
//		return ar;
//	}
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, tuple_type& t) const
//	{
//		ar >> YGGR_SERIALIZATION_NAME_NVP("tuple",
//										boost::serialization::base_object< cons_type >(t));
//		return ar;
//	}
//};
//
//template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)>
//struct cons_io_helper< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> >
//{
//	typedef ::yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> tuple_type;
//	typedef typename tuple_type::inherited cons_type;
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, const tuple_type& t) const
//	{
//		ar << YGGR_SERIALIZATION_NAME_NVP("tuple",
//										boost::serialization::base_object< cons_type >(t));
//		return ar;
//	}
//
//	template<typename Archive> inline
//	Archive& operator()(Archive& ar, tuple_type& t) const
//	{
//		ar >> YGGR_SERIALIZATION_NAME_NVP("tuple",
//										boost::serialization::base_object< cons_type >(t));
//		return ar;
//	}
//};

template<>
struct cons_io_helper< yggr::tuples::null_type >
{
	typedef ::yggr::tuples::null_type cons_type;

	template<typename Archive> inline
	Archive& operator()(Archive& ar, const cons_type&) const
	{
		return ar;
	}

	template<typename Archive> inline
	Archive& operator()(Archive& ar, cons_type&) const
	{
		return ar;
	}
};


#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

template<typename Tuple, std::size_t I, std::size_t ISize>
struct std_tuple_io_helper
{
	typedef Tuple tuple_type;

	template<typename Archive> inline
	Archive& operator()(Archive& ar, const tuple_type& t) const
	{
		typedef std_tuple_io_helper<tuple_type, I + 1, ISize> next_helper_type;

		next_helper_type next_h;
		//int n = 10;

		ar << YGGR_SERIALIZATION_NAME_NVP("item", (::yggr::get<I>(t)));
		return next_h(ar, t);
	}

	template<typename Archive> inline
	Archive& operator()(Archive& ar, tuple_type& t) const
	{
		typedef std_tuple_io_helper<tuple_type, I + 1, ISize> next_helper_type;

		next_helper_type next_h;

		ar >> YGGR_SERIALIZATION_NAME_NVP("item", (::yggr::get<I>(t)));
		return next_h(ar, t);
	}
};

template<typename Tuple, std::size_t ISize>
struct std_tuple_io_helper<Tuple, ISize, ISize>
{
	typedef Tuple tuple_type;

	template<typename Archive> inline
	Archive& operator()(Archive& ar, const tuple_type&) const
	{
		return ar;
	}

	template<typename Archive> inline
	Archive& operator()(Archive& ar, tuple_type&) const
	{
		return ar;
	}
};

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

} // namespace serialization
} // namespace yggr

namespace boost
{
namespace serialization
{

//---------------save---------
template<typename Archive, typename HH, typename TT> inline
void save(Archive& ar,
			const boost::tuples::cons<HH, TT>& t,
			const unsigned int ver)
{
	typedef boost::tuples::cons<HH, TT> cons_type;
	typedef yggr::serialization::cons_io_helper<cons_type> h_type;

	h_type h;

	yggr::ser_size_type count = boost::tuples::length<cons_type>::value;
	ar << YGGR_SERIALIZATION_NVP(count);
	h(ar, t);
}

template<typename Archive, typename HH, typename TT> inline
void save(Archive& ar,
			const yggr::tuples::cons<HH, TT>& t,
			const unsigned int ver)
{
	typedef yggr::tuples::cons<HH, TT> cons_type;
	typedef yggr::serialization::cons_io_helper<cons_type> h_type;

	h_type h;

	yggr::ser_size_type count = yggr::tuples::length<cons_type>::value;
	ar << YGGR_SERIALIZATION_NVP(count);
	h(ar, t);
}

template<typename Archive, YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH())> inline
void save(Archive& ar,
			const boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>& t,
			const unsigned int ver)
{
	typedef boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())> tuple_type;
	typedef yggr::serialization::cons_io_helper<tuple_type> h_type;

	h_type h;

	yggr::ser_size_type count = boost::tuples::length<tuple_type>::value;
	ar << YGGR_SERIALIZATION_NVP(count);
	h(ar, t);
}

template<typename Archive, YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
void save(Archive& ar,
			const yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& t,
			const unsigned int ver)
{
	typedef yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())> tuple_type;
	typedef yggr::serialization::cons_io_helper<tuple_type> h_type;

	h_type h;

	yggr::ser_size_type count = yggr::tuples::length<tuple_type>::value;
	ar << YGGR_SERIALIZATION_NVP(count);
	h(ar, t);
}

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

template<typename Archive, typename ...Args> inline
void save(Archive& ar, const std::tuple<Args...>& t, const unsigned int ver)
{
	typedef std::tuple<Args...> tuple_type;
	typedef
		yggr::serialization::std_tuple_io_helper
		<
			tuple_type,
			0,
			yggr::tuples::length<tuple_type>::value
		> h_type;

	h_type h;

	yggr::ser_size_type count = yggr::tuples::length<tuple_type>::value;
	ar << YGGR_SERIALIZATION_NVP(count);
	h(ar, t);
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

//---------------load---------
template<typename Archive, typename HH, typename TT> inline
void load(Archive& ar,
			boost::tuples::cons<HH, TT>& t,
			const unsigned int ver)
{
	typedef boost::tuples::cons<HH, TT> cons_type;
	typedef yggr::serialization::cons_io_helper<cons_type> h_type;

	h_type h;
	yggr::ser_size_type count = 0;

	ar >> YGGR_SERIALIZATION_NVP(count);
	assert(count == boost::tuples::length<cons_type>::value);
	h(ar, t);
}

template<typename Archive, typename HH, typename TT> inline
void load(Archive& ar,
			yggr::tuples::cons<HH, TT>& t,
			const unsigned int ver)
{
	typedef yggr::tuples::cons<HH, TT> cons_type;
	typedef yggr::serialization::cons_io_helper<cons_type> h_type;

	h_type h;
	yggr::ser_size_type count = 0;

	ar >> YGGR_SERIALIZATION_NVP(count);
	assert(count == yggr::tuples::length<cons_type>::value);
	h(ar, t);
}

template<typename Archive, YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH())> inline
void load(Archive& ar,
			boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>& t,
			const unsigned int ver)
{
	typedef boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())> tuple_type;
	typedef yggr::serialization::cons_io_helper<tuple_type> h_type;

	h_type h;
	yggr::ser_size_type count = 0;

	ar >> YGGR_SERIALIZATION_NVP(count);
	assert(count == boost::tuples::length<tuple_type>::value);
	h(ar, t);
}

template<typename Archive, YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
void load(Archive& ar,
			yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& t,
			const unsigned int ver)
{
	typedef yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())> tuple_type;
	typedef yggr::serialization::cons_io_helper<tuple_type> h_type;

	h_type h;
	yggr::ser_size_type count = 0;

	ar >> YGGR_SERIALIZATION_NVP(count);
	assert(count == yggr::tuples::length<tuple_type>::value);
	h(ar, t);
}

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

template<typename Archive, typename ...Args> inline
void load(Archive& ar, std::tuple<Args...>& t, const unsigned int ver)
{
	typedef std::tuple<Args...> tuple_type;
	typedef
		yggr::serialization::std_tuple_io_helper
		<
			tuple_type,
			0,
			yggr::tuples::length<tuple_type>::value
		> h_type;

	h_type h;
	yggr::ser_size_type count = 0;

	ar >> YGGR_SERIALIZATION_NVP(count);
	assert(count == yggr::tuples::length<tuple_type>::value);
	h(ar, t);
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

//---------------serialize---------
template< typename Archive, typename HH, typename TT > inline
void serialize(Archive& ar,
				boost::tuples::cons<HH, TT>& t,
				const unsigned int ver)
{
	boost::serialization::split_free(ar, t, ver);
}

template< typename Archive, typename HH, typename TT > inline
void serialize(Archive& ar,
				yggr::tuples::cons<HH, TT>& t,
				const unsigned int ver)
{
	boost::serialization::split_free(ar, t, ver);
}

template< typename Archive, YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH())> inline
void serialize(Archive& ar,
				boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>& t,
				const unsigned int ver)
{
	boost::serialization::split_free(ar, t, ver);
}

template< typename Archive, YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
void serialize(Archive& ar,
				yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& t,
				const unsigned int ver)
{
	boost::serialization::split_free(ar, t, ver);
}

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

template<typename Archive, typename ...Args> inline
void serialize(Archive& ar, std::tuple<Args...>& t, const unsigned int ver)
{
	boost::serialization::split_free(ar, t, ver);
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

} // namespace serialization
} // namespace boost

#undef _YGGR_TMP_PP_TUPLE_PARAMS_OP
#undef _YGGR_TMP_PP_TUPLE_ARCHIVE_OP

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, boost::tuples::cons, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, yggr::tuples::cons, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(BOOST_TUPLE_LIMIT_LENGTH(), boost::tuples::tuple, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(YGGR_TUPLE_LIMIT_LENGTH(), yggr::tuples::tuple, object_serializable)

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

YGGR_SERIALIZATION_VARIADIC_CONTAINER_IMPLEMENTATION_LEVEL_DEF(YGGR_TUPLE_LIMIT_LENGTH(), std::tuple, object_serializable)

#endif // YGGR_NO_CXX11_HDR_TUPLE

#endif // __YGGR_SERIALIZATION_TUPLE_HPP__

#include <yggr/serialization/tuple_bson_impl.hpp>
