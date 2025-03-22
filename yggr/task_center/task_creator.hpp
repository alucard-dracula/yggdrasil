//task_creator.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_CREATOR_HPP__
#define __YGGR_TASK_CENTER_TASK_CREATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace task_center
{

template<typename ID_Generator, typename Task>
class task_creator
{
public:
	typedef ID_Generator id_gener_type;
	typedef Task task_type;
	
private:
	BOOST_MPL_ASSERT((
		boost::is_same
		<
			typename task_type::id_type,
			typename id_gener_type::id_type
		> ));
	
	typedef task_creator this_type;

public:
	task_creator(void)
	{
	}

	~task_creator(void)
	{
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	task_type operator()( BOOST_FWD_REF(Args)... args ) const
	{
		return task_type(_id_gener(), boost::forward<Args>(args)...); 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		task_type operator()(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS )) const { \
			return task_type(_id_gener() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA) ); }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline task_type& operator()(task_type& task) const
	{
		assert(!task.task_id());
		copy_or_move_or_swap(task.task_id(), _id_gener());
		return task;
	}

private:
	id_gener_type _id_gener;
};

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_TASK_CREATOR_HPP__