//database_command.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_COMMAND_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_COMMAND_HPP__

#include <vector>

#include <yggr/base/yggrdef.h>
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

//public:
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

public:
	database_command(void)
	{
	}

	database_command(const std::string& op_str, const std::string& type_str, const std::vector<std::string>& tb_names, const std::string& postfix_str)
		: _op_str(op_str), _type_str(type_str), _tb_names(tb_names), _postfix_str(postfix_str)
	{
	}

	database_command(const database_command& dbc)
		: _op_str(dbc._op_str), _type_str(dbc._type_str), _tb_names(dbc._tb_names), _postfix_str(dbc._postfix_str)
	{
	}

	~database_command(void)
	{
		_tb_names.clear();
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

//#include <yggr/task_center/support/task_import.ipp>
//#include <yggr/support/condition_fast.ipp>
	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	std::string _op_str;
	std::string _type_str;

	std::vector<std::string> _tb_names;
	std::string _postfix_str;
};

} // namespace database_system
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_COMMAND_HPP__
