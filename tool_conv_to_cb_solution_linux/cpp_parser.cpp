// xpp_parser.cpp

#include "cpp_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "file_operator.hpp"

#include "file_operator.hpp"
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/algorithm/knuth_morris_pratt.hpp>

#include <sstream>
#include <utility>

namespace cpp
{


inline bool is_validate_char(char ch)
{
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

inline ptree_string_type& remove_invalidate_char(ptree_string_type& str, char ch)
{
	typedef ptree_string_type::iterator iter_type;

	iter_type new_end = std::remove(str.begin(), str.end(), ch);
	if(new_end != str.end())
	{
		str.erase(new_end, str.end());
	}

	return str;
}

inline ptree_string_type& lib_name_to_lib_mark(ptree_string_type& str)
{
	typedef ptree_string_type::iterator iter_type;

	static const ptree_string_type s_str_vc_ver = "-vc$(PlatformToolsetVersion)";
	static const ptree_string_type s_str_vc70_ver = "-vc70";
	static const ptree_string_type s_str_vc71_ver = "-vc71";
	static const ptree_string_type s_str_vc80_ver = "-vc80";
	static const ptree_string_type s_str_vc90_ver = "-vc90";
	static const ptree_string_type s_str_vc100_ver = "-vc100";
	static const ptree_string_type s_str_vc110_ver = "-vc110";
	static const ptree_string_type s_str_vc120_ver = "-vc120";
	static const ptree_string_type s_str_vc140_ver = "-vc140";
	static const ptree_string_type s_str_vc141_ver = "-vc141";
	static const ptree_string_type s_str_vc142_ver = "-vc142";
	static const ptree_string_type s_str_vc143_ver = "-vc143";
	static const ptree_string_type s_str_debug_ver = "-d";

	// remove first "lib"
	{
		if(3 < str.size() && std::equal(str.begin(), str.begin() + 3, "lib"))
		{
			str.erase(str.begin(), str.begin() + 3);
		}
	}

	// remove ext
	{
		if(4 < str.size() && std::equal(str.begin() + (str.size() - 4), str.end(), ".lib"))
		{
			str.erase(str.begin() + (str.size() - 4), str.end());
		}
	}


#define TMP_PP_REMOVE_STR( __ver_str__ ) \
	{	std::size_t pos = str.find( __ver_str__ ); \
		if(pos != ptree_string_type::npos) { \
			str.erase(str.begin() + pos, str.begin() + (pos + __ver_str__.size())); } }

	// remove "-vc$(PlatformToolsetVersion)"
	TMP_PP_REMOVE_STR(s_str_vc_ver);

	// remove "-vc70" - "vc143"
	TMP_PP_REMOVE_STR(s_str_vc70_ver);
	TMP_PP_REMOVE_STR(s_str_vc71_ver);
	TMP_PP_REMOVE_STR(s_str_vc80_ver);
	TMP_PP_REMOVE_STR(s_str_vc90_ver);
	TMP_PP_REMOVE_STR(s_str_vc100_ver);
	TMP_PP_REMOVE_STR(s_str_vc110_ver);
	TMP_PP_REMOVE_STR(s_str_vc120_ver);
	TMP_PP_REMOVE_STR(s_str_vc140_ver);
	TMP_PP_REMOVE_STR(s_str_vc141_ver);
	TMP_PP_REMOVE_STR(s_str_vc142_ver);
	TMP_PP_REMOVE_STR(s_str_vc143_ver);

	// remove -d
	TMP_PP_REMOVE_STR(s_str_debug_ver);

#undef TMP_PP_REMOVE_STR

	return str;
}

template<typename Iter> inline
Iter parse_yggr_code_pair(libmark_set_type& out, Iter s, Iter e)
{
	typedef libmark_set_type::value_type value_type;

	typedef Iter iter_type;

	if(s == e)
	{
		return e;
	}

	iter_type i = s, isize = e;
	value_type val;
	
	if((isize = std::find(i, isize, ')')) == e) 
	{
		return e;
	}

	for(;!is_validate_char(*i); yggr::advance(i, 1));

	value_type libmark(i, isize);
	if(remove_invalidate_char(libmark, ' ').empty()
		|| lib_name_to_lib_mark(libmark).empty())
	{
		return e;
	}

	out.insert(boost::move(libmark));

	return yggr::advance_copy(isize, 1);

}

template<typename Iter> inline
Iter pragma_comment_to_libmark(libmark_set_type& out, Iter s, Iter e)
{
	typedef libmark_set_type::value_type value_type;
	typedef Iter iter_type;

	static const ptree_string_type s_str_libmark = "lib";

	if(s == e)
	{
		return e;
	}

	iter_type i = s, imid = e, isize = e;
	value_type val;
	
	if((isize = std::find(i, isize, ')')) == e) 
	{
		return e;
	}

	if((i = std::search(i, isize, s_str_libmark.begin(), s_str_libmark.end())) == isize)
	{
		return yggr::advance_copy(isize, 1);
	}

	if((i = std::find(i, isize, ',')) == isize)
	{
		return yggr::advance_copy(isize, 1);
	}

	for(;!is_validate_char(*i); yggr::advance(i, 1));

	if((imid = std::find(i, isize, '\"')) == e) 
	{
		return e;
	}

	value_type libmark(i, imid);

	if(remove_invalidate_char(libmark, ' ').empty()
		|| lib_name_to_lib_mark(libmark).empty())
	{
		return e;
	}

	//std::cout << "pc libmark: " << libmark << std::endl;
	out.insert(boost::move(libmark));

	return yggr::advance_copy(isize, 1);

}

libmark_set_type& get_libmarks_from_code_yggr_pp_link_lib(libmark_set_type& out, const ptree_string_type& code)
{
	typedef ptree_string_type::const_iterator str_citer_type;
	typedef yggr::knuth_morris_pratt<str_citer_type> kmp_type;
	typedef std::pair<str_citer_type, str_citer_type> kmp_return_type;

	static const ptree_string_type s_str_mark = "YGGR_PP_LINK_LIB(";
	static const kmp_type s_kmp_mark(s_str_mark.begin(), s_str_mark.end());
	
	if(code.empty())
	{
		return out;
	}

	kmp_return_type ipair = s_kmp_mark(code.begin(), code.end());

	if(ipair.second == code.end())
	{
		return out;
	}

	str_citer_type isize = code.end();

	libmark_set_type tmp_out;
	for(;ipair.second != isize; ipair = s_kmp_mark(yggr::advance_copy(ipair.second, 1), isize))
	{
		if((ipair.second = parse_yggr_code_pair(tmp_out, /*out, */ipair.second, isize)) == isize)
		{
			return out;
		}

		assert(ipair.second < isize);
	}

	out.insert(tmp_out.begin(), tmp_out.end());

	//{
	//	for(libmark_set_type::const_iterator i = out.begin(), isize = out.end(); i != isize; ++i)
	//	{
	//		std::cout << "lib mark: " << *i << std::endl;
	//	}
	//}

	return out;
}

libmark_set_type& get_libmarks_from_code_pragma_comment(libmark_set_type& out, const ptree_string_type& code)
{
	typedef ptree_string_type::const_iterator str_citer_type;
	typedef yggr::knuth_morris_pratt<str_citer_type> kmp_type;
	typedef std::pair<str_citer_type, str_citer_type> kmp_return_type;

	static const ptree_string_type s_str_mark = "pragma comment";

	static const kmp_type s_kmp_mark(s_str_mark.begin(), s_str_mark.end());
	
	if(code.empty())
	{
		return out;
	}

	kmp_return_type ipair = s_kmp_mark(code.begin(), code.end());

	if(ipair.second == code.end())
	{
		return out;
	}

	str_citer_type isize = code.end();

	libmark_set_type tmp_out;
	for(;ipair.second != isize; ipair = s_kmp_mark(yggr::advance_copy(ipair.second, 1), isize))
	{
		if((ipair.second = pragma_comment_to_libmark(tmp_out, ipair.second, isize)) == isize)
		{
			return out;
		}

		assert(ipair.second < isize);
	}

	out.insert(tmp_out.begin(), tmp_out.end());

	//{
	//	for(libmark_set_type::const_iterator i = out.begin(), isize = out.end(); i != isize; ++i)
	//	{
	//		std::cout << "lib mark: " << *i << std::endl;
	//	}
	//}

	return out;
}

template<typename Iter> inline
Iter include_to_libmark(libmark_set_type& out, Iter s, Iter e)
{
	typedef libmark_set_type::value_type value_type;
	typedef Iter iter_type;

	if(s == e)
	{
		return e;
	}

	value_type libmark;
	libmark.reserve(std::distance(s, e));

	std::size_t modify_count = 0;
	iter_type i = s, isize = e;
	for(; (i != isize) && (modify_count != 2); ++i)
	{
		if(*i == '/')
		{
			if(!modify_count)
			{
				libmark.push_back('_');
			}

			++modify_count;
		}
		else
		{
			libmark.push_back(*i);
		}
	}

	if(remove_invalidate_char(libmark, ' ').empty())
	{
		return e;
	}

	out.insert(boost::move(libmark));

	return yggr::advance_copy(isize, 1);

}

libmark_set_type& get_libmarks_from_code_include(libmark_set_type& out, const ptree_string_type& code)
{
	typedef ptree_string_type::const_iterator str_citer_type;
	typedef yggr::knuth_morris_pratt<str_citer_type> kmp_type;
	typedef std::pair<str_citer_type, str_citer_type> kmp_return_type;

	static const ptree_string_type s_str_break_l = "\"<";
	static const ptree_string_type s_str_break_r = "\">";
	static const ptree_string_type s_str_mark_inc = "include";

	static const kmp_type s_kmp_mark_inc(s_str_mark_inc.begin(), s_str_mark_inc.end());
	
	if(code.empty())
	{
		return out;
	}

	kmp_return_type ipair = kmp_return_type(code.begin(), code.end());

	libmark_set_type tmp_out;
	kmp_return_type ipair_inc_data;

	for(;ipair.first != ipair.second;)
	{
		ipair.first = std::find(ipair.first, ipair.second, '#');
		if(ipair.first == ipair.second)
		{
			break;
		}

		ipair.first = s_kmp_mark_inc(ipair.first + 1, ipair.second).second;
		if(ipair.first == ipair.second)
		{
			break;
		}

		ipair_inc_data.first = ipair.first = 
			std::find_first_of(ipair.first, ipair.second, s_str_break_l.begin(), s_str_break_l.end());
		if(ipair.first == ipair.second)
		{
			break;
		}

		ipair_inc_data.second = ipair.first = 
			std::find_first_of(ipair.first + 1, ipair.second, s_str_break_r.begin(), s_str_break_r.end());

		if(ipair_inc_data.first != ipair_inc_data.second)
		{
			include_to_libmark(tmp_out, yggr::advance_copy(ipair_inc_data.first, 1), ipair_inc_data.second);
		}

		if(ipair.first != ipair.second)
		{
			++ipair.first;
		}
	}

	//std::cout << "include libmarks: size = " << tmp_out.size() << std::endl;

	//for(libmark_set_type::const_iterator i = tmp_out.begin(), isize = tmp_out.end(); i != isize; ++i)
	//{
	//	std::cout << *i << std::endl;
	//}

	out.insert(tmp_out.begin(), tmp_out.end());

	return out;
}

libmark_set_type& get_libmarks_from_code(libmark_set_type& out, const ptree_string_type& code)
{
	get_libmarks_from_code_include(out, code);
	get_libmarks_from_code_pragma_comment(out, code);
	get_libmarks_from_code_yggr_pp_link_lib(out, code);

	return out;
}

libmark_set_type& get_libmarks_from_code_file(libmark_set_type& out, const ptree_string_type& file_path)
{
	ptree_string_type code;

	file_op::read_from_file(code, yggr::utf8_string(file_path, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

	return get_libmarks_from_code(out, code);
}

} // namespace cpp

#endif // !(BOOST_VERSION < 105600)
