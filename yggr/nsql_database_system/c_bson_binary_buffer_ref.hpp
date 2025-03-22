//c_bson_binary_buffer_ref.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_BINARY_BUFFER_REF_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_BINARY_BUFFER_REF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/bytes/byte_size.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/const_strings.hpp>

#include <yggr/container/resize.hpp>
#include <yggr/container/clear.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/size.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/container_ex/has_iterator_t.hpp>
#include <yggr/container_ex/is_static_container.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/algorithm.hpp>

#include <boost/move/iterator.hpp>

#include <cassert>
#include <algorithm>
#include <iomanip>

#include <base64.h>

namespace yggr
{
namespace nsql_database_system
{

///****************** the Buffer recommend use network byte order*******************/
template<u32 BINARY_ID>
class c_bson_binary_buffer;

template<u32 BINARY_ID, typename Buffer>
class c_bson_binary_buffer_ref;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<u32 BINARY_ID, typename Buffer>
struct bson_typeid< c_bson_binary_buffer_ref<BINARY_ID, Buffer> >
{
	typedef c_bson_binary_buffer_ref<BINARY_ID, Buffer> type;
	typedef c_bson_binary_buffer_ref<BINARY_ID, Buffer> wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type, 
		value = bson_typeid_def::E_BSON_TYPE_BINARY);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const
	{
		return static_cast<u8>(value);
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

// c_bson_binary_buffer_ref_assign_iterator_helper
template<typename Buf, typename InputIter,
			bool is_random_access_buf = is_random_access_container<Buf>::value,
			bool is_random_access_iter = is_random_access_iterator<InputIter>::value>
struct c_bson_binary_buffer_ref_assign_iterator_helper;

template<typename Buf, typename InputIter>
struct c_bson_binary_buffer_ref_assign_iterator_helper<Buf, InputIter, false, false>
{
public:
	typedef Buf buf_type;
	typedef InputIter iter_type;
	
public:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_value_type;

public:
	inline void operator()(buf_type& buf, iter_type s, iter_type e) const
	{
		typedef vector<iter_value_type> tmp_buf_type;

		u64 tmp_buf_size = iterator_ex::distance(s, e);
		u64 iter_bytes_size = tmp_buf_size * sizeof(iter_value_type);
		if(!iter_bytes_size)
		{
			container::clear(buf);
			return;
		}


		u64 buf_size = (iter_bytes_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		tmp_buf_type tmp_buf(static_cast<std::size_t>(tmp_buf_size));
		std::copy(s, e, boost::begin(tmp_buf));

		container::clear(buf);
		std::copy(
			reinterpret_cast<buf_value_type*>(boost::addressof(tmp_buf[0])), 
			iterator_ex::advance_copy(reinterpret_cast<buf_value_type*>(boost::addressof(tmp_buf[0])), buf_size), 
			iterator_ex::inserter(buf, boost::end(buf)));
	}
};

template<typename Buf, typename InputIter>
struct c_bson_binary_buffer_ref_assign_iterator_helper<Buf, InputIter, false, true>
{
public:
	typedef Buf buf_type;
	typedef InputIter iter_type;
	
public:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_value_type;

public:
	inline void operator()(buf_type& buf, iter_type s, iter_type e) const
	{
		u64 iter_bytes_size = bytes::byte_size(s, e);
		if(!iter_bytes_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (iter_bytes_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		container::clear(buf);
		std::copy(
			reinterpret_cast<const buf_value_type*>(boost::addressof(*s)), 
			iterator_ex::advance_copy(reinterpret_cast<const buf_value_type*>(boost::addressof(*s)), buf_size), 
			iterator_ex::inserter(buf, boost::end(buf)));
	}
};

template<typename Buf, typename InputIter>
struct c_bson_binary_buffer_ref_assign_iterator_helper<Buf, InputIter, true, false>
{
public:
	typedef Buf buf_type;
	typedef InputIter iter_type;
	
public:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_value_type;

public:
	inline void operator()(buf_type& buf, iter_type s, iter_type e) const
	{
		u64 iter_bytes_size = bytes::byte_size(s, e);
		if(!iter_bytes_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (iter_bytes_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		container::resize(buf, buf_size);
		assert(iter_bytes_size <= bytes::byte_size(buf));
		std::copy(s, e, reinterpret_cast<iter_value_type*>(boost::addressof(*boost::begin(buf))));
	}
};

template<typename Buf, typename InputIter>
struct c_bson_binary_buffer_ref_assign_iterator_helper<Buf, InputIter, true, true>
{
public:
	typedef Buf buf_type;
	typedef InputIter iter_type;
	
public:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_value_type;

public:
	inline void operator()(buf_type& buf, iter_type s, iter_type e) const
	{
		u64 iter_bytes_size = bytes::byte_size(s, e);
		if(!iter_bytes_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (iter_bytes_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		container::resize(buf, buf_size);
		assert(iter_bytes_size <= bytes::byte_size(buf));
		memcpy(boost::addressof(*boost::begin(buf)), boost::addressof(*s), iter_bytes_size);
	}
};

// c_bson_binary_buffer_ref_assign_container_helper
template<typename Buf, typename Container, 
			bool is_random_access_buf =  is_random_access_container<Buf>::value,
			bool is_random_access_cont = is_random_access_container<Container>::value>
struct c_bson_binary_buffer_ref_assign_container_helper;

template<typename Buf, typename Container>
struct c_bson_binary_buffer_ref_assign_container_helper<Buf, Container, false, false>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

protected:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

public:
	inline void operator()(buf_type& buf, const cont_type& cont) const
	{
		typedef vector<cont_value_type> tmp_buf_type;

		u64 tmp_buf_size = container::size(cont);
		u64 cont_byte_size = tmp_buf_size * sizeof(cont_value_type);

		if(!cont_byte_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (cont_byte_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		tmp_buf_type tmp_buf(static_cast<std::size_t>(tmp_buf_size));
		std::copy(boost::begin(cont), boost::end(cont), boost::begin(tmp_buf));

		container::clear(buf);
		std::copy(
			reinterpret_cast<const buf_value_type*>(boost::addressof(tmp_buf[0])), 
			iterator_ex::advance_copy(reinterpret_cast<const buf_value_type*>(boost::addressof(tmp_buf[0])), buf_size), 
			iterator_ex::inserter(buf, boost::end(buf)));
	}
};

template<typename Buf, typename Container>
struct c_bson_binary_buffer_ref_assign_container_helper<Buf, Container, false, true>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

protected:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

public:
	inline void operator()(buf_type& buf, const cont_type& cont) const
	{
		u64 cont_byte_size = bytes::byte_size(cont);
		if(!cont_byte_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (cont_byte_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		container::clear(buf);
		std::copy(
			reinterpret_cast<const buf_value_type*>(boost::addressof(*boost::begin(cont))), 
			iterator_ex::advance_copy(reinterpret_cast<const buf_value_type*>(boost::addressof(*boost::begin(cont))), buf_size), 
			iterator_ex::inserter(buf, boost::end(buf)));
	}
};

template<typename Buf, typename Container>
struct c_bson_binary_buffer_ref_assign_container_helper<Buf, Container, true, false>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

protected:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

public:
	inline void operator()(buf_type& buf, const cont_type& cont) const
	{
		u64 cont_byte_size = bytes::byte_size(cont);
		if(!cont_byte_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (cont_byte_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		container::resize(buf, buf_size);
		assert(cont_byte_size <= bytes::byte_size(buf));

		std::copy(
			boost::begin(cont), boost::end(cont),
			reinterpret_cast<cont_value_type*>(boost::addressof(*boost::begin(buf))));
	}
};

template<typename Buf, typename Container>
struct c_bson_binary_buffer_ref_assign_container_helper<Buf, Container, true, true>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

protected:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

public:
	inline void operator()(buf_type& buf, const cont_type& cont) const
	{
		u64 cont_byte_size = bytes::byte_size(cont);
		if(!cont_byte_size)
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = (cont_byte_size + sizeof(buf_value_type) - 1) / sizeof(buf_value_type);

		container::resize(buf, buf_size);
		assert(cont_byte_size <= bytes::byte_size(buf));
		memcpy(boost::addressof(buf[0]), boost::addressof(*boost::begin(cont)), cont_byte_size);
	}
};

// c_bson_binary_buffer_ref_buffer_swapper
template<typename Buf, bool is_static_cont = container_ex::is_static_container<Buf>::value>
struct c_bson_binary_buffer_ref_buffer_swapper;

template<typename Buf>
struct c_bson_binary_buffer_ref_buffer_swapper<Buf, true>
{
public:
	typedef Buf buf_type;
	typedef typename range_ex::range_value_ex<buf_type>::type buf_val_type;

public:
	inline void operator()(buf_type& l, buf_type& r) const
	{
		if(boost::addressof(l[0]) == boost::addressof(r[0]))
		{
			return;
		}

		u64 byte_size = bytes::byte_size(l);
		assert(byte_size == bytes::byte_size(r));
		buf_type tmp;

		assert(byte_size);

		if(byte_size)
		{
			memcpy(boost::addressof(tmp[0]), boost::addressof(l[0]), byte_size);
			memcpy(boost::addressof(l[0]), boost::addressof(r[0]), byte_size);
			memcpy(boost::addressof(r[0]), boost::addressof(tmp[0]), byte_size);
		}
	}
};

template<typename Buf>
struct c_bson_binary_buffer_ref_buffer_swapper<Buf, false>
{
	typedef Buf buf_type;

	inline void operator()(buf_type& l, buf_type& r) const
	{
		if(boost::addressof(l) == boost::addressof(r))
		{
			return;
		}
		yggr::swap(l, r);
	}
};


// c_bson_binary_buffer_ref_buffer_comparer

// buffer_comparer_inner_inner_buffer_comparer
template<typename LBuf, typename RBuf, 
			typename LBufVal, typename RBufVal,
			bool l_isra = is_random_access_container<LBuf>::value,
			bool r_isra = is_random_access_container<RBuf>::value>
struct buffer_comparer_inner_inner_buffer_comparer;

template<typename LBuf, typename RBuf, typename LBufVal, typename RBufVal>
struct buffer_comparer_inner_inner_buffer_comparer<LBuf, RBuf, LBufVal, RBufVal, true, true>
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef RBufVal rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		u64 l_byte_size = bytes::byte_size(l);
		return 
			(l_byte_size == bytes::byte_size(r))
			&& ((reinterpret_cast<const void*>(boost::addressof(*boost::begin(l)))
					== reinterpret_cast<const void*>(boost::addressof(*boost::begin(r))) )
				|| (0 == memcmp(
							boost::addressof(*boost::begin(l)), 
							boost::addressof(*boost::begin(r)), 
							l_byte_size)) );
	}

	inline bool operator()(const lbuf_type& l, const rbuf_type& r, u64 byte_size) const
	{
		return (0 == memcmp(
						boost::addressof(*boost::begin(l)), 
						boost::addressof(*boost::begin(r)), 
						byte_size)) ;
	}
};

template<typename LBuf, typename RBuf, typename LBufVal, typename RBufVal>
struct buffer_comparer_inner_inner_buffer_comparer<LBuf, RBuf, LBufVal, RBufVal, true, false>
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef RBufVal rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		typedef container::vector<rbuf_val_type> vt_type;
		typedef 
			buffer_comparer_inner_inner_buffer_comparer
			<
				lbuf_type, vt_type,
				lbuf_val_type, rbuf_val_type,
				true, true
			> cmp_type;

		u64 l_byte_size = bytes::byte_size(l);
		if(l_byte_size != bytes::byte_size(r))
		{
			return false;
		}

		vt_type vt(boost::begin(r), boost::end(r));
		cmp_type cmp;

		return cmp(l, vt, l_byte_size);
	}
};

template<typename LBuf, typename RBuf, typename LBufVal, typename RBufVal>
struct buffer_comparer_inner_inner_buffer_comparer<LBuf, RBuf, LBufVal, RBufVal, false, true>
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef RBufVal rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		typedef container::vector<lbuf_val_type> vt_type;
		typedef 
			buffer_comparer_inner_inner_buffer_comparer
			<
				vt_type, rbuf_type,
				lbuf_val_type, rbuf_val_type,
				true, true
			> cmp_type;

		u64 l_byte_size = bytes::byte_size(l);
		if(l_byte_size != bytes::byte_size(r))
		{
			return false;
		}

		vt_type vt(boost::begin(l), boost::end(l));
		cmp_type cmp;

		return cmp(vt, r, l_byte_size);
	}
};

template<typename LBuf, typename RBuf, typename LBufVal, typename RBufVal>
struct buffer_comparer_inner_inner_buffer_comparer<LBuf, RBuf, LBufVal, RBufVal, false, false>
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef RBufVal rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		typedef container::vector<lbuf_val_type> lvt_type;
		typedef container::vector<rbuf_val_type> rvt_type;
		typedef 
			buffer_comparer_inner_inner_buffer_comparer
			<
				lvt_type, rvt_type,
				lbuf_val_type, rbuf_val_type,
				true, true
			> cmp_type;

		u64 l_byte_size = bytes::byte_size(l);
		if(l_byte_size != bytes::byte_size(r))
		{
			return false;
		}

		lvt_type lvt(boost::begin(l), boost::end(l));
		rvt_type rvt(boost::begin(r), boost::end(r));
		cmp_type cmp;

		return cmp(lvt, rvt, l_byte_size);
	}
};

// bson_value_t
template<typename LBuf, typename LBufVal>
struct buffer_comparer_inner_inner_buffer_comparer<LBuf, bson_value_t, LBufVal, u8, true, true>
{
public:
	typedef LBuf lbuf_type;
	typedef bson_value_t rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef u8 rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		u64 l_byte_size = bytes::byte_size(l);
		return 
			(l_byte_size == r.value.v_binary.data_len)
			&& ((0 == memcmp(
					boost::addressof(*boost::begin(l)), 
					r.value.v_binary.data, 
					l_byte_size)) );
	}

	inline bool operator()(const lbuf_type& l, const rbuf_type& r, u64 byte_size) const
	{
		return 
			(0 == memcmp(
					boost::addressof(*boost::begin(l)), 
					r.value.v_binary.data, 
					byte_size)) ;
	}
};


template<typename LBuf, typename LBufVal>
struct buffer_comparer_inner_inner_buffer_comparer<LBuf, bson_value_t, LBufVal, u8, false, true>
{
public:
	typedef LBuf lbuf_type;
	typedef bson_value_t rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef u8 rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		typedef container::vector<lbuf_val_type> vt_type;
		typedef 
			buffer_comparer_inner_inner_buffer_comparer
			<
				vt_type, rbuf_type,
				lbuf_val_type, rbuf_val_type,
				true, true
			> cmp_type;

		u64 l_byte_size = bytes::byte_size(l);
		if(l_byte_size != r.value.v_binary.data_len)
		{
			return false;
		}

		vt_type vt(boost::begin(l), boost::end(l));
		cmp_type cmp;

		return cmp(vt,r, l_byte_size);
	}
};

// buffer_comparer_inner_buffer_comparer
template<typename LBuf, typename RBuf, typename LBufVal, typename RBufVal>
struct buffer_comparer_inner_buffer_comparer
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef RBufVal rbuf_val_type;

private:
	typedef 
		buffer_comparer_inner_inner_buffer_comparer
		<
			lbuf_type, rbuf_type,
			lbuf_val_type, rbuf_val_type
		> cmp_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		cmp_type cmp;
		return cmp(l, r);
	}
};

template<typename LBuf, typename RBuf, typename LBufVal>
struct buffer_comparer_inner_buffer_comparer<LBuf, RBuf, LBufVal, LBufVal>
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef LBufVal rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		return std::equal(boost::begin(l), boost::end(l), boost::begin(r));
	}
};

template<typename LBuf, typename LBufVal>
struct buffer_comparer_inner_buffer_comparer<LBuf, bson_value_t, LBufVal, u8>
{
public:
	typedef LBuf lbuf_type;
	typedef bson_value_t rbuf_type;

	typedef LBufVal lbuf_val_type;
	typedef u8 rbuf_val_type;

private:
	typedef 
		buffer_comparer_inner_inner_buffer_comparer
		<
			lbuf_type, rbuf_type,
			lbuf_val_type, 
			rbuf_val_type,
			is_random_access_container<lbuf_type>::value,
			true
		> cmp_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		cmp_type cmp;
		return cmp(l, r);
	}
};

//c_bson_binary_buffer_ref_buffer_comparer
template<typename LBuf, typename RBuf>
struct c_bson_binary_buffer_ref_buffer_comparer
{
public:
	typedef LBuf lbuf_type;
	typedef RBuf rbuf_type;

	typedef typename range_ex::range_value_ex<lbuf_type>::type lbuf_val_type;
	typedef typename range_ex::range_value_ex<rbuf_type>::type rbuf_val_type;

private:
	typedef
		buffer_comparer_inner_buffer_comparer
		<
			lbuf_type, rbuf_type,
			lbuf_val_type, rbuf_val_type
		> cmp_type;

public:
	// same buf_val_type;
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		cmp_type cmp;
		return cmp(l, r);
	}
};

template<typename LBuf>
struct c_bson_binary_buffer_ref_buffer_comparer<LBuf, LBuf> // same container and buf_type
{
public:
	typedef LBuf lbuf_type;
	typedef LBuf rbuf_type;

	typedef typename range_ex::range_value_ex<lbuf_type>::type lbuf_val_type;
	typedef typename range_ex::range_value_ex<rbuf_type>::type rbuf_val_type;

public:
	inline bool operator()(const lbuf_type& l, const rbuf_type& r) const
	{
		return std::equal(boost::begin(l), boost::end(l), boost::begin(l));
	}
};

template<typename LBuf>
struct c_bson_binary_buffer_ref_bson_value_comparer
{
public:
	typedef LBuf lbuf_type;
	typedef bson_value_t rbuf_type;

	typedef typename range_ex::range_value_ex<lbuf_type>::type lbuf_val_type;
	typedef u8 rbuf_val_type;

private:
	typedef
		buffer_comparer_inner_buffer_comparer
		<
			lbuf_type, rbuf_type,
			lbuf_val_type, rbuf_val_type
		> cmp_type;

public:
	// same buf_val_type;
	inline bool operator()(const lbuf_type& l, u32 buf_id, const rbuf_type& r) const
	{
		cmp_type cmp;
		return 
			(bson_typeid_def::E_BSON_TYPE_BINARY == r.value_type)
			&& (buf_id == r.value.v_binary.subtype)
			&& cmp(l, r);
	}
};

// c_bson_binary_buffer_ref_save_helper
template<u32 BINARY_ID, typename BufVal, bool is_random_access>
class c_bson_binary_buffer_ref_save_helper;

template<u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_save_helper<BINARY_ID, BufVal, true>
{
public:
	typedef BufVal buf_val_type;

private:
	typedef c_bson_binary_buffer_ref_save_helper this_type;

public:
	template<typename Bson, typename Buffer, typename String > inline
	typename boost::enable_if<charset::utf8_string_constructable< String >, bool>::type
		operator()(Bson& bs, const String& name, const Buffer& buf) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(boost::addressof(bs)))
		{
			::bson_init(boost::addressof(bs));
		}

		u32 byte_size = bytes::byte_size(buf);
		const u8* pbuf = 
			byte_size? 
				reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buf))) 
				: this_type::prv_s_zero_buf();

		assert(pbuf);

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return 
			bson_append_binary(
				boost::addressof(bs), str_name.data(), str_name.size(),
				static_cast<bson_subtype_t>(BINARY_ID),
				pbuf, byte_size);
	}

private:
	inline static const u8* prv_s_zero_buf(void)
	{
		static const u8 zero_buf[] = {0};
		return zero_buf;
	}
};

template<u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_save_helper<BINARY_ID, BufVal, false>
{
public:
	typedef BufVal buf_val_type;

private:
	typedef c_bson_binary_buffer_ref_save_helper this_type;

public:
	//template<typename Bson, typename Char, typename Buffer> inline
	//typename boost::enable_if<is_native_char<Char>, bool>::type
	//	operator()(Bson& bs, const Char* name, const Buffer& buf) const
	//{
	//	typedef container::vector<buf_val_type> inner_buffer_type;
	//	typedef c_bson_binary_buffer_ref_save_helper<BINARY_ID, buf_val_type, true> now_helper_type;

	//	inner_buffer_type tmp_buf(buf.begin(), buf.end());
	//	now_helper_type now_helper;

	//	return now_helper(bs, name, tmp_buf);
	//}

	//template<typename Bson, typename Buffer, typename Char, typename Traits, typename Alloc,
	//		template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	//typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	//	operator()(Bson& bs, const Basic_String<Char, Traits, Alloc>& name, const Buffer& buf) const
	//{
	//	typedef container::vector<buf_val_type> inner_buffer_type;
	//	typedef c_bson_binary_buffer_ref_save_helper<BINARY_ID, buf_val_type, true> now_helper_type;

	//	inner_buffer_type tmp_buf(buf.begin(), buf.end());
	//	now_helper_type now_helper;

	//	return now_helper(bs, name, tmp_buf);
	//}

	template<typename Bson, typename Buffer, typename String > inline
	typename boost::enable_if< charset::utf8_string_constructable< String >, bool>::type
		operator()(Bson& bs, const String& name, const Buffer& buf) const
	{
		typedef container::vector<buf_val_type> inner_buffer_type;
		typedef c_bson_binary_buffer_ref_save_helper<BINARY_ID, buf_val_type, true> now_helper_type;

		inner_buffer_type tmp_buf(buf.begin(), buf.end());
		now_helper_type now_helper;

		return now_helper(bs, name, tmp_buf);
	}
};

template<u32 BINARY_ID, typename BufVal, bool is_random_access>
class c_bson_binary_buffer_ref_save_value_helper;

template<u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_save_value_helper<BINARY_ID, BufVal, true>
{
public:
	typedef BufVal buf_val_type;

public:
	template<typename Bson, typename Buffer> inline
	bool operator()(Bson& bs, const Buffer& buf) const
	{
		return 
			bson_native_ex::s_bson_save_value_of_binary_buffer(
				boost::addressof(bs), BINARY_ID,
				reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buf))),
				bytes::byte_size(buf));
	}
};

template<u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_save_value_helper<BINARY_ID, BufVal, false>
{
public:
	typedef BufVal buf_val_type;

public:
	template<typename Bson, typename Buffer> inline
	bool operator()(Bson& bs, const Buffer& buf) const
	{
		typedef container::vector<buf_val_type> inner_buffer_type;
		typedef 
			c_bson_binary_buffer_ref_save_value_helper
			<
				BINARY_ID,
				buf_val_type,
				true
			> now_helper_type;

		inner_buffer_type tmp_buf(buf.begin(), buf.end());
		now_helper_type now_helper;

		return now_helper(bs, buf);
	}
};

template<u32 TypeID, u32 BINARY_ID, typename BufVal, bool is_random_access>
class c_bson_binary_buffer_ref_load_helper;

template<u32 TypeID, u32 BINARY_ID, typename BufVal >
class c_bson_binary_buffer_ref_load_helper<TypeID, BINARY_ID, BufVal, true>
{
public:
	typedef BufVal buf_val_type;

private:
	typedef c_bson_binary_buffer_ref_load_helper this_type;

public:
	template<typename BsonIter, typename Char, typename Buffer> inline
	typename boost::enable_if< is_native_char<Char>, void >::type
		operator()(const BsonIter& iter, const Char* name, Buffer& buf)
	{
		assert((iter.load_check(TypeID, name, BINARY_ID)));
		if(!iter.load_check(TypeID, name, BINARY_ID))
		{
			container::clear(buf);
			return;
		}

		bson_subtype_t sub_t = static_cast<bson_subtype_t>(BINARY_ID);
		u32 byte_size = 0;
		const u8* pbuf = 0;
		bson_iter_binary(boost::addressof(iter), &sub_t, &byte_size, &pbuf);

		u32 buf_size = (byte_size + sizeof(buf_val_type) - 1) / sizeof(buf_val_type);

		if(buf_size)
		{
			yggr::resize(buf, buf_size);
			memset(boost::addressof(buf[0]), 0, buf_size * sizeof(buf_val_type));
			memcpy(boost::addressof(buf[0]), pbuf, byte_size);
		}
	}

	template<typename BsonIter, typename Char, typename Traits, typename Alloc, typename Buffer,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, void >::type
		operator()(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name, Buffer& buf)
	{
		assert((iter.load_check(TypeID, name, BINARY_ID)));
		if(!iter.load_check(TypeID, name, BINARY_ID))
		{
			container::clear(buf);
			return;
		}

		bson_subtype_t sub_t = static_cast<bson_subtype_t>(BINARY_ID);
		u32 byte_size = 0;
		const u8* pbuf = 0;
		bson_iter_binary(boost::addressof(iter), &sub_t, &byte_size, &pbuf);

		u32 buf_size = (byte_size + sizeof(buf_val_type) - 1) / sizeof(buf_val_type);

		if(buf_size)
		{
			yggr::resize(buf, buf_size);
			memset(boost::addressof(buf[0]), 0, buf_size * sizeof(buf_val_type));
			memcpy(boost::addressof(buf[0]), pbuf, byte_size);
		}
	}
};

template<u32 TypeID, u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_load_helper<TypeID, BINARY_ID, BufVal, false>
{
public:
	typedef BufVal buf_val_type;

private:
	typedef c_bson_binary_buffer_ref_load_helper this_type;

public:
	template<typename BsonIter, typename Buffer, typename Char> inline
	typename boost::enable_if< is_native_char<Char>, void >::type
		operator()(const BsonIter& iter, const Char* name, Buffer& buf)
	{
		typedef container::vector<buf_val_type> inner_buffer_type;
		typedef 
			c_bson_binary_buffer_ref_load_helper
			<
				TypeID,
				BINARY_ID,
				buf_val_type,
				true
			> helper_type;

		inner_buffer_type tmp_buf;
		helper_type helper;
		helper(iter, name, tmp_buf);
		std::copy(boost::begin(tmp_buf), boost::end(tmp_buf), iterator_ex::inserter(buf, boost::begin(buf)));
	}

	template<typename BsonIter, typename Buffer, typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, void >::type
		operator()(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name, Buffer& buf)
	{
		typedef container::vector<buf_val_type> inner_buffer_type;
		typedef 
			c_bson_binary_buffer_ref_load_helper
			<
				TypeID,
				BINARY_ID,
				buf_val_type,
				true
			> helper_type;

		inner_buffer_type tmp_buf;
		helper_type helper;
		helper(iter, name, tmp_buf);
		std::copy(boost::begin(tmp_buf), boost::end(tmp_buf), iterator_ex::inserter(buf, boost::begin(buf)));
	}
};

//c_bson_binary_buffer_ref_load_value_helper
template<u32 BINARY_ID, typename BufVal, bool is_random_access>
class c_bson_binary_buffer_ref_load_value_helper;

template<u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_load_value_helper<BINARY_ID, BufVal, true>
{
public:
	typedef BufVal buf_val_type;

public:
	template<typename BsonIter, typename Buffer> inline
	void operator()(BsonIter& iter, Buffer& buf) const
	{
		bson_subtype_t sub_t = static_cast<bson_subtype_t>(BINARY_ID);
		u32 byte_size = 0;
		const u8* pbuf = 0;
		bson_iter_binary(&iter, &sub_t, &byte_size, &pbuf);

		u32 buf_size = (byte_size + sizeof(buf_val_type) - 1) / sizeof(buf_val_type);

		if(buf_size)
		{
			yggr::resize(buf, buf_size);
			memset(&buf[0], 0, buf_size * sizeof(buf_val_type));
			memcpy(&buf[0], pbuf, byte_size);
		}
	}
};

template<u32 BINARY_ID, typename BufVal>
class c_bson_binary_buffer_ref_load_value_helper<BINARY_ID, BufVal, false>
{
public:
	typedef BufVal buf_val_type;

public:
	template<typename BsonIter, typename Buffer> inline
	void operator()(const BsonIter& iter, Buffer& buf) const
	{
		typedef container::vector<buf_val_type> inner_buffer_type;
		typedef 
			c_bson_binary_buffer_ref_load_value_helper
			<
				BINARY_ID, 
				buf_val_type,
				true
			> helper_type;

		inner_buffer_type tmp_buf;
		helper_type helper;
		helper(iter, tmp_buf);
		std::copy(boost::begin(tmp_buf), boost::end(tmp_buf), iterator_ex::inserter(buf, boost::begin(buf)));
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<u32 BINARY_ID, typename TBuffer>
class c_bson_binary_buffer_ref
{
public:
	YGGR_STATIC_CONSTANT(u32, E_TYPE_ID = BINARY_ID);

	typedef TBuffer buf_type;
	typedef typename range_ex::range_value_ex<buf_type>::type buf_val_type;

private:
	BOOST_MPL_ASSERT_NOT((boost::is_class<buf_val_type>));

protected:
	typedef 
		detail::c_bson_binary_buffer_ref_buffer_swapper
		<
			buf_type
		> buf_swapper_type;

	typedef 
		detail::c_bson_binary_buffer_ref_bson_value_comparer
		<
			buf_type
		> buf_value_comparer_type;

private:
	typedef c_bson_binary_buffer_ref this_type;

public:
	explicit c_bson_binary_buffer_ref(buf_type& buf)
		: _buf(buf)
	{
	}

	c_bson_binary_buffer_ref(const this_type& right)
		: _buf(right._buf)
	{
	}

	~c_bson_binary_buffer_ref(void)
	{
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			this_type&
		>::type
		operator=(const Container& right)
	{
		this_type::assign(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(buf_type) right)
	{
		this_type::assign(boost::move(right));
		return *this;
	}

	inline this_type& operator=(const buf_type& right)
	{
		this_type::assign(right);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		this_type::assign(right);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(buf_type) right)
	{
		buf_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(buf_type& right)
	{
		if(boost::addressof(_buf) == boost::addressof(right))
		{
			return;
		}

		buf_swapper_type swapper;
		swapper(_buf, right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		buf_swapper_type swapper;
		swapper(_buf, right._buf);
	}

public:
	inline operator c_bson_binary_buffer<static_cast<u32>(this_type::E_TYPE_ID)>(void) const
	{
		typedef c_bson_binary_buffer<static_cast<u32>(this_type::E_TYPE_ID)> ret_type;
		ret_type ret;
		ret.assign(boost::begin(_buf), boost::end(_buf));
		return ret;
	}

	// inline operator const buf_type&(void) const conflict (const c_bson_value& == const c_bson_value&) and some const c_bson_value& foo,
	// so dont't write this code
//public:
//	inline operator const u8*(void) const;
//	inline operator u8*(void);

	// don't add these two functions because the operator== function conflict
	//operator buf_type&(void);
	//operator const buf_type&(void) const;
//
public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_BINARY;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_BINARY;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 buffer_typeid(void)
	{
		return this_type::E_TYPE_ID;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_buffer_typeid(void)
	{
		return this_type::E_TYPE_ID;
	}

public:
	inline size_type size(void) const
	{
		return bytes::byte_size(_buf);
	}

	inline bool empty(void) const
	{
		return boost::empty(_buf);
	}

	inline void clear(void)
	{
		yggr::clear(_buf);
	}

	inline buf_type& buffer(void)
	{
		return _buf;
	}

	inline const buf_type& buffer(void) const
	{
		return _buf;
	}

	// dont't write this code, because inner buffer maybe not a random_access container

	//inline u8* data(void);
	//inline const u8* data(void) const;

public:
	inline c_bson_binary_buffer<static_cast<u32>(this_type::E_TYPE_ID)>
		conv_to_binary_buffer(void) const
	{
		typedef c_bson_binary_buffer<static_cast<u32>(this_type::E_TYPE_ID)> ret_type;
		ret_type ret;
		ret.assign(boost::begin(_buf), boost::end(_buf));
		return ret;
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		//typedef detail::c_bson_binary_buffer_ref_bson_value_comparer<buf_type> cmper_type;

		buf_value_comparer_type cmper;
		return cmper(_buf, this_type::buffer_typeid(), val);
		//return 
		//	bson_value_native_ex::s_value_cmp_eq_binary_buffer(
		//		&val, this_type::buffer_typeid(), this_type::data(), this_type::size());
	}

	inline bool compare_eq(const buf_type& right) const
	{
		typedef 
			detail::c_bson_binary_buffer_ref_buffer_comparer
			<
				buf_type,
				buf_type
			> cmp_type;

		cmp_type cmp;
		return cmp(_buf, right);
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			bool
		>::type
		compare_eq(const Container& right) const
	{
		typedef Container cont_type;
		typedef 
			detail::c_bson_binary_buffer_ref_buffer_comparer
			<
				buf_type,
				cont_type
			> cmp_type;

		const cont_type& cont = right;
		cmp_type cmp;

		return cmp(_buf, cont);
	}

	inline bool compare_eq(const this_type& right) const
	{
		typedef 
			detail::c_bson_binary_buffer_ref_buffer_comparer
			<
				buf_type,
				buf_type
			> cmp_type;

		if(this == &right)
		{
			return true;
		}

		cmp_type cmp;
		return cmp(_buf, right._buf);
	}

protected:
	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename
							boost::remove_cv
							<
								typename iterator_ex::iterator_value_ex<InputIter>::type
							>::type
					>
				>
			>,
			void
		>::type
		pro_assign_iterator(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef 
			detail::c_bson_binary_buffer_ref_assign_iterator_helper
			<
				buf_type,
				InputIter
			> h_type;

		h_type h;
		h(_buf, s, e);
	}

	inline void pro_assign_bson_value_t(const bson_value_t& val)
	{
		if(!((static_cast<u32>(val.value_type) == bson_typeid_def::E_BSON_TYPE_BINARY)
				&& (this_type::E_TYPE_ID == val.value.v_binary.subtype)
				&& (val.value.v_binary.data && val.value.v_binary.data_len) ))
		{
			container::clear(_buf);
		}
		else
		{
			this_type::pro_assign_iterator(
				val.value.v_binary.data, 
				val.value.v_binary.data + val.value.v_binary.data_len);
		}
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
		assign(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
	}

	inline void assign(const u8* pdata, u64 len)
	{
		if(!(pdata && len))
		{
			container::clear(_buf);
			return;
		}

		this_type::pro_assign_iterator(pdata, pdata + len);
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename
							boost::remove_cv
							<
								typename iterator_ex::iterator_value_ex<InputIter>::type
							>::type
					>
				>
			>,
			void
		>::type
		assign(InputIter s, InputIter e)
	{
		return this_type::pro_assign_iterator(s, e);
	}

	inline void assign(BOOST_RV_REF(buf_type) buf)
	{
		buf_type& buf_ref(buf);
		if(boost::addressof(_buf) == boost::addressof(buf_ref))
		{
			return;
		}

		buf_swapper_type swapper;
		buf_type tmp;

		swapper(_buf, buf_ref);
	}

	inline void assign(const buf_type& buf)
	{
		typedef 
			detail::c_bson_binary_buffer_ref_assign_container_helper
			<
				buf_type,
				buf_type
			> h_type;

		if(boost::addressof(_buf) == boost::addressof(buf))
		{
			return;
		}

		h_type h;
		h(_buf, buf);
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			void
		>::type
		assign(const Container& cont)
	{
		typedef Container cv_cont_type;
		typedef typename boost::remove_cv<cv_cont_type>::type cont_type;

		typedef 
			detail::c_bson_binary_buffer_ref_assign_container_helper
			<
				buf_type,
				cont_type
			> h_type;

		h_type h;
		h(_buf, cont);
	}

	inline void assign(const this_type& right)
	{
		typedef 
			detail::c_bson_binary_buffer_ref_assign_container_helper
			<
				buf_type,
				buf_type
			> h_type;

		if(boost::addressof(_buf) == boost::addressof(right._buf))
		{
			return;
		}

		h_type h;
		h(_buf, right._buf);
	}

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );

	//save
	template<u32 ID, typename BufVal, bool is_random_access>
	friend class detail::c_bson_binary_buffer_ref_save_helper;

	template<typename Bson, typename String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable< String >
			>,
			bool
		>::type
		save(Bson& bs, const String& name) const
	{
		typedef typename boost::range_iterator<buf_type>::type iterator_type;
		typedef typename is_random_access_iterator<iterator_type>::type is_random_access_type;
		typedef 
			detail::c_bson_binary_buffer_ref_save_helper
			<
				this_type::E_TYPE_ID,
				buf_val_type,
				is_random_access_type::value
			> save_helper_type;

		save_helper_type helper;
		return helper(bs, name, _buf);
	}

	//save_value
	template<u32 BID, typename BufVal, bool is_random_access>
	friend class detail::c_bson_binary_buffer_ref_save_value_helper;

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		typedef typename boost::range_iterator<buf_type>::type iterator_type;
		typedef typename is_random_access_iterator<iterator_type>::type is_random_access_type;

		typedef 
			detail::c_bson_binary_buffer_ref_save_value_helper
			<
				this_type::E_TYPE_ID, 
				buf_val_type,
				is_random_access_type::value
			> save_value_helper_type;

		save_value_helper_type helper;

		return helper(bs, _buf);
	}

	// load
	template<u32 TID, u32 BID, typename BufVal, bool is_random_access>
	friend class c_bson_binary_buffer_ref_load_helper;

	template<typename BsonIter, typename Char> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				is_native_char<Char>
			>,
			void 
		>::type
		load(const BsonIter& iter, const Char* name)
	{
		typedef typename boost::range_iterator<buf_type>::type iterator_type;
		typedef typename is_random_access_iterator<iterator_type>::type is_random_access_type;

		typedef 
			detail::c_bson_binary_buffer_ref_load_helper
			<
				bson_typeid< typename boost::remove_const<this_type>::type >::value,
				this_type::E_TYPE_ID,
				buf_val_type,
				is_random_access_type::value
			> load_helper_type;

		load_helper_type helper;
		helper(iter, name, _buf);
	}

	template<typename BsonIter, typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::is_string_t< Basic_String<Char, Traits, Alloc> >
			>, 
			void 
		>::type
		load(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name )
	{
		typedef typename boost::range_iterator<buf_type>::type iterator_type;
		typedef typename is_random_access_iterator<iterator_type>::type is_random_access_type;

		typedef 
			detail::c_bson_binary_buffer_ref_load_helper
			<
				bson_typeid< typename boost::remove_const<this_type>::type >::value,
				this_type::E_TYPE_ID,
				buf_val_type,
				is_random_access_type::value
			> load_helper_type;

		load_helper_type helper;

		return helper(iter, name, _buf);
	}

	// load value
	template<u32 BID, typename BufVal, bool is_random_access>
	friend class c_bson_binary_buffer_ref_load_value_helper;

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(BsonIter& iter)
	{
		typedef typename boost::range_iterator<buf_type>::type iterator_type;
		typedef typename is_random_access_iterator<iterator_type>::type is_random_access_type;

		typedef 
			detail::c_bson_binary_buffer_ref_load_value_helper
			<
				this_type::E_TYPE_ID,
				buf_val_type,
				is_random_access_type::value
			> load_value_helper_type;

		load_value_helper_type helper;

		helper(iter, _buf);
	}

private:
	buf_type& _buf;
};

// non member foo
template<u32 BINARY_ID, typename Buffer> inline
bool operator==(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, const Buffer& r)
{
	return l.compare_eq(r);
}

template<u32 BINARY_ID, typename Buffer> inline
bool operator==(const Buffer& l, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	return r.compare_eq(l);
}

template<u32 BINARY_ID, typename Buffer> inline
bool operator!=(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, const Buffer& r)
{
	return !l.compare_eq(r);
}

template<u32 BINARY_ID, typename Buffer> inline
bool operator!=(const Buffer& l, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	return !r.compare_eq(l);
}

template<typename Container, u32 BINARY_ID, typename Buffer> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
		operator==(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, const Container& r)
{
	typedef Container cont_type;
	return l.compare_eq(r);
}

template<typename Container, u32 BINARY_ID, typename Buffer> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
		operator==(const Container& l, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	typedef Container cont_type;
	return r.compare_eq(l);
}

template<typename Container, u32 BINARY_ID, typename Buffer> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
	operator!=(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, const Container& r)
{
	typedef Container cont_type;
	return !l.compare_eq(r);
}

template<typename Container, u32 BINARY_ID, typename Buffer> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
	operator!=(const Container& l, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	typedef Container cont_type;
	return !r.compare_eq(l);
}

template<u32 BINARY_ID, typename Buffer> inline
bool operator==(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l,
				const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	return l.compare_eq(r);
}

template<u32 BINARY_ID, typename Buffer> inline
bool operator!=(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l,
				const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	return !l.compare_eq(r);
}

// ==
template<typename BsonValue, yggr::u32 BINARY_ID, typename Buffer> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue, yggr::u32 BINARY_ID, typename Buffer> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

// !=
template<typename BsonValue, yggr::u32 BINARY_ID, typename Buffer> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue, yggr::u32 BINARY_ID, typename Buffer> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

namespace detail
{

template<bool is_random_access, u32 BINARY_ID, typename Buffer>
class c_bson_binary_buffer_ref_b64_ntop_helper;

template<u32 BINARY_ID, typename Buffer>
class c_bson_binary_buffer_ref_b64_ntop_helper<true, BINARY_ID, Buffer>
{

public:
	typedef Buffer buffer_type;
	typedef c_bson_binary_buffer_ref<BINARY_ID, Buffer> type;
	typedef yggr::string string_type;

private:
	typedef c_bson_binary_buffer_ref_b64_ntop_helper this_type;

public:
	c_bson_binary_buffer_ref_b64_ntop_helper(const type& right)
	{
		this_type::prv_gen(right);
	}

	~c_bson_binary_buffer_ref_b64_ntop_helper(void)
	{
	}

public:
	inline operator const string_type&(void) const
	{
		return _str;
	}

private:
	void prv_gen(const type& right)
	{
		if(right.empty())
		{
			return;
		}

		const buffer_type& buf_ref = right.buffer();
		u64 buf_byte_size = bytes::byte_size(buf_ref);
		assert(buf_byte_size);

		u64 b64_len = b64_ntop_byte_size(buf_byte_size);
		string_type b64_buf(b64_len, 0);

		b64_ntop(
			reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buf_ref))),
			buf_byte_size, boost::addressof(b64_buf[0]), b64_len);

		_str.swap(b64_buf);
	}

private:
	string_type _str;
};

template<u32 BINARY_ID, typename Buffer>
class c_bson_binary_buffer_ref_b64_ntop_helper<false, BINARY_ID, Buffer>
{
public:
	typedef Buffer buffer_type;
	typedef c_bson_binary_buffer_ref<BINARY_ID, Buffer> type;
	typedef yggr::string string_type;

private:
	typedef c_bson_binary_buffer_ref_b64_ntop_helper this_type;

public:
	c_bson_binary_buffer_ref_b64_ntop_helper(const type& right)
	{
		this_type::prv_gen(right);
	}

	~c_bson_binary_buffer_ref_b64_ntop_helper(void)
	{
	}

public:
	inline operator const string_type&(void) const
	{
		return _str;
	}

private:
	void prv_gen(const type& right)
	{
		typedef typename type::buf_val_type buf_val_type;
		typedef container::vector<buf_val_type> inner_buf_type;

		if(right.empty())
		{
			return;
		}

		const buffer_type& org_buf_ref = right.buffer();
		inner_buf_type inner_buf(boost::begin(org_buf_ref), boost::end(org_buf_ref));
		const inner_buf_type& buf_ref = inner_buf;
		assert(bytes::byte_size(org_buf_ref) == bytes::byte_size(buf_ref));

		u64 buf_byte_size = bytes::byte_size(buf_ref);
		assert(buf_byte_size);

		u64 b64_len = b64_ntop_byte_size(buf_byte_size);
		string_type b64_buf(b64_len, 0);

		b64_ntop(
			reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buf_ref))),
			buf_byte_size, boost::addressof(b64_buf[0]), b64_len);

		_str.swap(b64_buf);
	}

private:
	string_type _str;
};

} // namespace detail

namespace detail
{

template<u32 BINARY_ID, typename Buffer>
struct bson_operator_outter< c_bson_binary_buffer_ref<BINARY_ID, Buffer> >
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& right, 
					bson_json_mode_def::mode_type mode) const
	{
		typedef 
			detail::c_bson_binary_buffer_ref_b64_ntop_helper
			<
				is_random_access_iterator
				<
					typename boost::range_iterator<Buffer>::type
				>::value,
				BINARY_ID,
				Buffer
			> b64_ntop_h_type;
		YGGR_PP_CONST_CHAR_LOCAL_DEF(char_zero, '0')

		std::ios_base::fmtflags org_flags = os.flags();
		Char org_fill = os.fill();

		std::ios_base::fmtflags hex_flags = (org_flags & ~std::ios::basefield) | std::ios::hex;
		Char hex_fill = YGGR_PP_CONST_CHAR_LOCAL_GET(char_zero, Char);

		b64_ntop_h_type h(right);

		const typename b64_ntop_h_type::string_type& ostr = h;

		switch(mode)
		{
		case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
		case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
			os << "{ \"$binary\" : { \"base64\" : \""
				<< ostr
				<< "\", \"subType\" : \"";
			os.flags(hex_flags);
			os.fill(hex_fill);
			os << std::setw(2) << BINARY_ID;
			os.flags(org_flags);
			os.fill(org_fill);
			os << "\" } }";
			break;
		default:
			os << "{ \"$binary\" : \"" 
				<< ostr
				<< "\", \"$type\" : \"";
			os.flags(hex_flags);
			os.fill(hex_fill);
			os << std::setw(2) << BINARY_ID;
			os.flags(org_flags);
			os.fill(org_fill);
			os << "\" }";
		}

		return os;
	}
};


} // namespace detail

template<typename Char, typename Traits, u32 BINARY_ID, typename Buffer> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& right)
{
	typedef c_bson_binary_buffer_ref<BINARY_ID, Buffer> value_type;
	typedef detail::bson_operator_outter<value_type> outter_type;

	outter_type outter;
	return outter(os, right, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits,
//			u32 BINARY_ID, typename Buffer>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF_BEG c_bson_binary_buffer_ref<BINARY_ID, Buffer> BOOST_RV_REF_END right)
//{
//	const c_bson_binary_buffer_ref<BINARY_ID, Buffer>& right_cref = right;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
template<u32 BINARY_ID, typename Buffer> inline 
void swap(typename c_bson_binary_buffer_ref<BINARY_ID, Buffer>::buf_type& l, 
			c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r)
{ 
	r.swap(l);
} 

template<u32 BINARY_ID, typename Buffer> inline 
void swap(c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, 
			typename c_bson_binary_buffer_ref<BINARY_ID, Buffer>::buf_type& r)
{ 
	l.swap(r); 
} 
	
template<u32 BINARY_ID, typename Buffer> inline 
void swap(c_bson_binary_buffer_ref<BINARY_ID, Buffer>& l, 
			c_bson_binary_buffer_ref<BINARY_ID, Buffer>& r) 
{ 
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_BINARY_BUFFER_REF_HPP__
