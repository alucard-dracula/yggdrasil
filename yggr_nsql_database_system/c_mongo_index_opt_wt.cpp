//c_mongo_index_opt_wt.cpp

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

#include <yggr/nsql_database_system/c_mongo_index_opt_wt.hpp>
#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//typedef struct
//{
//   mongoc_index_opt_storage_t  base;
//   const char                 *config_str;
//   void                       *padding[8];
//} mongoc_index_opt_wt_t;

c_mongo_index_opt_wt::c_mongo_index_opt_wt(void)
{
	this_type::prv_init();
}

c_mongo_index_opt_wt::c_mongo_index_opt_wt(const base_type* ptr)
{
	this_type::prv_init();
	this_type::prv_copy(ptr);
}

c_mongo_index_opt_wt::c_mongo_index_opt_wt(const base_type& right)
{
	this_type::prv_init();
	this_type::prv_copy(&right);
}

c_mongo_index_opt_wt::c_mongo_index_opt_wt(const this_type& right)
{
	this_type::prv_copy(right);
}

c_mongo_index_opt_wt::~c_mongo_index_opt_wt(void)
{
}

c_mongo_index_opt_wt::this_type& c_mongo_index_opt_wt::operator=(const base_type& right)
{
	return this_type::prv_copy(&right);
}

c_mongo_index_opt_wt::this_type& c_mongo_index_opt_wt::operator=(const this_type& right)
{
	return this_type::prv_copy(right);
}

void c_mongo_index_opt_wt::swap(this_type& right)
{
	this_type::prv_swap(right);
}

//private:
c_mongo_index_opt_wt::this_type& c_mongo_index_opt_wt::prv_copy(const base_type* ptr)
{
	if((!ptr) || (this == ptr))
	{
		return *this;
	}

	base_type::base = ptr->base;
	_config_str = ptr->config_str? ptr->config_str : "";
	base_type::config_str = _config_str.data();

	return *this;
}

c_mongo_index_opt_wt::this_type& c_mongo_index_opt_wt::prv_copy(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	const base_type& rbase_cref = right;
	base_type::base = rbase_cref.base;
	_config_str = right._config_str;
	base_type::config_str = _config_str.data();
	return *this;
}

void c_mongo_index_opt_wt::prv_swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	base_type& rbase_ref = right;
	yggr::swap(base_type::base, rbase_ref.base);
		
	_config_str.swap(right._config_str);
	base_type::config_str = _config_str.data();
	rbase_ref.config_str = right._config_str.data();
}

c_mongo_index_opt_wt::this_type& c_mongo_index_opt_wt::pro_move(this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type& rbase_ref = right;
	yggr::swap(base_type::base, rbase_ref.base);
		
	_config_str.swap(right._config_str);
	base_type::config_str = _config_str.data();

	return *this;
}

} // namespace nsql_database_system
} // namespace yggr

