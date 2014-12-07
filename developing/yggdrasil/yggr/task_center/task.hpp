//task.hpp

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

#ifndef __YGGR_DATA_CENTER_TASK_HPP__
#define __YGGR_DATA_CENTER_TASK_HPP__

#include <boost/any.hpp>
#include <yggr/move/move.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/concat.hpp>
#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_owner_info_parser.hpp>
#include <yggr/task_center/support/task_data_info_parser.hpp>

#include <cassert>

namespace yggr
{
namespace task_center
{

template<typename Task_ID,
			typename Owner_Info,
			typename Task_Data_Info,
			typename DataContainer = boost::any,
			template<typename _Owner_Info>
				class Task_Owner_Info_Parser = yggr::task_center::support::task_owner_info_parser,
			template<typename _Data_Info>
				class Task_Data_Info_Parser = yggr::task_center::support::task_data_info_parser>
class task
{
public:
	typedef Task_ID id_type;
	typedef Owner_Info owner_info_type;
	typedef Task_Data_Info task_data_info_type;

	typedef Task_Owner_Info_Parser<owner_info_type> owner_info_parser_type;
	typedef Task_Data_Info_Parser<task_data_info_type> data_info_parser_type;

	//typedef typename owner_info_parser_type::id_type owner_info_id_type;
	typedef typename owner_info_parser_type::owner_id_type owner_id_type;
	typedef typename data_info_parser_type::data_id_type data_id_type;
	typedef typename data_info_parser_type::data_ver_type data_ver_type;
	typedef typename data_info_parser_type::data_info_type data_info_type;
	typedef typename data_info_parser_type::class_info_type class_info_type;
	typedef typename data_info_parser_type::class_name_type class_name_type;

	typedef DataContainer data_container_type;

private:

	typedef task this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// id getters
	struct task_id_getter
	{
		typedef const id_type& result_type;
		const id_type& operator()(const task& right) const
		{
			return right.task_id();
		}
	};

	struct owner_info_getter
	{
		typedef const owner_info_type& result_type;
		const owner_info_type operator()(const task& right) const
		{
			return right.owner_info();
		}
	};

	struct owner_id_getter
	{
		typedef const owner_id_type result_type;
		const owner_id_type operator()(const task& right) const
		{
			return right.owner_id();
		}
	};

	struct data_info_getter
	{
		typedef const data_info_type& result_type;
		const data_info_type& operator()(const task& right) const
		{
			return right.data_info();
		}
	};

	struct data_id_getter
	{
		typedef const data_id_type& result_type;
		const data_id_type& operator()(const task& right) const
		{
			return right.data_id();
		}
	};

	struct data_ver_getter
	{
		typedef const data_ver_type& result_type;
		const data_ver_type& operator()(const task& right) const
		{
			return right.data_ver();
		}
	};

	struct class_info_getter
	{
		typedef const class_info_type& result_type;
		const class_info_type& operator()(const task& right) const
		{
			return right.class_info();
		}

		template<typename Other>
		const class_info_type& operator()(const Other& other) const
		{
			return typeid(Other);
		}

		template<typename Other>
		const class_info_type& get(void) const
		{
			return typeid(Other);
		}
	};

	struct class_name_getter
	{
		typedef class_name_type result_type;
		class_name_type operator()(const task& right) const
		{
			return right.class_name();
		}

		template<typename Other>
		class_name_type operator()(const Other& other) const
		{
			return typeid(Other).name();
		}

		template<typename Other>
		class_name_type get(void) const
		{
			return typeid(Other).name();
		}
	};

public:
	task(void)
		: _id(),
			_owner_info(),
			_data_info(),
			_cal_type(task_cal_type::E_CAL_FAST)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	task(const owner_info_type& owner_info,
			const data_info_type& data_info,
			u32 cal_type, BOOST_RV_REF(data_container_type) data)
		: _id(),
			_owner_info(owner_info),
			_data_info(data_info),
			_cal_type(cal_type),
			_data_cont(boost::forward<data_container_type>(data))
	{
	}

	task(const id_type& id,
			const owner_info_type& owner_info,
			const data_info_type& data_info,
			u32 cal_type, BOOST_RV_REF(data_container_type) data)
		: _id(id),
			_owner_info(owner_info),
			_data_info(data_info),
			_cal_type(cal_type),
			_data_cont(boost::forward<data_container_type>(data))
	{
	}
#else
	task(const owner_info_type& owner_info,
			const data_info_type& data_info,
			u32 cal_type, BOOST_RV_REF(data_container_type) data)
		: _id(),
			_owner_info(owner_info),
			_data_info(data_info),
			_cal_type(cal_type)
	{
		data_container_type& cont = data;
		_data_cont.swap(cont);
	}

	task(const id_type& id,
			const owner_info_type& owner_info,
			const data_info_type& data_info,
			u32 cal_type, BOOST_RV_REF(data_container_type) data)
		: _id(id),
			_owner_info(owner_info),
			_data_info(data_info),
			_cal_type(cal_type)
	{
		data_container_type& cont = data;
		_data_cont.swap(cont);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

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

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	task(BOOST_RV_REF(this_type) right)
		: _id(boost::forward<id_type>(right._id)),
			_owner_info(boost::forward<owner_info_type>(right._owner_info)),
			_data_info(boost::forward<data_info_type>(right._data_info)),
			_cal_type(boost::forward<u32>(right._cal_type)),
			_data_cont(boost::forward<data_container_type>(right._data_cont))
	{
	}
#else
	task(BOOST_RV_REF(this_type) right)
		: _id(),
			_owner_info(),
			_data_info(),
			_cal_type(),
			_data_cont()
	{
	    this_type& right_ref = right;
        this_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

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

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
		_owner_info = boost::forward<owner_info_type>(right._owner_info);
		_data_info = boost::forward<data_info_type>(right._data_info);
		_cal_type = boost::forward<u32>(right._cal_type);
		_data_cont = boost::forward<data_container_type>(right._data_cont);
#else
        this_type& right_ref = right;
        this_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_id = right._id;
		_owner_info = right._owner_info;
		_data_info = right._data_info;
		_cal_type = right._cal_type;
		_data_cont = right._data_cont;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//boost::swap(_id, boost::forward<id_type>(right._id));
		//boost::swap(_owner_info, boost::forward<owner_info_type>(right._owner_info));
		//boost::swap(_data_info, boost::forward<data_info_type>(right._data_info));
		//boost::swap(_cal_type, boost::forward<u32>(right._cal_type));
		//_data_cont.swap(boost::forward<data_container_type>(right._data_cont));

		boost::swap(_id, right._id);
		boost::swap(_owner_info, right._owner_info);
		boost::swap(_data_info, right._data_info);
		boost::swap(_cal_type, right._cal_type);
		_data_cont.swap(right._data_cont);
#else
		this_type& right_ref = right;
		this_type::swap(right_ref);

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		boost::swap(_id, right._id);
		//_owner_info.swap(right._owner_info);
		boost::swap(_owner_info, right._owner_info);
		boost::swap(_data_info, right._data_info);
		boost::swap(_cal_type, right._cal_type);
		_data_cont.swap(right._data_cont);
	}

	bool empty(void) const
	{
		return _data_cont.empty();
	}

	id_type copy_task_id(void) const
	{
		return _id;
	}

	id_type& task_id(void)
	{
		return _id;
	}

	const id_type& task_id(void) const
	{
		return _id;
	}

	data_info_type copy_data_info(void) const
	{
		return _data_info;
	}

	const data_info_type& data_info(void) const
	{
		return _data_info;
	}

	data_id_type copy_data_id(void) const
	{
		return data_info_parser_type::data_id(_data_info);
	}

	const data_id_type& data_id(void) const
	{
		return data_info_parser_type::data_id(_data_info);
	}

	data_ver_type copy_data_ver(void) const
	{
		return data_info_parser_type::data_ver(_data_info);
	}

	const data_ver_type& data_ver(void) const
	{
		return data_info_parser_type::data_ver(_data_info);
	}

	owner_info_type copy_owner_info(void) const
	{
		return _owner_info;
	}

	const owner_info_type& owner_info(void) const
	{
		return _owner_info;
	}

	owner_id_type copy_owner_id(void) const
	{
		return owner_info_parser_type::owner_id(_owner_info);
	}

	const owner_id_type owner_id(void) const
	{
		return owner_info_parser_type::owner_id(_owner_info);
	}

	const class_info_type& class_info(void) const
	{
		return _data_cont.type();
	}

	class_name_type class_name(void) const
	{
		return class_name_type(_data_cont.type().name());
	}

	const u32 cal_type(void) const
	{
		return _cal_type;
	}

	bool operator==(const id_type& id) const
	{
		return _id == id;
	}

	bool operator==(const this_type& right) const
	{
		return _id == right._id;
	}

	template<typename T>
	const T* get(void) const
	{
		assert(typeid(T) == _data_cont.type());
		return boost::any_cast<T>(&_data_cont);
	}

	template<typename T>
	T* get(void)
	{
		assert(typeid(T) == _data_cont.type());
		return boost::any_cast<T>(&_data_cont);
	}

	const std::type_info& type(void) const
	{
		return _data_cont.type();
	}

//private:
//	data_info_type make_tid(void) const
//	{
//		return task_data_info_type::make_cmp(_did, _dver);
//	}

private:
	id_type _id;
	owner_info_type _owner_info;
	data_info_type _data_info;
	u32 _cal_type;
	data_container_type _data_cont;
};

} //namespace task_center
} //namespace yggr

#define _YGGR_PP_TASK_CENTER_TASK_SWAP() \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(4, typename T), \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1) > class Task_Owner_Info_Parser, \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2) > class Task_Data_Info_Parser > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::task_center::task, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										YGGR_PP_CONCAT(Task_Owner_Info_Parser, Task_Data_Info_Parser))& l, \
				YGGR_PP_TEMPLATE_TYPE2(yggr::task_center::task, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										YGGR_PP_CONCAT(Task_Owner_Info_Parser, Task_Data_Info_Parser))& r) { \
		l.swap(r); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(4, typename T), \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1) > class Task_Owner_Info_Parser, \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2) > class Task_Data_Info_Parser > inline \
	void swap(BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::task_center::task, \
													YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
													YGGR_PP_CONCAT(Task_Owner_Info_Parser, Task_Data_Info_Parser))) l, \
				YGGR_PP_TEMPLATE_TYPE2(yggr::task_center::task, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										YGGR_PP_CONCAT(Task_Owner_Info_Parser, Task_Data_Info_Parser))& r) { \
		r.swap(l); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(4, typename T), \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1) > class Task_Owner_Info_Parser, \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2) > class Task_Data_Info_Parser > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::task_center::task, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										YGGR_PP_CONCAT(Task_Owner_Info_Parser, Task_Data_Info_Parser))& l, \
				BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::task_center::task, \
														YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
														YGGR_PP_CONCAT(Task_Owner_Info_Parser, Task_Data_Info_Parser))) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_TASK_CENTER_TASK_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_TASK_CENTER_TASK_SWAP()
} // namespace boost

#undef _YGGR_PP_TASK_CENTER_TASK_SWAP

#include <yggr/task_center/support/task_real_data_getter.hpp>

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename Task_ID,
			typename Owner_Info,
			typename Task_Data_Info,
			typename DataContainer,
			template<typename _Owner_Info>
				class Task_Owner_Info_Parser,
			template<typename _Data_Info>
				class Task_Data_Info_Parser>
struct task_real_data_getter
		<
			yggr::task_center::task
			<
				Task_ID,
				Owner_Info,
				Task_Data_Info,
				DataContainer,
				Task_Owner_Info_Parser,
				Task_Data_Info_Parser
			>
		>
{
	typedef yggr::task_center::task
			<
				Task_ID,
				Owner_Info,
				Task_Data_Info,
				DataContainer,
				Task_Owner_Info_Parser,
				Task_Data_Info_Parser
			> task_type;

	template<typename Real_Data>
	Real_Data* operator()(task_type& right) const
	{
		return right.template get<Real_Data>();
	}

	template<typename Real_Data>
	const Real_Data* operator()(const task_type& right) const
	{
		return right.template get<Real_Data>();
	}
};

} // namespace support
} // namespace task_center
} // namespace yggr

#endif // __YGGR_DATA_CENTR_TASK_HPP__
