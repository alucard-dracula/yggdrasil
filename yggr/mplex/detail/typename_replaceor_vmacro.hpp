//typename_repaceor_vmacro.hpp

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

#ifndef __YGGR_MPLEX_TYPENAME_REPLACEOR_VMACRO_HPP__
#define __YGGR_MPLEX_TYPENAME_REPLACEOR_VMACRO_HPP__

#ifndef __YGGR_MPLEX_TYPENAME_REPLACEOR_HPP__
#	error "please inclue typename_replaceor.hpp"
#endif // __YGGR_MPLEX_TYPENAME_REPLACEOR_HPP__

#include <yggr/ppex/typedef.hpp>
#include <yggr/mplex/null_t.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace mplex
{

typedef null_type _keep;

namespace detail
{

template<typename T>
struct typename_replaceor_detail
{
	typedef boost::mpl::size_t<0> size_type;
};

#define YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_OP_IMPL( __z__, __n__, __op__ ) \
	YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
	typename boost::mpl::if_< \
		boost::is_same< BOOST_PP_CAT( YGGR_TMP_PP_APPLY_PARAMS_NAME(), __n__ ), YGGR_PP_TEMPLATE_DEFAULT_TYPE() >, \
		BOOST_PP_CAT( YGGR_TMP_PP_PARAMS_NAME(), __n__ ), \
		BOOST_PP_CAT( YGGR_TMP_PP_APPLY_PARAMS_NAME(), __n__ ) >::type \

#define YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_IMPL( __n__ ) \
	YGGR_PP_REPEAT_CUSTOM_OP_NOW( __n__, YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_OP_IMPL, YGGR_PP_SYMBOL_COMMA )

#define YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_IMPL( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename YGGR_TMP_PP_PARAMS_NAME() ), \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) > class TPL> \
	struct typename_replaceor_detail< TPL< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, YGGR_TMP_PP_PARAMS_NAME() ) > > { \
		typedef boost::mpl::size_t< __n__ > size_type; \
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(__n__, typename YGGR_TMP_PP_APPLY_PARAMS_NAME())> \
		struct apply { typedef TPL< YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_IMPL( __n__ ) > type; }; };

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_IMPL( __n__ )

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() mplex::_keep
#define YGGR_TMP_PP_PARAMS_NAME() T
#define YGGR_TMP_PP_APPLY_PARAMS_NAME() IT

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_TMP_PP_APPLY_PARAMS_NAME
#undef YGGR_TMP_PP_PARAMS_NAME
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE

#undef YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_IMPL
#undef YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_IMPL
#undef YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_OP_IMPL

} // namespace detail

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() _keep
template<typename Src, YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(YGGR_PP_TEMPLATE_PARAMS_LEN(), typename T)>
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct typename_replaceor
{
private:
	typedef typename 
		boost::mpl::vector<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_PARAMS_LEN(), T)>::type params_type;
	typedef detail::typename_replaceor_detail<Src> rep_type;

	template<typename Rep, typename Params, std::size_t N, typename Nil = void>
	struct apply
	{
		typedef Src type;
	};

#define YGGR_TMP_PP_TYPENAME_REPLACEOR_APPLY_OP_IMPL( __z__, __n__, __op__ ) \
	YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
	typename boost::mpl::at_c< YGGR_TMP_PP_PARAMS_NAME(), __n__ >::type

#define YGGR_TMP_PP_TYPENAME_REPLACEOR_APPLY_IMPL( __n__ ) \
	YGGR_PP_REPEAT_CUSTOM_OP_NOW( __n__, YGGR_TMP_PP_TYPENAME_REPLACEOR_APPLY_OP_IMPL, YGGR_PP_SYMBOL_COMMA )

#define YGGR_TMP_PP_TYPENAME_REPLACEOR_IMPL( __n__ ) \
	template< typename Rep, typename YGGR_TMP_PP_PARAMS_NAME(), typename Nil > \
	struct apply< Rep, YGGR_TMP_PP_PARAMS_NAME(), __n__, Nil > { \
		typedef typename Rep::template apply< YGGR_TMP_PP_TYPENAME_REPLACEOR_APPLY_IMPL(__n__) >::type type; };

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	YGGR_TMP_PP_TYPENAME_REPLACEOR_IMPL( __n__ )

#define YGGR_TMP_PP_PARAMS_NAME() Params

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_TMP_PP_PARAMS_NAME

#undef YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_IMPL
#undef YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_IMPL
#undef YGGR_TMP_PP_TYPENAME_REPLACEOR_DETAIL_APPLY_OP_IMPL

public:
	typedef typename apply<rep_type, params_type, rep_type::size_type::value>::type type;
};


} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_TYPENAME_REPLACEOR_VMACRO_HPP__
