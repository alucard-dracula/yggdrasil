// ctrl_center_dispatcher_container.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_DISPATCHER_CONTAINER_HPP__
#define __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_DISPATCHER_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/system_controller/detail/ctrl_center_controller.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace system_controller
{
namespace detail
{

class dispatcher_container
{
public:
	typedef ctrl_center_types_def::handler_id_type handler_id_type;
	typedef ctrl_center_types_def::param_type param_type;
	typedef ctrl_center_types_def::handler_wrap_type handler_wrap_type;

private:
	typedef 
		safe_container::safe_unordered_map
		<
			handler_id_type, 
			base_ctrl_ptr_type
		> controller_container_type;

	typedef controller_container_type::iterator controller_container_iter_type;
	typedef controller_container_type::const_iterator controller_container_citer_type;

	typedef container::vector<base_ctrl_ptr_type> ctrl_cont_type;
	typedef ctrl_cont_type::iterator ctrl_cont_iter_type;
	typedef ctrl_cont_type::const_iterator ctrl_cont_citer_type;

private:
	typedef dispatcher_container this_type;

public:
	dispatcher_container(void);
	~dispatcher_container(void);

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	void dispatch(const Args&... args) const
	{
		ctrl_cont_type cont;
		cont.reserve(_cont.size());
		_cont.using_handler(
			boost::bind(
				&dispatcher_container::handler_get_dispatchers,
				_1, boost::ref(cont)));

		for(ctrl_cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i)
		{
			if(*i)
			{
				(*i)->ctrl(args...);
			}
		}
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		void dispatch( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) const { \
			ctrl_cont_type cont; \
			cont.reserve(_cont.size()); \
			_cont.using_handler( \
				boost::bind( \
					&dispatcher_container::handler_get_dispatchers, \
					_1, boost::ref(cont))); \
			for(ctrl_cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i) { \
				if(*i) { (*i)->ctrl(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } } }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME


#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	inline bool empty(void) const
	{
		return _cont.empty();
	}

	inline void clear(void)
	{
		_cont.clear();
	}

	template<typename Ctrl> inline
	bool append_dispatcher(const handler_wrap_type& handler_wrap)
	{
		typedef Ctrl ctrl_type;
		typedef typename ctrl_type::handler_type handler_type;

		return
			_cont.using_handler(
				boost::bind(
					&dispatcher_container::handler_append_dispatcher<ctrl_type>,
					_1, 
					boost::addressof(typeid(typename ctrl_type::handler_type)), 
					boost::cref(handler_wrap) ) );
	}

	inline bool erase_dispatcher(handler_id_type id, 
														const handler_wrap_type& handler_wrap)
	{
		return 
			_cont.find(
				id,
				boost::bind(
					&dispatcher_container::handler_erase_dispatcher,
					_1, _2, boost::cref(handler_wrap) ) );
	}

private:
	template<typename Ctrl> inline
	static bool handler_append_dispatcher(controller_container_type::base_type& base,
											handler_id_type name,
											const handler_wrap_type& handler_wrap)
	{
		typedef Ctrl ctrl_type;
		typedef typename controller_container_type::mapped_type mapped_type;

		mapped_type* pval = 0;
		try
		{
			pval = boost::addressof(base[name]);
			
		}
		catch(const ::yggr::stl_exception&)
		{
			return false;
		}

		assert(pval);
		mapped_type& mapped = *pval;

		return 
			mapped?
				mapped->insert(handler_wrap)
				: !!(mapped = construct_shared<base_controller>(
								yggr_nothrow_new ctrl_type(handler_wrap) ));
	}

	static bool handler_erase_dispatcher(controller_container_type::base_type& base,
											controller_container_type::iterator iter,
											const handler_wrap_type& handler_wrap);

	static ctrl_cont_type& 
		handler_get_dispatchers(const controller_container_type::base_type& base,
								ctrl_cont_type& cont);

private:
	controller_container_type _cont;
};

} // namespace detail
} // namespace system_contrller
} // namespace yggr

#endif // __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_DISPATCHER_CONTAINER_HPP__
