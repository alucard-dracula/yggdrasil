//timer_mgr_test.cpp

#include <iostream>
#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>
#include <yggr/time/timer_mgr.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

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

	yggr::u32 operator()(void)
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
	typedef yggr::ids::id64_type id_type;

	typedef yggr::time::timer_mgr<id_type> timer_mgr_type;

	timer_mgr_type tm_mgr;
	tm_mgr.start();

	id_type id = tm_mgr.add_task<task_pak>(yggr::time::time(3, 0));

	char cc = 0;
	std::cin >> cc;

	//tm_mgr.erase(id);
	tm_mgr.exit_task(id);

	std::cout << "erased" << std::endl;

	std::cin >> cc;

	tm_mgr.stop();
	return 0;
}
