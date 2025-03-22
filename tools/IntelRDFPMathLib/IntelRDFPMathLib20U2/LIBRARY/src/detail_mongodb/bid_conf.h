/******************************************************************************
  Copyright (c) 2007-2018, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#if defined(__cplusplus) 
#define BID_EXTERN_C extern "C" 
#else 
#define BID_EXTERN_C extern
#endif 

#ifndef _BID_CONF_H
#define _BID_CONF_H

// Name Changes

#define _IDEC_glbflags __bid_IDEC_glbflags
#define _IDEC_glbround __bid_IDEC_glbround
#define _IDEC_glbexcepthandling __bid_IDEC_glbexcepthandling
#define _IDEC_glbexceptionmasks __bid_IDEC_glbexceptionmasks

#define bid32_inf          __mongocrypt_bid32_inf
#define bid64_inf          __mongocrypt_bid64_inf
#define bid128_inf         __mongocrypt_bid128_inf
#define bid32_exp          __mongocrypt_bid32_exp
#define bid32_log          __mongocrypt_bid32_log
#define bid32_pow          __mongocrypt_bid32_pow
#define bid64_exp          __mongocrypt_bid64_exp
#define bid64_log          __mongocrypt_bid64_log
#define bid64_pow          __mongocrypt_bid64_pow
#define bid128_exp         __mongocrypt_bid128_exp
#define bid128_log         __mongocrypt_bid128_log
#define bid128_pow         __mongocrypt_bid128_pow
#define bid32_cbrt         __mongocrypt_bid32_cbrt
#define bid64_cbrt         __mongocrypt_bid64_cbrt
#define bid128_cbrt        __mongocrypt_bid128_cbrt
#define bid32_atan2        __mongocrypt_bid32_atan2
#define bid64_atan2        __mongocrypt_bid64_atan2
#define bid128_atan2       __mongocrypt_bid128_atan2
#define bid32_fmod         __mongocrypt_bid32_fmod
#define bid64_fmod         __mongocrypt_bid64_fmod
#define bid128_fmod        __mongocrypt_bid128_fmod
#define bid32_modf         __mongocrypt_bid32_modf
#define bid64_modf         __mongocrypt_bid64_modf
#define bid128_modf        __mongocrypt_bid128_modf
#define bid32_hypot        __mongocrypt_bid32_hypot
#define bid64_hypot        __mongocrypt_bid64_hypot
#define bid128_hypot       __mongocrypt_bid128_hypot
#define bid32_sin          __mongocrypt_bid32_sin
#define bid64_sin          __mongocrypt_bid64_sin
#define bid128_sin         __mongocrypt_bid128_sin
#define bid32_cos          __mongocrypt_bid32_cos
#define bid64_cos          __mongocrypt_bid64_cos
#define bid128_cos         __mongocrypt_bid128_cos
#define bid32_tan          __mongocrypt_bid32_tan
#define bid64_tan          __mongocrypt_bid64_tan
#define bid128_tan         __mongocrypt_bid128_tan
#define bid32_asin          __mongocrypt_bid32_asin
#define bid64_asin          __mongocrypt_bid64_asin
#define bid128_asin         __mongocrypt_bid128_asin
#define bid32_acos          __mongocrypt_bid32_acos
#define bid64_acos          __mongocrypt_bid64_acos
#define bid128_acos         __mongocrypt_bid128_acos
#define bid32_atan          __mongocrypt_bid32_atan
#define bid64_atan          __mongocrypt_bid64_atan
#define bid128_atan         __mongocrypt_bid128_atan
#define bid32_sinh          __mongocrypt_bid32_sinh
#define bid64_sinh          __mongocrypt_bid64_sinh
#define bid128_sinh         __mongocrypt_bid128_sinh
#define bid32_cosh          __mongocrypt_bid32_cosh
#define bid64_cosh          __mongocrypt_bid64_cosh
#define bid128_cosh         __mongocrypt_bid128_cosh
#define bid32_tanh          __mongocrypt_bid32_tanh
#define bid64_tanh          __mongocrypt_bid64_tanh
#define bid128_tanh         __mongocrypt_bid128_tanh
#define bid32_asinh         __mongocrypt_bid32_asinh
#define bid64_asinh         __mongocrypt_bid64_asinh
#define bid128_asinh        __mongocrypt_bid128_asinh
#define bid32_acosh         __mongocrypt_bid32_acosh
#define bid64_acosh         __mongocrypt_bid64_acosh
#define bid128_acosh        __mongocrypt_bid128_acosh
#define bid32_atanh         __mongocrypt_bid32_atanh
#define bid64_atanh         __mongocrypt_bid64_atanh
#define bid128_atanh        __mongocrypt_bid128_atanh
#define bid32_log1p         __mongocrypt_bid32_log1p
#define bid64_log1p         __mongocrypt_bid64_log1p
#define bid128_log1p        __mongocrypt_bid128_log1p
#define bid32_exp2           __mongocrypt_bid32_exp2
#define bid64_exp2          __mongocrypt_bid64_exp2
#define bid128_exp2         __mongocrypt_bid128_exp2
#define bid32_exp10           __mongocrypt_bid32_exp10
#define bid64_exp10          __mongocrypt_bid64_exp10
#define bid128_exp10         __mongocrypt_bid128_exp10
#define bid32_expm1         __mongocrypt_bid32_expm1
#define bid64_expm1         __mongocrypt_bid64_expm1
#define bid128_expm1        __mongocrypt_bid128_expm1
#define bid32_log10         __mongocrypt_bid32_log10
#define bid64_log10         __mongocrypt_bid64_log10
#define bid128_log10        __mongocrypt_bid128_log10
#define bid32_log2          __mongocrypt_bid32_log2
#define bid64_log2          __mongocrypt_bid64_log2
#define bid128_log2         __mongocrypt_bid128_log2
#define bid32_erf           __mongocrypt_bid32_erf
#define bid64_erf           __mongocrypt_bid64_erf
#define bid128_erf          __mongocrypt_bid128_erf
#define bid32_erfc          __mongocrypt_bid32_erfc
#define bid64_erfc          __mongocrypt_bid64_erfc
#define bid128_erfc         __mongocrypt_bid128_erfc
#define bid32_tgamma        __mongocrypt_bid32_tgamma
#define bid64_tgamma        __mongocrypt_bid64_tgamma
#define bid128_tgamma       __mongocrypt_bid128_tgamma
#define bid32_lgamma        __mongocrypt_bid32_lgamma
#define bid64_lgamma        __mongocrypt_bid64_lgamma
#define bid128_lgamma       __mongocrypt_bid128_lgamma

#define bid32_frexp         __mongocrypt_bid32_frexp
#define bid64_frexp         __mongocrypt_bid64_frexp
#define bid128_frexp        __mongocrypt_bid128_frexp
#define bid32_logb          __mongocrypt_bid32_logb
#define bid64_logb          __mongocrypt_bid64_logb
#define bid128_logb         __mongocrypt_bid128_logb
#define bid32_scalbln       __mongocrypt_bid32_scalbln
#define bid64_scalbln       __mongocrypt_bid64_scalbln
#define bid128_scalbln      __mongocrypt_bid128_scalbln
#define bid32_nearbyint     __mongocrypt_bid32_nearbyint
#define bid64_nearbyint     __mongocrypt_bid64_nearbyint
#define bid128_nearbyint    __mongocrypt_bid128_nearbyint
#define bid32_lrint         __mongocrypt_bid32_lrint
#define bid64_lrint         __mongocrypt_bid64_lrint
#define bid128_lrint        __mongocrypt_bid128_lrint
#define bid32_llrint        __mongocrypt_bid32_llrint
#define bid64_llrint        __mongocrypt_bid64_llrint
#define bid128_llrint       __mongocrypt_bid128_llrint
#define bid32_lround        __mongocrypt_bid32_lround
#define bid64_lround        __mongocrypt_bid64_lround
#define bid128_lround       __mongocrypt_bid128_lround
#define bid32_llround       __mongocrypt_bid32_llround
#define bid64_llround       __mongocrypt_bid64_llround
#define bid128_llround      __mongocrypt_bid128_llround
#define bid32_nan           __mongocrypt_bid32_nan
#define bid64_nan           __mongocrypt_bid64_nan
#define bid128_nan          __mongocrypt_bid128_nan
#define bid32_nexttoward    __mongocrypt_bid32_nexttoward
#define bid64_nexttoward    __mongocrypt_bid64_nexttoward
#define bid128_nexttoward   __mongocrypt_bid128_nexttoward
#define bid32_fdim           __mongocrypt_bid32_fdim
#define bid64_fdim           __mongocrypt_bid64_fdim
#define bid128_fdim          __mongocrypt_bid128_fdim
#define bid32_quantexp      __mongocrypt_bid32_quantexp
#define bid64_quantexp      __mongocrypt_bid64_quantexp
#define bid128_quantexp     __mongocrypt_bid128_quantexp
#define bid32_quantum       __mongocrypt_bid32_quantum
#define bid64_quantum       __mongocrypt_bid64_quantum
#define bid128_quantum      __mongocrypt_bid128_quantum
#define bid32_llquantexp    __mongocrypt_bid32_llquantexp
#define bid64_llquantexp    __mongocrypt_bid64_llquantexp
#define bid128_llquantexp   __mongocrypt_bid128_llquantexp

#define bid32_add          __mongocrypt_bid32_add
#define bid32_sub          __mongocrypt_bid32_sub
#define bid32_mul          __mongocrypt_bid32_mul
#define bid32_div          __mongocrypt_bid32_div
#define bid32_fma          __mongocrypt_bid32_fma
#define bid32_sqrt         __mongocrypt_bid32_sqrt
#define bid32_rem          __mongocrypt_bid32_rem
#define bid32_ilogb        __mongocrypt_bid32_ilogb
#define bid32_scalbn       __mongocrypt_bid32_scalbn
#define bid32_ldexp        __mongocrypt_bid32_ldexp
#define bid32_to_string    __mongocrypt_bid32_to_string
#define bid32_from_string  __mongocrypt_bid32_from_string
#define bid32_quantize     __mongocrypt_bid32_quantize
#define bid32_nextup       __mongocrypt_bid32_nextup
#define bid32_nextdown     __mongocrypt_bid32_nextdown
#define bid32_minnum       __mongocrypt_bid32_minnum
#define bid32_minnum_mag   __mongocrypt_bid32_minnum_mag
#define bid32_maxnum       __mongocrypt_bid32_maxnum
#define bid32_maxnum_mag   __mongocrypt_bid32_maxnum_mag
#define bid32_from_int32   __mongocrypt_bid32_from_int32
#define bid32_from_uint32  __mongocrypt_bid32_from_uint32
#define bid32_from_int64   __mongocrypt_bid32_from_int64
#define bid32_from_uint64  __mongocrypt_bid32_from_uint64
#define bid32_isSigned     __mongocrypt_bid32_isSigned
#define bid32_isNormal     __mongocrypt_bid32_isNormal
#define bid32_isSubnormal  __mongocrypt_bid32_isSubnormal
#define bid32_isFinite     __mongocrypt_bid32_isFinite
#define bid32_isZero       __mongocrypt_bid32_isZero
#define bid32_isInf        __mongocrypt_bid32_isInf
#define bid32_isSignaling  __mongocrypt_bid32_isSignaling
#define bid32_isNaN        __mongocrypt_bid32_isNaN
#define bid32_copy         __mongocrypt_bid32_copy
#define bid32_negate       __mongocrypt_bid32_negate
#define bid32_abs          __mongocrypt_bid32_abs
#define bid32_copySign     __mongocrypt_bid32_copySign
#define bid32_class        __mongocrypt_bid32_class
#define bid32_sameQuantum  __mongocrypt_bid32_sameQuantum
#define bid32_totalOrder   __mongocrypt_bid32_totalOrder
#define bid32_totalOrderMag  __mongocrypt_bid32_totalOrderMag
#define bid32_radix        __mongocrypt_bid32_radix
#define bid32_quiet_equal  __mongocrypt_bid32_quiet_equal
#define bid32_quiet_greater  __mongocrypt_bid32_quiet_greater
#define bid32_quiet_greater_equal  __mongocrypt_bid32_quiet_greater_equal
#define bid32_quiet_greater_unordered  __mongocrypt_bid32_quiet_greater_unordered
#define bid32_quiet_less  __mongocrypt_bid32_quiet_less
#define bid32_quiet_less_equal  __mongocrypt_bid32_quiet_less_equal
#define bid32_quiet_less_unordered  __mongocrypt_bid32_quiet_less_unordered
#define bid32_quiet_not_equal  __mongocrypt_bid32_quiet_not_equal
#define bid32_quiet_not_greater  __mongocrypt_bid32_quiet_not_greater
#define bid32_quiet_not_less  __mongocrypt_bid32_quiet_not_less
#define bid32_quiet_ordered  __mongocrypt_bid32_quiet_ordered
#define bid32_quiet_unordered  __mongocrypt_bid32_quiet_unordered
#define bid32_signaling_greater  __mongocrypt_bid32_signaling_greater
#define bid32_signaling_greater_equal  __mongocrypt_bid32_signaling_greater_equal
#define bid32_signaling_greater_unordered  __mongocrypt_bid32_signaling_greater_unordered
#define bid32_signaling_less  __mongocrypt_bid32_signaling_less
#define bid32_signaling_less_equal  __mongocrypt_bid32_signaling_less_equal
#define bid32_signaling_less_unordered  __mongocrypt_bid32_signaling_less_unordered
#define bid32_signaling_not_greater  __mongocrypt_bid32_signaling_not_greater
#define bid32_signaling_not_less  __mongocrypt_bid32_signaling_not_less
#define bid32_isCanonical  __mongocrypt_bid32_isCanonical
#define bid32_nextafter  __mongocrypt_bid32_nextafter
#define bid32_round_integral_exact  __mongocrypt_bid32_round_integral_exact
#define bid32_round_integral_nearest_away  __mongocrypt_bid32_round_integral_nearest_away
#define bid32_round_integral_nearest_even  __mongocrypt_bid32_round_integral_nearest_even
#define bid32_round_integral_negative  __mongocrypt_bid32_round_integral_negative
#define bid32_round_integral_positive  __mongocrypt_bid32_round_integral_positive
#define bid32_round_integral_zero  __mongocrypt_bid32_round_integral_zero
#define bid32_to_int16_ceil  __mongocrypt_bid32_to_int16_ceil
#define bid32_to_int16_floor  __mongocrypt_bid32_to_int16_floor
#define bid32_to_int16_int  __mongocrypt_bid32_to_int16_int
#define bid32_to_int16_rnint  __mongocrypt_bid32_to_int16_rnint
#define bid32_to_int16_rninta  __mongocrypt_bid32_to_int16_rninta
#define bid32_to_int16_xceil  __mongocrypt_bid32_to_int16_xceil
#define bid32_to_int16_xfloor  __mongocrypt_bid32_to_int16_xfloor
#define bid32_to_int16_xint  __mongocrypt_bid32_to_int16_xint
#define bid32_to_int16_xrnint  __mongocrypt_bid32_to_int16_xrnint
#define bid32_to_int16_xrninta  __mongocrypt_bid32_to_int16_xrninta
#define bid32_to_int32_ceil  __mongocrypt_bid32_to_int32_ceil
#define bid32_to_int32_floor  __mongocrypt_bid32_to_int32_floor
#define bid32_to_int32_int  __mongocrypt_bid32_to_int32_int
#define bid32_to_int32_rnint  __mongocrypt_bid32_to_int32_rnint
#define bid32_to_int32_rninta  __mongocrypt_bid32_to_int32_rninta
#define bid32_to_int32_xceil  __mongocrypt_bid32_to_int32_xceil
#define bid32_to_int32_xfloor  __mongocrypt_bid32_to_int32_xfloor
#define bid32_to_int32_xint  __mongocrypt_bid32_to_int32_xint
#define bid32_to_int32_xrnint  __mongocrypt_bid32_to_int32_xrnint
#define bid32_to_int32_xrninta  __mongocrypt_bid32_to_int32_xrninta
#define bid32_to_int64_ceil  __mongocrypt_bid32_to_int64_ceil
#define bid32_to_int64_floor  __mongocrypt_bid32_to_int64_floor
#define bid32_to_int64_int  __mongocrypt_bid32_to_int64_int
#define bid32_to_int64_rnint  __mongocrypt_bid32_to_int64_rnint
#define bid32_to_int64_rninta  __mongocrypt_bid32_to_int64_rninta
#define bid32_to_int64_xceil  __mongocrypt_bid32_to_int64_xceil
#define bid32_to_int64_xfloor  __mongocrypt_bid32_to_int64_xfloor
#define bid32_to_int64_xint  __mongocrypt_bid32_to_int64_xint
#define bid32_to_int64_xrnint  __mongocrypt_bid32_to_int64_xrnint
#define bid32_to_int64_xrninta  __mongocrypt_bid32_to_int64_xrninta
#define bid32_to_int8_ceil  __mongocrypt_bid32_to_int8_ceil
#define bid32_to_int8_floor  __mongocrypt_bid32_to_int8_floor
#define bid32_to_int8_int  __mongocrypt_bid32_to_int8_int
#define bid32_to_int8_rnint  __mongocrypt_bid32_to_int8_rnint
#define bid32_to_int8_rninta  __mongocrypt_bid32_to_int8_rninta
#define bid32_to_int8_xceil  __mongocrypt_bid32_to_int8_xceil
#define bid32_to_int8_xfloor  __mongocrypt_bid32_to_int8_xfloor
#define bid32_to_int8_xint  __mongocrypt_bid32_to_int8_xint
#define bid32_to_int8_xrnint  __mongocrypt_bid32_to_int8_xrnint
#define bid32_to_int8_xrninta  __mongocrypt_bid32_to_int8_xrninta
#define bid32_to_uint16_ceil  __mongocrypt_bid32_to_uint16_ceil
#define bid32_to_uint16_floor  __mongocrypt_bid32_to_uint16_floor
#define bid32_to_uint16_int  __mongocrypt_bid32_to_uint16_int
#define bid32_to_uint16_rnint  __mongocrypt_bid32_to_uint16_rnint
#define bid32_to_uint16_rninta  __mongocrypt_bid32_to_uint16_rninta
#define bid32_to_uint16_xceil  __mongocrypt_bid32_to_uint16_xceil
#define bid32_to_uint16_xfloor  __mongocrypt_bid32_to_uint16_xfloor
#define bid32_to_uint16_xint  __mongocrypt_bid32_to_uint16_xint
#define bid32_to_uint16_xrnint  __mongocrypt_bid32_to_uint16_xrnint
#define bid32_to_uint16_xrninta  __mongocrypt_bid32_to_uint16_xrninta
#define bid32_to_uint32_ceil  __mongocrypt_bid32_to_uint32_ceil
#define bid32_to_uint32_floor  __mongocrypt_bid32_to_uint32_floor
#define bid32_to_uint32_int  __mongocrypt_bid32_to_uint32_int
#define bid32_to_uint32_rnint  __mongocrypt_bid32_to_uint32_rnint
#define bid32_to_uint32_rninta  __mongocrypt_bid32_to_uint32_rninta
#define bid32_to_uint32_xceil  __mongocrypt_bid32_to_uint32_xceil
#define bid32_to_uint32_xfloor  __mongocrypt_bid32_to_uint32_xfloor
#define bid32_to_uint32_xint  __mongocrypt_bid32_to_uint32_xint
#define bid32_to_uint32_xrnint  __mongocrypt_bid32_to_uint32_xrnint
#define bid32_to_uint32_xrninta  __mongocrypt_bid32_to_uint32_xrninta
#define bid32_to_uint64_ceil  __mongocrypt_bid32_to_uint64_ceil
#define bid32_to_uint64_floor  __mongocrypt_bid32_to_uint64_floor
#define bid32_to_uint64_int  __mongocrypt_bid32_to_uint64_int
#define bid32_to_uint64_rnint  __mongocrypt_bid32_to_uint64_rnint
#define bid32_to_uint64_rninta  __mongocrypt_bid32_to_uint64_rninta
#define bid32_to_uint64_xceil  __mongocrypt_bid32_to_uint64_xceil
#define bid32_to_uint64_xfloor  __mongocrypt_bid32_to_uint64_xfloor
#define bid32_to_uint64_xint  __mongocrypt_bid32_to_uint64_xint
#define bid32_to_uint64_xrnint  __mongocrypt_bid32_to_uint64_xrnint
#define bid32_to_uint64_xrninta  __mongocrypt_bid32_to_uint64_xrninta
#define bid32_to_uint8_ceil  __mongocrypt_bid32_to_uint8_ceil
#define bid32_to_uint8_floor  __mongocrypt_bid32_to_uint8_floor
#define bid32_to_uint8_int  __mongocrypt_bid32_to_uint8_int
#define bid32_to_uint8_rnint  __mongocrypt_bid32_to_uint8_rnint
#define bid32_to_uint8_rninta  __mongocrypt_bid32_to_uint8_rninta
#define bid32_to_uint8_xceil  __mongocrypt_bid32_to_uint8_xceil
#define bid32_to_uint8_xfloor  __mongocrypt_bid32_to_uint8_xfloor
#define bid32_to_uint8_xint  __mongocrypt_bid32_to_uint8_xint
#define bid32_to_uint8_xrnint  __mongocrypt_bid32_to_uint8_xrnint
#define bid32_to_uint8_xrninta  __mongocrypt_bid32_to_uint8_xrninta

#define bid64_add  __mongocrypt_bid64_add
#define bid64_sub  __mongocrypt_bid64_sub
#define bid64_mul  __mongocrypt_bid64_mul
#define bid64_div  __mongocrypt_bid64_div
#define bid64dq_div  __mongocrypt_bid64dq_div
#define bid64qd_div  __mongocrypt_bid64qd_div
#define bid64qq_div  __mongocrypt_bid64qq_div
#define bid64q_sqrt  __mongocrypt_bid64q_sqrt
#define bid64_sqrt  __mongocrypt_bid64_sqrt
#define bid64_rem  __mongocrypt_bid64_rem
#define bid64_fma  __mongocrypt_bid64_fma
#define bid64_scalbn  __mongocrypt_bid64_scalbn
#define bid64_ldexp   __mongocrypt_bid64_ldexp
#define bid_round128_19_38  __mongocrypt_bid_round128_19_38
#define bid_round192_39_57  __mongocrypt_bid_round192_39_57
#define bid_round256_58_76  __mongocrypt_bid_round256_58_76
#define bid_round64_2_18  __mongocrypt_bid_round64_2_18
#define bid64_nextafter  __mongocrypt_bid64_nextafter
#define bid64_nextdown  __mongocrypt_bid64_nextdown
#define bid64_nextup  __mongocrypt_bid64_nextup
#define bid_b2d  __mongocrypt_bid_b2d
#define bid_b2d2  __mongocrypt_bid_b2d2
#define bid_b2d3  __mongocrypt_bid_b2d3
#define bid_b2d4  __mongocrypt_bid_b2d4
#define bid_b2d5  __mongocrypt_bid_b2d5
#define bid_to_dpd128  __mongocrypt_bid_to_dpd128
#define bid_to_dpd32  __mongocrypt_bid_to_dpd32
#define bid_to_dpd64  __mongocrypt_bid_to_dpd64
#define bid_d2b  __mongocrypt_bid_d2b
#define bid_d2b2  __mongocrypt_bid_d2b2
#define bid_d2b3  __mongocrypt_bid_d2b3
#define bid_d2b4  __mongocrypt_bid_d2b4
#define bid_d2b5  __mongocrypt_bid_d2b5
#define bid_d2b6  __mongocrypt_bid_d2b6
#define bid_dpd_to_bid128  __mongocrypt_bid_dpd_to_bid128
#define bid_dpd_to_bid32  __mongocrypt_bid_dpd_to_bid32
#define bid_dpd_to_bid64  __mongocrypt_bid_dpd_to_bid64
#define bid128_nextafter  __mongocrypt_bid128_nextafter
#define bid128_nextdown  __mongocrypt_bid128_nextdown
#define bid128_nextup  __mongocrypt_bid128_nextup
#define bid64_ilogb  __mongocrypt_bid64_ilogb
#define bid64_quantize  __mongocrypt_bid64_quantize
#define bid_estimate_bin_expon  __mongocrypt_bid_estimate_bin_expon
#define bid_estimate_decimal_digits  __mongocrypt_bid_estimate_decimal_digits
#define bid_power10_index_binexp  __mongocrypt_bid_power10_index_binexp
#define bid_power10_index_binexp_128  __mongocrypt_bid_power10_index_binexp_128
#define bid_power10_table_128  __mongocrypt_bid_power10_table_128
#define bid_reciprocals10_128  __mongocrypt_bid_reciprocals10_128
#define bid_reciprocals10_64  __mongocrypt_bid_reciprocals10_64
#define bid_recip_scale  __mongocrypt_bid_recip_scale
#define bid_round_const_table  __mongocrypt_bid_round_const_table
#define bid_round_const_table_128  __mongocrypt_bid_round_const_table_128
#define bid_short_recip_scale  __mongocrypt_bid_short_recip_scale
#define bid64_from_string  __mongocrypt_bid64_from_string
#define bid64_to_string  __mongocrypt_bid64_to_string
#define bid_Inv_Tento9  __mongocrypt_bid_Inv_Tento9
#define bid_midi_tbl  __mongocrypt_bid_midi_tbl
#define bid_Tento3  __mongocrypt_bid_Tento3
#define bid_Tento6  __mongocrypt_bid_Tento6
#define bid_Tento9  __mongocrypt_bid_Tento9
#define bid_Twoto30_m_10to9  __mongocrypt_bid_Twoto30_m_10to9
#define bid_Twoto60  __mongocrypt_bid_Twoto60
#define bid_Twoto60_m_10to18  __mongocrypt_bid_Twoto60_m_10to18
#define bid_convert_table  __mongocrypt_bid_convert_table
#define bid_factors  __mongocrypt_bid_factors
#define bid_packed_10000_zeros  __mongocrypt_bid_packed_10000_zeros
#define bid_char_table2  __mongocrypt_bid_char_table2
#define bid_char_table3  __mongocrypt_bid_char_table3
#define bid_Ex128m128  __mongocrypt_bid_Ex128m128
#define bid_Ex192m192  __mongocrypt_bid_Ex192m192
#define bid_Ex256m256  __mongocrypt_bid_Ex256m256
#define bid_Ex64m64  __mongocrypt_bid_Ex64m64
#define bid_half128  __mongocrypt_bid_half128
#define bid_half192  __mongocrypt_bid_half192
#define bid_half256  __mongocrypt_bid_half256
#define bid_half64  __mongocrypt_bid_half64
#define bid_Kx128  __mongocrypt_bid_Kx128
#define bid_Kx192  __mongocrypt_bid_Kx192
#define bid_Kx256  __mongocrypt_bid_Kx256
#define bid_Kx64  __mongocrypt_bid_Kx64
#define bid_mask128  __mongocrypt_bid_mask128
#define bid_mask192  __mongocrypt_bid_mask192
#define bid_mask256  __mongocrypt_bid_mask256
#define bid_mask64  __mongocrypt_bid_mask64
#define bid_maskhigh128  __mongocrypt_bid_maskhigh128
#define bid_maskhigh128M  __mongocrypt_bid_maskhigh128M
#define bid_maskhigh192M  __mongocrypt_bid_maskhigh192M
#define bid_maskhigh256M  __mongocrypt_bid_maskhigh256M
#define bid_midpoint128  __mongocrypt_bid_midpoint128
#define bid_midpoint192  __mongocrypt_bid_midpoint192
#define bid_midpoint256  __mongocrypt_bid_midpoint256
#define bid_midpoint64  __mongocrypt_bid_midpoint64
#define bid_nr_digits  __mongocrypt_bid_nr_digits
#define bid_onehalf128  __mongocrypt_bid_onehalf128
#define bid_onehalf128M  __mongocrypt_bid_onehalf128M
#define bid_onehalf192M  __mongocrypt_bid_onehalf192M
#define bid_onehalf256M  __mongocrypt_bid_onehalf256M
#define bid_shiftright128  __mongocrypt_bid_shiftright128
#define bid_shiftright128M  __mongocrypt_bid_shiftright128M
#define bid_shiftright192M  __mongocrypt_bid_shiftright192M
#define bid_shiftright256M  __mongocrypt_bid_shiftright256M
#define bid_shift_ten2m3k128  __mongocrypt_bid_shift_ten2m3k128
#define bid_shift_ten2m3k64  __mongocrypt_bid_shift_ten2m3k64
#define bid_ten2k128  __mongocrypt_bid_ten2k128
#define bid_ten2k256  __mongocrypt_bid_ten2k256
#define bid_ten2k64  __mongocrypt_bid_ten2k64
#define bid_ten2m3k128  __mongocrypt_bid_ten2m3k128
#define bid_ten2m3k64  __mongocrypt_bid_ten2m3k64
#define bid_ten2mk128  __mongocrypt_bid_ten2mk128
#define bid_ten2mk128M  __mongocrypt_bid_ten2mk128M
#define bid_ten2mk128trunc  __mongocrypt_bid_ten2mk128trunc
#define bid_ten2mk128truncM  __mongocrypt_bid_ten2mk128truncM
#define bid_ten2mk192M  __mongocrypt_bid_ten2mk192M
#define bid_ten2mk192truncM  __mongocrypt_bid_ten2mk192truncM
#define bid_ten2mk256M  __mongocrypt_bid_ten2mk256M
#define bid_ten2mk256truncM  __mongocrypt_bid_ten2mk256truncM
#define bid_ten2mk64  __mongocrypt_bid_ten2mk64
#define bid_ten2mxtrunc128  __mongocrypt_bid_ten2mxtrunc128
#define bid_ten2mxtrunc192  __mongocrypt_bid_ten2mxtrunc192
#define bid_ten2mxtrunc256  __mongocrypt_bid_ten2mxtrunc256
#define bid_ten2mxtrunc64  __mongocrypt_bid_ten2mxtrunc64
#define bid128_add  __mongocrypt_bid128_add
#define bid128dd_add  __mongocrypt_bid128dd_add
#define bid128dd_sub  __mongocrypt_bid128dd_sub
#define bid128dq_add  __mongocrypt_bid128dq_add
#define bid128dq_sub  __mongocrypt_bid128dq_sub
#define bid128qd_add  __mongocrypt_bid128qd_add
#define bid128qd_sub  __mongocrypt_bid128qd_sub
#define bid128_sub  __mongocrypt_bid128_sub
#define bid64dq_add  __mongocrypt_bid64dq_add
#define bid64dq_sub  __mongocrypt_bid64dq_sub
#define bid64qd_add  __mongocrypt_bid64qd_add
#define bid64qd_sub  __mongocrypt_bid64qd_sub
#define bid64qq_add  __mongocrypt_bid64qq_add
#define bid64qq_sub  __mongocrypt_bid64qq_sub
#define bid128dd_mul  __mongocrypt_bid128dd_mul
#define bid128dq_mul  __mongocrypt_bid128dq_mul
#define bid128_mul  __mongocrypt_bid128_mul
#define bid128qd_mul  __mongocrypt_bid128qd_mul
#define bid64dq_mul  __mongocrypt_bid64dq_mul
#define bid64qd_mul  __mongocrypt_bid64qd_mul
#define bid64qq_mul  __mongocrypt_bid64qq_mul
#define bid128dd_div  __mongocrypt_bid128dd_div
#define bid128_div  __mongocrypt_bid128_div
#define bid128dq_div  __mongocrypt_bid128dq_div
#define bid128qd_div  __mongocrypt_bid128qd_div
#define bid128d_sqrt  __mongocrypt_bid128d_sqrt
#define bid128_sqrt  __mongocrypt_bid128_sqrt
#define bid128ddd_fma  __mongocrypt_bid128ddd_fma
#define bid128ddq_fma  __mongocrypt_bid128ddq_fma
#define bid128dqd_fma  __mongocrypt_bid128dqd_fma
#define bid128dqq_fma  __mongocrypt_bid128dqq_fma
#define bid128_fma  __mongocrypt_bid128_fma
#define bid128qdd_fma  __mongocrypt_bid128qdd_fma
#define bid128qdq_fma  __mongocrypt_bid128qdq_fma
#define bid128qqd_fma  __mongocrypt_bid128qqd_fma
#define bid64ddq_fma  __mongocrypt_bid64ddq_fma
#define bid64dqd_fma  __mongocrypt_bid64dqd_fma
#define bid64dqq_fma  __mongocrypt_bid64dqq_fma
#define bid64qdd_fma  __mongocrypt_bid64qdd_fma
#define bid64qdq_fma  __mongocrypt_bid64qdq_fma
#define bid64qqd_fma  __mongocrypt_bid64qqd_fma
#define bid64qqq_fma  __mongocrypt_bid64qqq_fma
#define bid128_round_integral_exact  __mongocrypt_bid128_round_integral_exact
#define bid128_round_integral_nearest_away  __mongocrypt_bid128_round_integral_nearest_away
#define bid128_round_integral_nearest_even  __mongocrypt_bid128_round_integral_nearest_even
#define bid128_round_integral_negative  __mongocrypt_bid128_round_integral_negative
#define bid128_round_integral_positive  __mongocrypt_bid128_round_integral_positive
#define bid128_round_integral_zero  __mongocrypt_bid128_round_integral_zero
#define bid64_round_integral_exact  __mongocrypt_bid64_round_integral_exact
#define bid64_round_integral_nearest_away  __mongocrypt_bid64_round_integral_nearest_away
#define bid64_round_integral_nearest_even  __mongocrypt_bid64_round_integral_nearest_even
#define bid64_round_integral_negative  __mongocrypt_bid64_round_integral_negative
#define bid64_round_integral_positive  __mongocrypt_bid64_round_integral_positive
#define bid64_round_integral_zero  __mongocrypt_bid64_round_integral_zero
#define bid128_quantize  __mongocrypt_bid128_quantize
#define bid128_scalbn  __mongocrypt_bid128_scalbn
#define bid128_ldexp   __mongocrypt_bid128_ldexp
#define bid64_maxnum  __mongocrypt_bid64_maxnum
#define bid64_maxnum_mag  __mongocrypt_bid64_maxnum_mag
#define bid64_minnum  __mongocrypt_bid64_minnum
#define bid64_minnum_mag  __mongocrypt_bid64_minnum_mag
#define bid128_maxnum  __mongocrypt_bid128_maxnum
#define bid128_maxnum_mag  __mongocrypt_bid128_maxnum_mag
#define bid128_minnum  __mongocrypt_bid128_minnum
#define bid128_minnum_mag  __mongocrypt_bid128_minnum_mag
#define bid128_rem  __mongocrypt_bid128_rem
#define bid128_ilogb  __mongocrypt_bid128_ilogb
#define bid_getDecimalRoundingDirection  __mongocrypt_bid_getDecimalRoundingDirection
#define bid_is754  __mongocrypt_bid_is754
#define bid_is754R  __mongocrypt_bid_is754R
#define bid_signalException  __mongocrypt_bid_signalException
#define bid_lowerFlags  __mongocrypt_bid_lowerFlags
#define bid_restoreFlags  __mongocrypt_bid_restoreFlags
#define bid_saveFlags  __mongocrypt_bid_saveFlags
#define bid_setDecimalRoundingDirection  __mongocrypt_bid_setDecimalRoundingDirection
#define bid_testFlags  __mongocrypt_bid_testFlags
#define bid_testSavedFlags  __mongocrypt_bid_testSavedFlags
#define bid32_to_bid64  __mongocrypt_bid32_to_bid64
#define bid64_to_bid32  __mongocrypt_bid64_to_bid32
#define bid128_to_string  __mongocrypt_bid128_to_string
#define mod10_18_tbl __bid_mod10_18_tbl
#define bid128_to_bid32  __mongocrypt_bid128_to_bid32
#define bid32_to_bid128  __mongocrypt_bid32_to_bid128
#define bid128_to_bid64  __mongocrypt_bid128_to_bid64
#define bid64_to_bid128  __mongocrypt_bid64_to_bid128
#define bid128_from_string  __mongocrypt_bid128_from_string
#define bid128_from_int32  __mongocrypt_bid128_from_int32
#define bid128_from_int64  __mongocrypt_bid128_from_int64
#define bid128_from_uint32  __mongocrypt_bid128_from_uint32
#define bid128_from_uint64  __mongocrypt_bid128_from_uint64
#define bid64_from_int32  __mongocrypt_bid64_from_int32
#define bid64_from_int64  __mongocrypt_bid64_from_int64
#define bid64_from_uint32  __mongocrypt_bid64_from_uint32
#define bid64_from_uint64  __mongocrypt_bid64_from_uint64
#define bid64_abs  __mongocrypt_bid64_abs
#define bid64_class  __mongocrypt_bid64_class
#define bid64_copy  __mongocrypt_bid64_copy
#define bid64_copySign  __mongocrypt_bid64_copySign
#define bid64_isCanonical  __mongocrypt_bid64_isCanonical
#define bid64_isFinite  __mongocrypt_bid64_isFinite
#define bid64_isInf  __mongocrypt_bid64_isInf
#define bid64_isNaN  __mongocrypt_bid64_isNaN
#define bid64_isNormal  __mongocrypt_bid64_isNormal
#define bid64_isSignaling  __mongocrypt_bid64_isSignaling
#define bid64_isSigned  __mongocrypt_bid64_isSigned
#define bid64_isSubnormal  __mongocrypt_bid64_isSubnormal
#define bid64_isZero  __mongocrypt_bid64_isZero
#define bid64_negate  __mongocrypt_bid64_negate
#define bid64_radix  __mongocrypt_bid64_radix
#define bid64_sameQuantum  __mongocrypt_bid64_sameQuantum
#define bid64_totalOrder  __mongocrypt_bid64_totalOrder
#define bid64_totalOrderMag  __mongocrypt_bid64_totalOrderMag
#define bid128_abs  __mongocrypt_bid128_abs
#define bid128_class  __mongocrypt_bid128_class
#define bid128_copy  __mongocrypt_bid128_copy
#define bid128_copySign  __mongocrypt_bid128_copySign
#define bid128_isCanonical  __mongocrypt_bid128_isCanonical
#define bid128_isFinite  __mongocrypt_bid128_isFinite
#define bid128_isInf  __mongocrypt_bid128_isInf
#define bid128_isNaN  __mongocrypt_bid128_isNaN
#define bid128_isNormal  __mongocrypt_bid128_isNormal
#define bid128_isSignaling  __mongocrypt_bid128_isSignaling
#define bid128_isSigned  __mongocrypt_bid128_isSigned
#define bid128_isSubnormal  __mongocrypt_bid128_isSubnormal
#define bid128_isZero  __mongocrypt_bid128_isZero
#define bid128_negate  __mongocrypt_bid128_negate
#define bid128_radix  __mongocrypt_bid128_radix
#define bid128_sameQuantum  __mongocrypt_bid128_sameQuantum
#define bid128_totalOrder  __mongocrypt_bid128_totalOrder
#define bid128_totalOrderMag  __mongocrypt_bid128_totalOrderMag
#define bid64_quiet_equal  __mongocrypt_bid64_quiet_equal
#define bid64_quiet_greater  __mongocrypt_bid64_quiet_greater
#define bid64_quiet_greater_equal  __mongocrypt_bid64_quiet_greater_equal
#define bid64_quiet_greater_unordered  __mongocrypt_bid64_quiet_greater_unordered
#define bid64_quiet_less  __mongocrypt_bid64_quiet_less
#define bid64_quiet_less_equal  __mongocrypt_bid64_quiet_less_equal
#define bid64_quiet_less_unordered  __mongocrypt_bid64_quiet_less_unordered
#define bid64_quiet_not_equal  __mongocrypt_bid64_quiet_not_equal
#define bid64_quiet_not_greater  __mongocrypt_bid64_quiet_not_greater
#define bid64_quiet_not_less  __mongocrypt_bid64_quiet_not_less
#define bid64_quiet_ordered  __mongocrypt_bid64_quiet_ordered
#define bid64_quiet_unordered  __mongocrypt_bid64_quiet_unordered
#define bid64_signaling_greater  __mongocrypt_bid64_signaling_greater
#define bid64_signaling_greater_equal  __mongocrypt_bid64_signaling_greater_equal
#define bid64_signaling_greater_unordered  __mongocrypt_bid64_signaling_greater_unordered
#define bid64_signaling_less  __mongocrypt_bid64_signaling_less
#define bid64_signaling_less_equal  __mongocrypt_bid64_signaling_less_equal
#define bid64_signaling_less_unordered  __mongocrypt_bid64_signaling_less_unordered
#define bid64_signaling_not_greater  __mongocrypt_bid64_signaling_not_greater
#define bid64_signaling_not_less  __mongocrypt_bid64_signaling_not_less
#define bid128_quiet_equal  __mongocrypt_bid128_quiet_equal
#define bid128_quiet_greater  __mongocrypt_bid128_quiet_greater
#define bid128_quiet_greater_equal  __mongocrypt_bid128_quiet_greater_equal
#define bid128_quiet_greater_unordered  __mongocrypt_bid128_quiet_greater_unordered
#define bid128_quiet_less  __mongocrypt_bid128_quiet_less
#define bid128_quiet_less_equal  __mongocrypt_bid128_quiet_less_equal
#define bid128_quiet_less_unordered  __mongocrypt_bid128_quiet_less_unordered
#define bid128_quiet_not_equal  __mongocrypt_bid128_quiet_not_equal
#define bid128_quiet_not_greater  __mongocrypt_bid128_quiet_not_greater
#define bid128_quiet_not_less  __mongocrypt_bid128_quiet_not_less
#define bid128_quiet_ordered  __mongocrypt_bid128_quiet_ordered
#define bid128_quiet_unordered  __mongocrypt_bid128_quiet_unordered
#define bid128_signaling_greater  __mongocrypt_bid128_signaling_greater
#define bid128_signaling_greater_equal  __mongocrypt_bid128_signaling_greater_equal
#define bid128_signaling_greater_unordered  __mongocrypt_bid128_signaling_greater_unordered
#define bid128_signaling_less  __mongocrypt_bid128_signaling_less
#define bid128_signaling_less_equal  __mongocrypt_bid128_signaling_less_equal
#define bid128_signaling_less_unordered  __mongocrypt_bid128_signaling_less_unordered
#define bid128_signaling_not_greater  __mongocrypt_bid128_signaling_not_greater
#define bid128_signaling_not_less  __mongocrypt_bid128_signaling_not_less
#define bid64_to_int32_ceil  __mongocrypt_bid64_to_int32_ceil
#define bid64_to_int32_floor  __mongocrypt_bid64_to_int32_floor
#define bid64_to_int32_int  __mongocrypt_bid64_to_int32_int
#define bid64_to_int32_rnint  __mongocrypt_bid64_to_int32_rnint
#define bid64_to_int32_rninta  __mongocrypt_bid64_to_int32_rninta
#define bid64_to_int32_xceil  __mongocrypt_bid64_to_int32_xceil
#define bid64_to_int32_xfloor  __mongocrypt_bid64_to_int32_xfloor
#define bid64_to_int32_xint  __mongocrypt_bid64_to_int32_xint
#define bid64_to_int32_xrnint  __mongocrypt_bid64_to_int32_xrnint
#define bid64_to_int32_xrninta  __mongocrypt_bid64_to_int32_xrninta
#define bid64_to_uint32_ceil  __mongocrypt_bid64_to_uint32_ceil
#define bid64_to_uint32_floor  __mongocrypt_bid64_to_uint32_floor
#define bid64_to_uint32_int  __mongocrypt_bid64_to_uint32_int
#define bid64_to_uint32_rnint  __mongocrypt_bid64_to_uint32_rnint
#define bid64_to_uint32_rninta  __mongocrypt_bid64_to_uint32_rninta
#define bid64_to_uint32_xceil  __mongocrypt_bid64_to_uint32_xceil
#define bid64_to_uint32_xfloor  __mongocrypt_bid64_to_uint32_xfloor
#define bid64_to_uint32_xint  __mongocrypt_bid64_to_uint32_xint
#define bid64_to_uint32_xrnint  __mongocrypt_bid64_to_uint32_xrnint
#define bid64_to_uint32_xrninta  __mongocrypt_bid64_to_uint32_xrninta
#define bid64_to_int64_ceil  __mongocrypt_bid64_to_int64_ceil
#define bid64_to_int64_floor  __mongocrypt_bid64_to_int64_floor
#define bid64_to_int64_int  __mongocrypt_bid64_to_int64_int
#define bid64_to_int64_rnint  __mongocrypt_bid64_to_int64_rnint
#define bid64_to_int64_rninta  __mongocrypt_bid64_to_int64_rninta
#define bid64_to_int64_xceil  __mongocrypt_bid64_to_int64_xceil
#define bid64_to_int64_xfloor  __mongocrypt_bid64_to_int64_xfloor
#define bid64_to_int64_xint  __mongocrypt_bid64_to_int64_xint
#define bid64_to_int64_xrnint  __mongocrypt_bid64_to_int64_xrnint
#define bid64_to_int64_xrninta  __mongocrypt_bid64_to_int64_xrninta
#define bid64_to_uint64_ceil  __mongocrypt_bid64_to_uint64_ceil
#define bid64_to_uint64_floor  __mongocrypt_bid64_to_uint64_floor
#define bid64_to_uint64_int  __mongocrypt_bid64_to_uint64_int
#define bid64_to_uint64_rnint  __mongocrypt_bid64_to_uint64_rnint
#define bid64_to_uint64_rninta  __mongocrypt_bid64_to_uint64_rninta
#define bid64_to_uint64_xceil  __mongocrypt_bid64_to_uint64_xceil
#define bid64_to_uint64_xfloor  __mongocrypt_bid64_to_uint64_xfloor
#define bid64_to_uint64_xint  __mongocrypt_bid64_to_uint64_xint
#define bid64_to_uint64_xrnint  __mongocrypt_bid64_to_uint64_xrnint
#define bid64_to_uint64_xrninta  __mongocrypt_bid64_to_uint64_xrninta
#define bid128_to_int32_ceil  __mongocrypt_bid128_to_int32_ceil
#define bid128_to_int32_floor  __mongocrypt_bid128_to_int32_floor
#define bid128_to_int32_int  __mongocrypt_bid128_to_int32_int
#define bid128_to_int32_rnint  __mongocrypt_bid128_to_int32_rnint
#define bid128_to_int32_rninta  __mongocrypt_bid128_to_int32_rninta
#define bid128_to_int32_xceil  __mongocrypt_bid128_to_int32_xceil
#define bid128_to_int32_xfloor  __mongocrypt_bid128_to_int32_xfloor
#define bid128_to_int32_xint  __mongocrypt_bid128_to_int32_xint
#define bid128_to_int32_xrnint  __mongocrypt_bid128_to_int32_xrnint
#define bid128_to_int32_xrninta  __mongocrypt_bid128_to_int32_xrninta
#define bid128_to_uint32_ceil  __mongocrypt_bid128_to_uint32_ceil
#define bid128_to_uint32_floor  __mongocrypt_bid128_to_uint32_floor
#define bid128_to_uint32_int  __mongocrypt_bid128_to_uint32_int
#define bid128_to_uint32_rnint  __mongocrypt_bid128_to_uint32_rnint
#define bid128_to_uint32_rninta  __mongocrypt_bid128_to_uint32_rninta
#define bid128_to_uint32_xceil  __mongocrypt_bid128_to_uint32_xceil
#define bid128_to_uint32_xfloor  __mongocrypt_bid128_to_uint32_xfloor
#define bid128_to_uint32_xint  __mongocrypt_bid128_to_uint32_xint
#define bid128_to_uint32_xrnint  __mongocrypt_bid128_to_uint32_xrnint
#define bid128_to_uint32_xrninta  __mongocrypt_bid128_to_uint32_xrninta
#define bid128_to_int64_ceil  __mongocrypt_bid128_to_int64_ceil
#define bid128_to_int64_floor  __mongocrypt_bid128_to_int64_floor
#define bid128_to_int64_int  __mongocrypt_bid128_to_int64_int
#define bid128_to_int64_rnint  __mongocrypt_bid128_to_int64_rnint
#define bid128_to_int64_rninta  __mongocrypt_bid128_to_int64_rninta
#define bid128_to_int64_xceil  __mongocrypt_bid128_to_int64_xceil
#define bid128_to_int64_xfloor  __mongocrypt_bid128_to_int64_xfloor
#define bid128_to_int64_xint  __mongocrypt_bid128_to_int64_xint
#define bid128_to_int64_xrnint  __mongocrypt_bid128_to_int64_xrnint
#define bid128_to_int64_xrninta  __mongocrypt_bid128_to_int64_xrninta
#define bid128_to_uint64_ceil  __mongocrypt_bid128_to_uint64_ceil
#define bid128_to_uint64_floor  __mongocrypt_bid128_to_uint64_floor
#define bid128_to_uint64_int  __mongocrypt_bid128_to_uint64_int
#define bid128_to_uint64_rnint  __mongocrypt_bid128_to_uint64_rnint
#define bid128_to_uint64_rninta  __mongocrypt_bid128_to_uint64_rninta
#define bid128_to_uint64_xceil  __mongocrypt_bid128_to_uint64_xceil
#define bid128_to_uint64_xfloor  __mongocrypt_bid128_to_uint64_xfloor
#define bid128_to_uint64_xint  __mongocrypt_bid128_to_uint64_xint
#define bid128_to_uint64_xrnint  __mongocrypt_bid128_to_uint64_xrnint
#define bid128_to_uint64_xrninta  __mongocrypt_bid128_to_uint64_xrninta
#define bid128_to_binary128  __mongocrypt_bid128_to_binary128
#define bid128_to_binary32  __mongocrypt_bid128_to_binary32
#define bid128_to_binary64  __mongocrypt_bid128_to_binary64
#define bid128_to_binary80  __mongocrypt_bid128_to_binary80
#define bid32_to_binary128  __mongocrypt_bid32_to_binary128
#define bid32_to_binary32  __mongocrypt_bid32_to_binary32
#define bid32_to_binary64  __mongocrypt_bid32_to_binary64
#define bid32_to_binary80  __mongocrypt_bid32_to_binary80
#define bid64_to_binary128  __mongocrypt_bid64_to_binary128
#define bid64_to_binary32  __mongocrypt_bid64_to_binary32
#define bid64_to_binary64  __mongocrypt_bid64_to_binary64
#define bid64_to_binary80  __mongocrypt_bid64_to_binary80
#define binary128_to_bid128  __mongocrypt_binary128_to_bid128
#define binary128_to_bid32  __mongocrypt_binary128_to_bid32
#define binary128_to_bid64  __mongocrypt_binary128_to_bid64
#define binary32_to_bid128  __mongocrypt_binary32_to_bid128
#define binary32_to_bid32  __mongocrypt_binary32_to_bid32
#define binary32_to_bid64  __mongocrypt_binary32_to_bid64
#define binary64_to_bid128  __mongocrypt_binary64_to_bid128
#define binary64_to_bid32  __mongocrypt_binary64_to_bid32
#define binary64_to_bid64  __mongocrypt_binary64_to_bid64
#define binary80_to_bid128  __mongocrypt_binary80_to_bid128
#define binary80_to_bid32  __mongocrypt_binary80_to_bid32
#define binary80_to_bid64  __mongocrypt_binary80_to_bid64
#define bid64_to_uint16_ceil  __mongocrypt_bid64_to_uint16_ceil
#define bid64_to_uint16_floor  __mongocrypt_bid64_to_uint16_floor
#define bid64_to_uint16_int  __mongocrypt_bid64_to_uint16_int
#define bid64_to_uint16_rnint  __mongocrypt_bid64_to_uint16_rnint
#define bid64_to_uint16_rninta  __mongocrypt_bid64_to_uint16_rninta
#define bid64_to_uint16_xceil  __mongocrypt_bid64_to_uint16_xceil
#define bid64_to_uint16_xfloor  __mongocrypt_bid64_to_uint16_xfloor
#define bid64_to_uint16_xint  __mongocrypt_bid64_to_uint16_xint
#define bid64_to_uint16_xrnint  __mongocrypt_bid64_to_uint16_xrnint
#define bid64_to_uint16_xrninta  __mongocrypt_bid64_to_uint16_xrninta
#define bid64_to_int16_ceil  __mongocrypt_bid64_to_int16_ceil
#define bid64_to_int16_floor  __mongocrypt_bid64_to_int16_floor
#define bid64_to_int16_int  __mongocrypt_bid64_to_int16_int
#define bid64_to_int16_rnint  __mongocrypt_bid64_to_int16_rnint
#define bid64_to_int16_rninta  __mongocrypt_bid64_to_int16_rninta
#define bid64_to_int16_xceil  __mongocrypt_bid64_to_int16_xceil
#define bid64_to_int16_xfloor  __mongocrypt_bid64_to_int16_xfloor
#define bid64_to_int16_xint  __mongocrypt_bid64_to_int16_xint
#define bid64_to_int16_xrnint  __mongocrypt_bid64_to_int16_xrnint
#define bid64_to_int16_xrninta  __mongocrypt_bid64_to_int16_xrninta
#define bid128_to_uint16_ceil  __mongocrypt_bid128_to_uint16_ceil
#define bid128_to_uint16_floor  __mongocrypt_bid128_to_uint16_floor
#define bid128_to_uint16_int  __mongocrypt_bid128_to_uint16_int
#define bid128_to_uint16_rnint  __mongocrypt_bid128_to_uint16_rnint
#define bid128_to_uint16_rninta  __mongocrypt_bid128_to_uint16_rninta
#define bid128_to_uint16_xceil  __mongocrypt_bid128_to_uint16_xceil
#define bid128_to_uint16_xfloor  __mongocrypt_bid128_to_uint16_xfloor
#define bid128_to_uint16_xint  __mongocrypt_bid128_to_uint16_xint
#define bid128_to_uint16_xrnint  __mongocrypt_bid128_to_uint16_xrnint
#define bid128_to_uint16_xrninta  __mongocrypt_bid128_to_uint16_xrninta
#define bid128_to_int16_ceil  __mongocrypt_bid128_to_int16_ceil
#define bid128_to_int16_floor  __mongocrypt_bid128_to_int16_floor
#define bid128_to_int16_int  __mongocrypt_bid128_to_int16_int
#define bid128_to_int16_rnint  __mongocrypt_bid128_to_int16_rnint
#define bid128_to_int16_rninta  __mongocrypt_bid128_to_int16_rninta
#define bid128_to_int16_xceil  __mongocrypt_bid128_to_int16_xceil
#define bid128_to_int16_xfloor  __mongocrypt_bid128_to_int16_xfloor
#define bid128_to_int16_xint  __mongocrypt_bid128_to_int16_xint
#define bid128_to_int16_xrnint  __mongocrypt_bid128_to_int16_xrnint
#define bid128_to_int16_xrninta  __mongocrypt_bid128_to_int16_xrninta
#define bid64_to_uint8_ceil  __mongocrypt_bid64_to_uint8_ceil
#define bid64_to_uint8_floor  __mongocrypt_bid64_to_uint8_floor
#define bid64_to_uint8_int  __mongocrypt_bid64_to_uint8_int
#define bid64_to_uint8_rnint  __mongocrypt_bid64_to_uint8_rnint
#define bid64_to_uint8_rninta  __mongocrypt_bid64_to_uint8_rninta
#define bid64_to_uint8_xceil  __mongocrypt_bid64_to_uint8_xceil
#define bid64_to_uint8_xfloor  __mongocrypt_bid64_to_uint8_xfloor
#define bid64_to_uint8_xint  __mongocrypt_bid64_to_uint8_xint
#define bid64_to_uint8_xrnint  __mongocrypt_bid64_to_uint8_xrnint
#define bid64_to_uint8_xrninta  __mongocrypt_bid64_to_uint8_xrninta
#define bid64_to_int8_ceil  __mongocrypt_bid64_to_int8_ceil
#define bid64_to_int8_floor  __mongocrypt_bid64_to_int8_floor
#define bid64_to_int8_int  __mongocrypt_bid64_to_int8_int
#define bid64_to_int8_rnint  __mongocrypt_bid64_to_int8_rnint
#define bid64_to_int8_rninta  __mongocrypt_bid64_to_int8_rninta
#define bid64_to_int8_xceil  __mongocrypt_bid64_to_int8_xceil
#define bid64_to_int8_xfloor  __mongocrypt_bid64_to_int8_xfloor
#define bid64_to_int8_xint  __mongocrypt_bid64_to_int8_xint
#define bid64_to_int8_xrnint  __mongocrypt_bid64_to_int8_xrnint
#define bid64_to_int8_xrninta  __mongocrypt_bid64_to_int8_xrninta
#define bid128_to_uint8_ceil  __mongocrypt_bid128_to_uint8_ceil
#define bid128_to_uint8_floor  __mongocrypt_bid128_to_uint8_floor
#define bid128_to_uint8_int  __mongocrypt_bid128_to_uint8_int
#define bid128_to_uint8_rnint  __mongocrypt_bid128_to_uint8_rnint
#define bid128_to_uint8_rninta  __mongocrypt_bid128_to_uint8_rninta
#define bid128_to_uint8_xceil  __mongocrypt_bid128_to_uint8_xceil
#define bid128_to_uint8_xfloor  __mongocrypt_bid128_to_uint8_xfloor
#define bid128_to_uint8_xint  __mongocrypt_bid128_to_uint8_xint
#define bid128_to_uint8_xrnint  __mongocrypt_bid128_to_uint8_xrnint
#define bid128_to_uint8_xrninta  __mongocrypt_bid128_to_uint8_xrninta
#define bid128_to_int8_ceil  __mongocrypt_bid128_to_int8_ceil
#define bid128_to_int8_floor  __mongocrypt_bid128_to_int8_floor
#define bid128_to_int8_int  __mongocrypt_bid128_to_int8_int
#define bid128_to_int8_rnint  __mongocrypt_bid128_to_int8_rnint
#define bid128_to_int8_rninta  __mongocrypt_bid128_to_int8_rninta
#define bid128_to_int8_xceil  __mongocrypt_bid128_to_int8_xceil
#define bid128_to_int8_xfloor  __mongocrypt_bid128_to_int8_xfloor
#define bid128_to_int8_xint  __mongocrypt_bid128_to_int8_xint
#define bid128_to_int8_xrnint  __mongocrypt_bid128_to_int8_xrnint
#define bid128_to_int8_xrninta  __mongocrypt_bid128_to_int8_xrninta

#define bid32_inf  __mongocrypt_bid32_inf
#define bid64_inf  __mongocrypt_bid64_inf
#define bid128_inf  __mongocrypt_bid128_inf

#define bid_feclearexcept  __mongocrypt_bid_feclearexcept
#define bid_fegetexceptflag  __mongocrypt_bid_fegetexceptflag
#define bid_feraiseexcept  __mongocrypt_bid_feraiseexcept
#define bid_fesetexceptflag  __mongocrypt_bid_fesetexceptflag
#define bid_fetestexcept  __mongocrypt_bid_fetestexcept

#define bid_strtod128  __mongocrypt_bid_strtod128
#define bid_strtod64   __mongocrypt_bid_strtod64
#define bid_strtod32   __mongocrypt_bid_strtod32
#define bid_wcstod128  __mongocrypt_bid_wcstod128
#define bid_wcstod64   __mongocrypt_bid_wcstod64
#define bid_wcstod32   __mongocrypt_bid_wcstod32



///////////////////////////////////////////////////////////////
#ifdef IN_LIBGCC2
#if !defined ENABLE_DECIMAL_BID_FORMAT || !ENABLE_DECIMAL_BID_FORMAT
#error BID not enabled in libbid
#endif

#ifndef BID_BIG_ENDIAN
#define BID_BIG_ENDIAN LIBGCC2_FLOAT_WORDS_BIG_ENDIAN
#endif

#ifndef BID_THREAD
#if defined (HAVE_CC_TLS) && defined (USE_TLS)
#define BID_THREAD __thread
#endif
#endif

#define BID__intptr_t_defined
#define DECIMAL_CALL_BY_REFERENCE 0
#define DECIMAL_GLOBAL_ROUNDING 1
#define DECIMAL_GLOBAL_ROUNDING_ACCESS_FUNCTIONS 1
#define DECIMAL_GLOBAL_EXCEPTION_FLAGS 1
#define DECIMAL_GLOBAL_EXCEPTION_FLAGS_ACCESS_FUNCTIONS 1
#define BID_HAS_GCC_DECIMAL_INTRINSICS 1
#endif /* IN_LIBGCC2 */

// Configuration Options

#define DECIMAL_TINY_DETECTION_AFTER_ROUNDING 0
#define BINARY_TINY_DETECTION_AFTER_ROUNDING 1

#define BID_SET_STATUS_FLAGS

#ifndef BID_THREAD
#if defined (_MSC_VER) //Windows
#define BID_THREAD __declspec(thread)
#else
#if !defined(__APPLE__) //Linux, FreeBSD
#define BID_THREAD __thread
#else //Mac OSX, TBD
#define BID_THREAD
#endif //Linux or Mac
#endif //Windows
#endif //BID_THREAD


#ifndef BID_HAS_GCC_DECIMAL_INTRINSICS
#define BID_HAS_GCC_DECIMAL_INTRINSICS 0
#endif

// set sizeof (long) here, for bid32_lrint(), bid64_lrint(), bid128_lrint(),
// and for bid32_lround(), bid64_lround(), bid128_lround()
#ifndef BID_SIZE_LONG
#if defined(WINDOWS)
#define BID_SIZE_LONG 4
#else
#if defined(__x86_64__) || defined (__ia64__)  || defined(HPUX_OS_64)
#define BID_SIZE_LONG 8
#else
#define BID_SIZE_LONG 4
#endif
#endif
#endif

#if !defined(WINDOWS) || defined(__INTEL_COMPILER)
// #define UNCHANGED_BINARY_STATUS_FLAGS
#endif
// #define HPUX_OS

// If DECIMAL_CALL_BY_REFERENCE is defined then numerical arguments and results
// are passed by reference otherwise they are passed by value (except that
// a pointer is always passed to the status flags)

#ifndef DECIMAL_CALL_BY_REFERENCE
#define DECIMAL_CALL_BY_REFERENCE 0
#endif

// If DECIMAL_GLOBAL_ROUNDING is defined then the rounding mode is a global
// variable _IDEC_glbround, otherwise it is passed as a parameter when needed

#ifndef DECIMAL_GLOBAL_ROUNDING
#define DECIMAL_GLOBAL_ROUNDING 0
#endif

#ifndef DECIMAL_GLOBAL_ROUNDING_ACCESS_FUNCTIONS
#define DECIMAL_GLOBAL_ROUNDING_ACCESS_FUNCTIONS 0
#endif

// If DECIMAL_GLOBAL_EXCEPTION_FLAGS is defined then the exception status flags
// are represented by a global variable _IDEC_glbflags, otherwise they are
// passed as a parameter when needed

#ifndef DECIMAL_GLOBAL_EXCEPTION_FLAGS
#define DECIMAL_GLOBAL_EXCEPTION_FLAGS 0
#endif

#ifndef DECIMAL_GLOBAL_EXCEPTION_FLAGS_ACCESS_FUNCTIONS
#define DECIMAL_GLOBAL_EXCEPTION_FLAGS_ACCESS_FUNCTIONS 0
#endif

// If DECIMAL_ALTERNATE_EXCEPTION_HANDLING is defined then the exception masks
// are examined and exception handling information is provided to the caller
// if alternate exception handling is necessary

#ifndef DECIMAL_ALTERNATE_EXCEPTION_HANDLING
#define DECIMAL_ALTERNATE_EXCEPTION_HANDLING 0
#endif

typedef unsigned int _IDEC_round;
typedef unsigned int _IDEC_flags;       // could be a struct with diagnostic info

#if DECIMAL_ALTERNATE_EXCEPTION_HANDLING
  // If DECIMAL_GLOBAL_EXCEPTION_MASKS is defined then the exception mask bits
  // are represented by a global variable _IDEC_exceptionmasks, otherwise they
  // are passed as a parameter when needed; DECIMAL_GLOBAL_EXCEPTION_MASKS is
  // ignored
  // if DECIMAL_ALTERNATE_EXCEPTION_HANDLING is not defined
  // **************************************************************************
#define DECIMAL_GLOBAL_EXCEPTION_MASKS 0
  // **************************************************************************

  // If DECIMAL_GLOBAL_EXCEPTION_INFO is defined then the alternate exception
  // handling information is represented by a global data structure
  // _IDEC_glbexcepthandling, otherwise it is passed by reference as a
  // parameter when needed; DECIMAL_GLOBAL_EXCEPTION_INFO is ignored
  // if DECIMAL_ALTERNATE_EXCEPTION_HANDLING is not defined
  // **************************************************************************
#define DECIMAL_GLOBAL_EXCEPTION_INFO 0
  // **************************************************************************
#endif

// Notes: 1) rnd_mode from _RND_MODE_ARG is used by the caller of a function
//           from this library, and can be any name
//        2) rnd_mode and prnd_mode from _RND_MODE_PARAM are fixed names
//           and *must* be used in the library functions
//        3) _IDEC_glbround is the fixed name for the global variable holding
//           the rounding mode

#if !DECIMAL_GLOBAL_ROUNDING
#if DECIMAL_CALL_BY_REFERENCE
#define _RND_MODE_ARG , &rnd_mode
#define _RND_MODE_PARAM , _IDEC_round *prnd_mode
#define _RND_MODE_PARAM_0 _IDEC_round *prnd_mode
#define _RND_MODE_ARG_ALONE &rnd_mode
#define _RND_MODE_PARAM_ALONE _IDEC_round *prnd_mode
#else
#define _RND_MODE_ARG , rnd_mode
#define _RND_MODE_PARAM , _IDEC_round rnd_mode
#define _RND_MODE_PARAM_0 _IDEC_round rnd_mode
#define _RND_MODE_ARG_ALONE rnd_mode
#define _RND_MODE_PARAM_ALONE _IDEC_round rnd_mode
#endif
#else
#define _RND_MODE_ARG
#define _RND_MODE_PARAM
#define _RND_MODE_ARG_ALONE
#define _RND_MODE_PARAM_ALONE
#define rnd_mode _IDEC_glbround
#endif

// Notes: 1) pfpsf from _EXC_FLAGS_ARG is used by the caller of a function
//           from this library, and can be any name
//        2) pfpsf from _EXC_FLAGS_PARAM is a fixed name and *must* be used
//           in the library functions
//        3) _IDEC_glbflags is the fixed name for the global variable holding
//           the floating-point status flags
#if !DECIMAL_GLOBAL_EXCEPTION_FLAGS
#define _EXC_FLAGS_ARG , pfpsf
#define _EXC_FLAGS_PARAM , _IDEC_flags *pfpsf
#else
#define _EXC_FLAGS_ARG
#define _EXC_FLAGS_PARAM
#define pfpsf &_IDEC_glbflags
#endif

#if DECIMAL_GLOBAL_ROUNDING
BID_EXTERN_C BID_THREAD _IDEC_round _IDEC_glbround;
#endif

#if DECIMAL_GLOBAL_EXCEPTION_FLAGS
BID_EXTERN_C BID_THREAD _IDEC_flags _IDEC_glbflags;
#endif

#if DECIMAL_ALTERNATE_EXCEPTION_HANDLING
#if DECIMAL_GLOBAL_EXCEPTION_MASKS
BID_EXTERN_C BID_THREAD _IDEC_exceptionmasks _IDEC_glbexceptionmasks;
#endif
#if DECIMAL_GLOBAL_EXCEPTION_INFO
BID_EXTERN_C BID_THREAD _IDEC_excepthandling _IDEC_glbexcepthandling;
#endif
#endif

#if DECIMAL_ALTERNATE_EXCEPTION_HANDLING

  // Notes: 1) exc_mask from _EXC_MASKS_ARG is used by the caller of a function
  //           from this library, and can be any name
  //        2) exc_mask and pexc_mask from _EXC_MASKS_PARAM are fixed names
  //           and *must* be used in the library functions
  //        3) _IDEC_glbexceptionmasks is the fixed name for the global
  //           variable holding the floating-point exception masks
#if !DECIMAL_GLOBAL_EXCEPTION_MASKS
#if DECIMAL_CALL_BY_REFERENCE
#define _EXC_MASKS_ARG , &exc_mask
#define _EXC_MASKS_PARAM , _IDEC_exceptionmasks *pexc_mask
#else
#define _EXC_MASKS_ARG , exc_mask
#define _EXC_MASKS_PARAM , _IDEC_exceptionmasks exc_mask
#endif
#else
#define _EXC_MASKS_ARG
#define _EXC_MASKS_PARAM
#define exc_mask _IDEC_glbexceptionmasks
#endif

  // Notes: 1) BID_pexc_info from _EXC_INFO_ARG is used by the caller of a function
  //           from this library, and can be any name
  //        2) BID_pexc_info from _EXC_INFO_PARAM is a fixed name and *must* be
  //           used in the library functions
  //        3) _IDEC_glbexcepthandling is the fixed name for the global
  //           variable holding the floating-point exception information
#if !DECIMAL_GLOBAL_EXCEPTION_INFO
#define _EXC_INFO_ARG , BID_pexc_info
#define _EXC_INFO_PARAM , _IDEC_excepthandling *BID_pexc_info
#else
#define _EXC_INFO_ARG
#define _EXC_INFO_PARAM
#define BID_pexc_info &_IDEC_glbexcepthandling
#endif
#else
#define _EXC_MASKS_ARG
#define _EXC_MASKS_PARAM
#define _EXC_INFO_ARG
#define _EXC_INFO_PARAM
#endif

#ifndef BID_BIG_ENDIAN
#define BID_BIG_ENDIAN 0
#endif

#if BID_BIG_ENDIAN
#define BID_SWAP128(x) {  \
  BID_UINT64 sw;              \
  sw = (x).w[1];          \
  (x).w[1] = (x).w[0];    \
  (x).w[0] = sw;          \
  }
#else
#define BID_SWAP128(x)
#endif

#if DECIMAL_CALL_BY_REFERENCE
#define BID_RETURN_VAL(x) { BID_OPT_RESTORE_BINARY_FLAGS()  *pres = (x); return; }
#if BID_BIG_ENDIAN && defined BID_128RES
#define BID_RETURN(x) { BID_OPT_RESTORE_BINARY_FLAGS()  BID_SWAP128(x); *pres = (x); return; }
#define BID_RETURN_NOFLAGS(x) { BID_SWAP128(x); *pres = (x); return; }
#else
#define BID_RETURN(x) { BID_OPT_RESTORE_BINARY_FLAGS()  *pres = (x); return; }
#define BID_RETURN_NOFLAGS(x) { *pres = (x); return; }
#endif
#else
#define BID_RETURN_VAL(x) { BID_OPT_RESTORE_BINARY_FLAGS()  return(x); }
#if BID_BIG_ENDIAN && defined BID_128RES
#define BID_RETURN(x) { BID_OPT_RESTORE_BINARY_FLAGS()  BID_SWAP128(x); return(x); }
#define BID_RETURN_NOFLAGS(x) { BID_SWAP128(x); return(x); }
#else
#define BID_RETURN(x) { BID_OPT_RESTORE_BINARY_FLAGS()  return(x); }
#define BID_RETURN_NOFLAGS(x) { return(x); }
#endif
#endif

#if DECIMAL_CALL_BY_REFERENCE
#define BIDECIMAL_CALL1(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), &(_OP1) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), &(_OP1) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2(_FUNC, _RES, _OP1, _OP2) \
    _FUNC(&(_RES), &(_OP1), &(_OP2) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2_YPTR_NORND(_FUNC, _RES, _OP1, _OP2) \
    _FUNC(&(_RES), &(_OP1), &(_OP2) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2_NORND(_FUNC, _RES, _OP1, _OP2) \
    _FUNC(&(_RES), &(_OP1), &(_OP2) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND_RESREF(_FUNC, _RES, _OP1) \
    _FUNC((_RES), &(_OP1) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_RESARG(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), (_OP1) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_RESREF(_FUNC, _RES, _OP1) \
    _FUNC((_RES), &(_OP1) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND_NOSTAT(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), &(_OP1) _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2_NORND_NOSTAT(_FUNC, _RES, _OP1, _OP2) \
    _FUNC(&(_RES), &(_OP1), &(_OP2) _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL3(_FUNC, _RES, _OP1, _OP2, _OP3) \
    _FUNC(&(_RES), &(_OP1), &(_OP2), &(_OP3) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), &(_OP1) _EXC_FLAGS_ARG )
#define BIDECIMAL_CALL1_NORND_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), &(_OP1) )
#define BIDECIMAL_CALL1_NORND_NOFLAGS_NOMASK_NOINFO_ARGREF(_FUNC, _RES, _OP1) \
    _FUNC(&(_RES), (_OP1) )
#define BIDECIMAL_CALL2_NORND_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1, _OP2) \
    _FUNC(&(_RES), &(_OP1), &(_OP2) )
#define BIDECIMAL_CALL2_NORND_NOFLAGS_NOMASK_NOINFO_ARG2REF(_FUNC, _RES, _OP1, _OP2) \
    _FUNC(&(_RES), &(_OP1), (_OP2) )
#define BIDECIMAL_CALL1_NORND_NOMASK_NOINFO_RESVOID(_FUNC, _OP1) \
    _FUNC(&(_OP1) _EXC_FLAGS_ARG )
#define BIDECIMAL_CALL2_NORND_NOMASK_NOINFO_RESVOID(_FUNC, _OP1, _OP2) \
    _FUNC(&(_OP1), &(_OP2) _EXC_FLAGS_ARG )
#define BIDECIMAL_CALLV_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES) \
    _FUNC(&(_RES) _RND_MODE_ARG)
#define BIDECIMAL_CALL1_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _FUNC(&(_OP1) _RND_MODE_ARG)
#define BIDECIMAL_CALLV_EMPTY(_FUNC, _RES) \
    _FUNC(&(_RES))
#else
#define BIDECIMAL_CALL1(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2(_FUNC, _RES, _OP1, _OP2) \
    _RES = _FUNC((_OP1), (_OP2) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2_YPTR_NORND(_FUNC, _RES, _OP1, _OP2) \
    _RES = _FUNC((_OP1), &(_OP2) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2_NORND(_FUNC, _RES, _OP1, _OP2) \
    _RES = _FUNC((_OP1), (_OP2) _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND_RESREF(_FUNC, _RES, _OP1) \
    _FUNC((_RES), _OP1 _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_RESARG(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_RESREF(_FUNC, _RES, _OP1) \
    _FUNC((_RES), _OP1 _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND_NOSTAT(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL2_NORND_NOSTAT(_FUNC, _RES, _OP1, _OP2) \
    _RES = _FUNC((_OP1), (_OP2) _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL3(_FUNC, _RES, _OP1, _OP2, _OP3) \
    _RES = _FUNC((_OP1), (_OP2), (_OP3) _RND_MODE_ARG _EXC_FLAGS_ARG _EXC_MASKS_ARG _EXC_INFO_ARG)
#define BIDECIMAL_CALL1_NORND_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) _EXC_FLAGS_ARG)
#define BIDECIMAL_CALL1_NORND_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) )
#define BIDECIMAL_CALL1_NORND_NOFLAGS_NOMASK_NOINFO_ARGREF(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) )
#define BIDECIMAL_CALL2_NORND_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1, _OP2) \
    _RES = _FUNC((_OP1), (_OP2) )
#define BIDECIMAL_CALL2_NORND_NOFLAGS_NOMASK_NOINFO_ARG2REF(_FUNC, _RES, _OP1, _OP2) \
    _RES = _FUNC((_OP1), (_OP2) )
#define BIDECIMAL_CALL1_NORND_NOMASK_NOINFO_RESVOID(_FUNC, _OP1) \
    _FUNC((_OP1) _EXC_FLAGS_ARG)
#define BIDECIMAL_CALL2_NORND_NOMASK_NOINFO_RESVOID(_FUNC, _OP1, _OP2) \
    _FUNC((_OP1), (_OP2) _EXC_FLAGS_ARG)
#define BIDECIMAL_CALLV_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES) \
    _RES = _FUNC(_RND_MODE_ARG_ALONE)
#if !DECIMAL_GLOBAL_ROUNDING
#define BIDECIMAL_CALL1_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _RES = _FUNC((_OP1) _RND_MODE_ARG)
#else
#define BIDECIMAL_CALL1_NOFLAGS_NOMASK_NOINFO(_FUNC, _RES, _OP1) \
    _FUNC((_OP1) _RND_MODE_ARG)
#endif
#define BIDECIMAL_CALLV_EMPTY(_FUNC, _RES) \
    _RES=_FUNC()
#endif



///////////////////////////////////////////////////////////////////////////
//
//  Wrapper macros for ICL
//
///////////////////////////////////////////////////////////////////////////

#if defined (__INTEL_COMPILER) && (__DFP_WRAPPERS_ON) && (!DECIMAL_CALL_BY_REFERENCE) && (DECIMAL_GLOBAL_ROUNDING) && (DECIMAL_GLOBAL_EXCEPTION_FLAGS)

#include "bid_wrap_names.h"

#define DECLSPEC_OPT __declspec(noinline)

#define bit_size_BID_UINT128 128
#define bit_size_BID_UINT64 64
#define bit_size_BID_SINT64 64
#define bit_size_BID_UINT32 32
#define bit_size_BID_SINT32 32

#define bidsize(x) bit_size_##x

#define form_type(type, size)  type##size

#define wrapper_name(x)  __wrap_##x

#define DFP_WRAPFN_OTHERTYPE(rsize, fn_name, othertype)\
	form_type(_Decimal,rsize) wrapper_name(fn_name) (othertype __wraparg1)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
   \
   r.i = fn_name(__wraparg1);\
   return r.d;\
}

#define DFP_WRAPFN_DFP(rsize, fn_name, isize1)\
	form_type(_Decimal,rsize) wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
   \
   \
   r.i = fn_name(in1.i);\
   \
   return r.d;\
}

#define DFP_WRAPFN_DFP_DFP(rsize, fn_name, isize1, isize2)\
form_type(_Decimal, rsize) wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1, form_type(_Decimal, isize2) __wraparg2)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
      \
union {\
   form_type(_Decimal, isize2) d;\
   form_type(BID_UINT, isize2) i;\
   } in2 = { __wraparg2};\
   \
   \
   r.i = fn_name(in1.i, in2.i);\
   \
   return r.d;\
}

#define DFP_WRAPFN_DFP_DFP_POINTER(rsize, fn_name, isize1, isize2)\
form_type(_Decimal, rsize) wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1, form_type(_Decimal, isize2) *__wraparg2)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
      \
union {\
   form_type(_Decimal, isize2) d;\
   form_type(BID_UINT, isize2) i;\
   } out2;\
   \
   \
   r.i = fn_name(in1.i, &out2.i);   *__wraparg2 = out2.d;\
   \
   return r.d;\
}

#define DFP_WRAPFN_DFP_DFP_DFP(rsize, fn_name, isize1, isize2, isize3)\
form_type(_Decimal, rsize) wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1, form_type(_Decimal, isize2) __wraparg2, form_type(_Decimal, isize3) __wraparg3)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
      \
union {\
   form_type(_Decimal, isize2) d;\
   form_type(BID_UINT, isize2) i;\
   } in2 = { __wraparg2};\
   \
union {\
   form_type(_Decimal, isize3) d;\
   form_type(BID_UINT, isize3) i;\
   } in3 = { __wraparg3};\
   \
   \
   r.i = fn_name(in1.i, in2.i, in3.i);\
   \
   return r.d;\
}

#define RES_WRAPFN_DFP(restype, fn_name, isize1)\
restype wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1)\
{\
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
   \
   \
   return fn_name(in1.i);\
}

#define RES_WRAPFN_DFP_DFP(restype, fn_name, isize1, isize2)\
restype wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1, form_type(_Decimal, isize2) __wraparg2)\
{\
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
      \
union {\
   form_type(_Decimal, isize2) d;\
   form_type(BID_UINT, isize2) i;\
   } in2 = { __wraparg2};\
   \
   \
   return fn_name(in1.i, in2.i);\
}

#define DFP_WRAPFN_DFP_OTHERTYPE(rsize, fn_name, isize1, othertype)\
form_type(_Decimal, rsize) wrapper_name(fn_name) (form_type(_Decimal, isize1) __wraparg1, othertype __wraparg2)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
   \
   \
   r.i = fn_name(in1.i, __wraparg2);\
   \
   return r.d;\
}

#define VOID_WRAPFN_OTHERTYPERES_DFP(fn_name, othertype, isize1)\
void wrapper_name(fn_name) (othertype *__wrapres, form_type(_Decimal, isize1) __wraparg1)\
{\
union {\
   form_type(_Decimal, isize1) d;\
   form_type(BID_UINT, isize1) i;\
   } in1 = { __wraparg1};\
   \
   \
   fn_name(__wrapres, in1.i);\
   \
}

#define DFP_WRAPFN_TYPE1_TYPE2(rsize, fn_name, type1, type2)\
form_type(_Decimal, rsize) wrapper_name(fn_name) (type1 __wraparg1, type2 __wraparg2)\
{\
union {\
   form_type(_Decimal, rsize) d;\
   form_type(BID_UINT, rsize) i;\
   } r;\
   \
   \
   r.i = fn_name(__wraparg1, __wraparg2);\
   \
   return r.d;\
}

#else

#define DECLSPEC_OPT

#define DFP_WRAPFN_OTHERTYPE(rsize, fn_name, othertype)
#define DFP_WRAPFN_DFP(rsize, fn_name, isize1)
#define DFP_WRAPFN_DFP_DFP(rsize, fn_name, isize1, isize2)
#define RES_WRAPFN_DFP(rsize, fn_name, isize1)
#define RES_WRAPFN_DFP_DFP(rsize, fn_name, isize1, isize2)
#define DFP_WRAPFN_DFP_DFP_DFP(rsize, fn_name, isize1, isize2, isize3)
#define DFP_WRAPFN_DFP_OTHERTYPE(rsize, fn_name, isize1, othertype)
#define DFP_WRAPFN_DFP_DFP_POINTER(rsize, fn_name, isize1, isize2)
#define VOID_WRAPFN_OTHERTYPERES_DFP(fn_name, othertype, isize1)
#define DFP_WRAPFN_TYPE1_TYPE2(rsize, fn_name, type1, type2)

#endif


///////////////////////////////////////////////////////////////////////////

#if BID_BIG_ENDIAN
#define BID_HIGH_128W 0
#define BID_LOW_128W  1
#else
#define BID_HIGH_128W 1
#define BID_LOW_128W  0
#endif

#if (BID_BIG_ENDIAN) && defined(BID_128RES)
#define BID_COPY_ARG_REF(arg_name) \
       BID_UINT128 arg_name={{ pbid_##arg_name->w[1], pbid_##arg_name->w[0]}};
#define BID_COPY_ARG_VAL(arg_name) \
       BID_UINT128 arg_name={{ bid_##arg_name.w[1], bid_##arg_name.w[0]}};
#else
#define BID_COPY_ARG_REF(arg_name) \
       BID_UINT128 arg_name=*pbid_##arg_name;
#define BID_COPY_ARG_VAL(arg_name) \
       BID_UINT128 arg_name= bid_##arg_name;
#endif

#define BID_COPY_ARG_TYPE_REF(type, arg_name) \
       type arg_name=*pbid_##arg_name;
#define BID_COPY_ARG_TYPE_VAL(type, arg_name) \
       type arg_name= bid_##arg_name;

#if !DECIMAL_GLOBAL_ROUNDING
#define BID_SET_RND_MODE() \
  _IDEC_round rnd_mode = *prnd_mode;
#else
#define BID_SET_RND_MODE()
#endif

#if !defined(BID_MS_FLAGS) && (defined(_MSC_VER) && !defined(__INTEL_COMPILER))
#   define BID_MS_FLAGS
#endif

#if (defined(_MSC_VER) && !defined(__INTEL_COMPILER))
#   include <math.h>    // needed for MS build of some BID32 transcendentals (hypot)
#endif



#if defined (UNCHANGED_BINARY_STATUS_FLAGS) && defined (BID_FUNCTION_SETS_BINARY_FLAGS)
#   if defined( BID_MS_FLAGS )

#       include <float.h>

        extern unsigned int __bid_ms_restore_flags(unsigned int*);

#       define BID_OPT_FLAG_DECLARE() \
            unsigned int binaryflags = 0; 
#       define BID_OPT_SAVE_BINARY_FLAGS() \
             binaryflags = _statusfp();
#        define BID_OPT_RESTORE_BINARY_FLAGS() \
              __bid_ms_restore_flags(&binaryflags);

#   else
#       include <fenv.h>
#       define BID_FE_ALL_FLAGS FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT
#       define BID_OPT_FLAG_DECLARE() \
            fexcept_t binaryflags = 0; 
#       define BID_OPT_SAVE_BINARY_FLAGS() \
            (void) fegetexceptflag (&binaryflags, BID_FE_ALL_FLAGS);
#       define BID_OPT_RESTORE_BINARY_FLAGS() \
            (void) fesetexceptflag (&binaryflags, BID_FE_ALL_FLAGS);
#   endif
#else
#   define BID_OPT_FLAG_DECLARE() 
#   define BID_OPT_SAVE_BINARY_FLAGS()
#   define BID_OPT_RESTORE_BINARY_FLAGS()
#endif

#define BID_PROLOG_REF(arg_name) \
       BID_COPY_ARG_REF(arg_name)

#define BID_PROLOG_VAL(arg_name) \
       BID_COPY_ARG_VAL(arg_name)

#define BID_PROLOG_TYPE_REF(type, arg_name) \
       BID_COPY_ARG_TYPE_REF(type, arg_name)

#define BID_PROLOG_TYPE_VAL(type, arg_name) \
      BID_COPY_ARG_TYPE_VAL(type, arg_name)

#define OTHER_BID_PROLOG_REF()    BID_OPT_FLAG_DECLARE()
#define OTHER_BID_PROLOG_VAL()    BID_OPT_FLAG_DECLARE()

#if DECIMAL_CALL_BY_REFERENCE
#define       BID128_FUNCTION_ARG1(fn_name, arg_name)\
      void fn_name (BID_UINT128 * pres, \
           BID_UINT128 *  \
           pbid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG1_NORND(fn_name, arg_name)\
      void fn_name (BID_UINT128 * pres, \
           BID_UINT128 *  \
           pbid_##arg_name _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name)   \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name)\
      void fn_name (restype * pres, \
           BID_UINT128 *  \
           pbid_##arg_name _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name)   \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG2(fn_name, arg_name1, arg_name2)\
      void fn_name (BID_UINT128 * pres, \
           BID_UINT128 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG2_NORND(fn_name, arg_name1, arg_name2)\
      void fn_name (BID_UINT128 * pres, \
           BID_UINT128 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2  \
           _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name1, arg_name2)\
      void fn_name (restype * pres, \
           BID_UINT128 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2  \
           _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG2P_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name1, arg_name2)\
      void fn_name (restype * pres, \
           BID_UINT128 *pbid_##arg_name1,  BID_UINT128 *arg_name2  \
           _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG3P_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name1, arg_name2, res_name3)\
      void fn_name (restype * pres, \
           BID_UINT128 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2, BID_UINT128 *res_name3  \
           _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARG128_ARGTYPE2(fn_name, arg_name1, type2, arg_name2)\
      void fn_name (BID_UINT128 * pres, \
           BID_UINT128 *pbid_##arg_name1,  type2 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_TYPE_REF(type2, arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define BID128_FUNCTION_ARG128_CUSTOMARGTYPE2(fn_name, arg_name1, type2, arg_name2) BID128_FUNCTION_ARG128_ARGTYPE2(fn_name, arg_name1, type2, arg_name2)

#define       BID128_FUNCTION_ARG128_CUSTOMARGTYPE2_PLAIN(fn_name, arg_name1, type2, arg_name2)\
      void fn_name (BID_UINT128 * pres, \
           BID_UINT128 *pbid_##arg_name1,  type2 arg_name2  \
           ) {\
     BID_PROLOG_REF(arg_name1)   \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2(type0, fn_name, type1, arg_name1, type2, arg_name2)\
      void fn_name (type0 *pres, \
           type1 *pbid_##arg_name1,  type2 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name1)   \
     BID_PROLOG_TYPE_REF(type2, arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARGTYPE1_OTHER_ARGTYPE2  BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2

#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2_ARGTYPE3(type0, fn_name, type1, arg_name1, type2, arg_name2, type3, arg_name3)\
      void fn_name (type0 *pres, \
           type1 *pbid_##arg_name1,  type2 *pbid_##arg_name2,  type3 *pbid_##arg_name3  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name1)   \
     BID_PROLOG_TYPE_REF(type2, arg_name2)   \
     BID_PROLOG_TYPE_REF(type3, arg_name3)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE_FUNCTION_ARG2(type0, fn_name, arg_name1, arg_name2)\
      void fn_name (type0 *pres, \
           type0 *pbid_##arg_name1,  type0 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type0, arg_name1)   \
     BID_PROLOG_TYPE_REF(type0, arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE_FUNCTION_ARG2_CUSTOMRESULT_NORND(typeres, fn_name, type0, arg_name1, arg_name2)\
      void fn_name (typeres *pres, \
           type0 *pbid_##arg_name1,  type0 *pbid_##arg_name2  \
           _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type0, arg_name1)   \
     BID_PROLOG_TYPE_REF(type0, arg_name2)   \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE_FUNCTION_ARG1(type0, fn_name, arg_name1)\
      void fn_name (type0 *pres, \
           type0 *pbid_##arg_name1  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type0, arg_name1)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARGTYPE1_ARG128(fn_name, type1, arg_name1, arg_name2)\
      void fn_name (BID_UINT128 * pres, \
           type1 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARG128_ARGTYPE2(type0, fn_name, arg_name1, type2, arg_name2)\
      void fn_name (type0 *pres, \
           BID_UINT128 *pbid_##arg_name1,  type2 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_TYPE_REF(type2, arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARG128(type0, fn_name, type1, arg_name1, arg_name2)\
      void fn_name (type0 *pres, \
           type1 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARG128_ARG128(type0, fn_name, arg_name1, arg_name2)\
      void fn_name (type0 * pres, \
           BID_UINT128 *pbid_##arg_name1,  BID_UINT128 *pbid_##arg_name2  \
           _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name1)   \
     BID_PROLOG_REF(arg_name2)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARG1(type0, fn_name, arg_name)\
      void fn_name (type0 * pres, \
           BID_UINT128 *  \
           pbid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_REF(arg_name)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID128_FUNCTION_ARGTYPE1(fn_name, type1, arg_name)\
      void fn_name (BID_UINT128 * pres, \
           type1 *  \
           pbid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARGTYPE1(type0, fn_name, type1, arg_name)\
      void fn_name (type0 * pres, \
           type1 *  \
           pbid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name)   \
     BID_SET_RND_MODE()         \
     OTHER_BID_PROLOG_REF()

#define       BID_RESTYPE0_FUNCTION_ARGTYPE1   BID_TYPE0_FUNCTION_ARGTYPE1

#define       BID_TYPE0_FUNCTION_ARGTYPE1_NORND(type0, fn_name, type1, arg_name)\
      void fn_name (type0 * pres, \
           type1 *  \
           pbid_##arg_name _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name)   \
     OTHER_BID_PROLOG_REF()

#define       BID_TYPE0_FUNCTION_ARGTYPE1_NORND_DFP BID_TYPE0_FUNCTION_ARGTYPE1_NORND

#define       BID_TYPE0_FUNCTION_ARGTYPE1_NORND_NOFLAGS(type0, fn_name, type1, arg_name)\
      void fn_name (type0 * pres, \
           type1 *  \
           pbid_##arg_name _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name)   

#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2_NORND(type0, fn_name, type1, arg_name1, type2, arg_name2)\
      void fn_name (type0 * pres, \
           type1 *  \
           pbid_##arg_name1, type2 * pbid_##arg_name2 _EXC_FLAGS_PARAM _EXC_MASKS_PARAM \
           _EXC_INFO_PARAM) {\
     BID_PROLOG_TYPE_REF(type1, arg_name1)   \
     BID_PROLOG_TYPE_REF(type2, arg_name2)   \
     OTHER_BID_PROLOG_REF()
//////////////////////////////////////////
/////////////////////////////////////////
////////////////////////////////////////

#else

//////////////////////////////////////////
/////////////////////////////////////////
////////////////////////////////////////

// BID args and result
#define       BID128_FUNCTION_ARG1(fn_name, arg_name)\
	 DFP_WRAPFN_DFP(128, fn_name, 128);              \
	                                                 \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_VAL(arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID128_FUNCTION_ARG1_NORND(fn_name, arg_name)\
	 DFP_WRAPFN_DFP(128, fn_name, 128);              \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_VAL(arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// result is not BID type
#define       BID128_FUNCTION_ARG1_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name)\
	 RES_WRAPFN_DFP(restype, fn_name, 128);              \
DECLSPEC_OPT      restype                                     \
     fn_name (BID_UINT128 bid_##arg_name _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_VAL(arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID128_FUNCTION_ARG2(fn_name, arg_name1, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(128, fn_name, 128, 128);                 \
	                                                             \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_VAL(arg_name2)                      \
     OTHER_BID_PROLOG_VAL()

// fmod, rem
#define       BID128_FUNCTION_ARG2_NORND(fn_name, arg_name1, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(128, fn_name, 128, 128);                 \
	                                                             \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2 _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_VAL(arg_name2)                      \
     OTHER_BID_PROLOG_VAL()

// compares
#define       BID128_FUNCTION_ARG2_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name1, arg_name2)\
	 RES_WRAPFN_DFP_DFP(restype, fn_name, 128, 128);                 \
DECLSPEC_OPT      restype                                    \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2 _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_VAL(arg_name2)                      \
     OTHER_BID_PROLOG_VAL()

// not currently used
#define       BID128_FUNCTION_ARG2P_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name1, res_name2)\
	 RES_WRAPFN_DFP_DFP(restype, fn_name, 128, 128);                 \
DECLSPEC_OPT      restype                                    \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            BID_UINT128* res_name2 _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     OTHER_BID_PROLOG_VAL()

// not currently used
#define       BID128_FUNCTION_ARG3P_NORND_CUSTOMRESTYPE(restype, fn_name, arg_name1, arg_name2, res_name3)\
	 RES_WRAPFN_DFP_DFP_DFP(restype, fn_name, 128, 128, 128);                 \
DECLSPEC_OPT      restype                                    \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2, BID_UINT128* res_name3 _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_VAL(arg_name2)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID128_FUNCTION_ARG128_ARGTYPE2(fn_name, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(128, fn_name, 128, bidsize(type2));                 \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            type2 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// scalb, ldexp
#define       BID128_FUNCTION_ARG128_CUSTOMARGTYPE2(fn_name, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_OTHERTYPE(128, fn_name, 128, type2);                 \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            type2 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// frexp
#define       BID128_FUNCTION_ARG128_CUSTOMARGTYPE2_PLAIN(fn_name, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_OTHERTYPE(128, fn_name, 128, type2);                 \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            type2 arg_name2   \
           ) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2(type0, fn_name, type1, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(bidsize(type0), fn_name, bidsize(type1), bidsize(type2));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name1,      \
            type2 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type1, arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// BID arg1 and result
#define       BID_TYPE0_FUNCTION_ARGTYPE1_OTHER_ARGTYPE2(type0, fn_name, type1, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_OTHERTYPE(bidsize(type0), fn_name, bidsize(type1), type2);                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name1,      \
            type2 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type1, arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2_ARGTYPE3(type0, fn_name, type1, arg_name1, type2, arg_name2, type3, arg_name3)\
	 DFP_WRAPFN_DFP_DFP_DFP(bidsize(type0), fn_name, bidsize(type1), bidsize(type2), bidsize(type3));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name1,      \
            type2 bid_##arg_name2, type3 bid_##arg_name3 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type1, arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)          \
     BID_PROLOG_TYPE_VAL(type3, arg_name3)          \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE_FUNCTION_ARG2(type0, fn_name, arg_name1, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(bidsize(type0), fn_name, bidsize(type0), bidsize(type0));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type0 bid_##arg_name1,      \
            type0 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type0, arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type0, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// BID args, result a different type (e.g. for compares)
#define       BID_TYPE_FUNCTION_ARG2_CUSTOMRESULT_NORND(typeres, fn_name, type0, arg_name1, arg_name2)\
	 RES_WRAPFN_DFP_DFP(typeres, fn_name, bidsize(type0), bidsize(type0));                 \
DECLSPEC_OPT      typeres                                     \
     fn_name (type0 bid_##arg_name1,      \
            type0 bid_##arg_name2 _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type0, arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type0, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE_FUNCTION_ARG1(type0, fn_name, arg_name1)\
	 DFP_WRAPFN_DFP(bidsize(type0), fn_name, bidsize(type0));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type0 bid_##arg_name1      \
             _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type0, arg_name1)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID128_FUNCTION_ARGTYPE1_ARG128(fn_name, type1, arg_name1, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(128, fn_name, bidsize(type1), 128);                 \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (type1 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type1, arg_name1)          \
     BID_PROLOG_VAL(arg_name2)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARG128_ARGTYPE2(type0, fn_name, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(bidsize(type0), fn_name, 128, bidsize(type2));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            type2 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARG128(type0, fn_name, type1, arg_name1, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(bidsize(type0), fn_name, bidsize(type1), 128);                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_TYPE_VAL(type1, arg_name1)                      \
     BID_PROLOG_VAL(arg_name2)          \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARG128_ARG128(type0, fn_name, arg_name1, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(bidsize(type0), fn_name, 128, 128);                 \
DECLSPEC_OPT      type0                                     \
     fn_name (BID_UINT128 bid_##arg_name1,      \
            BID_UINT128 bid_##arg_name2 _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) {  \
     BID_PROLOG_VAL(arg_name1)                      \
     BID_PROLOG_VAL(arg_name2)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARG1(type0, fn_name, arg_name)\
	 DFP_WRAPFN_DFP(bidsize(type0), fn_name, 128);                 \
DECLSPEC_OPT      type0                                     \
     fn_name (BID_UINT128 bid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_VAL(arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID128_FUNCTION_ARGTYPE1(fn_name, type1, arg_name)\
	 DFP_WRAPFN_DFP(128, fn_name, bidsize(type1));                 \
DECLSPEC_OPT      BID_UINT128                                     \
     fn_name (type1 bid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARGTYPE1(type0, fn_name, type1, arg_name)\
	 DFP_WRAPFN_DFP(bidsize(type0), fn_name, bidsize(type1))                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID args and result
#define       BID_TYPE0_FUNCTION_ARGTYPE1_NORND_DFP(type0, fn_name, type1, arg_name)\
	 DFP_WRAPFN_DFP(bidsize(type0), fn_name, bidsize(type1))                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID args, different type result
#define       BID_RESTYPE0_FUNCTION_ARGTYPE1(type0, fn_name, type1, arg_name)\
	 RES_WRAPFN_DFP(type0, fn_name, bidsize(type1))                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name _RND_MODE_PARAM _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// BID to int/uint functions
#define       BID_TYPE0_FUNCTION_ARGTYPE1_NORND(type0, fn_name, type1, arg_name)\
	 RES_WRAPFN_DFP(type0, fn_name, bidsize(type1));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name)                      \
     OTHER_BID_PROLOG_VAL()

// used for BID-to-BID conversions
#define       BID_TYPE0_FUNCTION_ARGTYPE1_NORND_NOFLAGS(type0, fn_name, type1, arg_name)\
	 DFP_WRAPFN_DFP(bidsize(type0), fn_name, bidsize(type1));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name)                      

// fmod, rem
#define       BID_TYPE0_FUNCTION_ARGTYPE1_ARGTYPE2_NORND(type0, fn_name, type1, arg_name1, type2, arg_name2)\
	 DFP_WRAPFN_DFP_DFP(bidsize(type0), fn_name, bidsize(type1), bidsize(type2));                 \
DECLSPEC_OPT      type0                                     \
     fn_name (type1 bid_##arg_name1, type2 bid_##arg_name2 _EXC_FLAGS_PARAM  \
           _EXC_MASKS_PARAM _EXC_INFO_PARAM) { \
     BID_PROLOG_TYPE_VAL(type1, arg_name1)                      \
     BID_PROLOG_TYPE_VAL(type2, arg_name2)                      \
     OTHER_BID_PROLOG_VAL()
#endif



#define   BID_TO_SMALL_BID_UINT_CVT_FUNCTION(type0, fn_name, type1, arg_name, cvt_fn_name, type2, size_mask, invalid_res)\
    BID_TYPE0_FUNCTION_ARGTYPE1_NORND(type0, fn_name, type1, arg_name)\
        type2 res;                                                    \
        _IDEC_flags saved_fpsc=*pfpsf;                                \
    BIDECIMAL_CALL1_NORND(cvt_fn_name, res, arg_name);            \
        if(res & size_mask) {                                         \
      *pfpsf = saved_fpsc | BID_INVALID_EXCEPTION;                    \
          res = invalid_res; }                                        \
    BID_RETURN_VAL((type0)res);                                   \
                   }

#define   BID_TO_SMALL_INT_CVT_FUNCTION(type0, fn_name, type1, arg_name, cvt_fn_name, type2, size_mask, invalid_res)\
    BID_TYPE0_FUNCTION_ARGTYPE1_NORND(type0, fn_name, type1, arg_name)\
        type2 res, sgn_mask;                                          \
        _IDEC_flags saved_fpsc=*pfpsf;                                \
    BIDECIMAL_CALL1_NORND(cvt_fn_name, res, arg_name);            \
        sgn_mask = res & size_mask;                                   \
        if(sgn_mask && (sgn_mask != (type2)size_mask)) {                     \
      *pfpsf = saved_fpsc | BID_INVALID_EXCEPTION;                    \
          res = invalid_res; }                                        \
    BID_RETURN_VAL((type0)res);                                   \
                   }
#endif

