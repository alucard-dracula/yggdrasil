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

#include <common-prelude.h>


#ifndef MONGO_C_DRIVER_COMMON_ATOMIC_PRIVATE_H
#define MONGO_C_DRIVER_COMMON_ATOMIC_PRIVATE_H


#include <bson/bson.h> // BSON_INLINE

#ifdef _MSC_VER
#include <intrin.h>
#endif


enum mcommon_memory_order {
   mcommon_memory_order_seq_cst,
   mcommon_memory_order_acquire,
   mcommon_memory_order_release,
   mcommon_memory_order_relaxed,
   mcommon_memory_order_acq_rel,
   mcommon_memory_order_consume,
};

#if defined(_M_ARM) /* MSVC memorder atomics are only avail on ARM */
#define MSVC_MEMORDER_SUFFIX(X) X
#else
#define MSVC_MEMORDER_SUFFIX(X)
#endif

#if defined(USE_LEGACY_GCC_ATOMICS) || (!defined(__clang__) && __GNUC__ == 4) || defined(__xlC__)
#define MCOMMON_USE_LEGACY_GCC_ATOMICS
#else
#undef MCOMMON_USE_LEGACY_GCC_ATOMICS
#endif

/* Not all GCC-like compilers support the current __atomic built-ins.  Older
 * GCC (pre-5) used different built-ins named with the __sync prefix.  When
 * compiling with such older GCC versions, it is necessary to use the applicable
 * functions, which requires redefining BSON_IF_GNU_LIKE and defining the
 * additional MCOMMON_IF_GNU_LEGACY_ATOMICS macro here. */
#ifdef MCOMMON_USE_LEGACY_GCC_ATOMICS
#undef BSON_IF_GNU_LIKE
#define BSON_IF_GNU_LIKE(...)
#define MCOMMON_IF_GNU_LEGACY_ATOMICS(...) __VA_ARGS__
#else
#define MCOMMON_IF_GNU_LEGACY_ATOMICS(...)
#endif

/* CDRIVER-4229 zSeries with gcc 4.8.4 produces illegal instructions for int and
 * int32 atomic intrinsics. */
#if defined(__s390__) || defined(__s390x__) || defined(__zarch__)
#define MCOMMON_EMULATE_INT32
#define MCOMMON_EMULATE_INT
#endif

/* CDRIVER-4264 Contrary to documentation, VS 2013 targeting x86 does not
 * correctly/consistently provide _InterlockedPointerExchange. */
#if defined(_MSC_VER) && _MSC_VER < 1900 && defined(_M_IX86)
#define MCOMMON_EMULATE_PTR
#endif

#define DEF_ATOMIC_OP(MSVC_Intrinsic, GNU_Intrinsic, GNU_Legacy_Intrinsic, Order, ...)                  \
   do {                                                                                                 \
      switch (Order) {                                                                                  \
      case mcommon_memory_order_acq_rel:                                                                \
         BSON_IF_MSVC (return MSVC_Intrinsic (__VA_ARGS__);)                                            \
         BSON_IF_GNU_LIKE (return GNU_Intrinsic (__VA_ARGS__, __ATOMIC_ACQ_REL);)                       \
         MCOMMON_IF_GNU_LEGACY_ATOMICS (return GNU_Legacy_Intrinsic (__VA_ARGS__);)                     \
      case mcommon_memory_order_seq_cst:                                                                \
         BSON_IF_MSVC (return MSVC_Intrinsic (__VA_ARGS__);)                                            \
         BSON_IF_GNU_LIKE (return GNU_Intrinsic (__VA_ARGS__, __ATOMIC_SEQ_CST);)                       \
         MCOMMON_IF_GNU_LEGACY_ATOMICS (return GNU_Legacy_Intrinsic (__VA_ARGS__);)                     \
      case mcommon_memory_order_acquire:                                                                \
         BSON_IF_MSVC (return BSON_CONCAT (MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX (_acq)) (__VA_ARGS__);) \
         BSON_IF_GNU_LIKE (return GNU_Intrinsic (__VA_ARGS__, __ATOMIC_ACQUIRE);)                       \
         MCOMMON_IF_GNU_LEGACY_ATOMICS (return GNU_Legacy_Intrinsic (__VA_ARGS__);)                     \
      case mcommon_memory_order_consume:                                                                \
         BSON_IF_MSVC (return BSON_CONCAT (MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX (_acq)) (__VA_ARGS__);) \
         BSON_IF_GNU_LIKE (return GNU_Intrinsic (__VA_ARGS__, __ATOMIC_CONSUME);)                       \
         MCOMMON_IF_GNU_LEGACY_ATOMICS (return GNU_Legacy_Intrinsic (__VA_ARGS__);)                     \
      case mcommon_memory_order_release:                                                                \
         BSON_IF_MSVC (return BSON_CONCAT (MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX (_rel)) (__VA_ARGS__);) \
         BSON_IF_GNU_LIKE (return GNU_Intrinsic (__VA_ARGS__, __ATOMIC_RELEASE);)                       \
         MCOMMON_IF_GNU_LEGACY_ATOMICS (return GNU_Legacy_Intrinsic (__VA_ARGS__);)                     \
      case mcommon_memory_order_relaxed:                                                                \
         BSON_IF_MSVC (return BSON_CONCAT (MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX (_nf)) (__VA_ARGS__);)  \
         BSON_IF_GNU_LIKE (return GNU_Intrinsic (__VA_ARGS__, __ATOMIC_RELAXED);)                       \
         MCOMMON_IF_GNU_LEGACY_ATOMICS (return GNU_Legacy_Intrinsic (__VA_ARGS__);)                     \
      default:                                                                                          \
         BSON_UNREACHABLE ("Invalid mcommon_memory_order value");                                       \
      }                                                                                                 \
   } while (0)


#define DEF_ATOMIC_CMPEXCH_STRONG(VCSuffix1, VCSuffix2, GNU_MemOrder, Ptr, ExpectActualVar, NewValue)     \
   do {                                                                                                   \
      BSON_IF_MSVC (ExpectActualVar = BSON_CONCAT3 (_InterlockedCompareExchange, VCSuffix1, VCSuffix2) (  \
                       Ptr, NewValue, ExpectActualVar);)                                                  \
      BSON_IF_GNU_LIKE ((void) __atomic_compare_exchange_n (Ptr,                                          \
                                                            &ExpectActualVar,                             \
                                                            NewValue,                                     \
                                                            false, /* Not weak */                         \
                                                            GNU_MemOrder,                                 \
                                                            GNU_MemOrder);)                               \
      MCOMMON_IF_GNU_LEGACY_ATOMICS (__typeof__ (ExpectActualVar) _val;                                   \
                                     _val = __sync_val_compare_and_swap (Ptr, ExpectActualVar, NewValue); \
                                     ExpectActualVar = _val;)                                             \
   } while (0)


#define DEF_ATOMIC_CMPEXCH_WEAK(VCSuffix1, VCSuffix2, GNU_MemOrder, Ptr, ExpectActualVar, NewValue)       \
   do {                                                                                                   \
      BSON_IF_MSVC (ExpectActualVar = BSON_CONCAT3 (_InterlockedCompareExchange, VCSuffix1, VCSuffix2) (  \
                       Ptr, NewValue, ExpectActualVar);)                                                  \
      BSON_IF_GNU_LIKE ((void) __atomic_compare_exchange_n (Ptr,                                          \
                                                            &ExpectActualVar,                             \
                                                            NewValue,                                     \
                                                            true, /* Yes weak */                          \
                                                            GNU_MemOrder,                                 \
                                                            GNU_MemOrder);)                               \
      MCOMMON_IF_GNU_LEGACY_ATOMICS (__typeof__ (ExpectActualVar) _val;                                   \
                                     _val = __sync_val_compare_and_swap (Ptr, ExpectActualVar, NewValue); \
                                     ExpectActualVar = _val;)                                             \
   } while (0)


#if 0

#define DECL_ATOMIC_INTEGRAL(NamePart, Type, VCIntrinSuffix)                                                           \
   static BSON_INLINE Type mcommon_atomic_##NamePart##_fetch_add (                                                     \
      Type volatile *a, Type addend, enum mcommon_memory_order ord)                                                    \
   {                                                                                                                   \
      DEF_ATOMIC_OP (BSON_CONCAT (_InterlockedExchangeAdd, VCIntrinSuffix),                                            \
                     __atomic_fetch_add,                                                                               \
                     __sync_fetch_and_add,                                                                             \
                     ord,                                                                                              \
                     a,                                                                                                \
                     addend);                                                                                          \
   }                                                                                                                   \
                                                                                                                       \
   static BSON_INLINE Type mcommon_atomic_##NamePart##_fetch_sub (                                                     \
      Type volatile *a, Type subtrahend, enum mcommon_memory_order ord)                                                \
   {                                                                                                                   \
      /* MSVC doesn't have a subtract intrinsic, so just reuse addition    */                                          \
      BSON_IF_MSVC (return mcommon_atomic_##NamePart##_fetch_add (a, -subtrahend, ord);)                               \
      BSON_IF_GNU_LIKE (DEF_ATOMIC_OP (~, __atomic_fetch_sub, ~, ord, a, subtrahend);)                                 \
      MCOMMON_IF_GNU_LEGACY_ATOMICS (DEF_ATOMIC_OP (~, ~, __sync_fetch_and_sub, ord, a, subtrahend);)                  \
   }                                                                                                                   \
                                                                                                                       \
   static BSON_INLINE Type mcommon_atomic_##NamePart##_fetch (Type volatile const *a, enum mcommon_memory_order order) \
   {                                                                                                                   \
      /* MSVC doesn't have a load intrinsic, so just add zero */                                                       \
      BSON_IF_MSVC (return mcommon_atomic_##NamePart##_fetch_add ((Type volatile *) a, 0, order);)                     \
      /* GNU doesn't want RELEASE order for the fetch operation, so we can't                                           \
       * just use DEF_ATOMIC_OP. */                                                                                    \
      BSON_IF_GNU_LIKE (switch (order) {                                                                               \
         case mcommon_memory_order_release: /* Fall back to seqcst */                                                  \
         case mcommon_memory_order_acq_rel: /* Fall back to seqcst */                                                  \
         case mcommon_memory_order_seq_cst:                                                                            \
            return __atomic_load_n (a, __ATOMIC_SEQ_CST);                                                              \
         case mcommon_memory_order_acquire:                                                                            \
            return __atomic_load_n (a, __ATOMIC_ACQUIRE);                                                              \
         case mcommon_memory_order_consume:                                                                            \
            return __atomic_load_n (a, __ATOMIC_CONSUME);                                                              \
         case mcommon_memory_order_relaxed:                                                                            \
            return __atomic_load_n (a, __ATOMIC_RELAXED);                                                              \
         default:                                                                                                      \
            BSON_UNREACHABLE ("Invalid mcommon_memory_order value");                                                   \
      })                                                                                                               \
      MCOMMON_IF_GNU_LEGACY_ATOMICS ({                                                                                 \
         BSON_UNUSED (order);                                                                                          \
         __sync_synchronize ();                                                                                        \
         return *a;                                                                                                    \
      })                                                                                                               \
   }                                                                                                                   \
                                                                                                                       \
   static BSON_INLINE Type mcommon_atomic_##NamePart##_exchange (                                                      \
      Type volatile *a, Type value, enum mcommon_memory_order ord)                                                     \
   {                                                                                                                   \
      BSON_IF_MSVC (DEF_ATOMIC_OP (BSON_CONCAT (_InterlockedExchange, VCIntrinSuffix), ~, ~, ord, a, value);)          \
      /* GNU doesn't want CONSUME order for the exchange operation, so we                                              \
       * cannot use DEF_ATOMIC_OP. */                                                                                  \
      BSON_IF_GNU_LIKE (switch (ord) {                                                                                 \
         case mcommon_memory_order_acq_rel:                                                                            \
            return __atomic_exchange_n (a, value, __ATOMIC_ACQ_REL);                                                   \
         case mcommon_memory_order_release:                                                                            \
            return __atomic_exchange_n (a, value, __ATOMIC_RELEASE);                                                   \
         case mcommon_memory_order_seq_cst:                                                                            \
            return __atomic_exchange_n (a, value, __ATOMIC_SEQ_CST);                                                   \
         case mcommon_memory_order_consume: /* Fall back to acquire */                                                 \
         case mcommon_memory_order_acquire:                                                                            \
            return __atomic_exchange_n (a, value, __ATOMIC_ACQUIRE);                                                   \
         case mcommon_memory_order_relaxed:                                                                            \
            return __atomic_exchange_n (a, value, __ATOMIC_RELAXED);                                                   \
         default:                                                                                                      \
            BSON_UNREACHABLE ("Invalid mcommon_memory_order value");                                                   \
      })                                                                                                               \
      MCOMMON_IF_GNU_LEGACY_ATOMICS (BSON_UNUSED (ord); return __sync_val_compare_and_swap (a, *a, value);)            \
   }                                                                                                                   \
                                                                                                                       \
   static BSON_INLINE Type mcommon_atomic_##NamePart##_compare_exchange_strong (                                       \
      Type volatile *a, Type expect, Type new_value, enum mcommon_memory_order ord)                                    \
   {                                                                                                                   \
      Type actual = expect;                                                                                            \
      switch (ord) {                                                                                                   \
      case mcommon_memory_order_release:                                                                               \
      case mcommon_memory_order_acq_rel:                                                                               \
      case mcommon_memory_order_seq_cst:                                                                               \
         DEF_ATOMIC_CMPEXCH_STRONG (VCIntrinSuffix, , __ATOMIC_SEQ_CST, a, actual, new_value);                         \
         break;                                                                                                        \
      case mcommon_memory_order_acquire:                                                                               \
         DEF_ATOMIC_CMPEXCH_STRONG (                                                                                   \
            VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, a, actual, new_value);                      \
         break;                                                                                                        \
      case mcommon_memory_order_consume:                                                                               \
         DEF_ATOMIC_CMPEXCH_STRONG (                                                                                   \
            VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, a, actual, new_value);                      \
         break;                                                                                                        \
      case mcommon_memory_order_relaxed:                                                                               \
         DEF_ATOMIC_CMPEXCH_STRONG (                                                                                   \
            VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, a, actual, new_value);                       \
         break;                                                                                                        \
      default:                                                                                                         \
         BSON_UNREACHABLE ("Invalid mcommon_memory_order value");                                                      \
      }                                                                                                                \
      return actual;                                                                                                   \
   }                                                                                                                   \
                                                                                                                       \
   static BSON_INLINE Type mcommon_atomic_##NamePart##_compare_exchange_weak (                                         \
      Type volatile *a, Type expect, Type new_value, enum mcommon_memory_order ord)                                    \
   {                                                                                                                   \
      Type actual = expect;                                                                                            \
      switch (ord) {                                                                                                   \
      case mcommon_memory_order_release:                                                                               \
      case mcommon_memory_order_acq_rel:                                                                               \
      case mcommon_memory_order_seq_cst:                                                                               \
         DEF_ATOMIC_CMPEXCH_WEAK (VCIntrinSuffix, , __ATOMIC_SEQ_CST, a, actual, new_value);                           \
         break;                                                                                                        \
      case mcommon_memory_order_acquire:                                                                               \
         DEF_ATOMIC_CMPEXCH_WEAK (                                                                                     \
            VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, a, actual, new_value);                      \
         break;                                                                                                        \
      case mcommon_memory_order_consume:                                                                               \
         DEF_ATOMIC_CMPEXCH_WEAK (                                                                                     \
            VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, a, actual, new_value);                      \
         break;                                                                                                        \
      case mcommon_memory_order_relaxed:                                                                               \
         DEF_ATOMIC_CMPEXCH_WEAK (VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, a, actual, new_value); \
         break;                                                                                                        \
      default:                                                                                                         \
         BSON_UNREACHABLE ("Invalid mcommon_memory_order value");                                                      \
      }                                                                                                                \
      return actual;                                                                                                   \
   }

#else

#define DECL_ATOMIC_INTEGRAL(NamePart, Type, VCIntrinSuffix)																\
	static BSON_INLINE Type mcommon_atomic_##NamePart##_fetch_add (															\
		Type volatile *a, Type addend, enum mcommon_memory_order ord)														\
	{																														\
		DEF_ATOMIC_OP (BSON_CONCAT (_InterlockedExchangeAdd, VCIntrinSuffix),												\
						__atomic_fetch_add,																					\
						__sync_fetch_and_add,																				\
						ord,																								\
						a,																									\
						addend);																							\
		assert(0);																											\
		return (Type)0; /*failed*/																							\
	}																														\
																															\
	static BSON_INLINE Type mcommon_atomic_##NamePart##_fetch_sub (															\
		Type volatile *a, Type subtrahend, enum mcommon_memory_order ord)													\
	{																														\
		/* MSVC doesn't have a subtract intrinsic, so just reuse addition    */												\
		BSON_IF_MSVC (return mcommon_atomic_##NamePart##_fetch_add (a, -subtrahend, ord);)									\
		BSON_IF_GNU_LIKE (DEF_ATOMIC_OP (~, __atomic_fetch_sub, ~, ord, a, subtrahend);)									\
		MCOMMON_IF_GNU_LEGACY_ATOMICS (DEF_ATOMIC_OP (~, ~, __sync_fetch_and_sub, ord, a, subtrahend);)						\
	}																														\
																															\
	static BSON_INLINE Type mcommon_atomic_##NamePart##_fetch (Type volatile const *a, enum mcommon_memory_order order)		\
	{																														\
		/* MSVC doesn't have a load intrinsic, so just add zero */															\
		BSON_IF_MSVC (return mcommon_atomic_##NamePart##_fetch_add ((Type volatile *) a, 0, order);)						\
		/* GNU doesn't want RELEASE order for the fetch operation, so we can't												\
		* just use DEF_ATOMIC_OP. */																						\
		BSON_IF_GNU_LIKE (switch (order) {																					\
			case mcommon_memory_order_release: /* Fall back to seqcst */													\
			case mcommon_memory_order_acq_rel: /* Fall back to seqcst */													\
			case mcommon_memory_order_seq_cst:																				\
			return __atomic_load_n (a, __ATOMIC_SEQ_CST);																	\
			case mcommon_memory_order_acquire:																				\
			return __atomic_load_n (a, __ATOMIC_ACQUIRE);																	\
			case mcommon_memory_order_consume:																				\
			return __atomic_load_n (a, __ATOMIC_CONSUME);																	\
			case mcommon_memory_order_relaxed:																				\
			return __atomic_load_n (a, __ATOMIC_RELAXED);																	\
			default:																										\
			BSON_UNREACHABLE ("Invalid mcommon_memory_order value");														\
		})																													\
		MCOMMON_IF_GNU_LEGACY_ATOMICS ({																					\
			BSON_UNUSED (order);																							\
			__sync_synchronize ();																							\
			return *a;																										\
		})																													\
		assert(0);																											\
		return (Type)0; /*failed*/																							\
	}																														\
																															\
	static BSON_INLINE Type mcommon_atomic_##NamePart##_exchange (															\
		Type volatile *a, Type value, enum mcommon_memory_order ord)														\
	{																														\
		BSON_IF_MSVC (DEF_ATOMIC_OP (BSON_CONCAT (_InterlockedExchange, VCIntrinSuffix), ~, ~, ord, a, value);)				\
		/* GNU doesn't want CONSUME order for the exchange operation, so we													\
		* cannot use DEF_ATOMIC_OP. */																						\
		BSON_IF_GNU_LIKE (switch (ord) {																					\
			case mcommon_memory_order_acq_rel:																				\
			return __atomic_exchange_n (a, value, __ATOMIC_ACQ_REL);														\
			case mcommon_memory_order_release:																				\
			return __atomic_exchange_n (a, value, __ATOMIC_RELEASE);														\
			case mcommon_memory_order_seq_cst:																				\
			return __atomic_exchange_n (a, value, __ATOMIC_SEQ_CST);														\
			case mcommon_memory_order_consume: /* Fall back to acquire */													\
			case mcommon_memory_order_acquire:																				\
			return __atomic_exchange_n (a, value, __ATOMIC_ACQUIRE);														\
			case mcommon_memory_order_relaxed:																				\
			return __atomic_exchange_n (a, value, __ATOMIC_RELAXED);														\
			default:																										\
			BSON_UNREACHABLE ("Invalid mcommon_memory_order value");														\
		})																													\
		MCOMMON_IF_GNU_LEGACY_ATOMICS (BSON_UNUSED (ord); return __sync_val_compare_and_swap (a, *a, value);)				\
		assert(0);																											\
		return (Type)0; /*failed*/																							\
	}																														\
																															\
	static BSON_INLINE Type mcommon_atomic_##NamePart##_compare_exchange_strong (											\
		Type volatile *a, Type expect, Type new_value, enum mcommon_memory_order ord)										\
	{																														\
		Type actual = expect;																								\
		switch (ord) {																										\
		case mcommon_memory_order_release:																					\
		case mcommon_memory_order_acq_rel:																					\
		case mcommon_memory_order_seq_cst:																					\
			DEF_ATOMIC_CMPEXCH_STRONG (VCIntrinSuffix, , __ATOMIC_SEQ_CST, a, actual, new_value);							\
			break;																											\
		case mcommon_memory_order_acquire:																					\
			DEF_ATOMIC_CMPEXCH_STRONG (																						\
			VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, a, actual, new_value);							\
			break;																											\
		case mcommon_memory_order_consume:																					\
			DEF_ATOMIC_CMPEXCH_STRONG (																						\
			VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, a, actual, new_value);							\
			break;																											\
		case mcommon_memory_order_relaxed:																					\
			DEF_ATOMIC_CMPEXCH_STRONG (																						\
			VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, a, actual, new_value);							\
			break;																											\
		default:																											\
			BSON_UNREACHABLE ("Invalid mcommon_memory_order value");														\
		}																													\
		return actual;																										\
	}																														\
																															\
	static BSON_INLINE Type mcommon_atomic_##NamePart##_compare_exchange_weak (												\
		Type volatile *a, Type expect, Type new_value, enum mcommon_memory_order ord)										\
	{																														\
		Type actual = expect;																								\
		switch (ord) {																										\
		case mcommon_memory_order_release:																					\
		case mcommon_memory_order_acq_rel:																					\
		case mcommon_memory_order_seq_cst:																					\
			DEF_ATOMIC_CMPEXCH_WEAK (VCIntrinSuffix, , __ATOMIC_SEQ_CST, a, actual, new_value);								\
			break;																											\
		case mcommon_memory_order_acquire:																					\
			DEF_ATOMIC_CMPEXCH_WEAK (																						\
			VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, a, actual, new_value);							\
			break;																											\
		case mcommon_memory_order_consume:																					\
			DEF_ATOMIC_CMPEXCH_WEAK (																						\
			VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, a, actual, new_value);							\
			break;																											\
		case mcommon_memory_order_relaxed:																					\
			DEF_ATOMIC_CMPEXCH_WEAK (VCIntrinSuffix, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, a, actual, new_value);	\
			break;																											\
		default:																											\
			BSON_UNREACHABLE ("Invalid mcommon_memory_order value");														\
		}																													\
		return actual;																										\
	}

#endif // 0, 1

#define DECL_ATOMIC_STDINT(Name, VCSuffix) DECL_ATOMIC_INTEGRAL (Name, Name##_t, VCSuffix)

#if defined(_MSC_VER) || defined(MCOMMON_USE_LEGACY_GCC_ATOMICS)
/* MSVC and GCC require built-in types (not typedefs) for their atomic
 * intrinsics. */
#if defined(_MSC_VER)
#define DECL_ATOMIC_INTEGRAL_INT8 char
#define DECL_ATOMIC_INTEGRAL_INT32 long
#define DECL_ATOMIC_INTEGRAL_INT long
#else
#define DECL_ATOMIC_INTEGRAL_INT8 signed char
#define DECL_ATOMIC_INTEGRAL_INT32 int
#define DECL_ATOMIC_INTEGRAL_INT int
#endif
DECL_ATOMIC_INTEGRAL (int8, DECL_ATOMIC_INTEGRAL_INT8, 8)
DECL_ATOMIC_INTEGRAL (int16, short, 16)
#if !defined(MCOMMON_EMULATE_INT32)
DECL_ATOMIC_INTEGRAL (int32, DECL_ATOMIC_INTEGRAL_INT32, )
#endif
#if !defined(MCOMMON_EMULATE_INT)
DECL_ATOMIC_INTEGRAL (int, DECL_ATOMIC_INTEGRAL_INT, )
#endif
#else
/* Other compilers that we support provide generic intrinsics */
DECL_ATOMIC_STDINT (int8, 8)
DECL_ATOMIC_STDINT (int16, 16)
#if !defined(MCOMMON_EMULATE_INT32)
DECL_ATOMIC_STDINT (int32, )
#endif
#if !defined(MCOMMON_EMULATE_INT)
DECL_ATOMIC_INTEGRAL (int, int, )
#endif
#endif

#ifndef DECL_ATOMIC_INTEGRAL_INT32
#define DECL_ATOMIC_INTEGRAL_INT32 int32_t
#endif

#define _mcommon_emul_atomic_int64_fetch_add COMMON_NAME (emul_atomic_int64_fetch_add)
#define _mcommon_emul_atomic_int64_exchange COMMON_NAME (emul_atomic_int64_exchange)
#define _mcommon_emul_atomic_int64_compare_exchange_strong COMMON_NAME (emul_atomic_int64_compare_exchange_strong)
#define _mcommon_emul_atomic_int64_compare_exchange_weak COMMON_NAME (emul_atomic_int64_compare_exchange_weak)
#define _mcommon_emul_atomic_int32_fetch_add COMMON_NAME (emul_atomic_int32_fetch_add)
#define _mcommon_emul_atomic_int32_exchange COMMON_NAME (emul_atomic_int32_exchange)
#define _mcommon_emul_atomic_int32_compare_exchange_strong COMMON_NAME (emul_atomic_int32_compare_exchange_strong)
#define _mcommon_emul_atomic_int32_compare_exchange_weak COMMON_NAME (emul_atomic_int32_compare_exchange_weak)
#define _mcommon_emul_atomic_int_fetch_add COMMON_NAME (emul_atomic_int_fetch_add)
#define _mcommon_emul_atomic_int_exchange COMMON_NAME (emul_atomic_int_exchange)
#define _mcommon_emul_atomic_int_compare_exchange_strong COMMON_NAME (emul_atomic_int_compare_exchange_strong)
#define _mcommon_emul_atomic_int_compare_exchange_weak COMMON_NAME (emul_atomic_int_compare_exchange_weak)
#define _mcommon_emul_atomic_ptr_exchange COMMON_NAME (emul_atomic_ptr_exchange)
#define mcommon_thrd_yield COMMON_NAME (thrd_yield)

int64_t
_mcommon_emul_atomic_int64_fetch_add (int64_t volatile *val, int64_t v, enum mcommon_memory_order);
int64_t
_mcommon_emul_atomic_int64_exchange (int64_t volatile *val, int64_t v, enum mcommon_memory_order);
int64_t
_mcommon_emul_atomic_int64_compare_exchange_strong (int64_t volatile *val,
                                                    int64_t expect_value,
                                                    int64_t new_value,
                                                    enum mcommon_memory_order);

int64_t
_mcommon_emul_atomic_int64_compare_exchange_weak (int64_t volatile *val,
                                                  int64_t expect_value,
                                                  int64_t new_value,
                                                  enum mcommon_memory_order);

int32_t
_mcommon_emul_atomic_int32_fetch_add (int32_t volatile *val, int32_t v, enum mcommon_memory_order);
int32_t
_mcommon_emul_atomic_int32_exchange (int32_t volatile *val, int32_t v, enum mcommon_memory_order);
int32_t
_mcommon_emul_atomic_int32_compare_exchange_strong (int32_t volatile *val,
                                                    int32_t expect_value,
                                                    int32_t new_value,
                                                    enum mcommon_memory_order);

int32_t
_mcommon_emul_atomic_int32_compare_exchange_weak (int32_t volatile *val,
                                                  int32_t expect_value,
                                                  int32_t new_value,
                                                  enum mcommon_memory_order);

int
_mcommon_emul_atomic_int_fetch_add (int volatile *val, int v, enum mcommon_memory_order);
int
_mcommon_emul_atomic_int_exchange (int volatile *val, int v, enum mcommon_memory_order);
int
_mcommon_emul_atomic_int_compare_exchange_strong (int volatile *val,
                                                  int expect_value,
                                                  int new_value,
                                                  enum mcommon_memory_order);

int
_mcommon_emul_atomic_int_compare_exchange_weak (int volatile *val,
                                                int expect_value,
                                                int new_value,
                                                enum mcommon_memory_order);

void *
_mcommon_emul_atomic_ptr_exchange (void *volatile *val, void *v, enum mcommon_memory_order);

void
mcommon_thrd_yield (void);

#if (defined(_MSC_VER) && !defined(_M_IX86)) || (defined(__LP64__) && __LP64__)
/* (64-bit intrinsics are only available in x64) */
#ifdef _MSC_VER
DECL_ATOMIC_INTEGRAL (int64, __int64, 64)
#else
DECL_ATOMIC_STDINT (int64, 64)
#endif
#else
static BSON_INLINE int64_t
mcommon_atomic_int64_fetch (const int64_t volatile *val, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int64_fetch_add ((int64_t volatile *) val, 0, order);
}

static BSON_INLINE int64_t
mcommon_atomic_int64_fetch_add (int64_t volatile *val, int64_t v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int64_fetch_add (val, v, order);
}

static BSON_INLINE int64_t
mcommon_atomic_int64_fetch_sub (int64_t volatile *val, int64_t v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int64_fetch_add (val, -v, order);
}

static BSON_INLINE int64_t
mcommon_atomic_int64_exchange (int64_t volatile *val, int64_t v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int64_exchange (val, v, order);
}

static BSON_INLINE int64_t
mcommon_atomic_int64_compare_exchange_strong (int64_t volatile *val,
                                              int64_t expect_value,
                                              int64_t new_value,
                                              enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int64_compare_exchange_strong (val, expect_value, new_value, order);
}

static BSON_INLINE int64_t
mcommon_atomic_int64_compare_exchange_weak (int64_t volatile *val,
                                            int64_t expect_value,
                                            int64_t new_value,
                                            enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int64_compare_exchange_weak (val, expect_value, new_value, order);
}
#endif

#if defined(MCOMMON_EMULATE_INT32)
static BSON_INLINE int32_t
mcommon_atomic_int32_fetch (const int32_t volatile *val, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int32_fetch_add ((int32_t volatile *) val, 0, order);
}

static BSON_INLINE int32_t
mcommon_atomic_int32_fetch_add (int32_t volatile *val, int32_t v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int32_fetch_add (val, v, order);
}

static BSON_INLINE int32_t
mcommon_atomic_int32_fetch_sub (int32_t volatile *val, int32_t v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int32_fetch_add (val, -v, order);
}

static BSON_INLINE int32_t
mcommon_atomic_int32_exchange (int32_t volatile *val, int32_t v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int32_exchange (val, v, order);
}

static BSON_INLINE int32_t
mcommon_atomic_int32_compare_exchange_strong (int32_t volatile *val,
                                              int32_t expect_value,
                                              int32_t new_value,
                                              enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int32_compare_exchange_strong (val, expect_value, new_value, order);
}

static BSON_INLINE int32_t
mcommon_atomic_int32_compare_exchange_weak (int32_t volatile *val,
                                            int32_t expect_value,
                                            int32_t new_value,
                                            enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int32_compare_exchange_weak (val, expect_value, new_value, order);
}
#endif /* MCOMMON_EMULATE_INT32 */

#if defined(MCOMMON_EMULATE_INT)
static BSON_INLINE int
mcommon_atomic_int_fetch (const int volatile *val, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int_fetch_add ((int volatile *) val, 0, order);
}

static BSON_INLINE int
mcommon_atomic_int_fetch_add (int volatile *val, int v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int_fetch_add (val, v, order);
}

static BSON_INLINE int
mcommon_atomic_int_fetch_sub (int volatile *val, int v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int_fetch_add (val, -v, order);
}

static BSON_INLINE int
mcommon_atomic_int_exchange (int volatile *val, int v, enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int_exchange (val, v, order);
}

static BSON_INLINE int
mcommon_atomic_int_compare_exchange_strong (int volatile *val,
                                            int expect_value,
                                            int new_value,
                                            enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int_compare_exchange_strong (val, expect_value, new_value, order);
}

static BSON_INLINE int
mcommon_atomic_int_compare_exchange_weak (int volatile *val,
                                          int expect_value,
                                          int new_value,
                                          enum mcommon_memory_order order)
{
   return _mcommon_emul_atomic_int_compare_exchange_weak (val, expect_value, new_value, order);
}
#endif /* MCOMMON_EMULATE_INT */

static BSON_INLINE void *
mcommon_atomic_ptr_exchange (void *volatile *ptr, void *new_value, enum mcommon_memory_order ord)
{
#if defined(MCOMMON_EMULATE_PTR)
   return _mcommon_emul_atomic_ptr_exchange (ptr, new_value, ord);
#elif defined(MCOMMON_USE_LEGACY_GCC_ATOMICS)
   /* The older __sync_val_compare_and_swap also takes oldval */
   DEF_ATOMIC_OP (_InterlockedExchangePointer, , __sync_val_compare_and_swap, ord, ptr, *ptr, new_value);
#else
   DEF_ATOMIC_OP (_InterlockedExchangePointer, __atomic_exchange_n, , ord, ptr, new_value);
#endif
}

//static BSON_INLINE void *
//mcommon_atomic_ptr_compare_exchange_strong (void *volatile *ptr,
//                                            void *expect,
//                                            void *new_value,
//                                            enum mcommon_memory_order ord)
//{
//   switch (ord) {
//   case mcommon_memory_order_release:
//   case mcommon_memory_order_acq_rel:
//   case mcommon_memory_order_seq_cst:
//      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value);
//      return expect;
//   case mcommon_memory_order_relaxed:
//      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, ptr, expect, new_value);
//      return expect;
//   case mcommon_memory_order_consume:
//      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, ptr, expect, new_value);
//      return expect;
//   case mcommon_memory_order_acquire:
//      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, ptr, expect, new_value);
//      return expect;
//   default:
//      BSON_UNREACHABLE ("Invalid mcommon_memory_order value");
//   }
//}

#if !(defined(_MSC_VER) && (_MSC_VER < 1700) && !defined(WIN64))

static BSON_INLINE void *
mcommon_atomic_ptr_compare_exchange_strong (void *volatile *ptr,
                                            void *expect,
                                            void *new_value,
                                            enum mcommon_memory_order ord)
{
   switch (ord) {
   case mcommon_memory_order_release:
   case mcommon_memory_order_acq_rel:
   case mcommon_memory_order_seq_cst:
      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value);
      return expect;
   case mcommon_memory_order_relaxed:
      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, ptr, expect, new_value);
      return expect;
   case mcommon_memory_order_consume:
      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, ptr, expect, new_value);
      return expect;
   case mcommon_memory_order_acquire:
      DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, ptr, expect, new_value);
      return expect;
   default:
      BSON_UNREACHABLE ("Invalid mcommon_memory_order value");
	  return NULL;
   }
}

#else

static BSON_INLINE void *
mcommon_atomic_ptr_compare_exchange_strong (void *volatile *ptr,
											void *expect,
											void *new_value,
											enum mcommon_memory_order ord)
{
	switch (ord) {
	case mcommon_memory_order_release:
	case mcommon_memory_order_acq_rel:
	case mcommon_memory_order_seq_cst:
		//DEF_ATOMIC_CMPEXCH_STRONG (Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value);
		//return expect;
	case mcommon_memory_order_relaxed:
		//DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, ptr, expect, new_value);
		//return expect;
	case mcommon_memory_order_consume:
		//DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, ptr, expect, new_value);
		//return expect;
	case mcommon_memory_order_acquire:
		//DEF_ATOMIC_CMPEXCH_STRONG (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, ptr, expect, new_value);
		//return expect;
		do 
		{ 
			expect = (void*)_InterlockedCompareExchange( (long volatile *)ptr, (long)new_value, (long)expect); 
		} while (0);
		return expect;
	default:
		BSON_UNREACHABLE ("Invalid mcommon_memory_order value");
		return NULL;
	}
}

#endif // #if !(defined(_MSC_VER) && (_MSC_VER < 1700) && !defined(WIN64))


//static BSON_INLINE void *
//mcommon_atomic_ptr_compare_exchange_weak (void *volatile *ptr,
//                                          void *expect,
//                                          void *new_value,
//                                          enum mcommon_memory_order ord)
//{
//   switch (ord) {
//   case mcommon_memory_order_release:
//   case mcommon_memory_order_acq_rel:
//   case mcommon_memory_order_seq_cst:
//      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value);
//      return expect;
//   case mcommon_memory_order_relaxed:
//      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, ptr, expect, new_value);
//      return expect;
//   case mcommon_memory_order_consume:
//      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, ptr, expect, new_value);
//      return expect;
//   case mcommon_memory_order_acquire:
//      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, ptr, expect, new_value);
//      return expect;
//   default:
//      BSON_UNREACHABLE ("Invalid mcommon_memory_order value");
//   }
//}

#if !(defined(_MSC_VER) && (_MSC_VER < 1700) && !defined(WIN64))

static BSON_INLINE void *
mcommon_atomic_ptr_compare_exchange_weak (void *volatile *ptr,
                                          void *expect,
                                          void *new_value,
                                          enum mcommon_memory_order ord)
{
   switch (ord) {
   case mcommon_memory_order_release:
   case mcommon_memory_order_acq_rel:
   case mcommon_memory_order_seq_cst:
      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value);
      return expect;
   case mcommon_memory_order_relaxed:
      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, ptr, expect, new_value);
      return expect;
   case mcommon_memory_order_consume:
      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, ptr, expect, new_value);
      return expect;
   case mcommon_memory_order_acquire:
      DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, ptr, expect, new_value);
      return expect;
   default:
      BSON_UNREACHABLE ("Invalid mcommon_memory_order value");
	  return NULL;
   }
}

#else

static BSON_INLINE void *
mcommon_atomic_ptr_compare_exchange_weak (void *volatile *ptr,
											void *expect,
											void *new_value,
											enum mcommon_memory_order ord)
{
	switch (ord) {
	case mcommon_memory_order_release:
	case mcommon_memory_order_acq_rel:
	case mcommon_memory_order_seq_cst:
		//DEF_ATOMIC_CMPEXCH_WEAK (Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value);
		//return expect;
	case mcommon_memory_order_relaxed:
		//DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_nf), __ATOMIC_RELAXED, ptr, expect, new_value);
		//return expect;
	case mcommon_memory_order_consume:
		//DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_CONSUME, ptr, expect, new_value);
		//return expect;
	case mcommon_memory_order_acquire:
		//DEF_ATOMIC_CMPEXCH_WEAK (Pointer, MSVC_MEMORDER_SUFFIX (_acq), __ATOMIC_ACQUIRE, ptr, expect, new_value);
		//return expect;
		do 
		{ 
			expect = (void*)_InterlockedCompareExchange( (long volatile *)ptr, (long)new_value, (long)expect); 
		} while (0);
		return expect;
	default:
		BSON_UNREACHABLE ("Invalid mcommon_memory_order value");
		return NULL;
	}
}

#endif // #if !(defined(_MSC_VER) && (_MSC_VER < 1700) && !defined(WIN64))

static BSON_INLINE void *
mcommon_atomic_ptr_fetch (void *volatile const *ptr, enum mcommon_memory_order ord)
{
   return mcommon_atomic_ptr_compare_exchange_strong ((void *volatile *) ptr, NULL, NULL, ord);
}

#undef DECL_ATOMIC_STDINT
#undef DECL_ATOMIC_INTEGRAL
#undef DEF_ATOMIC_OP
#undef DEF_ATOMIC_CMPEXCH_STRONG
#undef DEF_ATOMIC_CMPEXCH_WEAK
#undef MSVC_MEMORDER_SUFFIX

/**
 * @brief Generate a full-fence memory barrier at the call site.
 */
static BSON_INLINE void
mcommon_atomic_thread_fence (void)
{
   BSON_IF_MSVC (MemoryBarrier ();)
   BSON_IF_GNU_LIKE (__sync_synchronize ();)
   MCOMMON_IF_GNU_LEGACY_ATOMICS (__sync_synchronize ();)
}

#ifdef MCOMMON_USE_LEGACY_GCC_ATOMICS
#undef BSON_IF_GNU_LIKE
#define BSON_IF_GNU_LIKE(...) __VA_ARGS__
#endif
#undef MCOMMON_IF_GNU_LEGACY_ATOMICS
#undef MCOMMON_USE_LEGACY_GCC_ATOMICS


#undef MCOMMON_EMULATE_PTR
#undef MCOMMON_EMULATE_INT32
#undef MCOMMON_EMULATE_INT


#endif /* MONGO_C_DRIVER_COMMON_ATOMIC_PRIVATE_H */
