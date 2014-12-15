//ctrl_center.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_CTRL_CENTER_HPP__
#define __YGGR_SYSTEM_CONTROLLER_CTRL_CENTER_HPP__

#include <algorithm>
#include <sstream>
#include <cassert>
#include <list>

#include <boost/any.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/interface_ptr.hpp>
#include <yggr/base/ptr_single.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/ids/inner_process_id.hpp>

#include <yggr/struct_support/tuple.hpp>
#include <yggr/struct_support/bind.hpp>

#include <yggr/safe_container/safe_list.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>

#define CTRL_HANDLER_PARAM_0(__tag__, __ctrl_obj__, __code__, __class_code__, __handler__) \
	{ __tag__ tag_obj0tmp; tag_obj0tmp.fix0(__ctrl_obj__, __code__, __class_code__, __handler__); }

#define CTRL_HANDLER_PARAM_1(__tag__, __ctrl_obj__, __param_type__, __code__, __class_code__, __handler__) \
	{ __tag__ tag_obj1tmp; tag_obj1tmp.template fix1<__param_type__>(__ctrl_obj__, __code__, __class_code__, __handler__); }


namespace yggr
{
namespace system_controller
{

class ctrl_center
{
public:

	typedef u32 code_type;
	typedef u64 ctrl_id_type;
	typedef yggr::ids::inner_process_id modules_id_type;
	typedef std::string handler_id_type;
	typedef boost::any param_type;
	typedef boost::any handler_wrap_type;

private:

	class base_controller
	{
	public:
		virtual ~base_controller(void)
		{
		}

		virtual const char* typeid_name(void) const = 0;
		virtual bool insert(const handler_wrap_type&) = 0;
		virtual void erase(const handler_wrap_type&) = 0;
		virtual bool empty(void) const = 0;
		virtual void ctrl(const param_type& param) const = 0;
		virtual void clear(void) = 0;

	protected:
		template<typename Handler>
		static bool equal_to_of_handler(const handler_wrap_type& left, const Handler& right)
		{
			typedef Handler handler_type;
			const handler_type* phandler = boost::any_cast<handler_type>(&left);
			
			if(phandler == &right)
			{
				return true;
			}

			return phandler && *phandler == right;
		}

		template<typename Handler>
		static bool equal_to_of_handler_wrap(const handler_wrap_type& left, const handler_wrap_type& right)
		{
			typedef Handler handler_type;
			const handler_type* pleft = boost::any_cast<handler_type>(&left);
			const handler_type* pright = boost::any_cast<handler_type>(&right);
			
			if((&left == &right) || (pleft == pright))
			{
				return true;
			}

			return pleft && pright && *pleft == *pright;
		}
	};

	typedef interface_ptr<base_controller> base_ctrl_ptr_type;

	template<typename Handler>
	class controller_param_0 
		: public base_controller
	{
		typedef Handler handler_type;
		typedef yggr::safe_container::safe_list<handler_wrap_type> handler_list_type;
		typedef typename handler_list_type::iterator handler_list_iter_type;
		typedef typename handler_list_type::const_iterator handler_list_citer_type;

	public:
		controller_param_0(const handler_type& handler)
		{
			_handler_list.push_back(handler_wrap_type(handler));
		}

		virtual ~controller_param_0(void)
		{
		}

		virtual const char* typeid_name(void) const
		{
			return typeid(handler_type).name();
		}

		virtual bool insert(const handler_wrap_type& handler_wrap)
		{
			return _handler_list.use_handler(
									boost::bind(
													&controller_param_0::handler_insert,
													this, _1, boost::cref(handler_wrap)
												)
											);
		}

		virtual void erase(const handler_wrap_type& handler_wrap)
		{
			const handler_type* p = boost::any_cast<handler_type>(&handler_wrap);
			if(!p)
			{
				return;
			}

			_handler_list.remove_if(
					boost::bind(&base_controller::template equal_to_of_handler_wrap<handler_type>,
									_1, boost::cref(handler_wrap)));
		}

		virtual bool empty(void) const
		{
			return _handler_list.empty();
		}

		virtual void ctrl(const param_type& param) const
		{
			typedef typename handler_list_type::base_type cont_type;

            assert(param.empty());
            if(!param.empty())
            {
                return;
            }

			cont_type cont;

			_handler_list.use_handler(
							boost::bind(
											&controller_param_0::handler_get_handlers,
											this, _1, boost::ref(cont)
										)
									);

			for(handler_list_citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
			{
				const handler_type* phandler = boost::any_cast<handler_type>(&(*i));
				assert(phandler);
				if(!phandler)
				{
					continue;
				}
				(*phandler)();
			}
		}

		virtual void clear(void)
		{
			_handler_list.clear();
		}

	private:
		bool handler_insert(typename handler_list_type::base_type& base, const handler_wrap_type& handler_wrap)
		{
			const handler_type* p = boost::any_cast<handler_type>(&handler_wrap);
			if(!p)
			{
				return false;
			}

			if(std::find_if(base.begin(), base.end(), 
								boost::bind(&base_controller::template equal_to_of_handler_wrap<handler_type>, 
								_1, boost::cref(handler_wrap))) == base.end())
			{
				base.push_back(handler_wrap);
				return true;
			}


			return false;
		}

		void handler_get_handlers(const typename handler_list_type::base_type& base,
									typename handler_list_type::base_type& cont) const
		{
			cont.assign(base.begin(), base.end());
		}

	private:
		handler_list_type _handler_list;
	};

	template<typename Handler, typename Real_Param>
	class controller_param_1 
		: public base_controller
	{
		typedef Handler handler_type;
		typedef Real_Param real_param_type;
		typedef yggr::safe_container::safe_list<handler_wrap_type> handler_list_type;
		typedef typename handler_list_type::iterator handler_list_iter_type;
		typedef typename handler_list_type::const_iterator handler_list_citer_type;
	public:
		controller_param_1(const handler_type& handler)
		{
			_handler_list.push_back(handler_wrap_type(handler));
		}

		virtual ~controller_param_1(void)
		{
		}

		virtual const char* typeid_name(void) const
		{
			return typeid(handler_type).name();
		}

		virtual void erase(const handler_wrap_type& handler_wrap)
		{
			const handler_type* p = boost::any_cast<handler_type>(&handler_wrap);
			if(!p)
			{
				return;
			}

			_handler_list.remove_if(
					boost::bind(&base_controller::template equal_to_of_handler_wrap<handler_type>,
									_1, boost::cref(handler_wrap)));
		}

		virtual bool insert(const handler_wrap_type& handler_wrap)
		{
			return _handler_list.use_handler(
									boost::bind(
													&controller_param_1::handler_insert,
													this, _1, boost::cref(handler_wrap)
												)
											);
		}

		virtual bool empty(void) const
		{
			return _handler_list.empty();
		}

		virtual void ctrl(const param_type& param) const
		{
			typedef typename handler_list_type::base_type cont_type;

			const real_param_type* p = boost::any_cast<real_param_type>(&param);
			if(!p)
			{
				return;
			}

			cont_type cont;

			_handler_list.use_handler(
							boost::bind(
											&controller_param_1::handler_get_handlers,
											this, _1, boost::ref(cont)
										)
									);

			for(handler_list_citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
			{
				const handler_type* phandler = boost::any_cast<handler_type>(&(*i));
				assert(phandler);
				if(!phandler)
				{
					continue;
				}
				(*phandler)(*p);
			}
		}

		virtual void clear(void)
		{
			_handler_list.clear();
		}

	private:
		bool handler_insert(typename handler_list_type::base_type& base, const handler_wrap_type& handler_wrap)
		{
			const handler_type* p = boost::any_cast<handler_type>(&handler_wrap);
			if(!p)
			{
				return false;
			}

			if(std::find_if(base.begin(), base.end(), 
								boost::bind(&base_controller::template equal_to_of_handler_wrap<handler_type>, 
								_1, boost::cref(handler_wrap))) == base.end())
			{
				base.push_back(handler_wrap);
				return true;
			}


			return false;
		}

		void handler_get_handlers(const typename handler_list_type::base_type& base,
									typename handler_list_type::base_type& cont) const
		{
			cont.assign(base.begin(), base.end());
		}

	private:
		handler_list_type _handler_list;
	};

	class dispatcher_container
	{
		typedef yggr::safe_container::safe_unordered_map<std::string, base_ctrl_ptr_type> controller_container_type;
		typedef controller_container_type::iterator controller_container_iter_type;
		typedef controller_container_type::const_iterator controller_container_citer_type;

		typedef std::list<base_ctrl_ptr_type> ctrl_cont_type;
		typedef ctrl_cont_type::iterator ctrl_cont_iter_type;
		typedef ctrl_cont_type::const_iterator ctrl_cont_citer_type;

	public:
		dispatcher_container(void);
		~dispatcher_container(void);

		void dispatcher(void) const;
		void dispatcher(const param_type& param) const;
		bool empty(void) const;
		void clear(void);


		template<typename Ctrl, typename Handler>
		void append_dispatcher(const Handler& handler)
		{
			typedef Ctrl ctrl_type;
			typedef Handler handler_type;

			_cont.use_handler(
					boost::bind(
									&dispatcher_container::handler_append_dispatcher
															<
																ctrl_type,
																handler_type
															>,
									this, _1, boost::cref(handler)
								)
							);
		}

		template<typename Handler>
		void erase_dispatcher(const Handler& handler)
		{
			typedef Handler handler_type;
			_cont.use_handler(
					boost::bind(
									&dispatcher_container::handler_erase_dispatcher
															<
																handler_type
															>,
									this, _1, boost::cref(handler)
								)
							);
		}

	private:
		template<typename Ctrl, typename Handler>
		void handler_append_dispatcher(controller_container_type::base_type& base,
											const Handler& handler)
		{
			typedef Ctrl ctrl_type;
			typedef Handler handler_type;
			typedef typename controller_container_type::value_type ins_val_type;
			typedef std::pair<controller_container_iter_type, bool> ins_rst_type;

			std::string name(typeid(handler_type).name());
			controller_container_iter_type iter = base.find(name);
			if(iter == base.end())
			{
				ins_rst_type rst = base.insert(ins_val_type(name, base_ctrl_ptr_type(new ctrl_type(handler))));
				if(rst.second)
				{
					iter = rst.first;
				}
			}

			if(iter == base.end() || !iter->second)
			{
				return;
			}

			iter->second->insert(handler_wrap_type(handler));
		}

		template<typename Handler>
		void handler_erase_dispatcher(controller_container_type::base_type& base,
										const Handler& handler)
		{
			typedef Handler handler_type;

			std::string name(typeid(handler_type).name());
			controller_container_iter_type iter = base.find(name);
			if(iter == base.end())
			{
				return;
			}

			if(!iter->second)
			{
				base.erase(iter);
				return;
			}

			iter->second->erase(handler_wrap_type(handler));

			if(iter->second->empty())
			{
				base.erase(iter);
			}
		}

		void handler_get_dispatchers(const controller_container_type::base_type& base,
										ctrl_cont_type& cont) const;

	private:
		controller_container_type _cont;
	};

	struct controller_group
	{
	public:
		controller_group(void);
		~controller_group(void);

		bool empty(void) const;
		void clear(void);
		void clear_controller(void);
		void clear_dispatcher(void);

		void ctrl(void) const;
		void ctrl(const param_type& param) const;
		void dispatch(void) const;
		void dispatch(const param_type& param) const;

		template<typename Ctrl, typename Handler>
		void append_controller(const Handler& handler)
		{
			_ctrl_cont.append_dispatcher<Ctrl>(handler);
		}

		template<typename Ctrl, typename Handler>
		void append_dispatcher(const Handler& handler)
		{
			_dis_cont.append_dispatcher<Ctrl>(handler);
		}

		template<typename Handler>
		void erase_controller(const Handler& handler)
		{
			_ctrl_cont.erase_dispatcher(handler);
		}

		template<typename Handler>
		void erase_dispatcher(const Handler& handler)
		{
			_dis_cont.erase_dispatcher(handler);
		}

	private:
		dispatcher_container _ctrl_cont;
		dispatcher_container _dis_cont;
	};

	typedef boost::shared_ptr<controller_group> controller_group_ptr_type;
	typedef std::list<controller_group_ptr_type> controller_group_ptr_list_type;

	typedef boost::tuple<modules_id_type, ctrl_id_type> midx_container_id_type;
	struct midx_container
	{
	public:
		struct tag_id {};
		struct tag_mid {};
		struct tag_cid {};

		midx_container(void);
		midx_container(const ctrl_id_type& ncid);
		midx_container(const modules_id_type& nmid,
						const ctrl_id_type& ncid);
		midx_container(const midx_container_id_type& nid);
		~midx_container(void);

		bool empty(void) const;

		const modules_id_type& modules_id(void) const;
		const ctrl_id_type& ctrl_id(void) const;
		controller_group_ptr_type get_group(void) const;


	public:
		midx_container_id_type id;
		controller_group_ptr_type pgroup;
	};

	typedef yggr::safe_container::safe_multi_index_container
			<
				midx_container,
				boost::multi_index::indexed_by
				<
					boost::multi_index::hashed_unique
					<
						boost::multi_index::tag<midx_container::tag_id>,
						BOOST_MULTI_INDEX_MEMBER(midx_container, midx_container_id_type, id)
					>,

					boost::multi_index::hashed_non_unique
					<
						boost::multi_index::tag<midx_container::tag_mid>,
						BOOST_MULTI_INDEX_CONST_MEM_FUN(midx_container, const modules_id_type&, modules_id)
					>,

					boost::multi_index::hashed_non_unique
					<
						boost::multi_index::tag<midx_container::tag_cid>,
						BOOST_MULTI_INDEX_CONST_MEM_FUN(midx_container, const ctrl_id_type&, ctrl_id)
					>
				>
			> ctrl_cont_type;

private:
	typedef yggr::ptr_single<modules_id_type> modules_id_single_type;
	typedef modules_id_single_type::obj_ptr_type modules_id_ptr_type;

	typedef ctrl_center this_type;

public:
	struct tag_reg_ctrl
	{
		template<typename Handler>
		ctrl_center& fix0(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.reg_ctrl(code, class_code, handler);
			}
			else
			{
				cc.reg_ctrl(*ptr, code, class_code, handler);
			}
			return cc;
		}

		template<typename Param, typename Handler>
		ctrl_center& fix1(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.reg_ctrl<Param>(code, class_code, handler);
			}
			else
			{
				cc.reg_ctrl<Param>(*ptr, code, class_code, handler);
			}
			return cc;
		}
	};

	struct tag_reg_dis
	{
		template<typename Handler>
		ctrl_center& fix0(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.reg_dis(code, class_code, handler);
			}
			else
			{
				cc.reg_dis(*ptr, code, class_code, handler);
			}
			return cc;
		}

		template<typename Param, typename Handler>
		ctrl_center& fix1(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.reg_dis<Param>(code, class_code, handler);
			}
			else
			{
				cc.reg_dis<Param>(*ptr, code, class_code, handler);
			}
			return cc;
		}
	};

	struct tag_unreg_ctrl
	{
		template<typename Handler>
		ctrl_center& fix0(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.unreg_ctrl(code, class_code, handler);
			}
			else
			{
				cc.unreg_ctrl(*ptr, code, class_code, handler);
			}
			return cc;
		}

		template<typename Param, typename Handler>
		ctrl_center& fix1(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.unreg_ctrl(code, class_code, handler);
			}
			else
			{
				cc.unreg_ctrl(*ptr, code, class_code, handler);
			}
			return cc;
		}
	};

	struct tag_unreg_dis
	{
		template<typename Handler>
		ctrl_center& fix0(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.unreg_dis(code, class_code, handler);
			}
			else
			{
				cc.unreg_dis(*ptr, code, class_code, handler);
			}
			return cc;
		}

		template<typename Param, typename Handler>
		ctrl_center& fix1(ctrl_center& cc, const code_type& code, const code_type& class_code, const Handler& handler)
		{
			modules_id_ptr_type ptr = modules_id_single_type::get_ins();
			if(!ptr)
			{
				cc.unreg_dis(code, class_code, handler);
			}
			else
			{
				cc.unreg_dis(*ptr, code, class_code, handler);
			}
			return cc;
		}
	};

public:

	ctrl_center(void);
	~ctrl_center(void);
	void ctrl(const code_type& code, const code_type& class_code) const;
	void ctrl(const code_type& code, const code_type& class_code, const param_type& param) const;
	void clear(void);

	template<typename Sys_Obj>
	this_type& register_controller(Sys_Obj& obj)
	{
		obj.template register_controller<tag_reg_ctrl, this_type>(*this);
		return *this;
	}

	template<typename Dis_Obj>
	this_type& register_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<tag_reg_dis, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Sys_Obj>
	this_type& register_controller(Sys_Obj& obj)
	{
		obj.template register_controller<Tag, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Dis_Obj>
	this_type& register_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<Tag, this_type>(*this);
		return *this;
	}

	template<typename Sys_Obj>
	this_type& unregister_controller(Sys_Obj& obj)
	{
		obj.template register_controller<tag_unreg_ctrl, this_type>(*this);
		return *this;
	}

	template<typename Dis_Obj>
	this_type& unregister_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<tag_unreg_dis, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Sys_Obj>
	this_type& unregister_controller(Sys_Obj& obj)
	{
		obj.template register_controller<Tag, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Dis_Obj>
	this_type& unregister_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<Tag, this_type>(*this);
		return *this;
	}

	this_type& unregister_module(void);

	// reg
	template<typename Param, typename Handler>
	this_type& reg_dis(const code_type& code, const code_type& class_code, const Handler& handler)
	{
		return this_type::reg_dis<Param>(modules_id_type(), code, class_code, handler);
	}

	template<typename Param, typename Handler>
	this_type& reg_dis(const modules_id_type& mid,
								const code_type& code,
								const code_type& class_code,
								const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef Param real_param_type;
		typedef controller_param_1<now_handler_type, real_param_type> now_ctrl_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));

		_ctrl_cont.use_handler(
								boost::bind(
												&this_type::handler_reg_dis
												<
													now_ctrl_type,
													now_handler_type
												>,
												this, _1,
												boost::cref(mid),
												boost::cref(ctrl_id),
												boost::any(handler)
											)
							);
		return *this;

	}

	template<typename Handler>
	this_type& reg_dis(const code_type& code, const code_type& class_code, const Handler& handler)
	{
		return this_type::reg_dis(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler>
	this_type& reg_dis(const modules_id_type& mid,
								const code_type& code,
								const code_type& class_code,
								const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef controller_param_0<now_handler_type> now_ctrl_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));

		_ctrl_cont.use_handler(
								boost::bind(
												&this_type::handler_reg_dis
												<
													now_ctrl_type,
													now_handler_type
												>,
												this, _1,
												boost::cref(mid),
												boost::cref(ctrl_id),
												boost::any(handler)
											)
							);

		return *this;
	}

	template<typename Param, typename Handler>
	this_type& reg_ctrl(const code_type& code,
						const code_type& class_code,
						const Handler& handler)
	{
		return this_type::reg_ctrl<Param>(modules_id_type(), code, class_code, handler);
	}

	template<typename Param, typename Handler>
	this_type& reg_ctrl(const modules_id_type& mid,
						const code_type& code,
						const code_type& class_code,
						const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef Param real_param_type;
		typedef controller_param_1<now_handler_type, real_param_type> now_ctrl_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));

		_ctrl_cont.use_handler(
								boost::bind(
												&this_type::handler_reg_ctrl
												<
													now_ctrl_type,
													now_handler_type
												>,
												this, _1,
												boost::cref(mid),
												boost::cref(ctrl_id),
												boost::any(handler)
											)
							);

		return *this;
	}

	template<typename Handler>
	this_type& reg_ctrl(const code_type& code,
						const code_type& class_code,
						const Handler& handler)
	{
		return this_type::reg_ctrl(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler>
	this_type& reg_ctrl(const modules_id_type& mid,
						const code_type& code,
						const code_type& class_code,
						const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef controller_param_0<now_handler_type> now_ctrl_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));

		_ctrl_cont.use_handler(
								boost::bind(
												&this_type::handler_reg_ctrl
												<
													now_ctrl_type,
													now_handler_type
												>,
												this, _1,
												boost::cref(mid),
												boost::cref(ctrl_id),
												boost::any(handler)
											)
							);

		return *this;
	}

	// unreg

	this_type& unreg_dis(const modules_id_type& mid);

	template<typename Handler>
	this_type& unreg_dis(const code_type& code,
									const code_type& class_code,
									const Handler& handler)
	{
		return this_type::unreg_dis(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler>
	this_type& unreg_dis(const modules_id_type& mid,
									const code_type& code,
									const code_type& class_code,
									const Handler& handler)
	{
		typedef Handler now_handler_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));

		_ctrl_cont.use_handler(
								boost::bind(
												&this_type::handler_unreg_dis_of_sub
												<
													now_handler_type
												>, this, _1,
												boost::cref(mid),
												boost::cref(ctrl_id),
												boost::any(handler)
											)
								);

		return *this;
	}

	template<typename Handler>
	this_type& unreg_ctrl(const code_type& code,
							const code_type& class_code,
							const Handler& handler)
	{
		return this_type::unreg_ctrl(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler>
	this_type& unreg_ctrl(const modules_id_type& mid,
							const code_type& code,
							const code_type& class_code,
							const Handler& handler)
	{
		typedef Handler now_handler_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));

		_ctrl_cont.use_handler(
								boost::bind(
												&this_type::handler_unreg_ctrl_of_sub
												<
													now_handler_type
												>, this, _1,
												boost::cref(mid),
												boost::cref(ctrl_id),
												boost::any(handler)
											)
								);
		return *this;
	}

private:

	template<typename Ctrl, typename Handler>
	void handler_reg_ctrl(ctrl_cont_type::base_type& base,
							const modules_id_type& mid,
							const ctrl_id_type& cid,
							const boost::any& any_handler)
	{
		typedef Ctrl now_ctrl_type;
		typedef Handler handler_type;
		typedef ctrl_cont_type::base_type cont_type;
		typedef boost::multi_index::nth_index_iterator<cont_type, 0>::type iter_type;

		const handler_type* phandler = boost::any_cast<handler_type>(&any_handler);
		if(!phandler)
		{
			return;
		}

		midx_container_id_type cont_id(create_midx_cont_id(mid, cid));

		iter_type iter = base.get<0>().find(cont_id);

		controller_group_ptr_type pgroup;

		if(iter == base.end())
		{
			midx_container mcont(cont_id);
			pgroup = mcont.get_group();

			if(!pgroup)
			{
				return;
			}
			pgroup->append_controller<now_ctrl_type>(*phandler);

			base.insert(mcont);
			return;
		}

		pgroup = (*iter).get_group();
		if(!pgroup)
		{
			return;
		}

		pgroup->append_controller<now_ctrl_type>(*phandler);
		return;
	}

	template<typename Ctrl, typename Handler>
	void handler_reg_dis(ctrl_cont_type::base_type& base, const modules_id_type& mid, const ctrl_id_type& cid,
							const boost::any& any_handler)
	{
		typedef Ctrl now_ctrl_type;
		typedef Handler handler_type;
		typedef ctrl_cont_type::base_type cont_type;
		typedef boost::multi_index::nth_index_iterator<cont_type, 0>::type iter_type;
        
		const handler_type* phandler = boost::any_cast<handler_type>(&any_handler);
		if(!phandler)
		{
			return;
		}

		midx_container_id_type cont_id(create_midx_cont_id(mid, cid));
		iter_type iter = base.get<0>().find(cont_id);

		controller_group_ptr_type pgroup;

		if(iter == base.end())
		{
			midx_container mcont(cont_id);
			pgroup = mcont.get_group();

			if(!pgroup)
			{
				return;
			}
			pgroup->append_dispatcher<now_ctrl_type>(*phandler);

			base.insert(mcont);
			return;
		}

		pgroup = (*iter).get_group();
		if(!pgroup)
		{
			return;
		}

		pgroup->append_dispatcher<now_ctrl_type>(*phandler);
	}

	template<typename Handler>
	void handler_unreg_dis_of_sub(ctrl_cont_type::base_type& base,
										const modules_id_type& mid,
										const ctrl_id_type& cid,
										const boost::any& any_handler )
	{
		typedef Handler handler_type;
		typedef ctrl_cont_type::base_type cont_type;
		typedef boost::multi_index::nth_index_iterator<cont_type, 0>::type iter_type;

		const handler_type* phandler = boost::any_cast<handler_type>(&any_handler);
		if(!phandler)
		{
			return;
		}

		midx_container_id_type cont_id(create_midx_cont_id(mid, cid));

		iter_type iter = base.get<0>().find(cont_id);

		controller_group_ptr_type pgroup;

		if(iter == base.end())
		{
			return;
		}

		pgroup = (*iter).get_group();
		if(!pgroup)
		{
			return;
		}

		pgroup->erase_dispatcher(*phandler);

		if((*iter).empty())
		{
			base.get<0>().erase(iter);
		}
	}

	template<typename Handler>
	void handler_unreg_ctrl_of_sub(ctrl_cont_type::base_type& base,
										const modules_id_type& mid,
										const ctrl_id_type& cid,
										const boost::any& any_handler )
	{
		typedef Handler handler_type;
		typedef ctrl_cont_type::base_type cont_type;
		typedef boost::multi_index::nth_index_iterator<cont_type, 0>::type iter_type;

		const handler_type* phandler = boost::any_cast<handler_type>(&any_handler);
		if(!phandler)
		{
			return;
		}

		midx_container_id_type cont_id(create_midx_cont_id(mid, cid));

		iter_type iter = base.get<0>().find(cont_id);

		controller_group_ptr_type pgroup;

		if(iter == base.end())
		{
			return;
		}

		pgroup = (*iter).get_group();
		if(!pgroup)
		{
			return;
		}

		pgroup->erase_controller(*phandler);

		if((*iter).empty())
		{
			base.get<0>().erase(iter);
		}
	}


	void handler_unreg_dis_of_modules(ctrl_cont_type::base_type& base,
										const modules_id_type& mid);

private:
	//void handler_ctrl_of_void(const ctrl_cont_type::base_type& base, const ctrl_id_type& cid) const;
	//void handler_ctrl_of_params(const ctrl_cont_type::base_type& base, const ctrl_id_type& cid, const param_type& param) const;

	controller_group_ptr_list_type handler_get_controller(const ctrl_cont_type::base_type& base, const ctrl_id_type& cid) const;

private:
	midx_container_id_type create_midx_cont_id(const modules_id_type& mid, const ctrl_id_type& cid) const;
	ctrl_id_type create_ctrl_id(const code_type& code, const code_type& class_code) const;

private:
	ctrl_cont_type _ctrl_cont;
};


} // namespce system_controller
} // namespace yggr

#endif //__YGGR_SYSTEM_CONTROLLER_CTRL_CENTER_HPP__
