// lib_depend_info.hpp

#ifndef __LIB_DEPEND_INFO_HPP__
#define __LIB_DEPEND_INFO_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "vcproj_tags_def.hpp"

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

#include <yggr/container/set.hpp>
#include <yggr/container/map.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/container/unordered_map.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>

#include <boost/ref.hpp>

namespace lib
{

typedef yggr::set<ptree_string_type> string_set_type;
typedef yggr::multimap<yggr::s32, ptree_string_type/*, std::greater<yggr::s32>*/ > ordered_libname_map_type;

struct lib_depend_info_cfg_sample {};

class lib_depend_info
{
public:
	typedef ptree_string_type string_type;
	typedef yggr::unordered_set<string_type> string_uset_type;
	typedef yggr::unordered_map<string_type, string_uset_type> mark_depend_mapping_type;
	typedef yggr::unordered_map<string_type, yggr::s32> mark_level_mapping_type;
	typedef yggr::unordered_map<string_type, string_type > mark_name_mapping_type;
	typedef yggr::multimap<yggr::s32, ptree_string_type/*, std::greater<yggr::s32>*/ > ordered_string_map_type;

private:
	typedef lib_depend_info this_type;

public:
	lib_depend_info(void);
	lib_depend_info(lib_depend_info_cfg_sample);

	lib_depend_info(BOOST_RV_REF(this_type) right)
		: _str_boost_ver(boost::move(right._str_boost_ver)),
			_str_lua_ver(boost::move(right._str_lua_ver)),
			_str_python_ver(boost::move(right._str_python_ver)),
			_marks_dep(boost::move(right._marks_dep)),
			_marks_lv(boost::move(right._marks_lv)),
			_marks_name(boost::move(right._marks_name))
	{
	}

	lib_depend_info(const this_type& right);
	~lib_depend_info(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		_str_boost_ver = boost::move(right._str_boost_ver);
		_str_lua_ver = boost::move(right._str_lua_ver);
		_str_python_ver = boost::move(right._str_python_ver);

		_marks_dep = boost::move(right._marks_dep);
		_marks_lv = boost::move(right._marks_lv);
		_marks_name = boost::move(right._marks_name);

		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

private:
	string_set_type& format_libmarks(string_set_type& out, 
										const string_set_type& libmarks) const;

	ordered_string_map_type& sort_libmarks(ordered_string_map_type& out, 
											const string_set_type& libmarks) const;

	string_type libmark_to_libname(const string_type& libmark,
									const string_type& vc_cfg_mark, 
									const string_type& str_boost_ver,
									const string_type& str_lua_ver,
									const string_type& str_python_ver) const;

	ordered_string_map_type& 
		libmarks_to_libnames(ordered_string_map_type& inout, 
								const string_type& vc_cfg_mark, 
								const string_type& str_boost_ver,
								const string_type& str_lua_ver,
								const string_type& str_python_ver) const;

public:
	inline ordered_string_map_type& 
		gen_libnames(ordered_string_map_type& out, 
						const string_set_type& libmarks,
						const string_type& vc_cfg_mark, 
						const string_type& str_boost_ver,
						const string_type& str_lua_ver,
						const string_type& str_python_ver) const
	{
		string_set_type formated_libmarks;
		return 
			libmarks_to_libnames(
				sort_libmarks(out, format_libmarks(formated_libmarks, libmarks)),
				vc_cfg_mark, str_boost_ver, str_lua_ver, str_python_ver);
	}

	inline ordered_string_map_type& 
		gen_libnames(ordered_string_map_type& out, 
						const string_set_type& libmarks,
						const string_type& vc_cfg_mark) const
	{
		return 
			this_type::gen_libnames(
				out, libmarks, vc_cfg_mark,
				this_type::_str_boost_ver, this_type::_str_lua_ver, this_type::_str_python_ver);
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive> inline
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_str_boost_ver);
		ar & YGGR_SERIALIZATION_NVP(_str_lua_ver);
		ar & YGGR_SERIALIZATION_NVP(_str_python_ver);

		ar & YGGR_SERIALIZATION_NVP(_marks_dep);
		ar & YGGR_SERIALIZATION_NVP(_marks_lv);
		ar & YGGR_SERIALIZATION_NVP(_marks_name);
	}

private:
	string_type _str_boost_ver;
	string_type _str_lua_ver;
	string_type _str_python_ver;

	mark_depend_mapping_type _marks_dep;
	mark_level_mapping_type _marks_lv;
	mark_name_mapping_type _marks_name;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(lib_depend_info)
} // namespace swap_support

} // namespace lib

namespace std
{

using lib::swap_support::swap;

} // namespace std

namespace boost
{

using lib::swap_support::swap;

} // namespace boost

#endif // BOOST_VERSION < 105600

#endif // __LIB_DEPEND_INFO_HPP__
