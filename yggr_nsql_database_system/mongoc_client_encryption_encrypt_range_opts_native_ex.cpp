// mongoc_client_encryption_encrypt_range_opts_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_client_encryption_encrypt_range_opts_native_ex.hpp>

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
void mongoc_client_encryption_encrypt_range_opts_native_ex::s_mongoc_client_encryption_encrypt_range_opts_clear(void* popts)
{
	if(!popts)
	{
		return;
	}

	yggr_client_encryption_encrypt_range_opts_type* pyggr_opts = 
		reinterpret_cast<yggr_client_encryption_encrypt_range_opts_type*>(popts);

	if(pyggr_opts->min.value.value_type)
	{
		bson_value_destroy(boost::addressof(pyggr_opts->min.value));
	}

	if(pyggr_opts->max.value.value_type)
	{
		bson_value_destroy(boost::addressof(pyggr_opts->max.value));
	}

	memset(popts, 0, sizeof(yggr_client_encryption_encrypt_range_opts_type));
}

/*static*/ 
bool mongoc_client_encryption_encrypt_range_opts_native_ex::s_mongoc_client_encryption_encrypt_range_opts_copy(void* pdst_opts, const void* psrc_opts)
{
	assert(pdst_opts && psrc_opts);
	if(!(pdst_opts && psrc_opts))
	{
		return false;
	}

	if(pdst_opts != psrc_opts)
	{
		yggr_client_encryption_encrypt_range_opts_type* pyggr_dst_opts = 
			reinterpret_cast<yggr_client_encryption_encrypt_range_opts_type*>(pdst_opts);

		const yggr_client_encryption_encrypt_range_opts_type* pyggr_src_opts = 
			reinterpret_cast<const yggr_client_encryption_encrypt_range_opts_type*>(psrc_opts);

		bson_value_native_ex::s_bson_value_copy(
			boost::addressof(pyggr_dst_opts->min.value), 
			boost::addressof(pyggr_src_opts->min.value) );
		pyggr_dst_opts->min.set = pyggr_src_opts->min.set;

		bson_value_native_ex::s_bson_value_copy(
			boost::addressof(pyggr_dst_opts->max.value), 
			boost::addressof(pyggr_src_opts->max.value) );
		pyggr_dst_opts->max.set = pyggr_src_opts->max.set;

#if !(YGGR_MONGOC_HEX() < 0x011C0000)
		pyggr_dst_opts->trim_factor = pyggr_src_opts->trim_factor;
#endif // #if !(YGGR_MONGOC_HEX() < 0x011C0000)

		pyggr_dst_opts->sparsity = pyggr_src_opts->sparsity;

		pyggr_dst_opts->precision = pyggr_src_opts->precision;
	}

	return true;
}

/*static*/ 
void mongoc_client_encryption_encrypt_range_opts_native_ex::s_mongoc_client_encryption_encrypt_range_opts_swap(void* pdst_opts, void* psrc_opts)
{
	assert(pdst_opts && psrc_opts);
	if(!(pdst_opts && psrc_opts))
	{
		return;
	}

	if(pdst_opts == psrc_opts)
	{
		return;
	}

	yggr_client_encryption_encrypt_range_opts_type* pyggr_dst_opts = 
		reinterpret_cast<yggr_client_encryption_encrypt_range_opts_type*>(pdst_opts);

	yggr_client_encryption_encrypt_range_opts_type* pyggr_src_opts = 
		reinterpret_cast<yggr_client_encryption_encrypt_range_opts_type*>(psrc_opts);

	bson_value_native_ex::s_bson_value_swap(
		boost::addressof(pyggr_dst_opts->min.value), 
		boost::addressof(pyggr_src_opts->min.value));
	::yggr::swap(pyggr_dst_opts->min.set, pyggr_src_opts->min.set);

	bson_value_native_ex::s_bson_value_swap(
		boost::addressof(pyggr_dst_opts->max.value), 
		boost::addressof(pyggr_src_opts->max.value));
	::yggr::swap(pyggr_dst_opts->max.set, pyggr_src_opts->max.set);

#if !(YGGR_MONGOC_HEX() < 0x011C0000)
	::yggr::swap(pyggr_dst_opts->trim_factor, pyggr_src_opts->trim_factor);
#endif // #if !(YGGR_MONGOC_HEX() < 0x011C0000)

	::yggr::swap(pyggr_dst_opts->sparsity, pyggr_src_opts->sparsity);

	::yggr::swap(pyggr_dst_opts->precision, pyggr_src_opts->precision);
}

/*static*/ 
void mongoc_client_encryption_encrypt_range_opts_native_ex::s_mongoc_client_encryption_encrypt_range_opts_move(void* pdst_opts, void* psrc_opts)
{
	assert(pdst_opts && psrc_opts);
	if(!(pdst_opts && psrc_opts))
	{
		return;
	}

	if(pdst_opts == psrc_opts)
	{
		return;
	}

	yggr_client_encryption_encrypt_range_opts_type* pyggr_dst_opts = 
		reinterpret_cast<yggr_client_encryption_encrypt_range_opts_type*>(pdst_opts);

	yggr_client_encryption_encrypt_range_opts_type* pyggr_src_opts = 
		reinterpret_cast<yggr_client_encryption_encrypt_range_opts_type*>(psrc_opts);

	bson_value_native_ex::s_bson_value_swap(
		boost::addressof(pyggr_dst_opts->min.value), 
		boost::addressof(pyggr_src_opts->min.value));
	pyggr_dst_opts->min.set = pyggr_src_opts->min.set;

	bson_value_native_ex::s_bson_value_swap(
		boost::addressof(pyggr_dst_opts->max.value), 
		boost::addressof(pyggr_src_opts->max.value));
	pyggr_dst_opts->max.set = pyggr_src_opts->max.set;

#if !(YGGR_MONGOC_HEX() < 0x011C0000)
	pyggr_dst_opts->trim_factor = pyggr_src_opts->trim_factor;
#endif // #if !(YGGR_MONGOC_HEX() < 0x011C0000)

	pyggr_dst_opts->sparsity = pyggr_src_opts->sparsity;

	pyggr_dst_opts->precision = pyggr_src_opts->precision;
}

} // namespace nsql_database_system
} // namespace yggr
