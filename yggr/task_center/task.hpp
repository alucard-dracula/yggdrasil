//task.hpp

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

#ifndef __YGGR_DATA_CENTER_TASK_HPP__
#define __YGGR_DATA_CENTER_TASK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/any_val/any.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/concat.hpp>

#include <yggr/task_center/task_info.hpp>

#include <yggr/task_center/support/task_inner_data_parser.hpp>
#include <yggr/task_center/support/task_owner_info_parser.hpp>
#include <yggr/task_center/support/task_data_info_parser.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <cassert>

namespace yggr
{
namespace task_center
{
namespace detail
{

// task_has_class_info
template<typename Result, typename Container>
struct task_has_class_info_impl 
{
private: 
	typedef Result ret_type;
	typedef Container cont_type;
	typedef u8 true_type;
	typedef u32 false_type;
	
	template<typename T, ret_type (T::*)(void) const, typename Nil = void > 
	struct check_t;
	
	template<typename T>
	static true_type chk_foo(const check_t<T, &T::class_info, void>* );
	
	template<typename T>
	static false_type chk_foo(...);
	
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; 
};

template<typename Result, typename Container>
struct task_has_class_info
	: public task_has_class_info_impl<Result, Container>::type 
{
};

// task_has_class_name
template<typename Result, typename Container>
struct task_has_class_name_impl 
{
private: 
	typedef Result ret_type;
	typedef Container cont_type;
	typedef u8 true_type;
	typedef u32 false_type;
	
	template<typename T, ret_type (T::*)(void) const, typename Nil = void > 
	struct check_t;
	
	template<typename T>
	static true_type chk_foo(const check_t<T, &T::class_name, void>* );
	
	template<typename T>
	static false_type chk_foo(...);
	
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; 
};

template<typename Result, typename Container>
struct task_has_class_name
	: public task_has_class_name_impl<Result, Container>::type 
{
};

// id getters
template<typename TaskID>
struct task_inner_task_id_getter
{
	typedef TaskID id_type;
	typedef const id_type& result_type;

	template<typename Task> inline
	result_type operator()(const Task& right) const
	{
		return right.task_id();
	}
};

template<typename OwnerInfo>
struct task_inner_owner_info_getter
{
	typedef OwnerInfo owner_info_type;
	typedef const owner_info_type& result_type;

	template<typename Task> inline
	result_type operator()(const Task& right) const
	{
		return right.owner_info();
	}
};

template<typename OwnerID>
struct task_inner_owner_id_getter
{
	typedef OwnerID owner_id_type;
	typedef owner_id_type result_type;

	template<typename Task> inline
	result_type operator()(const Task& right) const
	{
		return right.owner_id();
	}
};

template<typename DataInfo>
struct task_inner_data_info_getter
{
	typedef DataInfo data_info_type;
	typedef const data_info_type& result_type;

	template<typename Task> inline
	result_type operator()(const Task& right) const
	{
		return right.data_info();
	}
};

template<typename DataID>
struct task_inner_data_id_getter
{
	typedef DataID data_id_type;
	typedef const data_id_type& result_type;

	template<typename Task> inline
	result_type operator()(const Task& right) const
	{
		return right.data_id();
	}
};

template<typename DataVer>
struct task_inner_data_ver_getter
{
	typedef DataVer data_ver_type;
	typedef const data_ver_type& result_type;

	template<typename Task> inline
	result_type operator()(const Task& right) const
	{
		return right.data_ver();
	}
};

namespace detail
{

template<typename T, bool is_ref = boost::is_reference<T>::value >
struct class_info_result_t;

template<typename T>
struct class_info_result_t<T, true>
{
	typedef T type;
};

template<typename T>
struct class_info_result_t<T, false>
	: public
		boost::add_reference
		<
			typename boost::remove_pointer<T>::type
		>
{
};

} // namespace detail

template<typename ClassInfo>
struct task_inner_class_info_getter
{
	typedef ClassInfo class_info_type;
	//typedef const class_info_type& result_type;
	typedef typename detail::class_info_result_t<class_info_type>::type result_type;

	template<typename Task> inline
	typename boost::enable_if<task_has_class_info<result_type, Task>, result_type>::type
		operator()(const Task& right) const
	{
		return right.class_info();
	}

	template<typename Other> inline
	typename boost::disable_if<task_has_class_info<result_type, Other>, result_type>::type
		operator()(const Other& other) const
	{
		return typeid(Other);
	}

	template<typename Other> inline
	typename boost::disable_if<task_has_class_info<result_type, Other>, result_type>::type
		get(void) const
	{
		return typeid(Other);
	}
};

template<typename ClassName>
struct task_inner_class_name_getter
{
	typedef ClassName class_name_type;
	typedef class_name_type result_type;

	template<typename Task> inline
	typename boost::enable_if<task_has_class_name<result_type, Task>, result_type>::type
		operator()(const Task& right) const
	{
		return right.class_name();
	}

	template<typename Other> inline
	typename boost::disable_if<task_has_class_name<result_type, Other>, result_type>::type
		operator()(const Other& other) const
	{
		return typeid(Other).name();
	}

	template<typename Other> inline
	typename boost::disable_if<task_has_class_name<result_type, Other>, result_type>::type
		get(void) const
	{
		return typeid(Other).name();
	}
};

} // namespace detail
} // namespace task_center
} // namespace yggr

namespace yggr
{
namespace task_center
{

template<typename TaskID,
			typename OwnerInfo,
			typename TaskDataInfo,
			typename DataContainer = ::yggr::any,
			template<typename _DataContainer>
				class Task_Inner_Data_Parser = yggr::task_center::support::task_inner_data_parser,
			template<typename _Owner_Info>
				class Task_Owner_Info_Parser = yggr::task_center::support::task_owner_info_parser,
			template<typename _Data_Info>
				class Task_Data_Info_Parser = yggr::task_center::support::task_data_info_parser>
class task
{
public:
	typedef TaskID id_type;
	typedef OwnerInfo owner_info_type;
	typedef TaskDataInfo task_data_info_type;
	typedef DataContainer data_container_type;

	typedef Task_Inner_Data_Parser<data_container_type> inner_data_parser_type;
	typedef Task_Owner_Info_Parser<owner_info_type> owner_info_parser_type;
	typedef Task_Data_Info_Parser<task_data_info_type> data_info_parser_type;

	typedef typename owner_info_parser_type::owner_id_type owner_id_type;
	typedef typename data_info_parser_type::data_id_type data_id_type;
	typedef typename data_info_parser_type::data_ver_type data_ver_type;
	typedef typename data_info_parser_type::data_info_type data_info_type;
	typedef typename data_info_parser_type::class_info_type class_info_type;
	typedef typename data_info_parser_type::class_name_type class_name_type;

private:
	typedef task this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// id getters
	typedef detail::task_inner_task_id_getter<id_type> task_id_getter;
	typedef detail::task_inner_owner_info_getter<owner_info_type> owner_info_getter;
	typedef detail::task_inner_owner_id_getter<owner_id_type> owner_id_getter;
	typedef detail::task_inner_data_info_getter<data_info_type> data_info_getter;
	typedef detail::task_inner_data_id_getter<data_id_type> data_id_getter;
	typedef detail::task_inner_data_ver_getter<data_ver_type> data_ver_getter;
	typedef detail::task_inner_class_info_getter<class_info_type> class_info_getter;
	typedef detail::task_inner_class_name_getter<class_name_type> class_name_getter;

public:
	task(void)
		: _id(),
			_owner_info(),
			_data_info(),
			_cal_type(task_cal_type::E_CAL_FAST)
	{
	}

	task(const owner_info_type& owner_info,
			const data_info_type& data_info,
			u32 cal_type, const data_container_type& data)
		: _id(),
			_owner_info(owner_info),
			_data_info(data_info),
			_cal_type(cal_type),
			_data_cont(data)
	{
	}

	task(const id_type& id,
			const owner_info_type& owner_info,
			const data_info_type& data_info,
			u32 cal_type, const data_container_type& data)
		: _id(id),
			_owner_info(owner_info),
			_data_info(data_info),
			_cal_type(cal_type),
			_data_cont(data)
	{
	}

	template<typename Data>
	task(const owner_info_type& owner_info, const Data& data)
		: _id(),
			_owner_info(owner_info),
			_data_info(data_info_parser_type::template data_info<Data>()),
			_cal_type(data.cal_type()),
			_data_cont(data)
	{
	}

	template<typename Data>
	task(const id_type& id, const owner_info_type& owner_info, const Data& data)
		: _id(id),
			_owner_info(owner_info),
			_data_info(data_info_parser_type::template data_info<Data>()),
			_cal_type(data.cal_type()),
			_data_cont(data)
	{
	}

	task(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id)),
			_owner_info(boost::move(right._owner_info)),
			_data_info(boost::move(right._data_info)),
			_cal_type(boost::move(right._cal_type)),
			_data_cont(boost::move(right._data_cont))
	{
	}

	task(const this_type& right)
		: _id(right._id),
			_owner_info(right._owner_info),
			_data_info(right._data_info),
			_cal_type(right._cal_type),
			_data_cont(right._data_cont)
	{
	}

	~task(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		copy_or_move_or_swap(_id, boost::move(right_ref._id));
		copy_or_move_or_swap(_owner_info, boost::move(right_ref._owner_info));
		copy_or_move_or_swap(_data_info, boost::move(right_ref._data_info));
		copy_or_move_or_swap(_cal_type, boost::move(right_ref._cal_type));
		copy_or_move_or_swap(_data_cont, boost::move(right_ref._data_cont));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		_id = right._id;
		_owner_info = right._owner_info;
		_data_info = right._data_info;
		_cal_type = right._cal_type;
		_data_cont = right._data_cont;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(_id, right._id);
		yggr::swap(_owner_info, right._owner_info);
		yggr::swap(_data_info, right._data_info);
		yggr::swap(_cal_type, right._cal_type);
		_data_cont.swap(right._data_cont);
	}

public:
	
	inline bool empty(void) const
	{
		return _data_cont.empty();
	}

	inline id_type copy_task_id(void) const
	{
		return _id;
	}

	inline id_type& task_id(void)
	{
		return _id;
	}

	inline const id_type& task_id(void) const
	{
		return _id;
	}

	inline data_info_type copy_data_info(void) const
	{
		return _data_info;
	}

	inline const data_info_type& data_info(void) const
	{
		return _data_info;
	}

	inline data_id_type copy_data_id(void) const
	{
		return data_info_parser_type::data_id(_data_info);
	}

	inline const data_id_type& data_id(void) const
	{
		return data_info_parser_type::data_id(_data_info);
	}

	inline data_ver_type copy_data_ver(void) const
	{
		return data_info_parser_type::data_ver(_data_info);
	}

	inline const data_ver_type& data_ver(void) const
	{
		return data_info_parser_type::data_ver(_data_info);
	}

	inline owner_info_type copy_owner_info(void) const
	{
		return _owner_info;
	}

	inline const owner_info_type& owner_info(void) const
	{
		return _owner_info;
	}

	inline owner_id_type copy_owner_id(void) const
	{
		return owner_info_parser_type::owner_id(_owner_info);
	}

	inline owner_id_type owner_id(void) const
	{
		return owner_info_parser_type::owner_id(_owner_info);
	}

	//inline const class_info_type& class_info(void) const
	//{
	//	return _data_cont.type();
	//}

	inline class_info_type class_info(void) const
	{
		return _data_cont.type();
	}

	inline class_name_type class_name(void) const
	{
		class_name_type cname = _data_cont.type().name();
		return cname;
	}

	inline u32 cal_type(void) const
	{
		return _cal_type;
	}

	template<typename T> inline
	const T* get(void) const
	{
		return inner_data_parser_type::template inner_data<T>(_data_cont);
	}

	template<typename T> inline
	T* get(void)
	{
		return inner_data_parser_type::template inner_data<T>(_data_cont);
	}

	inline const std::type_info& type(void) const
	{
		return _data_cont.type();
	}

private:
	id_type _id;
	owner_info_type _owner_info;
	data_info_type _data_info;
	u32 _cal_type;
	data_container_type _data_cont;
};

// operator==
template<typename Task_ID, typename Owner_Info, 
			typename TaskDataInfo, typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser> inline
bool operator==(const Task_ID& l, 
					const 
						task
						<
							Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
							Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
						>& r)
{
	return l == r.task_id();
}

template<typename Task_ID, typename Owner_Info, 
			typename TaskDataInfo, typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser> inline
bool operator==(const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Owner_Info_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& l,
				const Task_ID& r)
{
	return l.task_id() == r;
}

template<typename Task_ID, typename Owner_Info, 
			typename TaskDataInfo, typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser> inline
bool operator==(const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& l,
				const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& r)
{
	return (boost::addressof(l) == boost::addressof(r)) || (l.task_id() == r.task_id());
}

// operator !=
template<typename Task_ID, typename Owner_Info, 
			typename TaskDataInfo, typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser> inline
bool operator!=(const Task_ID& l, 
				const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& r)
{
	return !(l == r.task_id());
}

template<typename Task_ID, typename Owner_Info, 
			typename TaskDataInfo, typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser> inline
bool operator!=(const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& l,
				const Task_ID& r)
{
	return !(l.task_id() == r);
}

template<typename Task_ID, typename Owner_Info, 
			typename TaskDataInfo, typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser> inline
bool operator!=(const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& l,
				const 
					task
					<
						Task_ID, Owner_Info, TaskDataInfo, DataContainer, 
						Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser
					>& r)
{
	return !((boost::addressof(l) == boost::addressof(r)) || (l.task_id() == r.task_id()));
}

} //namespace task_center
} //namespace yggr

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS

#	define _YGGR_PP_CONCAT_PARAMS() YGGR_PP_CONCAT(Task_Inner_Data_Parser, Task_Owner_Info_Parser, Task_Data_Info_Parser)

#else

#	define _YGGR_PP_CONCAT_ARGS() \
		Task_Inner_Data_Parser YGGR_PP_SYMBOL_COMMA() \
		Task_Owner_Info_Parser YGGR_PP_SYMBOL_COMMA() \
		Task_Data_Info_Parser

#	define _YGGR_PP_CONCAT_PARAMS() YGGR_PP_CONCAT(_YGGR_PP_CONCAT_ARGS)

#endif //YGGR_NO_CXX11_VARIADIC_MACROS

namespace yggr
{
namespace task_center
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(4, typename T), 
			template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1) > class Task_Inner_Data_Parser, 
			template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1) > class Task_Owner_Info_Parser, 
			template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2) > class Task_Data_Info_Parser > inline 
void swap(YGGR_PP_TEMPLATE_TYPE2(task, YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), _YGGR_PP_CONCAT_PARAMS())& l, 
			YGGR_PP_TEMPLATE_TYPE2(task, YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), _YGGR_PP_CONCAT_PARAMS())& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace task_center
} // namespace yggr

namespace std
{
	using ::yggr::task_center::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::task_center::swap_support::swap;
} // namespace boost

#undef _YGGR_PP_CONCAT_PARAMS

#ifndef __VAR_ARGS__
#undef _YGGR_PP_CONCAT_ARGS
#endif // __VAR_ARGS__

#include <yggr/task_center/support/task_real_data_getter.hpp>

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename Task_ID,
			typename Owner_Info,
			typename TaskDataInfo,
			typename DataContainer,
			template<typename _DataContainer> class Task_Inner_Data_Parser,
			template<typename _Owner_Info> class Task_Owner_Info_Parser,
			template<typename _Data_Info> class Task_Data_Info_Parser>
struct task_real_data_getter
		<
			yggr::task_center::task
			<
				Task_ID,
				Owner_Info,
				TaskDataInfo,
				DataContainer,
				Task_Inner_Data_Parser,
				Task_Owner_Info_Parser,
				Task_Data_Info_Parser
			>
		>
{
	typedef 
		yggr::task_center::task
		<
			Task_ID,
			Owner_Info,
			TaskDataInfo,
			DataContainer,
			Task_Inner_Data_Parser,
			Task_Owner_Info_Parser,
			Task_Data_Info_Parser
		> task_type;

	template<typename Real_Data> inline
	Real_Data* operator()(task_type& right) const
	{
		return right.template get<Real_Data>();
	}

	template<typename Real_Data> inline
	const Real_Data* operator()(const task_type& right) const
	{
		return right.template get<Real_Data>();
	}
};

} // namespace support
} // namespace task_center
} // namespace yggr

#endif // __YGGR_DATA_CENTR_TASK_HPP__
