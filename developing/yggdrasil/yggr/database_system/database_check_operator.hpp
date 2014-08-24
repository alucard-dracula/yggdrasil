//database_check_operator.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CHECK_OPERATOR_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CHECK_OPERATOR_HPP__

#include <yggr/charset/string.hpp>
#include <vector>
namespace yggr
{
namespace database_system
{

class database_check_operator
{
public:
	template<typename View_Struct>
	static const std::string make_database_command_id(const std::string& op_str)
	{
		typedef View_Struct view_struct_type;
		std::stringstream ss;
		ss << op_str << ", " << typeid(view_struct_type).name();
		return ss.str();
	}

	static const std::string make_database_command_id(const std::string& op_str, const std::string& type_str)
	{
		std::stringstream ss;
		ss << op_str << ", " << type_str;
		return ss.str();
	}

	static const std::string make_table_names(const std::vector<std::string >& tb_names)
	{
		std::stringstream ss;
		size_type isize = tb_names.size();
		for(size_type i = 0; i != isize; ++i)
		{
			ss << tb_names[i] << (((i + 1) == isize)? "" : ", ");
		}
		
		return ss.str();
	}
};

} // namesapce database_system
} // namesapce

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_CHECK_OPERATOR_HPP__
