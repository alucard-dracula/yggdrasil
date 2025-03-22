//proxy_calculator.hpp

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

#ifndef __YGGR_PROXY_PROXY_REPEATER_HPP__
#define __YGGR_PROXY_PROXY_REPEATER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/proxy/detail/basic_repeater.hpp>
#include <yggr/proxy/proxy_args/default_args_builder.hpp>
#include <yggr/proxy/proxy_special_data_filter.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range/value_type.hpp>

namespace yggr
{
namespace proxy
{

template<	typename SharedInfoPtr,
			typename ProxyData,
			typename OwnerInfoContainer,
			typename Error,
			typename Router,
			typename ArgsBuilder
				= proxy_args::default_args_builder
					<
						typename boost::range_value<OwnerInfoContainer>::type,
						ProxyData
					>,
			typename Special_Data_Filter
				= proxy_special_data_filter
					<
						ProxyData,
						void
					>
		>
class proxy_repeater
	: public boost::enable_shared_from_this
				<
					proxy_repeater
					<
						SharedInfoPtr,
						ProxyData,
						OwnerInfoContainer,
						Error,
						Router,
						ArgsBuilder,
						Special_Data_Filter
					>
				>,
		public detail::basic_repeater<SharedInfoPtr, Router>
{

public:
	typedef SharedInfoPtr shared_info_ptr_type;
	typedef ProxyData proxy_data_type;
	typedef OwnerInfoContainer owner_info_container_type;
	typedef Error error_type;
	typedef Router router_type;
	typedef ArgsBuilder args_builder_type;
	typedef Special_Data_Filter special_data_filter_type;

	typedef typename proxy_data_type::data_info_type data_info_type;

	typedef typename boost::range_value<owner_info_container_type>::type owner_info_type;
	typedef detail::basic_repeater<shared_info_ptr_type, router_type> base_type;

	typedef typename base_type::proxy_id_type proxy_id_type;
	typedef typename base_type::router_ptr_type router_ptr_type;
	typedef typename base_type::router_wrap_error_type router_wrap_error_type;

	typedef safe_container::safe_wrap<args_builder_type> args_builder_wrap_type;
	typedef typename args_builder_wrap_type::value_ptr_type args_builder_ptr_type;
	typedef typename args_builder_wrap_type::error_type args_builder_wrap_error_type;

private:

	// don't move reg_cal_obj_helper to outside
	template<typename Container, typename Real_Data = void, typename Fix_Handler = void, typename Seh_Handler = void>
	struct reg_cal_obj_helper;

	template<typename Container>
	struct reg_cal_obj_helper<Container, void, void, void>
	{
		typedef Container container_type;
		typedef proxy_repeater outside_type;
		typedef typename outside_type::error_type error_type;
		typedef typename outside_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(outside_type& outside,
							const data_info_type& data_info,
							u32 cal_t)
			: _outside(outside), _data_info(data_info), _cal_t(cal_t)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Runner, typename Action_Table, typename Recv_Handler> inline
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			//at.template register_calculator<container_type, Recv_Handler>(
			//	_data_info, handler, 
			//	boost::bind(&outside_type::handler_proxy_data<runner_type,
			//													recv_handler_type>,
			//					_outside.this_type::shared_from_this(), _1, _2, _3, _4, _cal_t));

			at.template register_calculator<container_type, Recv_Handler>(
				_data_info, handler, 
				boost::bind(&outside_type::handler_proxy_data<runner_type,
																recv_handler_type>,
								_outside.shared_from_this(), _1, _2, _3, _4, _cal_t));
		}

	private:
		outside_type& _outside;
		const data_info_type& _data_info;
		u32 _cal_t;
	};

	template<typename Container, typename Fix_Handler>
	struct reg_cal_obj_helper<Container, void, Fix_Handler, void>
	{
		typedef Container container_type;
		typedef Fix_Handler fix_handler_type;
		typedef proxy_repeater outside_type;
		typedef typename outside_type::error_type error_type;
		typedef typename outside_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(const data_info_type& data_info,
							const fix_handler_type& fix_handler)
			: _data_info(data_info),
				_fix_handler(fix_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Runner, typename Action_Table, typename Recv_Handler> inline
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator<container_type, Recv_Handler>(
				_data_info, handler, _fix_handler);
		}

	private:
		const data_info_type& _data_info;
		const fix_handler_type& _fix_handler;
	};

	template<typename Container, typename Real_Data, typename Fix_Handler>
	struct reg_cal_obj_helper<Container, Real_Data, Fix_Handler, void>
	{
		typedef Container container_type;
		typedef Real_Data real_data_type;
		typedef Fix_Handler fix_handler_type;
		typedef proxy_repeater outside_type;
		typedef typename outside_type::error_type error_type;
		typedef typename outside_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(const fix_handler_type& fix_handler)
			: _fix_handler(fix_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Runner, typename Action_Table, typename Recv_Handler> inline
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator_of_container<real_data_type, container_type, Recv_Handler>(
				handler, _fix_handler);
		}

	private:
		const fix_handler_type& _fix_handler;
	};


#ifdef YGGR_USE_SEH //seh ver
	template<typename Container, typename Real_Data, typename Fix_Handler, typename Seh_Handler>
	struct reg_cal_obj_helper
	{
		typedef Container container_type;
		typedef Real_Data real_data_type;
		typedef Fix_Handler fix_handler_type;
		typedef Seh_Handler seh_handler_type;
		typedef proxy_repeater outside_type;
		typedef typename outside_type::error_type error_type;
		typedef typename outside_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(const fix_handler_type& fix_handler,
							const seh_handler_type& seh_handler)
			: _fix_handler(fix_handler), _seh_handler(seh_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Runner, typename Action_Table, typename Recv_Handler> inline
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator_of_container<real_data_type, container_type, Recv_Handler>(
				handler, _fix_handler, _seh_handler);
		}

	private:
		const fix_handler_type& _fix_handler;
		const seh_handler_type& _seh_handler;
	};

	template<typename Container, typename Fix_Handler, typename Seh_Handler>
	struct reg_cal_obj_helper<Container, void, Fix_Handler, Seh_Handler>
	{
		typedef Container container_type;
		typedef Fix_Handler fix_handler_type;
		typedef Seh_Handler seh_handler_type;
		typedef proxy_repeater outside_type;
		typedef typename outside_type::error_type error_type;
		typedef typename outside_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(const data_info_type& data_info,
							const fix_handler_type& fix_handler,
							const seh_handler_type& seh_handler)
			: _data_info(data_info),
				_fix_handler(fix_handler), 
				_seh_handler(seh_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Runner, typename Action_Table, typename Recv_Handler> inline
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator<container_type, Recv_Handler>(
				_data_info, handler, _fix_handler, _seh_handler);
		}

	private:
		const data_info_type& _data_info;
		const fix_handler_type& _fix_handler;
		const seh_handler_type& _seh_handler;
	};

	template<typename Container, typename Seh_Handler>
	struct reg_cal_obj_helper<Container, void, void, Seh_Handler>
	{
		typedef Container container_type;
		typedef Seh_Handler seh_handler_type;
		typedef proxy_repeater outside_type;
		typedef typename outside_type::error_type error_type;
		typedef typename outside_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(outside_type& outside,
							const data_info_type& data_info,
							u32 cal_t,
							const seh_handler_type& seh_handler)
			: _outside(outside),
				_data_info(data_info),
				_cal_t(cal_t),
				_seh_handler(seh_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Runner, typename Action_Table, typename Recv_Handler> inline
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			//at.template register_calculator_helper<container_type, Recv_Handler>(
			//	_data_info, handler, 
			//	boost::bind(&outside_type::handler_proxy_data<runner_type,
			//													recv_handler_type>,
			//				_outside.this_type::shared_from_this(), _1, _2, _3, _4,
			//				_cal_t),
			//	seh_handler);

			at.template register_calculator_helper<container_type, Recv_Handler>(
				_data_info, handler, 
				boost::bind(&outside_type::handler_proxy_data<runner_type,
																recv_handler_type>,
							_outside.shared_from_this(), _1, _2, _3, _4,
							_cal_t),
				seh_handler);
		}

	private:
		outside_type& _outside;
		const data_info_type& _data_info;
		u32 _cal_t;
		const seh_handler_type& _seh_handler;
	};

#endif // YGGR_USE_SEH

	// don't move unreg_cal_obj_helper to outside
	template<typename Real_Data = void, typename Nil_T = int>
	struct unreg_cal_obj_helper
	{
		typedef Real_Data real_data_type;

	public:
		unreg_cal_obj_helper(void)
		{
		}

		~unreg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Action_Table> inline
		void unregister_cal_object(Action_Table& at)
		{
			at.template unregister_calculator<real_data_type>();
		}
	};

	template<typename Nil_T>
	struct unreg_cal_obj_helper<void, Nil_T>
	{
	public:
		unreg_cal_obj_helper(const data_info_type& data_info)
			: _data_info(data_info)
		{
		}

		~unreg_cal_obj_helper(void)
		{
		}

	public:
		template<typename Action_Table> inline
		void unregister_cal_object(Action_Table& at)
		{
			at.unregister_calculator(_data_info);
		}

	private:
		const data_info_type& _data_info;
	};

private:
	typedef proxy_repeater this_type;

public:
	proxy_repeater(const shared_info_ptr_type& siptr, 
					const router_ptr_type& prouter,
					const args_builder_ptr_type& pargs_builder)
		: base_type(siptr, prouter), 
			_args_builder_wrap(pargs_builder)
	{
	}

	~proxy_repeater(void)
	{
	}

public:
	template<typename RealData, typename Fix_Handler> inline
	void register_repeater_data(const Fix_Handler& fix_handler)
	{
		typedef RealData real_data_type;
		typedef Fix_Handler fix_handler_type;

		typedef 
			reg_cal_obj_helper
			<
				proxy_data_type,
				real_data_type,
				fix_handler_type
			> reg_cont_type;
		
		if(_sp_data_filter.template append<real_data_type>())
		{
			reg_cont_type cont(fix_handler);
			base_type::register_repeater_data(cont);
		}
	}

	template<typename RealData, typename Fix_Handler> inline
	void register_repeater_special_data(const Fix_Handler& fix_handler)
	{
		typedef RealData real_data_type;

		this_type::register_repeater_data<real_data_type>(fix_handler);
	}

	template<typename Fix_Handler> inline 
	typename 
		boost::enable_if
		<
			typename func::foo_t_info<Fix_Handler>::is_callable_type,
			void
		>::type
		register_repeater_data(const data_info_type& dinfo, const Fix_Handler& fix_handler)
	{
		typedef Fix_Handler fix_handler_type;
		typedef
			reg_cal_obj_helper
			<
				proxy_data_type, 
				void, 
				fix_handler_type
			> reg_cont_type;

		if(!_sp_data_filter.need_filter(dinfo))
		{
			reg_cont_type cont(dinfo, fix_handler);
			base_type::register_repeater_data(cont);
		}
	}

	inline void register_repeater_data(const data_info_type& dinfo, u32 cal_t)
	{
		typedef reg_cal_obj_helper<proxy_data_type> reg_cont_type;

		if(!_sp_data_filter.need_filter(dinfo))
		{
			reg_cont_type cont(*this, dinfo, cal_t);
			base_type::register_repeater_data(cont);
		}
	}

#ifdef YGGR_USE_SEH //seh ver
	template<typename RealData, typename Fix_Handler, typename Seh_Handler> inline
	void register_repeater_data(const Fix_Handler& fix_handler, const Seh_Handler& seh_handler)
	{
		typedef RealData real_data_type;
		typedef Fix_Handler fix_handler_type;
		typedef Seh_Handler seh_handler_type;
		typedef reg_cal_obj_helper
				<
					proxy_data_type,
					real_data_type,
					fix_handler_type,
					seh_handler_type
				> reg_cont_type;

		if(_sp_data_filter.template append<real_data_type>())
		{
			reg_cont_type cont(fix_handler, seh_handler);
			base_type::register_repeater_data(cont);
		}
	}

	template<typename RealData, typename Fix_Handler, typename Seh_Handler> inline
	void register_repeater_special_data(const Fix_Handler& fix_handler, const Seh_Handler& seh_handler)
	{
		typedef RealData real_data_type;
		this_type::register_repeater_data<real_data_type>(fix_handler, seh_handler);
	}

	template<typename Fix_Handler, typename Seh_Handler> inline 
	typename 
		boost::enable_if
		<
			typename func::foo_t_info<Fix_Handler>::is_callable_type,
			void
		>::type
		register_repeater_data(const data_info_type& dinfo, const Fix_Handler& fix_handler, const Seh_Handler& seh_handler)
	{
		typedef Fix_Handler fix_handler_type;
		typedef Seh_Handler seh_handler_type;
		typedef
			reg_cal_obj_helper
			<
				proxy_data_type, 
				void, 
				fix_handler_type,
				seh_handler_type
			> reg_cont_type;

		if(!_sp_data_filter.need_filter(dinfo))
		{
			reg_cont_type cont(dinfo, fix_handler, seh_handler);
			base_type::register_repeater_data(cont);
		}
	}

	template<typename Seh_Handler> inline
	void register_repeater_data(const data_info_type& dinfo, u32 cal_t, const Seh_Handler& seh_handler)
	{
		typedef Seh_Handler seh_handler_type;
		typedef reg_cal_obj_helper<proxy_data_type, void, void, seh_handler_type> reg_cont_type;

		if(!_sp_data_filter.need_filter(dinfo))
		{
			reg_cont_type cont(*this, dinfo, cal_t, seh_handler);
			base_type::register_repeater_data(cont);
		}
	}

#endif // YGGR_USE_SEH

	template<typename RealData> inline
	void unregister_repeater_data(void)
	{
		typedef RealData real_data_type;
		typedef unreg_cal_obj_helper<real_data_type> unreg_cont_type;

		if(_sp_data_filter.template remove<real_data_type>())
		{
			unreg_cont_type cont;
			base_type::unregister_repeater_data(cont);
		}
	}

	template<typename RealData> inline
	void unregister_repeater_special_data(void)
	{
		typedef RealData real_data_type;

		this_type::unregister_repeater_data<real_data_type>();
	}

	inline void unregister_repeater_data(const data_info_type& dinfo)
	{
		typedef unreg_cal_obj_helper<> unreg_cont_type;

		if(!_sp_data_filter.need_filter(dinfo))
		{
			unreg_cont_type cont(dinfo);
			base_type::unregister_repeater_data(cont);
		}
	}

	// only op special_data
	
	template<typename RealData> inline
	bool append_special_data(void)
	{
		typedef RealData real_data_type;

		return _sp_data_filter.template append<real_data_type>();
	}

	inline bool append_special_data(const data_info_type& info)
	{
		return _sp_data_filter.append(info);
	}

	template<typename RealData> inline
	bool remove_special_data(void)
	{
		typedef RealData real_data_type;

		return _sp_data_filter.template remove<real_data_type>();
	}

	inline bool remove_special_data(const data_info_type& info)
	{
		return _sp_data_filter.remove(info);
	}

	inline void clear(void)
	{
		base_type::clear();
		_sp_data_filter.clear();
	}

	using base_type::get_shared_info;
	using base_type::get_router;

	template<typename Runner, typename Handler> inline
	void route(const owner_info_type& owner,
				const proxy_data_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_t) const
	{
		this_type::handler_proxy_data(owner, proxy_data, prunner, handler, cal_t);
	}

	template<typename Runner, typename Handler, typename Args> inline
	void route(const owner_info_type& owner,
				const proxy_data_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_t,
				Args& args) const
	{
		this_type::handler_proxy_data_of_args(owner, proxy_data, prunner, handler, cal_t, args);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Tag, typename ...Args> inline
	bool modify_args_build_rule(BOOST_FWD_REF(Args)... args)
	{
		typedef Tag tag_type;
		args_builder_ptr_type ptr = _args_builder_wrap.get_shared_ptr();
		return ptr && ptr->template modify_rule<tag_type>(boost::forward<Args>(args)...);
	}

	template<typename Tag, typename ...Args> inline
	bool check_args_build_rule(BOOST_FWD_REF(Args)... args) const
	{
		typedef Tag tag_type;
		args_builder_ptr_type ptr = _args_builder_wrap.get_shared_ptr();
		return ptr && ptr->template check_rule<tag_type>(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< \
			typename Tag \
			YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
			YGGR_PP_FOO_TYPES_DEF( __n__ )  >  inline \
		bool modify_args_build_rule( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Tag tag_type; \
			args_builder_ptr_type ptr = _args_builder_wrap.get_shared_ptr(); \
			return ptr && ptr->template modify_rule<tag_type>(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template< \
			typename Tag \
			YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
			YGGR_PP_FOO_TYPES_DEF( __n__ )  >  inline \
		bool check_args_build_rule( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Tag tag_type; \
			args_builder_ptr_type ptr = _args_builder_wrap.get_shared_ptr(); \
			return ptr && ptr->template check_rule<tag_type>(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	template<typename Runner, typename Handler> inline
	void handler_proxy_data(const owner_info_type& owner,
								const proxy_data_type& proxy_data,
								Runner* prunner,
								const Handler& handler,
								u32 cal_t) const
	{

		args_builder_ptr_type pargs_builde = _args_builder_wrap.get_shared_ptr();

		if(pargs_builde)
		{
			pargs_builde->route(
				base_type::get_router(), owner, proxy_data, prunner, handler, cal_t);
		}
	}

	template<typename Runner, typename Handler, typename Args> inline
	void handler_proxy_data_of_args(const owner_info_type& owner,
									const proxy_data_type& proxy_data,
									Runner* prunner,
									const Handler& handler,
									u32 cal_t,
									Args& args) const
	{

		args_builder_ptr_type pargs_builde = _args_builder_wrap.get_shared_ptr();

		if(pargs_builde)
		{
			pargs_builde->route(
				base_type::get_router(), owner, proxy_data, prunner, handler, cal_t, args);
		}
	}


protected:
	args_builder_wrap_type _args_builder_wrap;
	special_data_filter_type _sp_data_filter;
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_REPEATER_HPP__
