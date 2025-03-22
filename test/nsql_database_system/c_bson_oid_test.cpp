//c_bson_oid_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/array.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/bson_inner_data.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // #ifndef YGGR_NO_CWCHAR

	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	{
		bson_oid_type oid;
		std::string str = oid;
		std::cout << str << std::endl;
	}

	{
		bson_oid_type::org_type oid = {0};
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		std::cout << oid << std::endl;
	}

	{
		const char* str_id = "00112233445566778899aabb";
		const char* str_id_diff = "00112233445566778899aabc";

		bson_oid_type oid(str_id);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
		}
	}

	{
		const wchar_t* str_id = L"00112233445566778899aabb";
		const wchar_t* str_id_diff = L"00112233445566778899aabc";

		bson_oid_type oid(str_id);
		std::wstring str = oid;
		
		std::wcout << str << std::endl;
		std::wcout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
		}
	}

	{
		std::string str_id("00112233445566778899aabb");
		std::string str_id_diff("00112233445566778899aabc");
		std::string str_id_diff2("00112233445566778899");
		std::string str_id_diff3("00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		boost::container::string str_id("00112233445566778899aabb");
		boost::container::string str_id_diff("00112233445566778899aabc");
		boost::container::string str_id_diff2("00112233445566778899");
		boost::container::string str_id_diff3("00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		boost::container::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		yggr::string str_id("00112233445566778899aabb");
		yggr::string str_id_diff("00112233445566778899aabc");
		yggr::string str_id_diff2("00112233445566778899");
		yggr::string str_id_diff3("00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		std::wstring str_id(L"00112233445566778899aabb");
		std::wstring str_id_diff(L"00112233445566778899aabc");
		std::wstring str_id_diff2(L"00112233445566778899");
		std::wstring str_id_diff3(L"00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		std::wstring str = oid;
		
		std::wcout << str << std::endl;
		std::wcout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		boost::container::wstring str_id(L"00112233445566778899aabb");
		boost::container::wstring str_id_diff(L"00112233445566778899aabc");
		boost::container::wstring str_id_diff2(L"00112233445566778899");
		boost::container::wstring str_id_diff3(L"00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		boost::container::wstring str = oid;
		
		std::wcout << str << std::endl;
		std::wcout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		yggr::wstring str_id(L"00112233445566778899aabb");
		yggr::wstring str_id_diff(L"00112233445566778899aabc");
		yggr::wstring str_id_diff2(L"00112233445566778899");
		yggr::wstring str_id_diff3(L"00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		std::wstring str = oid;
		
		std::wcout << str << std::endl;
		std::wcout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		yggr::utf8_string str_id("00112233445566778899aabb");
		yggr::utf8_string str_id_diff("00112233445566778899aabc");
		yggr::utf8_string str_id_diff2("00112233445566778899");
		yggr::utf8_string str_id_diff3("00112233445566778899aabbcc");

		bson_oid_type oid(str_id);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(str_id == oid);
		yggr_test_assert(!(str_id != oid));

		bson_oid_type oid2;
		oid2 = str_id;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(str_id_diff);

			yggr_test_assert((oid_same == str_id));
			yggr_test_assert((str_id == oid_same));
			yggr_test_assert(!(oid_diff == str_id));
			yggr_test_assert(!(str_id == oid_diff));
			yggr_test_assert(!(oid_same == str_id_diff2));
			yggr_test_assert(!(str_id_diff2 == oid_same));
			yggr_test_assert(!(oid_same == str_id_diff3));
			yggr_test_assert(!(str_id_diff3 == oid_same));

			yggr_test_assert(!(oid_same != str_id));
			yggr_test_assert(!(str_id != oid_same));
			yggr_test_assert((oid_diff != str_id));
			yggr_test_assert((str_id != oid_diff));
			yggr_test_assert((oid_same != str_id_diff2));
			yggr_test_assert((str_id_diff2 != oid_same));
			yggr_test_assert((oid_same != str_id_diff3));
			yggr_test_assert((str_id_diff3 != oid_same));

			yggr_test_assert(!(oid_same < str_id));
			yggr_test_assert(!(str_id < oid_same));
			yggr_test_assert(!(oid_diff < str_id));
			yggr_test_assert((str_id < oid_diff));
			yggr_test_assert(!(oid_same < str_id_diff2));
			yggr_test_assert((str_id_diff2 < oid_same));
			yggr_test_assert((oid_same < str_id_diff3));
			yggr_test_assert(!(str_id_diff3 < oid_same));

			yggr_test_assert((oid_same <= str_id));
			yggr_test_assert((str_id <= oid_same));
			yggr_test_assert(!(oid_diff <= str_id));
			yggr_test_assert((str_id <= oid_diff));
			yggr_test_assert(!(oid_same <= str_id_diff2));
			yggr_test_assert((str_id_diff2 <= oid_same));
			yggr_test_assert((oid_same <= str_id_diff3));
			yggr_test_assert(!(str_id_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > str_id));
			yggr_test_assert(!(str_id > oid_same));
			yggr_test_assert((oid_diff > str_id));
			yggr_test_assert(!(str_id > oid_diff));
			yggr_test_assert((oid_same > str_id_diff2));
			yggr_test_assert(!(str_id_diff2 > oid_same));
			yggr_test_assert(!(oid_same > str_id_diff3));
			yggr_test_assert((str_id_diff3 > oid_same));

			yggr_test_assert((oid_same >= str_id));
			yggr_test_assert((str_id >= oid_same));
			yggr_test_assert((oid_diff >= str_id));
			yggr_test_assert(!(str_id >= oid_diff));
			yggr_test_assert((oid_same >= str_id_diff2));
			yggr_test_assert(!(str_id_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= str_id_diff3));
			yggr_test_assert((str_id_diff3 >= oid_same));
		}
	}

	{
		yggr::u8 data[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
		yggr::u8 data_diff[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x67 };
		yggr::u8 data_diff2[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
		yggr::u8 data_diff3[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		yggr::u8 data[22] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
		yggr::u8 data_diff[22] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x67 };
		yggr::u8 data_diff2[22] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
		yggr::u8 data_diff3[22] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		yggr::u8 data[22] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };

		bson_oid_type oid(data + 1, data + 13);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(oid.compare_eq(data + 1, data + 13));
		yggr_test_assert(0 == oid.compare(data + 1, data + 13));


		bson_oid_type oid2;
		oid2 = data + 1;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

	}

	{
		int data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aa00) };
		int data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x9899aabb) };

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		int tmp_data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int tmp_data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int tmp_data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb) };
		int tmp_data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};
		std::vector<int> data(tmp_data, tmp_data + 3);
		std::vector<int> data_diff(tmp_data_diff, tmp_data_diff + 3);
		std::vector<int> data_diff2(tmp_data_diff2, tmp_data_diff2 + 2);
		std::vector<int> data_diff3(tmp_data_diff3, tmp_data_diff3 + 4);

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		int tmp_data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int tmp_data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int tmp_data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb) };
		int tmp_data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};
		boost::container::vector<int> data(tmp_data, tmp_data + 3);
		boost::container::vector<int> data_diff(tmp_data_diff, tmp_data_diff + 3);
		boost::container::vector<int> data_diff2(tmp_data_diff2, tmp_data_diff2 + 2);
		boost::container::vector<int> data_diff3(tmp_data_diff3, tmp_data_diff3 + 4);

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		int tmp_data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int tmp_data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int tmp_data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb) };
		int tmp_data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};
		yggr::vector<int> data(tmp_data, tmp_data + 3);
		yggr::vector<int> data_diff(tmp_data_diff, tmp_data_diff + 3);
		yggr::vector<int> data_diff2(tmp_data_diff2, tmp_data_diff2 + 2);
		yggr::vector<int> data_diff3(tmp_data_diff3, tmp_data_diff3 + 4);

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		int tmp_data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int tmp_data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int tmp_data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb) };
		int tmp_data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};
		std::list<int> data(tmp_data, tmp_data + 3);
		std::list<int> data_diff(tmp_data_diff, tmp_data_diff + 3);
		std::list<int> data_diff2(tmp_data_diff2, tmp_data_diff2 + 2);
		std::list<int> data_diff3(tmp_data_diff3, tmp_data_diff3 + 4);

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		int tmp_data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int tmp_data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int tmp_data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb) };
		int tmp_data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};
		boost::container::list<int> data(tmp_data, tmp_data + 3);
		boost::container::list<int> data_diff(tmp_data_diff, tmp_data_diff + 3);
		boost::container::list<int> data_diff2(tmp_data_diff2, tmp_data_diff2 + 2);
		boost::container::list<int> data_diff3(tmp_data_diff3, tmp_data_diff3 + 4);

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		int tmp_data[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		int tmp_data_diff[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		int tmp_data_diff2[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb) };
		int tmp_data_diff3[] = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};
		yggr::list<int> data(tmp_data, tmp_data + 3);
		yggr::list<int> data_diff(tmp_data_diff, tmp_data_diff + 3);
		yggr::list<int> data_diff2(tmp_data_diff2, tmp_data_diff2 + 2);
		yggr::list<int> data_diff3(tmp_data_diff3, tmp_data_diff3 + 4);

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}


	{
		boost::array<char, 12> data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
		boost::array<char, 12> data_diff = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x67 };
		boost::array<char, 11> data_diff2 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
		boost::array<char, 13> data_diff3 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };

		bson_oid_type oid(data);

		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		yggr::array<char, 12> data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
		yggr::array<char, 12> data_diff = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x67 };
		yggr::array<char, 11> data_diff2 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
		yggr::array<char, 13> data_diff3 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };

		bson_oid_type oid(data);

		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		boost::array<int, 3> data = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		boost::array<int, 3> data_diff = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		boost::array<int, 2> data_diff2 = {0x00112233, 0x44556677 };
		boost::array<int, 4> data_diff3 = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	{
		yggr::array<int, 3> data = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb)};
		yggr::array<int, 3> data_diff = {0x00112233, 0x44556677, static_cast<int>(0x8899aabc) };
		yggr::array<int, 2> data_diff2 = {0x00112233, 0x44556677 };
		yggr::array<int, 4> data_diff3 = {0x00112233, 0x44556677, static_cast<int>(0x8899aabb), static_cast<int>(0xccddeeff)};

		bson_oid_type oid(data);
		std::string str = oid;
		std::cout << str << std::endl;
		std::cout << oid << std::endl;
		yggr_test_assert(data == oid);
		yggr_test_assert(!(data != oid));

		bson_oid_type oid2;
		oid2 = data;
		yggr_test_assert(!(oid != oid2));

		bson_oid_type oid3(oid);
		bson_oid_type oid4;
		oid4 = oid2;
		yggr_test_assert(oid3 == oid4);

		{
			bson_oid_type oid_same = oid;
			bson_oid_type oid_diff(data_diff);

			yggr_test_assert((oid_same == data));
			yggr_test_assert((data == oid_same));
			yggr_test_assert(!(oid_diff == data));
			yggr_test_assert(!(data == oid_diff));
			yggr_test_assert(!(oid_same == data_diff2));
			yggr_test_assert(!(data_diff2 == oid_same));
			yggr_test_assert(!(oid_same == data_diff3));
			yggr_test_assert(!(data_diff3 == oid_same));

			yggr_test_assert(!(oid_same != data));
			yggr_test_assert(!(data != oid_same));
			yggr_test_assert((oid_diff != data));
			yggr_test_assert((data != oid_diff));
			yggr_test_assert((oid_same != data_diff2));
			yggr_test_assert((data_diff2 != oid_same));
			yggr_test_assert((oid_same != data_diff3));
			yggr_test_assert((data_diff3 != oid_same));

			yggr_test_assert(!(oid_same < data));
			yggr_test_assert(!(data < oid_same));
			yggr_test_assert(!(oid_diff < data));
			yggr_test_assert((data < oid_diff));
			yggr_test_assert(!(oid_same < data_diff2));
			yggr_test_assert((data_diff2 < oid_same));
			yggr_test_assert((oid_same < data_diff3));
			yggr_test_assert(!(data_diff3 < oid_same));

			yggr_test_assert((oid_same <= data));
			yggr_test_assert((data <= oid_same));
			yggr_test_assert(!(oid_diff <= data));
			yggr_test_assert((data <= oid_diff));
			yggr_test_assert(!(oid_same <= data_diff2));
			yggr_test_assert((data_diff2 <= oid_same));
			yggr_test_assert((oid_same <= data_diff3));
			yggr_test_assert(!(data_diff3 <= oid_same));

			yggr_test_assert(!(oid_same > data));
			yggr_test_assert(!(data > oid_same));
			yggr_test_assert((oid_diff > data));
			yggr_test_assert(!(data > oid_diff));
			yggr_test_assert((oid_same > data_diff2));
			yggr_test_assert(!(data_diff2 > oid_same));
			yggr_test_assert(!(oid_same > data_diff3));
			yggr_test_assert((data_diff3 > oid_same));

			yggr_test_assert((oid_same >= data));
			yggr_test_assert((data >= oid_same));
			yggr_test_assert((oid_diff >= data));
			yggr_test_assert(!(data >= oid_diff));
			yggr_test_assert((oid_same >= data_diff2));
			yggr_test_assert(!(data_diff2 >= oid_same));
			yggr_test_assert(!(oid_same >= data_diff3));
			yggr_test_assert((data_diff3 >= oid_same));
		}
	}

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	// output
	{
		yggr::string str_id("00112233445566778899aabb");
		bson_oid_type data(str_id);

		std::cout << data << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		yggr::string str_id("00112233445566778899aabb");
		bson_oid_type data(str_id);

		std::string chk_str = "{ \"$oid\" : \"00112233445566778899aabb\" }";

		{
			std::stringstream ss;
			ss << data;
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data);
			yggr_test_assert(ss.str() == chk_str);
		}
	}

	{
		bson_type bs;
		yggr::string str_id("00112233445566778899aabb");
		bson_oid_type data(str_id);
		bs.save("c_bson_oid", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_oid_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_oid");
		bs_iter.load("c_bson_oid", ld_data);

		yggr_test_assert(ld_data == data);
	}

	wait_any_key(argc, argv);
	return 0;
}
