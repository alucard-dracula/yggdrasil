//unique_mapping.hpp

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

#ifndef __YGGR_MAPPING_UNIQUE_MAPPING_HPP__
#define __YGGR_MAPPING_UNIQUE_MAPPING_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/support/return_arg.hpp>

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#	include <yggr/type_traits/integer_sequence.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/packet/packet_info.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_sequenced_index.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/revert_to_vector.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/mpl/vector.hpp>

#include <boost/multi_index/key_extractors.hpp>

#if defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// _YGGR_TMP_PP_TEP_SUB_OP(__n__, __val__ )
// _YGGR_TMP_PP_TEP_VALUE ()
#	define _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			_YGGR_TMP_PP_TEP_SUB_OP( __n__, YGGR_PP_OPEN( _YGGR_TMP_PP_TEP_VALUE  ) )

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace mapping
{

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() boost::tuples::null_type
template<
			YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_TEMPLATE_PARAMS_LEN(), typename Val )
			YGGR_PP_SYMBOL_IF(YGGR_PP_TEMPLATE_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			typename Mutex = void
		>
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
class unique_mapping
{
private:
	typedef Mutex tpl_arg_mutex_type;

public:
	typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_PARAMS_LEN(), Val) > value_type;

private:
	typedef unique_mapping this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	template<typename Tag> inline
	static const typename Tag::type& s_prv_get_elem(const value_type& val)
	{
		return val.template get<Tag::index>();
	}

private:
	// make_indexed_by don't outside unique_mapping
	template<size_type i = 0,
				size_type isize = value_type::E_length,
				typename Nil = int>
	struct make_indexed_by
	{
		typedef make_indexed_by<i + 1, isize> next_type;

		typedef typename value_type::template arg<i> tag_type;
		typedef typename tag_type::type elem_type;

		// don't use BOOST_MULTI_INDEX_CONST_MEM_FUN at msvc, because this is msvc's bug
		//typedef 
		//	boost::multi_index::hashed_unique
		//	<
		//		boost::multi_index::tag< typename value_type::template arg<i> >,
		//		BOOST_MULTI_INDEX_CONST_MEM_FUN(
		//			value_type,
		//			const typename value_type::template arg<i>::type&,
		//			template arg_get< value_type::template arg<i> >)
		//	> now_cont_type;

		typedef 
			boost::multi_index::hashed_unique
			<
				boost::multi_index::tag< tag_type >,
				boost::multi_index::global_fun
				< 
					const value_type&, 
					const elem_type&, 
					&this_type::s_prv_get_elem<tag_type> 
				>
			> now_cont_type;

	public:
		// in gcc boo::mpl::push_front reuslt is boost::mpl::v_item
		typedef typename boost::mpl::push_front<typename next_type::type, now_cont_type>::type type;
	};

	template<size_type i, typename Nil>
	struct make_indexed_by<i, i, Nil>
	{
	public:
		//typedef boost::multi_index::indexed_by< boost::multi_index::sequenced<> > type;
		typedef boost::mpl::vector< boost::multi_index::sequenced<> > type;
	};

	typedef 
		safe_container::safe_multi_index_container
		<
			value_type,
			typename 
				container::cast_to_indexed_by // gcc mpl::push_front<>::result is boost::mpl::v_item
				<
					typename
						mplex::revert_to_vector
						<
							typename make_indexed_by<>::type
						>::type
				>::type,
			tpl_arg_mutex_type
		> midx_cont_type;

public:
	unique_mapping(void)
	{
	}

	unique_mapping(BOOST_RV_REF(this_type) right)
		: _cont(boost::move(right._cont))
	{
	}

	unique_mapping(const this_type& right)
		: _cont(right._cont)
	{
	}

	~unique_mapping(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right)
		{
			return *this;
		}

		copy_or_move_or_swap(_cont, boost::move(right._cont));

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_cont = right._cont;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		_cont.swap(right._cont);
	}

public:
	inline void clear(void)
	{
		_cont.clear();
	}

	inline std::size_t size(void) const
	{
		return _cont.size();
	}

	inline bool empty(void) const
	{
		return _cont.empty();
	}

	inline static std::size_t mapping_value_size(void)
	{
		return value_type::E_value_length;
	}

	inline bool append(BOOST_RV_REF(value_type) val)
	{
		return this_type::prv_append(boost::move(val));
	}

	inline bool append(const value_type& val)
	{
		return this_type::prv_append(val);
	}

	inline bool compulsory_append(BOOST_RV_REF(value_type) val)
	{
		return this_type::prv_compulsory_append(val);
	}

	inline bool compulsory_append(const value_type& val)
	{
		return prv_compulsory_append(val);
	}

	inline bool compulsory_exchange(value_type& val)
	{
		return prv_compulsory_exchange(val);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	bool append( BOOST_FWD_REF(Args)... args)
	{
		return this_type::prv_append(value_type(boost::forward<Args>(args)...));
	}

	template<typename ...Args> inline
	bool compulsory_append(BOOST_FWD_REF(Args)... args)
	{
		return this_type::prv_compulsory_append(value_type(boost::forward<Args>(args)...));
	}

private:
	template<typename std::size_t ...I, typename ...Args> inline 
	static void _prv_s_handler_exchanger_call_helper(value_type& tmp,
													::yggr::index_sequence<I...>,
													Args& ...args)
	{
		this_type::prv_s_handler_exchanger<typename value_type::template arg<I>...>(tmp, args...);
	}

public:
	template<typename ...Args> inline
	bool compulsory_exchange(Args&... args)
	{
		value_type tmp;
		this_type::_prv_s_handler_exchanger_call_helper(
			tmp, ::yggr::make_index_sequence<sizeof...(Args)>{}, args...);
		bool bret = this_type::prv_compulsory_exchange(tmp); 
		this_type::_prv_s_handler_exchanger_call_helper(
			tmp, ::yggr::make_index_sequence<sizeof...(Args)>{}, args...);
		return bret;
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool append(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::prv_append(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool compulsory_append(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::prv_compulsory_append(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool compulsory_exchange(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REF_PARAMS ) ) { \
			value_type tmp; \
			this_type::prv_s_handler_exchanger< \
					YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >( \
				tmp YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			bool bret = this_type::prv_compulsory_exchange(tmp); \
			this_type::prv_s_handler_exchanger< \
					YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >( \
				tmp YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return bret; }

#	define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) typename value_type::template arg< __n__ >
#	define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_PP_TEMPLATE_CUSTOM_TYPE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Tag> inline
	bool remove(const typename Tag::type& key)
	{
		return !!(_cont.template get<Tag>().erase(key));
	}

	template<size_type i> inline
	bool remove(const typename value_type::template arg<i>::type& key)
	{
		return !!(_cont.template get<i>().erase(key));
	}

	inline bool remove(const typename value_type::template arg<0>::type& key)
	{
		return !!(_cont.template get<0>().erase(key));
	}

	template<typename Tag> inline
	bool is_exists(const typename Tag::type& key) const
	{
		return _cont.template get<Tag>().find(key);
	}

	template<size_type i> inline
	bool is_exists(const typename value_type::template arg<i>::type& key) const
	{
		return _cont.template get<i>().find(key);
	}

	inline bool is_exists(const typename value_type::template arg<0>::type& key) const
	{
		return _cont.template get<0>().find(key);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename TagFind, typename ...TagMods> inline
	bool modify(const typename TagFind::type& find_key, 
				const typename TagMods::type&... mod_keys) 
	{
		typedef TagFind tag_find_type;
		value_type tmp;
		return 
			_cont.template get<tag_find_type>().find_and_modify(
				find_key,
				boost::bind(
					&this_type::prv_s_handler_modifier<TagMods...>,
					_1, boost::ref(tmp), boost::cref(mod_keys)...),
				boost::bind( &this_type::prv_s_handler_rollback, _1, boost::cref(tmp) ) ); 
	}

	template< size_type find, size_type ...mods> inline
	bool modify(const typename value_type::template arg<find>::type& find_key,
					const typename value_type::template arg<mods>::type&... mod_keys) 
	{
		typedef typename value_type::template arg<find> tag_find_type;
		value_type tmp;
		return 
			_cont.template get<tag_find_type>().find_and_modify(
				find_key,
				boost::bind( 
					&this_type::prv_s_handler_modifier< typename value_type::template arg<mods>... >,
					_1, boost::ref(tmp), boost::cref(mod_keys)... ), 
				boost::bind( &this_type::prv_s_handler_rollback, _1, boost::cref(tmp) ) );
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename TagFind \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TagMods)> inline \
		bool modify(const typename TagFind::type& find_key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_CREF_PARAMS )) { \
			typedef TagFind tag_find_type; \
			value_type tmp; \
			return _cont.template get<tag_find_type>().find_and_modify( \
						find_key, \
						boost::bind( \
							&this_type::prv_s_handler_modifier<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, TagMods)>, \
							_1, boost::ref(tmp) YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ), \
						boost::bind( \
							&this_type::prv_s_handler_rollback, _1, boost::cref(tmp) ) ); }


#	define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) ( boost::cref( BOOST_PP_CAT(__val__, __n__) ) )

#	define YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT(typename TagMods, __n__ )::type

#	define _YGGR_TMP_PP_TEP_VALUE() mod_key
#	define YGGR_PP_FOO_ARG_NAME() mod_key
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef _YGGR_TMP_PP_TEP_VALUE
#	undef YGGR_PP_FOO_ARG_TYPE
#	undef _YGGR_TMP_PP_TEP_SUB_OP


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< size_type find \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, size_type mod) > inline \
		bool modify(const typename value_type::template arg<find>::type& find_key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_CREF_PARAMS )) { \
			typedef typename value_type::template arg<find> tag_find_type; \
			value_type tmp; \
			return _cont.template get<tag_find_type>() \
					.find_and_modify( \
						find_key, \
						boost::bind( \
							&this_type::prv_s_handler_modifier< \
								YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >, \
							_1, boost::ref(tmp) YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ), \
						boost::bind( \
							&this_type::prv_s_handler_rollback, _1, boost::cref(tmp) ) ); }

#	define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) ( boost::cref(BOOST_PP_CAT(__val__, __n__) ) )

#	define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) typename value_type::template arg<BOOST_PP_CAT(mod, __n__)>
#	define YGGR_PP_FOO_ARG_TYPE(__n__) typename value_type::template arg<BOOST_PP_CAT(mod, __n__)>::type
#	define _YGGR_TMP_PP_TEP_VALUE() mod_key
#	define YGGR_PP_FOO_ARG_NAME() mod_key
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef _YGGR_TMP_PP_TEP_VALUE
#	undef YGGR_PP_FOO_ARG_TYPE
#	undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#	undef _YGGR_TMP_PP_TEP_SUB_OP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename TagFind, typename ...TagMods> inline 
	bool exchange(const typename TagFind::type& find_key, typename TagMods::type&... mod_keys) 
	{ 
		typedef TagFind tag_find_type; 
		return 
			_cont.template get<tag_find_type>().find_and_modify(
				find_key,
				boost::bind(&this_type::prv_s_handler_exchanger<TagMods...>, _1, boost::ref(mod_keys)... ),
				boost::bind(&this_type::prv_s_handler_exchanger<TagMods...>, _1, boost::ref(mod_keys)... ) ); 
	}

	template< size_type find, size_type ...mods > inline 
	bool exchange(const typename value_type::template arg<find>::type& find_key,
					typename value_type::template arg<mods>::type&... mod_keys) 
	{ 
		typedef typename value_type::template arg<find> tag_find_type; 
		return 
			_cont.template get<tag_find_type>()
				.find_and_modify(
					find_key,
					boost::bind(
						&this_type::prv_s_handler_exchanger<typename value_type::template arg<mods>... >, 
						_1, boost::ref(mod_keys)... ),
					boost::bind(
						&this_type::prv_s_handler_exchanger<typename value_type::template arg<mods>... >, 
						_1, boost::ref(mod_keys)... ) ); 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename TagFind \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TagMods)> inline \
		bool exchange(const typename TagFind::type& find_key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_REF_PARAMS )) { \
			typedef TagFind tag_find_type; \
			return _cont.template get<tag_find_type>().find_and_modify( \
						find_key, \
						boost::bind( \
							&this_type::prv_s_handler_exchanger<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, TagMods)>, \
							_1 YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ), \
						boost::bind( \
							&this_type::prv_s_handler_exchanger<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, TagMods)>, \
							_1 YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ) ); }


#	define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) ( boost::ref( BOOST_PP_CAT(__val__, __n__) ) )

#	define YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT(typename TagMods, __n__ )::type

#	define _YGGR_TMP_PP_TEP_VALUE() mod_key
#	define YGGR_PP_FOO_ARG_NAME() mod_key
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef _YGGR_TMP_PP_TEP_VALUE
#	undef YGGR_PP_FOO_ARG_TYPE
#	undef _YGGR_TMP_PP_TEP_SUB_OP


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< size_type find \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, size_type mod) > inline \
		bool exchange(const typename value_type::template arg<find>::type& find_key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_REF_PARAMS )) { \
			typedef typename value_type::template arg<find> tag_find_type; \
			return _cont.template get<tag_find_type>() \
					.find_and_modify( \
						find_key, \
						boost::bind( \
							&this_type::prv_s_handler_exchanger< \
								YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >, \
							_1 YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ), \
						boost::bind( \
							&this_type::prv_s_handler_exchanger< \
									YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >, \
							_1 YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ) ); }

#	define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) ( boost::ref(BOOST_PP_CAT(__val__, __n__) ) )

#	define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) typename value_type::template arg<BOOST_PP_CAT(mod, __n__)>
#	define YGGR_PP_FOO_ARG_TYPE( __n__ ) typename value_type::template arg<BOOST_PP_CAT(mod, __n__)>::type
#	define _YGGR_TMP_PP_TEP_VALUE() mod_key
#	define YGGR_PP_FOO_ARG_NAME() mod_key
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef _YGGR_TMP_PP_TEP_VALUE
#	undef YGGR_PP_FOO_ARG_TYPE
#	undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#	undef _YGGR_TMP_PP_TEP_SUB_OP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename TagSrc, typename TagDst> inline
	std::pair<typename TagDst::type, bool>
		mapping_value(const typename TagSrc::type& key) const
	{
		typedef TagSrc tag_src_type;
		typedef TagDst tag_dst_type;

		return 
			_cont.template get<tag_src_type>().find(
				key, 
				boost::bind(
					&this_type::prv_s_handler_mapping_value<tag_src_type, tag_dst_type>, _1, _2 ));
	}

	template<size_type src, size_type dst> inline
	std::pair<typename value_type::template arg<dst>::type, bool>
		mapping_value(const typename value_type::template arg<src>::type& key) const
	{
		typedef typename value_type::template arg<src> tag_src_type;
		typedef typename value_type::template arg<dst> tag_dst_type;

		return 
			_cont.template get<tag_src_type>().find(
				key, 
				boost::bind(
					&this_type::prv_s_handler_mapping_value<tag_src_type, tag_dst_type>, _1, _2 ));
	}

	template<typename TagSrc, typename TagDst, typename Container> inline
	Container mapping_other_values(const typename TagSrc::type& key) const
	{
		typedef TagSrc tag_src_type;
		typedef TagDst tag_dst_type;
		typedef Container out_cont_type;
		
		out_cont_type out;

		this_type::mapping_other_values<tag_src_type, tag_dst_type, out_cont_type>(key, out);

		return out;
	}

	template<size_type src, size_type dst, typename Container> inline
	Container mapping_other_values(const typename value_type::template arg<src>::type& key) const
	{
		typedef typename value_type::template arg<src> tag_src_type;
		typedef typename value_type::template arg<dst> tag_dst_type;
		typedef Container out_cont_type;

		out_cont_type out;

		this_type::mapping_other_values<tag_src_type, tag_dst_type, out_cont_type>(key, out);

		return out;
	}

	template<typename TagSrc, typename TagDst, typename Container> 
	Container& mapping_other_values(const typename TagSrc::type& key, Container& out) const
	{
		typedef TagSrc tag_src_type;
		typedef TagDst tag_dst_type;
		typedef Container out_cont_type;
		typedef typename boost::range_value<out_cont_type>::type out_value_type;
		typedef typename tag_dst_type::type ret_val_type;

		BOOST_MPL_ASSERT((boost::is_same<out_value_type, ret_val_type>));

		typedef typename boost::multi_index::nth_index<typename midx_cont_type::base_type, tag_src_type::index>::type cont_type;
		typedef typename cont_type::const_iterator cont_iter_type;

		typename midx_cont_type::base_type mcont;
		_cont.copy_to_base(mcont);

		cont_type& cont = mcont.template get<tag_src_type::index>();
		cont.erase(key);

		for(cont_iter_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i)
		{
			iterator_ex::inserter(out, boost::end(out)) = (*i).template get<tag_dst_type::index>();
		}

		return out;
	}

	template<size_type src, size_type dst, typename Container> inline
	Container& mapping_other_values(const typename value_type::template arg<src>::type& key, Container& out) const
	{
		typedef typename value_type::template arg<src> tag_src_type;
		typedef typename value_type::template arg<dst> tag_dst_type;
		typedef Container out_cont_type;

		return 
			this_type::mapping_other_values< tag_src_type, 
												tag_dst_type, 
												out_cont_type >(key, out);
	}

private:

	inline bool prv_append(BOOST_RV_REF(value_type) val)
	{
		return _cont.template get<static_cast<int>(value_type::E_length)>()
					.push_back(boost::move(val));
	}

	inline bool prv_append(const value_type& val)
	{
		return _cont.template get<static_cast<int>(value_type::E_length)>()
					.push_back(val);
	}

	inline bool prv_compulsory_append(const value_type& val)
	{
		return _cont.template get<static_cast<int>(value_type::E_length)>()
					.push_back(val, boost::bind(&this_type::prv_handler_compulsory_append,
												this, _1, _2, boost::cref(val)));
	}

	inline bool prv_compulsory_exchange(value_type& val)
	{
		return _cont.template get<static_cast<int>(value_type::E_length)>()
					.push_back(val, boost::bind(&this_type::prv_handler_compulsory_exchange,
												this, _1, _2, boost::ref(val)));
	}


	inline bool prv_handler_compulsory_append(typename midx_cont_type::base_type& base,
												boost::fusion::pair
												<
													typename 
														safe_container::nth_index
														<
															midx_cont_type, 
															static_cast<int>(value_type::E_length)
														>::type::index_id_type,
													std::pair
													<
														typename 
															safe_container::nth_index
															<
																midx_cont_type, 
																static_cast<int>(value_type::E_length)
															>::type::iterator,
														bool
													>
												> rst, 
												const value_type& val)
	{
		typedef typename midx_cont_type::base_type midx_cont;
		typedef typename 
			boost::multi_index::nth_index
			<
				midx_cont, 
				static_cast<int>(value_type::E_length)
			>::type cont_type;

		if(rst.second.second)
		{
			return true;
		}
		else
		{
			cont_type& cont = base.template get<static_cast<int>(value_type::E_length)>();
			
			value_type tmp;
			return 
				rst.second.first != cont.end()
					&& cont.modify(
						rst.second.first,
						boost::bind(
							&this_type::prv_s_handler_modifier_of_value, _1, boost::cref(val), boost::ref(tmp)),
						boost::bind(
							&this_type::prv_s_handler_rollback, _1, boost::cref(tmp)));
			
		}
	}

	inline bool prv_handler_compulsory_exchange(typename midx_cont_type::base_type& base,
													boost::fusion::pair
													<
														typename 
															safe_container::nth_index
															<
																midx_cont_type, 
																static_cast<int>(value_type::E_length)
															>::type::index_id_type,
														std::pair
														<
															typename 
																safe_container::nth_index
																<
																	midx_cont_type, 
																	static_cast<int>(value_type::E_length)
																>::type::iterator,
															bool
														>
													> rst, 
													value_type& val)
	{
		typedef typename midx_cont_type::base_type midx_cont;
		typedef typename 
			boost::multi_index::nth_index
			<
				midx_cont, 
				static_cast<int>(value_type::E_length)
			>::type cont_type;

		if(rst.second.second)
		{
			return true;
		}
		else
		{
			cont_type& cont = base.template get<static_cast<int>(value_type::E_length)>();
			
			return 
				rst.second.first != cont.end()
				&& cont.modify(
					rst.second.first,
					boost::bind(&this_type::prv_s_handler_exchanger_of_value, _1, boost::ref(val)),
					boost::bind(&this_type::prv_s_handler_exchanger_of_value, _1, boost::ref(val)) );
			
		}
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...TagMods> inline
	static void prv_s_handler_modifier(value_type& val, value_type& tmp, 
										const typename TagMods::type&... mods) 
	{ 
		tmp = val; 
		return support::return_void(0, (val.template get<TagMods::index>() = mods)...);
	}
#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TagMods) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static void prv_s_handler_modifier(value_type& _YGGR_TMP_PP_TEP_VALUE(), \
											value_type& tmp \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_CREF_PARAMS ) ) { \
			tmp = _YGGR_TMP_PP_TEP_VALUE(); \
			YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER(__n__, YGGR_PP_SYMBOL_SEMICOLON); }

#	define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) \
		( __val__.template get<BOOST_PP_CAT(TagMods, __n__)::index>() = BOOST_PP_CAT(YGGR_PP_FOO_ARG_NAME(), __n__ ) )

#	define YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT(typename TagMods, __n__)::type
#	define _YGGR_TMP_PP_TEP_VALUE() val
#	define YGGR_PP_FOO_ARG_NAME() mod
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef _YGGR_TMP_PP_TEP_VALUE
#	undef YGGR_PP_FOO_ARG_TYPE
#	undef _YGGR_TMP_PP_TEP_SUB_OP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...TagMods> inline
	static void prv_s_handler_exchanger(value_type& val, typename TagMods::type&... mods )
	{
		return support::return_void((yggr::swap( val.template get<TagMods::index>(), mods ), 0)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TagMods) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static void prv_s_handler_exchanger(value_type& _YGGR_TMP_PP_TEP_VALUE() \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_REF_PARAMS ) ) { \
			YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER(__n__, YGGR_PP_SYMBOL_SEMICOLON); }

#	define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) \
		( yggr::swap( __val__.template get<BOOST_PP_CAT(TagMods, __n__)::index>(), BOOST_PP_CAT(YGGR_PP_FOO_ARG_NAME(), __n__ ) ) )

#	define YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT(typename TagMods, __n__)::type
#	define _YGGR_TMP_PP_TEP_VALUE() val
#	define YGGR_PP_FOO_ARG_NAME() mod
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef _YGGR_TMP_PP_TEP_VALUE
#	undef YGGR_PP_FOO_ARG_TYPE
#	undef _YGGR_TMP_PP_TEP_SUB_OP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline static void prv_s_handler_modifier_of_value(value_type& src, const value_type& mod, value_type& bak)
	{
		bak = src;
		src = mod;
	}

	inline static void prv_s_handler_exchanger_of_value(value_type& src, value_type& mod)
	{
		yggr::swap(src, mod);
	}

	inline static void prv_s_handler_rollback(value_type& val, const value_type& tmp)
	{
		val = tmp;
	}

	template<typename TagSrc, typename TagDst> inline
	static std::pair<typename TagDst::type, bool>
		prv_s_handler_mapping_value(const typename midx_cont_type::base_type& base,
									boost::fusion::pair
									<
										typename 
											safe_container::nth_index
											<
												midx_cont_type, 
												TagSrc::index
											>::type::index_id_type,
										typename 
											safe_container::nth_index
											<
												midx_cont_type, 
												TagSrc::index
											>::type::const_iterator
									> rst)
	{
		typedef TagSrc tag_src_type;
		typedef TagDst tag_dst_type;
		typedef typename tag_dst_type::type ret_val_type;
		typedef std::pair<ret_val_type, bool> ret_type;

		return
			rst.second == base.template get<static_cast<int>(tag_src_type::index)>().end()?
				ret_type(ret_val_type(), false) 
				: ret_type((*rst.second).template get<tag_dst_type::index>(), true);
	}

public:
	template<size_type i> inline
	static typename value_type::template arg<i> create_tag_object(void)
	{
		return typename value_type::template arg<i>();
	}

private:
	midx_cont_type _cont;
};

} // namespace mapping
} // namespace yggr

#undef _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

namespace yggr
{
namespace mapping
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_PARAMS_LEN(), typename Val)
			YGGR_PP_SYMBOL_IF(YGGR_PP_TEMPLATE_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			typename Mutex> inline
void swap(unique_mapping<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_PARAMS_LEN(), Val)
							YGGR_PP_SYMBOL_IF(YGGR_PP_TEMPLATE_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
							Mutex>& l,
			unique_mapping<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_PARAMS_LEN(), Val)
						YGGR_PP_SYMBOL_IF(YGGR_PP_TEMPLATE_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
						Mutex>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace mapping
} // namespace yggr

namespace std
{
	using ::yggr::mapping::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::mapping::swap_support::swap;
} // namespace boost

#endif // __YGGR_MAPPING_UNIQUE_MAPPING_HPP__
