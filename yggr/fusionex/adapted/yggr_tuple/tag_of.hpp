//tag_of.hpp

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

#ifndef __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_TAG_OF_HPP__
#define __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_TAG_OF_HPP__

// !!!yggr need this file because yggr::tuples::null_type is using boost::tuples::null_type !!!
// !!!so, yggr need include <boost/fusion/adapted/boost_tuple/tag_of.hpp> !!!

#include <yggr/tuple/tuple.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/fusion/adapted/boost_tuple/tag_of.hpp>
    
namespace boost 
{ 
namespace fusion 
{
	
struct yggr_tuple_tag;
struct fusion_sequence_tag;

namespace traits
{
	
template <YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T )>

#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
	struct tag_of< yggr::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T ) >, void >
#else
	struct tag_of< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T )> >
#endif // BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
{
	typedef yggr_tuple_tag type;
};

template <typename  HH, typename TT>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
struct tag_of<yggr::tuples::cons<HH, TT>, void >
#else
struct tag_of<yggr::tuples::cons<HH, TT> >
#endif // BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
{
	typedef yggr_tuple_tag type;
};

// !!!!! the tag_of is defined in <boost/fusion/support/tag_of_fwd.hpp>!!!
//template <>
//struct tag_of<yggr::tuples::null_type>
//{
//	typedef yggr_tuple_tag type;
//};

} // namespace traits
} // namespace fusion
} // namespace boost

namespace boost 
{ 
namespace mpl
{

template<typename>
struct sequence_tag;

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T ) >
struct sequence_tag< yggr::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T ) > >
{
	typedef fusion::fusion_sequence_tag type;
};

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T ) >
struct sequence_tag< yggr::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T ) > const >
{
	typedef fusion::fusion_sequence_tag type;
};

template <typename HH, typename TT>
struct sequence_tag< yggr::tuples::cons<HH, TT> >
{
	typedef fusion::fusion_sequence_tag type;
};

template <typename HH, typename TT>
struct sequence_tag< yggr::tuples::cons<HH, TT> const>
{
	typedef fusion::fusion_sequence_tag type;
};

// !!!!! the two sequence_tag  is defined in <boost/fusion/support/tag_of_fwd.hpp>!!!
//template <>
//struct sequence_tag<yggr::tuples::null_type>
//{
//	typedef fusion::fusion_sequence_tag type;
//};
//
//template <>
//struct sequence_tag<yggr::tuples::null_type const>
//{
//	typedef fusion::fusion_sequence_tag type;
//};

} // namespace mpl
} // namespace boost

#endif // __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_TAG_OF_HPP__
