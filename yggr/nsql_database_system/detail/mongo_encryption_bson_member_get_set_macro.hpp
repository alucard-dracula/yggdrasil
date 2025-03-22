// mongo_encryption_bson_member_get_set_macro.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_MONGO_ENCRYPTION_BSON_MEMBER_GET_SET_MACRO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_MONGO_ENCRYPTION_BSON_MEMBER_GET_SET_MACRO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <bson.h>

#define YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET( __base_var_name__ ) \
	protected: \
		template<typename Bson> inline \
		typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type \
			BOOST_PP_CAT(pro_get_var_, __base_var_name__)(void) const { \
			typedef Bson now_bson_type; \
			return reinterpret_cast<const now_bson_type&>(BOOST_PP_CAT(_, __base_var_name__ )); } \
		\
		inline void BOOST_PP_CAT(pro_set_var_, __base_var_name__)(const bson_t&  BOOST_PP_CAT(arg_, __base_var_name__)) { \
			BOOST_PP_CAT(_, __base_var_name__) = (BOOST_PP_CAT(arg_, __base_var_name__ )); \
			base_type::__base_var_name__ = \
				(BOOST_PP_CAT(_, __base_var_name__)).empty()? \
					0 : (BOOST_PP_CAT(_, __base_var_name__)).org_pointer(); } \
	\
	public: \
		inline bson_member_modify_helper_type BOOST_PP_CAT(var_, __base_var_name__)(void) { \
			return \
				bson_member_modify_helper_type( \
					*this, (BOOST_PP_CAT(_, __base_var_name__ )), \
					&this_type::BOOST_PP_CAT(pro_set_var_, __base_var_name__ ), \
					&this_type::BOOST_PP_CAT(pro_get_var_, __base_var_name__ )<bson_type>); } \
		\
		template<typename Bson> inline \
		typename \
			boost::enable_if< \
				boost::is_base_of<bson_t, Bson>, \
				utility::member_var_modify_helper< \
					this_type, \
					bson_type, \
					bson_t, \
					const Bson& > >::type \
			BOOST_PP_CAT( var_, __base_var_name__ )(void) { \
			typedef Bson now_bson_type; \
			typedef \
				utility::member_var_modify_helper< \
					this_type, \
					bson_type, \
					bson_t, \
					const now_bson_type& \
				> now_bson_member_modify_helper_type; \
			\
			return \
				now_bson_member_modify_helper_type( \
					*this, BOOST_PP_CAT(_, __base_var_name__ ), \
					&this_type::BOOST_PP_CAT(pro_set_var_, __base_var_name__ ), \
					&this_type::BOOST_PP_CAT(pro_get_var_, __base_var_name__ )<now_bson_type>); } \
		\
		inline const bson_type& BOOST_PP_CAT(var_, __base_var_name__)(void) const { \
			return this_type::BOOST_PP_CAT(pro_get_var_, __base_var_name__)<bson_type>(); } \
		\
		template<typename Bson> inline \
		typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type \
			BOOST_PP_CAT(var_, __base_var_name__ )(void) const { \
			typedef Bson now_bson_type; \
			return this_type::BOOST_PP_CAT(pro_get_var_, __base_var_name__ )<now_bson_type>(); }

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{
} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_MONGO_ENCRYPTION_BSON_MEMBER_GET_SET_MACRO_HPP__
