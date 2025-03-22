//log_file_mgr.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_FILE_MGR_HPP__
#define __YGGR_LOG_LOGOP_LOG_FILE_MGR_HPP__


#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/tuple/tuple.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/mapped_value.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/thread/mutex.hpp>


namespace yggr
{
namespace log
{
namespace log_op
{
template<typename File>
class log_file_mgr
{
public:
	typedef File log_file_type;
	typedef typename log_file_type::path_string_type path_string_type;
	typedef typename log_file_type::inner_string_type inner_string_type;

private:
	typedef container::vector<path_string_type> path_string_vt_type;
	typedef ::yggr::shared_ptr<log_file_type> log_file_ptr_type;

	typedef safe_container::safe_unordered_map<path_string_type, log_file_ptr_type> map_type;

public:
	typedef path_string_vt_type init_type;

private:
	typedef log_file_mgr this_type;

public:
	log_file_mgr(void)
	{
		this_type::prv_create_log_files(path_string_type(log_file_type::unknown_log_name()));
	}

	template<typename Char>
	log_file_mgr(const Char* fname)
	{
		this_type::prv_create_log_files(fname);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	log_file_mgr(const Basic_String<Char, Traits, Alloc>& fname)
	{
		this_type::prv_create_log_files(fname);
	}

	template<typename Container>
	log_file_mgr(const Container& cont)
	{
		this_type::prv_create_log_files(cont);
	}

	template<typename Container, typename Handler>
	log_file_mgr(const Container& cont, const Handler& handler)
	{
		this_type::prv_create_log_files(cont, handler);
	}

	log_file_mgr(const this_type& right)
		: _map(right._map)
	{
	}

	~log_file_mgr(void)
	{
		_map.clear();
	}

public:

	// bool append(name, ...)

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Char, typename ...Args> inline
	bool append(const Char* fname, BOOST_FWD_REF(Args)... args)
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);
		log_file_ptr_type ptr;
		_map.get_value(str_fname, ptr);
		return ptr && ptr->append(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Char \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool append( const Char* fname \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			path_string_type str_fname; \
			charset::string_converter::s_conv(str_fname, fname); \
			log_file_ptr_type ptr; \
			_map.get_value(str_fname, ptr); \
			return ptr && ptr->append(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String,
				typename ...Args > inline
	bool append( const Basic_String<Char, Traits, Alloc>& fname, BOOST_FWD_REF(Args)... args)
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);
		log_file_ptr_type ptr;
		_map.get_value(str_fname, ptr);
		return ptr && ptr->append(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Char, typename Traits, typename Alloc, \
					template<typename _Char, typename _Traits, typename _Alloc> class Basic_String \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool append( const Basic_String<Char, Traits, Alloc>& fname \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			path_string_type str_fname; \
			charset::string_converter::s_conv(str_fname, fname); \
			log_file_ptr_type ptr; \
			_map.get_value(str_fname, ptr); \
			return ptr && ptr->append(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ));}

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	bool append(const path_string_type& fname, BOOST_FWD_REF(Args)... args)
	{
		log_file_ptr_type ptr;
		_map.get_value(fname, ptr);
		return ptr && ptr->append(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool append( const path_string_type& fname \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			log_file_ptr_type ptr; \
			_map.get_value(fname, ptr); \
			return ptr && ptr->append(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// append_all(...)
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args >
	bool append_all( const Args& ...args )
	{
		typename map_type::base_type tmp(_map.load());
		for(typename map_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->append(args...);
			}
		}
		return !_map.empty();
	}

#else

// YGGR_TMP_PP_ARG_CREF(__n__, __val__)
#	define YGGR_TMP_PP_ARGS_CREF( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			YGGR_TMP_PP_ARG_CREF( __n__, YGGR_PP_OPEN( YGGR_PP_FOO_ARG_NAME  ) )

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		bool append_all( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
			typename map_type::base_type tmp(_map.load()); \
			for(typename map_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i) { \
				if(i->second) { i->second->append(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } } \
			return !_map.empty(); }


#	define YGGR_TMP_PP_ARG_CREF( __n__, __val__ ) boost::cref( BOOST_PP_CAT(__val__, __n__ ) )
#	define YGGR_PP_FOO_CUSTOM_OP() YGGR_TMP_PP_ARGS_CREF
#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_ARG_CREF
#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_TMP_PP_ARGS_CREF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// bool clear(name, ...)
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Char, typename ...Args > inline
	bool clear(const Char* fname, BOOST_FWD_REF(Args)... args)
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);
		log_file_ptr_type ptr;
		_map.get_value(str_fname, ptr);
		return ptr && ptr->clear(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Char \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool clear(const Char* fname \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			path_string_type str_fname; \
			charset::string_converter::s_conv(str_fname, fname); \
			log_file_ptr_type ptr; \
			_map.get_value(str_fname, ptr); \
			return ptr && ptr->clear(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String,
				typename ...Args > inline
	bool clear(const Basic_String<Char, Traits, Alloc>& fname, BOOST_FWD_REF(Args)... args)
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);
		log_file_ptr_type ptr;
		_map.get_value(str_fname, ptr);
		return ptr && ptr->clear(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Char, typename Traits, typename Alloc, \
					template<typename _Char, typename _Traits, typename _Alloc> class Basic_String \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool clear(const Basic_String<Char, Traits, Alloc>& fname \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			path_string_type str_fname; \
			charset::string_converter::s_conv(str_fname, fname); \
			log_file_ptr_type ptr; \
			_map.get_value(str_fname, ptr); \
			return ptr && ptr->clear(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args > inline
	bool clear(const path_string_type& fname, BOOST_FWD_REF(Args)... args)
	{
		log_file_ptr_type ptr;
		_map.get_value(fname, ptr);
		return ptr && ptr->clear(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool clear(const path_string_type& fname \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			log_file_ptr_type ptr; \
			_map.get_value(fname, ptr); \
			return ptr && ptr->clear(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


// clear_all(...)
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args >
	bool clear_all(const Args& ...args)
	{
		typename map_type::base_type tmp(_map.load());
		for(typename map_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->clear(boost::cref(args)...);
			}
		}
		return true;
	}

#else

// YGGR_TMP_PP_ARG_CREF(__n__, __val__)
#	define YGGR_TMP_PP_ARGS_CREF( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			YGGR_TMP_PP_ARG_CREF( __n__, YGGR_PP_OPEN( YGGR_PP_FOO_ARG_NAME  ) )

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		bool clear_all( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
			typename map_type::base_type tmp(_map.load()); \
			for(typename map_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i) { \
				if(i->second){ i->second->clear( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ); } } \
			return true; }

#	define YGGR_TMP_PP_ARG_CREF( __n__, __val__ ) boost::cref( BOOST_PP_CAT(__val__, __n__ ) )
#	define YGGR_PP_FOO_CUSTOM_OP() YGGR_TMP_PP_ARGS_CREF
#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_PP_FOO_ARG_NAME
#undef YGGR_TMP_PP_ARG_CREF
#undef YGGR_PP_FOO_CUSTOM_OP
#undef YGGR_TMP_PP_ARGS_CREF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	// create files
	template<typename Container>
	void prv_create_log_files(const Container& cont)
	{
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type now_string_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_citer_type;
		typedef typename boost::range_value<now_string_type>::type char_type;

		typedef typename map_type::base_type now_map_type;

		YGGR_ASSERT_CHAR_TYPE(char_type);
		now_map_type map;

		path_string_type fname;
		for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i/*, ++idx*/)
		{
			charset::string_converter::s_conv(fname, container::mapped_value<cont_type>(*i));
			if(fname.empty())
			{
				continue;
			}

			map[fname] = construct_shared<log_file_type>(yggr_nothrow_new log_file_type(fname));
		}

		_map.swap(map);
	}

	template<typename Container, typename Handler>
	void prv_create_log_files(const Container& cont, const Handler& handler)
	{
		typedef Container cont_type;
		typedef typename container_mapped_value_t<cont_type>::type now_string_type;
		typedef typename boost::range_const_iterator<cont_type>::type cont_citer_type;
		typedef typename boost::range_value<now_string_type>::type char_type;

		typedef typename map_type::base_type now_map_type;

		YGGR_ASSERT_CHAR_TYPE(char_type);

		now_map_type map;

		path_string_type fname;
		for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i/*, ++idx*/)
		{
			charset::string_converter::s_conv(fname, handler(*i));
			if(fname.empty())
			{
				continue;
			}

			map[fname] = construct_shared<log_file_type>(yggr_nothrow_new log_file_type(fname));
		}

		_map.swap(map);
	}

	inline void prv_create_log_files(const path_string_vt_type& fname_vt)
	{
		if(fname_vt.empty())
		{
			return;
		}

		this_type::prv_org_create_log_files(fname_vt);
	}

	// create one file
	template<typename Char> inline
	typename boost::enable_if<is_native_char<Char>, void>::type
		prv_create_log_files(const Char* fname)
	{
		if(!fname)
		{
			return;
		}

		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);

		return this_type::prv_org_create_log_file(str_fname);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	typename boost::enable_if<charset::is_string_t< Basic_String<Char, Traits, Alloc> >, void>::type
		prv_create_log_files(const Basic_String<Char, Traits, Alloc>& fname)
	{
		if(fname.empty())
		{
			return;
		}

		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);

		this_type::prv_org_create_log_file(fname);
	}

	inline void prv_create_log_files(const path_string_type& fname)
	{
		if(fname.emtpy())
		{
			return;
		}

		this_type::prv_org_create_log_file(fname);
	}

	// org_create
	void prv_org_create_log_files(const path_string_vt_type& fname_vt)
	{
		typedef typename path_string_vt_type::const_iterator vt_iter_type;
		typedef typename map_type::base_type now_map_type;

		assert(!fname_vt.empty());

		now_map_type map;
		for(vt_iter_type i = boost::begin(fname_vt), isize = boost::end(fname_vt);
				i != isize; ++i)
		{
			if((*i).empty())
			{
				continue;
			}

			map[(*i)] = construct_shared<log_file_type>(yggr_nothrow_new log_file_type((*i)));
		}

		_map.swap(map);
	}

	inline void prv_org_create_log_file(const path_string_type& fname)
	{
		assert(!fname.empty());
		_map.insert(fname, construct_shared<log_file_type>(yggr_nothrow_new log_file_type(fname)));
	}

private:
	map_type _map;
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_FILE_MGR_HPP__
