// foo_t_maker_vmacro.hpp

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

#ifndef __YGGR_FUNC_DETAIL_FOO_T_MAKER_VMACRO_HPP__
#define __YGGR_FUNC_DETAIL_FOO_T_MAKER_VMACRO_HPP__

#ifndef __YGGR_FUNC_FOO_T_MAKER_HPP__
#	error "please inclue foo_t_maker.hpp"
#endif // __YGGR_FUNC_FOO_T_MAKER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/ppex/typedef.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/mplex/args.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/cv_mark.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/type_traits/add_pointer.hpp>

#ifndef YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN
#	define YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN YGGR_PP_TEMPLATE_PARAMS_LEN()
#endif // YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN

// _YGGR_TMP_PP_VECTOR_SUB_GET(__n__, __val__)
// _YGGR_TMP_PP_VECTOR_VAL ()
#define _YGGR_TMP_PP_VECTOR_ARGS_GET( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		_YGGR_TMP_PP_VECTOR_SUB_GET( \
			__n__, YGGR_PP_OPEN( _YGGR_TMP_PP_VECTOR_VAL  ) )

#define YGGR_PP_FOO_TYPEINFO_HELPERS( __n__ ) \
	template< typename R, typename _YGGR_TMP_PP_VECTOR_VAL() > \
	struct foo_t_maker_helper< R, _YGGR_TMP_PP_VECTOR_VAL(), __n__ > { \
		public: \
			typedef R ( foo_type )( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			typedef typename boost::add_pointer<foo_type>::type foo_pointer_type; };

#define YGGR_PP_MEMBER_FOO_TYPEINFO_HELPERS( __n__ ) \
	template< typename R, typename Class, u32 cv_mark_in, typename _YGGR_TMP_PP_VECTOR_VAL() > \
	struct member_foo_t_maker_helper< R, Class, cv_mark_in, _YGGR_TMP_PP_VECTOR_VAL(), __n__ > { \
		private: \
			typedef typename \
				boost::mpl::vector< \
					R ( Class::* )( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ), \
					R ( Class::* )( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ) const, \
					R ( Class::* )( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ) volatile, \
					R ( Class::* )( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ) const volatile \
				>::type foo_chk_vt_type; \
		public: \
			typedef typename boost::mpl::at_c<foo_chk_vt_type, cv_mark_in >::type foo_type; \
			typedef foo_type foo_pointer_type; };

namespace yggr
{
namespace func
{
namespace detail
{

// foo_t_maker_helper
template<typename R, typename Params,
			std::size_t params_size = boost::mpl::size<Params>::value >
struct foo_t_maker_helper;

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	YGGR_PP_FOO_TYPEINFO_HELPERS( __n__ )

#define _YGGR_TMP_PP_VECTOR_SUB_GET( __n__, __val__ ) \
		typename mplex::get_arg< __val__, __n__ >::type

#define _YGGR_TMP_PP_VECTOR_VAL() Params
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_VECTOR_ARGS_GET

#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef _YGGR_TMP_PP_VECTOR_VAL
#undef _YGGR_TMP_PP_VECTOR_SUB_GET

//member_foo_t_maker_helper
template<typename R, typename Class, u32 cv_mark_in, typename Params,
			std::size_t params_size = boost::mpl::size<Params>::value >
struct member_foo_t_maker_helper;

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	YGGR_PP_MEMBER_FOO_TYPEINFO_HELPERS( __n__ )

#define _YGGR_TMP_PP_VECTOR_SUB_GET( __n__, __val__ ) \
		typename mplex::get_arg< __val__, __n__ >::type

#define _YGGR_TMP_PP_VECTOR_VAL() Params
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_VECTOR_ARGS_GET

#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef _YGGR_TMP_PP_VECTOR_VAL
#undef _YGGR_TMP_PP_VECTOR_SUB_GET


} // namespace detail
} // namespace func
} // namespace yggr

namespace yggr
{
namespace func
{

//-------------------foo_t_maker------------------
#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::mplex::null_type
	template< typename R, YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN, typename Val ) >
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct foo_t_maker
{
public:
	typedef yggr::mplex::null_type null_type;

private:
	typedef 
		boost::mpl::vector
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN,
				Val)
		> tmp_vt_t_type;

	typedef typename boost::mpl::find<tmp_vt_t_type, null_type>::type i;
	typedef typename 
		boost::mpl::erase
		<
			tmp_vt_t_type,
			i,
			typename boost::mpl::end<tmp_vt_t_type>::type
		>::type params_type;

public:
	typedef R result_type;
	typedef null_type class_type;
	typedef params_type arg_list_type;
	typedef typename boost::mpl::size<arg_list_type>::type arg_list_size_type;
	typedef boost::mpl::false_ is_const_type;
	typedef boost::mpl::false_ is_volatile_type;

protected:
	typedef detail::foo_t_maker_helper<result_type, params_type> helper_type;

public:
	typedef typename helper_type::foo_type foo_type;
	typedef typename helper_type::foo_pointer_type foo_pointer_type;
	typedef foo_pointer_type type;

public:
	template<std::size_t N> 
	struct arg
		: public mplex::get_arg<arg_list_type, N>
	{
	};

public:
	YGGR_STATIC_CONSTANT(bool, is_const = is_const_type::value);
	YGGR_STATIC_CONSTANT(bool, is_volatile = is_volatile_type::value);

	YGGR_STATIC_CONSTANT(std::size_t, arg_list_size = arg_list_size_type::value);
};

//-------------------member_foo_t_maker------------------

using ::yggr::mplex::cv_mark_t;


#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::mplex::null_type
	template< typename R, typename C, u32 cv_mark, 
				YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN, typename Val ) >
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct member_foo_t_maker
{
public:
	typedef yggr::mplex::null_type null_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(cv_mark < cv_mark_t::E_cv_end)>));
	typedef 
		boost::mpl::vector
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN,
				Val)
		> tmp_vt_t_type;

	typedef typename boost::mpl::find<tmp_vt_t_type, null_type>::type i;
	typedef typename 
		boost::mpl::erase
		<
			tmp_vt_t_type,
			i,
			typename boost::mpl::end<tmp_vt_t_type>::type
		>::type params_type;

public:
	typedef R result_type;
	typedef C class_type;
	typedef params_type arg_list_type;
	typedef typename boost::mpl::size<arg_list_type>::type arg_list_size_type;
	typedef boost::mpl::bool_<!!(cv_mark & cv_mark_t::E_cv_const)> is_const_type;
	typedef boost::mpl::bool_<!!(cv_mark & cv_mark_t::E_cv_volatile)> is_volatile_type;

protected:
	typedef 
		detail::member_foo_t_maker_helper
		<
			result_type,
			class_type, 
			cv_mark, 
			params_type
		> helper_type;

public:
	typedef typename helper_type::foo_type foo_type;
	typedef typename helper_type::foo_pointer_type foo_pointer_type;
	typedef foo_pointer_type type;

public:
	template<std::size_t N> 
	struct arg
		: public mplex::get_arg<params_type, N>
	{
	};

public:
	YGGR_STATIC_CONSTANT(bool, is_const = is_const_type::value);
	YGGR_STATIC_CONSTANT(bool, is_volatile = is_volatile_type::value);

	YGGR_STATIC_CONSTANT(std::size_t, arg_list_size = arg_list_size_type::value);

};

} // namespace func
} // namespace yggr

#undef _YGGR_TMP_PP_VECTOR_ARGS_GET
#undef YGGR_PP_FOO_TYPEINFO_HELPERS
#undef YGGR_PP_MEMBER_FOO_TYPEINFO_HELPERS

#endif // __YGGR_FUNC_DETAIL_FOO_T_MAKER_VMACRO_HPP__
