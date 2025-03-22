//database_command.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_COMMAND_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_COMMAND_HPP__

#include <vector>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/database_system/database_check_operator.hpp>
#include <yggr/database_system/database_pak_id_def.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>

namespace yggr
{
namespace database_system
{

template<typename Pak_ID = u32, typename Ver = u16,
			typename Task_Data_Info = task_center::default_task_data_info_type>
class database_command
{

public:
	typedef database_command this_type;
	typedef Pak_ID pak_id_type;
	typedef Ver pak_ver_type;
	typedef Task_Data_Info task_data_info_type;

	typedef std::vector<std::string> tb_names_type;
private:
	enum
	{
		E_pak_id_start = database_pak_id_def::E_pak_id_start,
		E_pak_id = E_pak_id_start + 1,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

private:
	typedef database_command this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	database_command(void)
	{
	}

	database_command(const std::string& op_str, const std::string& type_str, 
						const std::vector<std::string>& tb_names, const std::string& postfix_str)
		: _op_str(op_str), _type_str(type_str), _tb_names(tb_names), _postfix_str(postfix_str)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	database_command(BOOST_RV_REF(this_type) right)
		: _op_str(boost::forward<std::string>(right._op_str)),
			_type_str(boost::forward<std::string>(right._type_str)),
			_tb_names(boost::forward<tb_names_type>(right._tb_names)),
			_postfix_str(boost::forward<std::string>(right._postfix_str))


	{
	}
#else
	database_command(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_op_str.swap(right_ref._op_str);
		_type_str.swap(right_ref._type_str);
		_tb_names.swap(right_ref._tb_names);
		_postfix_str.swap(right_ref._postfix_str);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	database_command(const this_type& right)
		: _op_str(right._op_str), _type_str(right._type_str), 
			_tb_names(right._tb_names), _postfix_str(right._postfix_str)
	{
	}

	~database_command(void)
	{
		_tb_names.clear();
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_op_str = boost::forward<std::string>(right._op_str);
		_type_str = boost::forward<std::string>(right._type_str);
		_tb_names = boost::forward<tb_names_type>(right._tb_names);
		_postfix_str = boost::forward<std::string>(right._postfix_str);
#else
		this_type& right_ref = right;
		_op_str.swap(right_ref._op_str);
		_type_str.swap(right_ref._type_str);
		_tb_names.swap(right_ref._tb_names);
		_postfix_str.swap(right_ref._postfix_str);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_op_str = right._op_str;
		_type_str = right._type_str;
		_tb_names = right._tb_names;
		_postfix_str = right._postfix_str;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		_op_str.swap(right._op_str);
		_type_str.swap(right._type_str);
		_tb_names.swap(right._tb_names);
		_postfix_str.swap(right._postfix_str);
	}

	const std::string id(void) const
	{
		return database_check_operator::make_database_command_id(_op_str, _type_str);
	}

	const std::vector<std::string>& get_tb_names(void) const
	{
		return _tb_names;
	}

	const std::string& get_postfix_str(void) const
	{
		return _postfix_str;
	}

	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	std::string _op_str;
	std::string _type_str;

	tb_names_type _tb_names;
	std::string _postfix_str;
};

} // namespace database_system
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_COMMAND_HPP__
