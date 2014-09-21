//timer_mgr_test.cpp

#include <iostream>
#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>
#include <yggr/time/timer_mgr.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>

struct task_pak
{
	typedef yggr::time::time time_type;
	typedef yggr::time::timer<time_type> timer_type;

	task_pak(const time_type& step, int n)
		: _step(step), _n(n)
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
	typedef yggr::ids::id64_type id_type;

	typedef yggr::time::timer_mgr<id_type, yggr::thread::boost_thread_config_type, yggr::u64> timer_mgr_type;

	timer_mgr_type tm_mgr;
	tm_mgr.start();

	timer_mgr_type::delegate_ptr_type pgate1 = tm_mgr.get_delegate(yggr::u64(10));
	assert(pgate1);
	id_type id1 = pgate1->add_task<task_pak>(yggr::time::time(3, 0), 1);

	timer_mgr_type::delegate_ptr_type pgate2 = tm_mgr.get_delegate(yggr::u64(20));
	assert(pgate2);
	id_type id2 = pgate2->add_task<task_pak>(yggr::time::time(3, 0), 2);

	char cc = 0;
	std::cin >> cc;

	pgate1->clear();
	pgate2->clear();

	std::cout << "erased" << std::endl;

	std::cin >> cc;

	tm_mgr.stop();
	return 0;
}
