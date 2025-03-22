//python_mgr.hpp

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

#ifndef __YGGR_SCRIPT_PYTHON_PYTHON_MANAGER_HPP__
#define __YGGR_SCRIPT_PYTHON_PYTHON_MANAGER_HPP__

#include <boost/unordered_map.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace yggr
{
namespace script
{

template<typename Rule_ID, typename Script, typename Environment>
class script_mgr : public Environment, private boost::noncopyable
{
	typedef Environment base_type;
	typedef Rule_ID rule_id_type;
	typedef Script script_type;
	typedef boost::shared_ptr<script_type> script_ptr_type;

	typedef boost::unordered_map<rule_id_type, script_ptr_type> script_map_type;

	typedef typename script_map_type::iterator script_iter_type;
	typedef typename script_map_type::const_iterator script_c_iter_type;
	typedef typename script_map_type::value_type script_map_val_type;
	typedef std::pair<script_iter_type, bool> script_map_ins_rst_type;


public:
	script_mgr(void)
	{
	}

	~script_mgr(void)
	{
		clear();
	}

	bool insert(const std::string& rule_name, const std::string& code, const std::string& enter_foo)
	{
		return _script_map.insert(script_map_val_type(rule_name, script_type(code, enter_foo, base_type::get_grobal_environment()))).second;
	}

	bool insert(const std::string& rule_name, const script_type& script)
	{
		return _script_map.insert(script_map_val_type(rule_name, script)).second;
	}

	template<typename Output_Real_Data, typename Input_Real_Data>
	bool execute_rule(const rule_id_type& id, const Input_Real_Data& in, Output_Real_Data& out)
	{
		typedef Output_Real_Data output_real_data_type;
		script_iter_type iter = _script_map.find(id);
		if(iter == _script_map.end())
		{
			return false;
		}

		out = output_real_data_type(iter->second.template execute_rule<output_real_data_type>(boost::cref(in)));
		return true;
	}

	template<typename Input_Real_Data>
	bool execute_rule(const rule_id_type& id, const Input_Real_Data& in)
	{
		script_iter_type iter = _script_map.find(id);
		if(iter == _script_map.end())
		{
			return false;
		}

		iter->second.template execute_rule_no_ret(boost::cref(in));
		return true;
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
	script_map_type _script_map;
};

} // namespace script
} // namespace yggr



#endif //__YGGR_SCRIPT_PYTHON_PYTHON_MANAGER_HPP__

