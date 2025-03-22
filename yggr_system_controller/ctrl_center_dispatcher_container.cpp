// ctrl_center_dispatcher_container.cpp

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

#include <yggr/system_controller/detail/ctrl_center_dispatcher_container.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace system_controller
{
namespace detail
{


dispatcher_container::dispatcher_container(void)
{
}

dispatcher_container::~dispatcher_container(void)
{
	_cont.clear();
}

/*static*/
bool dispatcher_container::handler_erase_dispatcher(controller_container_type::base_type& base, 
													controller_container_type::iterator iter, 
													const handler_wrap_type& handler_wrap)
{
	if(iter == base.end())
	{
		return true;
	}

	if(!iter->second)
	{
		base.erase(iter);
		return true;
	}

	iter->second->erase(handler_wrap);

	if(iter->second->empty())
	{
		base.erase(iter);
	}

	return true;
}

/*static*/
dispatcher_container::ctrl_cont_type&
	dispatcher_container::handler_get_dispatchers(const controller_container_type::base_type& base, 
													ctrl_cont_type& cont)
{
	assert(cont.empty());
	for(controller_container_citer_type i = base.begin(), isize = base.end();
		 i != isize; ++i)
	{
		cont.push_back(i->second);
	}

	return cont;
}

} // namespace detail
} // namespace system_contrller
} // namespace yggr

