//mongoc_read_prefs_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_read_prefs_native_ex.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

#include <cassert>

// check mongoc driver private type and yggr type space size
#define MONGOC_COMPILATION

#	include <mongoc-read-prefs.h>
#	include <mongoc-read-prefs-private.h>

namespace yggr
{
namespace nsql_database_system
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(yggr_mongoc_read_prefs) == sizeof(mongoc_read_prefs_t)>));
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
void* mongoc_read_prefs_native_ex::s_mongoc_read_prefs_init(void* pmrp, mode_type mode)
{
	yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);
	if(!pmrp_real)
	{
		return 0;
	}

	memset(pmrp_real, 0, sizeof(yggr_mongoc_read_prefs));

	pmrp_real->mode = 
		this_type::E_MONGOC_READ_PRIMARY <= mode && mode <= this_type::E_MONGOC_READ_NEAREST? 
			mode : this_type::E_MONGOC_READ_PRIMARY;
	bson_init(boost::addressof(pmrp_real->tags));

	pmrp_real->max_staleness_seconds = this_type::E_MONGOC_NO_MAX_STALENESS;
	bson_init(boost::addressof(pmrp_real->hedge));

	return pmrp_real;
}

/*static*/ 
bool mongoc_read_prefs_native_ex::s_mongoc_read_prefs_static_destroy(void* pmrp)
{
	yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pmrp);
	if(!pmrp_real)
	{
		return false;
	}

	bson_destroy(boost::addressof(pmrp_real->hedge));
	pmrp_real->max_staleness_seconds = this_type::E_MONGOC_NO_MAX_STALENESS;

	bson_destroy(boost::addressof(pmrp_real->tags));
	pmrp_real->mode = this_type::E_MONGOC_READ_INVALIDATE; // = 0

	return true;
}

/*static*/ 
void* mongoc_read_prefs_native_ex::s_mongoc_read_prefs_copy(void* pdst, const void* psrc)
{
	yggr_mongoc_read_prefs* pdst_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pdst);
	const yggr_mongoc_read_prefs* psrc_real = reinterpret_cast<const yggr_mongoc_read_prefs*>(psrc);
	assert((psrc_real && pdst_real));
	if(!(psrc_real && pdst_real))
	{
		return 0;
	}

	if(psrc_real == pdst_real)
	{
		return pdst_real;
	}

	pdst_real->mode = psrc_real->mode;
	bson_native_ex::s_bson_copy(&(pdst_real->tags), &(psrc_real->tags));

	pdst_real->max_staleness_seconds = psrc_real->max_staleness_seconds;
	bson_native_ex::s_bson_copy(&(pdst_real->hedge), &(psrc_real->hedge));

	return pdst;
}

/*static*/ 
void* mongoc_read_prefs_native_ex::s_mongoc_read_prefs_move(void* pdst, void* psrc)
{
	yggr_mongoc_read_prefs* pdst_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pdst);
	yggr_mongoc_read_prefs* psrc_real = reinterpret_cast<yggr_mongoc_read_prefs*>(psrc);
	assert((psrc_real && pdst_real));
	if(!(psrc_real && pdst_real))
	{
		return 0;
	}

	if(psrc_real == pdst_real)
	{
		return pdst_real;
	}

	pdst_real->mode = psrc_real->mode;
	bson_native_ex::s_bson_swap(&(pdst_real->tags), &(psrc_real->tags));

	pdst_real->max_staleness_seconds = psrc_real->max_staleness_seconds;
	bson_native_ex::s_bson_swap(&(pdst_real->hedge), &(psrc_real->hedge));

	return pdst;
}

/*static*/ 
void mongoc_read_prefs_native_ex::s_mongoc_read_prefs_swap(void* pl, void* pr)
{
	yggr_mongoc_read_prefs* pl_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pl);
	yggr_mongoc_read_prefs* pr_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pr);

	assert((pl_real && pr_real));
	if(!(pl_real && pr_real) || (pl_real == pr_real))
	{
		return;
	}

	yggr::swap(pl_real->mode, pr_real->mode);
	bson_native_ex::s_bson_swap(&(pl_real->tags), &(pr_real->tags));

	yggr::swap(pl_real->max_staleness_seconds, pr_real->max_staleness_seconds);
	bson_native_ex::s_bson_swap(&(pl_real->hedge), &(pr_real->hedge));
}

/*static*/ 
bool mongoc_read_prefs_native_ex::s_mongoc_read_prefs_add_tag(void* pmrp, const bson_t* tag)
{
	mongoc_read_prefs_t* pmrp_real = reinterpret_cast<mongoc_read_prefs_t*>(pmrp);
	assert(pmrp_real);

	return pmrp_real && (mongoc_read_prefs_add_tag(pmrp_real, tag), true);
}

/*static*/ 
bool mongoc_read_prefs_native_ex::s_mongoc_read_prefs_is_valid(const void* pmrp)
{
	const yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<const yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);

	if(!pmrp_real)
	{
		return false;
	}

	switch(pmrp_real->mode)
	{
	//case this_type::E_MONGOC_READ_INVALIDATE:
	//	return false;
	case this_type::E_MONGOC_READ_PRIMARY:
		{
			if(!(bson_empty(&pmrp_real->tags) 
					&& pmrp_real->max_staleness_seconds == this_type::E_MONGOC_NO_MAX_STALENESS
					&& bson_empty(&pmrp_real->hedge)))
			{
				return false;
			}
		}
	case this_type::E_MONGOC_READ_SECONDARY:
	case this_type::E_MONGOC_READ_PRIMARY_PREFERRED:
	case this_type::E_MONGOC_READ_SECONDARY_PREFERRED:
	case this_type::E_MONGOC_READ_NEAREST:
		return 
			pmrp_real->max_staleness_seconds == this_type::E_MONGOC_NO_MAX_STALENESS
			|| 0 < pmrp_real->max_staleness_seconds;
	default:
		return false;
	};
}

/*static*/ 
u32 mongoc_read_prefs_native_ex::s_mongoc_read_prefs_get_mode(const void* pmrp)
{
	const yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<const yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);

	return pmrp_real? pmrp_real->mode : this_type::E_MONGOC_READ_INVALIDATE;
}

/*static*/ 
bool mongoc_read_prefs_native_ex::s_mongoc_read_prefs_set_mode(void* pmrp, mode_type mode)
{
	yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);
	assert(this_type::E_MONGOC_READ_PRIMARY <= mode && mode <= this_type::E_MONGOC_READ_NEAREST);

	return 
		pmrp_real
		&& (this_type::E_MONGOC_READ_PRIMARY <= mode && mode <= this_type::E_MONGOC_READ_NEAREST)
		&& (pmrp_real->mode = mode, true);
}

/*static*/ 
const bson_t* mongoc_read_prefs_native_ex::s_mongoc_read_prefs_get_tags(const void* pmrp)
{
	const yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<const yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);
	
	return pmrp_real? &(pmrp_real->tags) : 0;
}

/*static*/ 
bool mongoc_read_prefs_native_ex::s_mongoc_read_prefs_set_tags(void* pmrp, const bson_t* ptags)
{
	mongoc_read_prefs_t* pmrp_real = reinterpret_cast<mongoc_read_prefs_t*>(pmrp);
	assert(pmrp_real);

	return pmrp_real && ptags && (mongoc_read_prefs_set_tags(pmrp_real, ptags), true);
}

/*static*/ 
s64 mongoc_read_prefs_native_ex::s_mongoc_read_prefs_get_max_staleness_seconds(const void* pmrp)
{
	const yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<const yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);
	
	return pmrp_real? pmrp_real->max_staleness_seconds : this_type::E_MONGOC_NO_MAX_STALENESS;
}

/*static*/ 
bool mongoc_read_prefs_native_ex::s_mongoc_read_prefs_set_max_staleness_seconds(void* pmrp, s64 max_staleness_seconds)
{
	yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);

	return pmrp_real && (pmrp_real->max_staleness_seconds = max_staleness_seconds, true);
}

/*static*/ 
const bson_t* mongoc_read_prefs_native_ex::s_mongoc_read_prefs_get_hedge(const void* pmrp)
{
	const yggr_mongoc_read_prefs* pmrp_real = reinterpret_cast<const yggr_mongoc_read_prefs*>(pmrp);
	assert(pmrp_real);

	return pmrp_real? &(pmrp_real->hedge) : 0;
}

/*static*/ 
bool s_mongoc_read_prefs_set_hedge(void* pmrp, const bson_t* phedge)
{
	mongoc_read_prefs_t* pmrp_real = reinterpret_cast<mongoc_read_prefs_t*>(pmrp);
	assert(pmrp_real);

	return pmrp_real && phedge && (mongoc_read_prefs_set_hedge(pmrp_real, phedge), true);
}

} // namespace nsql_database_system
} // namespace yggr
