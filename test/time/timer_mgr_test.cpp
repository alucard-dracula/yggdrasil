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

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

struct task_pak
{
	typedef yggr::time::time time_type;
	typedef yggr::time::timer<time_type> timer_type;

	task_pak(const time_type& step)
		: _step(step)
	{
	}

	~task_pak(void)
	{
	}

	inline yggr::u32 operator()(void)
	{
		if(_step < _timer.elapsed())
		{
			std::cout << "runit" << std::endl;
			_timer.restart();
			return yggr::time::timer_task_state::E_timer_task_ran;
		}

		return yggr::time::timer_task_state::E_timer_task_not_ran;
	}

private:
	time_type _step;
	timer_type _timer;
};

int main(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	typedef yggr::ids::id64_type id_type;
	typedef yggr::time::timer_mgr<id_type> timer_mgr_type;

	timer_mgr_type tm_mgr;
	tm_mgr.start();

	id_type id = tm_mgr.add_task<task_pak>(yggr::time::time(3, 0));

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	tm_mgr.exit_task(id);

	std::cout << "erased" << std::endl;
	std::cout << "wait some time check timer task is running" << std::endl;

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	tm_mgr.stop();
	std::cout << "stopped" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
