//params.hpp

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

#ifndef __YGGR_ANY_VAL_PARAMS_HPP__
#define __YGGR_ANY_VAL_PARAMS_HPP__

#include <utility>
#include <map>
#include <cassert>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <yggr/move/move.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/error_make.hpp>


#define __PARAMS_THIS_TYPE_RET_OP2_CONST(__op__, __op_foo__) \
	const this_type operator __op__ (const this_type& right) const { \
		this_type rst; \
		base_c_iter_type isize = base_type::end(); \
		base_c_iter_type re = right.end(); \
		base_c_iter_type ri; \
		for(base_c_iter_type i = base_type::begin(); i != isize; ++i) { \
			if((ri = right.find(i->first)) != re) { \
				rst[i->first] = op_type:: __op_foo__ (i->second, ri->second); } } \
				return rst; }

#define __PARAMS_THIS_TYPE_REF_RET_OP2(__op__, __op_foo__) \
	this_type& operator __op__ (const this_type& right) { \
		base_iter_type isize = base_type::end(); \
		base_c_iter_type re = right.end(); \
		base_c_iter_type ri; \
		for(base_iter_type i = base_type::begin(); i != isize; ++i) { \
			if((ri = right.find(i->first)) != re) { \
				op_type:: __op_foo__ (i->second, ri->second); } } \
                return *this; }

namespace yggr
{
namespace any_val
{

template<class Key, typename Op,
			class Cmp = std::less<Key>,
			class Alloc = std::allocator<std::pair<const Key, boost::any> >,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map = std::map
		>
class params
	: private Map<Key, boost::any, Cmp, Alloc>
{
public:
	ERROR_MAKER_BEGIN("parmas")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_22)
			ERROR_CODE_DEF(E_empty_param)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_start_22)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_empty_param, "empty param")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()
public:
	typedef Key key_type;
	typedef boost::any val_type;
	typedef Cmp cmp_type;
	typedef Alloc alloc_type;

	typedef Op op_type;

	typedef Map<key_type, val_type, cmp_type, alloc_type> base_type;

private:
	typedef typename base_type::value_type pair_type;
	typedef typename base_type::iterator base_iter_type;
	typedef typename base_type::const_iterator base_c_iter_type;

private:
	typedef params this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	params(void)
	{
	}

	params(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		base_type::swap(right_ref);
	}

	params(const this_type& right)
		: base_type(right)
	{
	}

	~params(void)
	{
		base_type::clear();
	}

	//this_type& operator=(const this_type& right) // use copy_from
	//{
	//	const base_type& base = *this;
	//	base = right;

	//	return *this;
	//}

	template<typename Handler>
	const this_type& operator()(const Handler& handler)
	{
		return handler(boost::ref(*this));
	}

	template<typename Handler>
	const this_type operator()(const Handler& handler) const
	{
		return handler(boost::cref(*this));
	}

	void clear(void)
	{
		base_type::clear();
	}

	size_type erase(const key_type& key)
	{
		return base_type::erase(key);
	}

	bool empty(void) const
	{
		return base_type::empty();
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type& lbase = *this;
		base_type& rbase = right;

		lbase.swap(rbase);
	}

	bool swap(const key_type& key, val_type& val)
	{
		base_iter_type iter = base_type::find(key);
		if(iter != base_type::end())
		{
			val.swap(*iter);
			return true;
		}

		return false;
	}

	this_type& copy_from(BOOST_RV_REF(this_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
		return *this;
	}

	this_type& copy_from(const this_type& right)
	{
		base_type& lbase = *this;
		const base_type& rbase = right;
		lbase = rbase;
		return *this;
	}

	this_type& merge_from(const this_type& right)
	{
		base_type& base = *this;
		base_c_iter_type re = right.end();

		base_iter_type i;
		base_iter_type e = base.end();

		for(base_c_iter_type ri = right.begin(); ri != re; ++ri)
		{
			if((i = base.find(ri->first)) == e)
			{
				base[ri->first] = (ri->second);
			}
		}

		return *this;
	}

	//template<typename T>
	//std::pair<T, bool> get_val(const key_type& key) const
	//{
	//	typedef T rst_val_type;
	//	typedef std::pair<rst_val_type, bool> rst_type;
	//	const base_type& base = *this;

	//	base_c_iter_type iter = base.find(key);

	//	if(iter == base.end())
	//	{
	//		return rst_type(T(), false);
	//	}

	//	const val_type& val = iter->second;
	//	const rst_val_type *p = boost::any_cast<rst_val_type>(&val);
	//	if(!p)
	//	{
	//		return rst_type(T(), false);
	//	}

	//	return rst_type(*p, true);
	//}

	const std::string type_name(const key_type& key) const
	{
		const base_type& base = *this;

		base_c_iter_type iter = base.find(key);
		if(iter == base.end())
		{
			return std::string();
		}
	
		return std::string(iter->second.type().name());
	}

	template<typename T>
	const T* get_val_ptr(const key_type& key) const
	{
		const base_type& base = *this;

		base_c_iter_type iter = base.find(key);
		if(iter == base.end())
		{
			return (const T*)0;
		}

		const val_type& val = iter->second;
		return boost::any_cast<const T>(&val);
	}

	template<typename T>
	T* get_val_ptr(const key_type& key)
	{
		base_type& base = *this;

		base_iter_type iter = base.find(key);
		if(iter == base.end())
		{
			return (T*)0;
		}

		val_type& val = iter->second;
		return boost::any_cast<T>(&val);
	}

	template<typename T>
	const T& get_val(const key_type& key) const
	{
		const T* p = get_val_ptr<T>(key);
		//assert(p);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	template<typename T>
	T& get_val(const key_type& key)
	{
		T *p = get_val_ptr<T>(key);
		//assert(p);
		if(!p)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_param);
		}

		return *p;
	}

	val_type& operator[](const key_type& key)
	{
		base_type& base = *this;
		return base[key];
	}

	const val_type& operator[](const key_type& key) const
	{
		const base_type& base = *this;
		return base[key];
	}

	template<typename Handler>
	void val_export(const Handler& handler) const
	{
		handler(*this);
	}

	template<typename Handler>
	void val_import(const Handler& handler)
	{
		handler(*this);
	}

	bool operator==(const this_type& right) const
	{
		const base_type& base = *this;

		if(base.size() != right.size())
		{
			return false;
		}

		base_c_iter_type isize = base.end();

		base_c_iter_type ri;
		base_c_iter_type re = right.end();

		for(base_c_iter_type i = base.begin(); i != isize; ++i)
		{
			if((ri = right.find(i->first)) == re)
			{
				return false;
			}

			if(!(op_type::equal_to(i->second, ri->second)))
			{
				return false;
			}
		}

		return true;
	}

	bool operator!=(const this_type& right) const
	{
		const base_type& base = *this;

		if(base.size() != right.size())
		{
			return true;
		}

		base_c_iter_type isize = base.end();

		base_c_iter_type ri;
		base_c_iter_type re = right.end();

		for(base_c_iter_type i = base.begin(); i != isize; ++i)
		{
			if((ri = right.find(i->first)) == re)
			{
				return true;
			}

			if(!(op_type::equal_to(i->second, ri->second)))
			{
				return true;
			}
		}

		return false;
	}

	//bool is_exists(const key_type& key) const
	//{
	//	return base_type::find(key) != base_type::end();
	//}

	__PARAMS_THIS_TYPE_REF_RET_OP2(=, set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(+, plus)
	__PARAMS_THIS_TYPE_REF_RET_OP2(+=, plus_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(-, minus);
	__PARAMS_THIS_TYPE_REF_RET_OP2(-=, minus_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(*, multiplies)
	__PARAMS_THIS_TYPE_REF_RET_OP2(*=, multiplies_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(/, divides)
	__PARAMS_THIS_TYPE_REF_RET_OP2(/=, divides_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(%, modulus)
	__PARAMS_THIS_TYPE_REF_RET_OP2(%=, modulus_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(&, bit_and)
	__PARAMS_THIS_TYPE_REF_RET_OP2(&=, bit_and_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(|, bit_or)
	__PARAMS_THIS_TYPE_REF_RET_OP2(|=, bit_or_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(^, bit_xor)
	__PARAMS_THIS_TYPE_REF_RET_OP2(^=, bit_xor_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(<<, bit_left_trans)
	__PARAMS_THIS_TYPE_REF_RET_OP2(<<=, bit_left_trans_set)

	__PARAMS_THIS_TYPE_RET_OP2_CONST(>>, bit_right_trans)
	__PARAMS_THIS_TYPE_REF_RET_OP2(>>=, bit_right_trans_set)

};

} // namespace any_val
} // namespace yggr

#endif //__YGGR_ANY_VAL_PARAMS_HPP__
