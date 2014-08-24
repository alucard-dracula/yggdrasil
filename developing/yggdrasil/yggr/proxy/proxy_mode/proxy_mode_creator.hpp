//proxy_mode_creator.hpp

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODE_CREATOR_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODE_CREATOR_HPP__

#include <algorithm>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/push_back.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/base/interface_ptr.hpp>

#include <yggr/proxy/proxy_mode/basic_proxy_mode.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename ModeVector,
			typename OwnerInfoContainer>
class proxy_mode_creator
{
public:
	typedef ModeVector mode_vt_type;
	typedef OwnerInfoContainer owner_info_container_type;
	//typedef OwnerIDContainer owner_id_container_type;

	typedef basic_proxy_mode<
								owner_info_container_type
							> base_proxy_mode_type;

	typedef typename base_proxy_mode_type::owner_id_container_type owner_id_container_type;
	typedef yggr::interface_ptr<base_proxy_mode_type> interface_proxy_mode_type;

private:

	typedef proxy_mode_creator this_type;
	typedef interface_proxy_mode_type (this_type::*creator_type)(void) const;

	typedef u32 key_type;
	typedef safe_container::safe_unordered_map<key_type, creator_type> creator_map_type;

	enum
	{
		E_mode_length = boost::mpl::size<mode_vt_type>::value,

		E_compile_u32 = 0xffffffff
	};

	struct end_creator
	{
		bool operator()(creator_map_type& map)
		{
			return true;
		}
	};

	template<u32 idx, u32 idx_size>
	struct init_creator
	{
	private:
		typedef proxy_mode_creator parent_type;
		YGGR_PP_FRIEND_TYPENAME(parent_type);

		typedef typename boost::mpl::at_c<mode_vt_type, idx>::type mode_type;
		enum
		{
			E_mode = mode_type::E_mode
		};

		BOOST_MPL_ASSERT((boost::is_same<typename mode_type::owner_info_container_type, owner_info_container_type>));
		BOOST_MPL_ASSERT((boost::is_same<typename mode_type::owner_id_container_type, owner_id_container_type>));

		typedef init_creator this_type;
	public:
		bool operator()(creator_map_type& map)
		{
			if(!map.insert(this_type::E_mode, &parent_type::prv_create_mode<mode_type>))
			{
				return false;
			}

            typename boost::mpl::if_c
                        <
                            (idx + 1 < idx_size),
                            init_creator<idx + 1, idx_size>,
                            end_creator
                        >::type init;
			return init(map);
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
		_creator_map.clear();
	}

	interface_proxy_mode_type operator()(const key_type& key) const
	{
		return _creator_map.use_handler(boost::bind(&this_type::handler_creator, this, _1, boost::cref(key)));
	}

private:
	template<typename T>
	interface_proxy_mode_type prv_create_mode(void) const
	{
		return interface_proxy_mode_type(new T());
	}

	interface_proxy_mode_type
		handler_creator(const typename creator_map_type::base_type& base, const key_type& key) const
	{
		typedef typename creator_map_type::const_iterator citer_type;
		citer_type iter = base.find(key);
		if(iter == base.end())
		{
			return interface_proxy_mode_type();
		}

		assert(iter->second);
		return (this->*(iter->second))();
	}

private:
	creator_map_type _creator_map;

};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif //__YGGR_PROXY_PROXY_MODE_PROXY_MODE_CREATOR_HPP__
