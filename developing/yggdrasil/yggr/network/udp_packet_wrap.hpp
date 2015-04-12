//udp_packet_wrap.hpp

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

#ifndef __YGGR_NETWORK_UDP_PACKET_WARP_HPP__
#define __YGGR_NETWORK_UDP_PACKET_WARP_HPP__

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif // _MSC_VER

#include <vector>
#include <limits>

#include <yggr/move/move.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/atomic.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/network/udp_pak_info.hpp>
#include <yggr/network/hn_conv.hpp>

#include <yggr/network/type_traits/tags.hpp>

#include <yggr/encryption_tool/md5_tool.hpp>

//-------------------udp_packet-------------------------
namespace yggr
{
namespace network
{

template<typename Buffer, typename Verify = encryption_tool::md5_tool>
class udp_packet
{
public:
	typedef Buffer data_buf_type;
	typedef Verify verify_type;
	typedef typename verify_type::check_type check_type;
	typedef udp_pak_id pak_id_type;
	typedef pak_id_type::id_type id_type;
	typedef udp_pak_header pak_header_type;

public:
	enum
	{
		E_INFO_SIZE = pak_id_type::E_LENGTH + pak_header_type::E_LENGTH,
		E_VERIFY_SIZE = encryption_tool::tool_buf_size<verify_type>::size,
		E_MAX_DATA_LENGTH = 1024 - pak_id_type::E_LENGTH - pak_header_type::E_LENGTH - E_VERIFY_SIZE,
		E_MAX_LENGTH = 1024,
		E_compile_u32_of_size = 0xffffffff
	};

	enum
	{
		E_unknow = 0,
		E_static,
		E_dynamic,
		E_compile_u16_of_type = 0xffff
	};

private:
	typedef udp_packet this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	udp_packet(void)
		: _id(id_type())
	{
	}

	udp_packet(const id_type& id, u16 idx, u16 count, u16 type, const data_buf_type& buf)
		: _id(id), _header(idx, count, buf.size(), type), _buf(buf)
	{
	}

	udp_packet(const id_type& id, u16 idx, u16 count, u16 type, BOOST_RV_REF(data_buf_type) buf)
		: _id(id),
			_header(idx, count, type, buf.size()),
			_buf(boost::forward<data_buf_type>(buf))
	{
	}

	template<typename InputIter>
	udp_packet(const id_type& id, u16 idx,  u16 count, u16 type, InputIter s, InputIter e)
		: _id(id),
			_buf(s, e)

	{
		_header.idx = idx;
		_header.count = count;
		_header.size = _buf.size();
		_header.type = type;
	}

	udp_packet(BOOST_RV_REF(data_buf_type) buf)
	{
		bool bright = from_buffer(boost::forward<data_buf_type>(buf));
		if(!bright)
		{
			_buf.clear();
		}
	}

	udp_packet(const data_buf_type& buf)
	{
		bool bright = from_buffer(buf);
		if(!bright)
		{
			_buf.clear();
		}
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	udp_packet(BOOST_RV_REF(this_type) right)
		: _id(right._id),
			_header(boost::forward<pak_header_type>(right._header)),
			_buf(boost::forward<data_buf_type>(right._buf))
	{
	}
#else
	udp_packet(BOOST_RV_REF(this_type) right)
		: _id(), _header(), _buf()
	{
		this_type& right_ref = right;
		boost::swap(_id, right_ref._id);
		boost::swap(_header, right_ref._deader);
		boost::swap(_buf, right_ref._buf);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	udp_packet(const this_type& right)
		: _id(right._id), _header(right._header), _buf(right._buf)
	{
	}

	~udp_packet(void)
	{
		_buf.clear();
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = right._id;
		_header = boost::forward<pak_header_type>(right._header);
		_buf = boost::forward<data_buf_type>(right._buf);
#else
		std::swap(_id, right._id);
		_header = boost::forward<pak_header_type>(right._header);
		_buf = boost::forward<data_buf_type>(right._buf);

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_id = right._id;
		_header = right._header;
		_buf = right._buf;
		return *this;
	}

	bool operator==(const id_type& id) const
	{
		return _id == id;
	}

	bool operator==(const pak_id_type& id) const
	{
		return _id == id;
	}

	const id_type& id(void) const
	{
		return _id;
	}

	pak_header_type& header(void)
	{
		return _header;
	}

	const pak_header_type& header(void) const
	{
		return _header;
	}

	data_buf_type& data_buf(void)
	{
		return _buf;
	}

	const data_buf_type& data_buf(void) const
	{
		return _buf;
	}

	size_type data_size(void) const
	{
		return _buf.size();
	}

	u16 idx(void) const
	{
		return _header.idx;
	}

	u16 count(void) const
	{
		return _header.count;
	}

	u16 type(void) const
	{
		return _header.type;
	}

	void set(const id_type& id, u16 idx, u16 count, u16 type, const data_buf_type& buf)
	{
		_id = id;
		_header.idx = idx;
		_header.count = count;
		_header.type = type;
		_buf = buf;
		_header.size = _buf.size();
	}

	void set(const id_type& id, u16 idx, u16 count, u16 type, BOOST_RV_REF(data_buf_type) buf)
	{
		_id = id;
		_header.idx = idx;
		_header.count = count;
		_header.type = type;
		_buf = boost::forward<data_buf_type>(buf);
		_header.size = _buf.size();
	}

	template<typename InputIter>
	void set(const id_type& id, u16 idx, u16 count, u16 type, InputIter start, InputIter end)
	{
		_id = id;
		_header.idx = idx;
		_header.count = count;
		_header.type = type;
		_buf.assign(start, end);
		_header.size = _buf.size();
	}

	void to_buffer(data_buf_type& buf) const
	{
		data_buf_type id_buf;
		_id.to_buffer(id_buf);

		data_buf_type h_buf;
		_header.to_buffer(h_buf);

		data_buf_type tmp_buf(id_buf.size() + h_buf.size() + _buf.size(), 0);
		memcpy(&tmp_buf[0], &id_buf[0], pak_id_type::E_LENGTH);
		memcpy(&tmp_buf[pak_id_type::E_LENGTH], &h_buf[0], pak_header_type::E_LENGTH);
		memcpy(&tmp_buf[pak_id_type::E_LENGTH + pak_header_type::E_LENGTH], &_buf[0], _buf.size());

		verify_type tool;
		check_type chk = tool.encrypt(tmp_buf);

		buf.resize(tmp_buf.size() + this_type::E_VERIFY_SIZE);
		memcpy(&buf[0], &tmp_buf[0], tmp_buf.size());
		memcpy(&buf[tmp_buf.size()], &chk[0], this_type::E_VERIFY_SIZE);
	}

	data_buf_type to_buffer(void) const
	{
		data_buf_type buf;
		this_type::to_buffer(buf);
		return buf;
	}

	bool from_buffer(const data_buf_type& buf)
	{
		size_type chk_size = pak_id_type::E_LENGTH;
		if(buf.size() < chk_size)
		{
			return false;
		}

		if(!_id.from_buffer(&buf[0], (&buf[0]) + pak_id_type::E_LENGTH))
		{
			return false;
		}

		if(_id == pak_id_type())
		{
			return false;
		}

		chk_size += pak_header_type::E_LENGTH;
		if(buf.size() < chk_size)
		{
			return false;
		}

		if(!_header.from_buffer(&buf[pak_id_type::E_LENGTH], (&buf[chk_size]) + pak_header_type::E_LENGTH))
		{
			return false;
		}

		chk_size += _header.size;
		if(buf.size() < chk_size)
		{
			return false;
		}

		data_buf_type tmp_buf(chk_size, 0);
		memcpy(&tmp_buf[0], &buf[0], chk_size);

		check_type chk_src;
		memcpy(&chk_src[0], &buf[chk_size], this_type::E_VERIFY_SIZE);

		verify_type tool;
		check_type chk_now = tool.encrypt(tmp_buf);

		if(chk_src != chk_now)
		{
			return false;
		}

		_buf.resize(_header.size);
		memcpy(&_buf[0], &tmp_buf[pak_id_type::E_LENGTH + pak_header_type::E_LENGTH], _header.size);

		return true;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		boost::swap(_id, right._id);
		boost::swap(_header, right._deader);
		boost::swap(_buf, right._buf);
	}

	bool empty(void) const
	{
		return _id == pak_id_type() || _buf.empty();
	}

private:
	pak_id_type _id;
	pak_header_type _header;
	data_buf_type _buf;
};

} // namespace network
} // naemspace yggr

#define _YGGR_PP_NETWORK_UDP_PACEKT_SWAP() \
	template<typename Buffer> inline \
	void swap(yggr::network::udp_packet<Buffer>& l, \
				yggr::network::udp_packet<Buffer>& r) { \
		l.swap(r); } \
	\
	template<typename Buffer> inline \
	void swap(BOOST_RV_REF(yggr::network::udp_packet<Buffer>) l, \
				yggr::network::udp_packet<Buffer>& r) { \
		r.swap(l); } \
	\
	template<typename Buffer> inline \
	void swap(yggr::network::udp_packet<Buffer>& l, \
				BOOST_RV_REF(yggr::network::udp_packet<Buffer>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_NETWORK_UDP_PACEKT_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_UDP_PACEKT_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_UDP_PACEKT_SWAP

//------------------------udp_packet_wrap------------------------------

namespace yggr
{
namespace network
{

template<typename Buffer, typename Verify = encryption_tool::md5_tool >
class udp_packet_wrap
{
public:
	typedef Buffer data_buf_type;
	typedef typename data_buf_type::value_type data_buf_val_type;

	typedef yggr::u64 id_type;
	typedef Verify verify_type;
	typedef udp_packet<data_buf_type, verify_type> udp_packet_type;
	typedef std::vector<udp_packet_type> udp_packet_vt_type;
	typedef typename udp_packet_vt_type::iterator udp_packet_vt_iter_type;
	typedef typename udp_packet_vt_type::const_iterator udp_packet_vt_c_iter_type;

	enum
	{
		E_MAX_SINGLE_BUF_LENGTH = udp_packet_type::E_MAX_LENGTH,
		E_compile_u32 = 0xffffffff
	};

	enum
	{
		E_state_die = 0,
		E_state_wait_check,
		E_state_alive,

		E_compile_state_u32 = 0xffffff
	};

private:
	typedef boost::atomic<u32> state_type;

	typedef yggr::network::type_traits::tag_pak_static tag_static_type;
	typedef yggr::network::type_traits::tag_pak_dynamic tag_dynamic_type;

	template<typename T, typename Nil_T = int>
	struct check_packet
	{
		enum
		{
			value = udp_packet_type::E_unknow
		};
	};

	template<typename Nil_T>
	struct check_packet<tag_static_type, Nil_T>
	{
		enum
		{
			value = udp_packet_type::E_static
		};
	};

	template<typename Nil_T>
	struct check_packet<tag_dynamic_type, Nil_T>
	{
		enum
		{
			value = udp_packet_type::E_dynamic
		};
	};

private:
	typedef udp_packet_wrap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	udp_packet_wrap(void)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknow),
			_bcomplete(false),
			_state(E_state_alive)
	{
	}

	udp_packet_wrap(BOOST_RV_REF(data_buf_type) buf)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknow),
			_bcomplete(false)
	{
		prv_collect(boost::forward<data_buf_type>(buf));
	}

	udp_packet_wrap(const data_buf_type& buf)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknow),
			_bcomplete(false)
	{
		prv_collect(buf);
	}

	template<typename Packet>
	udp_packet_wrap(const Packet& pak)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknow),
			_bcomplete(false),
			_state(E_state_alive)
	{
		split_packet(gen_id(), pak);
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

	udp_packet_wrap(BOOST_RV_REF(this_type) right)
		: _id(right._id),
			_now_count(right._now_count),
			_count(right._count),
			_type(right._type),
			_bcomplete(right._bcomplete),
			_udp_packet_vt(boost::forward<udp_packet_vt_type>(right._udp_packet_vt)),
			_state(E_state_alive)
	{
	}

#else

	udp_packet_wrap(BOOST_RV_REF(this_type) right)
		: _id(right._id),
			_now_count(right._now_count),
			_count(right._count),
			_type(right._type),
			_bcomplete(right._bcomplete),
			_state(E_state_alive)
	{
		this_type& right_ref = right;
		_udp_packet_vt.swap(right_ref._udp_packet_vt);
	}

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	udp_packet_wrap(const this_type& right)
		: _id(right._id),
			_now_count(right._now_count),
			_count(right._count),
			_type(right._type),
			_bcomplete(right._bcomplete),
			_udp_packet_vt(right._udp_packet_vt),
			_state(E_state_alive)
	{
	}

	~udp_packet_wrap(void)
	{
	}

	bool empty(void) const
	{
#ifdef _DEBUG
		if(_id == 0)
		{
			assert(_now_count == 0);
			assert(_count == 0);
			assert(_type == udp_packet_type::E_unknow);
			assert(_bcomplete == false);
			assert(_udp_packet_vt.empty());
		}
#endif //_DEBUG
		return _id == 0;
	}

	void clear(void)
	{
		_id = 0;
		_now_count = 0;
		_count = 0;
		_type = udp_packet_type::E_unknow;
		_bcomplete = false;
		_udp_packet_vt.clear();
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		change_state(E_state_alive);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
		_now_count = right._now_count;
		_count = right._count;
		_type = right._type;
		_udp_packet_vt = boost::forward<udp_packet_vt_type>(right._udp_packet_vt);
#else
		boost::swap(_id, right._id);
		boost::swap(_now_count, right._now_count);
		boost::swap(_count, right._count);
		boost::swap(_type, right._type);
		boost::swap(_bcomplete, right._bcomplete);
		_udp_packet_vt.swap(right._udp_packet_vt);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		change_state(E_state_alive);
		_id = right._id;
		_now_count = right._now_count;
		_count = right._count;
		_type = right._type;
		_bcomplete = right._bcomplete;
		_udp_packet_vt = right._udp_packet_vt;
		//change_state(E_state_alive);
		return *this;
	}

	bool is_complete(void) const
	{
		return _bcomplete;
	}

	data_buf_type operator[](size_type idx) const
	{
		data_buf_type buf;
		_udp_packet_vt[idx].to_buffer(buf);
		return buf;
	}

	data_buf_type& get_data_buf(size_type idx, data_buf_type& out) const
	{
		_udp_packet_vt[idx].to_buffer(out);
		return out;
	}

	template<typename Packet>
	Packet get_packet(void) const
	{
		typedef Packet pak_type;

		if(!_bcomplete)
		{
			return pak_type();
		}

		pak_type pak;
		merge_data(pak);
		return pak;
	}

	id_type& id(void)
	{
		return _id;
	}

	const id_type& id(void) const
	{
		return _id;
	}

	u16 now_count(void) const
	{
		return _now_count;
	}

	u16 count(void) const
	{
		return _count;
	}

	u16 type(void) const
	{
		return _type;
	}

	const udp_packet_vt_type& udp_packets(void) const
	{
		return _udp_packet_vt;
	}

	udp_packet_vt_type& udp_packets(void)
	{
		return _udp_packet_vt;
	}

	void collect(const data_buf_type& buf)
	{
		prv_collect(buf);
	}

	udp_packet_wrap& operator<<(BOOST_RV_REF(data_buf_type) buf)
	{
		change_state(E_state_alive);
		if(is_complete())
		{
			return *this;
		}
		prv_collect(boost::forward<data_buf_type>(buf));
		return *this;
	}

	udp_packet_wrap& operator<<(const data_buf_type& buf)
	{
		change_state(E_state_alive);
		if(is_complete())
		{
			return *this;
		}
		prv_collect(buf);
		return *this;
	}

	template<typename Packet>
	const udp_packet_wrap& operator<<(BOOST_RV_REF(Packet) pak)
	{
		typedef Packet pak_type;

		if(_id || _count)
		{
			clear();
		}

		split_packet(gen_id(), boost::forward<pak_type>(pak));
	}

	template<typename Packet>
	const udp_packet_wrap& operator<<(const Packet& pak)
	{
		typedef Packet pak_type;

		if(_id || _count)
		{
			clear();
		}

		split_packet(gen_id(), pak);
	}

	template<typename Packet>
	const udp_packet_wrap& operator>>(Packet& pak) const
	{
		if(!_bcomplete)
		{
			return *this;
		}

		merge_data(pak);
		return *this;
	}

	u32 check_state(void)
	{
		if(_state == E_state_alive)
		{
			change_state(E_state_wait_check);
			return E_state_alive;
		}

		if(_state == E_state_wait_check)
		{
			change_state(E_state_die);
			return E_state_wait_check;
		}

		return E_state_die;
	}

private:

	id_type gen_id(void)
	{
		ids::id_generator<id_type> id_gen;
		return id_gen();
	}

	void buf_count(size_type size)
	{
		_count = (size + udp_packet_type::E_MAX_DATA_LENGTH - 1) / udp_packet_type::E_MAX_DATA_LENGTH;
		assert(_count < std::numeric_limits<u16>::max());
	}

	template<typename Packet>
	void split_packet(const id_type& id, const Packet& pak)
	{
		typedef Packet pak_type;
		typedef typename pak_type::packet_tag_type tag_type;
		this_type::split_packet(id, pak, (tag_type*)0);
	}

	template<typename Packet>
	void split_packet(const id_type& id, const Packet& pak, const tag_static_type*)
	{
		typedef Packet packet_type;

		if(_id)
		{
			clear();
		}

		_id = id;

		const u8* data_buf = pak.data_buf();
		size_type count_size = pak.size();

		if(!(count_size && data_buf))
		{
			clear();
			return;
		}

		this_type::buf_count(count_size);

		data_buf_type tmp_buf(count_size, 0);
		memcpy(&tmp_buf[0], data_buf, count_size);

		_udp_packet_vt.resize(_count);

		size_type now_size = 0;
		size_type fix_size = 0;

		for(u16 i = 0, j = tmp_buf.size();
				i != _count;
				++i, j -= (j > udp_packet_type::E_MAX_DATA_LENGTH? udp_packet_type::E_MAX_DATA_LENGTH : j))
		{
			now_size = (j < udp_packet_type::E_MAX_DATA_LENGTH? j : udp_packet_type::E_MAX_DATA_LENGTH);
			_udp_packet_vt[i].set(_id, i, _count, udp_packet_type::E_static,
									&tmp_buf[fix_size], (&tmp_buf[fix_size]) + now_size);
			fix_size += now_size;
		}

		_now_count = _count;
		_type = udp_packet_type::E_static;
		_bcomplete = true;
	}

	template<typename Packet>
	void split_packet(const id_type& id, const Packet& pak, const tag_dynamic_type*)
	{
		typedef Packet packet_type;
		typedef typename packet_type::size_container_type size_container_type;
		typedef typename packet_type::net_info_type net_info_type;

		if(_id)
		{
			clear();
		}

		_id = id;

		size_container_type size_cont(pak.net_info_size(), pak.data_size());
		size_type size_size = size_cont.size();
		size_type info_size = size_cont.info_size();
		size_type data_size = size_cont.data_size();
		size_type count_size = size_size + size_cont.count_size();

		size_container_type bufed_size_cont(yggr::network::hton(size_cont));
		const net_info_type& bufed_info = pak.net_info();

		const u8* size_cont_buf = bufed_size_cont.buf();
		const u8* info_buf = bufed_info.buf();
		const u8* data_buf = pak.data_buf();

		if(!(size_size && info_size && data_size && size_cont_buf && info_buf && data_buf))
		{
			clear();
			return;
		}

		data_buf_type tmp_buf(count_size, 0);
		memcpy(&tmp_buf[0], size_cont_buf, size_size);
		memcpy(&tmp_buf[size_size], info_buf, info_size);
		memcpy(&tmp_buf[size_size + info_size], data_buf, data_size);

		this_type::buf_count(count_size);
		_udp_packet_vt.resize(_count);

		size_type now_size = 0;
		size_type fix_size = 0;

		for(u16 i = 0, j = tmp_buf.size();
				i != _count;
				++i, j -= (j > udp_packet_type::E_MAX_DATA_LENGTH? udp_packet_type::E_MAX_DATA_LENGTH : j))
		{
			now_size = ( j < udp_packet_type::E_MAX_DATA_LENGTH? j : udp_packet_type::E_MAX_DATA_LENGTH );
			_udp_packet_vt[i].set(_id, i, _count, udp_packet_type::E_dynamic,
									&tmp_buf[fix_size], (&tmp_buf[fix_size]) + now_size);
			fix_size += now_size;
		}

		_now_count = _count;
		_type = udp_packet_type::E_dynamic;
		_bcomplete = true;
	}

	void prv_collect(BOOST_RV_REF(data_buf_type) buf)
	{
		change_state(E_state_alive);
		udp_packet_type upak(boost::forward<data_buf_type>(buf));

		if(upak.empty())
		{
			return;
		}

		if(empty())
		{
			_id = upak.id();
			_count = upak.count();
			_type = upak.type();
			_udp_packet_vt.resize(_count);
		}
		else
		{
			assert(_id == upak.id());
			assert(_count == upak.count());
			assert(_type == upak.type());
		}

		assert(upak.idx() < _count);
		_udp_packet_vt[upak.idx()] = boost::move(upak);
		++_now_count;

		if(_now_count == _count)
		{
			_bcomplete = true;
		}
		change_state(E_state_alive);
	}

	void prv_collect(const data_buf_type& buf)
	{
		change_state(E_state_alive);
		udp_packet_type upak(buf);

		if(upak.empty())
		{
			return;
		}

		if(empty())
		{
			_id = upak.id();
			_count = upak.count();
			_type = upak.type();
			_udp_packet_vt.resize(_count);
		}
		else
		{
			assert(_id == upak.id());
			assert(_count == upak.count());
			assert(_type == upak.type());
		}

		assert(upak.idx() < _count);
		_udp_packet_vt[upak.idx()] = boost::move(upak);
		++_now_count;

		if(_now_count == _count)
		{
			_bcomplete = true;
		}
		change_state(E_state_alive);
	}

	template<typename Packet>
	void merge_data(Packet& pak) const
	{
		typedef Packet pak_type;
		typedef typename pak_type::packet_tag_type tag_type;
		assert(_type == check_packet<tag_type>::value);

		merge_data(pak, (tag_type*)0);
	}

	template<typename Packet>
	void merge_data(Packet& pak, const tag_static_type*) const
	{
		typedef Packet pak_type;

		data_buf_type tmp_buf;
		merge_data_buf(tmp_buf);
		if(tmp_buf.empty() || tmp_buf.size() != pak_type::s_buf_size())
		{
			return;
		}

		pak_type tmp_pak(boost::move(tmp_buf));
		pak.swap(tmp_pak);
	}

	template<typename Packet>
	void merge_data(Packet& pak, const tag_dynamic_type*) const
	{
		typedef Packet pak_type;
		typedef typename pak_type::size_container_type size_cont_type;
		typedef typename pak_type::net_info_type net_info_type;

		data_buf_type tmp_buf;
		merge_data_buf(tmp_buf);

		if(tmp_buf.size() < size_cont_type::s_size())
		{
			return;
		}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

		size_cont_type size_cont(
						boost::move(
							yggr::network::ntoh(
									size_cont_type(
														&tmp_buf[0],
														(&tmp_buf[0]) + size_cont_type::s_size()
													)
												)
									)
								);
#else
		size_cont_type size_cont(
									yggr::network::ntoh(
											size_cont_type(
																&tmp_buf[0],
																(&tmp_buf[0]) + size_cont_type::s_size()
															)
														)
									);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		if(!size_cont.effective() || tmp_buf.size() <  size_cont_type::s_size() + size_cont.count_size())
		{
			return;
		}

		net_info_type net_info(
									&tmp_buf[size_cont_type::s_size()],
									(&tmp_buf[size_cont_type::s_size()]) +  size_cont.info_size()
								);
		pak.swap(
					pak_type(
								net_info,
								(&tmp_buf[size_cont_type::s_size()]) + size_cont.info_size(),
								(&tmp_buf[size_cont_type::s_size()]) + size_cont.count_size()
							)
				);
	}

	void merge_data_buf(data_buf_type& buf) const
	{
		size_type data_size = (_count - 1) * udp_packet_type::E_MAX_DATA_LENGTH + _udp_packet_vt[_count - 1].data_size();
		buf.resize(data_size);

		size_type offset = 0;

		for(size_type i = 0; i != _count; ++i)
		{
			memcpy(&buf[offset], &(_udp_packet_vt[i].data_buf()[0]), _udp_packet_vt[i].data_size());
			offset += _udp_packet_vt[i].data_size();
		}
	}

	void change_state(u32 new_sat)
	{
		u32 old_sat = 0;
		do
		{
			old_sat = _state.load();
		} while((old_sat != new_sat) && (!_state.compare_exchange_strong(old_sat, new_sat)));
	}

private:
	id_type _id;
	u16 _now_count;
	u16 _count;
	u16 _type;
	bool _bcomplete;
	udp_packet_vt_type _udp_packet_vt;
	state_type _state;
};

} // nemspace network
} // namespace yggr

#endif //__YGGR_NETWORK_UDP_PACKET_WARP_HPP__
