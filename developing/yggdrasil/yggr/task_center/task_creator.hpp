//task_creator.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace task_center
{

template<typename ID_Generator, 
			typename Task>
class task_creator
{
public:
	typedef ID_Generator id_gener_type;
	typedef Task task_type;
	//typedef typename task_type::data_container_type task_data_container_type;
	
private:
	BOOST_MPL_ASSERT((
						boost::is_same
						<
							typename task_type::id_type,
							typename id_gener_type::id_type
						>
					));
	
	typedef task_creator this_type;

public:
	task_creator(void)
	{
	}

	~task_creator(void)
	{
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename Obj > \
	task_type operator()(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) const Obj& obj) const { \
		return task_type(_id_gener(), YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj); } \
	\
	template<YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename Obj > \
	task_type operator()(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) Obj& obj) const { \
		return task_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj); }

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

	task_type& operator()(task_type& task) const
	{
		assert(!task.task_id());
		task.task_id() = _id_gener();
		return task;
	}

private:
	id_gener_type _id_gener;
};

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_TASK_CREATOR_HPP__