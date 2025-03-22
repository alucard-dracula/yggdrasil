// c_mongo_apm_callbacks.cpp

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

#include <yggr/nsql_database_system/c_mongo_apm_callbacks.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_apm_callbacks::c_mongo_apm_callbacks(void)
{
	this_type::pro_init();
}

c_mongo_apm_callbacks::c_mongo_apm_callbacks(mongoc_apm_command_started_cb_type pcb_cmd_started,
												mongoc_apm_command_succeeded_cb_type pcb_cmd_succeeded,
												mongoc_apm_command_failed_cb_type pcb_cmd_failed,
												mongoc_apm_server_changed_cb_type pcb_svr_changed,
												mongoc_apm_server_opening_cb_type pcb_svr_opening,
												mongoc_apm_server_closed_cb_type pcb_svr_closed,
												mongoc_apm_topology_changed_cb_type pcb_tpy_changed,
												mongoc_apm_topology_opening_cb_type pcb_tpy_opening,
												mongoc_apm_topology_closed_cb_type pcb_tpy_closed,
												mongoc_apm_server_heartbeat_started_cb_type pcb_shb_started,
												mongoc_apm_server_heartbeat_succeeded_cb_type pcb_shb_succeeded,
												mongoc_apm_server_heartbeat_failed_cb_type pcb_shb_failed)
{
	this_type::pro_init();

	base_type::started = pcb_cmd_started;
	base_type::succeeded = pcb_cmd_succeeded;
	base_type::failed = pcb_cmd_failed;

	base_type::server_changed = pcb_svr_changed;
	base_type::server_opening = pcb_svr_opening;
	base_type::server_closed = pcb_svr_closed;

	base_type::topology_changed = pcb_tpy_changed;
	base_type::topology_opening = pcb_tpy_opening;
	base_type::topology_closed = pcb_tpy_closed;

	base_type::server_heartbeat_started = pcb_shb_started;
	base_type::server_heartbeat_succeeded = pcb_shb_succeeded;
	base_type::server_heartbeat_failed = pcb_shb_failed;
}

c_mongo_apm_callbacks::c_mongo_apm_callbacks(const org_type* pcallbacks)
{
	if(pcallbacks)
	{
		this_type::pro_copy(*(reinterpret_cast<const base_type*>(pcallbacks)));
	}
	else
	{
		this_type::pro_init();
	}
}

c_mongo_apm_callbacks::c_mongo_apm_callbacks(const org_type& callbacks)
{
	this_type::pro_copy(reinterpret_cast<const base_type&>(callbacks));
}

c_mongo_apm_callbacks::c_mongo_apm_callbacks(const this_type& right)
{
	this_type::pro_copy(right);
}

c_mongo_apm_callbacks::~c_mongo_apm_callbacks(void)
{
}

//public:
c_mongo_apm_callbacks::this_type& 
	c_mongo_apm_callbacks::operator=(const org_type& right)
{
	this_type::pro_copy(reinterpret_cast<const base_type&>(right));
	return *this;
}

c_mongo_apm_callbacks::this_type& 
	c_mongo_apm_callbacks::operator=(const this_type& right)
{
	this_type::pro_copy(right);
	return *this;
}

//public:
void c_mongo_apm_callbacks::swap(org_type& right)
{
	this_type::pro_swap(reinterpret_cast<base_type&>(right));
}

void c_mongo_apm_callbacks::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_apm_callbacks::pro_init(void)
{
	base_type& base = *this;
	memset(boost::addressof(base), 0, sizeof(base_type));
}

void c_mongo_apm_callbacks::pro_copy(const base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return;
	}
	base_type& base = *this;
	memcpy(boost::addressof(base), boost::addressof(right), sizeof(base_type));
}

void c_mongo_apm_callbacks::pro_copy(const this_type& right)
{
	this_type::pro_copy(static_cast<const base_type&>(right));
}

void c_mongo_apm_callbacks::pro_swap(base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return;
	}

	base_type tmp = {0};
	memcpy(boost::addressof(tmp), static_cast<base_type*>(this), sizeof(base_type));
	memcpy(static_cast<base_type*>(this), boost::addressof(right), sizeof(base_type));
	memcpy(boost::addressof(right), boost::addressof(tmp), sizeof(base_type));
}

void c_mongo_apm_callbacks::pro_swap(this_type& right)
{
	this_type::pro_swap(static_cast<base_type&>(right));
}

} // namespace nsql_database_system
} // namespace yggr
