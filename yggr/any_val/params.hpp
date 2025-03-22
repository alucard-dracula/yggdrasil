//params.hpp

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

#ifndef __YGGR_ANY_VAL_PARAMS_HPP__
#define __YGGR_ANY_VAL_PARAMS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/exception/exception_sort.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/container/map.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/any_val/any_operator_mgr.hpp>
#include <yggr/any_val/any_set.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/any.hpp>
#include <yggr/serialization/map.hpp>

#include <boost/ref.hpp>
#include <boost/preprocessor/cat.hpp>

#include <utility>
#include <cassert>

#define __PARAMS_THIS_TYPE_RET_OP2_CONST(__op_foo__) \
	this_type __op_foo__ (const this_type& right) const { \
		assert(_pmgr_op); \
		if(!_pmgr_op) { \
			throw error_maker_type::make_error(error_maker_type::E_non_operator); } \
		this_type rst; \
		if(_base.size() <= right._base.size()) { \
			base_c_iter_type re = right._base.end(); \
			base_c_iter_type ri; \
			for(base_c_iter_type i = _base.begin(), isize = _base.end(); i != isize; ++i) { \
				if((ri = right._base.find(i->first)) != re) { \
					rst[i->first] =_pmgr_op-> BOOST_PP_CAT(call_, __op_foo__) (i->second, ri->second); } } } \
		else { \
			base_c_iter_type le = _base.end(); \
			base_c_iter_type li; \
			for(base_c_iter_type i = right._base.begin(), isize = right._base.end(); i != isize; ++i) { \
				if((li = _base.find(i->first)) != le) { \
					rst[i->first] =_pmgr_op-> BOOST_PP_CAT(call_, __op_foo__) (li->second, i->second); } } } \
		return rst; }

#define __PARAMS_THIS_TYPE_REF_RET_OP2(__op_foo__) \
	this_type& __op_foo__ (const this_type& right) { \
		assert(_pmgr_op); \
		if(!_pmgr_op) { \
			throw error_maker_type::make_error(error_maker_type::E_non_operator); } \
		if(_base.size() <= right._base.size()) { \
			base_c_iter_type re = right._base.end(); \
			base_c_iter_type ri; \
			for(base_iter_type i = _base.begin(), isize = _base.end(); i != isize; ++i) { \
				if((ri = right._base.find(i->first)) != re) { \
					_pmgr_op-> BOOST_PP_CAT(call_, __op_foo__) (i->second, ri->second); } } } \
		else { \
			base_iter_type le = _base.end(); \
			base_iter_type li; \
			for(base_c_iter_type i = right._base.begin(), isize = right._base.end(); i != isize; ++i) { \
				if((li = _base.find(i->first)) != le) { \
					_pmgr_op-> BOOST_PP_CAT(call_, __op_foo__) (li->second, i->second); } } } \
		return *this; }

namespace yggr
{
namespace any_val
{

// don't delete this comment
//template<typename Key, 
//			typename Op_Mgr = any_operator_mgr<>,
//			typename Map = container::map<Key, typename Op_Mgr::any_type> >
//class params; // !!!! msvc may cause C1001 error !!!!

template<typename Key, 
			typename Op_Mgr = any_operator_mgr<>,
			typename Cmp = typename container::map<Key, typename Op_Mgr::any_type>::key_compare,
			typename Alloc = typename container::map<Key, typename Op_Mgr::any_type>::allocator_type,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map = container::map>
class params
{
public:
	ERROR_MAKER_BEGIN("parmas")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_22)
			ERROR_CODE_DEF(E_empty_param)
			ERROR_CODE_DEF(E_param_not_existed)
			ERROR_CODE_DEF(E_non_operator)
			ERROR_CODE_DEF(E_any_not_support_serialize)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_start_22)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_empty_param, "empty param")
			ERROR_CODE_MSG(E_param_not_existed, "param not existed")
			ERROR_CODE_MSG(E_non_operator, "operator is nil")
			ERROR_CODE_MSG(E_any_not_support_serialize, "now any not_support_serialize")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

public:
	typedef Map<Key, typename Op_Mgr::any_type, Cmp, Alloc> base_type;

	typedef Op_Mgr op_mgr_type;
	typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type value_type;
	typedef typename base_type::key_compare key_compare;
	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::size_type size_type;

public:
	typedef yggr::ptr_single<op_mgr_type> op_mgr_single_type;
	typedef typename op_mgr_single_type::obj_ptr_type op_mgr_ptr_type;

private:
	typedef typename base_type::value_type pair_type;
	typedef typename base_type::iterator base_iter_type;
	typedef typename base_type::const_iterator base_c_iter_type;

private:
	typedef params this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	params(void)
		: _pmgr_op(op_mgr_single_type::get_ins())
	{
	}

	params(const op_mgr_ptr_type& pmgr_op)
		: _pmgr_op(pmgr_op)
	{
	}

	params(BOOST_RV_REF(this_type) right)
		: _pmgr_op(boost::move(right._pmgr_op)),
			_base(boost::move(right._base))
			
	{
	}

	params(const this_type& right)
		: _pmgr_op(right._pmgr_op),
			_base(right._base) 
	{
	}

	~params(void)
	{
	}

public:
	inline this_type& copy_from(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_pmgr_op, boost::move(right_ref._pmgr_op));
		copy_or_move_or_swap(_base, boost::move(right_ref._base));
		
		return *this;
	}

	this_type& copy_from(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_pmgr_op = right._pmgr_op;
		_base = right._base;
		
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{ 
		return 
			_pmgr_op? 
				this_type::set(boost::move(right)) 
				: this_type::copy_from(right);
	}

	this_type& operator=(const this_type& right)
	{ 
		return _pmgr_op? this_type::set(right) : this_type::copy_from(right);
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(_pmgr_op, right._pmgr_op);
		::yggr::swap(_base, right._base);
		
	}

public:
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(const Handler& handler)
	{
		return handler(_pmgr_op, _base);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(const Handler& handler) const
	{
		return handler(_pmgr_op, _base);
	}

public:
	inline void clear(void)
	{
		_base.clear();
	}

	inline void clear_op(void)
	{
		op_mgr_ptr_type tmp;
		::yggr::swap(_pmgr_op, tmp);
	}

	inline size_type erase(const key_type& key)
	{
		return _base.erase(key);
	}

	inline bool empty(void) const
	{
		return _base.empty();
	}

	inline bool empty(const key_type& key) const
	{
		base_c_iter_type iter = _base.find(key);
		return iter == _base.end() || iter->second.empty();
	}

	inline bool empty_op(void) const
	{
		return !_pmgr_op;
	}

	inline this_type& copy_datas_from(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_base, boost::move(right_ref._base));
		return *this;
	}

	inline this_type& copy_datas_from(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_base = right._base;

		return *this;
	}

	this_type& merge_datas_from(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_c_iter_type re = right_ref._base.end();

		base_iter_type i;
		base_iter_type e = _base.end();

		for(base_c_iter_type ri = right_ref._base.begin(); ri != re; ++ri)
		{
			if((i = _base.find(ri->first)) == e)
			{
				yggr::copy_or_move_or_swap(_base[ri->first], boost::move(ri->second));
			}
		}

		return *this;
	}

	this_type& merge_datas_from(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_c_iter_type re = right._base.end();

		base_iter_type i;
		base_iter_type e = _base.end();

		for(base_c_iter_type ri = right._base.begin(); ri != re; ++ri)
		{
			if((i = _base.find(ri->first)) == e)
			{
				_base[ri->first] = (ri->second);
			}
		}

		return *this;
	}

	inline void swap_datas(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(_base, right._base);
	}

	inline bool swap_data(const key_type& key, value_type& val)
	{
		base_iter_type iter;

		return 
			((iter = _base.find(key)) != _base.end())
			&& (iter->second.swap(val), true);
				
	}

	inline const std::type_info& type(const key_type& key) const
	{
		base_c_iter_type iter;
		return 
			(iter = _base.find(key)) == _base.end()?
				typeid(void) 
				: iter->second.type();
	}

	inline const char* type_name(const key_type& key) const
	{
		return this_type::type(key).name();	
	}

	inline op_mgr_ptr_type& op_mgr(void)
	{
		return _pmgr_op;
	}

	inline const op_mgr_ptr_type& op_mgr(void) const
	{
		return _pmgr_op;
	}

	template<typename CastHandler> inline
	typename func::foo_t_info<CastHandler>::result_type
		get(const key_type& key, const CastHandler& handler) const
	{
		base_c_iter_type iter = _base.find(key);
		if(iter == _base.end())
		{
			return handler(static_cast<const value_type*>(0));
		}

		const value_type& val = iter->second;
		return handler(boost::addressof(val));
	}

	template<typename CastHandler> inline
	typename func::foo_t_info<CastHandler>::result_type
		get(const key_type& key, const CastHandler& handler)
	{
		base_iter_type iter = _base.find(key);
		if(iter == _base.end())
		{
			return handler(static_cast<value_type*>(0));
		}

		value_type& val = iter->second;
		return handler(boost::addressof(val));
	}

	template<typename T> inline
	const T* safe_get_val_ptr(const key_type& key) const
	{
		typedef const T* ret_type;

		base_c_iter_type iter = _base.find(key);
		if(iter == _base.end())
		{
			return static_cast<ret_type>(0);
		}

		const value_type& val = iter->second;
		return ::yggr::any_cast<const T>(boost::addressof(val));
	}

	template<typename T> inline
	T* safe_get_val_ptr(const key_type& key)
	{
		typedef T* ret_type;

		base_iter_type iter = _base.find(key);
		if(iter == _base.end())
		{
			return static_cast<ret_type>(0);
		}

		value_type& val = iter->second;
		return ::yggr::any_cast<T>(boost::addressof(val));
	}

	template<typename T> inline
	const T* unsafe_get_val_ptr(const key_type& key) const
	{
		typedef const T* ret_type;

		base_c_iter_type iter = _base.find(key);
		if(iter == _base.end())
		{
			return static_cast<ret_type>(0);
		}

		const value_type& val = iter->second;
		return ::yggr::unsafe_any_cast<const T>(boost::addressof(val));
	}

	template<typename T> inline
	T* unsafe_get_val_ptr(const key_type& key)
	{
		typedef T* ret_type;

		base_iter_type iter = _base.find(key);
		if(iter == _base.end())
		{
			return static_cast<ret_type>(0);
		}

		value_type& val = iter->second;
		return ::yggr::unsafe_any_cast<T>(boost::addressof(val));
	}

#ifndef YGGR_PARAMS_USING_SAFE_CAST
	template<typename T> inline
	const T* get_val_ptr(const key_type& key) const
	{
		return this_type::unsafe_get_val_ptr<T>(key);
	}

	template<typename T> inline
	T* get_val_ptr(const key_type& key)
	{
		return this_type::unsafe_get_val_ptr<T>(key);
	}

#else

	template<typename T> inline
	const T* get_val_ptr(const key_type& key) const
	{
		return this_type::safe_get_val_ptr<T>(key);
	}

	template<typename T> inline
	T* get_val_ptr(const key_type& key)
	{
		return this_type::safe_get_val_ptr<T>(key);
	}

#endif // YGGR_PARAMS_USING_SAFE_CAST

	template<typename T> inline
	const T& safe_get_value(const key_type& key) const
	{
		const T* p = this_type::safe_get_val_ptr<T>(key);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	template<typename T> inline
	T& safe_get_value(const key_type& key)
	{
		T *p = this_type::safe_get_val_ptr<T>(key);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	template<typename T> inline
	const T& unsafe_get_value(const key_type& key) const
	{
		const T* p = this_type::unsafe_get_val_ptr<T>(key);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	template<typename T> inline
	T& unsafe_get_value(const key_type& key)
	{
		T *p = this_type::unsafe_get_val_ptr<T>(key);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	template<typename T> inline 
	const T& get_value(const key_type& key) const
	{
		const T* p = this_type::get_val_ptr<T>(key);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	template<typename T> inline
	T& get_value(const key_type& key)
	{
		T *p = this_type::get_val_ptr<T>(key);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	inline value_type& operator[](const key_type& key)
	{
		return _base[key];
	}

	inline const value_type& operator[](const key_type& key) const
	{
		base_c_iter_type iter;

		return 
			((iter = _base.find(key)) == _base.end())?
				this_type::prv_s_failed_value()
				: iter->second;

	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		val_export(const Handler& handler) const
	{
		return handler(_pmgr_op, _base);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		val_import(const Handler& handler)
	{
		return handler(_pmgr_op, _base);
	}

	inline size_type size(void) const
	{
		return _base.size();
	}

	inline allocator_type get_allocator(void) const
	{
		return _base.get_allocator();
	}

public:
	this_type positive(void) const
	{
		assert(_pmgr_op);
		if(!_pmgr_op) 
		{ 
			throw error_maker_type::make_error(error_maker_type::E_non_operator);  
		} 

		this_type tmp;
		tmp._base = _base;
		tmp._pmgr_op = _pmgr_op;

		for(base_iter_type i = tmp._base.begin(), isize = tmp._base.end(); i != isize; ++i)
		{
			i->second = _pmgr_op->call_positive(i->second);
		}

		return tmp;
	}


	this_type negitive(void) const
	{
		assert(_pmgr_op);
		if(!_pmgr_op) 
		{ 
			throw error_maker_type::make_error(error_maker_type::E_non_operator);  
		} 

		this_type tmp;
		tmp._base = _base;
		tmp._pmgr_op = _pmgr_op;

		for(base_iter_type i = tmp._base.begin(), isize = tmp._base.end(); i != isize; ++i)
		{
			i->second = _pmgr_op->call_negate(i->second);
		}

		return tmp;
	}

	__PARAMS_THIS_TYPE_REF_RET_OP2(set);

	__PARAMS_THIS_TYPE_RET_OP2_CONST(plus)
	__PARAMS_THIS_TYPE_REF_RET_OP2(plus_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(minus);
	__PARAMS_THIS_TYPE_REF_RET_OP2(minus_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(multiplies)
	__PARAMS_THIS_TYPE_REF_RET_OP2(multiplies_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(divides)
	__PARAMS_THIS_TYPE_REF_RET_OP2(divides_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(modulus)
	__PARAMS_THIS_TYPE_REF_RET_OP2(modulus_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(bitwise_and)
	__PARAMS_THIS_TYPE_REF_RET_OP2(bitwise_and_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(bitwise_or)
	__PARAMS_THIS_TYPE_REF_RET_OP2(bitwise_or_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(bitwise_xor)
	__PARAMS_THIS_TYPE_REF_RET_OP2(bitwise_xor_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(bitwise_left_shift)
	__PARAMS_THIS_TYPE_REF_RET_OP2(bitwise_left_shift_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(bitwise_right_shift)
	__PARAMS_THIS_TYPE_REF_RET_OP2(bitwise_right_shift_set)

public:
	bool equal_to(const this_type& right) const
	{
		if(!_pmgr_op)
		{
			throw error_maker_type::make_error(error_maker_type::E_non_operator);
		}

		if(this == boost::addressof(right))
		{
			return true;
		}

		if(_base.size() != right._base.size())
		{
			return false;
		}

		base_c_iter_type ri;
		base_c_iter_type re = right._base.end();

		for(base_c_iter_type i = _base.begin(), isize = _base.end(); i != isize; ++i)
		{
			if((ri = right._base.find(i->first)) == re)
			{
				return false;
			}

			if(i->second.empty() != ri->second.empty())
			{
				return false;
			}
			else
			{
				if(i->second.empty())
				{
					continue;
				}
			}

			if(!(yggr::any_cast<bool>(_pmgr_op->call_equal_to(i->second, ri->second))))
			{
				return false;
			}
		}

		return true;
	}

private:
	inline static const value_type& prv_s_failed_value(void)
	{
		static const value_type failed_val;
		return failed_val;
	}

private:

	template<typename Any_T, typename Nil_T = void>
	struct serialize_helper
	{
		template<typename Archive> inline
		void operator()(base_type& t, Archive& ar, const u32 version) const
		{
			ar & YGGR_SERIALIZATION_NAME_NVP("_base", t);
		}
	};

	template<typename Nil_T> 
	struct serialize_helper< ::yggr::any, Nil_T>
	{
		template<typename Archive> inline
		void operator()(base_type& t, Archive& ar, const u32 version) const
		{
			ar & YGGR_SERIALIZATION_NAME_NVP("_base", t);
		}
	};

	template<typename Nil_T>
	struct serialize_helper< ::boost::any, Nil_T>
	{
		template<typename Archive> inline
		void operator()(base_type& t, Archive& ar, const u32 version) const
		{
			throw error_maker_type::make_error(error_maker_type::E_any_not_support_serialize);
		}
	};

	friend class yggr::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const u32 version) 
	{
		typedef serialize_helper<value_type> helper_type;
		helper_type helper;
		helper(_base, ar, version);
	}

private:
	mutable op_mgr_ptr_type _pmgr_op;
	base_type _base;
};

// non-member foo

//positive
template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline
params<Key, Op_Mgr, Cmp, Alloc, Map>
	operator+(const params<Key, Op_Mgr, Cmp, Alloc, Map>& val) 
{ 
	return val.positive(); 
}

//negitive
template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline
params<Key, Op_Mgr, Cmp, Alloc, Map> 
	operator-(const params<Key, Op_Mgr, Cmp, Alloc, Map>& val) 
{ 
	return val.negitive(); 
}


#define __PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(__op__, __op_foo__) \
	template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc, \
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline \
	params<Key, Op_Mgr, Cmp, Alloc, Map> \
		operator __op__(const params<Key, Op_Mgr, Cmp, Alloc, Map>& l, \
						const params<Key, Op_Mgr, Cmp, Alloc, Map>& r) { \
		return l.__op_foo__(r); }

#define __PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(__op__, __op_foo__) \
	template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc, \
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline \
	params<Key, Op_Mgr, Cmp, Alloc, Map>& \
		operator __op__(params<Key, Op_Mgr, Cmp, Alloc, Map>& l, \
						const params<Key, Op_Mgr, Cmp, Alloc, Map>& r) { \
		return l.__op_foo__(r); }

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(+, plus)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(+=, plus_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(-, minus);
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(-=, minus_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(*, multiplies)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(*=, multiplies_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(/, divides)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(/=, divides_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(%, modulus)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(%=, modulus_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(&, bitwise_and)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(&=, bitwise_and_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(|, bitwise_or)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(|=, bitwise_or_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(^, bitwise_xor)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(^=, bitwise_xor_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(<<, bitwise_left_shift)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(<<=, bitwise_left_shift_set)

	__PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST(>>, bitwise_right_shift)
	__PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2(>>=, bitwise_right_shift_set)

//equal_to
template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline
bool operator==(const params<Key, Op_Mgr, Cmp, Alloc, Map>& l,
				const params<Key, Op_Mgr, Cmp, Alloc, Map>& r) 
{
	return l.equal_to(r);
}

template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline
bool operator!=(const params<Key, Op_Mgr, Cmp, Alloc, Map>& l,
				const params<Key, Op_Mgr, Cmp, Alloc, Map>& r) 
{
	return !l.equal_to(r);
}

} // namespace any_val
} // namespace yggr

#undef __PARAMS_THIS_TYPE_RET_NON_MEMBER_OP2_CONST
#undef __PARAMS_THIS_TYPE_REF_RET_NON_MEMBER_OP2

#undef __PARAMS_THIS_TYPE_RET_OP2_CONST
#undef __PARAMS_THIS_TYPE_REF_RET_OP2

namespace yggr
{
namespace any_val
{
namespace swap_support
{

template<typename Key, typename Op_Mgr, typename Cmp, typename Alloc,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map> inline
void swap(params<Key, Op_Mgr, Cmp, Alloc, Map>& l, params<Key, Op_Mgr, Cmp, Alloc, Map>& r) 
{
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace any_val
} // namespace yggr

namespace std
{
	using ::yggr::any_val::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::any_val::swap_support::swap;
} // namespace boost

#endif //__YGGR_ANY_VAL_PARAMS_HPP__
