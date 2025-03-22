// c_bson_oid_test2.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/array.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/network/hn_conv.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/bson_inner_data.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_value bson_val_type;

	{
		bson_oid_type oid;
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid("00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(std::string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid_sample(std::string("00112233445566778899aabb"));
		bson_oid_type oid(std::string(oid_sample.bytes, oid_sample.bytes + 12));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(boost::container::string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid_sample(boost::container::string("00112233445566778899aabb"));
		bson_oid_type oid(boost::container::string(oid_sample.bytes, oid_sample.bytes + 12));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid_sample(yggr::string("00112233445566778899aabb"));
		bson_oid_type oid(yggr::string(oid_sample.bytes, oid_sample.bytes + 12));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::utf8_string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(std::string("00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(boost::container::string("00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(yggr::string("00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(yggr::utf8_string("00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		bson_oid_type oid(L"00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(std::wstring(L"00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(boost::container::wstring(L"00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::wstring(L"00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(L"00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(std::wstring(L"00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(boost::container::wstring(L"00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid(yggr::charset::make_string_charset_helper(yggr::wstring(L"00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	{
		int arr[] = {1, 2, 3};
		bson_oid_type oid(arr);

		if(yggr::network::is_big_endian())
		{
			yggr_test_assert(oid == "000000010000000200000003");
		}
		else if(yggr::network::is_little_endian())
		{
			yggr_test_assert(oid == "010000000200000003000000");
		}
		else
		{
			yggr_test_assert(false);
		}
		std::cout << oid << std::endl;
	}

	{
		boost::array<int, 3> arr = {1, 2, 3};
		bson_oid_type oid(arr);

		if(yggr::network::is_big_endian())
		{
			yggr_test_assert(oid == "000000010000000200000003");
		}
		else if(yggr::network::is_little_endian())
		{
			yggr_test_assert(oid == "010000000200000003000000");
		}
		else
		{
			yggr_test_assert(false);
		}
		std::cout << oid << std::endl;
	}

	{
		int org_arr[] = {1, 2, 3};
		yggr::vector<int> arr(org_arr, org_arr + 3);
		bson_oid_type oid(arr);

		if(yggr::network::is_big_endian())
		{
			yggr_test_assert(oid == "000000010000000200000003");
		}
		else if(yggr::network::is_little_endian())
		{
			yggr_test_assert(oid == "010000000200000003000000");
		}
		else
		{
			yggr_test_assert(false);
		}
		std::cout << oid << std::endl;
	}

	{
		int arr[] = {1, 2, 3};
		bson_oid_type oid(arr + 0, arr + 3);

		if(yggr::network::is_big_endian())
		{
			yggr_test_assert(oid == "000000010000000200000003");
		}
		else if(yggr::network::is_little_endian())
		{
			yggr_test_assert(oid == "010000000200000003000000");
		}
		else
		{
			yggr_test_assert(false);
		}
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid("00112233445566778899aabb");
		bson_val_type val_oid(org_oid);
		bson_oid_type oid(val_oid);

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		//bson_oid_type::base_type& org_oid_ref = org_oid_smp;
		bson_oid_type oid(boost::move(org_oid_smp));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		bson_oid_type::base_type& org_oid_ref = org_oid_smp;
		bson_oid_type oid(org_oid_ref);

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		bson_oid_type oid(boost::move(org_oid_smp));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		bson_oid_type oid(org_oid_smp);

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_value bson_val_type;

	{
		bson_oid_type oid;
		oid = "00112233445566778899aabb";

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = std::string("00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = boost::container::string("00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::string("00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::utf8_string("00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::charset::make_string_charset_helper("00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::charset::make_string_charset_helper(std::string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::charset::make_string_charset_helper(boost::container::string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::charset::make_string_charset_helper(yggr::string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = yggr::charset::make_string_charset_helper(yggr::utf8_string("00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		bson_oid_type oid;
		oid = L"00112233445566778899aabb";

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = std::wstring(L"00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = boost::container::wstring(L"00112233445566778899aabb");

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = (yggr::wstring(L"00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = (yggr::charset::make_string_charset_helper(L"00112233445566778899aabb"));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = (yggr::charset::make_string_charset_helper(std::wstring(L"00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = (yggr::charset::make_string_charset_helper(boost::container::wstring(L"00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type oid;
		oid = (yggr::charset::make_string_charset_helper(yggr::wstring(L"00112233445566778899aabb")));

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	{
		int arr[] = {1, 2, 3};
		bson_oid_type oid;
		oid = arr;

		if(yggr::network::is_big_endian())
		{
			yggr_test_assert(oid == "000000010000000200000003");
		}
		else if(yggr::network::is_little_endian())
		{
			yggr_test_assert(oid == "010000000200000003000000");
		}
		else
		{
			yggr_test_assert(false);
		}
		std::cout << oid << std::endl;
	}

	{
		int org_arr[] = {1, 2, 3};
		yggr::vector<int> arr(org_arr, org_arr + 3);
		bson_oid_type oid;
		oid = arr;

		if(yggr::network::is_big_endian())
		{
			yggr_test_assert(oid == "000000010000000200000003");
		}
		else if(yggr::network::is_little_endian())
		{
			yggr_test_assert(oid == "010000000200000003000000");
		}
		else
		{
			yggr_test_assert(false);
		}
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid("00112233445566778899aabb");
		bson_val_type val_oid(org_oid);
		bson_oid_type oid;
		oid = val_oid;

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		//bson_oid_type::base_type& org_oid_ref = org_oid_smp;
		bson_oid_type oid;
		oid = boost::move(org_oid_smp);

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		bson_oid_type::base_type& org_oid_ref = org_oid_smp;
		bson_oid_type oid;
		oid = org_oid_ref;

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		bson_oid_type oid;
		oid = boost::move(org_oid_smp);

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	{
		bson_oid_type org_oid_smp("00112233445566778899aabb");
		bson_oid_type oid;
		oid = org_oid_smp;

		yggr_test_assert(oid == "00112233445566778899aabb");
		std::cout << oid << std::endl;
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_value bson_val_type;

	bson_oid_type oid1_chk("00112233445566778899aabb");
	bson_oid_type oid2_chk("445566778899aabbccddeeff");

	bson_oid_type oid1(oid1_chk);
	bson_oid_type oid2(oid2_chk);

	bson_oid_type::org_type& base_oid1 = oid1;
	bson_oid_type::org_type& base_oid2 = oid2;

	{
		{
			oid1.swap(boost::move(base_oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			oid1.swap(base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			oid1.swap(boost::move(oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			oid1.swap(oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		// swap org org
		//std::swap
		{
			std::swap(base_oid1, base_oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			std::swap(boost::move(base_oid1), base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			std::swap(base_oid1, boost::move(base_oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		// boost::swap
		{
			boost::swap(base_oid1, base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			boost::swap(boost::move(base_oid1), base_oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			boost::swap(base_oid1, boost::move(base_oid2));

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		// yggr::swap
		{
			yggr::swap(base_oid1, base_oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			yggr::swap(boost::move(base_oid1), base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			yggr::swap(base_oid1, boost::move(base_oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		// swap org yggr
		// std::swap
		{
			std::swap(base_oid1, oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			std::swap(boost::move(base_oid1), oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			std::swap(base_oid1, boost::move(oid2));

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		// boost::swap
		{
			boost::swap(base_oid1, oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			boost::swap(boost::move(base_oid1), oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			boost::swap(base_oid1, boost::move(oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		// yggr::swap
		{
			yggr::swap(base_oid1, oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			yggr::swap(boost::move(base_oid1), oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			yggr::swap(base_oid1, boost::move(oid2));

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		// swap yggr org
		//std::swap
		{
			std::swap(oid1, base_oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			std::swap(boost::move(oid1), base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			std::swap(oid1, boost::move(base_oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		// boost::swap
		{
			boost::swap(oid1, base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			boost::swap(boost::move(oid1), base_oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			boost::swap(oid1, boost::move(base_oid2));

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		// yggr::swap
		{
			yggr::swap(oid1, base_oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			yggr::swap(boost::move(oid1), base_oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			yggr::swap(oid1, boost::move(base_oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		// swap yggr yggr
		// std::swap
		{
			std::swap(oid1, oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			std::swap(boost::move(oid1), oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			std::swap(oid1, boost::move(oid2));

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		// boost::swap
		{
			boost::swap(oid1, oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			boost::swap(boost::move(oid1), oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			boost::swap(oid1, boost::move(oid2));

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		// yggr::swap
		{
			yggr::swap(oid1, oid2);

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}

		{
			yggr::swap(boost::move(oid1), oid2);

			yggr_test_assert(oid1 == oid2_chk);
			yggr_test_assert(oid2 == oid1_chk);
		}

		{
			yggr::swap(oid1, boost::move(oid2));

			yggr_test_assert(oid1 == oid1_chk);
			yggr_test_assert(oid2 == oid2_chk);
		}
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_value bson_val_type;
	typedef yggr::nsql_database_system::c_bson bson_type;

	// value_typeid
	{
		yggr_test_assert(bson_oid_type::s_value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
		yggr_test_assert(bson_oid_type().value_typeid() == bson_typeid_def_type::E_BSON_TYPE_OID);
	}

	// to_string
	{
		yggr::utf8_string str_oid_chk = "00112233445566778899aabb";

		bson_oid_type oid("00112233445566778899aabb");

		bool bchk = false;

		bchk = (oid.to_string() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string(str_oid_chk.get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<std::string>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<std::string>(str_oid_chk.str<std::string>().get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<boost::container::string>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<boost::container::string>(str_oid_chk.str<boost::container::string>().get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<yggr::string>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<yggr::string>(str_oid_chk.str<yggr::string>().get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<yggr::utf8_string>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<yggr::utf8_string>(str_oid_chk.get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
		bchk = (oid.to_string<std::wstring>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<std::wstring>(str_oid_chk.str<std::wstring>().get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<boost::container::wstring>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<boost::container::wstring>(str_oid_chk.str<boost::container::wstring>().get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<yggr::wstring>() == str_oid_chk);
		yggr_test_assert(bchk);

		bchk = (oid.to_string<yggr::wstring>(str_oid_chk.str<yggr::wstring>().get_allocator()) == str_oid_chk);
		yggr_test_assert(bchk);
#endif // YGGR_NO_CWCHAR

	}

	// operator type
	{
		yggr::utf8_string str_oid_chk = "00112233445566778899aabb";

		bson_oid_type oid("00112233445566778899aabb");

		bool bchk = false;

		{
			yggr::utf8_string str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

		{
			std::string str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

		{
			boost::container::string str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

		{
			yggr::string str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			std::wstring str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

		{
			boost::container::wstring str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

		{
			yggr::wstring str = oid;
			bchk = (str == oid);
			yggr_test_assert(bchk);
		}

#endif // YGGR_NO_CWCHAR

	}

	// assign
	{
		{
			bson_oid_type oid;
			oid.assign("00112233445566778899aabb");

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(std::string("00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid_sample(std::string("00112233445566778899aabb"));
			bson_oid_type oid;
			oid.assign(std::string(oid_sample.bytes, oid_sample.bytes + 12));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(boost::container::string("00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid_sample(boost::container::string("00112233445566778899aabb"));
			bson_oid_type oid;
			oid.assign(boost::container::string(oid_sample.bytes, oid_sample.bytes + 12));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::string("00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid_sample(yggr::string("00112233445566778899aabb"));
			bson_oid_type oid;
			oid.assign(yggr::string(oid_sample.bytes, oid_sample.bytes + 12));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::utf8_string("00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper("00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(std::string("00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(boost::container::string("00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(yggr::string("00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(yggr::utf8_string("00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_oid_type oid;
			oid.assign(L"00112233445566778899aabb");

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(std::wstring(L"00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(boost::container::wstring(L"00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::wstring(L"00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(L"00112233445566778899aabb"));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(std::wstring(L"00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(boost::container::wstring(L"00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type oid;
			oid.assign(yggr::charset::make_string_charset_helper(yggr::wstring(L"00112233445566778899aabb")));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

#endif // YGGR_NO_CWCHAR

		{
			int arr[] = {1, 2, 3};
			bson_oid_type oid;
			oid.assign(arr);

			if(yggr::network::is_big_endian())
			{
				yggr_test_assert(oid == "000000010000000200000003");
			}
			else if(yggr::network::is_little_endian())
			{
				yggr_test_assert(oid == "010000000200000003000000");
			}
			else
			{
				yggr_test_assert(false);
			}
		}

		{
			boost::array<int, 3> arr = {1, 2, 3};
			bson_oid_type oid;
			oid.assign(arr);

			if(yggr::network::is_big_endian())
			{
				yggr_test_assert(oid == "000000010000000200000003");
			}
			else if(yggr::network::is_little_endian())
			{
				yggr_test_assert(oid == "010000000200000003000000");
			}
			else
			{
				yggr_test_assert(false);
			}
		}

		{
			int org_arr[] = {1, 2, 3};
			yggr::vector<int> arr(org_arr, org_arr + 3);
			bson_oid_type oid;
			oid.assign(arr);

			if(yggr::network::is_big_endian())
			{
				yggr_test_assert(oid == "000000010000000200000003");
			}
			else if(yggr::network::is_little_endian())
			{
				yggr_test_assert(oid == "010000000200000003000000");
			}
			else
			{
				yggr_test_assert(false);
			}
		}

		{
			int arr[] = {1, 2, 3};
			bson_oid_type oid;
			oid.assign(arr + 0, arr + 3);

			if(yggr::network::is_big_endian())
			{
				yggr_test_assert(oid == "000000010000000200000003");
			}
			else if(yggr::network::is_little_endian())
			{
				yggr_test_assert(oid == "010000000200000003000000");
			}
			else
			{
				yggr_test_assert(false);
			}
		}

		{
			bson_oid_type org_oid("00112233445566778899aabb");
			bson_val_type val_oid(org_oid);
			bson_oid_type oid;
			oid.assign(val_oid);

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			//bson_oid_type::base_type& org_oid_ref = org_oid_smp;
			bson_oid_type oid;
			oid.assign(boost::move(org_oid_smp));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type::base_type& org_oid_ref = org_oid_smp;
			bson_oid_type oid;
			oid.assign(org_oid_ref);

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type oid;
			oid.assign(boost::move(org_oid_smp));

			yggr_test_assert(oid == "00112233445566778899aabb");
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type oid;
			oid.assign(org_oid_smp);

			yggr_test_assert(oid == "00112233445566778899aabb");
		}
	}

	// compare_eq
	{
		{
			bson_oid_type oid("00112233445566778899aabb");

			bool bchk = false;

			bchk = oid.compare_eq("00112233445566778899aabb");
			yggr_test_assert(bchk);

			bchk = oid.compare_eq("00112233445566778899aacc");
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(std::string("00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(std::string("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(std::string("00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(std::string("00112233445566778899aabb"));
			bson_oid_type oid_diff(std::string("00112233445566778899aacc"));

			bool bchk = false;

			bchk = oid.compare_eq(std::string(oid.bytes, oid.bytes + 12));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(std::string(oid_diff.bytes, oid_diff.bytes + 12));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(boost::container::string("00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(boost::container::string("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(boost::container::string("00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(boost::container::string("00112233445566778899aabb"));
			bson_oid_type oid_diff(boost::container::string("00112233445566778899aacc"));

			bool bchk = false;

			bchk = oid.compare_eq(boost::container::string(oid.bytes, oid.bytes + 12));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(boost::container::string(oid_diff.bytes, oid_diff.bytes + 12));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::string("00112233445566778899aabb"));

			bool bchk = oid.compare_eq(yggr::string("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::string("00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::string("00112233445566778899aabb"));
			bson_oid_type oid_diff(yggr::string("00112233445566778899aacc"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::string(oid.bytes, oid.bytes + 12));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::string(oid_diff.bytes, oid_diff.bytes + 12));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::utf8_string("00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::utf8_string("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::utf8_string("00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::utf8_string("00112233445566778899aabb"));
			bson_oid_type oid_diff(yggr::utf8_string("00112233445566778899aacc"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::utf8_string(oid.bytes, oid.bytes + 12, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::utf8_string(oid_diff.bytes, oid_diff.bytes + 12, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::string_view("00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::string_view("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::string_view("00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::string_view("00112233445566778899aabb"));
			bson_oid_type oid_diff(yggr::string_view("00112233445566778899aacc"));

			bool bchk = false;

			bchk = oid.compare_eq(
					yggr::string_view(
						reinterpret_cast<const char*>(oid.bytes),
						reinterpret_cast<const char*>(oid.bytes + 12) ));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(
					yggr::string_view(
						reinterpret_cast<const char*>(oid_diff.bytes),
						reinterpret_cast<const char*>(oid_diff.bytes + 12) ));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::utf8_string_view("00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::utf8_string_view("00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::utf8_string_view("00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_oid_type oid(L"00112233445566778899aabb");

			bool bchk = false;

			bchk = oid.compare_eq(L"00112233445566778899aabb");
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(L"00112233445566778899aacc");
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(std::wstring(L"00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(std::wstring(L"00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(std::wstring(L"00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(boost::container::wstring(L"00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(boost::container::wstring(L"00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(boost::container::wstring(L"00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::wstring(L"00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::wstring(L"00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::wstring(L"00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type oid(yggr::wstring_view(L"00112233445566778899aabb"));

			bool bchk = false;

			bchk = oid.compare_eq(yggr::wstring_view(L"00112233445566778899aabb"));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(yggr::wstring_view(L"00112233445566778899aacc"));
			yggr_test_assert(!bchk);
		}

#endif // YGGR_NO_CWCHAR

		{
			int arr[] = {1, 2, 3};
			int arr2[] = {1, 2, 2};
			bson_oid_type oid(arr);

			bool bchk = false;

			bchk = oid.compare_eq(arr);
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(arr2);
			yggr_test_assert(!bchk);
		}

		{
			boost::array<int, 3> arr = {1, 2, 3};
			boost::array<int, 3> arr2 = {1, 2, 2};
			bson_oid_type oid(arr);

			bool bchk = false;

			bchk = oid.compare_eq(arr);
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(arr2);
			yggr_test_assert(!bchk);
		}

		{
			int org_arr[] = {1, 2, 3};
			int org_arr2[] = {1, 2, 3};
			yggr::vector<int> arr(org_arr, org_arr + 3);
			yggr::vector<int> arr2(org_arr2, org_arr2 + 3);
			bson_oid_type oid(arr);

			bool bchk = false;

			bchk = oid.compare_eq(arr);
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(arr2);
			yggr_test_assert(bchk);
		}


		{
			bson_oid_type oid("00112233445566778899aabb");
			bson_oid_type oid2("00112233445566778899aacc");
			bson_val_type val_oid(oid);
			bson_val_type val_oid2(oid2);

			bool bchk = false;

			bchk = oid.compare_eq(static_cast<const bson_val_type::org_type&>(val_oid));
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(val_oid);
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(static_cast<const bson_val_type::org_type&>(val_oid2));
			yggr_test_assert(!bchk);

			bchk = oid.compare_eq(val_oid2);
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type org_oid_smp2("00112233445566778899aacc");
			bson_oid_type::base_type& org_oid_ref = org_oid_smp;
			bson_oid_type::base_type& org_oid_ref2 = org_oid_smp2;

			bson_oid_type oid = org_oid_smp;

			bool bchk = false;

			bchk = oid.compare_eq(org_oid_ref);
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(org_oid_ref2);
			yggr_test_assert(!bchk);
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type org_oid_smp2("00112233445566778899aacc");
			bson_oid_type oid = org_oid_smp;

			bool bchk = false;

			bchk = oid.compare_eq(org_oid_smp);
			yggr_test_assert(bchk);

			bchk = oid.compare_eq(org_oid_smp2);
			yggr_test_assert(!bchk);
		}
	}

	// compare
	{
		{
			bson_oid_type oid("00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare("00112233445566778899aabb");
			yggr_test_assert(nchk == 0);

			nchk = oid.compare("00112233445566778899aacc");
			yggr_test_assert(nchk == -1);

			nchk = oid.compare("00112233445566778899aaaa");
			yggr_test_assert(nchk == 1);

		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare(std::string("00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(std::string("00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(std::string("00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			bson_oid_type oid_greater("00112233445566778899aacc");
			bson_oid_type oid_less("00112233445566778899aaaa");
			yggr::s32 nchk = 0;

			nchk = oid.compare(std::string(oid.bytes, oid.bytes + 12));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(std::string(oid_greater.bytes, oid_greater.bytes + 12));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(std::string(oid_less.bytes, oid_less.bytes + 12));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare(boost::container::string("00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(boost::container::string("00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(boost::container::string("00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			bson_oid_type oid_greater("00112233445566778899aacc");
			bson_oid_type oid_less("00112233445566778899aaaa");
			yggr::s32 nchk = 0;

			nchk = oid.compare(boost::container::string(oid.bytes, oid.bytes + 12));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(boost::container::string(oid_greater.bytes, oid_greater.bytes + 12));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(boost::container::string(oid_less.bytes, oid_less.bytes + 12));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare(yggr::string("00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(yggr::string("00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(yggr::string("00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			bson_oid_type oid_greater("00112233445566778899aacc");
			bson_oid_type oid_less("00112233445566778899aaaa");
			yggr::s32 nchk = 0;

			nchk = oid.compare(yggr::string(oid.bytes, oid.bytes + 12));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(yggr::string(oid_greater.bytes, oid_greater.bytes + 12));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(yggr::string(oid_less.bytes, oid_less.bytes + 12));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare(yggr::utf8_string("00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(yggr::utf8_string("00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(yggr::utf8_string("00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_oid_type oid(L"00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare(L"00112233445566778899aabb");
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(L"00112233445566778899aacc");
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(L"00112233445566778899aaaa");
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid(L"00112233445566778899aabb");
			yggr::s32 nchk = 0;

			nchk = oid.compare(std::wstring(L"00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(std::wstring(L"00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(std::wstring(L"00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid(boost::container::wstring(L"00112233445566778899aabb"));
			yggr::s32 nchk = 0;

			nchk = oid.compare(boost::container::wstring(L"00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(boost::container::wstring(L"00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(boost::container::wstring(L"00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid(yggr::wstring(L"00112233445566778899aabb"));
			yggr::s32 nchk = 0;

			nchk = oid.compare(yggr::wstring(L"00112233445566778899aabb"));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(yggr::wstring(L"00112233445566778899aacc"));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(yggr::wstring(L"00112233445566778899aaaa"));
			yggr_test_assert(nchk == 1);
		}

#endif // YGGR_NO_CWCHAR

		{
			int arr[] = {1, 2, 3};
			int arr2[] = {1, 2, 4};
			int arr3[] = {1, 2, 2};

			bson_oid_type oid(arr);
			yggr::s32 nchk = 0;

			nchk = oid.compare(arr);
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(arr2);
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(arr3);
			yggr_test_assert(nchk == 1);
		}

		{
			boost::array<int, 3> arr = {1, 2, 3};
			boost::array<int, 3> arr2 = {1, 2, 4};
			boost::array<int, 3> arr3 = {1, 2, 2};

			bson_oid_type oid(arr);
			yggr::s32 nchk = 0;

			nchk = oid.compare(arr);
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(arr2);
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(arr3);
			yggr_test_assert(nchk == 1);
		}

		{
			int org_arr[] = {1, 2, 3};
			int org_arr2[] = {1, 2, 4};
			int org_arr3[] = {1, 2, 2};
			yggr::vector<int> arr(org_arr, org_arr + 3);
			yggr::vector<int> arr2(org_arr2, org_arr2 + 3);
			yggr::vector<int> arr3(org_arr3, org_arr3 + 3);

			bson_oid_type oid(arr);
			yggr::s32 nchk = 0;

			nchk = oid.compare(arr);
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(arr2);
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(arr3);
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type oid("00112233445566778899aabb");

			bson_val_type val_oid(oid);
			bson_val_type val_oid2(bson_oid_type("00112233445566778899aacc"));
			bson_val_type val_oid3(bson_oid_type("00112233445566778899aaaa"));

			yggr::s32 nchk = 0;

			nchk = oid.compare(static_cast<bson_val_type::org_type&>(val_oid));
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(val_oid);
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(static_cast<bson_val_type::org_type&>(val_oid2));
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(val_oid2);
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(static_cast<bson_val_type::org_type&>(val_oid3));
			yggr_test_assert(nchk == 1);

			nchk = oid.compare(val_oid3);
			yggr_test_assert(nchk == 1);
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type org_oid_smp2("00112233445566778899aacc");
			bson_oid_type org_oid_smp3("00112233445566778899aaaa");

			bson_oid_type::base_type& org_oid_ref = org_oid_smp;
			bson_oid_type::base_type& org_oid_ref2 = org_oid_smp2;
			bson_oid_type::base_type& org_oid_ref3 = org_oid_smp3;

			bson_oid_type oid = org_oid_smp;
			yggr::s32 nchk = 0;

			nchk = oid.compare(org_oid_ref);
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(org_oid_smp);
			yggr_test_assert(nchk == 0);

			nchk = oid.compare(org_oid_ref2);
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(org_oid_smp2);
			yggr_test_assert(nchk == -1);

			nchk = oid.compare(org_oid_ref3);
			yggr_test_assert(nchk == 1);

			nchk = oid.compare(org_oid_smp3);
			yggr_test_assert(nchk == 1);
		}
	}

	//gen_oid
	{
		std::cout << bson_oid_type::s_gen_empty_oid() << std::endl;
		yggr_test_assert(bson_oid_type::s_gen_empty_oid() == bson_oid_type("000000000000000000000000"));

		std::cout << bson_oid_type::s_gen_oid() << std::endl;
		yggr_test_assert(bson_oid_type::s_gen_oid() != bson_oid_type::s_gen_empty_oid());
	}

	// output
	{
		bson_oid_type bs_oid("00112233445566778899aabb");

		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_oid) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_oid) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_oid) << std::endl;
	}

	{
		bson_oid_type bs_oid("00112233445566778899aabb");

		std::string chk_str = "{ \"$oid\" : \"00112233445566778899aabb\" }";

		{
			std::stringstream ss;
			ss << bs_oid;
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_oid);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_oid);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_oid);
			yggr_test_assert(ss.str() == chk_str);
		}
	}

	//save load
	{
		bson_oid_type bs_oid;

		bson_type bs;
		bs.save("c_bson_oid", bs_oid);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_oid_type ld_bs_oid;
		bson_type::iterator bs_iter(bs, "c_bson_oid");
		bs_iter.load("c_bson_oid", ld_bs_oid);

		yggr_test_assert(bs_oid == ld_bs_oid);
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

void test_compare(void)
{
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_value bson_val_type;

	// compare
	{
		{
			bson_oid_type oid("00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == "00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aabb" == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == "00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aabb" == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != "00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aaaa" != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != "00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aaaa" != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < "00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aaaa" < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < "00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aaaa" < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= "00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aabb" <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= "00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aabb" <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= "00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aaaa" <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= "00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aaaa" <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > "00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aacc" > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > "00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aacc" > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= "00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aabb" >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= "00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aabb" >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= "00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aacc" >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= "00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = ("00112233445566778899aacc" >= oid);
				yggr_test_assert(bchk);
			}
		}

		// std::string
		{
			bson_oid_type oid("00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == std::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == std::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != std::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != std::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < std::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < std::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= std::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= std::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= std::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= std::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > std::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > std::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= std::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= std::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= std::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= std::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::string("00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}

		// boost::container::string
		{
			bson_oid_type oid("00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == boost::container::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == boost::container::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != boost::container::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != boost::container::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < boost::container::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < boost::container::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= boost::container::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= boost::container::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= boost::container::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= boost::container::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > boost::container::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > boost::container::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= boost::container::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= boost::container::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= boost::container::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= boost::container::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::string("00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}

		// yggr::string
		{
			bson_oid_type oid("00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == yggr::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == yggr::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != yggr::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != yggr::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < yggr::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < yggr::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= yggr::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= yggr::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= yggr::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= yggr::string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > yggr::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > yggr::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= yggr::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= yggr::string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= yggr::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= yggr::string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::string("00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}

		// yggr::utf8_string
		{
			bson_oid_type oid("00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == yggr::utf8_string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == yggr::utf8_string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != yggr::utf8_string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != yggr::utf8_string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < yggr::utf8_string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < yggr::utf8_string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= yggr::utf8_string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= yggr::utf8_string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= yggr::utf8_string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= yggr::utf8_string("00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > yggr::utf8_string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > yggr::utf8_string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= yggr::utf8_string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= yggr::utf8_string("00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= yggr::utf8_string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= yggr::utf8_string("00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::utf8_string("00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			bson_oid_type oid(L"00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == L"00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aabb" == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == L"00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aabb" == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != L"00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aaaa" != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != L"00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aaaa" != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < L"00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aaaa" < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < L"00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aaaa" < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= L"00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aabb" <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= L"00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aabb" <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= L"00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aaaa" <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= L"00112233445566778899aacc");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aaaa" <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > L"00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aacc" > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > L"00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aacc" > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= L"00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aabb" >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= L"00112233445566778899aabb");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aabb" >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= L"00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aacc" >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= L"00112233445566778899aaaa");
				yggr_test_assert(bchk);

				bchk = (L"00112233445566778899aacc" >= oid);
				yggr_test_assert(bchk);
			}
		}

		// std::wstring
		{
			bson_oid_type oid(L"00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == std::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == std::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != std::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != std::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < std::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < std::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= std::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= std::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= std::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= std::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > std::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > std::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= std::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= std::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= std::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= std::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (std::wstring(L"00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}

		// boost::container::wstring
		{
			bson_oid_type oid(L"00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == boost::container::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == boost::container::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != boost::container::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != boost::container::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < boost::container::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < boost::container::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= boost::container::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= boost::container::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= boost::container::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= boost::container::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > boost::container::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > boost::container::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= boost::container::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= boost::container::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= boost::container::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= boost::container::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (boost::container::wstring(L"00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}

		// yggr::wstring
		{
			bson_oid_type oid(L"00112233445566778899aabb");

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == yggr::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aabb") == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == yggr::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aabb") == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != yggr::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aaaa") != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != yggr::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aaaa") != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < yggr::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aaaa") < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < yggr::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aaaa") < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= yggr::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aabb") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= yggr::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aabb") <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= yggr::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aaaa") <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= yggr::wstring(L"00112233445566778899aacc"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aaaa") <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > yggr::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aacc") > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > yggr::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aacc") > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= yggr::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aabb") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= yggr::wstring(L"00112233445566778899aabb"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aabb") >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= yggr::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aacc") >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= yggr::wstring(L"00112233445566778899aaaa"));
				yggr_test_assert(bchk);

				bchk = (yggr::wstring(L"00112233445566778899aacc") >= oid);
				yggr_test_assert(bchk);
			}
		}
#endif // YGGR_NO_CWCHAR

		{
			int arr[] = {1, 2, 3};
			int arr2[] = {1, 2, 4};
			int arr3[] = {1, 2, 2};

			bson_oid_type oid(arr);

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == arr);
				yggr_test_assert(bchk);

				bchk = (arr == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == arr);
				yggr_test_assert(bchk);

				bchk = (arr == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != arr2);
				yggr_test_assert(bchk);

				bchk = (arr2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != arr2);
				yggr_test_assert(bchk);

				bchk = (arr2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= arr);
				yggr_test_assert(bchk);

				bchk = (arr <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= arr);
				yggr_test_assert(bchk);

				bchk = (arr <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= arr);
				yggr_test_assert(bchk);

				bchk = (arr >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= arr);
				yggr_test_assert(bchk);

				bchk = (arr >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 >= oid);
				yggr_test_assert(bchk);
			}
		}

		{
			boost::array<int, 3> arr = {1, 2, 3};
			boost::array<int, 3> arr2 = {1, 2, 4};
			boost::array<int, 3> arr3 = {1, 2, 2};

			bson_oid_type oid(arr);

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == arr);
				yggr_test_assert(bchk);

				bchk = (arr == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == arr);
				yggr_test_assert(bchk);

				bchk = (arr == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != arr2);
				yggr_test_assert(bchk);

				bchk = (arr2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != arr2);
				yggr_test_assert(bchk);

				bchk = (arr2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= arr);
				yggr_test_assert(bchk);

				bchk = (arr <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= arr);
				yggr_test_assert(bchk);

				bchk = (arr <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= arr);
				yggr_test_assert(bchk);

				bchk = (arr >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= arr);
				yggr_test_assert(bchk);

				bchk = (arr >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 >= oid);
				yggr_test_assert(bchk);
			}
		}

		{
			int org_arr[] = {1, 2, 3};
			int org_arr2[] = {1, 2, 4};
			int org_arr3[] = {1, 2, 2};

			yggr::vector<int> arr(org_arr, org_arr + 3);
			yggr::vector<int> arr2(org_arr2, org_arr2 + 3);
			yggr::vector<int> arr3(org_arr3, org_arr3 + 3);

			bson_oid_type oid(arr);

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == arr);
				yggr_test_assert(bchk);

				bchk = (arr == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == arr);
				yggr_test_assert(bchk);

				bchk = (arr == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != arr2);
				yggr_test_assert(bchk);

				bchk = (arr2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != arr2);
				yggr_test_assert(bchk);

				bchk = (arr2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= arr);
				yggr_test_assert(bchk);

				bchk = (arr <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= arr);
				yggr_test_assert(bchk);

				bchk = (arr <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= arr2);
				yggr_test_assert(bchk);

				bchk = (arr3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= arr);
				yggr_test_assert(bchk);

				bchk = (arr >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= arr);
				yggr_test_assert(bchk);

				bchk = (arr >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= arr3);
				yggr_test_assert(bchk);

				bchk = (arr2 >= oid);
				yggr_test_assert(bchk);
			}
		}

		{
			bson_oid_type oid("00112233445566778899aabb");

			bson_val_type val_oid_chk(oid);
			bson_val_type val_oid2_chk(bson_oid_type("00112233445566778899aacc"));
			bson_val_type val_oid3_chk(bson_oid_type("00112233445566778899aaaa"));

			bson_val_type::org_type& val_oid = val_oid_chk;
			bson_val_type::org_type& val_oid2 = val_oid2_chk;
			bson_val_type::org_type& val_oid3 = val_oid3_chk;

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 >= oid);
				yggr_test_assert(bchk);
			}
		}

		{
			bson_oid_type oid("00112233445566778899aabb");

			bson_val_type val_oid(oid);
			bson_val_type val_oid2(bson_oid_type("00112233445566778899aacc"));
			bson_val_type val_oid3(bson_oid_type("00112233445566778899aaaa"));

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 >= oid);
				yggr_test_assert(bchk);
			}
		}

		{
			bson_oid_type oid("00112233445566778899aabb");

			bson_val_type val_oid_chk(oid);
			bson_val_type val_oid2_chk(bson_oid_type("00112233445566778899aacc"));
			bson_val_type val_oid3_chk(bson_oid_type("00112233445566778899aaaa"));

			bson_val_type::org_type& val_oid = val_oid_chk;
			bson_val_type::org_type& val_oid2 = val_oid2_chk;
			bson_val_type::org_type& val_oid3 = val_oid3_chk;

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= val_oid2);
				yggr_test_assert(bchk);

				bchk = (val_oid3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= val_oid);
				yggr_test_assert(bchk);

				bchk = (val_oid >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= val_oid3);
				yggr_test_assert(bchk);

				bchk = (val_oid2 >= oid);
				yggr_test_assert(bchk);
			}
		}

		{
			bson_oid_type org_oid_smp("00112233445566778899aabb");
			bson_oid_type org_oid_smp2("00112233445566778899aacc");
			bson_oid_type org_oid_smp3("00112233445566778899aaaa");

			bson_oid_type::base_type& org_oid_ref = org_oid_smp;
			bson_oid_type::base_type& org_oid_ref2 = org_oid_smp2;
			bson_oid_type::base_type& org_oid_ref3 = org_oid_smp3;

			bson_oid_type oid = org_oid_smp;

			bool bchk = false;

			// ==
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) == org_oid_ref);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref == static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid == org_oid_ref);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref == oid);
				yggr_test_assert(bchk);
			}

			// !=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) != org_oid_ref2);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref2 != static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid != org_oid_ref2);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref2 != oid);
				yggr_test_assert(bchk);
			}

			// <
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) < org_oid_ref2);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref3 < static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid < org_oid_ref2);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref3 < oid);
				yggr_test_assert(bchk);
			}

			// <=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= org_oid_ref);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= org_oid_ref);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref <= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) <= org_oid_ref2);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref3 <= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid <= org_oid_ref2);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref3 <= oid);
				yggr_test_assert(bchk);
			}

			// >
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) > org_oid_ref3);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref2 > static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid > org_oid_ref3);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref2 > oid);
				yggr_test_assert(bchk);
			}

			// >=
			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= org_oid_ref);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= org_oid_ref);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref >= oid);
				yggr_test_assert(bchk);
			}

			{
				bchk = (static_cast<const bson_oid_type::org_type&>(oid) >= org_oid_ref3);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref2 >= static_cast<const bson_oid_type::org_type&>(oid));
				yggr_test_assert(bchk);

				bchk = (oid >= org_oid_ref3);
				yggr_test_assert(bchk);

				bchk = (org_oid_ref2 >= oid);
				yggr_test_assert(bchk);
			}
		}
	}

	std::cout << "-----------------test_compare end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();
	test_compare();

	wait_any_key(argc, argv);
	return 0;
}

