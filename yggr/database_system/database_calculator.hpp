//database_calcualtor.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CALCULATOR_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CALCULATOR_HPP__

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

#include <yggr/base/error_make.hpp>

#include <yggr/database_system/database_mgr.hpp>
#include <yggr/database_system/database_command.hpp>
#include <yggr/thread/action_table.hpp>

namespace yggr
{
namespace database_system
{

template<typename OwnerInfo, typename Command, typename DB_OP,
	typename OwnerInfo_Container = boost::unordered_multiset<OwnerInfo>,
	template<typename _DB_OP> class Base = database_mgr>
class database_calculator
	: public boost::enable_shared_from_this<database_calculator<OwnerInfo, Command, DB_OP, OwnerInfo_Container, Base> >,
	public Base<DB_OP>
{

public:
	ERROR_MAKER_BEGIN("database_calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

public:
	typedef OwnerInfo owner_info_type;
	typedef OwnerInfo_Container owner_info_container_type;
	typedef Command command_type;
	typedef DB_OP db_op_type;
	typedef typename db_op_type::conn_type conn_type;
	typedef Base<db_op_type> base_type;

protected:
	typedef yggr::ref_count_info<> ref_count_info_type;
	typedef yggr::ref_count_info_lock<ref_count_info_type> ref_count_info_lock_type;

private:
    typedef database_calculator this_type;

public:

	explicit database_calculator(const std::string& link_str, bool b_prompt = false, bool b_auto_commit = false)
		: base_type(link_str, b_prompt, b_auto_commit)
	{
	}

	~database_calculator(void)
	{
	}

#ifndef YGGR_USE_SEH
	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

        at.template register_calculator<command_type, recv_handler_type>(
				handler, boost::bind(&this_type::cal_db_command_type<owner_info_type,
																		runner_type,
																		recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4));

	}
#else
	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

        at.template register_calculator<command_type, recv_handler_type>(
				handler, boost::bind(&this_type::cal_db_command_type<owner_info_type,
																		runner_type,
																		recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4),
							boost::bind(&this_type::handler_seh<owner_info_type, command_type>,
											shared_from_this(), _1, _2) );

	}
#endif // YGGR_USE_SEH

	template<typename Owner, typename Runner, typename Handler>
	void cal_db_command_type(const Owner& uid, const command_type& cdt, Runner* prunner, const Handler& handler)
	{
		ref_count_info_lock_type lk(_count);
		base_type::execute_view(cdt.id(), cdt.get_tb_names(), cdt.get_postfix_str());
	}

#ifdef YGGR_USE_SEH
protected:
	template<typename Owner_Info, typename Value>
	void handler_seh(Owner_Info owner_info, const Value* pval)
	{
		ref_count_info_lock_type lk(_count);
	}
#endif //YGGR_USE_SEH

protected:
	mutable ref_count_info_type _count;
};

} // namespace database_system
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_CALCULATOR_HPP__
