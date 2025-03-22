// mongo_operators_dollar_cmd_def.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_OPERATORS_DOLLAR_CMD_DEF_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_OPERATORS_DOLLAR_CMD_DEF_HPP__

#include <yggr/nsql_database_system/detail/const_string_make_function_macro.hpp>

namespace yggr
{
namespace nsql_database_system
{

#define YGGR_TMP_PP_MAKE_FUNCTION YGGR_TMP_PP_CONST_STRING_MAKE_FUNCTION

struct mongo_operators_dollar_cmd_def
{
	// querty and projection operators

	// comparison
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_eq, "$eq")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_gt, "$gt")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_gte, "$gte")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_in, "$in")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_lt, "$lt")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_lte, "$lte")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_ne, "$ne")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_nin, "$nin")

	// logical
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_and, "$and")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_not, "$not")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_nor, "$nor")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_or, "$or")

	// element
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_exists, "$exists")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_type, "$type")

	// evaluation
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_expr, "$expr")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_json_schema, "$jsonSchema")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_mod, "$mod")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_regex, "$regex")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_text, "$text")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_where, "$where")

	// geospatial
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_geo_intersects, "$geoIntersects")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_geo_within, "$geoWithin")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_near, "$near")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_near_sphere, "$nearSphere")

	// array
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_all, "$all")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_elem_match, "$elemMatch")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_size, "$size")

	// bitwise
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_all_clear, "$bitsAllClear")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_all_set, "$bitsAllSet")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_any_clear, "$bitsAnyClear")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_any_set, "$bitsAnySet")

	// comments
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_comment, "$comment")

	// projection operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar, "$")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_elem_match, "$elemMatch") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_meta, "$meta")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_slice, "$slice")

	// Update Operators
	// fields
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_current_date, "$currentData")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_inc, "$inc")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_min, "$min")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_max, "$max")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_mul, "$mul")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_rename, "$rename")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set, "$set")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_on_insert, "$setOnInsert")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_unset, "$unset")

	// array
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar, "$")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_array_identifier, "$[]")

	template<typename T> static 
	utf8_string dollar_array_identifier(const T& id) 
	{
		utf8_string tmp; 
		std::stringstream ss;
		ss << "$[" << id << "]";
		ss >> tmp;
		return tmp;
	} 
	
	template<typename String, typename T> static
	String dollar_array_identifier(const T& id) 
	{ 
		typedef String now_string_type;
		typedef typename boost::range_value<now_string_type>::type string_val_type;
		typedef std::basic_stringstream<string_val_type> ss_type;
		now_string_type tmp; 
		ss_type ss;
		ss << "$[" << id << "]";
		ss >> tmp;
		return tmp;
	}

	YGGR_TMP_PP_MAKE_FUNCTION(dollar_add_to_set, "$addToSet")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_pop, "$pop")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_pull, "$pull")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_push, "$push")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_pull_all, "$pullAll")

	// modifiers
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_each, "$each")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_position, "$position")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_slice, "$slice") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_sort, "$sort")

	// bitwise
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bit, "$bit")

	// Aggregation Pipeline Stages
	// stage db.collection.aggregate() Stages

	YGGR_TMP_PP_MAKE_FUNCTION(dollar_add_fields, "$addFields")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bucket, "$bucket")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_bucket_auto, "$bucketAuto")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_coll_stats, "$collStats")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_count, "$count")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_facet, "$facet")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_geo_near, "$geoNear")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_graph_lookup, "$graphLookup")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_group, "$group")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_stats, "$indexStats")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_limit, "$limit")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_list_sessions, "$listSessions")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_lookup, "$lookup")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_match, "$match")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_out, "$out")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_project, "$project")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_redact, "$redact")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_replace_root, "$replaceRoot")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_sample, "$sample")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_skip, "$skip")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_sort, "$sort") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_sort_by_count, "$sortByCount")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_unwind, "$unwind")

	// stage db.aggregate() Stages
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_current_op, "$currentOp")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_list_local_sessions, "$listLocalSessions")

	// Aggregation Pipeline Operators
	// arithmetic expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_abs, "$abs")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_add, "$add")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_ceil, "$ceil")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_divide, "$divide")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_exp, "$exp")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_floor, "$floor")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_ln, "$ln")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_log, "$log")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_log10, "$log10")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_mod, "$mod") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_multiply, "$multiply")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_pow, "$pow")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_sqrt, "$sqrt")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_subtract, "$subtract")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_trunc, "$trunc")

	// array expression operators

	YGGR_TMP_PP_MAKE_FUNCTION(dollar_array_elem_at, "$arrayElemAt")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_array_to_object, "$arrayToObject")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_concat_arrays, "$concatArrays")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_filter, "$filter")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_in, "$in") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_of_array, "$indexOfArray")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_is_array, "$isArray")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_map, "$map")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_object_to_array, "$objectToArray") // version 3.6 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_range, "$range")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_reduce, "$reduce")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_reverse_array, "$reverseArray")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_size, "$size") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_slice, "$slice") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_zip, "$zip")

	// boolean expression operators
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_and, "$and") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_not, "$not") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_or, "$or") // this is a mark, don't deleted

	// comparison expression operators

	YGGR_TMP_PP_MAKE_FUNCTION(dollar_cmp, "$cmp")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_eq, "$eq") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_gt, "$gt") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_gte, "$gte") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_lt, "$lt") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_lte, "$lte") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_ne, "$ne") // this is a mark, don't deleted

	// conditional expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_cond, "$cond")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_if_null, "$ifNull")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_switch, "$switch")

	// date expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_from_parts, "$dateFromParts")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_from_string, "$dateFromString")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_to_parts, "$dateToParts")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_to_string, "$dateToString")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_day_of_month, "$dayOfMonth")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_day_of_week, "$dayOfWeek")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_day_of_year, "$dayOfYear")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_hour, "$hour")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_iso_day_of_week, "$isoDayOfWeek")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_iso_week, "$isoWeek")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_iso_week_year, "$isoWeekYear")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_millisecond, "$millisecond")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_minute, "$minute")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_month, "$month")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_second, "$second")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_date, "$toDate") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_week, "$week")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_year, "$year")

	// literal expression operator
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_literal, "$literal")

	// object expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_merge_objects, "$mergeObjects") // version 3.6 or later
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_object_to_array, "$objectToArray") // version 3.6 or later // this is a mark, don't deleted

	// set expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_all_elements_true, "$allElementsTrue")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_any_element_true, "$anyElementTrue")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_difference, "$setDifference")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_equals, "$setEquals")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_intersection, "$setIntersection")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_is_subset, "$setIsSubset")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_union, "$setUnion")

	// string expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_concat, "$concat")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_from_string, "$dateFromString") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_to_string, "$dateToString") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_of_bytes, "$indexOfBytes")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_of_cp, "$indexOfCP")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_ltrim, "$ltrim") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_rtrim, "$rtrim") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_split, "$split")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_str_len_bytes, "$strLenBytes")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_str_len_cp, "$strLenCP")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_strcasecmp, "$strcasecmp")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_substr, "$substr")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_substr_bytes, "$substrBytes")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_substr_cp, "$substrCP")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_lower, "$toLower")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_string, "$toString") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_trim, "$trim") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_upper, "$toUpper")

	// type expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_convert, "$convert") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_bool, "$toBool") // version 4.0 or later
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_date, "$toDate") // version 4.0 or later // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_decimal, "$toDecimal") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_double, "$toDouble") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_int, "$toInt") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_long, "$toLong") // version 4.0 or later
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_object_id, "$toObjectId") // version 4.0 or later
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_string, "$toString") // version 4.0 or later // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_type, "$type") // this is a mark, don't deleted

	// accumulators($group)
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_avg, "$avg")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_first, "$first")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_last, "$last")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_max, "$max") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_min, "$min") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_push, "$push") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_add_to_set, "$addToSet") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_std_dev_pop, "$stdDevPop")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_std_dev_samp, "$stdDevSamp")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_sum, "$sum")

	// accumulators($project)
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_avg, "$avg") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_max, "$max") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_min, "$min") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_push, "$push") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_std_dev_pop, "$stdDevPop") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_std_dev_samp, "$stdDevSamp") // this is a mark, don't deleted
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_sum, "$sum") // this is a mark, don't deleted

	// variable expression operators
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_let, "$let")

	// Query Modifiers

	// modifiers
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_comment, "$comment") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_explain, "$explain")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_hint, "$hint")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_max_scan, "$maxScan")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_max, "$max") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_max_time_ms, "$maxTimeMS")
	//YGGR_TMP_PP_MAKE_FUNCTION(dollar_min, "$min") // this is a mark, don't deleted
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_orderby, "$orderby")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_query, "$query")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_return_key, "$returnKey")
	YGGR_TMP_PP_MAKE_FUNCTION(dollar_show_disk_loc, "$showDiskLoc")

	// sort order

	YGGR_TMP_PP_MAKE_FUNCTION(dollar_natural, "$natural")

};

#undef YGGR_TMP_PP_MAKE_FUNCTION

typedef mongo_operators_dollar_cmd_def mongo_operators_dollar_cmd_def_type;

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGO_OPERATORS_DOLLAR_CMD_DEF_HPP__
