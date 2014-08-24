//bson_data_type.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATA_TYPE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATA_TYPE_HPP__

#include <ctime>
#include <sstream>
#include <memory>
#include <boost/mpl/if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>

#include <yggr/nsql_database_system/bson_inside_type_access_proxy.hpp>
#include <encoding.h>


#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{
class c_bson_date;
class c_bson_time;
class c_bson_oid;
template<int BINARY_ID>
class c_bson_binary_buffer;
class c_bson_null;
class c_bson_undefined;
class c_bson_regex;
class c_bson_code;
class c_bson_symbol;

template<typename Scope>
class c_bson_code_w_scope;

class c_bson_timestamp;

} // namespace nsql_database_system
} // namespace yggr


YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_date, bson_value_type::E_BSON_TYPE_DATE)
YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_time, bson_value_type::E_BSON_TYPE_DATE)
YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_oid, bson_value_type::E_BSON_TYPE_OID)

//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<BSON_BIN_FUNC>, bson_value_type::E_BSON_TYPE_BINDATA)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<BSON_BIN_BINARY_OLD>, bson_value_type::E_BSON_TYPE_BINDATA)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<BSON_BIN_UUID>, bson_value_type::E_BSON_TYPE_BINDATA)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<BSON_BIN_MD5>, bson_value_type::E_BSON_TYPE_BINDATA)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<BSON_BIN_USER>, bson_value_type::E_BSON_TYPE_BINDATA)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<bson_binary_type_ex::E_BSON_BIN_TYPE_U8_BUFFER>, bson_value_type::E_BSON_TYPE_BINDATA)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_binary_buffer<bson_binary_type_ex::E_BSON_BIN_TYPE_PACKET>, bson_value_type::E_BSON_TYPE_BINDATA)

namespace yggr
{
namespace nsql_database_system
{

template<int BINARY_ID>
struct bson_value_typeid< c_bson_binary_buffer<BINARY_ID> >
{
	enum { value = bson_value_type::E_BSON_TYPE_BINDATA };
};

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_null, bson_value_type::E_BSON_TYPE_NULL)
YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_undefined, bson_value_type::E_BSON_TYPE_UNDEFINED)
YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_regex, bson_value_type::E_BSON_TYPE_REGEX)
YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_code, bson_value_type::E_BSON_TYPE_CODE)
YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_symbol, bson_value_type::E_BSON_TYPE_SYMBOL)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_code_w_scope, bson_value_type::E_BSON_TYPE_CODEWSCOPE) // see c_bson.h

namespace yggr
{
namespace nsql_database_system
{

template<typename Scope>
struct bson_value_typeid< c_bson_code_w_scope<Scope> >
{
	BOOST_MPL_ASSERT((typename boost::mpl::if_< typename boost::is_same<Scope, bson>::type,
						boost::mpl::bool_<false>,
						boost::mpl::bool_<true> >));
	enum { value = bson_value_type::E_BSON_TYPE_CODEWSCOPE };
};

} // namespace nsql_database_system
} // namesapce yggr

YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_timestamp, bson_value_type::E_BSON_TYPE_TIMESTAMP)

#ifdef _DEBUG
namespace yggr
{
namespace nsql_database_system
{

enum
{
	save_name_check_num = 0xff
};

bool check_save_name(void* cur, char num);
} // namespace nsql_database_system
}// namespace yggr

#endif // _DEBUG

namespace yggr
{
namespace nsql_database_system
{

// -----------------------------c_bson_date--------------------------------------------

class c_bson_date
{
private:
	typedef c_bson_date this_type;

public:
	c_bson_date(void)
		: _date(std::time(0))
	{
	}

	explicit c_bson_date(u64 date)
		:_date(date)
	{
	}

	c_bson_date(const this_type& right)
		: _date(right._date)
	{
	}

	~c_bson_date(void)
	{
	}

	operator const u64(void) const
	{
		return _date;
	}

	this_type& operator=(u64 date)
	{
		_date = date;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		_date = right._date;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_date, right._date);
	}

	bool operator==(u64 date) const
	{
		return _date == date;
	}

	bool operator==(const c_bson_date& right) const
	{
		return _date == right._date;
	}

	bool operator<(u64 date) const
	{
		return _date < date;
	}

	bool operator<(const c_bson_date& right) const
	{
		return _date < right._date;
	}

	bool operator<=(u64 date) const
	{
		return _date <= date;
	}

	bool operator<=(const c_bson_date& right) const
	{
		return _date <= right._date;
	}

	bool operator>(u64 date) const
	{
		return _date > date;
	}

	bool operator>(const c_bson_date& right) const
	{
		return _date > right._date;
	}

	bool operator>=(u64 date) const
	{
		return _date >= date;
	}

	bool operator>=(const c_bson_date& right) const
	{
		return _date >= right._date;
	}

	bool operator!=(u64 date) const
	{
		return _date != date;
	}

	bool operator!=(const c_bson_date& right) const
	{
		return _date != right._date;
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	//save's def

	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_date(&bs, name.c_str(), _date);
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		if(bson_ensure_space(&bs, 8) == BSON_ERROR)
		{
			return false;
		}
		bson_append64(&bs , &_date);
		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		_date = bson_iterator_date(&iter);
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		_date = bson_iterator_date(&iter);
	}

private:
	u64 _date;
};

// -----------------------------c_bson_time--------------------------------------------
class c_bson_time
{
private:
	typedef c_bson_time this_type;

public:
	c_bson_time(void)
		: _time(std::time(0))
	{
	}

	explicit c_bson_time(u64 time)
		: _time(time)
	{
	}

	c_bson_time(const this_type& right)
		:_time(right._time)
	{
	}

	~c_bson_time(void)
	{
	}

	operator u64(void) const
	{
		return _time;
	}

	this_type& operator=(u64 time)
	{
		_time = time;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_time = right._time;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_time, right._time);
	}

	bool operator==(u64 time) const
	{
		return _time == time;
	}

	bool operator==(const this_type& right) const
	{
		return _time == right._time;
	}

	bool operator<(u64 time) const
	{
		return _time < time;
	}

	bool operator<(const this_type& right) const
	{
		return _time < right._time;
	}

	bool operator<=(u64 time) const
	{
		return _time <= time;
	}

	bool operator<=(const this_type& right) const
	{
		return _time <= right._time;
	}

	bool operator>(u64 time) const
	{
		return _time > time;
	}

	bool operator>(const this_type& right) const
	{
		return _time > right._time;
	}

	bool operator>=(u64 time) const
	{
		return _time >= time;
	}

	bool operator>=(const this_type& right) const
	{
		return _time >= right._time;
	}

	bool operator!=(u64 time) const
	{
		return _time != time;
	}

	bool operator!=(const this_type& right) const
	{
		return _time != right._time;
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_time_t(&bs, name.c_str(), _time);
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		if(bson_ensure_space(&bs, 8) == BSON_ERROR)
		{
			return false;
		}
		u64 tmp = _time * 1000;
		bson_append64(&bs , &tmp);
		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}
		_time = bson_iterator_time_t(&iter);
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		_time = bson_iterator_time_t(&iter);
	}

private:
	u64 _time;
};

// -----------------------------c_bson_oid--------------------------------------------

class c_bson_oid
{
public:
	typedef bson_oid_t id_type;

private:
	typedef c_bson_oid this_type;

public:
	explicit c_bson_oid(bool time_only = false)
	{
		if(time_only)
		{
			bson_oid_generated_time(&_oid);
		}
		else
		{
			bson_oid_gen(&_oid);
		}
	}

	explicit c_bson_oid(const std::string& str_oid)
	{
		bson_oid_from_string(&_oid, str_oid.c_str());
	}

	explicit c_bson_oid(const id_type& oid)
	{
		memcpy(&_oid, &oid, sizeof(id_type));
	}

	c_bson_oid(const this_type& right)
	{
		memcpy(&_oid, &(right._oid), sizeof(id_type));
	}

	~c_bson_oid(void)
	{
	}

	operator id_type&(void)
	{
		return _oid;
	}

	operator const id_type&(void) const
	{
		return _oid;
	}

	operator id_type*(void)
	{
		return &_oid;
	}

	operator const id_type*(void) const
	{
		return &_oid;
	}

	operator std::string(void) const
	{
		char tstr[25] = {0};
		bson_oid_to_string(&_oid, tstr);
		return std::string(tstr);
	}

	this_type& operator=(const std::string& str_oid)
	{
		bson_oid_from_string(&_oid, str_oid.c_str());
		return *this;
	}

	c_bson_oid& operator=(const id_type& oid)
	{
		memcpy(&_oid, &oid, sizeof(id_type));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		memcpy(&_oid, &(right._oid), sizeof(id_type));
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		id_type tmp;
		memcpy(&tmp, &_oid, sizeof(id_type));
		memcpy(&_oid, &right._oid, sizeof(id_type));
		memcpy(&right._oid, &tmp, sizeof(id_type));
	}

	bool operator==(const std::string& str_oid) const
	{
		id_type tid;
		bson_oid_from_string(&tid, str_oid.c_str());
		return !memcmp(&_oid, &tid, sizeof(id_type));
	}

	bool operator==(const id_type& oid) const
	{
		return !memcmp(&_oid, &oid, sizeof(id_type));
	}

	bool operator==(const this_type& right) const
	{
		return !memcmp(&_oid, &(right._oid), sizeof(id_type));
	}

	bool operator!=(const std::string& str_oid) const
	{
		id_type tid;
		bson_oid_from_string(&tid, str_oid.c_str());
		return memcmp(&_oid, &tid, sizeof(id_type));
	}

	bool operator!=(const id_type& oid) const
	{
		return memcmp(&_oid, &oid, sizeof(id_type));
	}

	bool operator!=(const c_bson_oid& right) const
	{
		return memcmp(&_oid, &(right._oid), sizeof(id_type));
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_oid(&bs, name.c_str(), &_oid);
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		if(bson_ensure_space(&bs, 12) == BSON_ERROR)
		{
			return false;
		}
		bson_append(&bs, &_oid, 12);
		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}
		id_type *pid = bson_iterator_oid(&iter);

		if(!pid)
		{
			return;
		}

		//_oid = *pid;
		memcpy(&_oid, pid, sizeof(id_type));
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		id_type *pid = bson_iterator_oid(&iter);

		if(!pid)
		{
			return;
		}

		//_oid = *pid;
		memcpy(&_oid, pid, sizeof(id_type));
	}

private:
	id_type _oid;
};

//--------------------------------------c_bson_binary_buffer-------------------------------------

template<int BINARY_ID>
class c_bson_binary_buffer
{
public:
	enum
	{
		E_TYPE_ID = BINARY_ID,
		E_compile_u8 = 0xff
	};

	typedef std::string buf_type;
	typedef u8 buffer_val_type;

private:
	typedef c_bson_binary_buffer this_type;

public:

	explicit c_bson_binary_buffer(buf_type& buf)
		: _buf(buf)
	{
	}

	c_bson_binary_buffer(const this_type& right)
		:_buf(right._buf)
	{
	}

	~c_bson_binary_buffer(void)
	{
	}

	const c_bson_binary_buffer& copy_buf(const u8* pbuf, u32 len)
	{
		_buf.resize(len);
		memcpy(&_buf[0], pbuf, sizeof(buffer_val_type) * len);
		return *this;
	}

	this_type& operator=(const buf_type& buf)
	{
		size_type len = buf.size();
		_buf.resize(len);
		memcpy(&_buf[0], &buf[0], sizeof(buffer_val_type)* len);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		size_type len = right.size();
		_buf.resize(len);
		memcpy(&_buf[0], &((right._buf)[0]), sizeof(buffer_val_type)* len);
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_buf.swap(right._buf);
	}

	bool operator==(const buf_type& buf) const
	{
		size_type len = buf.size();
		if(len != buf.size())
		{
			return false;
		}

		if(_buf.empty() && buf.empty())
		{
			return true;
		}

		return !memcmp(&_buf[0], &buf[0], sizeof(buffer_val_type) * len);
	}

	bool operator==(const this_type& right) const
	{
		size_type len = _buf.size();
		if(len != right.size())
		{
			return false;
		}

		if(_buf.empty() && right.empty())
		{
			return true;
		}

		return !memcmp(&_buf[0], &((right._buf)[0]), sizeof(buffer_val_type) * len);
	}

	bool operator!=(const buf_type& buf) const
	{
		return !(*this == buf);
	}

	bool operator!=(const this_type& right) const
	{
		return !(*this == right);
	}

	const size_type size(void) const
	{
		return _buf.size();
	}

	void resize(size_type size)
	{
		_buf.resize(size);
	}

	bool empty(void) const
	{
		return _buf.empty();
	}

	const u8* buf(void) const
	{
		return (const u8*)(_buf.data());
	}

	operator const u8*(void) const
	{
		return (const u8*)(_buf.data());
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def

	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_binary(&bs, name.c_str(), E_TYPE_ID, (const char*)_buf.data(), _buf.size());
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
//#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
//#	endif //_DEBUG
		int bufsize = (E_TYPE_ID == BSON_BIN_BINARY_OLD?
						4 + 1 + 4 + _buf.size() : 4 + 1 + _buf.size());
		int len = _buf.size();
		char sub_typeid = static_cast<char>(E_TYPE_ID);


		if(bson_ensure_space(&bs, bufsize) == BSON_ERROR)
		{
			return false;
		}

		if(E_TYPE_ID == BSON_BIN_BINARY_OLD)
		{
			int subtwolen = len + 4;
			bson_append32(&bs, &subtwolen);
			bson_append_byte(&bs, sub_typeid);
			bson_append32(&bs, &len);
			bson_append(&bs, &_buf[0], len);
		}
		else
		{
			bson_append32(&bs, &len);
			bson_append_byte(&bs, sub_typeid);
			bson_append(&bs, &_buf[0], len);
		}

		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert((iter.load_check(bson_value_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID)));
		if(!iter.load_check(bson_value_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID))
		{
			return;
		}

		size_type len = bson_iterator_bin_len(&iter);
		copy_buf((const u8*)bson_iterator_bin_data(&iter), len);
	}

	template<typename Bson_Iter>
	void load_value(Bson_Iter& iter)
	{
		size_type len = bson_iterator_bin_len(&iter);
		copy_buf((const u8*)bson_iterator_bin_data(&iter), len);
	}

private:
	buf_type& _buf;
};

typedef c_bson_binary_buffer<BSON_BIN_FUNC> c_bson_func_binary_buffer;
typedef c_bson_binary_buffer<BSON_BIN_BINARY_OLD> c_bson_old_binary_buffer;
typedef c_bson_binary_buffer<BSON_BIN_UUID> c_bson_uuid_binary_buffer;
typedef c_bson_binary_buffer<BSON_BIN_MD5> c_bson_md5_binary_buffer;
typedef c_bson_binary_buffer<BSON_BIN_USER> c_bson_user_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_type_ex::E_BSON_BIN_TYPE_U8_BUFFER> c_bson_u8_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_type_ex::E_BSON_BIN_TYPE_PACKET> c_bson_bpak_binary_buffer;

//---------------------------c_bson_null-----------------------------------

class c_bson_null
{
private:
	typedef c_bson_null this_type;
public:
	c_bson_null(void)
	{
	}

	~c_bson_null(void)
	{
	}

	inline void swap(this_type& right)
	{
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	//save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_null(&bs, name.c_str());
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		return true;
		// don't need save
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		// don't need load
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		//don't need load
	}

};

//--------------------------c_bson_undefined---------------------------------

class c_bson_undefined
{
private:
	typedef c_bson_undefined this_type;
public:
	c_bson_undefined(void)
	{
	}

	~c_bson_undefined(void)
	{
	}

	void swap(void)
	{
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	//save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_undefined(&bs, name.c_str());
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		// don't need save
		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		// don't need load
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		//don't need load
	}
};

//-------------------------c_bson_string_item----------------------------------------

class c_bson_string_item
{
private:
	typedef c_bson_string_item this_type;
protected:
	c_bson_string_item(void)
	{
	}

	c_bson_string_item(const char* str)
		: _str(str)
	{
	}

	c_bson_string_item(const std::string& str)
		: _str(str)
	{
	}

	c_bson_string_item(const this_type& right)
		: _str(right._str)
	{
	}

	~c_bson_string_item(void)
	{
	}

public:
	this_type& operator=(const char* str)
	{
		_str = str;
		return *this;
	}

	this_type& operator=(const std::string& str)
	{
		_str = str;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_str = right._str;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_str.swap(right._str);
	}

	operator std::string&(void)
	{
		return _str;
	}

	operator const std::string&(void) const
	{
		return _str;
	}

	std::string& str(void)
	{
		return _str;
	}

	const std::string& str(void) const
	{
		return _str;
	}

protected:
	std::string _str;
};

//-------------------c_bson_regex-------------------------------------------

class c_bson_regex
	: public c_bson_string_item
{
public:
	typedef c_bson_string_item base_type;

private:
	typedef c_bson_regex this_type;

public:

	c_bson_regex(void)
	{
	}

	c_bson_regex(const char* regex_str, const char* val)
		: base_type(regex_str), _opts(val)
	{
	}

	c_bson_regex(const std::string& regex_str, const std::string& val)
		: base_type(regex_str), _opts(val)
	{
	}

	c_bson_regex(const this_type& right)
		: base_type(right), _opts(right._opts)
	{
	}

	~c_bson_regex(void)
	{
	}

	std::string& regex(void)
	{
		return base_type::str();
	}

	const std::string& regex(void) const
	{
		return base_type::str();
	}

	std::string& text(void)
	{
		return _opts;
	}

	const std::string& text(void) const
	{
		return _opts;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& l = *this;
		const base_type& r = right;

		l = r;
		_opts = right._opts;

		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		base_type::swap(right);
		_opts.swap(right._opts);
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_regex(&bs, name.c_str(), base_type::str().c_str(), _opts.c_str());
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		const int plen = base_type::str().size() + 1;
		const int olen = _opts.size() + 1;

		if(bson_check_string(&bs, base_type::str().c_str(), plen - 1) == BSON_ERROR)
		{
			return false;
		}

		if(bson_ensure_space(&bs, plen + olen) == BSON_ERROR)
		{
			return false;
		}

		bson_append(&bs, base_type::str().c_str() , base_type::_str.size() + 1);
		bson_append(&bs, _opts.c_str() , _opts.size() + 1 );
		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		base_type::str() = bson_iterator_regex(&iter);
		_opts = bson_iterator_regex_opts(&iter);
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		base_type::str() = bson_iterator_regex(&iter);
		_opts = bson_iterator_regex_opts(&iter);
	}

private:
	std::string _opts;
};

//-------------------------------c_bson_code-------------------------------------

class c_bson_code
	: public c_bson_string_item
{
public:
	typedef c_bson_string_item base_type;

private:
	typedef c_bson_code this_type;

public:
	c_bson_code(void)
	{
	}

	c_bson_code(const char* code_str)
		: base_type(code_str)
	{
	}

	c_bson_code(const std::string& code_str)
		: base_type(code_str)
	{
	}

	c_bson_code(const this_type& right)
		: base_type(right)
	{
	}

	~c_bson_code(void)
	{
	}

	std::string& code(void)
	{
		return base_type::str();
	}

	const std::string& code(void) const
	{
		return base_type::str();
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_code(&bs, name.c_str(), base_type::str().c_str());
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		int sl = base_type::str().size() + 1;
		if(bson_check_string(&bs, base_type::str().c_str(), sl - 1) == BSON_ERROR)
		{
			return false;
		}

		if(bson_ensure_space(&bs, 4 + sl) == BSON_ERROR)
		{
			return false;
		}

		bson_append32(&bs, &sl);
		bson_append(&bs, base_type::str().c_str(), sl - 1);
		bson_append(&bs, "\0", 1 );

		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		base_type::str() = bson_iterator_code(&iter);
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		base_type::str() = bson_iterator_code(&iter);
	}
};

//--------------------------------c_bson_symbol---------------------------------

class c_bson_symbol 
	: public c_bson_string_item
{
public:
	typedef c_bson_string_item base_type;

private:
	typedef c_bson_symbol this_type;

public:
	c_bson_symbol(void)
	{
	}

	c_bson_symbol(const char* symbol_str)
		: base_type(symbol_str)
	{
	}

	c_bson_symbol(const std::string& symbol_str)
		: base_type(symbol_str)
	{
	}

	c_bson_symbol(const this_type& right)
		: base_type(right)
	{
	}

	~c_bson_symbol(void)
	{
	}

	std::string& symbol(void)
	{
		return base_type::str();
	}

	const std::string& symbol(void) const
	{
		return base_type::str();
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_symbol(&bs, name.c_str(), base_type::str().c_str());
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		int sl = base_type::str().size() + 1;

		if(bson_check_string(&bs, base_type::str().c_str(), sl - 1) == BSON_ERROR)
		{
			return false;
		}

		if(bson_ensure_space(&bs, 4 + sl) == BSON_ERROR)
		{
			return false;
		}

		bson_append32(&bs, &sl);
		bson_append(&bs, base_type::str().c_str(), sl - 1);
		bson_append(&bs, "\0", 1 );

		return true;
	}

	template<typename Bson_Iter>
	void load(Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		base_type::str() = bson_iterator_string(&iter);
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		base_type::str() = bson_iterator_string(&iter);
	}
};

//-----------------c_bson_code_w_scope--------------------------

template<typename Scope>
class c_bson_code_w_scope 
	: public c_bson_string_item
{
	BOOST_MPL_ASSERT((typename boost::mpl::if_< typename boost::is_same<Scope, bson>::type,
						boost::mpl::bool_<false>,
						boost::mpl::bool_<true> >));
public:
	typedef Scope scope_type;
	typedef c_bson_string_item base_type;

private:
	typedef c_bson_code_w_scope this_type;

public:
	c_bson_code_w_scope(void)
	{
	}

	c_bson_code_w_scope(const char* code, scope_type& scope)
		: base_type(code), _scope(scope)
	{
	}

	c_bson_code_w_scope(const std::string& code, scope_type& scope)
		: base_type(code), _scope(scope)
	{
	}

	c_bson_code_w_scope(const this_type& right)
		: base_type(right), _scope(right._scope)
	{
	}

	~c_bson_code_w_scope(void)
	{
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& l = *this;
		const base_type& r = right;

		l = r;
		_scope = right._scope;

		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		base_type::swap(right);
		_scope.swap(right._scope);
	}

	std::string& code(void)
	{
		return base_type::str();
	}

	const std::string& code(void) const
	{
		return base_type::str();
	}

	scope_type& scope(void)
	{
		return _scope;
	}

	const scope_type& scope(void) const
	{
		return _scope;
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	//save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		return !bson_append_code_w_scope(&bs, name.c_str(), base_type::str().c_str(), _scope);
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
		BOOST_MPL_ASSERT((boost::is_same<Scope, Bson>));
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG
		int sl = base_type::str().size() + 1;
		int size = 4 + 4 + sl + bson_size(&_scope);

		if(bson_ensure_space(&bs, size) == BSON_ERROR)
		{
			return false;
		}

		bson_append32(&bs, &size);
		bson_append32(&bs, &sl);
		bson_append(&bs, base_type::str().c_str(), sl);
		bson_append(&bs, bson_data(&_scope), bson_size(&_scope));

		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< typename boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< typename boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		base_type::str() = bson_iterator_code(&iter);
		bson scope;
		bson_iterator_code_scope(&iter, &scope);
		_scope = scope;
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		base_type::str() = bson_iterator_code(&iter);
		bson scope;
		bson_iterator_code_scope(&iter, &scope);
		//bson_copy(&_scope, &scope);
		_scope = scope;
	}

private:
	scope_type _scope;
};

//--------------------c_bson_timestamp------------------------

class c_bson_timestamp 
	: public bson_timestamp_t
{
public:
	typedef bson_timestamp_t base_type;
private:
	typedef c_bson_timestamp this_type;

public:
	c_bson_timestamp(void)
	{
		i = 0;
		t = 0;
	}

	c_bson_timestamp(int inc, int time)
	{
		i = inc;
		t = time;
	}

	c_bson_timestamp(const base_type& right)
	{
		i = right.i;
		t = right.t;
	}

	c_bson_timestamp(const this_type& right)
	{
		i = right.i;
		t = right.t;
	}

	~c_bson_timestamp(void)
	{
	}

	this_type& operator=(const base_type& right)
	{
		i = right.i;
		t = right.t;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		i = right.i;
		t = right.t;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(i, right.i);
		std::swap(t, right.t);
	}

protected:
	friend class bson_inside_type_access_proxy::access<true>;
	// save's def
	template<typename Bson>
	bool save(Bson& bs, const std::string& name) const
	{
		base_type* pbase = &(*this);
		return !bson_append_timestamp(&bs, name.c_str(), pbase);
	}

	template<typename Bson>
	bool save_value(Bson& bs) const
	{
#	ifdef _DEBUG
		assert(check_save_name(bs.cur, static_cast<char>(save_name_check_num)));
#	endif //_DEBUG

		if(bson_ensure_space(&bs, 8) == BSON_ERROR)
		{
			return false;
		}

		bson_append32(&bs, &(this->i));
		bson_append32(&bs, &(this->t));
		return true;
	}

	template<typename Bson_Iter>
	void load(const Bson_Iter& iter, const std::string& name)
	{
		assert(iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_value_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		*this = bson_iterator_timestamp(&iter);
	}

	template<typename Bson_Iter>
	void load_value(const Bson_Iter& iter)
	{
		*this = bson_iterator_timestamp(&iter);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_date& l, 
					yggr::nsql_database_system::c_bson_date& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_time& l, 
					yggr::nsql_database_system::c_bson_time& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_oid& l, 
					yggr::nsql_database_system::c_bson_oid& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

template<int BINARY_ID>
inline void swap(yggr::nsql_database_system::c_bson_binary_buffer<BINARY_ID>& l,
					yggr::nsql_database_system::c_bson_binary_buffer<BINARY_ID>& r)
{
	l.swap(r);
}

} // namespace std


namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_null& l, 
					yggr::nsql_database_system::c_bson_null& r)
{
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_undefined& l,
					yggr::nsql_database_system::c_bson_undefined& r)
{
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_string_item& l, 
					yggr::nsql_database_system::c_bson_string_item& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_regex& l, 
					yggr::nsql_database_system::c_bson_regex& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_code& l,
					yggr::nsql_database_system::c_bson_code& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_symbol& l,
					yggr::nsql_database_system::c_bson_symbol& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

template<typename Scope>
inline void swap(yggr::nsql_database_system::c_bson_code_w_scope<Scope>& l,
					yggr::nsql_database_system::c_bson_code_w_scope<Scope>& r)
{
	l.swap(r);
}

} // namespace std

namespace std
{

inline void swap(yggr::nsql_database_system::c_bson_timestamp& l, 
					yggr::nsql_database_system::c_bson_timestamp& r)
{
	l.swap(r);
}

} // namespace std

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATA_TYPE_HPP__
