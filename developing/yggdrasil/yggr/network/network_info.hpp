//network_info.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_INFO_HPP__
#define __YGGR_NETWORK_NETWORK_INFO_HPP__

#include <limits>
#include <memory>
#include <ctime>

#include <boost/static_assert.hpp>
#include <boost/functional/hash/hash.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <yggr/move/move.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/time/time.hpp>
#include <yggr/network/hn_conv.hpp>
#include <yggr/mplex/iterator_to_value_t.hpp>
#include <yggr/mplex/strict_sizeof.hpp>


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

	BOOST_STATIC_ASSERT(((yggr::mplex::strict_sizeof<owner_id_type>::value == 4)
						|| (yggr::mplex::strict_sizeof<owner_id_type>::value == 8)
						|| (yggr::mplex::strict_sizeof<owner_id_type>::value == 16)
						|| (yggr::mplex::strict_sizeof<owner_id_type>::value == 32)));

	typedef typename buf_type::iterator buf_iter_type;
	typedef typename buf_type::const_iterator buf_citer_type;

	enum
	{
		E_time_idx_s = 0,
		E_time_length = yggr::mplex::strict_sizeof<time_type>::value,
		E_id_idx_s = E_time_length,
		E_id_length = yggr::mplex::strict_sizeof<owner_id_type>::value,
		E_id_idx_len = E_id_length - 1,
		E_init_length = E_time_length + E_id_length,
		E_compile_u32 = 0xffffffff
	};

	typedef network_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	network_info(void)
		: _buf(E_init_length, 0)
	{
		time_type tm = yggr::network::hton(yggr_time_type::s_time_millisecond<time_type>());
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

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	network_info(BOOST_RV_REF(buf_type) buf)
		: _buf(boost::forward<buf_type>(buf))
	{
	}
#else
	network_info(BOOST_RV_REF(buf_type) buf)
	{
		buf_type& rbuf = buf;
		_buf.swap(buf);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	network_info(const buf_type& buf, size_type size = std::numeric_limits<size_type>::max())
		: _buf(buf.begin(), (size < buf.size()? buf.begin() + size : buf.end()))
	{
	}

	network_info(const owner_id_type& id_owner)
		: _buf(E_init_length, 0)
	{
		time_type tm = yggr::network::hton(yggr_time_type::s_time_millisecond<time_type>());
		memcpy(&_buf[0], &tm, E_time_length);

		owner_id_type id = yggr::network::hton(id_owner);
		memcpy(&_buf[E_id_idx_s], prv_get_owner_id_buf_ptr(id), E_id_length);
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	network_info(BOOST_RV_REF(this_type) right)
		: _buf(boost::forward<buf_type>(right._buf))
	{
	}
#else
	network_info(BOOST_RV_REF(this_type) right)
	{
		buf_type& rbuf = right._buf;
		_buf.swap(rbuf);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	network_info(const this_type& right)
		: _buf(right._buf)
	{
	}

	~network_info(void)
	{
	}

	template<typename InputIter>
	void assign(InputIter start, InputIter end)
	{
		_buf.assign(start, end);
	}

	this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_buf = boost::forward<buf_type>(buf);
#else
		buf_type& ref_buf = buf;
		_buf.swap(ref_buf);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const buf_type& buf)
	{
		_buf = buf;
		return *this;
	}

//  this foo gcc rv_ref<this_type> collision
//	template<typename Buffer>
//	this_type& operator=(const Buffer& buf)
//	{
//		_buf.assign(buf.begin(), buf.end());
//		return *this;
//	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_buf = boost::forward<buf_type>(right._buf);
#else
		this_type& right_ref = right;
		_buf.swap(right_ref._buf);
#endif
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_buf = right._buf;
		return *this;
	}

//	void swap(BOOST_RV_REF(this_type) right)
//	{
//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
//		_buf.swap(right._buf);
//#else
//		this_type& right_ref = right;
//		_buf.swap(right_ref._buf);
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
//	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_buf.swap(right._buf);
	}

	bool operator==(const this_type& right) const
	{
		return _buf == right._buf;
	}

	bool operator!=(const this_type& right) const
	{
		return _buf != right._buf;
	}

	template<typename Time>
	bool time_alive(const Time& tm_step) const
	{
		time_type tm = yggr_time_type::s_time_millisecond<time_type>();

		time_type now_tm_step(std::abs(tm - this_type::time()));
		return now_tm_step <= tm_step;
	}

	bool effective(void) const
	{
		return (_buf.size() >= E_init_length)
				&& ((_buf.size() - E_time_length) % E_id_length == 0);
	}

	//bool empty(void) const
	//{
	//	return _buf.empty();
	//}

	size_type size(void) const
	{
		assert( effective() ); // raw _buf foo checker

		return effective() ? (_buf.size() - E_time_length) / E_id_length : 0;
	}

	size_type buf_size(void) const
	{
		return _buf.size();
	}

	void clear(void)
	{
		_buf.resize(E_init_length, 0);
	}

	u8* buf(void)
	{
		return _buf.empty()? 0 : reinterpret_cast<u8*>(&_buf[0]);
	}

	const u8* buf(void) const
	{
		return _buf.empty()? 0 : reinterpret_cast<const u8*>(&_buf[0]);
	}

	std::size_t hash(void) const
	{
		buf_type tmp_buf(_buf.begin() + E_time_length, _buf.end());
		boost::hash<buf_type> hasher;
		return hasher(tmp_buf);
	}

	time_type time(void) const
	{
		return yggr::network::ntoh(*(reinterpret_cast<const time_type*>(&_buf[0])));
	}

	void update_time(void)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return;
		}

		time_type tm = yggr::network::hton(yggr_time_type::s_time_millisecond<time_type>());
		
		memcpy(&_buf[0], &tm, E_time_length);
	}

	void set_now_owner_id(const owner_id_type& nid)
	{
		assert( effective() ); // raw _buf foo checker

		if(!effective())
		{
			return;
		}

		owner_id_type id = yggr::network::hton(nid);
		memcpy(reinterpret_cast<buf_val_type*>(&(*(_buf.rbegin() + E_id_idx_len))), prv_get_owner_id_buf_ptr(id), E_id_length);
	}

	owner_id_type owner_id(void) const
	{
		assert( effective() ); // raw _buf foo checker

		return (!effective())
					? owner_id_type()
					: yggr::network::ntoh(
							prv_get_owner_id(&(*(_buf.rbegin() + E_id_idx_len))));
	}

	void push(const owner_id_type& nid)
	{
		assert( effective() ); // raw _buf foo checker

		if(!effective())
		{
			return;
		}
		owner_id_type id = yggr::network::hton(nid);
		const buf_val_type* ptr = prv_get_owner_id_buf_ptr(id);
		assert(ptr);
		if(!ptr)
		{
			return;
		}
		_buf.append(ptr, ptr + E_id_length);
	}

	owner_id_type pop(void)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return owner_id_type();
		}
		
		owner_id_type tid = yggr::network::ntoh(
								prv_get_owner_id(
								reinterpret_cast
									<
										const owner_id_type*
									>(&(*(_buf.rbegin() + E_id_idx_len)))));
		_buf.erase(_buf.end() - E_id_length, _buf.end());
		return tid;
	}

	bool modify(size_type nidx, const owner_id_type& nid)
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return false;
		}

		size_type idx = E_time_length + nidx * E_id_length;
		if(idx < _buf.size())
		{
			owner_id_type id = yggr::network::hton(nid);
			memcpy(&_buf[idx], prv_get_owner_id_buf_ptr(id), E_id_length);
			return true;
		}

		return false;
	}

	owner_id_type src(void) const
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return owner_id_type();
		}

		return (!effective())
					? owner_id_type()
					: yggr::network::ntoh(
							prv_get_owner_id(&_buf[E_id_idx_s]));
	}

	owner_id_type dst(void) const
	{
		assert( effective() ); // raw _buf foo checker
		if(!effective())
		{
			return owner_id_type();
		}

		return (!effective())
					? owner_id_type()
					: yggr::network::ntoh(
							prv_get_owner_id(&(*(_buf.rbegin() + E_id_idx_len))));
	}

	//template<typename Handler>
	//typename Handler::result_type cypher(const Handler& handler)
	//{
	//	return handler(boost::ref(_buf));
	//}

private:
	size_type owners_byte_size(void) const
	{
		return _buf.size() - E_time_length;
	}

	template<bool is_class = boost::is_class<owner_id_type>::value, typename Nil_T = void>
	class owner_id_creator;

	template<typename Nil_T>
	class owner_id_creator<true, Nil_T>
	{
	public:
		owner_id_type operator()(const buf_val_type* ptr) const
		{
			return owner_id_type(ptr, ptr + E_id_length);
		}
	};

	template<typename Nil_T>
	class owner_id_creator<false, Nil_T>
	{
	public:
		owner_id_type operator()(const buf_val_type* ptr) const
		{
			owner_id_type owner_id;
			memcpy(&owner_id, ptr, E_id_length);
			return owner_id;
		}
	};

	owner_id_type prv_get_owner_id(const buf_val_type* ptr) const
	{
		typedef owner_id_creator<> creator_type;
		assert(ptr);
		creator_type creator;
		return creator(ptr);
	}

	template<bool is_class = boost::is_class<owner_id_type>::value, typename Nil_T = void>
	class owner_id_buffer_pointer;

	template<typename Nil_T>
	class owner_id_buffer_pointer<true, Nil_T>
	{
	public:
		const buf_val_type* operator()(const owner_id_type& oid) const
		{
			return reinterpret_cast<const buf_val_type*>(&(*(oid.begin())));
		}

		buf_val_type* operator()(owner_id_type& oid) const
		{
			return reinterpret_cast<buf_val_type*>(&(*(oid.begin())));
		}
	};

	template<typename Nil_T>
	class owner_id_buffer_pointer<false, Nil_T>
	{
	public:
		const buf_val_type* operator()(const owner_id_type& oid) const
		{
			return reinterpret_cast<const buf_val_type*>(&oid);
		}

		buf_val_type* operator()(owner_id_type& oid) const
		{
			return reinterpret_cast<buf_val_type*>(&oid);
		}
	};

	const buf_val_type* prv_get_owner_id_buf_ptr(const owner_id_type& oid) const
	{
		typedef owner_id_buffer_pointer<> pointer_getter_type;
		pointer_getter_type getter;
		return getter(oid);
	}

private:
	buf_type _buf;
};

} // namespace network
} // namespace yggr

#define _YGGR_TMP_PP_NETWORK_INFO_SWAP() \
	template<typename ID> \
	void swap(yggr::network::network_info<ID>& left, \
				yggr::network::network_info<ID>& right) { \
		left.swap(right); } \
	\
	template<typename ID> \
	void swap(BOOST_RV_REF(yggr::network::network_info<ID>) left, \
				yggr::network::network_info<ID>& right) { \
		right.swap(left); } \
	\
	template<typename ID> \
	void swap(yggr::network::network_info<ID>& left, \
				BOOST_RV_REF(yggr::network::network_info<ID>) right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_NETWORK_INFO_SWAP()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_NETWORK_INFO_SWAP()
} // namespace boost


#undef _YGGR_TMP_PP_NETWORK_INFO_SWAP

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace network
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename ID>
std::size_t hash_value(const yggr::network::network_info<ID>& info)
{
	return info.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP


#endif // __YGGR_NETWORK_NETWORK_INFO_HPP__
