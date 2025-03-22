// foo_t_maker_vtpl.hpp

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

#ifndef __YGGR_FUNC_DETAIL_FOO_T_MAKER_VTPL_HPP__
#define __YGGR_FUNC_DETAIL_FOO_T_MAKER_VTPL_HPP__

#ifndef __YGGR_FUNC_FOO_T_MAKER_HPP__
#	error "please inclue foo_t_maker.hpp"
#endif // __YGGR_FUNC_FOO_T_MAKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/integer_sequence.hpp>

#include <yggr/mplex/args.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/cv_mark.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/vector.hpp>

namespace yggr
{
namespace func
{

namespace detail
{

// foo_t_maker_helper
template<typename R, typename Params, typename Indexes>
struct foo_t_maker_helper;

template<typename R, typename Params, std::size_t ...I>
struct foo_t_maker_helper<R, Params, ::yggr::index_sequence<I...> >
{
	typedef R (foo_type)(typename mplex::get_arg<Params, I>::type...);
	typedef typename boost::add_pointer<foo_type>::type foo_pointer_type;
};

// member_foo_t_maker_helper
template<typename R, typename Class, u32 cv_mark_in, typename Params, typename Indexes>
struct member_foo_t_maker_helper;

template<typename R, typename Class, u32 cv_mark_in, typename Params, std::size_t ...I>
struct member_foo_t_maker_helper<R, Class, cv_mark_in, Params, ::yggr::index_sequence<I...> >
{
private: 
	typedef typename 
		boost::mpl::vector< 
			R ( Class::* )( typename mplex::get_arg<Params, I>::type... ), 
			R ( Class::* )( typename mplex::get_arg<Params, I>::type... ) const, 
			R ( Class::* )( typename mplex::get_arg<Params, I>::type... ) volatile, 
			R ( Class::* )( typename mplex::get_arg<Params, I>::type... ) const volatile 
		>::type foo_chk_vt_type;
public: 
	typedef typename boost::mpl::at_c<foo_chk_vt_type, cv_mark_in >::type foo_type;
	typedef foo_type foo_pointer_type;
};

} // namespace detail

//-------------------foo_t_maker------------------
template< typename R, typename ...Args >
struct foo_t_maker
{
public:
	typedef yggr::mplex::null_type null_type;

private:
	typedef boost::mpl::vector< Args... > tmp_vt_t_type;

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
	typedef 
		detail::foo_t_maker_helper
		<
			result_type, 
			params_type, 
			::yggr::make_index_sequence<boost::mpl::size<params_type>::value> 
		> helper_type;

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

template< typename R, typename C, u32 cv_mark, typename ...Args >
struct member_foo_t_maker
{
public:
	typedef yggr::mplex::null_type null_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(cv_mark < cv_mark_t::E_cv_end)>));

	typedef boost::mpl::vector< Args... > tmp_vt_t_type;
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
			params_type, 
			::yggr::make_index_sequence<boost::mpl::size<params_type>::value> 
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

#endif // __YGGR_FUNC_DETAIL_FOO_T_MAKER_VTPL_HPP__
