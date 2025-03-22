//c_mongo_basic_stream.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_BASIC_STREAM_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_BASIC_STREAM_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/support/operator_t.hpp>
#include <yggr/nonable/noncopyable.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/assert_char.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/bytes/byte_size.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>

#include <boost/ref.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_basic_stream;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

class mongoc_stream_t_shared_ptr_deleter
{
public:
	typedef c_mongo_basic_stream outside_type;

	mongoc_stream_t_shared_ptr_deleter(outside_type& outside)
		: _outside(outside)
	{
	}

public:
	void operator()(mongoc_stream_t* p) const;

private:
	outside_type& _outside;
};

// c_mongo_basic_stream_read_helper

template<typename Container,
			bool is_iovec
				= boost::is_base_of
					<
						mongoc_iovec_t, 
						typename boost::range_value<Container>::type
					>::value>
struct c_mongo_basic_stream_read_helper;

template<typename Container>
struct c_mongo_basic_stream_read_helper<Container, true>
{
public:
	typedef ::yggr::shared_ptr<mongoc_stream_t> stream_ptr_type;
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type value_type;

public:
	inline ssize_t operator()(stream_ptr_type pstream, cont_type& cont, size_t min_bytes, int32_t timeout_msec) const
	{
		size_t size = boost::size(cont);
		return 
			(pstream && size)? 
				mongoc_stream_readv(
					pstream.get(), 
					&(*boost::begin(cont)), 
					size, min_bytes, timeout_msec) 
				: -1;
	}
};
	
template<typename Container>
struct c_mongo_basic_stream_read_helper<Container, false>
{
public:
	typedef ::yggr::shared_ptr<mongoc_stream_t> stream_ptr_type;
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type value_type;

public:
	inline ssize_t operator()(stream_ptr_type pstream, cont_type& cont, size_t min_bytes, int32_t timeout_msec) const
	{
		typedef typename boost::range_iterator<cont_type>::type iter_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<iter_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		size_t byte_size = bytes::byte_size(cont);
		return 
			(pstream && byte_size)? 
				mongoc_stream_read(
					pstream.get(), 
					reinterpret_cast<void*>(&(*boost::begin(cont))), 
					byte_size, min_bytes, timeout_msec) 
				: -1;
	}
};

// c_mongo_basic_stream_write_helper

template<typename Container, 
			bool is_iovec
				= boost::is_base_of
					<
						mongoc_iovec_t, 
						typename boost::range_value<Container>::type
					>::value >
struct c_mongo_basic_stream_write_helper;

template<typename Container>
struct c_mongo_basic_stream_write_helper<Container, true>
{
public:
	typedef ::yggr::shared_ptr<mongoc_stream_t> stream_ptr_type;
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type value_type;

public:
	inline ssize_t operator()(stream_ptr_type pstream, const cont_type& cont, size_t min_bytes, int32_t timeout_msec) const
	{
		size_t size = boost::size(cont);
		return
			pstream?
				mongoc_stream_writev(
					pstream.get(), 
					const_cast<value_type*>(&(*boost::begin(cont))), 
					size, timeout_msec)
				: -1;
	}
};
	
template<typename Container>
struct c_mongo_basic_stream_write_helper<Container, false>
{
public:
	typedef ::yggr::shared_ptr<mongoc_stream_t> stream_ptr_type;
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type value_type;

public:
	inline ssize_t operator()(stream_ptr_type pstream, const cont_type& cont, int32_t timeout_msec) const
	{
		typedef typename boost::range_iterator<cont_type>::type iter_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<iter_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		size_t byte_size = bytes::byte_size(cont);
		return 
			pstream? 
				mongoc_stream_write(
					pstream.get(), 
					reinterpret_cast<void*>(const_cast<value_type*>(&(*boost::begin(cont)))), 
					byte_size, timeout_msec) 
				: -1;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr


namespace yggr
{
namespace nsql_database_system
{

class c_mongo_basic_stream
{
public:
	typedef mongoc_stream_t org_type;
	typedef ::yggr::shared_ptr<org_type> stream_ptr_type;

	//#define MONGOC_STREAM_SOCKET 1
	//#define MONGOC_STREAM_FILE 2
	//#define MONGOC_STREAM_BUFFERED 3
	//#define MONGOC_STREAM_GRIDFS 4
	//#define MONGOC_STREAM_TLS 5
	//#define MONGOC_STREAM_GRIDFS_UPLOAD 6
	//#define MONGOC_STREAM_GRIDFS_DOWNLOAD 7

	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_SOCKET = 1);
	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_FILE = 2);
	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_BUFFERED = 3);
	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_GRIDFS = 4);
	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_TLS = 5);
	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_GRIDFS_UPLOAD = 6);
	YGGR_STATIC_CONSTANT(u32, E_STREAM_T_GRIDFS_DOWNLOAD = 7);

private:
	typedef c_mongo_basic_stream this_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(c_mongo_basic_stream) // not using this_type cpp20 has issue

protected:
	YGGR_PP_FRIEND_CLASS(detail::mongoc_stream_t_shared_ptr_deleter);
	typedef detail::mongoc_stream_t_shared_ptr_deleter shared_ptr_deleter_type;

public:
	c_mongo_basic_stream(void);

	c_mongo_basic_stream(org_type* pstream);

	c_mongo_basic_stream(org_type* pstream, bool need_del);

	c_mongo_basic_stream(BOOST_RV_REF(this_type) right)
		: _need_del(true)
	{
		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}
	
	~c_mongo_basic_stream(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_move(right_ref);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void) const
	{
		return _pstream.get();
	}

public:
	inline operator org_type*(void) const
	{
		return this_type::org_pointer();
	}

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !this_type::empty();
	}

public:
	inline bool close(void)
	{
		return 0 == mongoc_stream_close(_pstream.get());
	}

	inline bool flush(void)
	{
		return 0 == mongoc_stream_flush(_pstream.get());
	}

	// mongoc_dirver not found mongoc_stream_cork and mongoc_stream_uncork 
	//inline bool cork(void);
	//inline bool uncork(void);

	inline void clear(void)
	{
		// must using {} can free tmp
		{
			stream_ptr_type tmp;
			tmp.swap(_pstream);
		}
		_need_del = true;
	}

	inline org_type* release(void)
	{
		org_type* pstream = this_type::org_pointer();
		_need_del = false;
		{
			stream_ptr_type tmp;
			tmp.swap(_pstream);
		}
		_need_del = true;

		return pstream;
	}

public:
	inline bool empty(void) const
	{
		return !_pstream;
	}

	inline u32 typeinfo(void) const
	{
		return _pstream->type;
	}

public:
	inline bool& need_delete(void)
	{
		return _need_del;
	}

	inline bool need_delete(void) const
	{
		return _need_del;
	}

public:
	template<typename Container> inline
	ssize_t read(Container& cont, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		typedef Container cont_type;
		typedef detail::c_mongo_basic_stream_read_helper<cont_type> h_type;

		h_type h;

		return h(_pstream, cont, min_bytes, timeout_msec);
	}

	inline ssize_t read(void* buf, size_t count, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		assert(buf);
		return
			buf && _pstream?
				mongoc_stream_read(_pstream.get(), buf, count, min_bytes, timeout_msec)
				: -1;
	}

	inline ssize_t read(mongoc_iovec_t* iov, size_t iovcnt, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		assert(iov);
		return 
			iov && _pstream?
				mongoc_stream_readv(_pstream.get(), iov, iovcnt, min_bytes, timeout_msec)
				: -1;
	}

public:
	template<typename Container> inline
	ssize_t write(Container& cont, int32_t timeout_msec = 0) const
	{
		typedef Container cont_type;
		typedef detail::c_mongo_basic_stream_write_helper<cont_type> h_type;

		h_type h;

		return h(_pstream, cont, timeout_msec);
	}

	inline ssize_t write(void* buf, size_t count, int32_t timeout_msec = 0)
	{
		assert(buf);
		return 
			buf && _pstream?
				mongoc_stream_write(_pstream.get(), buf, count, timeout_msec)
				: -1;
	}

	inline ssize_t write(mongoc_iovec_t* iov, size_t iovcnt, int32_t timeout_msec = 0)
	{
		assert(iov);
		return 
			iov && _pstream? 
				mongoc_stream_writev(_pstream.get(), iov, iovcnt, timeout_msec)
				: -1;
	}

protected:
	void pro_init(org_type* pstream, bool need_del);
	void pro_move(this_type& right);
	void pro_swap(this_type& right);
	
protected:
	mutable stream_ptr_type _pstream;
	mutable bool _need_del;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

inline void mongoc_stream_t_shared_ptr_deleter::operator()(mongoc_stream_t* p) const
{
	if(p && _outside._need_del)
	{
		mongoc_stream_destroy(p);
	}
}

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr


namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_basic_stream)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_BASIC_STREAM_HPP__
