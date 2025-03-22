//c_bson_basic_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_save_load(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef bson_type::iterator bson_iter_type;

	bson_type bs, bs2, bs3;
	bs.save("bool", false);
	bs.save("char", static_cast<char>(1));
	bs.save("s8", static_cast<yggr::s8>(2));
	bs.save("u8", static_cast<yggr::u8>(3));
	bs.save("wchar_t", static_cast<wchar_t>(4));
	bs.save("s16", static_cast<yggr::s16>(5));
	bs.save("u16", static_cast<yggr::u16>(6));
	bs.save("s32", static_cast<yggr::s32>(7));
	bs.save("u32", static_cast<yggr::u32>(8));
	bs.save("s64", static_cast<yggr::s64>(9));
	bs.save("u64", static_cast<yggr::u64>(10));
	bs.save("f32", static_cast<yggr::f32>(11.5f));
	bs.save("f64", static_cast<yggr::f64>(12.5));

	{
		bson_decimal128_t dec128 = {1, 1};
		bs.save("bson_decimal128_t", dec128);
	}
	bs.save("c_bson_decimal128", yggr::nsql_database_system::c_bson_decimal128(2, 2));

	bs.save("const char*", "this is a const char*");
	bs.save("const wchar*", L"this is a const wchar_t*");
	bs.save("std::string", std::string("this is a std::string"));
	bs.save("std::wstring", std::wstring(L"this is a std::wstring"));
	bs.save("boost::string", boost::container::string("this is a boost::string"));
	bs.save("boost::wstring", boost::container::wstring(L"this is a boost::wstring"));
	bs.save("yggr::utf8_string", yggr::utf8_string("this is a yggr::utf8_string"));

	bs.save("yggr::string_view", yggr::string_view("this is a yggr::string_view"));
	bs.save(yggr::string_view("yggr::wstring_view"), yggr::wstring_view(L"this is a yggr::wstring_view"));
	bs.save("yggr::utf8_string_view", yggr::utf8_string_view("this is a yggr::utf8_string_view"));
	bs.save(yggr::utf8_string_view("const char* 2"), "const char* 2");
	bs.save(yggr::utf8_string_view("yggr::utf8_string_view2"), yggr::utf8_string_view("yggr::utf8_string_view2"));

	{
		bson_iter_type iter = bs.find("const char*");
		yggr_test_assert(iter != bs.end());

		yggr::utf8_string_view str_view;
		iter.load_value(str_view);
		yggr_test_assert(str_view == "this is a const char*");

		yggr::utf8_string str;
		iter.load_value(str);
		yggr_test_assert(str_view == "this is a const char*");
	}

	std::cout << bs << std::endl;

	bs2.save(std::string("bson"), bs);

	bs2.save("c_bson_date", yggr::nsql_database_system::c_bson_date(13));
	std::cout << yggr::nsql_database_system::c_bson_date(13) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_date(13)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_date(13)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_date(13)) << std::endl;

	bs2.save("c_bson_time", yggr::nsql_database_system::c_bson_time(14));
	std::cout << yggr::nsql_database_system::c_bson_time(14) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_time(14)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_time(14)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_time(14)) << std::endl;

	bs2.save("c_bson_timeval", yggr::nsql_database_system::c_bson_timeval(15, 15000));
	std::cout << yggr::nsql_database_system::c_bson_timeval(15, 15000) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_timeval(15, 15000)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_timeval(15, 15000)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_timeval(15, 15000)) << std::endl;

	bs2.save("c_bson_timeval_zwei", yggr::nsql_database_system::c_bson_timeval(16, 16));
	std::cout << yggr::nsql_database_system::c_bson_timeval(16, 16) << std::endl;

	{
		yggr::nsql_database_system::c_bson_timeval::org_type timeval = {15, 15000};
		bs2.save("c_bson_basic_timeval", timeval);
	}

	yggr::nsql_database_system::c_bson_oid chk_id;
	bs2.save("c_bson_oid", chk_id);
	std::cout << chk_id << std::endl;

	yggr::nsql_database_system::c_bson_oid::org_type org_chk_id = chk_id;
	bs2.save("c_bson_basic_oid", org_chk_id);
	std::cout << org_chk_id << std::endl;


	int bin_buf_arr_data[] = {1, 2, 3};
	// c_bson_binary_buffer_ref
	{
		std::vector<int> in_vt(bin_buf_arr_data, bin_buf_arr_data + 3);
		yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				std::vector<int>
			> buf_ref(in_vt);

		std::cout << buf_ref << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(buf_ref) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(buf_ref) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(buf_ref) << std::endl;
		bs2.save("c_bson_bin_buffer_ref_int_vt", buf_ref);
	}

	{
		std::list<int> in_list(bin_buf_arr_data, bin_buf_arr_data + 3);
		yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				std::list<int>
			> buf_ref(in_list);
		bs2.save("c_bson_bin_buffer_ref_int_list", buf_ref);
	}

	{
		int in_native_vt[3] = {0};
		memcpy(in_native_vt, bin_buf_arr_data, sizeof(in_native_vt));
		yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				int[3]
			> buf_ref(in_native_vt);
		bs2.save("c_bson_bin_buffer_ref_native_int_vt", buf_ref);
	}

	// c_bson_binary_buffer
	{
		std::vector<int> in_vt(bin_buf_arr_data, bin_buf_arr_data + 3);
		yggr::nsql_database_system::c_bson_binary_buffer
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY
			> buf(in_vt);

		std::cout << buf << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(buf) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(buf) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(buf) << std::endl;
		bs2.save("c_bson_bin_buffer_int_vt", buf);
	}

	{
		std::list<int> in_list(bin_buf_arr_data, bin_buf_arr_data + 3);
		yggr::nsql_database_system::c_bson_binary_buffer
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY
			> buf(in_list);
		bs2.save("c_bson_bin_buffer_int_list", buf);
	}

	{
		int in_native_vt[3] = {0};
		memcpy(in_native_vt, bin_buf_arr_data, sizeof(in_native_vt));
		yggr::nsql_database_system::c_bson_binary_buffer
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY
			> buf(in_native_vt);
		bs2.save("c_bson_bin_buffer_native_int_vt", buf);
	}

	bs2.save("c_bson_null", yggr::nsql_database_system::c_bson_null());
	std::cout << yggr::nsql_database_system::c_bson_null() << std::endl;

	bs2.save("c_bson_undefined", yggr::nsql_database_system::c_bson_undefined());
	std::cout <<  yggr::nsql_database_system::c_bson_undefined() << std::endl;

	bs2.save("c_bson_regex", yggr::nsql_database_system::c_bson_regex("regex_first", L"regex_second"));
	std::cout << yggr::nsql_database_system::c_bson_regex("regex_first", L"regex_second") << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_regex("regex_first", L"regex_second")) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_regex("regex_first", L"regex_second")) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_regex("regex_first", L"regex_second")) << std::endl;

	bs2.save("c_bson_code", yggr::nsql_database_system::c_bson_code("this is a js code"));
	std::cout << yggr::nsql_database_system::c_bson_code("this is a js code") << std::endl;

	//std::cout << bs2 << std::endl;

	bs2.save("c_bson_symbol", yggr::nsql_database_system::c_bson_symbol("this is a symbol"));
	std::cout << yggr::nsql_database_system::c_bson_symbol("this is a symbol") << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_symbol("this is a symbol")) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_symbol("this is a symbol")) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_symbol("this is a symbol")) << std::endl;

	{
		bson_type::base_type bson_scope_data = {0};
		bson_init(&bson_scope_data);
		bson_append_utf8(&bson_scope_data, "scope", -1, "this is a bson_scope_data", -1);

		bson_type cbson_scope_data;
		cbson_scope_data.save("scope", "this is a c_bson_scope_data");

		yggr::nsql_database_system::c_bson_code_w_scope<bson_type::base_type> bson_scope("this is a s c_bson_code", cbson_scope_data);
		yggr::nsql_database_system::c_bson_code_w_scope<bson_type> c_bson_scope("this is a s bson_code", bson_scope_data);


		bson_destroy(&bson_scope_data);
		c_bson_scope.swap(bson_scope);

		std::cout << bson_scope << std::endl;
		std::cout << c_bson_scope << std::endl;

		bs2.save("bson_code_w_scope", bson_scope);
		bs2.save("c_bson_code_w_scope", c_bson_scope);
	}

	bs2.save("c_bson_timestamp", yggr::nsql_database_system::c_bson_timestamp(999, 1));
	std::cout << yggr::nsql_database_system::c_bson_timestamp(999, 1) << std::endl;

	bs2.save("c_bson_dbpointer_base", yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t>("dbpointer", chk_id));
	std::cout << yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t>("dbpointer", chk_id) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t>("dbpointer", chk_id)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t>("dbpointer", chk_id)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t>("dbpointer", chk_id)) << std::endl;

	bs2.save("c_bson_dbpointer", yggr::nsql_database_system::c_bson_dbpointer<
									yggr::nsql_database_system::c_bson_oid>("dbpointer", chk_id));
	std::cout << yggr::nsql_database_system::c_bson_dbpointer<
					yggr::nsql_database_system::c_bson_oid>("dbpointer", chk_id) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(
				yggr::nsql_database_system::c_bson_dbpointer<
					yggr::nsql_database_system::c_bson_oid>("dbpointer", chk_id)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(
				yggr::nsql_database_system::c_bson_dbpointer<
					yggr::nsql_database_system::c_bson_oid>("dbpointer", chk_id)) << std::endl;
	std::cout <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(
				yggr::nsql_database_system::c_bson_dbpointer<
					yggr::nsql_database_system::c_bson_oid>("dbpointer", chk_id)) << std::endl;

	bs2.save("c_bson_minkey", yggr::nsql_database_system::c_bson_minkey());
	bs2.save("c_bson_maxkey", yggr::nsql_database_system::c_bson_maxkey());

	bs2.save("c_bson_value", yggr::nsql_database_system::c_bson_value("this is a c_bson_value"));

	{
		yggr::nsql_database_system::c_bson_value data("this is a bson_value_t");
		yggr::nsql_database_system::c_bson_value::org_type val;
		memset(&val, 0, sizeof(bson_value_t));

		bson_value_copy(&data, &val);
		bs2.save("bson_value_t", val);

		bson_value_destroy(&val);
	}

	std::cout << bs << std::endl;

	std::cout << "\n" <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
	std::cout << "\n" <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
	std::cout << "\n" <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

	std::cout << "\n" << bs2 << std::endl;
	std::cout << "\n" <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs2) << std::endl;
	std::cout << "\n" <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs2) << std::endl;
	std::cout << "\n" <<
		yggr::nsql_database_system::make_bson_visit<
			bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs2) << std::endl;

	yggr::nsql_database_system::c_bson bs_create_of_value;
	for(bson_iter_type i = bs2.begin(), isize = bs2.end(); i != isize; ++i)
	{
		std::cout << i.get_key<bson_type::inner_string_type>() << std::endl;
	}

	bson_iter_type iter_bs2 = bs2.begin();

	//----------- test find s ----------------
	{
		bson_iter_type iter(bs2, "c_bson_regex");
		yggr_test_assert(iter != bs2.end());

		yggr::nsql_database_system::c_bson_regex val;
		iter.load("c_bson_regex", val);

#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
	}

	{
		bson_iter_type iter(bs2, "abcdefg");
		yggr_test_assert(iter == bs2.end());
	}

	{
		bson_iter_type iter(YGGR_BSON_ITER_FIND_CASE_TAG_OBJECT(noncase), bs2, "c_bson_regex");
		yggr_test_assert(iter != bs2.end());

		yggr::nsql_database_system::c_bson_regex val;
		iter.load("c_bson_regex", val);
#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
	}

	{
		bson_iter_type iter(YGGR_BSON_ITER_FIND_CASE_TAG_OBJECT(case), bs2, std::string("C_bson_Regex"));
		yggr_test_assert(iter != bs2.end());

		yggr::nsql_database_system::c_bson_regex val;
		iter.load("c_bson_regex", val);
#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
	}

	{
		bson_iter_type iter(YGGR_BSON_ITER_FIND_CASE_TAG_OBJECT(case), bs2, std::string("abcdefg"));
		yggr_test_assert(iter == bs2.end());
	}

	{
		bson_iter_type iter = iter_bs2.find<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(noncase)>("c_bson_regex");
		yggr_test_assert(iter != bs2.end());

		yggr::nsql_database_system::c_bson_regex val;
		iter.load("c_bson_regex", val);
#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
	}

	{
		bson_iter_type iter = iter_bs2.find("c_bson_regex");
		yggr_test_assert(iter != bs2.end());

		yggr::nsql_database_system::c_bson_regex val;
		iter.load("c_bson_regex", val);
#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
	}

	{
		bson_iter_type iter = iter_bs2.find<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>(std::wstring(L"c_bSOn_regex"));
		yggr_test_assert(iter != bs2.end());

		yggr::nsql_database_system::c_bson_regex val;
		iter.load("c_bson_regex", val);
#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
	}

	{
		bson_iter_type iter = iter_bs2.find<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(noncase)>("bson");
		yggr_test_assert(iter != bs2.end());
	}

	{
		std::pair<bson_iter_type, bson_iter_type> iters
			= iter_bs2.find_descendant("bson.u32");
		yggr_test_assert(iters.first != bs2.end());
		yggr_test_assert(iters.second != bs2.end());

		yggr_test_assert(iters.first.get_key<bson_iter_type::inner_string_type>() == "bson");

		yggr::u32 chk = 0;
		iters.second.load("u32", chk);
		yggr_test_assert(chk == 8);
	}

	{
		std::pair<bson_iter_type, bson_iter_type> iters
			= iter_bs2.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>("bSon.U32");
		yggr_test_assert(iters.first != bs2.end());
		yggr_test_assert(iters.second != bs2.end());

		yggr_test_assert(iters.first.get_key<bson_iter_type::inner_string_type>() == "bson");

		yggr::u32 chk = 0;
		iters.second.load("u32", chk);
		yggr_test_assert(chk == 8);
	}

	//----------- test find e ----------------

	{
		bson_type val;
		iter_bs2.load("bson", val);
		++iter_bs2;

		bson_iter_type iter = val.begin();
		{
			bool val = true;
			iter.load("bool", val);
			yggr_test_assert(val == false);
			++iter;
		}

		{
			char val = 0;
			iter.load("char", val);
			yggr_test_assert(val == 1);
			++iter;
		}

		{
			yggr::s8 val = 0;
			iter.load("s8", val);
			yggr_test_assert(val == 2);
			++iter;
		}

		{
			yggr::u8 val = 0;
			iter.load("u8", val);
			yggr_test_assert(val == 3);
			++iter;
		}

		{
			wchar_t val = 0;
			iter.load("wchar_t", val);
			yggr_test_assert(val == 4);
			++iter;
		}

		{
			yggr::s16 val = 0;
			iter.load("s16", val);
			yggr_test_assert(val == 5);
			++iter;
		}

		{
			yggr::u16 val = 0;
			iter.load("u16", val);
			yggr_test_assert(val == 6);
			++iter;
		}

		{
			yggr::s32 val = 0;
			iter.load("s32", val);
			yggr_test_assert(val == 7);
			++iter;
		}

		{
			yggr::u32 val = 0;
			iter.load("u32", val);
			yggr_test_assert(val == 8);
			++iter;
		}

		{
			yggr::s64 val = 0;
			iter.load("s64", val);
			yggr_test_assert(val == 9);
			++iter;
		}

		{
			yggr::u64 val = 0;
			iter.load("u64", val);
			yggr_test_assert(val == 10);
			++iter;
		}

		{
			yggr::f32 val = 0;
			iter.load("f32", val);
			yggr_test_assert(val == 11.5);
			++iter;
		}

		{
			yggr::f64 val = 0;
			iter.load("f64", val);
			yggr_test_assert(val == 12.5);
			++iter;
		}

		{
			bson_decimal128_t val = {0};
			iter.load("bson_decimal128_t", val);
			yggr_test_assert(val.high == 1);
			yggr_test_assert(val.low == 1);
			++iter;
		}

		{
			yggr::nsql_database_system::c_bson_decimal128 val;
			iter.load("c_bson_decimal128", val);
			yggr_test_assert(val.var_high() == 2);
			yggr_test_assert(val.var_low() == 2);
			++iter;
		}

		{
			std::string val;
			iter.load("const char*", val);
			yggr_test_assert(val == "this is a const char*");
			++iter;
		}

		{
			std::wstring val;
			iter.load("const wchar*", val);
			yggr_test_assert(val == L"this is a const wchar_t*");
			++iter;
		}

		{
			std::string val;
			iter.load("std::string", val);
			yggr_test_assert(val == "this is a std::string");
			++iter;
		}

		{
			std::wstring val;
			iter.load("std::wstring", val);
			yggr_test_assert(val == L"this is a std::wstring");
			++iter;
		}

		{
			boost::container::string val;
			iter.load("boost::string", val);
			yggr_test_assert(val == "this is a boost::string");
			++iter;
		}

		{
			boost::container::wstring val;
			iter.load("boost::wstring", val);
			yggr_test_assert(val == L"this is a boost::wstring");
			++iter;
		}

		{
			yggr::utf8_string val;
			iter.load("yggr::utf8_string", val);
			yggr_test_assert(val == "this is a yggr::utf8_string");
			++iter;
		}
	}

	{
		yggr::nsql_database_system::c_bson_date val;
		iter_bs2.load("c_bson_date", val);
		yggr_test_assert(val == yggr::nsql_database_system::c_bson_date(13));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_time val;
		iter_bs2.load("c_bson_time", val);
		yggr_test_assert(val == yggr::nsql_database_system::c_bson_time(14));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_timeval val;
		iter_bs2.load("c_bson_timeval", val);
		yggr_test_assert(val == yggr::nsql_database_system::c_bson_timeval(15, 15000));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_timeval val;
		iter_bs2.load("c_bson_timeval_zwei", val);
		// saved (c_bson_timeval(16, 16)), but (16 / 1000 == 0), so load result is c_bson_timeval(16, 0)
		yggr_test_assert(val == yggr::nsql_database_system::c_bson_timeval(16, 0));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_timeval::org_type val;
		iter_bs2.load("c_bson_basic_timeval", val);
		yggr_test_assert(yggr::nsql_database_system::c_bson_timeval(val)
				== yggr::nsql_database_system::c_bson_timeval(15, 15000));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_oid val;
		iter_bs2.load("c_bson_oid", val);
		yggr_test_assert(val == chk_id);
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_oid::org_type val;
		iter_bs2.load("c_bson_basic_oid", val);
		yggr_test_assert(val == chk_id);
		++iter_bs2;
	}

	// c_bson_binary_buffer_ref
	{
		//int bin_buf_arr_data[] = {1, 2, 3};

		std::vector<int> out_vt;
		yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				std::vector<int>
			> buf_ref(out_vt);
		iter_bs2.load("c_bson_bin_buffer_ref_int_vt", buf_ref);
		yggr_test_assert(0 == memcmp(&out_vt[0], bin_buf_arr_data, sizeof(bin_buf_arr_data)));
		++iter_bs2;
	}

	{
		//int bin_buf_arr_data[] = {1, 2, 3};

		std::list<int> out_list;
		yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				std::list<int>
			> buf_ref(out_list);
		iter_bs2.load("c_bson_bin_buffer_ref_int_list", buf_ref);

		std::vector<int> chk_vt(out_list.begin(), out_list.end());
		yggr_test_assert(0 == memcmp(&chk_vt[0], bin_buf_arr_data, sizeof(bin_buf_arr_data)));
		++iter_bs2;
	}

	{
		//int bin_buf_arr_data[] = {1, 2, 3};

		int out_native_vt[3] = {0};
		yggr::nsql_database_system::c_bson_binary_buffer_ref
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				int[3]
			> buf_ref(out_native_vt);

		iter_bs2.load("c_bson_bin_buffer_ref_native_int_vt", buf_ref);
		yggr_test_assert(0 == memcmp(out_native_vt, bin_buf_arr_data, sizeof(bin_buf_arr_data)));
		++iter_bs2;
	}

	// c_bson_binary_buffer
	{
		//int bin_buf_arr_data[] = {1, 2, 3};

		std::vector<int> out_vt;
		yggr::nsql_database_system::c_bson_binary_buffer
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY
			> buf;
		iter_bs2.load("c_bson_bin_buffer_int_vt", buf);

		yggr_test_assert(0 == buf.size() % sizeof(int));
		out_vt.resize(buf.size() / sizeof(int));
		memcpy(&out_vt[0], buf, buf.size());
		yggr_test_assert(0 == memcmp(&out_vt[0], bin_buf_arr_data, sizeof(bin_buf_arr_data)));
		++iter_bs2;
	}

	{
		//int bin_buf_arr_data[] = {1, 2, 3}; // save is list

		std::vector<int> out_vt;
		yggr::nsql_database_system::c_bson_binary_buffer
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY
			> buf;
		iter_bs2.load("c_bson_bin_buffer_int_list", buf);

		yggr_test_assert(0 == buf.size() % sizeof(int));
		out_vt.resize(buf.size() / sizeof(int));
		memcpy(&out_vt[0], buf, buf.size());
		yggr_test_assert(0 == memcmp(&out_vt[0], bin_buf_arr_data, sizeof(bin_buf_arr_data)));
		++iter_bs2;
	}

	{
		//int bin_buf_arr_data[] = {1, 2, 3}; // save is native_array

		int out_native_vt[3] = {0};
		yggr::nsql_database_system::c_bson_binary_buffer
			<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY
			> buf;
		iter_bs2.load("c_bson_bin_buffer_native_int_vt", buf);

		yggr_test_assert(0 == buf.size() % sizeof(int));
		memcpy(&out_native_vt[0], buf, buf.size());
		yggr_test_assert(0 == memcmp(&out_native_vt[0], bin_buf_arr_data, sizeof(bin_buf_arr_data)));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_null val;
		iter_bs2.load("c_bson_null", val);
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_undefined val;
		iter_bs2.load("c_bson_undefined", val);
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_regex val;
		iter_bs2.load("c_bson_regex", val);
#if (BSON_VERSION_HEX < 0x01070000)
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "regex_second");
#else
		yggr_test_assert(val.regex() == "regex_first" && val.options() == "sx");
#endif // BSON_VERSION_HEX < 0x01070000
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_code val;
		iter_bs2.load("c_bson_code", val);
		yggr_test_assert(val.code() == "this is a js code");
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_symbol val;
		iter_bs2.load("c_bson_symbol", val);
		yggr_test_assert(val.symbol() == "this is a symbol");
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_code_w_scope<bson_type::base_type> bson_val;
		yggr::nsql_database_system::c_bson_code_w_scope<bson_type> c_bson_val;

		iter_bs2.load("bson_code_w_scope", bson_val);
		yggr_test_assert(bson_val.code() == "this is a s bson_code");
		++iter_bs2;

		iter_bs2.load("c_bson_code_w_scope", c_bson_val);
		yggr_test_assert(c_bson_val.code() == "this is a s c_bson_code");
		++iter_bs2;

		{
			const bson_type* pbs = reinterpret_cast<const bson_type*>(&(bson_val.scope()));
			const bson_type& bs_val = *pbs;
			bson_iter_type iter_scope = bs_val.begin();
			{
				yggr::utf8_string val;
				iter_scope.load("scope", val);
				yggr_test_assert(val == "this is a bson_scope_data");
				++iter_scope;
			}
			yggr_test_assert(iter_scope == bs_val.end());
		}

		{
			bson_iter_type iter_scope = c_bson_val.scope().begin();
			{
				yggr::utf8_string val;
				iter_scope.load("scope", val);
				yggr_test_assert(val == "this is a c_bson_scope_data");
				++iter_scope;
			}
			yggr_test_assert(iter_scope == c_bson_val.scope().end());
		}
	}

	{
		yggr::nsql_database_system::c_bson_timestamp val;
		iter_bs2.load("c_bson_timestamp", val);
		yggr_test_assert(val == yggr::nsql_database_system::c_bson_timestamp(999, 1));
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_dbpointer<bson_oid_t> val_base;
		yggr::nsql_database_system::c_bson_dbpointer
			<
				yggr::nsql_database_system::c_bson_oid
			> val;

		iter_bs2.load("c_bson_dbpointer_base", val_base);
		++iter_bs2;
		iter_bs2.load("c_bson_dbpointer", val);

		yggr_test_assert(val_base.collection<yggr::utf8_string>() == "dbpointer");
		yggr_test_assert(val_base.oid() == chk_id);
		yggr_test_assert(val_base == val);
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_minkey minkey;
		iter_bs2.load("c_bson_minkey", minkey);
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_maxkey maxkey;
		iter_bs2.load("c_bson_maxkey", maxkey);
		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_value val;
		iter_bs2.load("c_bson_value", val);
		yggr_test_assert(val == yggr::nsql_database_system::c_bson_value("this is a c_bson_value"));

		std::string str;
		iter_bs2.load("c_bson_value", str);

		yggr_test_assert(str == "this is a c_bson_value");

		++iter_bs2;
	}

	{
		yggr::nsql_database_system::c_bson_value::org_type val;
		memset(&val, 0, sizeof(bson_value_t));
		iter_bs2.load("bson_value_t", val);

		yggr_test_assert((val == yggr::nsql_database_system::c_bson_value("this is a bson_value_t")));

		std::string str;
		iter_bs2.load("bson_value_t", str);

		yggr_test_assert(str == "this is a bson_value_t");

		bson_value_destroy(&val);
		++iter_bs2;
	}

	yggr_test_assert(iter_bs2 == bs2.end());

	std::cout << "test_save_load completed" << std::endl;
}

void save_begin_end_c_bson_test(void)
{
	yggr::nsql_database_system::c_bson bs;
	yggr::nsql_database_system::bson_mark_splice sp;

	bs.save_array_start("aaa", sp);
	bs.save("0", 10);
	bs.save("1", 20);
	bs.save_array_end(sp);

	std::cout << bs << std::endl;

	std::cout << "save_begin_end_c_bson_test completed" << std::endl;

}

void save_begin_end_bson_t_test(void)
{
	yggr::nsql_database_system::c_bson bs;
	yggr::nsql_database_system::bson_mark_splice sp;

	bson_append_array_begin(&bs, "aaa", -1, &(sp.bs_));;
	bson_append_int32(&sp.bs_, "0", -1, 10);
	bson_append_int32(&sp.bs_, "1", -1, 20);
	bson_append_array_end(&bs, &(sp.bs_));

	std::cout << bs << std::endl;

	std::cout << "save_begin_end_bson_t_test completed" << std::endl;
}

void save_begin_end_empty_c_bson_test(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef bson_type::mark_type mark_type;

	bson_type bs;
	mark_type sp;

	bs.save_array_start("aaa", sp);
	bs.save("0", 10);
	bs.save("1", 20);
	bs.save_array_end(sp);
	bs.save("bbb", 10);

	bson_append_document_begin(&bs, "ccc", -1, &(sp.bs_));
	bson_append_document_end(&bs, &(sp.bs_));

	const yggr::u8* pdata = bson_get_data(&bs);
	assert(pdata);
	for(bson_type::iterator i = bs.begin(), isize = bs.end(); i != isize; ++i)
	{
		std::cout << i.get_key<bson_type::inner_string_type>() << std::endl;
	}

	std::cout << bs << std::endl;

	{
		bson_type bs2, bs3(bs);
		std::cout << bs2 << std::endl;
		std::cout << bs3 << std::endl;

		bs2.swap(bs);
		bs.swap(bs2);
		bs.swap(bs2);

		std::cout << bs << std::endl;

		for(bson_type::iterator i = bs2.begin(), isize = bs2.end(); i != isize; ++i)
		{
			std::cout << i.get_key<bson_type::inner_string_type>() << std::endl;
			if(i.get_key<bson_type::inner_string_type>() == "aaa")
			{
				bson_type::iterator iter_arr = i.sub_array_iterator("aaa");
				std::cout << iter_arr.get_key<bson_type::inner_string_type>() << std::endl;
				++iter_arr;
				std::cout << iter_arr.get_key<bson_type::inner_string_type>() << std::endl;
			}
		}

		bs3.save("ddd", 100);
		bs3.swap(bs2);
		bs2.swap(bs3);

		std::cout << bs2 << std::endl;
		std::cout << bs3 << std::endl;
	}

	std::cout << "save_begin_end_empty_c_bson_test completed" << std::endl;
}

void test_nil_bson(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	bson_type bs(0), bs2(0), bs11("");
	yggr_test_assert(bs.is_nil());
	yggr_test_assert(bs2.is_nil());
	yggr_test_assert(!bs11.is_nil() && bs11.empty());
	bs.save("a", 1);

	std::cout << bs << std::endl;

	bs.clear_to_nil();
	yggr_test_assert(bs.empty());
	yggr_test_assert(bs.is_nil());

	bs2.save("b", 2);

	bson_type bs3(0);
	bs3.swap(bs2);

	std::cout << bs3 << std::endl;

	std::cout << "test_nil_bson completed" << std::endl;
}

void test_native_bson_save_start_end(void)
{
	bson_t bs = {0};
	bson_init(&bs);
	bson_t mark = {0}, mark2 = {0};

	bson_append_array_begin(&bs, "aaa", -1, &mark);
		bson_append_int64(&mark, "0", -1, 1);
		bson_append_int32(&mark, "1", -1, 1);
		bson_append_array_begin(&mark, "2", -1, &mark2);
			bson_append_int32(&mark2, "0", -1, 2);
			bson_append_int32(&mark2, "1", -1, 2);
		bson_append_array_end(&mark, &mark2);
	bson_append_array_end(&bs, &mark);

	std::cout << bs << std::endl;

#ifdef _DEBUG
	{	// watch break point
		bson_impl_alloc_t* p0 = reinterpret_cast<bson_impl_alloc_t*>(&bs);
		bson_impl_alloc_t* p1 = reinterpret_cast<bson_impl_alloc_t*>(&mark);
		bson_impl_alloc_t* p2 = reinterpret_cast<bson_impl_alloc_t*>(&mark2);
		int nnn = 0;
		assert(!nnn);
		assert((p0 && p1 && p2));
	}
#endif // _DEBUG

	std::cout << mark << std::endl;
	std::cout << mark2 << std::endl;

	bson_destroy(&bs);

	std::cout << "test_native_bson_save_start_end success" << std::endl;
}

void test_s_bson_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	bson_type bs;
	bson_type::mark_type mark, mark2, mark3, mark4;
	bs.save_array_start("aaa", mark);
		bs.save("0", static_cast<yggr::u64>(1));
		bs.save("1", static_cast<yggr::u32>(1));
		bs.save_array_start("2", mark2);
			bs.save("0", static_cast<yggr::u32>(2));
			bs.save("1", static_cast<yggr::u32>(2));
		bs.save_array_end(mark2);
		bs.save_array_start("3", mark3);
			bs.save("0", static_cast<yggr::u32>(3));
			bs.save("1", static_cast<yggr::u32>(3));
		bs.save_array_end(mark3);
	bs.save_array_end(mark);
	bs.save_array_start("ddd", mark4);
		bs.save("0", static_cast<yggr::u64>(4));
		bs.save("1", static_cast<yggr::u32>(4));
	bs.save_array_end(mark4);

	std::cout << bs << std::endl;

#ifdef _DEBUG
	{	// watch break point
		bson_impl_alloc_t* p0 = reinterpret_cast<bson_impl_alloc_t*>(&bs);
		bson_impl_alloc_t* p1 = reinterpret_cast<bson_impl_alloc_t*>(&mark.bs_);
		bson_impl_alloc_t* p2 = reinterpret_cast<bson_impl_alloc_t*>(&mark2.bs_);
		bson_impl_alloc_t* p3 = reinterpret_cast<bson_impl_alloc_t*>(&mark3.bs_);
		bson_impl_alloc_t* p4 = reinterpret_cast<bson_impl_alloc_t*>(&mark4.bs_);
		int nnn = 0;
		assert(!nnn);
		assert((p0 && p1 && p2 && p3 && p4));
	}
#endif // _DEBUG

	std::cout << mark.bs_ << std::endl;
	std::cout << mark2.bs_ << std::endl;
	std::cout << mark3.bs_ << std::endl;
	std::cout << mark4.bs_ << std::endl;

	yggr_test_assert(!(yggr::nsql_database_system::bson_native_ex::s_bson_mark_get_child(&bs)));
	yggr_test_assert(!(yggr::nsql_database_system::bson_native_ex::s_bson_mark_get_child(&mark.bs_)));
	yggr_test_assert(!(yggr::nsql_database_system::bson_native_ex::s_bson_mark_get_child(&mark2.bs_)));
	yggr_test_assert(!(yggr::nsql_database_system::bson_native_ex::s_bson_mark_get_child(&mark3.bs_)));
	yggr_test_assert(!(yggr::nsql_database_system::bson_native_ex::s_bson_mark_get_child(&mark4.bs_)));

	std::cout << "test_s_bson_swap success" << std::endl;
}

void test_bson_json_conv(void)
{
	{
		yggr::nsql_database_system::c_bson bs("{\"a\":1}");
		std::cout << bs << std::endl;

		std::string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs(L"{\"a\":1}");
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs(std::string("{\"a\":1}"));
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs(yggr::charset::make_string_charset_helper("{\"a\":1}"));
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	// -----------------------------------------------------------

#ifndef YGGR_NO_CWCHAR
	{
		yggr::nsql_database_system::c_bson bs(std::wstring(L"{\"a\":1}"));
		std::cout << bs << std::endl;

		std::string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs;
		bs = "{\"a\":1}";
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs;
		bs = L"{\"a\":1}";
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs;
		bs = std::string("{\"a\":1}");
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs;
		bs = std::wstring(L"{\"a\":1}");
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;

		std::string astr_bs = str_bs;
		std::cout << astr_bs << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs(yggr::charset::make_string_charset_helper(L"{\"a\":1}"));
		std::cout << bs << std::endl;

		yggr::utf8_string str_bs = bs;
		std::cout << str_bs << std::endl;
	}

#endif // YGGR_NO_CWCHAR
}

void find_descendant_test(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef bson_type::iterator bson_iter_type;

	yggr::nsql_database_system::c_bson bs("{\"aaa\" : {\"bbb\" : 2}}");

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant("aaa.bbb");

		yggr_test_assert(iters.first != bs.end());
		yggr_test_assert(iters.second != bs.end());

		yggr::s32 chk = 0;
		iters.second.load("bbb", chk);
		yggr_test_assert(iters.first.get_key() == "aaa");
		yggr_test_assert(chk == 2);
	}

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(noncase)>("aaa.bbb");

		yggr_test_assert(iters.first != bs.end());
		yggr_test_assert(iters.second != bs.end());

		yggr::s32 chk = 0;
		iters.second.load("bbb", chk);
		yggr_test_assert(iters.first.get_key() == "aaa");
		yggr_test_assert(chk == 2);
	}

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant("aAa.bbb");

		yggr_test_assert(iters.first == bs.end());
		yggr_test_assert(iters.second == bs.end());
	}

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(noncase)>("aAa.bbb");

		yggr_test_assert(iters.first == bs.end());
		yggr_test_assert(iters.second == bs.end());
	}


	{
		bson_iter_type iter(bs);
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>("aAa.bbb");

		yggr_test_assert(iters.first != bs.end());
		yggr_test_assert(iters.second != bs.end());

		yggr::s32 chk = 0;
		iters.second.load("bbb", chk);
		yggr_test_assert(iters.first.get_key() == "aaa");
		yggr_test_assert(chk == 2);
	}

	{
		bson_iter_type iter(bs);
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>("aca.bcb");

		yggr_test_assert(iters.first == bs.end());
		yggr_test_assert(iters.second == bs.end());
	}

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>("aAa.bbb");

		yggr_test_assert(iters.first != bs.end());
		yggr_test_assert(iters.second != bs.end());

		yggr::s32 chk = 0;
		iters.second.load("bbb", chk);
		yggr_test_assert(iters.first.get_key() == "aaa");
		yggr_test_assert(chk == 2);
	}

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>("aAa.bbb");

		yggr_test_assert(iters.first != bs.end());
		yggr_test_assert(iters.second != bs.end());

		yggr::s32 chk = 0;
		iters.second.load("bbb", chk);
		yggr_test_assert(iters.first.get_key() == "aaa");
		yggr_test_assert(chk == 2);
	}

	{
		bson_iter_type iter = bs.begin();
		std::pair<bson_iter_type, bson_iter_type> iters = iter.find_descendant<YGGR_BSON_ITER_FIND_CASE_TAG_TYPE(case)>("aca.bcb");

		yggr_test_assert(iters.first == bs.end());
		yggr_test_assert(iters.second == bs.end());
	}

	std::cout << "find_descendant_test success" << std::endl;
}

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs("{\"key\" : 1}");

		yggr_test_assert(bs == bs_chk);
	}

	// std::string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs(std::string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// boost::contianer::string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs(boost::container::string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs(yggr::string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::utf8_string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs(yggr::utf8_string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::string_view
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs(yggr::string_view("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::string_view
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs((yggr::charset::make_string_charset_helper(yggr::string_view("{\"key\" : 1}"))));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::utf8_string_view
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs(yggr::utf8_string_view("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs((yggr::charset::make_string_charset_helper(yggr::utf8_string_view("{\"key\" : 1}"))));

		yggr_test_assert(bs == bs_chk);
	}

#if !defined(YGGR_NO_CWCHAR)

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs(L"{\"Կ��\" : 1}");

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs(std::wstring(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs(boost::container::wstring(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs(yggr::wstring(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs(yggr::utf8_string(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs(yggr::wstring_view(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		yggr::utf8_string str_js(L"{\"Կ��\" : 1}");
		bson_type bs_chk(L"{\"Կ��\" : 1}");

		//bson_type bs(yggr::utf8_string_view(str_js)); // syntax error
		bson_type bs((yggr::utf8_string_view(str_js)));

		const bson_type& bs_cref((bs));

		yggr_test_assert(bs_cref == bs_chk);
		yggr_test_assert(bs == bs_chk);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_construct success" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = "{\"key\" : 1}";

		yggr_test_assert(bs == bs_chk);
	}

	// std::string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = (std::string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// boost::contianer::string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = (boost::container::string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = (yggr::string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::utf8_string
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = (yggr::utf8_string("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::string_view
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = (yggr::string_view("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = yggr::charset::make_string_charset_helper(yggr::string_view("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	// yggr::utf8_string_view
	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = (yggr::utf8_string_view("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk("{\"key\" : 1}");
		bson_type bs;

		bs = yggr::charset::make_string_charset_helper(yggr::utf8_string_view("{\"key\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

#if !defined(YGGR_NO_CWCHAR)

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = (L"{\"Կ��\" : 1}");

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = (std::wstring(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = (boost::container::wstring(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = (yggr::wstring(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = (yggr::utf8_string(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = (yggr::wstring_view(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		bson_type bs_chk(L"{\"Կ��\" : 1}");
		bson_type bs;

		bs = yggr::charset::make_string_charset_helper(yggr::wstring_view(L"{\"Կ��\" : 1}"));

		yggr_test_assert(bs == bs_chk);
	}

	{
		yggr::utf8_string str_js(L"{\"Կ��\" : 1}");
		bson_type bs_chk(L"{\"Կ��\" : 1}");

		bson_type bs;

		bs = ((yggr::utf8_string_view(str_js)));

		const bson_type& bs_cref((bs));

		yggr_test_assert(bs_cref == bs_chk);
		yggr_test_assert(bs == bs_chk);
	}

	{
		yggr::utf8_string str_js(L"{\"Կ��\" : 1}");
		bson_type bs_chk(L"{\"Կ��\" : 1}");

		bson_type bs;

		bs = yggr::charset::make_string_charset_helper((yggr::utf8_string_view(str_js)));

		const bson_type& bs_cref((bs));

		yggr_test_assert(bs_cref == bs_chk);
		yggr_test_assert(bs == bs_chk);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_operator_set success" << std::endl;
}

int main(int argc, char* argv[])
{
	// part1
	test_save_load();
	save_begin_end_c_bson_test();
	save_begin_end_bson_t_test();
	save_begin_end_empty_c_bson_test();
	test_nil_bson();

	test_native_bson_save_start_end();
	test_s_bson_swap();

	test_bson_json_conv();

	find_descendant_test();

	// part2
	test_construct();
	test_operator_set();

	wait_any_key(argc, argv);
	return 0;
}
