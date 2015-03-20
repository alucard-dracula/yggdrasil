//module.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/modules/module.hpp>

namespace yggr
{
namespace modules
{

module::module(void)
    : _handle(0)
{
}

module::module(const std::string& module_name, int mode)
    : _name(module_name), _handle(modules_accesser::load_module(module_name, mode))
{
}

module::module(const this_type& right)
    : _name(right._name), _handle(right._handle)
{
}

module::~module(void)
{
    modules_accesser::free_module(_handle);
}

bool module::load(const std::string& module_name, int mode)
{
	_name = module_name;
	_handle = modules_accesser::load_module(module_name, mode);

	return (bool)_handle;
}

void module::close(void)
{
	modules_accesser::free_module(_handle);
}

module::this_type& module::operator=(const this_type& right)
{
	if(this == &right) {return *this;}
    _name = right._name;
    _handle = right._handle;

    return *this;
}

void* module::get_entry(const std::string& name) const
{
	 return modules_accesser::get_entry(_handle, name);
}

bool module::operator==(const std::string& name) const
{
    return name == _name;
}

bool module::operator==(const module& right) const
{
    return _name == right._name;
}

bool module::empty(void) const
{
    return !_handle;
}

} // namespace modules
} // namespace yggr

#define _YGGR_MODULES_MODULE_SWAP_IMPL() \
	void swap(yggr::modules::module& l, yggr::modules::module& r) { \
		l.swap(r); }

namespace std
{
	_YGGR_MODULES_MODULE_SWAP_IMPL()
} // namespace std

namespace boost
{
	_YGGR_MODULES_MODULE_SWAP_IMPL()
} // namespace boost

#undef _YGGR_MODULES_MODULE_SWAP_IMPL