// ctrl_center_controller_group.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CONTROLLER_GROUP_HPP__
#define __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CONTROLLER_GROUP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/system_controller/detail/ctrl_center_dispatcher_container.hpp>

namespace yggr
{
namespace system_controller
{
namespace detail
{

class controller_group
{
public:
	typedef dispatcher_container dispatcher_container_type;
	typedef dispatcher_container_type::handler_id_type handler_id_type;
	typedef dispatcher_container_type::param_type param_type;
	typedef dispatcher_container_type::handler_wrap_type handler_wrap_type;

private:
	typedef controller_group this_type;

public:
	controller_group(void)
	{
	}

	~controller_group(void)
	{
	}

public:
	inline bool empty(void) const
	{
		return 
			_ctrl_cont.empty() 
			&& _dis_cont.empty();
	}

	inline void clear(void)
	{
		_ctrl_cont.clear();
		_dis_cont.clear();
	}

	inline void clear_controller(void)
	{
		_ctrl_cont.clear();
	}

	inline void clear_dispatcher(void)
	{
		_dis_cont.clear();
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	void ctrl(const Args&... args) const
	{
		_ctrl_cont.dispatch(args...);
	}

	template<typename ...Args> inline
	void dispatch(const Args&... args) const
	{
		_dis_cont.dispatch(args...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void ctrl( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) const { \
			_ctrl_cont.dispatch(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void dispatch( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) const { \
			_dis_cont.dispatch(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME


#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	template<typename Ctrl> inline
	bool append_controller(const handler_wrap_type& handler_wrap)
	{
		return _ctrl_cont.append_dispatcher<Ctrl>(handler_wrap);
	}

	template<typename Dis> inline
	bool append_dispatcher(const handler_wrap_type& handler_wrap)
	{
		return _dis_cont.append_dispatcher<Dis>(handler_wrap);
	}

	inline bool erase_controller(handler_id_type ctrl_id, const handler_wrap_type& handler_wrap)
	{
		return _ctrl_cont.erase_dispatcher(ctrl_id, handler_wrap);
	}

	inline bool erase_dispatcher(handler_id_type dis_id, const handler_wrap_type& handler_wrap)
	{
		return _dis_cont.erase_dispatcher(dis_id, handler_wrap);
	}

private:
	dispatcher_container_type _ctrl_cont;
	dispatcher_container_type _dis_cont;
};

} // namespace detail
} // namespace system_contrller
} // namespace yggr

#endif // __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CONTROLLER_GROUP_HPP__

