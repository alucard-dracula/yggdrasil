//c_bson.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_HPP__

#include <yggr/utility/swap.hpp>

#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/nsql_database_system/bson_data_type.hpp>
#include <yggr/nonable/noncopyable.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_access_proxy;
class c_bson;
class c_bson_const_iterator;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_VALUE_TYPE_ID(c_bson, bson_value_type::E_BSON_TYPE_OBJECT)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_code_w_scope<c_bson>, bson_value_type::E_BSON_TYPE_CODEWSCOPE)
//YGGR_PP_BSON_VALUE_TYPE_ID(c_bson_code_w_scope<bson>, bson_value_type::E_BSON_TYPE_CODEWSCOPE) // this scope have memory leak //don't use it

namespace yggr
{
namespace nsql_database_system
{

class c_bson_access_proxy
{
public:
	template<typename Bson, typename Val>
	static bool save(Bson& bs, const std::string& name, const Val& val)
	{
		return bs.save(name, val);
	}

	template<typename Val, typename Bson>
	static bool save_begin(Bson& bs, const std::string& name)
	{
		return bs.save_begin<Val>(name);
	}

	template<typename Val, typename Bson>
	static bool save_end(Bson& bs)
	{
		return bs.save_end<Val>();
	}

	template<typename Bson, typename Val>
	static bool save_value(Bson& bs, const Val& val)
	{
		return bs.save_value(val);
	}

	template<typename BsonIter, typename Val>
	static void load(const BsonIter& iter, const std::string& name, Val& val)
	{
		iter.load(name, val);
	}

	template<typename Val, typename BsonIter>
	static bool load_begin(BsonIter& tmp_iter, BsonIter& iter, const std::string& name)
	{
		return iter.load_begin<Val>(tmp_iter, name);
	}

	template<typename Val, typename BsonIter>
	static bool load_end(const BsonIter& tmp_iter, BsonIter& iter)
	{
		return iter.load_end<Val>(tmp_iter);
	}

	template<typename BsonIter, typename Val>
	static void load_value(const BsonIter& iter, Val& val)
	{
		iter.load_value(val);
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

// c_bson_const_iterator
class c_bson_const_iterator : public bson_iterator
{
private:
	friend class c_bson_access_proxy;
public:
	typedef bson_iterator base_type;

private:
	typedef c_bson_const_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_bson_const_iterator(void);
	explicit c_bson_const_iterator(const bson& right);
	explicit c_bson_const_iterator(const c_bson& right);

	explicit c_bson_const_iterator(BOOST_RV_REF(base_type) right)
	{
		cur = right.cur;
		first = right.first;
	}

	explicit c_bson_const_iterator(const base_type& right);

	explicit c_bson_const_iterator(BOOST_RV_REF(this_type) right)
	{
		cur = right.cur;
		first = right.first;
	}

	c_bson_const_iterator(const this_type& right);

	~c_bson_const_iterator(void);

	inline operator bson_iterator*(void)
	{
		return this;
	}

	inline operator const bson_iterator*(void) const
	{
		return this;
	}

	inline operator bson_iterator&(void)
	{
		return *this;
	}

	inline operator const bson_iterator&(void) const
	{
		return *this;
	}

	this_type& operator++(void);
	this_type operator++(int);

	this_type& operator=(const bson& right);
	this_type& operator=(const c_bson& right);

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		cur = right.cur;
		first = right.first;
		return *this;
	}

	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		cur = right.cur;
		first = right.first;
		return *this;
	}

	this_type& operator=(const this_type& right);

	bool empty(void) const;
	std::string key(void) const;
	u32 typeid_of_value(void) const;
	this_type& skip_auto_id(void);

	template<typename T>
	void get_value(T& val) const
	{
		this_type::load(this_type::key(), val);
	}

	inline void swap(BOOST_RV_REF(base_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		boost::swap(cur, right.cur);
		boost::swap(first, right.first);
#else
		base_type& right_ref = right;
		boost::swap(cur, right_ref.cur);
		boost::swap(first, right_ref.first);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		boost::swap(cur, right.cur);
		boost::swap(first, right.first);
#else
		this_type& right_ref = right;
		boost::swap(cur, right_ref.cur);
		boost::swap(first, right_ref.first);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}
	void swap(this_type& right);

	bool load_check(u32 type_id, const std::string& name) const;
	bool load_check(u32 type_id, const std::string& name, u32 sub_type_id) const;

	void load(const std::string& name, s8& val) const;
	void load(const std::string& name, char& val) const;
	void load(const std::string& name, wchar_t& val) const;
	void load(const std::string& name, u8& val) const;
	void load(const std::string& name, s16& val) const;
	void load(const std::string& name, u16& val) const;
	void load(const std::string& name, s32& val) const;
	void load(const std::string& name, u32& val) const;
	void load(const std::string& name, s64& val) const;
	void load(const std::string& name, u64& val) const;
	void load(const std::string& name, bool& val) const;
	void load(const std::string& name, f32& val) const;
	void load(const std::string& name, f64& val) const;
	void load(const std::string& name, std::string& val) const;
	void load(const std::string& name, c_bson& val) const;

	this_type sub_array_iterator(const std::string& name) const;

	template<typename Obj>
	void load(const std::string& name, Obj& obj) const
	{
		//const bson_iterator* pb = this;
		//obj.load(*this, name);
		bson_inside_type_access_proxy::load(*this, name, obj);
	}

	/*
	c_bson_date
	c_bson_time
	c_bson_oid
	c_bson_binary_buffer
	c_bson_null
	c_bson_undefined
	c_bson_regex
	these types provided by the member function load
	*/

protected:
	bool load_name(int type, const std::string& name) const;

	template<typename Obj>
	bool load_name(const std::string& name) const
	{
		return this_type::load_name(get_overlap_bson_value_typeid<
										typename boost::remove_const<Obj>::type
									>::value, name);
	}

	bool load_mark_start(this_type& tmp_iter, const bson_mark_splice&);
	bool load_mark_end(const this_type& tmp_iter, const bson_mark_splice&);

	template<typename Mark>
	bool load_mark_start(this_type& tmp_iter, const Mark&)
	{
		return true;
	}

	template<typename Mark>
	bool load_mark_end(const this_type& tmp_iter, const Mark&)
	{
		return true;
	}

	template<typename Obj>
	bool load_begin(this_type& tmp_iter, const std::string& name)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;
		return this_type::load_name<Obj>(name)
					&& this_type::load_mark_start(tmp_iter, mark_type());
	}

	template<typename Obj>
	bool load_end(const this_type& tmp_iter)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;
		return load_mark_end(tmp_iter, mark_type());
	}

	void load_value(s8& val) const;
	void load_value(char& val) const;
	void load_value(wchar_t& val) const;
	void load_value(u8& val) const;
	void load_value(s16& val) const;
	void load_value(u16& val) const;
	void load_value(s32& val) const;
	void load_value(u32& val) const;
	void load_value(s64& val) const;
	void load_value(u64& val) const;
	void load_value(bool& val) const;
	void load_value(f32& val) const;
	void load_value(f64& val) const;
	void load_value(std::string& val) const;
	void load_value(c_bson& val) const;

	template<typename Obj>
	void load_value(Obj& obj) const
	{
		bson_inside_type_access_proxy::load_value(*this, obj);
	}



private:
	void init(const bson& right);
};

} // namespace nsql_database_system
} // namespace yggr

// nonmember foo

bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l, yggr::u8 r);
bool operator==(yggr::u8 l, const yggr::nsql_database_system::c_bson_const_iterator& r);

bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l, const bson_iterator& r);
bool operator==(const bson_iterator& l, const yggr::nsql_database_system::c_bson_const_iterator& r);

bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l,
					const yggr::nsql_database_system::c_bson_const_iterator& r);

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator==(const yggr::nsql_database_system::c_bson_const_iterator& l,
				const Basic_String<Char, Traits, Alloc>& r)
{
	const bson_iterator& base = l;
	return r == bson_iterator_key(&base);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator==(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	const bson_iterator& base = r;
	return l == bson_iterator_key(&base);
}


bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l, yggr::u8 r);
bool operator!=(yggr::u8 l, const yggr::nsql_database_system::c_bson_const_iterator& r);

bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l, const bson_iterator& r);
bool operator!=(const bson_iterator& l, const yggr::nsql_database_system::c_bson_const_iterator& r);

bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l,
				const yggr::nsql_database_system::c_bson_const_iterator& r);

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator!=(const yggr::nsql_database_system::c_bson_const_iterator& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return !operator==<Char, Traits, Alloc, Basic_String>(l, r);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator!=(const Basic_String<Char, Traits, Alloc>& l,
				const yggr::nsql_database_system::c_bson_const_iterator& r)
{
	return !operator==<Char, Traits, Alloc, Basic_String>(l, r);
}

#define YGGR_PP_C_BSON_CONST_ITERATOR_DECLEAR_SWAP() \
	void swap(bson_iterator& l, yggr::nsql_database_system::c_bson_const_iterator& r); \
	void swap(yggr::nsql_database_system::c_bson_const_iterator& l, bson_iterator& r); \
	void swap(yggr::nsql_database_system::c_bson_const_iterator& l, yggr::nsql_database_system::c_bson_const_iterator& r); \
	\
	inline void swap(bson_iterator& l, BOOST_RV_REF(yggr::nsql_database_system::c_bson_const_iterator) r) { \
		boost::swap(l.cur, r.cur); \
		boost::swap(l.first, r.first); } \
	\
	inline void swap(BOOST_RV_REF(bson_iterator) l, yggr::nsql_database_system::c_bson_const_iterator& r) { \
		boost::swap(r.cur, l.cur); \
		boost::swap(r.first, l.first); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson_const_iterator) l, bson_iterator& r) { \
		boost::swap(r.cur, l.cur); \
		boost::swap(r.first, l.first); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson_const_iterator& l, BOOST_RV_REF(bson_iterator) r) { \
		boost::swap(l.cur, r.cur); \
		boost::swap(l.first, r.first); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson_const_iterator) l, yggr::nsql_database_system::c_bson_const_iterator& r) { \
		typedef yggr::nsql_database_system::c_bson_const_iterator now_this_type; \
		r.swap(l); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson_const_iterator& l, BOOST_RV_REF(yggr::nsql_database_system::c_bson_const_iterator) r) { \
		typedef yggr::nsql_database_system::c_bson_const_iterator now_this_type; \
		l.swap(r); }

namespace std
{
	YGGR_PP_C_BSON_CONST_ITERATOR_DECLEAR_SWAP();
} // namespace std

namespace boost
{
	YGGR_PP_C_BSON_CONST_ITERATOR_DECLEAR_SWAP();
} // namespace boost

#undef YGGR_PP_C_BSON_CONST_ITERATOR_DECLEAR_SWAP

// c_bson
namespace yggr
{
namespace nsql_database_system
{

class c_bson : public bson
{
private:
	friend class c_bson_access_proxy;
public:
	typedef bson base_type;
	typedef c_bson_const_iterator const_iterator;

private:
	typedef c_bson this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

//		/*
//		typedef struct {
//    char *data;    /**< Pointer to a block of data in this BSON object. */
//    char *cur;     /**< Pointer to the current position. */
//    int dataSize;  /**< The number of bytes allocated to char *data. */
//    bson_bool_t finished; /**< When finished, the BSON object can no longer be modified. */
//    int stack[32];        /**< A stack used to keep track of nested BSON elements. */
//    int stackPos;         /**< Index of current stack position. */
//    int err; /**< Bitfield representing errors or warnings on this buffer */
//    char *errstr; /**< A string representation of the most recent error or warning. */
//} bson;
//		*/
public:
	c_bson(void);
	c_bson(bool empty);

	explicit c_bson(BOOST_RV_REF(base_type) base) // rval_ref not address
	{
		const base_type& right_cref = base;
		if(bson_copy(this, &right_cref))
		{
			bson_init(this);
		}
	}
	explicit c_bson(const base_type& base);

	explicit c_bson(BOOST_RV_REF(this_type) right)
	{
		const base_type& base = right;
		if(bson_copy(this, &base))
		{
			bson_init(this);
		}
	}

	c_bson(const this_type& right);
	~c_bson(void);


	inline operator base_type*(void)
	{
		return this;
	}

	inline operator const base_type*(void) const
	{
		return this;
	}

	inline operator base_type&(void)
	{
		return *this;
	}

	inline operator const base_type&(void) const
	{
		return *this;
	}

	const_iterator begin(void) const;
	const_iterator end(void) const;
	void init(void);
	void clear(void);

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		const base_type& base = right;
		bson_destroy(this);
		if(bson_copy(this, &base))
		{
			bson_init(this);
		}
		return *this;
	}

	this_type& operator=(const base_type& base);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const base_type& base = right;
		bson_destroy(this);
		if(bson_copy(this, &base))
		{
			bson_init(this);
		}
		return *this;
	}

	this_type& operator=(const this_type& right);

	const char* data(void) const;
	size_type size(void) const;
	bool empty(void) const;

	s32 finish(void);
	bool is_finished(void) const;
	bool is_operator_enable(void) const;

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		bson& right_ref = right;
		bson tmp = {0};
		memcpy(&tmp, &right_ref, sizeof(bson));
		memcpy(&right_ref, this, sizeof(bson));
		memcpy(this, &tmp, sizeof(bson));
	}

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		c_bson& r = right;
		bson tmp = {0};
		memcpy(&tmp, &r, sizeof(bson));
		memcpy(&r, this, sizeof(bson));
		memcpy(this, &tmp, sizeof(bson));
	}

	void swap(this_type& right);

	bool save(const std::string& name, s8 val);
	bool save(const std::string& name, char val);
	bool save(const std::string& name, wchar_t val);
	bool save(const std::string& name, u8 val);
	bool save(const std::string& name, s16 val);
	bool save(const std::string& name, u16 val);
	bool save(const std::string& name, s32 val);
	bool save(const std::string& name, u32 val);
	bool save(const std::string& name, s64 val);
	bool save(const std::string& name, u64 val);
	bool save(const std::string& name, bool val);
	bool save(const std::string& name, f32 val);
	bool save(const std::string& name, f64 val);
	bool save(const std::string& name, const std::string& val);
	bool save(const std::string& name, const this_type& val);

	c_bson& save_array_start(const std::string& name);
	c_bson& save_array_end(void);

	c_bson& save_object_start(const std::string& name);
	c_bson& save_object_end(void);

	template<typename Obj>
	bool save(const std::string& name, const Obj& obj)
	{
		//base_type* pb = this;
		//return obj.save(*this, name);
		return bson_inside_type_access_proxy::save(*this, name, obj);
	}

	/*
	c_bson_date
	c_bson_time
	c_bson_oid
	c_bson_binary_buffer
	c_bson_null
	c_bson_undefined
	c_bson_regex
	these types provided by the member function save
	*/

protected:
	bool save_name(int type, const std::string& name);

	bool save_mark_start(const bson_mark_splice&);
	bool save_mark_end(const bson_mark_splice&);

	//bool save_mark_start(const bson_mark_object&);
	//bool save_mark_end(const bson_mark_object&);

	template<typename Mark>
	bool save_mark_start(const Mark&)
	{
		return true;
	}

	template<typename Mark>
	bool save_mark_end(const Mark&)
	{
		return true;
	}

	template<typename Obj>
	bool save_name(const std::string& name)
	{
		return this_type::save_name(get_overlap_bson_value_typeid<
										typename boost::remove_const<Obj>::type
										>::value, name);
	}

	template<typename Obj>
	bool save_begin(const std::string& name)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;
		return this_type::save_name<Obj>(name)
				&& this_type::save_mark_start(mark_type());
	}

	template<typename Obj>
	bool save_end(void)
	{
		typedef typename bson_value_mark<typename boost::remove_const<Obj>::type>::type mark_type;
		return this_type::save_mark_end(mark_type());
	}

	bool save_value(s8 val);
	bool save_value(char val);
	bool save_value(wchar_t val);
	bool save_value(u8 val);
	bool save_value(s16 val);
	bool save_value(u16 val);
	bool save_value(s32 val);
	bool save_value(u32 val);
	bool save_value(s64 val);
	bool save_value(u64 val);
	bool save_value(bool val);
	bool save_value(f32 val);
	bool save_value(f64 val);
	bool save_value(const std::string& val);
	bool save_value(const c_bson& val);

	template<typename Obj>
	bool save_value(const Obj& obj)
	{
		//base_type* pb = this;
		//obj.save_value(*this);
		return bson_inside_type_access_proxy::save_value(*this, obj);
	}
};

} // namespace nsql_database_system
} // namespace yggr

// nonmember foo
//io
std::ostream& operator<<(std::ostream& os, const yggr::nsql_database_system::c_bson& cb);

// ==
bool operator==(const bson& l, const yggr::nsql_database_system::c_bson& r);
bool operator==(const yggr::nsql_database_system::c_bson& l, const bson& r);
bool operator==(const yggr::nsql_database_system::c_bson& l, const yggr::nsql_database_system::c_bson& r);

bool operator!=(const bson& l, const yggr::nsql_database_system::c_bson& r);
bool operator!=(const yggr::nsql_database_system::c_bson& l, const bson& r);
bool operator!=(const yggr::nsql_database_system::c_bson& l, const yggr::nsql_database_system::c_bson& r);


#define YGGR_PP_C_BSON_DECLEAR_SWAP() \
	inline void swap(bson& l, BOOST_RV_REF(yggr::nsql_database_system::c_bson) r) { \
		typedef yggr::nsql_database_system::c_bson now_this_type; \
		now_this_type& r_ref = r; \
		bson tmp = {0}; \
		memcpy(&tmp, &r_ref, sizeof(bson)); \
		memcpy(&r_ref, &l, sizeof(bson)); \
		memcpy(&l, &tmp, sizeof(bson)); } \
	\
	inline void swap(BOOST_RV_REF(bson) l, yggr::nsql_database_system::c_bson& r) { \
		typedef bson now_base_type; \
		r.swap(l); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson) l, bson& r) { \
		typedef yggr::nsql_database_system::c_bson now_this_type; \
		now_this_type& l_ref = l; \
		bson tmp = {0}; \
		memcpy(&tmp, &l_ref, sizeof(bson)); \
		memcpy(&l_ref, &r, sizeof(bson)); \
		memcpy(&r, &tmp, sizeof(bson)); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson& l, BOOST_RV_REF(bson) r) { \
		typedef bson now_base_type; \
		l.swap(r); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson) l, yggr::nsql_database_system::c_bson& r) { \
		typedef yggr::nsql_database_system::c_bson now_this_type; \
		r.swap(l); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson& l, BOOST_RV_REF(yggr::nsql_database_system::c_bson) r) { \
		typedef yggr::nsql_database_system::c_bson now_this_type; \
		l.swap(r); } \
	\
	void swap(bson& l, yggr::nsql_database_system::c_bson& r); \
	void swap(yggr::nsql_database_system::c_bson& l, bson& r); \
	void swap(yggr::nsql_database_system::c_bson& l, yggr::nsql_database_system::c_bson& r);

namespace std
{
	YGGR_PP_C_BSON_DECLEAR_SWAP();
} // namespace std

namespace boost
{
	YGGR_PP_C_BSON_DECLEAR_SWAP();
} // namespace boost


#undef YGGR_PP_C_BSON_DECLEAR_SWAP

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_BSON_HPP__
