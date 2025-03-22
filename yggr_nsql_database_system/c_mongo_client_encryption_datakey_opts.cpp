// c_mongo_client_encryption_datakey_opts.cpp

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

#include <yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:

c_mongo_client_encryption_datakey_opts::c_mongo_client_encryption_datakey_opts(void)
{
	mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
}

c_mongo_client_encryption_datakey_opts::c_mongo_client_encryption_datakey_opts(const org_type* porg)
{
	mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
	this_type::pro_copy(porg);
}

c_mongo_client_encryption_datakey_opts::c_mongo_client_encryption_datakey_opts(const org_type& org)
{
	mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
	this_type::pro_copy(boost::addressof(org));
}

c_mongo_client_encryption_datakey_opts::c_mongo_client_encryption_datakey_opts(const this_type& right)
{
	mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
	this_type::pro_copy(right);
}

c_mongo_client_encryption_datakey_opts::~c_mongo_client_encryption_datakey_opts(void)
{
}

//public:
c_mongo_client_encryption_datakey_opts::this_type&
	c_mongo_client_encryption_datakey_opts::operator=(const org_type& right)
{
	return this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_datakey_opts::this_type&
	c_mongo_client_encryption_datakey_opts::operator=(const base_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const org_type*>(boost::addressof(right)));
}

c_mongo_client_encryption_datakey_opts::this_type&
	c_mongo_client_encryption_datakey_opts::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

//public:
void c_mongo_client_encryption_datakey_opts::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_client_encryption_datakey_opts::pro_rebuild_base(void)
{
	base_type::masterkey = _masterkey.empty()? 0 : _masterkey.org_pointer();
	base_type::keyaltnames = _keyaltnames.size()? boost::addressof(_keyaltnames[0]) : 0;
	base_type::keyaltnames_count = _keyaltnames.size();
	base_type::keymaterial = reinterpret_cast<uint8_t*>(_keymaterial.size()? boost::addressof(_keymaterial[0]) : 0);
	base_type::keymaterial_len = _keymaterial.size();
}

c_mongo_client_encryption_datakey_opts::this_type&
	c_mongo_client_encryption_datakey_opts::pro_copy(const org_type* porg)
{
	if(!porg || porg == reinterpret_cast<const org_type*>(this))
	{
		return *this;
	}

	const base_type* prbase = reinterpret_cast<const base_type*>(porg);

	if(prbase->masterkey)
	{
		_masterkey = *(prbase->masterkey);
		base_type::masterkey = _masterkey.empty()? 0 : _masterkey.org_pointer();
	}
	else
	{
		_masterkey.clear();
		base_type::masterkey = 0;
	}

	if(prbase->keyaltnames && prbase->keyaltnames_count)
	{
		this_type::set_keyaltnames(
			prbase->keyaltnames,
			iterator_ex::advance_copy(prbase->keyaltnames, prbase->keyaltnames_count));
	}
	else
	{
		base_type::keyaltnames = 0;
		base_type::keyaltnames_count = 0;
	}

	if(prbase->keymaterial && prbase->keymaterial_len)
	{
		this_type::set_keymaterial(prbase->keymaterial, prbase->keymaterial_len);
	}
	else
	{
		base_type::keymaterial = 0;
		base_type::keymaterial_len = 0;
	}

	return *this;
}

c_mongo_client_encryption_datakey_opts::this_type&
	c_mongo_client_encryption_datakey_opts::pro_copy(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	_masterkey = right._masterkey;
	base_type::masterkey = _masterkey.empty()? 0 : _masterkey.org_pointer();

	this_type::pro_s_copy_keyaltname(_keyaltnames_datas, _keyaltnames, right._keyaltnames_datas);
	base_type::keyaltnames = _keyaltnames.size()? boost::addressof(_keyaltnames[0]) : 0;
	base_type::keyaltnames_count = _keyaltnames.size();

	_keymaterial = right._keymaterial;
	base_type::keymaterial = reinterpret_cast<uint8_t*>(_keymaterial.size()? boost::addressof(_keymaterial[0]) : 0);
	base_type::keymaterial_len = _keymaterial.size();

	return *this;
}

void c_mongo_client_encryption_datakey_opts::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_masterkey.swap(right._masterkey);

	_keyaltnames_datas.swap(right._keyaltnames_datas);
	_keyaltnames.swap(right._keyaltnames);

	_keymaterial.swap(right._keymaterial);

	this_type::pro_rebuild_base();
	right.pro_rebuild_base();
}

c_mongo_client_encryption_datakey_opts::this_type&
	c_mongo_client_encryption_datakey_opts::pro_move(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	_masterkey = boost::move(right._masterkey);

	_keyaltnames_datas = boost::move(right._keyaltnames_datas);
	_keyaltnames = boost::move(right._keyaltnames);

	_keymaterial = boost::move(right._keymaterial);

	this_type::pro_rebuild_base();

	return *this;
}

//protected:
/*static*/
void c_mongo_client_encryption_datakey_opts::pro_s_copy_keyaltname(inner_string_ptr_vt_type& dst_datas, inner_char_ptr_vt_type& dst_views,
																	const inner_string_ptr_vt_type& src_datas)
{
	typedef inner_string_ptr_vt_type::const_iterator inner_string_ptr_vt_iter_type;

	inner_string_ptr_vt_type tmp_datas;
	inner_char_ptr_vt_type tmp_views;
	inner_string_ptr_type ptr;

	for(inner_string_ptr_vt_iter_type i = boost::begin(src_datas), isize = boost::end(src_datas);
			i != isize; ++i)
	{
		ptr = *i;
		if(ptr && ptr->size())
		{
			inner_string_ptr_type tmp_ptr(yggr_nothrow_new inner_string_type(*ptr));
			tmp_datas.push_back(tmp_ptr);
			tmp_views.push_back(tmp_ptr->data());
		}
	}

	dst_datas.swap(tmp_datas);
	dst_views.swap(tmp_views);
}

/*static*/
void c_mongo_client_encryption_datakey_opts::pro_s_swap_keyaltname(inner_string_ptr_vt_type& ldatas, inner_char_ptr_vt_type& lviews,
																	inner_string_ptr_vt_type& rdatas, inner_char_ptr_vt_type& rviews)
{
	ldatas.swap(rdatas);
	lviews.swap(rviews);
}

} // namespace nsql_database_system
} // namespace yggr
