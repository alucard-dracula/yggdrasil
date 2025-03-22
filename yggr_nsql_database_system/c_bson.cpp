//c_bson.cpp

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

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/bson_iterator_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/utility/swap.hpp>

#include <cassert>
#include <memory>
#include <cstring>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

//---------------------------c_bson_const_iterator----------------------------------

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4244)
#	pragma warning (disable : 4267)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{

// c_bson_const_iterator
c_bson_const_iterator::c_bson_const_iterator(void)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
}

c_bson_const_iterator::c_bson_const_iterator(const bson_t& right)
{
	this_type::init(right);
}

c_bson_const_iterator::c_bson_const_iterator(const c_bson& right)
{
	this_type::init(right);
}

c_bson_const_iterator::c_bson_const_iterator(const base_type& right)
{
	base_type& base = *this;
	memcpy(&base, &right, sizeof(base_type));
}

c_bson_const_iterator::c_bson_const_iterator(const base_type* right)
{
	base_type& base = *this;
	if(!right)
	{
		memset(&base, 0, sizeof(base_type));
	}
	memcpy(&base, right, sizeof(base_type));
}

c_bson_const_iterator::c_bson_const_iterator(const this_type& right)
{
	base_type& base = *this;
	const base_type& rbase = right;
	memcpy(&base, &rbase, sizeof(base_type));
}

c_bson_const_iterator::~c_bson_const_iterator(void)
{
}

c_bson_const_iterator::this_type& c_bson_const_iterator::pro_operator_set(const base_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	if(0 == right.len)
	{
		base_type& base = *this;
		memset(boost::addressof(base), 0, sizeof(base_type));
	}

	base_type::raw = right.raw;
	base_type::len = right.len;
	base_type::off = right.off;
	base_type::type = right.type;
	base_type::key = right.key;
	base_type::d1 = right.d1;
	base_type::d2 = right.d2;
	base_type::d3 = right.d3;
	base_type::d4 = right.d4;
	base_type::next_off = right.next_off;
	base_type::err_off = right.err_off;
	base_type::value = right.value;
	return *this;
}

void c_bson_const_iterator::pro_swap(base_type& right)
{
	base_type& l = *this;
	bson_iterator_native_ex::s_bson_iter_swap(&l, &right);
}

c_bson_const_iterator::this_type& c_bson_const_iterator::operator++(void)
{
	bson_iterator_native_ex::s_bson_iter_next(this);
	return *this;
}

c_bson_const_iterator::this_type c_bson_const_iterator::operator++(int)
{
	c_bson_const_iterator c_iter(*this);
	bson_iterator_native_ex::s_bson_iter_next(this);
	return c_iter;
}

u32 c_bson_const_iterator::typeid_of_value(void) const
{
	assert(bson_iterator_native_ex::s_bson_iter_validate(this));
	return bson_iter_type(this);
}

bool c_bson_const_iterator::pro_compare_eq(const base_type& right) const
{
	const base_type& l = *this;
	return 
		bson_iterator_native_ex::s_bson_iter_cmp_eq(
			boost::addressof(l), boost::addressof(right));
}

void c_bson_const_iterator::init(const bson_t& right)
{
	if(0 == right.len)
	{
		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));
	}
	bson_iter_init(this, &right);
}

void c_bson_const_iterator::init_end(const bson_t& right)
{
	if(0 == right.len)
	{
		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));
	}

	base_type::key = right.len;
	base_type::type = base_type::off = base_type::key - 1;
}

const char* c_bson_const_iterator::org_get_key(void) const
{
	assert(bson_iterator_native_ex::s_bson_iter_validate(this));
	return bson_iter_key(this);
}

bool c_bson_const_iterator::org_key_cmp(const char* key, u32 key_len) const
{
	assert(key);
	assert(bson_iterator_native_ex::s_bson_iter_validate(this));

	const char* this_key = bson_iter_key(this);
	u32 this_key_len = bson_iterator_native_ex::s_bson_iter_key_size(this);

	return (key) && (this_key) 
			&& (key_len == this_key_len)
			&& (0 == memcmp(key, this_key, key_len));
}

bool c_bson_const_iterator::org_key_cmp_case(const char* key, u32 key_len) const
{
	assert(key);
	assert(bson_iterator_native_ex::s_bson_iter_validate(this));

	const char* this_key = bson_iter_key(this);
	u32 this_key_len = bson_iterator_native_ex::s_bson_iter_key_size(this);

	return (key) && (this_key) 
			&& (key_len == this_key_len)
#	ifdef BSON_OS_WIN32
			&& (0 ==_stricmp(key, this_key))
#	else
			&& (0 == strcasecmp(key, this_key))
#	endif //BSON_OS_WIN32
			;
}

void c_bson_const_iterator::init_find(const bson_t& right, const char* key, u32 key_len, const tag_noncase&)
{
	assert(key);
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));

	for(this_type i(right); !i.empty(); ++i)
	{
		if(i.org_key_cmp(key, key_len))
		{
			this_type::swap(i);
			return;
		}
	}

	this_type::init_end(right);
}

void c_bson_const_iterator::init_find(const bson_t& right, const char* key, u32 key_len, const tag_case&)
{
	assert(key);
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));

	for(this_type i(right); !i.empty(); ++i)
	{
		if(i.org_key_cmp_case(key, key_len))
		{
			this_type::swap(i);
			return;
		}
	}

	this_type::init_end(right);
}

c_bson_const_iterator::this_type 
	c_bson_const_iterator::org_find(const char* key, u32 key_len, const tag_noncase&) const
{
	assert(key);

	this_type i = *this;
	for(; !(i.empty() || i.org_key_cmp(key, key_len)); ++i);

	return i;
}

c_bson_const_iterator::this_type 
	c_bson_const_iterator::org_find(const char* key, u32 key_len, const tag_case&) const
{
	assert(key);

	this_type i = *this;
	for(; !(i.empty() || i.org_key_cmp_case(key, key_len)); ++i);

	return i;
}

std::pair<c_bson_const_iterator::this_type, 
			c_bson_const_iterator::this_type> 
	c_bson_const_iterator::org_find_descendant(const char* dotkey, const tag_noncase&) const
{
	this_type i = *this;
	this_type j;

	bool bright = bson_iterator_native_ex::s_bson_iter_find_descendant(&i, dotkey, &j);

	if(!bright)
	{
		bson_iterator_native_ex::s_bson_iter_make_end(&i, this);
		bson_iterator_native_ex::s_bson_iter_make_end(&j, this);
	}

	return std::make_pair(i, j);
}

std::pair<c_bson_const_iterator::this_type, 
			c_bson_const_iterator::this_type> 
	c_bson_const_iterator::org_find_descendant(const char* dotkey, const tag_case&) const
{
	this_type i = *this;
	this_type j;

	bool bright = bson_iterator_native_ex::s_bson_iter_find_descendant_case(&i, dotkey, &j);

	if(!bright)
	{
		bson_iterator_native_ex::s_bson_iter_make_end(&i, this);
		bson_iterator_native_ex::s_bson_iter_make_end(&j, this);
	}

	return std::make_pair(i, j);
}

// nonmember foo

bool operator==(const c_bson_const_iterator::base_type& l, const c_bson_const_iterator::base_type& r)
{
	return bson_iterator_native_ex::s_bson_iter_cmp_eq(&l, &r);
}

} // namespace nsql_database_system
} // namesapce yggr

//---------------------------c_bson----------------------------------

namespace yggr
{
namespace nsql_database_system
{

// c_bson
c_bson::c_bson(void)
{
	memset(static_cast<base_type*>(this), 0, sizeof(base_type));
	::bson_init(this);
}

c_bson::c_bson(const bson_value_t& val)
{
	memset(static_cast<base_type*>(this), 0, sizeof(base_type));
	::bson_init(this);
	this_type::pro_assign_bson_value_t(val);
}

c_bson::c_bson(const base_type& base)
{
	memset(static_cast<base_type*>(this), 0, sizeof(base_type));
	::bson_init(this);
	bson_native_ex::s_bson_copy(this, &base);
}

c_bson::c_bson(const base_type* right)
{
	memset(static_cast<base_type*>(this), 0, sizeof(base_type));

	if(!right)
	{	
		return;
	}

	::bson_init(this);
	bson_native_ex::s_bson_copy(this, right);
}

c_bson::c_bson(const this_type& right)
{
	memset(static_cast<base_type*>(this), 0, sizeof(base_type));
	::bson_init(this);
	const base_type& rbase = right;
	bson_native_ex::s_bson_copy(this, &rbase);
}

c_bson::~c_bson(void)
{
	if(!this_type::is_nil())
	{
		bson_destroy(this);
	}
}

c_bson::this_type& c_bson::operator=(const this_type& right)
{
	this_type::assign(right);
	return *this;
}

void c_bson::pro_swap(base_type& right)
{
	bson_native_ex::s_bson_swap(this, boost::addressof(right));
}

void c_bson::swap(this_type& right)
{
	base_type& right_ref = right;
	this_type::pro_swap(right_ref);
}

void c_bson::init(void)
{
	switch(base_type::len)
	{
	case 0:
	case 5:
		::bson_reinit(this);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		assert(false);
		break;
	default:
		::bson_destroy(this);
		::bson_init(this);
	}
}

// assign

void c_bson::pro_assign_bson_value_t(const bson_value_t& val)
{
	base_type& base = *this;
	bool bsuccess = false;

	switch(val.value_type)
	{
	case bson_typeid_def::E_BSON_TYPE_DOCUMENT:
		bsuccess = bson_value_native_ex::s_value_to_bson(&base, &val);
		break;
	case bson_typeid_def::E_BSON_TYPE_ARRAY:
		bsuccess = bson_value_native_ex::s_value_to_array(&base, &val);
		break;
	default:
        break;
	}
	assert(bsuccess);
}

// save_mark
bool c_bson::save_mark_start(const bson_mark_splice& mark)
{
	if(this_type::is_nil())
	{
		::bson_init(this);
	}

	assert(bson_native_ex::s_check_save_name(this, bson_typeid_def::E_BSON_TYPE_ARRAY)
			|| bson_native_ex::s_check_save_name(this, bson_typeid_def::E_BSON_TYPE_DOCUMENT));

	const uint8_t empty[5] = { 5 };

	BSON_ASSERT(!(base_type::flags & BSON_FLAG_RDONLY));
	BSON_ASSERT(!(base_type::flags & BSON_FLAG_IN_CHILD));


	if((base_type::flags & BSON_FLAG_INLINE)) 
	{
		BSON_ASSERT (base_type::len <= 120);
		if(!bson_native_ex::s_bson_grow(this, 128 - base_type::len)) 
		{
			return false;
		
		}
		BSON_ASSERT (!(base_type::flags & BSON_FLAG_INLINE));
	}

	const int32_t byte_size = 5;

	if(BSON_UNLIKELY(!bson_native_ex::s_bson_grow(this, byte_size))) 
	{
		return false;
	}

	uint8_t* pbuf = bson_native_ex::s_bson_data(this) +  base_type::len - 1;
	memcpy(pbuf, empty, 5);

	pbuf[byte_size] = 0;
	base_type::len += byte_size;

	bson_native_ex::s_bson_encode_length(this);

	/*
	* Mark the document as working on a child document so that no
	* further modifications can happen until the caller has called
	* bson_append_{document,array}_end().
	*/
	base_type::flags |= BSON_FLAG_IN_CHILD;

	/*
	* Initialize the child bson_t structure and point it at the parents
	* buffers. This allows us to realloc directly from the child without
	* walking up to the parent bson_t.
	*/

	//	member use info
	//	alloc->flags = static_cast<bson_flags_t>(alloc->flags & ~BSON_FLAG_INLINE);
	//	alloc->parent = NULL;
	//	alloc->depth = 0;
	//	alloc->buf = &alloc->alloc; // nc
	//	alloc->buflen = &alloc->alloclen; // nc
	//	alloc->offset = 0;
	//	alloc->alloc = data; // nc
	//	alloc->alloclen = req;
	//	alloc->realloc = bson_realloc_ctx;
	//	alloc->realloc_func_ctx = NULL;

	memcpy(&mark.bs_, this, sizeof(base_type));

	bson_t* pparent = &mark.bs_;
	bson_t* pchild = this;
	bson_impl_alloc_t* aparent = reinterpret_cast<bson_impl_alloc_t*>(pparent);
	bson_impl_alloc_t* achild = reinterpret_cast<bson_impl_alloc_t*>(pchild);

	// keep alloc alloclen address at this_object
	aparent->alloc = 0;
	aparent->alloclen = 0;

	bson_native_ex::s_bson_mark_set_child(pparent, pchild);

	pchild->flags = (BSON_FLAG_CHILD | BSON_FLAG_NO_FREE | BSON_FLAG_STATIC); 

	if((aparent->flags & BSON_FLAG_CHILD)) 
	{
		achild->depth = aparent->depth + 1;
	} 
	else
	{
		achild->depth = 1;
	}

	achild->parent = pparent;
	//achild->buf = aparent->buf; // now achild is this object address, so keep member var buf
	//achild->buflen = aparent->buflen; // now achild is this object address, so keep member var buflen
	achild->offset = aparent->offset + aparent->len - 1 - 5;
	achild->len = 5;
	//achild->alloc = 0; // now achild is this object address, so keep member var alloc
	//achild->alloclen = 0; // now achild is this object address, so keep member var alloclen
	//achild->realloc = aparent->realloc; // now achild is this object address, so keep member var realloc
	//achild->realloc_func_ctx = aparent->realloc_func_ctx; // now achild is this object address, so keep member var realloc_func_ctx

	bson_native_ex::s_bson_mark_set_child(pchild, 0);
	
	return true;
}

bool c_bson::save_mark_end(const bson_mark_splice& mark)
{
	if(this_type::is_nil())
	{
		return false;
	}

	bson_t* pthis = this;
	bson_t* pmark = &(mark.bs_);
	bson_t* pmark_child = bson_native_ex::s_bson_mark_get_child(pthis);

	bson_impl_alloc_t* athis = reinterpret_cast<bson_impl_alloc_t*>(pthis);
	bson_impl_alloc_t* amark = reinterpret_cast<bson_impl_alloc_t*>(pmark);


	yggr::swap(athis->flags, amark->flags);
	yggr::swap(athis->depth, amark->depth);
	yggr::swap(athis->offset, amark->offset);
	yggr::swap(athis->len, amark->len);

	if(pmark_child)
	{
		bson_impl_alloc_t* amark_child = reinterpret_cast<bson_impl_alloc_t*>(pmark_child);
		amark_child->parent = pmark;
	}

	amark->parent = pthis;
	bson_native_ex::s_bson_mark_set_child(pmark, 0);

	bson_native_ex::s_bson_mark_set_child(pthis, athis->depth? pmark : 0);
	athis->parent = 0;
	
	BSON_ASSERT((base_type::flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(mark.bs_.flags & BSON_FLAG_IN_CHILD));

	/*
	* Unmark the IN_CHILD flag.
	*/
	base_type::flags &= ~BSON_FLAG_IN_CHILD;

	/*
	* Now that we are done building the sub-document, add the size to the
	* parent, not including the default 5 byte empty document already added.
	*/
	base_type::len = (base_type::len + mark.bs_.len - 5);

	/*
	* Ensure we have a \0 byte at the end and proper length encoded at
	* the beginning of the document.
	*/
	bson_native_ex::s_bson_data(this)[base_type::len - 1] = 0; // = '\0';
	bson_native_ex::s_bson_encode_length (this);

	return true;
}

bool c_bson::pro_compare_eq(const base_type& right) const
{
	return 
		(this == boost::addressof(right))
		|| (base_type::len == 0 || right.len == 0?
				base_type::len == right.len 
				: bson_equal(this, boost::addressof(right)));
}

} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER
