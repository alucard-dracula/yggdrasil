// incomplete t_move_patch_macro.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_INCOMPLETE_T_MOVE_PATCH_MACRO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_INCOMPLETE_T_MOVE_PATCH_MACRO_HPP__

#include <yggr/move/move.hpp>

// gcc needed fix invalid use of incomplete type 'struct xxx'

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#define YGGR_PP_NSQL_DBS_INCOMPLETE_T_MOVE_HELPER( __org__, __shadow__) \
	namespace boost { namespace move_detail { \
	template<> \
	class is_convertible< __org__, ::boost::rv< __org__ >& > { \
	public: \
	   static const bool value = true; }; } \
	\
	template<> \
	class BOOST_RV_ATTRIBUTE_MAY_ALIAS rv< __org__ > \
		: public __shadow__ { \
	private: \
		typedef __org__ org_type; \
		typedef __shadow__ base_type; \
	public: \
		inline operator org_type&(void) { \
			return reinterpret_cast< org_type& >(static_cast< base_type& >(*this)); } \
		\
		inline operator const org_type&(void) const { \
			return reinterpret_cast< const org_type& >(static_cast< const base_type& >(*this)); } \
	private: \
		rv(); \
		~rv() throw(); \
		rv(rv const&); \
		void operator=(rv const&); }; }

#else

#define YGGR_PP_NSQL_DBS_INCOMPLETE_T_MOVE_HELPER( __org__, __shadow__)

#endif // #if !defined(_MSC_VER) && defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_INCOMPLETE_T_MOVE_PATCH_MACRO_HPP__
