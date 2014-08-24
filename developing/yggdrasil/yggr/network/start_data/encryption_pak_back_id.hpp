//encryption_pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_START_DATA_PACKET_ENCRYPTION_PAK_BACK_ID_HPP__
#define __YGGR_NETWORK_START_DATA_PACKET_ENCRYPTION_PAK_BACK_ID_HPP__

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/start_data/start_data_def.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename Key, u32 key_len,
			typename Task_Data_Info = task_center::default_task_data_info_type>
class encryption_pak_back_id
	: public Task_Data_Info
{
public:
	typedef ID id_type;
	typedef Key key_type;
	typedef Task_Data_Info task_data_info_type;

	enum
	{
		E_key_len = key_len,

		E_compile_u32 = 0xffffffff
	};

public:
	enum
	{
		E_pak_id = start_data_def::E_id_start + 2,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

private:
	YGGR_PP_CHECK_START_DATA_ID(E_pak_id);
	typedef encryption_pak_back_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_id);
		ar & YGGR_SERIALIZE_NVP(_key);
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

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	encryption_pak_back_id(BOOST_RV_REF(this_type) right)
		: _id(boost::forward<id_type>(right._id)),
			_key(boost::forward<key_type>(right._key))
	{
	}

#else
	encryption_pak_back_id(BOOST_RV_REF(this_type) right)
		: _id(id_type())
	{
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
		_key.swap(right_ref._key);
	}
#endif

	encryption_pak_back_id(const this_type& right)
		: _id(right._id), _key(right._key)
	{
	}

	~encryption_pak_back_id(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
		_key = boost::forward<key_type>(right._key);
#else
		this_type& right_ref = right;
		boost::swap(_id, right_ref._id);
		_key.swap(right_ref._key);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_id = right._id;
		_key = right.key;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
		_key = boost::forward<key_type>(right._key);
#else
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
		_key.swap(right_ref._key);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_id, right._id);
		_key.swap(right._key);
	}

	const id_type& id(void) const
	{
		return _id;
	}

	const key_type& key(void) const
	{
		return _key;
	}

	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	id_type _id;
	key_type _key;
};

} // namespace start_data
} // namespace network
} // namespace yggr

#define _YGGR_PP_NETWORK_START_DATA_ENCRYPTION_PAK_BACK_ID_SWAP() \
	template<typename ID, typename Key, yggr::u32 key_len, typename Task_Data_Info> inline \
	void swap(yggr::network::start_data::encryption_pak_back_id<ID, Key, key_len, Task_Data_Info>& l, \
				yggr::network::start_data::encryption_pak_back_id<ID, Key, key_len, Task_Data_Info>& r) { \
		l.swap(r); } \
	\
	template<typename ID, typename Key, yggr::u32 key_len, typename Task_Data_Info> inline \
	void swap(BOOST_RV_REF(yggr::network::start_data::encryption_pak_back_id<ID YGGR_PP_SYMBOL_COMMA() \
																				Key YGGR_PP_SYMBOL_COMMA() \
																				key_len YGGR_PP_SYMBOL_COMMA() \
																				Task_Data_Info>) l, \
				yggr::network::start_data::encryption_pak_back_id<ID, Key, key_len, Task_Data_Info>& r) { \
		r.swap(l); } \
	\
	template<typename ID, typename Key, yggr::u32 key_len, typename Task_Data_Info> inline \
	void swap(yggr::network::start_data::encryption_pak_back_id<ID, Key, key_len, Task_Data_Info>& l, \
				BOOST_RV_REF(yggr::network::start_data::encryption_pak_back_id<ID YGGR_PP_SYMBOL_COMMA() \
																				Key YGGR_PP_SYMBOL_COMMA() \
																				key_len YGGR_PP_SYMBOL_COMMA() \
		 																		Task_Data_Info>) r) { \
		l.swap(r); }


namespace std
{
	_YGGR_PP_NETWORK_START_DATA_ENCRYPTION_PAK_BACK_ID_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_START_DATA_ENCRYPTION_PAK_BACK_ID_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_START_DATA_ENCRYPTION_PAK_BACK_ID_SWAP


#include <yggr/ids/id_n.hpp>
#include <yggr/ids/id_generator.hpp>
#include <yggr/network/start_data/start_data_generator.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename Key, u32 key_len, typename Task_Data_Info>
struct start_data_generator< encryption_pak_back_id<ID, Key, key_len, Task_Data_Info> >
{
	typedef ID id_type;
	typedef Key key_type;
	typedef Task_Data_Info task_data_info_type;

	typedef encryption_pak_back_id<id_type, key_type, key_len, task_data_info_type> start_data_type;

	typedef yggr::ids::id_n<key_len> pwd_cont_type;
	typedef yggr::ids::id_generator<pwd_cont_type> gen_type;

	start_data_type operator()(const id_type& id) const //create
	{
		gen_type gen;
		return start_data_type(id, gen());
	}
};

template<typename ID, typename Key, u32 key_len, typename Task_Data_Info>
struct start_data_loader< encryption_pak_back_id<ID, Key, key_len, Task_Data_Info> >
{
	typedef ID id_type;
	typedef Key key_type;
	typedef Task_Data_Info task_data_info_type;

	typedef encryption_pak_back_id<id_type, key_type, key_len, task_data_info_type> start_data_type;

	template<typename Packet>
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
