//timer_task.hpp

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

#ifndef __YGGR_TIME_DETAIL_TIMER_TASK_HPP__
#define __YGGR_TIME_DETAIL_TIMER_TASK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/time/timer_task_state.hpp>

namespace yggr
{
namespace time
{
namespace detail
{

template<typename Task_ID>
class base_timer_task
{
public:
	typedef Task_ID task_id_type;

private:
	typedef base_timer_task this_type;

public:
	base_timer_task(const task_id_type& id)
		: _id(id)
	{
	}

	virtual ~base_timer_task(void)
	{
	}

public:
	inline const task_id_type& id(void) const
	{
		return _id;
	}

public:
	virtual yggr::u32 do_timer(void) const = 0;

private:
	task_id_type _id;
};

template<typename Task_ID, typename Time_Task_Pak>
class timer_task
	: public base_timer_task<Task_ID>,
		private nonable::noncopyable
{
public:
	typedef base_timer_task<Task_ID> base_type;
	typedef typename base_type::task_id_type task_id_type;
	typedef Time_Task_Pak time_task_pak_type;

private:
	typedef timer_task this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> 
	timer_task(const task_id_type& id, BOOST_FWD_REF(Args)... args) 
		: base_type(id), 
			_time_task_pak(boost::forward<Args>(args)...) 
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		timer_task(const task_id_type& id \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) \
			: base_type(id), \
				_time_task_pak(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)) {}

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	timer_task(const task_id_type& id, BOOST_RV_REF(time_task_pak_type) pak)
		: base_type(id),
			_time_task_pak(boost::move(pak))
	{
	}

	timer_task(const task_id_type& id, const time_task_pak_type& pak)
		: base_type(id), 
			_time_task_pak(pak)
	{
	}

	~timer_task(void)
	{
	}

public:
	virtual yggr::u32 do_timer(void) const
	{
		return ((_time_task_pak))();
	}

private:
	mutable time_task_pak_type _time_task_pak;
};

} // namespace detail
} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_DETAIL_TIMER_TASK_HPP__
