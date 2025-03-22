//local_fsys_operator_options.hpp

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

#ifndef __YGGR_FILE_SYSTEM_LOCAL_FSYS_OPERATOR_OPTIONS_HPP__
#define __YGGR_FILE_SYSTEM_LOCAL_FSYS_OPERATOR_OPTIONS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

namespace yggr
{
namespace file_system
{

/*
 3126      24       18       11       6        0
 +-+-------+--------+--------+--------+--------+
 |1|   2   |   6    |   6    |   5    |    6   |
 +-+-------+--------+--------+--------+--------+
 |n|ex_mark|hardlink|symlink |  file  |   dir  |
 +-+-------+--------+--------+--------+--------+
*/
struct local_fsys_operator_opts
{
	YGGR_STATIC_CONSTANT(u32, none = 0);
	YGGR_STATIC_CONSTANT(u32, dir_success_if_exists = 1);
	YGGR_STATIC_CONSTANT(u32, dir_merge_if_exists = (1 << 1));
	YGGR_STATIC_CONSTANT(u32, dir_replace_if_exists_not_dir = (1 << 2));
	YGGR_STATIC_CONSTANT(u32, dir_replace_if_exists = (1 << 3));
	YGGR_STATIC_CONSTANT(u32, dir_rename_if_exists = (1 << 4));
	YGGR_STATIC_CONSTANT(u32, dir_create_if_not_exists = (1 << 5));

	YGGR_STATIC_CONSTANT(u32, dir_options_end = (1 << 6));

	YGGR_STATIC_CONSTANT(u32, file_success_if_exists = (1 << 7));
	YGGR_STATIC_CONSTANT(u32, file_replace_if_exists_not_dir = (1 << 8));
	YGGR_STATIC_CONSTANT(u32, file_replace_if_exists = (1 << 9));
	YGGR_STATIC_CONSTANT(u32, file_rename_if_exists = (1 << 10));

	YGGR_STATIC_CONSTANT(u32, file_options_end = (1 << 11));

	YGGR_STATIC_CONSTANT(u32, symlink_success_if_exists = (1 << 13));
	YGGR_STATIC_CONSTANT(u32, symlink_replace_if_exists_not_dir = (1 << 14));
	YGGR_STATIC_CONSTANT(u32, symlink_replace_if_exists = (1 << 15));
	YGGR_STATIC_CONSTANT(u32, symlink_rename_if_exists = (1 << 16));
	YGGR_STATIC_CONSTANT(u32, symlink_recurse = (1 << 17));

	YGGR_STATIC_CONSTANT(u32, symlink_options_end = (1 << 18));

	YGGR_STATIC_CONSTANT(u32, hard_link_success_if_exists = (1 << 18));
	YGGR_STATIC_CONSTANT(u32, hard_link_success_if_equivalent = (1 << 20));
	YGGR_STATIC_CONSTANT(u32, hard_link_replace_if_exists_not_dir = (1 << 21));
	YGGR_STATIC_CONSTANT(u32, hard_link_replace_if_exists = (1 << 22));
	YGGR_STATIC_CONSTANT(u32, hard_link_rename_if_exists = (1 << 23));

	YGGR_STATIC_CONSTANT(u32, hard_link_options_end = (1 << 24));

	YGGR_STATIC_CONSTANT(u32, recursive_operator_ignore_if_failed = (1 << 25));
	YGGR_STATIC_CONSTANT(u32, operator_ignore_if_failed = recursive_operator_ignore_if_failed);

	YGGR_STATIC_CONSTANT(u32, remove_success_if_not_exists = (1 << 26));

	YGGR_STATIC_CONSTANT(
		u32,
		all_success_if_exists =
			(dir_success_if_exists
			| file_success_if_exists
			| symlink_success_if_exists));

	YGGR_STATIC_CONSTANT(
		u32,
		all_replace_if_exists_not_dir =
			(dir_replace_if_exists_not_dir
			| file_replace_if_exists_not_dir
			| symlink_replace_if_exists_not_dir));

	YGGR_STATIC_CONSTANT(
		u32,
		all_replace_if_exists =
			(dir_replace_if_exists
			| file_replace_if_exists
			| symlink_replace_if_exists));

	YGGR_STATIC_CONSTANT(
		u32,
		all_rename_if_exists =
			(dir_rename_if_exists
			| file_rename_if_exists
			| symlink_rename_if_exists));

	YGGR_STATIC_CONSTANT(u32, E_opts_end = (1 << 31));
};

struct local_fsys_file_types_opts
{
	YGGR_STATIC_CONSTANT(u32, none = 0);

	YGGR_STATIC_CONSTANT(u32, file_not_found = 1);
	YGGR_STATIC_CONSTANT(u32, regular_file = (1 << 1));
	YGGR_STATIC_CONSTANT(u32, directory_file = (1 << 2));
	// the following may not apply to some operating systems or file systems
	YGGR_STATIC_CONSTANT(u32, symlink_file = (1 << 3));
	YGGR_STATIC_CONSTANT(u32, block_file = (1 << 4));
	YGGR_STATIC_CONSTANT(u32, character_file = (1 << 5));
	YGGR_STATIC_CONSTANT(u32, fifo_file = (1 << 6));
	YGGR_STATIC_CONSTANT(u32, socket_file = (1 << 7));
	YGGR_STATIC_CONSTANT(u32, reparse_file = (1 << 8)); // Windows: FILE_ATTRIBUTE_REPARSE_POINT that is not a symlink
	YGGR_STATIC_CONSTANT(u32, type_unknown = (1 << 9));

	YGGR_STATIC_CONSTANT(u32, general_file = regular_file);
	YGGR_STATIC_CONSTANT(u32, general_directory = directory_file);
	YGGR_STATIC_CONSTANT(u32, general_link = symlink_file);
	YGGR_STATIC_CONSTANT(u32, general_all = (regular_file | directory_file | symlink_file));

	YGGR_STATIC_CONSTANT(
		u32,
		all_file =
			(general_file
			| block_file
			| character_file
			| fifo_file
			| socket_file
			| reparse_file));

	YGGR_STATIC_CONSTANT(
		u32,
		types_all =
			(general_all
			| block_file
			| character_file
			| fifo_file
			| socket_file
			| reparse_file));

	YGGR_STATIC_CONSTANT(u32, failed = (1 << 30));

	// opts desc
	YGGR_STATIC_CONSTANT(u32, E_opts_bit_begin = 0);
	YGGR_STATIC_CONSTANT(u32, E_opts_bit_end = 10);
	YGGR_STATIC_CONSTANT(u32, E_opts_bit_size = 10);

	inline static u32 conv_to_file_type(u32 opts)
	{
		switch(opts)
		{
		case none:
			return boost::filesystem::status_error;
		case file_not_found:
			return boost::filesystem::file_not_found;
		case regular_file:
			return boost::filesystem::regular_file;
		case directory_file:
			return boost::filesystem::directory_file;
		case symlink_file:
			return boost::filesystem::symlink_file;
		case block_file:
			return boost::filesystem::block_file;
		case character_file:
			return boost::filesystem::character_file;
		case fifo_file:
			return boost::filesystem::fifo_file;
		case socket_file:
			return boost::filesystem::socket_file;
		case reparse_file:
			return boost::filesystem::reparse_file;
		case type_unknown:
			return boost::filesystem::type_unknown;
		case failed:
			return boost::filesystem::status_error;
		default:
			assert(false);
			return boost::filesystem::status_error;
		};
	}

	inline static u32 conv_to_file_type_opts(u32 ftype)
	{
		switch(ftype)
		{
		case boost::filesystem::status_error:
			return failed;
		case boost::filesystem::file_not_found:
			return file_not_found;
		case boost::filesystem::regular_file:
			return regular_file;
		case boost::filesystem::directory_file:
			return directory_file;
		case boost::filesystem::symlink_file:
			return symlink_file;
		case boost::filesystem::block_file:
			return block_file;
		case boost::filesystem::character_file:
			return character_file;
		case boost::filesystem::fifo_file:
			return fifo_file;
		case boost::filesystem::socket_file:
			return socket_file;
		case boost::filesystem::reparse_file:
			return reparse_file;
		case boost::filesystem::type_unknown:
			return type_unknown;
		default:
			assert(false);
			return failed;
		};
	}
};

} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_LOCAL_FSYS_OPERATOR_OPTIONS_HPP__
