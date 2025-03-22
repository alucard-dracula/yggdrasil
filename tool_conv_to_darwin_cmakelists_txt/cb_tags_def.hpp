// cb_tags_def.hpp

#ifndef __CB_TAGS_DEF_HPP__
#define __CB_TAGS_DEF_HPP__

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#if !(BOOST_VERSION < 105600)

namespace cb
{

struct cb_cfg_marks
{
public:
	inline static const ptree_string_type& s_mark_debug_x32(void)
	{
		static const ptree_string_type s_cfg_mark = "Debug-x32";
		return s_cfg_mark;
	}

	inline static const ptree_string_type& s_mark_release_x32(void)
	{
		static const ptree_string_type s_cfg_mark = "Release-x32";
		return s_cfg_mark;
	}

	inline static const ptree_string_type& s_mark_debug_x64(void)
	{
		static const ptree_string_type s_cfg_mark = "Debug-x64";
		return s_cfg_mark;
	}

	inline static const ptree_string_type& s_mark_release_x64(void)
	{
		static const ptree_string_type s_cfg_mark = "Release-x64";
		return s_cfg_mark;
	}

	inline static bool s_is_debug_mark(const ptree_string_type& str)
	{
		return (str == s_mark_debug_x32()) || (str == s_mark_debug_x64());
	}

	inline static bool s_is_release_mark(const ptree_string_type& str)
	{
		return (str == s_mark_release_x32()) || (str == s_mark_release_x64());
	}

	inline static bool s_is_x32_mark(const ptree_string_type& str)
	{
		return (str == s_mark_debug_x32()) || (str == s_mark_release_x32());
	}

	inline static bool s_is_x64_mark(const ptree_string_type& str)
	{
		return (str == s_mark_debug_x64()) || (str == s_mark_release_x64());
	}
};

struct cb_app_tags
{
public:
	inline static const ptree_string_type& s_tag_exe(void)
	{
		static const ptree_string_type s_app_tag = "1";
		return s_app_tag;
	}

	inline static const ptree_string_type& s_tag_dll(void)
	{
		static const ptree_string_type s_app_tag = "3";
		return s_app_tag;
	}

	inline static const ptree_string_type& s_tag_static_lib(void)
	{
		static const ptree_string_type s_app_tag = "2";
		return s_app_tag;
	}

	inline static const ptree_string_type& s_tag_utility(void)
	{
		static const ptree_string_type s_app_tag = "4";
		return s_app_tag;
	}

	//inline static yggr::u32 s_tag_makefile(void)
	//{
	//	static const ptree_string_type s_app_tag = "unknow";
	//	return s_app_tag;
	//}

	inline static bool s_has_compile_option(const ptree_string_type& app_tag)
	{
		return 
			(app_tag == s_tag_exe())
			|| (app_tag == s_tag_dll())
			|| (app_tag == s_tag_static_lib());
	}

	inline static bool s_has_linker_option(const ptree_string_type& app_tag)
	{
		return 
			(app_tag == s_tag_exe())
			|| (app_tag == s_tag_dll());
	}

};

struct cb_cpp_lang_std
{
public:
	inline static const ptree_string_type& s_cpp98(void)
	{
		static const ptree_string_type s_cpp_ver = "-std=c++98";
		return s_cpp_ver;
	}

	inline static const ptree_string_type& s_cpp03(void)
	{
		static const ptree_string_type s_cpp_ver = "-std=c++03";
		return s_cpp_ver;
	}

	inline static const ptree_string_type& s_cpp11(void)
	{
		static const ptree_string_type s_cpp_ver = "-std=c++11";
		return s_cpp_ver;
	}

	inline static const ptree_string_type& s_cpp14(void)
	{
		static const ptree_string_type s_cpp_ver = "-std=c++14";
		return s_cpp_ver;
	}

	inline static const ptree_string_type& s_cpp17(void)
	{
		static const ptree_string_type s_cpp_ver = "-std=c++17";
		return s_cpp_ver;
	}

	inline static const ptree_string_type& s_cpp20(void)
	{
		static const ptree_string_type s_cpp_ver = "-std=c++20";
		return s_cpp_ver;
	}
};

} // namespace cb

#endif // BOOST_VERSION < 105600

#endif // __CB_TAGS_DEF_HPP__
