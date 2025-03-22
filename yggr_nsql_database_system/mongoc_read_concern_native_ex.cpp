//mongoc_read_concern_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_read_concern_native_ex.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_string_native_ex.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

#include <cassert>

// check mongoc driver private type and yggr type space size
#define MONGOC_COMPILATION

#	include <mongoc-read-concern.h>
#	include <mongoc-read-concern-private.h>

namespace yggr
{
namespace nsql_database_system
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(yggr_mongoc_read_concern) == sizeof(mongoc_read_concern_t)>));
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
void* mongoc_read_concern_native_ex::s_mongoc_read_concern_init(void* pmrc)
{
	yggr_mongoc_read_concern* preal_pmrc = reinterpret_cast<yggr_mongoc_read_concern*>(pmrc);
	assert(preal_pmrc);
	if(!preal_pmrc)
	{
		return 0;
	}

	memset(preal_pmrc, 0, sizeof(yggr_mongoc_read_concern));
	bson_init(boost::addressof(preal_pmrc->compiled));
	return preal_pmrc;
}

/*static*/ 
void* mongoc_read_concern_native_ex::s_mongoc_read_concern_move(void* pdst, void* psrc)
{
	yggr_mongoc_read_concern* preal_dst = reinterpret_cast<yggr_mongoc_read_concern*>(pdst);
	yggr_mongoc_read_concern* preal_src = reinterpret_cast<yggr_mongoc_read_concern*>(psrc);
	assert((preal_dst && preal_src));
	if(!(preal_dst && preal_src))
	{
		return 0;
	}

	if(preal_dst == preal_src)
	{
		return preal_dst;
	}

	yggr::swap(preal_dst->level, preal_src->level);
	preal_dst->frozen = preal_src->frozen;

	bson_native_ex::s_bson_swap(
		boost::addressof(preal_dst->compiled), 
		boost::addressof(preal_src->compiled));
	
	return pdst;
}

/*static*/ 
void* mongoc_read_concern_native_ex::s_mongoc_read_concern_copy(void* pdst, const void* psrc)
{
	yggr_mongoc_read_concern* preal_dst = reinterpret_cast<yggr_mongoc_read_concern*>(pdst);
	const yggr_mongoc_read_concern* preal_src = reinterpret_cast<const yggr_mongoc_read_concern*>(psrc);
	assert((preal_dst && preal_src));
	if(!(preal_dst && preal_src))
	{
		return 0;
	}

	if(preal_dst == preal_src)
	{
		return preal_dst;
	}

	if(preal_dst->level)
	{
		bson_free(preal_dst->level);
	}
	preal_dst->level = bson_string_native_ex::s_bson_strdup(preal_src->level);
	preal_dst->frozen = preal_src->frozen;

	bson_native_ex::s_bson_copy(
		boost::addressof(preal_dst->compiled), 
		boost::addressof(preal_src->compiled));
	
	return pdst;
}

/*static*/ 
void* mongoc_read_concern_native_ex::s_mongoc_read_concern_unfreeze_copy(void* pdst, const void* psrc)
{
	yggr_mongoc_read_concern* preal_dst = reinterpret_cast<yggr_mongoc_read_concern*>(pdst);
	const yggr_mongoc_read_concern* preal_src = reinterpret_cast<const yggr_mongoc_read_concern*>(psrc);
	assert((preal_dst && preal_src));
	if(!(preal_dst && preal_src))
	{
		return 0;
	}

	if(preal_dst == preal_src)
	{
		return preal_dst;
	}

	if(preal_dst->level)
	{
		bson_free(preal_dst->level);
	}
	preal_dst->level = bson_string_native_ex::s_bson_strdup(preal_src->level);
	preal_dst->frozen = false;

	if((preal_dst->compiled).len)
	{
		bson_reinit(boost::addressof(preal_dst->compiled));
	}
	else
	{
		memset(boost::addressof(preal_dst->compiled), 0, sizeof(bson_t));
		bson_init(boost::addressof(preal_dst->compiled));
	}
	
	return pdst;
}

/*static*/ 
void mongoc_read_concern_native_ex::s_mongoc_read_concern_swap(void* pl, void* pr)
{
	yggr_mongoc_read_concern* preal_pl = reinterpret_cast<yggr_mongoc_read_concern*>(pl);
	yggr_mongoc_read_concern* preal_pr = reinterpret_cast<yggr_mongoc_read_concern*>(pr);
	assert((preal_pl && preal_pr));
	if(!(preal_pl && preal_pr) || (preal_pl == preal_pr))
	{
		return;
	}

	yggr::swap(preal_pl->level, preal_pr->level);
	yggr::swap(preal_pl->frozen, preal_pr->frozen);
	bson_native_ex::s_bson_swap(&(preal_pl->compiled), &(preal_pr->compiled));
}

/*static*/ 
bool mongoc_read_concern_native_ex::s_mongoc_read_concern_static_destroy(void* pmrc)
{
	yggr_mongoc_read_concern* preal_pmrc = reinterpret_cast<yggr_mongoc_read_concern*>(pmrc);
	assert(preal_pmrc);
	if(!preal_pmrc)
	{
		return false;
	}

	if(preal_pmrc->compiled.len)
	{
		bson_destroy(&(preal_pmrc->compiled));
	}

	if(preal_pmrc->level)
	{
		bson_free(preal_pmrc->level);
	}

	return true;
}

/*static*/ 
const char* mongoc_read_concern_native_ex::s_mongoc_read_concern_get_level(const void* pmrc)
{
	const yggr_mongoc_read_concern* preal_pmrc = reinterpret_cast<const yggr_mongoc_read_concern*>(pmrc);
	assert(preal_pmrc);

	return preal_pmrc? preal_pmrc->level : 0;
}

/*static*/ 
bool mongoc_read_concern_native_ex::s_mongoc_read_concern_set_level(void* pmrc, const char* level, std::size_t n)
{
	yggr_mongoc_read_concern* preal_pmrc = reinterpret_cast<yggr_mongoc_read_concern*>(pmrc);
	assert(preal_pmrc);

	if(!preal_pmrc)
	{
		return false;
	}

	if(preal_pmrc->level)
	{
		bson_free(preal_pmrc->level);
	}

	preal_pmrc->level = bson_string_native_ex::s_bson_strdup(level, n);
	this_type::prv_s_mongoc_read_concern_unfreeze(preal_pmrc);
	return true;
}

/*static*/ 
bool mongoc_read_concern_native_ex::s_mongoc_read_concern_is_default(const void* pmrc)
{
	const yggr_mongoc_read_concern* preal_pmrc = reinterpret_cast<const yggr_mongoc_read_concern*>(pmrc);
	assert(preal_pmrc);

	return !(preal_pmrc && preal_pmrc->level);
}

/*static*/ 
bool mongoc_read_concern_native_ex::s_mongoc_read_concern_append(void* pmrc, bson_t* command)
{
	mongoc_read_concern_t* preal_pmrc = reinterpret_cast<mongoc_read_concern_t*>(pmrc);
	assert(preal_pmrc);

	return preal_pmrc && command && mongoc_read_concern_append(preal_pmrc, command);
}

/*static*/ 
bool mongoc_read_concern_native_ex::s_mongoc_read_concern_freeze(void* pmrc)
{
	yggr_mongoc_read_concern* preal_mrc = reinterpret_cast<yggr_mongoc_read_concern*>(pmrc);
	
	assert(preal_mrc);
	if((preal_mrc && !preal_mrc->frozen))
	{
		preal_mrc->frozen = true;

		if((preal_mrc->compiled).len)
		{
			bson_reinit(boost::addressof(preal_mrc->compiled));
		}
		else
		{
			bson_init(boost::addressof(preal_mrc->compiled));
		}

		if(preal_mrc->level) 
		{
			BSON_APPEND_UTF8((boost::addressof(preal_mrc->compiled)), "level", (preal_mrc->level));
		}

		return true;
	}
	else
	{
		return false;
	}
}

//private:
/*static*/ 
bool mongoc_read_concern_native_ex::prv_s_mongoc_read_concern_unfreeze(yggr_mongoc_read_concern* preal_mrc)
{
	assert(preal_mrc);
	if((preal_mrc && preal_mrc->frozen))
	{
		if((preal_mrc->compiled).len)
		{
			bson_reinit(boost::addressof(preal_mrc->compiled));
		}
		else
		{
			bson_init(boost::addressof(preal_mrc->compiled));
		}

		preal_mrc->frozen = false;

		return true;
	}
	else
	{
		return false;
	}
}

} // namespace nsql_database_system
} // namespace yggr

