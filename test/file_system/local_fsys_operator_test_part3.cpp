//local_file_operator_test_part1.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <cassert>

#include <boost/bind.hpp>
#include <boost/progress.hpp>

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

const int item_count = 10;

bool create_test_base(const local_fsys_operators::path_type& test_path)
{
	local_fsys_operators::path_type root_dir_path = (test_path / "root_dir");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type sub_dir_path = "sub_dir";

	local_fsys_operators::path_type file_path = (root_dir_path / "file");
	local_fsys_operators::path_type sub_file_path = "sub_file";
	local_fsys_operators::path_type sub_sub_file_path = "sub_sub_file";

#if TEST_OF_ADMIN
	local_fsys_operators::path_type root_dir_path_in_sym = ("../root_dir");
	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");

	local_fsys_operators::path_type root_dir_path_in_sub_sym = ("../../root_dir");
	local_fsys_operators::path_type sub_symlink_path = "sub_symlink";

	local_fsys_operators::path_type root_dir_path_in_sub_sub_sym = ("../../../root_dir");
	local_fsys_operators::path_type sub_sub_symlink_path = "sub_sub_symlink";
#endif // TEST_OF_ADMIN


	local_fsys_operators::path_type op_dir_path;
	local_fsys_operators::path_type op_sub_dir_path;

	local_fsys_operators::path_type op_file_path;
	local_fsys_operators::path_type op_sub_file_path;
	local_fsys_operators::path_type op_sub_sub_file_path;

#if TEST_OF_ADMIN
	local_fsys_operators::path_type op_symlink_path;
	local_fsys_operators::path_type op_sub_symlink_path;
	local_fsys_operators::path_type op_sub_sub_symlink_path;
#endif // TEST_OF_ADMIN

	try
	{
		for(int i = 0, isize = item_count; i != isize; ++i)
		{
			std::stringstream ss;
			ss << i;

			op_dir_path = dir_path;
			op_dir_path	+= ss.str();

			op_file_path = file_path;
			op_file_path += ss.str();

			op_sub_dir_path = op_dir_path / sub_dir_path;
			op_sub_file_path = op_dir_path / sub_file_path;

			op_sub_sub_file_path = op_sub_dir_path /sub_sub_file_path;

#if TEST_OF_ADMIN
			op_symlink_path = symlink_path;
			op_symlink_path += ss.str();

			op_sub_symlink_path = op_dir_path / sub_symlink_path;
			sub_sub_symlink_path = op_sub_dir_path / op_sub_sub_symlink_path;
#endif // TEST_OF_ADMIN

			local_fsys_operators::create_directories(root_dir_path);

			local_fsys_operators::create_directories(op_dir_path);
			local_fsys_operators::create_directories(op_sub_dir_path);

			local_fsys_operators::create_file_of_binary(op_file_path, 1024);
			local_fsys_operators::create_file_of_binary(op_sub_file_path, 2048);
			local_fsys_operators::create_file_of_binary(op_sub_sub_file_path, 4096);

#if TEST_OF_ADMIN
			local_fsys_operators::create_symlink(root_dir_path_in_sym, op_symlink_path);
			local_fsys_operators::create_symlink(root_dir_path_in_sub_sym, op_sub_symlink_path);
			local_fsys_operators::create_symlink(root_dir_path_in_sub_sub_sym, op_sub_sub_symlink_path);
#endif // TEST_OF_ADMIN
		}
		return true;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return false;
	}
}

bool create_test_base2(const local_fsys_operators::path_type& test_path)
{
	local_fsys_operators::path_type root_dir_path = (test_path / "root_dir");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type sub_dir_path = "sub_dir";

	local_fsys_operators::path_type file_path = (root_dir_path / "file");
	local_fsys_operators::path_type file_hard_link_path = (root_dir_path / "file_hard_link");

	local_fsys_operators::path_type sub_file_path = "sub_file";
	local_fsys_operators::path_type sub_file_hard_link_path = "sub_file_hard_link";

	local_fsys_operators::path_type sub_sub_file_path = "sub_sub_file";
	local_fsys_operators::path_type sub_sub_file_hard_link_path = "sub_sub_file_hard_link";

#if TEST_OF_ADMIN
	local_fsys_operators::path_type root_dir_path_in_sym = ("../root_dir");
	local_fsys_operators::path_type root_dir_path_in_sub_sym = ("../../root_dir");
	local_fsys_operators::path_type root_dir_path_in_sub_sub_sym = ("../../../root_dir");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type symlink_hard_link_path = (root_dir_path / "symlink_hard_link");
	local_fsys_operators::path_type sub_symlink_path = "sub_symlink";
	local_fsys_operators::path_type sub_symlink_hard_link_path = "sub_symlink_hard_link";
	local_fsys_operators::path_type sub_sub_symlink_path = "sub_sub_symlink";
	local_fsys_operators::path_type sub_sub_symlink_hard_link_path = "sub_sub_symlink_hard_link";
#endif // TEST_OF_ADMIN


	local_fsys_operators::path_type op_dir_path;
	local_fsys_operators::path_type op_sub_dir_path;

	local_fsys_operators::path_type op_file_path;
	local_fsys_operators::path_type op_sub_file_path;
	local_fsys_operators::path_type op_sub_sub_file_path;

	local_fsys_operators::path_type op_file_hard_link_path;
	local_fsys_operators::path_type op_sub_file_hard_link_path;
	local_fsys_operators::path_type op_sub_sub_file_hard_link_path;

#if TEST_OF_ADMIN
	local_fsys_operators::path_type op_symlink_path;
	local_fsys_operators::path_type op_sub_symlink_path;
	local_fsys_operators::path_type op_sub_sub_symlink_path;

	local_fsys_operators::path_type op_symlink_hard_link_path;
	local_fsys_operators::path_type op_sub_symlink_hard_link_path;
	local_fsys_operators::path_type op_sub_sub_symlink_hard_link_path;
#endif // TEST_OF_ADMIN

    int i = 0;
	try
	{
		for(int isize = item_count; i != isize; ++i)
		{
			std::stringstream ss;
			ss << i;

			op_dir_path = dir_path;
			op_dir_path	+= ss.str();

			op_file_path = file_path;
			op_file_path += ss.str();

			op_file_hard_link_path = file_hard_link_path;
			op_file_hard_link_path += ss.str();

			op_sub_dir_path = op_dir_path / sub_dir_path;
			op_sub_file_path = op_dir_path / sub_file_path;
			op_sub_file_hard_link_path = op_dir_path / sub_file_hard_link_path;

			op_sub_sub_file_path = op_sub_dir_path / sub_sub_file_path;
			op_sub_sub_file_hard_link_path = op_sub_dir_path / sub_sub_file_hard_link_path;

#if TEST_OF_ADMIN
			op_symlink_path = symlink_path;
			op_symlink_path += ss.str();

			op_symlink_hard_link_path = symlink_hard_link_path;
			op_symlink_hard_link_path += ss.str();

			op_sub_symlink_path = op_dir_path / sub_symlink_path;
			op_sub_symlink_hard_link_path = op_dir_path / sub_symlink_hard_link_path;

			sub_sub_symlink_path = op_sub_dir_path / op_sub_sub_symlink_path;
			sub_sub_symlink_hard_link_path = op_sub_dir_path / op_sub_sub_symlink_hard_link_path;
#endif // TEST_OF_ADMIN

			local_fsys_operators::create_directories(root_dir_path);

			local_fsys_operators::create_directories(op_dir_path);
			local_fsys_operators::create_directories(op_sub_dir_path);

			local_fsys_operators::create_file_of_binary(op_file_path, 1024);
			local_fsys_operators::create_file_of_binary(op_sub_file_path, 2048);
			local_fsys_operators::create_file_of_binary(op_sub_sub_file_path, 4096);

#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			local_fsys_operators::create_hard_link(op_file_path, op_file_hard_link_path);
			local_fsys_operators::create_hard_link(op_sub_file_path, op_sub_file_hard_link_path);
			local_fsys_operators::create_hard_link(op_sub_sub_file_path, op_sub_sub_file_hard_link_path);
#endif // #	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root	

#if TEST_OF_ADMIN
			local_fsys_operators::create_symlink(root_dir_path_in_sym, op_symlink_path);
			local_fsys_operators::create_symlink(root_dir_path_in_sub_sym, op_sub_symlink_path);
			local_fsys_operators::create_symlink(root_dir_path_in_sub_sub_sym, op_sub_sub_symlink_path);

#	if !(defined(__ANDROID__) || defined(ANDROID) || defined(__APPLE__) || defined(__APPLE_CC__)) \
        || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			local_fsys_operators::create_hard_link(op_symlink_path, op_symlink_hard_link_path);
			local_fsys_operators::create_hard_link(op_sub_symlink_path, op_sub_symlink_hard_link_path);
			local_fsys_operators::create_hard_link(op_sub_sub_symlink_path, op_sub_sub_symlink_hard_link_path);
#	endif // #	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
#endif // TEST_OF_ADMIN
		}
		return true;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
        std::cout << "now_i = " << i << " now_item_count = " << item_count << std::endl;
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return false;
	}
}

bool remove_test_base(const local_fsys_operators::path_type& test_path)
{
	try
	{
		local_fsys_operators::remove_all(test_path);
		return true;
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
}

void test_recursive_copy_not_dir(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_recursive_copy_not_dir");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type cped_dir_path = (root_dir_path / "cped_dir");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type cped_file_path = (root_dir_path / "cped_file.txt");
	local_fsys_operators::path_type exists_file_path = (root_dir_path / "exists_file.txt");
	local_fsys_operators::path_type exists_file_dir_path = (root_dir_path / "exists_file_dir");

#if TEST_OF_ADMIN

	local_fsys_operators::path_type root_dir_path_in_sym =
		local_fsys_operators::make_path_object("../test_recursive_copy_not_dir");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type cped_symlink_path = (root_dir_path / "cped_symlink");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_symlink_dir_path = (root_dir_path / "exists_symlink_dir");

#endif //TEST_OF_ADMIN

	local_fsys_operators::path_type not_exists_dir_path = (root_dir_path / "not_exists_dir/dir");
	local_fsys_operators::path_type not_exists_file_path = (root_dir_path / "not_exists_file/file.txt");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists_symlink/symlink");


	local_fsys_operators::path_type sub_file_path = (dir_path / "file.txt");
	local_fsys_operators::path_type sub_exists_file_path = (exists_dir_path / "file.txt");

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
		local_fsys_operators::create_directories(exists_dir_path);
		local_fsys_operators::create_directories(exists_file_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);

#if TEST_OF_ADMIN
		local_fsys_operators::create_directories(exists_symlink_dir_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, exists_symlink_path);
#endif // TEST_OF_ADMIN

		local_fsys_operators::create_file_of_binary(sub_file_path, 1024);
		local_fsys_operators::create_file_of_binary(sub_exists_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type size_chk = false;

		// file
		size_chk = local_fsys_operators::recursive_copy(file_path, cped_file_path, 0, 0);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(file_path, cped_file_path);
		yggr_test_assert(size_chk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(cped_file_path));

		size_chk = local_fsys_operators::recursive_copy(file_path, exists_file_path);
		yggr_test_assert(!size_chk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		size_chk = local_fsys_operators::recursive_copy(file_path, exists_dir_path);
		yggr_test_assert(!size_chk);
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(size_chk);

		{
			custom_unique_type my_unique_handler(exists_file_path);
			size_chk = local_fsys_operators::recursive_copy(
					file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(size_chk);
		}

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(size_chk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));


		size_chk = local_fsys_operators::recursive_copy(file_path, not_exists_file_path);
		yggr_test_assert(!size_chk);


		size_chk = local_fsys_operators::recursive_copy(
				file_path, not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(size_chk);

		// symlink
#if TEST_OF_ADMIN
		size_chk = local_fsys_operators::recursive_copy(symlink_path, cped_symlink_path, 0, 0);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, cped_symlink_path);
		yggr_test_assert(size_chk);
		yggr_test_assert(local_fsys_operators::is_symlink(cped_symlink_path));

		size_chk = local_fsys_operators::recursive_copy(symlink_path, exists_symlink_path);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, exists_dir_path);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(size_chk);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(size_chk);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			size_chk = local_fsys_operators::recursive_copy(
					symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(size_chk);
		}

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(size_chk);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(size_chk);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, not_exists_symlink_path);
		yggr_test_assert(!size_chk);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(size_chk);
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

		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);
		local_fsys_operators::create_directories(exists_file_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);

#if TEST_OF_ADMIN
		local_fsys_operators::create_directories(exists_symlink_dir_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, exists_symlink_path);
#endif // TEST_OF_ADMIN

		local_fsys_operators::create_file_of_binary(sub_file_path, 1024);
		local_fsys_operators::create_file_of_binary(sub_exists_file_path, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type size_chk = false;
		local_fsys_operators::error_code_type ec;

		// file
		size_chk = local_fsys_operators::recursive_copy(file_path, cped_file_path, ec, 0, 0);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(file_path, cped_file_path, ec);
		yggr_test_assert(size_chk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(cped_file_path));
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(file_path, exists_file_path, ec);
		yggr_test_assert(!size_chk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(file_path, exists_dir_path, ec);
		yggr_test_assert(!size_chk);
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_file_path);
			size_chk = local_fsys_operators::recursive_copy(
					file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(size_chk);
			yggr_test_assert(!ec);
		}

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(size_chk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));
		yggr_test_assert(!ec);


		size_chk = local_fsys_operators::recursive_copy(file_path, not_exists_file_path, ec);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		size_chk = local_fsys_operators::recursive_copy(
				file_path, not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);

		// symlink
#if TEST_OF_ADMIN
		size_chk = local_fsys_operators::recursive_copy(symlink_path, cped_symlink_path, ec, 0, 0);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, cped_symlink_path, ec);
		yggr_test_assert(size_chk);
		yggr_test_assert(local_fsys_operators::is_symlink(cped_symlink_path));
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, exists_dir_path, ec);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			size_chk = local_fsys_operators::recursive_copy(
					symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(size_chk);
			yggr_test_assert(!ec);
		}

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);

		size_chk = local_fsys_operators::recursive_copy(symlink_path, not_exists_symlink_path, ec);
		yggr_test_assert(!size_chk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		size_chk = local_fsys_operators::recursive_copy(
				symlink_path, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(size_chk);
		yggr_test_assert(!ec);
#endif // TEST_OF_ADMIN
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

	std::cout << "--------------test_recursive_copy_not_dir succeed-------------" << std::endl;
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


void test_recursive_copy_dir(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_recursive_copy_dir";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type cped_root_dir_path = (test_dir_path / "cped_root_dir");
	local_fsys_operators::path_type cped_root_dir_path2 = (test_dir_path / "cped_root_dir2");
	local_fsys_operators::path_type exists_root_dir = (test_dir_path / "exists_root_dir");
	local_fsys_operators::path_type not_exists_dir = (test_dir_path / "not_exists_root_dir/dir");

	local_fsys_operators::path_type root_sub_dir_path = (root_dir_path / "dir0");

	remove_test_base(test_dir_path);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		std::cout << cmp_children_size << std::endl;
		std::cout << cmp_children_dir_size << std::endl;
		std::cout << cmp_children_file_size << std::endl;
		std::cout << cmp_children_symlink_size << std::endl;
		std::cout << cmp_size << std::endl;


		local_fsys_operators::size_type chk_size = 0;
		local_fsys_operators::create_directories(exists_root_dir);
		yggr_test_assert(local_fsys_operators::is_directory(exists_root_dir));

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, root_sub_dir_path);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, not_exists_dir);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, not_exists_dir,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(not_exists_dir) + 1);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, root_dir_path);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, exists_root_dir);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, exists_root_dir,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(chk_size == 1);

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, exists_root_dir,
					local_fsys_operators::options_type::dir_merge_if_exists);
		yggr_test_assert(chk_size == cmp_size);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, cped_root_dir_path, 0, 0);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, cped_root_dir_path);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(cped_root_dir_path) + 1);

#if TEST_OF_ADMIN
		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, 0,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(cped_root_dir_path2));

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == cmp_children_symlink_size);
		yggr_test_assert(cmp_children_symlink_size == local_fsys_operators::recursive_count_children_symlink(cped_root_dir_path2));

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2,
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size - 10); // 10 is sub dir file
		yggr_test_assert(cmp_children_file_size - 10 == local_fsys_operators::recursive_count_children_file(cped_root_dir_path2));

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2,
						local_fsys_operators::options_type::dir_create_if_not_exists
						| local_fsys_operators::options_type::recursive_operator_ignore_if_failed,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 10); // 10 is sub dir file
		yggr_test_assert(cmp_children_file_size == local_fsys_operators::recursive_count_children_file(cped_root_dir_path2));

#else
		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, 0,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(cped_root_dir_path2));

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size);
		yggr_test_assert(cmp_children_file_size == local_fsys_operators::recursive_count_children_file(cped_root_dir_path2));

#endif // TEST_OF_ADMIN

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, exists_root_dir,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);

		{
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, exists_root_dir, my_unique_handler,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
		}

		chk_size = local_fsys_operators::recursive_copy(
					cped_root_dir_path, cped_root_dir_path,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size + cmp_children_file_size + cmp_children_symlink_size - 1
					== local_fsys_operators::recursive_count_children(cped_root_dir_path));

		{
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_copy(
					cped_root_dir_path2, cped_root_dir_path2, my_unique_handler,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size + cmp_children_file_size + cmp_children_symlink_size - 1
						== local_fsys_operators::recursive_count_children(cped_root_dir_path2));
		}

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, cped_root_dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(cped_root_dir_path) + 1);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path,
						boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_replace_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(cped_root_dir_path) + 1);
		}

		{
			boost::progress_display pd(cmp_size);
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, exists_root_dir,
						my_unique_handler,
						boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
		}
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	{
		local_fsys_operators::error_code_type ec;

		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		std::cout << cmp_children_size << std::endl;
		std::cout << cmp_children_dir_size << std::endl;
		std::cout << cmp_children_file_size << std::endl;
		std::cout << cmp_children_symlink_size << std::endl;
		std::cout << cmp_size << std::endl;

		local_fsys_operators::size_type chk_size = 0;
		local_fsys_operators::create_directories(exists_root_dir);
		yggr_test_assert(local_fsys_operators::is_directory(exists_root_dir));

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, root_sub_dir_path, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, not_exists_dir, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, not_exists_dir, ec,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(not_exists_dir) + 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, root_dir_path, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, exists_root_dir, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, exists_root_dir, ec,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(chk_size == 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, exists_root_dir, ec,
					local_fsys_operators::options_type::dir_merge_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, cped_root_dir_path, ec, 0, 0);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(root_dir_path, cped_root_dir_path, ec);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(cped_root_dir_path) + 1);
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN
		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, ec, 0,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(cped_root_dir_path2));
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == cmp_children_symlink_size);
		yggr_test_assert(cmp_children_symlink_size == local_fsys_operators::recursive_count_children_symlink(cped_root_dir_path2));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, ec,
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size - 10); // 10 is sub dir file
		yggr_test_assert(cmp_children_file_size - 10 == local_fsys_operators::recursive_count_children_file(cped_root_dir_path2));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists
						| local_fsys_operators::options_type::recursive_operator_ignore_if_failed,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 10); // 10 is sub dir file
		yggr_test_assert(cmp_children_file_size == local_fsys_operators::recursive_count_children_file(cped_root_dir_path2));
		yggr_test_assert(!ec);
#else
		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, ec, 0,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(cped_root_dir_path2));
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path2, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size);
		yggr_test_assert(cmp_children_file_size == local_fsys_operators::recursive_count_children_file(cped_root_dir_path2));
		yggr_test_assert(!ec);
#endif // TEST_OF_ADMIN

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, exists_root_dir, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, exists_root_dir, my_unique_handler, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!ec);
		}

		chk_size = local_fsys_operators::recursive_copy(
					cped_root_dir_path, cped_root_dir_path, ec,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size + cmp_children_file_size + cmp_children_symlink_size - 1
					== local_fsys_operators::recursive_count_children(cped_root_dir_path));
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_copy(
					cped_root_dir_path2, cped_root_dir_path2, my_unique_handler, ec,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size + cmp_children_file_size + cmp_children_symlink_size - 1
						== local_fsys_operators::recursive_count_children(cped_root_dir_path2));
			yggr_test_assert(!ec);
		}

		chk_size = local_fsys_operators::recursive_copy(
					root_dir_path, cped_root_dir_path, ec,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(cped_root_dir_path) + 1);
		yggr_test_assert(!ec);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, cped_root_dir_path,
						boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_replace_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(cped_root_dir_path) + 1);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_size);
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_copy(
						root_dir_path, exists_root_dir,
						my_unique_handler,
						boost::bind(&copy_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!ec);
		}
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	std::cout << "--------------test_recursive_copy_dir succeed-------------" << std::endl;
}

void test_recursive_move_not_dir(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_recursive_move_not_dir");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type moved_dir_path = (root_dir_path / "moved_dir");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type exists_sub_dir_path = (exists_dir_path / "dir");
	local_fsys_operators::path_type not_exists_dir_path = (root_dir_path / "not_exists_dir/dir");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type moved_file_path = (root_dir_path / "moved_file.txt");
	local_fsys_operators::path_type exists_file_path = (root_dir_path / "exists_file.txt");
	local_fsys_operators::path_type exists_file_dir_path = (root_dir_path / "exists_file_dir");
	local_fsys_operators::path_type exists_sub_file_path = (exists_file_dir_path / "file.txt");
	local_fsys_operators::path_type not_exists_file_path = (root_dir_path / "not_exists_file_dir/file.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type root_dir_path_in_sym =
		local_fsys_operators::make_path_object("../test_recursive_move_not_dir");

	local_fsys_operators::path_type dir_path_in_sym =
		local_fsys_operators::make_path_object("dir");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type moved_symlink_path = (root_dir_path / "moved_symlink");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_symlink_dir_path = (root_dir_path / "exists_symlink_dir");
	local_fsys_operators::path_type exists_sub_symlink_path = (exists_symlink_dir_path / "symlink");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists_symlink_dir/symlink");
#endif // TEST_OF_ADMIN

#if defined(YGGR_AT_WINDOWS)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("f:/yggr_diff_vol_test_out/test_recursive_move_not_dir");
#elif defined(YGGR_AT_LINUX)
#	if (defined(__ANDROID__) || defined(ANDROID))
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/storage/C5B8-A1D4/yggr_diff_vol_test_out/diff_test_recursive_move_not_dir/");
#	else
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/run/media/alucard/win10_games/yggr_diff_vol_test_out/diff_test_recursive_move_not_dir/");
#	endif // #	if defined(__ANDROID__) || defined(ANDROID)
#elif defined(YGGR_AT_DARWIN)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/Volumes/win10_games/yggr_diff_vol_test_out/");
#else
#	error "!!!append this platfrom diff vol path!!!"
#endif // YGGR_AT_WINDOWS

	local_fsys_operators::path_type diff_vol_moved_dir_path = (diff_vol_root_dir_path / "moved_dir");
	local_fsys_operators::path_type diff_vol_not_exists_dir_path = (diff_vol_root_dir_path / "not_exists_dir/moved_dir");

	local_fsys_operators::path_type diff_vol_moved_file_path = (diff_vol_root_dir_path / "moved_file.txt");
	local_fsys_operators::path_type diff_vol_not_exists_file_path = (diff_vol_root_dir_path / "not_exists_file_dir/moved_file.txt");

#if TEST_OF_ADMIN
	local_fsys_operators::path_type diff_vol_moved_symlink_path = (diff_vol_root_dir_path / "moved_symlink");
	local_fsys_operators::path_type diff_vol_not_exists_symlink_path = (diff_vol_root_dir_path / "not_exists_symlink_dir/moved_symlink");
#endif // TEST_OF_ADMIN

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);

		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_directories(exists_file_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);

#if TEST_OF_ADMIN
		local_fsys_operators::create_directories(exists_symlink_dir_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, exists_symlink_path);
#endif // TEST_OF_ADMIN

		local_fsys_operators::create_directories(diff_vol_root_dir_path);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type chk_size = false;

		// file
		chk_size = local_fsys_operators::recursive_move(file_path, moved_file_path, 0, 0);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_move(file_path, moved_file_path);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));

		chk_size = local_fsys_operators::recursive_move(moved_file_path, exists_file_path);
		yggr_test_assert(!chk_size);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));

		chk_size = local_fsys_operators::recursive_move(moved_file_path, exists_file_dir_path);
		yggr_test_assert(!chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_file_dir_path));

		chk_size = local_fsys_operators::recursive_move(moved_file_path, exists_sub_file_path);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_sub_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));

		{
			local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			chk_size = local_fsys_operators::recursive_move(
					moved_file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(chk_size);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		}

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(moved_file_path, not_exists_file_path);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, diff_vol_moved_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_vol_moved_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(moved_file_path, diff_vol_not_exists_file_path);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, diff_vol_not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);

		//symlink
#if TEST_OF_ADMIN
		chk_size = local_fsys_operators::recursive_move(symlink_path, moved_symlink_path, 0, 0);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_move(symlink_path, moved_symlink_path);
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));

		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, exists_symlink_path);
		yggr_test_assert(!chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));

		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, exists_symlink_dir_path);
		yggr_test_assert(!chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));

		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, exists_sub_symlink_path);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_sub_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			chk_size = local_fsys_operators::recursive_move(
					moved_symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(chk_size);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_dir_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, not_exists_symlink_path);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, diff_vol_moved_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#	else
		yggr_test_assert(!chk_size);	
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#	endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, diff_vol_not_exists_symlink_path);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, diff_vol_not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#	else
		yggr_test_assert(!chk_size);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#	endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
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
		local_fsys_operators::remove_all(root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
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

		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_directories(exists_file_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);

#if TEST_OF_ADMIN
		local_fsys_operators::create_directories(exists_symlink_dir_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(root_dir_path_in_sym, exists_symlink_path);

#endif // TEST_OF_ADMIN

		local_fsys_operators::create_directories(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type chk_size = false;
		local_fsys_operators::error_code_type ec;

		// file
		chk_size = local_fsys_operators::recursive_move(file_path, moved_file_path, ec, 0, 0);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(file_path, moved_file_path, ec);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(moved_file_path, exists_file_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(moved_file_path, exists_file_dir_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_file_dir_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(moved_file_path, exists_sub_file_path, ec);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_sub_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			chk_size = local_fsys_operators::recursive_move(
					moved_file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(chk_size);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(moved_file_path, not_exists_file_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, diff_vol_moved_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_vol_moved_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		chk_size = local_fsys_operators::recursive_move(moved_file_path, diff_vol_not_exists_file_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
				moved_file_path, diff_vol_not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!ec);

		//symlink
#if TEST_OF_ADMIN
		chk_size = local_fsys_operators::recursive_move(symlink_path, moved_symlink_path, ec, 0, 0);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(symlink_path, moved_symlink_path, ec);
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, exists_symlink_dir_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, exists_sub_symlink_path, ec);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_sub_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			chk_size = local_fsys_operators::recursive_move(
					moved_symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(chk_size);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!chk_size);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, not_exists_symlink_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, diff_vol_moved_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
		yggr_test_assert(!ec);
#	else
		yggr_test_assert(!chk_size);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
		yggr_test_assert(!!ec);
		ec.clear();
#	endif // #	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		chk_size = local_fsys_operators::recursive_move(moved_symlink_path, diff_vol_not_exists_symlink_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
				moved_symlink_path, diff_vol_not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(chk_size);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
		yggr_test_assert(!ec);
#	else
		yggr_test_assert(!chk_size);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
		yggr_test_assert(!!ec);
		ec.clear();
#	endif // #	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
#endif // TEST_OF_ADMIN
	}

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
		local_fsys_operators::remove_all(diff_vol_root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	std::cout << "--------------test_recursive_move_not_dir succeed-------------" << std::endl;
}


void move_notify(const local_fsys_operators::path_type& old_path,
					const local_fsys_operators::path_type& new_path,
					const local_fsys_operators::path_type& current_old_path,
					const local_fsys_operators::path_type& current_new_path,
					bool current_result,
					boost::uintmax_t current_count,
					local_fsys_operators::error_code_type* current_ec,
					bool continueable,
					boost::progress_display& pd)
{
	copy_notify(old_path, new_path, current_old_path, current_new_path,
				current_result, current_count, current_ec,
				continueable, pd);
}

void test_recursive_move_dir(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_recursive_move_dir";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type moved_root_dir_path = (test_dir_path / "moved_root_dir");
	local_fsys_operators::path_type moved_root_dir_path2 = (test_dir_path / "moved_root_dir2");
	local_fsys_operators::path_type exists_root_dir = (test_dir_path / "exists_root_dir");
	local_fsys_operators::path_type not_exists_root_dir = (test_dir_path / "not_exists_dir/moved_root_dir");

	local_fsys_operators::path_type root_sub_dir_path = (root_dir_path / "dir0");

#if defined(YGGR_AT_WINDOWS)
	local_fsys_operators::path_type diff_vol_test_dir_path = "f:/yggr_diff_vol_test_out/test_recursive_move_dir";
#elif defined(YGGR_AT_LINUX)
#	if (defined(__ANDROID__) || defined(ANDROID))
	local_fsys_operators::path_type diff_vol_test_dir_path =
		local_fsys_operators::make_path_object("/storage/C5B8-A1D4/yggr_diff_vol_test_out/diff_test_recursive_move_dir/");
#	else
	local_fsys_operators::path_type diff_vol_test_dir_path =
		local_fsys_operators::make_path_object("/run/media/alucard/win10_games/yggr_diff_vol_test_out/diff_test_recursive_move_dir/");
#	endif // #	if defined(__ANDROID__) || defined(ANDROID)
#elif defined(YGGR_AT_DARWIN)
	local_fsys_operators::path_type diff_vol_test_dir_path =
		local_fsys_operators::make_path_object("/Volumes/win10_games/yggr_diff_vol_test_out/");
#else
#	error "!!!append this platfrom diff vol path!!!"
#endif // YGGR_AT_WINDOWS


	local_fsys_operators::path_type diff_vol_moved_root_dir_path = (diff_vol_test_dir_path / "moved_root_dir");
	local_fsys_operators::path_type diff_vol_exists_root_dir = (diff_vol_test_dir_path / "exists_root_dir");
	local_fsys_operators::path_type diff_vol_not_exists_root_dir = (diff_vol_test_dir_path / "not_exists_dir/moved_root_dir");

	remove_test_base(test_dir_path);
	remove_test_base(diff_vol_test_dir_path);


	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		std::cout << cmp_children_size << std::endl;
		std::cout << cmp_children_dir_size << std::endl;
		std::cout << cmp_children_file_size << std::endl;
		std::cout << cmp_children_symlink_size << std::endl;
		std::cout << cmp_size << std::endl;


		local_fsys_operators::size_type chk_size = 0;
		local_fsys_operators::create_directories(exists_root_dir);
		yggr_test_assert(local_fsys_operators::is_directory(exists_root_dir));

		local_fsys_operators::create_directories(diff_vol_test_dir_path);
		yggr_test_assert(local_fsys_operators::is_directory(diff_vol_test_dir_path));

		local_fsys_operators::create_directories(diff_vol_exists_root_dir);
		yggr_test_assert(local_fsys_operators::is_directory(diff_vol_exists_root_dir));

		chk_size = local_fsys_operators::recursive_move(root_dir_path, root_sub_dir_path);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, not_exists_root_dir);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, not_exists_root_dir,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(not_exists_root_dir) + 1);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, root_dir_path);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, exists_root_dir);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, exists_root_dir,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(chk_size == 1);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, exists_root_dir,
					local_fsys_operators::options_type::dir_merge_if_exists);
		yggr_test_assert(chk_size == cmp_size);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, moved_root_dir_path, 0, 0);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, moved_root_dir_path);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

#if TEST_OF_ADMIN
		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2,
						0,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_root_dir_path2));

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == cmp_children_symlink_size);
		yggr_test_assert(cmp_children_symlink_size == local_fsys_operators::recursive_count_children_symlink(moved_root_dir_path2));

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2,
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size - 10);
		yggr_test_assert(cmp_children_file_size - 10 == local_fsys_operators::recursive_count_children_file(moved_root_dir_path2));

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 10);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path2) + 1);
#else
		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2,
						0,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_root_dir_path2));

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size);
		yggr_test_assert(cmp_children_file_size == local_fsys_operators::recursive_count_children_file(moved_root_dir_path2));
#endif //TEST_OF_ADMIN

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, exists_root_dir,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, exists_root_dir, my_unique_handler,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
		}

		chk_size = local_fsys_operators::recursive_move(
					moved_root_dir_path, moved_root_dir_path,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);

		{
			custom_unique_type my_unique_handler(moved_root_dir_path2);
			chk_size = local_fsys_operators::recursive_move(
					moved_root_dir_path, moved_root_dir_path2, my_unique_handler,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size + cmp_children_file_size + cmp_children_symlink_size - 1
						== local_fsys_operators::recursive_count_children(moved_root_dir_path2));
		}

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, moved_root_dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);
		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_replace_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);
		}

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);
		{
			boost::progress_display pd(cmp_size);
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, exists_root_dir,
						my_unique_handler,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
		}

		bchk = create_test_base(test_dir_path);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_moved_root_dir_path,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_replace_if_exists);
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(diff_vol_moved_root_dir_path) + 1);
#else
			yggr_test_assert(chk_size != cmp_size);
			yggr_test_assert(cmp_size != local_fsys_operators::recursive_count_children(diff_vol_moved_root_dir_path) + 1);
#endif // #	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		}

		bchk = create_test_base(test_dir_path);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_exists_root_dir,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_merge_if_exists);
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(diff_vol_exists_root_dir) + 1);
#else
			yggr_test_assert(chk_size != cmp_size);
			yggr_test_assert(cmp_size != local_fsys_operators::recursive_count_children(diff_vol_exists_root_dir) + 1);
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		}

		bchk = create_test_base(test_dir_path);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_not_exists_root_dir,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)));
			yggr_test_assert(chk_size == 0);
		}

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_not_exists_root_dir,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						local_fsys_operators::options_type::dir_create_if_not_exists);
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(diff_vol_not_exists_root_dir) + 1);
#else
			yggr_test_assert(chk_size != cmp_size);
			yggr_test_assert(cmp_size != local_fsys_operators::recursive_count_children(diff_vol_not_exists_root_dir) + 1);
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		}
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);
	bchk = remove_test_base(diff_vol_test_dir_path);
	yggr_test_assert(bchk);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	{
		local_fsys_operators::error_code_type ec;

		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		std::cout << cmp_children_size << std::endl;
		std::cout << cmp_children_dir_size << std::endl;
		std::cout << cmp_children_file_size << std::endl;
		std::cout << cmp_children_symlink_size << std::endl;
		std::cout << cmp_size << std::endl;

		local_fsys_operators::size_type chk_size = 0;
		local_fsys_operators::create_directories(exists_root_dir);
		yggr_test_assert(local_fsys_operators::is_directory(exists_root_dir));

		local_fsys_operators::create_directories(diff_vol_test_dir_path);
		yggr_test_assert(local_fsys_operators::is_directory(diff_vol_test_dir_path));

		local_fsys_operators::create_directories(diff_vol_exists_root_dir);
		yggr_test_assert(local_fsys_operators::is_directory(diff_vol_exists_root_dir));

		chk_size = local_fsys_operators::recursive_move(root_dir_path, root_sub_dir_path, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, not_exists_root_dir, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, not_exists_root_dir, ec,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(not_exists_root_dir) + 1);
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, root_dir_path, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, exists_root_dir, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, exists_root_dir, ec,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(chk_size == 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, exists_root_dir, ec,
					local_fsys_operators::options_type::dir_merge_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, moved_root_dir_path, ec, 0, 0);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(root_dir_path, moved_root_dir_path, ec);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

#if TEST_OF_ADMIN
		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2, ec,
						0, local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_root_dir_path2));
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == cmp_children_symlink_size);
		yggr_test_assert(cmp_children_symlink_size == local_fsys_operators::recursive_count_children_symlink(moved_root_dir_path2));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2, ec,
						local_fsys_operators::options_type::recursive_operator_ignore_if_failed,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size - 10);
		yggr_test_assert(cmp_children_file_size - 10 == local_fsys_operators::recursive_count_children_file(moved_root_dir_path2));
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 10);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path2) + 1);
		yggr_test_assert(!ec);

#else

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2, ec,
						0, local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_root_dir_path2));
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path2, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists,
						local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size);
		yggr_test_assert(cmp_children_file_size == local_fsys_operators::recursive_count_children_file(moved_root_dir_path2));
		yggr_test_assert(!ec);

#endif // TEST_OF_ADMIN

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, exists_root_dir, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, exists_root_dir, my_unique_handler, ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!ec);
		}

		chk_size = local_fsys_operators::recursive_move(
					moved_root_dir_path, moved_root_dir_path,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(moved_root_dir_path2);
			chk_size = local_fsys_operators::recursive_move(
					moved_root_dir_path, moved_root_dir_path2, my_unique_handler, ec,
					local_fsys_operators::options_type::dir_merge_if_exists
					| local_fsys_operators::options_type::file_rename_if_exists
					| local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size + cmp_children_file_size + cmp_children_symlink_size - 1
						== local_fsys_operators::recursive_count_children(moved_root_dir_path2));
			yggr_test_assert(!ec);
		}

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_move(
					root_dir_path, moved_root_dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);
		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, moved_root_dir_path,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_replace_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(moved_root_dir_path) + 1);
			yggr_test_assert(!ec);
		}

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);
		{
			boost::progress_display pd(cmp_size);
			custom_unique_type my_unique_handler(exists_root_dir);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, exists_root_dir,
						my_unique_handler,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!ec);
		}

		bchk = create_test_base(test_dir_path);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_moved_root_dir_path,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_replace_if_exists);

#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(diff_vol_moved_root_dir_path) + 1);
			yggr_test_assert(!ec);
#else
			yggr_test_assert(chk_size != cmp_size);
			yggr_test_assert(cmp_size != local_fsys_operators::recursive_count_children(diff_vol_moved_root_dir_path) + 1);
			yggr_test_assert(!!ec);
			ec.clear();
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		}

		bchk = create_test_base(test_dir_path);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_exists_root_dir,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_merge_if_exists);
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(diff_vol_exists_root_dir) + 1);
			yggr_test_assert(!ec);
#else
			yggr_test_assert(chk_size != cmp_size);
			yggr_test_assert(cmp_size != local_fsys_operators::recursive_count_children(diff_vol_exists_root_dir) + 1);
			yggr_test_assert(!!ec);
			ec.clear();
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		}

		bchk = create_test_base(test_dir_path);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_not_exists_root_dir,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec);
			yggr_test_assert(chk_size == 0);
			yggr_test_assert(!!ec);
			std::cout << ec.message() << std::endl;
			ec.clear();
		}

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_move(
						root_dir_path, diff_vol_not_exists_root_dir,
						boost::bind(&move_notify, _1, _2, _3, _4, _5, _6, _7, _8, boost::ref(pd)),
						ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(diff_vol_not_exists_root_dir) + 1);
			yggr_test_assert(!ec);
#else
			yggr_test_assert(chk_size != cmp_size);
			yggr_test_assert(cmp_size != local_fsys_operators::recursive_count_children(diff_vol_not_exists_root_dir) + 1);
			yggr_test_assert(!!ec);
			ec.clear();
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		}
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);
	bchk = remove_test_base(diff_vol_test_dir_path);
	yggr_test_assert(bchk);

	std::cout << "--------------test_recursive_move_dir succeed-------------" << std::endl;
}

void test_recursive_remove_not_dir(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_recursive_remove_not_dir");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
#if TEST_OF_ADMIN
	local_fsys_operators::path_type root_dir_path_in_sym =
		local_fsys_operators::make_path_object("../test_recursive_remove_not_dir");
	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
#endif //TEST_OF_ADMIN

	local_fsys_operators::path_type non_empty_dir_path = (root_dir_path / "non_empty_dir");
	local_fsys_operators::path_type non_empty_file_path = (non_empty_dir_path / "non_empty_file.txt");

	local_fsys_operators::path_type not_exists_path = (root_dir_path / "not_exists");


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
		local_fsys_operators::create_directories(non_empty_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(non_empty_file_path, 1024);

#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(root_dir_path_in_sym, symlink_path);
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
		local_fsys_operators::size_type chk_size = 0;

		// gen

		chk_size = local_fsys_operators::recursive_remove(not_exists_path);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_remove(
				not_exists_path, local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(chk_size);

		// file
		chk_size = local_fsys_operators::recursive_remove(file_path, 0, 0);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_remove(file_path);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));

#if TEST_OF_ADMIN

		chk_size = local_fsys_operators::recursive_remove(symlink_path, 0, 0);
		yggr_test_assert(!chk_size);

		chk_size = local_fsys_operators::recursive_remove(symlink_path);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));

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
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(non_empty_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(non_empty_file_path, 1024);

#if TEST_OF_ADMIN
		local_fsys_operators::create_symlink(root_dir_path_in_sym, symlink_path);
#endif // TEST_OF_ADMIN
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type chk_size = 0;
		local_fsys_operators::error_code_type ec;

		// gen

		chk_size = local_fsys_operators::recursive_remove(not_exists_path, ec);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(
				not_exists_path, ec,
				local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(chk_size);
		yggr_test_assert(!ec);

		// file
		chk_size = local_fsys_operators::recursive_remove(file_path, ec, 0, 0);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(file_path, ec);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN

		chk_size = local_fsys_operators::recursive_remove(symlink_path, ec, 0, 0);
		yggr_test_assert(!chk_size);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(symlink_path, ec);
		yggr_test_assert(chk_size);
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));
		yggr_test_assert(!ec);

#endif // TEST_OF_ADMIN
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

	std::cout << "--------------test_recursive_remove_not_dir succeed-------------" << std::endl;
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

void test_recursive_remove_dir(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_recursive_remove_dir";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type not_exists_root_dir = (test_dir_path / "not_exists_dir/rmable_root_dir");

	remove_test_base(test_dir_path);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		chk_size = local_fsys_operators::recursive_remove(not_exists_root_dir);
		yggr_test_assert(chk_size == 0);

		chk_size = local_fsys_operators::recursive_remove(
						not_exists_root_dir,
						local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(chk_size == 1);

		chk_size = local_fsys_operators::recursive_remove(root_dir_path);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(!local_fsys_operators::is_exists(root_dir_path));

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_remove(root_dir_path, 0, 0);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(root_dir_path) + 1);

		chk_size = local_fsys_operators::recursive_remove(
					root_dir_path, 0,
					local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == cmp_children_symlink_size);
		yggr_test_assert(cmp_size - cmp_children_symlink_size == local_fsys_operators::recursive_count_children(root_dir_path) + 1);

		chk_size = local_fsys_operators::recursive_remove(
					root_dir_path, 0,
					local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size);
		yggr_test_assert(cmp_size - (cmp_children_file_size + cmp_children_symlink_size)
				== local_fsys_operators::recursive_count_children(root_dir_path) + 1);

		chk_size = local_fsys_operators::recursive_remove(
					root_dir_path, 0,
					local_fsys_operators::file_types_options_type::general_directory);
		yggr_test_assert(chk_size == cmp_children_dir_size + 1);
		yggr_test_assert(!local_fsys_operators::is_exists(root_dir_path));

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_remove(
						root_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)));
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!local_fsys_operators::is_exists(root_dir_path));
		}
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	{
		local_fsys_operators::error_code_type ec;

		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		chk_size = local_fsys_operators::recursive_remove(not_exists_root_dir, ec);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(
						not_exists_root_dir, ec,
						local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(chk_size == 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(root_dir_path, ec);
		yggr_test_assert(chk_size == cmp_size);
		yggr_test_assert(!local_fsys_operators::is_exists(root_dir_path));
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		chk_size = local_fsys_operators::recursive_remove(root_dir_path, ec, 0, 0);
		yggr_test_assert(chk_size == 0);
		yggr_test_assert(cmp_size == local_fsys_operators::recursive_count_children(root_dir_path) + 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(
					root_dir_path, ec, 0,
					local_fsys_operators::file_types_options_type::general_link);
		yggr_test_assert(chk_size == cmp_children_symlink_size);
		yggr_test_assert(cmp_size - cmp_children_symlink_size == local_fsys_operators::recursive_count_children(root_dir_path) + 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(
					root_dir_path, ec, 0,
					local_fsys_operators::file_types_options_type::all_file);
		yggr_test_assert(chk_size == cmp_children_file_size);
		yggr_test_assert(cmp_size - (cmp_children_file_size + cmp_children_symlink_size)
				== local_fsys_operators::recursive_count_children(root_dir_path) + 1);
		yggr_test_assert(!ec);

		chk_size = local_fsys_operators::recursive_remove(
					root_dir_path, ec, 0,
					local_fsys_operators::file_types_options_type::general_directory);
		yggr_test_assert(chk_size == cmp_children_dir_size + 1);
		yggr_test_assert(!local_fsys_operators::is_exists(root_dir_path));
		yggr_test_assert(!ec);

		bchk = create_test_base(test_dir_path);
		yggr_test_assert(bchk);

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_remove(
						root_dir_path,
						boost::bind(&rm_notify, _1, _2, _3, _4, _5, _6, boost::ref(pd)),
						ec);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!local_fsys_operators::is_exists(root_dir_path));
			yggr_test_assert(!ec);
		}
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	std::cout << "--------------test_recursive_remove_dir succeed-------------" << std::endl;
}

void handler_for_each(const local_fsys_operators::directory_entry_type& entry,
						boost::uintmax_t current_count,
						local_fsys_operators::error_code_type* current_ec,
						boost::progress_display& pd)
{
	pd += current_count - pd.count();
}

void test_for_each(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_for_each";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type not_exists_root_dir = (test_dir_path / "not_exists_dir/rmable_root_dir");


	remove_test_base(test_dir_path);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::for_each(
						not_exists_root_dir,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)));
			yggr_test_assert(chk_size == 0);
		}

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)));
			yggr_test_assert(chk_size == cmp_size);
		}

		{
			boost::progress_display pd(cmp_children_file_size);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						local_fsys_operators::file_types_options_type::all_file);
			yggr_test_assert(chk_size == cmp_children_file_size);
		}

		{
			boost::progress_display pd(cmp_children_symlink_size);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == cmp_children_symlink_size);
		}

		{
			boost::progress_display pd(cmp_children_dir_size + 1);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						local_fsys_operators::file_types_options_type::general_directory);
			yggr_test_assert(chk_size == cmp_children_dir_size + 1);
		}

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	{
		local_fsys_operators::error_code_type ec;

		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::for_each(
						not_exists_root_dir,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec);
			yggr_test_assert(chk_size == 0);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_children_file_size);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec,
						local_fsys_operators::file_types_options_type::all_file);
			yggr_test_assert(chk_size == cmp_children_file_size);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_children_symlink_size);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec,
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == cmp_children_symlink_size);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_children_dir_size + 1);
			chk_size = local_fsys_operators::for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec,
						local_fsys_operators::file_types_options_type::general_directory);
			yggr_test_assert(chk_size == cmp_children_dir_size + 1);
			yggr_test_assert(!ec);
		}

	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	std::cout << "--------------test_for_each succeed-------------" << std::endl;
}

void test_recursive_for_each(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_recursive_for_each";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type not_exists_root_dir = (test_dir_path / "not_exists_dir/rmable_root_dir");


	remove_test_base(test_dir_path);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_for_each(
						not_exists_root_dir,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)));
			yggr_test_assert(chk_size == 0);
		}

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)));
			yggr_test_assert(chk_size == cmp_size);
		}

		{
			boost::progress_display pd(cmp_children_file_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						local_fsys_operators::file_types_options_type::all_file);
			yggr_test_assert(chk_size == cmp_children_file_size);
		}

		{
			boost::progress_display pd(cmp_children_symlink_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == cmp_children_symlink_size);
		}

		{
			boost::progress_display pd(cmp_children_dir_size + 1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						local_fsys_operators::file_types_options_type::general_directory);
			yggr_test_assert(chk_size == cmp_children_dir_size + 1);
		}

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	{
		local_fsys_operators::error_code_type ec;

		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_for_each(
						not_exists_root_dir,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec);
			yggr_test_assert(chk_size == 0);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec);
			yggr_test_assert(chk_size == cmp_size);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_children_file_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec,
						local_fsys_operators::file_types_options_type::all_file);
			yggr_test_assert(chk_size == cmp_children_file_size);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_children_symlink_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec,
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == cmp_children_symlink_size);
			yggr_test_assert(!ec);
		}

		{
			boost::progress_display pd(cmp_children_dir_size + 1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						ec,
						local_fsys_operators::file_types_options_type::general_directory);
			yggr_test_assert(chk_size == cmp_children_dir_size + 1);
			yggr_test_assert(!ec);
		}

	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	std::cout << "--------------test_recursive_for_each succeed-------------" << std::endl;
}

void test_recursive_file_size(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_recursive_file_size";
	local_fsys_operators::path_type test_dir_path2 = "../test_out/file_system/test_recursive_file_size2";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type root_dir_path2 = (test_dir_path2 / "root_dir");

	local_fsys_operators::path_type not_exists_root_dir = (test_dir_path / "not_exists_dir/rmable_root_dir");


	remove_test_base(test_dir_path);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	bchk = create_test_base2(test_dir_path2);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::file_size_type fsize = 0;

		fsize = local_fsys_operators::recursive_file_size(root_dir_path / "file0");
		yggr_test_assert(fsize == 1024);

#if TEST_OF_ADMIN
		fsize = local_fsys_operators::recursive_file_size(root_dir_path / "symlink0");
		yggr_test_assert(fsize == local_fsys_operators::npos);
#endif // TEST_OF_ADMIN

		fsize = local_fsys_operators::recursive_file_size(root_dir_path);
		yggr_test_assert(fsize == 71680);

		fsize = local_fsys_operators::recursive_file_size(root_dir_path2);
		yggr_test_assert(fsize == 71680);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	bchk = remove_test_base(test_dir_path2);
	yggr_test_assert(bchk);

	std::cout << "--------------test_recursive_file_size succeed-------------" << std::endl;
}

bool handler_no_push_filter(local_fsys_operators::recursive_directory_iterator_type dir_iter,
								const local_fsys_operators::directory_entry_type& entry,
								boost::uintmax_t current_count,
								local_fsys_operators::error_code_type* current_ec,
								const std::string filter_name)
{
	return entry.path().filename() == filter_name;
}

void recursive_for_each_no_push_filter(void)
{
	bool bchk = false;

	local_fsys_operators::path_type test_dir_path = "../test_out/file_system/test_recursive_for_each_custom";

	local_fsys_operators::path_type root_dir_path = (test_dir_path / "root_dir");
	local_fsys_operators::path_type not_exists_root_dir = (test_dir_path / "not_exists_dir/rmable_root_dir");


	remove_test_base(test_dir_path);

	bchk = create_test_base(test_dir_path);
	yggr_test_assert(bchk);

	try
	{
		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		{
			boost::progress_display pd(0);
			chk_size = local_fsys_operators::recursive_for_each(
						not_exists_root_dir,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")) );
			yggr_test_assert(chk_size == 0);
		}

		{
			boost::progress_display pd(1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("root_dir")) );
			yggr_test_assert(chk_size == 1);
		}

#if (TEST_OF_ADMIN == 1)

		{
			boost::progress_display pd(cmp_size - 4);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")) );
			yggr_test_assert(chk_size == cmp_size - 4);
		}

#else
		{
			boost::progress_display pd(cmp_size - 3);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")) );
			yggr_test_assert(chk_size == cmp_size - 3);
		}
#endif // TEST_OF_ADMIN

		{
			boost::progress_display pd(cmp_children_file_size - 2);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						local_fsys_operators::file_types_options_type::all_file);
			yggr_test_assert(chk_size == cmp_children_file_size - 2);
		}


#if (TEST_OF_ADMIN == 1)

		{
			boost::progress_display pd(cmp_children_symlink_size - 1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == cmp_children_symlink_size - 1);
		}

#else
		{
			boost::progress_display pd(cmp_children_symlink_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == 0);
		}
#endif // TEST_OF_ADMIN

		{
			// +1 root_dir, -1 dir0/sub_dir
			boost::progress_display pd(cmp_children_dir_size + 1 - 1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						local_fsys_operators::file_types_options_type::general_directory);
			yggr_test_assert(chk_size == cmp_children_dir_size + 1 - 1);
		}

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	{
		local_fsys_operators::error_code_type ec;

		local_fsys_operators::size_type cmp_children_size = local_fsys_operators::recursive_count_children(root_dir_path);
		local_fsys_operators::size_type cmp_children_dir_size = local_fsys_operators::recursive_count_children_directory(root_dir_path);
		local_fsys_operators::size_type cmp_children_file_size = local_fsys_operators::recursive_count_children_file(root_dir_path);
		local_fsys_operators::size_type cmp_children_symlink_size = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		local_fsys_operators::size_type cmp_size = cmp_children_size + 1;

		local_fsys_operators::size_type chk_size = 0;

		{
			boost::progress_display pd(0);
			chk_size = local_fsys_operators::recursive_for_each(
						not_exists_root_dir,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec);
			yggr_test_assert(chk_size == 0);
			yggr_test_assert(!ec);
		}


		{
			boost::progress_display pd(1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("root_dir")),
						ec);
			yggr_test_assert(chk_size == 1);
		}

#if (TEST_OF_ADMIN == 1)
		{
			boost::progress_display pd(cmp_size - 4);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec);
			yggr_test_assert(chk_size == cmp_size - 4);
			yggr_test_assert(!ec);
		}
#else
		{
			boost::progress_display pd(cmp_size - 3);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec);
			yggr_test_assert(chk_size == cmp_size - 3);
			yggr_test_assert(!ec);
		}
#endif // #if (TEST_OF_ADMIN == 1)

		{
			boost::progress_display pd(cmp_children_file_size - 2);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec,
						local_fsys_operators::file_types_options_type::all_file);
			yggr_test_assert(chk_size == cmp_children_file_size - 2);
			yggr_test_assert(!ec);
		}

#if (TEST_OF_ADMIN == 1)
		{
			boost::progress_display pd(cmp_children_symlink_size - 1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec,
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == cmp_children_symlink_size - 1);
			yggr_test_assert(!ec);
		}
#else
		{
			boost::progress_display pd(cmp_children_symlink_size);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec,
						local_fsys_operators::file_types_options_type::general_link);
			yggr_test_assert(chk_size == 0);
			yggr_test_assert(!ec);
		}
#endif // #if (TEST_OF_ADMIN == 1)

		{
			// +1 root_dir, -1 dir0/sub_dir
			boost::progress_display pd(cmp_children_dir_size + 1 - 1);
			chk_size = local_fsys_operators::recursive_for_each(
						root_dir_path,
						boost::bind(&handler_for_each, _1, _2, _3, boost::ref(pd)),
						boost::bind(&handler_no_push_filter, _1, _2, _3, _4, std::string("dir0")),
						ec,
						local_fsys_operators::file_types_options_type::general_directory);
			yggr_test_assert(chk_size == cmp_children_dir_size + 1 - 1);
			yggr_test_assert(!ec);
		}
	}

	bchk = remove_test_base(test_dir_path);
	yggr_test_assert(bchk);

	std::cout << "--------------recursive_for_each_no_push_filter succeed-------------" << std::endl;
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_recursive_copy_not_dir();
	test_recursive_copy_dir();

	test_recursive_move_not_dir();
	test_recursive_move_dir();

	test_recursive_remove_not_dir();
	test_recursive_remove_dir();

	test_for_each();
	test_recursive_for_each();

	test_recursive_file_size();

	recursive_for_each_no_push_filter();

	wait_any_key(argc, argv);
	return 0;
}
