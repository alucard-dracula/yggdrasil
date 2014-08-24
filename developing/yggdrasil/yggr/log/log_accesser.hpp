//log_accesser.hpp

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

#ifndef __YGGR_LOG_LOG_ACCESSER_HPP__
#define __YGGR_LOG_LOG_ACCESSER_HPP__

#include <map>
#include <list>

namespace yggr
{
namespace log
{

template<typename MSG_ID, typename Op, typename Data_Creator>
class log_accesser
{
public:
	typedef Op op_type;
	typedef MSG_ID msg_id_type;
	typedef Data_Creator data_creator_type;
	typedef typename op_type::init_type init_type;
	typedef typename data_creator_type::init_type data_creator_init_type;

	typedef std::list<data_creator_type> data_creator_list_type;
	typedef typename data_creator_list_type::iterator data_creator_list_iter_type;
	typedef typename data_creator_list_type::const_iterator data_creator_list_c_iter_type;

	typedef log_accesser<msg_id_type, op_type, data_creator_type> this_type;

	typedef std::map<msg_id_type, data_creator_list_iter_type> msg_id_map_type;
	typedef typename msg_id_map_type::iterator msg_id_map_iter_type;
	typedef typename msg_id_map_type::const_iterator msg_id_map_c_iter_type;
	typedef typename msg_id_map_type::value_type ins_val_type;
public:
	log_accesser(const init_type& init)
		: _op(init)
	{
	}

	~log_accesser(void)
	{
		_msg_ids.clear();
		_creators.clear();
	}

	bool register_msg(const msg_id_type& id, const data_creator_init_type& init)
	{
		data_creator_list_iter_type iter_crt = std::find(_creators.begin(), _creators.end(), init);

		if(iter_crt == _creators.end())
		{
			data_creator_type dc(init);
			_creators.push_back(dc);
			iter_crt = _creators.end();
			--iter_crt;
		}

		msg_id_map_iter_type iter = _msg_ids.find(id);
		if(iter == _msg_ids.end())
		{
			_msg_ids[id] = iter_crt;
			return true;
		}

		return false;
	}

	bool register_msg(const msg_id_type& id, const data_creator_type& creator)
	{
		data_creator_list_iter_type iter_crt = std::find(_creators.begin(), _creators.end(), creator);

		if(iter_crt == _creators.end())
		{
			_creators.push_back(creator);
			iter_crt = _creators.end();
			--iter_crt;
		}

		msg_id_map_iter_type iter = _msg_ids.find(id);
		if(iter == _msg_ids.end())
		{
			_msg_ids[id] = iter_crt;
			return true;
		}

		return false;
	}

	template<typename Value>
	void append(const Value& val)
	{
		msg_id_map_iter_type iter = _msg_ids.find(val.code());

		if(iter == _msg_ids.end())
		{
			return;
		}

		_op.append(iter->second->create_param(val));
	}


private:
	op_type _op;
	msg_id_map_type _msg_ids;
	data_creator_list_type _creators;
};

} // namespace log_accesser
} // namespace yggr 

#endif //__YGGR_LOG_LOG_ACCESSER_HPP__