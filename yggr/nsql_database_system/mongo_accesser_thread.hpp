// mongo_accesser_thread.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_THRAD_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_THRAD_HPP__

#include <yggr/nsql_database_system/basic_mongo_accesser_thread.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser_thread
	: public basic_mongo_accesser_thread
{
public:
	typedef basic_mongo_accesser_thread base_type;

private:
	typedef mongo_accesser_thread this_type;

public:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// don't coding get_param or set_params function, because that is call time used only, 
	// example get_read_concern(...)
	// if you want to use that, please call foo "using_handler" execute you custom native operations
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//------------------------thread s----------------------------

public:
	using base_type::s_thread_clear;

	//------------------------thread e----------------------------
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_THRAD_HPP__
