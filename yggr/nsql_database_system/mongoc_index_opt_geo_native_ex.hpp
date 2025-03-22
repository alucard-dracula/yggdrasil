// mongoc_index_opt_geo_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_OPT_GEN_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_OPT_GEN_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>

#include <cassert>


//typedef struct
//{
//   uint8_t twod_sphere_version;
//   uint8_t twod_bits_precision;
//   double  twod_location_min;
//   double  twod_location_max;
//   double  haystack_bucket_size;
//   uint8_t *padding[32];
//} mongoc_index_opt_geo_t;


namespace yggr
{
namespace nsql_database_system
{

class mongoc_index_opt_geo_native_ex
{
private:
	typedef mongoc_index_opt_geo_native_ex this_type;

public:
	inline static const mongoc_index_opt_geo_t* s_mongoc_index_opt_geo_get_default(void)
	{
		return mongoc_index_opt_geo_get_default();
	}

	inline static mongoc_index_opt_geo_t* s_mongoc_index_opt_geo_init(mongoc_index_opt_geo_t* popt)
	{
		assert(popt);
		if(!popt)
		{
			return 0;
		}

		mongoc_index_opt_geo_init(popt);
		return popt;
	}

	inline static mongoc_index_opt_geo_t* 
		s_mongoc_index_opt_geo_copy(mongoc_index_opt_geo_t* pdst, const mongoc_index_opt_geo_t* psrc)
	{
		assert(pdst && psrc);
		if(!(pdst && psrc))
		{
			return 0;
		}

		if(pdst == psrc)
		{
			return pdst;
		}

		memcpy(pdst, psrc, sizeof(mongoc_index_opt_geo_t));
		return pdst;
	}

	inline static void s_mongoc_index_opt_geo_swap(mongoc_index_opt_geo_t* pl, mongoc_index_opt_geo_t* pr)
	{
		assert(pl && pr);
		if(!(pl && pr) || (pl == pr))
		{
			return;
		}

		mongoc_index_opt_geo_t tmp;
		memcpy(&tmp, pl, sizeof(mongoc_index_opt_geo_t));
		memcpy(pl, pr, sizeof(mongoc_index_opt_geo_t));
		memcpy(pr, &tmp, sizeof(mongoc_index_opt_geo_t));
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_OPT_GEN_NATIVE_EX_HPP__
