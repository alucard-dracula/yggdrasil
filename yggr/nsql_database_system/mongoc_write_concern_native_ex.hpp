//mongoc_write_concern_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_WRITE_CONCERN_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_WRITE_CONCERN_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_write_concern_t {
//   int8_t fsync_; /* deprecated */
//   int8_t journal;
//   int32_t w;
//   int64_t wtimeout;
//   char *wtag;
//   bool frozen;
//   bson_t compiled;
//   bool is_default;
//};

//#define MONGOC_WRITE_CONCERN_FSYNC_DEFAULT -1
//#define MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT -1

//#define MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED 0
//#define MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED -1 /* deprecated */
//#define MONGOC_WRITE_CONCERN_W_DEFAULT -2
//#define MONGOC_WRITE_CONCERN_W_MAJORITY -3
//#define MONGOC_WRITE_CONCERN_W_TAG -4

struct yggr_mongoc_write_concern
{
	int8_t		fsync_;
	int8_t		journal;
	int32_t		w;
	int64_t		wtimeout;
	char		*wtag;
	bool		frozen;
	bson_t		compiled;
	bool		is_default_v; //_mongoc_write_concern_t is is_default
};

class mongoc_write_concern_native_ex
{
public:
	YGGR_STATIC_CONSTANT(s8, E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT = -1);
	YGGR_STATIC_CONSTANT(s8, E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT = -1);

public:
	YGGR_STATIC_CONSTANT(s32, E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED = MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED);
	YGGR_STATIC_CONSTANT(s32, E_MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED = MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED); // deprecated
	YGGR_STATIC_CONSTANT(s32, E_MONGOC_WRITE_CONCERN_W_DEFAULT = MONGOC_WRITE_CONCERN_W_DEFAULT);
	YGGR_STATIC_CONSTANT(s32, E_MONGOC_WRITE_CONCERN_W_MAJORITY = MONGOC_WRITE_CONCERN_W_MAJORITY);
	YGGR_STATIC_CONSTANT(s32, E_MONGOC_WRITE_CONCERN_W_TAG = MONGOC_WRITE_CONCERN_W_TAG);

private:
	typedef mongoc_write_concern_native_ex this_type;

public:
	static void* s_mongoc_write_concern_init(void* pmwc);
	static bool s_mongoc_write_concern_static_destroy(void* pmwc);

	static void* s_mongoc_write_concern_move(void* pdst, void* psrc);

	static void* s_mongoc_write_concern_copy(void* pdst, const void* psrc);
	static void* s_mongoc_write_concern_unfreeze_copy(void* pdst, const void* psrc);
	
	static void s_mongoc_write_concern_swap(void* pl, void* pr);

	static bool s_mongoc_write_concern_is_valid(const void* pmwc);

public:
	static bool s_mongoc_write_concern_append(void* pmwc, bson_t* command);

	inline static bool s_mongoc_write_concern_get_fsync(const void* pmwc)
	{
		const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);
		assert(pmwc_real);
		return this_type::prv_s_mongoc_write_concern_get_fsync(pmwc_real);
	}

	static bool s_mongoc_write_concern_set_fsync(void* pmwc, bool fsync);

	inline static bool s_mongoc_write_concern_get_journal(const void* pmwc)
	{
		const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);
		assert(pmwc_real);
		return this_type::prv_s_mongoc_write_concern_get_journal(pmwc_real);
	}

	static bool s_mongoc_write_concern_set_journal(void* pmwc, bool journal);

	inline static s32 s_mongoc_write_concern_get_w(const void* pmwc)
	{
		const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);
		assert(pmwc_real);
		return pmwc_real->w;
	}

	static bool s_mongoc_write_concern_set_w(void* pmwc, s32 w);

	inline static bool s_mongoc_write_concern_get_wmajority(const void* pmwc)
	{
		const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);
		assert(pmwc_real);

		return pmwc_real && (pmwc_real->w == MONGOC_WRITE_CONCERN_W_MAJORITY);
	}

	static bool s_mongoc_write_concern_set_wmajority(void* pmwc, s32 wtimeout_msec);

	inline static const char* s_mongoc_write_concern_get_wtag(const void* pmwc)
	{
		const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);
		assert(pmwc_real);

		return (pmwc_real && (pmwc_real->w == MONGOC_WRITE_CONCERN_W_TAG))? pmwc_real->wtag : 0;
	}

	static bool s_mongoc_write_concern_set_wtag(void* pmwc, const char* wtag, std::size_t n = -1);
	//static bool s_mongoc_write_concern_set_wtag_attach(void* pmwc, char* wtag);

	inline static s32 s_mongoc_write_concern_get_wtimeout(const void* pmwc)
	{
		return static_cast<s32>(this_type::s_mongoc_write_concern_get_wtimeout_int64(pmwc));
	}

	inline static s64 s_mongoc_write_concern_get_wtimeout_int64(const void* pmwc)
	{
		const yggr_mongoc_write_concern* pmwc_real = reinterpret_cast<const yggr_mongoc_write_concern*>(pmwc);
		assert(pmwc_real);

		return pmwc_real? pmwc_real->wtimeout : 0;
	}

	inline static bool mongoc_write_concern_set_wtimeout(void* pmwc, s32 wtimeout_msec)
	{
		return this_type::mongoc_write_concern_set_wtimeout_int64(pmwc, static_cast<s64>(wtimeout_msec));
	}

	static bool mongoc_write_concern_set_wtimeout_int64(void* pmwc, s64 wtimeout_msec);

	inline static bool s_mongoc_write_concern_freeze(void* pmwc)
	{
		return this_type::prv_s_mongoc_write_concern_freeze(reinterpret_cast<yggr_mongoc_write_concern*>(pmwc));
	}

	inline static bool s_mongoc_write_concern_unfreeze(void* pmwc)
	{
		return this_type::prv_s_mongoc_write_concern_unfreeze(reinterpret_cast<yggr_mongoc_write_concern*>(pmwc));
	}

	inline static const bson_t* s_mongoc_write_concern_get_bson(void* pmwc)
	{
		return this_type::prv_s_mongoc_write_concern_get_bson(reinterpret_cast<yggr_mongoc_write_concern*>(pmwc));
	}

	inline static bool s_mongoc_write_concern_journal_is_set(const void* pmwc)
	{
		const mongoc_write_concern_t* pmwc_real = reinterpret_cast<const mongoc_write_concern_t*>(pmwc);
		assert(pmwc_real);

		return pmwc_real && mongoc_write_concern_journal_is_set(pmwc_real);
	}

	inline static bool s_mongoc_write_concern_is_acknowledged(const void* pmwc)
	{
		const mongoc_write_concern_t* pmwc_real = reinterpret_cast<const mongoc_write_concern_t*>(pmwc);
		assert(pmwc_real);

		return pmwc_real && mongoc_write_concern_is_acknowledged(pmwc_real);
	}

private:
	static bool prv_s_mongoc_write_concern_get_fsync(const yggr_mongoc_write_concern* write_concern);
	static bool prv_s_mongoc_write_concern_get_journal(const yggr_mongoc_write_concern* write_concern);
	static bool prv_s_mongoc_write_concern_freeze(yggr_mongoc_write_concern* write_concern);
	static bool prv_s_mongoc_write_concern_unfreeze(yggr_mongoc_write_concern* write_concern);
	static const bson_t* prv_s_mongoc_write_concern_get_bson(yggr_mongoc_write_concern* write_concern);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGO_WRITE_CONCERN_NATIVE_EX_HPP__
