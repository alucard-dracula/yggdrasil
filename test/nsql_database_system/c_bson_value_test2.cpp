//c_bson_value_test2.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// E_BSON_TYPE_EOD
	{
		bson_value_type bs_val;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_EOD );

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_INT32
	{
		{
			bson_value_type bs_val(1);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
			yggr_test_assert(bs_val == 1);

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(YGGR_BSON_VALUE_TAG_OBJECT(int32), 1);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
			yggr_test_assert(bs_val == 1);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_INT64
	{
		{
			bson_value_type bs_val(INT64_C(1));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT64 );
			yggr_test_assert(bs_val == INT64_C(1));

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(YGGR_BSON_VALUE_TAG_OBJECT(int64), 1);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT64 );
			yggr_test_assert(bs_val == INT64_C(1));

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_BOOL
	{
		bson_value_type bs_val(true);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BOOL );
		yggr_test_assert(bs_val == true);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_DOUBLE
	{
		{
			bson_value_type bs_val(1.0f);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
			yggr_test_assert(bs_val == 1.0f);

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(1.0);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
			yggr_test_assert(bs_val == 1.0);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_UTF8
	{
		{
			bson_value_type bs_val("this is a const char*");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a const char*");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(std::string("this is a std::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a std::string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(boost::container::string("this is a boost::container::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a boost::container::string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::string("this is a yggr::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::utf8_string("this is a yggr::utf8_string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::utf8_string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::string_view("this is a string_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a string_view");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::utf8_string_view("this is a utf8_string_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a utf8_string_view");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper("this is a const char*"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a const char*");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(std::string("this is a std::string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a std::string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(boost::container::string("this is a boost::container::string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a boost::container::string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(yggr::string("this is a yggr::string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(yggr::utf8_string("this is a yggr::utf8_string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::utf8_string");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(yggr::string_view("this is a string_view")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a string_view");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("this is a utf8_string_view")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a utf8_string_view");

			std::cout << bs_val << std::endl;
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_value_type bs_val(L"this is a const wchar_t*");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a const wchar_t*");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(std::wstring(L"this is a std::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a std::wstring");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(boost::container::wstring(L"this is a boost::container::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a boost::container::wstring");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::wstring(L"this is a yggr::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a yggr::wstring");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::wstring_view(L"this is a wstring_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a wstring_view");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(L"this is a const wchar_t*"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a const wchar_t*");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(std::wstring(L"this is a std::wstring")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a std::wstring");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(boost::container::wstring(L"this is a boost::container::wstring")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a boost::container::wstring");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(yggr::wstring(L"this is a yggr::wstring")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a yggr::wstring");

			std::cout << bs_val << std::endl;
		}

		{
			bson_value_type bs_val(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"this is a wstring_view")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a wstring_view");

			std::cout << bs_val << std::endl;
		}
#endif // YGGR_NO_CWCHAR
	}

	// E_BSON_TYPE_NULL
	{
		yggr::nsql_database_system::c_bson_null bs_null;
		bson_value_type bs_val(bs_null);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_NULL);
		yggr_test_assert(bs_val == bs_null);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_UNDEFINED
	{
		yggr::nsql_database_system::c_bson_undefined bs_undefined;
		bson_value_type bs_val(bs_undefined);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UNDEFINED);
		yggr_test_assert(bs_val == bs_undefined);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_MAXKEY
	{
		yggr::nsql_database_system::c_bson_maxkey bs_maxkey;
		bson_value_type bs_val(bs_maxkey);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MAXKEY);
		yggr_test_assert(bs_val == bs_maxkey);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_MINKEY
	{
		yggr::nsql_database_system::c_bson_minkey bs_minkey;
		bson_value_type bs_val(bs_minkey);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MINKEY);
		yggr_test_assert(bs_val == bs_minkey);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_DATE_TIME
	{
		{
			yggr::nsql_database_system::c_bson_date bs_date(1000);
			bson_value_type bs_val(bs_date);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_date);

			std::cout << bs_val << std::endl;
		}

		{
			yggr::nsql_database_system::c_bson_time bs_time(1);
			bson_value_type bs_val(bs_time);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_time);

			std::cout << bs_val << std::endl;
		}

		{
			{
				yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
				bson_value_type bs_val(static_cast<timeval&>(bs_timeval));

				yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
				yggr_test_assert(bs_val == bs_timeval);

				std::cout << bs_val << std::endl;
			}

			{
				yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
				bson_value_type bs_val(bs_timeval);

				yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
				yggr_test_assert(bs_val == bs_timeval);

				std::cout << bs_val << std::endl;
			}
		}
	}

	// E_BSON_TYPE_OID
	{
		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val(static_cast<bson_oid_t&>(bs_oid));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
			yggr_test_assert(bs_val == bs_oid);

			std::cout << bs_val << std::endl;
		}

		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val(bs_oid);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
			yggr_test_assert(bs_val == bs_oid);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_DECIMAL128
	{
		{
			yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
			bson_value_type bs_val(static_cast<bson_decimal128_t&>(bs_dec128));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
			yggr_test_assert(bs_val == bs_dec128);

			std::cout << bs_val << std::endl;
		}

		{
			yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
			bson_value_type bs_val(bs_dec128);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
			yggr_test_assert(bs_val == bs_dec128);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_BINARY
	{
		{
			int arr[] = {1, 2, 3};
			yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin(arr);
			bson_value_type bs_val(bs_bin);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);

			std::cout << bs_val << std::endl;
		}

		{
			typedef yggr::vector<int> buffer_type;

			int arr[] = {1, 2, 3};
			buffer_type buf(arr + 0, arr + 3);
			yggr::nsql_database_system::c_bson_binary_buffer_ref<10, buffer_type> bs_bin(buf);
			bson_value_type bs_val(bs_bin);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_REGEX
	{
		yggr::nsql_database_system::c_bson_regex bs_reg("this is a bson regex", "isx");
		bson_value_type bs_val(bs_reg);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_REGEX);
		yggr_test_assert(bs_val == bs_reg);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_CODE
	{
		yggr::nsql_database_system::c_bson_code bs_code("this is a bson code");
		bson_value_type bs_val(bs_code);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODE);
		yggr_test_assert(bs_val == bs_code);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_SYMBOL
	{
		yggr::nsql_database_system::c_bson_symbol bs_symbol("this is a bson symbol");
		bson_value_type bs_val(bs_symbol);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_SYMBOL);
		yggr_test_assert(bs_val == bs_symbol);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_CODEWSCOPE
	{
		{
			yggr::nsql_database_system::c_bson_code_w_scope<bson_t>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val(bs_cws);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);

			std::cout << bs_val << std::endl;
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val(bs_cws);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_TIMESTAMP
	{
		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val(static_cast<const yggr::nsql_database_system::c_bson_timestamp::base_type&>(bs_tmstamp));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);

			std::cout << bs_val << std::endl;
		}

		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val(bs_tmstamp);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_DBPOINTER
	{
		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type::org_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val(bs_dbptr);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);

			std::cout << bs_val << std::endl;
		}

		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val(bs_dbptr);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);

			std::cout << bs_val << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : 1 }");
		bson_value_type bs_val(bs);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOCUMENT);
		yggr_test_assert(bs_val == bs);

		std::cout << bs_val << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : [1, 2, 3] }");
		bson_value_type bs_val(bs);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_ARRAY);
		yggr_test_assert(bs_val == bs);

		std::cout << bs_val << std::endl;
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type::org_type& bs_val_org_smp(bs_val_smp);
		bson_value_type bs_val(boost::move(bs_val_org_smp));

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);

		std::cout << bs_val << std::endl;
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type::org_type& bs_val_org_smp(bs_val_smp);
		bson_value_type bs_val(bs_val_org_smp);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
		yggr_test_assert(bs_val == bs_val_org_smp);

		std::cout << bs_val << std::endl;
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type bs_val(boost::move(bs_val_smp));

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);

		std::cout << bs_val << std::endl;
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type::org_type& bs_val_org_smp(bs_val_smp);
		bson_value_type bs_val(bs_val_org_smp);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
		yggr_test_assert(bs_val == bs_val_smp);

		std::cout << bs_val << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// E_BSON_TYPE_INT32
	{
		bson_value_type bs_val;
		bs_val = 1;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
	}

	// E_BSON_TYPE_INT64
	{
		bson_value_type bs_val;
		bs_val = INT64_C(1);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT64 );
		yggr_test_assert(bs_val == INT64_C(1));
	}

	// E_BSON_TYPE_BOOL
	{
		bson_value_type bs_val;
		bs_val = true;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BOOL );
		yggr_test_assert(bs_val == true);
	}

	// E_BSON_TYPE_DOUBLE
	{
		{
			bson_value_type bs_val;
			bs_val = 1.0f;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
			yggr_test_assert(bs_val == 1.0f);
		}

		{
			bson_value_type bs_val;
			bs_val = 1.0;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
			yggr_test_assert(bs_val == 1.0);
		}
	}

	// E_BSON_TYPE_UTF8
	{
		{
			bson_value_type bs_val;
			bs_val = "this is a const char*";

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a const char*");
		}

		{
			bson_value_type bs_val;
			bs_val = std::string("this is a std::string");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a std::string");
		}

		{
			bson_value_type bs_val;
			bs_val = boost::container::string("this is a boost::container::string");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a boost::container::string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::string("this is a yggr::string");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::utf8_string("this is a yggr::utf8_string");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::utf8_string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::string_view("this is a string_view");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a string_view");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::utf8_string_view("this is a utf8_string_view");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a utf8_string_view");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper("this is a const char*");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a const char*");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(std::string("this is a std::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a std::string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(boost::container::string("this is a boost::container::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a boost::container::string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(yggr::string("this is a yggr::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(yggr::utf8_string("this is a yggr::utf8_string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::utf8_string");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(yggr::string_view("this is a string_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a string_view");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(yggr::utf8_string_view("this is a utf8_string_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a utf8_string_view");
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_value_type bs_val;
			bs_val = L"this is a const wchar_t*";

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a const wchar_t*");
		}

		{
			bson_value_type bs_val;
			bs_val = std::wstring(L"this is a std::wstring");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a std::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val = boost::container::wstring(L"this is a boost::container::wstring");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a boost::container::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::wstring(L"this is a yggr::wstring");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a yggr::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::wstring_view(L"this is a wstring_view");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a wstring_view");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(L"this is a const wchar_t*");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a const wchar_t*");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(std::wstring(L"this is a std::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a std::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(boost::container::wstring(L"this is a boost::container::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a boost::container::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(yggr::wstring(L"this is a yggr::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a yggr::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val = yggr::charset::make_string_charset_helper(yggr::wstring_view(L"this is a wstring_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a wstring_view");
		}
#endif // YGGR_NO_CWCHAR
	}

	// E_BSON_TYPE_NULL
	{
		yggr::nsql_database_system::c_bson_null bs_null;
		bson_value_type bs_val;
		bs_val = bs_null;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_NULL);
		yggr_test_assert(bs_val == bs_null);
	}

	// E_BSON_TYPE_UNDEFINED
	{
		yggr::nsql_database_system::c_bson_undefined bs_undefined;
		bson_value_type bs_val;
		bs_val = bs_undefined;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UNDEFINED);
		yggr_test_assert(bs_val == bs_undefined);
	}

	// E_BSON_TYPE_MAXKEY
	{
		yggr::nsql_database_system::c_bson_maxkey bs_maxkey;
		bson_value_type bs_val;
		bs_val = bs_maxkey;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MAXKEY);
		yggr_test_assert(bs_val == bs_maxkey);
	}

	// E_BSON_TYPE_MINKEY
	{
		yggr::nsql_database_system::c_bson_minkey bs_minkey;
		bson_value_type bs_val;
		bs_val = bs_minkey;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MINKEY);
		yggr_test_assert(bs_val == bs_minkey);
	}

	// E_BSON_TYPE_DATE_TIME
	{
		{
			yggr::nsql_database_system::c_bson_date bs_date(1000);
			bson_value_type bs_val;
			bs_val = bs_date;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_time bs_time(1);
			bson_value_type bs_val;
			bs_val = bs_time;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_time);
		}

		{
			{
				yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
				bson_value_type bs_val;
				bs_val = static_cast<timeval&>(bs_timeval);

				yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
				yggr_test_assert(bs_val == bs_timeval);
			}

			{
				yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
				bson_value_type bs_val;
				bs_val = bs_timeval;

				yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
				yggr_test_assert(bs_val == bs_timeval);
			}
		}
	}

	// E_BSON_TYPE_OID
	{
		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val;
			bs_val = static_cast<bson_oid_t&>(bs_oid);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
			yggr_test_assert(bs_val == bs_oid);
		}

		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val;
			bs_val = bs_oid;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
			yggr_test_assert(bs_val == bs_oid);
		}
	}

	// E_BSON_TYPE_DECIMAL128
	{
		{
			yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
			bson_value_type bs_val;
			bs_val = (static_cast<bson_decimal128_t&>(bs_dec128));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
			yggr_test_assert(bs_val == bs_dec128);
		}

		{
			yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
			bson_value_type bs_val;
			bs_val = (bs_dec128);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
			yggr_test_assert(bs_val == bs_dec128);
		}
	}

	// E_BSON_TYPE_BINARY
	{
		{
			int arr[] = {1, 2, 3};
			yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin(arr);
			bson_value_type bs_val;
			bs_val = bs_bin;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);
		}

		{
			typedef yggr::vector<int> buffer_type;

			int arr[] = {1, 2, 3};
			buffer_type buf(arr + 0, arr + 3);
			yggr::nsql_database_system::c_bson_binary_buffer_ref<10, buffer_type> bs_bin(buf);
			bson_value_type bs_val;
			bs_val = bs_bin;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);
		}
	}

	// E_BSON_TYPE_REGEX
	{
		yggr::nsql_database_system::c_bson_regex bs_reg("this is a bson regex", "isx");
		bson_value_type bs_val;
		bs_val = bs_reg;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_REGEX);
		yggr_test_assert(bs_val == bs_reg);
	}

	// E_BSON_TYPE_CODE
	{
		yggr::nsql_database_system::c_bson_code bs_code("this is a bson code");
		bson_value_type bs_val;
		bs_val = bs_code;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODE);
		yggr_test_assert(bs_val == bs_code);
	}

	// E_BSON_TYPE_SYMBOL
	{
		yggr::nsql_database_system::c_bson_symbol bs_symbol("this is a bson symbol");
		bson_value_type bs_val;
		bs_val = bs_symbol;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_SYMBOL);
		yggr_test_assert(bs_val == bs_symbol);

		std::cout << bs_val << std::endl;
	}

	// E_BSON_TYPE_CODEWSCOPE
	{
		{
			yggr::nsql_database_system::c_bson_code_w_scope<bson_t>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val;
			bs_val = bs_cws;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);

			std::cout << bs_val << std::endl;
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val;
			bs_val = bs_cws;

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);

			std::cout << bs_val << std::endl;
		}
	}

	// E_BSON_TYPE_TIMESTAMP
	{
		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val;
			bs_val = (static_cast<const yggr::nsql_database_system::c_bson_timestamp::base_type&>(bs_tmstamp));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);
		}

		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val;
			bs_val = (bs_tmstamp);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);
		}
	}

	// E_BSON_TYPE_DBPOINTER
	{
		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type::org_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val;
			bs_val = (bs_dbptr);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);
		}

		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val;
			bs_val = (bs_dbptr);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : 1 }");
		bson_value_type bs_val;
		bs_val = bs;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOCUMENT);
		yggr_test_assert(bs_val == bs);
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : [1, 2, 3] }");
		bson_value_type bs_val;
		bs_val = bs;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_ARRAY);
		yggr_test_assert(bs_val == bs);
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type::org_type& bs_val_org_smp(bs_val_smp);
		bson_value_type bs_val;
		bs_val = boost::move(bs_val_org_smp);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type::org_type& bs_val_org_smp(bs_val_smp);
		bson_value_type bs_val;
		bs_val = bs_val_org_smp;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
		yggr_test_assert(bs_val == bs_val_org_smp);
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type bs_val;
		bs_val = boost::move(bs_val_smp);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
	}

	{
		bson_value_type bs_val_smp(1);
		bson_value_type::org_type& bs_val_org_smp(bs_val_smp);
		bson_value_type bs_val;
		bs_val = bs_val_org_smp;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
		yggr_test_assert(bs_val == bs_val_smp);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_value_type org_bs_val1(1);
		bson_value_type org_bs_val2("this is a string");

		bson_value_type bs_val1(org_bs_val1);
		bson_value_type bs_val2(org_bs_val2);

		//bson_value_type& base_bs_val1_ref = bs_val1;
		bson_value_type& base_bs_val2_ref = bs_val2;

		bs_val1.swap(boost::move(base_bs_val2_ref));

		yggr_test_assert(bs_val1 == org_bs_val2);
		yggr_test_assert(bs_val2 == org_bs_val1);

		bs_val1.swap(base_bs_val2_ref);

		yggr_test_assert(bs_val1 == org_bs_val1);
		yggr_test_assert(bs_val2 == org_bs_val2);

		bs_val1.swap(boost::move(bs_val2));

		yggr_test_assert(bs_val1 == org_bs_val2);
		yggr_test_assert(bs_val2 == org_bs_val1);

		bs_val1.swap(bs_val2);

		yggr_test_assert(bs_val1 == org_bs_val1);
		yggr_test_assert(bs_val2 == org_bs_val2);
	}

	{
		bson_value_type org_bs_val1(1);
		bson_value_type org_bs_val2("this is a string");

		bson_value_type bs_val1(org_bs_val1);
		bson_value_type bs_val2(org_bs_val2);

		bson_value_type& base_bs_val1_ref = bs_val1;
		bson_value_type& base_bs_val2_ref = bs_val2;

		// base - base
		{
			std::swap(base_bs_val1_ref, boost::move(base_bs_val2_ref));

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			std::swap(boost::move(base_bs_val1_ref), base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			std::swap(base_bs_val1_ref, base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);
		}

		// base - child
		{
			std::swap(base_bs_val1_ref, boost::move(bs_val2));

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			std::swap(boost::move(base_bs_val1_ref), bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			std::swap(base_bs_val1_ref, bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);
		}

		// child - base
		{
			std::swap(bs_val1, boost::move(base_bs_val2_ref));

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			std::swap(boost::move(bs_val1), base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			std::swap(bs_val1, base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);
		}

		// child - child
		{
			std::swap(bs_val1, boost::move(bs_val2));

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			std::swap(boost::move(bs_val1), bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			std::swap(bs_val1, bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);
		}
	}

	{
		bson_value_type org_bs_val1(1);
		bson_value_type org_bs_val2("this is a string");

		bson_value_type bs_val1(org_bs_val1);
		bson_value_type bs_val2(org_bs_val2);

		bson_value_type& base_bs_val1_ref = bs_val1;
		bson_value_type& base_bs_val2_ref = bs_val2;

		// base - base
		{
			boost::swap(base_bs_val1_ref, boost::move(base_bs_val2_ref));

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			boost::swap(boost::move(base_bs_val1_ref), base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			boost::swap(base_bs_val1_ref, base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);
		}

		// base - child
		{
			boost::swap(base_bs_val1_ref, boost::move(bs_val2));

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			boost::swap(boost::move(base_bs_val1_ref), bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			boost::swap(base_bs_val1_ref, bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);
		}

		// child - base
		{
			boost::swap(bs_val1, boost::move(base_bs_val2_ref));

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			boost::swap(boost::move(bs_val1), base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			boost::swap(bs_val1, base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);
		}

		// child - child
		{
			boost::swap(bs_val1, boost::move(bs_val2));

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			boost::swap(boost::move(bs_val1), bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			boost::swap(bs_val1, bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);
		}
	}

	{
		bson_value_type org_bs_val1(1);
		bson_value_type org_bs_val2("this is a string");

		bson_value_type bs_val1(org_bs_val1);
		bson_value_type bs_val2(org_bs_val2);

		bson_value_type& base_bs_val1_ref = bs_val1;
		bson_value_type& base_bs_val2_ref = bs_val2;

		// base - base
		{
			yggr::swap(base_bs_val1_ref, boost::move(base_bs_val2_ref));

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			yggr::swap(boost::move(base_bs_val1_ref), base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			yggr::swap(base_bs_val1_ref, base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);
		}

		// base - child
		{
			yggr::swap(base_bs_val1_ref, boost::move(bs_val2));

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			yggr::swap(boost::move(base_bs_val1_ref), bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			yggr::swap(base_bs_val1_ref, bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);
		}

		// child - base
		{
			yggr::swap(bs_val1, boost::move(base_bs_val2_ref));

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			yggr::swap(boost::move(bs_val1), base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			yggr::swap(bs_val1, base_bs_val2_ref);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);
		}

		// child - child
		{
			yggr::swap(bs_val1, boost::move(bs_val2));

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);

			yggr::swap(boost::move(bs_val1), bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val2);
			yggr_test_assert(bs_val2 == org_bs_val1);

			yggr::swap(bs_val1, bs_val2);

			yggr_test_assert(bs_val1 == org_bs_val1);
			yggr_test_assert(bs_val2 == org_bs_val2);
		}
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo_operator_pair(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// E_BSON_TYPE_EOD
	{
		bson_value_type bs_val;

		std::pair<yggr::s32, bool> ret = bs_val;
		yggr_test_assert(!ret.second);
	}

	// E_BSON_TYPE_INT32
	{
		bson_value_type bs_val(1);

		{
			std::pair<yggr::s32, bool> ret = bs_val;

			yggr_test_assert(ret.first == 1);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_INT64
	{
		bson_value_type bs_val(INT64_C(1));

		{
			std::pair<yggr::s64, bool> ret = bs_val;

			yggr_test_assert(ret.first == 1);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_BOOL
	{
		bson_value_type bs_val(true);

		{
			std::pair<bool, bool> ret = bs_val;

			yggr_test_assert(ret.first == true);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::s32, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_DOUBLE
	{
		bson_value_type bs_val(1.0f);

		{
			std::pair<yggr::f32, bool> ret = bs_val;
			yggr_test_assert(ret.first == 1.0f);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::f64, bool> ret = bs_val;
			yggr_test_assert(ret.first == 1.0f);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_DECIMAL128
	{
		yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
		bson_value_type bs_val(bs_dec128);

		{
			std::pair<bson_decimal128_t, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_dec128);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::nsql_database_system::c_bson_decimal128, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_dec128);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}

	}

	// E_BSON_TYPE_UTF8
	{
		bson_value_type bs_val("this is a utf8_string");

		{
			std::pair<yggr::utf8_string, bool> ret = bs_val;
			yggr_test_assert(ret.first == "this is a utf8_string");
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::utf8_string_view, bool> ret = bs_val;
			yggr_test_assert(ret.first == "this is a utf8_string");
			yggr_test_assert(ret.second);
		}

		{
			std::pair<std::string, bool> ret = bs_val;
			yggr_test_assert(ret.first == "this is a utf8_string");
			yggr_test_assert(ret.second);
		}

		{
			std::pair<boost::container::string, bool> ret = bs_val;
			yggr_test_assert(ret.first == "this is a utf8_string");
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::string, bool> ret = bs_val;
			yggr_test_assert(ret.first == "this is a utf8_string");
			yggr_test_assert(ret.second);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			std::pair<std::wstring, bool> ret = bs_val;
			yggr_test_assert(ret.first == L"this is a utf8_string");
			yggr_test_assert(ret.second);
		}

		{
			std::pair<boost::container::wstring, bool> ret = bs_val;
			yggr_test_assert(ret.first == L"this is a utf8_string");
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::wstring, bool> ret = bs_val;
			yggr_test_assert(ret.first == L"this is a utf8_string");
			yggr_test_assert(ret.second);
		}
#endif // YGGR_NO_CWCHAR

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_NULL
	{
		yggr::nsql_database_system::c_bson_null bs_null;
		bson_value_type bs_val(bs_null);

		{
			std::pair<yggr::nsql_database_system::c_bson_null, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_null);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_UNDEFINED
	{
		yggr::nsql_database_system::c_bson_undefined bs_undefined;
		bson_value_type bs_val(bs_undefined);

		{
			std::pair<yggr::nsql_database_system::c_bson_undefined, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_undefined);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_MAXKEY
	{
		yggr::nsql_database_system::c_bson_maxkey bs_maxkey;
		bson_value_type bs_val(bs_maxkey);

		{
			std::pair<yggr::nsql_database_system::c_bson_maxkey, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_maxkey);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_MINKEY
	{
		yggr::nsql_database_system::c_bson_minkey bs_minkey;
		bson_value_type bs_val(bs_minkey);

		{
			std::pair<yggr::nsql_database_system::c_bson_minkey, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_minkey);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_DATE_TIME
	{
		{
			yggr::nsql_database_system::c_bson_date bs_date(1000);
			bson_value_type bs_val(bs_date);

			{
				std::pair<yggr::nsql_database_system::c_bson_date, bool> ret = bs_val;
				yggr_test_assert(ret.first == bs_date);
				yggr_test_assert(ret.second);
			}

			{
				std::pair<bool, bool> ret = bs_val;
				yggr_test_assert(!ret.second);
			}
		}

		{
			yggr::nsql_database_system::c_bson_time bs_time(1);
			bson_value_type bs_val(bs_time);

			{
				std::pair<yggr::nsql_database_system::c_bson_time, bool> ret = bs_val;
				yggr_test_assert(ret.first == bs_time);
				yggr_test_assert(ret.second);
			}

			{
				std::pair<bool, bool> ret = bs_val;
				yggr_test_assert(!ret.second);
			}
		}

		{
			yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
			bson_value_type bs_val(bs_timeval);

			{
				std::pair<yggr::nsql_database_system::c_bson_timeval, bool> ret = bs_val;
				yggr_test_assert(ret.first == bs_timeval);
				yggr_test_assert(ret.second);
			}

			{
				std::pair<bool, bool> ret = bs_val;
				yggr_test_assert(!ret.second);
			}
		}
	}

	// E_BSON_TYPE_OID
	{
		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val(bs_oid);

			{
				std::pair<yggr::nsql_database_system::c_bson_oid, bool> ret = bs_val;
				yggr_test_assert(ret.first == bs_oid);
				yggr_test_assert(ret.second);
			}

			{
				std::pair<bson_oid_t, bool> ret = bs_val;
				yggr_test_assert(ret.first == bs_oid);
				yggr_test_assert(ret.second);
			}

			{
				std::pair<bool, bool> ret = bs_val;
				yggr_test_assert(!ret.second);
			}
		}
	}


	// E_BSON_TYPE_BINARY
	{
		typedef yggr::nsql_database_system::c_bson_binary_buffer<10> bson_binary_buffer_type;
		int arr[] = {1, 2, 3};
		bson_binary_buffer_type bs_bin(arr);
		bson_value_type bs_val(bs_bin);

		{
			std::pair<bson_binary_buffer_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_bin);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_REGEX
	{
		yggr::nsql_database_system::c_bson_regex bs_reg("this is a bson regex", "isx");
		bson_value_type bs_val(bs_reg);

		{
			std::pair<yggr::nsql_database_system::c_bson_regex, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_reg);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_CODE
	{
		yggr::nsql_database_system::c_bson_code bs_code("this is a bson code");
		bson_value_type bs_val(bs_code);

		{
			std::pair<yggr::nsql_database_system::c_bson_code, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_code);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_SYMBOL
	{
		yggr::nsql_database_system::c_bson_symbol bs_symbol("this is a bson symbol");
		bson_value_type bs_val(bs_symbol);

		{
			std::pair<yggr::nsql_database_system::c_bson_symbol, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_symbol);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_CODEWSCOPE
	{
		typedef yggr::nsql_database_system::c_bson_code_w_scope<bson_t> c_bson_code_w_scope_type;
		c_bson_code_w_scope_type
			bs_cws(
				"this is a code w scope",
				yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));
		bson_value_type bs_val(bs_cws);

		{
			std::pair<c_bson_code_w_scope_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_cws);
			yggr_test_assert(ret.second);
		}

		{
			typedef yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson> now_bson_code_w_scope_type;
			std::pair<now_bson_code_w_scope_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_cws);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_TIMESTAMP
	{
		yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
		bson_value_type bs_val(bs_tmstamp);

		{
			std::pair<yggr::nsql_database_system::c_bson_timestamp, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_tmstamp);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::nsql_database_system::c_bson_timestamp::base_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_tmstamp);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	// E_BSON_TYPE_DBPOINTER
	{

		typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
		typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type::org_type> bson_dbpointer_type;

		bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

		bson_value_type bs_val(bs_dbptr);

		{
			std::pair<bson_dbpointer_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_dbptr);
			yggr_test_assert(ret.second);
		}

		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> now_bson_dbpointer_type;

			std::pair<now_bson_dbpointer_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs_dbptr);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : 1 }");
		bson_value_type bs_val(bs);

		{
			std::pair<yggr::nsql_database_system::c_bson, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<yggr::nsql_database_system::c_bson::org_type, bool> ret = bs_val;
			yggr_test_assert(ret.first == bs);
			yggr_test_assert(ret.second);
		}

		{
			std::pair<bool, bool> ret = bs_val;
			yggr_test_assert(!ret.second);
		}
	}
}

void test_member_foo_assign(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// E_BSON_TYPE_INT32
	{
		bson_value_type bs_val;
		bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(int32), 1);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);
	}

	// E_BSON_TYPE_INT64
	{
		bson_value_type bs_val;
		bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(int64), INT64_C(1));

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT64 );
		yggr_test_assert(bs_val == INT64_C(1));
	}

	// E_BSON_TYPE_BOOL
	{
		bson_value_type bs_val;
		bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(bool), true);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BOOL );
		yggr_test_assert(bs_val == true);
	}

	// E_BSON_TYPE_DOUBLE
	{
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(double), 1.0f);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
			yggr_test_assert(bs_val == 1.0f);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(double), 1.0f);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
			yggr_test_assert(bs_val == 1.0);
		}
	}

	// E_BSON_TYPE_UTF8
	{
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), "this is a const char*");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a const char*");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::string("this is a std::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a std::string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::string("this is a boost::container::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a boost::container::string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string("this is a yggr::string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string("this is a yggr::utf8_string"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::utf8_string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string_view("this is a string_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a string_view");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string_view("this is a utf8_string_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a utf8_string_view");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper("this is a const char*"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a const char*");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(std::string("this is a std::string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a std::string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(boost::container::string("this is a boost::container::string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a boost::container::string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(yggr::string("this is a yggr::string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(yggr::utf8_string("this is a yggr::utf8_string")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a yggr::utf8_string");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(yggr::string_view("this is a string_view")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a string_view");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(yggr::utf8_string_view("this is a utf8_string_view")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == "this is a utf8_string_view");
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), L"this is a const wchar_t*");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a const wchar_t*");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::wstring(L"this is a std::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a std::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::wstring(L"this is a boost::container::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a boost::container::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring(L"this is a yggr::wstring"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a yggr::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring_view(L"this is a wstring_view"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a wstring_view");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::charset::make_string_charset_helper(L"this is a const wchar_t*"));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a const wchar_t*");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(std::wstring(L"this is a std::wstring")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a std::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(boost::container::wstring(L"this is a boost::container::wstring")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a boost::container::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(yggr::wstring(L"this is a yggr::wstring")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a yggr::wstring");
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8),
							yggr::charset::make_string_charset_helper(yggr::wstring_view(L"this is a wstring_view")));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
			yggr_test_assert(bs_val == L"this is a wstring_view");
		}
#endif // YGGR_NO_CWCHAR
	}

	// E_BSON_TYPE_NULL
	{
		yggr::nsql_database_system::c_bson_null bs_null;
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(null));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_NULL);
			yggr_test_assert(bs_val == bs_null);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(null), bs_null);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_NULL);
			yggr_test_assert(bs_val == bs_null);
		}
	}

	// E_BSON_TYPE_UNDEFINED
	{
		yggr::nsql_database_system::c_bson_undefined bs_undefined;
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(undefined));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UNDEFINED);
			yggr_test_assert(bs_val == bs_undefined);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(undefined), bs_undefined);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UNDEFINED);
			yggr_test_assert(bs_val == bs_undefined);
		}
	}

	// E_BSON_TYPE_MAXKEY
	{
		yggr::nsql_database_system::c_bson_maxkey bs_maxkey;
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(maxkey));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MAXKEY);
			yggr_test_assert(bs_val == bs_maxkey);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(maxkey), bs_maxkey);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MAXKEY);
			yggr_test_assert(bs_val == bs_maxkey);
		}
	}

	// E_BSON_TYPE_MINKEY
	{
		yggr::nsql_database_system::c_bson_minkey bs_minkey;

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(minkey));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MINKEY);
			yggr_test_assert(bs_val == bs_minkey);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(minkey), bs_minkey);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_MINKEY);
			yggr_test_assert(bs_val == bs_minkey);
		}
	}

	// E_BSON_TYPE_DATE_TIME
	{
		{
			yggr::nsql_database_system::c_bson_date bs_date(1000);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(date_time), 1000);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_date bs_date(1000);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(date_time), bs_date);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_time bs_time(1);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(time), 1);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_time);
		}

		{
			yggr::nsql_database_system::c_bson_time bs_time(1);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(time), bs_time);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
			yggr_test_assert(bs_val == bs_time);
		}

		{
			{
				yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
				bson_value_type bs_val;
				bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(timeval), static_cast<timeval&>(bs_timeval));

				yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
				yggr_test_assert(bs_val == bs_timeval);
			}

			{
				yggr::nsql_database_system::c_bson_timeval bs_timeval(1, 1000);
				bson_value_type bs_val;
				bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(timeval), bs_timeval);

				yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DATE_TIME);
				yggr_test_assert(bs_val == bs_timeval);
			}
		}
	}

	// E_BSON_TYPE_OID
	{
		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(oid), static_cast<bson_oid_t&>(bs_oid));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
			yggr_test_assert(bs_val == bs_oid);
		}

		{
			yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(oid), bs_oid);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
			yggr_test_assert(bs_val == bs_oid);
		}
	}


	// E_BSON_TYPE_BINARY
	{
		{
			int arr[] = {1, 2, 3};
			yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin(arr);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);
		}

		{
			int arr[] = {1, 2, 3};
			yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin(arr);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin.buffer_typeid(), bs_bin.data(), bs_bin.size());

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);
		}

		{
			typedef yggr::vector<int> buffer_type;

			int arr[] = {1, 2, 3};
			buffer_type buf(arr + 0, arr + 3);
			yggr::nsql_database_system::c_bson_binary_buffer_ref<10, buffer_type> bs_bin(buf);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BINARY);
			yggr_test_assert(bs_val.value.v_binary.subtype == 10);
			yggr_test_assert(bs_val == bs_bin);
		}
	}

	// E_BSON_TYPE_REGEX
	{
		yggr::nsql_database_system::c_bson_regex bs_reg("this is a bson regex", "isx");
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(regex), bs_reg);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_REGEX);
			yggr_test_assert(bs_val == bs_reg);
		}

		{
			bson_value_type bs_val;
			const yggr::nsql_database_system::c_bson_regex& bs_reg_cref = bs_reg;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(regex), bs_reg_cref.regex(), bs_reg_cref.options());

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_REGEX);
			yggr_test_assert(bs_val == bs_reg);
		}
	}

	// E_BSON_TYPE_CODE
	{
		yggr::nsql_database_system::c_bson_code bs_code("this is a bson code");

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(code), bs_code.code());

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODE);
			yggr_test_assert(bs_val == bs_code);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(code), bs_code);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODE);
			yggr_test_assert(bs_val == bs_code);
		}
	}

	// E_BSON_TYPE_SYMBOL
	{
		yggr::nsql_database_system::c_bson_symbol bs_symbol("this is a bson symbol");
		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(symbol), bs_symbol);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_SYMBOL);
			yggr_test_assert(bs_val == bs_symbol);
		}

		{
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(symbol), "this is a bson symbol");

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_SYMBOL);
			yggr_test_assert(bs_val == bs_symbol);
		}
	}

	// E_BSON_TYPE_CODEWSCOPE
	{
		{
			yggr::nsql_database_system::c_bson_code_w_scope<bson_t>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), bs_cws);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), bs_cws);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson>
				bs_cws(
					"this is a code w scope",
					yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), bs_cws.code(), bs_cws.scope());

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_CODEWSCOPE);
			yggr_test_assert(bs_val == bs_cws);
		}
	}

	// E_BSON_TYPE_TIMESTAMP
	{
		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(timestamp),
							static_cast<const yggr::nsql_database_system::c_bson_timestamp::base_type&>(bs_tmstamp));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);
		}

		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), bs_tmstamp);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);
		}

		{
			yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 1);
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), bs_tmstamp.time(), bs_tmstamp.inc());

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_TIMESTAMP);
			yggr_test_assert(bs_val == bs_tmstamp);
		}
	}

	// E_BSON_TYPE_DECIMAL128
	{
		{
			yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), static_cast<bson_decimal128_t&>(bs_dec128));

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
			yggr_test_assert(bs_val == bs_dec128);
		}

		{
			yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), bs_dec128);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
			yggr_test_assert(bs_val == bs_dec128);
		}
	}

	// E_BSON_TYPE_DBPOINTER
	{
		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type::org_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), bs_dbptr);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);
		}

		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), bs_dbptr);

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);
		}

		{
			typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type::org_type> bson_dbpointer_type;

			bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));

			bson_value_type bs_val;
			bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), bs_dbptr.collection(), bs_dbptr.oid());

			yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DBPOINTER);
			yggr_test_assert(bs_val == bs_dbptr);
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : 1 }");
		bson_value_type bs_val;
		bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(document), bs);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOCUMENT);
		yggr_test_assert(bs_val == bs);
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : [1, 2, 3] }");
		bson_value_type bs_val;
		bs_val.assign(YGGR_BSON_VALUE_TAG_OBJECT(array), bs);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_ARRAY);
		yggr_test_assert(bs_val == bs);
	}
}

void test_member_foo_get_value(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// E_BSON_TYPE_INT32
	{
		bson_value_type bs_val(1);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT32 );
		yggr_test_assert(bs_val == 1);

		{
			yggr::s32 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(int32), ld_val);

			yggr_test_assert(bchk && ld_val == 1);
		}

		{
			bool ld_val = false;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(bool), ld_val);

			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_INT64
	{
		bson_value_type bs_val(INT64_C(1));

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_INT64 );
		yggr_test_assert(bs_val == INT64_C(1));

		{
			yggr::s64 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(int64), ld_val);

			yggr_test_assert(bchk && ld_val == 1);
		}

		{
			bool ld_val = false;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(bool), ld_val);

			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_BOOL
	{
		bson_value_type bs_val(true);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_BOOL );
		yggr_test_assert(bs_val == true);

		{
			bool ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(bool), ld_val);

			yggr_test_assert(bchk && ld_val == 1);
		}

		{
			yggr::s32 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(int32), ld_val);

			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_DOUBLE
	{
		bson_value_type bs_val(1.0f);

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DOUBLE );
		yggr_test_assert(bs_val == 1.0f);

		{
			yggr::f32 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(double), ld_val);

			yggr_test_assert(bchk && ld_val == 1.0f);
		}

		{
			yggr::f64 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(double), ld_val);

			yggr_test_assert(bchk && ld_val == 1.0f);
		}

		{
			bool ld_val = false;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(bool), ld_val);

			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_UTF8
	{
		bson_value_type bs_val("this is a utf8_string");

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_UTF8 );
		yggr_test_assert(bs_val == "this is a utf8_string");

		{
			yggr::utf8_string ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == "this is a utf8_string");
		}

		{
			std::string ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == "this is a utf8_string");
		}

		{
			boost::container::string ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == "this is a utf8_string");
		}

		{
			yggr::string ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == "this is a utf8_string");
		}

		{
			yggr::utf8_string ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == "this is a utf8_string");
		}

		{
			bool ld_val = false;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(bool), ld_val);

			yggr_test_assert(!bchk);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			std::wstring ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == L"this is a utf8_string");
		}

		{
			boost::container::wstring ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == L"this is a utf8_string");
		}

		{
			yggr::wstring ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(utf8), ld_val);

			yggr_test_assert(bchk && ld_val == L"this is a utf8_string");
		}
#endif // YGGR_NO_CWCHAR
	}

	// E_BSON_TYPE_NULL
	{
		yggr::nsql_database_system::c_bson_null bs_null;
		bson_value_type bs_val(bs_null);

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(bchk && ld_val == bs_null);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null));
			yggr_test_assert(bchk);
		}

		{
			yggr::nsql_database_system::c_bson_undefined ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(undefined), ld_val);
			yggr_test_assert(!bchk);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(undefined));
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_UNDEFINED
	{
		yggr::nsql_database_system::c_bson_undefined bs_undefined;
		bson_value_type bs_val(bs_undefined);

		{
			yggr::nsql_database_system::c_bson_undefined ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(undefined), ld_val);
			yggr_test_assert(bchk && ld_val == bs_undefined);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(undefined));
			yggr_test_assert(bchk);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null));
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_MAXKEY
	{
		yggr::nsql_database_system::c_bson_maxkey bs_maxkey;
		bson_value_type bs_val(bs_maxkey);

		{
			yggr::nsql_database_system::c_bson_maxkey ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(maxkey), ld_val);
			yggr_test_assert(bchk && ld_val == bs_maxkey);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(maxkey));
			yggr_test_assert(bchk);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null));
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_MINKEY
	{
		yggr::nsql_database_system::c_bson_minkey bs_minkey;
		bson_value_type bs_val(bs_minkey);

		{
			yggr::nsql_database_system::c_bson_minkey ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(minkey), ld_val);
			yggr_test_assert(bchk && ld_val == bs_minkey);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(minkey));
			yggr_test_assert(bchk);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}

		{
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null));
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_DATE_TIME
	{
		yggr::nsql_database_system::c_bson_date bs_date(1000);
		bson_value_type bs_val(bs_date);

		{
			yggr::s64 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(date_time), ld_val);
			yggr_test_assert(bchk && ld_val == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_date ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(date_time), ld_val);
			yggr_test_assert(bchk && ld_val == bs_date);
		}

		{
			yggr::s64 ld_val = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(time), ld_val);
			yggr_test_assert(bchk && yggr::nsql_database_system::c_bson_time(ld_val) == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_time ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(time), ld_val);
			yggr_test_assert(bchk && ld_val == bs_date);
		}

		{
			timeval ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(timeval), ld_val);
			yggr_test_assert(bchk && ld_val == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_timeval ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(timeval), ld_val);
			yggr_test_assert(bchk && ld_val == bs_date);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_OID
	{
		yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
		bson_value_type bs_val(bs_oid);

		{
			bson_oid_t ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(oid), ld_val);
			yggr_test_assert(bchk && ld_val == bs_oid);
		}

		{
			yggr::nsql_database_system::c_bson_oid ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(oid), ld_val);
			yggr_test_assert(bchk && ld_val == bs_oid);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_BINARY
	{
		int arr[] = {1, 2, 3};
		yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin(arr);
		bson_value_type bs_val(bs_bin);

		{
			yggr::u32 ld_val_sub_t = 0;
			yggr::string ld_val_buf;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(binary), ld_val_sub_t, ld_val_buf);
			yggr_test_assert(bchk && ld_val_sub_t == bs_bin.buffer_typeid() && ld_val_buf == bs_bin );
		}

		{
			yggr::nsql_database_system::c_bson_binary_buffer<10> ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(binary), ld_val);
			yggr_test_assert(bchk && ld_val == bs_bin );
		}

		{
			yggr::nsql_database_system::c_bson_binary_buffer<9> ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(binary), ld_val);
			yggr_test_assert(!bchk);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_REGEX
	{
		yggr::nsql_database_system::c_bson_regex bs_reg("this is a bson regex", "isx");
		bson_value_type bs_val(bs_reg);

		{
			yggr::utf8_string ld_val_regex;
			yggr::utf8_string ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == "this is a bson regex"
				&& ld_val_opts == "isx");
		}

		{
			std::string ld_val_regex;
			std::string ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == "this is a bson regex"
				&& ld_val_opts == "isx");
		}

		{
			boost::container::string ld_val_regex;
			boost::container::string ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == "this is a bson regex"
				&& ld_val_opts == "isx");
		}

		{
			yggr::string ld_val_regex;
			yggr::string ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == "this is a bson regex"
				&& ld_val_opts == "isx");
		}

#if !defined(YGGR_NO_CWCHAR)

		{
			std::wstring ld_val_regex;
			std::wstring ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == L"this is a bson regex"
				&& ld_val_opts == L"isx");
		}

		{
			boost::container::wstring ld_val_regex;
			boost::container::wstring ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == L"this is a bson regex"
				&& ld_val_opts == L"isx");
		}

		{
			yggr::wstring ld_val_regex;
			yggr::wstring ld_val_opts;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val_regex, ld_val_opts);
			yggr_test_assert(
				bchk
				&& ld_val_regex == L"this is a bson regex"
				&& ld_val_opts == L"isx");
		}

#endif // YGGR_NO_CWCHAR

		{
			yggr::nsql_database_system::c_bson_regex ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(regex), ld_val);
			yggr_test_assert(bchk && bs_reg == ld_val);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_CODE
	{
		yggr::nsql_database_system::c_bson_code bs_code("this is a bson code");
		bson_value_type bs_val(bs_code);

		{
			yggr::utf8_string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

		{
			std::string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

		{
			boost::container::string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

		{
			yggr::string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

#if !defined(YGGR_NO_CWCHAR)

		{
			std::wstring ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

		{
			boost::container::wstring ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

		{
			yggr::wstring ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code.code());
		}

#endif // YGGR_NO_CWCHAR

		{
			yggr::nsql_database_system::c_bson_code ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(code), ld_val);
			yggr_test_assert(bchk && ld_val == bs_code);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_SYMBOL
	{
		yggr::nsql_database_system::c_bson_symbol bs_symbol("this is a bson symbol");
		bson_value_type bs_val(bs_symbol);

		{
			yggr::utf8_string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

		{
			std::string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

		{
			boost::container::string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

		{
			yggr::string ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

#if !defined(YGGR_NO_CWCHAR)

		{
			std::wstring ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

		{
			boost::container::wstring ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

		{
			yggr::wstring ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol.symbol());
		}

#endif // YGGR_NO_CWCHAR

		{
			yggr::nsql_database_system::c_bson_symbol ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(symbol), ld_val);
			yggr_test_assert(bchk && ld_val == bs_symbol);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_CODEWSCOPE
	{
		yggr::nsql_database_system::c_bson_code_w_scope<bson_t>
			bs_cws(
				"this is a code w scope",
				yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_value_type bs_val(bs_cws);

		{
			yggr::utf8_string ld_val_code;
			bson_t ld_val_scope;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), ld_val_code, ld_val_scope);
			yggr_test_assert(bchk && ld_val_code == bs_cws.code() && ld_val_scope == bs_cws.scope());
		}

		{
			yggr::string ld_val_code;
			bson_t ld_val_scope;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), ld_val_code, ld_val_scope);
			yggr_test_assert(bchk && ld_val_code == bs_cws.code() && ld_val_scope == bs_cws.scope());
		}

		{
			yggr::utf8_string ld_val_code;
			yggr::nsql_database_system::c_bson ld_val_scope;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), ld_val_code, ld_val_scope);
			yggr_test_assert(bchk && ld_val_code == bs_cws.code() && ld_val_scope == bs_cws.scope());
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<bson_t> ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), ld_val);
			yggr_test_assert(bchk && ld_val == bs_cws);
		}

		{
			yggr::nsql_database_system::c_bson_code_w_scope<yggr::nsql_database_system::c_bson> ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), ld_val);
			yggr_test_assert(bchk && ld_val == bs_cws);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_TIMESTAMP
	{
		yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 2);
		bson_value_type bs_val(static_cast<const yggr::nsql_database_system::c_bson_timestamp::base_type&>(bs_tmstamp));

		{
			yggr::u32 ld_val_tm = 0, ld_val_inc = 0;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), ld_val_tm, ld_val_inc);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val_tm == 1 && ld_val_inc == 2);
		}

		{
			yggr::nsql_database_system::c_bson_timestamp ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val.time() == 1 && ld_val.inc() == 2);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_DBPOINTER
	{
		typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
		typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;

		bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));
		bson_value_type bs_val(bs_dbptr);

		{
			yggr::utf8_string ld_val_col;
			bson_oid_type::org_type ld_val_oid;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), ld_val_col, ld_val_oid);

			yggr_test_assert(bchk);
			yggr_test_assert(ld_val_col == bs_dbptr.collection());
			yggr_test_assert(ld_val_oid == bs_dbptr.oid());
		}

		{
			yggr::utf8_string ld_val_col;
			bson_oid_type ld_val_oid;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), ld_val_col, ld_val_oid);

			yggr_test_assert(bchk);
			yggr_test_assert(ld_val_col == bs_dbptr.collection());
			yggr_test_assert(ld_val_oid == bs_dbptr.oid());
		}

		{
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type::org_type> bson_dbpointer_type;
			bson_dbpointer_type ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), ld_val);

			yggr_test_assert(bchk);
			yggr_test_assert(ld_val.collection() == bs_dbptr.collection());
			yggr_test_assert(ld_val.oid() == bs_dbptr.oid());
		}

		{
			typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;
			bson_dbpointer_type ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), ld_val);

			yggr_test_assert(bchk);
			yggr_test_assert(ld_val.collection() == bs_dbptr.collection());
			yggr_test_assert(ld_val.oid() == bs_dbptr.oid());
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_DECIMAL128
	{
		yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
		bson_value_type bs_val(static_cast<bson_decimal128_t&>(bs_dec128));

		{
			bson_decimal128_t ld_val = {0};

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val == bs_dec128);
		}

		{
			yggr::nsql_database_system::c_bson_decimal128 ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val == bs_dec128);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : 1 }");
		bson_value_type bs_val(bs);

		{
			bson_t ld_val = {0};

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(document), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val == bs);
		}

		{
			yggr::nsql_database_system::c_bson ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(document), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val == bs);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : [1, 2, 3] }");
		bson_value_type bs_val(bs);

		{
			bson_t ld_val = {0};

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(array), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val == bs);
		}

		{
			yggr::nsql_database_system::c_bson ld_val;

			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(array), ld_val);
			yggr_test_assert(bchk);
			yggr_test_assert(ld_val == bs);
		}

		{
			yggr::nsql_database_system::c_bson_null ld_val;
			bool bchk = bs_val.get_value(YGGR_BSON_VALUE_TAG_OBJECT(null), ld_val);
			yggr_test_assert(!bchk);
		}
	}
}

void test_member_foo_compare(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// E_BSON_TYPE_INT32
	{
		bson_value_type bs_val(1);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int32), 1);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int32), 0);
			yggr_test_assert(!bchk);

			bchk = (1 == bs_val);
			yggr_test_assert(bchk);

			bchk = (1 != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == 1);
			yggr_test_assert(bchk);

			bchk = (bs_val != 1);
			yggr_test_assert(!bchk);

			bchk = (bs_val == 0);
			yggr_test_assert(!bchk);

			bchk = (0 != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), 1);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), 0);
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(int32), 2);
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < 2);
			yggr_test_assert(bchk);

			bchk = (bs_val <= 1);
			yggr_test_assert(bchk);

			bchk = (bs_val > 0);
			yggr_test_assert(bchk);

			bchk = (bs_val >= 0);
			yggr_test_assert(bchk);

			bchk = (0 < bs_val);
			yggr_test_assert(bchk);

			bchk = (0 <= bs_val);
			yggr_test_assert(bchk);

			bchk = (2 > bs_val);
			yggr_test_assert(bchk);

			bchk = (2 >= bs_val);
			yggr_test_assert(bchk);

		}
	}

	// E_BSON_TYPE_INT64
	{
		bson_value_type bs_val(INT64_C(1));

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int64), INT64_C(1));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(int64), INT64_C(0));
			yggr_test_assert(!bchk);

			bchk = (INT64_C(1) == bs_val);
			yggr_test_assert(bchk);

			bchk = (INT64_C(1) != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == INT64_C(1));
			yggr_test_assert(bchk);

			bchk = (bs_val != INT64_C(1));
			yggr_test_assert(!bchk);

			bchk = (bs_val == INT64_C(0));
			yggr_test_assert(!bchk);

			bchk = (INT64_C(0) != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), INT64_C(1));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), INT64_C(0));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(int64), INT64_C(2));
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < INT64_C(2));
			yggr_test_assert(bchk);

			bchk = (bs_val <= INT64_C(1));
			yggr_test_assert(bchk);

			bchk = (bs_val > INT64_C(0));
			yggr_test_assert(bchk);

			bchk = (bs_val >= INT64_C(0));
			yggr_test_assert(bchk);

			bchk = (INT64_C(0) < bs_val);
			yggr_test_assert(bchk);

			bchk = (INT64_C(0) <= bs_val);
			yggr_test_assert(bchk);

			bchk = (INT64_C(2) > bs_val);
			yggr_test_assert(bchk);

			bchk = (INT64_C(2) >= bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_BOOL
	{
		bson_value_type bs_val(true);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(bool), true);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(bool), false);
			yggr_test_assert(!bchk);

			bchk = (true == bs_val);
			yggr_test_assert(bchk);

			bchk = (true != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == true);
			yggr_test_assert(bchk);

			bchk = (bs_val != true);
			yggr_test_assert(!bchk);

			bchk = (bs_val == false);
			yggr_test_assert(!bchk);

			bchk = (false != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), true);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), false);
			yggr_test_assert(1 == nchk);

			nchk = bson_value_type(false).compare(YGGR_BSON_VALUE_TAG_OBJECT(bool), true);
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bson_value_type(false) < true);
			yggr_test_assert(bchk);

			bchk = (bs_val <= true);
			yggr_test_assert(bchk);

			bchk = (bs_val > false);
			yggr_test_assert(bchk);

			bchk = (bs_val >= false);
			yggr_test_assert(bchk);

			bchk = (false < bs_val);
			yggr_test_assert(bchk);

			bchk = (false <= bs_val);
			yggr_test_assert(bchk);

			bchk = (true > bson_value_type(false));
			yggr_test_assert(bchk);

			bchk = (true >= bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_DOUBLE
	{
		bson_value_type bs_val(1.0f);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(double), 1.0);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(double), 0.0f);
			yggr_test_assert(!bchk);

			bchk = (1.0 == bs_val);
			yggr_test_assert(bchk);

			bchk = (1.0 != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == 1.0f);
			yggr_test_assert(bchk);

			bchk = (bs_val != 1.0f);
			yggr_test_assert(!bchk);

			bchk = (bs_val == 0.0);
			yggr_test_assert(!bchk);

			bchk = (0.0f != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(double), 1);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(double), 0);
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(double), 2);
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < 2.0);
			yggr_test_assert(bchk);

			bchk = (bs_val <= 1.0f);
			yggr_test_assert(bchk);

			bchk = (bs_val > 0.0);
			yggr_test_assert(bchk);

			bchk = (bs_val >= 0.0f);
			yggr_test_assert(bchk);

			bchk = (0.0f < bs_val);
			yggr_test_assert(bchk);

			bchk = (0.0 <= bs_val);
			yggr_test_assert(bchk);

			bchk = (2.0 > bs_val);
			yggr_test_assert(bchk);

			bchk = (2.0f >= bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_UTF8
	{
		bson_value_type bs_val("this is a utf8_string");

		{
			bool bchk = false;

			// const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), "this is a utf8_string");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), "this is a not utf8_string");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::string("this is a not utf8_string"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::string("this is a not utf8_string"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string("this is a not utf8_string"));
			yggr_test_assert(!bchk);

			// yggr::utf8_string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string("this is a not utf8_string"));
			yggr_test_assert(!bchk);

#if !defined(YGGR_NO_CWCHAR)
			// const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), L"this is a utf8_string");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), L"this is a not utf8_string");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::wstring(L"this is a not utf8_string"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::wstring(L"this is a not utf8_string"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring(L"this is a not utf8_string"));
			yggr_test_assert(!bchk);
#endif // YGGR_NO_CWCHAR

			// const char*
			bchk = ("this is a utf8_string" == bs_val);
			yggr_test_assert(bchk);

			bchk = ("this is a utf8_string" != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == "this is a utf8_string");
			yggr_test_assert(bchk);

			bchk = (bs_val != "this is a utf8_string");
			yggr_test_assert(!bchk);

			//std::string
			bchk = (std::string("this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (std::string("this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == std::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != std::string("this is a utf8_string"));
			yggr_test_assert(!bchk);

			//boost::container::string
			bchk = (boost::container::string("this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::string("this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == boost::container::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != boost::container::string("this is a utf8_string"));
			yggr_test_assert(!bchk);

			//yggr::string
			bchk = (yggr::string("this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::string("this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != yggr::string("this is a utf8_string"));
			yggr_test_assert(!bchk);

			//yggr::utf8_string
			bchk = (yggr::utf8_string("this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::utf8_string("this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::utf8_string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != yggr::utf8_string("this is a utf8_string"));
			yggr_test_assert(!bchk);

#if !defined(YGGR_NO_CWCHAR)
			// const char*
			bchk = (L"this is a utf8_string" == bs_val);
			yggr_test_assert(bchk);

			bchk = (L"this is a utf8_string" != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == L"this is a utf8_string");
			yggr_test_assert(bchk);

			bchk = (bs_val != L"this is a utf8_string");
			yggr_test_assert(!bchk);

			//std::string
			bchk = (std::wstring(L"this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (std::wstring(L"this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == std::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != std::wstring(L"this is a utf8_string"));
			yggr_test_assert(!bchk);

			//boost::container::string
			bchk = (boost::container::wstring(L"this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::wstring(L"this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == boost::container::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != boost::container::wstring(L"this is a utf8_string"));
			yggr_test_assert(!bchk);

			//yggr::string
			bchk = (yggr::wstring(L"this is a utf8_string") == bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::wstring(L"this is a utf8_string") != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val != yggr::wstring(L"this is a utf8_string"));
			yggr_test_assert(!bchk);
#endif // YGGR_NO_CWCHAR
		}

		{
			yggr::s32 nchk = 0;

			// const char*
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), "this is a utf8_string");
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), "this is a utf8_strin");
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), "this is a utf8_string1");
			yggr_test_assert(-1 == nchk);

			// std::string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::string("this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::string("this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::string("this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);

			// boost::container::string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::string("this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::string("this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::string("this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);

			// yggr::string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string("this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string("this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::string("this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);

			// yggr::utf8_string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string("this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string("this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::utf8_string("this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);

#if !defined(YGGR_NO_CWCHAR)
			// const char*
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), L"this is a utf8_string");
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), L"this is a utf8_strin");
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), L"this is a utf8_string1");
			yggr_test_assert(-1 == nchk);

			// std::string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::wstring(L"this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::wstring(L"this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), std::wstring(L"this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);

			// boost::container::string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::wstring(L"this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::wstring(L"this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), boost::container::wstring(L"this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);

			// yggr::string
			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring(L"this is a utf8_string"));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring(L"this is a utf8_strin"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(utf8), yggr::wstring(L"this is a utf8_string1"));
			yggr_test_assert(-1 == nchk);
#endif // YGGR_NO_CWCHAR

			bool bchk = false;

			// const char*
			bchk = (bs_val < "this is a utf8_string1");
			yggr_test_assert(bchk);

			bchk = (bs_val <= "this is a utf8_string");
			yggr_test_assert(bchk);

			bchk = (bs_val > "this is a utf8_strin");
			yggr_test_assert(bchk);

			bchk = (bs_val >= "this is a utf8_strin");
			yggr_test_assert(bchk);

			// std::string
			bchk = (bs_val < std::string("this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= std::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > std::string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= std::string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			// boost::container::string
			bchk = (bs_val < boost::container::string("this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= boost::container::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > boost::container::string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= boost::container::string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			// yggr::string
			bchk = (bs_val < yggr::string("this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= yggr::string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			// yggr::utf8_string
			bchk = (bs_val < yggr::utf8_string("this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= yggr::utf8_string("this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::utf8_string("this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::utf8_string("this is a utf8_strin"));
			yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
			// const char*
			bchk = (bs_val < L"this is a utf8_string1");
			yggr_test_assert(bchk);

			bchk = (bs_val <= L"this is a utf8_string");
			yggr_test_assert(bchk);

			bchk = (bs_val > L"this is a utf8_strin");
			yggr_test_assert(bchk);

			bchk = (bs_val >= L"this is a utf8_strin");
			yggr_test_assert(bchk);

			// std::string
			bchk = (bs_val < std::wstring(L"this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= std::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > std::wstring(L"this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= std::wstring(L"this is a utf8_strin"));
			yggr_test_assert(bchk);

			// boost::container::string
			bchk = (bs_val < boost::container::wstring(L"this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= boost::container::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > boost::container::wstring(L"this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= boost::container::wstring(L"this is a utf8_strin"));
			yggr_test_assert(bchk);

			// yggr::string
			bchk = (bs_val < yggr::wstring(L"this is a utf8_string1"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= yggr::wstring(L"this is a utf8_string"));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::wstring(L"this is a utf8_strin"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::wstring(L"this is a utf8_strin"));
			yggr_test_assert(bchk);

#endif // YGGR_NO_CWCHAR

			// const char*
			bchk = ("this is a utf8_strin" < bs_val);
			yggr_test_assert(bchk);

			bchk = ("this is a utf8_string" <= bs_val);
			yggr_test_assert(bchk);

			bchk = ("this is a utf8_string1" > bs_val);
			yggr_test_assert(bchk);

			bchk = ("this is a utf8_string1" >= bs_val);
			yggr_test_assert(bchk);

			// std::string
			bchk = (std::string("this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (std::string("this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (std::string("this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (std::string("this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);

			// boost::container::string
			bchk = (boost::container::string("this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::string("this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::string("this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::string("this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);

			// yggr::string
			bchk = (yggr::string("this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::string("this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::string("this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::string("this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);

			// yggr::utf8_string
			bchk = (yggr::utf8_string("this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::utf8_string("this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::utf8_string("this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::utf8_string("this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
			// const char*
			bchk = (L"this is a utf8_strin" < bs_val);
			yggr_test_assert(bchk);

			bchk = (L"this is a utf8_string" <= bs_val);
			yggr_test_assert(bchk);

			bchk = (L"this is a utf8_string1" > bs_val);
			yggr_test_assert(bchk);

			bchk = (L"this is a utf8_string1" >= bs_val);
			yggr_test_assert(bchk);

			// std::string
			bchk = (std::wstring(L"this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (std::wstring(L"this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (std::wstring(L"this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (std::wstring(L"this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);

			// boost::container::string
			bchk = (boost::container::wstring(L"this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::wstring(L"this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::wstring(L"this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (boost::container::wstring(L"this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);

			// yggr::string
			bchk = (yggr::wstring(L"this is a utf8_strin") < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::wstring(L"this is a utf8_string") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::wstring(L"this is a utf8_string1") > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::wstring(L"this is a utf8_string1") >= bs_val);
			yggr_test_assert(bchk);
#endif // YGGR_NO_CWCHAR
		}
	}

	// E_BSON_TYPE_NULL
	{
		yggr::nsql_database_system::c_bson_null bs_null;
		bson_value_type bs_val(bs_null);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(null), bs_null);
			yggr_test_assert(bchk);

			bchk = (bs_val == bs_null);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_null);
			yggr_test_assert(!bchk);

			bchk = (bs_null == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_null != bs_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_UNDEFINED
	{
		yggr::nsql_database_system::c_bson_undefined bs_undefined;
		bson_value_type bs_val(bs_undefined);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(undefined), bs_undefined);
			yggr_test_assert(bchk);

			bchk = (bs_val == bs_undefined);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_undefined);
			yggr_test_assert(!bchk);

			bchk = (bs_undefined == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_undefined != bs_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_MAXKEY
	{
		yggr::nsql_database_system::c_bson_maxkey bs_maxkey;
		bson_value_type bs_val(bs_maxkey);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(maxkey), bs_maxkey);
			yggr_test_assert(bchk);

			bchk = (bs_val == bs_maxkey);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_maxkey);
			yggr_test_assert(!bchk);

			bchk = (bs_maxkey == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_maxkey != bs_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_MINKEY
	{
		yggr::nsql_database_system::c_bson_minkey bs_minkey;
		bson_value_type bs_val(bs_minkey);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(minkey), bs_minkey);
			yggr_test_assert(bchk);

			bchk = (bs_val == bs_minkey);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_minkey);
			yggr_test_assert(!bchk);

			bchk = (bs_minkey == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_minkey != bs_val);
			yggr_test_assert(!bchk);
		}
	}

	// E_BSON_TYPE_DATE_TIME
	{
		yggr::nsql_database_system::c_bson_date bs_date(1000);
		bson_value_type bs_val(bs_date);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), 1000);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), 0);
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), bs_date);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), bs_date);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(1000) == bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(1000) != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_date(1000));
			yggr_test_assert(bchk);

			bchk = (bs_val != yggr::nsql_database_system::c_bson_date(1000));
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(0) != bs_val);
			yggr_test_assert(bchk);
		}

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(time), 1);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(time), 0);
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(time), yggr::nsql_database_system::c_bson_time(1));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(time), yggr::nsql_database_system::c_bson_time(0));
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(time), yggr::nsql_database_system::c_bson_time(1));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(date_time), yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(1) == bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(1) != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_time(1));
			yggr_test_assert(bchk);

			bchk = (bs_val != yggr::nsql_database_system::c_bson_time(1));
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_time(0));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(0) != bs_val);
			yggr_test_assert(bchk);
		}

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(1, 0));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(0, 0));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(1, 0) == bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(1, 0) != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_timeval(1, 0));
			yggr_test_assert(bchk);

			bchk = (bs_val != yggr::nsql_database_system::c_bson_timeval(1, 0));
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_timeval(0, 0));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(0, 0) != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(date_time), 1000);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(date_time), 0);
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(date_time), 2000);
			yggr_test_assert(-1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(date_time), bs_date);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(date_time), yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(date_time), yggr::nsql_database_system::c_bson_date(2000));
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < yggr::nsql_database_system::c_bson_date(2000));
			yggr_test_assert(bchk);

			bchk = (bs_val <= yggr::nsql_database_system::c_bson_date(1000));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::nsql_database_system::c_bson_date(0));
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(0) < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(0) <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(2000) > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_date(2000) >= bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(time), 1);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(time), 0);
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(time), 2);
			yggr_test_assert(-1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(time), yggr::nsql_database_system::c_bson_time(1));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(time), yggr::nsql_database_system::c_bson_time(0));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(time), yggr::nsql_database_system::c_bson_time(2));
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < yggr::nsql_database_system::c_bson_time(2));
			yggr_test_assert(bchk);

			bchk = (bs_val <= yggr::nsql_database_system::c_bson_time(1));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::nsql_database_system::c_bson_time(0));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::nsql_database_system::c_bson_time(0));
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(0) < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(0) <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(2) > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_time(2) >= bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(1, 0));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(0, 0));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(2, 0));
			yggr_test_assert(-1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(1, 0));
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(0, 0));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(timeval), yggr::nsql_database_system::c_bson_timeval(2, 0));
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < yggr::nsql_database_system::c_bson_timeval(2, 0));
			yggr_test_assert(bchk);

			bchk = (bs_val <= yggr::nsql_database_system::c_bson_timeval(1, 0));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::nsql_database_system::c_bson_timeval(0, 0));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::nsql_database_system::c_bson_timeval(0, 0));
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(0, 0) < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(0, 0) <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(2, 0) > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_timeval(2, 0) >= bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_OID
	{
		yggr::nsql_database_system::c_bson_oid bs_oid("00112233445566778899aabb");
		bson_value_type bs_val(bs_oid);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(oid), bs_oid);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(oid), yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa"));
			yggr_test_assert(!bchk);

			bchk = (bs_oid == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_oid != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_oid);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_oid);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa"));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa") != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(oid), bs_oid);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(oid), yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(oid), yggr::nsql_database_system::c_bson_oid("00112233445566778899aacc"));
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < yggr::nsql_database_system::c_bson_oid("00112233445566778899aacc"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= bs_oid);
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa"));
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa") < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_oid("00112233445566778899aaaa") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_oid("00112233445566778899aacc") > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_oid("00112233445566778899aacc") >= bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_DECIMAL128
	{
		yggr::nsql_database_system::c_bson_decimal128 bs_dec128("123456");
		bson_value_type bs_val(static_cast<bson_decimal128_t&>(bs_dec128));

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), static_cast<const bson_decimal128_t&>(bs_dec128));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), yggr::nsql_database_system::c_bson_decimal128("12345"));
			yggr_test_assert(!bchk);

			bchk = (static_cast<const bson_decimal128_t&>(bs_dec128) == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_dec128 != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == static_cast<const bson_decimal128_t&>(bs_dec128));
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_dec128);
			yggr_test_assert(!bchk);

			bchk = (bs_val == yggr::nsql_database_system::c_bson_decimal128("12345"));
			yggr_test_assert(!bchk);

			bchk = (yggr::nsql_database_system::c_bson_decimal128("12345") != bs_val);
			yggr_test_assert(bchk);
		}

		{
			yggr::s32 nchk = 0;

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), bs_dec128);
			yggr_test_assert(0 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), yggr::nsql_database_system::c_bson_decimal128("12345"));
			yggr_test_assert(1 == nchk);

			nchk = bs_val.compare(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), yggr::nsql_database_system::c_bson_decimal128("1234567"));
			yggr_test_assert(-1 == nchk);

			bool bchk = false;

			bchk = (bs_val < yggr::nsql_database_system::c_bson_decimal128("1234567"));
			yggr_test_assert(bchk);

			bchk = (bs_val <= static_cast<const bson_decimal128_t&>(bs_dec128));
			yggr_test_assert(bchk);

			bchk = (bs_val > yggr::nsql_database_system::c_bson_decimal128("12345"));
			yggr_test_assert(bchk);

			bchk = (bs_val >= yggr::nsql_database_system::c_bson_decimal128("12345"));
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_decimal128("12345") < bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_decimal128("12345") <= bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_decimal128("1234567") > bs_val);
			yggr_test_assert(bchk);

			bchk = (yggr::nsql_database_system::c_bson_decimal128("1234567") >= bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_BINARY
	{
		int arr[] = {1, 2, 3};
		int arr_diff[] = {1, 2, 3, 4};
		yggr::vector<int> vt(arr + 0, arr + 3);
		yggr::list<int> lst_diff(arr_diff, arr_diff + 4);

		yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin(arr);
		yggr::nsql_database_system::c_bson_binary_buffer<10> bs_bin_diff(arr_diff);

		yggr::nsql_database_system::c_bson_binary_buffer_ref<10, yggr::vector<int> > bs_bin_ref(vt);
		yggr::nsql_database_system::c_bson_binary_buffer_ref<10, yggr::list<int> > bs_bin_ref_diff(lst_diff);

		bson_value_type bs_val(bs_bin);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin_diff);
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin_ref);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(binary), bs_bin_ref_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_bin == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_bin != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_bin_ref == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_bin_ref != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_bin);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_bin);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_bin_ref);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_bin_ref);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_bin_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_bin_diff != bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_val == bs_bin_ref_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_bin_ref_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_REGEX
	{
		yggr::nsql_database_system::c_bson_regex bs_reg("this is a bson regex", "isx");
		yggr::nsql_database_system::c_bson_regex bs_reg_diff("this is a bson not_regex", "isx");
		bson_value_type bs_val(bs_reg);

		{
			bool bchk = false;

			// const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson regex", "nisxn");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson not_regex", "isx");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson regex", std::string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), std::string("this is a bson not_regex"), "isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), std::string("this is a bson regex"), std::string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), std::string("this is a bson not_regex"), std::string("isx"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson regex", boost::container::string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), boost::container::string("this is a bson not_regex"), "isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), boost::container::string("this is a bson regex"), boost::container::string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), boost::container::string("this is a bson not_regex"), boost::container::string("isx"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson regex", yggr::string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::string("this is a bson not_regex"), "isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::string("this is a bson regex"), yggr::string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::string("this is a bson not_regex"), yggr::string("isx"));
			yggr_test_assert(!bchk);

			// yggr::utf8_string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson regex", yggr::utf8_string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::utf8_string("this is a bson not_regex"), "isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::utf8_string("this is a bson regex"), yggr::utf8_string("nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::utf8_string("this is a bson not_regex"), yggr::utf8_string("isx"));
			yggr_test_assert(!bchk);

#if !defined(YGGR_NO_CWCHAR)
			// const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), L"this is a bson regex", "nisxn");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), L"this is a bson not_regex", L"isx");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), "this is a bson regex", std::wstring(L"nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), std::string("this is a bson not_regex"), L"isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), std::wstring(L"this is a bson regex"), std::wstring(L"nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), std::wstring(L"this is a bson not_regex"), std::wstring(L"isx"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), L"this is a bson regex", boost::container::wstring(L"nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), boost::container::wstring(L"this is a bson not_regex"), L"isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), boost::container::wstring(L"this is a bson regex"), boost::container::wstring(L"nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), boost::container::wstring(L"this is a bson not_regex"), boost::container::wstring(L"isx"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), L"this is a bson regex", yggr::wstring(L"nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::wstring(L"this is a bson not_regex"), L"isx");
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::wstring(L"this is a bson regex"), yggr::wstring(L"nisxn"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), yggr::wstring(L"this is a bson not_regex"), yggr::wstring(L"isx"));
			yggr_test_assert(!bchk);
#endif // YGGR_NO_CWCHAR


			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), bs_reg);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(regex), bs_reg_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_reg == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_reg != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_reg);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_reg);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_reg_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_reg_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_CODE
	{
		yggr::nsql_database_system::c_bson_code bs_code("this is a bson code");
		yggr::nsql_database_system::c_bson_code bs_code_diff("this is not a bson code");
		bson_value_type bs_val(bs_code);

		{
			bool bchk = false;

			//const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), "this is a bson code");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), "this is not a bson code");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), std::string("this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), std::string("this is not a bson code"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), boost::container::string("this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), boost::container::string("this is not a bson code"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), yggr::string("this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), yggr::string("this is not a bson code"));
			yggr_test_assert(!bchk);

			// yggr::utf8_string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), yggr::utf8_string("this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), yggr::utf8_string("this is not a bson code"));
			yggr_test_assert(!bchk);

#if !defined(YGGR_NO_CWCHAR)
			//const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), L"this is a bson code");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), L"this is not a bson code");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), std::wstring(L"this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), std::wstring(L"this is not a bson code"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), boost::container::wstring(L"this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), boost::container::wstring(L"this is not a bson code"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), yggr::wstring(L"this is a bson code"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), yggr::wstring(L"this is not a bson code"));
			yggr_test_assert(!bchk);
#endif // YGGR_NO_CWCHAR


			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), bs_code);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(code), bs_code_diff);
			yggr_test_assert(!bchk);


			bchk = (bs_code == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_code != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_code);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_code);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_code_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_code_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_SYMBOL
	{
		yggr::nsql_database_system::c_bson_symbol bs_symbol("this is a bson symbol");
		yggr::nsql_database_system::c_bson_symbol bs_symbol_diff("this is not a bson symbol");
		bson_value_type bs_val(bs_symbol);

		{
			bool bchk = false;

			//const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), "this is a bson symbol");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), "this is not a bson symbol");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), std::string("this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), std::string("this is not a bson symbol"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), boost::container::string("this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), boost::container::string("this is not a bson symbol"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), yggr::string("this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), yggr::string("this is not a bson symbol"));
			yggr_test_assert(!bchk);

			// yggr::utf8_string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), yggr::utf8_string("this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), yggr::utf8_string("this is not a bson symbol"));
			yggr_test_assert(!bchk);

#if !defined(YGGR_NO_CWCHAR)
			//const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), L"this is a bson symbol");
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), L"this is not a bson symbol");
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), std::wstring(L"this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), std::wstring(L"this is not a bson symbol"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), boost::container::wstring(L"this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), boost::container::wstring(L"this is not a bson symbol"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), yggr::wstring(L"this is a bson symbol"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), yggr::wstring(L"this is not a bson symbol"));
			yggr_test_assert(!bchk);
#endif // YGGR_NO_CWCHAR


			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), bs_symbol);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(symbol), bs_symbol_diff);
			yggr_test_assert(!bchk);


			bchk = (bs_symbol == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_symbol != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_symbol);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_symbol);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_symbol_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_symbol_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_CODEWSCOPE
	{
		yggr::nsql_database_system::c_bson_code_w_scope<bson_t>
			bs_cws(
				"this is a code w scope",
				yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		yggr::nsql_database_system::c_bson_code_w_scope<bson_t>
			bs_cws_diff(
				"this is a code w scope",
				yggr::nsql_database_system::c_bson("{\"aaa\" : 2}"));

		bson_value_type bs_val(bs_cws);

		{
			bool bchk = false;

			//const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), "this is a code w scope", bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), "this is a code w scope", bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), std::string("this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), std::string("this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), boost::container::string("this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), boost::container::string("this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), yggr::string("this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), yggr::string("this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// yggr::utf8_string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), yggr::utf8_string("this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), yggr::utf8_string("this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

#if !defined(YGGR_NO_CWCHAR)

			//const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), L"this is a code w scope", bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), L"this is a code w scope", bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), std::wstring(L"this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), std::wstring(L"this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), boost::container::wstring(L"this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), boost::container::wstring(L"this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), yggr::wstring(L"this is a code w scope"), bs_cws.scope<bson_t>());
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(codewscope), yggr::wstring(L"this is a code w scope"), bs_cws_diff.scope());
			yggr_test_assert(!bchk);

#endif // YGGR_NO_CWCHAR
		}
	}

	// E_BSON_TYPE_TIMESTAMP
	{
		yggr::nsql_database_system::c_bson_timestamp bs_tmstamp(1, 2);
		yggr::nsql_database_system::c_bson_timestamp bs_tmstamp_diff(1, 3);
		bson_value_type bs_val(bs_tmstamp);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), 1, 2);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), 1, 3);
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), bs_tmstamp);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), bs_tmstamp_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_tmstamp == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_tmstamp != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_tmstamp);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_tmstamp);
			yggr_test_assert(!bchk);

			bchk = (bs_tmstamp_diff == bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_tmstamp_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}

	// E_BSON_TYPE_DBPOINTER
	{
		typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
		typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;

		bson_dbpointer_type bs_dbptr("dbpointer", bson_oid_type("00112233445566778899aabb"));
		bson_dbpointer_type bs_dbptr_diff("dbpointer", bson_oid_type("00112233445566778899aacc"));
		bson_value_type bs_val(bs_dbptr);

		{
			bool bchk = false;

			// const char*
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), "dbpointer", bson_oid_type("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), "dbpointer", bson_oid_type("00112233445566778899aacc"));
			yggr_test_assert(!bchk);

			// std::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), std::string("dbpointer"), bson_oid_type("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), std::string("dbpointer"), bson_oid_type("00112233445566778899aacc"));
			yggr_test_assert(!bchk);

			// boost::container::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer),
										boost::container::string("dbpointer"), bson_oid_type("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer),
										boost::container::string("dbpointer"), bson_oid_type("00112233445566778899aacc"));
			yggr_test_assert(!bchk);

			// yggr::string
			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), yggr::string("dbpointer"), bson_oid_type("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), yggr::string("dbpointer"), bson_oid_type("00112233445566778899aacc"));
			yggr_test_assert(!bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), bs_dbptr);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(dbpointer), bs_dbptr_diff);
			yggr_test_assert(!bchk);

			bchk = (bs_dbptr == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs_dbptr != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs_dbptr);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs_dbptr);
			yggr_test_assert(!bchk);

			bchk = (bs_dbptr_diff == bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_dbptr_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}


	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : 1 }");
		yggr::nsql_database_system::c_bson bs_diff("{\"aaa\" : 2 }");
		bson_value_type bs_val(bs);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(document), bs);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(document), bs_diff);
			yggr_test_assert(!bchk);

			bchk = (bs == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs);
			yggr_test_assert(!bchk);

			bchk = (bs_diff == bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}

	{
		yggr::nsql_database_system::c_bson bs("{\"aaa\" : [1, 2, 3] }");
		yggr::nsql_database_system::c_bson bs_diff("{\"aaa\" : [1, 2, 3, 4] }");
		bson_value_type bs_val(bs);

		{
			bool bchk = false;

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(array), bs);
			yggr_test_assert(bchk);

			bchk = bs_val.compare_eq(YGGR_BSON_VALUE_TAG_OBJECT(array), bs_diff);
			yggr_test_assert(!bchk);

			bchk = (bs == bs_val);
			yggr_test_assert(bchk);

			bchk = (bs != bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_val == bs);
			yggr_test_assert(bchk);

			bchk = (bs_val != bs);
			yggr_test_assert(!bchk);

			bchk = (bs_diff == bs_val);
			yggr_test_assert(!bchk);

			bchk = (bs_diff != bs_val);
			yggr_test_assert(bchk);
		}
	}
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// value_typeid
	{
		bson_value_type bs_val;

		yggr_test_assert(bs_val.value_typeid() == bson_typeid_def_type::E_BSON_TYPE_EOD);
		yggr_test_assert(bson_value_type::s_value_typeid() == bson_typeid_def_type::E_BSON_TYPE_ANY_VALUE);
	}

	// org_pointer
	{
		bson_value_type bs_val;
		yggr_test_assert(bs_val.org_pointer() == boost::addressof(bs_val));
	}

	// operator pair
	{
		test_member_foo_operator_pair();
	}

	// assign
	{
		test_member_foo_assign();
	}

	// get_value
	{
		test_member_foo_get_value();
	}

	// compare_eq compare
	{
		test_member_foo_compare();
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	wait_any_key(argc, argv);
	return 0;
}
