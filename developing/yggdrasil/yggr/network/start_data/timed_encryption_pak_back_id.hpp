//timed_encryption_pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_START_DATA_PACKET_TIMED_ENCRYPTION_PAK_BACK_ID_HPP__
#define __YGGR_NETWORK_START_DATA_PACKET_TIMED_ENCRYPTION_PAK_BACK_ID_HPP__

#error "need mend!!!!!!!"

#include <yggr/task_center/task_info.hpp>
#include <boost/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename Key, typename Time,
			typename Task_Info = task_center::default_cdt_fast_task_info_type >
class pak_back_id
{
public:
	typedef ID id_type;
	typedef Key key_type;
	typedef Time time_type;

	typedef Task_Info base_type;
	

public:
	enum
	{
		E_pak_id_start = 0x00008000,
		E_pak_id = E_pak_id_start + 4,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

private:
	typedef pak_back_id this_type;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_id);
		ar & YGGR_SERIALIZE_NVP(_key);
		ar & YGGR_SERIALIZE_NVP(_tm);
	}


public:

	pak_back_id(void)
		: _id(0), _tm(std::time(0))
	{
	}

	explicit pak_back_id(const id_type& nid, const key_type& nkey)
		: _id(nid), _tm(std::time(0))
	{
		_key.resize(nkey.size());
		memcpy(&(_key[0]), &(nkey[0]), _key.size());
	}

	pak_back_id(const pak_back_id& pak)
		: _id(pak._id), _tm(pak._tm)
	{
		_key.resize(pak._key.size());
		memcpy(&(_key[0]), &((pak._key)[0]), _key.size());

	}

	~pak_back_id(void)
	{
	}

	const id_type& id(void) const
	{
		return _id;
	}

	const key_type& key(void) const
	{
		return _key;
	}

	const time_type& time(void) const
	{
		return _tm;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_id = right._id;
		_key.resize(right._key.size());
		memcpy(&(_key[0]), &((right._key)[0]), _key.size());
		_tm = right._tm;
		return *this;
	}

	YGGR_TASK_IMPORT(typename base_type, this_type, E_pak_id, E_now_ver)

private:
	id_type _id;
	key_type _key;
	time_type _tm;
};

} // namespace network_packet
} // namespace network
} // namespace yggr


#endif //__YGGR_NETWORK_START_DATA_PACKET_TIMED_ENCRYPTION_PAK_BACK_ID_HPP__
