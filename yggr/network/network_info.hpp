//network_info.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_INFO_HPP__
#define __YGGR_NETWORK_NETWORK_INFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/time/time.hpp>
#include <yggr/network/hn_conv.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/strict_sizeof.hpp>
#include <yggr/mplex/limits.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <boost/static_assert.hpp>
#include <boost/ref.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/iterator/iterator_traits.hpp>
#include <boost/functional/hash/hash.hpp>

#include <memory>
#include <ctime>

namespace yggr
{
namespace network
{
namespace detail
{

// network_info_owner_id_creator
template<typename BufVal, typename OwnerID, bool is_class = boost::is_class<OwnerID>::value>
class network_info_owner_id_creator;

template<typename BufVal, typename OwnerID>
class network_info_owner_id_creator<BufVal, OwnerID, true>
{
public:
	typedef BufVal buf_val_type;
	typedef OwnerID owner_id_type;

public:
	inline owner_id_type operator()(const buf_val_type* ptr) const
	{
		return owner_id_type(ptr, ptr + (mplex::strict_sizeof<owner_id_type>::value));
	}
};

template<typename BufVal, typename OwnerID>
class network_info_owner_id_creator<BufVal, OwnerID, false>
{
public:
	typedef BufVal buf_val_type;
	typedef OwnerID owner_id_type;

public:
	inline owner_id_type operator()(const buf_val_type* ptr) const
	{
		owner_id_type owner_id;
		memcpy(&owner_id, ptr, (mplex::strict_sizeof<owner_id_type>::value));
		return owner_id;
	}
};

// network_info_owner_id_buffer_pointer
template<typename BufVal, typename OwnerID, 
			bool is_class = boost::is_class<OwnerID>::value>
class network_info_owner_id_buffer_pointer;

template<typename BufVal, typename OwnerID>
class network_info_owner_id_buffer_pointer<BufVal, OwnerID, true>
{
public:
	typedef BufVal buf_val_type;
	typedef OwnerID owner_id_type;

public:
	inline const buf_val_type* operator()(const owner_id_type& oid) const
	{
		return reinterpret_cast<const buf_val_type*>(&(*(oid.begin())));
	}

	inline buf_val_type* operator()(owner_id_type& oid) const
	{
		return reinterpret_cast<buf_val_type*>(&(*(oid.begin())));
	}
};

template<typename BufVal, typename OwnerID>
class network_info_owner_id_buffer_pointer<BufVal, OwnerID, false>
{
public:
	typedef BufVal buf_val_type;
	typedef OwnerID owner_id_type;

public:
	inline const buf_val_type* operator()(const owner_id_type& oid) const
	{
		return reinterpret_cast<const buf_val_type*>(&oid);
	}

	inline buf_val_type* operator()(owner_id_type& oid) const
	{
		return reinterpret_cast<buf_val_type*>(&oid);
	}
};

} // namespace detail
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{

template<typename ID>
class network_info
{
public:
	typedef ID owner_id_type;

	typedef yggr::s64 time_type;
	typedef yggr::string buf_type;
	typedef buf_type::value_type buf_val_type;

private:
	typedef yggr::time::time yggr_time_type;

private:
	BOOST_STATIC_ASSERT((yggr::mplex::strict_sizeof<buf_val_type>::value == 1));

	BOOST_STATIC_ASSERT((
		(yggr::mplex::strict_sizeof<owner_id_type>::value == 4)
		|| (yggr::mplex::strict_sizeof<owner_id_type>::value == 8)
		|| (yggr::mplex::strict_sizeof<owner_id_type>::value == 16)
		|| (yggr::mplex::strict_sizeof<owner_id_type>::value == 32) ));

	typedef typename buf_type::iterator buf_iter_type;
	typedef typename buf_type::const_iterator buf_citer_type;

	YGGR_STATIC_CONSTANT(u32, E_time_idx_s = 0);
	YGGR_STATIC_CONSTANT(u32, E_time_length = (mplex::strict_sizeof<time_type>::value));
	YGGR_STATIC_CONSTANT(u32, E_id_idx_s = E_time_length);
	YGGR_STATIC_CONSTANT(u32, E_id_length = (mplex::strict_sizeof<owner_id_type>::value));
	YGGR_STATIC_CONSTANT(u32, E_id_idx_len = E_id_length - 1);
	YGGR_STATIC_CONSTANT(u32, E_init_length = E_time_length + E_id_length);

private:
	typedef network_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	network_info(void)
		: _buf(E_init_length, 0)
	{
		time_type tm = ::yggr::network::hton(yggr_time_type::time_millisecond<time_type>());
		memcpy(&_buf[0], &tm, E_time_length);
	}

	template<typename Buffer>
	network_info(const Buffer& buf)
		: _buf(buf.begin(), buf.end())
	{
	}

	template<typename InputIter>
	network_info(InputIter s, InputIter e)
		: _buf(s, e)
	{
	}

	network_info(BOOST_RV_REF(buf_type) buf)
		: _buf(boost::move(buf))
	{
	}

	network_info(const buf_type& buf, size_type size = mplex::numeric_limits<size_type>::max_type::value)
		: _buf(buf.begin(), (size < buf.size()? buf.begin() + size : buf.end()))
	{
	}

	network_info(const owner_id_type& id_owner)
		: _buf(E_init_length, 0)
	{
		time_type tm = ::yggr::network::hton(yggr_time_type::time_millisecond<time_type>());
		memcpy(&_buf[0], &tm, E_time_length);

		owner_id_type id = ::yggr::network::hton(id_owner);
		memcpy(&_buf[E_id_idx_s], this_type::prv_s_get_owner_id_buf_ptr(id), E_id_length);
	}

	network_info(BOOST_RV_REF(this_type) right)
		: _buf(boost::move(right._buf))
	{
	}

	network_info(const this_type& right)
		: _buf(right._buf)
	{
	}

	~network_info(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		copy_or_move_or_swap(_buf, boost::move(buf));
		return *this;
	}

	this_type& operator=(const buf_type& buf)
	{
		_buf = buf;
		return *this;
	}

	// this foo gcc rv_ref<this_type> collision // test it
	template<typename Buffer>
	typename boost::enable_if<container_ex::is_container<Buffer>, this_type&>::type
		operator=(const Buffer& buf)
	{
		_buf.assign(boost::begin(buf), boost::end(buf));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_buf, boost::move(right._buf));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

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

		_buf.swap(right._buf);
	}

public:
	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (_buf == right._buf);
	}

public:
	template<typename InputIter> inline
	void assign(InputIter start, InputIter end)
	{
		_buf.assign(start, end);
	}

public:
	template<typename Time> inline
	bool time_alive(const Time& tm_step) const
	{
		time_type tm = yggr_time_type::time_millisecond<time_type>();

		time_type now_tm_step(std::abs(tm - this_type::time()));
		return now_tm_step <= tm_step;
	}

	inline bool effective(void) const
	{
		return
			(_buf.size() >= E_init_length)
			&& ((_buf.size() - E_time_length) % E_id_length == 0);
	}

	inline bool empty(void) const
	{
		return _buf.empty();
	}

	inline size_type size(void) const
	{
		assert( effective() ); // raw _buf foo checker

		return effective()? (_buf.size() - E_time_length) / E_id_length : 0;
	}

	inline size_type buf_size(void) const
	{
		return _buf.size();
	}

	inline void clear(void)
	{
		_buf.resize(E_init_length, 0);
	}

	inline u8* buf(void)
	{
		return 
			_buf.empty()? 
				static_cast<u8*>(0)
				: reinterpret_cast<u8*>(&_buf[0]);
	}

	inline const u8* buf(void) const
	{
		return 
			_buf.empty()? 
				static_cast<const u8*>(0) 
				: reinterpret_cast<const u8*>(&_buf[0]);
	}

	inline std::size_t hash(void) const
	{
		buf_type tmp_buf(_buf.begin() + E_time_length, _buf.end());
		boost::hash<buf_type> hasher;
		return hasher(tmp_buf);
	}

	inline time_type time(void) const
	{
		return ::yggr::network::ntoh(*(reinterpret_cast<const time_type*>(&_buf[0])));
	}

	inline void update_time(void)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return;
		}

		time_type tm = ::yggr::network::hton(yggr_time_type::time_millisecond<time_type>());

		memcpy(&_buf[0], &tm, E_time_length);
	}

	inline void update_time(time_type tm)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return;
		}

		tm = ::yggr::network::hton(tm);

		memcpy(&_buf[0], &tm, E_time_length);
	}

	inline void set_now_owner_id(const owner_id_type& nid)
	{
		assert( effective() ); // raw _buf foo checker

		if(!effective())
		{
			return;
		}

		owner_id_type id = ::yggr::network::hton(nid);
		memcpy(
			reinterpret_cast<buf_val_type*>(&(*(_buf.rbegin() + E_id_idx_len))),
			this_type::prv_s_get_owner_id_buf_ptr(id), 
			E_id_length);
	}

	inline owner_id_type owner_id(void) const
	{
		assert( effective() ); // raw _buf foo checker

		return 
			(!effective())? 
				owner_id_type()
				: ::yggr::network::ntoh(
					this_type::prv_s_get_owner_id(&(*(_buf.rbegin() + E_id_idx_len))));
	}

	inline owner_id_type owner_id(size_type nidx) const
	{
		assert( effective() ); // raw _buf foo checker

		return
			(!(effective() && nidx < this_type::size()))? 
				owner_id_type()
				: ::yggr::network::ntoh(
					this_type::prv_s_get_owner_id(&_buf[E_time_length + nidx * E_id_length]));
	}

	inline void push(const owner_id_type& nid)
	{
		assert( effective() ); // raw _buf foo checker

		if(!effective())
		{
			return;
		}
		owner_id_type id = ::yggr::network::hton(nid);
		const buf_val_type* ptr = this_type::prv_s_get_owner_id_buf_ptr(id);
		assert(ptr);
		if(!ptr)
		{
			return;
		}
		_buf.append(ptr, ptr + E_id_length);
	}

	inline owner_id_type pop(void)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return owner_id_type();
		}

		owner_id_type tid =
			::yggr::network::ntoh(
				this_type::prv_s_get_owner_id(
					&(*(_buf.rbegin() + E_id_idx_len))));
		_buf.erase(_buf.end() - E_id_length, _buf.end());
		return tid;
	}

	inline bool modify(size_type nidx, const owner_id_type& nid)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return false;
		}

		size_type idx = E_time_length + nidx * E_id_length;
		if(idx < _buf.size())
		{
			owner_id_type id = ::yggr::network::hton(nid);
			memcpy(&_buf[idx], this_type::prv_s_get_owner_id_buf_ptr(id), E_id_length);
			return true;
		}

		return false;
	}

	inline owner_id_type front(void) const
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return owner_id_type();
		}

		return 
			(!effective())? 
				owner_id_type()
				: ::yggr::network::ntoh(
						prv_s_get_owner_id(&_buf[E_id_idx_s]));
	}

	inline owner_id_type back(void) const
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return owner_id_type();
		}

		return 
			(!effective())? 
				owner_id_type()
				: ::yggr::network::ntoh(
					this_type::prv_s_get_owner_id(&(*(_buf.rbegin() + E_id_idx_len))));
	}

private:
	inline size_type owners_byte_size(void) const
	{
		return _buf.size() - E_time_length;
	}

	typedef 
		detail::network_info_owner_id_creator
		<
			buf_val_type,
			owner_id_type
		> owener_id_creator_type;

	inline static owner_id_type prv_s_get_owner_id(const buf_val_type* ptr)
	{
		assert(ptr);
		owener_id_creator_type creator;
		return creator(ptr);
	}

	typedef 
		detail::network_info_owner_id_buffer_pointer
		<
			buf_val_type, 
			owner_id_type
		> owner_id_buffer_pointer_type;
	
	inline static const buf_val_type* 
		prv_s_get_owner_id_buf_ptr(const owner_id_type& oid)
	{
		owner_id_buffer_pointer_type getter;
		return getter(oid);
	}

private:
	buf_type _buf;
};

// non-member function
template<typename ID> inline
bool operator==(const network_info<ID>& l, const network_info<ID>& r)
{
	return l.compare_eq(r);
}

template<typename ID> inline
bool operator!=(const network_info<ID>& l, const network_info<ID>& r)
{
	return !l.compare_eq(r);
}

} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

template<typename ID> inline 
void swap(network_info<ID>& left, network_info<ID>& right)
{ 
	left.swap(right); 
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

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace network
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename ID> inline
std::size_t hash_value(const ::yggr::network::network_info<ID>& info)
{
	return info.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename ID>
struct hash< ::yggr::network::network_info<ID> >
{
	typedef ::yggr::network::network_info<ID> type;

	inline std::size_t operator()(const type& info) const
	{
		return info.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_NETWORK_NETWORK_INFO_HPP__
