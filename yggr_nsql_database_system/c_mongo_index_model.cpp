//c_mongo_index_model.cpp

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

#include <yggr/nsql_database_system/c_mongo_index_model.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_index_model::c_mongo_index_model(void)
{
	base_type::keys = base_type::opts = 0;
}

c_mongo_index_model::c_mongo_index_model(const bson_t& k)
{
	this_type::assign(k);
}

c_mongo_index_model::c_mongo_index_model(const c_bson& k)
{
	this_type::assign(k);
}

c_mongo_index_model::c_mongo_index_model(const org_type* right)
{
	this_type::assign(right);
}

c_mongo_index_model::c_mongo_index_model(const this_type& right)
{
	this_type::assign(right);
}

c_mongo_index_model::~c_mongo_index_model(void)
{
	base_type::keys = base_type::opts = 0;
}

//public:
c_mongo_index_model::this_type& c_mongo_index_model::operator=(const org_type* right)
{
	this_type::assign(right);
	return *this;
}

c_mongo_index_model::this_type& c_mongo_index_model::operator=(const this_type& right)
{
	this_type::assign(right);
	return *this;
}

//public:
void c_mongo_index_model::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_keys.swap(right._keys);
	_opts.swap(right._opts);
	this_type::pro_rebuild_base();
	right.pro_rebuild_base();
}

//public:
void c_mongo_index_model::assign(const org_type* right)
{
	if(!right)
	{
		base_type::keys = base_type::opts = 0;
	}
	else
	{
		const base_type* pbase = reinterpret_cast<const base_type*>(right);

		if(pbase->keys)
		{
			_keys = *(pbase->keys);
			base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		}

		if(pbase->opts)
		{
			_opts = *(pbase->opts);
			base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
		}
	}
}

void c_mongo_index_model::assign(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_keys = right._keys;
	_opts = right._opts;
	base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
	base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
}

//protected:
void c_mongo_index_model::pro_set_var_keys(const bson_t& k)
{
	_keys = k;
	base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
}

void c_mongo_index_model::pro_set_var_opts(const bson_t& o)
{
	_opts = o;
	base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
}

//protected:
void c_mongo_index_model::pro_rebuild_base(void)
{
	base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
	base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
}

} // namespace nsql_database_system
} // namespace yggr

