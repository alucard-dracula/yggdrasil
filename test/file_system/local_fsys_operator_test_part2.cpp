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

#if defined(YGGR_AT_WINDOWS)
#	define TEST_OF_ADMIN 0 // if you not want to test of admin set the macro to 0
#else
#	define TEST_OF_ADMIN 1
#endif // YGGR_AT_WINDOWS

#if defined(__ANDROID__) || defined(ANDROID)
#	define ANDROID_ROOTED 0 // if you android device is rooted, set ANDROID_ROOTED 1
#endif // #if defined(__ANDROID__) || defined(ANDROID)

#define TEST_STL_VERSION 1

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

void test_file_size(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_file_size");

	local_fsys_operators::path_type file_path_buf = (root_dir_path / "file_path_buf");
	local_fsys_operators::path_type file_path_1024 = (root_dir_path / "1024.txt");
	local_fsys_operators::path_type file_path_2048 = (root_dir_path / "2048.txt");
	local_fsys_operators::path_type file_path_not_exists = (root_dir_path / "not_exists.txt");

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
		local_fsys_operators::create_file_of_binary(file_path_1024, 1024);
		local_fsys_operators::create_file_of_binary(file_path_2048, 2048);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::file_size_type fsize = 0;

		fsize = local_fsys_operators::file_size(file_path_1024);
		yggr_test_assert(fsize == 1024);

		fsize = local_fsys_operators::file_size(file_path_2048);
		yggr_test_assert(fsize == 2048);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::file_size_type fsize = 0;
		fsize = local_fsys_operators::file_size(file_path_not_exists);
		yggr_test_assert(false);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	{
		local_fsys_operators::file_size_type fsize = 0;
		local_fsys_operators::error_code_type ec;

		{
			local_fsys_operators::ifstream_type ifs;
			ifs.open(YGGR_FILESYSTEM_C_STR(file_path_1024), std::ios_base::in | std::ios_base::binary);
			fsize = local_fsys_operators::file_size(ifs);
			yggr_test_assert(fsize == 1024);

			ifs.seekg(100, std::ios_base::beg);
			fsize = local_fsys_operators::file_size_of_begin_to_current(ifs);
			yggr_test_assert(fsize == 100);

			fsize = local_fsys_operators::file_size_of_current_to_end(ifs);
			yggr_test_assert(fsize == 924); // 1024 - 100

			ifs.close();
		}

		fsize = local_fsys_operators::file_size(file_path_1024, ec);
		yggr_test_assert(fsize == 1024);
		yggr_test_assert(!ec);

		fsize = local_fsys_operators::file_size(file_path_2048, ec);
		yggr_test_assert(fsize == 2048);
		yggr_test_assert(!ec);

		fsize = local_fsys_operators::file_size(file_path_not_exists, ec);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();
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

	std::cout << "--------------test_file_size succeed-------------" << std::endl;
}

void test_count_children_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_count_children_file");

	local_fsys_operators::path_type dir_path1 = (root_dir_path / "dir1");
	local_fsys_operators::path_type dir_path2 = (root_dir_path / "dir2");

	local_fsys_operators::path_type file_path1 = (root_dir_path / "file1.txt");
	local_fsys_operators::path_type file_path2 = (root_dir_path / "file2.txt");

	local_fsys_operators::path_type file_path11 = (dir_path1 / "file1.txt");
	local_fsys_operators::path_type file_path12 = (dir_path1 / "file2.txt");

	local_fsys_operators::path_type file_path21 = (dir_path2 / "file1.txt");
	local_fsys_operators::path_type file_path22 = (dir_path2 / "file2.txt");

	// 6 file

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
		local_fsys_operators::create_directories(dir_path1);
		local_fsys_operators::create_directories(dir_path2);

		local_fsys_operators::create_file_of_binary(file_path1, 1024);
		local_fsys_operators::create_file_of_binary(file_path2, 1024);

		local_fsys_operators::create_file_of_binary(file_path11, 1024);
		local_fsys_operators::create_file_of_binary(file_path12, 1024);

		local_fsys_operators::create_file_of_binary(file_path21, 1024);
		local_fsys_operators::create_file_of_binary(file_path22, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type count = 0;

		count = local_fsys_operators::count_children_file(root_dir_path);
		yggr_test_assert(count == 2);

		count = local_fsys_operators::recursive_count_children_file(root_dir_path);
		yggr_test_assert(count == 6);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type count = 0;
		local_fsys_operators::error_code_type ec;

		count = local_fsys_operators::count_children_file(root_dir_path, ec);
		yggr_test_assert(count == 2);
		yggr_test_assert(!ec);

		count = local_fsys_operators::recursive_count_children_file(root_dir_path, ec);
		yggr_test_assert(count == 6);
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
	}

	std::cout << "--------------test_count_children_file succeed-------------" << std::endl;
}


void test_create_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_create_file");

	local_fsys_operators::path_type bin_file_path_1024 = (root_dir_path / "bin_1024.txt");
	local_fsys_operators::path_type bin_file_path_2048 = (root_dir_path / "bin_2048.txt");
	local_fsys_operators::path_type bin_file_path_exists = (root_dir_path / "bin_exists.txt");
	local_fsys_operators::path_type bin_file_path_not_exists = (root_dir_path / "bin_not_exists/bin_file.txt");

	local_fsys_operators::path_type txt_file_path_1024 = (root_dir_path / "txt_1024.txt");
	local_fsys_operators::path_type txt_file_path_2048 = (root_dir_path / "txt_2048.txt");
	local_fsys_operators::path_type txt_file_path_exists = (root_dir_path / "txt_exists.txt");
	local_fsys_operators::path_type txt_file_path_not_exists = (root_dir_path / "txt_not_exists/txt_file.txt");

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
		local_fsys_operators::create_file_of_binary(bin_file_path_exists, 1024);
		local_fsys_operators::create_file_of_binary(txt_file_path_exists, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::file_size_type fsize = 0;

		// binary
		fsize = local_fsys_operators::create_file_of_binary(bin_file_path_1024, 1024);
		yggr_test_assert(fsize == 1024);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_1024));

		fsize = local_fsys_operators::create_file_of_binary(bin_file_path_2048, 2048);
		yggr_test_assert(fsize == 2048);
		yggr_test_assert(2048 == local_fsys_operators::file_size(bin_file_path_2048));

		fsize = local_fsys_operators::create_file_of_binary(bin_file_path_exists, 4096);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_exists));

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_exists, 4096,
					local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(fsize == 1024);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_exists));

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_exists, 4096,
					local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(fsize == 4096);

		{
			custom_unique_type my_handler(bin_file_path_exists);
			fsize = local_fsys_operators::create_file_of_binary(
						bin_file_path_exists, 4096,
						my_handler,
						local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(fsize == 4096);
		}

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_exists, 4096,
					local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(fsize == 4096);
		yggr_test_assert(4096 == local_fsys_operators::file_size(bin_file_path_exists));

		// text

		typedef local_fsys_operators::char_type file_char_type;

		fsize = local_fsys_operators::create_file_of_text(txt_file_path_1024, 1024);
		yggr_test_assert(fsize == 1024 * sizeof(file_char_type));
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_1024));

		fsize = local_fsys_operators::create_file_of_text(txt_file_path_2048, 2048);
		yggr_test_assert(fsize == 2048 * sizeof(file_char_type));
		yggr_test_assert(2048 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_2048));

		fsize = local_fsys_operators::create_file_of_text(txt_file_path_exists, 4096);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_exists));

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_exists, 4096,
					local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(fsize == 1024 * sizeof(file_char_type));
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_exists));

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_exists, 4096,
					local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(fsize == 4096 * sizeof(file_char_type));

		{
			custom_unique_type my_handler(txt_file_path_exists);
			fsize = local_fsys_operators::create_file_of_text(
						txt_file_path_exists, 4096,
						my_handler,
						local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(fsize == 4096 * sizeof(file_char_type));
		}

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_exists, 4096,
					local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(fsize == 4096 * sizeof(file_char_type));
		yggr_test_assert(4096 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_exists));
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
		local_fsys_operators::create_file_of_binary(bin_file_path_exists, 1024);
		local_fsys_operators::create_file_of_binary(txt_file_path_exists, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::file_size_type fsize = 0;
		local_fsys_operators::error_code_type ec;

		// binary
		fsize = local_fsys_operators::create_file_of_binary(bin_file_path_1024, 1024, ec);
		yggr_test_assert(fsize == 1024);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_1024));

		fsize = local_fsys_operators::create_file_of_binary(bin_file_path_2048, 2048, ec);
		yggr_test_assert(fsize == 2048);
		yggr_test_assert(!ec);
		yggr_test_assert(2048 == local_fsys_operators::file_size(bin_file_path_2048));

		fsize = local_fsys_operators::create_file_of_binary(bin_file_path_exists, 4096, ec);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_exists));

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_exists, 4096, ec,
					local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(fsize == 1024);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_exists));

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_exists, 4096, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(fsize == 4096);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_handler(bin_file_path_exists);
			fsize = local_fsys_operators::create_file_of_binary(
						bin_file_path_exists, 4096,
						my_handler, ec,
						local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(fsize == 4096);
			yggr_test_assert(!ec);
		}

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_exists, 4096, ec,
					local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(fsize == 4096);
		yggr_test_assert(!ec);
		yggr_test_assert(4096 == local_fsys_operators::file_size(bin_file_path_exists));


		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_not_exists, 1024, ec);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(!ec);

		fsize = local_fsys_operators::create_file_of_binary(
					bin_file_path_not_exists, 1024, ec,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(fsize == 1024);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(bin_file_path_not_exists));

		// text

		typedef local_fsys_operators::char_type file_char_type;

		fsize = local_fsys_operators::create_file_of_text(txt_file_path_1024, 1024, ec);
		yggr_test_assert(fsize == 1024 * sizeof(file_char_type));
		yggr_test_assert(!ec);
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_1024));

		fsize = local_fsys_operators::create_file_of_text(txt_file_path_2048, 2048, ec);
		yggr_test_assert(fsize == 2048 * sizeof(file_char_type));
		yggr_test_assert(!ec);
		yggr_test_assert(2048 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_2048));

		fsize = local_fsys_operators::create_file_of_text(txt_file_path_exists, 4096, ec);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_exists));

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_exists, 4096, ec,
					local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(fsize == 1024 * sizeof(file_char_type));
		yggr_test_assert(!ec);
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_exists));

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_exists, 4096, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(fsize == 4096 * sizeof(file_char_type));
		yggr_test_assert(!ec);

		{
			custom_unique_type my_handler(txt_file_path_exists);
			fsize = local_fsys_operators::create_file_of_text(
						txt_file_path_exists, 4096,
						my_handler, ec,
						local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(fsize == 4096 * sizeof(file_char_type));
			yggr_test_assert(!ec);
		}

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_exists, 4096, ec,
					local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(fsize == 4096 * sizeof(file_char_type));
		yggr_test_assert(4096 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_exists));
		yggr_test_assert(!ec);

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_not_exists, 1024, ec);
		yggr_test_assert(fsize == local_fsys_operators::npos);
		yggr_test_assert(!ec);

		fsize = local_fsys_operators::create_file_of_text(
					txt_file_path_not_exists, 1024, ec,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(fsize == 1024 * sizeof(file_char_type));
		yggr_test_assert(!ec);
		yggr_test_assert(1024 * sizeof(file_char_type) == local_fsys_operators::file_size(txt_file_path_not_exists));
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

	std::cout << "--------------test_create_file succeed-------------" << std::endl;
}

void test_write_and_read_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_write_file");

	local_fsys_operators::path_type bin_file_path_1 = (root_dir_path / "bin_file1.txt");
	local_fsys_operators::path_type bin_file_path_2 = (root_dir_path / "bin_file2.txt");
	local_fsys_operators::path_type bin_file_path_exists = (root_dir_path / "bin_file_exists.txt");
	local_fsys_operators::path_type bin_dir_path_not_exists = (root_dir_path / "bin_not_exists/");
	local_fsys_operators::path_type bin_file_path_not_exists = (root_dir_path / "bin_not_exists/bin_file.txt");

	local_fsys_operators::path_type txt_file_path_1 = (root_dir_path / "txt_file1.txt");
	local_fsys_operators::path_type txt_file_path_2 = (root_dir_path / "txt_file2.txt");
	local_fsys_operators::path_type txt_file_path_exists = (root_dir_path / "txt_file_exists.txt");
	local_fsys_operators::path_type txt_dir_path_not_exists = (root_dir_path / "txt_not_exists/");
	local_fsys_operators::path_type txt_file_path_not_exists = (root_dir_path / "txt_not_exists/txt_file.txt");

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
		local_fsys_operators::create_file_of_binary(bin_file_path_exists, 1024);
		local_fsys_operators::create_file_of_binary(txt_file_path_exists, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		// binary
		{
			local_fsys_operators::file_size_type fsize = 0;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			int rarr[] = {0, 0, 0, 0};
			yggr::vector<int> rvt;
			yggr::list<int> rlist;
			yggr::multiset<int> rset;

			{
				local_fsys_operators::ofstream_type ofs;
				ofs.open(YGGR_FILESYSTEM_C_STR(bin_file_path_1), std::ios::out | std::ios::binary);

				fsize = local_fsys_operators::write_file_of_binary(ofs, &arr[0], sizeof(arr));
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::write_file_of_binary(ofs, arr);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::write_file_of_binary(ofs, vt);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::write_file_of_binary(ofs, list);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::write_file_of_binary(ofs, set);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::write_file_of_binary(ofs, arr + 0, arr + 4);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::write_file_of_binary(ofs, vt.begin(), vt.end());
				yggr_test_assert(fsize == sizeof(arr));

				// not recomment, because inner operator is copy
				fsize = local_fsys_operators::write_file_of_binary(ofs, vt.rbegin(), vt.rend());
				yggr_test_assert(fsize == sizeof(arr));

				// not recomment, because inner operator is copy
				fsize = local_fsys_operators::write_file_of_binary(ofs, list.begin(), list.end());
				yggr_test_assert(fsize == sizeof(arr));

				// not recomment, because inner operator is copy
				fsize = local_fsys_operators::write_file_of_binary(ofs, set.begin(), set.end());
				yggr_test_assert(fsize == sizeof(arr));

				ofs.close();

				yggr_test_assert(sizeof(arr) * 10 == local_fsys_operators::file_size(bin_file_path_1));
			}

			{
				local_fsys_operators::ifstream_type ifs;
				ifs.open(YGGR_FILESYSTEM_C_STR(bin_file_path_1), std::ios::in | std::ios::binary);

				fsize = local_fsys_operators::read_file_of_binary(ifs, &rarr[0], sizeof(rarr));
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));
				memset(&rarr[0], 0, sizeof(rarr));

				fsize = local_fsys_operators::read_file_of_binary(ifs, rarr, sizeof(rarr));
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

				fsize = local_fsys_operators::read_file_of_binary(ifs, rvt, sizeof(arr));
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

				fsize = local_fsys_operators::read_file_of_binary(ifs, rlist, sizeof(arr));
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

				fsize = local_fsys_operators::read_file_of_binary(ifs, rset, sizeof(arr));
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));


				fsize = local_fsys_operators::read_file_of_binary(ifs, rarr, rarr + 4);
				yggr_test_assert(fsize == sizeof(arr));
				memset(&rarr[0], 0, sizeof(rarr));

				yggr_test_assert(rvt.size() == 4);
				std::fill(rvt.begin(), rvt.end(), 0);
				fsize = local_fsys_operators::read_file_of_binary(ifs, rvt.begin(), rvt.end());
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

				// not recomment, because inner operator is copy
				yggr_test_assert(rvt.size() == 4);
				std::fill(rvt.begin(), rvt.end(), 0);
				fsize = local_fsys_operators::read_file_of_binary(ifs, rvt.rbegin(), rvt.rend());
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

				// not recomment, because inner operator is copy
				yggr_test_assert(rlist.size() == 4);
				std::fill(rlist.begin(), rlist.end(), 0);
				fsize = local_fsys_operators::read_file_of_binary(ifs, rlist.begin(), rlist.end());
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

				// not recomment, because inner operator is copy
				yggr_test_assert(rset.size() == 4);
				std::fill(rset.begin(), rset.end(), 0);
				fsize = local_fsys_operators::read_file_of_binary(ifs, rset.begin(), rset.end());
				yggr_test_assert(fsize == sizeof(arr));
				yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));


				yggr_test_assert(static_cast<std::size_t>(ifs.tellg()) == sizeof(arr) * 10);
				ifs.ignore(ifs.tellg(), 1);
				yggr_test_assert(ifs.eof());

				ifs.close();

				memset(&rarr[0], 0, sizeof(rarr));
				rvt.clear();
				rlist.clear();
				rset.clear();
			}

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, &arr[0], sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(0 != memcmp(&rarr[0], &arr, sizeof(arr)));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, &rarr[0], sizeof(rarr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(0 == memcmp(&rarr[0], &arr, sizeof(arr)));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, arr);
			yggr_test_assert(fsize == sizeof(arr));

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr, sizeof(arr)));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rarr, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(0 == memcmp(&rarr[0], &arr, sizeof(arr)));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, vt);
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rvt.empty());
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rvt, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, list);
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rlist.empty());
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rlist, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, set);
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rset.empty());
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rset, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, arr + 0, arr + 4);
			yggr_test_assert(fsize == sizeof(arr));

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr, sizeof(arr)));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rarr, rarr + 4);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(0 == memcmp(&rarr[0], &arr, sizeof(arr)));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, vt.begin(), vt.end());
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rvt.size() == 4);
			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rvt.begin(), rvt.end());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, vt.rbegin(), vt.rend());
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rvt.size() == 4);
			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rvt.rbegin(), rvt.rend());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, list.begin(), list.end());
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rlist.size() == 4);
			std::fill(rlist.begin(), rlist.end(), 0);
			yggr_test_assert(!std::equal(rlist.begin(), rlist.end(), list.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rlist.begin(), rlist.end());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, set.begin(), set.end());
			yggr_test_assert(fsize == sizeof(arr));

			yggr_test_assert(rset.size() == 4);
			std::fill(rset.begin(), rset.end(), 0);
			yggr_test_assert(!std::equal(rset.begin(), rset.end(), set.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rset.begin(), rset.end());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr));
			yggr_test_assert(fsize == local_fsys_operators::npos);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr),
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, arr);
			yggr_test_assert(fsize == local_fsys_operators::npos);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, arr,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end());
			yggr_test_assert(fsize == local_fsys_operators::npos);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end(),
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));

		}

		// text
		{
			typedef yggr::vector<int>::iterator vt_iter_type;
			typedef yggr::vector<int>::reverse_iterator vt_riter_type;
			typedef yggr::list<int>::iterator list_iter_type;
			typedef yggr::multiset<int>::iterator set_iter_type;

			bool bchk = false;
			int* chk_arr_iter;
			vt_iter_type chk_vt_iter;
			vt_riter_type chk_vt_riter;
			list_iter_type chk_list_iter;
			set_iter_type chk_set_iter;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			int rarr[] = {0, 0, 0, 0};
			yggr::vector<int> rvt(rarr, rarr + 4);
			yggr::list<int> rlist(rarr, rarr + 4);
			yggr::multiset<int> rset(rarr, rarr + 4);

			{
				local_fsys_operators::ofstream_type ofs;
				ofs.open(YGGR_FILESYSTEM_C_STR(txt_file_path_1), std::ios::out);

				bchk = local_fsys_operators::write_file_of_text(ofs, arr[0]);
				yggr_test_assert(bchk);

				chk_arr_iter = local_fsys_operators::write_file_of_text(ofs, arr + 0, arr + 4);
				yggr_test_assert(chk_arr_iter == arr + 4);

				chk_vt_iter = local_fsys_operators::write_file_of_text(ofs, vt.begin(), vt.end());
				yggr_test_assert(chk_vt_iter == vt.end());

				chk_vt_riter = local_fsys_operators::write_file_of_text(ofs, vt.rbegin(), vt.rend());
				yggr_test_assert(chk_vt_riter == vt.rend());

				chk_list_iter = local_fsys_operators::write_file_of_text(ofs, list.begin(), list.end());
				yggr_test_assert(chk_list_iter == list.end());

				chk_set_iter = local_fsys_operators::write_file_of_text(ofs, set.begin(), set.end());
				yggr_test_assert(chk_set_iter == set.end());

				ofs.close();
			}

			{
				local_fsys_operators::ifstream_type ifs;
				ifs.open(YGGR_FILESYSTEM_C_STR(txt_file_path_1), std::ios::in);

				bchk = local_fsys_operators::read_file_of_text(ifs, rarr[0]);
				yggr_test_assert(bchk);
				yggr_test_assert(arr[0] == rarr[0]);

				chk_arr_iter = local_fsys_operators::read_file_of_text(ifs, rarr, rarr + 4);
				yggr_test_assert(chk_arr_iter == rarr + 4);
				yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

				yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
				chk_vt_iter = local_fsys_operators::read_file_of_text(ifs, rvt.begin(), rvt.end());
				yggr_test_assert(chk_vt_iter == rvt.end());
				yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

				std::fill(rvt.begin(), rvt.end(), 0);
				yggr_test_assert(!std::equal(rvt.rbegin(), rvt.rend(), vt.rbegin()));
				chk_vt_riter = local_fsys_operators::read_file_of_text(ifs, rvt.rbegin(), rvt.rend());
				yggr_test_assert(chk_vt_riter == rvt.rend());
				yggr_test_assert(std::equal(rvt.rbegin(), rvt.rend(), vt.rbegin()));

				yggr_test_assert(!std::equal(rlist.rbegin(), rlist.rend(), list.rbegin()));
				chk_list_iter = local_fsys_operators::read_file_of_text(ifs, rlist.begin(), rlist.end());
				yggr_test_assert(chk_list_iter == rlist.end());
				yggr_test_assert(std::equal(rlist.rbegin(), rlist.rend(), list.rbegin()));

				yggr_test_assert(!std::equal(rset.rbegin(), rset.rend(), set.rbegin()));
				chk_set_iter = local_fsys_operators::read_file_of_text(ifs, rset.begin(), rset.end());
				yggr_test_assert(chk_set_iter == rset.end());
				yggr_test_assert(std::equal(rset.rbegin(), rset.rend(), set.rbegin()));

				ifs.close();
			}

			{
				local_fsys_operators::ofstream_type ofs;
				ofs.open(YGGR_FILESYSTEM_C_STR(txt_file_path_1), std::ios::out);

				bchk = local_fsys_operators::write_file_of_text(ofs, arr[0], custom_spr);
				yggr_test_assert(bchk);

				chk_arr_iter = local_fsys_operators::write_file_of_text(ofs, arr + 0, arr + 4, custom_spr);
				yggr_test_assert(chk_arr_iter == arr + 4);

				chk_vt_iter = local_fsys_operators::write_file_of_text(ofs, vt.begin(), vt.end(), custom_spr);
				yggr_test_assert(chk_vt_iter == vt.end());

				chk_vt_riter = local_fsys_operators::write_file_of_text(ofs, vt.rbegin(), vt.rend(), custom_spr);
				yggr_test_assert(chk_vt_riter == vt.rend());

				chk_list_iter = local_fsys_operators::write_file_of_text(ofs, list.begin(), list.end(), custom_spr);
				yggr_test_assert(chk_list_iter == list.end());

				chk_set_iter = local_fsys_operators::write_file_of_text(ofs, set.begin(), set.end(), custom_spr);
				yggr_test_assert(chk_set_iter == set.end());

				ofs.close();
			}

			{
				local_fsys_operators::ifstream_type ifs;
				ifs.open(YGGR_FILESYSTEM_C_STR(txt_file_path_1), std::ios::in);

				memset(&rarr[0], 0, sizeof(rarr));
				bchk = local_fsys_operators::read_file_of_text(ifs, rarr[0], custom_spr);
				yggr_test_assert(bchk);
				yggr_test_assert(arr[0] == rarr[0]);

				memset(&rarr[0], 0, sizeof(rarr));
				chk_arr_iter = local_fsys_operators::read_file_of_text(ifs, rarr, rarr + 4, custom_spr);
				yggr_test_assert(chk_arr_iter == rarr + 4);
				yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

				std::fill(rvt.begin(), rvt.end(), 0);
				yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
				chk_vt_iter = local_fsys_operators::read_file_of_text(ifs, rvt.begin(), rvt.end(), custom_spr);
				yggr_test_assert(chk_vt_iter == rvt.end());
				yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

				std::fill(rvt.begin(), rvt.end(), 0);
				yggr_test_assert(!std::equal(rvt.rbegin(), rvt.rend(), vt.rbegin()));
				chk_vt_riter = local_fsys_operators::read_file_of_text(ifs, rvt.rbegin(), rvt.rend(), custom_spr);
				yggr_test_assert(chk_vt_riter == rvt.rend());
				yggr_test_assert(std::equal(rvt.rbegin(), rvt.rend(), vt.rbegin()));

				std::fill(rlist.begin(), rlist.end(), 0);
				yggr_test_assert(!std::equal(rlist.rbegin(), rlist.rend(), list.rbegin()));
				chk_list_iter = local_fsys_operators::read_file_of_text(ifs, rlist.begin(), rlist.end(), custom_spr);
				yggr_test_assert(chk_list_iter == rlist.end());
				yggr_test_assert(std::equal(rlist.rbegin(), rlist.rend(), list.rbegin()));

				std::fill(rset.begin(), rset.end(), 0);
				yggr_test_assert(!std::equal(rset.rbegin(), rset.rend(), set.rbegin()));
				chk_set_iter = local_fsys_operators::read_file_of_text(ifs, rset.begin(), rset.end(), custom_spr);
				yggr_test_assert(chk_set_iter == rset.end());
				yggr_test_assert(std::equal(rset.rbegin(), rset.rend(), set.rbegin()));

				ifs.close();

				memset(&rarr[0], 0, sizeof(rarr));
				std::fill(rvt.begin(), rvt.end(), 0);
				std::fill(rlist.begin(), rlist.end(), 0);
				std::fill(rset.begin(), rset.end(), 0);
			}

			bchk = local_fsys_operators::write_file_of_text(txt_file_path_2, arr[0]);
			yggr_test_assert(bchk);

			bchk = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr[0]);
			yggr_test_assert(bchk);
			yggr_test_assert(rarr[0] == arr[0]);

			chk_arr_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, arr + 0, arr + 4);
			yggr_test_assert(chk_arr_iter == arr + 4);

			yggr_test_assert(0 != memcmp(&rarr[0], &arr[0], sizeof(arr)));
			chk_arr_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr, rarr + 4);
			yggr_test_assert(chk_arr_iter == rarr + 4);
			yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

			chk_vt_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.begin(), vt.end());
			yggr_test_assert(chk_vt_iter == vt.end());

			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.begin(), rvt.end());
			yggr_test_assert(chk_vt_iter == rvt.end());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_vt_riter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend());
			yggr_test_assert(chk_vt_riter == vt.rend());

			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_riter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.rbegin(), rvt.rend());
			yggr_test_assert(chk_vt_riter == rvt.rend());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_list_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, list.begin(), list.end());
			yggr_test_assert(chk_list_iter == list.end());

			std::fill(rlist.begin(), rlist.end(), 0);
			yggr_test_assert(!std::equal(rlist.begin(), rlist.end(), list.begin()));
			chk_list_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rlist.begin(), rlist.end());
			yggr_test_assert(chk_list_iter == rlist.end());
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			chk_set_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, set.begin(), set.end());
			yggr_test_assert(chk_set_iter == set.end());

			std::fill(rset.begin(), rset.end(), 0);
			yggr_test_assert(!std::equal(rset.begin(), rset.end(), set.begin()));
			chk_set_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rset.begin(), rset.end());
			yggr_test_assert(chk_set_iter == rset.end());
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			bchk = local_fsys_operators::write_file_of_text(txt_file_path_2, arr[0], custom_spr);
			yggr_test_assert(bchk);

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr[0], sizeof(arr)));
			bchk = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr[0], custom_spr);
			yggr_test_assert(bchk);
			yggr_test_assert(rarr[0] == arr[0]);

			chk_arr_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, arr + 0, arr + 4, custom_spr);
			yggr_test_assert(chk_arr_iter == arr + 4);

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr[0], sizeof(arr)));
			chk_arr_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr, rarr + 4, custom_spr);
			yggr_test_assert(chk_arr_iter == rarr + 4);
			yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

			chk_vt_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.begin(), vt.end(), custom_spr);
			yggr_test_assert(chk_vt_iter == vt.end());

			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.begin(), rvt.end(), custom_spr);
			yggr_test_assert(chk_vt_iter == rvt.end());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_vt_riter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend(), custom_spr);
			yggr_test_assert(chk_vt_riter == vt.rend());

			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_riter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.rbegin(), rvt.rend(), custom_spr);
			yggr_test_assert(chk_vt_riter == rvt.rend());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_list_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, list.begin(), list.end(), custom_spr);
			yggr_test_assert(chk_list_iter == list.end());

			std::fill(rlist.begin(), rlist.end(), 0);
			yggr_test_assert(!std::equal(rlist.begin(), rlist.end(), list.begin()));
			chk_list_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rlist.begin(), rlist.end(), custom_spr);
			yggr_test_assert(chk_list_iter == rlist.end());
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			chk_set_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, set.begin(), set.end(), custom_spr);
			yggr_test_assert(chk_set_iter == set.end());

			std::fill(rset.begin(), rset.end(), 0);
			yggr_test_assert(!std::equal(rset.begin(), rset.end(), set.begin()));
			chk_set_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rset.begin(), rset.end(), custom_spr);
			yggr_test_assert(chk_set_iter == rset.end());
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			bchk = local_fsys_operators::write_file_of_text(
						txt_file_path_not_exists, arr[0]);
			yggr_test_assert(!bchk);

			bchk = local_fsys_operators::write_file_of_text(
						txt_file_path_not_exists, arr[0],
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(bchk);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_arr_iter = local_fsys_operators::write_file_of_text(
								txt_file_path_not_exists, arr + 0, arr + 4);
			yggr_test_assert(chk_arr_iter == arr);

			chk_arr_iter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, arr + 0, arr + 4,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_arr_iter == arr + 4);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_iter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end());
			yggr_test_assert(chk_vt_iter == vt.begin());

			chk_vt_iter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end(),
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_iter == vt.end());

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_riter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend());
			yggr_test_assert(chk_vt_riter == vt.rbegin());

			chk_vt_riter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend(),
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_riter == vt.rend());
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_file_of_binary(bin_file_path_exists, 1024);
		local_fsys_operators::create_file_of_binary(txt_file_path_exists, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		// binary
		{
			local_fsys_operators::file_size_type fsize = 0;
			local_fsys_operators::error_code_type ec;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			int rarr[] = {0, 0, 0, 0};
			yggr::vector<int> rvt;
			yggr::list<int> rlist;
			yggr::multiset<int> rset;

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, &arr[0], sizeof(arr), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(0 != memcmp(&rarr[0], &arr, sizeof(arr)));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, &rarr[0], sizeof(rarr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(0 == memcmp(&rarr[0], &arr, sizeof(arr)));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, arr, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr, sizeof(arr)));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rarr, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(0 == memcmp(&rarr[0], &arr, sizeof(arr)));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, vt, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rvt.empty());
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rvt, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, list, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rlist.empty());
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rlist, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, set, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rset.empty());
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rset, sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, arr + 0, arr + 4, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr, sizeof(arr)));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rarr, rarr + 4);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(0 == memcmp(&rarr[0], &arr, sizeof(arr)));

			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, vt.begin(), vt.end(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rvt.size() == 4);
			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rvt.begin(), rvt.end());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, vt.rbegin(), vt.rend(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rvt.size() == 4);
			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rvt.rbegin(), rvt.rend());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, list.begin(), list.end(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rlist.size() == 4);
			std::fill(rlist.begin(), rlist.end(), 0);
			yggr_test_assert(!std::equal(rlist.begin(), rlist.end(), list.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rlist.begin(), rlist.end());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::write_file_of_binary(bin_file_path_2, set.begin(), set.end(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			yggr_test_assert(rset.size() == 4);
			std::fill(rset.begin(), rset.end(), 0);
			yggr_test_assert(!std::equal(rset.begin(), rset.end(), set.begin()));
			fsize = local_fsys_operators::read_file_of_binary(bin_file_path_2, rset.begin(), rset.end());
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr), ec);
			yggr_test_assert(fsize == local_fsys_operators::npos);
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr), ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, arr, ec);
			yggr_test_assert(fsize == local_fsys_operators::npos);
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, arr, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end(), ec);
			yggr_test_assert(fsize == local_fsys_operators::npos);
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::write_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end(), ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);
		}

		// text
		{
			typedef yggr::vector<int>::iterator vt_iter_type;
			typedef yggr::vector<int>::reverse_iterator vt_riter_type;
			typedef yggr::list<int>::iterator list_iter_type;
			typedef yggr::multiset<int>::iterator set_iter_type;

			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			int* chk_arr_iter;
			vt_iter_type chk_vt_iter;
			vt_riter_type chk_vt_riter;
			list_iter_type chk_list_iter;
			set_iter_type chk_set_iter;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			int rarr[] = {0, 0, 0, 0};
			yggr::vector<int> rvt(rarr, rarr + 4);
			yggr::list<int> rlist(rarr, rarr + 4);
			yggr::multiset<int> rset(rarr, rarr + 4);

			bchk = local_fsys_operators::write_file_of_text(txt_file_path_2, arr[0], ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);

			bchk = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr[0]);
			yggr_test_assert(bchk);
			yggr_test_assert(rarr[0] == arr[0]);

			chk_arr_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, arr + 0, arr + 4, ec);
			yggr_test_assert(chk_arr_iter == arr + 4);
			yggr_test_assert(!ec);

			yggr_test_assert(0 != memcmp(&rarr[0], &arr[0], sizeof(arr)));
			chk_arr_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr, rarr + 4);
			yggr_test_assert(chk_arr_iter == rarr + 4);
			yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

			chk_vt_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.begin(), vt.end(), ec);
			yggr_test_assert(chk_vt_iter == vt.end());
			yggr_test_assert(!ec);

			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.begin(), rvt.end());
			yggr_test_assert(chk_vt_iter == rvt.end());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_vt_riter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend(), ec);
			yggr_test_assert(chk_vt_riter == vt.rend());
			yggr_test_assert(!ec);

			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_riter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.rbegin(), rvt.rend());
			yggr_test_assert(chk_vt_riter == rvt.rend());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_list_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, list.begin(), list.end(), ec);
			yggr_test_assert(chk_list_iter == list.end());
			yggr_test_assert(!ec);

			std::fill(rlist.begin(), rlist.end(), 0);
			yggr_test_assert(!std::equal(rlist.begin(), rlist.end(), list.begin()));
			chk_list_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rlist.begin(), rlist.end());
			yggr_test_assert(chk_list_iter == rlist.end());
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			chk_set_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, set.begin(), set.end(), ec);
			yggr_test_assert(chk_set_iter == set.end());
			yggr_test_assert(!ec);

			std::fill(rset.begin(), rset.end(), 0);
			yggr_test_assert(!std::equal(rset.begin(), rset.end(), set.begin()));
			chk_set_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rset.begin(), rset.end());
			yggr_test_assert(chk_set_iter == rset.end());
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			bchk = local_fsys_operators::write_file_of_text(txt_file_path_2, arr[0], custom_spr, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr[0], sizeof(arr)));
			bchk = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr[0], custom_spr);
			yggr_test_assert(bchk);
			yggr_test_assert(rarr[0] == arr[0]);

			chk_arr_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, arr + 0, arr + 4, custom_spr, ec);
			yggr_test_assert(chk_arr_iter == arr + 4);
			yggr_test_assert(!ec);

			memset(&rarr[0], 0, sizeof(rarr));
			yggr_test_assert(0 != memcmp(&rarr[0], &arr[0], sizeof(arr)));
			chk_arr_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rarr, rarr + 4, custom_spr);
			yggr_test_assert(chk_arr_iter == rarr + 4);
			yggr_test_assert(0 == memcmp(&rarr[0], &arr[0], sizeof(arr)));

			chk_vt_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.begin(), vt.end(), custom_spr, ec);
			yggr_test_assert(chk_vt_iter == vt.end());
			yggr_test_assert(!ec);

			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.begin(), rvt.end(), custom_spr);
			yggr_test_assert(chk_vt_iter == rvt.end());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_vt_riter = local_fsys_operators::write_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend(), custom_spr, ec);
			yggr_test_assert(chk_vt_riter == vt.rend());
			yggr_test_assert(!ec);

			std::fill(rvt.begin(), rvt.end(), 0);
			yggr_test_assert(!std::equal(rvt.begin(), rvt.end(), vt.begin()));
			chk_vt_riter = local_fsys_operators::read_file_of_text(txt_file_path_2, rvt.rbegin(), rvt.rend(), custom_spr);
			yggr_test_assert(chk_vt_riter == rvt.rend());
			yggr_test_assert(std::equal(rvt.begin(), rvt.end(), vt.begin()));

			chk_list_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, list.begin(), list.end(), custom_spr, ec);
			yggr_test_assert(chk_list_iter == list.end());
			yggr_test_assert(!ec);

			std::fill(rlist.begin(), rlist.end(), 0);
			yggr_test_assert(!std::equal(rlist.begin(), rlist.end(), list.begin()));
			chk_list_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rlist.begin(), rlist.end(), custom_spr);
			yggr_test_assert(chk_list_iter == rlist.end());
			yggr_test_assert(std::equal(rlist.begin(), rlist.end(), list.begin()));

			chk_set_iter = local_fsys_operators::write_file_of_text(txt_file_path_2, set.begin(), set.end(), custom_spr, ec);
			yggr_test_assert(chk_set_iter == set.end());
			yggr_test_assert(!ec);

			std::fill(rset.begin(), rset.end(), 0);
			yggr_test_assert(!std::equal(rset.begin(), rset.end(), set.begin()));
			chk_set_iter = local_fsys_operators::read_file_of_text(txt_file_path_2, rset.begin(), rset.end(), custom_spr);
			yggr_test_assert(chk_set_iter == rset.end());
			yggr_test_assert(std::equal(rset.begin(), rset.end(), set.begin()));

			bchk = local_fsys_operators::write_file_of_text(
						txt_file_path_not_exists, arr[0], ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);

			bchk = local_fsys_operators::write_file_of_text(
						txt_file_path_not_exists, arr[0], ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_arr_iter = local_fsys_operators::write_file_of_text(
								txt_file_path_not_exists, arr + 0, arr + 4, ec);
			yggr_test_assert(chk_arr_iter == arr);
			yggr_test_assert(!ec);

			chk_arr_iter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, arr + 0, arr + 4, ec,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_arr_iter == arr + 4);
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_iter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end(), ec);
			yggr_test_assert(chk_vt_iter == vt.begin());
			yggr_test_assert(!ec);

			chk_vt_iter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end(), ec,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_iter == vt.end());
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_riter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend(), ec);
			yggr_test_assert(chk_vt_riter == vt.rbegin());
			yggr_test_assert(!ec);

			chk_vt_riter = local_fsys_operators::write_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend(), ec,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_riter == vt.rend());
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
	}

	std::cout << "--------------test_write_and_read_file succeed-------------" << std::endl;
}

void test_append_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_append_file");

	local_fsys_operators::path_type bin_file_path_1 = (root_dir_path / "bin_file1.txt");
	local_fsys_operators::path_type bin_file_path_2 = (root_dir_path / "bin_file2.txt");
	local_fsys_operators::path_type bin_file_path_exists = (root_dir_path / "bin_file_exists.txt");
	local_fsys_operators::path_type bin_dir_path_not_exists = (root_dir_path / "bin_not_exists/");
	local_fsys_operators::path_type bin_file_path_not_exists = (root_dir_path / "bin_not_exists/bin_file.txt");

	local_fsys_operators::path_type txt_file_path_1 = (root_dir_path / "txt_file1.txt");
	local_fsys_operators::path_type txt_file_path_2 = (root_dir_path / "txt_file2.txt");
	local_fsys_operators::path_type txt_file_path_exists = (root_dir_path / "txt_file_exists.txt");
	local_fsys_operators::path_type txt_dir_path_not_exists = (root_dir_path / "txt_not_exists/");
	local_fsys_operators::path_type txt_file_path_not_exists = (root_dir_path / "txt_not_exists/txt_file.txt");

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
		local_fsys_operators::create_file_of_binary(bin_file_path_exists, 1024);
		local_fsys_operators::create_file_of_binary(txt_file_path_exists, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		// binary
		{
			local_fsys_operators::file_size_type fsize = 0;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			{
				local_fsys_operators::ofstream_type ofs;
				ofs.open(YGGR_FILESYSTEM_C_STR(bin_file_path_1),
							std::ios::out | std::ios::binary | std::ios::app);

				fsize = local_fsys_operators::append_file_of_binary(ofs, &arr[0], sizeof(arr));
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::append_file_of_binary(ofs, arr);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::append_file_of_binary(ofs, vt);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::append_file_of_binary(ofs, list);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::append_file_of_binary(ofs, set);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::append_file_of_binary(ofs, arr + 0, arr + 4);
				yggr_test_assert(fsize == sizeof(arr));

				fsize = local_fsys_operators::append_file_of_binary(ofs, vt.begin(), vt.end());
				yggr_test_assert(fsize == sizeof(arr));

				// not recomment, because inner operator is copy
				fsize = local_fsys_operators::append_file_of_binary(ofs, vt.rbegin(), vt.rend());
				yggr_test_assert(fsize == sizeof(arr));

				// not recomment, because inner operator is copy
				fsize = local_fsys_operators::append_file_of_binary(ofs, list.begin(), list.end());
				yggr_test_assert(fsize == sizeof(arr));

				// not recomment, because inner operator is copy
				fsize = local_fsys_operators::append_file_of_binary(ofs, set.begin(), set.end());
				yggr_test_assert(fsize == sizeof(arr));

				ofs.close();

				yggr_test_assert(sizeof(arr) * 10 == local_fsys_operators::file_size(bin_file_path_1));
			}

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, &arr[0], sizeof(arr));
			yggr_test_assert(fsize == sizeof(arr));

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, arr);
			yggr_test_assert(fsize == sizeof(arr));

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, vt);
			yggr_test_assert(fsize == sizeof(arr));

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, list);
			yggr_test_assert(fsize == sizeof(arr));

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, set);
			yggr_test_assert(fsize == sizeof(arr));

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, arr + 0, arr + 4);
			yggr_test_assert(fsize == sizeof(arr));

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, vt.begin(), vt.end());
			yggr_test_assert(fsize == sizeof(arr));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, vt.rbegin(), vt.rend());
			yggr_test_assert(fsize == sizeof(arr));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, list.begin(), list.end());
			yggr_test_assert(fsize == sizeof(arr));

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, set.begin(), set.end());
			yggr_test_assert(fsize == sizeof(arr));


			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr));
			yggr_test_assert(fsize == local_fsys_operators::npos);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr),
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, arr);
			yggr_test_assert(fsize == local_fsys_operators::npos);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, arr,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end());
			yggr_test_assert(fsize == local_fsys_operators::npos);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end(),
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));

		}

		// text
		{
			typedef yggr::vector<int>::iterator vt_iter_type;
			typedef yggr::vector<int>::reverse_iterator vt_riter_type;
			typedef yggr::list<int>::iterator list_iter_type;
			typedef yggr::multiset<int>::iterator set_iter_type;

			bool bchk = false;
			int* chk_arr_iter;
			vt_iter_type chk_vt_iter;
			vt_riter_type chk_vt_riter;
			list_iter_type chk_list_iter;
			set_iter_type chk_set_iter;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			{
				local_fsys_operators::ofstream_type ofs;
				ofs.open(YGGR_FILESYSTEM_C_STR(txt_file_path_1), std::ios::out);

				bchk = local_fsys_operators::write_file_of_text(ofs, arr[0]);
				yggr_test_assert(bchk);

				chk_arr_iter = local_fsys_operators::write_file_of_text(ofs, arr + 0, arr + 4);
				yggr_test_assert(chk_arr_iter == arr + 4);

				chk_vt_iter = local_fsys_operators::write_file_of_text(ofs, vt.begin(), vt.end());
				yggr_test_assert(chk_vt_iter == vt.end());

				chk_vt_riter = local_fsys_operators::write_file_of_text(ofs, vt.rbegin(), vt.rend());
				yggr_test_assert(chk_vt_riter == vt.rend());

				chk_list_iter = local_fsys_operators::write_file_of_text(ofs, list.begin(), list.end());
				yggr_test_assert(chk_list_iter == list.end());

				chk_set_iter = local_fsys_operators::write_file_of_text(ofs, set.begin(), set.end());
				yggr_test_assert(chk_set_iter == set.end());

				ofs.close();
			}

			{
				local_fsys_operators::ofstream_type ofs;
				ofs.open(YGGR_FILESYSTEM_C_STR(txt_file_path_1), std::ios::out);

				bchk = local_fsys_operators::append_file_of_text(ofs, arr[0], custom_spr);
				yggr_test_assert(bchk);

				chk_arr_iter = local_fsys_operators::append_file_of_text(ofs, arr + 0, arr + 4, custom_spr);
				yggr_test_assert(chk_arr_iter == arr + 4);

				chk_vt_iter = local_fsys_operators::append_file_of_text(ofs, vt.begin(), vt.end(), custom_spr);
				yggr_test_assert(chk_vt_iter == vt.end());

				chk_vt_riter = local_fsys_operators::append_file_of_text(ofs, vt.rbegin(), vt.rend(), custom_spr);
				yggr_test_assert(chk_vt_riter == vt.rend());

				chk_list_iter = local_fsys_operators::append_file_of_text(ofs, list.begin(), list.end(), custom_spr);
				yggr_test_assert(chk_list_iter == list.end());

				chk_set_iter = local_fsys_operators::append_file_of_text(ofs, set.begin(), set.end(), custom_spr);
				yggr_test_assert(chk_set_iter == set.end());

				ofs.close();
			}

			bchk = local_fsys_operators::append_file_of_text(txt_file_path_2, arr[0]);
			yggr_test_assert(bchk);

			chk_arr_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, arr + 0, arr + 4);
			yggr_test_assert(chk_arr_iter == arr + 4);

			chk_vt_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.begin(), vt.end());
			yggr_test_assert(chk_vt_iter == vt.end());

			chk_vt_riter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend());
			yggr_test_assert(chk_vt_riter == vt.rend());

			chk_list_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, list.begin(), list.end());
			yggr_test_assert(chk_list_iter == list.end());

			chk_set_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, set.begin(), set.end());
			yggr_test_assert(chk_set_iter == set.end());

			bchk = local_fsys_operators::append_file_of_text(txt_file_path_2, arr[0], custom_spr);
			yggr_test_assert(bchk);

			chk_arr_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, arr + 0, arr + 4, custom_spr);
			yggr_test_assert(chk_arr_iter == arr + 4);

			chk_vt_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.begin(), vt.end(), custom_spr);
			yggr_test_assert(chk_vt_iter == vt.end());

			chk_vt_riter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend(), custom_spr);
			yggr_test_assert(chk_vt_riter == vt.rend());

			chk_list_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, list.begin(), list.end(), custom_spr);
			yggr_test_assert(chk_list_iter == list.end());

			chk_set_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, set.begin(), set.end(), custom_spr);
			yggr_test_assert(chk_set_iter == set.end());


			bchk = local_fsys_operators::append_file_of_text(
						txt_file_path_not_exists, arr[0]);
			yggr_test_assert(!bchk);

			bchk = local_fsys_operators::append_file_of_text(
						txt_file_path_not_exists, arr[0],
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(bchk);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_arr_iter = local_fsys_operators::append_file_of_text(
								txt_file_path_not_exists, arr + 0, arr + 4);
			yggr_test_assert(chk_arr_iter == arr);

			chk_arr_iter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, arr + 0, arr + 4,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_arr_iter == arr + 4);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_iter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end());
			yggr_test_assert(chk_vt_iter == vt.begin());

			chk_vt_iter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end(),
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_iter == vt.end());

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_riter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend());
			yggr_test_assert(chk_vt_riter == vt.rbegin());

			chk_vt_riter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend(),
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_riter == vt.rend());
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_file_of_binary(bin_file_path_exists, 1024);
		local_fsys_operators::create_file_of_binary(txt_file_path_exists, 1024);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		// binary
		{
			local_fsys_operators::file_size_type fsize = 0;
			local_fsys_operators::error_code_type ec;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, &arr[0], sizeof(arr), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, arr, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, vt, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, list, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, set, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, arr + 0, arr + 4, ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, vt.begin(), vt.end(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, vt.rbegin(), vt.rend(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, list.begin(), list.end(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			// not recomment, because inner operator is copy
			fsize = local_fsys_operators::append_file_of_binary(bin_file_path_2, set.begin(), set.end(), ec);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);


			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr), ec);
			yggr_test_assert(fsize == local_fsys_operators::npos);
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, &arr[0], sizeof(arr), ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, arr, ec);
			yggr_test_assert(fsize == local_fsys_operators::npos);
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, arr, ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(bin_dir_path_not_exists);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end(), ec);
			yggr_test_assert(fsize == local_fsys_operators::npos);
			yggr_test_assert(!ec);

			fsize = local_fsys_operators::append_file_of_binary(
						bin_file_path_not_exists, vt.begin(), vt.end(), ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(fsize == sizeof(arr));
			yggr_test_assert(!ec);

		}

		// text
		{
			typedef yggr::vector<int>::iterator vt_iter_type;
			typedef yggr::vector<int>::reverse_iterator vt_riter_type;
			typedef yggr::list<int>::iterator list_iter_type;
			typedef yggr::multiset<int>::iterator set_iter_type;

			bool bchk = false;
			local_fsys_operators::error_code_type ec;

			int* chk_arr_iter;
			vt_iter_type chk_vt_iter;
			vt_riter_type chk_vt_riter;
			list_iter_type chk_list_iter;
			set_iter_type chk_set_iter;

			int arr[] = {2, 3, 1, 4};
			yggr::vector<int> vt(arr + 0, arr + 4);
			yggr::list<int> list(arr + 0, arr + 4);
			yggr::multiset<int> set(arr + 0, arr + 4);

			bchk = local_fsys_operators::append_file_of_text(txt_file_path_2, arr[0], ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);

			chk_arr_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, arr + 0, arr + 4, ec);
			yggr_test_assert(chk_arr_iter == arr + 4);
			yggr_test_assert(!ec);

			chk_vt_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.begin(), vt.end(), ec);
			yggr_test_assert(chk_vt_iter == vt.end());
			yggr_test_assert(!ec);

			chk_vt_riter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend(), ec);
			yggr_test_assert(chk_vt_riter == vt.rend());
			yggr_test_assert(!ec);

			chk_list_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, list.begin(), list.end(), ec);
			yggr_test_assert(chk_list_iter == list.end());
			yggr_test_assert(!ec);

			chk_set_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, set.begin(), set.end(), ec);
			yggr_test_assert(chk_set_iter == set.end());
			yggr_test_assert(!ec);

			bchk = local_fsys_operators::append_file_of_text(txt_file_path_2, arr[0], custom_spr, ec);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);

			chk_arr_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, arr + 0, arr + 4, custom_spr, ec);
			yggr_test_assert(chk_arr_iter == arr + 4);
			yggr_test_assert(!ec);

			chk_vt_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.begin(), vt.end(), custom_spr, ec);
			yggr_test_assert(chk_vt_iter == vt.end());
			yggr_test_assert(!ec);

			chk_vt_riter = local_fsys_operators::append_file_of_text(txt_file_path_2, vt.rbegin(), vt.rend(), custom_spr, ec);
			yggr_test_assert(chk_vt_riter == vt.rend());
			yggr_test_assert(!ec);

			chk_list_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, list.begin(), list.end(), custom_spr, ec);
			yggr_test_assert(chk_list_iter == list.end());
			yggr_test_assert(!ec);

			chk_set_iter = local_fsys_operators::append_file_of_text(txt_file_path_2, set.begin(), set.end(), custom_spr, ec);
			yggr_test_assert(chk_set_iter == set.end());
			yggr_test_assert(!ec);

			bchk = local_fsys_operators::append_file_of_text(
						txt_file_path_not_exists, arr[0], ec);
			yggr_test_assert(!bchk);
			yggr_test_assert(!ec);

			bchk = local_fsys_operators::append_file_of_text(
						txt_file_path_not_exists, arr[0], ec,
						local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_arr_iter = local_fsys_operators::append_file_of_text(
								txt_file_path_not_exists, arr + 0, arr + 4, ec);
			yggr_test_assert(chk_arr_iter == arr);
			yggr_test_assert(!ec);

			chk_arr_iter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, arr + 0, arr + 4, ec,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_arr_iter == arr + 4);
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_iter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end(), ec);
			yggr_test_assert(chk_vt_iter == vt.begin());
			yggr_test_assert(!ec);

			chk_vt_iter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.begin(), vt.end(), ec,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_iter == vt.end());
			yggr_test_assert(!ec);

			local_fsys_operators::remove_all(txt_dir_path_not_exists);

			chk_vt_riter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend(), ec);
			yggr_test_assert(chk_vt_riter == vt.rbegin());
			yggr_test_assert(!ec);

			chk_vt_riter = local_fsys_operators::append_file_of_text(
							txt_file_path_not_exists, vt.rbegin(), vt.rend(), ec,
							local_fsys_operators::options_type::dir_create_if_not_exists);
			yggr_test_assert(chk_vt_riter == vt.rend());
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
	}

	std::cout << "--------------test_append_file succeed-------------" << std::endl;
}

void test_copy_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_copy_file");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type cped_file_path = (root_dir_path / "cped_file.txt");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type exists_file_path = (root_dir_path / "exists_file.txt");
	local_fsys_operators::path_type not_exists_file_path = (root_dir_path / "not_exists/file.txt");


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

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);
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

		bchk = local_fsys_operators::copy_file(file_path, cped_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(cped_file_path));

		bchk = local_fsys_operators::copy_file(file_path, exists_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy_file(file_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::copy_file(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy_file(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::copy_file(
					file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
		}

		bchk = local_fsys_operators::copy_file(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy_file(
				file_path, exists_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_file(
				file_path, exists_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_dir_path));

		try
		{
			bchk = local_fsys_operators::copy_file(file_path, not_exists_file_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_file(
				file_path, not_exists_file_path,
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);
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

		bchk = local_fsys_operators::copy_file(file_path, cped_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(cped_file_path));

		bchk = local_fsys_operators::copy_file(file_path, exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy_file(file_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::copy_file(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy_file(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::copy_file(
					file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		bchk = local_fsys_operators::copy_file(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy_file(
				file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy_file(
				file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_dir_path));

		bchk = local_fsys_operators::copy_file(file_path, not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		ec.clear();

		bchk = local_fsys_operators::copy_file(
				file_path, not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
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
	}

	std::cout << "--------------test_copy_file succeed-------------" << std::endl;
}

void test_rename_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_rename_file");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type renamed_file_path = (root_dir_path / "renamed_file.txt");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type exists_file_path = (root_dir_path / "exists_file.txt");
	local_fsys_operators::path_type exists_sub_file_path = (root_dir_path / "exists_dir/file.txt");
	local_fsys_operators::path_type not_exists_file_path = (root_dir_path / "not_exists/file.txt");

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

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);
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

		bchk = local_fsys_operators::rename_file(file_path, renamed_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));

		bchk = local_fsys_operators::rename_file(renamed_file_path, exists_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(renamed_file_path));

		bchk = local_fsys_operators::rename_file(renamed_file_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(renamed_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::rename_file(renamed_file_path, exists_sub_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));

		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(renamed_file_path));

		{
			local_fsys_operators::create_file_of_binary(renamed_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::rename_file(
					renamed_file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(renamed_file_path));
		}

		local_fsys_operators::create_file_of_binary(renamed_file_path, 1024);
		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_file_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));

		local_fsys_operators::create_file_of_binary(renamed_file_path, 1024);
		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));

		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_dir_path));
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
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);
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

		bchk = local_fsys_operators::rename_file(file_path, renamed_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_file(renamed_file_path, exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(renamed_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_file(renamed_file_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(renamed_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_file(renamed_file_path, exists_sub_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_file(
					renamed_file_path, exists_file_path, ec,
					local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(renamed_file_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_file_of_binary(renamed_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::rename_file(
					renamed_file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(renamed_file_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_file_of_binary(renamed_file_path, 1024);
		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(renamed_file_path, 1024);
		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(renamed_file_path));

		bchk = local_fsys_operators::rename_file(
				renamed_file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_dir_path));
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
	}

	std::cout << "--------------test_rename_file succeed-------------" << std::endl;
}

void test_move_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_move_file");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type moved_file_path = (root_dir_path / "moved_file.txt");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type exists_file_path = (root_dir_path / "exists_file.txt");
	local_fsys_operators::path_type exists_sub_file_path = (root_dir_path / "exists_dir/file.txt");
	local_fsys_operators::path_type not_exists_file_path = (root_dir_path / "not_exists/file.txt");

#if defined(YGGR_AT_WINDOWS)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("f:/yggr_diff_vol_test_out/test_move_file");
#elif defined(YGGR_AT_LINUX)
#	if (defined(__ANDROID__) || defined(ANDROID))
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/storage/C5B8-A1D4/yggr_diff_vol_test_out/diff_test_move_file/");
#	else
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/run/media/alucard/win10_games/yggr_diff_vol_test_out/diff_test_move_file/");
#	endif // #	if defined(__ANDROID__) || defined(ANDROID)
#elif defined(YGGR_AT_DARWIN)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/Volumes/win10_games/yggr_diff_vol_test_out/");
#else
#	error "!!!append this platfrom diff vol path!!!"
#endif // YGGR_AT_WINDOWS

	local_fsys_operators::path_type diff_vol_moved_file_path = (diff_vol_root_dir_path / "moved_file.txt");
	local_fsys_operators::path_type diff_vol_not_exists_file_path = (diff_vol_root_dir_path / "not_exists/moved_file.txt");

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
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);
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

		bchk = local_fsys_operators::move_file(file_path, moved_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));

		bchk = local_fsys_operators::move_file(moved_file_path, exists_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));

		bchk = local_fsys_operators::move_file(moved_file_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::move_file(moved_file_path, exists_sub_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_sub_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));

		{
			local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::move_file(
					moved_file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		}

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));

		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_dir_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		try
		{
			bchk = local_fsys_operators::move_file(moved_file_path, not_exists_file_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_file(
				moved_file_path, not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, diff_vol_moved_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_vol_moved_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		try
		{
			bchk = local_fsys_operators::move_file(moved_file_path, diff_vol_not_exists_file_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_file(
				moved_file_path, diff_vol_not_exists_file_path,
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
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(exists_file_path, 2048);
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

		bchk = local_fsys_operators::move_file(file_path, moved_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_file(moved_file_path, exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_file(moved_file_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_file(moved_file_path, exists_sub_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_sub_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::move_file(
					moved_file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_file(
				moved_file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);

		bchk = local_fsys_operators::move_file(moved_file_path, not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		ec.clear();

		bchk = local_fsys_operators::move_file(
				moved_file_path, not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move_file(
				moved_file_path, diff_vol_moved_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_vol_moved_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);

		bchk = local_fsys_operators::move_file(moved_file_path, diff_vol_not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		ec.clear();

		bchk = local_fsys_operators::move_file(
				moved_file_path, diff_vol_not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);
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

	std::cout << "--------------test_move_file succeed-------------" << std::endl;
}

void test_remove_file(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_remove_file");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type not_exists_file_path = (root_dir_path / "not_exists_file.txt");


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
		local_fsys_operators::create_file_of_binary(file_path, 1024);
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

		bchk = local_fsys_operators::remove_file(dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove_file(not_exists_file_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove_file(
					not_exists_file_path,
					local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::remove_file(file_path);
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_file_of_binary(file_path, 1024);
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

		bchk = local_fsys_operators::remove_file(dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove_file(not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove_file(
					not_exists_file_path, ec,
					local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove_file(file_path, ec);
		yggr_test_assert(bchk);
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
	}

	std::cout << "--------------test_remove_file succeed-------------" << std::endl;
}


#if TEST_OF_ADMIN

void test_count_children_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_count_children_symlink");

	local_fsys_operators::path_type dir_path1 = (root_dir_path / "dir1");
	local_fsys_operators::path_type dir_path2 = (root_dir_path / "dir2");

	local_fsys_operators::path_type dir_path1_in_sym = ("dir1");
	local_fsys_operators::path_type dir_path2_in_sym = ("dir2");

	local_fsys_operators::path_type symlink_path1 = (root_dir_path / "symlink1.txt");
	local_fsys_operators::path_type symlink_path2 = (root_dir_path / "symlink2.txt");

	local_fsys_operators::path_type dir_path1_in_sub_sym = ("../dir1");
	local_fsys_operators::path_type dir_path2_in_sub_sym = ("../dir2");

	local_fsys_operators::path_type symlink_path11 = (dir_path1 / "symlink1.txt");
	local_fsys_operators::path_type symlink_path12 = (dir_path1 / "symlink2.txt");

	local_fsys_operators::path_type symlink_path21 = (dir_path2 / "symlink1.txt");
	local_fsys_operators::path_type symlink_path22 = (dir_path2 / "symlink2.txt");

	// 6 symlink

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
		local_fsys_operators::create_directories(dir_path1);
		local_fsys_operators::create_directories(dir_path2);

		local_fsys_operators::create_symlink(dir_path1_in_sym, symlink_path1);
		local_fsys_operators::create_symlink(dir_path2_in_sym, symlink_path2);

		local_fsys_operators::create_symlink(dir_path1_in_sub_sym, symlink_path11);
		local_fsys_operators::create_symlink(dir_path2_in_sub_sym, symlink_path12);

		local_fsys_operators::create_symlink(dir_path1_in_sub_sym, symlink_path21);
		local_fsys_operators::create_symlink(dir_path2_in_sub_sym, symlink_path22);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::size_type count = 0;

		count = local_fsys_operators::count_children_symlink(root_dir_path);
		yggr_test_assert(count == 2);

		count = local_fsys_operators::recursive_count_children_symlink(root_dir_path);
		yggr_test_assert(count == 6);

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	{
		local_fsys_operators::size_type count = 0;
		local_fsys_operators::error_code_type ec;

		count = local_fsys_operators::count_children_symlink(root_dir_path, ec);
		yggr_test_assert(count == 2);
		yggr_test_assert(!ec);

		count = local_fsys_operators::recursive_count_children_symlink(root_dir_path, ec);
		yggr_test_assert(count == 6);
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
	}

	std::cout << "--------------test_count_children_symlink succeed-------------" << std::endl;
}

void test_create_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_create_symlink");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type dir_path_in_sym = ("dir");
	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists/symlink");

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

		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_symlink_path,
					local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_symlink_path,
					local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::create_symlink(
						dir_path_in_sym, exists_symlink_path, my_unique_handler,
						local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
		}

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_symlink_path,
					local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_dir_path,
					local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_dir_path,
					local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);

		try
		{
			bchk = local_fsys_operators::create_symlink(dir_path_in_sym, not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::create_symlink(
				dir_path_in_sym, not_exists_symlink_path,
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_symlink_path, ec,
					local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_symlink_path, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::create_symlink(
						dir_path_in_sym, exists_symlink_path, my_unique_handler, ec,
						local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_symlink_path, ec,
					local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_dir_path, ec,
					local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_symlink(
					dir_path_in_sym, exists_dir_path, ec,
					local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_symlink(dir_path_in_sym, not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;

		bchk = local_fsys_operators::create_symlink(
				dir_path_in_sym, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
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
	}

	std::cout << "--------------test_create_symlink succeed-------------" << std::endl;
}

void test_copy_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_copy_symlink");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type dir_path_in_sym = ("dir");
	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type cped_symlink_path = (root_dir_path / "cped_symlink");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists/symlink");

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

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::copy_symlink(symlink_path, cped_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(cped_symlink_path));

		bchk = local_fsys_operators::copy_symlink(symlink_path, exists_symlink_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(symlink_path, exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::copy_symlink(
					symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
		}

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);

		try
		{
			bchk = local_fsys_operators::copy_symlink(symlink_path, not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, not_exists_symlink_path,
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::copy_symlink(symlink_path, cped_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(cped_symlink_path));

		bchk = local_fsys_operators::copy_symlink(symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(symlink_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::copy_symlink(
					symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
		}

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, exists_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);


		bchk = local_fsys_operators::copy_symlink(symlink_path, not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;

		bchk = local_fsys_operators::copy_symlink(
				symlink_path, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
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

	std::cout << "--------------test_copy_symlink succeed-------------" << std::endl;
}

void test_rename_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_rename_symlink");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");

	local_fsys_operators::path_type dir_path_in_sym = ("dir");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type rename_symlink_path = (root_dir_path / "renamed_symlink");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type exists_sub_symlink_path = (root_dir_path / "exists_dir/symlink");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists/symlink");

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

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::rename_symlink(symlink_path, rename_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));

		bchk = local_fsys_operators::rename_symlink(rename_symlink_path, exists_symlink_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(rename_symlink_path));

		bchk = local_fsys_operators::rename_symlink(rename_symlink_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::rename_symlink(rename_symlink_path, exists_sub_symlink_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(rename_symlink_path));

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, rename_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::rename_symlink(
					rename_symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(rename_symlink_path));
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, rename_symlink_path);
		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(rename_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, rename_symlink_path);
		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_symlink(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_dir_path));
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

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::rename_symlink(symlink_path, rename_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_symlink(rename_symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_symlink(rename_symlink_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_symlink(rename_symlink_path, exists_sub_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_symlink(dir_path, rename_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::rename_symlink(
					rename_symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(rename_symlink_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, rename_symlink_path);
		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(rename_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, rename_symlink_path);
		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::rename_symlink(
				rename_symlink_path, exists_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_symlink(rename_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_dir_path));
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
	}

	std::cout << "--------------test_rename_symlink succeed-------------" << std::endl;
}

void test_move_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_move_symlink");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type dir_path_in_sym = ("dir");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type moved_symlink_path = (root_dir_path / "moved_symlink");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_sub_symlink_path = (root_dir_path / "exists_dir/symlink");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists/symlink");

#if defined(YGGR_AT_WINDOWS)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("f:/yggr_diff_vol_test_out/test_move_symlink");
#elif defined(YGGR_AT_LINUX)
#	if (defined(__ANDROID__) || defined(ANDROID))
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/storage/C5B8-A1D4/yggr_diff_vol_test_out/diff_test_move_symlink/");
#	else
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/run/media/alucard/win10_games/yggr_diff_vol_test_out/diff_test_move_symlink/");
#	endif // #	if defined(__ANDROID__) || defined(ANDROID)
#elif defined(YGGR_AT_DARWIN)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/Volumes/win10_games/yggr_diff_vol_test_out/");
#else
#	error "!!!append this platfrom diff vol path!!!"
#endif // YGGR_AT_WINDOWS

	local_fsys_operators::path_type diff_vol_moved_symlink_path = (diff_vol_root_dir_path / "moved_symlink");
	local_fsys_operators::path_type diff_vol_not_exists_symlink_path = (diff_vol_root_dir_path / "not_exists/moved_symlink");

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
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::move_symlink(symlink_path, moved_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));

		bchk = local_fsys_operators::move_symlink(moved_symlink_path, exists_symlink_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));

		bchk = local_fsys_operators::move_symlink(moved_symlink_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::move_symlink(moved_symlink_path, exists_sub_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_sub_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));

		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::move_symlink(
					moved_symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_dir_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		try
		{
			bchk = local_fsys_operators::move_symlink(moved_symlink_path, not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);

		// if using android, need root
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, diff_vol_moved_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		try
		{
			bchk = local_fsys_operators::move_symlink(moved_symlink_path, diff_vol_not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		// if using android, need root
#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, diff_vol_not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)
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
		local_fsys_operators::create_directories(diff_vol_root_dir_path);

		local_fsys_operators::create_directories(dir_path);
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(dir_path_in_sym, exists_symlink_path);
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

		bchk = local_fsys_operators::move_symlink(symlink_path, moved_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_symlink(moved_symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_symlink(moved_symlink_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_symlink(moved_symlink_path, exists_sub_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_sub_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::move_symlink(
					moved_symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, exists_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(moved_symlink_path, not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, diff_vol_moved_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
#if !(defined(__ANDROID__) || defined(ANDROID)) // need root
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#else
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID))

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move_symlink(moved_symlink_path, diff_vol_not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

#if !(defined(__ANDROID__) || defined(ANDROID)) // need root
		bchk = local_fsys_operators::move_symlink(
				moved_symlink_path, diff_vol_not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);

		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#else
		try
		{
			bchk = local_fsys_operators::move_symlink(
					moved_symlink_path, diff_vol_not_exists_symlink_path,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cout << ec.message() << std::endl;
		}
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID))
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

	std::cout << "--------------test_move_symlink succeed-------------" << std::endl;
}

void test_remove_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_remove_symlink");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");

	local_fsys_operators::path_type dir_path_in_sym = ("dir");
	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists_symlink");

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

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
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

		bchk = local_fsys_operators::remove_symlink(dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove_symlink(not_exists_symlink_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove_symlink(
					not_exists_symlink_path,
					local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::remove_symlink(symlink_path);
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
		return;
	}

	try
	{
		local_fsys_operators::create_directories(root_dir_path);
		local_fsys_operators::create_directories(dir_path);

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
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

		bchk = local_fsys_operators::remove_symlink(dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove_symlink(not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove_symlink(
					not_exists_symlink_path, ec,
					local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove_symlink(symlink_path, ec);
		yggr_test_assert(bchk);
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
	}

	std::cout << "--------------test_remove_symlink succeed-------------" << std::endl;
}

void test_read_symlink(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_read_symlink");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");

	local_fsys_operators::path_type dir_path_in_sym = ("dir");
	local_fsys_operators::path_type symlink_path_in_sym = ("symlink");
	local_fsys_operators::path_type symlink_path2_in_sym = ("symlink2");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type symlink_path2 = (root_dir_path / "symlink2");
	local_fsys_operators::path_type symlink_path3 = (root_dir_path / "symlink3");

	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists_symlink");

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

		local_fsys_operators::create_file_of_binary(file_path, 1024);

		local_fsys_operators::create_symlink(dir_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(symlink_path_in_sym, symlink_path2);
		local_fsys_operators::create_symlink(symlink_path2_in_sym, symlink_path3);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

	try
	{
		local_fsys_operators::path_type rpath;
		local_fsys_operators::file_status_type rsat;
		std::pair<local_fsys_operators::path_type,
					local_fsys_operators::file_status_type> rpair;

		local_fsys_operators::file_status_type chk_sat = local_fsys_operators::path_symlink_status(dir_path);

		rpath = local_fsys_operators::read_symlink(symlink_path);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(dir_path.parent_path() / rpath, dir_path));

		rpath = local_fsys_operators::read_symlink(symlink_path2);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(symlink_path.parent_path() / rpath, symlink_path));

		rpath = local_fsys_operators::read_symlink(symlink_path3);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(symlink_path2.parent_path() / rpath, symlink_path2));

		try
		{
			rpath = local_fsys_operators::read_symlink(file_path);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			rpath.clear();
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(rpath.empty());
		yggr_test_assert(chk_sat != rsat);

		rsat = local_fsys_operators::recursive_read_symlink(rpath, symlink_path);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(rpath, dir_path));
		yggr_test_assert(chk_sat == rsat);

		rsat = local_fsys_operators::recursive_read_symlink(rpath, symlink_path2);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(rpath, dir_path));
		yggr_test_assert(chk_sat == rsat);

		rsat = local_fsys_operators::recursive_read_symlink(rpath, symlink_path3);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(rpath, dir_path));
		yggr_test_assert(chk_sat == rsat);

		rsat = local_fsys_operators::recursive_read_symlink(rpath, file_path);
		yggr_test_assert(rpath.empty());
		yggr_test_assert(chk_sat != rsat);

		rpair = local_fsys_operators::recursive_read_symlink(symlink_path);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(rpair.first, dir_path));
		yggr_test_assert(chk_sat == rpair.second);

		rpair = local_fsys_operators::recursive_read_symlink(symlink_path2);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(rpair.first, dir_path));
		yggr_test_assert(chk_sat == rpair.second);

		rpair = local_fsys_operators::recursive_read_symlink(symlink_path3);
		yggr_test_assert(local_fsys_operators::symlink_equivalent(rpair.first, dir_path));
		yggr_test_assert(chk_sat == rpair.second);
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

		local_fsys_operators::create_file_of_binary(file_path, 1024);

		local_fsys_operators::create_symlink(dir_path, symlink_path);
		local_fsys_operators::create_symlink(symlink_path, symlink_path2);
		local_fsys_operators::create_symlink(symlink_path2, symlink_path3);
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

	std::cout << "--------------test_read_symlink succeed-------------" << std::endl;
}

void test_create_hard_link(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_create_hard_link");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type diff_file_path = (root_dir_path / "diff_file.txt");

	local_fsys_operators::path_type file_path_in_sym = ("file.txt");
	local_fsys_operators::path_type diff_file_path_in_sym = ("diff_file.txt");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type diff_symlink_path = (root_dir_path / "diff_symlink");

	local_fsys_operators::path_type hard_link_file_path = (root_dir_path / "hard_link_file.txt");
	local_fsys_operators::path_type hard_link_symlink_path = (root_dir_path / "hard_link_symlink");

	local_fsys_operators::path_type exists_hard_link_file_path = (root_dir_path / "exists_hard_link_file.txt");
	local_fsys_operators::path_type diff_exists_hard_link_file_path = (root_dir_path / "diff_exists_hard_link_file.txt");

	local_fsys_operators::path_type exists_hard_link_symlink_path = (root_dir_path / "exists_hard_link_symlink.txt");
	local_fsys_operators::path_type diff_exists_hard_link_symlink_path = (root_dir_path / "diff_exists_hard_link_link.txt");

	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");
	local_fsys_operators::path_type not_exists_hard_link_path = (root_dir_path / "not_exists/hard_link.txt");


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

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(diff_file_path, 2024);

		local_fsys_operators::create_symlink(file_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(diff_file_path_in_sym, diff_symlink_path);

#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		local_fsys_operators::create_hard_link(file_path, exists_hard_link_file_path);
		local_fsys_operators::create_hard_link(diff_file_path, diff_exists_hard_link_file_path);

		local_fsys_operators::create_hard_link(symlink_path, exists_hard_link_symlink_path);
		local_fsys_operators::create_hard_link(diff_symlink_path, diff_exists_hard_link_symlink_path);
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)

		// check
#if defined(__APPLE__) || defined(__APPLE_CC__)
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(file_path));
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::equivalent(file_path, exists_hard_link_file_path));

        yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
        yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path));

        yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path));
#elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(2 == local_fsys_operators::hard_link_count(file_path));
		yggr_test_assert(2 == local_fsys_operators::hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::equivalent(file_path, exists_hard_link_file_path));

		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path));

		yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path));
		yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path));
#else
		yggr_test_assert(1 == local_fsys_operators::hard_link_count(file_path));
		//yggr_test_assert(0 == local_fsys_operators::hard_link_count(exists_hard_link_file_path)); // has throw
		//yggr_test_assert(!local_fsys_operators::equivalent(file_path, exists_hard_link_file_path)); // has throw

		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
		//yggr_test_assert(0 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path)); // has throw
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path)); // has throw

		//yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path)); // has throw
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path)); // has throw
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)

		
#if 0
		// boost version < 108000 not support using hard_link_count get symlink, 
		// but yggdrasil fixed it, so aways run #else
		try
		{
			yggr_test_assert(2 == local_fsys_operators::hard_link_count(symlink_path));
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cout << e.what() << std::endl;
		}
#else

#   if defined(__APPLE__) || defined(__APPLE_CC__)
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(symlink_path));
#	elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(2 == local_fsys_operators::hard_link_count(symlink_path));
#	else
		try
		{
			yggr_test_assert(1 == local_fsys_operators::hard_link_count(symlink_path));
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cout << e.what() << std::endl;
		}
#	endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)
#endif // 0, 1

#if defined(__APPLE__) || defined(__APPLE_CC__)
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(file_path));
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::equivalent(file_path, exists_hard_link_file_path));

        yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
        yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path));

        yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path));
#elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(symlink_path));
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_symlink_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(symlink_path, exists_hard_link_symlink_path));

		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_symlink_path));
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_symlink_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_symlink_path, diff_exists_hard_link_symlink_path));

		yggr_test_assert(!local_fsys_operators::symlink_equivalent(symlink_path, diff_exists_hard_link_symlink_path));
		yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_symlink_path, exists_hard_link_symlink_path));
#else
		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(symlink_path));
		//yggr_test_assert(0 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_symlink_path)); // has throw
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(symlink_path, exists_hard_link_symlink_path)); // has_throw

		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(diff_symlink_path));
		//yggr_test_assert(0 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_symlink_path)); // has throw
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_symlink_path, diff_exists_hard_link_symlink_path)); // has_throw

		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(symlink_path, diff_exists_hard_link_symlink_path)); // has throw
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_symlink_path, exists_hard_link_symlink_path)); // has throw
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

#if (defined(__APPLE__) || defined(__APPLE_CC__))
    try
    {
        bool bchk = false;

        bchk = local_fsys_operators::create_hard_link(file_path, hard_link_file_path);
        yggr_test_assert(bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::symlink_equivalent(file_path, hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, hard_link_file_path));

        bchk = local_fsys_operators::create_hard_link(file_path, exists_hard_link_file_path);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path,
                local_fsys_operators::options_type::hard_link_success_if_equivalent);
        yggr_test_assert(bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path,
                local_fsys_operators::options_type::hard_link_success_if_equivalent);
        yggr_test_assert(bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        // !!! if equivalent hard_link not replaceable !!!
        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path,
                local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path,
                local_fsys_operators::options_type::symlink_replace_if_exists);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_dir_path,
                local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_dir_path,
                local_fsys_operators::options_type::hard_link_replace_if_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::is_directory(exists_dir_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, diff_exists_hard_link_file_path,
                local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
        yggr_test_assert(bchk);
        yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, diff_file_path,
                local_fsys_operators::options_type::hard_link_replace_if_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(1024 == local_fsys_operators::file_size(diff_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path,
                local_fsys_operators::options_type::hard_link_rename_if_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        {
            custom_unique_type my_unique_handler(exists_hard_link_file_path);
            bchk = local_fsys_operators::create_hard_link(
                    file_path, exists_hard_link_file_path, my_unique_handler,
                    local_fsys_operators::options_type::hard_link_rename_if_exists);
            yggr_test_assert(bchk);
            yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(file_path));
            yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
            yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        }

        try
        {
            bchk = local_fsys_operators::create_hard_link(file_path, not_exists_hard_link_path);
            yggr_test_assert(false);
        }
        catch(const local_fsys_operators::exception_type& e)
        {
            bchk = false;
            std::cout << e.what() << std::endl;
        }

        yggr_test_assert(!bchk);
        yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

        bchk = local_fsys_operators::create_hard_link(
                file_path, not_exists_hard_link_path,
                local_fsys_operators::options_type::dir_create_if_not_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(10 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(10 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(10 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
    }
    catch(const local_fsys_operators::exception_type& e)
    {
        std::cout << e.what() << std::endl;
        yggr_test_assert(false);
        return;
    }
#elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
	try
	{
		bool bchk = false;

		bchk = local_fsys_operators::create_hard_link(file_path, hard_link_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(file_path, hard_link_file_path));
		yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, hard_link_file_path));

		bchk = local_fsys_operators::create_hard_link(file_path, exists_hard_link_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path,
				local_fsys_operators::options_type::hard_link_success_if_equivalent);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path,
				local_fsys_operators::options_type::hard_link_success_if_equivalent);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		// !!! if equivalent hard_link not replaceable !!!
		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path,
				local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_dir_path,
				local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_dir_path,
				local_fsys_operators::options_type::hard_link_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, diff_exists_hard_link_file_path,
				local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(diff_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, diff_file_path,
				local_fsys_operators::options_type::hard_link_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path,
				local_fsys_operators::options_type::hard_link_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		{
			custom_unique_type my_unique_handler(exists_hard_link_file_path);
			bchk = local_fsys_operators::create_hard_link(
					file_path, exists_hard_link_file_path, my_unique_handler,
					local_fsys_operators::options_type::hard_link_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(file_path));
			yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
			yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		}

		try
		{
			bchk = local_fsys_operators::create_hard_link(file_path, not_exists_hard_link_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}

		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));

		bchk = local_fsys_operators::create_hard_link(
				file_path, not_exists_hard_link_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)

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
		local_fsys_operators::create_directories(exists_dir_path);

		local_fsys_operators::create_file_of_binary(file_path, 1024);
		local_fsys_operators::create_file_of_binary(diff_file_path, 2024);

		local_fsys_operators::create_symlink(file_path_in_sym, symlink_path);
		local_fsys_operators::create_symlink(diff_file_path_in_sym, diff_symlink_path);

#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		local_fsys_operators::create_hard_link(file_path, exists_hard_link_file_path);
		local_fsys_operators::create_hard_link(diff_file_path, diff_exists_hard_link_file_path);

		local_fsys_operators::create_hard_link(symlink_path, exists_hard_link_symlink_path);
		local_fsys_operators::create_hard_link(diff_symlink_path, diff_exists_hard_link_symlink_path);

#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

		// check
#if defined(__APPLE__) || defined(__APPLE_CC__)
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(file_path));
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::equivalent(file_path, exists_hard_link_file_path));

        yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
        yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path));

        yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path));
        
#elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(2 == local_fsys_operators::hard_link_count(file_path));
		yggr_test_assert(2 == local_fsys_operators::hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::equivalent(file_path, exists_hard_link_file_path));

		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path));

		yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path));
		yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path));

#else
		yggr_test_assert(1 == local_fsys_operators::hard_link_count(file_path));
		//yggr_test_assert(0 == local_fsys_operators::hard_link_count(exists_hard_link_file_path));
		//yggr_test_assert(!local_fsys_operators::equivalent(file_path, exists_hard_link_file_path));

		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
		//yggr_test_assert(0 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_file_path));
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, diff_exists_hard_link_file_path));

		//yggr_test_assert(!local_fsys_operators::equivalent(file_path, diff_exists_hard_link_file_path));
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, exists_hard_link_file_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

#if 0
		try
		{
			yggr_test_assert(2 == local_fsys_operators::hard_link_count(symlink_path));
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			std::cout << e.what() << std::endl;
		}
#else
#   if defined(__APPLE__) || defined(__APPLE_CC__)
        yggr_test_assert(3 == local_fsys_operators::hard_link_count(symlink_path));
#	elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(2 == local_fsys_operators::hard_link_count(symlink_path));
#	else
		yggr_test_assert(1 == local_fsys_operators::hard_link_count(symlink_path));
#	endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
#endif // 0, 1

        
#if !(defined(__ANDROID__) || defined(ANDROID) || defined(__APPLE__) || defined(__APPLE_CC__)) \
    || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(symlink_path));
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_symlink_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(symlink_path, exists_hard_link_symlink_path));

		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_symlink_path));
		yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_symlink_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(diff_symlink_path, diff_exists_hard_link_symlink_path));

		yggr_test_assert(!local_fsys_operators::symlink_equivalent(symlink_path, diff_exists_hard_link_symlink_path));
		yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_symlink_path, exists_hard_link_symlink_path));

#else
		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(symlink_path));
		//yggr_test_assert(0 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_symlink_path));
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(symlink_path, exists_hard_link_symlink_path));

		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(diff_symlink_path));
		//yggr_test_assert(0 == local_fsys_operators::symlink_hard_link_count(diff_exists_hard_link_symlink_path));
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_symlink_path, diff_exists_hard_link_symlink_path));

		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(symlink_path, diff_exists_hard_link_symlink_path));
		//yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_symlink_path, exists_hard_link_symlink_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
		return;
	}

#if defined(__APPLE__) || defined(__APPLE_CC__)
    {
        bool bchk = false;
        local_fsys_operators::error_code_type ec;

        bchk = local_fsys_operators::create_hard_link(file_path, hard_link_file_path, ec);
        yggr_test_assert(bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::symlink_equivalent(file_path, hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, hard_link_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(file_path, exists_hard_link_file_path, ec);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path, ec,
                local_fsys_operators::options_type::hard_link_success_if_equivalent);
        yggr_test_assert(bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path, ec,
                local_fsys_operators::options_type::hard_link_success_if_equivalent);
        yggr_test_assert(bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);

        // !!! if equivalent hard_link not replaceable !!!
        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path, ec,
                local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path, ec,
                local_fsys_operators::options_type::symlink_replace_if_exists);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_dir_path, ec,
                local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
        yggr_test_assert(!bchk);
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_dir_path, ec,
                local_fsys_operators::options_type::hard_link_replace_if_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!local_fsys_operators::is_directory(exists_dir_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, diff_exists_hard_link_file_path, ec,
                local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
        yggr_test_assert(bchk);
        yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(2 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, diff_file_path, ec,
                local_fsys_operators::options_type::hard_link_replace_if_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(1024 == local_fsys_operators::file_size(diff_file_path));
        yggr_test_assert(!ec);

        bchk = local_fsys_operators::create_hard_link(
                file_path, exists_hard_link_file_path, ec,
                local_fsys_operators::options_type::hard_link_rename_if_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);

        {
            custom_unique_type my_unique_handler(exists_hard_link_file_path);
            bchk = local_fsys_operators::create_hard_link(
                    file_path, exists_hard_link_file_path, my_unique_handler, ec,
                    local_fsys_operators::options_type::hard_link_rename_if_exists);
            yggr_test_assert(bchk);
            yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(file_path));
            yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
            yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
            yggr_test_assert(!ec);
        }


        bchk = local_fsys_operators::create_hard_link(file_path, not_exists_hard_link_path, ec);
        yggr_test_assert(!bchk);
        yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!!ec);
        std::cout << ec.message() << std::endl;

        bchk = local_fsys_operators::create_hard_link(
                file_path, not_exists_hard_link_path, ec,
                local_fsys_operators::options_type::dir_create_if_not_exists);
        yggr_test_assert(bchk);
        yggr_test_assert(10 == local_fsys_operators::symlink_hard_link_count(file_path));
        yggr_test_assert(10 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
        yggr_test_assert(10 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
        yggr_test_assert(!ec);
    }

#elif !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
	{
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		bchk = local_fsys_operators::create_hard_link(file_path, hard_link_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::symlink_equivalent(file_path, hard_link_file_path));
		yggr_test_assert(!local_fsys_operators::symlink_equivalent(diff_file_path, hard_link_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(file_path, exists_hard_link_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path, ec,
				local_fsys_operators::options_type::hard_link_success_if_equivalent);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path, ec,
				local_fsys_operators::options_type::hard_link_success_if_equivalent);
		yggr_test_assert(bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);

		// !!! if equivalent hard_link not replaceable !!!
		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path, ec,
				local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(3 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_dir_path, ec,
				local_fsys_operators::options_type::hard_link_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(4 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, diff_exists_hard_link_file_path, ec,
				local_fsys_operators::options_type::hard_link_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(5 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(1 == local_fsys_operators::symlink_hard_link_count(diff_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, diff_file_path, ec,
				local_fsys_operators::options_type::hard_link_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(6 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::create_hard_link(
				file_path, exists_hard_link_file_path, ec,
				local_fsys_operators::options_type::hard_link_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(7 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_hard_link_file_path);
			bchk = local_fsys_operators::create_hard_link(
					file_path, exists_hard_link_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::hard_link_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(file_path));
			yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
			yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
			yggr_test_assert(!ec);
		}


		bchk = local_fsys_operators::create_hard_link(file_path, not_exists_hard_link_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(8 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;

		bchk = local_fsys_operators::create_hard_link(
				file_path, not_exists_hard_link_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(file_path));
		yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(hard_link_file_path));
		yggr_test_assert(9 == local_fsys_operators::symlink_hard_link_count(exists_hard_link_file_path));
		yggr_test_assert(!ec);
	}

#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

	try
	{
		local_fsys_operators::remove_all(root_dir_path);
	}
	catch(const local_fsys_operators::exception_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	std::cout << "--------------test_create_hard_link succeed-------------" << std::endl;
}

#endif // TEST_OF_ADMIN

void test_copy(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_copy");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type cped_dir_path = (root_dir_path / "cped_dir");
	local_fsys_operators::path_type exists_dir_path = (root_dir_path / "exists_dir");

	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
	local_fsys_operators::path_type cped_file_path = (root_dir_path / "cped_file.txt");
	local_fsys_operators::path_type exists_file_path = (root_dir_path / "exists_file.txt");
	local_fsys_operators::path_type exists_file_dir_path = (root_dir_path / "exists_file_dir");

#if TEST_OF_ADMIN

	local_fsys_operators::path_type root_dir_path_in_sym =
		local_fsys_operators::make_path_object("../test_copy");

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
		bool bchk = false;

		// directory
		bchk = local_fsys_operators::copy(dir_path, cped_dir_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(dir_path, cped_dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(cped_dir_path));

		bchk = local_fsys_operators::copy(dir_path, dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(dir_path, exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);

		custom_unique_type my_handler(exists_dir_path);
		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path,
					my_handler,
					local_fsys_operators::options_type::dir_rename_if_exists);

		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		yggr_test_assert(!local_fsys_operators::is_empty(exists_dir_path));
		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_empty(exists_dir_path));

		try
		{
			bchk = local_fsys_operators::copy(dir_path, not_exists_dir_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
				dir_path, not_exists_dir_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(not_exists_dir_path));

		// file
		bchk = local_fsys_operators::copy(file_path, cped_file_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(file_path, cped_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(cped_file_path));

		bchk = local_fsys_operators::copy(file_path, exists_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy(file_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::copy(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::copy(
					file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
		}

		bchk = local_fsys_operators::copy(
				file_path, exists_file_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::copy(
				file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
				file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));

		try
		{
			bchk = local_fsys_operators::copy(file_path, not_exists_file_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
				file_path, not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		// symlink
#if TEST_OF_ADMIN
		bchk = local_fsys_operators::copy(symlink_path, cped_symlink_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(symlink_path, cped_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(cped_symlink_path));

		bchk = local_fsys_operators::copy(symlink_path, exists_symlink_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(symlink_path, exists_dir_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::copy(
					symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
		}

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);

		try
		{
			bchk = local_fsys_operators::copy(symlink_path, not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::copy(
				symlink_path, not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
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
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		// directory
		bchk = local_fsys_operators::copy(dir_path, cped_dir_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(dir_path, cped_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(cped_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(dir_path, dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(dir_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path, ec,
					local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		custom_unique_type my_handler(exists_dir_path);
		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path,
					my_handler, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path, ec,
					local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		yggr_test_assert(!local_fsys_operators::is_empty(exists_dir_path));
		bchk = local_fsys_operators::copy(
					dir_path, exists_dir_path, ec,
					local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_empty(exists_dir_path));
		yggr_test_assert(!ec);


		bchk = local_fsys_operators::copy(dir_path, not_exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::copy(
				dir_path, not_exists_dir_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(not_exists_dir_path));
		yggr_test_assert(!ec);

		// file
		bchk = local_fsys_operators::copy(file_path, cped_file_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(file_path, cped_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(cped_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(file_path, exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(file_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::copy(
					file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		bchk = local_fsys_operators::copy(
				file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(file_path, not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::copy(
				file_path, not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		// symlink
#if TEST_OF_ADMIN
		bchk = local_fsys_operators::copy(symlink_path, cped_symlink_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(symlink_path, cped_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(cped_symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(symlink_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		{
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::copy(
					symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!ec);
		}

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(
				symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::copy(symlink_path, not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::copy(
				symlink_path, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
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

	std::cout << "--------------test_copy succeed-------------" << std::endl;
}

void test_move(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_move");

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
		local_fsys_operators::make_path_object("../test_move");

	local_fsys_operators::path_type dir_path_in_sym = ("dir");

	local_fsys_operators::path_type symlink_path = (root_dir_path / "symlink");
	local_fsys_operators::path_type moved_symlink_path = (root_dir_path / "moved_symlink");
	local_fsys_operators::path_type exists_symlink_path = (root_dir_path / "exists_symlink");
	local_fsys_operators::path_type exists_symlink_dir_path = (root_dir_path / "exists_symlink_dir");
	local_fsys_operators::path_type exists_sub_symlink_path = (exists_symlink_dir_path / "symlink");
	local_fsys_operators::path_type not_exists_symlink_path = (root_dir_path / "not_exists_symlink_dir/symlink");
#endif // TEST_OF_ADMIN

#if defined(YGGR_AT_WINDOWS)
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("f:/yggr_diff_vol_test_out/test_move");
#elif defined(YGGR_AT_LINUX)
#	if (defined(__ANDROID__) || defined(ANDROID)) 
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/storage/C5B8-A1D4/yggr_diff_vol_test_out/diff_test_move/");
#	else
	local_fsys_operators::path_type diff_vol_root_dir_path =
		local_fsys_operators::make_path_object("/run/media/alucard/win10_games/yggr_diff_vol_test_out/diff_test_move/");
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
		bool bchk = false;

		//directory
		bchk = local_fsys_operators::move(dir_path, moved_dir_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(dir_path, moved_dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(!local_fsys_operators::is_exists(dir_path));

		bchk = local_fsys_operators::move(moved_dir_path, exists_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::move(moved_dir_path, exists_sub_dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_sub_dir_path));

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path,
				local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_dir_path));

		{
			local_fsys_operators::create_directories(moved_dir_path);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::move(
					moved_dir_path, exists_dir_path, my_unique_handler,
					local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		}

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path,
				local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));

		local_fsys_operators::create_directories(moved_dir_path);
		try
		{
			bchk = local_fsys_operators::move(moved_dir_path, not_exists_dir_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(
				moved_dir_path, not_exists_dir_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(not_exists_dir_path));

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(moved_dir_path, diff_vol_moved_dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(diff_vol_moved_dir_path));

		local_fsys_operators::create_directories(moved_dir_path);
		try
		{
			bchk = local_fsys_operators::move(moved_dir_path, diff_vol_not_exists_dir_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(
				moved_dir_path, diff_vol_not_exists_dir_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		// file
		bchk = local_fsys_operators::move(file_path, moved_file_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(file_path, moved_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));

		bchk = local_fsys_operators::move(moved_file_path, exists_file_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));

		bchk = local_fsys_operators::move(moved_file_path, exists_file_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_file_dir_path));

		bchk = local_fsys_operators::move(moved_file_path, exists_sub_file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_sub_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));

		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_path,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));

		{
			local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::move(
					moved_file_path, exists_file_path, my_unique_handler,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		}

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));

		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_dir_path,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		try
		{
			bchk = local_fsys_operators::move(moved_file_path, not_exists_file_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(
				moved_file_path, not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(
				moved_file_path, diff_vol_moved_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_vol_moved_file_path));

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		try
		{
			bchk = local_fsys_operators::move(moved_file_path, diff_vol_not_exists_file_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(
				moved_file_path, diff_vol_not_exists_file_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		//symlink
#if TEST_OF_ADMIN
		bchk = local_fsys_operators::move(symlink_path, moved_symlink_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(symlink_path, moved_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));

		bchk = local_fsys_operators::move(moved_symlink_path, exists_symlink_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));

		bchk = local_fsys_operators::move(moved_symlink_path, exists_symlink_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));

		bchk = local_fsys_operators::move(moved_symlink_path, exists_sub_symlink_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_sub_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));

		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::move(
					moved_symlink_path, exists_symlink_path, my_unique_handler,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));

		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_dir_path,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_dir_path));

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		try
		{
			bchk = local_fsys_operators::move(moved_symlink_path, not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::move(
				moved_symlink_path, not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);

#if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		bchk = local_fsys_operators::move(
				moved_symlink_path, diff_vol_moved_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#else
		try
		{
			bchk = local_fsys_operators::move(
					moved_symlink_path, diff_vol_moved_symlink_path,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
#endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		try
		{
			bchk = local_fsys_operators::move(moved_symlink_path, diff_vol_not_exists_symlink_path);
			yggr_test_assert(false);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);

#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		bchk = local_fsys_operators::move(
				moved_symlink_path, diff_vol_not_exists_symlink_path,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#	else
		try
		{
			bchk = local_fsys_operators::move(
					moved_symlink_path, diff_vol_not_exists_symlink_path,
					local_fsys_operators::options_type::dir_create_if_not_exists);
		}
		catch(const local_fsys_operators::exception_type& e)
		{
			bchk = false;
			std::cout << e.what() << std::endl;
		}
		yggr_test_assert(!bchk);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
#	endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED)
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
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		//directory
		bchk = local_fsys_operators::move(dir_path, moved_dir_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(dir_path, moved_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(!local_fsys_operators::is_exists(dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_dir_path, exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_dir_path, exists_sub_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_sub_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path, ec,
				local_fsys_operators::options_type::dir_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path, ec,
				local_fsys_operators::options_type::dir_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_dir_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_directories(moved_dir_path);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::move(
					moved_dir_path, exists_dir_path, my_unique_handler, ec,
					local_fsys_operators::options_type::dir_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path, ec,
				local_fsys_operators::options_type::dir_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(
				moved_dir_path, exists_dir_path, ec,
				local_fsys_operators::options_type::dir_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(moved_dir_path, not_exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move(
				moved_dir_path, not_exists_dir_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(not_exists_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(moved_dir_path, diff_vol_moved_dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_directory(moved_dir_path));
		yggr_test_assert(local_fsys_operators::is_directory(diff_vol_moved_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_directories(moved_dir_path);
		bchk = local_fsys_operators::move(moved_dir_path, diff_vol_not_exists_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move(
				moved_dir_path, diff_vol_not_exists_dir_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		// file
		bchk = local_fsys_operators::move(file_path, moved_file_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(file_path, moved_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_file_path, exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_file_path, exists_file_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_file_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_file_path, exists_sub_file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_sub_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(2048 == local_fsys_operators::file_size(exists_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_path, ec,
				local_fsys_operators::options_type::file_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
			custom_unique_type my_unique_handler(exists_file_path);
			bchk = local_fsys_operators::move(
					moved_file_path, exists_file_path, my_unique_handler, ec,
					local_fsys_operators::options_type::file_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_file_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(moved_file_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(
				moved_file_path, exists_file_dir_path, ec,
				local_fsys_operators::options_type::file_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(exists_file_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(moved_file_path, not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move(
				moved_file_path, not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(
				moved_file_path, diff_vol_moved_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(1024 == local_fsys_operators::file_size(diff_vol_moved_file_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_file_of_binary(moved_file_path, 1024);
		bchk = local_fsys_operators::move(moved_file_path, diff_vol_not_exists_file_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move(
				moved_file_path, diff_vol_not_exists_file_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		//symlink
#if TEST_OF_ADMIN
		bchk = local_fsys_operators::move(symlink_path, moved_symlink_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(symlink_path, moved_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!local_fsys_operators::is_exists(symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_symlink_path, exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_symlink_path, exists_symlink_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(moved_symlink_path, exists_sub_symlink_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_sub_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_success_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_exists(exists_symlink_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_rename_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(!ec);

		{
			local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
			custom_unique_type my_unique_handler(exists_symlink_path);
			bchk = local_fsys_operators::move(
					moved_symlink_path, exists_symlink_path, my_unique_handler, ec,
					local_fsys_operators::options_type::symlink_rename_if_exists);
			yggr_test_assert(bchk);
			yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
			yggr_test_assert(!ec);
		}

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists_not_dir);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_directory(exists_symlink_dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::move(
				moved_symlink_path, exists_symlink_dir_path, ec,
				local_fsys_operators::options_type::symlink_replace_if_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(moved_symlink_path));
		yggr_test_assert(local_fsys_operators::is_symlink(exists_symlink_dir_path));
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(moved_symlink_path, not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move(
				moved_symlink_path, not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);

		bchk = local_fsys_operators::move(
				moved_symlink_path, diff_vol_moved_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);

#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
		yggr_test_assert(!ec);
#	else
		yggr_test_assert(!bchk);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_moved_symlink_path));
		yggr_test_assert(!!ec);
		ec.clear();
#	endif // #	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root

		local_fsys_operators::create_symlink(dir_path_in_sym, moved_symlink_path);
		bchk = local_fsys_operators::move(moved_symlink_path, diff_vol_not_exists_symlink_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!!ec);
		std::cout << ec.message() << std::endl;
		ec.clear();

		bchk = local_fsys_operators::move(
				moved_symlink_path, diff_vol_not_exists_symlink_path, ec,
				local_fsys_operators::options_type::dir_create_if_not_exists);
#	if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
		yggr_test_assert(bchk);
		yggr_test_assert(local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
		yggr_test_assert(!ec);
#	else
		yggr_test_assert(!bchk);
		yggr_test_assert(!local_fsys_operators::is_symlink(diff_vol_not_exists_symlink_path));
		yggr_test_assert(!!ec);
		ec.clear();
#	endif // #if !(defined(__ANDROID__) || defined(ANDROID)) || (defined(ANDROID_ROOTED) && ANDROID_ROOTED) // need root
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

	std::cout << "--------------test_move succeed-------------" << std::endl;
}

void test_remove(void)
{
	local_fsys_operators::path_type root_dir_path =
		local_fsys_operators::make_path_object("../test_out/file_system/test_remove");

	local_fsys_operators::path_type dir_path = (root_dir_path / "dir");
	local_fsys_operators::path_type file_path = (root_dir_path / "file.txt");
#if TEST_OF_ADMIN

	local_fsys_operators::path_type root_dir_path_in_sym =
		local_fsys_operators::make_path_object("../test_remove");

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
		bool bchk = false;

		// gen

		bchk = local_fsys_operators::remove(not_exists_path);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove(
				not_exists_path, local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(bchk);

		// directory

		bchk = local_fsys_operators::remove(dir_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove(dir_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(dir_path));

		bchk = local_fsys_operators::remove(non_empty_dir_path);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(non_empty_dir_path));

		// file
		bchk = local_fsys_operators::remove(file_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove(file_path);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));

#if TEST_OF_ADMIN

		bchk = local_fsys_operators::remove(symlink_path, 0, 0);
		yggr_test_assert(!bchk);

		bchk = local_fsys_operators::remove(symlink_path);
		yggr_test_assert(bchk);
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
		bool bchk = false;
		local_fsys_operators::error_code_type ec;

		// gen

		bchk = local_fsys_operators::remove(not_exists_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove(
				not_exists_path, ec,
				local_fsys_operators::options_type::remove_success_if_not_exists);
		yggr_test_assert(bchk);
		yggr_test_assert(!ec);

		// directory

		bchk = local_fsys_operators::remove(dir_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove(dir_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(dir_path));
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove(non_empty_dir_path, ec);
		yggr_test_assert(!bchk);
		yggr_test_assert(local_fsys_operators::is_directory(non_empty_dir_path));
		yggr_test_assert(!ec);

		// file
		bchk = local_fsys_operators::remove(file_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove(file_path, ec);
		yggr_test_assert(bchk);
		yggr_test_assert(!local_fsys_operators::is_exists(file_path));
		yggr_test_assert(!ec);

#if TEST_OF_ADMIN

		bchk = local_fsys_operators::remove(symlink_path, ec, 0, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(!ec);

		bchk = local_fsys_operators::remove(symlink_path, ec);
		yggr_test_assert(bchk);
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

	std::cout << "--------------test_remove succeed-------------" << std::endl;
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_file_size();
	test_count_children_file();
	test_create_file();
	test_write_and_read_file();
	test_append_file();
	test_copy_file();
	test_rename_file();
	test_move_file();
	test_remove_file();

#if TEST_OF_ADMIN

	test_count_children_symlink();
	test_create_symlink();
	test_copy_symlink();
	test_rename_symlink();
	test_move_symlink();
	test_remove_symlink();
	test_read_symlink();
	test_create_hard_link();

#endif // TEST_OF_ADMIN

	test_copy();
	test_move();
	test_remove();

	wait_any_key(argc, argv);
	return 0;
}
