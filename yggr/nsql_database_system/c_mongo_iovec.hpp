//c_mongo_iovec.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_IOVEC_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_IOVEC_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/move/move.hpp>
#include <yggr/typeof/typeof.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/bytes/byte_size.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/iterator.hpp>

#include <boost/type_traits/is_class.hpp>

#if !defined(MONGOC_INSIDE)
#	define MONGOC_INSIDE
#	define YGGR_USING_MONGOC_INSIDE
#endif // MONGOC_INSIDE

#if !defined(BUILD_MONGODB_DRIVER_YGGR_VER)
#   define BUILD_MONGODB_DRIVER_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#include <mongoc-iovec.h>

#if defined(BUILD_MONGODB_DRIVER_YGGR_VER)
#   undef BUILD_MONGODB_DRIVER_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#if defined(YGGR_USING_MONGOC_INSIDE)
#	undef MONGOC_INSIDE
#	undef YGGR_USING_MONGOC_INSIDE
#endif // YGGR_USING_MONGOC_INSIDE

namespace yggr
{
namespace nsql_database_system
{

//typedef struct
//{
//   u_long  iov_len;
//   char   *iov_base;
//} mongoc_iovec_t;

class c_mongo_iovec
	: public mongoc_iovec_t
{
public:
	typedef mongoc_iovec_t base_type;
	typedef base_type org_type;

	typedef YGGR_TYPEOF(gen_value_t(org_type().iov_len)) iov_len_type;

private:
	typedef c_mongo_iovec this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_iovec(void);
	c_mongo_iovec(u32 byte_size, char* buf_ptr);

	template<typename Buffer>
	c_mongo_iovec(Buffer& buf)
	{
		typedef Buffer buf_type;
		typedef typename boost::range_iterator<buf_type>::type iter_type;
		BOOST_MPL_ASSERT((is_random_access_iterator<iter_type>));

		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));

		base_type::iov_len = bytes::byte_size(buf);
		if(base_type::iov_len)
		{
			base_type::iov_base = reinterpret_cast<char*>(&(*boost::begin(buf)));
		}
	}

	c_mongo_iovec(const base_type& right);

	c_mongo_iovec(BOOST_RV_REF(this_type) right)
	{
		base_type::iov_len = right.iov_len;
		base_type::iov_base = right.iov_base;
	}

	c_mongo_iovec(const this_type& right);
	~c_mongo_iovec(void);

public:
	template<typename Buffer>
	this_type& operator=(Buffer& buf)
	{
		typedef Buffer buf_type;
		typedef typename boost::range_iterator<buf_type>::type iter_type;
		BOOST_MPL_ASSERT((is_random_access_iterator<iter_type>));

		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));

		base_type::iov_len == bytes::byte_size(buf);
		if(base_type::iov_len)
		{
			base_type::iov_base = reinterpret_cast<char*>(&(*boost::begin(buf)));
		}

		return *this;
	}

	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::iov_len = right_ref.iov_len;
		base_type::iov_base = right_ref.iov_base;
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

public:
	//inline operator org_type*(void)
	//{
	//	return this;
	//}

	//inline operator const org_type*(void) const
	//{
	//	return this;
	//}

	//template<typename T> inline
	//operator T*(void)
	//{
	//	return reinterpret_cast<T*>(base_type::iov_base);
	//}

	//template<typename T> inline
	//operator const T*(void) const
	//{
	//	return reinterpret_cast<const T*>(base_type::iov_base);
	//}

public:
	template<typename T> inline
	T* buffer(void)
	{
		return reinterpret_cast<T*>(base_type::iov_base);
	}

	template<typename T> inline
	const T* buffer(void) const
	{
		return reinterpret_cast<const T*>(base_type::iov_base);
	}

	inline iov_len_type& size(void)
	{
		return base_type::iov_len;
	}

	inline iov_len_type size(void) const
	{
		return base_type::iov_len;
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_iovec& l, c_mongo_iovec::base_type& r)
{ 
	l.swap(r); 
} 

inline void swap(c_mongo_iovec::base_type& l, c_mongo_iovec& r) 
{ 
	r.swap(l); 
} 

inline void swap(c_mongo_iovec& l, c_mongo_iovec& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_IOVEC_HPP__
