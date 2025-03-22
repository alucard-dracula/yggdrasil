// local_fsys_native_operators.hpp

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

#ifndef __YGGR_FILE_SYSTEM_LOCAL_FSYS_NATIVE_OPERATORS_HPP__
#define __YGGR_FILE_SYSTEM_LOCAL_FSYS_NATIVE_OPERATORS_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/tuple/tuple.hpp>
#include <yggr/ppex/open.hpp>

#include <boost/preprocessor/cat.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/functional/hash/hash.hpp>

#include <string>
#include <utility>

#if defined(BOOST_POSIX_API)

#   include <sys/types.h>
#   include <sys/stat.h>

#	define YGGR_FSYS_ERRNO() errno
#	define YGGR_FSYS_ERROR_PATH_NAME_FAILED() ENOENT
#	define YGGR_FSYS_ERROR_ALREADY_EXISTS() EEXIST
#	define YGGR_FSYS_ERROR_NOT_SUPPORTED() ENOSYS // not used

#else  // BOOST_WINDOWS_API

#include <yggr/network/socket_conflict_fixer.hpp>
#include <windows.h>

#	define YGGR_FSYS_ERRNO() ::GetLastError()
#	define YGGR_FSYS_ERROR_PATH_NAME_FAILED() ERROR_PATH_NOT_FOUND
#   define YGGR_FSYS_ERROR_ALREADY_EXISTS() ERROR_ALREADY_EXISTS
#	define YGGR_FSYS_ERROR_NOT_SUPPORTED() ERROR_NOT_SUPPORTED // not used

# endif // BOOST_POSIX_API

#if !defined(YGGR_FSYS_PATH_BUF_RECOMMEND_SIZE)
#	define YGGR_FSYS_PATH_BUF_RECOMMEND_SIZE() 256
#endif // YGGR_FSYS_PATH_BUF_RECOMMEND_SIZE

#if !defined(YGGR_FSYS_DIR_HARD_LINK_SUPPORT_ENABLE)
#	define YGGR_FSYS_DIR_HARD_LINK_SUPPORT_ENABLE 0
#endif // YGGR_FSYS_DIR_HARD_LINK_SUPPORT_ENABLE

#if !defined(YGGR_FSYS_SYMLINK_HARD_LINK_SUPPORT_ENABLE)
#	define YGGR_FSYS_SYMLINK_HARD_LINK_SUPPORT_ENABLE 0
#endif //YGGR_FSYS_SYMLINK_HARD_LINK_SUPPORT_ENABLE

// fixed here need test linux
#if YGGR_FSYS_DIR_HARD_LINK_SUPPORT_ENABLE
#	define YGGR_FSYS_DIRECTORY_HARD_LINK_SUPPORT
#endif // YGGR_FSYS_DIRECTORY_HARD_LINK_SUPPORT_ENABLE

#if defined(BOOST_LIKELY)
#	define YGGR_FSYS_LINKELY BOOST_LIKELY
#else
#	define YGGR_FSYS_LINKELY( __x__ ) __x__
#endif //BOOST_LIKELY


#if defined(BOOST_UNLIKELY)
#	define YGGR_FSYS_UNLIKELY BOOST_UNLIKELY
#else
#	define YGGR_FSYS_UNLIKELY( __x__ ) __x__
#endif //BOOST_UNLIKELY

namespace yggr
{
namespace file_system
{
namespace native
{
namespace detail
{

#if defined(BOOST_POSIX_API)

typedef int yggr_fsys_err_t;

#else

typedef boost::winapi::DWORD_ yggr_fsys_err_t;

#endif // BOOST_POSIX_API

inline void emit_error(yggr_fsys_err_t error_num, boost::system::error_code* ec, const char* message)
{
    if (!ec)
        BOOST_FILESYSTEM_THROW(
			boost::filesystem::filesystem_error(
				message, boost::system::error_code(error_num, boost::system::system_category())));
    else
        *ec = boost::system::error_code(error_num, boost::system::system_category());
}

inline void emit_error(yggr_fsys_err_t error_num, boost::filesystem::path const& p,
						boost::system::error_code* ec, const char* message)
{
    if (!ec)
        BOOST_FILESYSTEM_THROW(
			boost::filesystem::filesystem_error(
				message, p, boost::system::error_code(error_num, boost::system::system_category())));
    else
       *ec = boost::system::error_code(error_num, boost::system::system_category());
}

inline void emit_error(yggr_fsys_err_t error_num,
						boost::filesystem::path const& p1, boost::filesystem::path const& p2,
						boost::system::error_code* ec, const char* message)
{
    if (!ec)
        BOOST_FILESYSTEM_THROW(
			boost::filesystem::filesystem_error(
				message, p1, p2, boost::system::error_code(error_num, boost::system::system_category())));
    else
        *ec = boost::system::error_code(error_num, boost::system::system_category());
}

inline bool error(yggr_fsys_err_t error_num, boost::system::error_code* ec, const char* message)
{
    if (YGGR_FSYS_LINKELY(!error_num))
    {
        if (ec)
            ec->clear();
        return false;
    }
    else
    { //  error
        emit_error(error_num, ec, message);
        return true;
    }
}

inline bool error(yggr_fsys_err_t error_num, boost::filesystem::path const& p,
					boost::system::error_code* ec, const char* message)
{
    if (YGGR_FSYS_LINKELY(!error_num))
    {
        if (ec)
            ec->clear();
        return false;
    }
    else
    { //  error
        emit_error(error_num, p, ec, message);
        return true;
    }
}

inline bool error(yggr_fsys_err_t error_num,
					boost::filesystem::path const& p1, boost::filesystem::path const& p2,
					boost::system::error_code* ec, const char* message)
{
    if (YGGR_FSYS_LINKELY(!error_num))
    {
        if (ec)
            ec->clear();
        return false;
    }
    else
    { //  error
        emit_error(error_num, p1, p2, ec, message);
        return true;
    }
}

#if defined(BOOST_POSIX_API)

#else

struct handle_wrapper
{
	HANDLE handle;

	handle_wrapper(void) YGGR_NOEXCEPT
		: handle(INVALID_HANDLE_VALUE)
	{
	}

	explicit handle_wrapper(HANDLE h) YGGR_NOEXCEPT
		: handle(h)
	{
	}

	~handle_wrapper(void) YGGR_NOEXCEPT
	{
		if(handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(handle);
		}
	}

private:
	handle_wrapper(const handle_wrapper&);
	handle_wrapper& operator=(const handle_wrapper&);
};


template<typename Char> inline
typename boost::enable_if_c<sizeof(Char) == 1, HANDLE>::type
	create_file_handle(const Char* p, DWORD dwDesiredAccess,
						DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
						DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
						HANDLE hTemplateFile)
{
	assert(p);
	return ::CreateFile(p, dwDesiredAccess, dwShareMode,
						lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
						hTemplateFile);
}

template<typename Char> inline
typename boost::disable_if_c<sizeof(Char) == 1, HANDLE>::type
	create_file_handle(const Char* p, DWORD dwDesiredAccess,
						DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
						DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
						HANDLE hTemplateFile)
{
	assert(p);
	return ::CreateFileW(p, dwDesiredAccess, dwShareMode,
							lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
							hTemplateFile);
}

#endif // BOOST_POSIX_API

inline bool symlink_equivalent(const boost::filesystem::path& p1,
								const boost::filesystem::path& p2,
								boost::system::error_code* ec)
{
#ifdef BOOST_POSIX_API
	struct stat s2;
	int e2(::lstat(p2.c_str(), &s2));
	struct stat s1;
	int e1(::lstat(p1.c_str(), &s1));

	if(YGGR_FSYS_UNLIKELY(e1 != 0 || e2 != 0))
	{
		// if one is invalid and the other isn't then they aren't equivalent,
		// but if both are invalid then it is an error
		if(e1 != 0 && e2 != 0)
            emit_error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::symlink_equivalent");
		return false;
	}

#if BOOST_VERSION < 107400
	// both stats now known to be valid
	return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino
		// According to the POSIX stat specs, "The st_ino and st_dev fields
		// taken together uniquely identify the file within the system."
		// Just to be sure, size and mod time are also checked.
		&& s1.st_size == s2.st_size && s1.st_mtime == s2.st_mtime; // high boost version filesystem not check this
#else
	return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;
#endif // BOOST_VERSION < 107400

#else  // Windows

	// Thanks to Jeremy Maitin-Shepard for much help and for permission to
    // base the equivalent() implementation on portions of his
    // file-equivalence-win32.cpp experimental code.

    // Note well: Physical location on external media is part of the
    // equivalence criteria. If there are no open handles, physical location
    // can change due to defragmentation or other relocations. Thus handles
    // must be held open until location information for both paths has
    // been retrieved.

    // p2 is done first, so any error reported is for p1
    handle_wrapper h2(create_file_handle(
        p2.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
		0));

    handle_wrapper h1(create_file_handle(
        p1.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
		0));

    if(YGGR_FSYS_UNLIKELY(h1.handle == INVALID_HANDLE_VALUE || h2.handle == INVALID_HANDLE_VALUE))
    {
        // if one is invalid and the other isn't, then they aren't equivalent,
        // but if both are invalid then it is an error
        if (h1.handle == INVALID_HANDLE_VALUE && h2.handle == INVALID_HANDLE_VALUE)
            error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::symlink_equivalent");
        return false;
    }

    // at this point, both handles are known to be valid

    BY_HANDLE_FILE_INFORMATION info1, info2;

    if (error(!::GetFileInformationByHandle(h1.handle, &info1)? YGGR_FSYS_ERRNO() : 0,
				p1, p2, ec, "yggr::file_system::native::symlink_equivalent"))
	{
        return false;
	}

    if (error(!::GetFileInformationByHandle(h2.handle, &info2)? YGGR_FSYS_ERRNO() : 0,
				p1, p2, ec, "yggr::file_system::native::symlink_equivalent"))
	{
        return false;
	}

    // In theory, volume serial numbers are sufficient to distinguish between
    // devices, but in practice VSN's are sometimes duplicated, so last write
    // time and file size are also checked.
    return
		info1.dwVolumeSerialNumber == info2.dwVolumeSerialNumber
        && info1.nFileIndexHigh == info2.nFileIndexHigh
        && info1.nFileIndexLow == info2.nFileIndexLow
        && info1.nFileSizeHigh == info2.nFileSizeHigh
        && info1.nFileSizeLow == info2.nFileSizeLow
        && info1.ftLastWriteTime.dwLowDateTime == info2.ftLastWriteTime.dwLowDateTime
        && info1.ftLastWriteTime.dwHighDateTime == info2.ftLastWriteTime.dwHighDateTime;

#endif // BOOST_POSIX_API
}

inline std::size_t equivalent_hash_value(const boost::filesystem::path& p,
											boost::system::error_code* ec)
{
	std::size_t seed = 0;

#ifdef BOOST_POSIX_API
	struct stat s;
	int e(::stat(p.c_str(), &s));

	if(YGGR_FSYS_UNLIKELY(e != 0))
	{
		// if one is invalid and the other isn't then they aren't equivalent,
		// but if both are invalid then it is an error
		emit_error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::equivalent_hash_value");
		return seed;
	}

	boost::hash_combine(seed, s.st_dev);
	boost::hash_combine(seed, s.st_ino);
	boost::hash_combine(seed, s.st_size);
	boost::hash_combine(seed, s.st_mtime);

#else  // Windows

	handle_wrapper h(
		create_file_handle(
			p.c_str(),
			FILE_READ_ATTRIBUTES,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			0));

	if(YGGR_FSYS_UNLIKELY(h.handle == INVALID_HANDLE_VALUE))
    {
        // if one is invalid and the other isn't, then they aren't equivalent,
        // but if both are invalid then it is an error
		error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::equivalent_hash_value");
        return seed;
    }

	// at this point, both handles are known to be valid

	BY_HANDLE_FILE_INFORMATION info;

    if (error(!::GetFileInformationByHandle(h.handle, &info)? YGGR_FSYS_ERRNO() : 0,
				p, ec, "yggr::file_system::native::equivalent_hash_value"))
	{
        return seed;
	}

	boost::hash_combine(seed, info.dwVolumeSerialNumber);
	boost::hash_combine(seed, info.nFileIndexHigh);
	boost::hash_combine(seed, info.nFileIndexLow);
	boost::hash_combine(seed, info.nFileSizeHigh);
	boost::hash_combine(seed, info.nFileSizeLow);
	boost::hash_combine(seed, info.ftLastWriteTime.dwLowDateTime);
	boost::hash_combine(seed, info.ftLastWriteTime.dwHighDateTime);

#endif // BOOST_POSIX_API

	return seed;
}

inline std::size_t equivalent_hash_value_if_has_hard_link(
					const boost::filesystem::path& p,
					boost::system::error_code* ec)
{
std::size_t seed = 0;

#ifdef BOOST_POSIX_API
	struct stat s;
	int e(::stat(p.c_str(), &s));

	if(YGGR_FSYS_UNLIKELY(e != 0))
	{
		// if one is invalid and the other isn't then they aren't equivalent,
		// but if both are invalid then it is an error
		emit_error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::equivalent_hash_value_if_has_hard_link");
		return seed;
	}

	if(s.st_nlink > 1)
	{
		boost::hash_combine(seed, s.st_dev);
		boost::hash_combine(seed, s.st_ino);
		boost::hash_combine(seed, s.st_size);
		boost::hash_combine(seed, s.st_mtime);
	}

#else  // Windows

	handle_wrapper h(
		create_file_handle(
			p.c_str(),
			FILE_READ_ATTRIBUTES,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			0));

	if(YGGR_FSYS_UNLIKELY(h.handle == INVALID_HANDLE_VALUE))
    {
        // if one is invalid and the other isn't, then they aren't equivalent,
        // but if both are invalid then it is an error
		error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::equivalent_hash_value_if_has_hard_link");
        return seed;
    }

	// at this point, both handles are known to be valid

	BY_HANDLE_FILE_INFORMATION info;

    if (error(!::GetFileInformationByHandle(h.handle, &info)? YGGR_FSYS_ERRNO() : 0,
				p, ec, "yggr::file_system::native::equivalent_hash_value_if_has_hard_link"))
    {
        return seed;
    }

	if(info.nNumberOfLinks > 1)
	{
		boost::hash_combine(seed, info.dwVolumeSerialNumber);
		boost::hash_combine(seed, info.nFileIndexHigh);
		boost::hash_combine(seed, info.nFileIndexLow);
		boost::hash_combine(seed, info.nFileSizeHigh);
		boost::hash_combine(seed, info.nFileSizeLow);
		boost::hash_combine(seed, info.ftLastWriteTime.dwLowDateTime);
		boost::hash_combine(seed, info.ftLastWriteTime.dwHighDateTime);
	}

#endif // BOOST_POSIX_API

	return seed;
}

inline std::size_t symlink_equivalent_hash_value(const boost::filesystem::path& p,
													boost::system::error_code* ec)
{
	std::size_t seed = 0;

#ifdef BOOST_POSIX_API
	struct stat s;
	int e(::lstat(p.c_str(), &s));

	if(YGGR_FSYS_UNLIKELY(e != 0))
	{
		// if one is invalid and the other isn't then they aren't equivalent,
		// but if both are invalid then it is an error
		emit_error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::symlink_equivalent_hash_value");
		return seed;
	}

	boost::hash_combine(seed, s.st_dev);
	boost::hash_combine(seed, s.st_ino);
	boost::hash_combine(seed, s.st_size);
	boost::hash_combine(seed, s.st_mtime);

#else  // Windows

	// Note well: Physical location on external media is part of the
	// equivalence criteria. If there are no open handles, physical location
	// can change due to defragmentation or other relocations. Thus handles
	// must be held open until location information for both paths has
	// been retrieved.

	handle_wrapper h(
		create_file_handle(
			p.c_str(),
			FILE_READ_ATTRIBUTES,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
			0));

	if(YGGR_FSYS_UNLIKELY(h.handle == INVALID_HANDLE_VALUE))
	{
		// if one is invalid and the other isn't, then they aren't equivalent,
		// but if both are invalid then it is an error

		error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::symlink_equivalent_hash_value");
		return seed;
	}

	// at this point, both handles are known to be valid

	BY_HANDLE_FILE_INFORMATION info;

	if(error(!::GetFileInformationByHandle(h.handle, &info),
		p, ec, "yggr::file_system::native::symlink_equivalent_hash_value"))
	{
		return seed;
	}


	boost::hash_combine(seed, info.dwVolumeSerialNumber);
	boost::hash_combine(seed, info.nFileIndexHigh);
	boost::hash_combine(seed, info.nFileIndexLow);
	boost::hash_combine(seed, info.nFileSizeHigh);
	boost::hash_combine(seed, info.nFileSizeLow);
	boost::hash_combine(seed, info.ftLastWriteTime.dwLowDateTime);
	boost::hash_combine(seed, info.ftLastWriteTime.dwHighDateTime);

#endif // BOOST_POSIX_API

	return seed;
}

inline std::size_t symlink_equivalent_hash_value_if_has_hard_link(
					const boost::filesystem::path& p,
					boost::system::error_code* ec)
{
	std::size_t seed = 0;

#ifdef BOOST_POSIX_API
	struct stat s;
	int e(::lstat(p.c_str(), &s));

	if(YGGR_FSYS_UNLIKELY(e != 0))
	{
		// if one is invalid and the other isn't then they aren't equivalent,
		// but if both are invalid then it is an error
		emit_error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::symlink_equivalent_hash_value_if_has_hard_link");
		return seed;
	}

	if(s.st_nlink > 1)
	{
		boost::hash_combine(seed, s.st_dev);
		boost::hash_combine(seed, s.st_ino);
		boost::hash_combine(seed, s.st_size);
		boost::hash_combine(seed, s.st_mtime);
	}

#else  // Windows

	// Note well: Physical location on external media is part of the
	// equivalence criteria. If there are no open handles, physical location
	// can change due to defragmentation or other relocations. Thus handles
	// must be held open until location information for both paths has
	// been retrieved.

	handle_wrapper h(
		create_file_handle(
			p.c_str(),
			FILE_READ_ATTRIBUTES,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
			0));

	if(YGGR_FSYS_UNLIKELY(h.handle == INVALID_HANDLE_VALUE))
	{
		// if one is invalid and the other isn't, then they aren't equivalent,
		// but if both are invalid then it is an error

		error(YGGR_FSYS_ERRNO(), p, ec, "yggr::file_system::native::symlink_equivalent_hash_value_if_has_hard_link");
		return seed;
	}

	// at this point, both handles are known to be valid

	BY_HANDLE_FILE_INFORMATION info;

	if(error(!::GetFileInformationByHandle(h.handle, &info),
		p, ec, "yggr::file_system::native::symlink_equivalent_hash_value_if_has_hard_link"))
	{
		return seed;
	}

	if(info.nNumberOfLinks > 1)
	{
		boost::hash_combine(seed, info.dwVolumeSerialNumber);
		boost::hash_combine(seed, info.nFileIndexHigh);
		boost::hash_combine(seed, info.nFileIndexLow);
		boost::hash_combine(seed, info.nFileSizeHigh);
		boost::hash_combine(seed, info.nFileSizeLow);
		boost::hash_combine(seed, info.ftLastWriteTime.dwLowDateTime);
		boost::hash_combine(seed, info.ftLastWriteTime.dwHighDateTime);
	}

#endif // BOOST_POSIX_API

	return seed;
}

inline bool is_in_same_volume(const boost::filesystem::path& p1,
								const boost::filesystem::path& p2,
								boost::system::error_code* ec)
{
#ifdef BOOST_POSIX_API
	struct stat s2;
	int e2(::stat(p2.c_str(), &s2));
	struct stat s1;
	int e1(::stat(p1.c_str(), &s1));

	if(YGGR_FSYS_UNLIKELY(e1 != 0 || e2 != 0))
	{
		if(e1 != 0 && e2 != 0)
            emit_error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::is_in_same_volume");
		return false;
	}

	return s1.st_dev == s2.st_dev;

#else  // Windows

	// Note well: Physical location on external media is part of the
	// equivalence criteria. If there are no open handles, physical location
	// can change due to defragmentation or other relocations. Thus handles
	// must be held open until location information for both paths has
	// been retrieved.

	// p2 is done first, so any error reported is for p1
	handle_wrapper h2(create_file_handle(
        p2.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
		0));

    handle_wrapper h1(create_file_handle(
        p1.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
		0));

    if (YGGR_FSYS_UNLIKELY(h1.handle == INVALID_HANDLE_VALUE || h2.handle == INVALID_HANDLE_VALUE))
    {
        // if one is invalid and the other isn't, then they aren't equivalent,
        // but if both are invalid then it is an error
        if (h1.handle == INVALID_HANDLE_VALUE && h2.handle == INVALID_HANDLE_VALUE)
            error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::is_in_same_volume");
        return false;
    }

    // at this point, both handles are known to be valid

    BY_HANDLE_FILE_INFORMATION info1, info2;

    if (error(!::GetFileInformationByHandle(h1.handle, &info1)? YGGR_FSYS_ERRNO() : 0,
				p1, p2, ec, "yggr::file_system::native::is_in_same_volume"))
    {
        return false;
    }

    if (error(!::GetFileInformationByHandle(h2.handle, &info2)? YGGR_FSYS_ERRNO() : 0,
				p1, p2, ec, "yggr::file_system::native::is_in_same_volume"))
	{
        return false;
	}

	return info1.dwVolumeSerialNumber == info2.dwVolumeSerialNumber;

#endif // BOOST_POSIX_API
}

inline bool symlink_is_in_same_volume(const boost::filesystem::path& p1,
										const boost::filesystem::path& p2,
										boost::system::error_code* ec)
{
#ifdef BOOST_POSIX_API
	struct stat s2;
	int e2(::lstat(p2.c_str(), &s2));
	struct stat s1;
	int e1(::lstat(p1.c_str(), &s1));

	if(YGGR_FSYS_UNLIKELY(e1 != 0 || e2 != 0))
	{
		if(e1 != 0 && e2 != 0)
            emit_error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::symlink_is_in_same_volume");
		return false;
	}

	return s1.st_dev == s2.st_dev;

#else  // Windows

	// Note well: Physical location on external media is part of the
	// equivalence criteria. If there are no open handles, physical location
	// can change due to defragmentation or other relocations. Thus handles
	// must be held open until location information for both paths has
	// been retrieved.

	// p2 is done first, so any error reported is for p1
	handle_wrapper h2(create_file_handle(
        p2.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
		0));

    handle_wrapper h1(create_file_handle(
        p1.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
		0));

    if (YGGR_FSYS_UNLIKELY(h1.handle == INVALID_HANDLE_VALUE || h2.handle == INVALID_HANDLE_VALUE))
    {
        // if one is invalid and the other isn't, then they aren't equivalent,
        // but if both are invalid then it is an error
        if (h1.handle == INVALID_HANDLE_VALUE && h2.handle == INVALID_HANDLE_VALUE)
            error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::symlink_is_in_same_volume");
        return false;
    }

    // at this point, both handles are known to be valid

    BY_HANDLE_FILE_INFORMATION info1, info2;

    if (error(!::GetFileInformationByHandle(h1.handle, &info1)? YGGR_FSYS_ERRNO() : 0,
				p1, p2, ec, "yggr::file_system::native::symlink_is_in_same_volume"))
    {
        return false;
    }

    if (error(!::GetFileInformationByHandle(h2.handle, &info2)? YGGR_FSYS_ERRNO() : 0,
				p1, p2, ec, "yggr::file_system::native::symlink_is_in_same_volume"))
    {
        return false;
    }

	return info1.dwVolumeSerialNumber == info2.dwVolumeSerialNumber;

#endif // BOOST_POSIX_API
}

inline boost::uintmax_t symlink_hard_link_count(const boost::filesystem::path& p, boost::system::error_code* ec)
{
#ifdef BOOST_POSIX_API

	struct stat path_stat;

	return
		error(
			::lstat(p.c_str(), &path_stat) != 0? YGGR_FSYS_ERRNO() : 0,
			p, ec, "yggr::file_system::native::symlink_hard_link_count")?
		0 :  static_cast<boost::uintmax_t>(path_stat.st_nlink);

#else // Windows

	// Link count info is only available through GetFileInformationByHandle
	BY_HANDLE_FILE_INFORMATION info;
	handle_wrapper h(
		create_file_handle(p.c_str(), FILE_READ_ATTRIBUTES,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, 0));

	return !error(h.handle == INVALID_HANDLE_VALUE? YGGR_FSYS_ERRNO() : 0,
					p, ec, "yggr::file_system::native::symlink_hard_link_count")
			&& !error(!::GetFileInformationByHandle(h.handle, &info)? YGGR_FSYS_ERRNO() : 0,
						p, ec, "yggr::file_system::native::symlink_hard_link_count")?
			info.nNumberOfLinks : 0;

#endif // BOOST_POSIX_API
}

// 0 - link_size, 1 - file_size, 2 - hash
inline yggr::tuple<boost::uintmax_t, boost::uintmax_t, std::size_t>
	hard_link_count_and_size_and_eqhash(const boost::filesystem::path& p,
										boost::system::error_code* ec)
{
	typedef std::pair<boost::uintmax_t, boost::uintmax_t> ret_type;
	std::size_t seed = 0;

#   ifdef BOOST_POSIX_API

	struct stat path_stat;
	if(error(::stat(p.c_str(), &path_stat) != 0? YGGR_FSYS_ERRNO() : 0,
				p, ec, "yggr::file_system::native::hard_link_count_and_size_and_eqhash"))
		return yggr::make_tuple(
			static_cast<boost::uintmax_t>(0),
			static_cast<boost::uintmax_t>(-1),
			seed);

	if(!S_ISREG(path_stat.st_mode))
	{
		return yggr::make_tuple(
			static_cast<boost::uintmax_t>(0),
			static_cast<boost::uintmax_t>(-1),
			seed);
	}

	boost::hash_combine(seed, path_stat.st_dev);
	boost::hash_combine(seed, path_stat.st_ino);
	boost::hash_combine(seed, path_stat.st_size);
	boost::hash_combine(seed, path_stat.st_mtime);

	return yggr::make_tuple(
			static_cast<boost::uintmax_t>(path_stat.st_nlink),
			static_cast<boost::uintmax_t>(path_stat.st_size),
			seed);

#   else  // Windows

	DWORD attr(::GetFileAttributesW(p.c_str()));
	if(attr == 0xFFFFFFFF
		|| attr & FILE_ATTRIBUTE_REPARSE_POINT
		|| attr & FILE_ATTRIBUTE_DIRECTORY)
	{
		return yggr::make_tuple(
			static_cast<boost::uintmax_t>(0),
			static_cast<boost::uintmax_t>(-1),
			seed);
	}

	BY_HANDLE_FILE_INFORMATION info;

	handle_wrapper h(
		create_file_handle(
			p.c_str(), FILE_READ_ATTRIBUTES,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0));

	if(!error(h.handle == INVALID_HANDLE_VALUE? YGGR_FSYS_ERRNO() : 0,
				p, ec, "boost::filesystem::hard_link_count_and_size_and_eqhash")
		&& !error(!::GetFileInformationByHandle(h.handle, &info)? YGGR_FSYS_ERRNO() : 0,
					p, ec, "boost::filesystem::hard_link_count_and_size_and_eqhash"))
	{
		boost::hash_combine(seed, info.dwVolumeSerialNumber);
		boost::hash_combine(seed, info.nFileIndexHigh);
		boost::hash_combine(seed, info.nFileIndexLow);
		boost::hash_combine(seed, info.nFileSizeHigh);
		boost::hash_combine(seed, info.nFileSizeLow);
		boost::hash_combine(seed, info.ftLastWriteTime.dwLowDateTime);
		boost::hash_combine(seed, info.ftLastWriteTime.dwHighDateTime);

		return yggr::make_tuple(
			static_cast<boost::uintmax_t>(info.nNumberOfLinks),
			(static_cast<boost::uintmax_t>(info.nFileSizeHigh)
				<< (sizeof(info.nFileSizeLow) * 8)) + info.nFileSizeLow,
			seed);
	}
	else
	{
		return yggr::make_tuple(
			static_cast<boost::uintmax_t>(0),
			static_cast<boost::uintmax_t>(-1),
			seed);
	}

#   endif // BOOST_POSIX_API
}

} // namespace detail

using boost::filesystem::path;

using boost::filesystem::file_type;
using boost::filesystem::perms;
using boost::filesystem::file_status;
using boost::filesystem::space_info;

using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
using boost::filesystem::recursive_directory_iterator;

#if BOOST_VERSION < 107200

using boost::filesystem::symlink_option;

BOOST_SCOPED_ENUM_UT_DECLARE_BEGIN(directory_options, unsigned int)
{
	none = static_cast< unsigned int >(symlink_option::none),
	skip_permission_denied = none, // not_support
	follow_directory_symlink = static_cast< unsigned int >(symlink_option::recurse),
	skip_dangling_symlinks = none, // not_support
	pop_on_error = none, // not_support

	_detail_no_follow = none, // not_support
	_detail_no_push = static_cast< unsigned int >(symlink_option::_detail_no_push)
}
BOOST_BITMASK(BOOST_SCOPED_ENUM_NATIVE(directory_options))

#else

using boost::filesystem::directory_options;

BOOST_SCOPED_ENUM_UT_DECLARE_BEGIN(symlink_option, unsigned int)
{
    none = static_cast< unsigned int >(directory_options::none),
    no_recurse = none,                                                                  // don't follow directory symlinks (default behavior)
    recurse = static_cast< unsigned int >(directory_options::follow_directory_symlink), // follow directory symlinks
    _detail_no_push = static_cast< unsigned int >(directory_options::_detail_no_push)   // internal use only
}
BOOST_SCOPED_ENUM_DECLARE_END(symlink_option)

BOOST_BITMASK(BOOST_SCOPED_ENUM_NATIVE(symlink_option))

#endif //BOOST_VERSION < 107200

#if (BOOST_VERSION < 107400)

namespace detail
{
	using boost::filesystem::copy_option;
} // namespace detail

BOOST_SCOPED_ENUM_UT_DECLARE_BEGIN(copy_options, unsigned int)
{
    none = 0u, // Default. For copy_file: error if the target file exists. For copy: do not recurse, follow symlinks, copy file contents.

    // copy_file options:
    skip_existing = none,       // not support
    overwrite_existing = detail::copy_option::overwrite_if_exists, // Overwrite existing file
    update_existing = none,    // not support
    synchronize_data = none,   // not support
    synchronize = none,        // not support

    // copy options:
    recursive = none,          // Recurse into sub-directories
    copy_symlinks = none,      // Copy symlinks as symlinks instead of copying the referenced file
    skip_symlinks = none,     // Don't copy symlinks
    directories_only = none,  // Only copy directory structure, do not copy non-directory files
    create_symlinks = none,   // Create symlinks instead of copying files
    create_hard_links = none, // Create hard links instead of copying files
    _detail_recursing = none  // Internal use only, do not use
}
BOOST_SCOPED_ENUM_DECLARE_END(copy_options)

BOOST_BITMASK(BOOST_SCOPED_ENUM_NATIVE(copy_options))

#else

using boost::filesystem::copy_options;

#endif // BOOST_VERSION < 107400

using boost::filesystem::filesystem_error;
using boost::system::error_code;

using boost::filesystem::absolute;
using boost::filesystem::canonical;

using boost::filesystem::copy;
using boost::filesystem::copy_directory;
using boost::filesystem::copy_file;
using boost::filesystem::copy_symlink;

using boost::filesystem::create_directories;
using boost::filesystem::create_directory;
using boost::filesystem::create_hard_link;
using boost::filesystem::create_symlink;
using boost::filesystem::create_directory_symlink;

using boost::filesystem::current_path;
using boost::filesystem::exists;
using boost::filesystem::equivalent;

inline std::size_t equivalent_hash_value(const path& p)
{
	return detail::equivalent_hash_value(p, 0);
}

inline std::size_t equivalent_hash_value(const path& p, error_code& ec)
{
	return detail::equivalent_hash_value(p, boost::addressof(ec));
}

inline std::size_t equivalent_hash_value_if_has_hard_link(const path& p)
{
	return detail::equivalent_hash_value_if_has_hard_link(p, 0);
}

inline std::size_t equivalent_hash_value_if_has_hard_link(const path& p, error_code& ec)
{
	return detail::equivalent_hash_value_if_has_hard_link(p, boost::addressof(ec));
}

inline bool symlink_equivalent(const path& p1, const path& p2)
{
	return detail::symlink_equivalent(p1, p2, 0);
}

inline bool symlink_equivalent(const path& p1, const path& p2, error_code& ec)
{
	return detail::symlink_equivalent(p1, p2, boost::addressof(ec));
}

inline std::size_t symlink_equivalent_hash_value(const path& p)
{
	return detail::symlink_equivalent_hash_value(p, 0);
}

inline std::size_t symlink_equivalent_hash_value(const path& p, error_code& ec)
{
	return detail::symlink_equivalent_hash_value(p, boost::addressof(ec));
}

inline std::size_t symlink_equivalent_hash_value_if_has_hard_link(const path& p)
{
	return detail::symlink_equivalent_hash_value_if_has_hard_link(p, 0);
}

inline std::size_t symlink_equivalent_hash_value_if_has_hard_link(const path& p, error_code& ec)
{
	return detail::symlink_equivalent_hash_value_if_has_hard_link(p, boost::addressof(ec));
}

using boost::filesystem::file_size;

#if (BOOST_VERSION < 108000)

BOOST_FILESYSTEM_DECL
uintmax_t hard_link_count(path const& p, error_code* ec)
{
    if (ec)
        ec->clear();

#if defined(BOOST_POSIX_API)

    struct ::stat path_stat;
    if(YGGR_FSYS_UNLIKELY(::stat(p.c_str(), &path_stat) < 0))
    {
        emit_error(errno, p, ec, "yggr::file_system::native::hard_link_count");
        return static_cast< uintmax_t >(-1);
    }

    return static_cast< uintmax_t >(path_stat.st_nlink);

#else // defined(BOOST_POSIX_API)

    handle_wrapper h(create_file_handle(
        p.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS));

    if(YGGR_FSYS_UNLIKELY(h.handle == INVALID_HANDLE_VALUE))
    {
    fail_errno:
        emit_error(BOOST_ERRNO, p, ec, "yggr::file_system::native::hard_link_count");
        return static_cast< uintmax_t >(-1);
    }

    // Link count info is only available through GetFileInformationByHandle
    BY_HANDLE_FILE_INFORMATION info;
    if (YGGR_FSYS_UNLIKELY(!::GetFileInformationByHandle(h.handle, &info)))
        goto fail_errno;

    return static_cast< uintmax_t >(info.nNumberOfLinks);

#endif // defined(BOOST_POSIX_API)
}

#else

using boost::filesystem::hard_link_count;

#endif // (BOOST_VERSION < 108000)


inline yggr::tuple<boost::uintmax_t, boost::uintmax_t, std::size_t>
	hard_link_count_and_size_and_eqhash(const path& p)
{
	return detail::hard_link_count_and_size_and_eqhash(p, 0);
}

inline yggr::tuple<boost::uintmax_t, boost::uintmax_t, std::size_t>
	hard_link_count_and_size_and_eqhash(const path& p, error_code& ec)
{
	return detail::hard_link_count_and_size_and_eqhash(p, boost::addressof(ec));
}

inline boost::uintmax_t symlink_hard_link_count(const path& p)
{
	return detail::symlink_hard_link_count(p, 0);
}

inline boost::uintmax_t symlink_hard_link_count(const path& p, error_code& ec)
{
	return detail::symlink_hard_link_count(p, boost::addressof(ec));
}

using boost::filesystem::initial_path;

using boost::filesystem::is_directory;
using boost::filesystem::is_empty;
using boost::filesystem::is_other;
using boost::filesystem::is_regular_file;
using boost::filesystem::is_symlink;

inline bool is_in_same_volume(const path& p1, const path& p2)
{
	return detail::is_in_same_volume(p1, p2, 0);
}

inline bool is_in_same_volume(const path& p1, const path& p2, error_code& ec)
{
	return detail::is_in_same_volume(p1, p2, boost::addressof(ec));
}

inline bool symlink_is_in_same_volume(const path& p1, const path& p2)
{
	return detail::symlink_is_in_same_volume(p1, p2, 0);
}

inline bool symlink_is_in_same_volume(const path& p1, const path& p2, error_code& ec)
{
	return detail::symlink_is_in_same_volume(p1, p2, boost::addressof(ec));
}

using boost::filesystem::last_write_time;
using boost::filesystem::permissions;
using boost::filesystem::read_symlink;

using boost::filesystem::remove;
using boost::filesystem::remove_all;
using boost::filesystem::rename;

using boost::filesystem::resize_file;
using boost::filesystem::space;
using boost::filesystem::status;
using boost::filesystem::status_known;
using boost::filesystem::symlink_status;
using boost::filesystem::system_complete;
using boost::filesystem::temp_directory_path;
using boost::filesystem::unique_path;

} // namespace native
} // namespace file_system
} // namespace yggr

#define YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_0_EC(__ret__, __name__, __args_t__, __args__) \
	inline __ret__ BOOST_PP_CAT(call_, __name__)(error_code* ec = 0) { \
		return ec? __name__(*ec) : __name__(); }

#define YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(__ret__, __name__, __args_t__, __args__) \
	inline __ret__ BOOST_PP_CAT(call_, __name__)(YGGR_PP_OPEN(__args_t__), error_code* ec = 0) { \
		return ec? __name__( YGGR_PP_OPEN(__args__), *ec ) : __name__( YGGR_PP_OPEN(__args__) ); }

namespace yggr
{
namespace file_system
{
namespace native
{

//path absolute(const path& p, const path& base=current_path()); // non-ec don't support it

//path canonical(const path& p, const path& base = current_path());
//path canonical(const path& p, system::error_code& ec);
//path canonical(const path& p, const path& base, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(path, canonical, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p, const path& base
#define YGGR_TMP_PP_FSYS_ARGS() p, base

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(path, canonical, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void copy(const path& from, const path& to);
//void copy(const path& from, const path& to, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& from, const path& to
#define YGGR_TMP_PP_FSYS_ARGS() from, to

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, copy, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void copy_directory(const path& from, const path& to);
//void copy_directory(const path& from, const path& to, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& from, const path& to
#define YGGR_TMP_PP_FSYS_ARGS() from, to

#if BOOST_VERSION < 107400
	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, copy_directory, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)
#else
	inline bool call_copy_directory(const path& from, const path& to, error_code* ec = 0)
	{
		return ec? create_directory(to, from, *ec) : create_directory(to, from);
	}
#endif // BOOST_VERSION < 107400

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

// if boost_version < 1.74.0
//	void copy_file(const path& from, const path& to);
//	void copy_file(const path& from, const path& to, system::error_code& ec);
//	void copy_file(const path& from, const path& to, copy_options option);
//	void copy_file(const path& from, const path& to, copy_options option, system::error_code& ec);
// else
//	bool copy_file(const path& from, const path& to);
//	bool copy_file(const path& from, const path& to, system::error_code& ec);
//	bool copy_file(const path& from, const path& to, copy_options option);
//	bool copy_file(const path& from, const path& to, copy_options option, system::error_code& ec);
// endif //boost_version < 1.74.0

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& from, const path& to
#define YGGR_TMP_PP_FSYS_ARGS() from, to

#if (BOOST_VERSION < 107400)
	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, copy_file, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)
#else
	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, copy_file, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)
#endif // (BOOST_VERSION < 107400)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T


#if (BOOST_VERSION < 107400)

#	define YGGR_TMP_PP_FSYS_ARGS_T() const path& from, const path& to, BOOST_SCOPED_ENUM(detail::copy_option) option
#	define YGGR_TMP_PP_FSYS_ARGS() from, to, option

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, copy_file, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

	inline void call_copy_file(const path& from, const path& to,  BOOST_SCOPED_ENUM(copy_options) opt, error_code* ec = 0)
	{
		return
			ec?
			copy_file(from, to, static_cast<BOOST_SCOPED_ENUM(detail::copy_option)>(opt), *ec)
			: copy_file(from, to, static_cast<BOOST_SCOPED_ENUM(detail::copy_option)>(opt));
	}

#else

#	define YGGR_TMP_PP_FSYS_ARGS_T() const path& from, const path& to, BOOST_SCOPED_ENUM(copy_options) options
#	define YGGR_TMP_PP_FSYS_ARGS() from, to, options

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, copy_file, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)
#endif // (BOOST_VERSION < 107400)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void copy_symlink(const path& existing_symlink, const path& new_symlink);
//void copy_symlink(const path& existing_symlink, const path& new_symlink, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& existing_symlink, const path& new_symlink
#define YGGR_TMP_PP_FSYS_ARGS() existing_symlink, new_symlink

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, copy_symlink, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool create_directories(const path& p);
//bool create_directories(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, create_directories, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool create_directory(const path& p);
//bool create_directory(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, create_directory, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void create_directory_symlink(const path& to, const path& new_symlink);
//void create_directory_symlink(const path& to, const path& new_symlink, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& to, const path& new_symlink
#define YGGR_TMP_PP_FSYS_ARGS() to, new_symlink

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, create_directory_symlink, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void create_hard_link(const path& to, const path& new_hard_link);
//void create_hard_link(const path& to, const path& new_hard_link, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& to, const path& new_hard_link
#define YGGR_TMP_PP_FSYS_ARGS() to, new_hard_link

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, create_hard_link, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void create_symlink(const path& to, const path& new_symlink);
//void create_symlink(const path& to, const path& new_symlink, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& to, const path& new_symlink
#define YGGR_TMP_PP_FSYS_ARGS() to, new_symlink

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, create_symlink, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//path current_path();
//path current_path(system::error_code& ec);
//void current_path(const path& p);
//void current_path(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T()
#define YGGR_TMP_PP_FSYS_ARGS()

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_0_EC(path, current_path, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, current_path, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool exists(file_status s) noexcept; // non-ec don't support it
//bool exists(const path& p);
//bool exists(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, exists, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool equivalent(const path& p1, const path& p2);
//bool equivalent(const path& p1, const path& p2, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p1, const path& p2
#define YGGR_TMP_PP_FSYS_ARGS() p1, p2

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, equivalent, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

// inline std::size_t equivalent_hash_value(const path& p);
// inline std::size_t equivalent_hash_value(const path& p, error_code& ec);

inline std::size_t call_equivalent_hash_value(const path& p, error_code* ec)
{
	return detail::equivalent_hash_value(p, ec);
}

// inline std::size_t equivalent_hash_value_if_has_hard_link(const path& p);
// inline std::size_t equivalent_hash_value_if_has_hard_link(const path& p, error_code& ec);

inline std::size_t call_equivalent_hash_value_if_has_hard_link(const path& p, error_code* ec)
{
	return detail::equivalent_hash_value_if_has_hard_link(p, ec);
}

//bool symlink_equivalent(const path& p1, const path& p2);
//bool symlink_equivalent(const path& p1, const path& p2, system::error_code& ec);

inline bool call_symlink_equivalent(const path& p1, const path& p2, error_code* ec)
{
	return detail::symlink_equivalent(p1, p2, ec);
}

// inline std::size_t symlink_equivalent_hash_value(const path& p);
// inline std::size_t symlink_equivalent_hash_value(const path& p, error_code& ec);

inline std::size_t call_symlink_equivalent_hash_value(const path& p, error_code* ec)
{
	return detail::symlink_equivalent_hash_value(p, ec);
}

// inline std::size_t symlink_equivalent_hash_value_if_has_hard_link(const path& p);
// inline std::size_t symlink_equivalent_hash_value_if_has_hard_link(const path& p, error_code& ec);

inline std::size_t call_symlink_equivalent_hash_value_if_has_hard_link(const path& p, error_code* ec)
{
	return detail::symlink_equivalent_hash_value_if_has_hard_link(p, ec);
}

//uintmax_t file_size(const path& p);
//uintmax_t file_size(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(boost::uintmax_t, file_size, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//uintmax_t hard_link_count(const path& p);
//uintmax_t hard_link_count(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(boost::uintmax_t, hard_link_count, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//inline std::pair<boost::uintmax_t, boost::uintmax_t>
//	hard_link_count_and_size_and_eqhash(const path& p);
//
//inline std::pair<boost::uintmax_t, boost::uintmax_t>
//	hard_link_count_and_size_and_eqhash(const path& p, error_code& ec);

inline yggr::tuple<boost::uintmax_t, boost::uintmax_t, std::size_t>
	call_hard_link_count_and_size_and_eqhash(const path& p, error_code* ec)
{
	return detail::hard_link_count_and_size_and_eqhash(p, ec);
}

//uintmax_t symlink_hard_link_count(const path& p);
//uintmax_t symlink_hard_link_count(const path& p, system::error_code& ec);

inline boost::uintmax_t call_symlink_hard_link_count(const path& p, error_code* ec)
{
	return detail::symlink_hard_link_count(p, ec);
}

//path initial_path(); // document is error return value is "path" not "const path&"
//path initial_path(system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T()
#define YGGR_TMP_PP_FSYS_ARGS()

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_0_EC(path, initial_path, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool is_directory(file_status s) noexcept; // non-ec don't support it
//bool is_directory(const path& p);
//bool is_directory(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, is_directory, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool is_empty(const path& p);
//bool is_empty(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, is_empty, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool is_regular_file(file_status s) noexcept; // non-ec don't support it
//bool is_regular_file(const path& p);
//bool is_regular_file(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, is_regular_file, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool is_other(file_status s) noexcept; // non-ec don't support it
//bool is_other(const path& p);
//bool is_other(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, is_other, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool is_symlink(file_status s) noexcept; // non-ec don't support it
//bool is_symlink(const path& p);
//bool is_symlink(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, is_symlink, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//inline bool is_in_same_volume(const path& p1, const path& p2);
//inline bool is_in_same_volume(const path& p1, const path& p2, error_code& ec);

inline bool call_is_in_same_volume(const path& p1, const path& p2, error_code* ec)
{
	return detail::is_in_same_volume(p1, p2, ec);
}

//inline bool symlink_is_in_same_volume(const path& p1, const path& p2);
//inline bool symlink_is_in_same_volume(const path& p1, const path& p2, error_code& ec);

inline bool call_symlink_is_in_same_volume(const path& p1, const path& p2, error_code* ec)
{
	return detail::symlink_is_in_same_volume(p1, p2, ec);
}

//std::time_t last_write_time(const path& p);
//std::time_t last_write_time(const path& p, system::error_code& ec);
//void last_write_time(const path& p, const std::time_t new_time);
//void last_write_time(const path& p, const std::time_t new_time, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(std::time_t, last_write_time, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p, const std::time_t new_time
#define YGGR_TMP_PP_FSYS_ARGS() p, new_time

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, last_write_time, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void permissions(const path& p, perms prms);
//void permissions(const path& p, perms prms, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p, perms prms
#define YGGR_TMP_PP_FSYS_ARGS() p, prms

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, permissions, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//path read_symlink(const path& p);
//path read_symlink(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(path, read_symlink, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool remove(const path& p);
//bool remove(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(bool, remove, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//uintmax_t remove_all(const path& p);
//uintmax_t remove_all(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(boost::uintmax_t, remove_all, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void rename(const path& old_p, const path& new_p);
//void rename(const path& old_p, const path& new_p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& old_p, const path& new_p
#define YGGR_TMP_PP_FSYS_ARGS() old_p, new_p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, rename, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//void resize_file(const path& p, uintmax_t new_size);
//void resize_file(const path& p, uintmax_t new_size, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p, boost::uintmax_t new_size
#define YGGR_TMP_PP_FSYS_ARGS() p, new_size

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(void, resize_file, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//space_info space(const path& p);
//space_info space(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(space_info, space, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//bool status_known(file_status s) noexcept; // non-ec don't support it

//file_status status(const path& p);
//file_status status(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(file_status, status, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

inline file_status call_status_ex(const path& p, boost::system::error_code* ec)
{
	try
	{
		return status(p);
	}
	catch(const filesystem_error& e)
	{
		if(!ec)
		{
			throw e;
		}
		else
		{
			*ec = e.code();
			return file_status();
		}
	}
}


//file_status symlink_status(const path& p);
//file_status symlink_status(const path& p, system::error_code& ec) noexcept;

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(file_status, symlink_status, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

inline file_status call_symlink_status_ex(const path& p, boost::system::error_code* ec)
{
	try
	{
		return symlink_status(p);
	}
	catch(const filesystem_error& e)
	{
		if(!ec)
		{
			throw e;
		}
		else
		{
			*ec = e.code();
			return file_status();
		}
	}
}


//path system_complete(const path& p);
//path system_complete(const path& p, system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& p
#define YGGR_TMP_PP_FSYS_ARGS() p

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(path, system_complete, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//path temp_directory_path();
//path temp_directory_path(system::error_code& ec);

#define YGGR_TMP_PP_FSYS_ARGS_T()
#define YGGR_TMP_PP_FSYS_ARGS()

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_0_EC(path, temp_directory_path, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T

//path unique_path(const path& model="%%%%-%%%%-%%%%-%%%%");
//path unique_path(const path& model, system::error_code& ec);

inline path call_unique_path(error_code* ec = 0)
{
	return ec? unique_path("%%%%-%%%%-%%%%-%%%%", *ec) : unique_path();
}

#define YGGR_TMP_PP_FSYS_ARGS_T() const path& model
#define YGGR_TMP_PP_FSYS_ARGS() model

	YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC(path, unique_path, YGGR_TMP_PP_FSYS_ARGS_T, YGGR_TMP_PP_FSYS_ARGS)

#undef YGGR_TMP_PP_FSYS_ARGS
#undef YGGR_TMP_PP_FSYS_ARGS_T


} // namespace native
} // namespace file_system
} // namespace yggr

// extended functions

namespace yggr
{
namespace file_system
{
namespace native
{

//directory_iterator() noexcept;  // non-ec don't support it
//directory_iterator(const directory_iterator&); // non-ec don't support it
//explicit directory_iterator(const path& p);
//directory_iterator(const path& p, system::error_code& ec);

inline directory_iterator
	emplace_directory_iterator(const path& p, error_code* ec = 0)
{
	return ec? directory_iterator(p, *ec) : directory_iterator(p);
}

//recursive_directory_iterator() noexcept; // non-ec don't support it
//recursive_directory_iterator(const recursive_directory_iterator&); // non-ec don't support it
//explicit recursive_directory_iterator(const path& p,  symlink_option opt = symlink_option::none);
//recursive_directory_iterator(const path& p, symlink_option opt, system::error_code& ec);
//recursive_directory_iterator(const path& p, system::error_code& ec);

inline recursive_directory_iterator
	emplace_recursive_directory_iterator(const path& p, error_code* ec)
{
#if BOOST_VERSION < 107200
	return
		ec?
			recursive_directory_iterator(p, symlink_option::none, *ec)
			: recursive_directory_iterator(p);
#else
	return
		ec?
			recursive_directory_iterator(p, directory_options::none, *ec)
			: recursive_directory_iterator(p);
#endif // BOOST_VERSION < 107200
}

#if BOOST_VERSION < 107200

inline recursive_directory_iterator
	emplace_recursive_directory_iterator(const path& p,
											BOOST_SCOPED_ENUM(symlink_option) opt = symlink_option::none,
											error_code* ec = 0)
{
	return ec? recursive_directory_iterator(p, opt, *ec) : recursive_directory_iterator(p, opt);
}

inline recursive_directory_iterator
	emplace_recursive_directory_iterator(const path& p,
											BOOST_SCOPED_ENUM(directory_options) opt,
											error_code* ec = 0)
{
	return
		ec?
		recursive_directory_iterator(p, static_cast<BOOST_SCOPED_ENUM(symlink_option)>(opt), *ec)
		: recursive_directory_iterator(p, static_cast<BOOST_SCOPED_ENUM(symlink_option)>(opt));
}

#else

inline recursive_directory_iterator
	emplace_recursive_directory_iterator(const path& p,
											BOOST_SCOPED_ENUM(directory_options) opt = directory_options::none,
											error_code* ec = 0)
{
	return ec? recursive_directory_iterator(p, opt, *ec) : recursive_directory_iterator(p, opt);
}

inline recursive_directory_iterator
	emplace_recursive_directory_iterator(const path& p,
											BOOST_SCOPED_ENUM(symlink_option) opt,
											error_code* ec = 0)
{
	return
		ec?
		recursive_directory_iterator(p, static_cast<BOOST_SCOPED_ENUM(directory_options)>(opt), *ec)
		: recursive_directory_iterator(p, static_cast<BOOST_SCOPED_ENUM(directory_options)>(opt));
}

#endif // BOOST_VERSION < 107200


//file_status  directory_entry::status() const;
//file_status  directory_entry::status(system::error_code& ec) const;

inline file_status directory_entry_status(const directory_entry& de, error_code* ec = 0)
{
	return ec? de.status(*ec) : de.status();
}

//file_status  directory_entry::symlink_status() const;
//file_status  directory_entry::symlink_status(system::error_code& ec) const;

inline file_status directory_entry_symlink_status(const directory_entry& de, error_code* ec = 0)
{
	return ec? de.symlink_status(*ec) : de.symlink_status();
}

//directory_iterator& directory_iterator::increment(system::error_code& ec);
//recursive_directory_iterator& directory_iterator::increment(system::error_code& ec);

template<typename DirIter> inline
DirIter& directory_iterator_increment(DirIter& iter, error_code* ec = 0)
{
	return ec? iter.increment(*ec) : ++iter;
}

} // namespace native
} // namespace file_system
} // namespace yggr

#undef YGGR_TMP_PP_FSYS_CALL_NATIVE_FOO_N_EC

#endif // __YGGR_FILE_SYSTEM_LOCAL_FSYS_NATIVE_OPERATORS_HPP__
