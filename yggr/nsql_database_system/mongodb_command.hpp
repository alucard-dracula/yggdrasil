//mongodb_command.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_COMMAND_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGODB_COMMAND_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/any_val/any.hpp>

#include <yggr/nsql_database_system/mongodb_pak_id_def.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename Key, typename InUserData = ::yggr::any,
			typename TaskDataInfo = task_center::default_task_data_info_type >
class mongodb_command
{
public:
	typedef Key key_type;
	typedef InUserData in_user_data_type;

	typedef TaskDataInfo task_data_info_type;

private:
	typedef mongodb_command this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = mongodb_pak_id_def::E_id_start + 1
	// E_now_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (mongodb_pak_id_def::E_id_start + 1), 1)

private:
	YGGR_PP_CHECK_MONGODB_PACKET_ID(E_pak_id);

public:
	mongodb_command(void)
		: _key(), _in_user_data()
	{
	}

	mongodb_command(const key_type& key)
		: _key(key), _in_user_data()
	{
	}

	mongodb_command(const key_type& key, const in_user_data_type& user_data)
		: _key(key), _in_user_data(user_data)
	{
	}

	mongodb_command(BOOST_RV_REF(this_type) right)
		: _key(boost::move(right._key)),
			_in_user_data(boost::move(right._in_user_data))
	{
	}

	mongodb_command(const this_type& right)
		: _key(right._key), _in_user_data(right._in_user_data)
	{
	}

	~mongodb_command(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_key, boost::move(right._key));
		copy_or_move_or_swap(_in_user_data, boost::move(right._in_user_data));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_key = right._key;
		_in_user_data = right._in_user_data;
		return *this;
	}

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

		yggr::swap(_key, right._key);
		yggr::swap(_in_user_data, right._in_user_data);
	}

public:
	inline const key_type& id(void) const
	{
		return _key;
	}

	inline const in_user_data_type& in_user_data(void) const
	{
		return _in_user_data;
	}

private:
	key_type _key;
	in_user_data_type _in_user_data;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

template<typename Key, typename InUserData,
		typename PakID, typename Ver, 
		typename TaskDataInfo > inline 
void swap(mongodb_command<Key, InUserData, PakID, Ver, TaskDataInfo>& l, 
			mongodb_command<Key, InUserData, PakID, Ver, TaskDataInfo>& r) 
{ 
	l.swap(r);
}


} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGODB_COMMAND_HPP__
