//timed_base_network_packet.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_PACKET_TIMED_BASE_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_NETWORK_PACKET_TIMED_BASE_NETWORK_PACKET_HPP__

#ifndef YGGR_BASE_NETWORK_PACKET_INCLUDE
#	error "include error timed_base_network_packet.hpp please include basic_network_packet.hpp"
#endif //YGGR_SESSION_INCLUDE

namespace yggr
{
namespace network
{
namespace network_packet
{

template<typename Archive_Partner, std::time_t tm_step, typename User_ID, typename Data_ID, typename Data_Ver>
class basic_network_packet : public yggr::packet::packet<Archive_Partner>
{

public:
	typedef yggr::packet::packet<Archive_Partner> base_type;

	typedef User_ID user_id_type;
	typedef Data_ID data_id_type;
	typedef Data_Ver data_ver_type;
	typedef std::time_t time_val_type;
	typedef std::pair<data_id_type, data_ver_type> data_cmp_type;


	typedef typename base_type::buf_type buf_type;
	typedef typename base_type::buf_val_type buf_val_type;
	typedef typename base_type::buf_real_val_type buf_real_val_type;

	typedef typename base_type::archive_partner_type archive_partner_type;

private:

	ERROR_MAKER_BEGIN("timed_base_network_packet")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_5)
			ERROR_CODE_DEF(E_invalid_version)
			ERROR_CODE_DEF(E_invalid_id_type)
			ERROR_CODE_DEF(E_invalid_version_type)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_5)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_version, "invalid version")
			ERROR_CODE_MSG(E_invalid_id_type, "invalid id type")
			ERROR_CODE_MSG(E_invalid_version_type, "invalid version type")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

//-------------------------------------------------------------------------

	class serialize_ack_container
	{
	public:

		serialize_ack_container(user_id_type& uid, data_id_type& did, data_ver_type& dver, time_val_type& tm)
			: _user_id(uid), _data_id(did), _data_ver(dver), _tm(tm)
		{
		}

		serialize_ack_container(const serialize_ack_container& right)
			: _user_id(right._user_id), _data_id(right._data_id), _data_ver(right._data_ver), _tm(right._tm)
		{
		}

		~serialize_ack_container(void)
		{
		}

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive& ar, const u32 version)
		{
			ar & YGGR_SERIALIZE_NVP(_user_id);
			ar & YGGR_SERIALIZE_NVP(_data_id);
			ar & YGGR_SERIALIZE_NVP(_data_ver);
			ar & YGGR_SERIALIZE_NVP(_tm);
		}

	private:
		user_id_type& _user_id;
		data_id_type& _data_id;
		data_ver_type& _data_ver;
		time_val_type& _tm;
	};

	template<typename Data_Object>
	class serialize_container
		: public serialize_ack_container
	{
	public:

		typedef Data_Object data_obj_type;

		serialize_container(user_id_type& uid, data_id_type& did, data_ver_type& dver, time_val_type& tm, data_obj_type& obj)
			: serialize_ack_container(uid, did, dver, tm), _data_obj(obj)
		{
		}

		serialize_container(const serialize_container& right)
			: serialize_ack_container(right) _data_obj(right._data_obj)
		{
		}

		~serialize_container(void)
		{
		}

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive& ar, const u32 version)
		{
			ar & YGGR_SERIALIZE_NAME_NVP("ack", boost::serialization::base_object<serialize_ack_container>(*this));
			ar & YGGR_SERIALIZE_NVP(_data_obj);
		}


	private:
		user_id_type& _user_id;
		data_id_type& _data_id;
		data_ver_type& _data_ver;
		time_val_type& _tm;
		data_obj_type& _data_obj;
	};

	

	typedef basic_network_packet this_type;
public:
	#include <yggr/support/condition_cal_fast_and_slow.ipp>


public:
	basic_network_packet(void)
		: _user_id(), _data_id(), 
		_data_ver(), _tm(std::time(0))
	{
		assert(archive_partner_type::is_support_network());
	}

	basic_network_packet(const user_id_type& uid, const data_id_type& id, const data_ver_type& ver)
		: _user_id(uid), _data_id(id), _data_ver(ver),  _tm(std::time(0))
	{
		assert(archive_partner_type::is_support_network());
	}

	template<typename T>
	basic_network_packet(const T& t, const user_id_type& uid)
		: _user_id(uid), _data_id(t.data_id()), _data_ver(t.data_ver()),  _tm(std::time(0))
	{
		assert(archive_partner_type::is_support_network());
		save(t);
	}

	basic_network_packet(const buf_type& buf)
		: base_type(buf), _user_id(user_id_type(0)), 
			_data_id(data_id_type(0)), _data_ver(data_ver_type(0)), _tm(0)
	{
		assert(archive_partner_type::is_support_network());
	}

	template<typename T>
	basic_network_packet(const T& t, const user_id_type& uid, const data_id_type& id, const data_ver_type& ver)
		: _user_id(uid), _data_id(id), _data_ver(ver),  _tm(std::time(0))
	{
		assert(archive_partner_type::is_support_network());
		save(t);
	}

	template<typename Other_Archive_Partner>
	basic_network_packet(const basic_network_packet<Other_Archive_Partner, tm_step, user_id_type, data_id_type, data_ver_type>& pak)
		: base_type(pak), _user_id(pak.user_id()), _data_id(pak.data_id()), _data_ver(pak.data_ver()),
		 _tm(pak.time())
	{
		assert(archive_partner_type::is_support_network());
	}


	basic_network_packet(const this_type& pak)
		: base_type(pak), _user_id(pak._user_id), _data_id(pak._data_id), _data_ver(pak._data_ver), _tm(pak._tm)
	{
		assert(archive_partner_type::is_support_network()); // change_boost_static_assert
	}

	~basic_network_packet(void)
	{
	}

	//void copy_from_buf(const buf_type& buf)
	//{
	//	base_type::copy_from_buf(buf);
	//}

	bool is_time_alive(void)
	{
		if(!_tm)
		{
			restore_uid_and_id_and_ver();
		}
		time_val_type now_tm(std::time(0));

		return (_tm <= now_tm) && ((now_tm - _tm) < time_step());
	}

	inline void clear(void)
	{
		base_type::clear();
		clear_ids();
	}

	inline void clear_ids(void)
	{
		_user_id = 0;
		_data_id = 0;
		_data_ver = 0;
		_tm = std::time(0);
	}

	inline time_val_type time_step(void) const
	{
		return tm_step;
	}

	//inline buf_type& org_buf(void)
	//{
	//	return base_type::org_buf();
	//}

	//inline const buf_type& org_buf(void) const
	//{
	//	return base_type::org_buf();
	//}

	template<typename T>
	bool save(const T& t)
	{
		typedef T obj_type;
		typedef serialize_container<E_SAVE, obj_type> serialize_container_type;

		serialize_container_type container(_user_id, _data_id, _data_ver, _tm, const_cast<obj_type&>(t));

		return base_type::save(container);
	}

	template<typename T>
	bool load(T& t)
	{
		typedef T obj_type;
		typedef serialize_container<E_LOAD, obj_type> serialize_container_type;

		serialize_container_type container(_user_id, _data_id, _data_ver, _tm, t);

		return base_type::load(container);
	}

	const data_id_type& data_id(void)
	{
		if(!_data_id)
		{
			restore_uid_and_id_and_ver();
		}
		return _data_id;
	}

	const data_ver_type& data_ver(void)
	{
		if(!_data_ver)
		{
			restore_uid_and_id_and_ver();
		}
		return _data_ver;
	}

	const user_id_type& user_id(void)
	{
		if(!_user_id)
		{
			restore_uid_and_id_and_ver();
		}

		return _user_id;
	}

	const time_val_type& time(void)
	{
		if(!_tm)
		{
			restore_uid_and_id_and_ver();
		}
		return _tm;
	}


	const data_id_type& data_id(void) const
	{
		return _data_id;
	}

	const data_ver_type& data_ver(void) const
	{
		return _data_ver;
	}

	const user_id_type& user_id(void) const
	{
		return _user_id;
	}

	const time_val_type& time(void) const
	{
		return _tm;
	}

	data_cmp_type data_info(void)
	{
		if(!_data_id || !_data_ver)
		{
			restore_uid_and_id_and_ver();
		}
		return data_cmp_type(_data_id, _data_ver);
	}

	const data_cmp_type data_info(void) const
	{
		return data_cmp_type(_data_id, _data_ver);
	}

	this_type& operator=(const buf_type& buf)
	{
		base_type& base = *this;
		base = buf;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		_user_id = right._user_id;
		_data_id = right._data_id;
		_data_ver = right._data_ver;
		_tm = right._tm;
		return *this;
	}

	bool operator==(const data_cmp_type& cmp) const
	{
		return (data_id() == cmp.first) && (data_ver() == cmp.second);
	}

	bool operator==(const basic_network_packet& pak) const
	{
		return base_type::_buf == pak._buf;
	}

	void swap(buf_type& obuf)
	{
		std::swap(base_type::_buf, obuf);
		restore_uid_and_id_and_ver();
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_user_id, right._user_id);
		std::swap(_data_id, right._data_id);
		std::swap(_data_ver, right._data_ver);
		std::swap(_tm, right._tm);
		base_type::swap(right);
	}

private:
	void restore_uid_and_id_and_ver(void)
	{
		serialize_ack_container ack_container(_user_id, _data_id, _data_ver, _tm);
		base_type::load(ack_container);
		return;
	}
private:
	user_id_type _user_id;
	data_id_type _data_id;
	data_ver_type _data_ver;
	time_val_type _tm;
};


} // namespace pak
} // namespace network
} // namespaxe yggr

#endif //__YGGR_NETWORK_NETWORK_PACKET_TIMED_BASE_NETWORK_PACKET_HPP__
