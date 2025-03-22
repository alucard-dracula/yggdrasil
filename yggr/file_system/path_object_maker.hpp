//path_object_maker.hpp

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

#ifndef __YGGR_FILE_SYSTEM_PATH_OBJECT_MAKER_HPP__
#define __YGGR_FILE_SYSTEM_PATH_OBJECT_MAKER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/type_traits/remove_cv_ref.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>
#include <yggr/iterator_ex/is_const_iterator.hpp>

#include <yggr/file_system/path_check.hpp>
#include <yggr/file_system/local_fsys_native_operators.hpp>

namespace yggr
{
namespace file_system
{

class path_object_maker
	: public path_check
{
public:
	typedef path_check base_type;
	typedef native::path path_type;
	typedef path_type::value_type path_char_type;
	typedef path_type::string_type path_string_type;

	typedef native::error_code error_code_type;
	typedef native::filesystem_error exception_type;

private:
	typedef path_object_maker this_type;

protected:

	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			is_iterator<Iter>,
			path_type
		>::type
		pro_s_make_path_object(Iter s, Iter e, error_code_type* ec)
	{
		typedef Iter iter_type;

		typedef regular_parse::detail::iterator_helper<iter_type> iter_h_type;
		typedef typename iter_h_type::org_iter_type org_iter_type;
		typedef typename iter_h_type::value_type src_char_type;

		typedef 
			regular_parse::detail::conv_iterator_helper
			<
				org_iter_type,
				path_char_type,
				src_char_type
			> conv_type;
		typedef typename conv_type::result_type iter_pair_type;

		//typedef regular_parse::detail::string_helper<path_string_type> path_string_h_type;

		path_type path;

		if(base_type::check_path_name(s, e))
		{
			conv_type h;
			path_string_type& path_str = get_path_string(path);
			iter_pair_type iter_pair = h(iter_h_type::org(s), iter_h_type::org(e));

			container::reserve(path_str, std::distance(iter_pair.first, iter_pair.second));
			path_str.assign(iter_pair.first, iter_pair.second);

			if(path.empty())
			{
				try
				{
					path = native::call_current_path(ec);
					if(ec && (*ec))
					{
						path.clear();
					}
				}
				catch(const exception_type&)
				{
					path.clear();
				}
			}
		}
		
		return path;
	}

public:
	// make_path_object
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			is_iterator<Iter>,
			path_type
		>::type
		make_path_object(Iter s, Iter e)
	{
		return this_type::pro_s_make_path_object(s, e, 0);
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			is_iterator<Iter>,
			path_type
		>::type
		make_path_object(Iter s, Iter e, error_code_type& ec)
	{
		return this_type::pro_s_make_path_object(s, e, boost::addressof(ec));
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			path_type
		>::type
		make_path_object(const Target& text)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		return 
			this_type::pro_s_make_path_object(
				target_h_type::begin(text), target_h_type::end(text), 0);
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			charset::is_generalized_string_t<Target>,
			path_type
		>::type
		make_path_object(const Target& text, error_code_type& ec)
	{
		typedef Target target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		return 
			this_type::pro_s_make_path_object(
				target_h_type::begin(text), 
				target_h_type::end(text), 
				boost::addressof(ec) );
	}

	template<typename T1, typename T2> inline
	static path_type make_path_object(const charset::string_charset_helper<T1, T2>& text)
	{
		typedef charset::string_charset_helper<T1, T2> string_charset_helper_type;
		typedef typename string_charset_helper_type::native_first_type now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);
		typedef typename remove_cv_ref<now_inner_string_type>::type target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		now_inner_string_type str_text((charset::string_charset_helper_data(text)));
		return this_type::pro_s_make_path_object(target_h_type::begin(str_text), target_h_type::end(str_text), 0);
	}

	template<typename T1, typename T2> inline
	static path_type make_path_object(const charset::string_charset_helper<T1, T2>& text, error_code_type& ec)
	{
		typedef charset::string_charset_helper<T1, T2> string_charset_helper_type;
		typedef typename string_charset_helper_type::native_first_type now_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		typedef typename remove_cv_ref<now_inner_string_type>::type target_type;
		typedef regular_parse::detail::string_helper<target_type> target_h_type;

		now_inner_string_type str_text((charset::string_charset_helper_data(text)));
		return 
			this_type::pro_s_make_path_object(
				target_h_type::begin(str_text), 
				target_h_type::end(str_text), 
				boost::addressof(ec));
	}

	// boost 181 compile make_path_object(const path_type& text) using path_type construct failed

#if 0 
	inline static const path_type& make_path_object(const path_type& text)
	{
		return text;
	}

	inline static const path_type& make_path_object(const path_type& text, error_code_type& /*ec*/)
	{
		return text;
	}

#else
	
	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			boost::is_same<Target, path_type>,
			const path_type&
		>::type
		make_path_object(const Target& text)
	{
		return text;
	}

	template<typename Target> inline
	static 
	typename 
		boost::enable_if
		<
			boost::is_same<Target, path_type>,
			const path_type&
		>::type
		make_path_object(const Target& text, error_code_type& /*ec*/)
	{
		return text;
	}

#endif // 0, 1
};

} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_PATH_OBJECT_MAKER_HPP__
