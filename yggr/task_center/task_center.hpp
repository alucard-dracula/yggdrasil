//task_center.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_CENTER_HPP__
#define __YGGR_TASK_CENTER_TASK_CENTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/task_center/type_traits/tags.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace task_center
{
namespace detail
{

template<typename Mark, typename CdtSaver, typename RstSaver>
struct task_center_saver_switch;

template<typename CdtSaver, typename RstSaver>
struct task_center_saver_switch<yggr::task_center::type_traits::mark_saver_condition,
								CdtSaver, RstSaver>
{
	typedef CdtSaver type;
};

template<typename CdtSaver, typename RstSaver>
struct task_center_saver_switch<yggr::task_center::type_traits::mark_saver_result,
								CdtSaver, RstSaver>
{
	typedef RstSaver type;
};

} // namespace detail
} // namespace task_center
} // namespace yggr

namespace yggr
{
namespace task_center
{

template<	typename Task_Creator,
			typename Cdt_Saver,
			typename Rst_Saver >
class task_center
	: protected Cdt_Saver,
		protected Rst_Saver
{
public:
	typedef Task_Creator task_creator_type;
	typedef typename task_creator_type::task_type task_type;
	typedef typename task_type::owner_id_type owner_id_type;

	typedef Cdt_Saver cdt_saver_type;
	typedef typename cdt_saver_type::task_type cdt_value_type;
	typedef typename cdt_saver_type::mark_type cdt_mark_type;
	typedef typename cdt_saver_type::init_type cdt_saver_init_type;

	typedef Rst_Saver rst_saver_type;
	typedef typename rst_saver_type::task_type rst_value_type;
	typedef typename rst_saver_type::mark_type rst_mark_type;
	typedef typename rst_saver_type::init_type rst_saver_init_type;

	BOOST_MPL_ASSERT((boost::mpl::if_<boost::is_same<cdt_mark_type, rst_mark_type>,
										boost::mpl::false_, boost::mpl::true_>));
	BOOST_MPL_ASSERT((boost::is_same<task_type, cdt_value_type>));
	BOOST_MPL_ASSERT((boost::mpl::if_< boost::is_same< cdt_mark_type, rst_mark_type >,
										boost::mpl::false_, boost::mpl::true_ >));

private:
	typedef typename cdt_saver_type::init_tag_type cdt_saver_init_tag_type;
	typedef typename rst_saver_type::init_tag_type rst_saver_init_tag_type;

	typedef task_center this_type;

public:
	task_center(void)
	{
		BOOST_MPL_ASSERT((boost::is_same<cdt_saver_init_tag_type,
											yggr::task_center::type_traits::nonmust_need_init>));
		BOOST_MPL_ASSERT((boost::is_same<rst_saver_init_tag_type,
											yggr::task_center::type_traits::nonmust_need_init>));
	}

	task_center(const cdt_saver_init_type& cdt_init,
				const rst_saver_init_type& rst_init)
		: cdt_saver_type(cdt_init), rst_saver_type(rst_init)
	{
        prv_init();
	}

	~task_center(void)
	{
        prv_destroy();
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Mark, typename ...Args> inline
	void send( BOOST_FWD_REF(Args)... args )
	{
		typedef typename
			detail::task_center_saver_switch
			<
				Mark,
				cdt_saver_type,
				rst_saver_type
			>::type saver_type;

		task_creator_type creator;
		task_type task(creator(boost::forward<Args>(args)...));
		if(!task.empty())
		{
			saver_type::push(boost::move(task));
		}
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Mark \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef typename detail::task_center_saver_switch<Mark, cdt_saver_type, rst_saver_type>::type saver_type; \
			task_creator_type creator; \
			task_type task(creator(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA))); \
			if(!task.empty()) { saver_type::push(boost::move(task));  } }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Mark> inline
	void send(task_type& task)
	{
		typedef Mark mark_type;
		this_type::pro_send<mark_type>(task);
	}

	template<typename Handler> inline
	bool hold_recv(const owner_id_type& id, const Handler& handler)
	{
		return rst_saver_type::hold_recv(id, handler);
	}

	inline bool stop_recv(const owner_id_type& id)
	{
		return rst_saver_type::stop_recv(id);
	}

	template<typename Mark> inline
	bool empty(void) const
	{
		typedef typename
			detail::task_center_saver_switch
			<
				Mark,
				cdt_saver_type,
				rst_saver_type
			>::type saver_type;

		return saver_type::empty();
	}

	template <typename Mark, typename Handler> inline
	bool get_task(const Handler& handler)
	{
		typedef typename
			detail::task_center_saver_switch
			<
				Mark,
				cdt_saver_type,
				rst_saver_type
			>::type saver_type;

		return saver_type::pop(handler);
	}

	template<typename Mark, typename IDs> inline
	void drop_task(const IDs& ids)
	{
		typedef typename
			detail::task_center_saver_switch
			<
				Mark,
				cdt_saver_type,
				rst_saver_type
			>::type saver_type;

		saver_type::drop_task(ids);
	}

	template<typename Mark, typename IDs, typename KeyGetter> inline
	void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
		typedef typename
			detail::task_center_saver_switch
			<
				Mark,
				cdt_saver_type,
				rst_saver_type
			>::type saver_type;

		saver_type::drop_task(ids, key_getter);
	}

	inline void drop_all_tasks(void)
	{
		cdt_saver_type::clear_tasks();
		rst_saver_type::clear_tasks();
	}

	inline void clear(void)
	{
		cdt_saver_type::clear();
		rst_saver_type::clear();
	}

private:
	// gcc bind(&this_type::send<Makr>) bind overload function
    template<typename Mark> inline
	void pro_send(task_type& task)
	{
		typedef typename
			detail::task_center_saver_switch
			<
				Mark,
				cdt_saver_type,
				rst_saver_type
			>::type saver_type;

		task_creator_type creator;
		task_type& ref_task = creator(task);
		if(!ref_task.empty())
		{
			saver_type::push(ref_task);
		}
	}

    inline void prv_init(void)
    {
        typedef void (task_center::*send_foo_type)(task_type&);

        if(cdt_saver_type::need_link())
		{
			cdt_saver_type::link(
				boost::bind(
					&task_center::pro_send<rst_mark_type>, // gcc bind(&this_type::send<Makr>) bind overload function
					this, _1));
		}

		if(rst_saver_type::need_link())
		{
			rst_saver_type::link(
				boost::bind(
					&task_center::pro_send<rst_mark_type>, // gcc bind(&this_type::send<Makr>) bind overload function
					this, _1));
		}
    }

    inline void prv_destroy(void)
    {
        if(rst_saver_type::need_link())
		{
			rst_saver_type::drop_link();
		}

		if(cdt_saver_type::need_link())
		{
			cdt_saver_type::drop_link();
		}
    }

};

} // namespace task_center
} // namespace yggr

#endif //__YGGR_TASK_CENTER_TASK_CENTER_HPP__
