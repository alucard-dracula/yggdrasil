/*
 * Copyright 2009-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <bson/bson.h>
#include <bson/bson-config.h>
#include <bson/bson-private.h>
#include <bson/bson-json-private.h>
#include <common-string-private.h>
#include <common-json-private.h>
#include <common-macros-private.h>
#include <bson/bson-iso8601-private.h>
#include <common-cmp-private.h>

#include <string.h>
#include <math.h>

#ifdef BSON_MEMCHECK
#pragma message( \
   "Do not define BSON_MEMCHECK. BSON_MEMCHECK changes the data layout of bson_t. BSON_MEMCHECK is deprecated may be removed in a future major release")
#endif


typedef enum {
   BSON_VALIDATE_PHASE_START,
   BSON_VALIDATE_PHASE_TOP,
   BSON_VALIDATE_PHASE_LF_REF_KEY,
   BSON_VALIDATE_PHASE_LF_REF_UTF8,
   BSON_VALIDATE_PHASE_LF_ID_KEY,
   BSON_VALIDATE_PHASE_LF_DB_KEY,
   BSON_VALIDATE_PHASE_LF_DB_UTF8,
   BSON_VALIDATE_PHASE_NOT_DBREF,
} bson_validate_phase_t;


/*
 * Structures.
 */
typedef struct {
   bson_validate_flags_t flags;
   ssize_t err_offset;
   bson_validate_phase_t phase;
   bson_error_t error;
} bson_validate_state_t;


/*
 * Globals.
 */
static const uint8_t gZero;

/*
 *--------------------------------------------------------------------------
 *
 * _bson_round_up_alloc_size --
 *
 *       Given a potential allocation length in bytes, round up to the
 *       next power of two without exceeding BSON_MAX_SIZE.
 *
 * Returns:
 *       If the input is <= BSON_MAX_SIZE, returns a value >= the input
 *       and still <= BSON_MAX_SIZE. If the input was greater than
 *       BSON_MAX_SIZE, it is returned unmodified.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

static BSON_INLINE size_t
_bson_round_up_alloc_size (size_t size)
{
   if (size <= BSON_MAX_SIZE) {
      size_t power_of_two = bson_next_power_of_two (size);
      return BSON_MIN (power_of_two, BSON_MAX_SIZE);
   } else {
      return size;
   }
}

/*
 *--------------------------------------------------------------------------
 *
 * _bson_impl_inline_grow --
 *
 *       Document growth implementation for documents that currently
 *       contain stack based buffers. The document may be switched to
 *       a malloc based buffer.
 *
 * Returns:
 *       true if successful; otherwise false indicating BSON_MAX_SIZE overflow.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//static bool
//_bson_impl_inline_grow (bson_impl_inline_t *impl, /* IN */
//                        uint32_t grow_size)       /* IN */
//{
//   bson_impl_alloc_t *alloc = (bson_impl_alloc_t *) impl;
//   uint8_t *data;
//
//   MONGOC_DEBUG_ASSERT ((size_t) impl->len <= BSON_MAX_SIZE);
//   MONGOC_DEBUG_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);
//   size_t req = (size_t) impl->len + (size_t) grow_size;
//
//   if (req <= sizeof impl->data) {
//      return true;
//   }
//
//   req = _bson_round_up_alloc_size (req);
//
//   if (req <= BSON_MAX_SIZE) {
//      data = bson_malloc (req);
//
//      memcpy (data, impl->data, impl->len);
//#ifdef BSON_MEMCHECK
//      bson_free (impl->canary);
//#endif
//      alloc->flags &= ~BSON_FLAG_INLINE;
//      alloc->parent = NULL;
//      alloc->depth = 0;
//      alloc->buf = &alloc->alloc;
//      alloc->buflen = &alloc->alloclen;
//      alloc->offset = 0;
//      alloc->alloc = data;
//      alloc->alloclen = req;
//      alloc->realloc = bson_realloc_ctx;
//      alloc->realloc_func_ctx = NULL;
//
//      return true;
//   }
//
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

static bool
_bson_impl_inline_grow (bson_impl_inline_t *impl, /* IN */
                        uint32_t grow_size)       /* IN */
{
   bson_impl_alloc_t *alloc = (bson_impl_alloc_t *) impl;
   uint8_t *data;

   MONGOC_DEBUG_ASSERT ((size_t) impl->len <= BSON_MAX_SIZE);
   MONGOC_DEBUG_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);
   size_t req = (size_t) impl->len + (size_t) grow_size;

   if (req <= sizeof impl->data) {
      return true;
   }

   req = _bson_round_up_alloc_size (req);

   if (req <= BSON_MAX_SIZE) {
      data = bson_malloc (req);

      memcpy (data, impl->data, impl->len);
#ifdef BSON_MEMCHECK
      bson_free (impl->canary);
#endif
      alloc->flags &= ~BSON_FLAG_INLINE;
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

#else

static bool
_bson_impl_inline_grow (bson_impl_inline_t *impl, /* IN */
                        uint32_t grow_size)       /* IN */
{
   bson_impl_alloc_t *alloc = (bson_impl_alloc_t *) impl;
   uint8_t *data = 0;
   size_t req = 0;

   MONGOC_DEBUG_ASSERT ((size_t) impl->len <= BSON_MAX_SIZE);
   MONGOC_DEBUG_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);
   req = (size_t) impl->len + (size_t) grow_size;

   if (req <= sizeof impl->data) {
      return true;
   }

   req = _bson_round_up_alloc_size (req);

   if (req <= BSON_MAX_SIZE) {
      data = bson_malloc (req);

      memcpy (data, impl->data, impl->len);
#ifdef BSON_MEMCHECK
      bson_free (impl->canary);
#endif
      alloc->flags &= ~BSON_FLAG_INLINE;
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

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

/*
 *--------------------------------------------------------------------------
 *
 * _bson_impl_alloc_grow --
 *
 *       Document growth implementation for non-inline documents, possibly
 *       containing a reallocatable buffer.
 *
 * Returns:
 *       true if successful; otherwise false indicating BSON_MAX_SIZE overflow
 *       or an attempt to grow a buffer with no realloc implementation.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//static bool
//_bson_impl_alloc_grow (bson_impl_alloc_t *impl, /* IN */
//                       uint32_t grow_size)      /* IN */
//{
//   /* Determine how many bytes we need for this document in the buffer
//    * including necessary trailing bytes for parent documents.
//    *
//    * On size assumptions: the previous grow operation has already checked
//    * (len + offset + previous_depth) against BSON_MAX_SIZE. Current depth can be at most (previous_depth + 1). The
//    * caller has checked grow_size against BSON_MAX_SIZE. On the smallest (32-bit) supported size_t, we can still add
//    * these maximum values (2x BSON_MAX_SIZE, 1 additional byte of depth) without arithmetic overflow.
//    */
//   MONGOC_DEBUG_ASSERT ((uint64_t) impl->len + (uint64_t) impl->offset + (uint64_t) impl->depth <=
//                        (uint64_t) BSON_MAX_SIZE);
//   MONGOC_DEBUG_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);
//   size_t req = impl->offset + (size_t) impl->len + (size_t) grow_size + (size_t) impl->depth;
//
//   if (req <= *impl->buflen) {
//      return true;
//   }
//
//   req = _bson_round_up_alloc_size (req);
//
//   if ((req <= BSON_MAX_SIZE) && impl->realloc) {
//      *impl->buf = impl->realloc (*impl->buf, req, impl->realloc_func_ctx);
//      *impl->buflen = req;
//      return true;
//   }
//
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

static bool
_bson_impl_alloc_grow (bson_impl_alloc_t *impl, /* IN */
                       uint32_t grow_size)      /* IN */
{
   /* Determine how many bytes we need for this document in the buffer
    * including necessary trailing bytes for parent documents.
    *
    * On size assumptions: the previous grow operation has already checked
    * (len + offset + previous_depth) against BSON_MAX_SIZE. Current depth can be at most (previous_depth + 1). The
    * caller has checked grow_size against BSON_MAX_SIZE. On the smallest (32-bit) supported size_t, we can still add
    * these maximum values (2x BSON_MAX_SIZE, 1 additional byte of depth) without arithmetic overflow.
    */
   MONGOC_DEBUG_ASSERT ((uint64_t) impl->len + (uint64_t) impl->offset + (uint64_t) impl->depth <=
                        (uint64_t) BSON_MAX_SIZE);
   MONGOC_DEBUG_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);
   size_t req = impl->offset + (size_t) impl->len + (size_t) grow_size + (size_t) impl->depth;

   if (req <= *impl->buflen) {
      return true;
   }

   req = _bson_round_up_alloc_size (req);

   if ((req <= BSON_MAX_SIZE) && impl->realloc) {
      *impl->buf = impl->realloc (*impl->buf, req, impl->realloc_func_ctx);
      *impl->buflen = req;
      return true;
   }

   return false;
}

#else

static bool
_bson_impl_alloc_grow (bson_impl_alloc_t *impl, /* IN */
						uint32_t grow_size)      /* IN */
{
	size_t req = 0;

	/* Determine how many bytes we need for this document in the buffer
	* including necessary trailing bytes for parent documents.
	*
	* On size assumptions: the previous grow operation has already checked
	* (len + offset + previous_depth) against BSON_MAX_SIZE. Current depth can be at most (previous_depth + 1). The
	* caller has checked grow_size against BSON_MAX_SIZE. On the smallest (32-bit) supported size_t, we can still add
	* these maximum values (2x BSON_MAX_SIZE, 1 additional byte of depth) without arithmetic overflow.
	*/
	MONGOC_DEBUG_ASSERT ((uint64_t) impl->len + (uint64_t) impl->offset + (uint64_t) impl->depth <=
						(uint64_t) BSON_MAX_SIZE);
	MONGOC_DEBUG_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);
	req = impl->offset + (size_t) impl->len + (size_t) grow_size + (size_t) impl->depth;

	if (req <= *impl->buflen) {
		return true;
	}

	req = _bson_round_up_alloc_size (req);

	if ((req <= BSON_MAX_SIZE) && impl->realloc) {
		*impl->buf = impl->realloc (*impl->buf, req, impl->realloc_func_ctx);
		*impl->buflen = req;
		return true;
	}

	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


/*
 *--------------------------------------------------------------------------
 *
 * _bson_grow --
 *
 *       Grows the bson_t structure to be large enough to contain @grow_size
 *       bytes in addition to its current content.
 *
 *       The caller is responsible for ensuring @grow_size itself is not
 *       above BSON_MAX_SIZE, but a final determination of overflow status
 *       can't be made until we are inside _bson_impl_*_grow().
 *
 * Returns:
 *       true if successful, false if the size would overflow or the buffer
 *       needs to grow but does not support reallocation.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

static bool
_bson_grow (bson_t *bson,       /* IN */
            uint32_t grow_size) /* IN */
{
   BSON_ASSERT ((size_t) grow_size <= BSON_MAX_SIZE);

   if ((bson->flags & BSON_FLAG_INLINE)) {
      return _bson_impl_inline_grow ((bson_impl_inline_t *) bson, grow_size);
   }

   return _bson_impl_alloc_grow ((bson_impl_alloc_t *) bson, grow_size);
}


/*
 *--------------------------------------------------------------------------
 *
 * _bson_data --
 *
 *       A helper function to return the contents of the bson document
 *       taking into account the polymorphic nature of bson_t.
 *
 * Returns:
 *       A buffer which should not be modified or freed.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

static BSON_INLINE uint8_t *
_bson_data (const bson_t *bson) /* IN */
{
   if ((bson->flags & BSON_FLAG_INLINE)) {
      return ((bson_impl_inline_t *) bson)->data;
   } else {
      bson_impl_alloc_t *impl = (bson_impl_alloc_t *) bson;
      return (*impl->buf) + impl->offset;
   }
}


/*
 *--------------------------------------------------------------------------
 *
 * _bson_encode_length --
 *
 *       Helper to encode the length of the bson_t in the first 4 bytes
 *       of the bson document. Little endian format is used as specified
 *       by bsonspec.
 *
 * Returns:
 *       None.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

static BSON_INLINE void
_bson_encode_length (bson_t *bson) /* IN */
{
#if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
   memcpy (_bson_data (bson), &bson->len, sizeof (bson->len));
#else
   uint32_t length_le = BSON_UINT32_TO_LE (bson->len);
   memcpy (_bson_data (bson), &length_le, sizeof (length_le));
#endif
}


typedef struct _bson_append_bytes_arg {
   const uint8_t *bytes; // Not null.
   uint32_t length;      // > 0.
} _bson_append_bytes_arg;

typedef struct _bson_append_bytes_list {
   _bson_append_bytes_arg args[8];  // Arbitrary length: just needs to be large enough.
   _bson_append_bytes_arg *current; // "Insert"/"End" pointer.
   uint32_t n_bytes;                // Total bytes to be appended.
} _bson_append_bytes_list;

// To support unchecked cast from non-negative `int` to `size_t`.
BSON_STATIC_ASSERT2 (size_t_gte_int, SIZE_MAX >= INT_MAX);

// To support unchecked cast from `uint32_t` to `size_t`.
BSON_STATIC_ASSERT2 (size_t_gte_uint32_t, SIZE_MAX >= UINT32_MAX);

// Support largest _bson_impl_alloc_grow on smallest size_t
BSON_STATIC_ASSERT2 (max_alloc_grow_fits_min_sizet, (uint64_t) BSON_MAX_SIZE * 2u + 1u <= (uint64_t) UINT32_MAX);

// Declare local state with the identifier `ident`.
//#define BSON_APPEND_BYTES_LIST_DECLARE(ident)                                \
//   _bson_append_bytes_list ident = {.current = (ident).args, .n_bytes = 0u}; \
//   ((void) 0)

#if defined(YGGR_EX_C99_SUPPORTED)

#define BSON_APPEND_BYTES_LIST_DECLARE(ident)                                \
   _bson_append_bytes_list ident = {.current = (ident).args, .n_bytes = 0u}; \
   ((void) 0)

#else

#define BSON_APPEND_BYTES_LIST_DECLARE(ident)									\
	_bson_append_bytes_list ident = {{{0}}, (ident).args, 0u};					\
   ((void) 0)

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

// Add a bytes+length pair only if `_length > 0`.
// Append failure if `n_bytes` will exceed BSON max size.
//#define BSON_APPEND_BYTES_ADD_ARGUMENT(_list, _bytes, _length)        \
//   if (BSON_UNLIKELY ((_length) > BSON_MAX_SIZE - (_list).n_bytes)) { \
//      goto append_failure;                                            \
//   } else if ((_length) > 0) {                                        \
//      *(_list).current++ = (_bson_append_bytes_arg){                  \
//         .bytes = (const uint8_t *) (_bytes),                         \
//         .length = (_length),                                         \
//      };                                                              \
//      (_list).n_bytes += (_length);                                   \
//   } else                                                             \
//      ((void) 0)


#if defined(YGGR_EX_C99_SUPPORTED)

#define BSON_APPEND_BYTES_ADD_ARGUMENT(_list, _bytes, _length)        \
   if (BSON_UNLIKELY ((_length) > BSON_MAX_SIZE - (_list).n_bytes)) { \
      goto append_failure;                                            \
   } else if ((_length) > 0) {                                        \
      *(_list).current++ = (_bson_append_bytes_arg){                  \
         .bytes = (const uint8_t *) (_bytes),                         \
         .length = (_length),                                         \
      };                                                              \
      (_list).n_bytes += (_length);                                   \
   } else                                                             \
      ((void) 0)

#else

#define BSON_APPEND_BYTES_ADD_ARGUMENT(_list, _bytes, _length)			\
	if (BSON_UNLIKELY ((_length) > BSON_MAX_SIZE - (_list).n_bytes)) {	\
		goto append_failure;                                            \
	} else if ((_length) > 0) {											\
		_bson_append_bytes_arg tmp = {									\
			(const uint8_t *) (_bytes),									\
			(_length) };												\
		*(_list).current++ = tmp;										\
		(_list).n_bytes += (_length);                                   \
	} else																\
		((void) 0)

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

// Add a UTF-8 string only if no embedded null bytes are present.
// Uses `strlen (_key)` when `_key_len < 0`, otherwise uses `_key_len`.
//#define BSON_APPEND_BYTES_ADD_CHECKED_STRING(_list, _key, _key_len)      \
//   uint32_t BSON_CONCAT (key_ulen_, __LINE__);                           \
//   if ((_key_len) < 0) {                                                 \
//      const size_t key_zulen = strlen ((_key));                          \
//      if (BSON_UNLIKELY (key_zulen > UINT32_MAX)) {                      \
//         goto append_failure;                                            \
//      }                                                                  \
//      BSON_CONCAT (key_ulen_, __LINE__) = (uint32_t) key_zulen;          \
//   } else {                                                              \
//      const size_t key_zulen = (size_t) (_key_len);                      \
//      if (BSON_UNLIKELY (key_zulen > UINT32_MAX)) {                      \
//         goto append_failure;                                            \
//      } /* Necessary to validate embedded NULL is not present in key. */ \
//      else if (memchr ((_key), '\0', key_zulen) != NULL) {               \
//         goto append_failure;                                            \
//      } else {                                                           \
//         BSON_CONCAT (key_ulen_, __LINE__) = (uint32_t) key_zulen;       \
//      }                                                                  \
//   }                                                                     \
//   BSON_APPEND_BYTES_ADD_ARGUMENT ((_list), (_key), BSON_CONCAT (key_ulen_, __LINE__))

#if defined(YGGR_EX_C99_SUPPORTED)

#define BSON_APPEND_BYTES_ADD_CHECKED_STRING(_list, _key, _key_len)      \
   uint32_t BSON_CONCAT (key_ulen_, __LINE__);                           \
   if ((_key_len) < 0) {                                                 \
      const size_t key_zulen = strlen ((_key));                          \
      if (BSON_UNLIKELY (key_zulen > UINT32_MAX)) {                      \
         goto append_failure;                                            \
      }                                                                  \
      BSON_CONCAT (key_ulen_, __LINE__) = (uint32_t) key_zulen;          \
   } else {                                                              \
      const size_t key_zulen = (size_t) (_key_len);                      \
      if (BSON_UNLIKELY (key_zulen > UINT32_MAX)) {                      \
         goto append_failure;                                            \
      } /* Necessary to validate embedded NULL is not present in key. */ \
      else if (memchr ((_key), '\0', key_zulen) != NULL) {               \
         goto append_failure;                                            \
      } else {                                                           \
         BSON_CONCAT (key_ulen_, __LINE__) = (uint32_t) key_zulen;       \
      }                                                                  \
   }                                                                     \
   BSON_APPEND_BYTES_ADD_ARGUMENT ((_list), (_key), BSON_CONCAT (key_ulen_, __LINE__))

#else

#define BSON_APPEND_BYTES_ADD_CHECKED_STRING(_list, _key, _key_len)			\
	do {																	\
	uint32_t BSON_CONCAT (key_ulen_, __LINE__);								\
	if ((_key_len) < 0) {													\
		const size_t key_zulen = strlen ((_key));							\
		if (BSON_UNLIKELY (key_zulen > UINT32_MAX)) {						\
			goto append_failure;                                            \
		}																	\
		BSON_CONCAT (key_ulen_, __LINE__) = (uint32_t) key_zulen;			\
	} else {																\
		const size_t key_zulen = (size_t) (_key_len);						\
		if (BSON_UNLIKELY (key_zulen > UINT32_MAX)) {						\
			goto append_failure;                                            \
		} /* Necessary to validate embedded NULL is not present in key. */	\
		else if (memchr ((_key), '\0', key_zulen) != NULL) {				\
			goto append_failure;                                            \
		} else {															\
			BSON_CONCAT (key_ulen_, __LINE__) = (uint32_t) key_zulen;       \
		}																	\
	}																		\
	BSON_APPEND_BYTES_ADD_ARGUMENT ((_list), (_key), BSON_CONCAT (key_ulen_, __LINE__)) \
	; } while(0)


#endif // #if defined(YGGR_EX_C99_SUPPORTED)

// Apply the list of arguments to be appended to `_bson`.
// Append failure if adding `_list.n_bytes` will exceed BSON max size.
//#define BSON_APPEND_BYTES_APPLY_ARGUMENTS(_bson, _list)                                       \
//   if (BSON_UNLIKELY ((_list).n_bytes > BSON_MAX_SIZE - (_bson)->len)) {                      \
//      goto append_failure;                                                                    \
//   } else if (BSON_UNLIKELY (!_bson_grow ((_bson), (_list).n_bytes))) {                       \
//      goto append_failure;                                                                    \
//   } else {                                                                                   \
//      uint8_t *data = _bson_data ((_bson)) + ((_bson)->len - 1u);                             \
//      for (const _bson_append_bytes_arg *arg = (_list).args; arg != (_list).current; ++arg) { \
//         memcpy (data, arg->bytes, arg->length);                                              \
//         (_bson)->len += arg->length;                                                         \
//         data += arg->length;                                                                 \
//      }                                                                                       \
//      _bson_encode_length ((_bson));                                                          \
//      data[0] = '\0';                                                                         \
//   }                                                                                          \
//   ((void) 0)


#if defined(YGGR_EX_C99_SUPPORTED)

#define BSON_APPEND_BYTES_APPLY_ARGUMENTS(_bson, _list)                                       \
   if (BSON_UNLIKELY ((_list).n_bytes > BSON_MAX_SIZE - (_bson)->len)) {                      \
      goto append_failure;                                                                    \
   } else if (BSON_UNLIKELY (!_bson_grow ((_bson), (_list).n_bytes))) {                       \
      goto append_failure;                                                                    \
   } else {                                                                                   \
      uint8_t *data = _bson_data ((_bson)) + ((_bson)->len - 1u);                             \
      for (const _bson_append_bytes_arg *arg = (_list).args; arg != (_list).current; ++arg) { \
         memcpy (data, arg->bytes, arg->length);                                              \
         (_bson)->len += arg->length;                                                         \
         data += arg->length;                                                                 \
      }                                                                                       \
      _bson_encode_length ((_bson));                                                          \
      data[0] = '\0';                                                                         \
   }                                                                                          \
   ((void) 0)

#else

#define BSON_APPEND_BYTES_APPLY_ARGUMENTS(_bson, _list)											\
	if (BSON_UNLIKELY ((_list).n_bytes > BSON_MAX_SIZE - (_bson)->len)) {						\
		goto append_failure;                                                                    \
	} else if (BSON_UNLIKELY (!_bson_grow ((_bson), (_list).n_bytes))) {						\
		goto append_failure;                                                                    \
	} else {																					\
		uint8_t *data = _bson_data ((_bson)) + ((_bson)->len - 1u);                             \
		const _bson_append_bytes_arg *arg = (_list).args;										\
		for (; arg != (_list).current; ++arg) {													\
			memcpy (data, arg->bytes, arg->length);                                             \
			(_bson)->len += arg->length;                                                        \
			data += arg->length;                                                                \
		}                                                                                       \
		_bson_encode_length ((_bson));                                                          \
		data[0] = '\0';                                                                         \
	}																							\
	((void) 0)

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

/*
 *--------------------------------------------------------------------------
 *
 * _bson_append_bson_begin --
 *
 *       Begin appending a subdocument or subarray to the document using
 *       the key provided by @key.
 *
 *       If @key_length is < 0, then strlen() will be called on @key
 *       to determine the length.
 *
 *       @key_type MUST be either BSON_TYPE_DOCUMENT or BSON_TYPE_ARRAY.
 *
 * Returns:
 *       true if successful; otherwise false indicating BSON_MAX_SIZE overflow.
 *
 * Side effects:
 *       @child is initialized if true is returned.
 *
 *--------------------------------------------------------------------------
 */

//static bool
//_bson_append_bson_begin (bson_t *bson,           /* IN */
//                         const char *key,        /* IN */
//                         int key_length,         /* IN */
//                         bson_type_t child_type, /* IN */
//                         bson_t *child)          /* OUT */
//{
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (child);
//
//   BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
//   BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
//   BSON_ASSERT ((child_type == BSON_TYPE_DOCUMENT) || (child_type == BSON_TYPE_ARRAY));
//
//   {
//      BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//      const uint8_t type = (uint8_t) child_type;
//
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//      BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//      /*
//       * If the parent is an inline bson_t, then we need to convert
//       * it to a heap allocated buffer. This makes extending buffers
//       * of child bson documents much simpler logic, as they can just
//       * realloc the *buf pointer.
//       */
//      if ((bson->flags & BSON_FLAG_INLINE)) {
//         BSON_ASSERT (bson->len <= 120);
//         if (!_bson_grow (bson, 128 - bson->len)) {
//            return false;
//         }
//         BSON_ASSERT (!(bson->flags & BSON_FLAG_INLINE));
//      }
//
//      const uint8_t empty[5] = {5};
//
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &empty, sizeof (empty));
//
//      BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//      goto append_success;
//
//   append_failure:
//      return false;
//   }
//
//append_success:
//   ((void) 0);
//
//   bson_impl_alloc_t *aparent = (bson_impl_alloc_t *) bson;
//   bson_impl_alloc_t *achild = (bson_impl_alloc_t *) child;
//
//   /*
//    * Mark the document as working on a child document so that no
//    * further modifications can happen until the caller has called
//    * bson_append_{document,array}_end().
//    */
//   bson->flags |= BSON_FLAG_IN_CHILD;
//
//   /*
//    * Initialize the child bson_t structure and point it at the parents
//    * buffers. This allows us to realloc directly from the child without
//    * walking up to the parent bson_t.
//    */
//   achild->flags = (BSON_FLAG_CHILD | BSON_FLAG_NO_FREE | BSON_FLAG_STATIC);
//
//   if ((bson->flags & BSON_FLAG_CHILD)) {
//      achild->depth = ((bson_impl_alloc_t *) bson)->depth + 1;
//   } else {
//      achild->depth = 1;
//   }
//
//   achild->parent = bson;
//   achild->buf = aparent->buf;
//   achild->buflen = aparent->buflen;
//   achild->offset = aparent->offset + aparent->len - 1 - 5;
//   achild->len = 5;
//   achild->alloc = NULL;
//   achild->alloclen = 0;
//   achild->realloc = aparent->realloc;
//   achild->realloc_func_ctx = aparent->realloc_func_ctx;
//
//   return true;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

static bool
_bson_append_bson_begin (bson_t *bson,           /* IN */
                         const char *key,        /* IN */
                         int key_length,         /* IN */
                         bson_type_t child_type, /* IN */
                         bson_t *child)          /* OUT */
{
   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (child);

   BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
   BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
   BSON_ASSERT ((child_type == BSON_TYPE_DOCUMENT) || (child_type == BSON_TYPE_ARRAY));

   {
      BSON_APPEND_BYTES_LIST_DECLARE (args);

      const uint8_t type = (uint8_t) child_type;

      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
      BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

      /*
       * If the parent is an inline bson_t, then we need to convert
       * it to a heap allocated buffer. This makes extending buffers
       * of child bson documents much simpler logic, as they can just
       * realloc the *buf pointer.
       */
      if ((bson->flags & BSON_FLAG_INLINE)) {
         BSON_ASSERT (bson->len <= 120);
         if (!_bson_grow (bson, 128 - bson->len)) {
            return false;
         }
         BSON_ASSERT (!(bson->flags & BSON_FLAG_INLINE));
      }

      const uint8_t empty[5] = {5};

      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &empty, sizeof (empty));

      BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

      goto append_success;

   append_failure:
      return false;
   }

append_success:
   ((void) 0);

   bson_impl_alloc_t *aparent = (bson_impl_alloc_t *) bson;
   bson_impl_alloc_t *achild = (bson_impl_alloc_t *) child;

   /*
    * Mark the document as working on a child document so that no
    * further modifications can happen until the caller has called
    * bson_append_{document,array}_end().
    */
   bson->flags |= BSON_FLAG_IN_CHILD;

   /*
    * Initialize the child bson_t structure and point it at the parents
    * buffers. This allows us to realloc directly from the child without
    * walking up to the parent bson_t.
    */
   achild->flags = (BSON_FLAG_CHILD | BSON_FLAG_NO_FREE | BSON_FLAG_STATIC);

   if ((bson->flags & BSON_FLAG_CHILD)) {
      achild->depth = ((bson_impl_alloc_t *) bson)->depth + 1;
   } else {
      achild->depth = 1;
   }

   achild->parent = bson;
   achild->buf = aparent->buf;
   achild->buflen = aparent->buflen;
   achild->offset = aparent->offset + aparent->len - 1 - 5;
   achild->len = 5;
   achild->alloc = NULL;
   achild->alloclen = 0;
   achild->realloc = aparent->realloc;
   achild->realloc_func_ctx = aparent->realloc_func_ctx;

   return true;
}

#else

static bool
_bson_append_bson_begin (bson_t *bson,           /* IN */
							const char *key,        /* IN */
							int key_length,         /* IN */
							bson_type_t child_type, /* IN */
							bson_t *child)          /* OUT */
{
	bson_impl_alloc_t *aparent = 0;
	bson_impl_alloc_t *achild = 0;

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (child);

	BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
	BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT ((child_type == BSON_TYPE_DOCUMENT) || (child_type == BSON_TYPE_ARRAY));

	{
		const uint8_t empty[5] = {5};
		const uint8_t type = (uint8_t) child_type;
		BSON_APPEND_BYTES_LIST_DECLARE (args);

		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
		BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

		/*
		* If the parent is an inline bson_t, then we need to convert
		* it to a heap allocated buffer. This makes extending buffers
		* of child bson documents much simpler logic, as they can just
		* realloc the *buf pointer.
		*/
		if ((bson->flags & BSON_FLAG_INLINE)) {
			BSON_ASSERT (bson->len <= 120);
			if (!_bson_grow (bson, 128 - bson->len)) {
			return false;
			}
			BSON_ASSERT (!(bson->flags & BSON_FLAG_INLINE));
		}

		//const uint8_t empty[5] = {5};

		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &empty, sizeof (empty));

		BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

		goto append_success;

	append_failure:
		return false;
	}

append_success:
	((void) 0);

	aparent = (bson_impl_alloc_t *) bson;
	achild = (bson_impl_alloc_t *) child;

	/*
	* Mark the document as working on a child document so that no
	* further modifications can happen until the caller has called
	* bson_append_{document,array}_end().
	*/
	bson->flags |= BSON_FLAG_IN_CHILD;

	/*
	* Initialize the child bson_t structure and point it at the parents
	* buffers. This allows us to realloc directly from the child without
	* walking up to the parent bson_t.
	*/
	achild->flags = (BSON_FLAG_CHILD | BSON_FLAG_NO_FREE | BSON_FLAG_STATIC);

	if ((bson->flags & BSON_FLAG_CHILD)) {
		achild->depth = ((bson_impl_alloc_t *) bson)->depth + 1;
	} else {
		achild->depth = 1;
	}

	achild->parent = bson;
	achild->buf = aparent->buf;
	achild->buflen = aparent->buflen;
	achild->offset = aparent->offset + aparent->len - 1 - 5;
	achild->len = 5;
	achild->alloc = NULL;
	achild->alloclen = 0;
	achild->realloc = aparent->realloc;
	achild->realloc_func_ctx = aparent->realloc_func_ctx;

	return true;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


/*
 *--------------------------------------------------------------------------
 *
 * _bson_append_bson_end --
 *
 *       Complete a call to _bson_append_bson_begin.
 *
 * Returns:
 *       true if successful.
 *
 * Side effects:
 *       @child is destroyed and no longer valid after calling this
 *       function.
 *
 *--------------------------------------------------------------------------
 */

static bool
_bson_append_bson_end (bson_t *bson,  /* IN */
                       bson_t *child) /* IN */
{
   BSON_ASSERT (bson);
   BSON_ASSERT ((bson->flags & BSON_FLAG_IN_CHILD));
   BSON_ASSERT (!(child->flags & BSON_FLAG_IN_CHILD));

   /*
    * Unmark the IN_CHILD flag.
    */
   bson->flags &= ~BSON_FLAG_IN_CHILD;

   /*
    * Now that we are done building the sub-document, add the size to the
    * parent, not including the default 5 byte empty document already added.
    */
   bson->len = (bson->len + child->len - 5);

   /*
    * Ensure we have a \0 byte at the end and proper length encoded at
    * the beginning of the document.
    */
   _bson_data (bson)[bson->len - 1] = '\0';
   _bson_encode_length (bson);

   return true;
}


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_array_begin --
 *
 *       Start appending a new array.
 *
 *       Use @child to append to the data area for the given field.
 *
 *       It is a programming error to call any other bson function on
 *       @bson until bson_append_array_end() has been called. It is
 *       valid to call bson_append*() functions on @child.
 *
 *       This function is useful to allow building nested documents using
 *       a single buffer owned by the top-level bson document.
 *
 * Returns:
 *       true if successful; otherwise false and @child is invalid.
 *
 * Side effects:
 *       @child is initialized if true is returned.
 *
 *--------------------------------------------------------------------------
 */

bool
bson_append_array_begin (bson_t *bson,    /* IN */
                         const char *key, /* IN */
                         int key_length,  /* IN */
                         bson_t *child)   /* IN */
{
   BSON_ASSERT (bson);
   BSON_ASSERT (key);
   BSON_ASSERT (child);

   return _bson_append_bson_begin (bson, key, key_length, BSON_TYPE_ARRAY, child);
}


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_array_end --
 *
 *       Complete a call to bson_append_array_begin().
 *
 *       It is safe to append other fields to @bson after calling this
 *       function.
 *
 * Returns:
 *       true if successful.
 *
 * Side effects:
 *       @child is invalid after calling this function.
 *
 *--------------------------------------------------------------------------
 */

bool
bson_append_array_end (bson_t *bson,  /* IN */
                       bson_t *child) /* IN */
{
   BSON_ASSERT (bson);
   BSON_ASSERT (child);

   return _bson_append_bson_end (bson, child);
}


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_document_begin --
 *
 *       Start appending a new document.
 *
 *       Use @child to append to the data area for the given field.
 *
 *       It is a programming error to call any other bson function on
 *       @bson until bson_append_document_end() has been called. It is
 *       valid to call bson_append*() functions on @child.
 *
 *       This function is useful to allow building nested documents using
 *       a single buffer owned by the top-level bson document.
 *
 * Returns:
 *       true if successful; otherwise false and @child is invalid.
 *
 * Side effects:
 *       @child is initialized if true is returned.
 *
 *--------------------------------------------------------------------------
 */
bool
bson_append_document_begin (bson_t *bson,    /* IN */
                            const char *key, /* IN */
                            int key_length,  /* IN */
                            bson_t *child)   /* IN */
{
   BSON_ASSERT (bson);
   BSON_ASSERT (key);
   BSON_ASSERT (child);

   return _bson_append_bson_begin (bson, key, key_length, BSON_TYPE_DOCUMENT, child);
}


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_document_end --
 *
 *       Complete a call to bson_append_document_begin().
 *
 *       It is safe to append new fields to @bson after calling this
 *       function, if true is returned.
 *
 * Returns:
 *       true if successful; otherwise false indicating BSON_MAX_SIZE overflow.
 *
 * Side effects:
 *       @child is destroyed and invalid after calling this function.
 *
 *--------------------------------------------------------------------------
 */

bool
bson_append_document_end (bson_t *bson,  /* IN */
                          bson_t *child) /* IN */
{
   BSON_ASSERT (bson);
   BSON_ASSERT (child);

   return _bson_append_bson_end (bson, child);
}


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_array --
 *
 *       Append an array to @bson.
 *
 *       Generally, bson_append_array_begin() will result in faster code
 *       since few buffers need to be malloced.
 *
 * Returns:
 *       true if successful; otherwise false indicating BSON_MAX_SIZE overflow.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_array (bson_t *bson,        /* IN */
//                   const char *key,     /* IN */
//                   int key_length,      /* IN */
//                   const bson_t *array) /* IN */
//{
//   static const uint8_t type = BSON_TYPE_ARRAY;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (array);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   /*
//    * Let's be a bit pedantic and ensure the array has properly formatted key
//    * names.  We will verify this simply by checking the first element for "0"
//    * if the array is non-empty.
//    */
//   if (array && !bson_empty (array)) {
//      bson_iter_t iter;
//
//      if (bson_iter_init (&iter, array) && bson_iter_next (&iter)) {
//         if (0 != strcmp ("0", bson_iter_key (&iter))) {
//            fprintf (stderr,
//                     "%s(): invalid array detected. first element of array "
//                     "parameter is not \"0\".\n",
//                     BSON_FUNC);
//         }
//      }
//   }
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (array), array->len);
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_array (bson_t *bson,        /* IN */
                   const char *key,     /* IN */
                   int key_length,      /* IN */
                   const bson_t *array) /* IN */
{
   static const uint8_t type = BSON_TYPE_ARRAY;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (array);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   /*
    * Let's be a bit pedantic and ensure the array has properly formatted key
    * names.  We will verify this simply by checking the first element for "0"
    * if the array is non-empty.
    */
   if (array && !bson_empty (array)) {
      bson_iter_t iter;

      if (bson_iter_init (&iter, array) && bson_iter_next (&iter)) {
         if (0 != strcmp ("0", bson_iter_key (&iter))) {
            fprintf (stderr,
                     "%s(): invalid array detected. first element of array "
                     "parameter is not \"0\".\n",
                     BSON_FUNC);
         }
      }
   }

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (array), array->len);

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_array (bson_t *bson,        /* IN */
                   const char *key,     /* IN */
                   int key_length,      /* IN */
                   const bson_t *array) /* IN */
{
   static const uint8_t type = BSON_TYPE_ARRAY;
   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (array);

   //BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   /*
    * Let's be a bit pedantic and ensure the array has properly formatted key
    * names.  We will verify this simply by checking the first element for "0"
    * if the array is non-empty.
    */
   if (array && !bson_empty (array)) {
      bson_iter_t iter;

      if (bson_iter_init (&iter, array) && bson_iter_next (&iter)) {
         if (0 != strcmp ("0", bson_iter_key (&iter))) {
            fprintf (stderr,
                     "%s(): invalid array detected. first element of array "
                     "parameter is not \"0\".\n",
                     BSON_FUNC);
         }
      }
   }

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (array), array->len);

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_binary --
 *
 *       Append binary data to @bson. The field will have the
 *       BSON_TYPE_BINARY type.
 *
 * Parameters:
 *       @subtype: the BSON Binary Subtype. See bsonspec.org for more
 *                 information.
 *       @binary: a pointer to the raw binary data.
 *       @length: the size of @binary in bytes.
 *
 * Returns:
 *       true if successful; otherwise false.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_binary (bson_t *bson,           /* IN */
//                    const char *key,        /* IN */
//                    int key_length,         /* IN */
//                    bson_subtype_t subtype, /* IN */
//                    const uint8_t *binary,  /* IN */
//                    uint32_t length)        /* IN */
//{
//   static const uint8_t type = BSON_TYPE_BINARY;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   if (!binary && length > 0u) {
//      return false;
//   }
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   const uint8_t subtype_arg = (uint8_t) subtype;
//
//   if (subtype == BSON_SUBTYPE_BINARY_DEPRECATED) {
//      if (length > UINT32_MAX - 4u) {
//         return false;
//      }
//
//      const uint32_t length_le = BSON_UINT32_TO_LE (length + 4u);
//      const uint32_t length_arg = BSON_UINT32_TO_LE (length);
//
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_le, sizeof (length_le));
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &subtype_arg, sizeof (subtype_arg));
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, binary, length);
//
//      BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//   } else {
//      const uint32_t length_arg = BSON_UINT32_TO_LE (length);
//
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &subtype_arg, sizeof (subtype_arg));
//      BSON_APPEND_BYTES_ADD_ARGUMENT (args, binary, length);
//
//      BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//   }
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_binary (bson_t *bson,           /* IN */
                    const char *key,        /* IN */
                    int key_length,         /* IN */
                    bson_subtype_t subtype, /* IN */
                    const uint8_t *binary,  /* IN */
                    uint32_t length)        /* IN */
{
   static const uint8_t type = BSON_TYPE_BINARY;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   if (!binary && length > 0u) {
      return false;
   }

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   const uint8_t subtype_arg = (uint8_t) subtype;

   if (subtype == BSON_SUBTYPE_BINARY_DEPRECATED) {
      if (length > UINT32_MAX - 4u) {
         return false;
      }

      const uint32_t length_le = BSON_UINT32_TO_LE (length + 4u);
      const uint32_t length_arg = BSON_UINT32_TO_LE (length);

      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_le, sizeof (length_le));
      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &subtype_arg, sizeof (subtype_arg));
      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
      BSON_APPEND_BYTES_ADD_ARGUMENT (args, binary, length);

      BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
   } else {
      const uint32_t length_arg = BSON_UINT32_TO_LE (length);

      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
      BSON_APPEND_BYTES_ADD_ARGUMENT (args, &subtype_arg, sizeof (subtype_arg));
      BSON_APPEND_BYTES_ADD_ARGUMENT (args, binary, length);

      BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
   }

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_binary (bson_t *bson,           /* IN */
					const char *key,        /* IN */
					int key_length,         /* IN */
					bson_subtype_t subtype, /* IN */
					const uint8_t *binary,  /* IN */
					uint32_t length)        /* IN */
{
	static const uint8_t type = BSON_TYPE_BINARY;
	const uint8_t subtype_arg = (uint8_t) subtype;
	BSON_APPEND_BYTES_LIST_DECLARE (args);
   
	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	if (!binary && length > 0u) {
		return false;
	}

	//BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

	//const uint8_t subtype_arg = (uint8_t) subtype;

	if (subtype == BSON_SUBTYPE_BINARY_DEPRECATED) {
		uint32_t length_le = 0;
		uint32_t length_arg = 0;
		if (length > UINT32_MAX - 4u) {
			return false;
		}

		length_le = BSON_UINT32_TO_LE (length + 4u);
		length_arg = BSON_UINT32_TO_LE (length);

		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_le, sizeof (length_le));
		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &subtype_arg, sizeof (subtype_arg));
		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
		BSON_APPEND_BYTES_ADD_ARGUMENT (args, binary, length);

		BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
	} else {
		const uint32_t length_arg = BSON_UINT32_TO_LE (length);

		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
		BSON_APPEND_BYTES_ADD_ARGUMENT (args, &subtype_arg, sizeof (subtype_arg));
		BSON_APPEND_BYTES_ADD_ARGUMENT (args, binary, length);

		BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
	}

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

/*
 *--------------------------------------------------------------------------
 *
 * bson_append_bool --
 *
 *       Append a new field to @bson with the name @key. The value is
 *       a boolean indicated by @value.
 *
 * Returns:
 *       true if successful; otherwise false.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_bool (bson_t *bson,    /* IN */
//                  const char *key, /* IN */
//                  int key_length,  /* IN */
//                  bool value)      /* IN */
//{
//   static const uint8_t type = BSON_TYPE_BOOL;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   const uint8_t byte_arg = value ? 1u : 0u;
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &byte_arg, sizeof (byte_arg));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_bool (bson_t *bson,    /* IN */
                  const char *key, /* IN */
                  int key_length,  /* IN */
                  bool value)      /* IN */
{
   static const uint8_t type = BSON_TYPE_BOOL;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   const uint8_t byte_arg = value ? 1u : 0u;

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &byte_arg, sizeof (byte_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_bool (bson_t *bson,    /* IN */
                  const char *key, /* IN */
                  int key_length,  /* IN */
                  bool value)      /* IN */
{
   static const uint8_t type = BSON_TYPE_BOOL;
   const uint8_t byte_arg = value ? 1u : 0u;
   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   //const uint8_t byte_arg = value ? 1u : 0u;

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &byte_arg, sizeof (byte_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


/*
 *--------------------------------------------------------------------------
 *
 * bson_append_code --
 *
 *       Append a new field to @bson containing javascript code.
 *
 *       @javascript MUST be a zero terminated UTF-8 string. It MUST NOT
 *       containing embedded \0 characters.
 *
 * Returns:
 *       true if successful; otherwise false.
 *
 * Side effects:
 *       None.
 *
 * See also:
 *       bson_append_code_with_scope().
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_code (bson_t *bson,           /* IN */
//                  const char *key,        /* IN */
//                  int key_length,         /* IN */
//                  const char *javascript) /* IN */
//{
//   static const uint8_t type = BSON_TYPE_CODE;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (javascript);
//
//   const size_t zulength = strlen (javascript);
//   if (zulength > UINT32_MAX - 1u) {
//      return false;
//   }
//   const uint32_t length = (uint32_t) zulength + 1u;
//   const uint32_t length_arg = BSON_UINT32_TO_LE (length);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, javascript, length);
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_code (bson_t *bson,           /* IN */
                  const char *key,        /* IN */
                  int key_length,         /* IN */
                  const char *javascript) /* IN */
{
   static const uint8_t type = BSON_TYPE_CODE;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (javascript);

   const size_t zulength = strlen (javascript);
   if (zulength > UINT32_MAX - 1u) {
      return false;
   }
   const uint32_t length = (uint32_t) zulength + 1u;
   const uint32_t length_arg = BSON_UINT32_TO_LE (length);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, javascript, length);

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_code (bson_t *bson,           /* IN */
					const char *key,        /* IN */
					int key_length,         /* IN */
					const char *javascript) /* IN */
{
	static const uint8_t type = BSON_TYPE_CODE;
	size_t zulength = 0;
	uint32_t length = 0;
	uint32_t length_arg = 0;

	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (javascript);

	zulength = strlen (javascript);
	if (zulength > UINT32_MAX - 1u) {
		return false;
	}
	length = (uint32_t) zulength + 1u;
	length_arg = BSON_UINT32_TO_LE (length);

	//BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, javascript, length);

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

/*
 *--------------------------------------------------------------------------
 *
 * bson_append_code_with_scope --
 *
 *       Append a new field to @bson containing javascript code with
 *       supplied scope.
 *
 * Returns:
 *       true if successful; otherwise false.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_code_with_scope (bson_t *bson,           /* IN */
//                             const char *key,        /* IN */
//                             int key_length,         /* IN */
//                             const char *javascript, /* IN */
//                             const bson_t *scope)    /* IN */
//{
//   static const uint8_t type = BSON_TYPE_CODEWSCOPE;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (javascript);
//
//   if (!scope) {
//      return bson_append_code (bson, key, key_length, javascript);
//   }
//
//   const size_t zulength = strlen (javascript);
//   if (zulength > UINT32_MAX - 1u) {
//      return false;
//   }
//
//   const uint32_t js_length = (uint32_t) zulength + 1u;
//   const uint32_t js_length_arg = BSON_UINT32_TO_LE (js_length);
//
//   if (js_length > UINT32_MAX - scope->len) {
//      return false;
//   }
//
//   if (js_length + scope->len > UINT32_MAX - (2u * sizeof (uint32_t))) {
//      return false;
//   }
//
//   const uint32_t total_length = (uint32_t) (2u * sizeof (uint32_t)) + js_length + scope->len;
//   const uint32_t total_length_arg = BSON_UINT32_TO_LE (total_length);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &total_length_arg, sizeof (total_length_arg));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &js_length_arg, sizeof (js_length_arg));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, javascript, js_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (scope), scope->len);
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}


#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_code_with_scope (bson_t *bson,           /* IN */
                             const char *key,        /* IN */
                             int key_length,         /* IN */
                             const char *javascript, /* IN */
                             const bson_t *scope)    /* IN */
{
   static const uint8_t type = BSON_TYPE_CODEWSCOPE;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (javascript);

   if (!scope) {
      return bson_append_code (bson, key, key_length, javascript);
   }

   const size_t zulength = strlen (javascript);
   if (zulength > UINT32_MAX - 1u) {
      return false;
   }

   const uint32_t js_length = (uint32_t) zulength + 1u;
   const uint32_t js_length_arg = BSON_UINT32_TO_LE (js_length);

   if (js_length > UINT32_MAX - scope->len) {
      return false;
   }

   if (js_length + scope->len > UINT32_MAX - (2u * sizeof (uint32_t))) {
      return false;
   }

   const uint32_t total_length = (uint32_t) (2u * sizeof (uint32_t)) + js_length + scope->len;
   const uint32_t total_length_arg = BSON_UINT32_TO_LE (total_length);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &total_length_arg, sizeof (total_length_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &js_length_arg, sizeof (js_length_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, javascript, js_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (scope), scope->len);

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_code_with_scope (bson_t *bson,           /* IN */
								const char *key,        /* IN */
								int key_length,         /* IN */
								const char *javascript, /* IN */
								const bson_t *scope)    /* IN */
{
	static const uint8_t type = BSON_TYPE_CODEWSCOPE;
	size_t zulength = 0;
	uint32_t js_length = 0;
	uint32_t js_length_arg = 0;
	uint32_t total_length = 0;
	uint32_t total_length_arg = 0;

	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (javascript);

	if (!scope) {
		return bson_append_code (bson, key, key_length, javascript);
	}

	zulength = strlen (javascript);
	if (zulength > UINT32_MAX - 1u) {
		return false;
	}

	js_length = (uint32_t) zulength + 1u;
	js_length_arg = BSON_UINT32_TO_LE (js_length);

	if (js_length > UINT32_MAX - scope->len) {
		return false;
	}

	if (js_length + scope->len > UINT32_MAX - (2u * sizeof (uint32_t))) {
		return false;
	}

	total_length = (uint32_t) (2u * sizeof (uint32_t)) + js_length + scope->len;
	total_length_arg = BSON_UINT32_TO_LE (total_length);

	//BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &total_length_arg, sizeof (total_length_arg));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &js_length_arg, sizeof (js_length_arg));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, javascript, js_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (scope), scope->len);

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

/*
 *--------------------------------------------------------------------------
 *
 * bson_append_dbpointer --
 *
 *       This BSON data type is DEPRECATED.
 *
 *       Append a BSON dbpointer field to @bson.
 *
 * Returns:
 *       true if successful; otherwise false.
 *
 * Side effects:
 *       None.
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_dbpointer (bson_t *bson,           /* IN */
//                       const char *key,        /* IN */
//                       int key_length,         /* IN */
//                       const char *collection, /* IN */
//                       const bson_oid_t *oid)
//{
//   static const uint8_t type = BSON_TYPE_DBPOINTER;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (collection);
//   BSON_ASSERT_PARAM (oid);
//
//   const size_t zulength = strlen (collection);
//   if (zulength > UINT32_MAX - 1u) {
//      return false;
//   }
//
//   const uint32_t length = (uint32_t) zulength + 1u;
//   const uint32_t length_arg = BSON_UINT32_TO_LE (length);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, collection, length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, oid->bytes, sizeof (oid->bytes));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}


#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_dbpointer (bson_t *bson,           /* IN */
                       const char *key,        /* IN */
                       int key_length,         /* IN */
                       const char *collection, /* IN */
                       const bson_oid_t *oid)
{
   static const uint8_t type = BSON_TYPE_DBPOINTER;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (collection);
   BSON_ASSERT_PARAM (oid);

   const size_t zulength = strlen (collection);
   if (zulength > UINT32_MAX - 1u) {
      return false;
   }

   const uint32_t length = (uint32_t) zulength + 1u;
   const uint32_t length_arg = BSON_UINT32_TO_LE (length);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, collection, length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, oid->bytes, sizeof (oid->bytes));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_dbpointer (bson_t *bson,           /* IN */
						const char *key,        /* IN */
						int key_length,         /* IN */
						const char *collection, /* IN */
						const bson_oid_t *oid)
{
	static const uint8_t type = BSON_TYPE_DBPOINTER;
	size_t zulength = 0;
	uint32_t length = 0;
	uint32_t length_arg = 0;

	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (collection);
	BSON_ASSERT_PARAM (oid);

	zulength = strlen (collection);
	if (zulength > UINT32_MAX - 1u) {
		return false;
	}

	length = (uint32_t) zulength + 1u;
	length_arg = BSON_UINT32_TO_LE (length);

	//BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &length_arg, sizeof (length_arg));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, collection, length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, oid->bytes, sizeof (oid->bytes));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

/*
 *--------------------------------------------------------------------------
 *
 * bson_append_document --
 *
 *       Append a new field to @bson containing a BSON document.
 *
 *       In general, using bson_append_document_begin() results in faster
 *       code and less memory fragmentation.
 *
 * Returns:
 *       true if successful; otherwise false.
 *
 * Side effects:
 *       None.
 *
 * See also:
 *       bson_append_document_begin().
 *
 *--------------------------------------------------------------------------
 */

//bool
//bson_append_document (bson_t *bson,        /* IN */
//                      const char *key,     /* IN */
//                      int key_length,      /* IN */
//                      const bson_t *value) /* IN */
//{
//   static const uint8_t type = BSON_TYPE_DOCUMENT;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (value);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (value), value->len);
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_document (bson_t *bson,        /* IN */
                      const char *key,     /* IN */
                      int key_length,      /* IN */
                      const bson_t *value) /* IN */
{
   static const uint8_t type = BSON_TYPE_DOCUMENT;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (value), value->len);

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_document (bson_t *bson,        /* IN */
                      const char *key,     /* IN */
                      int key_length,      /* IN */
                      const bson_t *value) /* IN */
{
   static const uint8_t type = BSON_TYPE_DOCUMENT;
   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (value);

   //BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (value), value->len);

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


//bool
//bson_append_double (bson_t *bson, const char *key, int key_length, double value)
//{
//   static const uint8_t type = BSON_TYPE_DOUBLE;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   const double value_arg = BSON_DOUBLE_TO_LE (value);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_double (bson_t *bson, const char *key, int key_length, double value)
{
   static const uint8_t type = BSON_TYPE_DOUBLE;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   const double value_arg = BSON_DOUBLE_TO_LE (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_double (bson_t *bson, const char *key, int key_length, double value)
{
	static const uint8_t type = BSON_TYPE_DOUBLE;
	const double value_arg = BSON_DOUBLE_TO_LE (value);
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

   

  

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

//bool
//bson_append_int32 (bson_t *bson, const char *key, int key_length, int32_t value)
//{
//   static const uint8_t type = BSON_TYPE_INT32;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   const uint32_t value_arg = BSON_UINT32_TO_LE (value);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_int32 (bson_t *bson, const char *key, int key_length, int32_t value)
{
   static const uint8_t type = BSON_TYPE_INT32;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   const uint32_t value_arg = BSON_UINT32_TO_LE (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}


#else

bool
bson_append_int32 (bson_t *bson, const char *key, int key_length, int32_t value)
{
   static const uint8_t type = BSON_TYPE_INT32;
   const uint32_t value_arg = BSON_UINT32_TO_LE (value);
   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}


#endif // #if defined(YGGR_EX_C99_SUPPORTED)

//bool
//bson_append_int64 (bson_t *bson, const char *key, int key_length, int64_t value)
//{
//   static const uint8_t type = BSON_TYPE_INT64;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   const uint64_t value_arg = BSON_UINT64_TO_LE (value);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_int64 (bson_t *bson, const char *key, int key_length, int64_t value)
{
   static const uint8_t type = BSON_TYPE_INT64;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   const uint64_t value_arg = BSON_UINT64_TO_LE (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_int64 (bson_t *bson, const char *key, int key_length, int64_t value)
{
	static const uint8_t type = BSON_TYPE_INT64;
	const uint64_t value_arg = BSON_UINT64_TO_LE (value);
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

//bool
//bson_append_decimal128 (bson_t *bson, const char *key, int key_length, const bson_decimal128_t *value)
//{
//   static const uint8_t type = BSON_TYPE_DECIMAL128;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   const uint64_t value_arg[] = {
//      BSON_UINT64_TO_LE (value->low),
//      BSON_UINT64_TO_LE (value->high),
//   };
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_decimal128 (bson_t *bson, const char *key, int key_length, const bson_decimal128_t *value)
{
   static const uint8_t type = BSON_TYPE_DECIMAL128;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (value);

   const uint64_t value_arg[] = {
      BSON_UINT64_TO_LE (value->low),
      BSON_UINT64_TO_LE (value->high),
   };

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_decimal128 (bson_t *bson, const char *key, int key_length, const bson_decimal128_t *value)
{
	static const uint8_t type = BSON_TYPE_DECIMAL128;
	uint64_t value_arg[2] = {0};
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (value);

	value_arg[0] = BSON_UINT64_TO_LE (value->low);
	value_arg[1] = BSON_UINT64_TO_LE (value->high);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_iter (bson_t *bson, const char *key, int key_length, const bson_iter_t *iter)
{
   bool ret = false;

   BSON_ASSERT (bson);
   BSON_ASSERT (iter);

   if (!key) {
      key = bson_iter_key (iter);
      key_length = -1;
   }

   switch (bson_iter_type_unsafe (iter)) {
   case BSON_TYPE_EOD:
      return false;
   case BSON_TYPE_DOUBLE:
      ret = bson_append_double (bson, key, key_length, bson_iter_double (iter));
      break;
   case BSON_TYPE_UTF8: {
      uint32_t len = 0;
      const char *str;

      str = bson_iter_utf8 (iter, &len);
      ret = bson_append_utf8 (bson, key, key_length, str, len);
   } break;
   case BSON_TYPE_DOCUMENT: {
      const uint8_t *buf = NULL;
      uint32_t len = 0;
      bson_t doc;

      bson_iter_document (iter, &len, &buf);

      if (bson_init_static (&doc, buf, len)) {
         ret = bson_append_document (bson, key, key_length, &doc);
         bson_destroy (&doc);
      }
   } break;
   case BSON_TYPE_ARRAY: {
      const uint8_t *buf = NULL;
      uint32_t len = 0;
      bson_t doc;

      bson_iter_array (iter, &len, &buf);

      if (bson_init_static (&doc, buf, len)) {
         ret = bson_append_array (bson, key, key_length, &doc);
         bson_destroy (&doc);
      }
   } break;
   case BSON_TYPE_BINARY: {
      const uint8_t *binary = NULL;
      bson_subtype_t subtype = BSON_SUBTYPE_BINARY;
      uint32_t len = 0;

      bson_iter_binary (iter, &subtype, &len, &binary);
      ret = bson_append_binary (bson, key, key_length, subtype, binary, len);
   } break;
   case BSON_TYPE_UNDEFINED:
      ret = bson_append_undefined (bson, key, key_length);
      break;
   case BSON_TYPE_OID:
      ret = bson_append_oid (bson, key, key_length, bson_iter_oid (iter));
      break;
   case BSON_TYPE_BOOL:
      ret = bson_append_bool (bson, key, key_length, bson_iter_bool (iter));
      break;
   case BSON_TYPE_DATE_TIME:
      ret = bson_append_date_time (bson, key, key_length, bson_iter_date_time (iter));
      break;
   case BSON_TYPE_NULL:
      ret = bson_append_null (bson, key, key_length);
      break;
   case BSON_TYPE_REGEX: {
      const char *regex;
      const char *options;

      regex = bson_iter_regex (iter, &options);
      ret = bson_append_regex (bson, key, key_length, regex, options);
   } break;
   case BSON_TYPE_DBPOINTER: {
      const bson_oid_t *oid;
      uint32_t len;
      const char *collection;

      bson_iter_dbpointer (iter, &len, &collection, &oid);
      ret = bson_append_dbpointer (bson, key, key_length, collection, oid);
   } break;
   case BSON_TYPE_CODE: {
      uint32_t len;
      const char *code;

      code = bson_iter_code (iter, &len);
      ret = bson_append_code (bson, key, key_length, code);
   } break;
   case BSON_TYPE_SYMBOL: {
      uint32_t len;
      const char *symbol;

      symbol = bson_iter_symbol (iter, &len);
      ret = bson_append_symbol (bson, key, key_length, symbol, len);
   } break;
   case BSON_TYPE_CODEWSCOPE: {
      const uint8_t *scope = NULL;
      uint32_t scope_len = 0;
      uint32_t len = 0;
      const char *javascript = NULL;
      bson_t doc;

      javascript = bson_iter_codewscope (iter, &len, &scope_len, &scope);

      if (bson_init_static (&doc, scope, scope_len)) {
         ret = bson_append_code_with_scope (bson, key, key_length, javascript, &doc);
         bson_destroy (&doc);
      }
   } break;
   case BSON_TYPE_INT32:
      ret = bson_append_int32 (bson, key, key_length, bson_iter_int32 (iter));
      break;
   case BSON_TYPE_TIMESTAMP: {
      uint32_t ts;
      uint32_t inc;

      bson_iter_timestamp (iter, &ts, &inc);
      ret = bson_append_timestamp (bson, key, key_length, ts, inc);
   } break;
   case BSON_TYPE_INT64:
      ret = bson_append_int64 (bson, key, key_length, bson_iter_int64 (iter));
      break;
   case BSON_TYPE_DECIMAL128: {
      bson_decimal128_t dec;

      if (!bson_iter_decimal128 (iter, &dec)) {
         return false;
      }

      ret = bson_append_decimal128 (bson, key, key_length, &dec);
   } break;
   case BSON_TYPE_MAXKEY:
      ret = bson_append_maxkey (bson, key, key_length);
      break;
   case BSON_TYPE_MINKEY:
      ret = bson_append_minkey (bson, key, key_length);
      break;
   default:
      break;
   }

   return ret;
}


//bool
//bson_append_maxkey (bson_t *bson, const char *key, int key_length)
//{
//   static const uint8_t type = BSON_TYPE_MAXKEY;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_maxkey (bson_t *bson, const char *key, int key_length)
{
   static const uint8_t type = BSON_TYPE_MAXKEY;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_maxkey (bson_t *bson, const char *key, int key_length)
{
	static const uint8_t type = BSON_TYPE_MAXKEY;
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


//bool
//bson_append_minkey (bson_t *bson, const char *key, int key_length)
//{
//   static const uint8_t type = BSON_TYPE_MINKEY;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_minkey (bson_t *bson, const char *key, int key_length)
{
   static const uint8_t type = BSON_TYPE_MINKEY;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_minkey (bson_t *bson, const char *key, int key_length)
{
	static const uint8_t type = BSON_TYPE_MINKEY;
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

//bool
//bson_append_null (bson_t *bson, const char *key, int key_length)
//{
//   static const uint8_t type = BSON_TYPE_NULL;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_null (bson_t *bson, const char *key, int key_length)
{
   static const uint8_t type = BSON_TYPE_NULL;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_null (bson_t *bson, const char *key, int key_length)
{
   static const uint8_t type = BSON_TYPE_NULL;
   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

//bool
//bson_append_oid (bson_t *bson, const char *key, int key_length, const bson_oid_t *value)
//{
//   static const uint8_t type = BSON_TYPE_OID;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (value);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value->bytes, sizeof (value->bytes));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_oid (bson_t *bson, const char *key, int key_length, const bson_oid_t *value)
{
   static const uint8_t type = BSON_TYPE_OID;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value->bytes, sizeof (value->bytes));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_oid (bson_t *bson, const char *key, int key_length, const bson_oid_t *value)
{
	static const uint8_t type = BSON_TYPE_OID;
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (value);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, value->bytes, sizeof (value->bytes));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_regex (bson_t *bson, const char *key, int key_length, const char *regex, const char *options)
{
   return bson_append_regex_w_len (bson, key, key_length, regex, -1, options);
}


//bool
//bson_append_regex_w_len (
//   bson_t *bson, const char *key, int key_length, const char *regex, int regex_length, const char *options)
//{
//   static const uint8_t type = BSON_TYPE_REGEX;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   bool ret = false;
//
//   if (!regex) {
//      regex = "";
//   }
//
//   if (!options) {
//      options = "";
//   }
//
//   size_t options_len = strlen (options);
//   mcommon_string_append_t options_sorted;
//   mcommon_string_new_with_capacity_as_append (&options_sorted, (uint32_t) options_len);
//   if (!mcommon_string_append_selected_chars (&options_sorted, BSON_REGEX_OPTIONS_SORTED, options, options_len)) {
//      goto append_failure;
//   }
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, regex, regex_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (
//      args, mcommon_str_from_append (&options_sorted), 1u + mcommon_strlen_from_append (&options_sorted));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   ret = true;
//
//append_failure:
//   mcommon_string_from_append_destroy (&options_sorted);
//
//   return ret;
//}


#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_regex_w_len (
   bson_t *bson, const char *key, int key_length, const char *regex, int regex_length, const char *options)
{
   static const uint8_t type = BSON_TYPE_REGEX;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   bool ret = false;

   if (!regex) {
      regex = "";
   }

   if (!options) {
      options = "";
   }

   size_t options_len = strlen (options);
   mcommon_string_append_t options_sorted;
   mcommon_string_new_with_capacity_as_append (&options_sorted, (uint32_t) options_len);
   if (!mcommon_string_append_selected_chars (&options_sorted, BSON_REGEX_OPTIONS_SORTED, options, options_len)) {
      goto append_failure;
   }

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, regex, regex_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (
      args, mcommon_str_from_append (&options_sorted), 1u + mcommon_strlen_from_append (&options_sorted));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   ret = true;

append_failure:
   mcommon_string_from_append_destroy (&options_sorted);

   return ret;
}

#else

bool
bson_append_regex_w_len (
	bson_t *bson, const char *key, int key_length, const char *regex, int regex_length, const char *options)
{
	static const uint8_t type = BSON_TYPE_REGEX;
	bool ret = false;
	size_t options_len = 0;
	mcommon_string_append_t options_sorted;

	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	if (!regex) {
		regex = "";
	}

	if (!options) {
		options = "";
	}

	options_len = strlen (options);
	//mcommon_string_append_t options_sorted;
	mcommon_string_new_with_capacity_as_append (&options_sorted, (uint32_t) options_len);
	if (!mcommon_string_append_selected_chars (&options_sorted, BSON_REGEX_OPTIONS_SORTED, options, options_len)) {
		goto append_failure;
	}

	//BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, regex, regex_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (
		args, mcommon_str_from_append (&options_sorted), 1u + mcommon_strlen_from_append (&options_sorted));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	ret = true;

append_failure:
	mcommon_string_from_append_destroy (&options_sorted);

	return ret;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

//bool
//bson_append_utf8 (bson_t *bson, const char *key, int key_length, const char *value, int length)
//{
//   static const uint8_t type = BSON_TYPE_UTF8;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   if (BSON_UNLIKELY (!value)) {
//      return bson_append_null (bson, key, key_length);
//   }
//
//   size_t zulength;
//   if (BSON_UNLIKELY (length < 0)) {
//      zulength = strlen (value);
//   } else {
//      zulength = (size_t) length;
//   }
//
//   if (zulength > UINT32_MAX - 1u) {
//      return false;
//   }
//
//   const uint32_t ulength = (uint32_t) zulength;
//   const uint32_t ulength_arg = BSON_UINT32_TO_LE (ulength + 1u);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &ulength_arg, sizeof (ulength_arg));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value, ulength);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_utf8 (bson_t *bson, const char *key, int key_length, const char *value, int length)
{
   static const uint8_t type = BSON_TYPE_UTF8;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   if (BSON_UNLIKELY (!value)) {
      return bson_append_null (bson, key, key_length);
   }

   size_t zulength;
   if (BSON_UNLIKELY (length < 0)) {
      zulength = strlen (value);
   } else {
      zulength = (size_t) length;
   }

   if (zulength > UINT32_MAX - 1u) {
      return false;
   }

   const uint32_t ulength = (uint32_t) zulength;
   const uint32_t ulength_arg = BSON_UINT32_TO_LE (ulength + 1u);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &ulength_arg, sizeof (ulength_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value, ulength);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_utf8 (bson_t *bson, const char *key, int key_length, const char *value, int length)
{
	static const uint8_t type = BSON_TYPE_UTF8;
	size_t zulength;
	uint32_t ulength = 0;
	uint32_t ulength_arg = 0;

	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	if (BSON_UNLIKELY (!value)) {
		return bson_append_null (bson, key, key_length);
	}

	//size_t zulength;
	if (BSON_UNLIKELY (length < 0)) {
		zulength = strlen (value);
	} else {
		zulength = (size_t) length;
	}

	if (zulength > UINT32_MAX - 1u) {
		return false;
	}

	ulength = (uint32_t) zulength;
	ulength_arg = BSON_UINT32_TO_LE (ulength + 1u);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &ulength_arg, sizeof (ulength_arg));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, value, ulength);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


//bool
//bson_append_symbol (bson_t *bson, const char *key, int key_length, const char *value, int length)
//{
//   static const uint8_t type = BSON_TYPE_SYMBOL;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   if (!value) {
//      return bson_append_null (bson, key, key_length);
//   }
//
//   size_t zulength;
//   if (BSON_UNLIKELY (length < 0)) {
//      zulength = strlen (value);
//   } else {
//      zulength = (size_t) length;
//   }
//
//   if (zulength > UINT32_MAX - 1u) {
//      return false;
//   }
//
//   const uint32_t ulength = (uint32_t) zulength;
//   const uint32_t ulength_arg = BSON_UINT32_TO_LE (ulength + 1u);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &ulength_arg, sizeof (ulength_arg));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value, ulength);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_symbol (bson_t *bson, const char *key, int key_length, const char *value, int length)
{
   static const uint8_t type = BSON_TYPE_SYMBOL;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   if (!value) {
      return bson_append_null (bson, key, key_length);
   }

   size_t zulength;
   if (BSON_UNLIKELY (length < 0)) {
      zulength = strlen (value);
   } else {
      zulength = (size_t) length;
   }

   if (zulength > UINT32_MAX - 1u) {
      return false;
   }

   const uint32_t ulength = (uint32_t) zulength;
   const uint32_t ulength_arg = BSON_UINT32_TO_LE (ulength + 1u);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &ulength_arg, sizeof (ulength_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value, ulength);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_symbol (bson_t *bson, const char *key, int key_length, const char *value, int length)
{
   static const uint8_t type = BSON_TYPE_SYMBOL;
   size_t zulength;

   uint32_t ulength = 0;
   uint32_t ulength_arg = 0;

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   if (!value) {
      return bson_append_null (bson, key, key_length);
   }
   
   if (BSON_UNLIKELY (length < 0)) {
      zulength = strlen (value);
   } else {
      zulength = (size_t) length;
   }

   if (zulength > UINT32_MAX - 1u) {
      return false;
   }

   ulength = (uint32_t) zulength;
   ulength_arg = BSON_UINT32_TO_LE (ulength + 1u);

   //BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &ulength_arg, sizeof (ulength_arg));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, value, ulength);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_time_t (bson_t *bson, const char *key, int key_length, time_t value)
{
#ifdef BSON_OS_WIN32
   struct timeval tv = {(long) value, 0};
#else
   struct timeval tv = {value, 0};
#endif

   BSON_ASSERT (bson);
   BSON_ASSERT (key);

   return bson_append_timeval (bson, key, key_length, &tv);
}


//bool
//bson_append_timestamp (bson_t *bson, const char *key, int key_length, uint32_t timestamp, uint32_t increment)
//{
//   static const uint8_t type = BSON_TYPE_TIMESTAMP;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   const uint64_t value = BSON_UINT64_TO_LE (((((uint64_t) timestamp) << 32) | ((uint64_t) increment)));
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value, sizeof (value));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_timestamp (bson_t *bson, const char *key, int key_length, uint32_t timestamp, uint32_t increment)
{
   static const uint8_t type = BSON_TYPE_TIMESTAMP;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   const uint64_t value = BSON_UINT64_TO_LE (((((uint64_t) timestamp) << 32) | ((uint64_t) increment)));

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value, sizeof (value));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_timestamp (bson_t *bson, const char *key, int key_length, uint32_t timestamp, uint32_t increment)
{
	static const uint8_t type = BSON_TYPE_TIMESTAMP;
	const uint64_t value = BSON_UINT64_TO_LE (((((uint64_t) timestamp) << 32) | ((uint64_t) increment)));

	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value, sizeof (value));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


bool
bson_append_now_utc (bson_t *bson, const char *key, int key_length)
{
   BSON_ASSERT (bson);
   BSON_ASSERT (key);
   BSON_ASSERT (key_length >= -1);

   return bson_append_time_t (bson, key, key_length, time (NULL));
}


//bool
//bson_append_date_time (bson_t *bson, const char *key, int key_length, int64_t value)
//{
//   static const uint8_t type = BSON_TYPE_DATE_TIME;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   const uint64_t value_arg = BSON_UINT64_TO_LE (value);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_date_time (bson_t *bson, const char *key, int key_length, int64_t value)
{
   static const uint8_t type = BSON_TYPE_DATE_TIME;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   const uint64_t value_arg = BSON_UINT64_TO_LE (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_date_time (bson_t *bson, const char *key, int key_length, int64_t value)
{
   static const uint8_t type = BSON_TYPE_DATE_TIME;
   const uint64_t value_arg = BSON_UINT64_TO_LE (value);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &value_arg, sizeof (value_arg));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


bool
bson_append_timeval (bson_t *bson, const char *key, int key_length, struct timeval *value)
{
   uint64_t unix_msec;

   BSON_ASSERT (bson);
   BSON_ASSERT (key);
   BSON_ASSERT (value);

   unix_msec = (((uint64_t) value->tv_sec) * 1000UL) + (value->tv_usec / 1000UL);
   return bson_append_date_time (bson, key, key_length, unix_msec);
}


//bool
//bson_append_undefined (bson_t *bson, const char *key, int key_length)
//{
//   static const uint8_t type = BSON_TYPE_UNDEFINED;
//
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//
//   BSON_APPEND_BYTES_LIST_DECLARE (args);
//
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
//   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
//   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));
//
//   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);
//
//   return true;
//
//append_failure:
//   return false;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_undefined (bson_t *bson, const char *key, int key_length)
{
   static const uint8_t type = BSON_TYPE_UNDEFINED;

   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);

   BSON_APPEND_BYTES_LIST_DECLARE (args);

   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
   BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
   BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

   BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

   return true;

append_failure:
   return false;
}

#else

bool
bson_append_undefined (bson_t *bson, const char *key, int key_length)
{
	static const uint8_t type = BSON_TYPE_UNDEFINED;
	BSON_APPEND_BYTES_LIST_DECLARE (args);

	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);

	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &type, sizeof (type));
	BSON_APPEND_BYTES_ADD_CHECKED_STRING (args, key, key_length);
	BSON_APPEND_BYTES_ADD_ARGUMENT (args, &gZero, sizeof (gZero));

	BSON_APPEND_BYTES_APPLY_ARGUMENTS (bson, args);

	return true;

append_failure:
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_value (bson_t *bson, const char *key, int key_length, const bson_value_t *value)
{
   bson_t local;
   bool ret = false;

   BSON_ASSERT (bson);
   BSON_ASSERT (key);
   BSON_ASSERT (value);

   switch (value->value_type) {
   case BSON_TYPE_DOUBLE:
      ret = bson_append_double (bson, key, key_length, value->value.v_double);
      break;
   case BSON_TYPE_UTF8:
      ret = bson_append_utf8 (bson, key, key_length, value->value.v_utf8.str, value->value.v_utf8.len);
      break;
   case BSON_TYPE_DOCUMENT:
      if (bson_init_static (&local, value->value.v_doc.data, value->value.v_doc.data_len)) {
         ret = bson_append_document (bson, key, key_length, &local);
         bson_destroy (&local);
      }
      break;
   case BSON_TYPE_ARRAY:
      if (bson_init_static (&local, value->value.v_doc.data, value->value.v_doc.data_len)) {
         ret = bson_append_array (bson, key, key_length, &local);
         bson_destroy (&local);
      }
      break;
   case BSON_TYPE_BINARY:
      ret = bson_append_binary (bson,
                                key,
                                key_length,
                                value->value.v_binary.subtype,
                                value->value.v_binary.data,
                                value->value.v_binary.data_len);
      break;
   case BSON_TYPE_UNDEFINED:
      ret = bson_append_undefined (bson, key, key_length);
      break;
   case BSON_TYPE_OID:
      ret = bson_append_oid (bson, key, key_length, &value->value.v_oid);
      break;
   case BSON_TYPE_BOOL:
      ret = bson_append_bool (bson, key, key_length, value->value.v_bool);
      break;
   case BSON_TYPE_DATE_TIME:
      ret = bson_append_date_time (bson, key, key_length, value->value.v_datetime);
      break;
   case BSON_TYPE_NULL:
      ret = bson_append_null (bson, key, key_length);
      break;
   case BSON_TYPE_REGEX:
      ret = bson_append_regex (bson, key, key_length, value->value.v_regex.regex, value->value.v_regex.options);
      break;
   case BSON_TYPE_DBPOINTER:
      ret = bson_append_dbpointer (
         bson, key, key_length, value->value.v_dbpointer.collection, &value->value.v_dbpointer.oid);
      break;
   case BSON_TYPE_CODE:
      ret = bson_append_code (bson, key, key_length, value->value.v_code.code);
      break;
   case BSON_TYPE_SYMBOL:
      ret = bson_append_symbol (bson, key, key_length, value->value.v_symbol.symbol, value->value.v_symbol.len);
      break;
   case BSON_TYPE_CODEWSCOPE:
      if (bson_init_static (&local, value->value.v_codewscope.scope_data, value->value.v_codewscope.scope_len)) {
         ret = bson_append_code_with_scope (bson, key, key_length, value->value.v_codewscope.code, &local);
         bson_destroy (&local);
      }
      break;
   case BSON_TYPE_INT32:
      ret = bson_append_int32 (bson, key, key_length, value->value.v_int32);
      break;
   case BSON_TYPE_TIMESTAMP:
      ret = bson_append_timestamp (
         bson, key, key_length, value->value.v_timestamp.timestamp, value->value.v_timestamp.increment);
      break;
   case BSON_TYPE_INT64:
      ret = bson_append_int64 (bson, key, key_length, value->value.v_int64);
      break;
   case BSON_TYPE_DECIMAL128:
      ret = bson_append_decimal128 (bson, key, key_length, &(value->value.v_decimal128));
      break;
   case BSON_TYPE_MAXKEY:
      ret = bson_append_maxkey (bson, key, key_length);
      break;
   case BSON_TYPE_MINKEY:
      ret = bson_append_minkey (bson, key, key_length);
      break;
   case BSON_TYPE_EOD:
   default:
      break;
   }

   return ret;
}


void
bson_init (bson_t *bson)
{
   bson_impl_inline_t *impl = (bson_impl_inline_t *) bson;

   BSON_ASSERT (bson);

#ifdef BSON_MEMCHECK
   impl->canary = bson_malloc (1);
#endif
   impl->flags = BSON_FLAG_INLINE | BSON_FLAG_STATIC;
   impl->len = 5;
   impl->data[0] = 5;
   impl->data[1] = 0;
   impl->data[2] = 0;
   impl->data[3] = 0;
   impl->data[4] = 0;
}


void
bson_reinit (bson_t *bson)
{
   uint8_t *data;

   BSON_ASSERT (bson);

   data = _bson_data (bson);

   bson->len = 5;

   data[0] = 5;
   data[1] = 0;
   data[2] = 0;
   data[3] = 0;
   data[4] = 0;
}


bool
bson_init_static (bson_t *bson, const uint8_t *data, size_t length)
{
   bson_impl_alloc_t *impl = (bson_impl_alloc_t *) bson;
   uint32_t len_le;

   BSON_ASSERT (bson);
   BSON_ASSERT (data);

   if ((length < 5) || (length > BSON_MAX_SIZE)) {
      return false;
   }

   memcpy (&len_le, data, sizeof (len_le));

   if ((size_t) BSON_UINT32_FROM_LE (len_le) != length) {
      return false;
   }

   if (data[length - 1]) {
      return false;
   }

   impl->flags = BSON_FLAG_STATIC | BSON_FLAG_RDONLY;
   impl->len = (uint32_t) length;
   impl->parent = NULL;
   impl->depth = 0;
   impl->buf = &impl->alloc;
   impl->buflen = &impl->alloclen;
   impl->offset = 0;
   impl->alloc = (uint8_t *) data;
   impl->alloclen = length;
   impl->realloc = NULL;
   impl->realloc_func_ctx = NULL;

   return true;
}


bson_t *
bson_new (void)
{
   bson_impl_inline_t *impl;
   bson_t *bson;

   bson = BSON_ALIGNED_ALLOC (bson_t);

   impl = (bson_impl_inline_t *) bson;
   impl->flags = BSON_FLAG_INLINE;
   impl->len = 5;
#ifdef BSON_MEMCHECK
   impl->canary = bson_malloc (1);
#endif
   impl->data[0] = 5;
   impl->data[1] = 0;
   impl->data[2] = 0;
   impl->data[3] = 0;
   impl->data[4] = 0;

   return bson;
}


bson_t *
bson_sized_new (size_t size)
{
   bson_impl_alloc_t *impl_a;
   bson_t *b;

   BSON_ASSERT (size <= BSON_MAX_SIZE);

   {
      b = BSON_ALIGNED_ALLOC (bson_t);
      impl_a = (bson_impl_alloc_t *) b;
   }

   if (size <= BSON_INLINE_DATA_SIZE) {
      bson_init (b);
      b->flags &= ~BSON_FLAG_STATIC;
   } else {
      impl_a->flags = BSON_FLAG_NONE;
      impl_a->len = 5;
      impl_a->parent = NULL;
      impl_a->depth = 0;
      impl_a->buf = &impl_a->alloc;
      impl_a->buflen = &impl_a->alloclen;
      impl_a->offset = 0;
      impl_a->alloclen = BSON_MAX (5, size);
      impl_a->alloc = bson_malloc (impl_a->alloclen);
      impl_a->alloc[0] = 5;
      impl_a->alloc[1] = 0;
      impl_a->alloc[2] = 0;
      impl_a->alloc[3] = 0;
      impl_a->alloc[4] = 0;
      impl_a->realloc = bson_realloc_ctx;
      impl_a->realloc_func_ctx = NULL;
   }

   return b;
}


bson_t *
bson_new_from_data (const uint8_t *data, size_t length)
{
   uint32_t len_le;
   bson_t *bson;

   BSON_ASSERT (data);

   if ((length < 5) || (length > BSON_MAX_SIZE) || data[length - 1]) {
      return NULL;
   }

   memcpy (&len_le, data, sizeof (len_le));

   if (length != (size_t) BSON_UINT32_FROM_LE (len_le)) {
      return NULL;
   }

   bson = bson_sized_new (length);
   memcpy (_bson_data (bson), data, length);
   bson->len = (uint32_t) length;

   return bson;
}


bson_t *
bson_new_from_buffer (uint8_t **buf, size_t *buf_len, bson_realloc_func realloc_func, void *realloc_func_ctx)
{
   bson_impl_alloc_t *impl;
   uint32_t len_le;
   uint32_t length;
   bson_t *bson;

   BSON_ASSERT (buf);
   BSON_ASSERT (buf_len);

   if (!realloc_func) {
      realloc_func = bson_realloc_ctx;
   }

   bson = BSON_ALIGNED_ALLOC0 (bson_t);
   impl = (bson_impl_alloc_t *) bson;

   if (!*buf) {
      length = 5;
      len_le = BSON_UINT32_TO_LE (length);
      *buf_len = 5;
      *buf = realloc_func (*buf, *buf_len, realloc_func_ctx);
      memcpy (*buf, &len_le, sizeof (len_le));
      (*buf)[4] = '\0';
   } else {
      if ((*buf_len < 5) || (*buf_len > BSON_MAX_SIZE)) {
         bson_free (bson);
         return NULL;
      }

      memcpy (&len_le, *buf, sizeof (len_le));
      length = BSON_UINT32_FROM_LE (len_le);
      if ((size_t) length > *buf_len) {
         bson_free (bson);
         return NULL;
      }
   }

   if ((*buf)[length - 1]) {
      bson_free (bson);
      return NULL;
   }

   impl->flags = BSON_FLAG_NO_FREE;
   impl->len = length;
   impl->buf = buf;
   impl->buflen = buf_len;
   impl->realloc = realloc_func;
   impl->realloc_func_ctx = realloc_func_ctx;

   return bson;
}


bson_t *
bson_copy (const bson_t *bson)
{
   const uint8_t *data;

   BSON_ASSERT (bson);

   data = _bson_data (bson);
   return bson_new_from_data (data, bson->len);
}


void
bson_copy_to (const bson_t *src, bson_t *dst)
{
   const uint8_t *data;
   bson_impl_alloc_t *adst;
   size_t len;

   BSON_ASSERT (src);
   BSON_ASSERT (dst);

   if ((src->flags & BSON_FLAG_INLINE)) {
#ifdef BSON_MEMCHECK
      dst->len = src->len;
      dst->canary = bson_malloc (1);
      memcpy (dst->padding, src->padding, sizeof dst->padding);
#else
      memcpy (dst, src, sizeof *dst);
#endif
      dst->flags = (BSON_FLAG_STATIC | BSON_FLAG_INLINE);
      return;
   }

   data = _bson_data (src);
   len = _bson_round_up_alloc_size ((size_t) src->len);
   MONGOC_DEBUG_ASSERT (len <= BSON_MAX_SIZE);

   adst = (bson_impl_alloc_t *) dst;
   adst->flags = BSON_FLAG_STATIC;
   adst->len = src->len;
   adst->parent = NULL;
   adst->depth = 0;
   adst->buf = &adst->alloc;
   adst->buflen = &adst->alloclen;
   adst->offset = 0;
   adst->alloc = bson_malloc (len);
   adst->alloclen = len;
   adst->realloc = bson_realloc_ctx;
   adst->realloc_func_ctx = NULL;
   memcpy (adst->alloc, data, src->len);
}


static bool
should_ignore (const char *first_exclude, va_list args, const char *name)
{
   bool ret = false;
   const char *exclude = first_exclude;
   va_list args_copy;

   va_copy (args_copy, args);

   do {
      if (!strcmp (name, exclude)) {
         ret = true;
         break;
      }
   } while ((exclude = va_arg (args_copy, const char *)));

   va_end (args_copy);

   return ret;
}


void
bson_copy_to_excluding_noinit_va (const bson_t *src, bson_t *dst, const char *first_exclude, va_list args)
{
   bson_iter_t iter;

   if (bson_iter_init (&iter, src)) {
      while (bson_iter_next (&iter)) {
         if (!should_ignore (first_exclude, args, bson_iter_key (&iter))) {
            if (!bson_append_iter (dst, NULL, 0, &iter)) {
               /*
                * This should not be able to happen since we are copying
                * from within a valid bson_t.
                */
               BSON_ASSERT (false);
               return;
            }
         }
      }
   }
}


void
bson_copy_to_excluding (const bson_t *src, bson_t *dst, const char *first_exclude, ...)
{
   va_list args;

   BSON_ASSERT (src);
   BSON_ASSERT (dst);
   BSON_ASSERT (first_exclude);

   bson_init (dst);

   va_start (args, first_exclude);
   bson_copy_to_excluding_noinit_va (src, dst, first_exclude, args);
   va_end (args);
}

void
bson_copy_to_excluding_noinit (const bson_t *src, bson_t *dst, const char *first_exclude, ...)
{
   va_list args;

   BSON_ASSERT (src);
   BSON_ASSERT (dst);
   BSON_ASSERT (first_exclude);

   va_start (args, first_exclude);
   bson_copy_to_excluding_noinit_va (src, dst, first_exclude, args);
   va_end (args);
}

void
bson_destroy (bson_t *bson)
{
   if (!bson) {
      return;
   }

   if (!(bson->flags & (BSON_FLAG_RDONLY | BSON_FLAG_INLINE | BSON_FLAG_NO_FREE))) {
      bson_free (*((bson_impl_alloc_t *) bson)->buf);
   }

#ifdef BSON_MEMCHECK
   if (bson->flags & BSON_FLAG_INLINE) {
      bson_free (bson->canary);
   }
#endif

   if (!(bson->flags & BSON_FLAG_STATIC)) {
      bson_free (bson);
   }
}


uint8_t *
bson_reserve_buffer (bson_t *bson, uint32_t total_size)
{
   if (bson->flags & (BSON_FLAG_CHILD | BSON_FLAG_IN_CHILD | BSON_FLAG_RDONLY)) {
      return NULL;
   }

   if (total_size > bson->len) {
      if ((size_t) total_size > BSON_MAX_SIZE) {
         return NULL;
      }

      /* Note that the bson_t can also include space for parent or sibling documents (offset) and for trailing bytes
       * (depth). These sizes will be considered by _bson_grow() but we can assume they are zero in documents without
       * BSON_FLAG_CHILD or BSON_FLAG_IN_CHILD. If this is called on a document that's part of a bson_writer_t, it is
       * correct to ignore offset: we set the size of the current document, leaving previous documents alone. */
      if (!_bson_grow (bson, total_size - bson->len)) {
         // Will fail due to overflow or when reallocation is needed on a buffer that does not support it.
         return NULL;
      }
   }

   if (bson->flags & BSON_FLAG_INLINE) {
      /* bson_grow didn't spill over */
      ((bson_impl_inline_t *) bson)->len = total_size;
      BSON_ASSERT (total_size <= BSON_INLINE_DATA_SIZE);
   } else {
      bson_impl_alloc_t *impl = (bson_impl_alloc_t *) bson;
      impl->len = total_size;
      BSON_ASSERT (impl->offset <= *impl->buflen && *impl->buflen - impl->offset >= (size_t) total_size);
   }

   return _bson_data (bson);
}


bool
bson_steal (bson_t *dst, bson_t *src)
{
   bson_impl_inline_t *src_inline;
   bson_impl_inline_t *dst_inline;
   bson_impl_alloc_t *alloc;

   BSON_ASSERT (dst);
   BSON_ASSERT (src);

   bson_init (dst);

   if (src->flags & (BSON_FLAG_CHILD | BSON_FLAG_IN_CHILD | BSON_FLAG_RDONLY)) {
      return false;
   }

   if (src->flags & BSON_FLAG_INLINE) {
      src_inline = (bson_impl_inline_t *) src;
      dst_inline = (bson_impl_inline_t *) dst;
      dst_inline->len = src_inline->len;
      memcpy (dst_inline->data, src_inline->data, sizeof src_inline->data);

      /* for consistency, src is always invalid after steal, even if inline */
      src->len = 0;
#ifdef BSON_MEMCHECK
      bson_free (src->canary);
#endif
   } else {
#ifdef BSON_MEMCHECK
      bson_free (dst->canary);
#endif
      memcpy (dst, src, sizeof (bson_t));
      alloc = (bson_impl_alloc_t *) dst;
      alloc->flags |= BSON_FLAG_STATIC;
      alloc->buf = &alloc->alloc;
      alloc->buflen = &alloc->alloclen;
   }

   if (!(src->flags & BSON_FLAG_STATIC)) {
      bson_free (src);
   } else {
      /* src is invalid after steal */
      src->len = 0;
   }

   return true;
}


uint8_t *
bson_destroy_with_steal (bson_t *bson, bool steal, uint32_t *length)
{
   uint8_t *ret = NULL;

   BSON_ASSERT (bson);

   if (length) {
      *length = bson->len;
   }

   if (!steal) {
      bson_destroy (bson);
      return NULL;
   }

   if ((bson->flags & (BSON_FLAG_CHILD | BSON_FLAG_IN_CHILD | BSON_FLAG_RDONLY))) {
      /* Do nothing */
   } else if ((bson->flags & BSON_FLAG_INLINE)) {
      bson_impl_inline_t *inl;

      inl = (bson_impl_inline_t *) bson;
      ret = bson_malloc (bson->len);
      memcpy (ret, inl->data, bson->len);
   } else {
      bson_impl_alloc_t *alloc;

      alloc = (bson_impl_alloc_t *) bson;
      ret = *alloc->buf;
      *alloc->buf = NULL;
   }

   bson_destroy (bson);

   return ret;
}


const uint8_t *
bson_get_data (const bson_t *bson)
{
   BSON_ASSERT (bson);

   return _bson_data (bson);
}


uint32_t
bson_count_keys (const bson_t *bson)
{
   uint32_t count = 0;
   bson_iter_t iter;

   BSON_ASSERT (bson);

   if (bson_iter_init (&iter, bson)) {
      while (bson_iter_next (&iter)) {
         count++;
      }
   }

   return count;
}


bool
bson_has_field (const bson_t *bson, const char *key)
{
   bson_iter_t iter;
   bson_iter_t child;

   BSON_ASSERT (bson);
   BSON_ASSERT (key);

   if (NULL != strchr (key, '.')) {
      return (bson_iter_init (&iter, bson) && bson_iter_find_descendant (&iter, key, &child));
   }

   return bson_iter_init_find (&iter, bson, key);
}


int
bson_compare (const bson_t *bson, const bson_t *other)
{
   const uint8_t *data1;
   const uint8_t *data2;
   size_t len1;
   size_t len2;
   int64_t ret;

   data1 = _bson_data (bson) + 4;
   len1 = bson->len - 4;

   data2 = _bson_data (other) + 4;
   len2 = other->len - 4;

   if (len1 == len2) {
      return memcmp (data1, data2, len1);
   }

   ret = memcmp (data1, data2, BSON_MIN (len1, len2));

   if (ret == 0) {
      ret = (int64_t) len1 - (int64_t) len2;
   }

   return (ret < 0) ? -1 : (ret > 0);
}


bool
bson_equal (const bson_t *bson, const bson_t *other)
{
   return !bson_compare (bson, other);
}


//char *
//bson_as_json_with_opts (const bson_t *bson, size_t *length, const bson_json_opts_t *opts)
//{
//   BSON_ASSERT_PARAM (bson);
//   BSON_OPTIONAL_PARAM (length);
//   BSON_ASSERT_PARAM (opts);
//
//   // Convert the API-specified max length into a literal byte count; max length is transformed from a special value
//   // (-1) to the maximum representable size.
//   int32_t limit_i32 = opts->max_len;
//   uint32_t limit_u32 = 0;
//   if (limit_i32 == BSON_MAX_LEN_UNLIMITED) {
//      limit_u32 = UINT32_MAX - 1u;
//   } else if (limit_i32 > 0) {
//      limit_u32 = (uint32_t) limit_i32;
//   }
//
//   // Use the bson length as an initial buffer capacity guess
//   mcommon_string_append_t append;
//   mcommon_string_set_append_with_limit (mcommon_string_new_with_capacity ("", 0, bson->len), &append, limit_u32);
//
//   if (opts->is_outermost_array ? mcommon_json_append_bson_array (&append, bson, opts->mode, BSON_MAX_RECURSION)
//                                : mcommon_json_append_bson_document (&append, bson, opts->mode, BSON_MAX_RECURSION)) {
//      if (length) {
//         *length = (size_t) mcommon_strlen_from_append (&append);
//      }
//      return mcommon_string_from_append_destroy_with_steal (&append);
//   } else {
//      if (length) {
//         *length = 0;
//      }
//      mcommon_string_from_append_destroy (&append);
//      return NULL;
//   }
//}

#if defined(YGGR_EX_C99_SUPPORTED)

char *
bson_as_json_with_opts (const bson_t *bson, size_t *length, const bson_json_opts_t *opts)
{
   BSON_ASSERT_PARAM (bson);
   BSON_OPTIONAL_PARAM (length);
   BSON_ASSERT_PARAM (opts);

   // Convert the API-specified max length into a literal byte count; max length is transformed from a special value
   // (-1) to the maximum representable size.
   int32_t limit_i32 = opts->max_len;
   uint32_t limit_u32 = 0;
   if (limit_i32 == BSON_MAX_LEN_UNLIMITED) {
      limit_u32 = UINT32_MAX - 1u;
   } else if (limit_i32 > 0) {
      limit_u32 = (uint32_t) limit_i32;
   }

   // Use the bson length as an initial buffer capacity guess
   mcommon_string_append_t append;
   mcommon_string_set_append_with_limit (mcommon_string_new_with_capacity ("", 0, bson->len), &append, limit_u32);

   if (opts->is_outermost_array ? mcommon_json_append_bson_array (&append, bson, opts->mode, BSON_MAX_RECURSION)
                                : mcommon_json_append_bson_document (&append, bson, opts->mode, BSON_MAX_RECURSION)) {
      if (length) {
         *length = (size_t) mcommon_strlen_from_append (&append);
      }
      return mcommon_string_from_append_destroy_with_steal (&append);
   } else {
      if (length) {
         *length = 0;
      }
      mcommon_string_from_append_destroy (&append);
      return NULL;
   }
}

#else

char *
bson_as_json_with_opts (const bson_t *bson, size_t *length, const bson_json_opts_t *opts)
{
	int32_t limit_i32 = 0;
	uint32_t limit_u32 = 0;
	mcommon_string_append_t append;

	BSON_ASSERT_PARAM (bson);
	BSON_OPTIONAL_PARAM (length);
	BSON_ASSERT_PARAM (opts);

	// Convert the API-specified max length into a literal byte count; max length is transformed from a special value
	// (-1) to the maximum representable size.
	limit_i32 = opts->max_len;
	//uint32_t limit_u32 = 0;
	if (limit_i32 == BSON_MAX_LEN_UNLIMITED) {
		limit_u32 = UINT32_MAX - 1u;
	} else if (limit_i32 > 0) {
		limit_u32 = (uint32_t) limit_i32;
	}

	// Use the bson length as an initial buffer capacity guess
	//mcommon_string_append_t append;
	mcommon_string_set_append_with_limit (mcommon_string_new_with_capacity ("", 0, bson->len), &append, limit_u32);

	if (opts->is_outermost_array ? mcommon_json_append_bson_array (&append, bson, opts->mode, BSON_MAX_RECURSION)
								: mcommon_json_append_bson_document (&append, bson, opts->mode, BSON_MAX_RECURSION)) {
		if (length) {
			*length = (size_t) mcommon_strlen_from_append (&append);
		}
		return mcommon_string_from_append_destroy_with_steal (&append);
	} else {
		if (length) {
			*length = 0;
		}
		mcommon_string_from_append_destroy (&append);
		return NULL;
	}
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

char *
bson_as_canonical_extended_json (const bson_t *bson, size_t *length)
{
   const bson_json_opts_t opts = {BSON_JSON_MODE_CANONICAL, BSON_MAX_LEN_UNLIMITED, false};
   return bson_as_json_with_opts (bson, length, &opts);
}


char *
bson_as_json (const bson_t *bson, size_t *length)
{
   return bson_as_legacy_extended_json (bson, length);
}

char *
bson_as_legacy_extended_json (const bson_t *bson, size_t *length)
{
   const bson_json_opts_t opts = {BSON_JSON_MODE_LEGACY, BSON_MAX_LEN_UNLIMITED, false};
   return bson_as_json_with_opts (bson, length, &opts);
}


char *
bson_as_relaxed_extended_json (const bson_t *bson, size_t *length)
{
   const bson_json_opts_t opts = {BSON_JSON_MODE_RELAXED, BSON_MAX_LEN_UNLIMITED, false};
   return bson_as_json_with_opts (bson, length, &opts);
}


char *
bson_array_as_json (const bson_t *bson, size_t *length)
{
   return bson_array_as_legacy_extended_json (bson, length);
}

char *
bson_array_as_legacy_extended_json (const bson_t *bson, size_t *length)
{
   const bson_json_opts_t opts = {BSON_JSON_MODE_LEGACY, BSON_MAX_LEN_UNLIMITED, true};
   return bson_as_json_with_opts (bson, length, &opts);
}


char *
bson_array_as_relaxed_extended_json (const bson_t *bson, size_t *length)
{
   const bson_json_opts_t opts = {BSON_JSON_MODE_RELAXED, BSON_MAX_LEN_UNLIMITED, true};
   return bson_as_json_with_opts (bson, length, &opts);
}


char *
bson_array_as_canonical_extended_json (const bson_t *bson, size_t *length)
{
   const bson_json_opts_t opts = {BSON_JSON_MODE_CANONICAL, BSON_MAX_LEN_UNLIMITED, true};
   return bson_as_json_with_opts (bson, length, &opts);
}


#define VALIDATION_ERR(_flag, _msg, ...) bson_set_error (&state->error, BSON_ERROR_INVALID, _flag, _msg, __VA_ARGS__)

static bool
_bson_iter_validate_utf8 (const bson_iter_t *iter, const char *key, size_t v_utf8_len, const char *v_utf8, void *data)
{
   bson_validate_state_t *state = data;
   bool allow_null;

   if ((state->flags & BSON_VALIDATE_UTF8)) {
      allow_null = !!(state->flags & BSON_VALIDATE_UTF8_ALLOW_NULL);

      if (!bson_utf8_validate (v_utf8, v_utf8_len, allow_null)) {
         state->err_offset = iter->off;
         VALIDATION_ERR (BSON_VALIDATE_UTF8, "invalid utf8 string for key \"%s\"", key);
         return true;
      }
   }

   if ((state->flags & BSON_VALIDATE_DOLLAR_KEYS)) {
      if (state->phase == BSON_VALIDATE_PHASE_LF_REF_UTF8) {
         state->phase = BSON_VALIDATE_PHASE_LF_ID_KEY;
      } else if (state->phase == BSON_VALIDATE_PHASE_LF_DB_UTF8) {
         state->phase = BSON_VALIDATE_PHASE_NOT_DBREF;
      }
   }

   return false;
}


static void
_bson_iter_validate_corrupt (const bson_iter_t *iter, void *data)
{
   bson_validate_state_t *state = data;

   state->err_offset = iter->err_off;
   VALIDATION_ERR (BSON_VALIDATE_NONE, "%s", "corrupt BSON");
}


static bool
_bson_iter_validate_before (const bson_iter_t *iter, const char *key, void *data)
{
   bson_validate_state_t *state = data;

   if ((state->flags & BSON_VALIDATE_EMPTY_KEYS)) {
      if (key[0] == '\0') {
         state->err_offset = iter->off;
         VALIDATION_ERR (BSON_VALIDATE_EMPTY_KEYS, "%s", "empty key");
         return true;
      }
   }

   if ((state->flags & BSON_VALIDATE_DOLLAR_KEYS)) {
      if (key[0] == '$') {
         if (state->phase == BSON_VALIDATE_PHASE_LF_REF_KEY && strcmp (key, "$ref") == 0) {
            state->phase = BSON_VALIDATE_PHASE_LF_REF_UTF8;
         } else if (state->phase == BSON_VALIDATE_PHASE_LF_ID_KEY && strcmp (key, "$id") == 0) {
            state->phase = BSON_VALIDATE_PHASE_LF_DB_KEY;
         } else if (state->phase == BSON_VALIDATE_PHASE_LF_DB_KEY && strcmp (key, "$db") == 0) {
            state->phase = BSON_VALIDATE_PHASE_LF_DB_UTF8;
         } else {
            state->err_offset = iter->off;
            VALIDATION_ERR (BSON_VALIDATE_DOLLAR_KEYS, "keys cannot begin with \"$\": \"%s\"", key);
            return true;
         }
      } else if (state->phase == BSON_VALIDATE_PHASE_LF_ID_KEY || state->phase == BSON_VALIDATE_PHASE_LF_REF_UTF8 ||
                 state->phase == BSON_VALIDATE_PHASE_LF_DB_UTF8) {
         state->err_offset = iter->off;
         VALIDATION_ERR (BSON_VALIDATE_DOLLAR_KEYS, "invalid key within DBRef subdocument: \"%s\"", key);
         return true;
      } else {
         state->phase = BSON_VALIDATE_PHASE_NOT_DBREF;
      }
   }

   if ((state->flags & BSON_VALIDATE_DOT_KEYS)) {
      if (strstr (key, ".")) {
         state->err_offset = iter->off;
         VALIDATION_ERR (BSON_VALIDATE_DOT_KEYS, "keys cannot contain \".\": \"%s\"", key);
         return true;
      }
   }

   return false;
}


static bool
_bson_iter_validate_codewscope (
   const bson_iter_t *iter, const char *key, size_t v_code_len, const char *v_code, const bson_t *v_scope, void *data)
{
   bson_validate_state_t *state = data;
   size_t offset = 0;

   BSON_UNUSED (key);
   BSON_UNUSED (v_code_len);
   BSON_UNUSED (v_code);

   if (!bson_validate (v_scope, state->flags, &offset)) {
      state->err_offset = iter->off + offset;
      VALIDATION_ERR (BSON_VALIDATE_NONE, "%s", "corrupt code-with-scope");
      return false;
   }

   return true;
}


static bool
_bson_iter_validate_document (const bson_iter_t *iter, const char *key, const bson_t *v_document, void *data);


static const bson_visitor_t bson_validate_funcs = {
   _bson_iter_validate_before,
   NULL, /* visit_after */
   _bson_iter_validate_corrupt,
   NULL, /* visit_double */
   _bson_iter_validate_utf8,
   _bson_iter_validate_document,
   _bson_iter_validate_document, /* visit_array */
   NULL,                         /* visit_binary */
   NULL,                         /* visit_undefined */
   NULL,                         /* visit_oid */
   NULL,                         /* visit_bool */
   NULL,                         /* visit_date_time */
   NULL,                         /* visit_null */
   NULL,                         /* visit_regex */
   NULL,                         /* visit_dbpoint */
   NULL,                         /* visit_code */
   NULL,                         /* visit_symbol */
   _bson_iter_validate_codewscope,
};


static bool
_bson_iter_validate_document (const bson_iter_t *iter, const char *key, const bson_t *v_document, void *data)
{
   bson_validate_state_t *state = data;
   bson_iter_t child;
   bson_validate_phase_t phase = state->phase;

   BSON_UNUSED (key);

   if (!bson_iter_init (&child, v_document)) {
      state->err_offset = iter->off;
      return true;
   }

   if (state->phase == BSON_VALIDATE_PHASE_START) {
      state->phase = BSON_VALIDATE_PHASE_TOP;
   } else {
      state->phase = BSON_VALIDATE_PHASE_LF_REF_KEY;
   }

   (void) bson_iter_visit_all (&child, &bson_validate_funcs, state);

   if (state->phase == BSON_VALIDATE_PHASE_LF_ID_KEY || state->phase == BSON_VALIDATE_PHASE_LF_REF_UTF8 ||
       state->phase == BSON_VALIDATE_PHASE_LF_DB_UTF8) {
      if (state->err_offset <= 0) {
         state->err_offset = iter->off;
      }

      return true;
   }

   state->phase = phase;

   return false;
}


static void
_bson_validate_internal (const bson_t *bson, bson_validate_state_t *state)
{
   bson_iter_t iter;

   state->err_offset = -1;
   state->phase = BSON_VALIDATE_PHASE_START;
   memset (&state->error, 0, sizeof state->error);

   if (!bson_iter_init (&iter, bson)) {
      state->err_offset = 0;
      VALIDATION_ERR (BSON_VALIDATE_NONE, "%s", "corrupt BSON");
   } else {
      _bson_iter_validate_document (&iter, NULL, bson, state);
   }
}


bool
bson_validate (const bson_t *bson, bson_validate_flags_t flags, size_t *offset)
{
   return bson_validate_with_error_and_offset (bson, flags, offset, NULL);
}


bool
bson_validate_with_error (const bson_t *bson, bson_validate_flags_t flags, bson_error_t *error)
{
   return bson_validate_with_error_and_offset (bson, flags, NULL, error);
}


bool
bson_validate_with_error_and_offset (const bson_t *bson,
                                     bson_validate_flags_t flags,
                                     size_t *offset,
                                     bson_error_t *error)
{
   bson_validate_state_t state;

   state.flags = flags;
   _bson_validate_internal (bson, &state);

   if (state.err_offset >= 0) {
      if (offset) {
         *offset = (size_t) state.err_offset;
      }
      if (error) {
         memcpy (error, &state.error, sizeof *error);
      }
      return false;
   }

   return true;
}


bool
bson_concat (bson_t *dst, const bson_t *src)
{
   BSON_ASSERT (dst);
   BSON_ASSERT (src);

   if (!bson_empty (src)) {
      BSON_APPEND_BYTES_LIST_DECLARE (args);

      BSON_APPEND_BYTES_ADD_ARGUMENT (args, _bson_data (src) + 4, src->len - 5u);

      BSON_APPEND_BYTES_APPLY_ARGUMENTS (dst, args);
   }

   return true;

append_failure:
   return false;
}

struct _bson_array_builder_t {
   uint32_t index;
   bson_t bson;
};

bson_array_builder_t *
bson_array_builder_new (void)
{
   bson_array_builder_t *bab = BSON_ALIGNED_ALLOC0 (bson_array_builder_t);
   bson_init (&bab->bson);
   return bab;
}

// `bson_array_builder_append_impl` generates the next key index, calls
// `append_fn`, and may update the tracked next index.
//#define bson_array_builder_append_impl(append_fn, ...)                               \
//   if (1) {                                                                          \
//      BSON_ASSERT_PARAM (bab);                                                       \
//      const char *key;                                                               \
//      char buf[16];                                                                  \
//      size_t key_length = bson_uint32_to_string (bab->index, &key, buf, sizeof buf); \
//      /* Expect enough room in `buf` for key string. UINT32_MAX is 10 digits.        \
//       * With the NULL terminator, 11 is expected maximum number of                  \
//       * characters.  */                                                             \
//      BSON_ASSERT (key_length < sizeof buf);                                         \
//      bool ok = append_fn (&bab->bson, key, (int) key_length, __VA_ARGS__);          \
//      if (ok) {                                                                      \
//         bab->index += 1;                                                            \
//      }                                                                              \
//      return ok;                                                                     \
//   } else                                                                            \
//      (void) 0


#if defined(YGGR_EX_C99_SUPPORTED)

#define bson_array_builder_append_impl(append_fn, ...)                               \
   if (1) {                                                                          \
      BSON_ASSERT_PARAM (bab);                                                       \
      const char *key;                                                               \
      char buf[16];                                                                  \
      size_t key_length = bson_uint32_to_string (bab->index, &key, buf, sizeof buf); \
      /* Expect enough room in `buf` for key string. UINT32_MAX is 10 digits.        \
       * With the NULL terminator, 11 is expected maximum number of                  \
       * characters.  */                                                             \
      BSON_ASSERT (key_length < sizeof buf);                                         \
      bool ok = append_fn (&bab->bson, key, (int) key_length, __VA_ARGS__);          \
      if (ok) {                                                                      \
         bab->index += 1;                                                            \
      }                                                                              \
      return ok;                                                                     \
   } else                                                                            \
      (void) 0

#else

#define bson_array_builder_append_impl(append_fn, ...)									\
	if (1) {																			\
		const char *key;																\
		char buf[16];																	\
		size_t key_length = 0;															\
		bool ok = false;																\
																						\
		BSON_ASSERT_PARAM (bab);														\
																						\
		key_length = bson_uint32_to_string (bab->index, &key, buf, sizeof buf);			\
		/* Expect enough room in `buf` for key string. UINT32_MAX is 10 digits.			\
		* With the NULL terminator, 11 is expected maximum number of					\
		* characters.  */																\
		BSON_ASSERT (key_length < sizeof buf);											\
		ok = append_fn (&bab->bson, key, (int) key_length, __VA_ARGS__);				\
		if (ok) {																		\
			bab->index += 1;                                                            \
		}																				\
		return ok;																		\
	} else																				\
		(void) 0

#endif // #if defined(YGGR_EX_C99_SUPPORTED)


//#define bson_array_builder_append_impl_noargs(append_fn)                             \
//   if (1) {                                                                          \
//      BSON_ASSERT_PARAM (bab);                                                       \
//      const char *key;                                                               \
//      char buf[16];                                                                  \
//      size_t key_length = bson_uint32_to_string (bab->index, &key, buf, sizeof buf); \
//      /* Expect enough room in `buf` for key string. UINT32_MAX is 10 digits.        \
//       * With the NULL terminator, 11 is expected maximum number of                  \
//       * characters.  */                                                             \
//      BSON_ASSERT (key_length < sizeof buf);                                         \
//      bool ok = append_fn (&bab->bson, key, (int) key_length);                       \
//      if (ok) {                                                                      \
//         bab->index += 1;                                                            \
//      }                                                                              \
//      return ok;                                                                     \
//   } else                                                                            \
//      (void) 0


#if defined(YGGR_EX_C99_SUPPORTED)

#define bson_array_builder_append_impl_noargs(append_fn)                             \
   if (1) {                                                                          \
      BSON_ASSERT_PARAM (bab);                                                       \
      const char *key;                                                               \
      char buf[16];                                                                  \
      size_t key_length = bson_uint32_to_string (bab->index, &key, buf, sizeof buf); \
      /* Expect enough room in `buf` for key string. UINT32_MAX is 10 digits.        \
       * With the NULL terminator, 11 is expected maximum number of                  \
       * characters.  */                                                             \
      BSON_ASSERT (key_length < sizeof buf);                                         \
      bool ok = append_fn (&bab->bson, key, (int) key_length);                       \
      if (ok) {                                                                      \
         bab->index += 1;                                                            \
      }                                                                              \
      return ok;                                                                     \
   } else                                                                            \
      (void) 0


#else

#define bson_array_builder_append_impl_noargs(append_fn)								\
	if (1) {																			\
		const char *key;																\
		char buf[16];																	\
		size_t key_length = 0;															\
		bool ok = false;																\
																						\
		BSON_ASSERT_PARAM (bab);														\
																						\
		key_length = bson_uint32_to_string (bab->index, &key, buf, sizeof buf);			\
		/* Expect enough room in `buf` for key string. UINT32_MAX is 10 digits.			\
		* With the NULL terminator, 11 is expected maximum number of					\
		* characters.  */																\
		BSON_ASSERT (key_length < sizeof buf);											\
		ok = append_fn (&bab->bson, key, (int) key_length);								\
		if (ok) {																		\
			bab->index += 1;                                                            \
		}																				\
		return ok;																		\
	} else																				\
		(void) 0


#endif // #if defined(YGGR_EX_C99_SUPPORTED)


bool
bson_array_builder_append_value (bson_array_builder_t *bab, const bson_value_t *value)
{
   bson_array_builder_append_impl (bson_append_value, value);
}

bool
bson_array_builder_append_array (bson_array_builder_t *bab, const bson_t *array)
{
   bson_array_builder_append_impl (bson_append_array, array);
}


bool
bson_array_builder_append_binary (bson_array_builder_t *bab,
                                  bson_subtype_t subtype,
                                  const uint8_t *binary,
                                  uint32_t length)
{
   bson_array_builder_append_impl (bson_append_binary, subtype, binary, length);
}


bool
bson_array_builder_append_bool (bson_array_builder_t *bab, bool value)
{
   bson_array_builder_append_impl (bson_append_bool, value);
}


bool
bson_array_builder_append_code (bson_array_builder_t *bab, const char *javascript)
{
   bson_array_builder_append_impl (bson_append_code, javascript);
}


bool
bson_array_builder_append_code_with_scope (bson_array_builder_t *bab, const char *javascript, const bson_t *scope)
{
   bson_array_builder_append_impl (bson_append_code_with_scope, javascript, scope);
}


bool
bson_array_builder_append_dbpointer (bson_array_builder_t *bab, const char *collection, const bson_oid_t *oid)
{
   bson_array_builder_append_impl (bson_append_dbpointer, collection, oid);
}


bool
bson_array_builder_append_double (bson_array_builder_t *bab, double value)
{
   bson_array_builder_append_impl (bson_append_double, value);
}


bool
bson_array_builder_append_document (bson_array_builder_t *bab, const bson_t *value)
{
   bson_array_builder_append_impl (bson_append_document, value);
}


bool
bson_array_builder_append_document_begin (bson_array_builder_t *bab, bson_t *child)
{
   bson_array_builder_append_impl (bson_append_document_begin, child);
}


bool
bson_array_builder_append_document_end (bson_array_builder_t *bab, bson_t *child)
{
   return bson_append_document_end (&bab->bson, child);
}


bool
bson_array_builder_append_int32 (bson_array_builder_t *bab, int32_t value)
{
   bson_array_builder_append_impl (bson_append_int32, value);
}


bool
bson_array_builder_append_int64 (bson_array_builder_t *bab, int64_t value)
{
   bson_array_builder_append_impl (bson_append_int64, value);
}


bool
bson_array_builder_append_decimal128 (bson_array_builder_t *bab, const bson_decimal128_t *value)
{
   bson_array_builder_append_impl (bson_append_decimal128, value);
}


bool
bson_array_builder_append_iter (bson_array_builder_t *bab, const bson_iter_t *iter)
{
   bson_array_builder_append_impl (bson_append_iter, iter);
}


bool
bson_array_builder_append_minkey (bson_array_builder_t *bab)
{
   bson_array_builder_append_impl_noargs (bson_append_minkey);
}


bool
bson_array_builder_append_maxkey (bson_array_builder_t *bab)
{
   bson_array_builder_append_impl_noargs (bson_append_maxkey);
}


bool
bson_array_builder_append_null (bson_array_builder_t *bab)
{
   bson_array_builder_append_impl_noargs (bson_append_null);
}


bool
bson_array_builder_append_oid (bson_array_builder_t *bab, const bson_oid_t *oid)
{
   bson_array_builder_append_impl (bson_append_oid, oid);
}


bool
bson_array_builder_append_regex (bson_array_builder_t *bab, const char *regex, const char *options)
{
   bson_array_builder_append_impl (bson_append_regex, regex, options);
}


bool
bson_array_builder_append_regex_w_len (bson_array_builder_t *bab,
                                       const char *regex,
                                       int regex_length,
                                       const char *options)
{
   bson_array_builder_append_impl (bson_append_regex_w_len, regex, regex_length, options);
}


bool
bson_array_builder_append_utf8 (bson_array_builder_t *bab, const char *value, int length)
{
   bson_array_builder_append_impl (bson_append_utf8, value, length);
}


bool
bson_array_builder_append_symbol (bson_array_builder_t *bab, const char *value, int length)
{
   bson_array_builder_append_impl (bson_append_symbol, value, length);
}


bool
bson_array_builder_append_time_t (bson_array_builder_t *bab, time_t value)
{
   bson_array_builder_append_impl (bson_append_time_t, value);
}


bool
bson_array_builder_append_timeval (bson_array_builder_t *bab, struct timeval *value)
{
   bson_array_builder_append_impl (bson_append_timeval, value);
}


bool
bson_array_builder_append_date_time (bson_array_builder_t *bab, int64_t value)
{
   bson_array_builder_append_impl (bson_append_date_time, value);
}


bool
bson_array_builder_append_now_utc (bson_array_builder_t *bab)
{
   bson_array_builder_append_impl_noargs (bson_append_now_utc);
}


bool
bson_array_builder_append_timestamp (bson_array_builder_t *bab, uint32_t timestamp, uint32_t increment)
{
   bson_array_builder_append_impl (bson_append_timestamp, timestamp, increment);
}


bool
bson_array_builder_append_undefined (bson_array_builder_t *bab)
{
   bson_array_builder_append_impl_noargs (bson_append_undefined);
}


bool
bson_array_builder_append_array_builder_begin (bson_array_builder_t *bab, bson_array_builder_t **child)
{
   bson_array_builder_append_impl (bson_append_array_builder_begin, child);
}

bool
bson_array_builder_append_array_builder_end (bson_array_builder_t *bab, bson_array_builder_t *child)
{
   return bson_append_array_builder_end (&bab->bson, child);
}


bool
bson_array_builder_build (bson_array_builder_t *bab, bson_t *out)
{
   BSON_ASSERT_PARAM (bab);
   BSON_ASSERT_PARAM (out);
   if (!bson_steal (out, &bab->bson)) {
      return false;
   }
   bson_init (&bab->bson);
   bab->index = 0;
   return true;
}

void
bson_array_builder_destroy (bson_array_builder_t *bab)
{
   if (!bab) {
      return;
   }
   bson_destroy (&bab->bson);
   bson_free (bab);
}

//bool
//bson_append_array_builder_begin (bson_t *bson, const char *key, int key_length, bson_array_builder_t **child)
//{
//   BSON_ASSERT_PARAM (bson);
//   BSON_ASSERT_PARAM (key);
//   BSON_ASSERT_PARAM (child);
//   *child = bson_array_builder_new ();
//   bool ok = bson_append_array_begin (bson, key, key_length, &(*child)->bson);
//   if (!ok) {
//      bson_array_builder_destroy (*child);
//      *child = NULL;
//   }
//   return ok;
//}

#if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_array_builder_begin (bson_t *bson, const char *key, int key_length, bson_array_builder_t **child)
{
   BSON_ASSERT_PARAM (bson);
   BSON_ASSERT_PARAM (key);
   BSON_ASSERT_PARAM (child);
   *child = bson_array_builder_new ();
   bool ok = bson_append_array_begin (bson, key, key_length, &(*child)->bson);
   if (!ok) {
      bson_array_builder_destroy (*child);
      *child = NULL;
   }
   return ok;
}

#else

bool
bson_append_array_builder_begin (bson_t *bson, const char *key, int key_length, bson_array_builder_t **child)
{
	bool ok = false;
	BSON_ASSERT_PARAM (bson);
	BSON_ASSERT_PARAM (key);
	BSON_ASSERT_PARAM (child);
	*child = bson_array_builder_new ();
	ok = bson_append_array_begin (bson, key, key_length, &(*child)->bson);
	if (!ok) {
		bson_array_builder_destroy (*child);
		*child = NULL;
	}
	return ok;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

bool
bson_append_array_builder_end (bson_t *bson, bson_array_builder_t *child)
{
   bool ok = bson_append_array_end (bson, &child->bson);
   bson_array_builder_destroy (child);
   return ok;
}
