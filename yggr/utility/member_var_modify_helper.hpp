// member_var_modify_helper.hpp

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

#ifndef __YGGR_UTILITY_MEMBER_VAR_MODIFY_HELPER_HPP__
#define __YGGR_UTILITY_MEMBER_VAR_MODIFY_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/pointer_to_reference_t.hpp>

#include <yggr/type_traits/operator_result.hpp>
#include <yggr/type_traits/detail/operator_result_stream_patch.hpp>
#include <yggr/type_traits/has_operator_index_member_access.hpp>
#include <yggr/type_traits/has_operator_pointer_member_access.hpp>

#include <yggr/support/operator_t.hpp>

#include <yggr/func/is_callable.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/typeof/typeof.hpp>

#include <yggr/math/modulus.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/ref.hpp>
#include <boost/function.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <ostream>

namespace yggr
{
namespace utility
{
namespace detail
{

typedef boost::mpl::size_t<static_cast<std::size_t>(-1)> tag_op_failed_type;
typedef boost::mpl::size_t<0> tag_op_non_type;
typedef boost::mpl::size_t<1> tag_op_index_member_access_type;
typedef boost::mpl::size_t<2> tag_op_pointer_member_access_type;
typedef boost::mpl::size_t<4> tag_op_call_type;

template<typename TObj, typename Member_Var_T>
class member_var_modify_helper_data_store
{
protected:
	typedef tag_op_non_type tag_op_type;

public:
	typedef TObj value_type;
	typedef Member_Var_T member_value_type;

private:
	BOOST_MPL_ASSERT_NOT((boost::is_reference<member_value_type>));

public:
	typedef boost::reference_wrapper<value_type> value_reference_type;
	typedef boost::reference_wrapper<member_value_type> member_value_ref_type;

public:
	typedef member_value_type& modify_reference_type;

private:
	typedef member_var_modify_helper_data_store this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	member_var_modify_helper_data_store(value_type& val, member_value_type& member_val)
		: _val(val), _member_val(member_val)
	{
	}

	member_var_modify_helper_data_store(BOOST_RV_REF(this_type) right)
		: _val(right._val), _member_val(right._member_val)
	{
	}

	member_var_modify_helper_data_store(const this_type& right)
		: _val(right._val), _member_val(right._member_val)
	{
	}

	~member_var_modify_helper_data_store(void)
	{
	}

public:
	inline const this_type& operator=(BOOST_RV_REF(this_type) right) const
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		_val = boost::move(right_ref._val);
		_member_val = boost::move(right_ref._member_val);
		return *this;
	}

	inline const this_type& operator=(const this_type& right) const
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_val = right._val;
		_member_val = right._member_val;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right) const
	{
		this_type& right_ref = right;
		return this_type::swap(right_ref);
	}

	inline void swap(this_type& right) const
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(_val, right._val);
		yggr::swap(_member_val, right._member_val);
	}

public:
	inline modify_reference_type modify(void) const
	{
		return _member_val.get();
	}

	inline value_type& value(void) const
	{
		return _val.get();
	}

protected:
	value_reference_type _val;
	member_value_ref_type _member_val;
};

template<typename TObj>
class member_var_modify_helper_data_store<TObj, mplex::null_type>
{
protected:
	typedef tag_op_non_type tag_op_type;

public:
	typedef TObj value_type;
	typedef mplex::null_type member_value_type;

private:
	BOOST_MPL_ASSERT_NOT((boost::is_reference<member_value_type>));

public:
	typedef boost::reference_wrapper<value_type> value_reference_type;

public:
	typedef member_value_type modify_reference_type;

private:
	typedef member_var_modify_helper_data_store this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	template<typename Ignore>
	member_var_modify_helper_data_store(value_type& val, Ignore&)
		: _val(val)
	{
	}

	member_var_modify_helper_data_store(BOOST_RV_REF(this_type) right)
		: _val(right._val)
	{
	}

	member_var_modify_helper_data_store(const this_type& right)
		: _val(right._val)
	{
	}

	~member_var_modify_helper_data_store(void)
	{
	}

public:
	inline const this_type& operator=(BOOST_RV_REF(this_type) right) const
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		_val = boost::move(right_ref._val);
		return *this;
	}

	inline const this_type& operator=(const this_type& right) const
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_val = right._val;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right) const
	{
		this_type& right_ref = right;
		return this_type::swap(right_ref);
	}

	inline void swap(this_type& right) const
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(_val, right._val);
	}

public:
	inline modify_reference_type modify(void) const
	{
		return member_value_type();
	}

	inline value_type& value(void) const
	{
		return _val.get();
	}

protected:
	value_reference_type _val;
};

template<typename Base,
			bool has_idx_mem_acc =
				has_operator_index_member_access<typename Base::member_value_type>::value >
class member_var_modify_helper_op_index_member_access;

template<typename Base,
			bool has_idx_mem_acc =
				has_operator_pointer_member_access<typename Base::member_value_type>::value >
class member_var_modify_helper_op_pointer_member_access;

template<typename Base,
			bool has_idx_mem_acc =
				func::is_callable<typename Base::member_value_type>::value >
class member_var_modify_helper_op_call;

#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_BEG( __op_name__, __op_tag__ ) \
	template<typename Base> \
	class __op_name__<Base, false>{ \
	public: \
		typedef tag_op_failed_type tag_op_type; }; \
	\
	template<typename Base> \
	class __op_name__<Base, true> : public Base { \
	\
	public: \
		typedef Base base_type; \
		typedef typename base_type::value_type value_type; \
		typedef typename base_type::member_value_type member_value_type; \
		\
		typedef typename base_type::value_reference_type value_reference_type; \
		typedef typename base_type::modify_reference_type modify_reference_type; \
	\
	protected: \
		typedef typename \
			boost::mpl::size_t< \
				((base_type::tag_op_type::value) | (__op_tag__::value)) \
			>::type tag_op_type; \
	\
	private: \
		typedef __op_name__ this_type; \
		BOOST_COPYABLE_AND_MOVABLE(this_type); \
	\
	public: \
		template<typename MemberVar> \
		__op_name__(value_type& val, MemberVar& member_val) \
			: base_type(val, member_val) { } \
		\
		__op_name__(BOOST_RV_REF(base_type) right) \
			: base_type(boost::move(right)) { } \
		\
		__op_name__(const base_type& right) \
			: base_type(right) { } \
		\
		__op_name__(BOOST_RV_REF(this_type) right) \
			: base_type(boost::move(static_cast<base_type&>(right))) { } \
		\
		__op_name__(const this_type& right) \
			: base_type(right) { } \
		\
		~__op_name__(void) { } \
	\
	public: \
		inline const this_type& operator=(BOOST_RV_REF(base_type) right) const { \
			base_type& right_ref = right; \
			base_type::operator=(boost::move(right_ref)); \
			return *this; } \
		\
		inline const this_type& operator=(const base_type& right) const { \
			base_type::operator=(right); \
			return *this; } \
		\
		inline const this_type& operator=(BOOST_RV_REF(this_type) right) const { \
			this_type& right_ref = right; \
			base_type::operator=(boost::move(static_cast<base_type&>(right_ref))); \
			return *this; } \
		\
		inline const this_type& operator=(const this_type& right) const { \
			base_type::operator=(static_cast<const base_type&>(right)); \
			return *this; } \
	\
	public: \
		using base_type::swap; \
		\
		inline void swap(BOOST_RV_REF(this_type) right) const { \
			this_type& right_ref = right; \
			return this_type::swap(right_ref); } \
		\
		inline void swap(this_type& right) const { \
			base_type::swap(static_cast<base_type&>(right)); } \
	\
	public: \
		using base_type::modify; \
		using base_type::value;

#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_END() };


// member_var_modify_helper_op_index_member_access

template<typename Cont, typename Idx, bool bis_class = boost::is_class<Cont>::value>
struct _member_var_modify_helper_op_index_result_of;

template<typename Cont, typename Idx>
struct _member_var_modify_helper_op_index_result_of<Cont, Idx, false>
{
private:
	typedef Cont cont_type;
	typedef Idx index_type;
	typedef typename boost::add_reference<cont_type>::type cont_ref_type;
	typedef YGGR_TYPEOF_TPL(((declval<cont_ref_type>())[declval<index_type>()])) tmp_type;

public:
	typedef typename boost::add_reference<tmp_type>::type type;
};

template<typename Cont, typename Idx>
struct _member_var_modify_helper_op_index_result_of<Cont, Idx, true>
{
private:
	typedef Cont cont_type;
	typedef Idx index_type;
	typedef typename boost::add_reference<cont_type>::type cont_ref_type;
	typedef YGGR_TYPEOF_TPL(declval<cont_ref_type>().operator[](declval<index_type>())) tmp_type;

public:
	typedef typename boost::add_reference<tmp_type>::type type;
};

template<typename Cont, typename Idx>
struct member_var_modify_helper_op_index_result_of
	: public _member_var_modify_helper_op_index_result_of<Cont, Idx>
{
};

YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_BEG(
	member_var_modify_helper_op_index_member_access, tag_op_index_member_access_type)

public:
	template<typename Idx> inline
	typename member_var_modify_helper_op_index_result_of<member_value_type, Idx>::type
		operator[](const Idx& idx) const
	{
		return (base_type::modify())[idx];
	}

YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_END()


// member_var_modify_helper_op_pointer_member_access

template<typename T, bool bis_pointer = boost::is_pointer<T>::value >
struct op_ptr_mem_acc_hlp;

template<typename T>
struct op_ptr_mem_acc_hlp<T, true>
{
public:
	typedef T arg_type;
	typedef T result_type;

public:
	inline result_type operator()(arg_type val) const
	{
		return val;
	}
};

template<typename T>
struct op_ptr_mem_acc_hlp<T, false>
{
public:
	typedef T arg_type;
	typedef YGGR_TYPEOF_TPL((declval<arg_type&>().operator->())) result_type;

public:
	inline result_type operator()(arg_type& val) const
	{
		return val.operator->();
	}
};

YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_BEG(
	member_var_modify_helper_op_pointer_member_access, tag_op_pointer_member_access_type)

protected:
	typedef op_ptr_mem_acc_hlp<member_value_type> op_ptr_mem_acc_hlp_type;

public:
	inline typename op_ptr_mem_acc_hlp_type::result_type
		operator->(void) const
	{
		op_ptr_mem_acc_hlp_type helper;
		return helper(base_type::modify());
	}

YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_END()

// member_var_modify_helper_op_call
// !! not support overload function
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_BEG(
	member_var_modify_helper_op_call, tag_op_call_type)

public:

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> inline
	typename func::foo_t_info<member_value_type>::result_type
		operator()(BOOST_FWD_REF(Args) ...args) const
	{
		return (base_type::modify())(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		typename func::foo_t_info<member_value_type>::result_type \
			operator()( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return (base_type::modify())(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // #if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)


YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_END()

#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_BEG
#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_OP_IMPL_END

// member_var_modify_helper_op_index_member_access_if_
template<typename MemberVal, typename Base,
			bool bchk = has_operator_index_member_access<MemberVal>::value>
struct member_var_modify_helper_op_index_member_access_if_;

template<typename MemberVal, typename Base>
struct member_var_modify_helper_op_index_member_access_if_<MemberVal, Base, true>
{
	typedef member_var_modify_helper_op_index_member_access<Base, true> type;
};

template<typename MemberVal, typename Base>
struct member_var_modify_helper_op_index_member_access_if_<MemberVal, Base, false>
{
	typedef Base type;
};

// member_var_modify_helper_op_index_member_access_if_
template<typename MemberVal, typename Base,
			bool bchk = has_operator_pointer_member_access<MemberVal>::value>
struct member_var_modify_helper_op_pointer_member_access_if_;

template<typename MemberVal, typename Base>
struct member_var_modify_helper_op_pointer_member_access_if_<MemberVal, Base, true>
{
	typedef member_var_modify_helper_op_pointer_member_access<Base> type;
};

template<typename MemberVal, typename Base>
struct member_var_modify_helper_op_pointer_member_access_if_<MemberVal, Base, false>
{
	typedef Base type;
};

// member_var_modify_helper_op_call_if_
template<typename MemberVal, typename Base,
			bool bchk = func::is_callable<MemberVal>::value>
struct member_var_modify_helper_op_call_if_;

template<typename MemberVal, typename Base>
struct member_var_modify_helper_op_call_if_<MemberVal, Base, true>
{
	typedef member_var_modify_helper_op_call<Base> type;
};

template<typename MemberVal, typename Base>
struct member_var_modify_helper_op_call_if_<MemberVal, Base, false>
{
	typedef Base type;
};

template<typename TObj, typename Member_Var_T>
struct _member_var_modify_helper_op_combo
{
public:
	typedef TObj value_type;
	typedef Member_Var_T member_value_type;

protected:
	typedef member_var_modify_helper_data_store<value_type, member_value_type> eins_type;

	typedef typename
		member_var_modify_helper_op_index_member_access_if_
		<
			member_value_type,
			eins_type
		>::type zwei_type;

	typedef typename
		member_var_modify_helper_op_pointer_member_access_if_
		<
			member_value_type,
			zwei_type
		>::type drei_type;

	typedef typename
		member_var_modify_helper_op_call_if_
		<
			member_value_type,
			drei_type
		>::type funf_type;

public:
	typedef funf_type type;
};

template<typename TObj, typename Member_Var_T>
struct member_var_modify_helper_op_combo
	: public _member_var_modify_helper_op_combo<TObj, Member_Var_T>
{
};

// member_var_modify_setter_helper
template<typename ModifyVal, bool is_pointer = boost::is_pointer<ModifyVal>::value>
struct member_var_modify_setter_helper;

template<typename ModifyVal>
struct member_var_modify_setter_helper<ModifyVal, true>
{
public:
	typedef ModifyVal modify_value_type;

public:
	template<typename Handler, typename MemVal> inline
	void operator()(const Handler& handler, MemVal& mem_val, int arg) const
	{
		assert(arg == 0);
		modify_value_type param = 0;
		handler(mem_val, param);
	}

	template<typename Handler, typename MemVal, typename Arg> inline
	void operator()(const Handler& handler, MemVal& mem_val, const Arg& arg) const
	{
		modify_value_type param(arg);
		handler(mem_val, param);
	}

	template<typename Handler, typename MemVal, typename Arg> inline
	void operator()(const Handler& handler, MemVal& mem_val, const Arg*& arg) const
	{
		modify_value_type param(arg);
		handler(mem_val, param);
	}

	template<typename Handler, typename MemVal, typename T, std::size_t N> inline
	void operator()(const Handler& handler, MemVal& mem_val, const T(&arg)[N]) const
	{
		modify_value_type param = boost::addressof(arg[0]);
		handler(mem_val, param);
	}
};

template<typename ModifyVal>
struct member_var_modify_setter_helper<ModifyVal, false>
{
public:
	typedef ModifyVal modify_value_type;

public:
	template<typename Handler, typename MemVal, typename Arg> inline
	void operator()(const Handler& handler, MemVal& mem_val, const Arg& arg) const
	{
		typedef typename
			boost::mpl::if_
			<
				typename boost::is_base_of<modify_value_type, Arg>::type,
				const modify_value_type&,
				modify_value_type
			>::type param_type;

		param_type param(arg);
		handler(mem_val, param);
	}
};

} //namespace detail

template<typename TObj,
			typename Member_Var_T,
			typename Modify_T,
			typename View_T>
class member_var_modify_helper
	: public detail::member_var_modify_helper_op_combo<TObj, Member_Var_T>::type
{
public:
	typedef TObj value_type;
	typedef Member_Var_T member_value_type;
	typedef Modify_T modify_value_type;
	typedef View_T view_value_type;

private:
	BOOST_MPL_ASSERT_NOT((boost::is_reference<member_value_type>));

public:
	typedef typename
		detail::member_var_modify_helper_op_combo
		<
			value_type,
			member_value_type
		>::type base_type;

public:
	typedef typename base_type::tag_op_type tag_op_type;
	BOOST_MPL_ASSERT_NOT((boost::is_same<tag_op_type, detail::tag_op_failed_type>));

public:
	typedef typename base_type::value_reference_type value_reference_type;

public:
	typedef typename base_type::modify_reference_type modify_reference_type;

public:
	typedef boost::function2<void, value_type&, const modify_value_type&> modify_handler_type;
	typedef boost::function1<view_value_type, const value_type&> view_handler_type;

private:
	typedef member_var_modify_helper this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	template<typename MemberVal>
	member_var_modify_helper(value_type& val, MemberVal& member_val,
								const modify_handler_type& modify_handler,
								const view_handler_type& view_handler)
		: base_type(val, member_val),
			_modify_handler(modify_handler),
			_view_handler(view_handler)
	{
	}

	template<typename MemberVal, typename ModifyHandler, typename ViewHandler>
	member_var_modify_helper(value_type& val, MemberVal& member_val,
								const ModifyHandler& modify_handler,
								const ViewHandler& view_handler)
		: base_type(val, member_val),
			_modify_handler(modify_handler),
			_view_handler(view_handler)
	{
	}

	member_var_modify_helper(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))),
            _modify_handler(boost::move(right._modify_handler)),
			_view_handler(boost::move(right._view_handler))
	{
	}

	member_var_modify_helper(const this_type& right)
		: base_type(static_cast<const base_type&>(right)),
			_modify_handler(right._modify_handler),
			_view_handler(right._view_handler)
	{
	}

	~member_var_modify_helper(void)
	{
	}

public:
	inline const this_type& operator=(const modify_value_type& right) const
	{
		_modify_handler(base_type::value(), right);
		return *this;
	}

	template<typename Param> inline
	const this_type& operator=(const Param& right) const
	{
		typedef detail::member_var_modify_setter_helper<modify_value_type> setter_helper_type;

		setter_helper_type helper;
		helper(_modify_handler, base_type::value(), right);
		return *this;
	}

	inline const this_type& operator=(BOOST_RV_REF(this_type) right) const
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right_ref)));
		_modify_handler = boost::move(right_ref._modify_handler);
		_view_handler = boost::move(right_ref._view_handler);
		return *this;
	}

	inline const this_type& operator=(const this_type& right) const
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::operator=(static_cast<const base_type&>(right));
		_modify_handler = right._modify_handler;
		_view_handler = right._view_handler;
		return *this;
	}

public:
	//Member_Var_T swap must using tmp value, because modify may be a group operator
	template<typename T> inline
	void swap(T& right) const
	{
		member_value_type tmp_val = base_type::modify();
		this_type::operator=(right);
		right = tmp_val;
	}

	template<typename OTObj, typename OMember_Var_T, typename OModify_T, typename OView_T> inline
	void swap(member_var_modify_helper<OTObj, OMember_Var_T, OModify_T, OView_T>& right) const
	{
		member_value_type tmp_val = base_type::modify();
		this_type::operator=(right.modify());
		right = tmp_val;
	}

	inline void swap(BOOST_RV_REF(this_type) right) const
	{
		this_type& right_ref = right;
		return this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		base_type::swap(static_cast<base_type&>(right));
		yggr::swap(_modify_handler, right._modify_handler);
		yggr::swap(_view_handler, right._view_handler);
	}

public:
	using base_type::value;
	using base_type::modify;

	inline view_value_type view(void) const
	{
		return _view_handler(base_type::value());
	}

public:
	inline operator modify_reference_type(void) const
	{
		return base_type::modify();
	}

	inline operator view_value_type(void) const
	{
		return this_type::view();
	}

private:
	modify_handler_type _modify_handler;
	view_handler_type _view_handler;
};

// io
template<typename Char, typename Traits,
			typename T, typename Member_Var_T, typename Modify_T, typename View_T>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const member_var_modify_helper<T, Member_Var_T, Modify_T, View_T>& val)
{
	typedef member_var_modify_helper<T, Member_Var_T, Modify_T, View_T> out_type;
	typedef typename out_type::member_value_type member_value_type;

	os << val.view();
	return os;
}

//positive
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline //in linux VT1 conflict <bits/termios-c_oflag.h>
typename positive_operator_result<VwT1>::type
	operator+(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return (val.view());
}

// negate
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
typename negate_operator_result<VwT1>::type
	operator-(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return -(val.view());
}

//dereference
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
typename mplex::pointer_to_reference_t<MVT1>::type
	operator*(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return *(val.modify());
}

// addressof
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
typename  boost::add_pointer<MVT1>::type
	operator&(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return &(val.modify());
}

// increment
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
typename boost::add_reference<MVT1>::type
	operator++(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return ++(val.modify());
}

template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
MVT1 operator++(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val, int)
{
	return (val.modify())++;
}

// decrement
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
typename boost::add_reference<MVT1>::type
	operator--(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return --(val.modify());
}

template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
MVT1 operator--(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val, int)
{
	return (val.modify())--;
}

// arithmetic

#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC( __op__, __result_of__ ) \
	template<typename T1, typename T2, typename MVT2, typename MT2, typename VwT2> inline \
	typename __result_of__<T1, VwT2>::type \
		operator __op__(const T1& l, const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r) { \
		return (l) __op__ (r.view()); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1, typename T2> inline \
	typename __result_of__<VwT1, T2>::type \
		operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, const T2& r) { \
		return (l.view()) __op__ (r); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1, \
				typename T2, typename MVT2, typename MT2, typename VwT2 > inline \
	typename __result_of__<VwT1, VwT2>::type \
		operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, \
							const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r) { \
		return (l.view()) __op__ (r.view()); }

// arithmetic set
#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET( __op__, __const_op__ ) \
	template<typename T1, typename T2, typename MVT2, typename MT2, typename VwT2> inline \
	T1& operator __op__(T1& l, const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r) { \
		return (l) __op__ (r.view()); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1, typename T2> inline \
	const member_var_modify_helper<T1, MVT1, MT1, VwT1>& \
		operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, const T2& r) { \
		return l = ((l.view()) __const_op__ (r)); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1, \
				typename T2, typename MVT2, typename MT2, typename VwT2 > inline \
	const member_var_modify_helper<T1, MVT1, MT1, VwT1>& \
		operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, \
							const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r) { \
		return l = ((l.view()) __const_op__ (r.view())); }

// +, +=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(+, plus_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(+=, +)

// -, -=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(-, minus_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(-=, -)

// *, *=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(*, multiplies_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(*=, *)

// /, /=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(/, divides_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(/=, /)

// bitwise
// &, &=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(&, bitwise_and_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(&=, &)

// |, |=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(|, bitwise_or_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(|=, |)

// ^, ^=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(^, bitwise_xor_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(^=, ^)

// <<, <<=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(<<, bitwise_left_shift_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(<<=, <<)

// >>, >>=
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC(>>, bitwise_right_shift_operator_result)
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET(>>=, >>)


#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC
#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_ARITHMETIC_SET

// %
template<typename T1, typename T2, typename MVT2, typename MT2, typename VwT2> inline
	typename modulus_operator_result<T1, VwT2>::type
		operator %(const T1& l, const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r)
{
	return math::modulus(l, r.view());
}

template<typename T1, typename MVT1, typename MT1, typename VwT1, typename T2> inline
typename modulus_operator_result<VwT1, T2>::type
	operator %(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, const T2& r)
{
	return math::modulus(l.view(), r);
}

template<typename T1, typename MVT1, typename MT1, typename VwT1,
			typename T2, typename MVT2, typename MT2, typename VwT2 > inline
typename modulus_operator_result<VwT1, VwT2>::type
	operator %(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l,
						const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r)
{
	return math::modulus(l.view(), r.view());
}

// %=
template<typename T1, typename T2, typename MVT2, typename MT2, typename VwT2> inline
T1& operator %=(T1& l, const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r)
{
	return (l) %= (r.view());
}

template<typename T1, typename MVT1, typename MT1, typename VwT1, typename T2> inline
const member_var_modify_helper<T1, MVT1, MT1, VwT1>&
	operator %=(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, const T2& r)
{
	return l = math::modulus(l.view(), r);
}

template<typename T1, typename MVT1, typename MT1, typename VwT1,
			typename T2, typename MVT2, typename MT2, typename VwT2 > inline
const member_var_modify_helper<T1, MVT1, MT1, VwT1>&
	operator %=(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l,
						const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r)
{
	return l = math::modulus(l.view(), r.view());
}

// bitwise_not
template<typename T1, typename MVT1, typename MT1, typename VwT1> inline
typename bitwise_not_operator_result<VwT1>::type
	operator~(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& val)
{
	return ~(val.view());
}

// compare
#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( __op__ ) \
	template<typename T1, typename MVT1, typename MT1, typename VwT1> inline \
	typename boost::enable_if<boost::is_pointer<typename boost::remove_reference<VwT1>::type>, bool>::type \
		operator __op__(int l, const member_var_modify_helper<T1, MVT1, MT1, VwT1>& r) { \
		assert(l == 0); \
		return l == 0 && (0) __op__ (r.view()); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1> inline \
	typename boost::enable_if<boost::is_pointer<typename boost::remove_reference<VwT1>::type>, bool>::type \
		operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, int r) { \
		assert(r == 0); \
		return r == 0 && (l.view()) __op__ (0); } \
	\
	template<typename T1, typename T2, typename MVT2, typename MT2, typename VwT2> inline \
	bool operator __op__(const T1& l, const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r) { \
		return (l) __op__ (r.view()); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1, typename T2> inline \
	bool operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, const T2& r) { \
		return (l.view()) __op__ (r); } \
	\
	template<typename T1, typename MVT1, typename MT1, typename VwT1, \
				typename T2, typename MVT2, typename MT2, typename VwT2 > inline \
	bool operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, \
							const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r) { \
		return (l.view()) __op__ (r.view()); }

#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_ENIS( __op__ ) \
	template<typename T1, typename MVT1, typename MT1, typename VwT1> inline \
	bool operator __op__(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l) { \
		return __op__(l.view()); }


#define YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_ZWEI YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP

YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( == )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( != )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( < )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( <= )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( > )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP( >= )

YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_ENIS( ! )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_ZWEI( && )
YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_ZWEI( || )

#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_ZWEI
#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_LOGIC_EINS
#undef YGGR_TMP_PP_MEMBER_VAR_MODIFY_HELPER_CMP


} // namespace utility

using utility::member_var_modify_helper;

} // namespace yggr

namespace yggr
{
namespace type_traits
{
namespace detail
{

// fix error C2664
template<typename TObj,
			typename Member_Var_T,
			typename Modify_T,
			typename View_T>
struct _has_op_idx_mem_acc_class_custom<utility::member_var_modify_helper<TObj, Member_Var_T, Modify_T, View_T> >
	: public
		boost::mpl::bool_
		<
			((utility::member_var_modify_helper<TObj, Member_Var_T, Modify_T, View_T>::tag_op_type::value
				& utility::detail::tag_op_index_member_access_type::value) != 0)
		>
{
};

} // namespace detail
} // namespace type_traits
} // namespace yggr

// swap
namespace yggr
{
namespace utility
{
namespace swap_support
{

template<typename T1, typename T2, typename MVT2, typename MT2, typename VwT2> inline
void swap(T1& l, const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r)
{
	r.swap(l);
}

template<typename T1, typename MVT1, typename MT1, typename VwT1, typename T2> inline
void swap(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l, T2& r)
{
	l.swap(r);
}

template<typename T1, typename MVT1, typename MT1, typename VwT1,
			typename T2, typename MVT2, typename MT2, typename VwT2> inline
void swap(const member_var_modify_helper<T1, MVT1, MT1, VwT1>& l,
			const member_var_modify_helper<T2, MVT2, MT2, VwT2>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace utility
} // namespace yggr

namespace std
{
	using ::yggr::utility::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::utility::swap_support::swap;
} // namespace boost

#endif // __YGGR_UTILITY_MEMBER_VAR_MODIFY_HELPER_HPP__
