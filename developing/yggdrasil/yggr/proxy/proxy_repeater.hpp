//proxy_calculator.hpp

#ifndef __YGGR_PROXY_PROXY_REPEATER_HPP__
#define __YGGR_PROXY_PROXY_REPEATER_HPP__

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/mplex/get_container_value_t.hpp>
#include <yggr/proxy/detail/basic_repeater.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

namespace yggr
{
namespace proxy
{

template<	typename SharedInfoPtr,
			typename ProxyData,
			typename OwnerInfoContainer,
			typename Error,
			typename Router
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
						Router
					>
				>,
		public yggr::proxy::detail::basic_repeater<SharedInfoPtr>
{

public:
	typedef SharedInfoPtr shared_info_ptr_type;
	typedef ProxyData proxy_data_type;
	typedef OwnerInfoContainer owner_info_container_type;
	typedef Error error_type;
	typedef Router router_type;

	typedef typename proxy_data_type::data_info_type data_info_type;

	typedef typename mplex::get_container_value_t<owner_info_container_type>::type owner_info_type;
	typedef yggr::proxy::detail::basic_repeater<SharedInfoPtr> base_type;

	typedef typename router_type::proxy_val_type proxy_id_type;
	typedef safe_container::safe_wrap<router_type> router_wrap_type;
	typedef typename router_wrap_type::value_ptr_type router_ptr_type;
	typedef typename router_wrap_type::error_type router_wrap_error_type;

private:

	template<typename Container, typename Real_Data = void, typename Seh_Handler = void>
	struct reg_cal_obj_helper;

	template<typename Container, typename Real_Data>
	struct reg_cal_obj_helper<Container, Real_Data, void>
	{
		typedef Container container_type;
		typedef Real_Data real_data_type;
		typedef proxy_repeater parent_type;
		typedef typename parent_type::error_type error_type;
		typedef typename parent_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(parent_type& parent)
			: _parent(parent)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

		template<typename Runner, typename Action_Table, typename Recv_Handler>
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator_of_container<real_data_type, container_type, Recv_Handler>(
				handler, boost::bind(&parent_type::handler_proxy_data<runner_type,
																		recv_handler_type>,
										_parent.shared_from_this(), _1, _2, _3, _4,
										real_data_type::s_cal_type()));
		}

	private:
		parent_type& _parent;
	};

	template<typename Container>
	struct reg_cal_obj_helper<Container, void, void>
	{
		typedef Container container_type;
		typedef proxy_repeater parent_type;
		typedef typename parent_type::error_type error_type;
		typedef typename parent_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(parent_type& parent,
							const data_info_type& data_info,
							u32 cal_type)
			: _parent(parent), _data_info(data_info), _cal_type(cal_type)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

		template<typename Runner, typename Action_Table, typename Recv_Handler>
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator<container_type, Recv_Handler>(
				_data_info, handler, boost::bind(&parent_type::handler_proxy_data<runner_type,
																		recv_handler_type>,
										_parent.shared_from_this(), _1, _2, _3, _4,
										_cal_type));
		}

	private:
		parent_type& _parent;
		const data_info_type& _data_info;
		u32 _cal_type;
	};

#ifdef YGGR_USE_SEH //seh ver
	template<typename Container, typename Real_Data, typename Seh_Handler>
	struct reg_cal_obj_helper
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;
		typedef Seh_Handler seh_handler_type;
		typedef proxy_repeater parent_type;
		typedef typename parent_type::error_type error_type;
		typedef typename parent_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(parent_type& parent,
											const seh_handler_type& seh_handler)
			: _parent(parent), _seh_handler(seh_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

		template<typename Runner, typename Action_Table, typename Recv_Handler>
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator_of_container<real_data_type, container_type, Recv_Handler>(
				handler, boost::bind(&parent_type::handler_proxy_data<runner_type,
																		recv_handler_type>,
										_parent.shared_from_this(), _1, _2, _3, _4,
										real_data_type::s_cal_type()),

				_seh_handler);
		}

	private:
		parent_type& _parent;
		const seh_handler_type& _seh_handler;
	};

	template<typename Container, typename Seh_Handler>
	struct reg_cal_obj_helper<Container, void, SehHandler>
	{
		typedef Container container_type;
		typedef Seh_Handler seh_handler_type;
		typedef proxy_repeater parent_type;
		typedef typename parent_type::error_type error_type;
		typedef typename parent_type::owner_info_container_type owner_info_container_type;

		reg_cal_obj_helper(parent_type& parent,
							const data_info_type& data_info,
							u32 cal_type,
							const seh_handler_type& seh_handler)
			: _parent(parent),
				_data_info(data_info),
				_cal_type(cal_type),
				_seh_handler(seh_handler)
		{
		}

		~reg_cal_obj_helper(void)
		{
		}

		template<typename Runner, typename Action_Table, typename Recv_Handler>
		void register_cal_object(Action_Table& at, const Recv_Handler& handler)
		{
			typedef Runner runner_type;
			typedef Action_Table action_table_type;
			typedef Recv_Handler recv_handler_type;

			at.template register_calculator_helper<container_type, Recv_Handler>(
				_data_info, handler, boost::bind(&parent_type::handler_proxy_data<runner_type,
																		recv_handler_type>,
										_parent.shared_from_this(), _1, _2, _3, _4,
										_cal_type),
				seh_handler);
		}

	private:
		parent_type& _parent;
		const data_info_type& _data_info;
		u32 _cal_type;
		const seh_handler_type& _seh_handler;
	};

#endif // YGGR_USE_SEH

	//template<typename Real_Data = void, typename Nil_T = int>
	//struct unreg_cal_obj;

	template<typename Real_Data = void, typename Nil_T = int>
	struct unreg_cal_obj
	{
		typedef Real_Data real_data_type;

		unreg_cal_obj(void)
		{
		}

		~unreg_cal_obj(void)
		{
		}

		template<typename Action_Table>
		void unregister_cal_object(Action_Table& at)
		{
			at.template unregister_calculator<real_data_type>();
		}
	};

	template<typename Nil_T>
	struct unreg_cal_obj<void, Nil_T>
	{
		unreg_cal_obj(const data_info_type& data_info)
			: _data_info(data_info)
		{
		}

		~unreg_cal_obj(void)
		{
		}

		template<typename Action_Table>
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
	proxy_repeater(const shared_info_ptr_type& siptr, const router_ptr_type& prouter)
		: base_type(siptr), _router_wrap(prouter)
	{
	}

	~proxy_repeater(void)
	{
	}

	template<typename RealData>
	void register_repeater_data(void)
	{
		typedef RealData real_data_type;
		typedef reg_cal_obj_helper
				<
					proxy_data_type,
					real_data_type
				> reg_cont_type;
		reg_cont_type cont(*this);
		base_type::register_repeater_data(cont);
	}

	void register_repeater_data(const data_info_type& dinfo, u32 cal_type)
	{
		typedef reg_cal_obj_helper<proxy_data_type> reg_cont_type;

		reg_cont_type cont(*this, dinfo, cal_type);
		base_type::register_repeater_data(cont);
	}

#ifdef YGGR_USE_SEH //seh ver
	template<typename RealData, typename Seh_Handler>
	void register_repeater_data(const Seh_Handler& seh_handler)
	{
		typedef RealData real_data_type;
		typedef Seh_Handler seh_handler_type;
		typedef reg_cal_obj_helper
				<
					proxy_data_type,
					real_data_type,
					seh_handler_type
				> reg_cont_type;
		reg_cont_type cont(*this, seh_handler);
		base_type::register_repeater_data(cont);
	}

	template<typename Seh_Handler>
	void register_repeater_data(const data_info_type& dinfo, u32 cal_type, const Seh_Handler& seh_handler)
	{
		typedef Seh_Handler seh_handler_type;
		typedef reg_cal_obj_helper<proxy_data_type, void, seh_handler_type> reg_cont_type;

		reg_cont_type cont(*this, dinfo, cal_type, seh_handler);
		base_type::register_repeater_data(cont);
	}

#endif // YGGR_USE_SEH

	template<typename RealData>
	void unregister_repeater_data(void)
	{
		typedef RealData real_data_type;
		typedef unreg_cal_obj<real_data_type> unreg_cont_type;

		unreg_cont_type cont;
		base_type::unregister_repeater_data(cont);
	}

	void unregister_repeater_data(const data_info_type& dinfo)
	{
		typedef unreg_cal_obj<> unreg_cont_type;

		unreg_cont_type cont(dinfo);
		base_type::unregister_repeater_data(cont);
	}

	void clear(void)
	{
		base_type::clear();
	}

private:

	template<typename Runner, typename Handler>
	void handler_proxy_data(const owner_info_type& owner,
								const proxy_data_type& proxy_data,
								Runner* prunner,
								const Handler& handler,
								u32 cal_type)
	{
		router_ptr_type prouter = _router_wrap.get_shared_ptr();

		if(!prouter)
		{
			return;
		}

		prouter->route(owner, proxy_data, prunner, handler, cal_type);
	}


private:
	//router_type& _router;
	router_wrap_type _router_wrap;
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_REPEATER_HPP__
