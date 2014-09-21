//task_center.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_CENTER_HPP__
#define __YGGR_TASK_CENTER_TASK_CENTER_HPP__

#include <boost/any.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/bind.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/task_center/type_traits/tags.hpp>


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
	//typedef ID_Genner task_id_genner_type;
	//typedef Task task_type;
	typedef Task_Creator task_creator_type;
	typedef typename task_creator_type::task_type task_type;
	typedef typename task_type::owner_id_type owner_id_type;
	//typedef typename task_type::data_container_type task_data_container_type;

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

	//typedef typename task_type::id_type task_id_type;

	//BOOST_MPL_ASSERT((boost::is_same< task_id_type, typename task_id_genner_type::id_type >));

private:
	typedef typename cdt_saver_type::init_tag_type cdt_saver_init_tag_type;
	typedef typename rst_saver_type::init_tag_type rst_saver_init_tag_type;

	typedef task_center this_type;

private:
	template<typename Mark, typename Nil_T = int>
	struct saver_switch;

	template<typename Nil_T>
	struct saver_switch<cdt_mark_type, Nil_T>
	{
		typedef cdt_saver_type type;
	};

	template<typename Nil_T>
	struct saver_switch<rst_mark_type, Nil_T>
	{
		typedef rst_saver_type type;
	};

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
		if(cdt_saver_type::s_need_link())
		{
			cdt_saver_type::link(boost::bind(&this_type::send<rst_mark_type>,
												this, _1));
		}

		if(rst_saver_type::s_need_link())
		{
			rst_saver_type::link(boost::bind(&this_type::send<cdt_mark_type>,
												this, _1));
		}
	}

	~task_center(void)
	{
		if(rst_saver_type::s_need_link())
		{
			rst_saver_type::drop_link();
		}

		if(cdt_saver_type::s_need_link())
		{
			cdt_saver_type::drop_link();
		}
	}

//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
//
//#define BOOST_PP_LOCAL_MACRO( __n__ ) \
//	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
//	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) const Obj& obj) { \
//		typedef typename saver_switch<Mark>::type saver_type; \
//		task_type task(boost::move(_task_creator(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
//													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj))); \
//		if(task.empty()) { return; } \
//		saver_type::push(yggr::move::move_helper::forced_move(task)); } \
//	\
//	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
//	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) Obj& obj) { \
//		typedef typename saver_switch<Mark>::type saver_type; \
//		task_type task(boost::move(_task_creator(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
//													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj))); \
//		if(task.empty()) { return; } \
//		saver_type::push(yggr::move::move_helper::forced_move(task)); }
//
//#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
//#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
//#include BOOST_PP_LOCAL_ITERATE(  )
//#undef YGGR_PP_FOO_ARG_NAME
//
//#else
//
//#define BOOST_PP_LOCAL_MACRO( __n__ ) \
//	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
//	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) const Obj& obj) { \
//		typedef typename saver_switch<Mark>::type saver_type; \
//		task_type task(_task_creator(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
//													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj)); \
//		if(task.empty()) { return; } \
//		saver_type::push(yggr::move::move_helper::forced_move(task)); } \
//	\
//	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
//	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) Obj& obj) { \
//		typedef typename saver_switch<Mark>::type saver_type; \
//		task_type task(_task_creator(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
//													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj)); \
//		if(task.empty()) { return; } \
//		saver_type::push(yggr::move::move_helper::forced_move(task)); }
//
//#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
//#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
//#include BOOST_PP_LOCAL_ITERATE(  )
//#undef YGGR_PP_FOO_ARG_NAME
//
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES


#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) const Obj& obj) { \
		typedef typename saver_switch<Mark>::type saver_type; \
		task_type task(boost::move(task_creator_type()(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj))); \
		if(task.empty()) { return; } \
		saver_type::push(yggr::move::move_helper::forced_move(task)); } \
	\
	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) Obj& obj) { \
		typedef typename saver_switch<Mark>::type saver_type; \
		task_type task(boost::move(task_creator_type()(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj))); \
		if(task.empty()) { return; } \
		saver_type::push(yggr::move::move_helper::forced_move(task)); }

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) const Obj& obj) { \
		typedef typename saver_switch<Mark>::type saver_type; \
		task_type task(task_creator_type()(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj)); \
		if(task.empty()) { return; } \
		saver_type::push(yggr::move::move_helper::forced_move(task)); } \
	\
	template<typename Mark, YGGR_PP_FOO_TYPES_DEF(__n__) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) typename Obj> \
	void send( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) Obj& obj) { \
		typedef typename saver_switch<Mark>::type saver_type; \
		task_type task(task_creator_type()(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
													YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) obj)); \
		if(task.empty()) { return; } \
		saver_type::push(yggr::move::move_helper::forced_move(task)); }

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	//template<typename Mark>
	//void send(task_type& task)
	//{
	//	typedef typename saver_switch<Mark>::type saver_type;

	//	task_type& ref_task = _task_creator(task);
	//	if(ref_task.empty())
	//	{
	//		return;
	//	}

	//	saver_type::push(ref_task);
	//}

	template<typename Mark>
	void send(task_type& task)
	{
		typedef typename saver_switch<Mark>::type saver_type;

		task_type& ref_task = task_creator_type()(task);
		if(ref_task.empty())
		{
			return;
		}

		saver_type::push(ref_task);
	}

	template<typename Handler>
	bool hold_recv(const owner_id_type& id, const Handler& handler)
	{
		return rst_saver_type::hold_recv(id, handler);
	}

	bool stop_recv(const owner_id_type& id)
	{
		return rst_saver_type::stop_recv(id);
	}

	template<typename Mark>
	bool empty(void) const
	{
		typedef typename saver_switch<Mark>::type saver_type;
		return saver_type::empty();
	}

	template <typename Mark, typename Handler>
	bool get_task(const Handler& handler)
	{
		typedef typename saver_switch<Mark>::type saver_type;
		return saver_type::pop(handler);
	}

	template<typename Mark, typename IDs>
	void drop_task(const IDs& ids)
	{
		typedef typename saver_switch<Mark>::type saver_type;
		saver_type::drop_task(ids);
	}

	template<typename Mark, typename IDs, typename KeyGetter>
	void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
		typedef typename saver_switch<Mark>::type saver_type;
		saver_type::drop_task(ids, key_getter);
	}

	void drop_all(void)
	{
		cdt_saver_type::clear();
		rst_saver_type::clear();
	}

	void clear(void)
	{
		cdt_saver_type::clear();
		rst_saver_type::clear();
	}
//private:
//	task_creator_type _task_creator;
};

} // namespace task_center
} // namespace yggr

#endif //__YGGR_TASK_CENTER_TASK_CENTER_HPP__
