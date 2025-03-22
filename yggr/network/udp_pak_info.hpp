//udp_pak_info.hpp

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

#ifndef __YGGR_NETWORK_UDP_PAK_INFO_HPP__
#define __YGGR_NETWORK_UDP_PAK_INFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/container/resize.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/network/hn_conv.hpp>

#include <boost/ref.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <iterator>

namespace yggr
{
namespace network
{

class udp_pak_id
{
public:
	typedef u64 id_type;
public:
	YGGR_STATIC_CONSTANT(u32, E_LENGTH = sizeof(id_type));

private:
	typedef udp_pak_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	udp_pak_id(void)
	: _id()
	{
	}

//	udp_pak_id(BOOST_RV_REF(id_type) id)
//		: _id(boost::move(id))
//	{
//	}
//
//	udp_pak_id(const id_type& id)
//		: _id(id)
//	{
//	}

    udp_pak_id(id_type id)
		: _id(id)
	{
	}

	udp_pak_id(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id))
	{
	}

	udp_pak_id(const this_type& right)
		: _id(right._id)
	{
	}

	~udp_pak_id(void)
	{
	}

public:
//	this_type& operator=(BOOST_RV_REF(id_type) id)
//	{
//		copy_or_move_or_swap(_id, boost::move(id));
//		return *this;
//	}
//
//	this_type& operator=(const id_type& id)
//	{
//		_id = id;
//		return *this;
//	}

    this_type& operator=(id_type id)
	{
		_id = id;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		//copy_or_move_or_swap(_id, boost::move(right._id));
		_id = right_chk._id;

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_id = right._id;
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
	}

public:
//	inline bool compare_eq(const id_type& id) const
//	{
//		return _id == id;
//	}

    inline bool compare_eq(id_type id) const
	{
		return _id == id;
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (_id == right._id);
	}

public:
	inline operator id_type&(void)
	{
		return _id;
	}

	inline operator const id_type&(void) const
	{
		return _id;
	}

public:
	template<typename Buffer> inline
	const Buffer to_buffer(void) const
	{
		typedef Buffer data_buf_type;
		typedef typename data_buf_type::value_type value_type;

		id_type id(yggr::network::hton(_id));

		return
			data_buf_type(
				reinterpret_cast<const value_type*>(boost::addressof(const_cast<const id_type&>(id))),
				reinterpret_cast<const value_type*>(boost::addressof(const_cast<const id_type&>(id)) + 1));
	}

	template<typename Buffer> inline
	Buffer& to_buffer(Buffer& buf) const
	{
		typedef Buffer data_buf_type;
		typedef typename data_buf_type::value_type value_type;

		id_type id(yggr::network::hton(_id));

		buf.assign(
			reinterpret_cast<const value_type*>(boost::addressof(const_cast<const id_type&>(id))),
			reinterpret_cast<const value_type*>(boost::addressof(const_cast<const id_type&>(id)) + 1));

		return buf;
	}

	template<typename Buffer> inline
	bool from_buffer(const Buffer& buf)
	{
		return
			(!(buf.size() < this_type::E_LENGTH))
			&& (_id = yggr::network::ntoh(*reinterpret_cast<const id_type*>(boost::addressof(buf[0]))), true);
	}

	template<typename InputIter> inline
	bool from_buffer(InputIter s, InputIter e)
	{
		return
			(!(std::distance(s, e) < this_type::E_LENGTH))
			&& (_id = yggr::network::ntoh(*reinterpret_cast<const id_type*>(boost::addressof(*s))), true);
	}

private:
	id_type _id;
};

// operator==
inline bool operator==(const udp_pak_id::id_type& l, const udp_pak_id& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const udp_pak_id& l, const udp_pak_id::id_type& r)
{
	return l.compare_eq(r);
}

inline bool operator==(const udp_pak_id& l, const udp_pak_id& r)
{
	return l.compare_eq(r);
}

// operator!=
inline bool operator!=(const udp_pak_id::id_type& l, const udp_pak_id& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const udp_pak_id& l, const udp_pak_id::id_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator!=(const udp_pak_id& l, const udp_pak_id& r)
{
	return !l.compare_eq(r);
}

} //namespace network
} //namespace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(udp_pak_id)

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

//--------------udp_pak_header------------------------

namespace yggr
{
namespace network
{

class udp_pak_header
{
public:
	YGGR_STATIC_CONSTANT(u32, E_LENGTH = 8);

private:
	YGGR_STATIC_CONSTANT(u32, E_s_idx = 0);
	YGGR_STATIC_CONSTANT(u32, E_s_count = 2);
	YGGR_STATIC_CONSTANT(u32, E_s_size = 4);
	YGGR_STATIC_CONSTANT(u32, E_s_type = 6);

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
		: idx(boost::move(right.idx)),
			count(boost::move(right.count)),
			size(boost::move(right.size)),
			type(boost::move(right.type))
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

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(idx, boost::move(right.idx));
		copy_or_move_or_swap(count, boost::move(right.count));
		copy_or_move_or_swap(size, boost::move(right.size));
		copy_or_move_or_swap(type, boost::move(right.type));

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		idx = right.idx;
		count = right.count;
		size = right.size;
		type = right.type;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(idx, right.idx);
		yggr::swap(count, right.count);
		yggr::swap(size, right.size);
		yggr::swap(type, right.type);
	}

public:
	template<typename Buffer> inline
	Buffer to_buffer(void) const
	{
		typedef Buffer data_buf_type;

		data_buf_type buf;
		container::resize(buf, E_LENGTH);

		(*(reinterpret_cast<u16*>(&(buf[E_s_idx])))) = yggr::network::hton(idx);
		(*(reinterpret_cast<u16*>(&(buf[E_s_count])))) = yggr::network::hton(count);
		(*(reinterpret_cast<u16*>(&(buf[E_s_size])))) = yggr::network::hton(size);
		(*(reinterpret_cast<u16*>(&(buf[E_s_type])))) = yggr::network::hton(type);

		return buf;
	}

	template<typename Buffer> inline
	Buffer& to_buffer(Buffer& buf) const
	{
		typedef Buffer data_buf_type;

		container::resize(buf, E_LENGTH);

		(*(reinterpret_cast<u16*>(&(buf[E_s_idx])))) = yggr::network::hton(idx);
		(*(reinterpret_cast<u16*>(&(buf[E_s_count])))) = yggr::network::hton(count);
		(*(reinterpret_cast<u16*>(&(buf[E_s_size])))) = yggr::network::hton(size);
		(*(reinterpret_cast<u16*>(&(buf[E_s_type])))) = yggr::network::hton(type);

		return buf;
	}



	template<typename Buffer> inline
	bool from_buffer(const Buffer& buf)
	{
		if(buf.size() < E_LENGTH)
		{
			return false;
		}

		idx = yggr::network::ntoh(*reinterpret_cast<u16* const>(&(buf[E_s_idx])));
		count = yggr::network::ntoh(*reinterpret_cast<u16* const>(&(buf[E_s_count])));
		size = yggr::network::ntoh(*reinterpret_cast<u16* const>(&(buf[E_s_size])));
		type = yggr::network::ntoh(*reinterpret_cast<u16* const>(&(buf[E_s_type])));

		return true;
	}

	template<typename InputIter> inline
	bool from_buffer(InputIter s, InputIter e)
	{
		if(std::distance(s, e) < E_LENGTH)
		{
			return false;
		}

		idx =
			yggr::network::ntoh(
				*reinterpret_cast<const u16*>(
					iterator_ex::advance_copy(
						reinterpret_cast<const u8*>(&(*s)),
						E_s_idx)));

		count =
			yggr::network::ntoh(
				*reinterpret_cast<const u16*>(
					iterator_ex::advance_copy(
						reinterpret_cast<const u8*>(&(*s)),
						E_s_count)));

		size =
			yggr::network::ntoh(
				*reinterpret_cast<const u16*>(
					iterator_ex::advance_copy(
						reinterpret_cast<const u8*>(&(*s)),
						E_s_size)));

		type =
			yggr::network::ntoh(
				*reinterpret_cast<const u16*>(
					iterator_ex::advance_copy(
						reinterpret_cast<const u8*>(&(*s)),
						E_s_type)));

		return true;
	}

public:
	u16 idx;
	u16 count;
	u16 size;
	u16 type;
};

} // nemspace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(udp_pak_header)

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

#endif // __YGGR_NETWORK_UDP_PAK_INFO_HPP__
