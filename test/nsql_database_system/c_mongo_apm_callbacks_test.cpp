// c_mongo_apm_callbacks_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_apm_callbacks.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

// callbacks_def
// command
void my_apm_command_started(const mongoc_apm_command_started_t *event)
{
	std::cout << "my_apm_command_started" << std::endl;
}

void my_apm_command_succeeded(const mongoc_apm_command_succeeded_t *event)
{
	std::cout << "my_apm_command_succeeded" << std::endl;
}

void my_apm_command_failed(const mongoc_apm_command_failed_t *event)
{
	std::cout << "my_apm_command_failed" << std::endl;
}

// server
void my_apm_server_changed(const mongoc_apm_server_changed_t *event)
{
	std::cout << "my_apm_server_changed" << std::endl;
}

void my_apm_server_opening(const mongoc_apm_server_opening_t *event)
{
	std::cout << "my_apm_server_opening" << std::endl;
}

void my_apm_server_closed(const mongoc_apm_server_closed_t *event)
{
	std::cout << "my_apm_server_closed" << std::endl;
}

// topology
void my_apm_topology_changed(const mongoc_apm_topology_changed_t *event)
{
	std::cout << "my_apm_topology_changed" << std::endl;
}

void my_apm_topology_opening(const mongoc_apm_topology_opening_t *event)
{
	std::cout << "my_apm_topology_opening" << std::endl;
}

void my_apm_topology_closed(const mongoc_apm_topology_closed_t *event)
{
	std::cout << "my_apm_topology_closed" << std::endl;
}

// server_heartbeat

void my_apm_server_heartbeat_started(const mongoc_apm_server_heartbeat_started_t *event)
{
	std::cout << "my_apm_server_heartbeat_started" << std::endl;
}

void my_apm_server_heartbeat_succeeded(const mongoc_apm_server_heartbeat_succeeded_t *event)
{
	std::cout << "my_apm_server_heartbeat_succeeded" << std::endl;
}

void my_apm_server_heartbeat_failed(const mongoc_apm_server_heartbeat_failed_t *event)
{
	std::cout << "my_apm_server_heartbeat_failed" << std::endl;
}

// test
void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_apm_callbacks apm_callbacks_type;

	{
		apm_callbacks_type callbacks;
		yggr_test_assert(callbacks.empty());
	}

	{
		apm_callbacks_type callbacks(&my_apm_command_started,
										&my_apm_command_succeeded,
										&my_apm_command_failed,
										&my_apm_server_changed,
										&my_apm_server_opening,
										&my_apm_server_closed,
										&my_apm_topology_changed,
										&my_apm_topology_opening,
										&my_apm_topology_closed,
										&my_apm_server_heartbeat_started,
										&my_apm_server_heartbeat_succeeded,
										&my_apm_server_heartbeat_failed);
		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks(callbacks_sample.org_pointer());

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks(*callbacks_sample.org_pointer());

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks(boost::move(callbacks_sample));

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks(callbacks_sample);

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_apm_callbacks apm_callbacks_type;

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks;
		callbacks = *(callbacks_sample.org_pointer());

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks;
		callbacks = boost::move(callbacks_sample);

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	{
		apm_callbacks_type callbacks_sample(&my_apm_command_started,
											&my_apm_command_succeeded,
											&my_apm_command_failed,
											&my_apm_server_changed,
											&my_apm_server_opening,
											&my_apm_server_closed,
											&my_apm_topology_changed,
											&my_apm_topology_opening,
											&my_apm_topology_closed,
											&my_apm_server_heartbeat_started,
											&my_apm_server_heartbeat_succeeded,
											&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks;
		callbacks = callbacks_sample;

		yggr_test_assert(!callbacks.empty());

		yggr_test_assert(callbacks.cb_command_strted() == &my_apm_command_started);
		yggr_test_assert(callbacks.cb_command_succeeded() == &my_apm_command_succeeded);
		yggr_test_assert(callbacks.cb_command_failed() == &my_apm_command_failed);

		yggr_test_assert(callbacks.cb_server_changed() == &my_apm_server_changed);
		yggr_test_assert(callbacks.cb_server_opening() == &my_apm_server_opening);
		yggr_test_assert(callbacks.cb_server_closed() == &my_apm_server_closed);

		yggr_test_assert(callbacks.cb_topology_changed() == &my_apm_topology_changed);
		yggr_test_assert(callbacks.cb_topology_opening() == &my_apm_topology_opening);
		yggr_test_assert(callbacks.cb_topology_closed() == &my_apm_topology_closed);

		yggr_test_assert(callbacks.cb_server_heartbeat_started() == &my_apm_server_heartbeat_started);
		yggr_test_assert(callbacks.cb_server_heartbeat_succeeded() == &my_apm_server_heartbeat_succeeded);
		yggr_test_assert(callbacks.cb_server_heartbeat_failed() == &my_apm_server_heartbeat_failed);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_apm_callbacks apm_callbacks_type;

	{
		apm_callbacks_type callbacks_sample1(&my_apm_command_started,
												&my_apm_command_succeeded,
												&my_apm_command_failed,
												&my_apm_server_changed,
												&my_apm_server_opening,
												&my_apm_server_closed,
												&my_apm_topology_changed,
												&my_apm_topology_opening,
												&my_apm_topology_closed,
												&my_apm_server_heartbeat_started,
												&my_apm_server_heartbeat_succeeded,
												&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks_sample2;

		apm_callbacks_type callbacks1(callbacks_sample1);
		apm_callbacks_type callbacks2(callbacks_sample2);

		callbacks1.swap(*callbacks2.org_pointer());

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		callbacks1.swap(boost::move(callbacks2));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		callbacks1.swap(callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
	}

	{
		apm_callbacks_type callbacks_sample1(&my_apm_command_started,
												&my_apm_command_succeeded,
												&my_apm_command_failed,
												&my_apm_server_changed,
												&my_apm_server_opening,
												&my_apm_server_closed,
												&my_apm_topology_changed,
												&my_apm_topology_opening,
												&my_apm_topology_closed,
												&my_apm_server_heartbeat_started,
												&my_apm_server_heartbeat_succeeded,
												&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks_sample2;

		apm_callbacks_type callbacks1(callbacks_sample1);
		apm_callbacks_type callbacks2(callbacks_sample2);

		std::swap(callbacks1, boost::move(*callbacks2.org_pointer()));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		std::swap(boost::move(callbacks1), *callbacks2.org_pointer());

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		std::swap(boost::move(callbacks1), callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		std::swap(callbacks1, boost::move(callbacks2));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		std::swap(callbacks1, callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
	}

	{
		apm_callbacks_type callbacks_sample1(&my_apm_command_started,
												&my_apm_command_succeeded,
												&my_apm_command_failed,
												&my_apm_server_changed,
												&my_apm_server_opening,
												&my_apm_server_closed,
												&my_apm_topology_changed,
												&my_apm_topology_opening,
												&my_apm_topology_closed,
												&my_apm_server_heartbeat_started,
												&my_apm_server_heartbeat_succeeded,
												&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks_sample2;

		apm_callbacks_type callbacks1(callbacks_sample1);
		apm_callbacks_type callbacks2(callbacks_sample2);

		boost::swap(callbacks1, boost::move(*callbacks2.org_pointer()));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		boost::swap(boost::move(callbacks1), *callbacks2.org_pointer());

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		boost::swap(boost::move(callbacks1), callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		boost::swap(callbacks1, boost::move(callbacks2));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		boost::swap(callbacks1, callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
	}

	{
		apm_callbacks_type callbacks_sample1(&my_apm_command_started,
												&my_apm_command_succeeded,
												&my_apm_command_failed,
												&my_apm_server_changed,
												&my_apm_server_opening,
												&my_apm_server_closed,
												&my_apm_topology_changed,
												&my_apm_topology_opening,
												&my_apm_topology_closed,
												&my_apm_server_heartbeat_started,
												&my_apm_server_heartbeat_succeeded,
												&my_apm_server_heartbeat_failed);

		apm_callbacks_type callbacks_sample2;

		apm_callbacks_type callbacks1(callbacks_sample1);
		apm_callbacks_type callbacks2(callbacks_sample2);

		yggr::swap(callbacks1, boost::move(*callbacks2.org_pointer()));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		yggr::swap(boost::move(callbacks1), *callbacks2.org_pointer());

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		yggr::swap(boost::move(callbacks1), callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));

		yggr::swap(callbacks1, boost::move(callbacks2));

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));

		yggr::swap(callbacks1, callbacks2);

		yggr_test_assert(0 == memcmp(callbacks1.org_pointer(), callbacks_sample2.org_pointer(), sizeof(apm_callbacks_type)));
		yggr_test_assert(0 == memcmp(callbacks2.org_pointer(), callbacks_sample1.org_pointer(), sizeof(apm_callbacks_type)));
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_apm_callbacks apm_callbacks_type;

	{
		apm_callbacks_type callbacks(&my_apm_command_started,
										&my_apm_command_succeeded,
										&my_apm_command_failed,
										&my_apm_server_changed,
										&my_apm_server_opening,
										&my_apm_server_closed,
										&my_apm_topology_changed,
										&my_apm_topology_opening,
										&my_apm_topology_closed,
										&my_apm_server_heartbeat_started,
										&my_apm_server_heartbeat_succeeded,
										&my_apm_server_heartbeat_failed);

		yggr_test_assert(!callbacks.empty());

		apm_callbacks_type& apm_callbacks_ref = callbacks;
		const apm_callbacks_type& apm_callbacks_cref = callbacks;

		// command
		apm_callbacks_ref.cb_command_strted() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_command_strted());

		apm_callbacks_ref.cb_command_succeeded() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_command_succeeded());

		apm_callbacks_ref.cb_command_failed() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_command_failed());

		// server
		apm_callbacks_ref.cb_server_changed() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_server_changed());

		apm_callbacks_ref.cb_server_opening() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_server_opening());

		apm_callbacks_ref.cb_server_closed() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_server_closed());

		// topology
		apm_callbacks_ref.cb_topology_changed() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_topology_changed());

		apm_callbacks_ref.cb_topology_opening() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_topology_opening());

		apm_callbacks_ref.cb_topology_closed() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_topology_closed());

		// server_heartbeat
		apm_callbacks_ref.cb_server_heartbeat_started() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_server_heartbeat_started());

		apm_callbacks_ref.cb_server_heartbeat_succeeded() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_server_heartbeat_succeeded());

		apm_callbacks_ref.cb_server_heartbeat_failed() = 0;
		yggr_test_assert(!apm_callbacks_cref.cb_server_heartbeat_failed());
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	wait_any_key(argc, argv);
	return 0;
}
