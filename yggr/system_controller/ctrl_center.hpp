//ctrl_center.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_CTRL_CENTER_HPP__
#define __YGGR_SYSTEM_CONTROLLER_CTRL_CENTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/obj_creator.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/move/move.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/tuple_ex/tuple.hpp>

#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>

#include <yggr/system_controller/detail/ctrl_center_controller_group.hpp>

#include <boost/ref.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>

#include <algorithm>
#include <sstream>
#include <cassert>

#ifdef _DEBUG

#	define CTRL_HANDLER( __tag__, __ctrl_obj__, __code__, __class_code__, __handler__ ) \
		assert((::yggr::obj_creator< __tag__ >::create())( __ctrl_obj__, __code__, __class_code__, __handler__ ))

#else

#	define CTRL_HANDLER( __tag__, __ctrl_obj__, __code__, __class_code__, __handler__ ) \
		((::yggr::obj_creator< __tag__ >::create())( __ctrl_obj__, __code__, __class_code__, __handler__ ))

#endif // _DEBUG

namespace yggr
{
namespace system_controller
{
namespace detail
{

struct ctrl_center_midx_container
{
public:
	struct tag_id {};
	struct tag_mid {};
	struct tag_cid {};

public:
	typedef controller_group controller_group_type;
	typedef ::yggr::shared_ptr<controller_group_type> controller_group_ptr_type;

	typedef ctrl_center_types_def::ctrl_id_type ctrl_id_type;
	typedef ctrl_center_types_def::modules_id_type modules_id_type;

	typedef ::yggr::tuple<modules_id_type, ctrl_id_type> midx_container_id_type;

private:
	typedef ctrl_center_midx_container this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	ctrl_center_midx_container(void);
	ctrl_center_midx_container(const ctrl_id_type& ncid);
	ctrl_center_midx_container(const modules_id_type& nmid,
								const ctrl_id_type& ncid);
	ctrl_center_midx_container(const midx_container_id_type& nid);

	ctrl_center_midx_container(BOOST_RV_REF(this_type) right)
		: id(boost::move(right.id)),
			pgroup(boost::move(right.pgroup))
	{
	}

	ctrl_center_midx_container(const this_type& right);
	~ctrl_center_midx_container(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		id = boost::move(right.id);
		pgroup = boost::move(right.pgroup);
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline bool empty(void) const
	{
		assert(pgroup);
		return !pgroup || pgroup->empty();
	}

	inline const modules_id_type& modules_id(void) const
	{
		return yggr::get<0>(id);
	}

	inline const ctrl_id_type& ctrl_id(void) const
	{
		return yggr::get<1>(id);
	}

	inline controller_group_ptr_type get_group(void) const
	{
		return pgroup;
	}

public:
	midx_container_id_type id;
	controller_group_ptr_type pgroup;
};

// tags

struct ctrl_center_tag_reg_ctrl
{
	typedef ctrl_center_types_def::code_type code_type;
	typedef ctrl_center_types_def::modules_id_type modules_id_type;
	typedef yggr::ptr_single<modules_id_type> modules_id_single_type;
	typedef modules_id_single_type::obj_ptr_type modules_id_ptr_type;

	template<typename CtrlCenter, typename Handler> inline
	bool operator()(CtrlCenter& cc,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler) const
	{
		modules_id_ptr_type ptr = modules_id_single_type::get_ins();
		return
			ptr?
				cc.reg_ctrl(*ptr, code, class_code, handler)
				: cc.reg_ctrl(code, class_code, handler);
	}
};

struct ctrl_center_tag_reg_dis
{
	typedef ctrl_center_types_def::code_type code_type;
	typedef ctrl_center_types_def::modules_id_type modules_id_type;
	typedef yggr::ptr_single<modules_id_type> modules_id_single_type;
	typedef modules_id_single_type::obj_ptr_type modules_id_ptr_type;

	template<typename CtrlCenter, typename Handler> inline
	bool operator()(CtrlCenter& cc,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler) const
	{
		modules_id_ptr_type ptr = modules_id_single_type::get_ins();
		return
			ptr?
				cc.reg_dis(*ptr, code, class_code, handler)
				: cc.reg_dis(code, class_code, handler);
	}
};

struct ctrl_center_tag_unreg_ctrl
{
	typedef ctrl_center_types_def::code_type code_type;
	typedef ctrl_center_types_def::modules_id_type modules_id_type;
	typedef yggr::ptr_single<modules_id_type> modules_id_single_type;
	typedef modules_id_single_type::obj_ptr_type modules_id_ptr_type;

	template<typename CtrlCenter, typename Handler> inline
	bool operator()(CtrlCenter& cc,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler) const
	{
		modules_id_ptr_type ptr = modules_id_single_type::get_ins();
		return
			ptr?
				cc.unreg_ctrl(*ptr, code, class_code, handler)
				: cc.unreg_ctrl(code, class_code, handler);
	}
};

struct ctrl_center_tag_unreg_dis
{
	typedef ctrl_center_types_def::code_type code_type;
	typedef ctrl_center_types_def::modules_id_type modules_id_type;
	typedef yggr::ptr_single<modules_id_type> modules_id_single_type;
	typedef modules_id_single_type::obj_ptr_type modules_id_ptr_type;

	template<typename CtrlCenter, typename Handler> inline
	bool operator()(CtrlCenter& cc,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler) const
	{

		modules_id_ptr_type ptr = modules_id_single_type::get_ins();
		return
			ptr?
				cc.unreg_dis(*ptr, code, class_code, handler)
				: cc.unreg_dis(code, class_code, handler);
	}
};

struct ctrl_center_tag_ctrl {};
struct ctrl_center_tag_dis {};

template<typename Tag>
struct ctrl_center_reg_helper;

template<>
struct ctrl_center_reg_helper<ctrl_center_tag_ctrl>
{
	typedef controller_group controller_group_type;
	typedef controller_group_type::handler_wrap_type handler_wrap_type;
	typedef controller_group_type::handler_id_type handler_id_type;
	typedef ::yggr::shared_ptr<controller_group_type> controller_group_ptr_type;

	template<typename Ctrl> inline
	bool append(controller_group_ptr_type pgroup, const handler_wrap_type& handler_wrap) const
	{
		assert(pgroup);
		return pgroup->append_controller<Ctrl>(handler_wrap);
	}

	inline bool erase(controller_group_ptr_type pgroup,
						handler_id_type id,
						const handler_wrap_type& handler_wrap) const
	{
		assert(pgroup);
		return pgroup->erase_controller(id, handler_wrap);
	}
};

template<>
struct ctrl_center_reg_helper<ctrl_center_tag_dis>
{
	typedef controller_group controller_group_type;
	typedef controller_group_type::handler_wrap_type handler_wrap_type;
	typedef controller_group_type::handler_id_type handler_id_type;
	typedef ::yggr::shared_ptr<controller_group_type> controller_group_ptr_type;

	template<typename Dis> inline
	bool append(controller_group_ptr_type pgroup, const handler_wrap_type& handler_wrap) const
	{
		assert(pgroup);
		return pgroup->append_dispatcher<Dis>(handler_wrap);
	}

	inline bool erase(controller_group_ptr_type pgroup,
						handler_id_type id,
						const handler_wrap_type& handler_wrap) const
	{
		assert(pgroup);
		return pgroup->erase_dispatcher(id, handler_wrap);
	}
};

} // namespace detail
} // namespace system_controller
} // namespace yggr

namespace yggr
{
namespace system_controller
{

class ctrl_center
{
public:
	typedef detail::ctrl_center_types_def::code_type code_type;
	typedef detail::ctrl_center_types_def::ctrl_id_type ctrl_id_type;
	typedef detail::ctrl_center_types_def::modules_id_type modules_id_type;

private:
	typedef detail::controller_group controller_group_type;

public:
	typedef controller_group_type::handler_id_type handler_id_type;
	typedef controller_group_type::param_type param_type;
	typedef controller_group_type::handler_wrap_type handler_wrap_type;

private:
	typedef detail::base_ctrl_ptr_type base_ctrl_ptr_type;

	typedef ::yggr::shared_ptr<controller_group_type> controller_group_ptr_type;
	typedef ::yggr::vector<controller_group_ptr_type> controller_group_ptr_vt_type;

	typedef ::yggr::tuple<modules_id_type, ctrl_id_type> midx_container_id_type;
	typedef detail::ctrl_center_midx_container midx_container_type;

	typedef
		yggr::safe_container::safe_multi_index_container
		<
			midx_container_type,
			boost::multi_index::indexed_by
			<
				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<midx_container_type::tag_id>,
					BOOST_MULTI_INDEX_MEMBER(midx_container_type, midx_container_id_type, id)
				>,

				boost::multi_index::hashed_non_unique
				<
					boost::multi_index::tag<midx_container_type::tag_mid>,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(midx_container_type, const modules_id_type&, modules_id)
				>,

				boost::multi_index::hashed_non_unique
				<
					boost::multi_index::tag<midx_container_type::tag_cid>,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(midx_container_type, const ctrl_id_type&, ctrl_id)
				>
			>
		> ctrl_cont_type;

private:
	typedef yggr::ptr_single<modules_id_type> modules_id_single_type;
	typedef modules_id_single_type::obj_ptr_type modules_id_ptr_type;

private:
	typedef ctrl_center this_type;

public:
	typedef detail::ctrl_center_tag_reg_ctrl tag_reg_ctrl;
	typedef detail::ctrl_center_tag_reg_dis tag_reg_dis;
	typedef detail::ctrl_center_tag_unreg_ctrl tag_unreg_ctrl;
	typedef detail::ctrl_center_tag_unreg_dis tag_unreg_dis;

private:
	typedef detail::ctrl_center_tag_ctrl tag_ctrl;
	typedef detail::ctrl_center_tag_dis tag_dis;

public:
	ctrl_center(void);
	~ctrl_center(void);

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	void ctrl(const code_type& code, const code_type& class_code,
				const Args&... args) const
	{
		typedef controller_group_ptr_vt_type::const_iterator citer_type;

		ctrl_id_type id(create_ctrl_id(code, class_code));

		std::size_t size = _ctrl_cont.get<2>().count(id);
		if(!size)
		{
			return;
		}

		controller_group_ptr_vt_type vt;
		vt.reserve((size + (size >> 1)));

		_ctrl_cont.get<2>().equal_range(
			id,
			boost::bind(
				&this_type::handler_get_controller,
				this, _1, _2, boost::ref(vt)));

		if(!vt.empty())
		{
			{
				for(citer_type i = boost::begin(vt), isize = boost::end(vt); i != isize; ++i)
				{
					if(*i)
					{
						(*i)->ctrl(args...);
					}
				}
			}

			{
				for(citer_type i = boost::begin(vt), isize = boost::end(vt); i != isize; ++i)
				{
					if(*i)
					{
						(*i)->dispatch(args...);
					}
				}
			}
		}
	}
#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		void ctrl(const code_type& code, const code_type& class_code \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) const { \
			typedef controller_group_ptr_vt_type::const_iterator citer_type; \
			ctrl_id_type id(create_ctrl_id(code, class_code)); \
			std::size_t size = _ctrl_cont.get<2>().count(id); \
			if(!size) { return; } \
			controller_group_ptr_vt_type vt; \
			vt.reserve((size + (size >> 1))); \
			_ctrl_cont.get<2>().equal_range( \
				id, \
				boost::bind( \
					&this_type::handler_get_controller, \
					this, _1, _2, boost::ref(vt))); \
			if(!vt.empty()) { \
				{ for(citer_type i = boost::begin(vt), isize = boost::end(vt); i != isize; ++i) { \
					if(*i) { (*i)->ctrl(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } } } \
				{ for(citer_type i = boost::begin(vt), isize = boost::end(vt); i != isize; ++i) { \
					if(*i) { (*i)->dispatch(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } } } } }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline void clear(void)
	{
		_ctrl_cont.clear();
	}

	template<typename Sys_Obj> inline
	this_type& register_controller(Sys_Obj& obj)
	{
		obj.template register_controller<tag_reg_ctrl, this_type>(*this);
		return *this;
	}

	template<typename Dis_Obj> inline
	this_type& register_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<tag_reg_dis, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Sys_Obj> inline
	this_type& register_controller(Sys_Obj& obj)
	{
		obj.template register_controller<Tag, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Dis_Obj> inline
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

	template<typename Dis_Obj> inline
	this_type& unregister_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<tag_unreg_dis, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Sys_Obj> inline
	this_type& unregister_controller(Sys_Obj& obj)
	{
		obj.template register_controller<Tag, this_type>(*this);
		return *this;
	}

	template<typename Tag, typename Dis_Obj> inline
	this_type& unregister_dispatchers(Dis_Obj& obj)
	{
		obj.template register_dispatchers<Tag, this_type>(*this);
		return *this;
	}

	inline this_type& unregister_module(void)
	{
		modules_id_ptr_type ptr = modules_id_single_type::get_ins();
		this_type::unreg_dis(ptr? *ptr : modules_id_type());
		return *this;
	}

	// reg
	template<typename Handler> inline
	bool reg_dis(const code_type& code, const code_type& class_code, const Handler& handler)
	{
		return this_type::reg_dis(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler> inline
	bool reg_dis(const modules_id_type& mid,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef detail::controller<now_handler_type> now_ctrl_type;
		typedef detail::ctrl_center_reg_helper<tag_dis> now_reg_helper_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));
		midx_container_id_type cont_id(create_midx_cont_id(mid, ctrl_id));

		midx_container_type mcont(cont_id);
		controller_group_ptr_type pgroup = mcont.get_group();
		if(!pgroup)
		{
			return false;
		}

		handler_wrap_type handler_wrap(handler);
		now_reg_helper_type helper;
		helper.append<now_ctrl_type>(pgroup, handler_wrap);

		return
			_ctrl_cont.get<0>().insert(
				mcont,
				boost::bind(
					&this_type::handler_reg< now_ctrl_type, now_reg_helper_type >,
					this, _1, _2, boost::cref(handler_wrap) ));
	}



	template<typename Handler> inline
	bool reg_ctrl(const code_type& code,
						const code_type& class_code,
						const Handler& handler)
	{
		return this_type::reg_ctrl(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler> inline
	bool reg_ctrl(const modules_id_type& mid,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef detail::controller<now_handler_type> now_ctrl_type;
		typedef detail::ctrl_center_reg_helper<tag_ctrl> now_reg_helper_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));
		midx_container_id_type cont_id(create_midx_cont_id(mid, ctrl_id));

		midx_container_type mcont(cont_id);
		controller_group_ptr_type pgroup = mcont.get_group();
		if(!pgroup)
		{
			return false;
		}

		handler_wrap_type handler_wrap(handler);
		now_reg_helper_type helper;
		helper.append<now_ctrl_type>(pgroup, handler_wrap);

		return
			_ctrl_cont.get<0>().insert(
				mcont,
				boost::bind(
					&this_type::handler_reg <now_ctrl_type, now_reg_helper_type >,
					this, _1, _2, boost::cref(handler_wrap) ));
	}

	// unreg

	inline bool unreg_dis(const modules_id_type& mid)
	{
		return 0 != _ctrl_cont.get<1>().erase(mid);
	}

	template<typename Handler> inline
	bool unreg_dis(const code_type& code,
					const code_type& class_code,
					const Handler& handler)
	{
		return this_type::unreg_dis(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler> inline
	bool unreg_dis(const modules_id_type& mid,
					const code_type& code,
					const code_type& class_code,
					const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef detail::ctrl_center_reg_helper<tag_dis> now_reg_helper_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));
		midx_container_id_type cont_id(create_midx_cont_id(mid, ctrl_id));

		handler_wrap_type handler_wrap(handler);
		return
			_ctrl_cont.get<0>().find(
				cont_id,
				boost::bind(
					&this_type::handler_unreg_of_sub<now_reg_helper_type>,
					this, _1, _2,
					boost::addressof(typeid(now_handler_type)),
					boost::cref(handler_wrap) ) );
	}

	template<typename Handler> inline
	bool unreg_ctrl(const code_type& code,
							const code_type& class_code,
							const Handler& handler)
	{
		return this_type::unreg_ctrl(modules_id_type(), code, class_code, handler);
	}

	template<typename Handler> inline
	bool unreg_ctrl(const modules_id_type& mid,
							const code_type& code,
							const code_type& class_code,
							const Handler& handler)
	{
		typedef Handler now_handler_type;
		typedef detail::ctrl_center_reg_helper<tag_ctrl> now_reg_helper_type;

		ctrl_id_type ctrl_id(create_ctrl_id(code, class_code));
		midx_container_id_type cont_id(create_midx_cont_id(mid, ctrl_id));

		handler_wrap_type handler_wrap(handler);
		return
			_ctrl_cont.get<0>().find(
				cont_id,
				boost::bind(
					&this_type::handler_unreg_of_sub<now_reg_helper_type >,
					this, _1, _2,
					boost::addressof(typeid(now_handler_type)),
					boost::cref(handler_wrap) ) );
	}


private:

	template<typename Ctrl, typename RegHelper> inline
	bool handler_reg(ctrl_cont_type::base_type& base,
						boost::fusion::pair
						<
							typename
								safe_container::nth_index
								<
									ctrl_cont_type,
									0
								>::type::index_id_type,

							std::pair
							<
								typename
									safe_container::nth_index
									<
										ctrl_cont_type,
										0
									>::type::iterator,
								bool
							>
						> rst,
						const handler_wrap_type& handler_wrap)
	{
		typedef Ctrl now_ctrl_type;
		typedef RegHelper now_reg_helper_type;

		if(handler_wrap.empty())
		{
			return false;
		}

		if(!rst.second.second)
		{
			if(rst.second.first == base.get<0>().end())
			{
				return false;
			}
			else
			{
				controller_group_ptr_type pgroup = (*rst.second.first).get_group();
				if(!pgroup)
				{
					return false;
				}
				now_reg_helper_type helper;
				helper.template append<now_ctrl_type>(pgroup, handler_wrap);
			}
		}

		return true;
	}

	template<typename RegHelper> inline
	bool handler_unreg_of_sub(ctrl_cont_type::base_type& base,
								boost::fusion::pair
								<
									typename
										safe_container::nth_index
										<
											ctrl_cont_type,
											0
										>::type::index_id_type,

									typename
										safe_container::nth_index
										<
											ctrl_cont_type,
											0
										>::type::iterator
								> rst,
								handler_id_type dispatch_id,
								const handler_wrap_type& any_handler )
	{
		typedef RegHelper now_reg_helper_type;

		if(any_handler.empty())
		{
			return false;
		}


		if(rst.second == base.get<0>().end())
		{
			return false;
		}
		else
		{
			controller_group_ptr_type pgroup = (*rst.second).get_group();
			if(!pgroup)
			{
				return false;
			}
			now_reg_helper_type helper;
			helper.erase(pgroup, dispatch_id, any_handler);

			if((*rst.second).empty())
			{
				base.get<0>().erase(rst.second);
			}
		}


		return true;
	}


private:
	void handler_get_controller(const ctrl_cont_type::base_type& base,
									boost::fusion::pair
									<
										safe_container::nth_index<ctrl_cont_type, 2>::type::index_id_type,
										std::pair
										<
											safe_container::nth_index<ctrl_cont_type, 2>::type::const_iterator,
											safe_container::nth_index<ctrl_cont_type, 2>::type::const_iterator
										>
									> rst,
									controller_group_ptr_vt_type& vt) const;

private:
	inline static midx_container_id_type
		create_midx_cont_id(const modules_id_type& mid, const ctrl_id_type& cid)
	{
		return yggr::make_tuple(mid, cid);
	}

	inline static ctrl_id_type create_ctrl_id(const code_type& code, const code_type& class_code)
	{
		ctrl_id_type code_id(code);
		ctrl_id_type class_code_id(class_code);

		return ((code_id << 32) | class_code_id);
	}

private:
	ctrl_cont_type _ctrl_cont;
};


} // namespce system_controller
} // namespace yggr

#endif //__YGGR_SYSTEM_CONTROLLER_CTRL_CENTER_HPP__
