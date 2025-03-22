//c_mongo_gridfs_file.hpp

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

#include <yggr/nsql_database_system/c_mongo_gridfs_file.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//--------------------c_mongo_gridfs_file---------------

c_mongo_gridfs_file::c_mongo_gridfs_file(void)
{
}

c_mongo_gridfs_file::c_mongo_gridfs_file(org_type* pfile)
{
	if(!pfile)
	{
		return;
	}

	gridfs_file_ptr_type tmp(pfile, shared_ptr_deleter_type());
	_pfile.swap(tmp);
}

c_mongo_gridfs_file::c_mongo_gridfs_file(const this_type& right)
	: _pfile(right._pfile)
{
}

c_mongo_gridfs_file::~c_mongo_gridfs_file(void)
{
}

c_mongo_gridfs_file::this_type& c_mongo_gridfs_file::operator=(const this_type& right)
{
	if((this == &right) || (_pfile == right._pfile))
	{
		return *this;
	}

	_pfile = right._pfile;
	return *this;
}

void c_mongo_gridfs_file::swap(this_type& right)
{
	if((this == &right) || (_pfile == right._pfile))
	{
		return;
	}

	_pfile.swap(right._pfile);
}

void c_mongo_gridfs_file::prv_init(mongoc_gridfs_t* pgridfs, const mongoc_gridfs_file_opt_t* popt)
{
	if(!(pgridfs && popt))
	{
		return;
	}
		
	org_type* pfile = 
		mongoc_gridfs_create_file(
			pgridfs, const_cast<mongoc_gridfs_file_opt_t*>(popt));
	if(!pfile)
	{
		return;
	}

	gridfs_file_ptr_type tmp(pfile, shared_ptr_deleter_type());
	_pfile.swap(tmp);
}

void c_mongo_gridfs_file::prv_init(mongoc_gridfs_t* pgridfs, mongoc_stream_t* pstream, const mongoc_gridfs_file_opt_t* popt)
{
	if(!(pgridfs && pstream && popt))
	{
		return;
	}
		
	org_type* pfile = 
		mongoc_gridfs_create_file_from_stream(
			pgridfs, pstream, const_cast<mongoc_gridfs_file_opt_t*>(popt));
	if(!pfile)
	{
		return;
	}

	gridfs_file_ptr_type tmp(pfile, shared_ptr_deleter_type());
	_pfile.swap(tmp);
}

} // namespace nsql_database_system
} // namespace yggr
