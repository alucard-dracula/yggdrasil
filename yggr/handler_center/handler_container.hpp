//handler_center.hpp

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

#ifndef __YGGR_HANDLER_CENTER_HANDLER_CONTAINER_HPP__
#define __YGGR_HANDLER_CENTER_HANDLER_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/utility/args_holder.hpp>

#include <yggr/packet/packet_info.hpp>
#include <yggr/packet/packet_info_caster.hpp>
#include <yggr/caller/packet_info_to_params_caller.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/support/operator_t.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <cassert>

#if !defined(YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST)
#	define YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST 0
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST

namespace yggr
{
namespace handler_center
{
namespace detail
{

struct invoke_result_t
{
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return bret;
	}

	bool bret;
};


// ret_type foo(...) version
// handler_container_detail_invoke_helper_call_t
template<typename R, typename Handler, typename Params, std::size_t N>
struct handler_container_detail_invoke_helper_call_t
{
public:
	typedef utility::basic_args_holder base_args_holder_type;

public:
	typedef R result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type& params) const
	{
#if defined(YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST) && YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
		result_type* pret = utility::args_holder_ref_wrap_static_cast<result_type>(ret);
		params_type* pparams = utility::args_holder_ref_wrap_static_cast<params_type>(params);
#else
		result_type* pret = utility::args_holder_ref_wrap_dynamic_cast<result_type>(ret);
		params_type* pparams = utility::args_holder_ref_wrap_dynamic_cast<params_type>(params);
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
		
		if(!(pret && pparams))
		{
			return false;
		}
		params_type& ref_params = *pparams;
		*pret = caller::to_params_call(handler, ref_params);
		return true;
	}
};

template<typename R, typename Handler, typename Params>
struct handler_container_detail_invoke_helper_call_t<R, Handler, Params, 0>
{
public:
	typedef utility::basic_args_holder base_args_holder_type;

public:
	typedef R result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type&) const
	{
#if defined(YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST) && YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
		result_type* pret = utility::args_holder_ref_wrap_static_cast<result_type>(ret);
#else
		result_type* pret = utility::args_holder_ref_wrap_dynamic_cast<result_type>(ret);
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
		return pret && (*pret = handler(), true);
	}
};

// handler_container_detail_invoke_helper
template<typename R, typename Handler, typename Params>
struct handler_container_detail_invoke_helper
{
public:
	typedef utility::basic_args_holder base_args_holder_type;

public:
	typedef R result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper this_type;

public:
	template<std::size_t N> inline
	bool call(const base_args_holder_type& ret,
				const handler_type& handler,
				const base_args_holder_type& params) const
	{
		typedef
			handler_container_detail_invoke_helper_call_t
			<
				result_type,
				handler_type,
				params_type,
				N
			> caller_type;
		caller_type caller;
		return caller(ret, handler, params);
	}
};

// void foo(...) version

//handler_container_detail_invoke_helper_void_call_t
template<typename Handler, typename Params, std::size_t N>
struct handler_container_detail_invoke_helper_call_t<void, Handler, Params, N>
{
public:
	typedef utility::basic_args_holder base_args_holder_type;

public:
	typedef void result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type& params) const
	{
		assert((utility::args_holder_ref_wrap_check<void>(ret)));
#if defined(YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST) && YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
		params_type* pparams = utility::args_holder_ref_wrap_static_cast<params_type>(params);
#else
		params_type* pparams = utility::args_holder_ref_wrap_dynamic_cast<params_type>(params);
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
		if(!pparams)
		{
			return false;
		}
		params_type& ref_params = *pparams;
		caller::to_params_call(handler, ref_params);
		return true;
	}
};

template<typename Handler, typename Params>
struct handler_container_detail_invoke_helper_call_t<void, Handler, Params, 0>
{
public:
	typedef utility::basic_args_holder base_args_holder_type;

public:
	typedef void result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type params) const
	{
		assert((utility::args_holder_ref_wrap_check<void>(ret)));
		handler();
		return true;
	}
};

// handler_container_detail_invoke_helper_void
template<typename Handler, typename Params>
struct handler_container_detail_invoke_helper<void, Handler, Params>
{
public:
	typedef utility::basic_args_holder base_args_holder_type;

public:
	typedef void result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper this_type;

public:
	template<std::size_t N> inline
	bool call(const base_args_holder_type& ret,
				const handler_type& handler,
				const base_args_holder_type& params) const
	{
		typedef
			handler_container_detail_invoke_helper_call_t
			<
				result_type,
				handler_type,
				params_type,
				N
			> caller_type;
		caller_type caller;
		return caller(ret, handler, params);
	}
};

} // namespace detail

// basic_handler_container
class basic_handler_container
{
public:
	typedef utility::basic_args_holder base_args_holder_type;
	typedef detail::invoke_result_t invoke_result_type;

private:
	typedef basic_handler_container this_type;

public:
	virtual ~basic_handler_container(void)
	{
	}

public:
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	template<typename ...Args> inline
	void invoke(invoke_result_type& out, Args... args) const
	{
		typedef packet::packet_info< Args... > packet_info_type;
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type;
		typedef utility::args_holder_ref_wrap<void> ret_holder_type;

		assert(!out.bret);
		packet_info_type pak_info(args...);
		params_holder_type params_holder(pak_info);
		ret_holder_type ret_holder;
		out.bret = this->pro_invoke(ret_holder, params_holder);
	}

	template<typename Ret, typename ...Args> inline
	void invoke(Ret& ret, invoke_result_type& out, Args... args) const
	{
		typedef Ret ret_type;
		typedef packet::packet_info< Args... > packet_info_type;
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type;
		typedef utility::args_holder_ref_wrap<ret_type> ret_holder_type;

		packet_info_type pak_info(args...);
		params_holder_type params_holder(pak_info);
		ret_holder_type ret_holder(ret);
		out.bret = this->pro_invoke(ret_holder, params_holder);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	void invoke(invoke_result_type& out \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
		typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type; \
		typedef utility::args_holder_ref_wrap<void> ret_holder_type; \
		assert(!out.bret); \
		packet_info_type pak_info \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_L ) \
								YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_R ) ; \
		params_holder_type params_holder(pak_info); \
		ret_holder_type ret_holder; \
		out.bret = this->pro_invoke(ret_holder, params_holder); } \
	\
	template< typename Ret \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	void invoke(Ret& ret, invoke_result_type& out \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
		typedef Ret ret_type; \
		typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type; \
		typedef utility::args_holder_ref_wrap<ret_type> ret_holder_type; \
		packet_info_type pak_info \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_L ) \
								YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_R ) ; \
		params_holder_type params_holder(pak_info); \
		ret_holder_type ret_holder(ret); \
		out.bret = this->pro_invoke(ret_holder, params_holder); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

protected:
	virtual bool pro_invoke(const base_args_holder_type& ret,
							const base_args_holder_type& params) const = 0;
};

typedef yggr::shared_ptr<basic_handler_container> basic_handler_container_sptr_type;

// handler_container class
template< typename Handler>
class handler_container
	: public basic_handler_container
{
private:
	typedef basic_handler_container base_type;
	typedef handler_container this_type;

private:
	typedef Handler handler_type;
	typedef func::foo_t_info<handler_type> foo_t_info_type;
	typedef typename foo_t_info_type::result_type result_type;

public:
	typedef typename
		boost::mpl::if_
		<
			typename foo_t_info_type::arg_holder_list_size_type,
			typename
				packet::packet_info_caster
				<
					typename foo_t_info_type::arg_holder_list_type
				>::type,
			packet::packet_info<>
		>::type params_type;

	typedef typename params_type::params_type check_data_type;

private:
	YGGR_STATIC_CONSTANT(std::size_t, E_params_length = params_type::E_length);

private:
	typedef
		detail::handler_container_detail_invoke_helper
		<
			result_type,
			handler_type,
			params_type
		> detail_invoke_helper_type;

public:
	handler_container(const handler_type& handler)
		: _handler(handler)
	{
	}

	virtual ~handler_container(void) YGGR_OVERRIDE
	{
	}

protected:
	virtual bool pro_invoke(const base_args_holder_type& ret,
							const base_args_holder_type& params) const YGGR_OVERRIDE
	{
		detail_invoke_helper_type helper;
		return helper.template call<E_params_length>(ret, _handler, params);
	}

private:
	handler_type _handler;
};

template<typename Handler> inline
handler_container<Handler> make_handler_container(const Handler& handler)
{
	typedef handler_container<Handler> cont_type;
	return cont_type(handler);
}

template<template<typename _T> class SPtr, typename Handler> inline
SPtr<basic_handler_container> make_shared_handler_container_custom(const Handler& handler)
{
	typedef SPtr<basic_handler_container> sptr_type;
	typedef handler_container<Handler> cont_type;
	return sptr_type(yggr_nothrow_new cont_type(handler));
}

template<typename Handler> inline
yggr::shared_ptr<basic_handler_container> make_shared_handler_container(const Handler& handler)
{
	return make_shared_handler_container_custom<yggr::shared_ptr>(handler);
}


} // namespace handler_center
} // namespace yggr

#endif // __YGGR_HANDLER_CENTER_HANDLER_CONTAINER_HPP__
