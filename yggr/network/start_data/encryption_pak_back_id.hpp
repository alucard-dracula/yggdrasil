//encryption_pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_START_DATA_PACKET_ENCRYPTION_PAK_BACK_ID_HPP__
#define __YGGR_NETWORK_START_DATA_PACKET_ENCRYPTION_PAK_BACK_ID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/start_data/start_data_def.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename Key, u32 key_len,
			typename TaskDataInfo = task_center::default_task_data_info_type>
class encryption_pak_back_id
	: public TaskDataInfo
{
public:
	typedef ID id_type;
	typedef Key key_type;
	typedef TaskDataInfo task_data_info_type;

public:
	YGGR_STATIC_CONSTANT(u32, E_key_len = key_len);

private:
	typedef encryption_pak_back_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = start_data_def::E_id_start + 2
	// E_now_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (start_data_def::E_id_start + 2), 1)

private:
	YGGR_PP_CHECK_START_DATA_ID(E_pak_id);

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_id);
		ar & YGGR_SERIALIZATION_NVP(_key);
	}

public:

	encryption_pak_back_id(void)
		: _id(id_type())
	{
	}

	encryption_pak_back_id(const id_type& nid, const key_type& nkey)
		: _id(nid), _key(nkey.begin(), nkey.end())
	{
		assert((_key.size() == E_key_len));
	}

	encryption_pak_back_id(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id)),
			_key(boost::move(right._key))
	{
	}

	encryption_pak_back_id(const this_type& right)
		: _id(right._id), _key(right._key)
	{
	}

	~encryption_pak_back_id(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_id, boost::move(right._id));
		copy_or_move_or_swap(_key, boost::move(right._key));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}
		_id = right._id;
		_key = right.key;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(_id, right._id);
		_key.swap(right._key);
	}

public:
	inline const id_type& id(void) const
	{
		return _id;
	}

	inline const key_type& key(void) const
	{
		return _key;
	}

private:
	id_type _id;
	key_type _key;
};

} // namespace start_data
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace start_data
{
namespace swap_support
{

template<typename ID, typename Key, yggr::u32 key_len, typename TaskDataInfo> inline 
void swap(encryption_pak_back_id<ID, Key, key_len, TaskDataInfo>& l, 
			encryption_pak_back_id<ID, Key, key_len, TaskDataInfo>& r) 
{ 
	l.swap(r);
}


} // namespace swap_support

using swap_support::swap;

} // namespace start_data
} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::start_data::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::start_data::swap_support::swap;
} // namespace boost

#include <yggr/ids/id_n.hpp>
#include <yggr/network/start_data/start_data_generator.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename Key, u32 key_len, typename TaskDataInfo>
struct start_data_generator< encryption_pak_back_id<ID, Key, key_len, TaskDataInfo> >
{
	typedef ID id_type;
	typedef Key key_type;
	typedef TaskDataInfo task_data_info_type;

	typedef encryption_pak_back_id<id_type, key_type, key_len, task_data_info_type> start_data_type;

	typedef yggr::ids::id_n<key_len> pwd_cont_type;
	typedef yggr::ids::id_generator<pwd_cont_type> gen_type;

	inline start_data_type operator()(const id_type& id) const //create
	{
		gen_type gen;
		return start_data_type(id, gen());
	}
};

template<typename ID, typename Key, u32 key_len, typename TaskDataInfo>
struct start_data_loader< encryption_pak_back_id<ID, Key, key_len, TaskDataInfo> >
{
	typedef ID id_type;
	typedef Key key_type;
	typedef TaskDataInfo task_data_info_type;

	typedef encryption_pak_back_id<id_type, key_type, key_len, task_data_info_type> start_data_type;

	template<typename Packet> inline
	start_data_type operator()(Packet& pak) const // getter
	{
		start_data_type sdata;
		pak.load(sdata);
		return sdata;
	}
};

} // namespcae start_data
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_START_DATA_PACKET_ENCRYPTION_PAK_BACK_ID_HPP__
