//unique_mapping.hpp

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

#ifndef __YGGR_MAPPING_UNIQUE_MAPPING_HPP__
#define __YGGR_MAPPING_UNIQUE_MAPPING_HPP__

#include <boost/multi_index/key_extractors.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/ppex/concat.hpp>
#include <yggr/ppex/foo_params.hpp>

// _YGGR_TMP_PP_TEP_SUB_OP(__n__, __val__, )
// _YGGR_TMP_PP_TEP_VALUE ()
#define _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_TEP_SUB_OP( __n__, \
													YGGR_PP_OPEN( _YGGR_TMP_PP_TEP_VALUE  ) )

namespace yggr
{
namespace mapping
{

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() boost::tuples::null_type
template<
			YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN, typename Val )
			YGGR_PP_SYMBOL_IF(YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN, YGGR_PP_SYMBOL_COMMA)
			typename Mutex = boost::mutex
		>
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
class unique_mapping
{
private:
	typedef Mutex mutex_type;

public:
	typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN, Val) > value_type;

private:
	typedef unique_mapping this_type;

private:

	template<size_type i = 0,
				size_type isize = value_type::E_length>
	struct make_indexed_by
	{
		typedef make_indexed_by<i + 1, isize> next_type;
		typedef boost::multi_index::hashed_unique
				<
					boost::multi_index::tag< typename value_type::template arg<i> >,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(value_type,
                                                        const typename value_type::template arg<i>::type&,
                                                        template get<i>)
				> now_cont_type;
	public:
		typedef typename boost::mpl::push_front<typename next_type::type, now_cont_type>::type type;
	};

	template<size_type i>
	struct make_indexed_by<i, i>
	{
	public:
		typedef boost::multi_index::indexed_by< boost::multi_index::sequenced<> > type;
	};

	typedef safe_container::safe_multi_index_container
			<
				value_type,
				typename make_indexed_by<>::type,
				mutex_type
			> midx_cont_type;

public:
	unique_mapping(void)
	{
	}

	~unique_mapping(void)
	{
	}

	void clear(void)
	{
		_cont.clear();
	}

	std::size_t size(void) const
	{
		return _cont.size();
	}

	inline std::size_t mapping_value_size(void) const
	{
		return value_type::E_value_length;
	}

	inline static std::size_t s_mapping_value_size(void)
	{
		return value_type::E_value_length;
	}

	bool append(const value_type& val)
	{
		return this_type::prv_append(val);
	}

	bool must_append(const value_type& val)
	{
		return prv_must_append(val);
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	bool append(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		return this_type::prv_append(value_type(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
	\
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	bool must_append(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		return this_type::prv_must_append(value_type(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME


	template<typename Tag>
	bool remove(const typename Tag::type& key)
	{
		return _cont
					.template use_method<Tag>(
                        YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, Tag)
							.erase(key));
	}

	template<size_type i>
	bool remove(const typename value_type::template arg<i>::type& key)
	{
		return _cont
					.template use_method<i>(
						YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, i)
							.erase(key));
	}

	bool remove(const typename value_type::template arg<0>::type& key)
	{
		return _cont
					.template use_method<0>(
						YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, 0)
							.erase(key));
	}

	//template<typename TagFind, typename TagMod>
	//bool modify(const typename TagFind::type& find_key, const typename TagMod::type& mod_key)
	//{
	//	typedef TagFind tag_find_type;
	//	typedef TagMod tag_mod_type;

	//	value_type tmp;
	//	return _cont
	//				.use_method<tag_find_type>(
	//					YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, tag_find_type)
	//						.modify(find_key,
	//									boost::bind(&this_type::handler_modifier<tag_mod_type>,
	//													this,
	//													_1,
	//													boost::cref(mod_key),
	//													boost::ref(tmp)
	//												),
	//									boost::bind(&this_type::handler_rollback,
	//													this,
	//													_1,
	//													boost::cref(tmp)
	//												)
	//								)
	//				);
	//}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename TagFind \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TagMod)> \
	bool modify(const typename TagFind::type& find_key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_CREF_PARAMS )) { \
		typedef TagFind tag_find_type; \
		value_type tmp; \
		return _cont \
					.template use_method<tag_find_type>( \
						YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, tag_find_type) \
							.modify(find_key, \
										boost::bind(&this_type::template handler_modifier<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, TagMod)>, \
														this, \
														_1, \
														YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ), \
														boost::ref(tmp) ), \
										boost::bind(&this_type::handler_rollback, \
														this, \
														_1, \
														boost::cref(tmp) ) ) ); }


#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) \
		( boost::cref( BOOST_PP_CAT(__val__, __n__) ) )

#define YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT(typename TagMod, __n__ )::type

#define _YGGR_TMP_PP_TEP_VALUE() mod_key
#define YGGR_PP_FOO_ARG_NAME() mod_key
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef YGGR_PP_FOO_ARG_NAME
#undef _YGGR_TMP_PP_TEP_VALUE
#undef YGGR_PP_FOO_ARG_TYPE
#undef _YGGR_TMP_PP_TEP_SUB_OP


	//template<size_type find, size_type mod>
	//bool modify(const typename value_type::arg<find>::type& find_key,
	//				const typename value_type::arg<mod>::type& mod_key)
	//{
	//	typedef value_type::arg<find> tag_find_type;
	//	typedef value_type::arg<mod> tag_mod_type;

	//	value_type tmp;
	//	return _cont
	//				.use_method<tag_find_type>(
	//					YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, tag_find_type)
	//						.modify(find_key,
	//									boost::bind(&this_type::handler_modifier<tag_mod_type>,
	//													this,
	//													_1,
	//													boost::cref(mod_key),
	//													boost::ref(tmp)
	//												),
	//									boost::bind(&this_type::handler_rollback,
	//													this,
	//													_1,
	//													boost::cref(tmp)
	//												)
	//								)
	//				);
	//}

	/*
	#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< size_type find \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, size_type mod) > \
	bool modify(const typename value_type::arg<find>::type& find_key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_CREF_PARAMS )) { \
		typedef value_type::arg<find> tag_find_type; \
		value_type tmp; \
		return _cont \
					.use_method<tag_find_type>( \
						YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, tag_find_type) \
							.modify(find_key, \
										boost::bind(&this_type::handler_modifier< \
															YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, \
																YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >, \
														this, \
														_1, \
														YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ), \
														boost::ref(tmp) ), \
										boost::bind(&this_type::handler_rollback, \
														this, \
														_1, \
														boost::cref(tmp) ) ) ); }*/

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< size_type find \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, size_type mod) > \
	bool modify(const typename value_type::template arg<find>::type& find_key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_REAL_CREF_PARAMS )) { \
		typedef typename value_type::template arg<find> tag_find_type; \
		value_type tmp; \
		return _cont \
					.template use_method<tag_find_type>( \
						YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type, tag_find_type) \
							.modify(find_key, \
										boost::bind(&this_type::template handler_modifier< \
															YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, \
																YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) >, \
														this, \
														_1, \
														YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ), \
														boost::ref(tmp) ), \
										boost::bind(&this_type::handler_rollback, \
														this, \
														_1, \
														boost::cref(tmp) ) ) ); }

#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) \
		( boost::cref(BOOST_PP_CAT(__val__, __n__) ) )

#define YGGR_PP_TEMPLATE_CUSTOM_TYPE(__n__) typename value_type::template arg<BOOST_PP_CAT(mod, __n__)>
#define YGGR_PP_FOO_ARG_TYPE(__n__) typename value_type::template arg<BOOST_PP_CAT(mod, __n__)>::type
#define _YGGR_TMP_PP_TEP_VALUE() mod_key
#define YGGR_PP_FOO_ARG_NAME() mod_key
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef YGGR_PP_FOO_ARG_NAME
#undef _YGGR_TMP_PP_TEP_VALUE
#undef YGGR_PP_FOO_ARG_TYPE
#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#undef _YGGR_TMP_PP_TEP_SUB_OP

	template<typename TagSrc, typename TagDst>
	std::pair<typename TagDst::type, bool>
		mapping_value(const typename TagSrc::type& key) const
	{
		typedef TagSrc tag_src_type;
		typedef TagDst tag_dst_type;

		return _cont.use_handler(
						boost::bind(
										&this_type::handler_mapping_value<tag_src_type, tag_dst_type>,
										this, _1, boost::cref(key)
									)
								);
	}

	template<size_type src, size_type dst>
	std::pair<typename value_type::template arg<dst>::type, bool>
		mapping_value(const typename value_type::template arg<src>::type& key) const
	{
		typedef typename value_type::template arg<src> tag_src_type;
		typedef typename value_type::template arg<dst> tag_dst_type;

		return _cont.use_handler(
						boost::bind(
										&this_type::template handler_mapping_value<tag_src_type, tag_dst_type>,
										this, _1, boost::cref(key)
									)
								);
	}

private:

	bool prv_append(const value_type& val)
	{
		return _cont
				.template use_method<
					value_type::E_length>(
						YGGR_PP_MIDX_MAKE_METHOD(midx_cont_type,
													value_type::E_length).push_back(val));
	}

	bool prv_must_append(const value_type& val)
	{
		return _cont.use_handler(boost::bind(&this_type::handler_prv_must_append,
												this, _1, boost::cref(val)));
	}

	bool handler_prv_must_append(typename midx_cont_type::base_type& base,
									const value_type& val)
	{
		typedef typename midx_cont_type::base_type midx_cont;
		typedef typename boost::multi_index::nth_index<midx_cont, 0>::type cont_type;
		typedef typename cont_type::iterator cont_iter_type;

		cont_type& cont = base.template get<0>();
		cont_iter_type iter = cont.find(val.template get<0>());

		if(iter == cont.end())
		{
			return cont.insert(val).second;
		}

		value_type tmp;
		return cont.modify(iter,
							boost::bind(&this_type::handler_modifier_of_value,
											this, _1, boost::cref(val), boost::ref(tmp)),
							boost::bind(&this_type::handler_rollback,
											this, _1, boost::cref(tmp)));
	}

	//template<typename TagMod>
	//void handler_modifier(value_type& val, const typename TagMod::type& mod, value_type& tmp)
	//{
	//	typedef TagMod tag_mod_type;
	//	tmp = val;
	//	val.get<tag_mod_type::index>() =  mod;
	//}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TagMod) > \
	void handler_modifier(value_type& _YGGR_TMP_PP_TEP_VALUE() \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CUSTOM_REAL_CREF_PARAMS ), value_type& tmp) { \
		tmp = val; \
		YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER(__n__, YGGR_PP_SYMBOL_SEMICOLON); }

#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __val__) \
	( __val__.template get<BOOST_PP_CAT(TagMod, __n__)::index>() = BOOST_PP_CAT(YGGR_PP_FOO_ARG_NAME(), __n__ ) )

#define YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT(typename TagMod, __n__)::type
#define _YGGR_TMP_PP_TEP_VALUE() val
#define YGGR_PP_FOO_ARG_NAME() mod
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef YGGR_PP_FOO_ARG_NAME
#undef _YGGR_TMP_PP_TEP_VALUE
#undef YGGR_PP_FOO_ARG_TYPE
#undef _YGGR_TMP_PP_TEP_SUB_OP

	void handler_modifier_of_value(value_type& src, const value_type& mod, value_type& bak)
	{
		bak = src;
		src = mod;
	}

	void handler_rollback(value_type& val, const value_type& tmp)
	{
		val = tmp;
	}

	template<typename TagSrc, typename TagDst>
	std::pair<typename TagDst::type, bool>
			handler_mapping_value(const typename midx_cont_type::base_type& base,
									const typename TagSrc::type& key) const
	{
		typedef TagSrc tag_src_type;
		typedef TagDst tag_dst_type;
		typedef typename tag_dst_type::type ret_val_type;
		typedef std::pair<ret_val_type, bool> ret_type;
		typedef typename boost::multi_index::index<midx_cont_type, tag_src_type>::type cont_type;
		typedef typename cont_type::const_iterator cont_iter_type;

		const cont_type& cont = base.template get<tag_src_type>();
		cont_iter_type iter = cont.find(key);
		if(iter == cont.end())
		{
			return ret_type(ret_val_type(), false);
		}

		return ret_type((*iter).template get<tag_dst_type::index>(), true);
	}

public:
	template<size_type i>
	static typename value_type::template arg<i> s_create_tag_object(void)
	{
		return typename value_type::template arg<i>();
	}

private:
	midx_cont_type _cont;
};

} // namespace mapping
} // namespace yggr

#undef _YGGR_TMP_PP_MAPPING_MODIFY_ARGS_OP

#endif // __YGGR_MAPPING_UNIQUE_MAPPING_HPP__
