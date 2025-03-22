//c_mongo_find_and_modify_opts.cpp

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

#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>

#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_find_and_modify_opts::c_mongo_find_and_modify_opts(void)
{
	this_type::prv_init();
}

c_mongo_find_and_modify_opts::c_mongo_find_and_modify_opts(const org_type* ptr)
{
	this_type::prv_init();
	this_type::prv_copy(ptr);
}

c_mongo_find_and_modify_opts::c_mongo_find_and_modify_opts(const org_type& right)
{
	this_type::prv_init();
	this_type::prv_copy(boost::addressof(right));
}

c_mongo_find_and_modify_opts::c_mongo_find_and_modify_opts(const this_type& right)
{
	this_type::prv_init();
	this_type::prv_copy(right);
}

c_mongo_find_and_modify_opts::~c_mongo_find_and_modify_opts(void)
{
	if(base_type::extra.len)
	{
		bson_destroy(boost::addressof(base_type::extra));
	}
}

c_mongo_find_and_modify_opts::this_type& 
	c_mongo_find_and_modify_opts::operator=(const org_type& right)
{
	return this_type::prv_copy(&right);
}

c_mongo_find_and_modify_opts::this_type& 
	c_mongo_find_and_modify_opts::operator=(const base_type& right)
{
	return this_type::prv_copy(reinterpret_cast<const org_type*>(&right));
}

c_mongo_find_and_modify_opts::this_type& 
	c_mongo_find_and_modify_opts::operator=(const this_type& right)
{
	return this_type::prv_copy(right);
}

void c_mongo_find_and_modify_opts::swap(this_type& right)
{
	this_type::prv_swap(right);
}

//private:
void c_mongo_find_and_modify_opts::prv_init(void)
{
	mongoc_find_and_modify_opts_native_ex::s_mongoc_find_and_modify_opts_init(this);
	base_type::sort = boost::addressof(_sort);
	base_type::update = boost::addressof(_update);
	base_type::fields = boost::addressof(_fields);
}

c_mongo_find_and_modify_opts::this_type& 
	c_mongo_find_and_modify_opts::prv_copy(const org_type* ptr)
{
	if((!ptr) || (reinterpret_cast<void*>(this) == reinterpret_cast<const void*>(ptr)))
	{
		return *this;
	}

	const base_type* preal_ptr = reinterpret_cast<const base_type*>(ptr);
	assert(preal_ptr);

	if(preal_ptr->sort)
	{
		bson_native_ex::s_bson_copy(base_type::sort, preal_ptr->sort);
	}

	if(preal_ptr->update)
	{
		bson_native_ex::s_bson_copy( base_type::update, preal_ptr->update);
	}

	if(preal_ptr->fields)
	{
		bson_native_ex::s_bson_copy(base_type::fields, preal_ptr->fields);
	}

	base_type::flags = preal_ptr->flags;
	base_type::bypass_document_validation = preal_ptr->bypass_document_validation;

	base_type::max_time_ms = preal_ptr->max_time_ms;
	bson_native_ex::s_bson_copy(&(base_type::extra), &(preal_ptr->extra));

	return *this;
}

c_mongo_find_and_modify_opts::this_type& 
	c_mongo_find_and_modify_opts::prv_copy(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_sort = right._sort;
	_update = right._update;
	_fields = right._fields;

	base_type::flags = right.flags;
	base_type::bypass_document_validation = right.bypass_document_validation;

	base_type::max_time_ms = right.max_time_ms;
	bson_native_ex::s_bson_copy(&(base_type::extra), &(right.extra));

	return *this;
}

void c_mongo_find_and_modify_opts::prv_swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	_sort.swap(right._sort);
	_update.swap(right._update);
	_fields.swap(right._fields);

	yggr::swap(base_type::flags, right.flags);
	yggr::swap(base_type::bypass_document_validation, right.bypass_document_validation);

	yggr::swap(base_type::max_time_ms, right.max_time_ms);
	bson_native_ex::s_bson_swap(&(base_type::extra), &(right.extra));
}

c_mongo_find_and_modify_opts::this_type& 
	c_mongo_find_and_modify_opts::pro_move(this_type& right)
{
	if(this == &right)
	{
		return *this;
	}
	_sort.swap(right._sort);
	_update.swap(right._update);
	_fields.swap(right._fields);

	base_type::flags = right.flags;
	base_type::bypass_document_validation = right.bypass_document_validation;

	base_type::max_time_ms = right.max_time_ms;
	bson_native_ex::s_bson_swap(&(base_type::extra), &(right.extra));

	return *this;
}

} // namespace nsql_database_system
} // namespace yggr

