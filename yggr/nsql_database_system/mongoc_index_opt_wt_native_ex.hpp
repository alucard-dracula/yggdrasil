// mongoc_index_opt_wt_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_OPT_WT_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_OPT_WT_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//typedef struct
//{
//   mongoc_index_opt_storage_t  base;
//   const char                 *config_str;
//   void                       *padding[8];
//} mongoc_index_opt_wt_t;

class mongoc_index_opt_wt_native_ex
{
private:
	typedef mongoc_index_opt_wt_native_ex this_type;

public:
	inline static const mongoc_index_opt_wt_t* s_mongoc_index_opt_wt_get_default(void)
	{
		return mongoc_index_opt_wt_get_default();
	}

	inline static mongoc_index_opt_wt_t* s_mongoc_index_opt_wt_init(mongoc_index_opt_wt_t* popt)
	{
		assert(popt);
		if(!popt)
		{
			return 0;
		}

		mongoc_index_opt_wt_init(popt);
		return popt;
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_OPT_WT_NATIVE_EX_HPP__
