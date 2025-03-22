//bson_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/limits.hpp>
#include <yggr/ppex/aligned_storage_sizeof.hpp>

#include <yggr/nsql_database_system/bson_iterator_native_ex.hpp>
#include <yggr/nsql_database_system/bson_string_native_ex.hpp>

#include <cassert>
#include <memory>
#include <cstring>
#include <algorithm>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
void bson_native_ex::s_bson_init(bson_t* bson)
{
	BSON_ASSERT (bson);
	memset(bson, 0, sizeof(bson_t));

	bson_impl_inline_t *impl = (bson_impl_inline_t *) bson;

#ifdef BSON_MEMCHECK
	impl->canary = bson_malloc (1);
#endif
	impl->flags = static_cast<bson_flags_t>(BSON_FLAG_INLINE | BSON_FLAG_STATIC);
	impl->len = static_cast<uint32_t>(5);;
	impl->data[0] = 5;
	impl->data[1] = 0;
	impl->data[2] = 0;
	impl->data[3] = 0;
	impl->data[4] = 0;
}

/*static*/
bson_t* bson_native_ex::s_bson_init_static(bson_t* bson, const uint8_t* data, size_t length)
{
	assert(bson && data);
	if(!(bson && data))
	{
		return 0;
	}

	if(bson->len)
	{
		bson_destroy(bson);
		memset(bson, 0, sizeof(bson_t));
	}

	bson_impl_alloc_t *impl = reinterpret_cast<bson_impl_alloc_t*>(bson);
	uint32_t len_le = 0;

	//const uint8_t empty[5] = {5};

	if(0 == memcmp(data, YGGR_BSON_EMPTY_MARK(), YGGR_BSON_EMPTY_MARK_LENGTH()))
	{
		impl->flags = static_cast<bson_flags_t>(BSON_FLAG_STATIC | BSON_FLAG_RDONLY);
		impl->len = static_cast<uint32_t>(length);
		impl->parent = NULL;
		impl->depth = 0;
		impl->buf = &impl->alloc;
		impl->buflen = &impl->alloclen;
		impl->offset = 0;
		impl->alloc = const_cast<uint8_t*>(data);
		impl->alloclen = length;
		impl->realloc = NULL;
		impl->realloc_func_ctx = NULL;
		return bson;
	}

	if((length < 5) || (length > INT_MAX))
	{
		return 0;
	}

	memcpy (&len_le, data, sizeof(len_le));

	if(static_cast<size_t>(BSON_UINT32_FROM_LE (len_le)) != length)
	{
		return 0;
	}

	if(data[length - 1])
	{
		return 0;
	}

	impl->flags = static_cast<bson_flags_t>(BSON_FLAG_STATIC | BSON_FLAG_RDONLY);
	impl->len = static_cast<uint32_t>(length);
	impl->parent = 0;
	impl->depth = 0;
	impl->buf = &impl->alloc;
	impl->buflen = &impl->alloclen;
	impl->offset = 0;
	impl->alloc = const_cast<uint8_t*>(data);
	impl->alloclen = length;
	impl->realloc = 0;
	impl->realloc_func_ctx = 0;

	return bson;
}

/*static*/
bson_t* bson_native_ex::s_bson_init_from_json(bson_t* bson, const char* str_json,
															ssize_t len, bson_error_t* error)
{
	assert((bson && str_json));
	if(!(bson && str_json))
	{
		return 0;
	}

	if(!bson->len)
	{
		bson_init(bson);
	}

	if(!bson_init_from_json(bson, str_json, len, error))
	{
		return 0;
	}

	return bson;
}

/*static*/
bson_t* bson_native_ex::s_bson_init_from_size(bson_t* bson, ssize_t size)
{
	assert(bson);
	if(!bson)
	{
		return 0;
	}

	BSON_ASSERT(size <= INT32_MAX);

	bson_impl_alloc_t* impl_a = reinterpret_cast<bson_impl_alloc_t*>(bson);
	bson_impl_inline_t* impl_i = reinterpret_cast<bson_impl_inline_t*>(bson);
	bson_t *b = bson;

	if(size <= static_cast<ssize_t>(sizeof(impl_i->data)))
	{
		bson_init(b);
	}
	else
	{
		impl_a->flags = BSON_FLAG_STATIC;
		impl_a->len = 5;
		impl_a->parent = 0; // NULL
		impl_a->depth = 0;
		impl_a->buf = &impl_a->alloc;
		impl_a->buflen = &impl_a->alloclen;
		impl_a->offset = 0;
		impl_a->alloclen = (std::max<ssize_t>)(5, size);
		impl_a->alloc = reinterpret_cast<u8*>(bson_malloc(impl_a->alloclen));
		impl_a->alloc[0] = 5;
		impl_a->alloc[1] = 0;
		impl_a->alloc[2] = 0;
		impl_a->alloc[3] = 0;
		impl_a->alloc[4] = 0;
		impl_a->realloc = bson_realloc_ctx;
		impl_a->realloc_func_ctx = 0; // NULL
	}

	return b;
}

bson_t* bson_native_ex::s_bson_init_from_data(bson_t* bson, const u8* data, ssize_t len)
{
	assert(bson && data);
	if(!(bson && data))
	{
		return 0;
	}

	uint32_t len_le = 0;

	if((len < 5) || (len > INT_MAX) || data [len - 1])
	{
		return 0;
	}

	memcpy (&len_le, data, sizeof(uint32_t));

	if(len != static_cast<ssize_t>(BSON_UINT32_FROM_LE (len_le)))
	{
		return 0;
	}

	this_type::s_bson_init_from_size(bson, len);
	memcpy (this_type::s_bson_data(bson), data, len);
	bson->len = static_cast<uint32_t>(len);

	return bson;
}

/*static*/
bson_t* bson_native_ex::s_bson_copy_to(const bson_t* psrc, bson_t* pdst)
{
	const uint8_t* data = 0;
	bson_impl_alloc_t* adst = 0;
	size_t len = 0;

	BSON_ASSERT (psrc);
	BSON_ASSERT (pdst);

	if((psrc->flags & BSON_FLAG_INLINE))
	{
#ifdef BSON_MEMCHECK
		pdst->len = psrc->len;
		pdst->canary = malloc(1);
		memcpy (pdst->padding, psrc->padding, sizeof pdst->padding);
#else
		memcpy (pdst, psrc, sizeof *pdst);
#endif
		pdst->flags = static_cast<bson_flags_t>(BSON_FLAG_STATIC | BSON_FLAG_INLINE);
		return pdst;
	}

	data = this_type::pro_s__bson_data(psrc);
	len = bson_next_power_of_two(static_cast<size_t>(psrc->len));

	adst = reinterpret_cast<bson_impl_alloc_t*>(pdst);

	adst->flags = BSON_FLAG_STATIC;
	adst->len = psrc->len;
	adst->parent = NULL;
	adst->depth = 0;
	
	if(adst->buf == NULL || adst->alloclen < len)
	{
		if(adst->buf && adst->buf == &adst->alloc)
		{
			bson_free(adst->alloc);
		}

		adst->alloc = reinterpret_cast<uint8_t*>(bson_malloc(len));
	}
	
	adst->buf = &adst->alloc;
	adst->buflen = &adst->alloclen;
	adst->offset = 0;

//	if(adst->alloclen < len)
//	{
//		if(adst->alloc)
//		{
//			bson_free(adst->alloc);
//		}
//
//		adst->alloc = reinterpret_cast<uint8_t*>(bson_malloc(len));
//	}

	adst->alloclen = len;
	adst->realloc = bson_realloc_ctx;
	adst->realloc_func_ctx = NULL;
	memcpy (adst->alloc, data, psrc->len);
	//std::copy(data, data + psrc->len, adst->alloc);

	return pdst;
}


/*static*/
bson_t* bson_native_ex::s_bson_copy(bson_t* pdst, const bson_t* psrc)
{
	assert((psrc && pdst));
	if(!(psrc && pdst))
	{
		return 0;
	}

	if(psrc == pdst)
	{
		return pdst;
	}

	if(!(psrc->len))
	{
		if(pdst->len)
		{
			bson_destroy(pdst);
		}
		memset(pdst, 0, sizeof(bson_t));
	}
	else
	{
		//::bson_copy_to(psrc, pdst); //a memory leak occurs when the data is not empty. //adst->alloc = bson_malloc (len)
		this_type::s_bson_copy_to(psrc, pdst);
	}

	return pdst;
}

/*static*/
bson_t* bson_native_ex::s_bson_copy_uninitialized(bson_t* pdst, const bson_t* psrc)
{
	assert((psrc && pdst));
	if(!(psrc && pdst))
	{
		return 0;
	}

	if(psrc == pdst)
	{
		return pdst;
	}

	//a memory leak occurs when the data is not empty. //adst->alloc = bson_malloc (len)
	::bson_copy_to(psrc, pdst);

	return pdst;
}

/*static*/
void bson_native_ex::s_bson_swap(bson_t* pl, bson_t* pr)
{
	assert((pl && pr));
	// root node and child node can not do exchange operations
	assert(!((pl->flags & BSON_FLAG_CHILD) || (pr->flags & BSON_FLAG_CHILD)));

	if(!(pl && pr) || (pl == pr))
	{
		return;
	}

	struct tag_swap
	{
		// !!! if use YGGR_STATIC_CONSTANT, error C2246 will occur !!!
		enum
		{
			E_zero = 0,
			E_inline = 1 << 0,
			E_not_inline = 1 << 1,

			E_group_zz = ((E_zero << 16) | E_zero),
			E_group_zi = ((E_zero << 16) | E_inline),
			E_group_zni = ((E_zero << 16) | E_not_inline),

			E_group_iz = ((E_inline << 16) | E_zero),
			E_group_ii = ((E_inline << 16) | E_inline),
			E_group_ini = (E_inline << 16 | E_not_inline),

			E_group_niz = ((E_not_inline) << 16 | E_zero),
			E_group_nii = ((E_not_inline) << 16 | E_inline),
			E_group_nini = ((E_not_inline) << 16 | E_not_inline),

			E_compile_u32 = 0xfffffff
		};
	};

	u32 lflag = pl->len?
				(pl->flags & BSON_FLAG_INLINE?
					tag_swap::E_inline : tag_swap::E_not_inline)
				: tag_swap::E_zero;

	u32 rflag = pr->len?
				(pr->flags & BSON_FLAG_INLINE?
					tag_swap::E_inline : tag_swap::E_not_inline)
				: tag_swap::E_zero;

	u32 flag = (lflag << 16) | rflag;

	switch(flag)
	{
	case tag_swap::E_group_zz:
		return;
	case tag_swap::E_group_zi:
	case tag_swap::E_group_iz:
	case tag_swap::E_group_ii:
		this_type::s_bson_swap_of_inline(pl, pr);
		return;

	case tag_swap::E_group_zni:
	case tag_swap::E_group_niz:
	case tag_swap::E_group_nini:
		this_type::s_bson_swap_of_not_inline(pl, pr);
		return;

	case tag_swap::E_group_ini:
	case tag_swap::E_group_nii:
		this_type::s_bson_swap_of_mix(pl, pr);
		return;
	};
}

// c_bson save_start save_end need a swap chain at serialization,
// but mongodb_c_driver not support parent->child, (^_^|||)
// ****lucky****(~@^_^@~), plenty of space for the bson_t.
// so ~~~~~~~~~~~~~~~~~~~

BSON_ALIGNED_BEGIN (128)
typedef struct
{
	bson_flags_t		flags;            /* flags describing the bson_t */
	uint32_t			len;              /* length of bson document in bytes */
	bson_t*				parent;           /* parent bson if a child */
	uint32_t			depth;            /* Subdocument depth. */
	uint8_t**			buf;              /* pointer to buffer pointer */
	size_t*				buflen;           /* pointer to buffer length */
	size_t				offset;           /* our offset inside *buf  */
	uint8_t*			alloc;            /* buffer that we own. */
	size_t				alloclen;         /* length of buffer that we own. */
	bson_realloc_func	realloc;          /* our realloc implementation */
	void*				realloc_func_ctx; /* context for our realloc func */
	bson_t*				pchild;			  // append pchild pointer
} inner_bson_impl_alloc_t BSON_ALIGNED_END (128);

// gcc linux x32 not supported
//BOOST_MPL_ASSERT((
//	boost::mpl::bool_<
//		(YGGR_PP_ALIGNED_STORAGE_SIZEOF(inner_bson_impl_alloc_t)
//		== YGGR_PP_ALIGNED_STORAGE_SIZEOF(bson_impl_alloc_t)) >));

BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_PP_ALIGNED_STORAGE_SIZEOF(inner_bson_impl_alloc_t) <= 128)>));
BOOST_MPL_ASSERT((boost::mpl::bool_<(YGGR_PP_ALIGNED_STORAGE_SIZEOF(bson_impl_alloc_t) <= YGGR_PP_ALIGNED_STORAGE_SIZEOF(inner_bson_impl_alloc_t))>));

/*static*/
void bson_native_ex::s_bson_mark_set_child(bson_t* pnow, bson_t* pchild)
{
	assert(!(pnow->flags & BSON_FLAG_INLINE));
	inner_bson_impl_alloc_t* pdata = reinterpret_cast<inner_bson_impl_alloc_t*>(pnow);
	pdata->pchild = pchild;
}

/*static*/
bson_t* bson_native_ex::s_bson_mark_get_child(bson_t* pnow)
{
	assert(!(pnow->flags & BSON_FLAG_INLINE));
	inner_bson_impl_alloc_t* pdata = reinterpret_cast<inner_bson_impl_alloc_t*>(pnow);
	return pdata->pchild;
}

/*static*/
void bson_native_ex::s_bson_swap_of_inline(bson_t* pl, bson_t* pr)
{
	assert(((pl->flags & BSON_FLAG_INLINE) || (0 == pl->len))
			&& ((pr->flags & BSON_FLAG_INLINE) || (0 == pr->len)));
	bson_t tmp = {0};
	memcpy(&tmp, pl, sizeof(bson_t));
	memcpy(pl, pr, sizeof(bson_t));
	memcpy(pr, &tmp, sizeof(bson_t));
}

void bson_native_ex::s_bson_swap_of_not_inline(bson_t* pl, bson_t* pr) // begin swap(this, helper) end swap(helper, this)
{
	assert((pl && pr));
	assert(!(pl->flags & BSON_FLAG_INLINE) || (pr->flags & BSON_FLAG_INLINE));

	if(!(pl && pr) || (pl == pr))
	{
		return;
	}

	bson_impl_alloc_t* pleft = reinterpret_cast<bson_impl_alloc_t*>(pl);
	bson_impl_alloc_t* pright = reinterpret_cast<bson_impl_alloc_t*>(pr);

	yggr::swap(pleft->alloc, pright->alloc);
	yggr::swap(pleft->alloclen, pright->alloclen);

	// must use self's address
	pleft->buf = &pleft->alloc;
	pleft->buflen = &pleft->alloclen;
	pright->buf = &pright->alloc;
	pright->buflen = &pright->alloclen;

	yggr::swap(pleft->flags, pright->flags);
	yggr::swap(pleft->len, pright->len);
	yggr::swap(pleft->parent, pright->parent);
	yggr::swap(pleft->depth, pright->depth);
	//yggr::swap(pleft->buf, pright->buf); // discard it, see the foo's upper
	//yggr::swap(pleft->buflen, pright->buflen); // discard it, see the foo's upper
	yggr::swap(pleft->offset, pright->offset);
	//yggr::swap(pleft->alloc, pright->alloc); // discard it, see the foo's upper
	//yggr::swap(pleft->alloclen, pright->alloclen); // discard it, see the foo's upper
	yggr::swap(pleft->realloc, pright->realloc);
	yggr::swap(pleft->realloc_func_ctx, pright->realloc_func_ctx);
}

/* static */
void bson_native_ex::s_bson_swap_of_mix(bson_t* pl, bson_t* pr)
{
	assert(!((pl->flags & BSON_FLAG_INLINE) && (pr->flags & BSON_FLAG_INLINE)));
	assert(((pl->flags & BSON_FLAG_INLINE) || (pr->flags & BSON_FLAG_INLINE)));
	bson_t* pi = pl;
	bson_t* pni = pr;
	if(!(pl->flags & BSON_FLAG_INLINE))
	{
		yggr::swap(pi, pni);
	}

	bson_t tmp = {0};
	memcpy(&tmp, pi, sizeof(bson_t));
	memset(pi, 0, sizeof(bson_t));

	this_type::s_bson_swap_of_not_inline(pi, pni);

	memcpy(pni, &tmp, sizeof(bson_t));
}

/*static*/
const u8* bson_native_ex::s_bson_data(const bson_t* pbs)
{
	return pbs && pbs->len? bson_get_data(pbs) : 0;
}

/*static*/
u8* bson_native_ex::s_bson_data (bson_t* pbs)
{
	if(!(pbs && pbs->len))
	{
		return 0;
	}

	if((pbs->flags & BSON_FLAG_INLINE))
	{
		return &((reinterpret_cast<bson_impl_inline_t*>(pbs))->data[0]);
	}
	else
	{
		bson_impl_alloc_t *pimpl = reinterpret_cast<bson_impl_alloc_t*>(pbs);
		return (*pimpl->buf) + pimpl->offset;
	}
}

/*static*/
u32 bson_native_ex::s_bson_size (const bson_t* pbs)
{
	return pbs? pbs->len : 0;
}

/*static*/
void bson_native_ex::s_bson_encode_length(bson_t* pbs)
{
	if(!(pbs && pbs->len))
	{
		return;
	}

#	if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
	memcpy(this_type::s_bson_data(pbs), &pbs->len, sizeof (pbs->len));
#	else
	uint32_t length_le = BSON_UINT32_TO_LE (pbs->len);
	memcpy(this_type::s_bson_data(pbs), &length_le, sizeof (length_le));
#	endif // BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
}

/*static*/
bool bson_native_ex::s_bson_impl_inline_grow(bson_impl_inline_t* pimpl, std::size_t size)
{
	bson_impl_alloc_t* alloc = reinterpret_cast<bson_impl_alloc_t*>(pimpl);
	uint8_t* data = 0;
	size_t req = 0;

	BSON_ASSERT(pimpl);
	BSON_ASSERT(!(pimpl->flags & BSON_FLAG_RDONLY));
	BSON_ASSERT(!(pimpl->flags & BSON_FLAG_CHILD));

	if((static_cast<size_t>(pimpl->len) + size) <= sizeof(pimpl->data))
	{
		return true;
	}

	req = bson_next_power_of_two (pimpl->len + size);

	if(req <= INT32_MAX)
	{
		data = reinterpret_cast<uint8_t*>(bson_malloc (req));

		memcpy (data, pimpl->data, pimpl->len);
		alloc->flags = static_cast<bson_flags_t>(alloc->flags & ~BSON_FLAG_INLINE);
		alloc->parent = NULL;
		alloc->depth = 0;
		alloc->buf = &alloc->alloc;
		alloc->buflen = &alloc->alloclen;
		alloc->offset = 0;
		alloc->alloc = data;
		alloc->alloclen = req;
		alloc->realloc = bson_realloc_ctx;
		alloc->realloc_func_ctx = NULL;

		return true;
	}

	return false;
}

/*static*/
bool bson_native_ex::s_bson_impl_alloc_grow(bson_impl_alloc_t* pimpl, std::size_t size)
{
	std::size_t req;

	BSON_ASSERT(pimpl);

	/*
	* Determine how many bytes we need for this document in the buffer
	* including necessary trailing bytes for parent documents.
	*/
	req = (pimpl->offset + pimpl->len + size + pimpl->depth);

	if(req <= *pimpl->buflen)
	{
		return true;
	}

	req = bson_next_power_of_two(req);

	if((req <= INT32_MAX) && pimpl->realloc)
	{
		*pimpl->buf = reinterpret_cast<uint8_t*>(pimpl->realloc (*pimpl->buf, req, pimpl->realloc_func_ctx));
		*pimpl->buflen = req;
		return true;
	}

	return false;
}

/*static*/
bool bson_native_ex::s_bson_grow(bson_t* pbs, uint32_t size)
{
	BSON_ASSERT(pbs);
	BSON_ASSERT(pbs->len);
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	return
		(pbs->flags & BSON_FLAG_INLINE)?
			this_type::s_bson_impl_inline_grow(reinterpret_cast<bson_impl_inline_t*>(pbs), size)
			: this_type::s_bson_impl_alloc_grow(reinterpret_cast<bson_impl_alloc_t*>(pbs), size);
}

/*static*/
char* bson_native_ex::s_bson_as_json_visit_all(const bson_t* bson,
												size_t* length,
												bson_json_mode_type mode,
												int32_t max_len)
{
	native_bson_json_state_t state = { 0 };
	bson_iter_t iter = { 0 };

	ssize_t err_offset = -1;
	int32_t remaining = 0;

	assert(bson);
	if(!bson)
	{
		return 0;
	}

	if(length)
	{
		*length = 0;
	}

	if(bson_empty0(bson))
	{
		if(length)
		{
			*length = 3;
		}

		return bson_string_native_ex::s_bson_strdup("{ }");
	}

	if(!bson_iter_init(&iter, bson))
	{
		return 0;
	}

	state.count = 0;
	state.keys = true;
	state.str = bson_string_new("{ ");
	state.depth = 0;
	state.err_offset = &err_offset;
	state.mode = static_cast<bson_json_mode_t>(mode);
	state.max_len = max_len;
	state.max_len_reached = false;

	if((bson_iterator_native_ex::s_bson_iter_visit_all(
			&iter, &(bson_iterator_native_ex::default_bson_as_json_visitors()), &state)
			|| err_offset != -1)
		&& !state.max_len_reached)
	{
		/*
		* We were prematurely exited due to corruption or failed visitor.
		*/
		bson_string_free(state.str, true);
		if(length)
		{
			*length = 0;
		}
		return 0;
	}

	/* Append closing space and } separately, in case we hit the max in between.
	*/
	remaining = state.max_len - state.str->len;
	if(state.max_len == BSON_MAX_LEN_UNLIMITED || remaining > 1)
	{
		bson_string_append(state.str, " }");
	}
	else if(remaining == 1)
	{
		bson_string_append(state.str, " ");
	}

	if(length)
	{
		*length = state.str->len;
	}

	return bson_string_free(state.str, false);
}


/*static*/
bool bson_native_ex::s_bson_is_nil(const bson_t* pbs)
{
	return !(pbs && pbs->len);
}

/*static*/
bool bson_native_ex::s_bson_is_validate(const bson_t* pbs, u32 flags, size_t* psize)
{
	return ::bson_validate(pbs, static_cast<bson_validate_flags_t>(flags), psize);
}

/*static*/
bool bson_native_ex::s_bson_is_validate(const bson_t* pbs)
{
	return ::bson_validate(pbs, static_cast<bson_validate_flags_t>(pbs->flags), 0);
}

/*static*/
bool bson_native_ex::s_bson_is_array(const bson_t* pbs)
{
	assert(pbs);

	if(this_type::s_bson_is_nil(pbs) || bson_empty(pbs))
	{
		return false;
	}

	bson_iter_t iter = {0};
	bson_iter_init(&iter, pbs);
	bson_iter_next(&iter);

	u32 first_obj_t = bson_iter_type(&iter);
	bool bonly = !bson_iter_next(&iter);

	return (bonly) && (first_obj_t == bson_typeid_def::E_BSON_TYPE_ARRAY);
}

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4267)
#endif

/*static*/
bool bson_native_ex::s_bson_concat_self(bson_t* pbs)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	if(this_type::s_bson_is_nil(pbs))
	{
		return false;
	}

	if(bson_empty(pbs))
	{
		return true;
	}

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	std::size_t byte_size = pbs->len - 5;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbson_data = this_type::s_bson_data(pbs) + 4;
	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, pbson_data, byte_size);

	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);

	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_root_iter(bson_t* pbs, const char* name, s32 nlen, const bson_iter_t* val)
{
	assert(val);
	assert(name);
	assert(nlen >= -1);
	if(!(val && bson_iterator_native_ex::s_bson_iter_is_root(val)))
	{
		return false;
	}

	if(this_type::s_bson_is_nil(pbs))
	{
		bson_init(pbs);
	}

	if(this_type::s_bson_data(pbs) == val->raw)
	{
		return this_type::s_bson_save_self(pbs, name, nlen);
	}

	if(nlen < 0)
	{
		nlen = strlen(name);
	}

	u8 type = static_cast<u8>(bson_typeid_def::E_BSON_TYPE_DOCUMENT);

	// if iter's bson_id is E_BSON_TYPE_ARRAY or E_BSON_TYPE_DOCUMENT need this grow
	if((pbs->flags & BSON_FLAG_INLINE))
	{
		BSON_ASSERT(pbs->len <= 120);
		if(!this_type::s_bson_grow(pbs, 128 - pbs->len))
		{
			return false;
		}
		BSON_ASSERT(!(pbs->flags & BSON_FLAG_INLINE));
	}

	std::size_t key_byte_size = 1 + nlen + 1;
	std::size_t data_byte_size = val->len;
	std::size_t byte_size = key_byte_size + data_byte_size;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	const u8* pval_buf = val->raw;

	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, &type, sizeof(u8));
	memcpy(pbuf + sizeof(u8), &(name[0]), nlen);
	pbuf[key_byte_size - 1] = 0;
	memcpy(pbuf + key_byte_size, pval_buf, data_byte_size);
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_iter(bson_t* pbs, const char* name, s32 nlen, const bson_iter_t* val)
{
	assert(val);
	assert(name);
	assert(nlen >= -1);

	if(!val)
	{
		return false;
	}

	if(!bson_iterator_native_ex::s_bson_iter_validate(val))
	{
		return false;
	}

	if(bson_iterator_native_ex::s_bson_iter_is_root(val))
	{
		return this_type::s_bson_save_root_iter(pbs, name, nlen, val);
	}

	if(this_type::s_bson_is_nil(pbs))
	{
		bson_init(pbs);
	}

	if(nlen < 0)
	{
		nlen = strlen(name);
	}

	std::size_t val_t_byte_size = 1;
	std::size_t data_byte_size = val->next_off - val->d1;

	u8 type = *(val->raw + val->type);

	bool is_same_addr = this_type::s_bson_data(pbs) == val->raw;

	if((type == bson_typeid_def::E_BSON_TYPE_ARRAY)
		|| (type == bson_typeid_def::E_BSON_TYPE_DOCUMENT))
	{
		if((pbs->flags & BSON_FLAG_INLINE))
		{
			BSON_ASSERT(pbs->len <= 120);
			if(!this_type::s_bson_grow(pbs, 128 - pbs->len))
			{
				return false;
			}
			BSON_ASSERT(!(pbs->flags & BSON_FLAG_INLINE));
		}
	}

	std::size_t key_byte_size = val_t_byte_size + nlen + 1;
	std::size_t byte_size = key_byte_size + data_byte_size ;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	if(is_same_addr && (this_type::s_bson_data(pbs) != val->raw))
	{
		(const_cast<bson_iter_t*>(val))->raw = this_type::s_bson_data(pbs);
	}

	const u8* pval_t_buf = val->raw + val->type;
	const u8* pval_buf = val->raw + val->d1;

	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, pval_t_buf, sizeof(u8));
	memcpy(pbuf + sizeof(u8), &(name[0]), nlen);
	pbuf[key_byte_size - 1] = 0;
	memcpy(pbuf + key_byte_size, pval_buf, data_byte_size);
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_self(bson_t* pbs, const char* name, s32 nlen)
{
	assert((pbs) && (name) && (nlen >= -1));

	if(!(name && nlen))
	{
		return false;
	}

	nlen = nlen < 0? std::strlen(name) : nlen;

	assert((std::strlen(name) == nlen));
	assert(!this_type::s_bson_is_nil(pbs));
	if(this_type::s_bson_is_nil(pbs))
	{
		return false;
	}

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

		/*
	* If the parent is an inline bson_t, then we need to convert
	* it to a heap allocated buffer. This makes extending buffers
	* of child bson documents much simpler logic, as they can just
	* realloc the *buf pointer.
	*/

	u8 type = static_cast<u8>(bson_typeid_def::E_BSON_TYPE_DOCUMENT);
	if((type == bson_typeid_def::E_BSON_TYPE_ARRAY)
		|| (type == bson_typeid_def::E_BSON_TYPE_DOCUMENT))
	{
		if((pbs->flags & BSON_FLAG_INLINE))
		{
			BSON_ASSERT(pbs->len <= 120);
			if(!this_type::s_bson_grow(pbs, 128 - pbs->len))
			{
				return false;
			}
			BSON_ASSERT(!(pbs->flags & BSON_FLAG_INLINE));
		}
	}

	std::size_t key_byte_size = 1 + nlen + 1;
	std::size_t data_byte_size = pbs->len;
	std::size_t byte_size = key_byte_size + data_byte_size;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbson_data = this_type::s_bson_data(pbs);
	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, &type, sizeof(u8));
	memcpy(pbuf + sizeof(u8), &(name[0]), nlen);
	pbuf[key_byte_size - 1] = 0;
	memcpy(pbuf + key_byte_size, pbson_data, data_byte_size);
	pbuf[byte_size - 1] = 0; // fix type value of 1 byte
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_name(bson_t* pbs, u32 ntype, const char* name, s32 nlen)
{
	assert((pbs) && (name) && (nlen >= -1));

	if(!(name && nlen))
	{
		return false;
	}

	if(this_type::s_bson_is_nil(pbs))
	{
		bson_init(pbs);
	}

	nlen = nlen < 0? strlen(name) : nlen;

	assert((std::strlen(name) == nlen));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	 /*
    * If the parent is an inline bson_t, then we need to convert
    * it to a heap allocated buffer. This makes extending buffers
    * of child bson documents much simpler logic, as they can just
    * realloc the *buf pointer.
    */

	assert(!(ntype >= bson_typeid_def::E_BSON_TYPE_YGGR_EX_BEG
				&& ntype < bson_typeid_def::E_BSON_TYPE_MINKEY));

	u8 type = static_cast<u8>(ntype);
	if((type == bson_typeid_def::E_BSON_TYPE_ARRAY)
		|| (type == bson_typeid_def::E_BSON_TYPE_DOCUMENT))
	{
		if((pbs->flags & BSON_FLAG_INLINE))
		{
			BSON_ASSERT(pbs->len <= 120);
			if(!this_type::s_bson_grow(pbs, 128 - pbs->len))
			{
				return false;
			}
			BSON_ASSERT(!(pbs->flags & BSON_FLAG_INLINE));
		}
	}

	std::size_t byte_size = 1 + nlen + 1;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, &type, sizeof(u8));
	memcpy(pbuf + sizeof(u8), &(name[0]), nlen);
	pbuf[byte_size - 1] = 0; // fix name's \0
#if _DEBUG
	pbuf[byte_size] = type;
#else
	pbuf[byte_size] = 0;
#endif // _DEBUG
	pbs->len += byte_size;
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_s32(bson_t* pbs, s32 val)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_INT32));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	s32 val_le = BSON_UINT32_TO_LE(val);
	std::size_t byte_size = sizeof(s32);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &val_le, sizeof(s32));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_s64(bson_t* pbs, s64 val)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_INT64));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	s64 val_le = BSON_UINT64_TO_LE(val);
	std::size_t byte_size = sizeof(s64);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &val_le, sizeof(s64));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_bool(bson_t* pbs, bool val)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_BOOL));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	u8 val_le = val;
	std::size_t byte_size = sizeof(u8);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &val_le, sizeof(u8));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_f64(bson_t* pbs, f64 val)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_DOUBLE));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	f64 val_le = BSON_DOUBLE_TO_LE(val);
	std::size_t byte_size = sizeof(f64);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &val_le, sizeof(f64));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_root_iter(bson_t* pbs, const bson_iter_t* val)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(val);
	if(!val)
	{
		return false;
	}

	if(!bson_iterator_native_ex::s_bson_iter_is_root(val))
	{
		return false;
	}

	if(this_type::s_bson_data(pbs) == val->raw)
	{
		return this_type::s_bson_save_value_of_bson_self(pbs);
	}

	u8 type = bson_typeid_def::E_BSON_TYPE_DOCUMENT;

	assert(this_type::s_check_save_name(pbs, type));

	std::size_t byte_size = val->len;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	const u8* pval_buf = val->raw;
	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, pval_buf, byte_size);
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_iter(bson_t* pbs, const bson_iter_t* val)
{
	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(val);
	if(!val)
	{
		return false;
	}

	if(!bson_iterator_native_ex::s_bson_iter_validate(val))
	{
		return false;
	}

	if(bson_iterator_native_ex::s_bson_iter_is_root(val))
	{
		return this_type::s_bson_save_value_of_root_iter(pbs, val);
	}

	//std::size_t val_t_byte_size = 1;
	std::size_t data_byte_size = val->next_off - val->d1;

	u8 type = *(val->raw + val->type);

	assert(this_type::s_check_save_name(pbs, type));

	bool is_same_addr = this_type::s_bson_data(pbs) == val->raw;

	std::size_t byte_size = data_byte_size;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	if(is_same_addr && (this_type::s_bson_data(pbs) != val->raw))
	{
		(const_cast<bson_iter_t*>(val))->raw = this_type::s_bson_data(pbs);
	}

	const u8* pval_buf = val->raw + val->d1;

	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, pval_buf, data_byte_size);
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_bson_self(bson_t* pbs)
{

	assert(pbs);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_DOCUMENT));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	std::size_t byte_size = BSON_UINT32_FROM_LE((*(reinterpret_cast<uint32_t*>(this_type::s_bson_data(pbs)))));

	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbson_data = this_type::s_bson_data(pbs);
	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;

	memcpy(pbuf, pbson_data, byte_size);
	pbuf[byte_size] = 0;
	pbuf[byte_size - 1] = 0; //fix copyed stored type value at save name
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_array(bson_t* pbs, const bson_t* val)
{
	assert(pbs && val);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_ARRAY));
	return this_type::s_bson_save_value_of_bson_object(pbs, val);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_bson(bson_t* pbs, const bson_t* val)
{
	assert(pbs && val);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_DOCUMENT));
	return this_type::s_bson_save_value_of_bson_object(pbs, val);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_bson_object(bson_t* pbs, const bson_t* val)
{
	if(pbs == val)
	{
		return this_type::s_bson_save_value_of_bson_self(pbs);
	}

	assert(pbs && val);
	assert(val->len);
	if(!val->len)
	{
		return false;
	}

	assert(!this_type::s_bson_is_nil(pbs));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	std::size_t byte_size = val->len;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	const u8* pbson_data = this_type::s_bson_data(val);
	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;

	memcpy(pbuf, pbson_data, byte_size);
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_utf8(bson_t* pbs, const char* val, s32 nlen)
{
	assert((pbs) && (nlen >= -1));

	static char zero_buf[2] = {0};
	val = val? val : zero_buf;
	nlen = nlen < 0? strlen(val) : nlen;

	assert((std::strlen(val) == nlen));
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_UTF8));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	s32 str_len = nlen;
	s32 str_len_le = BSON_UINT32_TO_LE((str_len + 1));

	std::size_t byte_size = sizeof(s32) + str_len + 1;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &str_len_le, sizeof(s32));
	if(str_len)
	{
		memcpy(pbuf + sizeof(s32), &val[0], str_len);
	}
	pbuf[byte_size - 1] = 0; // fix string \0
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_binary_buffer(bson_t* pbs, u32 sub_t, const u8* org_buf, s32 nlen)
{
	assert(pbs && nlen >= 0);
	assert(!this_type::s_bson_is_nil(pbs));
	assert(this_type::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_BINARY));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	u8 subt8 = static_cast<u8>(sub_t);
	static u8 zero_buf[] = {0};

	const u8* pbin_buf = org_buf? org_buf : zero_buf;
	u32 bin_buf_len = org_buf == zero_buf? 0 : nlen;

	assert(pbin_buf);

	std::size_t byte_size = (sub_t == bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY_DEPRECATED?
								4 + 1 + 4 + bin_buf_len : 4 + 1 + bin_buf_len);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;

	if(sub_t == bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY_DEPRECATED)
	{
		u32 bin_buf_len_le = BSON_UINT32_TO_LE(bin_buf_len + 4);
		u32 bin_buf_deprecated_len_le = BSON_UINT32_TO_LE(bin_buf_len);
		memcpy(pbuf, &bin_buf_len_le, sizeof(u32));
		memcpy(pbuf + sizeof(u32), &subt8, sizeof(u8));
		memcpy(pbuf + sizeof(u32) + sizeof(u8), &bin_buf_deprecated_len_le, sizeof(u32));
		if(bin_buf_len)
		{
			memcpy(pbuf + sizeof(u32) + sizeof(u8) + sizeof(u32), pbin_buf, bin_buf_len);
		}
	}
	else
	{
		u32 bin_buf_len_le = BSON_UINT32_TO_LE(bin_buf_len);
		memcpy(pbuf, &bin_buf_len_le, sizeof(u32));
		memcpy(pbuf + sizeof(u32), &subt8, sizeof(u8));
		if(bin_buf_len)
		{
			memcpy(pbuf + sizeof(u32) + sizeof(u8), pbin_buf, bin_buf_len);
		}
	}

	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_date(bson_t* pbs, u64 date)
{
	assert(pbs);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_DATE_TIME));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	u64 val_le = BSON_UINT64_TO_LE(date);
	std::size_t byte_size = sizeof(u64);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = bson_native_ex::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &val_le, sizeof(u64));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_timestamp(bson_t* pbs, u32 tm, u32 inc)
{
	u64 timestamp = (static_cast<u64>(tm) << 32) | (static_cast<u64>(inc));
	return this_type::s_bson_save_value_of_timestamp(pbs, timestamp);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_timestamp(bson_t* pbs, u64 timestamp)
{
	assert(pbs);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_TIMESTAMP));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	u64 value_le = BSON_UINT64_TO_LE(timestamp);
	std::size_t byte_size = sizeof(u64);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = bson_native_ex::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, &value_le, sizeof(u64));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_oid(bson_t* pbs, const bson_oid_t* poid)
{
	assert(pbs && poid);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_OID));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	static const std::size_t byte_size = sizeof(bson_oid_t);
	BOOST_MPL_ASSERT((boost::mpl::bool_<((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value))>));
	//assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = bson_native_ex::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, poid, byte_size);
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_regex(bson_t* pbs,
													const char* regex,
													const char* opts)
{
	regex = regex? regex : "";
	opts = opts? opts : "";
	return this_type::s_bson_save_value_of_regex(pbs, regex, strlen(regex), opts, strlen(opts));
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_regex(bson_t* pbs,
													const char* regex, u32 regex_len,
													const char* opts, u32 opts_len)
{
	assert(pbs);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_REGEX));
	assert(regex);
	assert(opts);
	assert(strlen(regex) == regex_len);
	assert(strlen(opts) == opts_len);

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	u32 plen = regex_len + 1;
	u32 olen = opts_len + 1;

	std::size_t byte_size = plen + olen;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = bson_native_ex::s_bson_data(pbs) +  pbs->len - 1;
	if(regex_len)
	{
		memcpy(pbuf, regex, regex_len);
	}
	pbuf[plen - 1] = 0;

	if(opts_len)
	{
		memcpy(pbuf + plen, opts, opts_len);
	}

	pbuf[byte_size - 1] = 0;
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_string_item(bson_t* pbs, u32 bs_t, const char* str)
{
	str = str? str : "";
	return this_type::s_bson_save_value_of_string_item(pbs, bs_t, str, strlen(str));
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_string_item(bson_t* pbs, u32 bs_t, const char* str, u32 len)
{
	assert(pbs);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bs_t));

	assert(str);
	assert(strlen(str) == len);

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	std::size_t byte_size = 4 + len + 1;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u32 len_le = BSON_UINT32_TO_LE(len + 1);

	u8* pbuf = bson_native_ex::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, &len_le, sizeof(u32));
	memcpy(pbuf + sizeof(u32), str, len);
	pbuf[byte_size - 1] = 0;
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_code(bson_t* pbs, const char* code)
{
	code = code? code : "";
	return this_type::s_bson_save_value_of_string_item(pbs, bson_typeid_def::E_BSON_TYPE_CODE, code, strlen(code));
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_code(bson_t* pbs, const char* code, u32 code_len)
{
	return this_type::s_bson_save_value_of_string_item(pbs, bson_typeid_def::E_BSON_TYPE_CODE, code, code_len);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_symbol(bson_t* pbs, const char* symbol)
{
	symbol = symbol? symbol : "";
	return this_type::s_bson_save_value_of_string_item(pbs, bson_typeid_def::E_BSON_TYPE_SYMBOL, symbol, strlen(symbol));
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_symbol(bson_t* pbs, const char* symbol, u32 symbol_len)
{
	return this_type::s_bson_save_value_of_string_item(pbs, bson_typeid_def::E_BSON_TYPE_SYMBOL, symbol, symbol_len);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_code_w_scope(bson_t* pbs, const char* code, const bson_t* pscope)
{
	assert(pscope);
	code = code? code : "";
	return this_type::s_bson_save_value_of_code_w_scope(pbs, code, strlen(code), pscope);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_code_w_scope(bson_t* pbs, const char* code, u32 len, const bson_t* pscope)
{
	assert(pbs && pscope);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_CODEWSCOPE));

	assert(code);
	assert(strlen(code) == len);

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	bson_t empty_scope = {0};
	bson_init(&empty_scope);

	pscope = bson_native_ex::s_bson_is_nil(pscope)? &empty_scope : pscope;

	u32 code_len = len + 1;

	std::size_t byte_size = 4 + 4 + code_len + pscope->len;
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u32 code_len_le = BSON_UINT32_TO_LE(code_len);

	u8* pbuf = bson_native_ex::s_bson_data(pbs) +  pbs->len - 1;

	u32 codews_len = byte_size;
	u32 codews_len_le = BSON_UINT32_TO_LE(codews_len);
	memcpy(pbuf, &codews_len_le, sizeof(u32));
	memcpy(pbuf + sizeof(u32), &code_len_le, sizeof(u32));
	memcpy(pbuf + sizeof(u32) + sizeof(u32), code, len);
	pbuf[sizeof(u32) + sizeof(u32) + len] = 0;
	memcpy(pbuf + sizeof(u32) + sizeof(u32) + code_len, bson_native_ex::s_bson_data(pscope), pscope->len);

	pbuf[byte_size] = 0;
	pbs->len += byte_size;
	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_dbpointer(bson_t* pbs, const char* str_col, const bson_oid_t* poid)
{
	assert(str_col && poid);
	return this_type::s_bson_save_value_of_dbpointer(pbs, str_col, strlen(str_col), poid);
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_dbpointer(bson_t* pbs, const char* str_col, u32 len, const bson_oid_t* poid)
{
	assert(pbs && poid);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_DBPOINTER));

	assert(str_col);
	assert(strlen(str_col) == len);

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	std::size_t byte_size = 4 + len + 1 + sizeof(bson_oid_t);
	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u32 len_le = BSON_UINT32_TO_LE(len + 1);

	u8* pbuf = bson_native_ex::s_bson_data(pbs) + pbs->len - 1;
	memcpy(pbuf, &len_le, sizeof(u32));
	memcpy(pbuf + sizeof(u32), str_col, len);
	pbuf[sizeof(u32) + len] = 0;
	memcpy(pbuf + sizeof(u32) + len + 1, poid, sizeof(bson_oid_t));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_decimal128(bson_t* pbs, const bson_decimal128_t* pdecimal128)
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(bson_decimal128_t) == 16>));
	assert(pbs && pdecimal128);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, bson_typeid_def::E_BSON_TYPE_DECIMAL128));

	BSON_ASSERT(!(pbs->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT(!(pbs->flags & BSON_FLAG_RDONLY));

	u64 value_le[2] = { BSON_UINT64_TO_LE (pdecimal128->low), BSON_UINT64_TO_LE (pdecimal128->high) };
	std::size_t byte_size = sizeof(bson_decimal128_t);

	assert((byte_size > 0) && (byte_size < mplex::numeric_limits<u32>::max_type::value));

	if(BSON_UNLIKELY(byte_size > (BSON_MAX_SIZE - pbs->len)))
	{
		this_type::pro_s_bson_save_name_undo(pbs);
		return false;
	}

	if(BSON_UNLIKELY(!this_type::s_bson_grow(pbs, byte_size)))
	{
		return false;
	}

	u8* pbuf = this_type::s_bson_data(pbs) +  pbs->len - 1;
	memcpy(pbuf, value_le, sizeof(bson_decimal128_t));
	pbuf[byte_size] = 0;
	pbs->len += byte_size;

	this_type::s_bson_encode_length(pbs);
	return true;
}

/*static*/
bool bson_native_ex::s_bson_save_value_of_value(bson_t* pbs, const bson_value_t* pval)
{
	assert(pbs && pval);
	assert(!bson_native_ex::s_bson_is_nil(pbs));
	assert(bson_native_ex::s_check_save_name(pbs, pval->value_type));

	switch(pval->value_type)
	{
	case bson_typeid_def::E_BSON_TYPE_DOUBLE:
		return this_type::s_bson_save_value_of_f64(pbs, pval->value.v_double);

	case bson_typeid_def::E_BSON_TYPE_UTF8:
		return this_type::s_bson_save_value_of_utf8(pbs, pval->value.v_utf8.str, pval->value.v_utf8.len);

	case bson_typeid_def::E_BSON_TYPE_DOCUMENT:
		{
			bson_t local = {0};
			bool bret = false;
			if(bson_init_static(
					&local,
					pval->value.v_doc.data,
					pval->value.v_doc.data_len))
			{
				bret = this_type::s_bson_save_value_of_bson(pbs, &local);
				bson_destroy (&local);
				return bret;
			}
		}
		return false;

	case bson_typeid_def::E_BSON_TYPE_ARRAY:
		{
			bson_t local = {0};
			bool bret = false;
			if(bson_init_static(
					&local,
					pval->value.v_doc.data,
					pval->value.v_doc.data_len))
			{
				bret = this_type::s_bson_save_value_of_array(pbs, &local);
				bson_destroy (&local);
				return bret;
			}
		}
		return false;

	case bson_typeid_def::E_BSON_TYPE_BINARY:
		return
			this_type::s_bson_save_value_of_binary_buffer(
				pbs,
				pval->value.v_binary.subtype,
				pval->value.v_binary.data,
				pval->value.v_binary.data_len);

	case bson_typeid_def::E_BSON_TYPE_NULL:
		return this_type::s_bson_save_value_of_null(pbs);

	case bson_typeid_def::E_BSON_TYPE_UNDEFINED:
		return this_type::s_bson_save_value_of_undefined(pbs);

	case bson_typeid_def::E_BSON_TYPE_OID:
		return this_type::s_bson_save_value_of_oid(pbs, &(pval->value.v_oid));

	case bson_typeid_def::E_BSON_TYPE_BOOL:
		return this_type::s_bson_save_value_of_bool(pbs, pval->value.v_bool);

	case bson_typeid_def::E_BSON_TYPE_DATE_TIME:
		return this_type::s_bson_save_value_of_date(pbs, pval->value.v_datetime);

	case bson_typeid_def::E_BSON_TYPE_REGEX:
		return
			this_type::s_bson_save_value_of_regex(
				pbs,
				pval->value.v_regex.regex,
				pval->value.v_regex.options);

	case bson_typeid_def::E_BSON_TYPE_CODE:
		return this_type::s_bson_save_value_of_code(pbs, pval->value.v_code.code);

	case bson_typeid_def::E_BSON_TYPE_SYMBOL:
		return
			this_type::s_bson_save_value_of_symbol(
				pbs,
				pval->value.v_symbol.symbol,
				pval->value.v_symbol.len);

	case bson_typeid_def::E_BSON_TYPE_CODEWSCOPE:
		{
			bson_t local = {0};
			bool bret = false;
			if(bson_init_static(
					&local,
					pval->value.v_codewscope.scope_data,
					pval->value.v_codewscope.scope_len))
			{
				bret = this_type::s_bson_save_value_of_code_w_scope(pbs, pval->value.v_codewscope.code, &local);
				bson_destroy (&local);
				return bret;
			}
		}
		return false;

	case bson_typeid_def::E_BSON_TYPE_DBPOINTER:
		return
			this_type::s_bson_save_value_of_dbpointer(
				pbs,
				pval->value.v_dbpointer.collection,
				&(pval->value.v_dbpointer.oid));

	case bson_typeid_def::E_BSON_TYPE_INT32:
		return this_type::s_bson_save_value_of_s32(pbs, pval->value.v_int32);

	case bson_typeid_def::E_BSON_TYPE_INT64:
		return this_type::s_bson_save_value_of_s64(pbs, pval->value.v_int64);

	case bson_typeid_def::E_BSON_TYPE_TIMESTAMP:
		return
			this_type::s_bson_save_value_of_timestamp(
				pbs,
				pval->value.v_timestamp.timestamp,
				pval->value.v_timestamp.increment);

	case bson_typeid_def::E_BSON_TYPE_DECIMAL128:
		return
			this_type::s_bson_save_value_of_decimal128(
				pbs,
				&(pval->value.v_decimal128));

	case bson_typeid_def::E_BSON_TYPE_MAXKEY:
		return this_type::s_bson_save_value_of_maxkey(pbs);

	case bson_typeid_def::E_BSON_TYPE_MINKEY:
		return this_type::s_bson_save_value_of_minkey(pbs);

	case bson_typeid_def::E_BSON_TYPE_EOD:
	default:
		break;
	}

	return false;
}

#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#	ifdef _DEBUG
/*static*/
bool bson_native_ex::s_check_save_name(bson_t* pbs, u32 nchk_num)
{
	assert((pbs && pbs->len));
	u8 chk_num = static_cast<u8>(nchk_num);
	u8* pbuf = this_type::s_bson_data(pbs) + pbs->len - 1;
	assert(pbuf);
	return *pbuf == chk_num;
}
#	endif //_DEBUG

/*static*/
bool bson_native_ex::pro_s_bson_save_name_undo(bson_t* pbs)
{
	assert(pbs);
	if(!pbs)
	{
		return false;
	}

	uint32_t old_len = BSON_UINT32_FROM_LE((*(reinterpret_cast<uint32_t*>(this_type::s_bson_data(pbs)))));

	assert(old_len <= pbs->len);

	if(old_len == pbs->len)
	{
		return false;
	}

	pbs->len = old_len;
	*(this_type::s_bson_data(pbs) + old_len - 1) = 0;

	return true;
}

/*static*/
uint8_t* bson_native_ex::pro_s__bson_data(const bson_t* pbs) /* IN */
{
	if((pbs->flags & BSON_FLAG_INLINE))
	{
		return const_cast<uint8_t*>((reinterpret_cast<const bson_impl_inline_t*>(pbs))->data);
	}
	else
	{
		//const bson_impl_alloc_t* impl = reinterpret_cast<const bson_impl_alloc_t*>(pbs);
		const void* vpbs = static_cast<const void*>(pbs);
		const bson_impl_alloc_t* impl = reinterpret_cast<const bson_impl_alloc_t*>(vpbs);
		return const_cast<uint8_t*>((*impl->buf) + impl->offset);
	}
}

} //namespace nsql_database_system
} //namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_t& l, bson_t& r)
{
	bson_native_ex::s_bson_swap(boost::addressof(l), boost::addressof(r));
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr
