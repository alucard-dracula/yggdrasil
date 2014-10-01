//work_runner_saver.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_TASK_CENTER_WORK_RUNNER_SAVER_HPP__
#define __YGGR_TASK_CENTER_WORK_RUNNER_SAVER_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/thread/type_traits/tags.hpp>
#include <yggr/task_center/type_traits/tags.hpp>

namespace yggr
{
namespace task_center
{

template<typename Mark, typename WorkRunner>
class work_runner_saver
	: public WorkRunner
{
public:
	
	typedef Mark mark_type;
	typedef WorkRunner work_runner_type;
	typedef work_runner_type base_type;
	
	BOOST_MPL_ASSERT((boost::is_same<typename work_runner_type::tag_type,
										yggr::thread::type_traits::tag_static_work_runner>));

	typedef typename base_type::task_type task_type;
	typedef typename base_type::init_type init_type;

	typedef yggr::task_center::type_traits::must_need_init init_tag_type;

private:
	typedef work_runner_saver this_type;

public:
	work_runner_saver(void)
	{
		assert(false);
	}

	work_runner_saver(const init_type& init)
		: base_type(init)
	{
	}

	static bool s_need_link(void)
	{
		return base_type::s_need_link();
	}

	bool need_link(void) const
	{
		return base_type::s_need_link();
	}

	template<typename Handler>
	void link(const Handler& handler)
	{
		base_type::link(handler);
	}

	void drop_link(void)
	{
		base_type::drop_link();
	}

	bool push(const task_type& val)
	{
		base_type& base = *this;
		return base(val);
	}

	template<typename IDs>
	inline void drop_task(const IDs& ids)
	{
		//empty op
	}

	template<typename IDs, typename KeyGetter>
	inline void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
		//empty op
	}

	void clear_tasks(void)
	{
		//empty op
		base_type::clear_tasks();
	}

	void clear(void)
	{
		//empty op
		base_type::clear();
	}
};

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_WORK_RUNNER_SAVER_HPP__