//c_bson.cpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/support/max_min_undef.ipp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <memory>
#include <encoding.h>
#include <limits>
#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

// c_bson_const_iterator
c_bson_const_iterator::c_bson_const_iterator(void)
{
	cur = 0;
	first = 0;
}

c_bson_const_iterator::c_bson_const_iterator(const bson& right)
{
	init(right);
}

c_bson_const_iterator::c_bson_const_iterator(const c_bson& right)
{
	init(right);
}

c_bson_const_iterator::c_bson_const_iterator(const bson_iterator& right)
{
	memcpy(this, &right, sizeof(bson_iterator));
}

//c_bson_const_iterator::c_bson_const_iterator(BOOST_RV_REF(c_bson_const_iterator) right)
//{
//	cur = 0;
//	first = 0;
//	right.swap(*this);
//}

c_bson_const_iterator::c_bson_const_iterator(const c_bson_const_iterator& right)
{
	const bson_iterator* p(&right);
	memcpy(this, p, sizeof(bson_iterator));
}

c_bson_const_iterator::~c_bson_const_iterator(void)
{
}

c_bson_const_iterator::this_type& c_bson_const_iterator::operator++(void)
{
	bson_iterator_next(this);
	return *this;
}

c_bson_const_iterator::this_type c_bson_const_iterator::operator++(int)
{
	c_bson_const_iterator c_iter(*this);
	bson_iterator_next(this);
	return c_iter;
}

c_bson_const_iterator::this_type& c_bson_const_iterator::operator=(const bson& right)
{
	init(right);
	return *this;
}

c_bson_const_iterator::this_type& c_bson_const_iterator::operator=(const c_bson& right)
{
	init(right);
	return *this;
}

c_bson_const_iterator::this_type& c_bson_const_iterator::operator=(const bson_iterator& right)
{
	base_type& base = *this;
	memcpy(&base, &right, sizeof(bson_iterator));
	return *this;
}

c_bson_const_iterator::this_type& c_bson_const_iterator::operator=(const this_type& right)
{
	base_type& base = *this;
	const base_type& rbase = right;

	memcpy(&base, &rbase, sizeof(bson_iterator));
	return *this;
}

bool c_bson_const_iterator::empty(void) const
{
	return base_type::cur == 0;
}

std::string c_bson_const_iterator::key(void) const
{
	return std::string(bson_iterator_key(this));
}

u32 c_bson_const_iterator::typeid_of_value(void) const
{
	return bson_iterator_type(this);
}

c_bson_const_iterator& c_bson_const_iterator::skip_auto_id(void)
{
	if(this_type::key() == "_id" && typeid_of_value() == bson_value_type::E_BSON_TYPE_OID)
	{
		return this_type::operator++();
	}

	return *this;
}

void c_bson_const_iterator::swap(bson_iterator& right)
{
	if(this == &right)
	{
		return;
	}
	bson_iterator tmp = {0};
	memcpy(&tmp, &right, sizeof(bson_iterator));
	memcpy(&right, this, sizeof(bson_iterator));
	memcpy(this, &tmp, sizeof(bson_iterator));
}

void c_bson_const_iterator::swap(c_bson_const_iterator& right)
{
	if(this == &right)
	{
		return;
	}

	bson_iterator tmp = {0};
	memcpy(&tmp, &right, sizeof(bson_iterator));
	memcpy(&right, this, sizeof(bson_iterator));
	memcpy(this, &tmp, sizeof(bson_iterator));
}

bool c_bson_const_iterator::load_check(u32 type_id, const std::string& name) const
{
	//u32 now_id = this_type::typeid_of_value();
	std::string now_name = this_type::key();
	return (this_type::typeid_of_value() == type_id) && (name == this_type::key());
}

bool c_bson_const_iterator::load_check(u32 type_id, const std::string& name, u32 sub_type_id) const
{
	assert(type_id == bson_value_type::E_BSON_TYPE_BINDATA);
	return this_type::load_check(type_id, name) && sub_type_id == bson_iterator_bin_type(this);
}

// load
void c_bson_const_iterator::load(const std::string& name, s8& val) const
{
	assert(this_type::load_check(bson_value_typeid<s8>::value, name));
	if(!this_type::load_check(bson_value_typeid<s8>::value, name))
	{
		return;
	}
	val = static_cast<s8>(bson_iterator_int(this));
}

void c_bson_const_iterator::load(const std::string& name, char& val) const
{
	assert(this_type::load_check(bson_value_typeid<char>::value, name));
	if(!this_type::load_check(bson_value_typeid<char>::value, name))
	{
		return;
	}
	val = static_cast<char>(bson_iterator_int(this));
}

void c_bson_const_iterator::load(const std::string& name, wchar_t& val) const
{
	assert(this_type::load_check(bson_value_typeid<wchar_t>::value, name));
	if(!this_type::load_check(bson_value_typeid<wchar_t>::value, name))
	{
		return;
	}
	val = static_cast<wchar_t>(bson_iterator_int(this));
}

void c_bson_const_iterator::load(const std::string& name, u8& val) const
{
	assert(this_type::load_check(bson_value_typeid<u8>::value, name));
	if(!this_type::load_check(bson_value_typeid<u8>::value, name))
	{
		return;
	}
	val = static_cast<u8>(bson_iterator_int(this));
}

void c_bson_const_iterator::load(const std::string& name, s16& val) const
{
	assert(this_type::load_check(bson_value_typeid<s16>::value, name));
	if(!this_type::load_check(bson_value_typeid<s16>::value, name))
	{
		return;
	}
	val = static_cast<s16>(bson_iterator_int(this));
}

void c_bson_const_iterator::load(const std::string& name, u16& val) const
{
	assert(this_type::load_check(bson_value_typeid<u16>::value, name));
	if(!this_type::load_check(bson_value_typeid<u16>::value, name))
	{
		return;
	}

	val = static_cast<u16>(bson_iterator_int(this));
}

void c_bson_const_iterator::load(const std::string& name, s32& val) const
{
	assert(this_type::load_check(bson_value_typeid<s32>::value, name));
	if(!this_type::load_check(bson_value_typeid<s32>::value, name))
	{
		return;
	}

	val = bson_iterator_int(this);
}

void c_bson_const_iterator::load(const std::string& name, u32& val) const
{
	assert(this_type::load_check(bson_value_typeid<u32>::value, name));
	if(!this_type::load_check(bson_value_typeid<u32>::value, name))
	{
		return;
	}
	s32 nval = bson_iterator_int(this);
	memcpy(&val, &nval, sizeof(u32));
}

void c_bson_const_iterator::load(const std::string& name, s64& val) const
{
	assert(this_type::load_check(bson_value_typeid<s64>::value, name));
	if(!this_type::load_check(bson_value_typeid<s64>::value, name))
	{
		return;
	}
	val = bson_iterator_long(this);
}

void c_bson_const_iterator::load(const std::string& name, u64& val) const
{
	assert(this_type::load_check(bson_value_typeid<u64>::value, name));
	if(!this_type::load_check(bson_value_typeid<u64>::value, name))
	{
		return;
	}

	s64 nval = bson_iterator_long(this);
	memcpy(&val, &nval, sizeof(u64));
}

void c_bson_const_iterator::load(const std::string& name, bool& val) const
{
	assert(this_type::load_check(bson_value_typeid<bool>::value, name));
	if(!this_type::load_check(bson_value_typeid<bool>::value, name))
	{
		return;
	}

	val = static_cast<bool>(bson_iterator_bool(this));
}

void c_bson_const_iterator::load(const std::string& name, f32& val) const
{
	assert(this_type::load_check(bson_value_typeid<f32>::value, name));
	if(!this_type::load_check(bson_value_typeid<f32>::value, name))
	{
		return;
	}

	val = static_cast<f32>(bson_iterator_double(this));
}

void c_bson_const_iterator::load(const std::string& name, f64& val) const
{
	assert(this_type::load_check(bson_value_typeid<f64>::value, name));
	if(!this_type::load_check(bson_value_typeid<f64>::value, name))
	{
		return;
	}
	val = bson_iterator_double(this);
}

void c_bson_const_iterator::load(const std::string& name, std::string& val) const
{
	assert(this_type::load_check(bson_value_typeid<std::string>::value, name));
	if(!this_type::load_check(bson_value_typeid<std::string>::value, name))
	{
		return;
	}
	val = bson_iterator_string(this);
}

void c_bson_const_iterator::load(const std::string& name, c_bson& val) const
{
	assert(this_type::load_check(bson_value_typeid<c_bson>::value, name));
	if(!this_type::load_check(bson_value_typeid<c_bson>::value, name))
	{
		return;
	}

	bson tval;
	bson_iterator_subobject(this, &tval);
	val = tval;
}

// load_name
bool c_bson_const_iterator::load_name(int type, const std::string& name) const
{
	return this_type::load_check(type, name);
}

bool c_bson_const_iterator::load_mark_start(c_bson_const_iterator& tmp_iter, const bson_mark_splice&)
{
	tmp_iter = *this;
	bson_iterator_subiterator(&tmp_iter, this);
	this_type::operator++();
	return true;
}

bool c_bson_const_iterator::load_mark_end(const c_bson_const_iterator& tmp_iter, const bson_mark_splice&)
{
	*this = tmp_iter;
	this_type::operator++();
	return true;
}

//load_value
void c_bson_const_iterator::load_value(s8& val) const
{
	val = static_cast<s8>(bson_iterator_int(this));
}

void c_bson_const_iterator::load_value(char& val) const
{
	val = static_cast<char>(bson_iterator_int(this));
}

void c_bson_const_iterator::load_value(wchar_t& val) const
{
	val = static_cast<wchar_t>(bson_iterator_int(this));
}

void c_bson_const_iterator::load_value(u8& val) const
{
	val = static_cast<u8>(bson_iterator_int(this));
}

void c_bson_const_iterator::load_value(s16& val) const
{
	val = static_cast<s16>(bson_iterator_int(this));
}

void c_bson_const_iterator::load_value(u16& val) const
{
	val = static_cast<u16>(bson_iterator_int(this));
}

void c_bson_const_iterator::load_value(s32& val) const
{
	val = bson_iterator_int(this);
}

void c_bson_const_iterator::load_value(u32& val) const
{
	s32 nval = bson_iterator_int(this);
	memcpy(&val, &nval, sizeof(u32));
}

void c_bson_const_iterator::load_value(s64& val) const
{
	val = bson_iterator_long(this);
}

void c_bson_const_iterator::load_value(u64& val) const
{
	s64 nval = bson_iterator_long(this);
	memcpy(&val, &nval, sizeof(u64));
}

void c_bson_const_iterator::load_value(bool& val) const
{
	val = static_cast<bool>(bson_iterator_bool(this));
}

void c_bson_const_iterator::load_value(f32& val) const
{
	val = static_cast<f32>(bson_iterator_double(this));
}

void c_bson_const_iterator::load_value(f64& val) const
{
	val = bson_iterator_double(this);
}

void c_bson_const_iterator::load_value(std::string& val) const
{
	val = bson_iterator_string(this);
}

void c_bson_const_iterator::load_value(c_bson& val) const
{
	bson tval;
	bson_iterator_subobject(this, &tval);
	val = tval;
}

c_bson_const_iterator c_bson_const_iterator::sub_array_iterator(const std::string& name) const
{
	assert(this_type::load_check(bson_value_type::E_BSON_TYPE_ARRAY, name));
	if(!this_type::load_check(bson_value_type::E_BSON_TYPE_ARRAY, name))
	{
		return this_type();
	}

	this_type iter;
	bson_iterator_subiterator(this, &iter);
	++iter;
	return iter;
}

void c_bson_const_iterator::init(const bson& right)
{
	bson_iterator_init(this, &right);
}

} // namespace nsql_database_system
} // yggr

// nonmember foo

bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l, yggr::u8 r)
{
	const bson_iterator& base = l;
	return bson_iterator_type(&base) == r;
}

bool operator==(yggr::u8 l, const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	const bson_iterator& base = r;
	return bson_iterator_type(&base) == l;
}

bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l, const bson_iterator& r)
{
	const bson_iterator& base = l;
	return !memcmp(&base, &r, sizeof(bson_iterator));
}

bool operator==(const bson_iterator& l, const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	const bson_iterator& base = r;
	return !memcmp(&base, &l, sizeof(bson_iterator));
}

bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l,
					const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	const bson_iterator& left = l;
	const bson_iterator& right = r;
	return !memcmp(&left, &right, sizeof(bson_iterator));
}

bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l, yggr::u8 r)
{
	return !operator==(l, r);
}

bool operator!=(yggr::u8 l, const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	return !operator==(l, r);
}

bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l, const bson_iterator& r)
{
	return !operator==(l, r);
}

bool operator!=(const bson_iterator& l, const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	return !operator==(l, r);
}

bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l,
					const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	return !operator==(l, r);
}

#define YGGR_PP_C_BSON_CONST_ITERATOR_IMPLEMENT_SWAP() \
	void swap(bson_iterator& l, yggr::nsql_database_system::c_bson_const_iterator& r){ \
		r.swap(l); } \
	void swap(yggr::nsql_database_system::c_bson_const_iterator& l, bson_iterator& r){ \
		l.swap(r); } \
	void swap(yggr::nsql_database_system::c_bson_const_iterator& l, yggr::nsql_database_system::c_bson_const_iterator& r){ \
		l.swap(r); }

namespace std
{
	YGGR_PP_C_BSON_CONST_ITERATOR_IMPLEMENT_SWAP();
} // namespace std

namespace boost
{
	YGGR_PP_C_BSON_CONST_ITERATOR_IMPLEMENT_SWAP();
} // namespace boost

#undef YGGR_PP_C_BSON_CONST_ITERATOR_IMPLEMENT_SWAP

namespace yggr
{
namespace nsql_database_system
{
// c_bson
c_bson::c_bson(void)
{
	bson_init(this);
}

c_bson::c_bson(bool bempty)
{
    if(bempty)
    {
        bson_empty(this);
        return;
    }

    bson_init(this);
	//(bempty? bson_empty(this) : bson_init(this));
}

//c_bson::c_bson(BOOST_RV_REF(base_type) base)
//{
//	bson_empty(this);
//	this_type::swap(boost::forward<base_type>(base));
//}

c_bson::c_bson(const base_type& base)
{
	if(bson_copy(this, &base))
	{
		bson_init(this);
	}
}

c_bson::c_bson(const c_bson& right)
{
	const base_type& base = right;
	if(bson_copy(this, &base))
	{
		bson_init(this);
	}
}

c_bson::~c_bson(void)
{
	if(this_type::is_operator_enable())
	{
		bson_destroy(this);
	}
}

void c_bson::init(void)
{
	bson_destroy(this);
	bson_init(this);
}

void c_bson::clear(void)
{
	init();
}

c_bson::const_iterator c_bson::begin(void) const
{
	const_iterator iter(*this);
	return ++iter;
}

c_bson::const_iterator c_bson::end(void) const
{
	const_iterator iter;
	iter.cur = this_type::data() + this_type::size() - 1;
	iter.first = 0;
	return iter;
}

s32 c_bson::finish(void)
{
	return finished? 0 : bson_finish(this);
}

bool c_bson::is_finished(void) const
{
	return finished;
}

bool c_bson::is_operator_enable(void) const
{
	return memcmp(base_type::data, "\005\0\0\0\0", 7);
}

bool c_bson::empty(void) const
{
	return !finished || 0 == memcmp(base_type::data, "\005\0\0\0\0", 7);
}

c_bson::this_type& c_bson::operator=(const base_type& base)
{
	bson_destroy(this);
	if(bson_copy(this, &base))
	{
		bson_init(this);
	}
	return *this;
}

//c_bson& c_bson::operator=(BOOST_RV_REF(c_bson) right)
//{
//	this_type::swap(right);
//	return *this;
//}

c_bson::this_type& c_bson::operator=(const this_type& right)
{
	bson_destroy(this);
	const base_type& base = right;
	if(bson_copy(this, &base))
	{
		bson_init(this);
	}
	return *this;
}

const char* c_bson::data(void) const
{
	return bson_data(this);
}

size_type c_bson::size(void) const
{
	return bson_size(this);
}

//void c_bson::swap(BOOST_RV_REF(base_type) right)
//{
//	bson& right_ref = right;
//	bson tmp = {0};
//	memcpy(&tmp, &right_ref, sizeof(bson));
//	memcpy(&right_ref, this, sizeof(bson));
//	memcpy(this, &tmp, sizeof(bson));
//}

void c_bson::swap(bson& right)
{
	bson tmp = {0};
	memcpy(&tmp, &right, sizeof(bson));
	memcpy(&right, this, sizeof(bson));
	memcpy(this, &tmp, sizeof(bson));
}

//void c_bson::swap(BOOST_RV_REF(c_bson) right)
//{
//	c_bson& r = right;
//	bson tmp = {0};
//	memcpy(&tmp, &r, sizeof(bson));
//	memcpy(&r, this, sizeof(bson));
//	memcpy(this, &tmp, sizeof(bson));
//}

void c_bson::swap(c_bson& right)
{
	bson tmp = {0};
	memcpy(&tmp, &right, sizeof(bson));
	memcpy(&right, this, sizeof(bson));
	memcpy(this, &tmp, sizeof(bson));
}

// save
bool c_bson::save(const std::string& name, s8 val)
{
	return !bson_append_int(this, name.c_str(), s32(val));
}

bool c_bson::save(const std::string& name, char val)
{
	return !bson_append_int(this, name.c_str(), s32(val));
}

bool c_bson::save(const std::string& name, wchar_t val)
{
	return !bson_append_int(this, name.c_str(), s32(val));
}

bool c_bson::save(const std::string& name, u8 val)
{
	return !bson_append_int(this, name.c_str(), s32(val));
}

bool c_bson::save(const std::string& name, s16 val)
{
	return !bson_append_int(this, name.c_str(), s32(val));
}

bool c_bson::save(const std::string& name, u16 val)
{
	return !bson_append_int(this, name.c_str(), s32(val));
}

bool c_bson::save(const std::string& name, s32 val)
{
	return !bson_append_int(this, name.c_str(),val);
}

bool c_bson::save(const std::string& name, u32 val)
{
	return !bson_append_int(this, name.c_str(), *(reinterpret_cast<s32*>(&val)));
}

bool c_bson::save(const std::string& name, s64 val)
{
	return !bson_append_long(this, name.c_str(), val);
}

bool c_bson::save(const std::string& name, u64 val)
{
	return !bson_append_long(this, name.c_str(), *(reinterpret_cast<s64*>(&val)));
}

bool c_bson::save(const std::string& name, bool val)
{
	return !bson_append_bool(this, name.c_str(), bson_bool_t(val));
}

bool c_bson::save(const std::string& name, f32 val)
{
	return !bson_append_double(this, name.c_str(), f64(val));
}

bool c_bson::save(const std::string& name, f64 val)
{
	return !bson_append_double(this, name.c_str(), val);
}

bool c_bson::save(const std::string& name, const std::string& val)
{
	return !bson_append_string(this, name.c_str(), val.c_str());
}

bool c_bson::save(const std::string& name, const c_bson& val)
{
	return !bson_append_bson(this, name.c_str(), val);
}

c_bson& c_bson::save_array_start(const std::string& name)
{
	bson_append_start_array(this, name.c_str());
	return *this;
}

c_bson& c_bson::save_array_end(void)
{
	bson_append_finish_object(this);
	return *this;
}

c_bson& c_bson::save_object_start(const std::string& name)
{
	bson_append_start_object(this, name.c_str());
	return *this;
}

c_bson& c_bson::save_object_end(void)
{
	bson_append_finish_object(this);
	return *this;
}

// save_name
bool c_bson::save_name(int type, const std::string& name)
{
	if(base_type::finished)
	{
		base_type::err |= BSON_ALREADY_FINISHED;
		return false;
	}

	int len = name.size() + 1;
#ifdef _DEBUG
	if(bson_ensure_space(this, 1 + len + 1) == BSON_ERROR)
#else
	if(bson_ensure_space(this, 1 + len) == BSON_ERROR)
#endif // _DEBUG
	{
		return false;
	}

	if(bson_check_field_name(this, name.c_str(), len - 1) == BSON_ERROR)
	{
		bson_builder_error(this);
		return false;
	}

	bson_append_byte(this, (char)type);
	bson_append(this, name.c_str(), len);

#ifdef _DEBUG
	memset(base_type::cur, save_name_check_num, 1);
#endif // _DEBUG
	return true;
}

// save_mark
bool c_bson::save_mark_start(const bson_mark_splice&)
{
	static u32 zero = 0;
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG

	if(bson_ensure_space(this, 5) == BSON_ERROR)
	{
		return false;
	}

	base_type::stack[ base_type::stackPos++ ] = base_type::cur - base_type::data;
	bson_append32(this, &zero);
	return true;
}

bool c_bson::save_mark_end(const bson_mark_splice&)
{
	char* start = 0;
	int i = 0;
	if(bson_ensure_space(this, 1) == BSON_ERROR)
	{
		return false;
	}

	bson_append_byte(this, 0);
	start = base_type::data + base_type::stack[--base_type::stackPos];
	i = base_type::cur - start;
	bson_little_endian32( start, &i );
	return true;
}

#ifdef _DEBUG
bool check_save_name(void* cur, char num)
{
	assert(cur);
	return 0 == memcmp(&num, cur, 1);
}
#endif //_DEBUG

#if defined(_DEBUG) && defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4309)
#endif //_DEBUG && _MSC_VER

//save_value
bool c_bson::save_value(s8 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR)
	{
		return false;
    }
	s32 tmp = val;
	bson_append32(this, &tmp);
	return true;
}

bool c_bson::save_value(char val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR)
	{
		return false;
    }
	s32 tmp = val;
	bson_append32(this, &tmp);
	return true;
}

bool c_bson::save_value(wchar_t val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<wchar_t>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR)
	{
		return false;
    }
	s32 tmp = val;
	bson_append32(this, &tmp);
	return true;
}

bool c_bson::save_value(u8 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR)
	{
		return false;
    }
	s32 tmp = val;
	bson_append32(this, &tmp);
	return true;
}
bool c_bson::save_value(s16 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR)
	{
		return false;
    }
	s32 tmp = val;
	bson_append32(this, &tmp);
	return true;
}
bool c_bson::save_value(u16 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR )
	{
		return false;
    }
	s32 tmp = val;
	bson_append32(this, &tmp);
	return true;
}

bool c_bson::save_value(s32 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR )
	{
		return false;
    }
	bson_append32(this, &val);
	return true;
}

bool c_bson::save_value(u32 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 4) == BSON_ERROR )
	{
		return false;
    }
	bson_append32(this, &val);
	return true;
}

bool c_bson::save_value(s64 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 8) == BSON_ERROR )
	{
		return false;
    }
	bson_append64(this, &val);
	return true;
}

bool c_bson::save_value(u64 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 8) == BSON_ERROR )
	{
		return false;
    }
	bson_append64(this, &val);
	return true;
}

bool c_bson::save_value(bool val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 1) == BSON_ERROR )
	{
		return false;
    }
	bson_append_byte(this, val);
	return true;
}

bool c_bson::save_value(f32 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 8) == BSON_ERROR )
	{
		 return false;
    }
	f64 tmp = val;
	bson_append64(this, &tmp);
	return true;
}

bool c_bson::save_value(f64 val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, 8) == BSON_ERROR )
	{
        return false;
    }
	bson_append64(this, &val);
	return true;
}

bool c_bson::save_value(const std::string& val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	s32 sl = val.size() + 1;
	if(bson_ensure_space(this, 4 + sl) == BSON_ERROR )
	{
        return false;
    }
	bson_append32(this , &sl);
	bson_append(this, val.c_str(), sl - 1);
	bson_append(this, "\0" , 1);
	return true;
}

bool c_bson::save_value(const c_bson& val)
{
#ifdef _DEBUG
	assert(check_save_name(base_type::cur, static_cast<char>(save_name_check_num)));
#endif //_DEBUG
	if(bson_ensure_space(this, val.size()) == BSON_ERROR )
	{
        return false;
    }

	bson_append(this, val.data(), val.size());
	return true;
}

#if defined(_DEBUG) && defined(_MSC_VER)
#pragma warning(pop)
#endif //_DEBUG && _MSC_VER

// other
MONGO_EXPORT void bson_print_raw( const char *data , int depth ) {
    bson_iterator i;
    const char *key;
    int temp;
    bson_timestamp_t ts;
    char oidhex[25];
    bson scope;
    bson_iterator_from_buffer( &i, data );

    while ( bson_iterator_next( &i ) ) {
        bson_type t = bson_iterator_type( &i );
        if ( t == 0 )
            break;
        key = bson_iterator_key( &i );

        for ( temp=0; temp<=depth; temp++ )
            bson_printf( "\t" );
        bson_printf( "%s : %d \t " , key , t );
        switch ( t ) {
        case BSON_DOUBLE:
            bson_printf( "%f" , bson_iterator_double( &i ) );
            break;
        case BSON_STRING:
            bson_printf( "%s" , bson_iterator_string( &i ) );
            break;
        case BSON_SYMBOL:
            bson_printf( "SYMBOL: %s" , bson_iterator_string( &i ) );
            break;
        case BSON_OID:
            bson_oid_to_string( bson_iterator_oid( &i ), oidhex );
            bson_printf( "%s" , oidhex );
            break;
        case BSON_BOOL:
            bson_printf( "%s" , bson_iterator_bool( &i ) ? "true" : "false" );
            break;
        case BSON_DATE:
            bson_printf( "%ld" , ( long int )bson_iterator_date( &i ) );
            break;
        case BSON_BINDATA:
            bson_printf( "BSON_BINDATA" );
            break;
        case BSON_UNDEFINED:
            bson_printf( "BSON_UNDEFINED" );
            break;
        case BSON_NULL:
            bson_printf( "BSON_NULL" );
            break;
        case BSON_REGEX:
            bson_printf( "BSON_REGEX: %s", bson_iterator_regex( &i ) );
            break;
        case BSON_CODE:
            bson_printf( "BSON_CODE: %s", bson_iterator_code( &i ) );
            break;
        case BSON_CODEWSCOPE:
            bson_printf( "BSON_CODE_W_SCOPE: %s", bson_iterator_code( &i ) );
            /* bson_init( &scope ); */ /* review - stepped on by bson_iterator_code_scope? */
            bson_iterator_code_scope( &i, &scope );
            bson_printf( "\n\t SCOPE: " );
            bson_print( &scope );
            /* bson_destroy( &scope ); */ /* review - causes free error */
            break;
        case BSON_INT:
            bson_printf( "%d" , bson_iterator_int( &i ) );
            break;
        case BSON_LONG:
            bson_printf( "%lld" , ( uint64_t )bson_iterator_long( &i ) );
            break;
        case BSON_TIMESTAMP:
            ts = bson_iterator_timestamp( &i );
            bson_printf( "i: %d, t: %d", ts.i, ts.t );
            break;
        case BSON_OBJECT:
        case BSON_ARRAY:
            bson_printf( "\n" );
            bson_print_raw( bson_iterator_value( &i ) , depth + 1 );
            break;
        default:
            bson_errprintf( "can't print type : %d\n" , t );
        }
        bson_printf( "\n" );
    }
}

std::ostream& out_bson_data(std::ostream& os, const char* data, int depth)
{
	bson_iterator i;
    const char *key;
    int temp;
    bson_timestamp_t ts;
    char oidhex[25];
    bson scope;
    bson_iterator_from_buffer( &i, data );

    while ( bson_iterator_next( &i ) ) {
        bson_type t = bson_iterator_type( &i );
        if ( t == 0 )
            break;
        key = bson_iterator_key( &i );

        for ( temp=0; temp<=depth; temp++ )
			os << "\t";
		os << key << " : " << t << " \t";
        switch ( t ) {
        case BSON_DOUBLE:
			os << bson_iterator_double( &i );
            break;
        case BSON_STRING:
			os << bson_iterator_string( &i );
            break;
        case BSON_SYMBOL:
			os << "SYMBOL: " << bson_iterator_string(&i);
            break;
        case BSON_OID:
            bson_oid_to_string( bson_iterator_oid( &i ), oidhex );
			os << oidhex;
            break;
        case BSON_BOOL:
			os << (bson_iterator_bool( &i ) ? "true" : "false");
            break;
        case BSON_DATE:
			os << (( long int )bson_iterator_date( &i ));
            break;
        case BSON_BINDATA:
			os << "BSON_BINDATA";
            break;
        case BSON_UNDEFINED:
			os << "BSON_UNDEFINED";
            break;
        case BSON_NULL:
			os << "BSON_NULL";
            break;
        case BSON_REGEX:
			os << "BSON_REGEX: " << bson_iterator_regex( &i );
            break;
        case BSON_CODE:
			os << "BSON_CODE: " << bson_iterator_code( &i );
            break;
        case BSON_CODEWSCOPE:
			os << "BSON_CODE_W_SCOPE: " << bson_iterator_code( &i );
			bson_iterator_code_scope( &i, &scope );
			os << "\n\t SCOPE: ";
			out_bson_data(os, scope.data, 0);
            break;
        case BSON_INT:
			os << bson_iterator_int( &i );
            break;
        case BSON_LONG:
			os << (( uint64_t )bson_iterator_long( &i ));
            break;
        case BSON_TIMESTAMP:
            ts = bson_iterator_timestamp( &i );
			os << "i: " << ts.i << ", t: " << ts.t;
            break;
        case BSON_OBJECT:
        case BSON_ARRAY:
			os << "\n";
            out_bson_data( os, bson_iterator_value( &i ) , depth + 1 );
            break;
        default:
			os << "can't print type : " << t << "\n";
        }
		os << "\n";
    }
	return os;
}

} // namespace nsql_database_system
} // namespace yggr

std::ostream& operator<<(std::ostream& os, const yggr::nsql_database_system::c_bson& cb)
{
	return cb.is_finished()? yggr::nsql_database_system::out_bson_data(os, cb.data(), 0) : os;
}


bool operator==(const bson& l, const yggr::nsql_database_system::c_bson& r)
{
	if(bson_size(&l) != r.size())
	{
		return false;
	}

	return 0 == memcmp(bson_data(&l), r.data(), r.size());
}

bool operator==(const yggr::nsql_database_system::c_bson& l, const bson& r)
{
	if(l.size() != bson_size(&r))
	{
		return false;
	}

	return 0 == memcmp(l.data(), bson_data(&r), l.size());
}

bool operator==(const yggr::nsql_database_system::c_bson& l, const yggr::nsql_database_system::c_bson& r)
{
	if(l.size() != r.size())
	{
		return false;
	}

	return 0 == memcmp(l.data(), r.data(), l.size());
}

bool operator!=(const bson& l, const yggr::nsql_database_system::c_bson& r)
{
	return !operator==(l, r);
}

bool operator!=(const yggr::nsql_database_system::c_bson& l, const bson& r)
{
	return !operator==(l, r);
}

bool operator!=(const yggr::nsql_database_system::c_bson& l, const yggr::nsql_database_system::c_bson& r)
{
	return !operator==(l, r);
}

#define YGGR_PP_C_BSON_IMPLEMENT_SWAP() \
	void swap(bson& l, yggr::nsql_database_system::c_bson& r){ \
		r.swap(l); } \
	void swap(yggr::nsql_database_system::c_bson& l, bson& r){ \
		l.swap(r); } \
	void swap(yggr::nsql_database_system::c_bson& l, yggr::nsql_database_system::c_bson& r){ \
		l.swap(r); }

namespace std
{
	YGGR_PP_C_BSON_IMPLEMENT_SWAP();
} // namespace std

namespace boost
{
	YGGR_PP_C_BSON_IMPLEMENT_SWAP();
} // namespace boost

#undef YGGR_PP_C_BSON_IMPLEMENT_SWAP

