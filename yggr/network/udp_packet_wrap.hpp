//udp_packet_wrap.hpp

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

#ifndef __YGGR_NETWORK_UDP_PACKET_WARP_HPP__
#define __YGGR_NETWORK_UDP_PACKET_WARP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/mplex/limits.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/network/udp_pak_info.hpp>
#include <yggr/network/hn_conv.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <yggr/encryption_tool/md5_tool.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

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

private:
	typedef typename boost::range_value<data_buf_type>::type data_buf_val_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(data_buf_val_type) == 1)>));

public:
	YGGR_STATIC_CONSTANT(u32, E_INFO_SIZE = pak_id_type::E_LENGTH + pak_header_type::E_LENGTH);
	YGGR_STATIC_CONSTANT(u32, E_VERIFY_SIZE = encryption_tool::tool_buf_size<verify_type>::value);
	YGGR_STATIC_CONSTANT(u32, E_MAX_LENGTH = 1024);
	YGGR_STATIC_CONSTANT(
		u32,
		E_MAX_DATA_LENGTH = E_MAX_LENGTH - pak_id_type::E_LENGTH - pak_header_type::E_LENGTH - E_VERIFY_SIZE);
	YGGR_STATIC_CONSTANT(u32, E_LIMIT_LENGTH = 1100);


	YGGR_STATIC_CONSTANT(u16, E_unknown = 0);
	YGGR_STATIC_CONSTANT(u16, E_static = E_unknown + 1);
	YGGR_STATIC_CONSTANT(u16, E_dynamic = E_unknown + 2);

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
		: _id(id), _header(idx, count, type, buf.size()), _buf(boost::move(buf))
	{
	}

	template<typename InputIter>
	udp_packet(const id_type& id, u16 idx,  u16 count, u16 type, InputIter s, InputIter e)
		: _id(id), _buf(s, e)

	{
		_header.idx = idx;
		_header.count = count;
		_header.size = _buf.size();
		_header.type = type;
	}

	// dont wright rvref's code, because the last operator is copy
	//udp_packet(BOOST_RV_REF(data_buf_type) buf);

	udp_packet(const data_buf_type& buf)
	{
		bool bright = from_buffer(buf);
		if(!bright)
		{
			_buf.clear();
		}
	}

	udp_packet(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id)),
			_header(boost::move(right._header)),
			_buf(boost::move(right._buf))
	{
	}

	udp_packet(const this_type& right)
		: _id(right._id), _header(right._header), _buf(right._buf)
	{
	}

	~udp_packet(void)
	{
		_buf.clear();
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
		copy_or_move_or_swap(_header, boost::move(right._header));
		copy_or_move_or_swap(_buf, boost::move(right._buf));

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
		yggr::swap(_header, right._header);
		yggr::swap(_buf, right._buf);
	}

public:
	inline bool compare_eq(const id_type& id) const
	{
		return _id == id;
	}

	inline bool compare_eq(const pak_id_type& id) const
	{
		return _id == id;
	}

public:
	inline const id_type& id(void) const
	{
		return _id;
	}

	inline pak_header_type& header(void)
	{
		return _header;
	}

	inline const pak_header_type& header(void) const
	{
		return _header;
	}

	inline data_buf_type& data_buf(void)
	{
		return _buf;
	}

	inline const data_buf_type& data_buf(void) const
	{
		return _buf;
	}

	inline size_type data_size(void) const
	{
		return _buf.size();
	}

	inline u16 idx(void) const
	{
		return _header.idx;
	}

	inline u16 count(void) const
	{
		return _header.count;
	}

	inline u16 type(void) const
	{
		return _header.type;
	}

	inline void set(const id_type& id, u16 idx, u16 count, u16 type, BOOST_RV_REF(data_buf_type) buf)
	{
		_id = id;
		_header.idx = idx;
		_header.count = count;
		_header.type = type;
		copy_or_move_or_swap(_buf, boost::move(buf));
		_header.size = _buf.size();
	}

	inline void set(const id_type& id, u16 idx, u16 count, u16 type, const data_buf_type& buf)
	{
		_id = id;
		_header.idx = idx;
		_header.count = count;
		_header.type = type;
		_buf = buf;
		_header.size = _buf.size();
	}

	template<typename InputIter> inline
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
		if(_buf.size()) memcpy(&tmp_buf[pak_id_type::E_LENGTH + pak_header_type::E_LENGTH], &_buf[0], _buf.size());

		verify_type tool;
		check_type chk = tool.encrypt(tmp_buf);

		buf.resize(tmp_buf.size() + this_type::E_VERIFY_SIZE);
		if(buf.size())
		{
			memcpy(&buf[0], &tmp_buf[0], tmp_buf.size());
			memcpy(&buf[tmp_buf.size()], &chk[0], this_type::E_VERIFY_SIZE);
		}
	}

	inline data_buf_type to_buffer(void) const
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

	inline bool empty(void) const
	{
		return _id == pak_id_type() || _buf.empty();
	}

private:
	pak_id_type _id;
	pak_header_type _header;
	data_buf_type _buf;
};

// operator ==
template<typename Buffer, typename Verify> inline
bool operator==(const typename udp_packet<Buffer, Verify>::id_type& l,
				const udp_packet<Buffer, Verify>& r)
{
	return r.compare_eq(l);
}

template<typename Buffer, typename Verify> inline
bool operator==(const udp_packet<Buffer, Verify>& l,
				const typename udp_packet<Buffer, Verify>::id_type& r)
{
	return l.compare_eq(r);
}

template<typename Buffer, typename Verify> inline
bool operator==(const typename udp_packet<Buffer, Verify>::pak_id_type& l,
				const udp_packet<Buffer, Verify>& r)
{
	return r.compare_eq(l);
}

template<typename Buffer, typename Verify> inline
bool operator==(const udp_packet<Buffer, Verify>& l,
				const typename udp_packet<Buffer, Verify>::pak_id_type& r)
{
	return l.compare_eq(r);
}

// operator !=
template<typename Buffer, typename Verify> inline
bool operator!=(const typename udp_packet<Buffer, Verify>::id_type& l,
				const udp_packet<Buffer, Verify>& r)
{
	return !r.compare_eq(l);
}

template<typename Buffer, typename Verify> inline
bool operator!=(const udp_packet<Buffer, Verify>& l,
				const typename udp_packet<Buffer, Verify>::id_type& r)
{
	return !l.compare_eq(r);
}

template<typename Buffer, typename Verify> inline
bool operator!=(const typename udp_packet<Buffer, Verify>::pak_id_type& l,
				const udp_packet<Buffer, Verify>& r)
{
	return !r.compare_eq(l);
}

template<typename Buffer, typename Verify> inline
bool operator!=(const udp_packet<Buffer, Verify>& l,
				const typename udp_packet<Buffer, Verify>::pak_id_type& r)
{
	return !l.compare_eq(r);
}

} // namespace network
} // naemspace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

template<typename Buffer, typename Verify> inline
void swap(udp_packet<Buffer, Verify>& l, udp_packet<Buffer, Verify>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::swap_support::swap;
} // namespace boost

//------------------------udp_packet_wrap------------------------------

namespace yggr
{
namespace network
{
namespace detail
{

template<typename UdpPacke, typename T >
struct udp_packet_wrap_check_packet
	: public boost::mpl::integral_c<u32, UdpPacke::E_unknown>
{
};

template<typename UdpPacke>
struct udp_packet_wrap_check_packet<UdpPacke, yggr::network::type_traits::tag_pak_static >
	: public boost::mpl::integral_c<u32, UdpPacke::E_static>
{
};

template<typename UdpPacke>
struct udp_packet_wrap_check_packet<UdpPacke, yggr::network::type_traits::tag_pak_dynamic>
	: public boost::mpl::integral_c<u32, UdpPacke::E_dynamic>
{
};

} // namespace detail
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{

template<typename Buffer, typename Verify = encryption_tool::md5_tool >
class udp_packet_wrap
{
public:
	typedef Buffer data_buf_type;
	typedef typename boost::range_value<data_buf_type>::type data_buf_val_type;

	typedef yggr::u64 id_type;
	typedef Verify verify_type;
	typedef udp_packet<data_buf_type, verify_type> udp_packet_type;
	typedef ::yggr::vector<udp_packet_type> udp_packet_vt_type;
	typedef typename udp_packet_vt_type::iterator udp_packet_vt_iter_type;
	typedef typename udp_packet_vt_type::const_iterator udp_packet_vt_c_iter_type;

	YGGR_STATIC_CONSTANT(u32, E_MAX_SINGLE_BUF_LENGTH = udp_packet_type::E_MAX_LENGTH);

	YGGR_STATIC_CONSTANT(u32, E_state_die = 0);
	YGGR_STATIC_CONSTANT(u32, E_state_wait_check = E_state_die + 1);
	YGGR_STATIC_CONSTANT(u32, E_state_alive = E_state_die + 2);

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(data_buf_val_type) == 1)>));

private:
	typedef u32 state_type;

	typedef yggr::network::type_traits::tag_pak_static tag_static_type;
	typedef yggr::network::type_traits::tag_pak_dynamic tag_dynamic_type;

private:
	typedef udp_packet_wrap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	udp_packet_wrap(void)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknown),
			_bcompleted(false),
			_state(E_state_alive)
	{
	}

	udp_packet_wrap(BOOST_RV_REF(data_buf_type) buf)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknown),
			_bcompleted(false)
	{
		this_type::prv_collect(boost::move(buf));
	}

	udp_packet_wrap(const data_buf_type& buf)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknown),
			_bcompleted(false)
	{
		this_type::prv_collect(buf);
	}

	template<typename Packet>
	udp_packet_wrap(const Packet& pak)
		: _id(),
			_now_count(0),
			_count(0),
			_type(udp_packet_type::E_unknown),
			_bcompleted(false),
			_state(E_state_alive)
	{
		this_type::prv_split_packet(this_type::prv_s_gen_id(), pak);
	}

	udp_packet_wrap(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id)),
			_now_count(boost:: move(right._now_count)),
			_count(boost::move(right._count)),
			_type(boost::move(right._type)),
			_bcompleted(boost::move(right._bcompleted)),
			_udp_packet_vt(boost::move(right._udp_packet_vt)),
			_state(E_state_alive)
	{
	}

	udp_packet_wrap(const this_type& right)
		: _id(right._id),
			_now_count(right._now_count),
			_count(right._count),
			_type(right._type),
			_bcompleted(right._bcompleted),
			_udp_packet_vt(right._udp_packet_vt),
			_state(E_state_alive)
	{
	}

	~udp_packet_wrap(void)
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
		copy_or_move_or_swap(_now_count, boost::move(right._now_count));
		copy_or_move_or_swap(_count, boost::move(right._count));
		copy_or_move_or_swap(_type, boost::move(right._type));
		copy_or_move_or_swap(_bcompleted, boost::move(right._bcompleted));
		copy_or_move_or_swap(_udp_packet_vt, boost::move(right._udp_packet_vt));
		copy_or_move_or_swap(_state, boost::move(right._state));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_id = right._id;
		_now_count = right._now_count;
		_count = right._count;
		_type = right._type;
		_bcompleted = right._bcompleted;
		_udp_packet_vt = right._udp_packet_vt;
		_state = right._state;

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

		::yggr::swap(_id, right._id);
		::yggr::swap(_now_count, right._now_count);
		::yggr::swap(_count, right._count);
		::yggr::swap(_type, right._type);
		::yggr::swap(_bcompleted, right._bcompleted);
		::yggr::swap(_udp_packet_vt, right._udp_packet_vt);
		::yggr::swap(_state, right._state);
	}

public:
	inline bool empty(void) const
	{
#ifdef _DEBUG
		if(_id == 0)
		{
			assert(_now_count == 0);
			assert(_count == 0);
			assert(_type == udp_packet_type::E_unknown);
			assert(_bcompleted == false);
			assert(_udp_packet_vt.empty());
		}
#endif //_DEBUG
		return _id == 0;
	}

	inline void clear(void)
	{
		_id = 0;
		_now_count = 0;
		_count = 0;
		_type = udp_packet_type::E_unknown;
		_bcompleted = false;
		_udp_packet_vt.clear();
		_state = E_state_alive;
	}

	inline bool is_completed(void) const
	{
		return _bcompleted;
	}

public:
	inline data_buf_type operator[](size_type idx) const
	{
		data_buf_type buf;
		_udp_packet_vt[idx].to_buffer(buf);
		return buf;
	}

	inline data_buf_type& get_data_buf(size_type idx, data_buf_type& out) const
	{
		_udp_packet_vt[idx].to_buffer(out);
		return out;
	}

public:
	inline id_type& id(void)
	{
		return _id;
	}

	inline const id_type& id(void) const
	{
		return _id;
	}

	inline u16 now_count(void) const
	{
		return _now_count;
	}

	inline u16 count(void) const
	{
		return _count;
	}

	inline u16 type(void) const
	{
		return _type;
	}

	inline udp_packet_vt_type& udp_packets(void)
	{
		return _udp_packet_vt;
	}

	inline const udp_packet_vt_type& udp_packets(void) const
	{
		return _udp_packet_vt;
	}

public:
	template<typename Packet> inline
	Packet get_packet(void) const
	{
		typedef Packet pak_type;

		pak_type pak;
		get_packet(pak);
		return pak;
	}

	template<typename Packet> inline
	Packet& get_packet(Packet& pak) const
	{
		typedef Packet pak_type;

		if(!_bcompleted)
		{
			return pak;
		}

		this_type::prv_merge_data(pak);
		return pak;
	}

	// dont write rvref code, because the last operator is copy
	//template<typename Packet>
	//const udp_packet_wrap& operator<<(BOOST_RV_REF(Packet) pak);

	template<typename Packet> inline
	void split_packet(const Packet& pak)
	{
		typedef Packet pak_type;

		if(_id || _count)
		{
			this_type::clear();
		}

		this_type::prv_split_packet(this_type::prv_s_gen_id(), pak);
	}

public:
	inline void collect(BOOST_RV_REF(data_buf_type) buf)
	{
		if(!_bcompleted)
		{
			this_type::prv_collect(boost::move(buf));
		}
	}

	inline void collect(const data_buf_type& buf)
	{
		if(!_bcompleted)
		{
			this_type::prv_collect(buf);
		}
	}

	inline void collect(BOOST_RV_REF(this_type) right)
	{
		if(!_bcompleted)
		{
			this_type::prv_collect(boost::move(right));
		}
	}

	inline void collect(const this_type& right)
	{
		if(!_bcompleted)
		{
			this_type::prv_collect(right);
		}
	}

public:
	inline u32 check_state(void)
	{
		switch(_state)
		{
		case E_state_alive:
			change_state(E_state_wait_check);
			return E_state_alive;

		case E_state_wait_check:
			change_state(E_state_die);
			return E_state_wait_check;

		default:
			return E_state_die;
		}
	}

private:
	inline static id_type prv_s_gen_id(void)
	{
		static ids::id_generator<id_type> id_gen;
		return id_gen();
	}

	inline void buf_count(size_type size)
	{
		_count = (size + udp_packet_type::E_MAX_DATA_LENGTH - 1) / udp_packet_type::E_MAX_DATA_LENGTH;
		assert(_count < mplex::numeric_limits<u16>::max_type::value);
	}

	template<typename Packet> inline
	void prv_split_packet(const id_type& id, const Packet& pak)
	{
		typedef Packet pak_type;
		typedef typename pak_type::packet_tag_type tag_type;
		this_type::prv_split_packet(id, pak, static_cast<tag_type*>(0));
	}

	template<typename Packet>
	void prv_split_packet(const id_type& id, const Packet& pak, const tag_static_type*)
	{
		typedef Packet packet_type;

		if(_id)
		{
			this_type::clear();
		}

		_id = id;

		const u8* data_buf = pak.data_buf();
		size_type count_size = pak.size();

		if(!(count_size && data_buf))
		{
			this_type::clear();
			return;
		}

		this_type::buf_count(count_size);

		_udp_packet_vt.resize(_count);

		size_type now_size = 0;
		size_type fix_size = 0;

		for(u16 i = 0, j = count_size;
				i != _count;
				++i, j -= (j > udp_packet_type::E_MAX_DATA_LENGTH? udp_packet_type::E_MAX_DATA_LENGTH : j))
		{
			now_size = (j < udp_packet_type::E_MAX_DATA_LENGTH? j : udp_packet_type::E_MAX_DATA_LENGTH);
			_udp_packet_vt[i].set(_id, i, _count, udp_packet_type::E_static,
									&data_buf[fix_size], (&data_buf[fix_size]) + now_size);
			fix_size += now_size;
		}

		_now_count = _count;
		_type = udp_packet_type::E_static;
		_bcompleted = true;
	}

	template<typename Packet>
	void prv_split_packet(const id_type& id, const Packet& pak, const tag_dynamic_type*)
	{
		typedef Packet packet_type;
		typedef typename packet_type::size_container_type size_container_type;
		typedef typename packet_type::net_info_type net_info_type;

		if(_id)
		{
			this_type::clear();
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
			this_type::clear();
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
		_bcompleted = true;
	}

	inline void prv_collect(BOOST_RV_REF(data_buf_type) buf)
	{
		udp_packet_type upak(boost::move(buf));
		this_type::prv_collect_detail(upak);
	}

	inline void prv_collect(const data_buf_type& buf)
	{
		udp_packet_type upak(buf);
		this_type::prv_collect_detail(upak);
	}

	void prv_collect_detail(udp_packet_type& upak)
	{
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
#ifdef _DEBUG
		else
		{
			assert(_id == upak.id());
			assert(_count == upak.count());
			assert(_type == upak.type());
		}
#endif // _DEBUG

		assert(upak.idx() < _count);
		_udp_packet_vt[upak.idx()].swap(upak);
		++_now_count;

		_bcompleted = (_now_count == _count);
		change_state(E_state_alive);
	}

	void prv_collect(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if((this == &right_chk) || (_id != right.id()))
		{
			return;
		}

		assert(_count == right._count);
		assert(_type == right._type);
		assert((_now_count + right._now_count <= _count));

		for(std::size_t i = 0, isize = _count; i != isize; ++i)
		{
			assert(!((!_udp_packet_vt[i].empty())
						&& (!right._udp_packet_vt[i].empty())));
			if(_udp_packet_vt[i].empty() && !right._udp_packet_vt[i].empty())
			{
				copy_or_move_or_swap(_udp_packet_vt[i], right._udp_packet_vt[i]);
			}
		}

		_now_count += right._now_count;
		change_state(E_state_alive);
	}

	void prv_collect(const this_type& right)
	{
		if((this == &right) || (_id != right.id()))
		{
			return;
		}

		assert(_count == right._count);
		assert(_type == right._type);
		assert((_now_count + right._now_count <= _count));

		for(std::size_t i = 0, isize = _count; i != isize; ++i)
		{
			assert(!((!_udp_packet_vt[i].empty())
						&& (!right._udp_packet_vt[i].empty())));
			if(_udp_packet_vt[i].empty() && !right._udp_packet_vt[i].empty())
			{
				_udp_packet_vt[i] = right._udp_packet_vt[i];
			}
		}

		_now_count += right._now_count;
		change_state(E_state_alive);
	}

	template<typename Packet> inline
	void prv_merge_data(Packet& pak) const
	{
		typedef Packet pak_type;
		typedef typename pak_type::packet_tag_type tag_type;
		assert((_type == detail::udp_packet_wrap_check_packet<udp_packet_type, tag_type>::value));

		prv_merge_data(pak, (tag_type*)0);
	}

	template<typename Packet> inline
	void prv_merge_data(Packet& pak, const tag_static_type*) const
	{
		typedef Packet pak_type;

		data_buf_type tmp_buf;
		merge_data_buf(tmp_buf);
		if(tmp_buf.empty() || tmp_buf.size() != pak_type::buf_size())
		{
			return;
		}

		pak_type tmp_pak(boost::move(tmp_buf));
		pak.swap(tmp_pak);
	}

	template<typename Packet>
	void prv_merge_data(Packet& pak, const tag_dynamic_type*) const
	{
		typedef Packet pak_type;
		typedef typename pak_type::size_container_type size_cont_type;
		typedef typename pak_type::net_info_type net_info_type;

		data_buf_type tmp_buf;
		merge_data_buf(tmp_buf);

		if(tmp_buf.size() < size_cont_type::size())
		{
			return;
		}

		size_cont_type size_cont(
						yggr::network::ntoh(
							size_cont_type(
								&tmp_buf[0],
								(&tmp_buf[0]) + size_cont_type::size() ) ) );

		if(!size_cont.effective() || tmp_buf.size() <  size_cont_type::size() + size_cont.count_size())
		{
			return;
		}

		net_info_type net_info( &tmp_buf[size_cont_type::size()],
									(&tmp_buf[size_cont_type::size()]) +  size_cont.info_size() );

		pak_type tmp_pak(net_info,
							(&tmp_buf[size_cont_type::size()]) + size_cont.info_size(),
							(&tmp_buf[size_cont_type::size()]) + size_cont.count_size() );
		pak.swap(tmp_pak);
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

	inline void change_state(state_type new_sat)
	{
		if(_state != new_sat)
		{
			_state = new_sat;
		}
	}

private:
	id_type _id;
	u16 _now_count;
	u16 _count;
	u16 _type;
	bool _bcompleted;
	udp_packet_vt_type _udp_packet_vt;
	state_type _state;
};

// non-member function

//template<typename Buffer, typename Verify> inline
//udp_packet_wrap<Buffer, Verify>&
//	operator<<(udp_packet_wrap<Buffer, Verify>& upw,
//				BOOST_RV_REF_BEG
//					typename
//						udp_packet_wrap
//						<
//							Buffer,
//							Verify
//						>::data_buf_type
//				BOOST_RV_REF_END buf)
//{
//	upw.collect(boost::move(buf));
//	return upw;
//}
//
//template<typename Buffer, typename Verify> inline
//udp_packet_wrap<Buffer, Verify>&
//	operator<<(udp_packet_wrap<Buffer, Verify>& upw,
//				const
//					typename
//						udp_packet_wrap
//						<
//							Buffer,
//							Verify
//						>::data_buf_type& buf)
//{
//	upw.collect(buf);
//	return upw;
//}

template<typename Buffer, typename Verify> inline
udp_packet_wrap<Buffer, Verify>&
	operator<<(udp_packet_wrap<Buffer, Verify>& upw,
				BOOST_RV_REF(Buffer) buf)
{
	upw.collect(boost::move(buf));
	return upw;
}

template<typename Buffer, typename Verify> inline
udp_packet_wrap<Buffer, Verify>&
	operator<<(udp_packet_wrap<Buffer, Verify>& upw,
				const Buffer& buf)
{
	upw.collect(buf);
	return upw;
}


template<typename Buffer, typename Verify> inline
udp_packet_wrap<Buffer, Verify>&
	operator<<(udp_packet_wrap<Buffer, Verify>& upw,
				BOOST_RV_REF_BEG
						udp_packet_wrap
						<
							Buffer,
							Verify
						>
				BOOST_RV_REF_END rupw)
{
	upw.collect(boost::move(rupw));
	return upw;
}

template<typename Buffer, typename Verify> inline
udp_packet_wrap<Buffer, Verify>&
	operator<<(udp_packet_wrap<Buffer, Verify>& upw,
				const udp_packet_wrap<Buffer, Verify>& rupw)
{
	upw.collect(rupw);
	return upw;
}

// dont write rvref code, because the last operator is copy
//template<typename Packet>
//const udp_packet_wrap& operator<<(BOOST_RV_REF(Packet) pak);

template<typename Buffer, typename Verify, typename Packet> inline
const udp_packet_wrap<Buffer, Verify>&
	operator<<(udp_packet_wrap<Buffer, Verify>& upw, const Packet& pak)
{
	upw.split_packet(pak);
	return upw;
}

template<typename Buffer, typename Verify, typename Packet> inline
const udp_packet_wrap<Buffer, Verify>&
	operator>>(const udp_packet_wrap<Buffer, Verify>& upw, Packet& pak)
{
	upw.get_packet(pak);
	return upw;
}


} // nemspace network
} // namespace yggr


namespace yggr
{
namespace network
{
namespace swap_support
{

template<typename Buffer, typename Verify> inline
void swap(udp_packet_wrap<Buffer, Verify>& l, udp_packet_wrap<Buffer, Verify>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::swap_support::swap;
} // namespace boost

#endif //__YGGR_NETWORK_UDP_PACKET_WARP_HPP__
