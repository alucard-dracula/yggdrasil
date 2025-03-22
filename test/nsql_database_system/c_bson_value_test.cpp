//c_bson_value_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/math/flaw_comparer.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_bson_value_conv(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_int32_to_value(&val, 100);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;

		{
			yggr::s32 chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_int32(chk, &val);
			yggr_test_assert(chk == 100);
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_int64_to_value(&val, 1000);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;

		{
			yggr::s64 chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_int64(chk, &val);
			yggr_test_assert(chk == 1000);
		}

		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_bool_to_value(&val, true);
		std::cout << val << std::endl;

		{
			bool chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_bool(chk, &val);
			yggr_test_assert(chk == true);
		}

		yggr::nsql_database_system::bson_value_native_ex::s_bool_to_value(&val, false);
		std::cout << val << std::endl;

		{
			bool chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_bool(chk, &val);
			yggr_test_assert(chk == false);
		}

		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_double_to_value(&val, 10000);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::f64 chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_double(chk, &val);
			yggr_test_assert(chk == 10000);
		}
		bson_value_destroy(&val);
	}

	{
	    typedef yggr::math::flaw_comparer<yggr::f64> f64_cmper_type;

		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_double_to_value(&val, 10000.5555);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::f64 chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_double(chk, &val);
			yggr_test_assert(chk == f64_cmper_type(10000.5555));
		}
		bson_value_destroy(&val);
	}

	{
		bson_decimal128_t bs_decmal128 = {1, 1};
		bson_value_t val = { static_cast<bson_type_t>(0) };
		yggr::nsql_database_system::bson_value_native_ex::s_decimal128_to_value(&val, &bs_decmal128);
		std::cout << val << std::endl;
		{
			bson_decimal128_t chk = {1, 1};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_decimal128(&chk, &val);
			yggr_test_assert(0 == memcmp(&chk, &bs_decmal128, sizeof(bson_decimal128_t)));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, "this is a const char*");
		std::cout << val << std::endl;
		{
			std::string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_string(chk, &val);
			yggr_test_assert(chk == "this is a const char*");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, L"this is a const wchar*");
		std::cout << val << std::endl;
		{
			std::wstring chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_string(chk, &val);
			yggr_test_assert(chk == L"this is a const wchar*");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, std::string("this is a string"));
		std::cout << val << std::endl;
		{
			std::string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_string(chk, &val);
			yggr_test_assert(chk == "this is a string");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, yggr::utf8_string("this is a utf8_string"));
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_string(chk, &val);
			yggr_test_assert(chk == "this is a utf8_string");
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_null_to_value(&val);
		std::cout << val << std::endl;
		{
			yggr_test_assert(yggr::nsql_database_system::bson_value_native_ex::s_value_to_null(&val));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_undefined_to_value(&val);
		std::cout << val << std::endl;
		{
			yggr_test_assert(yggr::nsql_database_system::bson_value_native_ex::s_value_to_undefined(&val));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_maxkey_to_value(&val);
		std::cout << val << std::endl;
		{
			yggr_test_assert(yggr::nsql_database_system::bson_value_native_ex::s_value_to_maxkey(&val));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_minkey_to_value(&val);
		std::cout << val << std::endl;
		{
			yggr_test_assert(yggr::nsql_database_system::bson_value_native_ex::s_value_to_minkey(&val));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_date_to_value(&val, 15);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::s64 chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_date(chk, &val);
			yggr_test_assert(chk == 15);
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_time_to_value(&val, 16);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::s64 chk = 0;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_time(chk, &val);
			yggr_test_assert(chk == 16);
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		timeval tmval = {17, 17000};
		yggr::nsql_database_system::bson_value_native_ex::s_timeval_to_value(&val, &tmval);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			timeval chk = {0};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_timeval(&chk, &val);
			yggr_test_assert(0 == memcmp(&chk, &tmval, sizeof(timeval)));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		bson_oid_t oid = {0};
		yggr::nsql_database_system::bson_value_native_ex::s_oid_to_value(&val, &oid);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			bson_oid_t chk = {1};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_oid(&chk, &val);
			yggr_test_assert(0 == memcmp(&chk, &oid, sizeof(bson_oid_t)));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		const char* pbuf = "this is a buffer";
		yggr::nsql_database_system
			::bson_value_native_ex
			::s_binary_buffer_to_value(&val, BSON_SUBTYPE_BINARY,
										reinterpret_cast<const yggr::u8*>(pbuf), strlen(pbuf));
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::u32 sub_t = 0;
			std::string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_binary_buffer(sub_t, chk, &val);
			yggr_test_assert((sub_t == BSON_SUBTYPE_BINARY && chk == "this is a buffer"));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_regex_to_value(&val, "this is a const char* regex",
																					L"this is a const wchar* opts");
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::wstring chk_regex;
			std::string chk_opts;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_regex(chk_regex, chk_opts, &val);
			yggr_test_assert(chk_regex == L"this is a const char* regex");
			yggr_test_assert(chk_opts == "is");
		}

		yggr::nsql_database_system::bson_value_native_ex::s_regex_to_value(&val, std::string("this is a string regex"),
																					yggr::utf8_string("this is a utf8_string opts"));

		{
			yggr::utf8_string chk_regex;
			std::wstring chk_opts;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_regex(chk_regex, chk_opts, &val);
			yggr_test_assert(chk_regex == "this is a string regex");
			yggr_test_assert(chk_opts == L"isu");
		}
		std::cout << val << std::endl;
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_code_to_value(&val, "this is a const char* code");
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::utf8_string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code(chk, &val);
			yggr_test_assert(chk == "this is a const char* code");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_code_to_value(&val, L"this is a const wchar* code");
		std::cout << val << std::endl;
		{
			yggr::string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code(chk, &val);
			yggr_test_assert(chk == "this is a const wchar* code");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_code_to_value(&val, std::string("this is a string code"));
		std::cout << val << std::endl;
		{
			yggr::string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code(chk, &val);
			yggr_test_assert(chk == "this is a string code");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_code_to_value(&val, yggr::utf8_string("this is a utf8_string code"));
		std::cout << val << std::endl;
		{
			yggr::wstring chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code(chk, &val);
			yggr_test_assert(chk == L"this is a utf8_string code");
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_symbol_to_value(&val, "this is a const char* symbol");
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::utf8_string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_symbol(chk, &val);
			yggr_test_assert(chk == "this is a const char* symbol");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_symbol_to_value(&val, L"this is a const wchar* symbol");
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_symbol(chk, &val);
			yggr_test_assert(chk == "this is a const wchar* symbol");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_symbol_to_value(&val, std::string("this is a string symbol"));
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_symbol(chk, &val);
			yggr_test_assert(chk == "this is a string symbol");
		}
		yggr::nsql_database_system::bson_value_native_ex::s_symbol_to_value(&val, yggr::utf8_string("this is a utf8_string symbol"));
		std::cout << val << std::endl;
		{
			std::string chk;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_symbol(chk, &val);
			yggr_test_assert(chk == "this is a utf8_string symbol");
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::c_bson scope("{ \"aaa\" : 1 }");
		yggr::nsql_database_system::bson_value_native_ex::s_code_w_scope_to_value(&val, "this is a const char* cws", &scope);
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk_code;
			yggr::nsql_database_system::c_bson chk_bs;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code_w_scope(chk_code, &chk_bs, &val);
			yggr_test_assert(chk_code == "this is a const char* cws");
			yggr_test_assert(chk_bs == scope);
		}
		yggr::nsql_database_system::bson_value_native_ex::s_code_w_scope_to_value(&val, L"this is a const wchar* cws", &scope);
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk_code;
			yggr::nsql_database_system::c_bson chk_bs;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code_w_scope(chk_code, &chk_bs, &val);
			yggr_test_assert(chk_code == "this is a const wchar* cws");
			yggr_test_assert(chk_bs == scope);
		}
		yggr::nsql_database_system::bson_value_native_ex::s_code_w_scope_to_value(&val, std::string("this is a string cws"), &scope);
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk_code;
			yggr::nsql_database_system::c_bson chk_bs;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code_w_scope(chk_code, &chk_bs, &val);
			yggr_test_assert(chk_code == "this is a string cws");
			yggr_test_assert(chk_bs == scope);
		}
		yggr::nsql_database_system::bson_value_native_ex::s_code_w_scope_to_value(&val, yggr::utf8_string("this is a utf8_string cws"), &scope);
		std::cout << val << std::endl;
		{
			std::string chk_code;
			yggr::nsql_database_system::c_bson chk_bs;
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_code_w_scope(chk_code, &chk_bs, &val);
			yggr_test_assert(chk_code == "this is a utf8_string cws");
			yggr_test_assert(chk_bs == scope);
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		bson_oid_t oid = {0};
		yggr::nsql_database_system::bson_value_native_ex::s_dbpointer_to_value(&val, "this is a const char* col", &oid);
		std::cout << val << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(val) << std::endl;
		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(val) << std::endl;
		{
			yggr::utf8_string chk_col;
			bson_oid_t chk_oid = {1};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_dbpointer(chk_col, &chk_oid, &val);
			yggr_test_assert(chk_col == "this is a const char* col");
			yggr_test_assert((0 == memcmp(&chk_oid, &oid, sizeof(bson_oid_t))));
		}
		yggr::nsql_database_system::bson_value_native_ex::s_dbpointer_to_value(&val, L"this is a const wchar* col", &oid);
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk_col;
			bson_oid_t chk_oid = {1};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_dbpointer(chk_col, &chk_oid, &val);
			yggr_test_assert(chk_col == "this is a const wchar* col");
			yggr_test_assert((0 == memcmp(&chk_oid, &oid, sizeof(bson_oid_t))));
		}
		yggr::nsql_database_system::bson_value_native_ex::s_dbpointer_to_value(&val, std::string("this is a string col"), &oid);
		std::cout << val << std::endl;
		{
			yggr::utf8_string chk_col;
			bson_oid_t chk_oid = {1};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_dbpointer(chk_col, &chk_oid, &val);
			yggr_test_assert(chk_col == "this is a string col");
			yggr_test_assert((0 == memcmp(&chk_oid, &oid, sizeof(bson_oid_t))));
		}
		yggr::nsql_database_system::bson_value_native_ex::s_dbpointer_to_value(&val, yggr::utf8_string("this is a utf8_string col"), &oid);
		std::cout << val << std::endl;
		{
			std::string chk_col;
			bson_oid_t chk_oid = {1};
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_dbpointer(chk_col, &chk_oid, &val);
			yggr_test_assert(chk_col == "this is a utf8_string col");
			yggr_test_assert((0 == memcmp(&chk_oid, &oid, sizeof(bson_oid_t))));
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::c_bson bs("{ \"aaa\" : 1 }");
		yggr::nsql_database_system::bson_value_native_ex::s_bson_to_value(&val, &bs);
		std::cout << val << std::endl;

		{
			yggr::nsql_database_system::c_bson chk(0);
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_bson(boost::addressof(chk), &val);
			yggr_test_assert(chk == bs);
		}
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::c_bson bs("{ \"aaa\" : [1, 2, 3] }");
		yggr::nsql_database_system::bson_value_native_ex::s_array_to_value(&val, &bs);
		std::cout << val << std::endl;
		{
			yggr::nsql_database_system::c_bson chk(0);
			yggr::nsql_database_system::bson_value_native_ex::s_value_to_array(boost::addressof(chk), &val);
			yggr_test_assert(chk == bs);
		}
		bson_value_destroy(&val);
	}

	std::cout << "test_bson_value_conv all success" << std::endl;
}

void test_bson_value_cmp(void)
{
	{
		yggr::s32 data = 100;
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_int32_to_value(&val, data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int32(&val, data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		yggr::s64 data = 1000;
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_int64_to_value(&val, data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_int64(&val, data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		bool data = 1000;
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_bool_to_value(&val, data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bool(&val, data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		yggr::f64 data = 10000.5555;
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_double_to_value(&val, data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_double(&val, data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		{
			const char* data = "this is a const char*";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, data);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(&val, data);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			const wchar_t* data = L"this is a const wchar*";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, data);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(&val, data);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			std::string data("this is a string");
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, data);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(&val, data);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			yggr::utf8_string data("this is a utf8_string");
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_string_to_value(&val, data);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_string(&val, data);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_null_to_value(&val);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_null(&val);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_undefined_to_value(&val);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_undefined(&val);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_maxkey_to_value(&val);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_maxkey(&val);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_minkey_to_value(&val);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_minkey(&val);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		yggr::s64 data = 15;
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_date_to_value(&val, data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_date(&val, data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		yggr::s64 data = 16;
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_time_to_value(&val, data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_time(&val, data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		timeval data = {17, 17000};
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_timeval_to_value(&val, &data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_timeval(&val, &data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		bson_oid_t data = {0};
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_oid_to_value(&val, &data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_oid(&val, &data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		const char* data = "this is a buffer";
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system
			::bson_value_native_ex
			::s_binary_buffer_to_value(&val, BSON_SUBTYPE_BINARY,
										reinterpret_cast<const yggr::u8*>(data), strlen(data));
		std::cout << val << std::endl;
		bool b =
			yggr::nsql_database_system
			::bson_value_native_ex
			::s_value_cmp_eq_binary_buffer(&val, BSON_SUBTYPE_BINARY,
											reinterpret_cast<const yggr::u8*>(data), strlen(data));
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		{
			const char* data_regex = "this is a const char* regex";
			const wchar_t* data_opts = L"this is a const wchar* opts";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_regex_to_value(&val, data_regex, data_opts);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_regex(&val, data_regex, data_opts);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			std::string data_regex = "this is a string regex";
			yggr::utf8_string data_opts = L"this is a utf8_string opts";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_regex_to_value(&val, data_regex, data_opts);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_regex(&val, data_regex, data_opts);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}
	}

	{
		{
			const char* data = "this is a const char* code";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_code_to_value(&val, data);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_code(&val, data);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			std::string data = "this is a string code";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_code_to_value(&val, data);
			std::cout << val << std::endl;
			bool b1 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_code(&val, data);
			bool b2 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_code(&val, yggr::utf8_string(data));
			yggr_test_assert(b1 && b2);
			bson_value_destroy(&val);
		}
	}

	{
		{
			const char* data = "this is a const char* symbol";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_symbol_to_value(&val, data);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_symbol(&val, data);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			std::string data = "this is a string symbol";
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_symbol_to_value(&val, data);
			std::cout << val << std::endl;
			bool b1 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_symbol(&val, data);
			bool b2 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_symbol(&val, yggr::utf8_string(data));
			yggr_test_assert(b1 && b2);
			bson_value_destroy(&val);
		}
	}

	{
		{
			const char* data_code = "this is a const char* cws";
			yggr::nsql_database_system::c_bson data_scope("{ \"aaa\" : 1 }");
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_code_w_scope_to_value(&val, data_code, &data_scope);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_code_w_scope(&val, data_code, &data_scope);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			std::string data_code = "this is a string cws";
			yggr::nsql_database_system::c_bson data_scope("{ \"aaa\" : 1 }");
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_code_w_scope_to_value(&val, data_code, &data_scope);
			std::cout << val << std::endl;
			bool b1 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_code_w_scope(&val, data_code, &data_scope);
			bool b2 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_code_w_scope(&val, yggr::utf8_string(data_code), &data_scope);
			yggr_test_assert(b1 && b2);
			bson_value_destroy(&val);
		}
	}

	{
		{
			const char* data_col = "this is a const char* col";
			bson_oid_t data_oid = {0};
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_dbpointer_to_value(&val, data_col, &data_oid);
			std::cout << val << std::endl;
			bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_dbpointer(&val, data_col, &data_oid);
			yggr_test_assert(b);
			bson_value_destroy(&val);
		}

		{
			std::string data_col = "this is a string col";
			bson_oid_t data_oid = {0};
			bson_value_t val = {static_cast<bson_type_t>(0)};
			yggr::nsql_database_system::bson_value_native_ex::s_dbpointer_to_value(&val, data_col, &data_oid);
			std::cout << val << std::endl;
			bool b1 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_dbpointer(&val, data_col, &data_oid);
			bool b2 = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_dbpointer(&val, yggr::utf8_string(data_col), &data_oid);
			yggr_test_assert(b1 && b2);
			bson_value_destroy(&val);
		}
	}

	{
		yggr::nsql_database_system::c_bson data("{ \"aaa\" : 1 }");
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_bson_to_value(&val, &data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_bson(&val, &data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	{
		yggr::nsql_database_system::c_bson data("{ \"aaa\" : [1, 2, 3] }");
		bson_value_t val = {static_cast<bson_type_t>(0)};
		yggr::nsql_database_system::bson_value_native_ex::s_array_to_value(&val, &data);
		std::cout << val << std::endl;
		bool b = yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_array(&val, &data);
		yggr_test_assert(b);
		bson_value_destroy(&val);
	}

	std::cout << "test_bson_value_cmp all success" << std::endl;
}

void test_c_bson_value(void)
{
	{
		{
			yggr::s32 data = 100;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(int32), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::s32 chk = 0;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(int32)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::s32 data = 100;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::s32, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::s64 data = 1000;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(int64), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::s64 chk = 0;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(int64)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::s64 data = 1000;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::s64, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			bool data = true;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(bool), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			bool chk = 0;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(bool)>(chk);
			yggr_test_assert(chk == data);

		}

		{
			bool data = false;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<bool, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::f64 data = 10000.5555;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(double), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::f64 chk = 0;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(double)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::f64 data = 10000.5555;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::f64, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}

		{
			bson_decimal128_t data = { 2, 2 };
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<bson_decimal128_t, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(0 == memcmp(&chk.first, &data, sizeof(bson_decimal128_t)));
		}

		{
			yggr::nsql_database_system::c_bson_decimal128 data(2, 2);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_decimal128, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			{
				const char* data = "this is a const char*";
				yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(utf8), data);
				std::cout << val << std::endl;

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				std::string chk;
				val.get_value<YGGR_BSON_VALUE_TAG_TYPE(utf8)>(chk);
				yggr_test_assert(chk == data);
			}

			{
				const char* data = "this is a const char*";
				yggr::nsql_database_system::c_bson_value val(data);
				std::cout << val << std::endl;

				yggr::nsql_database_system::c_bson_value val_cmp(data);
				yggr_test_assert((val == val_cmp));
				yggr_test_assert((!(val != val_cmp)));

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				std::pair<std::string, bool> chk = val;
				yggr_test_assert(chk.second);
				yggr_test_assert(chk.first == data);
				yggr_test_assert(data == chk.first);
			}
		}

		{
			{
				const wchar_t* data = L"this is a const wchar*";
				yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(utf8), data);
				std::cout << val << std::endl;

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				yggr::wstring chk;
				val.get_value<YGGR_BSON_VALUE_TAG_TYPE(utf8)>(chk);
				yggr_test_assert(chk == data);
			}

			{
				const wchar_t* data = L"this is a const wchar*";
				yggr::nsql_database_system::c_bson_value val(data);
				std::cout << val << std::endl;

				yggr::nsql_database_system::c_bson_value val_cmp(data);
				yggr_test_assert((val == val_cmp));
				yggr_test_assert((!(val != val_cmp)));

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				std::pair<yggr::wstring, bool> chk = val;
				yggr_test_assert(chk.second);
				yggr_test_assert(chk.first == data);
				yggr_test_assert(data == chk.first);
			}
		}

		{
			{
				std::string data = "this is a string";
				yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(utf8), data);
				std::cout << val << std::endl;

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				yggr::utf8_string chk;
				val.get_value<YGGR_BSON_VALUE_TAG_TYPE(utf8)>(chk);
				yggr_test_assert(chk == data);
			}

			{
				std::string data = "this is a string";
				yggr::nsql_database_system::c_bson_value val(data);
				std::cout << val << std::endl;

				yggr::nsql_database_system::c_bson_value val_cmp(data);
				yggr_test_assert((val == val_cmp));
				yggr_test_assert((!(val != val_cmp)));

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				std::pair<yggr::utf8_string, bool> chk = val;
				yggr_test_assert(chk.second);
				yggr_test_assert(chk.first == data);
				yggr_test_assert(data == chk.first);
			}
		}

		{
			{
				yggr::utf8_string data = "this is a utf8_string";
				yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(utf8), data);
				std::cout << val << std::endl;

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				yggr::utf8_string chk;
				val.get_value<YGGR_BSON_VALUE_TAG_TYPE(utf8)>(chk);
				yggr_test_assert(chk == data);
			}

			{
				yggr::utf8_string data = "this is a utf8_string";
				yggr::nsql_database_system::c_bson_value val(data);
				std::cout << val << std::endl;

				yggr::nsql_database_system::c_bson_value val_cmp(data);
				yggr_test_assert((val == val_cmp));
				yggr_test_assert((!(val != val_cmp)));

				yggr_test_assert((val == data));
				yggr_test_assert((!(val != data)));
				yggr_test_assert((data == val));
				yggr_test_assert((!(data != val)));

				std::pair<yggr::utf8_string, bool> chk = val;
				yggr_test_assert(chk.second);
				yggr_test_assert(chk.first == data);
				yggr_test_assert(data == chk.first);
			}
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_null data;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(null), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_null chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(null)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_null data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_null chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_null data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_null, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_undefined data;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(undefined), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_undefined chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(undefined)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_undefined data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_undefined chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_undefined data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_undefined, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_maxkey data;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(maxkey), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_maxkey chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(maxkey)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_maxkey data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_maxkey chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_maxkey data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_maxkey, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_minkey data;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(minkey), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_minkey chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(minkey)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_minkey data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_minkey chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_minkey data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_minkey, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_date data(15);
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(date_time), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_date chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(date_time)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_date data(15);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_date chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_date data(15);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_date, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}



	{
		{
			yggr::nsql_database_system::c_bson_time data(16);
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(time), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_time chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(time)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_time data(16);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_time chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_time data(16);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_time, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_timeval data(17, 17000);
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(timeval), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_timeval chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(timeval)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_timeval data(17, 17000);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_timeval chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_timeval data(17, 17000);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_timeval, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_oid data;
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(oid), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_oid chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(oid)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_oid data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_oid chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_oid::org_type data = {0};
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_oid::org_type, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}

		{
			yggr::nsql_database_system::c_bson_oid data;
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_oid, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> data("this is a buffer");
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(binary), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(binary)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> data("this is a buffer");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> data("this is a buffer");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_binary_buffer<
						yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY>, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}

	}

	{
		{
			yggr::string buf("this is a buffer");
			yggr::nsql_database_system::c_bson_binary_buffer_ref<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				yggr::string > data(buf);
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(binary), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(binary)>(chk);
			yggr_test_assert(chk == data.conv_to_binary_buffer());
		}

		{
			yggr::string buf("this is a buffer");
			yggr::nsql_database_system::c_bson_binary_buffer_ref<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				yggr::string > data(buf);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_binary_buffer<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> chk(val);
			yggr_test_assert(chk == data.conv_to_binary_buffer());
		}

		{
			yggr::string buf("this is a buffer");
			yggr::nsql_database_system::c_bson_binary_buffer_ref<
				yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY,
				yggr::string > data(buf);
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_binary_buffer<
						yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY>, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data.conv_to_binary_buffer());
			yggr_test_assert(data.conv_to_binary_buffer() == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_regex data("this is a const char* regex", "this is a const wchar* opts");
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(regex), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_regex chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(regex)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_regex data("this is a const char* regex", "this is a const wchar* opts");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_regex chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_regex data("this is a const char* regex", "this is a const wchar* opts");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_regex, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_code data("this is a const char* code");
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(code), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_code chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(code)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_code data("this is a const char* code");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_code chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_code data("this is a const char* code");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_code, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_symbol data("this is a const char* symbol");
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(symbol), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_symbol chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(symbol)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_symbol data("this is a const char* symbol");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_symbol chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_symbol data("this is a const char* symbol");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_symbol, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_code_w_scope<
				yggr::nsql_database_system::c_bson> data("this is a const char* cws",
															yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_code_w_scope<
				yggr::nsql_database_system::c_bson> chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(codewscope)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<
				yggr::nsql_database_system::c_bson> data("this is a const char* cws",
															yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_code_w_scope<
				yggr::nsql_database_system::c_bson> chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<
				yggr::nsql_database_system::c_bson> data("this is a const char* cws",
															yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_code_w_scope<
						yggr::nsql_database_system::c_bson>, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson_dbpointer<
				yggr::nsql_database_system::c_bson_oid> data("this is a const char* col",
																yggr::nsql_database_system::c_bson_oid());
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_dbpointer<
				yggr::nsql_database_system::c_bson_oid > chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(dbpointer)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_dbpointer<
				yggr::nsql_database_system::c_bson_oid> data("this is a const char* col",
																yggr::nsql_database_system::c_bson_oid());
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson_dbpointer<
				yggr::nsql_database_system::c_bson_oid > chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson_dbpointer<
				yggr::nsql_database_system::c_bson_oid> data("this is a const char* col",
																yggr::nsql_database_system::c_bson_oid());
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson_dbpointer<
						yggr::nsql_database_system::c_bson_oid>, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson data("{ \"aaa\" : 1 }");
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(document), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(document)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson data("{ \"aaa\" : 1 }");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson data("{ \"aaa\" : 1 }");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	{
		{
			yggr::nsql_database_system::c_bson data("{ \"aaa\" : [1, 2, 3] }");
			yggr::nsql_database_system::c_bson_value val(YGGR_BSON_VALUE_TAG_OBJECT(array), data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson chk;
			val.get_value<YGGR_BSON_VALUE_TAG_TYPE(array)>(chk);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson data("{ \"aaa\" : [1, 2, 3] }");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			yggr::nsql_database_system::c_bson chk(val);
			yggr_test_assert(chk == data);
		}

		{
			yggr::nsql_database_system::c_bson data("{ \"aaa\" : [1, 2, 3] }");
			yggr::nsql_database_system::c_bson_value val(data);
			std::cout << val << std::endl;

			yggr::nsql_database_system::c_bson_value val_cmp(data);
			yggr_test_assert((val == val_cmp));
			yggr_test_assert((!(val != val_cmp)));

			yggr_test_assert((val == data));
			yggr_test_assert((!(val != data)));
			yggr_test_assert((data == val));
			yggr_test_assert((!(data != val)));

			std::pair<yggr::nsql_database_system::c_bson, bool> chk = val;
			yggr_test_assert(chk.second);
			yggr_test_assert(chk.first == data);
			yggr_test_assert(data == chk.first);
		}
	}

	std::cout << "test_c_bson_value all success" << std::endl;
}

int main(int argc, char* argv[])
{
	test_bson_value_conv();
	test_bson_value_cmp();
	test_c_bson_value();

	wait_any_key(argc, argv);
	return 0;
}
