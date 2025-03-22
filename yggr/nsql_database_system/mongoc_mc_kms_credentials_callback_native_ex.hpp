//mongoc_mc_kms_credentials_callback_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_MC_KMS_CREDENTIALS_CALLBACK_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_MC_KMS_CREDENTIALS_CALLBACK_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//typedef struct mc_kms_credentials_callback {
//   mongoc_kms_credentials_provider_callback_fn fn;
//   void *userdata;
//} mc_kms_credentials_callback;


struct yggr_mc_kms_credentials_callback 
{
	mongoc_kms_credentials_provider_callback_fn fn;
	void *userdata;
};

BOOST_MPL_ASSERT((
	boost::mpl::bool_<
		(sizeof(yggr_mc_kms_credentials_callback) == sizeof(mongoc_kms_credentials_provider_callback_fn) + sizeof(void*))
	>));


} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_MC_KMS_CREDENTIALS_CALLBACK_NATIVE_EX_HPP__
