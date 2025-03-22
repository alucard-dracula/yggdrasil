//mongoc_write_concern_native_ex.cpp

/****************************************************************************
Copyright(c) 2014-2022 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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

#include <yggr/nsql_database_system/mongoc_write_concern_native_ex.hpp>

#include <yggr/nsql_database_system/bson_string_native_ex.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/utility/swap.hpp>

#include <boost/mpl/bool.hpp>

#include <cassert>

// check mongoc driver private type and yggr type space size
#define MONGOC_COMPILATION

#	include <mongoc-write-concern.h>
#	include <mongoc-write-concern-private.h>

namespace yggr
{
namespace nsql_database_system
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(yggr_mongoc_write_concern) == sizeof(mongoc_write_concern_t)>));
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

// ---------------mongoc_write_concern_t--------------------

/*static*/ 
void* mongoc_write_concern_native_ex::s_mongoc_write_concern_init(void* pmwc)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);
	if(!pmwc_real)
	{
		return 0;
	}
	memset(pmwc_real, 0, sizeof(yggr_mongoc_write_concern));

	pmwc_real->w = MONGOC_WRITE_CONCERN_W_DEFAULT;
	pmwc_real->fsync_ = this_type::E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT;
	pmwc_real->journal = this_type::E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT;
	pmwc_real->is_default_v = true;

	bson_init(boost::addressof(pmwc_real->compiled));

	return pmwc_real;
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_static_destroy(void* pmwc)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	if(!pmwc_real)
	{
		return false;
	}

	if(pmwc_real->wtag)
	{
		bson_free(pmwc_real->wtag);
	}

	if((pmwc_real->compiled).len) 
	{
		bson_destroy(&pmwc_real->compiled);
	}
	
	memset(pmwc_real, 0, sizeof(yggr_mongoc_write_concern));
	return true;
}

/*static*/ 
void* mongoc_write_concern_native_ex::s_mongoc_write_concern_move(void* pdst, void* psrc)
{
	yggr_mongoc_write_concern* pdst_real = reinterpret_cast<yggr_mongoc_write_concern*>(pdst);
	yggr_mongoc_write_concern* psrc_real = reinterpret_cast<yggr_mongoc_write_concern*>(psrc);
	assert((psrc_real && pdst_real));
	if(!(psrc_real && pdst_real))
	{
		return 0;
	}

	if(psrc_real == pdst_real)
	{
		return pdst_real;
	}

	pdst_real->fsync_ = psrc_real->fsync_;
	pdst_real->journal = psrc_real->journal;
	pdst_real->w = psrc_real->w;
	pdst_real->wtimeout = psrc_real->wtimeout;
	pdst_real->frozen = psrc_real->frozen;

	yggr::swap(pdst_real->wtag, psrc_real->wtag);

	bson_native_ex::s_bson_swap(
		boost::addressof(pdst_real->compiled), 
		boost::addressof(psrc_real->compiled));

	pdst_real->is_default_v = psrc_real->is_default_v;

	return pdst;
}

/*static*/ 
void* mongoc_write_concern_native_ex::s_mongoc_write_concern_copy(void* pdst, const void* psrc)
{
	yggr_mongoc_write_concern* pdst_real = reinterpret_cast<yggr_mongoc_write_concern*>(pdst);
	const yggr_mongoc_write_concern* psrc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(psrc);
	assert((psrc_real && pdst_real));
	if(!(psrc_real && pdst_real))
	{
		return 0;
	}

	if(psrc_real == pdst_real)
	{
		return pdst_real;
	}

	pdst_real->fsync_ = psrc_real->fsync_;
	pdst_real->journal = psrc_real->journal;
	pdst_real->w = psrc_real->w;
	pdst_real->wtimeout = psrc_real->wtimeout;
	pdst_real->frozen = psrc_real->frozen;
	if(pdst_real->wtag)
	{
		bson_free(pdst_real->wtag);
	}
	pdst_real->wtag = bson_string_native_ex::s_bson_strdup(psrc_real->wtag);

	bson_native_ex::s_bson_copy(
		boost::addressof(pdst_real->compiled), 
		boost::addressof(psrc_real->compiled));

	pdst_real->is_default_v = psrc_real->is_default_v;

	return pdst;
}

/*static*/ 
void* mongoc_write_concern_native_ex::s_mongoc_write_concern_unfreeze_copy(void* pdst, const void* psrc)
{
	yggr_mongoc_write_concern* pdst_real = reinterpret_cast<yggr_mongoc_write_concern*>(pdst);
	const yggr_mongoc_write_concern* psrc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(psrc);
	assert((psrc_real && pdst_real));
	if(!(psrc_real && pdst_real))
	{
		return 0;
	}

	if(psrc_real == pdst_real)
	{
		return pdst_real;
	}

	pdst_real->fsync_ = psrc_real->fsync_;
	pdst_real->journal = psrc_real->journal;
	pdst_real->w = psrc_real->w;
	pdst_real->wtimeout = psrc_real->wtimeout;
	pdst_real->frozen = false;
	if(pdst_real->wtag)
	{
		bson_free(pdst_real->wtag);
	}
	pdst_real->wtag = bson_string_native_ex::s_bson_strdup(psrc_real->wtag);

	if((pdst_real->compiled).len)
	{
		bson_reinit(boost::addressof(pdst_real->compiled));
	}
	else
	{
		memset(boost::addressof(pdst_real->compiled), 0, sizeof(bson_t));
		bson_init(boost::addressof(pdst_real->compiled));
	}

	pdst_real->is_default_v = psrc_real->is_default_v;

	return pdst;
}

/*static*/ 
void mongoc_write_concern_native_ex::s_mongoc_write_concern_swap(void* pl, void* pr)
{
	yggr_mongoc_write_concern* pl_real = reinterpret_cast<yggr_mongoc_write_concern*>(pl);
	yggr_mongoc_write_concern* pr_real = reinterpret_cast<yggr_mongoc_write_concern*>(pr);
	assert((pl_real && pr_real));
	if(!(pl_real && pr_real) || (pl_real == pr_real))
	{
		return;
	}

	yggr::swap(pl_real->fsync_, pr_real->fsync_);
	yggr::swap(pl_real->journal, pr_real->journal);
	yggr::swap(pl_real->w, pr_real->w);
	yggr::swap(pl_real->wtimeout, pr_real->wtimeout);
	yggr::swap(pl_real->wtag, pr_real->wtag);
	yggr::swap(pl_real->frozen, pr_real->frozen);

	bson_native_ex::s_bson_swap(&(pl_real->compiled), &(pr_real->compiled));
	yggr::swap(pl_real->is_default_v, pr_real->is_default_v);
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_is_valid(const void* pmwc)
{
	const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);

	assert(pmwc_real);
	if(!pmwc_real)
	{
		return false;
	}
	
	/*
	* Journal or fsync should require acknowledgement.
	*/
	if((this_type::prv_s_mongoc_write_concern_get_fsync(pmwc_real) ||
		this_type::prv_s_mongoc_write_concern_get_journal(pmwc_real)) &&
		(pmwc_real->w == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED ||
			pmwc_real->w == MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED)) 
	{
		return false;
	}

	return !((pmwc_real->wtimeout) < 0);
}

/*static*/
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_append(void* pmwc, bson_t* command)
{
	mongoc_write_concern_t* pmwc_real = reinterpret_cast<mongoc_write_concern_t*>(pmwc);
	assert(pmwc_real);

	return pmwc_real && mongoc_write_concern_append(pmwc_real, command);
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_set_fsync(void* pmwc, bool fsync)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);
	if(!pmwc_real)
	{
		return false;
	}

	pmwc_real->fsync_ = static_cast<int8_t>(!!fsync);
	pmwc_real->is_default_v = false;

	this_type::prv_s_mongoc_write_concern_unfreeze(pmwc_real);

	return true;
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_set_journal(void* pmwc, bool journal)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);
	if(!pmwc_real)
	{
		return false;
	}

	pmwc_real->journal = static_cast<int8_t>(!!journal);
	pmwc_real->is_default_v = false;
	this_type::prv_s_mongoc_write_concern_unfreeze(pmwc_real);

	return true;
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_set_w(void* pmwc, s32 w)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);
	BSON_ASSERT(w >= -3);

	if(!(pmwc_real && w >= -3))
	{
		return false;
	}

	pmwc_real->w = w;
	if(w != MONGOC_WRITE_CONCERN_W_DEFAULT) 
	{
		pmwc_real->is_default_v = false;
	}

	if(pmwc_real->wtag)
	{
		bson_free(pmwc_real->wtag);
		pmwc_real->wtag = 0;
	}

	this_type::prv_s_mongoc_write_concern_unfreeze(pmwc_real);

	return true;
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wmajority(void* pmwc, s32 wtimeout_msec)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);

	if(!pmwc_real)
	{
		return false;
	}

	pmwc_real->w = MONGOC_WRITE_CONCERN_W_MAJORITY;
	pmwc_real->is_default_v = false;
	this_type::prv_s_mongoc_write_concern_unfreeze(pmwc_real);

	if(wtimeout_msec >= 0) 
	{
		pmwc_real->wtimeout = wtimeout_msec;
	}

	if(pmwc_real->wtag)
	{
		bson_free(pmwc_real->wtag);
		pmwc_real->wtag = 0;
	}

	return true;
}

/*static*/ 
bool mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wtag(void* pmwc, const char* wtag, std::size_t n)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);
		
	if(!pmwc_real)
	{
		return false;
	}

	if(pmwc_real->wtag)
	{
		bson_free(pmwc_real->wtag);
	}

	pmwc_real->wtag = bson_string_native_ex::s_bson_strdup(wtag, n);
	pmwc_real->w = MONGOC_WRITE_CONCERN_W_TAG;
	pmwc_real->is_default_v = false;
	this_type::prv_s_mongoc_write_concern_unfreeze(pmwc_real);

	return true;
}

/*static*/ 
bool mongoc_write_concern_native_ex::mongoc_write_concern_set_wtimeout_int64(void* pmwc, s64 wtimeout_msec)
{
	yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<yggr_mongoc_write_concern*>(pmwc);
	assert(pmwc_real);

	if((!pmwc_real) || (wtimeout_msec < 0))
	{
		return false;
	}

	pmwc_real->wtimeout = wtimeout_msec;
	pmwc_real->is_default_v = false;
	this_type::prv_s_mongoc_write_concern_unfreeze(pmwc_real);

	return true;
}

//private:

/*static*/
bool mongoc_write_concern_native_ex::prv_s_mongoc_write_concern_get_fsync(const yggr_mongoc_write_concern* write_concern)
{
   assert(write_concern);
   return write_concern && (write_concern->fsync_ == 1);
}

/*static*/
bool mongoc_write_concern_native_ex::prv_s_mongoc_write_concern_get_journal (const yggr_mongoc_write_concern* write_concern)
{
   assert(write_concern);
   return write_concern && (write_concern->journal == 1);
}

/*static*/ 
bool mongoc_write_concern_native_ex::prv_s_mongoc_write_concern_freeze(yggr_mongoc_write_concern* write_concern)
{
	BSON_ASSERT(write_concern);

	if(!(write_concern && !(write_concern->frozen)))
	{
		return false;
	}

	bson_t* compiled = &write_concern->compiled;

	write_concern->frozen = true;

	bson_reinit(compiled);

	switch(write_concern->w)
	{
	case MONGOC_WRITE_CONCERN_W_TAG:
		{
			BSON_ASSERT(write_concern->wtag);
			BSON_APPEND_UTF8(compiled, "w", write_concern->wtag);
		}
		break;
	case MONGOC_WRITE_CONCERN_W_MAJORITY:
		{
			BSON_APPEND_UTF8(compiled, "w", "majority");
		}
		break;
	case MONGOC_WRITE_CONCERN_W_DEFAULT:
		break;
	default:
		BSON_APPEND_INT32(compiled, "w", write_concern->w);
	}

	switch(write_concern->fsync_)
	{
	case this_type::E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT:
		break;
	default:
		bson_append_bool(compiled, "fsync", 5, !!write_concern->fsync_);
	}

	switch(write_concern->journal)
	{
	case this_type::E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT:
		break;
	default:
		bson_append_bool (compiled, "j", 1, !!write_concern->journal);
	}

	if(write_concern->wtimeout) 
	{
		bson_append_int64 (compiled, "wtimeout", 8, write_concern->wtimeout);
	}

	return true;
}

/*static*/ 
bool mongoc_write_concern_native_ex::prv_s_mongoc_write_concern_unfreeze(yggr_mongoc_write_concern* write_concern)
{
	BSON_ASSERT(write_concern);

	if(!(write_concern && write_concern->frozen))
	{
		return false;
	}

	if((write_concern->compiled).len)
	{
		bson_reinit(boost::addressof(write_concern->compiled));
	}
	else
	{
		memset(boost::addressof(write_concern->compiled), 0, sizeof(bson_t));
		bson_init(boost::addressof(write_concern->compiled));
	}
	
	write_concern->frozen = false;

	return true;
}


/*static*/ 
const bson_t* mongoc_write_concern_native_ex::prv_s_mongoc_write_concern_get_bson(yggr_mongoc_write_concern* write_concern)
{
	BSON_ASSERT(write_concern);
	if(!write_concern)
	{
		return 0;
	}

	if(!write_concern->frozen) 
	{
		this_type::prv_s_mongoc_write_concern_freeze(write_concern);
	}

	return &write_concern->compiled;
}

} // namespace nsql_database_system
} // namespace yggr
