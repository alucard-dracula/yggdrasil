//test_boost_filesystem.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#if defined(BOOST_POSIX_API)

#   include <sys/types.h>
#   include <sys/stat.h>

#	define YGGR_FSYS_ERRNO() errno
#	define YGGR_FSYS_ERROR_PATH_NAME_FAILED() ENOENT
#	define YGGR_FSYS_ERROR_ALREADY_EXISTS() EEXIST
#	define YGGR_FSYS_ERROR_NOT_SUPPORTED() ENOSYS

typedef int yggr_fsys_err_t;

#else  // BOOST_WINDOWS_API

#include <windows.h>

#	define YGGR_FSYS_ERRNO() ::GetLastError()
#	define YGGR_FSYS_ERROR_PATH_NAME_FAILED() ERROR_PATH_NOT_FOUND
#   define YGGR_FSYS_ERROR_ALREADY_EXISTS() ERROR_ALREADY_EXISTS
#	define YGGR_FSYS_ERROR_NOT_SUPPORTED() ERROR_NOT_SUPPORTED

typedef boost::winapi::DWORD_ yggr_fsys_err_t;

# endif // BOOST_POSIX_API

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void emit_error(yggr_fsys_err_t error_num, boost::system::error_code* ec, const char* message)
{
    if (!ec)
        BOOST_FILESYSTEM_THROW(
			boost::filesystem::filesystem_error(
				message, boost::system::error_code(error_num, boost::system::system_category())));
    else
        *ec = boost::system::error_code(error_num, boost::system::system_category());
}

void emit_error(yggr_fsys_err_t error_num, boost::filesystem::path const& p, boost::system::error_code* ec, const char* message)
{
    if (!ec)
        BOOST_FILESYSTEM_THROW(
			boost::filesystem::filesystem_error(
				message, p, boost::system::error_code(error_num, boost::system::system_category())));
    else
       *ec = boost::system::error_code(error_num, boost::system::system_category());
}

void emit_error(yggr_fsys_err_t error_num, boost::filesystem::path const& p1, boost::filesystem::path const& p2, boost::system::error_code* ec, const char* message)
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
    if ((!error_num))
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

inline bool error(yggr_fsys_err_t error_num, boost::filesystem::path const& p, boost::system::error_code* ec, const char* message)
{
    if ((!error_num))
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

inline bool error(yggr_fsys_err_t error_num, boost::filesystem::path const& p1, boost::filesystem::path const& p2, boost::system::error_code* ec, const char* message)
{
    if ((!error_num))
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
	yggr_test_assert(p);
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
	yggr_test_assert(p);
	return ::CreateFileW(p, dwDesiredAccess, dwShareMode,
							lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
							hTemplateFile);
}

#endif // BOOST_POSIX_API

inline bool symlink_equivalent(const boost::filesystem::path& p1,
								const boost::filesystem::path& p2,
								boost::system::error_code* ec)
{
#if defined(BOOST_POSIX_API)

    // p2 is done first, so any error reported is for p1

    struct ::stat s2;
    int e2 = ::lstat(p2.c_str(), &s2);
    struct ::stat s1;
    int e1 = ::lstat(p1.c_str(), &s1);

    if ((e1 != 0 || e2 != 0))
    {
        // if one is invalid and the other isn't then they aren't equivalent,
        // but if both are invalid then it is an error
        if (e1 != 0 && e2 != 0)
            emit_error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::symlink_equivalent");
        return false;
    }

	return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;

#else // Windows

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

    if ((h1.handle == INVALID_HANDLE_VALUE || h2.handle == INVALID_HANDLE_VALUE))
    {
        // if one is invalid and the other isn't, then they aren't equivalent,
        // but if both are invalid then it is an error
        if (h1.handle == INVALID_HANDLE_VALUE && h2.handle == INVALID_HANDLE_VALUE)
            error(YGGR_FSYS_ERRNO(), p1, p2, ec, "yggr::file_system::native::symlink_equivalent");
        return false;
    }

    // at this point, both handles are known to be valid

    BY_HANDLE_FILE_INFORMATION info1, info2;

    if (error(!::GetFileInformationByHandle(h1.handle, &info1)? YGGR_FSYS_ERRNO() : 0, p1, p2, ec, "yggr::file_system::native::symlink_equivalent"))
        return false;

    if (error(!::GetFileInformationByHandle(h2.handle, &info2)? YGGR_FSYS_ERRNO() : 0, p1, p2, ec, "yggr::file_system::native::symlink_equivalent"))
        return false;

    // In theory, volume serial numbers are sufficient to distinguish between
    // devices, but in practice VSN's are sometimes duplicated, so last write
    // time and file size are also checked.
    return info1.dwVolumeSerialNumber == info2.dwVolumeSerialNumber &&
        info1.nFileIndexHigh == info2.nFileIndexHigh &&
        info1.nFileIndexLow == info2.nFileIndexLow &&
        info1.nFileSizeHigh == info2.nFileSizeHigh &&
        info1.nFileSizeLow == info2.nFileSizeLow &&
        info1.ftLastWriteTime.dwLowDateTime == info2.ftLastWriteTime.dwLowDateTime &&
        info1.ftLastWriteTime.dwHighDateTime == info2.ftLastWriteTime.dwHighDateTime;

#endif
}

inline boost::uintmax_t symlink_hard_link_count(const boost::filesystem::path& p, boost::system::error_code* ec)
{
#ifdef BOOST_POSIX_API

	struct stat path_stat;
	return 
		error(
			::lstat(p.c_str(), &path_stat) != 0? YGGR_FSYS_ERRNO() : 0,
			p, ec, "yggr::file_system::native::symlink_hard_link_count")? 
		0 : static_cast<boost::uintmax_t>(path_stat.st_nlink);

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

void test(void)
{
	boost::filesystem::path root_dir_path("../test_out/file_system");
	boost::filesystem::path src_file_path("./file_system/test_file_dont_remove.txt");
	boost::filesystem::path file_path = (root_dir_path / "file.txt");
	boost::filesystem::path symlink_path = (root_dir_path / "symlink");

	boost::filesystem::path hard_link_path = (root_dir_path / "hfile.txt");

	try
	{
		boost::filesystem::remove_all(root_dir_path);
	}
	catch(...)
	{
	}

	try
	{
		boost::filesystem::create_directories(root_dir_path);
#	if BOOST_VERSION < 107400
		boost::filesystem::copy_file(src_file_path, file_path, boost::filesystem::copy_option::overwrite_if_exists);
#	else
		boost::filesystem::copy_file(src_file_path, file_path, boost::filesystem::copy_options::overwrite_existing);
#	endif // BOOST_VERSION < 107400

		boost::filesystem::create_symlink("file.txt", symlink_path);
#	if !(defined(__ANDROID__) || defined(ANDROID))
		boost::filesystem::create_hard_link(file_path, hard_link_path); // if using android, need root
#	endif // #	if !(defined(__ANDROID__) || defined(ANDROID))
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(false);
	}

	bool b = false;
	b = boost::filesystem::equivalent(file_path, symlink_path);
	std::cout << b << std::endl;

	try
	{
		boost::system::error_code ec;
		b = boost::filesystem::equivalent(symlink_path, symlink_path, ec);
		std::cout << b << std::endl;
	}
	catch(...)
	{
		std::cout << "throw" << std::endl;
	}

	b = boost::filesystem::equivalent(file_path, hard_link_path);
	std::cout << b << std::endl;

	std::cout << "-----------------" << std::endl;

	b = symlink_equivalent(file_path, symlink_path, 0);
	std::cout << b << std::endl;

	try
	{
		boost::system::error_code ec;
		b = symlink_equivalent(symlink_path, symlink_path,  0);
		std::cout << b << std::endl;
	}
	catch(...)
	{
		std::cout << "throw" << std::endl;
	}

	b = symlink_equivalent(file_path, hard_link_path, 0);
	std::cout << b << std::endl;
}

void test_path_object(void)
{
	typedef boost::filesystem::path path_type;
	{
		path_type path_win("c:\\aaa\\bbb");
		std::cout << "path_win root_name = " << path_win.root_name() << std::endl;
		std::cout << "path_win root_directory = " << path_win.root_directory() << std::endl;
		std::cout << "path_win root_path = " << path_win.root_path() << std::endl;
		std::cout << "path_win is_absolute = " << path_win.is_absolute() << std::endl;
		std::cout << "path_win is_relative = " << path_win.is_relative() << std::endl;

		std::cout << "-------------" << std::endl;

		path_type path_unix("/aaa/bbb");
		std::cout << "path_unix root_name = " << path_unix.root_name() << std::endl;
		std::cout << "path_unix root_directory = " << path_unix.root_directory() << std::endl;
		std::cout << "path_unix root_path = " << path_unix.root_path() << std::endl;
		std::cout << "path_unix is_absolute = " << path_unix.is_absolute() << std::endl;
		std::cout << "path_unix is_relative = " << path_unix.is_relative() << std::endl;

		std::cout << "-------------" << std::endl;

		path_type path_relative("./aaa/bbb");
		std::cout << "path_relative root_name = " << path_relative.root_name() << std::endl;
		std::cout << "path_relative root_directory = " << path_relative.root_directory() << std::endl;
		std::cout << "path_relative root_path = " << path_relative.root_path() << std::endl;
		std::cout << "path_relative is_absolute = " << path_relative.is_absolute() << std::endl;
		std::cout << "path_relative is_relative = " << path_relative.is_relative() << std::endl;

		std::cout << "-------------" << std::endl;

		path_type path_relative2("../aaa/bbb");
		std::cout << "path_relative2 root_name = " << path_relative2.root_name() << std::endl;
		std::cout << "path_relative2 root_directory = " << path_relative2.root_directory() << std::endl;
		std::cout << "path_relative2 root_path = " << path_relative2.root_path() << std::endl;
		std::cout << "path_relative2 is_absolute = " << path_relative2.is_absolute() << std::endl;
		std::cout << "path_relative2 is_relative = " << path_relative2.is_relative() << std::endl;

		std::cout << "-------------" << std::endl;

		path_type path_relative3("../aaa/bbb.txt");
		std::cout << "path_relative3 parent_path = " << path_relative3.parent_path() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	test();
	test_path_object();

	wait_any_key(argc, argv);
	return 0;
}
