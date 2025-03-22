//ctrl_center.cpp

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

#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <utility>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace system_controller
{
namespace detail
{

//--------------------------midx_container----------------------------------------

ctrl_center_midx_container::ctrl_center_midx_container(void)
	: id(yggr::make_tuple(modules_id_type(), ctrl_id_type())),
		pgroup(::yggr::make_shared<controller_group_type>())
{
	assert(pgroup);
}

ctrl_center_midx_container::ctrl_center_midx_container(const ctrl_id_type& ncid)
	: id(yggr::make_tuple(modules_id_type(), ncid)),
		pgroup(::yggr::make_shared<controller_group_type>())
{
	assert(pgroup);
}

ctrl_center_midx_container::ctrl_center_midx_container(const modules_id_type& nmid,
				const ctrl_id_type& ncid)
	: id(yggr::make_tuple(nmid, ncid)),
		pgroup(::yggr::make_shared<controller_group_type>())
{
	assert(pgroup);
}

ctrl_center_midx_container::ctrl_center_midx_container(const midx_container_id_type& nid)
	: id(nid),
		pgroup(::yggr::make_shared<controller_group_type>())
{
}

ctrl_center_midx_container::ctrl_center_midx_container(const this_type& right)
	: id(right.id), pgroup(right.pgroup)
{
}

ctrl_center_midx_container::~ctrl_center_midx_container(void)
{
}

ctrl_center_midx_container::this_type& 
	ctrl_center_midx_container::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	id = right.id;
	pgroup = right.pgroup;
	return *this;
}

void ctrl_center_midx_container::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	::yggr::swap(id, right.id);
	::yggr::swap(pgroup, right.pgroup);
}

} // namespace detail
} // namespace system_controller
} // namespace yggr

namespace yggr
{
namespace system_controller
{

//--------------------------ctrl_center-------------------------------------------
ctrl_center::ctrl_center(void)
{
}

ctrl_center::~ctrl_center(void)
{
	_ctrl_cont.clear();
}

void ctrl_center::handler_get_controller(const ctrl_cont_type::base_type& base,
											boost::fusion::pair
											<
												safe_container::nth_index<ctrl_cont_type, 2>::type::index_id_type,
												std::pair
												<
													safe_container::nth_index<ctrl_cont_type, 2>::type::const_iterator,
													safe_container::nth_index<ctrl_cont_type, 2>::type::const_iterator
												>
											> rst,
											controller_group_ptr_vt_type& vt) const
{
	typedef ctrl_cont_type::base_type cont_type;

	for(; rst.second.first != rst.second.second; ++rst.second.first)
	{
		vt.push_back((*rst.second.first).get_group());
	}
}


} // namespce system_controller
} // namespace yggr


