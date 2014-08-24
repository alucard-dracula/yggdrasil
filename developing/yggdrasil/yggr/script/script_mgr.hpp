//script_mgr.hpp

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

#ifndef __YGGR_SCRIPT_SCRIPT_MANAGER_HPP__
#define __YGGR_SCRIPT_SCRIPT_MANAGER_HPP__

//#include <boost/unordered_map.hpp>
#include <cassert>
#include <boost/shared_ptr.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace script
{

template<typename Rule_ID, typename Script, typename Environment>
class script_mgr : public Environment, private nonable::noncopyable
{
private:
	typedef Environment base_type;
	typedef Rule_ID rule_id_type;
	typedef Script script_type;

	typedef boost::shared_ptr<script_type> script_ptr_type;
	typedef safe_container::safe_unordered_map<rule_id_type, script_ptr_type> script_map_type;

	typedef typename script_map_type::iterator script_iter_type;
	typedef typename script_map_type::const_iterator script_c_iter_type;

	typedef script_mgr this_type;

public:
	typedef typename script_type::string_code string_code_type;
	typedef typename script_type::file_code file_code_type;

public:
	script_mgr(void)
	{
	}

	~script_mgr(void)
	{
		clear();
	}

	/*template<typename Code>
	bool insert(const std::string& rule_name, const std::string& enter_foo, const Code& code)
	{
		script_ptr_type ptr(new script_type(enter_foo, code, base_type::get_grobal_environment()));
		return ptr && ptr->success() && _script_map.insert(rule_name, ptr);
	}*/

	template<typename Code>
	bool insert(const rule_id_type& rule_id, const std::string& enter_foo, const Code& code)
	{
		script_ptr_type ptr(new script_type(enter_foo, code, base_type::get_grobal_environment()));
		return ptr && ptr->success() && _script_map.insert(rule_id, ptr);
		//return false;
	}

	bool execute_rule(const rule_id_type& id)
	{
		return _script_map.use_handler(id, boost::bind(&this_type::handler_execute_rule, this, _1));
	}

	template<typename Input_Real_Data>
	bool execute_rule(const rule_id_type& id, const Input_Real_Data& in)
	{
		typedef Input_Real_Data input_real_data_type;

		return _script_map.use_handler(id, boost::bind(&this_type::handler_execute_rule_in_only<input_real_data_type>,
														this, _1, boost::cref(in)));
		//return false;
	}

	template<typename Output_Real_Data>
	bool execute_rule(const rule_id_type& id, Output_Real_Data& out)
	{
		typedef Output_Real_Data output_real_data_type;

		return _script_map.use_handler(id, boost::bind(&this_type::handler_execute_rule_out_only<output_real_data_type>,
															this, _1, boost::ref(out)));
		//return false;
	}

	template<typename Output_Real_Data, typename Input_Real_Data>
	bool execute_rule(const rule_id_type& id, const Input_Real_Data& in, Output_Real_Data& out)
	{
		typedef Input_Real_Data input_real_data_type;
		typedef Output_Real_Data output_real_data_type;
		return _script_map.use_handler(id,
										boost::bind(&this_type::handler_execute_rule<input_real_data_type, output_real_data_type>,
													this, _1, boost::cref(in), boost::ref(out)));
		//return false;
	}

	bool remove(const std::string& rule_name)
	{
		return _script_map.erase(rule_name);
	}

	void clear(void)
	{
		_script_map.clear();
	}

private:
	void handler_execute_rule(script_iter_type& iter)
	{
		assert(iter->second);
		iter->second->execute_rule_no_ret();
	}

	template<typename Input_Real_Data>
	void handler_execute_rule_in_only(script_iter_type& iter, const Input_Real_Data& in)
	{
		typedef Input_Real_Data input_real_data_type;
		assert(iter->second);
		iter->second->execute_rule_no_ret(in);
	}

	template<typename Output_Real_Data>
	void handler_execute_rule_out_only(script_iter_type& iter, Output_Real_Data& out)
	{
		typedef Output_Real_Data output_real_data_type;
		assert(iter->second);
		out = iter->second->template execute_rule<output_real_data_type>();
	}

	template<typename Input_Real_Data, typename Output_Real_Data>
	void handler_execute_rule(script_iter_type& iter, const Input_Real_Data& in, Output_Real_Data& out)
	{
		//typedef Input_Real_Data input_real_data_type;
		typedef Output_Real_Data output_real_data_type;

		assert(iter->second);
		out = iter->second->template execute_rule<output_real_data_type>(in);
	}

private:
	script_map_type _script_map;
};

} // namespace script
} // namespace yggr



#endif //__YGGR_SCRIPT_SCRIPT_MANAGER_HPP__

