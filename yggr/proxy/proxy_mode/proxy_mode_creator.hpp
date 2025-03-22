//proxy_mode_creator.hpp

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

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODE_CREATOR_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODE_CREATOR_HPP__

#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/proxy/proxy_mode/basic_proxy_mode.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/push_back.hpp>

#include <boost/function.hpp>

#include <algorithm>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename ModeVector, typename ModeDataStore>
class proxy_mode_creator
{
public:
	typedef ModeVector mode_vt_type;
	typedef ModeDataStore mode_data_store_type;
	typedef typename mode_data_store_type::owner_info_container_type owner_info_container_type;
	typedef typename mode_data_store_type::owner_info_type owner_info_type;
	typedef typename mode_data_store_type::owner_id_type owner_id_type;

	typedef typename mode_data_store_type::store_type store_type;

	typedef 
		basic_proxy_mode
		<
			mode_data_store_type
		> base_proxy_mode_type;
	
	typedef shared_ptr<base_proxy_mode_type> interface_proxy_mode_type;

private:
	typedef proxy_mode_creator this_type;

private:
	typedef boost::function1<interface_proxy_mode_type, const this_type&> creator_type;

	typedef u32 key_type;
	typedef safe_container::safe_unordered_map<key_type, creator_type> creator_map_type;

	YGGR_STATIC_CONSTANT(u32, E_mode_length = static_cast<u32>(boost::mpl::size<mode_vt_type>::value));

	// don't move to outside
	struct end_creator
	{
		inline bool operator()(creator_map_type& map) const
		{
			return true;
		}
	};

	// don't move to outside
	template<u32 idx, u32 idx_size>
	struct init_creator
	{
	private:
		typedef proxy_mode_creator outside_type;
		YGGR_PP_FRIEND_TYPENAME(outside_type);

		typedef typename boost::mpl::at_c<mode_vt_type, idx>::type mode_type;

		YGGR_STATIC_CONSTANT(u32, E_mode = static_cast<u32>(mode_type::E_mode));

		BOOST_MPL_ASSERT((boost::is_same<typename mode_type::owner_info_container_type, owner_info_container_type>));

	private:
		typedef init_creator this_type;
	
	public:
		inline bool operator()(creator_map_type& map) const
		{
			typedef typename 
				boost::mpl::if_c
				<
					(idx + 1 < idx_size),
					init_creator<idx + 1, idx_size>,
					end_creator
				>::type init_type;

			creator_type creator(boost::bind(&outside_type::prv_s_create_mode<mode_type>));
			if(!map.insert(this_type::E_mode, boost::move(creator)))
			{
				return false;
			}
			else
			{
				init_type init;
				return init(map);
			}
		}
	};

public:
	proxy_mode_creator(void)
	{
		init_creator<0, E_mode_length> init;
		init(_creator_map);

		assert((_creator_map.size() == this_type::E_mode_length));
	}

	~proxy_mode_creator(void)
	{
	}

public:
	inline interface_proxy_mode_type operator()(const key_type& key) const
	{
		creator_type creator;
		bool bright = _creator_map.get_value(key, creator);
		
		return bright? creator(*this) : interface_proxy_mode_type();
	}

private:
	template<typename T> inline
	static interface_proxy_mode_type prv_s_create_mode(void)
	{
		typedef T now_proxy_mode_type;
		return 
			construct_shared<base_proxy_mode_type>(
				yggr_nothrow_new now_proxy_mode_type() );
	}

private:
	creator_map_type _creator_map;

};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif //__YGGR_PROXY_PROXY_MODE_PROXY_MODE_CREATOR_HPP__
