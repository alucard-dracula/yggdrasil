//any_operator_mgr.hpp

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

#ifndef __YGGR_ANY_VAL_ANY_CALCULATOR_MGR_HPP__
#define __YGGR_ANY_VAL_ANY_CALCULATOR_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/move/move.hpp>

#include <yggr/type_traits/traits.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/operator_tags.hpp>
#include <yggr/mplex/pointer_int.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/derived_deleter.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <yggr/tuple_ex/tuple.hpp>
#include <yggr/utility/typeinfo.hpp>

#include <yggr/any_val/any.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/ref.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/integral_c.hpp>

#include <cassert>
#include <typeinfo>


#define YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF( __foo_name__ ) \
	template<typename Param> inline \
	bool BOOST_PP_CAT( register_, __foo_name__ )(void) { \
		typedef Param param_type; \
		return this_type::BOOST_PP_CAT( register_, __foo_name__ )< param_type >( \
					boost::bind(&operator_wrap::template __foo_name__< param_type >, _1)); } \
	\
	template<typename Param, typename Handler> inline \
	bool BOOST_PP_CAT( register_, __foo_name__ )(const Handler& handler) { \
		typedef Param param_type; \
		return this_type::register_unary_operator< param_type >( \
				BOOST_PP_CAT( yggr::mplex::make_op_tag_, __foo_name__ )(), handler); } \
	\
	template<typename Param> inline \
	bool BOOST_PP_CAT( unregister_, __foo_name__ )(void) { \
		typedef Param param_type; \
		return this_type::unregister_unary_operator< param_type >( \
				BOOST_PP_CAT( yggr::mplex::make_op_tag_, __foo_name__ )()); }

#define YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF( __foo_name__ ) \
	template<typename L, typename R> inline \
	bool BOOST_PP_CAT( register_, __foo_name__ )(void) { \
		typedef L left_type; \
		typedef R right_type; \
		return this_type::BOOST_PP_CAT( register_, __foo_name__ )< left_type, right_type >( \
				boost::bind(&operator_wrap::template __foo_name__< left_type, right_type >, _1, _2)); } \
	\
	template<typename L, typename R, typename Handler> inline \
	bool BOOST_PP_CAT( register_, __foo_name__ )(const Handler& handler) { \
		typedef L left_type; \
		typedef R right_type; \
		return this_type::register_binary_operator< left_type, right_type >( \
					BOOST_PP_CAT( yggr::mplex::make_op_tag_, __foo_name__ )(), handler); } \
	\
	template<typename L, typename R> inline \
	bool BOOST_PP_CAT( unregister_, __foo_name__ )(void){ \
		typedef L left_type; \
		typedef R right_type; \
		return this_type::unregister_binary_operator<left_type, right_type>( \
					BOOST_PP_CAT( yggr::mplex::make_op_tag_, __foo_name__ )()); }


namespace yggr
{
namespace any_val
{
namespace detail
{

template<typename Any>
class basic_any_operator
{
public:
	typedef Any any_type;
private:
	typedef basic_any_operator this_type;

public:
	typedef any_type (*vfoo_unary_op_type)(const this_type*, const any_type&);
	typedef any_type (*vfoo_mutable_unary_op_type)(const this_type*, any_type&);
	typedef any_type& (*vfoo_ref_ret_mutable_unary_op_type)(const this_type*, any_type&);
	typedef any_type (*vfoo_binary_op_type)(const this_type*, const any_type&, const any_type&);
	typedef any_type& (*vfoo_mutable_binary_op_type)(const this_type*, any_type&, const any_type&);

public:
	typedef mplex::pointer_uint::type mark_value_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(mark_value_type) == sizeof(void*))>));

public:
	typedef boost::mpl::integral_c<mark_value_type, 0> tag_vfoo_unkonw_type;
	typedef boost::mpl::integral_c<mark_value_type, 1> tag_vfoo_unary_op_type;
	typedef boost::mpl::integral_c<mark_value_type, 2> tag_vfoo_mutable_unary_op_type;
	typedef boost::mpl::integral_c<mark_value_type, 3> tag_vfoo_ref_ret_mutable_unary_op_type;
	typedef boost::mpl::integral_c<mark_value_type, 4> tag_vfoo_binary_op_type;
	typedef boost::mpl::integral_c<mark_value_type, 5> tag_vfoo_mutable_binary_op_type;

public:
	basic_any_operator(void)
		: _foo_mark(tag_vfoo_unkonw_type::value),
			_foo_val(0)
	{
	}

	basic_any_operator(mark_value_type foo_mark, void* pfoo)
		: _foo_mark(foo_mark), _foo_val(pfoo)
	{
	}

	~basic_any_operator(void)
	{
	}

public:
	inline any_type unary_op(const any_type& param1) const
	{
		switch(_foo_mark)
		{
		case tag_vfoo_unary_op_type::value:
			assert(_pfoo_unary_op);
			return _pfoo_unary_op? (*_pfoo_unary_op)(this, param1) : any_type();
		default:
			assert(false);
			return any_type();
		}
	}

	inline any_type mutable_unary_op(any_type& param1) const
	{
		switch(_foo_mark)
		{
		case tag_vfoo_mutable_unary_op_type::value:
			assert(_pfoo_mutable_unary_op);
			return _pfoo_mutable_unary_op? (*_pfoo_mutable_unary_op)(this, param1) : any_type();
		default:
			assert(false);
			return any_type();
		}
	}

	inline any_type& ref_ret_mutable_unary_op(any_type& param1) const
	{
		switch(_foo_mark)
		{
		case tag_vfoo_ref_ret_mutable_unary_op_type::value:
			assert(_pfoo_ref_ret_mutable_unary_op);
			if(_pfoo_ref_ret_mutable_unary_op)
			{
				return (*_pfoo_ref_ret_mutable_unary_op)(this, param1);
			}
		default:
			assert(false);
			{
				any_type tmp;
				tmp.swap(param1);
			}
			return param1;
		}
	}

	inline any_type binary_op(const any_type& param1, const any_type& param2) const
	{
		switch(_foo_mark)
		{
		case tag_vfoo_binary_op_type::value:
			assert(_pfoo_binary_op);
			return _pfoo_binary_op? (*_pfoo_binary_op)(this, param1, param2) : any_type();
		default:
			assert(false);
			return any_type();
		}
	}

	inline any_type& mutable_binary_op(any_type& param1, const any_type& param2) const
	{
		switch(_foo_mark)
		{
		case tag_vfoo_mutable_binary_op_type::value:
			assert(_pfoo_mutable_binary_op);
			if(_pfoo_mutable_binary_op)
			{
				return (*_pfoo_mutable_binary_op)(this, param1, param2);
			}
		default:
			assert(false);
			{
				any_type tmp;
				tmp.swap(param1);
			}
			return param1;
		}
	}

protected:
	struct
	{
		mark_value_type _foo_mark;
		union
		{
			void* _foo_val;
			vfoo_unary_op_type _pfoo_unary_op;
			vfoo_mutable_unary_op_type _pfoo_mutable_unary_op;
			vfoo_ref_ret_mutable_unary_op_type _pfoo_ref_ret_mutable_unary_op;
			vfoo_binary_op_type _pfoo_binary_op;
			vfoo_mutable_binary_op_type _pfoo_mutable_binary_op;
		};
	};
};

template<typename Any, typename Param, typename Handler,
			typename Result = typename func::foo_t_info<Handler>::result_type>
class any_unary_operator
	: public basic_any_operator<Any>
{
public:
	typedef Any any_type;
	typedef basic_any_operator<any_type> base_type;
	typedef Param param_type;
	typedef Result result_type;
	typedef Handler handler_type;
private:
	typedef any_unary_operator this_type;

public:
	any_unary_operator(const handler_type& handler)
		: base_type(
				base_type::tag_vfoo_unary_op_type::value,
				reinterpret_cast<void*>(&this_type::pro_s_unary_op)), // gcc need using reinterpret_cast<void*>
			_handler(handler)
	{
	}

	~any_unary_operator(void)
	{
	}

protected:
	inline any_type pro_unary_op(const any_type& param1) const
	{
		const param_type* ptr = yggr::any_cast<param_type>(boost::addressof(param1));
		return ptr? any_type(result_type((_handler)(*ptr))) : any_type();
	}

protected:
	inline static any_type pro_s_unary_op(const base_type* pbase, const any_type& param1)
	{
		assert(pbase);
		const this_type* pthis = reinterpret_cast<const this_type*>(pbase);
		assert(pthis);

		return pthis? pthis->pro_unary_op(param1) : any_type();
	}

private:
	handler_type _handler;
};

template<typename Any, typename Param, typename Handler,
			typename Result = typename func::foo_t_info<Handler>::result_type>
class any_mutable_unary_operator
	: public basic_any_operator<Any>
{
public:
	typedef Any any_type;
	typedef basic_any_operator<any_type> base_type;
	typedef Param param_type;
	typedef Result result_type;
	typedef Handler handler_type;
private:
	typedef any_mutable_unary_operator this_type;

public:
	any_mutable_unary_operator(const handler_type& handler)
		: base_type(
				base_type::tag_vfoo_mutable_unary_op_type::value,
				reinterpret_cast<void*>(&this_type::pro_s_mutable_unary_op)), // gcc need using reinterpret_cast<void*>
			_handler(handler)
	{
	}

	~any_mutable_unary_operator(void)
	{
	}

protected:
	inline any_type pro_mutable_unary_op(any_type& param1) const
	{
		param_type* ptr = yggr::any_cast<param_type>(boost::addressof(param1));
		return ptr? any_type(result_type((_handler)(*ptr))) : any_type();
	}

protected:
	inline static any_type pro_s_mutable_unary_op(const base_type* pbase, any_type& param1)
	{
		assert(pbase);
		const this_type* pthis = reinterpret_cast<const this_type*>(pbase);
		assert(pthis);

		return pthis? pthis->pro_mutable_unary_op(param1) : any_type();
	}

private:
	handler_type _handler;
};

template<typename Any, typename Param, typename Handler>
class any_ref_ret_mutable_unary_operator
	: public basic_any_operator<Any>
{
public:
	typedef Any any_type;
	typedef basic_any_operator<any_type> base_type;
	typedef Param param_type;
	typedef Handler handler_type;

private:
	typedef any_ref_ret_mutable_unary_operator this_type;

public:
	any_ref_ret_mutable_unary_operator(const handler_type& handler)
		: base_type(
				base_type::tag_vfoo_ref_ret_mutable_unary_op_type::value,
				reinterpret_cast<void*>(&this_type::pro_s_ref_ret_mutable_unary_op)), // gcc need using reinterpret_cast<void*>
			_handler(handler)
	{
	}

	~any_ref_ret_mutable_unary_operator(void)
	{
	}

protected:
	inline any_type& pro_ref_ret_mutable_unary_op(any_type& param1) const
	{
		param_type* ptr = yggr::any_cast<param_type>(boost::addressof(param1));
		if(ptr)
		{
			(_handler)(*ptr);
		}
		else
		{
			any_type tmp;
			param1.swap(tmp);
		}
		return param1;
	}

protected:
	inline static any_type& pro_s_ref_ret_mutable_unary_op(const base_type* pbase, any_type& param1)
	{
		assert(pbase);
		const this_type* pthis = reinterpret_cast<const this_type*>(pbase);
		assert(pthis);

		if(pthis)
		{
			return pthis->pro_ref_ret_mutable_unary_op(param1);
		}
		else
		{
			any_type tmp;
			param1.swap(tmp);
			return param1;
		}
	}

private:
	handler_type _handler;
};

template<typename Any, typename Param_Left, typename Param_Right, typename Handler,
			typename Result = typename func::foo_t_info<Handler>::result_type>
class any_binary_operator
	: public basic_any_operator<Any>
{
public:
	typedef Any any_type;
	typedef basic_any_operator<any_type> base_type;
	typedef Param_Left param_left_type;
	typedef Param_Right param_right_type;
	typedef Handler handler_type;
	typedef Result result_type;

private:
	typedef any_binary_operator this_type;

public:
	any_binary_operator(const Handler& handler)
		: base_type(
				base_type::tag_vfoo_binary_op_type::value,
				reinterpret_cast<void*>(&this_type::pro_s_binary_op)), // gcc need using reinterpret_cast<void*>
			_handler(handler)
	{
	}

	~any_binary_operator(void)
	{
	}

protected:
	inline any_type pro_binary_op(const any_type& param1, const any_type& param2) const
	{
		const param_left_type* pleft = yggr::any_cast<param_left_type>(boost::addressof(param1));
		const param_right_type* pright = yggr::any_cast<param_right_type>(boost::addressof(param2));

		return
			pleft && pright?
				any_type(result_type((_handler)(*pleft, *pright)))
				: any_type();
	}

protected:
	inline static any_type pro_s_binary_op(const base_type* pbase, const any_type& param1, const any_type& param2)
	{
		assert(pbase);
		const this_type* pthis = reinterpret_cast<const this_type*>(pbase);
		assert(pthis);

		return pthis? pthis->pro_binary_op(param1, param2) : any_type();
	}

private:
	handler_type _handler;
};

template<typename Any, typename Param_Left, typename Param_Right, typename Handler>
class any_mutable_binary_operator
	: public basic_any_operator<Any>
{
public:
	typedef Any any_type;
	typedef basic_any_operator<any_type> base_type;
	typedef Param_Left param_left_type;
	typedef Param_Right param_right_type;
	typedef Handler handler_type;

private:
	typedef any_mutable_binary_operator this_type;

public:
	any_mutable_binary_operator(const Handler& handler)
		: base_type(
				base_type::tag_vfoo_mutable_binary_op_type::value,
				reinterpret_cast<void*>(&this_type::pro_s_mutable_binary_op)), // gcc need using reinterpret_cast<void*>
			_handler(handler)
	{
	}

	~any_mutable_binary_operator(void)
	{
	}

protected:
	inline any_type& pro_mutable_binary_op(any_type& param1, const any_type& param2) const
	{
		param_left_type* pleft = yggr::any_cast<param_left_type>(boost::addressof(param1));
		const param_right_type* pright = yggr::any_cast<param_right_type>(boost::addressof(param2));

		if(pleft && pright)
		{
			(_handler)(*pleft, *pright);
		}
		else
		{
			any_type tmp;
			param1.swap(tmp);
		}

		return param1;
	}

protected:
	inline static any_type& pro_s_mutable_binary_op(const base_type* pbase, any_type& param1, const any_type& param2)
	{
		assert(pbase);
		const this_type* pthis = reinterpret_cast<const this_type*>(pbase);
		assert(pthis);

		if(pthis)
		{
			return pthis->pro_mutable_binary_op(param1, param2);
		}
		else
		{
			any_type tmp;
			param1.swap(tmp);
			return param1;
		}
	}

private:
	handler_type _handler;
};

} // namespace detail
} // namespace any_val
} // namespace yggr

namespace yggr
{
namespace any_val
{

struct operator_id_maker
{
public:
	typedef std::type_info type_info_type;
	typedef const type_info_type* type_info_cptr_type;

	typedef
		yggr::tuple
		<
			type_info_cptr_type, // tag
			type_info_cptr_type, // params1
			type_info_cptr_type	 // params2
		> id_type;

private:
	typedef operator_id_maker this_type;

public:
	template<typename Param, typename Tag> inline
	static id_type make(const Tag& tag)
	{
		typedef Param param_type;

		return yggr::make_tuple(
				addressof_typeinfo(tag),
				addressof_typeinfo<param_type>(),
				type_info_cptr_type(0) );
	}

	template<typename Tag> inline
	static id_type make(const Tag& tag, const std::type_info& param)
	{
		return yggr::make_tuple(
				addressof_typeinfo(tag),
				addressof_typeinfo(param),
				type_info_cptr_type(0) );
	}

	template<typename Param1, typename Param2, typename Tag> inline
	static id_type make(const Tag& tag)
	{
		typedef Param1 param1_type;
		typedef Param2 param2_type;

		return yggr::make_tuple(
				addressof_typeinfo(tag),
				addressof_typeinfo<param1_type>(),
				addressof_typeinfo<param2_type>());
	}

	template<typename Tag> inline
	static id_type make(const Tag& tag, const std::type_info& param1, const std::type_info& param2)
	{
		return yggr::make_tuple(
				addressof_typeinfo(tag),
				addressof_typeinfo(param1),
				addressof_typeinfo(param2));
	}
};

template<typename Any = yggr::any,
			typename OP_ID_Maker = operator_id_maker>
class any_operator_mgr
{
public:
	typedef Any any_type;
	typedef any_type value_type;

private:
	typedef OP_ID_Maker id_maker_type;
	typedef typename id_maker_type::id_type op_id_type;

private:
	typedef detail::basic_any_operator<any_type> basic_operator_type;
	typedef shared_ptr<basic_operator_type> interface_calculator_type;
	typedef yggr::safe_container::safe_unordered_map<op_id_type, interface_calculator_type> map_type;

private:
	typedef any_operator_mgr this_type;

public:
	any_operator_mgr(void)
	{
	}

	~any_operator_mgr(void)
	{
	}

public:

	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(positive)
	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(negate)

	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(head_increment)
	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(tail_increment)

	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(head_decrement)
	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(tail_decrement)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(plus)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(plus_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(minus)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(minus_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(multiplies)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(multiplies_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(divides)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(divides_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(modulus)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(modulus_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(logic_and)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(logic_or)
	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(logic_not)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_and)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_and_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_or)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_or_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_xor)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_xor_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_left_shift)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_left_shift_set)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_right_shift)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(bitwise_right_shift_set)

	YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF(bitwise_not)

	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(equal_to)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(not_equal_to)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(less)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(less_equal)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(greater)
	YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF(greater_equal)


	template<typename Param, typename Tag, typename Handler> inline
	bool register_unary_operator(const Tag& tag, const Handler& handler)
	{
		typedef Tag tag_type;
		typedef Param param_type;
		typedef Handler handler_type;

		typedef typename
			boost::mpl::if_
			<
				typename yggr::mplex::operator_is_mutable<tag_type>::type,
				typename
					boost::mpl::if_
					<
						typename yggr::mplex::operator_is_after<tag_type>::type,
						detail::any_mutable_unary_operator<any_type, param_type, handler_type>,
						detail::any_ref_ret_mutable_unary_operator<any_type, param_type, handler_type>
					>::type,
				detail::any_unary_operator<any_type, param_type, handler_type>
			>::type cal_type;

		typedef smart_ptr_ex::derived_deleter<cal_type> cal_deleter_type;

		op_id_type key(id_maker_type::template make<param_type>(tag));

		interface_calculator_type ptr =
			construct_shared<basic_operator_type>(
				yggr_nothrow_new cal_type(handler), cal_deleter_type() );

		return ptr && _map.insert(key, ptr);
	}

	template<typename L, typename R, typename Tag, typename Handler> inline
	bool register_binary_operator(const Tag& tag, const Handler& handler)
	{
		typedef Tag tag_type;
		typedef L left_type;
		typedef R right_type;
		typedef Handler handler_type;

		typedef typename
			boost::mpl::if_
			<
				typename yggr::mplex::operator_is_mutable<tag_type>::type,
				detail::any_mutable_binary_operator<any_type, left_type, right_type, handler_type>,
				detail::any_binary_operator<any_type, left_type, right_type, handler_type>
			>::type cal_type;

		typedef smart_ptr_ex::derived_deleter<cal_type> cal_deleter_type;

		op_id_type key = id_maker_type::template make<left_type, right_type>(tag);
		interface_calculator_type ptr =
			construct_shared<basic_operator_type>(
				yggr_nothrow_new cal_type(handler), cal_deleter_type() );

		return ptr && _map.insert(key, ptr);
	}

	template<typename Param, typename Tag> inline
	bool unregister_unary_operator(const Tag& tag)
	{
		typedef Param param_type;
		op_id_type key = id_maker_type::template make<param_type>(tag);
		return _map.erase(key);
	}

	template<typename L, typename R, typename Tag> inline
	bool unregister_binary_operator(const Tag& tag)
	{
		typedef L left_type;
		typedef R right_type;

		op_id_type key = id_maker_type::template make<left_type, right_type>(tag);

		return _map.erase(key);
	}

public:
	inline void clear(void)
	{
		_map.clear();
	}

	inline any_type call_greater_equal(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_greater_equal(), l, r);
	}

	inline any_type call_greater(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_greater(), l, r);
	}

	inline any_type call_less_equal(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_less_equal(), l, r);
	}

	inline any_type call_less(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_less(), l, r);
	}

	inline any_type call_not_equal_to(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_not_equal_to(), l, r);
	}

	inline any_type call_equal_to(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_equal_to(), l, r);
	}

	inline any_type call_bitwise_not(const any_type& val) const
	{
		return this_type::call_unary_op(yggr::mplex::make_op_tag_bitwise_not(), val);
	}

	inline any_type& call_bitwise_right_shift_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_bitwise_right_shift_set(), l, r);
	}

	inline any_type call_bitwise_right_shift(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_bitwise_right_shift(), l, r);
	}

	inline any_type& call_bitwise_left_shift_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_bitwise_left_shift_set(), l, r);
	}

	inline any_type call_bitwise_left_shift(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_bitwise_left_shift(), l, r);
	}

	inline any_type& call_bitwise_xor_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_bitwise_xor_set(), l, r);
	}

	inline any_type call_bitwise_xor(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_bitwise_xor(), l, r);
	}

	inline any_type& call_bitwise_or_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_bitwise_or_set(), l, r);
	}

	inline any_type call_bitwise_or(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_bitwise_or(), l, r);
	}

	inline any_type& call_bitwise_and_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_bitwise_and_set(), l, r);
	}

	inline any_type call_bitwise_and(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_bitwise_and(), l, r);
	}

	inline any_type call_logic_not(const any_type& val) const
	{
		return this_type::call_unary_op(yggr::mplex::make_op_tag_logic_not(), val);
	}

	inline any_type call_logic_or(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_logic_or(), l, r);
	}

	inline any_type call_logic_and(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_logic_and(), l, r);
	}

	inline any_type& call_modulus_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_modulus_set(), l, r);
	}

	inline any_type call_modulus(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_modulus(), l, r);
	}

	inline any_type& call_divides_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_divides_set(), l, r);
	}

	inline any_type call_divides(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_divides(), l, r);
	}

	inline any_type& call_multiplies_set(any_type& l, const any_type& r)
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_multiplies_set(), l, r);
	}

	inline any_type call_multiplies(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_multiplies(), l, r);
	}

	inline any_type& call_minus_set(any_type& l, const any_type& r) const
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_minus_set(), l, r);
	}

	inline any_type call_minus(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_minus(), l, r);
	}

	inline any_type& call_plus_set(any_type& l, const any_type& r) const
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_plus_set(), l, r);
	}

	inline any_type call_plus(const any_type& l, const any_type& r) const
	{
		return this_type::call_binary_op(yggr::mplex::make_op_tag_plus(), l, r);
	}

	inline any_type& call_set(any_type& l, const any_type& r) const
	{
		return this_type::call_mutable_binary_op(yggr::mplex::make_op_tag_set(), l, r);
	}

	inline any_type call_tail_decrement(any_type& val) const
	{
		return this_type::call_mutable_unary_op(yggr::mplex::make_op_tag_tail_decrement(), val);
	}

	inline any_type& call_head_decrement(any_type& val) const
	{
		return this_type::call_ref_ret_mutable_unary_op(yggr::mplex::make_op_tag_head_decrement(), val);
	}

	inline any_type call_tail_increment(any_type& val) const
	{
		return this_type::call_mutable_unary_op(yggr::mplex::make_op_tag_tail_increment(), val);
	}

	inline any_type& call_head_increment(any_type& val) const
	{
		return this_type::call_ref_ret_mutable_unary_op(yggr::mplex::make_op_tag_head_increment(), val);
	}

	inline any_type call_positive(const any_type& val) const
	{
		return this_type::call_unary_op(yggr::mplex::make_op_tag_positive(), val);
	}

	inline any_type call_negate(const any_type& val) const
	{
		return this_type::call_unary_op(yggr::mplex::make_op_tag_negate(), val);
	}

	template<typename Tag> inline
	any_type call_unary_op(const Tag& tag, const any_type& val) const
	{
		op_id_type key = id_maker_type::make(tag, val.type());
		interface_calculator_type pcal;
		_map.get_value(key, pcal);

		return pcal? pcal->unary_op(val) : any_type();
	}

	template<typename Tag> inline
	any_type& call_ref_ret_mutable_unary_op(const Tag& tag, any_type& val) const
	{
		op_id_type key = id_maker_type::make(tag, val.type());
		interface_calculator_type pcal;
		_map.get_value(key, pcal);

		if(pcal)
		{
			return pcal->ref_ret_mutable_unary_op(val);
		}
		else
		{
			any_type tmp;
			val.swap(tmp);
		}
		return val;
	}

	template<typename Tag> inline
	any_type call_mutable_unary_op(const Tag& tag, any_type& val) const
	{
		op_id_type key = id_maker_type::make(tag, val.type());
		interface_calculator_type pcal;
		_map.get_value(key, pcal);

		return pcal? pcal->mutable_unary_op(val) : any_type();
	}

	template<typename Tag> inline
	any_type call_binary_op(const Tag& tag, const any_type& l, const any_type& r) const
	{
		op_id_type key = id_maker_type::make(tag, l.type(), r.type());
		interface_calculator_type pcal;
		_map.get_value(key, pcal);
		return pcal? pcal->binary_op(l, r) : any_type();
	}

	template<typename Tag> inline
	any_type& call_mutable_binary_op(const Tag& tag, any_type& l, const any_type& r) const
	{
		op_id_type key = id_maker_type::make(tag, l.type(), r.type());
		interface_calculator_type pcal;
		_map.get_value(key, pcal);

		if(pcal)
		{
			return pcal->mutable_binary_op(l, r);
		}
		else
		{
			any_type tmp;
			l.swap(tmp);
		}
		return l;
	}

private:
	map_type _map;
};

} // namespace any_val
} // namespace yggr

#define _YGGR_ANY_VAL_ANY_OPERATOR() \
	struct any_cref { \
		typedef any_cref this_type; \
		typedef any any_type; \
		any_cref(const any_type& any_val) : _cref(any_val) {} \
		any_cref(const this_type& right) : _cref(right._cref) {} \
		inline operator const any_type&(void) const { return _cref; } \
		const any_type& _cref; }; \
	 \
	inline any operator>=(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_greater_equal(l, r) : any_type(); } \
	 \
	inline any operator>(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_greater(l, r) : any_type(); } \
	 \
	inline any operator<=(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_less_equal(l, r) : any_type(); } \
	 \
	inline any operator<(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_less(l, r) : any_type(); } \
	 \
	inline any operator!=(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_not_equal_to(l, r) : any_type(); } \
	 \
	inline any operator==(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_equal_to(l, r) : any_type(); } \
	 \
	inline any operator~(const any_cref& val){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_not(val) : any_type(); } \
	 \
	inline any& operator>>=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_right_shift_set(l, r) : l; } \
	 \
	inline any operator>>(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_right_shift(l, r) : any_type(); } \
	 \
	inline any& operator<<=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_left_shift_set(l, r) : l; } \
	 \
	inline any operator<<(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_left_shift(l, r) : any_type(); } \
	 \
	inline any& operator^=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_xor_set(l, r) : l; } \
	 \
	inline any operator^(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_xor(l, r) : any_type(); } \
	 \
	inline any& operator|=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_or_set(l, r) : l; } \
	 \
	inline any operator|(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_or(l, r) : any_type(); } \
	 \
	inline any& operator&=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_and_set(l, r) : l; } \
	 \
	inline any operator&(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_bitwise_and(l, r) : any_type(); } \
	 \
	inline any operator!(const any_cref& l){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_logic_not(l) : any_type(); } \
	 \
	inline any operator||(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_logic_or(l, r) : any_type(); } \
	 \
	inline any operator&&(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_logic_and(l, r) : any_type(); } \
	 \
	inline any& operator%=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_modulus_set(l, r) : l; } \
	 \
	inline any operator%(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_modulus(l, r) : any_type(); } \
	 \
	inline any& operator/=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_divides_set(l, r) : l; } \
	 \
	inline any operator/(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_divides(l, r) : any_type(); } \
	 \
	inline any& operator*=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_multiplies_set(l, r) : l; } \
	 \
	inline any operator*(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_multiplies(l, r) : any_type(); } \
	 \
	inline any& operator-=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_minus_set(l, r) : l; } \
	 \
	inline any operator-(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_minus(l, r) : any_type(); } \
	 \
	inline any& operator+=(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_plus_set(l, r) : l;	} \
	\
	inline any operator+(const any_cref& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_plus(l, r) : any_type(); } \
	 \
	inline any& operator_set(any& l, const any_cref& r){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_set(l, r) : l; } \
	 \
	inline any operator--(any& val, int){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_tail_decrement(val) : val; } \
	 \
	inline any& operator--(any& val){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_head_decrement(val) : val; } \
	 \
	inline any operator++(any& val, int){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_tail_increment(val) : any_type(); } \
	 \
	inline any& operator++(any& val){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_head_increment(val) : val; } \
	 \
	inline any operator+(const any_cref& val){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_positive(val) : any_type(); } \
	\
	inline any operator-(const any_cref& val){ \
		typedef any any_type; \
		typedef yggr::any_val::any_operator_mgr<any_type> mgr_type; \
		typedef yggr::ptr_single<mgr_type> single_type; \
		single_type::obj_ptr_type pmgr = single_type::get_ins(); \
		return pmgr? pmgr->call_negate(val) : any_type(); }

namespace yggr
{
	_YGGR_ANY_VAL_ANY_OPERATOR()
} // namespace yggr

namespace boost
{
	_YGGR_ANY_VAL_ANY_OPERATOR()
} // namespace boost


#undef _YGGR_ANY_VAL_ANY_OPERATOR

#undef YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR
#undef YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR
#undef YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR
#undef YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR

#undef YGGR_PP_ANY_VAL_UNREGISTER_UNARY_OPERATOR
#undef YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_UNARY_OPERATOR
#undef YGGR_PP_ANY_VAL_UNREGISTER_BINARY_OPERATOR
#undef YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_BINARY_OPERATOR

#undef YGGR_PP_ANY_VAL_MAKE_UNARY_CALCULATOR_REG_FOO_DEF
#undef YGGR_PP_ANY_VAL_MAKE_BINARY_CALCULATOR_REG_FOO_DEF

#endif // __YGGR_ANY_VAL_ANY_CALCULATOR_MGR_HPP__
