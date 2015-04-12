//udp_pak_info.hpp

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

#ifndef __YGGR_NETWORK_UDP_PAK_INFO_HPP__
#define __YGGR_NETWORK_UDP_PAK_INFO_HPP__

#include <memory>

#include <boost/asio.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/network/hn_conv.hpp>


namespace yggr
{
namespace network
{

class udp_pak_id
{
public:
	typedef u64 id_type;
public:
	enum
	{
		E_LENGTH = sizeof(id_type),
		E_compile_u32_len = 0xffffffff
	};

private:
	typedef udp_pak_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	udp_pak_id(void)
	: _id()
	{
	}

	udp_pak_id(const id_type& id)
		: _id(id)
	{
	}

	//template<typename Buffer>
	//udp_pak_id(BOOST_RV_REF(Buffer) buf)
	//{
	//	from_buffer(boost::forward<Buffer>(buf));
	//}

	//template<typename Buffer>
	//udp_pak_id(const Buffer& buf)
	//{
	//	from_buffer(buf);
	//}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	udp_pak_id(BOOST_RV_REF(this_type) right)
		: _id(boost::forward<id_type>(right._id))
	{
	}
#else
	udp_pak_id(BOOST_RV_REF(this_type) right)
		: _id(id_type())
	{
		this_type& right_ref = right;
		boost::swap(_id, right_ref._id);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	udp_pak_id(const this_type& right)
		: _id(right._id)
	{
	}

	~udp_pak_id(void)
	{
	}

	inline this_type& operator=(const id_type& id)
	{
		_id = yggr::network::hton(id);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
#else
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_id = right._id;
		return *this;
	}

	inline bool operator==(const id_type& id) const
	{
		return _id == id;
	}

	inline bool operator==(const this_type& right) const
	{
		return _id == right._id;
	}

	operator id_type&(void)
	{
		return _id;
	}

	operator const id_type&(void) const
	{
		return _id;
	}

	template<typename Buffer>
	void to_buffer(Buffer& buf) const
	{
		typedef Buffer data_buf_type;
		typedef typename data_buf_type::value_type value_type;

		id_type id(yggr::network::hton(_id));
		buf.assign((const value_type*)&id, (const value_type*)((&id) + 1));
	}

	template<typename Buffer>
	const Buffer to_buffer(void) const
	{
		typedef Buffer data_buf_type;
		typedef typename data_buf_type::value_type value_type;

		id_type id(yggr::network::hton(_id));
		return data_buf_type((const value_type*)&id, (const value_type*)((&id) + 1));
	}

	template<typename Buffer>
	bool from_buffer(const Buffer& buf)
	{
		if(buf.size() < E_LENGTH)
		{
			return false;
		}

		_id = yggr::network::ntoh(*((id_type*)&buf[0]));

		return true;
	}

	template<typename InputIter>
	bool from_buffer(InputIter s, InputIter e)
	{
		if(e - s < E_LENGTH)
		{
			return false;
		}
		_id = yggr::network::ntoh(*((id_type*)(&(*s))));
		return true;

	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
#else
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_id, right._id);
	}

private:
	id_type _id;
};

} //namespace network
} //namespace yggr

#define _YGGR_NETWORK_UDP_PAK_ID_SWAP() \
	inline void swap(yggr::network::udp_pak_id& l, yggr::network::udp_pak_id& r) { \
		l.swap(r); } \
	\
	inline void swap(BOOST_RV_REF(yggr::network::udp_pak_id) l, yggr::network::udp_pak_id& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::network::udp_pak_id& l, BOOST_RV_REF(yggr::network::udp_pak_id) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_NETWORK_UDP_PAK_ID_SWAP()
} // namespace std

namespace boost
{
	_YGGR_NETWORK_UDP_PAK_ID_SWAP()
} // namespace boost

#undef _YGGR_NETWORK_UDP_PAK_ID_SWAP

//--------------udp_pak_header------------------------

namespace yggr
{
namespace network
{

class udp_pak_header
{
public:
	enum
	{
		E_LENGTH = 8,
		E_compile_u32_len = 0xffffffff
	};

private:
	enum
	{
		E_s_idx = 0,
		E_s_count = 2,
		E_s_size = 4,
		E_s_type = 6,
		E_compile_u32_pos = 0xffffffff
	};

private:
	typedef udp_pak_header this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	udp_pak_header(void)
		: idx(0), count(0), size(0), type(0)
	{
	}

	udp_pak_header(u16 nidx, u16 ncount, u16 nsize, u16 ntype)
		: idx(nidx),
			count(ncount),
			size(nsize),
			type(ntype)
	{
	}

	udp_pak_header(BOOST_RV_REF(this_type) right)
		: idx(right.idx),
			count(right.count),
			size(right.size),
			type(right.type)
	{
	}

	udp_pak_header(const this_type& right)
		: idx(right.idx), count(right.count),
			size(right.size), type(right.type)
	{
	}

	~udp_pak_header(void)
	{
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		idx = right.idx;
		count = right.count;
		size = right.size;
		type = right.type;
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		idx = right.idx;
		count = right.count;
		size = right.size;
		type = right.type;
		return *this;
	}

	template<typename Buffer>
	void to_buffer(Buffer& buf) const
	{
		typedef Buffer data_buf_type;

		u16 nidx = yggr::network::hton(idx);
		u16 ncount = yggr::network::hton(count);
		u16 nsize = yggr::network::hton(size);
		u16 ntype = yggr::network::hton(type);

		buf.resize(E_LENGTH);
		memcpy(&buf[E_s_idx], &nidx, sizeof(u16));
		memcpy(&buf[E_s_count], &ncount, sizeof(u16));
		memcpy(&buf[E_s_size], &nsize, sizeof(u16));
		memcpy(&buf[E_s_type], &ntype, sizeof(u16));
	}

	template<typename Buffer>
	const Buffer to_buffer(void) const
	{
		typedef Buffer data_buf_type;
		data_buf_type buf(E_LENGTH, 0);

		u16 nidx = yggr::network::hton(idx);
		u16 ncount = yggr::network::hton(count);
		u16 nsize = yggr::network::hton(size);
		u16 ntype = yggr::network::hton(type);

		memcpy(&buf[E_s_idx], &nidx, sizeof(u16));
		memcpy(&buf[E_s_count], &ncount, sizeof(u16));
		memcpy(&buf[E_s_size], &nsize, sizeof(u16));
		memcpy(&buf[E_s_type], &ntype, sizeof(u16));

		return buf;
	}

	template<typename Buffer>
	bool from_buffer(const Buffer& buf)
	{
		if(buf.size() < E_LENGTH)
		{
			return false;
		}

		memcpy(&idx, &buf[E_s_idx], sizeof(u16));
		memcpy(&count, &buf[E_s_count], sizeof(u16));
		memcpy(&size, &buf[E_s_size], sizeof(u16));
		memcpy(&type, &buf[E_s_type], sizeof(u16));

		idx = yggr::network::ntoh(idx);
		count = yggr::network::ntoh(count);
		size = yggr::network::ntoh(size);
		type = yggr::network::ntoh(type);
		return true;
	}

	template<typename InputIter>
	bool from_buffer(InputIter s, InputIter e)
	{
		if(e - s < E_LENGTH)
		{
			return false;
		}

		memcpy(&idx, &((&(*s))[E_s_idx]), sizeof(u16));
		memcpy(&count, &((&(*s))[E_s_count]), sizeof(u16));
		memcpy(&size, &((&(*s))[E_s_size]), sizeof(u16));
		memcpy(&type, &((&(*s))[E_s_type]), sizeof(u16));

		idx = yggr::network::ntoh(idx);
		count = yggr::network::ntoh(count);
		size = yggr::network::ntoh(size);
		type = yggr::network::ntoh(type);
		return true;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(idx, right.idx);
		std::swap(count, right.count);
		std::swap(size, right.size);
		std::swap(type, right.type);
	}

public:
	u16 idx;
	u16 count;
	u16 size;
	u16 type;
};

} // nemspace network
} // namespace yggr

#define _YGGR_PP_NETWORK_UDP_PAK_HEADER_SWAP() \
	inline void swap(yggr::network::udp_pak_header& l, yggr::network::udp_pak_header& r) { \
		l.swap(r); } \
	\
	inline void swap(BOOST_RV_REF(yggr::network::udp_pak_header) l, yggr::network::udp_pak_header& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::network::udp_pak_header& l, BOOST_RV_REF(yggr::network::udp_pak_header) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_NETWORK_UDP_PAK_HEADER_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_UDP_PAK_HEADER_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_UDP_PAK_HEADER_SWAP

#endif // __YGGR_NETWORK_UDP_PAK_INFO_HPP__
