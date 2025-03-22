//local_file_operator_test_part1.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <cassert>

#include <boost/bind.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/set.hpp>

#include <yggr/time/time.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

#if defined(YGGR_AT_WINDOWS)
#	define TEST_OF_ADMIN 0 // if you not want to test of admin set the macro to 0
#else
#	define TEST_OF_ADMIN 1
#endif // YGGR_AT_WINDOWS

#if defined(__ANDROID__) || defined(ANDROID)
#	define ANDROID_ROOTED 0 // if you android device is rooted, set ANDROID_ROOTED 1
#endif // #if defined(__ANDROID__) || defined(ANDROID)

#define TEST_STL_VERSION 0

#if TEST_STL_VERSION
	typedef yggr::file_system::local_fsys_basic_operators<
				yggr::file_system::stl_fstream_config_type> local_fsys_operators;
#else
	typedef yggr::file_system::local_fsys_operators local_fsys_operators;
#endif // TEST_STL_VERSION

typedef yggr::file_system::rename_of_unique<yggr::file_system::rename_ext_bak> custom_unique_type;

typedef yggr::file_system::text_separator<char[3]> custom_spr_type;

char str_spr[] = ", ";
custom_spr_type custom_spr(str_spr);

void test_current_path(void)
{
	local_fsys_operators::path_type org_current_path;
	local_fsys_operators::path_type now_current_path;

	try
	{
		org_current_path = local_fsys_operators::current_path();
		now_current_path =
			local_fsys_operators::system_complete(
				local_fsys_operators::make_path_object("../test_out/file_system/test_current_path"));
		local_fsys_operators::create_directories(now_current_path); // create directories
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bright = false;
		try
		{
			// set current path
			bright =
				local_fsys_operators::current_path(local_fsys_operators::make_path_object("../"));
			yggr_test_assert(bright);

			// out current path
			std::cout << local_fsys_operators::current_path() << std::endl;
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cout << e.what() << std::endl;
			yggr_test_assert(false);
			return;
		}

		local_fsys_operators::error_code_type ec;
		bright =
			local_fsys_operators::current_path(now_current_path, ec);
		yggr_test_assert(bright);
		yggr_test_assert(!ec);
		std::cout << local_fsys_operators::current_path(ec) << std::endl;
		yggr_test_assert(!ec);
		local_fsys_operators::current_path(local_fsys_operators::make_path_object("../"), ec);
	}

	try
	{
		local_fsys_operators::remove_all(now_current_path);
		local_fsys_operators::current_path(org_current_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_current_path succeed-------------" << std::endl;
}

void test_root_path(void)
{
	try
	{
		yggr_test_assert(local_fsys_operators::root_path() == local_fsys_operators::current_path().root_path());
		std::cout << local_fsys_operators::root_path() << std::endl;
	}
	catch(local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_root_path succeed-------------" << std::endl;
}

void test_absolute(void)
{
	local_fsys_operators::path_type now_current_path;

	try
	{
		now_current_path =
			local_fsys_operators::system_complete(
				local_fsys_operators::make_path_object("../test_out/file_system/"));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << local_fsys_operators::absolute(now_current_path) << std::endl;

	std::cout << "--------------test_absolute succeed-------------" << std::endl;
}

void test_canonical(void)
{
	local_fsys_operators::path_type now_current_path;
	now_current_path = local_fsys_operators::make_path_object("../test_out/file_system/");

	try
	{
		std::cout << local_fsys_operators::canonical(now_current_path) << std::endl;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec;
		std::cout << local_fsys_operators::canonical(now_current_path, ec) << std::endl;
		yggr_test_assert(!ec);
	}

	std::cout << "--------------test_canonical succeed-------------" << std::endl;

}

void test_is_exists(void)
{
	bool bexists = false;
	try
	{
		bexists = local_fsys_operators::is_exists(local_fsys_operators::make_path_object("../test_out/file_system/"));
		yggr_test_assert(bexists);
		bexists = local_fsys_operators::is_exists(local_fsys_operators::make_path_object("./file_system/test_out_not_exists/"));
		yggr_test_assert(!bexists);
	}
	catch(local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec;
		bexists = local_fsys_operators::is_exists(local_fsys_operators::make_path_object("../test_out/file_system/"), ec);
		yggr_test_assert(bexists);
		yggr_test_assert(!ec);

		bexists = local_fsys_operators::is_exists(local_fsys_operators::make_path_object("./file_system/test_out_not_exists/"), ec);
		yggr_test_assert(!bexists);
		yggr_test_assert(!ec);
		std::cout << ec.message() << std::endl;
	}

	std::cout << "--------------test_is_exists succeed-------------" << std::endl;
}

void test_permissions(void)
{
	local_fsys_operators::path_type now_path = local_fsys_operators::make_path_object("../test_out/file_system/test_permissions");

	try
	{
		local_fsys_operators::remove_all(now_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(now_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		std::cout << std::oct << local_fsys_operators::permissions(now_path) << std::dec << std::endl;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec;
		std::cout << std::oct << local_fsys_operators::permissions(now_path, ec) << std::dec << std::endl;
		yggr_test_assert(!ec);
	}

	try
	{
		bool bright =
			local_fsys_operators::permissions(
				now_path, boost::filesystem::remove_perms | boost::filesystem::owner_all);
		yggr_test_assert(bright);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec;

		std::cout << std::oct << local_fsys_operators::permissions(now_path, ec) << std::dec << std::endl;
		yggr_test_assert(!ec);

		bool bright =
			local_fsys_operators::permissions(
				now_path, boost::filesystem::add_perms | boost::filesystem::all_all, ec);
		yggr_test_assert(bright);
		yggr_test_assert(!ec);

		std::cout << std::oct << local_fsys_operators::permissions(now_path, ec) << std::dec << std::endl;
		yggr_test_assert(!ec);
	}

	try
	{
		local_fsys_operators::remove_all(now_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_permissions succeed-------------" << std::endl;
}


// in windows the test need admin
void test_expand_path(void)
{
#if TEST_OF_ADMIN

	local_fsys_operators::path_type root_path = local_fsys_operators::make_path_object("../test_out/file_system/");
	local_fsys_operators::path_type dir_path = local_fsys_operators::make_path_object("../test_out/file_system/test_expand_path");
	local_fsys_operators::path_type dir_path_in_sym = local_fsys_operators::make_path_object("test_expand_path");
	local_fsys_operators::path_type symlink_path = local_fsys_operators::make_path_object("../test_out/file_system/symlink_test_expand_path");
	local_fsys_operators::path_type symlink_path_in_sym = local_fsys_operators::make_path_object("symlink_test_expand_path");
	local_fsys_operators::path_type symlink_path2 = local_fsys_operators::make_path_object("../test_out/file_system/symlink_test_expand_path2");

	try
	{
		local_fsys_operators::remove_all(dir_path);
		local_fsys_operators::remove_all(symlink_path);
		local_fsys_operators::remove_all(symlink_path2);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		//local_fsys_operators::create_directory_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(symlink_path_in_sym, symlink_path2);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bright = false;

		local_fsys_operators::path_type dst_path;

		bright = local_fsys_operators::symlink_equivalent(dir_path, local_fsys_operators::recursive_expand_path(dir_path));
		yggr_test_assert(bright);
		std::cout << local_fsys_operators::recursive_expand_path(dir_path) << std::endl;
		bright = local_fsys_operators::recursive_expand_path(dst_path, dir_path);
		yggr_test_assert(bright);

		bright = local_fsys_operators::symlink_equivalent(dir_path, local_fsys_operators::recursive_expand_path(symlink_path));
		yggr_test_assert(bright);
		std::cout << local_fsys_operators::recursive_expand_path(symlink_path) << std::endl;
		bright = local_fsys_operators::recursive_expand_path(dst_path, symlink_path);
		yggr_test_assert(bright);

		bright = local_fsys_operators::symlink_equivalent(dir_path, local_fsys_operators::recursive_expand_path(symlink_path2));
		yggr_test_assert(bright);
		std::cout << local_fsys_operators::recursive_expand_path(symlink_path2) << std::endl;
		bright = local_fsys_operators::recursive_expand_path(dst_path, symlink_path2);
		yggr_test_assert(bright);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec1, ec2;
		bool bright = false;
		local_fsys_operators::path_type dst_path;

		bright = local_fsys_operators::symlink_equivalent(dir_path, local_fsys_operators::recursive_expand_path(dir_path, ec1), ec2);
		yggr_test_assert(bright);
		yggr_test_assert(!ec1);
		yggr_test_assert(!ec2);
		std::cout << local_fsys_operators::recursive_expand_path(dir_path, ec1) << std::endl;
		yggr_test_assert(!ec1);
		bright = local_fsys_operators::recursive_expand_path(dst_path, dir_path, ec1);
		yggr_test_assert(bright);
		yggr_test_assert(!ec1);

		bright = local_fsys_operators::symlink_equivalent(dir_path, local_fsys_operators::recursive_expand_path(symlink_path, ec1), ec2);
		yggr_test_assert(bright);
		yggr_test_assert(!ec1);
		yggr_test_assert(!ec2);
		std::cout << local_fsys_operators::recursive_expand_path(symlink_path, ec1) << std::endl;
		yggr_test_assert(!ec1);
		bright = local_fsys_operators::recursive_expand_path(dst_path, symlink_path, ec1);
		yggr_test_assert(bright);
		yggr_test_assert(!ec1);

		bright = local_fsys_operators::symlink_equivalent(dir_path, local_fsys_operators::recursive_expand_path(symlink_path2, ec1), ec2);
		yggr_test_assert(bright);
		yggr_test_assert(!ec1);
		yggr_test_assert(!ec2);
		std::cout << local_fsys_operators::recursive_expand_path(symlink_path2, ec1) << std::endl;
		yggr_test_assert(!ec1);
		bright = local_fsys_operators::recursive_expand_path(dst_path, symlink_path2, ec1);
		yggr_test_assert(bright);
		yggr_test_assert(!ec1);
	}

	try
	{
		local_fsys_operators::remove_all(dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bright = false;
		local_fsys_operators::path_type dst_path;

		yggr_test_assert(local_fsys_operators::recursive_expand_path(dir_path).empty());
		std::cout << local_fsys_operators::recursive_expand_path(dir_path) << std::endl;
		bright = local_fsys_operators::recursive_expand_path(dst_path, dir_path);
		yggr_test_assert(!bright);

		yggr_test_assert(local_fsys_operators::recursive_expand_path(symlink_path).empty());
		std::cout << local_fsys_operators::recursive_expand_path(symlink_path) << std::endl;
		bright = local_fsys_operators::recursive_expand_path(dst_path, symlink_path);
		yggr_test_assert(!bright);

		yggr_test_assert(local_fsys_operators::recursive_expand_path(symlink_path2).empty());
		std::cout << local_fsys_operators::recursive_expand_path(symlink_path2) << std::endl;
		bright = local_fsys_operators::recursive_expand_path(dst_path, symlink_path2);
		yggr_test_assert(!bright);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(symlink_path);
		local_fsys_operators::remove_all(symlink_path2);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}
#endif // TEST_OF_ADMIN

	std::cout << "--------------test_expand_path succeed-------------" << std::endl;
}


void test_is_empty(void)
{
	local_fsys_operators::path_type zero_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_empty_zero");
	local_fsys_operators::path_type not_zero_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_empty_not_zero");
	local_fsys_operators::path_type zero_file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_empty_not_zero/zero_file.txt");
	local_fsys_operators::path_type not_zero_file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_empty_not_zero/not_zero_file.txt");
	local_fsys_operators::path_type not_exists_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists");

	try
	{
		local_fsys_operators::remove_all(zero_dir_path);
		local_fsys_operators::remove_all(not_zero_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(zero_dir_path);
		local_fsys_operators::create_directories(not_zero_dir_path);

		local_fsys_operators::create_file_of_binary(zero_file_path, 0);
		local_fsys_operators::create_file_of_binary(not_zero_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::is_empty(not_exists_path);
		yggr_test_assert(false);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::is_empty(zero_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_empty(not_zero_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_empty(zero_file_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_empty(not_zero_file_path);
		yggr_test_assert(!bchk);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec;
		bool bchk = false;

		bchk = local_fsys_operators::is_empty(zero_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_empty(not_zero_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_empty(zero_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_empty(not_zero_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
	}

	try
	{
		local_fsys_operators::remove_all(zero_dir_path);
		local_fsys_operators::remove_all(not_zero_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_is_empty succeed-------------" << std::endl;
}

void test_is_other(void)
{
	local_fsys_operators::path_type dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_other_dir");
	local_fsys_operators::path_type file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_other_file.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type dir_path_in_sym =
		local_fsys_operators::make_path_object("test_is_other_dir");
	local_fsys_operators::path_type symlink_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_other_symlink");
#endif //TEST_OF_ADMIN

	local_fsys_operators::path_type not_exists_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_other_not_exists");

	local_fsys_operators::path_type failed_path(".///failed");

	try
	{
		local_fsys_operators::remove_all(dir_path);
		local_fsys_operators::remove_all(file_path);
#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_file_of_binary(file_path, 1024);
#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::file_status_type sat;
		bool bchk = false;

		sat = local_fsys_operators::path_symlink_status(dir_path);
		bchk = local_fsys_operators::is_other(sat);
		yggr_test_assert(!bchk);

		sat = local_fsys_operators::path_symlink_status(file_path);
		bchk = local_fsys_operators::is_other(sat);
		yggr_test_assert(!bchk);

#if TEST_OF_ADMIN
		sat = local_fsys_operators::path_symlink_status(symlink_path);
		bchk = local_fsys_operators::is_other(sat);
		yggr_test_assert(!bchk);
#endif // TEST_OF_ADMIN

		sat = local_fsys_operators::path_symlink_status(not_exists_path);
		bchk = local_fsys_operators::is_other(sat);
		yggr_test_assert(!bchk);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::is_other(dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_other(file_path);
		yggr_test_assert(!bchk);

#if TEST_OF_ADMIN
		local_fsys_operators::file_status_type sat;
		sat = local_fsys_operators::path_symlink_status(symlink_path);
		bchk = local_fsys_operators::is_other(sat);
		yggr_test_assert(!bchk);

		try
		{
			bchk = local_fsys_operators::is_other(symlink_path);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cerr << e.what() << std::endl;
		}

#endif // TEST_OF_ADMIN

		bchk = local_fsys_operators::is_other(not_exists_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_other(failed_path);
		yggr_test_assert(!bchk);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::is_other(dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_other(file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN
		bchk = local_fsys_operators::is_other(symlink_path, ec);
		yggr_test_assert(!bchk); // same result but different process
#	if defined(YGGR_AT_WINDOWS)
		std::cout << ec.message() << std::endl;
		yggr_test_assert(!!ec);
#   elif defined(YGGR_AT_LINUX)
		std::cout << ec.message() << std::endl;
		yggr_test_assert(!ec);
#   elif defined(YGGR_AT_DARWIN)
        std::cout << ec.message() << std::endl;
        yggr_test_assert(!ec);
#	else
		yggr_test_assert(false); // don't forgot test otherOS, the assert is a mark
		yggr_test_assert(!ec);
#	endif // YGGR_AT_WINDOWS
#endif // TEST_OF_ADMIN

		bchk = local_fsys_operators::is_other(not_exists_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;

		bchk = local_fsys_operators::is_other(failed_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
	}

	try
	{
		local_fsys_operators::remove_all(dir_path);
		local_fsys_operators::remove_all(file_path);
#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_is_other succeed-------------" << std::endl;
}

void test_last_write_time(void)
{
	local_fsys_operators::path_type dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_last_write_time_dir");
	local_fsys_operators::path_type file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_last_write_time_file.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type dir_path_in_sym =
		local_fsys_operators::make_path_object("test_last_write_time_dir");
	local_fsys_operators::path_type symlink_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_last_write_time_symlink");
#endif //YGGR_AT_WINDOWS

	try
	{
		local_fsys_operators::remove_all(dir_path);
		local_fsys_operators::remove_all(file_path);
#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(symlink_path);
#endif // YGGR_AT_WINDOWS
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_file_of_binary(file_path, 1024);
#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		yggr::time::time tm;

		tm = local_fsys_operators::last_write_time(dir_path);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

		tm = local_fsys_operators::last_write_time(file_path);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

#if TEST_OF_ADMIN
		tm = local_fsys_operators::last_write_time(symlink_path);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
#endif // TEST_OF_ADMIN

		yggr::time::time old_tm = tm;
		tm -= 28800; // - 8 hour
		yggr::time::time ret_tm;

		ret_tm = local_fsys_operators::last_write_time(dir_path, tm.ex_to_second<std::time_t>());
		yggr_test_assert(ret_tm == old_tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

		ret_tm = local_fsys_operators::last_write_time(file_path, tm.ex_to_second<std::time_t>());
		yggr_test_assert(ret_tm == old_tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

#if TEST_OF_ADMIN
		ret_tm = local_fsys_operators::last_write_time(symlink_path, tm.ex_to_second<std::time_t>());
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
#endif // TEST_OF_ADMIN

		ret_tm = local_fsys_operators::last_write_time(dir_path);
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

		ret_tm = local_fsys_operators::last_write_time(file_path);
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

#if TEST_OF_ADMIN
		ret_tm = local_fsys_operators::last_write_time(symlink_path);
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
#endif // TEST_OF_ADMIN

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}


	{
		yggr::time::time tm;
		local_fsys_operators::error_code_type ec;

		tm = local_fsys_operators::last_write_time(dir_path, ec);
		yggr_test_assert(!ec);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

		tm = local_fsys_operators::last_write_time(file_path, ec);
		yggr_test_assert(!ec);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

#if TEST_OF_ADMIN
		tm = local_fsys_operators::last_write_time(symlink_path, ec);
		yggr_test_assert(!ec);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
#endif // TEST_OF_ADMIN

		yggr::time::time old_tm = tm;
		tm -= 28800; // - 8 hour
		yggr::time::time ret_tm;

		ret_tm = local_fsys_operators::last_write_time(dir_path, tm.ex_to_second<std::time_t>(), ec);
		yggr_test_assert(!ec);
		yggr_test_assert(ret_tm == old_tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

		ret_tm = local_fsys_operators::last_write_time(file_path, tm.ex_to_second<std::time_t>(), ec);
		yggr_test_assert(!ec);
		yggr_test_assert(ret_tm == old_tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

#if TEST_OF_ADMIN
		ret_tm = local_fsys_operators::last_write_time(symlink_path, tm.ex_to_second<std::time_t>(), ec);
		yggr_test_assert(!ec);
		yggr_test_assert(ret_tm == tm);
		std::cout << tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
#endif // TEST_OF_ADMIN

		ret_tm = local_fsys_operators::last_write_time(dir_path, ec);
		yggr_test_assert(!ec);
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

		ret_tm = local_fsys_operators::last_write_time(file_path, ec);
		yggr_test_assert(!ec);
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;

#if TEST_OF_ADMIN
		ret_tm = local_fsys_operators::last_write_time(symlink_path, ec);
		yggr_test_assert(!ec);
		yggr_test_assert(ret_tm == tm);
		std::cout << ret_tm.to_local_string("%Y-%m-%d %H:%M:%S") << std::endl;
#endif // TEST_OF_ADMIN

	}

	try
	{
		local_fsys_operators::remove_all(dir_path);
		local_fsys_operators::remove_all(file_path);
#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_last_write_time succeed-------------" << std::endl;
}


template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const local_fsys_operators::space_info_type& info)
{
	os << "capacity: " << info.capacity
		<< "\tfree: " << info.free
		<< "\tavailable: " << info.available;
	return os;
}

void test_space(void)
{
	local_fsys_operators::path_type dir_path = local_fsys_operators::make_path_object("./");

	try
	{
		local_fsys_operators::space_info_type info =
            {static_cast<boost::uintmax_t>(-1), static_cast<boost::uintmax_t>(-1), static_cast<boost::uintmax_t>(-1)};

		info = local_fsys_operators::space(dir_path);
		std::cout << info << std::endl;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{

		local_fsys_operators::space_info_type info =
             {static_cast<boost::uintmax_t>(-1), static_cast<boost::uintmax_t>(-1), static_cast<boost::uintmax_t>(-1)};
		local_fsys_operators::error_code_type ec;

		info = local_fsys_operators::space(dir_path, ec);
		std::cout << info << std::endl;
	}

	std::cout << "--------------test_space succeed-------------" << std::endl;
}

void test_check_is_included_in_directory()
{
	local_fsys_operators::path_type parent_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists");

	local_fsys_operators::path_type parent_dir_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/");

	local_fsys_operators::path_type child_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/child/");
	local_fsys_operators::path_type not_child_dir_path =
		local_fsys_operators::make_path_object("../test_out/not_child_dir_path/not_child");

	local_fsys_operators::path_type child_file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/child.txt");
	local_fsys_operators::path_type not_child_file_path =
		local_fsys_operators::make_path_object("../test_out/not_child_file_path/not_exists/not_child.txt");

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, parent_dir_path2);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, child_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(child_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, not_child_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, child_file_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(child_file_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, not_child_file_path);
		yggr_test_assert(!bchk);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, child_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_included_in_directory(child_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, not_child_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, child_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_included_in_directory(child_file_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_included_in_directory(parent_dir_path, not_child_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
	}

	std::cout << "--------------test_check_is_included_in_directory succeed-------------" << std::endl;
}

void test_is_included_in_directory(void)
{
	local_fsys_operators::path_type parent_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/tiiid");

	local_fsys_operators::path_type child_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/tiiid/child/");
	local_fsys_operators::path_type not_child_dir_path =
		local_fsys_operators::make_path_object("../test_out/not_child_dir_path/not_child");

	local_fsys_operators::path_type child_file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/tiiid/child/child.txt");
	local_fsys_operators::path_type not_child_file_path =
		local_fsys_operators::make_path_object("../test_out/not_child_file_path/not_child.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type parent_dir_path_in_child_sym =
		local_fsys_operators::make_path_object("../tiiid");
	local_fsys_operators::path_type parent_dir_path_in_not_child_sym =
		local_fsys_operators::make_path_object("../file_system/tiiid");

	local_fsys_operators::path_type child_symlink_path =
		local_fsys_operators::make_path_object("../test_out/file_system/tiiid/child_symlink");
	local_fsys_operators::path_type not_child_symlink_path =
		local_fsys_operators::make_path_object("../test_out/not_child_dir_path/not_child_symlink");
#endif //TEST_OF_ADMIN

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
		local_fsys_operators::remove_all(not_child_dir_path);
		local_fsys_operators::remove_all(not_child_file_path);

#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(not_child_symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(parent_dir_path);

		local_fsys_operators::create_directories(child_dir_path);
		local_fsys_operators::create_directories(not_child_dir_path);

		local_fsys_operators::create_file_of_binary(child_file_path, 1024);
		local_fsys_operators::create_file_of_binary(not_child_file_path, 1024);

#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(parent_dir_path_in_child_sym, child_symlink_path);
		local_fsys_operators::create_symlink(parent_dir_path_in_not_child_sym, not_child_symlink_path);
		//local_fsys_operators::create_directory_symlink(parent_dir_path_in_child_sym, child_symlink_path);
		//local_fsys_operators::create_directory_symlink(parent_dir_path_in_not_child_sym, not_child_symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, child_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_included_in_directory(child_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, not_child_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, child_file_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_included_in_directory(child_file_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, not_child_file_path);
		yggr_test_assert(!bchk);

#if TEST_OF_ADMIN
		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, child_symlink_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_included_in_directory(child_symlink_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, not_child_symlink_path);
		yggr_test_assert(!bchk);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, child_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(child_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, not_child_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, child_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(child_file_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, not_child_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN
		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, child_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(child_symlink_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_included_in_directory(parent_dir_path, not_child_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
#endif // TEST_OF_ADMIN
	}

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
		local_fsys_operators::remove_all(not_child_dir_path);
		local_fsys_operators::remove_all(not_child_file_path);

#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(not_child_symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_is_included_in_directory succeed-------------" << std::endl;
}

void test_check_is_child(void)
{
	local_fsys_operators::path_type parent_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists");

	local_fsys_operators::path_type child_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/child/");
	local_fsys_operators::path_type child_dir_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/child/child2");
	local_fsys_operators::path_type not_child_dir_path =
		local_fsys_operators::make_path_object("../test_out/not_child_dir_path/not_child");

	local_fsys_operators::path_type child_file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/child.txt");
	local_fsys_operators::path_type child_file_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/not_exists/child/child.txt");
	local_fsys_operators::path_type not_child_file_path =
		local_fsys_operators::make_path_object("../test_out/not_child_file_path/not_child.txt");

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::check_is_child(parent_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::check_is_child(child_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_dir_path2);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, not_child_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_file_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::check_is_child(child_file_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_file_path2);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, not_child_dir_path);
		yggr_test_assert(!bchk);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::check_is_child(parent_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(child_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_dir_path2, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, not_child_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(child_file_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, child_file_path2, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::check_is_child(parent_dir_path, not_child_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
	}

	std::cout << "--------------test_check_is_child succeed-------------" << std::endl;
}

void test_is_child(void)
{
	local_fsys_operators::path_type parent_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child");

	local_fsys_operators::path_type child_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child/child/");
	local_fsys_operators::path_type child_dir_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child/child/child2");
	local_fsys_operators::path_type not_child_dir_path =
		local_fsys_operators::make_path_object("../test_out/not_child_dir_path/not_child");

	local_fsys_operators::path_type child_file_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child/child.txt");
	local_fsys_operators::path_type child_file_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child/child/child.txt");
	local_fsys_operators::path_type not_child_file_path =
		local_fsys_operators::make_path_object("../test_out/not_child_file_path/not_child.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type parent_dir_path_in_child_sym =
		local_fsys_operators::make_path_object("../test_is_child");
	local_fsys_operators::path_type child_symlink_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child/child_symlink");

	local_fsys_operators::path_type parent_dir_path_in_child_sym2 =
		local_fsys_operators::make_path_object("../../test_is_child");
	local_fsys_operators::path_type child_symlink_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/test_is_child/child/child_symlink");

	local_fsys_operators::path_type parent_dir_path_in_not_child_sym =
		local_fsys_operators::make_path_object("../../test_is_child");
	local_fsys_operators::path_type not_child_symlink_path =
		local_fsys_operators::make_path_object("../test_out/not_child_dir_path/not_child_symlink");
#endif // TEST_OF_ADMIN


	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
		local_fsys_operators::remove_all(not_child_dir_path);
		local_fsys_operators::remove_all(not_child_file_path);

#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(not_child_symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(parent_dir_path);

		local_fsys_operators::create_directories(child_dir_path);
		local_fsys_operators::create_directories(child_dir_path2);
		local_fsys_operators::create_directories(not_child_dir_path);

		local_fsys_operators::create_file_of_binary(child_file_path, 1024);
		local_fsys_operators::create_file_of_binary(child_file_path2, 1024);
		local_fsys_operators::create_file_of_binary(not_child_file_path, 1024);

#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(parent_dir_path_in_child_sym, child_symlink_path);
		local_fsys_operators::create_symlink(parent_dir_path_in_child_sym2, child_symlink_path2);
		local_fsys_operators::create_symlink(parent_dir_path_in_not_child_sym, not_child_symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::is_child(parent_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_child(child_dir_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_dir_path2);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, not_child_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_file_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_child(child_file_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_file_path2);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, not_child_file_path);
		yggr_test_assert(!bchk);

#if TEST_OF_ADMIN
		bchk = local_fsys_operators::is_child(parent_dir_path, child_symlink_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_child(child_symlink_path, parent_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_symlink_path2);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::is_child(parent_dir_path, not_child_symlink_path);
		yggr_test_assert(!bchk);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}


	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::is_child(parent_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(child_dir_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_dir_path2, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, not_child_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_file_path, ec);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_child(child_file_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_file_path2, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, not_child_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN
		bchk = local_fsys_operators::is_child(parent_dir_path, child_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(child_symlink_path, parent_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, child_symlink_path2, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_child(parent_dir_path, not_child_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
#endif // TEST_OF_ADMIN
	}

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
		local_fsys_operators::remove_all(not_child_dir_path);
		local_fsys_operators::remove_all(not_child_file_path);

#if TEST_OF_ADMIN
		local_fsys_operators::remove_all(not_child_symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_is_child succeed-------------" << std::endl;
}

// test count_children_directory and recursive_count_children_directory
void test_count_children_directories(void)
{
	local_fsys_operators::path_type parent_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd");

	local_fsys_operators::path_type child_dir_path1 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/");
	local_fsys_operators::path_type child_dir_path11 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/dir1");
	local_fsys_operators::path_type child_dir_path12 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/dir2");

	local_fsys_operators::path_type child_dir_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/");
	local_fsys_operators::path_type child_dir_path21 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/dir1");
	local_fsys_operators::path_type child_dir_path22=
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/dir2");

	local_fsys_operators::path_type child_file_path1 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/child1.txt");
	local_fsys_operators::path_type child_file_path11 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/child1.txt");
	local_fsys_operators::path_type child_file_path12 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/child2.txt");

	local_fsys_operators::path_type child_file_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/child2.txt");
	local_fsys_operators::path_type child_file_path21 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/child1.txt");
	local_fsys_operators::path_type child_file_path22 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/child2.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type parent_dir_path_in_sub_sym =
		local_fsys_operators::make_path_object("../tccd");

	local_fsys_operators::path_type parent_dir_path_in_sub_sub_sym =
		local_fsys_operators::make_path_object("../../tccd");

	local_fsys_operators::path_type child_symlink_path1 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/symlink1");
	local_fsys_operators::path_type child_symlink_path11 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/symlink1");
	local_fsys_operators::path_type child_symlink_path12 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir1/symlink2");

	local_fsys_operators::path_type child_symlink_path2 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/symlink2");
	local_fsys_operators::path_type child_symlink_path21 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/symlink1");
	local_fsys_operators::path_type child_symlink_path22 =
		local_fsys_operators::make_path_object("../test_out/file_system/tccd/dir2/symlink2");
#endif // TEST_OF_ADMIN

	// 6 sub_dir,		2 child_dir
	// 6 sub_file,		2 child_file
	// 6 sub_symlink,	2 child_symlink

	const local_fsys_operators::size_type
			chk_sub_dir_size = 6,
			chk_sub_file_size = 6,
			chk_sub_symlink_size = 6,
			chk_child_dir_size = 2,
			chk_child_file_size = 2,
			chk_child_symlink_size = 2;

    assert(chk_sub_dir_size == 6);
    assert(chk_sub_file_size == 6);
    assert(chk_sub_symlink_size == 6);
    assert(chk_child_dir_size == 2);
    assert(chk_child_file_size == 2);
    assert(chk_child_symlink_size == 2);

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(parent_dir_path);

		local_fsys_operators::create_directories(child_dir_path1);
		local_fsys_operators::create_directories(child_dir_path11);
		local_fsys_operators::create_directories(child_dir_path12);
		local_fsys_operators::create_directories(child_dir_path2);
		local_fsys_operators::create_directories(child_dir_path21);
		local_fsys_operators::create_directories(child_dir_path22);

		local_fsys_operators::create_file_of_binary(child_file_path1, 1024);
		local_fsys_operators::create_file_of_binary(child_file_path11, 1024);
		local_fsys_operators::create_file_of_binary(child_file_path12, 1024);
		local_fsys_operators::create_file_of_binary(child_file_path2, 1024);
		local_fsys_operators::create_file_of_binary(child_file_path21, 1024);
		local_fsys_operators::create_file_of_binary(child_file_path22, 1024);


#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(parent_dir_path_in_sub_sym, child_symlink_path1);
		local_fsys_operators::create_symlink(parent_dir_path_in_sub_sub_sym, child_symlink_path11);
		local_fsys_operators::create_symlink(parent_dir_path_in_sub_sub_sym, child_symlink_path12);

		local_fsys_operators::create_symlink(parent_dir_path_in_sub_sym, child_symlink_path2);
		local_fsys_operators::create_symlink(parent_dir_path_in_sub_sub_sym, child_symlink_path21);
		local_fsys_operators::create_symlink(parent_dir_path_in_sub_sub_sym, child_symlink_path22);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type dir_size = 0;

		dir_size = local_fsys_operators::count_children_directory(parent_dir_path);
		yggr_test_assert(dir_size == chk_child_dir_size);

		dir_size = local_fsys_operators::recursive_count_children_directory(parent_dir_path);
		yggr_test_assert(dir_size == chk_sub_dir_size);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path1);
		yggr_test_assert(dir_size == 2);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path1);
		yggr_test_assert(dir_size == 2);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path2);
		yggr_test_assert(dir_size == 2);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path2);
		yggr_test_assert(dir_size == 2);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path11);
		yggr_test_assert(dir_size == 0);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path11);
		yggr_test_assert(dir_size == 0);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path21);
		yggr_test_assert(dir_size == 0);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path21);
		yggr_test_assert(dir_size == 0);

		dir_size = local_fsys_operators::count_children_directory(child_file_path1);
		yggr_test_assert(dir_size == 0);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_file_path1);
		yggr_test_assert(dir_size == 0);

#if TEST_OF_ADMIN
		dir_size = local_fsys_operators::count_children_directory(child_symlink_path1);
		yggr_test_assert(dir_size == 0);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_symlink_path1);
		yggr_test_assert(dir_size == 0);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type dir_size = 0;
		local_fsys_operators::error_code_type ec;

		dir_size = local_fsys_operators::count_children_directory(parent_dir_path, ec);
		yggr_test_assert(dir_size == chk_child_dir_size);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(parent_dir_path, ec);
		yggr_test_assert(dir_size == chk_sub_dir_size);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path1, ec);
		yggr_test_assert(dir_size == 2);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path1, ec);
		yggr_test_assert(dir_size == 2);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path2, ec);
		yggr_test_assert(dir_size == 2);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path2, ec);
		yggr_test_assert(dir_size == 2);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path11, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path11, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::count_children_directory(child_dir_path21, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_dir_path21, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::count_children_directory(child_file_path1, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_file_path1, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN
		dir_size = local_fsys_operators::count_children_directory(child_symlink_path1, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);

		dir_size = local_fsys_operators::recursive_count_children_directory(child_symlink_path1, ec);
		yggr_test_assert(dir_size == 0);
		yggr_test_assert(!ec);
#endif // TEST_OF_ADMIN
	}

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_count_children_directory succeed-------------" << std::endl;
}

void test_create_directories(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_create_directories");

	local_fsys_operators::path_type dir_path1 = (root_dir_path / "dir1");
	local_fsys_operators::path_type dir_path123 = (root_dir_path / "dir1/dir2/dir3");

	local_fsys_operators::path_type exists_dir = (root_dir_path / "exists_dir");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(exists_dir);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_directories(dir_path1);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directories(dir_path1);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directories(dir_path1, ec, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_directories(
				dir_path1, local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(
				dir_path1, local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(4 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		custom_unique_type my_handler(dir_path1);
		bchk = local_fsys_operators::create_directories(dir_path1, my_handler);
		yggr_test_assert(bchk);
		yggr_test_assert(5 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(dir_path1, my_handler);
		yggr_test_assert(bchk);
		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(dir_path123);
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(exists_dir, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(exists_dir); // default has dir_success_if_exists
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(
				exists_dir, local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(
				exists_dir, local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(exists_dir);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_directories(dir_path1, ec);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directories(dir_path1, ec);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directories(dir_path1, ec, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_directories(
				dir_path1, ec,
				local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(
				dir_path1, ec,
				local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(4 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		custom_unique_type my_handler(dir_path1);
		bchk = local_fsys_operators::create_directories(dir_path1, my_handler, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(5 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(dir_path1, my_handler, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(dir_path123, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(exists_dir, ec, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directories(exists_dir, ec); // default has dir_success_if_exists
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directories(
				exists_dir, ec, local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directories(
				exists_dir, ec, local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_create_directories succeed-------------" << std::endl;
}

void test_create_parent_directories(void)
{
	local_fsys_operators::path_type parent_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/parent");

	local_fsys_operators::path_type child_dir_path1 = parent_dir_path / "child";
	local_fsys_operators::path_type child_dir_path2 = parent_dir_path / "child/";

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		bool bchk = false;
		bchk = local_fsys_operators::create_parent_directories(child_dir_path2);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_directory(parent_dir_path);
		yggr_test_assert(bchk);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_parent_directories(child_dir_path1, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::is_directory(parent_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);
	}

	try
	{
		local_fsys_operators::remove_all(parent_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_create_parent_directories succeed-------------" << std::endl;

}

void test_create_directory(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_create_directory");

	local_fsys_operators::path_type dir_path1 = (root_dir_path / "dir1");
	local_fsys_operators::path_type dir_path2 = (root_dir_path / "dir2");
	local_fsys_operators::path_type dir_path3 = (root_dir_path / "dir3");
	local_fsys_operators::path_type dir_path123 = (root_dir_path / "dir1/dir2/dir3");

	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(exists_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_directory(dir_path1);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directory(dir_path1);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directory(dir_path1, ec, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_directory(
				dir_path1, local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(
				dir_path1, local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(4 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		custom_unique_type my_handler(dir_path1);
		bchk = local_fsys_operators::create_directory(dir_path1, my_handler);
		yggr_test_assert(bchk);
		yggr_test_assert(5 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(dir_path1, my_handler);
		yggr_test_assert(bchk);
		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(dir_path123, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(dir_path2);
		bchk = local_fsys_operators::create_directory(dir_path3);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(exists_dir_path, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(exists_dir_path); // default has dir_success_if_exists
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(
				exists_dir_path, local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(
				exists_dir_path, local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(exists_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_directory(dir_path1, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(dir_path1, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(dir_path1, ec, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(
				dir_path1, ec, local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(
				dir_path1, ec, local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(4 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		custom_unique_type my_handler(dir_path1);
		bchk = local_fsys_operators::create_directory(dir_path1, my_handler, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(5 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(dir_path1, my_handler, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(dir_path123, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(dir_path2);
		bchk = local_fsys_operators::create_directory(dir_path3);

		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directory(exists_dir_path, ec, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(exists_dir_path, ec); // default has dir_success_if_exists
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(
				exists_dir_path, ec, local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directory(
				exists_dir_path, ec, local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(8 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		yggr_test_assert(!ec);
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_create_directory succeed-------------" << std::endl;
}

void test_rename_directory(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_rename_directory");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type renamed_dir_path = (root_dir_path / "renamed_dir");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::create_directories(dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directories(exists_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::rename_directory(dir_path, renamed_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_directory(renamed_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_directory(dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::rename_directory(dir_path, renamed_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::rename_directory(renamed_dir_path, renamed_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::rename_directory(
				renamed_dir_path, renamed_dir_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::rename_directory(
				renamed_dir_path, renamed_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::rename_directory(
				renamed_dir_path, renamed_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, exists_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_directory(renamed_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(root_dir_path));

		bchk = local_fsys_operators::create_directories(dir_path);
		yggr_test_assert(bchk);

		custom_unique_type my_handler(exists_dir_path);
		bchk = local_fsys_operators::rename_directory(
					dir_path, exists_dir_path,
					my_handler,
					local_fsys_operators::options_type::dir_rename_if_exists);

		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_directory(dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::create_directories(dir_path, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::create_directories(exists_dir_path, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(dir_path, renamed_dir_path, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::is_directory(renamed_dir_path, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;
			bchk = local_fsys_operators::is_directory(dir_path, ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(dir_path, renamed_dir_path, ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(renamed_dir_path, renamed_dir_path, ec, 0);
			yggr_test_assert(!bchk);
			std::cerr << ec.what() << std::endl;
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, renamed_dir_path, ec,
					local_fsys_operators::options_type::dir_success_if_exists);
			yggr_test_assert(bchk);
			std::cerr << ec.what() << std::endl;
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, renamed_dir_path, ec,
					local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, renamed_dir_path, ec,
					local_fsys_operators::options_type::dir_replace_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
					renamed_dir_path, renamed_dir_path, ec, local_fsys_operators::options_type::dir_success_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
						renamed_dir_path, exists_dir_path, ec, 0);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
						renamed_dir_path, exists_dir_path, ec,
						local_fsys_operators::options_type::dir_success_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::rename_directory(
						renamed_dir_path, exists_dir_path, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::is_directory(renamed_dir_path, ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!!ec);
			yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::create_directories(dir_path);
			yggr_test_assert(bchk);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			custom_unique_type my_handler(exists_dir_path);
			bchk = local_fsys_operators::rename_directory(
						dir_path, exists_dir_path,
						my_handler, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);

			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;
			
			bchk = local_fsys_operators::is_directory(dir_path, ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!!ec);
			yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(root_dir_path));
		}
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_rename_directory succeed-------------" << std::endl;
}

void test_move_directory(void)
{
	local_fsys_operators::path_type same_vol_root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_move_directory");

#if defined(YGGR_AT_WINDOWS)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("f:/yggr_diff_vol_test_out/test_move_directory");
#elif defined(YGGR_AT_LINUX)
#	if (defined(__ANDROID__) || defined(ANDROID))
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/storage/C5B8-A1D4/yggr_diff_vol_test_out/diff_test_move_directory/");
#	else
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/run/media/alucard/win10_games/yggr_diff_vol_test_out/diff_test_move_directory/");
#	endif // #	if defined(__ANDROID__) || defined(ANDROID)
#elif defined(YGGR_AT_DARWIN)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/Volumes/win10_games/yggr_diff_vol_test_out/");
#else
#	error "!!!append this platfrom diff vol path!!!"
#endif // YGGR_AT_WINDOWS

	local_fsys_operators::path_type same_vol_dir_path = (same_vol_root_dir_path / "dir/");
	local_fsys_operators::path_type same_vol_file_path = (same_vol_root_dir_path / "dir/file.txt");
	local_fsys_operators::path_type same_vol_moved_dir_path = (same_vol_root_dir_path / "moved_dir");
	local_fsys_operators::path_type same_vol_exists_dir_path = (same_vol_root_dir_path / "exists_dir");
	local_fsys_operators::path_type same_vol_exists_file_path = (same_vol_root_dir_path / "exists_file.txt");

	local_fsys_operators::path_type diff_vol_dir_path = (diff_vol_root_dir_path / "dir");
	local_fsys_operators::path_type diff_vol_moved_dir_path = (diff_vol_root_dir_path / "moved_dir/");
	local_fsys_operators::path_type diff_vol_exists_dir_path = (diff_vol_root_dir_path / "exists_dir");
	local_fsys_operators::path_type diff_vol_exists_file_path = (diff_vol_root_dir_path / "exists_file.txt");

	local_fsys_operators::path_type diff_vol_not_exists_dir_path = (diff_vol_root_dir_path / "not_exists/vol_moved_dir");



	try
	{
		local_fsys_operators::remove_all(same_vol_root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(same_vol_root_dir_path);
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(same_vol_exists_dir_path);
		local_fsys_operators::create_directories(diff_vol_exists_dir_path);

		local_fsys_operators::create_file_of_binary(same_vol_exists_file_path, 1024);
		local_fsys_operators::create_file_of_binary(diff_vol_exists_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	// empty directory
	try
	{
		bool bchk = false;

		// same vol
		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, same_vol_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, same_vol_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, same_vol_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, same_vol_moved_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_moved_dir_path, same_vol_exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_moved_dir_path, same_vol_exists_dir_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_moved_dir_path, same_vol_exists_file_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(!bchk);

		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		bchk = local_fsys_operators::move_directory(
					same_vol_moved_dir_path, same_vol_exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_moved_dir_path));
		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_handler(same_vol_exists_file_path);
			bchk = local_fsys_operators::move_directory(
						same_vol_dir_path, same_vol_exists_file_path,
						my_handler,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
		}

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		//diff vol

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_dir_path, 0);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_not_exists_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_not_exists_dir_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk); // dir count + 2

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_exists_dir_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_exists_file_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(!bchk);

		yggr_test_assert(4 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));

		bchk = local_fsys_operators::move_directory(
					same_vol_dir_path, diff_vol_exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(5 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_handler(diff_vol_exists_file_path);
			bchk = local_fsys_operators::move_directory(
						same_vol_dir_path, diff_vol_exists_file_path,
						my_handler,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
		}

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(same_vol_root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(same_vol_root_dir_path);
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(same_vol_exists_dir_path);
		local_fsys_operators::create_directories(diff_vol_exists_dir_path);

		local_fsys_operators::create_file_of_binary(same_vol_exists_file_path, 1024);
		local_fsys_operators::create_file_of_binary(diff_vol_exists_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	// not empty directory
	try
	{
		bool bchk = false;

		// same vol
		bchk = local_fsys_operators::create_file_of_binary(
				same_vol_file_path, 1024,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, same_vol_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, same_vol_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, same_vol_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, same_vol_moved_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_moved_dir_path, same_vol_exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_moved_dir_path, same_vol_exists_dir_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_moved_dir_path, same_vol_exists_file_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(!bchk);

		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		bchk = local_fsys_operators::move_directory(
					same_vol_moved_dir_path, same_vol_exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_moved_dir_path));
		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		bchk = local_fsys_operators::create_file_of_binary(
				same_vol_file_path, 1024,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_handler(same_vol_exists_file_path);
			bchk = local_fsys_operators::move_directory(
						same_vol_dir_path, same_vol_exists_file_path,
						my_handler,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
		}

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		//diff vol

		bchk = local_fsys_operators::create_file_of_binary(
				same_vol_file_path, 1024,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		//same_vol_dir_path is not an empty directory,
		//	so can not support move_directory,
		//	if you want to move a non-empty directory,
		//	please use recursive_move function
		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_not_exists_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_not_exists_dir_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_exists_dir_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		yggr_test_assert(1 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));

		bchk = local_fsys_operators::move_directory(
					same_vol_dir_path, diff_vol_exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(!bchk);

		yggr_test_assert(local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(1 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));

		{
			custom_unique_type my_handler(diff_vol_exists_file_path);
			bchk = local_fsys_operators::move_directory(
						same_vol_dir_path, diff_vol_exists_file_path,
						my_handler,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(!bchk);
		}

		yggr_test_assert(local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(1 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(same_vol_root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(same_vol_root_dir_path);
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(same_vol_exists_dir_path);
		local_fsys_operators::create_directories(diff_vol_exists_dir_path);

		local_fsys_operators::create_file_of_binary(same_vol_exists_file_path, 1024);
		local_fsys_operators::create_file_of_binary(diff_vol_exists_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		// same vol
		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, same_vol_dir_path, ec, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, same_vol_dir_path, ec,
				local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, same_vol_dir_path, ec,
				local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, same_vol_moved_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(same_vol_moved_dir_path, same_vol_exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(
				same_vol_moved_dir_path, same_vol_exists_dir_path, ec,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(
				same_vol_moved_dir_path, same_vol_exists_file_path, ec,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		bchk = local_fsys_operators::move_directory(
					same_vol_moved_dir_path, same_vol_exists_dir_path, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_moved_dir_path));
		yggr_test_assert(2 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_handler(same_vol_exists_file_path);
			bchk = local_fsys_operators::move_directory(
						same_vol_dir_path, same_vol_exists_file_path,
						my_handler, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(3 == local_fsys_operators::recursive_count_children_directory(same_vol_root_dir_path));

		//diff vol

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_dir_path, ec, 0);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_not_exists_dir_path, ec, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_not_exists_dir_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk); // dir count + 2
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::move_directory(same_vol_dir_path, diff_vol_exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_exists_dir_path, ec,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_directory(
				same_vol_dir_path, diff_vol_exists_file_path, ec,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(4 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));

		bchk = local_fsys_operators::move_directory(
					same_vol_dir_path, diff_vol_exists_dir_path, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(5 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));

		bchk = local_fsys_operators::create_directories(same_vol_dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_handler(diff_vol_exists_file_path);
			bchk = local_fsys_operators::move_directory(
						same_vol_dir_path, diff_vol_exists_file_path,
						my_handler, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
		}

		yggr_test_assert(!local_fsys_operators::is_directory(same_vol_dir_path));
		yggr_test_assert(6 == local_fsys_operators::recursive_count_children_directory(diff_vol_root_dir_path));
	}

	try
	{
		local_fsys_operators::remove_all(same_vol_root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_move_directory succeed-------------" << std::endl;
}

void test_copy_directory(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_copy_directory");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type file_path = (root_dir_path / "dir/file.txt");
	local_fsys_operators::path_type copyed_dir_path = (root_dir_path / "copyed_dir");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type not_exists_dir_path = (root_dir_path / "not_exists_dir/copyed_dir");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_directories(dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_directories(exists_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_directory(dir_path, dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_directory(dir_path, copyed_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::is_directory(copyed_dir_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, dir_path,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, exists_dir_path, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_directory(
					dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);

		custom_unique_type my_handler(exists_dir_path);
		bchk = local_fsys_operators::copy_directory(
					dir_path, exists_dir_path,
					my_handler,
					local_fsys_operators::options_type::dir_rename_if_exists);

		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_directory(dir_path, not_exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);

		bchk = local_fsys_operators::copy_directory(
				dir_path, not_exists_dir_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::create_directories(dir_path);
			yggr_test_assert(bchk);
		}

		{

			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::create_directories(exists_dir_path);
			yggr_test_assert(bchk);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(dir_path, dir_path, ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
						dir_path, dir_path, ec, 0);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
						dir_path, dir_path, ec,
						local_fsys_operators::options_type::dir_success_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
						dir_path, dir_path, ec,
						local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
						dir_path, dir_path, ec,
						local_fsys_operators::options_type::dir_replace_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(dir_path, copyed_dir_path, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;
			
			bchk = local_fsys_operators::is_directory(copyed_dir_path);
			yggr_test_assert(bchk);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(dir_path, exists_dir_path, ec, 0);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
						dir_path, exists_dir_path,
						local_fsys_operators::options_type::dir_success_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
						dir_path, exists_dir_path, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			custom_unique_type my_handler(exists_dir_path);
			bchk = local_fsys_operators::copy_directory(
						dir_path, exists_dir_path,
						my_handler, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);

			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(dir_path, not_exists_dir_path, ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!!ec);
		}

		{
			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			bchk = local_fsys_operators::copy_directory(
					dir_path, not_exists_dir_path, ec,
					local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_copy_directory succeed-------------" << std::endl;
}

void copy_notify(const local_fsys_operators::path_type& from,
					const local_fsys_operators::path_type& to,
					const local_fsys_operators::path_type& current_from,
					const local_fsys_operators::path_type& current_to,
					bool current_result,
					boost::uintmax_t current_count,
					local_fsys_operators::error_code_type* current_ec,
					bool continueable,
					boost::progress_display& pd)
{
	if(current_result)
	{
		pd += current_count - pd.count();
	}
}

void test_recursive_copy_directories(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/trcd");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir/");

	local_fsys_operators::path_type dir_path1 = (root_dir_path / "dir/dir1");
	local_fsys_operators::path_type dir_path2 = (root_dir_path / "dir/dir2/");

	local_fsys_operators::path_type file_path1 = (root_dir_path / "dir/file1.txt");
	local_fsys_operators::path_type file_path2 = (root_dir_path / "dir/file2.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type dir_path_in_sym = ("../dir");

	local_fsys_operators::path_type symlink_path1 = (root_dir_path / "dir/symlink1");
	local_fsys_operators::path_type symlink_path2 = (root_dir_path / "dir/symlink2");
#endif // TEST_OF_ADMIN

	// 2 dir, 2 file, 2 symlink

	local_fsys_operators::path_type cped_dir_path = (root_dir_path / "r_cped_dir/");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir/");

	local_fsys_operators::path_type not_exists_dir_path = (root_dir_path / "not_exists_dir/dir");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(dir_path);

		local_fsys_operators::create_directories(dir_path1);
		local_fsys_operators::create_directories(dir_path2);

		local_fsys_operators::create_file_of_binary(file_path1, 1024);
		local_fsys_operators::create_file_of_binary(file_path2, 1024);

#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path1);
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path2);
#endif // TEST_OF_ADMIN

		local_fsys_operators::create_directories(exists_dir_path);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type copy_count = 0;
		local_fsys_operators::size_type dir_count = 0;

		local_fsys_operators::error_code_type ec;

		dir_count = 1 + local_fsys_operators::recursive_count_children_directory(dir_path);

		copy_count = local_fsys_operators::recursive_copy_directories(dir_path, dir_path);
		yggr_test_assert(0 == copy_count);

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, dir_path,
						local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(3 == copy_count);

		copy_count = local_fsys_operators::recursive_copy_directories(dir_path, cped_dir_path);
		yggr_test_assert(3 == copy_count);

		copy_count = local_fsys_operators::recursive_copy_directories(dir_path, exists_dir_path);
		yggr_test_assert(0 == copy_count);

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, exists_dir_path,
						local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(3 == copy_count);

		{
			boost::progress_display pd(dir_count);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path,
							boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
							local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(3 == copy_count);
		}

		{
			custom_unique_type my_handler(exists_dir_path);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path, my_handler,
							yggr::u32(local_fsys_operators::options_type::dir_rename_if_exists));
			yggr_test_assert(3 == copy_count);
		}

		{
			boost::progress_display pd(dir_count);
			custom_unique_type my_handler(exists_dir_path);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path,
							my_handler,
							boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
							yggr::u32(local_fsys_operators::options_type::dir_rename_if_exists));
			yggr_test_assert(3 == copy_count);
		}

		{
			boost::progress_display pd(dir_count);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path,
							boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
							local_fsys_operators::options_type::dir_merge_if_exists);
			yggr_test_assert(3 == copy_count);
		}

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, not_exists_dir_path, ec);
		yggr_test_assert(0 == copy_count);
		yggr_test_assert(!!ec);

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, not_exists_dir_path,
						local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(3 == copy_count);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(dir_path);

		local_fsys_operators::create_directories(dir_path1);
		local_fsys_operators::create_directories(dir_path2);

		local_fsys_operators::create_file_of_binary(file_path1, 1024);
		local_fsys_operators::create_file_of_binary(file_path2, 1024);

#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path1);
		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path2);
#endif // TEST_OF_ADMIN

		local_fsys_operators::create_directories(exists_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type copy_count = 0;
		local_fsys_operators::size_type dir_count = 0;

		local_fsys_operators::error_code_type ec;

		dir_count = 1 + local_fsys_operators::recursive_count_children_directory(dir_path);

		copy_count = local_fsys_operators::recursive_copy_directories(dir_path, dir_path, ec);
		yggr_test_assert(0 == copy_count);
		yggr_test_assert(!ec);

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, dir_path, ec,
						local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(3 == copy_count);
		yggr_test_assert(!ec);

		copy_count = local_fsys_operators::recursive_copy_directories(dir_path, cped_dir_path, ec);
		yggr_test_assert(3 == copy_count);
		yggr_test_assert(!ec);

		copy_count = local_fsys_operators::recursive_copy_directories(dir_path, exists_dir_path, ec);
		yggr_test_assert(0 == copy_count);
		yggr_test_assert(!ec);

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, exists_dir_path, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(3 == copy_count);
		yggr_test_assert(!ec);

		{
			boost::progress_display pd(dir_count);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path,
							boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
							ec,
							local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(3 == copy_count);
			yggr_test_assert(!ec);
		}

		{
			custom_unique_type my_handler(exists_dir_path);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path, my_handler,
							ec,
							yggr::u32(local_fsys_operators::options_type::dir_rename_if_exists));
			yggr_test_assert(3 == copy_count);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(dir_count);
			custom_unique_type my_handler(exists_dir_path);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path,
							my_handler,
							boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
							ec,
							yggr::u32(local_fsys_operators::options_type::dir_rename_if_exists));
			yggr_test_assert(3 == copy_count);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(dir_count);
			copy_count = local_fsys_operators::recursive_copy_directories(
							dir_path, exists_dir_path,
							boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
							ec,
							local_fsys_operators::options_type::dir_merge_if_exists);
			yggr_test_assert(3 == copy_count);
			yggr_test_assert(!ec);
		}

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, not_exists_dir_path, ec);
		yggr_test_assert(0 == copy_count);
		yggr_test_assert(!!ec);
		ec.clear();

		copy_count = local_fsys_operators::recursive_copy_directories(
						dir_path, not_exists_dir_path, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(3 == copy_count);
		yggr_test_assert(!ec);
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_recursive_copy_directories succeed-------------" << std::endl;
}

void test_remove_directory(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_remove_directory");

	local_fsys_operators::path_type rmable_dir_path = (root_dir_path / "rmable_dir");
	local_fsys_operators::path_type not_rmable_dir_path = (root_dir_path / "not_rmable_dir");
	local_fsys_operators::path_type not_rmable_file_path = (root_dir_path / "not_rmable_dir/file.txt");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(rmable_dir_path);
		local_fsys_operators::create_directories(not_rmable_dir_path);
		local_fsys_operators::create_file_of_binary(not_rmable_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::remove_directory(rmable_dir_path);
		yggr_test_assert(bchk);

		try
		{
			bchk = local_fsys_operators::remove_directory(not_rmable_dir_path);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cout << e.what() << std::endl;
			bchk = false;
		}
		yggr_test_assert(!bchk);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(rmable_dir_path);
		local_fsys_operators::create_directories(not_rmable_dir_path);
		local_fsys_operators::create_file_of_binary(not_rmable_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::remove_directory(rmable_dir_path, ec);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::remove_directory(not_rmable_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_remove_directory succeed-------------" << std::endl;
}

void rm_notify(const local_fsys_operators::path_type& path,
				const local_fsys_operators::path_type& current_path,
				bool current_result,
				boost::uintmax_t current_count,
				local_fsys_operators::error_code_type* current_ec,
				bool continueable,
				boost::progress_display& pd)
{
	if(current_result)
	{
		pd += current_count - pd.count();
	}
}

void test_recursive_remove_directories(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/trrd");

	local_fsys_operators::path_type rmable_dir_path = (root_dir_path / "rmable_dir/");

	local_fsys_operators::path_type rmable_dir_path1 = (rmable_dir_path / "dir1");
	local_fsys_operators::path_type rmable_dir_path2 = (rmable_dir_path / "dir2/");

	local_fsys_operators::path_type not_rmable_dir_path = (root_dir_path / "not_rmable_dir/");
	local_fsys_operators::path_type not_rmable_dir_path1 = (not_rmable_dir_path / "dir1");
	local_fsys_operators::path_type not_rmable_dir_path2 = (not_rmable_dir_path / "dir2/");

	local_fsys_operators::path_type not_rmable_file_path1 = (not_rmable_dir_path / "dir1/file.txt");
	local_fsys_operators::path_type not_rmable_file_path2 = (not_rmable_dir_path / "dir2/file.txt");

	local_fsys_operators::path_type part_rmable_dir_path = (root_dir_path / "part_rmable_dir/");
	local_fsys_operators::path_type part_rmable_dir_path1 = (part_rmable_dir_path / "dir1");
	local_fsys_operators::path_type part_rmable_dir_path2 = (part_rmable_dir_path / "dir2/");

	local_fsys_operators::path_type part_rmable_file_path2 = (part_rmable_dir_path / "dir2/file.txt");

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);

		local_fsys_operators::create_directories(rmable_dir_path);
		local_fsys_operators::create_directories(rmable_dir_path1);
		local_fsys_operators::create_directories(rmable_dir_path2);

		local_fsys_operators::create_directories(not_rmable_dir_path);
		local_fsys_operators::create_directories(not_rmable_dir_path1);
		local_fsys_operators::create_directories(not_rmable_dir_path2);

		local_fsys_operators::create_directories(part_rmable_dir_path);
		local_fsys_operators::create_directories(part_rmable_dir_path1);
		local_fsys_operators::create_directories(part_rmable_dir_path2);

		local_fsys_operators::create_file_of_binary(not_rmable_file_path1, 1024);
		local_fsys_operators::create_file_of_binary(not_rmable_file_path2, 1024);
		local_fsys_operators::create_file_of_binary(part_rmable_file_path2, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type rm_count = 0;

		rm_count = local_fsys_operators::recursive_remove_directories(rmable_dir_path);
		yggr_test_assert(rm_count == 3);

		rm_count = local_fsys_operators::recursive_remove_directories(not_rmable_dir_path);
		yggr_test_assert(rm_count == 0);

		// Different systems have different order of directory traversal, 
		// so use recursive_operator_ignore_if_failed to ensure the compatibility of test data
		rm_count = local_fsys_operators::recursive_remove_directories(
                        part_rmable_dir_path, 
                        local_fsys_operators::options_type::recursive_operator_ignore_if_failed);
		yggr_test_assert(rm_count == 1);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);

		local_fsys_operators::create_directories(rmable_dir_path);
		local_fsys_operators::create_directories(rmable_dir_path1);
		local_fsys_operators::create_directories(rmable_dir_path2);

		local_fsys_operators::create_directories(not_rmable_dir_path);
		local_fsys_operators::create_directories(not_rmable_dir_path1);
		local_fsys_operators::create_directories(not_rmable_dir_path2);

		local_fsys_operators::create_directories(part_rmable_dir_path);
		local_fsys_operators::create_directories(part_rmable_dir_path1);
		local_fsys_operators::create_directories(part_rmable_dir_path2);

		local_fsys_operators::create_file_of_binary(not_rmable_file_path1, 1024);
		local_fsys_operators::create_file_of_binary(not_rmable_file_path2, 1024);
		local_fsys_operators::create_file_of_binary(part_rmable_file_path2, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type rm_count = 0;

		{
			local_fsys_operators::size_type need_rm_count =
				1 + local_fsys_operators::recursive_count_children_directory(rmable_dir_path);

			boost::progress_display pd(need_rm_count);
			rm_count = local_fsys_operators::recursive_remove_directories(
						rmable_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)));
			yggr_test_assert(rm_count == 3);
		}

		{
			local_fsys_operators::size_type need_rm_count =
				1 + local_fsys_operators::recursive_count_children_directory(not_rmable_dir_path);
			boost::progress_display pd(need_rm_count);

			rm_count = local_fsys_operators::recursive_remove_directories(
						not_rmable_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)));
			yggr_test_assert(rm_count == 0);
			std::cout << std::endl;
		}

		{
			local_fsys_operators::size_type need_rm_count =
				1 + local_fsys_operators::recursive_count_children_directory(part_rmable_dir_path);
			boost::progress_display pd(need_rm_count);

			// Different systems have different order of directory traversal, 
			// so use recursive_operator_ignore_if_failed to ensure the compatibility of test data
			rm_count = local_fsys_operators::recursive_remove_directories(
						part_rmable_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)),
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed);
			yggr_test_assert(rm_count == 1);
			std::cout << std::endl;
		}
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);

		local_fsys_operators::create_directories(rmable_dir_path);
		local_fsys_operators::create_directories(rmable_dir_path1);
		local_fsys_operators::create_directories(rmable_dir_path2);

		local_fsys_operators::create_directories(not_rmable_dir_path);
		local_fsys_operators::create_directories(not_rmable_dir_path1);
		local_fsys_operators::create_directories(not_rmable_dir_path2);

		local_fsys_operators::create_directories(part_rmable_dir_path);
		local_fsys_operators::create_directories(part_rmable_dir_path1);
		local_fsys_operators::create_directories(part_rmable_dir_path2);

		local_fsys_operators::create_file_of_binary(not_rmable_file_path1, 1024);
		local_fsys_operators::create_file_of_binary(not_rmable_file_path2, 1024);
		local_fsys_operators::create_file_of_binary(part_rmable_file_path2, 1024);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		{
			local_fsys_operators::size_type rm_count = 0;
			local_fsys_operators::error_code_type ec;

			rm_count = local_fsys_operators::recursive_remove_directories(
						rmable_dir_path, ec);
			yggr_test_assert(rm_count == 3);
			yggr_test_assert(!ec);
		}

		{
			local_fsys_operators::size_type rm_count = 0;
			local_fsys_operators::error_code_type ec;

			rm_count = local_fsys_operators::recursive_remove_directories(
						not_rmable_dir_path, ec);
			yggr_test_assert(rm_count == 0);
			yggr_test_assert(!ec);
		}

		{
			local_fsys_operators::size_type rm_count = 0;
			local_fsys_operators::error_code_type ec;

			// Different systems have different order of directory traversal, 
			// so use recursive_operator_ignore_if_failed to ensure the compatibility of test data
			rm_count = local_fsys_operators::recursive_remove_directories(
						part_rmable_dir_path, ec,
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed);
			yggr_test_assert(rm_count == 1);
			yggr_test_assert(!ec);
		}
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);

		local_fsys_operators::create_directories(rmable_dir_path);
		local_fsys_operators::create_directories(rmable_dir_path1);
		local_fsys_operators::create_directories(rmable_dir_path2);

		local_fsys_operators::create_directories(not_rmable_dir_path);
		local_fsys_operators::create_directories(not_rmable_dir_path1);
		local_fsys_operators::create_directories(not_rmable_dir_path2);

		local_fsys_operators::create_directories(part_rmable_dir_path);
		local_fsys_operators::create_directories(part_rmable_dir_path1);
		local_fsys_operators::create_directories(part_rmable_dir_path2);

		local_fsys_operators::create_file_of_binary(not_rmable_file_path1, 1024);
		local_fsys_operators::create_file_of_binary(not_rmable_file_path2, 1024);
		local_fsys_operators::create_file_of_binary(part_rmable_file_path2, 1024);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type rm_count = 0;
		local_fsys_operators::error_code_type ec;

		{
			local_fsys_operators::size_type need_rm_count =
				1 + local_fsys_operators::recursive_count_children_directory(rmable_dir_path);

			boost::progress_display pd(need_rm_count);
			rm_count = local_fsys_operators::recursive_remove_directories(
						rmable_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)),
						ec);
			yggr_test_assert(rm_count == 3);
			yggr_test_assert(!ec);
		}

		{
			local_fsys_operators::size_type need_rm_count =
				1 + local_fsys_operators::recursive_count_children_directory(not_rmable_dir_path);
			boost::progress_display pd(need_rm_count);

			rm_count = local_fsys_operators::recursive_remove_directories(
						not_rmable_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)),
						ec);
			yggr_test_assert(rm_count == 0);
			yggr_test_assert(!ec);
			std::cout << std::endl;
		}

		{
			
			local_fsys_operators::size_type need_rm_count =
				1 + local_fsys_operators::recursive_count_children_directory(part_rmable_dir_path);
			boost::progress_display pd(need_rm_count);

			// Different systems have different order of directory traversal, 
			// so use recursive_operator_ignore_if_failed to ensure the compatibility of test data

			rm_count = local_fsys_operators::recursive_remove_directories(
						part_rmable_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed);
			yggr_test_assert(rm_count == 1);
			yggr_test_assert(!ec);
			std::cout << std::endl;
		}
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	std::cout << "--------------test_recursive_copy_directories succeed-------------" << std::endl;
}

void test_temp_directory_path(void)
{
	try
	{
		std::cout << local_fsys_operators::temp_directory_path() << std::endl;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::error_code_type ec;
		std::cout << local_fsys_operators::temp_directory_path(ec) << std::endl;

		yggr_test_assert(!ec);
	}

	std::cout << "--------------test_temp_directory_path succeed-------------" << std::endl;
}


int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_current_path();
	test_root_path();
	test_absolute();
	test_canonical();
	test_is_exists();
	test_permissions();
	test_expand_path();
	test_is_empty();
	test_is_other();
	test_last_write_time();
	test_space();

	test_check_is_included_in_directory();
	test_is_included_in_directory();
	test_check_is_child();
	test_is_child();
	test_count_children_directories();
	test_create_directories();
	test_create_parent_directories();
	test_create_directory();
	test_rename_directory();
	test_move_directory();
	test_copy_directory();
	test_recursive_copy_directories();
	test_remove_directory();
	test_recursive_remove_directories();
	test_temp_directory_path();

	wait_any_key(argc, argv);
	return 0;
}
