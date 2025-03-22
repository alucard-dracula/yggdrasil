//mongoc_client_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/type_traits/number_t.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_t {
//   mongoc_uri_t *uri;
//   mongoc_cluster_t cluster;
//   bool in_exhaust;
//   bool is_pooled;
//
//   mongoc_stream_initiator_t initiator;
//   void *initiator_data;
//
//#ifdef MONGOC_ENABLE_SSL
//   bool use_ssl;
//   mongoc_ssl_opt_t ssl_opts;
//#endif
//
//   mongoc_topology_t *topology;
//
//   mongoc_read_prefs_t *read_prefs;
//   mongoc_read_concern_t *read_concern;
//   mongoc_write_concern_t *write_concern;
//
//   mongoc_apm_callbacks_t apm_callbacks;
//   void *apm_context;
//
//   int32_t error_api_version;
//   bool error_api_set;
//
//   mongoc_server_api_t *api;
//
//   /* mongoc_client_session_t's in use, to look up lsids and clusterTimes */
//   mongoc_set_t *client_sessions;
//   unsigned int csid_rand_seed;
//
//   uint32_t generation;
//};


class mongoc_client_native_ex
{
private:
	typedef mongoc_client_native_ex this_type;

public:
	static mongoc_read_prefs_t* s_set_read_prefs_pointer(mongoc_client_t* pclt, mongoc_read_prefs_t* pread_prefs);
	static mongoc_read_concern_t* s_set_read_concern_pointer(mongoc_client_t* pclt, mongoc_read_concern_t* pread_concern);
	static mongoc_write_concern_t* s_set_write_concern_pointer(mongoc_client_t* pclt, mongoc_write_concern_t* pwrite_concern);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_NATIVE_EX_HPP__