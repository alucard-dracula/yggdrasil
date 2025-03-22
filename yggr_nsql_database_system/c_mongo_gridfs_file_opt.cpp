// c_mongo_gridfs_file_opt.cpp

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

#include <yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(void)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
}

c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(const char* fname)
	: _fname(fname)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
	base_type::filename = _fname.empty()? 0 : _fname.data();
}

#ifndef YGGR_NO_CWCHAR
c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(const wchar_t* fname)
	: _fname(fname)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
	base_type::filename = _fname.empty()? 0 : _fname.data();
}
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(const c8* fname)
	: _fname(fname)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
	base_type::filename = _fname.empty()? 0 : _fname.data();
}
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(const c16* fname)
	: _fname(fname)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
	base_type::filename = _fname.empty()? 0 : _fname.data();
}
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(const c32* fname)
	: _fname(fname)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
	base_type::filename = _fname.empty()? 0 : _fname.data();
}
#endif // YGGR_NO_CHAR32_T

c_mongo_gridfs_file_opt::c_mongo_gridfs_file_opt(const this_type& right)
	: _md5(right._md5),
		_fname(right._fname),
		_content(right._content),
		_aliases(right._aliases),
		_metadata(right._metadata)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));

	base_type::md5 = _md5.empty()? 0 : _md5.data();
	base_type::filename = _fname.empty()? 0 : _fname.data();
	base_type::content_type = _content.empty()? 0 : _content.data();
	base_type::aliases = _aliases.empty()? 0 : &_aliases;
	base_type::metadata = _metadata.empty()? 0 : &_metadata;
	base_type::chunk_size = right.chunk_size;
}

c_mongo_gridfs_file_opt::~c_mongo_gridfs_file_opt(void)
{
}

c_mongo_gridfs_file_opt::this_type& 
	c_mongo_gridfs_file_opt::operator=(const base_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_md5 = right.md5? right.md5 : "";
	_fname = right.filename? right.filename : "";
	_content = right.content_type? right.content_type : "";

	if(right.aliases)
	{
		_aliases = *right.aliases;
	}
	else
	{
		_aliases.clear();
	}

	if(right.metadata)
	{
		_metadata = *right.metadata;
	}
	else
	{
		_metadata.clear();
	}

	base_type::md5 = _md5.empty()? 0 : _md5.data();
	base_type::filename = _fname.empty()? 0 : _fname.data();
	base_type::content_type = _content.empty()? 0 : _content.data();
	base_type::aliases = _aliases.empty()? 0 : &_aliases;
	base_type::metadata = _metadata.empty()? 0 : &_metadata;

	base_type::chunk_size = right.chunk_size;

	return *this;
}

c_mongo_gridfs_file_opt::this_type& 
	c_mongo_gridfs_file_opt::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_md5 = right._md5;
	_fname = right._fname;
	_content = right._content;
	_aliases = right._aliases;
	_metadata = right._metadata;

	base_type::md5 = _md5.empty()? 0 : _md5.data();
	base_type::filename = _fname.empty()? 0 : _fname.data();
	base_type::content_type = _content.empty()? 0 : _content.data();
	base_type::aliases = _aliases.empty()? 0 : &_aliases;
	base_type::metadata = _metadata.empty()? 0 : &_metadata;

	base_type::chunk_size = right.chunk_size;

	return *this;
}

void c_mongo_gridfs_file_opt::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	_md5.swap(right._md5);
	_fname.swap(right._fname);
	_content.swap(right._content);
	_aliases.swap(right._aliases);
	_metadata.swap(right._metadata);

	base_type::md5 = _md5.empty()? 0 : _md5.data();
	base_type::filename = _fname.empty()? 0 : _fname.data();
	base_type::content_type = _content.empty()? 0 : _content.data();
	base_type::aliases = _aliases.empty()? 0 : &_aliases;
	base_type::metadata = _metadata.empty()? 0 : &_metadata;

	right.md5 = right._md5.empty()? 0 : right._md5.data();
	right.filename = right._fname.empty()? 0 : right._fname.data();
	right.content_type = right._content.empty()? 0 : right._content.data();
	right.aliases = right._aliases.empty()? 0 : &right._aliases;
	right.metadata = right._metadata.empty()? 0 : &right._metadata;

	yggr::swap(base_type::chunk_size, right.chunk_size);
}

} // namespace nsql_database_system
} // namespace yggr
