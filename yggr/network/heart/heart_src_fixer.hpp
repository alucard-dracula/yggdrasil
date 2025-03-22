//heart_src_fixer.hpp

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

#ifndef __YGGR_NETWORK_HEART_HEART_SRC_FIXER_HPP__
#define __YGGR_NETWORK_HEART_HEART_SRC_FIXER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/pointer_to_value_t.hpp>

namespace yggr
{
namespace network
{
namespace heart
{

template<typename Heart_Data>
struct heart_src_fixer
{
	typedef Heart_Data heart_data_type;

	template<typename Def, typename Packer, typename SessionPtr>
	std::pair<u32, bool> check_packet(const Packer& pak, SessionPtr sptr) const
	{
		typedef Def def_type;
		typedef SessionPtr session_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_ptr_type>::type session_type;
		typedef typename session_type::send_data_type send_data_type;
		typedef typename send_data_type::owner_info_type owner_info_type;
		typedef std::pair<u32, bool> ret_type;

		assert(sptr);

		if(pak.data_info() == heart_data_type::data_info())
		{
			heart_data_type hd;
			if(!pak.load(hd))
			{
				return ret_type(def_type::s_offline(), false);
			}

			assert(hd.size() == 2);
			if(hd.size() != 2)
			{
				return ret_type(def_type::s_offline(), false);
			}

			return ret_type(def_type::s_online(), false);
		}

		return ret_type(def_type::s_online(), true);
	}

	template<typename Def, typename State, typename SessionPtr>
	void check(State& state, SessionPtr sptr) const
	{
		typedef Def def_type;
		typedef SessionPtr session_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_ptr_type>::type session_type;
		typedef typename session_type::send_data_type send_data_type;
		typedef typename send_data_type::owner_info_type owner_info_type;

		if(!sptr)
		{
			return;
		}

		if(state == def_type::s_online())
		{
			change_state(state, def_type::s_need_check());
			return;
		}

		if(state == def_type::s_need_check())
		{
			heart_data_type heart_data;
			send_data_type sdata(owner_info_type(sptr->get_id()), heart_data);
			sptr->send_packet(sdata);
			change_state(state, def_type::s_offline());
			return;
		}

		sptr->handler_close_level_error();
	}

private:

	template<typename State>
	void change_state(State& sat, u32 new_sat) const
	{
		u32 old_sat = 0;
		do
		{
			old_sat = sat.load();
		} while((old_sat != new_sat) && (!sat.compare_exchange_weak(old_sat, new_sat)));
	}

};

} // namespace heart
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_HEART_HEART_SRC_FIXER_HPP__