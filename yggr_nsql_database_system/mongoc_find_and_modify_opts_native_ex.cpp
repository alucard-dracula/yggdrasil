//mongoc_find_and_modify_opts_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_find_and_modify_opts_native_ex.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

// check mongoc driver private type and yggr type space size
#define MONGOC_COMPILATION

#include <mongoc-find-and-modify.h>
#include <mongoc-find-and-modify-private.h>

namespace yggr
{
namespace nsql_database_system
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(yggr_mongoc_find_and_modify_opts) == sizeof(mongoc_find_and_modify_opts_t)>));
} //namespace nsql_database_system
} //namespace yggr

#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
void* mongoc_find_and_modify_opts_native_ex::s_mongoc_find_and_modify_opts_init(void* popts)
{
	yggr_mongoc_find_and_modify_opts* preal_opts = reinterpret_cast<yggr_mongoc_find_and_modify_opts*>(popts);
	assert(preal_opts);
	if(!preal_opts)
	{
		return 0;
	}

	preal_opts->sort = 0;
	preal_opts->update = 0;
	preal_opts->fields = 0;
	preal_opts->flags = 0;
	preal_opts->bypass_document_validation = this_type::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT;
	preal_opts->max_time_ms = 0;
	memset(boost::addressof(preal_opts->extra), 0, sizeof(bson_t));
	bson_init(boost::addressof(preal_opts->extra));

	return popts;
}

} // namespace nsql_database_system
} // namespace yggr

