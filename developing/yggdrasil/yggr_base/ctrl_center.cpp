//ctrl_center.cpp

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

#include <utility>
#include <yggr/base/ctrl_center.hpp>

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace system_controller
{

//--------------------dispatcher_container------------------------------------

ctrl_center::dispatcher_container::dispatcher_container(void)
{
}

ctrl_center::dispatcher_container::~dispatcher_container(void)
{
	_cont.clear();
}

bool ctrl_center::dispatcher_container::empty(void) const
{
	return _cont.empty();
}

void ctrl_center::dispatcher_container::clear(void)
{
	_cont.clear();
}

void ctrl_center::dispatcher_container::dispatcher(void) const
{
	param_type param;
	dispatcher_container::dispatcher(param);
}

void ctrl_center::dispatcher_container::dispatcher(const param_type& param) const
{
	ctrl_cont_type cont;
	_cont.use_handler(boost::bind(&dispatcher_container::handler_get_dispatchers,
									this, _1, boost::ref(cont)));

	for(ctrl_cont_citer_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
	{
		if(*i)
		{
			(*i)->ctrl(param);
		}
	}
}

void ctrl_center::dispatcher_container::handler_get_dispatchers(const controller_container_type::base_type& base,
																	ctrl_cont_type& cont) const
{
    {
        ctrl_cont_type tmp_cont;
        cont.swap(tmp_cont);
    }
	for(controller_container_citer_type i = base.begin(), isize = base.end();
		 i != isize; ++i)
	{
		if(i->second)
		{
			cont.push_back(i->second);
		}
	}
}

//--------------------------controller_group-------------------------------------------

ctrl_center::controller_group::controller_group(void)
{
}

ctrl_center::controller_group::~controller_group(void)
{
}

void ctrl_center::controller_group::ctrl(void) const
{
	_ctrl_cont.dispatcher();
}

void ctrl_center::controller_group::ctrl(const param_type& param) const
{
	_ctrl_cont.dispatcher(param);
}

void ctrl_center::controller_group::dispatch(void) const
{
	_dis_cont.dispatcher();
}

void ctrl_center::controller_group::dispatch(const param_type& param) const
{
	_dis_cont.dispatcher(param);
}

bool ctrl_center::controller_group::empty(void) const
{
	return _ctrl_cont.empty() && _dis_cont.empty();
}

void ctrl_center::controller_group::clear(void)
{
	_ctrl_cont.clear();
	_dis_cont.clear();
}

void ctrl_center::controller_group::clear_controller(void)
{
	_ctrl_cont.clear();
}

void ctrl_center::controller_group::clear_dispatcher(void)
{
	_dis_cont.clear();
}

//--------------------------midx_container----------------------------------------

ctrl_center::midx_container::midx_container(void)
	: id(boost::make_tuple(modules_id_type(), ctrl_id_type())),
		pgroup(new controller_group())
{
	assert(pgroup);
}

ctrl_center::midx_container::midx_container(const ctrl_id_type& ncid)
	: id(boost::make_tuple(modules_id_type(), ncid)),
		pgroup(new controller_group())
{
	assert(pgroup);
}

ctrl_center::midx_container::midx_container(const modules_id_type& nmid,
				const ctrl_id_type& ncid)
	: id(boost::make_tuple(nmid, ncid)),
		pgroup(new controller_group())
{
	assert(pgroup);
}

ctrl_center::midx_container::midx_container(const midx_container_id_type& nid)
	: id(nid),
		pgroup(new controller_group())
{
}

ctrl_center::midx_container::~midx_container(void)
{
}

bool ctrl_center::midx_container::empty(void) const
{
	assert(pgroup);
	return !pgroup || pgroup->empty();
}

const ctrl_center::modules_id_type& ctrl_center::midx_container::modules_id(void) const
{
	return boost::get<0>(id);
}

const ctrl_center::ctrl_id_type& ctrl_center::midx_container::ctrl_id(void) const
{
	return boost::get<1>(id);
}

ctrl_center::controller_group_ptr_type ctrl_center::midx_container::get_group(void) const
{
	return pgroup;
}

//--------------------------ctrl_center-------------------------------------------
ctrl_center::ctrl_center(void)
{
}

ctrl_center::~ctrl_center(void)
{
	_ctrl_cont.clear();
}

ctrl_center& ctrl_center::unregister_module(void)
{
	modules_id_ptr_type ptr = modules_id_single_type::get_ins();
	unreg_dis(ptr?*ptr : modules_id_type());
	return *this;
}

void ctrl_center::ctrl(const code_type& code, const code_type& class_code) const
{
	ctrl_id_type id(create_ctrl_id(code, class_code));

	_ctrl_cont.use_handler(
						boost::bind(
										&this_type::handler_ctrl_of_void,
										this,
										_1,
										boost::cref(id)
									)
							);

}

void ctrl_center::ctrl(const code_type& code, const code_type& class_code, const param_type& param) const
{
	ctrl_id_type id(create_ctrl_id(code, class_code));

	_ctrl_cont.use_handler(
							boost::bind(
											&this_type::handler_ctrl_of_params,
											this,
											_1,
											boost::cref(id),
											boost::cref(param)
										)
							);
}

void ctrl_center::handler_ctrl_of_void(const ctrl_cont_type::base_type& base,
											const ctrl_id_type& cid) const
{
	typedef ctrl_cont_type::base_type cont_type;
	typedef boost::multi_index::nth_index_const_iterator<cont_type, 2>::type iter_type;
	typedef std::pair<iter_type, iter_type> rst_type;

	rst_type rst = base.get<2>().equal_range(cid);

	controller_group_ptr_type ptr;
	for(iter_type i = rst.first, isize = rst.second; i != isize; ++i)
	{
		ptr = (*i).get_group();
		if(ptr)
		{
			ptr->ctrl();
		}
	}

	for(iter_type i = rst.first, isize = rst.second; i != isize; ++i)
	{
		ptr = (*i).get_group();
		if(ptr)
		{
			ptr->dispatch();
		}
	}
}

void ctrl_center::handler_ctrl_of_params(const ctrl_cont_type::base_type& base,
											const ctrl_id_type& cid,
											const param_type& param) const
{
	typedef ctrl_cont_type::base_type cont_type;
	typedef boost::multi_index::nth_index_const_iterator<cont_type, 2>::type iter_type;
	typedef std::pair<iter_type, iter_type> rst_type;

	rst_type rst = base.get<2>().equal_range(cid);

	controller_group_ptr_type ptr;
	for(iter_type i = rst.first, isize = rst.second; i != isize; ++i)
	{
		ptr = (*i).get_group();
		if(ptr)
		{
			ptr->ctrl(param);
		}
	}

	for(iter_type i = rst.first, isize = rst.second; i != isize; ++i)
	{
		ptr = (*i).get_group();
		if(ptr)
		{
			ptr->dispatch(param);
		}
	}
}

void ctrl_center::clear(void)
{
	_ctrl_cont.clear();
}

ctrl_center& ctrl_center::unreg_dis(const modules_id_type& mid)
{
	_ctrl_cont.use_handler(
					boost::bind(
									&this_type::handler_unreg_dis_of_modules,
									this, _1, boost::cref(mid)
								)
							);
	return *this;
}

void ctrl_center::handler_unreg_dis_of_modules(ctrl_cont_type::base_type& base,
													const modules_id_type& mid)
{
	base.get<1>().erase(mid);
}

ctrl_center::ctrl_id_type
		ctrl_center::create_ctrl_id(const code_type& code, const code_type& class_code) const
{
	ctrl_id_type id(0);

	id |= code;
	id <<= 32;
	id |= class_code;

	return id;
}

ctrl_center::midx_container_id_type
		ctrl_center::create_midx_cont_id(const modules_id_type& mid, const ctrl_id_type& cid) const
{
	return boost::make_tuple(mid, cid);
}

} // namespce system_controller
} // namespace yggr

