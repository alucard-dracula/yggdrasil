//timer_mgr_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>
#include <yggr/time/timer_mgr.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

struct task_pak
{
	typedef yggr::time::time time_type;
	typedef yggr::time::timer<time_type> timer_type;

	task_pak(const time_type& step, int n)
		: _n(n), _step(step)
	{
	}

	~task_pak(void)
	{
	}

	yggr::u32 operator()(void)
	{
		if(_step < _timer.elapsed())
		{
			std::cout << "runit " << _n << std::endl;
			_timer.restart();
			return yggr::time::timer_task_state::E_timer_task_ran;
		}

		return yggr::time::timer_task_state::E_timer_task_not_ran;
	}

private:
	int _n;
	time_type _step;
	timer_type _timer;
};

int main(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	typedef yggr::ids::id64_type id_type;
	typedef yggr::time::timer_mgr<id_type, yggr::thread::boost_thread_config_type, yggr::u64> timer_mgr_type;

	timer_mgr_type tm_mgr;
	tm_mgr.start();

	timer_mgr_type::delegate_ptr_type pgate1 = tm_mgr.get_delegate(yggr::u64(10));
	yggr_test_assert(pgate1);
	id_type id1 = pgate1->add_task<task_pak>(yggr::time::time(2, 0), 1);
	std::cout << id1 << std::endl;

	timer_mgr_type::delegate_ptr_type pgate2 = tm_mgr.get_delegate(yggr::u64(20));
	yggr_test_assert(pgate2);
	id_type id2 = pgate2->add_task<task_pak>(yggr::time::time(5, 0), 2);
	std::cout << id2 << std::endl;

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	pgate1->clear();
	std::cout << "erased_1" << std::endl;

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	pgate2->clear();
	std::cout << "erased_2" << std::endl;

	std::cout << "please wait some time check timer task is running" << std::endl;

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	tm_mgr.stop();
	std::cout << "stopped" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
