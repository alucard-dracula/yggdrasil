// c_mongo_index_description.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_DESCRIPTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_DESCRIPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>

#include <utility>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_index_description
{
public:
	typedef ::yggr::string string_type;
	typedef std::pair<string_type, s32> index_key_type;
	typedef c_bson bson_type;

private:
	typedef c_mongo_index_description this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_index_description(void);
	c_mongo_index_description(const bson_type& right);

	c_mongo_index_description(BOOST_RV_REF(this_type) right)
		: _v(boost::move(right._v)), 
			_idx_key(boost::move(right._idx_key)),
			_name(boost::move(right._name)), 
			_ns(boost::move(right._ns))
	{
	}

	c_mongo_index_description(const this_type& right);
	~c_mongo_index_description(void);

public:
	this_type& operator=(const bson_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_v, boost::move(right._v));
		copy_or_move_or_swap(_idx_key, boost::move(right._idx_key));
		copy_or_move_or_swap(_name, boost::move(right._name));
		copy_or_move_or_swap(_ns, boost::move(right._ns));

		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline s32& v(void)
	{
		return _v;
	}

	inline s32 v(void) const
	{
		return _v;
	}

	inline index_key_type& index_order(void)
	{
		return _idx_key;
	}

	inline const index_key_type& index_key(void) const
	{
		return _idx_key;
	}

	inline string_type& name(void)
	{
		return _name;
	}

	inline const string_type& name(void) const
	{
		return _name;
	}

	inline string_type& ns(void)
	{
		return _ns;
	}

	inline const string_type& ns(void) const
	{
		return _ns;
	}

	inline const string_type& index_key_name(void) const
	{
		return _idx_key.first;
	}

	inline s32 index_key_order(void) const
	{
		return _idx_key.second;
	}

public:
	inline static const char* s_v_name(void)
	{
		return "v";
	}

	inline static const char* s_key_name(void)
	{
		return "key";
	}

	inline static const char* s_name_name(void)
	{
		return "name";
	}

	inline static const char* s_ns_name(void)
	{
		return "ns";
	}

private:
	void prv_parse_bson(const bson_type& bs);

private:
	s32 _v;
	index_key_type _idx_key;
	string_type _name;
	string_type _ns;

};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_index_description)
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


#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_DESCRIPTION_HPP__

