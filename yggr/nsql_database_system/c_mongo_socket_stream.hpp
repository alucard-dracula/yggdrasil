//c_mongo_socket_stream.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SOCKET_STREAM_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SOCKET_STREAM_HPP__

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/nsql_database_system/c_mongo_basic_stream.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_socket_stream
	: public c_mongo_basic_stream
{
public:
	typedef c_mongo_basic_stream base_type;
	typedef mongoc_stream_socket_t org_type;
	typedef base_type::org_type base_org_type;
	typedef base_type::stream_ptr_type stream_ptr_type;

private:
	typedef c_mongo_socket_stream this_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(c_mongo_socket_stream) // not using this_type cpp20 has issue

public:
	c_mongo_socket_stream(void);

	c_mongo_socket_stream(base_org_type* pstream);
	c_mongo_socket_stream(base_org_type* pstream, bool need_del);

	c_mongo_socket_stream(mongoc_socket_t* psocket);

	c_mongo_socket_stream(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	~c_mongo_socket_stream(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::pro_move(right_ref);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void) const
	{
		return reinterpret_cast<org_type*>(base_type::org_pointer());
	}

public:
	inline operator org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	inline mongoc_socket_t* socket(void) const
	{
		org_type* ptr = this_type::org_pointer();
		return ptr? mongoc_stream_socket_get_socket(ptr) : 0; 
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
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_socket_stream)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SOCKET_STREAM_HPP__
