//session_checket_creator.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_SESSION_CRYPHER_CREATOR_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_SESSION_CRYPHER_CREATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#define YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_BEGIN(__start_data_type__, __start_data_name__, __crypher_type__) \
	namespace yggr { namespace network { namespace session_helper { \
		template<> \
		struct session_crypher_creator< __start_data_type__ > { \
			typedef __start_data_type__ start_data_type; \
			typedef __crypher_type__ crypher_type; \
			typedef ::yggr::shared_ptr<crypher_type> crypher_ptr_type; \
			typedef crypher_ptr_type result_type; \
			crypher_ptr_type operator()(const start_data_type& __start_data_name__) const { \
				return construct_shared<crypher_type>(yggr_nothrow_new crypher_type(

/*
mid write start_data_init_param_list
__start_data_name__.member1, __start_data_name__.member2, .....
*/


#define YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_END() \
			)); } }; } } }

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename Start_Data>
struct session_crypher_creator;

//template<>
//struct session_crypher_creator<Start_Data>;

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_HELPER_SESSION_CRYPHER_CREATOR_HPP__