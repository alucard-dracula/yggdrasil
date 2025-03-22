//ctrl_center_controller.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_CONTROLLER_HPP__
#define __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_CONTROLLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind_ex/bind.hpp>
#include <yggr/utility/args_holder.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/packet/packet_info_caster.hpp>

#include <yggr/caller/to_params_caller.hpp>

#include <yggr/safe_container/safe_vector.hpp>

#include <yggr/system_controller/detail/ctrl_center_types.hpp>

#include <boost/ref.hpp>
#include <boost/range/functions.hpp>

#include <cassert>
#include <typeinfo>

namespace yggr
{
namespace system_controller
{
namespace detail
{

template<typename Handler>
class base_controller_handler_wrap_op_helper;

// -----------base_controller----------
class base_controller
{
public:
	typedef ctrl_center_types_def::param_type param_type;
	typedef ctrl_center_types_def::handler_wrap_type handler_wrap_type;

private:
	template<typename Handler>
	friend class base_controller_handler_wrap_op_helper;

private:
	typedef base_controller this_type;

public:
	virtual ~base_controller(void)
	{
	}

public:
	virtual bool insert(const handler_wrap_type& handler_wrap) = 0;
	virtual std::size_t erase(const handler_wrap_type& handler_wrap) = 0;
	virtual void clear(void) = 0;

public:
	virtual const std::type_info& type(void) const = 0;
	virtual bool empty(void) const = 0;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename ...Args> inline
	void ctrl(const Args&... args) const
	{
		typedef packet::packet_info<Args...> packet_info_type;
		typedef utility::args_holder_ref_wrap<const packet_info_type> now_params_type;

		packet_info_type inner_params =
			packet_info_type::make_packet_info(args...);
		now_params_type now_params(inner_params);

		this->pro_ctrl(utility::args_holder_trans(now_params));
	}

#else

	// don't using YGGR_PP_FOO_ANYREF_PARAMS

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void ctrl( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) const { \
			typedef packet::packet_info< YGGR_PP_FOO_TYPES_GROUP( __n__ ) > packet_info_type; \
			typedef utility::args_holder_ref_wrap<const packet_info_type> now_params_type; \
			packet_info_type inner_params = \
				packet_info_type::make_packet_info( \
					YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); \
			now_params_type now_params(inner_params); \
			this->pro_ctrl(utility::args_holder_trans(now_params)); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

protected:
	virtual void pro_ctrl(const param_type& params) const = 0;

protected:
	template<typename Handler> inline
	static bool equal_to_of_handler(const handler_wrap_type& left, const Handler& right)
	{
		typedef Handler handler_type;

		const handler_type* phandler =
			::yggr::any_cast<handler_type>(boost::addressof(left));

		return
			(phandler == boost::addressof(right))
			|| (phandler && *phandler == right);
	}

	template<typename Handler> inline
	static bool equal_to_of_handler(const Handler& left, const handler_wrap_type& right)
	{
		typedef Handler handler_type;

		const handler_type* phandler =
			::yggr::any_cast<handler_type>(boost::addressof(right));

		return
			(phandler == boost::addressof(left))
			|| (phandler && *phandler == left);
	}

	template<typename Handler> inline
	static bool equal_to_of_handler_wrap(const handler_wrap_type& left, const handler_wrap_type& right)
	{
		typedef Handler handler_type;

		if(boost::addressof(left) == boost::addressof(right))
		{
			return true;
		}

		const handler_type* pleft =
			::yggr::any_cast<handler_type>(boost::addressof(left));

		const handler_type* pright =
			::yggr::any_cast<handler_type>(boost::addressof(right));

		return
			(pleft == pright)
			|| (pleft && pright && *pleft == *pright);
	}
};

typedef shared_ptr<base_controller> base_ctrl_ptr_type;

// base_controller_handler_wrap_op_helper
template<typename Handler>
class base_controller_handler_wrap_op_helper
{
public:
	typedef Handler handler_type;
	typedef base_controller::handler_wrap_type handler_wrap_type;

private:
	typedef base_controller_handler_wrap_op_helper this_type;

public:
	base_controller_handler_wrap_op_helper(const handler_wrap_type& val)
		: _cmp_val(val)
	{
	}

	base_controller_handler_wrap_op_helper(const this_type& right)
		: _cmp_val(right._cmp_val)
	{
	}

	~base_controller_handler_wrap_op_helper(void)
	{
	}

public:
	inline bool operator()(const handler_wrap_type& val) const
	{
		return base_controller::equal_to_of_handler_wrap<handler_type>(_cmp_val, val);
	}

	inline const handler_wrap_type& value(void) const
	{
		return _cmp_val;
	}

private:
	const handler_wrap_type& _cmp_val;
};

//-----------------controller------------------------

template<std::size_t N, typename Handler, typename HandlerArgs, typename HandlerContainer>
struct handler_caller
{
public:
	typedef ctrl_center_types_def::param_type param_type;
public:
	typedef Handler handler_type;
	typedef HandlerArgs handler_args_type;
	typedef HandlerContainer handler_container_type;

public:
	void operator()(const handler_container_type& handler_cont, const param_type& param) const
	{
		typedef typename boost::range_iterator<const handler_container_type>::type iter_type;

		const handler_type* phandler = 0;
		const handler_args_type* pargs = utility::args_holder_ref_wrap_dynamic_cast<const handler_args_type>(param);

		if(!pargs)
		{
			return;
		}

		const handler_args_type& pak_info = *pargs;
		for(iter_type i = boost::begin(handler_cont), isize = boost::end(handler_cont);
				i != isize; ++i)
		{
			phandler = ::yggr::unsafe_any_cast<handler_type>(boost::addressof(*i));
			if(!phandler)
			{
				continue;
			}

			caller::to_params_call(*phandler, pak_info);
		}
	}

};

template<typename Handler, typename HandlerArgs, typename HandlerContainer>
struct handler_caller< 0, Handler, HandlerArgs, HandlerContainer >
{
public:
	typedef ctrl_center_types_def::param_type param_type;

public:
	typedef Handler handler_type;
	typedef HandlerArgs handler_args_type;
	typedef HandlerContainer handler_container_type;
public:
	void operator()(const handler_container_type& handler_cont, const param_type&) const
	{
		typedef typename boost::range_iterator<const handler_container_type>::type iter_type;
		const handler_type* phandler = 0;

		for(iter_type i = boost::begin(handler_cont), isize = boost::end(handler_cont);
				i != isize; ++i)
		{
			phandler = ::yggr::unsafe_any_cast<handler_type>(boost::addressof(*i));
			if(!phandler)
			{
				continue;
			}

			(*phandler)();
		}
	}
};

template<typename Handler>
class controller
	: public base_controller
{
public:
	typedef Handler handler_type;
	typedef func::foo_t_info<handler_type> foo_t_info_type;

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
		>::type handler_args_type;

	typedef base_controller base_type;
	typedef base_type::param_type param_type;
	typedef base_type::handler_wrap_type handler_wrap_type;

private:
	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			(foo_t_info_type::arg_holder_list_size <= YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
		>));

	typedef detail::ctrl_center_types_def::bad_cast_type bad_cast_type;

private:
	typedef yggr::safe_container::safe_vector<handler_wrap_type> handler_vt_type;
	typedef handler_vt_type::base_type unsafe_handler_vt_type;
	typedef handler_vt_type::iterator handler_vt_iter_type;
	typedef handler_vt_type::const_iterator handler_vt_citer_type;

private:
	typedef
		base_controller_handler_wrap_op_helper
		<
			handler_type
		> handler_wrap_op_helper_type;

private:
	typedef controller this_type;

public:
	controller(BOOST_RV_REF(handler_type) handler)
	{
		_handler_vt.emplace_back(boost::move(handler));
	}

	controller(const handler_type& handler)
	{
		_handler_vt.emplace_back(handler);
	}

	controller(const handler_wrap_type& handler_wrap)
	{
		_handler_vt.push_back(handler_wrap);
	}

	~controller(void)
	{
	}

public:
	virtual bool insert(const handler_wrap_type& handler_wrap)
	{
		handler_wrap_op_helper_type helper(handler_wrap);
		return
			this_type::prv_s_validate_handler_wrap(handler_wrap)
			&& _handler_vt.using_handler(
					boost::bind(
						&this_type::prv_s_handler_insert,
						_1, boost::cref(helper) ) );
	}

	virtual std::size_t erase(const handler_wrap_type& handler_wrap)
	{
		handler_wrap_op_helper_type helper(handler_wrap);
		return
			this_type::prv_s_validate_handler_wrap(handler_wrap)?
				_handler_vt.erase(
					boost::bind(
						&this_type::prv_s_handler_erase_iter,
						_1, boost::cref(helper)) )? 1 : 0
				: 0;
	}

	virtual void clear(void)
	{
		_handler_vt.clear();
	}

public:
	virtual const std::type_info& type(void) const
	{
		return typeid(handler_type);
	}

	virtual bool empty(void) const
	{
		return _handler_vt.empty();
	}

protected:
	virtual void pro_ctrl(const param_type& params) const
	{
		typedef
			handler_caller
			<
				foo_t_info_type::arg_holder_list_size,
				handler_type,
				handler_args_type,
				unsafe_handler_vt_type
			> handler_caller_type;

		unsafe_handler_vt_type cont = _handler_vt;

		if(cont.empty())
		{
			return;
		}

		handler_caller_type caller;
		caller(cont, params);
	}

private:
	inline static bool prv_s_validate_handler_wrap(const handler_wrap_type& handler_wrap)
	{
		return !!::yggr::any_cast<handler_type>(boost::addressof(handler_wrap));
	}

	inline static bool prv_s_handler_insert(unsafe_handler_vt_type& base,
											const handler_wrap_op_helper_type& helper)
	{
		assert((this_type::prv_s_validate_handler_wrap(helper.value())));

		return
			(std::find_if(base.begin(), base.end(), helper) == base.end())
			&& (base.push_back(helper.value()), true);
	}

	inline static handler_vt_iter_type
		prv_s_handler_erase_iter(unsafe_handler_vt_type& base,
									const handler_wrap_op_helper_type& helper)
	{
		assert((this_type::prv_s_validate_handler_wrap(helper.value())));

		return std::find_if(boost::begin(base), boost::end(base), helper);
	}

private:
	handler_vt_type _handler_vt;
};

} // namespace detail
} // system_controller
} // namespace yggr

#endif // __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_CONTROLLER_HPP__
