//c_mongo_apm_callbacks.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_APM_CALLBACKS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_APM_CALLBACKS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/detail/incomplete t_move_patch_macro.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_apm_callbacks_t {
//   mongoc_apm_command_started_cb_t started;
//   mongoc_apm_command_succeeded_cb_t succeeded;
//   mongoc_apm_command_failed_cb_t failed;
//   mongoc_apm_server_changed_cb_t server_changed;
//   mongoc_apm_server_opening_cb_t server_opening;
//   mongoc_apm_server_closed_cb_t server_closed;
//   mongoc_apm_topology_changed_cb_t topology_changed;
//   mongoc_apm_topology_opening_cb_t topology_opening;
//   mongoc_apm_topology_closed_cb_t topology_closed;
//   mongoc_apm_server_heartbeat_started_cb_t server_heartbeat_started;
//   mongoc_apm_server_heartbeat_succeeded_cb_t server_heartbeat_succeeded;
//   mongoc_apm_server_heartbeat_failed_cb_t server_heartbeat_failed;
//};

struct yggr_mongoc_apm_callbacks_t
{
	mongoc_apm_command_started_cb_t started;
	mongoc_apm_command_succeeded_cb_t succeeded;
	mongoc_apm_command_failed_cb_t failed;

	mongoc_apm_server_changed_cb_t server_changed;
	mongoc_apm_server_opening_cb_t server_opening;
	mongoc_apm_server_closed_cb_t server_closed;

	mongoc_apm_topology_changed_cb_t topology_changed;
	mongoc_apm_topology_opening_cb_t topology_opening;
	mongoc_apm_topology_closed_cb_t topology_closed;

	mongoc_apm_server_heartbeat_started_cb_t server_heartbeat_started;
	mongoc_apm_server_heartbeat_succeeded_cb_t server_heartbeat_succeeded;
	mongoc_apm_server_heartbeat_failed_cb_t server_heartbeat_failed;
};

} // namespace nsql_database_system
} // namespace yggr

// gcc needed fix invalid use of incomplete type 'struct _mongoc_apm_callbacks_t'
YGGR_PP_NSQL_DBS_INCOMPLETE_T_MOVE_HELPER(
	mongoc_apm_callbacks_t, ::yggr::nsql_database_system::yggr_mongoc_apm_callbacks_t )

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_apm_callbacks_t {
//   mongoc_apm_command_started_cb_t started;
//   mongoc_apm_command_succeeded_cb_t succeeded;
//   mongoc_apm_command_failed_cb_t failed;
//   mongoc_apm_server_changed_cb_t server_changed;
//   mongoc_apm_server_opening_cb_t server_opening;
//   mongoc_apm_server_closed_cb_t server_closed;
//   mongoc_apm_topology_changed_cb_t topology_changed;
//   mongoc_apm_topology_opening_cb_t topology_opening;
//   mongoc_apm_topology_closed_cb_t topology_closed;
//   mongoc_apm_server_heartbeat_started_cb_t server_heartbeat_started;
//   mongoc_apm_server_heartbeat_succeeded_cb_t server_heartbeat_succeeded;
//   mongoc_apm_server_heartbeat_failed_cb_t server_heartbeat_failed;
//};

//struct yggr_mongoc_apm_callbacks_t
//{
//	mongoc_apm_command_started_cb_t started;
//	mongoc_apm_command_succeeded_cb_t succeeded;
//	mongoc_apm_command_failed_cb_t failed;
//
//	mongoc_apm_server_changed_cb_t server_changed;
//	mongoc_apm_server_opening_cb_t server_opening;
//	mongoc_apm_server_closed_cb_t server_closed;
//
//	mongoc_apm_topology_changed_cb_t topology_changed;
//	mongoc_apm_topology_opening_cb_t topology_opening;
//	mongoc_apm_topology_closed_cb_t topology_closed;
//
//	mongoc_apm_server_heartbeat_started_cb_t server_heartbeat_started;
//	mongoc_apm_server_heartbeat_succeeded_cb_t server_heartbeat_succeeded;
//	mongoc_apm_server_heartbeat_failed_cb_t server_heartbeat_failed;
//};

class c_mongo_apm_callbacks
	: protected yggr_mongoc_apm_callbacks_t
{
protected:
	typedef yggr_mongoc_apm_callbacks_t base_type;

public:
	typedef mongoc_apm_callbacks_t org_type;

public:
	// foo pointer types
	typedef mongoc_apm_command_started_cb_t mongoc_apm_command_started_cb_type;
	typedef mongoc_apm_command_succeeded_cb_t mongoc_apm_command_succeeded_cb_type;
	typedef mongoc_apm_command_failed_cb_t mongoc_apm_command_failed_cb_type;

	typedef mongoc_apm_server_changed_cb_t mongoc_apm_server_changed_cb_type;
	typedef mongoc_apm_server_opening_cb_t mongoc_apm_server_opening_cb_type;
	typedef mongoc_apm_server_closed_cb_t mongoc_apm_server_closed_cb_type;

	typedef mongoc_apm_topology_changed_cb_t mongoc_apm_topology_changed_cb_type;
	typedef mongoc_apm_topology_opening_cb_t mongoc_apm_topology_opening_cb_type;
	typedef mongoc_apm_topology_closed_cb_t mongoc_apm_topology_closed_cb_type;

	typedef mongoc_apm_server_heartbeat_started_cb_t mongoc_apm_server_heartbeat_started_cb_type;
	typedef mongoc_apm_server_heartbeat_succeeded_cb_t mongoc_apm_server_heartbeat_succeeded_cb_type;
	typedef mongoc_apm_server_heartbeat_failed_cb_t mongoc_apm_server_heartbeat_failed_cb_type;

	// foo arg event types
	typedef mongoc_apm_command_started_t mongoc_apm_command_started_type;
	typedef mongoc_apm_command_succeeded_t mongoc_apm_command_succeeded_type;
	typedef mongoc_apm_command_failed_t mongoc_apm_command_failed_type;

	typedef mongoc_apm_server_changed_t mongoc_apm_server_changed_type;
	typedef mongoc_apm_server_opening_t mongoc_apm_server_opening_type;
	typedef mongoc_apm_server_closed_t mongoc_apm_server_closed_type;

	typedef mongoc_apm_topology_changed_t mongoc_apm_topology_changed_type;
	typedef mongoc_apm_topology_opening_t mongoc_apm_topology_opening_type;
	typedef mongoc_apm_topology_closed_t mongoc_apm_topology_closed_type;

	typedef mongoc_apm_server_heartbeat_started_t mongoc_apm_server_heartbeat_started_type;
	typedef mongoc_apm_server_heartbeat_succeeded_t mongoc_apm_server_heartbeat_succeeded_type;
	typedef mongoc_apm_server_heartbeat_failed_t mongoc_apm_server_heartbeat_failed_type;

private:
	typedef c_mongo_apm_callbacks this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_apm_callbacks(void);
	c_mongo_apm_callbacks(mongoc_apm_command_started_cb_type pcb_cmd_started,
							mongoc_apm_command_succeeded_cb_type pcb_cmd_succeeded = 0,
							mongoc_apm_command_failed_cb_type pcb_cmd_failed = 0,
							mongoc_apm_server_changed_cb_type pcb_svr_changed = 0,
							mongoc_apm_server_opening_cb_type pcb_svr_opening = 0,
							mongoc_apm_server_closed_cb_type pcb_svr_closed = 0,
							mongoc_apm_topology_changed_cb_type pcb_tpy_changed = 0,
							mongoc_apm_topology_opening_cb_type pcb_tpy_opening = 0,
							mongoc_apm_topology_closed_cb_type pcb_tpy_closed = 0,
							mongoc_apm_server_heartbeat_started_cb_type pcb_shb_started = 0,
							mongoc_apm_server_heartbeat_succeeded_cb_type pcb_shb_succeeded = 0,
							mongoc_apm_server_heartbeat_failed_cb_type pcb_shb_failed = 0);

	c_mongo_apm_callbacks(const org_type* pcallbacks);
	c_mongo_apm_callbacks(const org_type& callbacks);

	c_mongo_apm_callbacks(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		this_type::pro_copy(right_cref);
	}

	c_mongo_apm_callbacks(const this_type& right);

	~c_mongo_apm_callbacks(void);

public:
	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		this_type::pro_copy(right_cref);
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	void swap(org_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		base_type& base = *this;
		return reinterpret_cast<org_type*>(boost::addressof(base));
	}

	inline const org_type* org_pointer(void) const
	{
		const base_type& base = *this;
		return reinterpret_cast<const org_type*>(boost::addressof(base));
	}

public:
	inline operator org_type&(void)
	{
		return *(this_type::org_pointer());
	}

	inline operator const org_type&(void) const
	{
		return *(this_type::org_pointer());
	}

	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	// command
	// cb_command_strted
	inline mongoc_apm_command_started_cb_type& cb_command_strted(void)
	{
		return base_type::started;
	}

	inline mongoc_apm_command_started_cb_type cb_command_strted(void) const
	{
		return base_type::started;
	}

	// cb_command_succeeded
	inline mongoc_apm_command_succeeded_cb_type& cb_command_succeeded(void)
	{
		return base_type::succeeded;
	}

	inline mongoc_apm_command_succeeded_cb_type cb_command_succeeded(void) const
	{
		return base_type::succeeded;
	}

	// cb_command_failed
	inline mongoc_apm_command_failed_cb_type& cb_command_failed(void)
	{
		return base_type::failed;
	}

	inline mongoc_apm_command_failed_cb_type cb_command_failed(void) const
	{
		return base_type::failed;
	}

	//server
	// cb_server_changed
	inline mongoc_apm_server_changed_cb_type& cb_server_changed(void)
	{
		return base_type::server_changed;
	}

	inline mongoc_apm_server_changed_cb_type cb_server_changed(void) const
	{
		return base_type::server_changed;
	}

	//cb_server_changed
	inline mongoc_apm_server_opening_cb_type& cb_server_opening(void)
	{
		return base_type::server_opening;
	}

	inline mongoc_apm_server_opening_cb_type cb_server_opening(void) const
	{
		return base_type::server_opening;
	}

	//cb_server_changed
	inline mongoc_apm_server_closed_cb_type& cb_server_closed(void)
	{
		return base_type::server_closed;
	}

	inline mongoc_apm_server_closed_cb_type cb_server_closed(void) const
	{
		return base_type::server_closed;
	}

	//topology
	//cb_topology_changed
	inline mongoc_apm_topology_changed_cb_type& cb_topology_changed(void)
	{
		return base_type::topology_changed;
	}

	inline mongoc_apm_topology_changed_cb_type cb_topology_changed(void) const
	{
		return base_type::topology_changed;
	}

	//cb_topology_changed
	inline mongoc_apm_topology_opening_cb_type& cb_topology_opening(void)
	{
		return base_type::topology_opening;
	}

	inline mongoc_apm_topology_opening_cb_type cb_topology_opening(void) const
	{
		return base_type::topology_opening;
	}

	//cb_topology_closed
	inline mongoc_apm_topology_closed_cb_type& cb_topology_closed(void)
	{
		return base_type::topology_closed;
	}

	inline mongoc_apm_topology_closed_cb_type cb_topology_closed(void) const
	{
		return base_type::topology_closed;
	}

	// server_heartbeat
	//cb_server_heartbeat_started
	inline mongoc_apm_server_heartbeat_started_cb_type& cb_server_heartbeat_started(void)
	{
		return base_type::server_heartbeat_started;
	}

	inline mongoc_apm_server_heartbeat_started_cb_type cb_server_heartbeat_started(void) const
	{
		return base_type::server_heartbeat_started;
	}

	//cb_server_heartbeat_succeeded
	inline mongoc_apm_server_heartbeat_succeeded_cb_type& cb_server_heartbeat_succeeded(void)
	{
		return base_type::server_heartbeat_succeeded;
	}

	inline mongoc_apm_server_heartbeat_succeeded_cb_type cb_server_heartbeat_succeeded(void) const
	{
		return base_type::server_heartbeat_succeeded;
	}

	//cb_server_heartbeat_failed
	inline mongoc_apm_server_heartbeat_failed_cb_type& cb_server_heartbeat_failed(void)
	{
		return base_type::server_heartbeat_failed;
	}

	inline mongoc_apm_server_heartbeat_failed_cb_type cb_server_heartbeat_failed(void) const
	{
		return base_type::server_heartbeat_failed;
	}

public:
	inline bool empty(void) const
	{
		static const base_type empty_obj = {0};
		return 0 == memcmp(static_cast<const base_type*>(this), boost::addressof(empty_obj), sizeof(base_type));
	}

protected:
	void pro_init(void);

	void pro_copy(const base_type& right);
	void pro_copy(const this_type& right);

	void pro_swap(base_type& right);
	void pro_swap(this_type& right);
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_apm_callbacks::org_type& l, c_mongo_apm_callbacks& r)
{
	r.swap(l);
}

inline void swap(c_mongo_apm_callbacks& l, c_mongo_apm_callbacks::org_type& r)
{
	l.swap(r);
}

inline void swap(c_mongo_apm_callbacks& l, c_mongo_apm_callbacks& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_APM_CALLBACKS_HPP__
