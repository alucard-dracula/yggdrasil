// c_mongo_index_description.cpp

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

#include <yggr/nsql_database_system/c_mongo_index_description.hpp>
#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_index_description::c_mongo_index_description(void)
	: _v(0), _idx_key(string_type(), 0)
{
}

c_mongo_index_description::c_mongo_index_description(const bson_type& right)
	: _v(0), _idx_key(string_type(), 0)
{
	this_type::prv_parse_bson(right);
}

c_mongo_index_description::c_mongo_index_description(const this_type& right)
	: _v(right._v), _idx_key(right._idx_key),
		_name(right._name), _ns(right._ns)

{
}

c_mongo_index_description::~c_mongo_index_description(void)
{
}

c_mongo_index_description::this_type&
	c_mongo_index_description::operator=(const bson_type& right)
{
	this_type::prv_parse_bson(right);
	return *this;
}


c_mongo_index_description::this_type&
	c_mongo_index_description::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_v = right._v;
	//_idx_key = right._idx_key;
	_idx_key.first = right._idx_key.first;
	_idx_key.second = right._idx_key.second;
	_name = right._name;
	_ns = right._ns;
	return *this;
}

void c_mongo_index_description::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(_v, right._v);
	yggr::swap(_idx_key, right._idx_key);
	_name.swap(right._name);
	_ns.swap(right._ns);
}

//private:

void c_mongo_index_description::prv_parse_bson(const bson_type& bs)
{
	for(bson_type::const_iterator i = bs.begin(), isize = bs.end(); i != isize; ++i)
	{
		if(i.load_check(bson_typeid_def::E_BSON_TYPE_INT32, this_type::s_v_name()))
		{
			i.load(this_type::s_v_name(), _v);
			continue;
		}

		if(i.load_check(bson_typeid_def::E_BSON_TYPE_INT32, this_type::s_v_name()))
		{
			i.load(this_type::s_v_name(), _v);
			continue;
		}

		if(i.load_check(bson_typeid_def::E_BSON_TYPE_DOCUMENT, this_type::s_key_name()))
		{
			bson_type bs;
			i.load(this_type::s_key_name(), bs);
			if(bs.empty())
			{
				continue;
			}
			//_idx_key.first = boost::move(bs.begin().get_key<string_type>());
			_idx_key.first = bs.begin().get_key<string_type>();
			bs.begin().get_value(_idx_key.second);
			continue;
		}

		if(i.load_check(bson_typeid_def::E_BSON_TYPE_UTF8, this_type::s_name_name()))
		{
			i.load(this_type::s_name_name(), _name);
			continue;
		}

		if(i.load_check(bson_typeid_def::E_BSON_TYPE_UTF8, this_type::s_ns_name()))
		{
			i.load(this_type::s_ns_name(), _ns);
			continue;
		}
	}
}

} // namespace nsql_database_system
} // namespace yggr

