/*
 * Copyright 2022-present MongoDB, Inc.
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

#include "mc-rangeopts-private.h"

#include "mc-check-conversions-private.h"
#include "mc-range-edge-generation-private.h" // mc_count_leading_zeros_XX
#include "mc-range-encoding-private.h"        // mc_getTypeInfoXX
#include "mongocrypt-private.h"
#include "mongocrypt-util-private.h" // mc_bson_type_to_string

#include <float.h> // DBL_MAX

// Common logic for testing field name, tracking duplication, and presence.
#define IF_FIELD(Name, ErrorPrefix)                                                                                    \
    if (0 == strcmp(field, #Name)) {                                                                                   \
        if (has_##Name) {                                                                                              \
            CLIENT_ERR("%sUnexpected duplicate field '" #Name "'", ErrorPrefix);                                       \
            return false;                                                                                              \
        }                                                                                                              \
        has_##Name = true;

#define END_IF_FIELD                                                                                                   \
    continue;                                                                                                          \
    }

#define CHECK_HAS(Name, ErrorPrefix)                                                                                   \
    if (!has_##Name) {                                                                                                 \
        CLIENT_ERR("%sMissing field '" #Name "'", ErrorPrefix);                                                        \
        return false;                                                                                                  \
    }

#if defined(YGGR_EX_C99_SUPPORTED)

bool mc_RangeOpts_parse(mc_RangeOpts_t *ro, const bson_t *in, bool use_range_v2, mongocrypt_status_t *status) {
    bson_iter_t iter;
    bool has_min = false, has_max = false, has_sparsity = false, has_precision = false, has_trimFactor = false;
    const char *const error_prefix = "Error parsing RangeOpts: ";

    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(in);
    BSON_ASSERT(status || true);

    *ro = (mc_RangeOpts_t){0};
    ro->bson = bson_copy(in);

    if (!bson_iter_init(&iter, ro->bson)) {
        CLIENT_ERR("%sInvalid BSON", error_prefix);
        return false;
    }

    while (bson_iter_next(&iter)) {
        const char *field = bson_iter_key(&iter);
        BSON_ASSERT(field);

        IF_FIELD(min, error_prefix)
        ro->min.set = true;
        ro->min.value = iter;
        END_IF_FIELD

        IF_FIELD(max, error_prefix)
        ro->max.set = true;
        ro->max.value = iter;
        END_IF_FIELD

        IF_FIELD(sparsity, error_prefix)
        if (!BSON_ITER_HOLDS_INT64(&iter)) {
            CLIENT_ERR("%sExpected int64 for sparsity, got: %s",
                       error_prefix,
                       mc_bson_type_to_string(bson_iter_type(&iter)));
            return false;
        };
        ro->sparsity = bson_iter_int64(&iter);
        END_IF_FIELD

        IF_FIELD(precision, error_prefix) {
            if (!BSON_ITER_HOLDS_INT32(&iter)) {
                CLIENT_ERR("%s'precision' must be an int32", error_prefix);
                return false;
            }
            int32_t val = bson_iter_int32(&iter);
            if (val < 0) {
                CLIENT_ERR("%s'precision' must be non-negative", error_prefix);
                return false;
            }
            ro->precision = OPT_U32((uint32_t)val);
        }
        END_IF_FIELD

        IF_FIELD(trimFactor, error_prefix) {
            if (!BSON_ITER_HOLDS_INT32(&iter)) {
                CLIENT_ERR("%sExpected int32 for trimFactor, got: %s",
                           error_prefix,
                           mc_bson_type_to_string(bson_iter_type(&iter)));
                return false;
            };
            int32_t val = bson_iter_int32(&iter);
            if (val < 0) {
                CLIENT_ERR("%s'trimFactor' must be non-negative", error_prefix);
                return false;
            }
            ro->trimFactor = OPT_U32((uint32_t)val);
        }
        END_IF_FIELD

        CLIENT_ERR("%sUnrecognized field: '%s'", error_prefix, field);
        return false;
    }

    // Do not error if min/max are not present. min/max are optional.
    CHECK_HAS(sparsity, error_prefix);
    // Do not error if precision is not present. Precision is optional and only
    // applies to double/decimal128.
    // Do not error if trimFactor is not present. It is optional.

    // Expect precision only to be set for double or decimal128.
    if (has_precision) {
        if (!ro->min.set) {
            CLIENT_ERR("setting precision requires min");
            return false;
        }

        bson_type_t minType = bson_iter_type(&ro->min.value);
        if (minType != BSON_TYPE_DOUBLE && minType != BSON_TYPE_DECIMAL128) {
            CLIENT_ERR("expected 'precision' to be set with double or decimal128 "
                       "index, but got: %s min",
                       mc_bson_type_to_string(minType));
            return false;
        }

        if (!ro->max.set) {
            CLIENT_ERR("setting precision requires max");
            return false;
        }

        bson_type_t maxType = bson_iter_type(&ro->max.value);
        if (maxType != BSON_TYPE_DOUBLE && maxType != BSON_TYPE_DECIMAL128) {
            CLIENT_ERR("expected 'precision' to be set with double or decimal128 "
                       "index, but got: %s max",
                       mc_bson_type_to_string(maxType));
            return false;
        }
    }

    // Expect min and max to match types.
    if (ro->min.set && ro->max.set) {
        bson_type_t minType = bson_iter_type(&ro->min.value), maxType = bson_iter_type(&ro->max.value);
        if (minType != maxType) {
            CLIENT_ERR("expected 'min' and 'max' to be same type, but got: %s "
                       "min and %s max",
                       mc_bson_type_to_string(minType),
                       mc_bson_type_to_string(maxType));
            return false;
        }
    }

    if (ro->min.set || ro->max.set) {
        // Setting min/max without precision is error for double and decimal128.
        if (ro->min.set) {
            bson_type_t minType = bson_iter_type(&ro->min.value);
            if (minType == BSON_TYPE_DOUBLE || minType == BSON_TYPE_DECIMAL128) {
                if (!has_precision) {
                    CLIENT_ERR("expected 'precision' to be set with 'min' for %s", mc_bson_type_to_string(minType));
                    return false;
                }
            }
        }

        if (ro->max.set) {
            bson_type_t maxType = bson_iter_type(&ro->max.value);
            if (maxType == BSON_TYPE_DOUBLE || maxType == BSON_TYPE_DECIMAL128) {
                if (!has_precision) {
                    CLIENT_ERR("expected 'precision' to be set with 'max' for %s", mc_bson_type_to_string(maxType));
                    return false;
                }
            }
        }
    }

    if (ro->trimFactor.set) {
        if (!use_range_v2) {
            // Once `use_range_v2` is default true, this block may be removed.
            CLIENT_ERR("trimFactor is not supported for QE range v1");
            return false;
        }
        // At this point, we do not know the type of the field if min and max are unspecified. Wait to
        // validate the value of trimFactor.
    }

    return true;
}

#else

bool mc_RangeOpts_parse(mc_RangeOpts_t *ro, const bson_t *in, bool use_range_v2, mongocrypt_status_t *status) {
    bson_iter_t iter;
    bool has_min = false, has_max = false, has_sparsity = false, has_precision = false, has_trimFactor = false;
    const char *const error_prefix = "Error parsing RangeOpts: ";

    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(in);
    BSON_ASSERT(status || true);

    //*ro = (mc_RangeOpts_t){0};
	memset(ro, 0, sizeof(mc_RangeOpts_t));
    ro->bson = bson_copy(in);

    if (!bson_iter_init(&iter, ro->bson)) {
        CLIENT_ERR("%sInvalid BSON", error_prefix);
        return false;
    }

    while (bson_iter_next(&iter)) {
        const char *field = bson_iter_key(&iter);
        BSON_ASSERT(field);

        IF_FIELD(min, error_prefix)
        ro->min.set = true;
        ro->min.value = iter;
        END_IF_FIELD

        IF_FIELD(max, error_prefix)
        ro->max.set = true;
        ro->max.value = iter;
        END_IF_FIELD

        IF_FIELD(sparsity, error_prefix)
        if (!BSON_ITER_HOLDS_INT64(&iter)) {
            CLIENT_ERR("%sExpected int64 for sparsity, got: %s",
                       error_prefix,
                       mc_bson_type_to_string(bson_iter_type(&iter)));
            return false;
        };
        ro->sparsity = bson_iter_int64(&iter);
        END_IF_FIELD

        IF_FIELD(precision, error_prefix) {
			int32_t val = 0;
            if (!BSON_ITER_HOLDS_INT32(&iter)) {
                CLIENT_ERR("%s'precision' must be an int32", error_prefix);
                return false;
            }
            /*int32_t*/ val = bson_iter_int32(&iter);
            if (val < 0) {
                CLIENT_ERR("%s'precision' must be non-negative", error_prefix);
                return false;
            }
            //ro->precision = OPT_U32((uint32_t)val);
			ro->precision.set = true;
			ro->precision.value = (uint32_t)val;
        }
        END_IF_FIELD

        IF_FIELD(trimFactor, error_prefix) {
			int32_t val = 0;
            if (!BSON_ITER_HOLDS_INT32(&iter)) {
                CLIENT_ERR("%sExpected int32 for trimFactor, got: %s",
                           error_prefix,
                           mc_bson_type_to_string(bson_iter_type(&iter)));
                return false;
            };
            /*int32_t*/ val = bson_iter_int32(&iter);
            if (val < 0) {
                CLIENT_ERR("%s'trimFactor' must be non-negative", error_prefix);
                return false;
            }
            //ro->trimFactor = OPT_U32((uint32_t)val);
			ro->trimFactor.set = true;
			ro->trimFactor.value = (uint32_t)val;
        }
        END_IF_FIELD

        CLIENT_ERR("%sUnrecognized field: '%s'", error_prefix, field);
        return false;
    }

    // Do not error if min/max are not present. min/max are optional.
    CHECK_HAS(sparsity, error_prefix);
    // Do not error if precision is not present. Precision is optional and only
    // applies to double/decimal128.
    // Do not error if trimFactor is not present. It is optional.

    // Expect precision only to be set for double or decimal128.
    if (has_precision) {
		bson_type_t minType;
		bson_type_t maxType;
        if (!ro->min.set) {
            CLIENT_ERR("setting precision requires min");
            return false;
        }

        /*bson_type_t*/ minType = bson_iter_type(&ro->min.value);
        if (minType != BSON_TYPE_DOUBLE && minType != BSON_TYPE_DECIMAL128) {
            CLIENT_ERR("expected 'precision' to be set with double or decimal128 "
                       "index, but got: %s min",
                       mc_bson_type_to_string(minType));
            return false;
        }

        if (!ro->max.set) {
            CLIENT_ERR("setting precision requires max");
            return false;
        }

        /*bson_type_t*/ maxType = bson_iter_type(&ro->max.value);
        if (maxType != BSON_TYPE_DOUBLE && maxType != BSON_TYPE_DECIMAL128) {
            CLIENT_ERR("expected 'precision' to be set with double or decimal128 "
                       "index, but got: %s max",
                       mc_bson_type_to_string(maxType));
            return false;
        }
    }

    // Expect min and max to match types.
    if (ro->min.set && ro->max.set) {
        bson_type_t minType = bson_iter_type(&ro->min.value), maxType = bson_iter_type(&ro->max.value);
        if (minType != maxType) {
            CLIENT_ERR("expected 'min' and 'max' to be same type, but got: %s "
                       "min and %s max",
                       mc_bson_type_to_string(minType),
                       mc_bson_type_to_string(maxType));
            return false;
        }
    }

    if (ro->min.set || ro->max.set) {
        // Setting min/max without precision is error for double and decimal128.
        if (ro->min.set) {
            bson_type_t minType = bson_iter_type(&ro->min.value);
            if (minType == BSON_TYPE_DOUBLE || minType == BSON_TYPE_DECIMAL128) {
                if (!has_precision) {
                    CLIENT_ERR("expected 'precision' to be set with 'min' for %s", mc_bson_type_to_string(minType));
                    return false;
                }
            }
        }

        if (ro->max.set) {
            bson_type_t maxType = bson_iter_type(&ro->max.value);
            if (maxType == BSON_TYPE_DOUBLE || maxType == BSON_TYPE_DECIMAL128) {
                if (!has_precision) {
                    CLIENT_ERR("expected 'precision' to be set with 'max' for %s", mc_bson_type_to_string(maxType));
                    return false;
                }
            }
        }
    }

    if (ro->trimFactor.set) {
        if (!use_range_v2) {
            // Once `use_range_v2` is default true, this block may be removed.
            CLIENT_ERR("trimFactor is not supported for QE range v1");
            return false;
        }
        // At this point, we do not know the type of the field if min and max are unspecified. Wait to
        // validate the value of trimFactor.
    }

    return true;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

#if defined(YGGR_EX_C99_SUPPORTED)

bool mc_RangeOpts_to_FLE2RangeInsertSpec(const mc_RangeOpts_t *ro,
                                         const bson_t *v,
                                         bson_t *out,
                                         mongocrypt_status_t *status) {
    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(v);
    BSON_ASSERT_PARAM(out);
    BSON_ASSERT(status || true);

    const char *const error_prefix = "Error making FLE2RangeInsertSpec: ";
    bson_iter_t v_iter;
    if (!bson_iter_init_find(&v_iter, v, "v")) {
        CLIENT_ERR("Unable to find 'v' in input");
        return false;
    }

    bson_t child;
    if (!BSON_APPEND_DOCUMENT_BEGIN(out, "v", &child)) {
        CLIENT_ERR("%sError appending to BSON", error_prefix);
        return false;
    }
    if (!bson_append_iter(&child, "v", 1, &v_iter)) {
        CLIENT_ERR("%sError appending to BSON", error_prefix);
        return false;
    }

    if (!mc_RangeOpts_appendMin(ro, bson_iter_type(&v_iter), "min", &child, status)) {
        return false;
    }

    if (!mc_RangeOpts_appendMax(ro, bson_iter_type(&v_iter), "max", &child, status)) {
        return false;
    }

    if (ro->precision.set) {
        BSON_ASSERT(ro->precision.value <= INT32_MAX);
        if (!BSON_APPEND_INT32(&child, "precision", (int32_t)ro->precision.value)) {
            CLIENT_ERR("%sError appending to BSON", error_prefix);
            return false;
        }
    }

    if (!mc_RangeOpts_appendTrimFactor(ro, bson_iter_type(&v_iter), "trimFactor", &child, status)) {
        return false;
    }

    if (!bson_append_document_end(out, &child)) {
        CLIENT_ERR("%sError appending to BSON", error_prefix);
        return false;
    }
    return true;
}

#else

bool mc_RangeOpts_to_FLE2RangeInsertSpec(const mc_RangeOpts_t *ro,
											const bson_t *v,
											bson_t *out,
											mongocrypt_status_t *status) {
	const char *const error_prefix = "Error making FLE2RangeInsertSpec: ";
	bson_iter_t v_iter;
	bson_t child;

	BSON_ASSERT_PARAM(ro);
	BSON_ASSERT_PARAM(v);
	BSON_ASSERT_PARAM(out);
	BSON_ASSERT(status || true);

	//const char *const error_prefix = "Error making FLE2RangeInsertSpec: ";
	//bson_iter_t v_iter;
	if (!bson_iter_init_find(&v_iter, v, "v")) {
		CLIENT_ERR("Unable to find 'v' in input");
		return false;
	}

	//bson_t child;
	if (!BSON_APPEND_DOCUMENT_BEGIN(out, "v", &child)) {
		CLIENT_ERR("%sError appending to BSON", error_prefix);
		return false;
	}
	if (!bson_append_iter(&child, "v", 1, &v_iter)) {
		CLIENT_ERR("%sError appending to BSON", error_prefix);
		return false;
	}

	if (!mc_RangeOpts_appendMin(ro, bson_iter_type(&v_iter), "min", &child, status)) {
		return false;
	}

	if (!mc_RangeOpts_appendMax(ro, bson_iter_type(&v_iter), "max", &child, status)) {
		return false;
	}

	if (ro->precision.set) {
		BSON_ASSERT(ro->precision.value <= INT32_MAX);
		if (!BSON_APPEND_INT32(&child, "precision", (int32_t)ro->precision.value)) {
			CLIENT_ERR("%sError appending to BSON", error_prefix);
			return false;
		}
	}

	if (!mc_RangeOpts_appendTrimFactor(ro, bson_iter_type(&v_iter), "trimFactor", &child, status)) {
		return false;
	}

	if (!bson_append_document_end(out, &child)) {
		CLIENT_ERR("%sError appending to BSON", error_prefix);
		return false;
	}
	return true;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

bool mc_RangeOpts_appendMin(const mc_RangeOpts_t *ro,
                            bson_type_t valueType,
                            const char *fieldName,
                            bson_t *out,
                            mongocrypt_status_t *status) {
    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(fieldName);
    BSON_ASSERT_PARAM(out);
    BSON_ASSERT(status || true);

    if (ro->min.set) {
        if (bson_iter_type(&ro->min.value) != valueType) {
            CLIENT_ERR("expected matching 'min' and value type. Got range option "
                       "'min' of type %s and value of type %s",
                       mc_bson_type_to_string(bson_iter_type(&ro->min.value)),
                       mc_bson_type_to_string(valueType));
            return false;
        }
        if (!bson_append_iter(out, fieldName, -1, &ro->min.value)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
        return true;
    }

    if (valueType == BSON_TYPE_INT32 || valueType == BSON_TYPE_INT64 || valueType == BSON_TYPE_DATE_TIME) {
        CLIENT_ERR("Range option 'min' is required for type: %s", mc_bson_type_to_string(valueType));
        return false;
    } else if (valueType == BSON_TYPE_DOUBLE) {
        if (!BSON_APPEND_DOUBLE(out, fieldName, -DBL_MAX)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
    } else if (valueType == BSON_TYPE_DECIMAL128) {
#if MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
        const bson_decimal128_t min = mc_dec128_to_bson_decimal128(MC_DEC128_LARGEST_NEGATIVE);
        if (!BSON_APPEND_DECIMAL128(out, fieldName, &min)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
#else  // ↑↑↑↑↑↑↑↑ With Decimal128 / Without ↓↓↓↓↓↓↓↓↓↓
        CLIENT_ERR("unsupported BSON type (Decimal128) for range: libmongocrypt "
                   "was built without extended Decimal128 support");
        return false;
#endif // MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
    } else {
        CLIENT_ERR("unsupported BSON type: %s for range", mc_bson_type_to_string(valueType));
        return false;
    }
    return true;
}

bool mc_RangeOpts_appendMax(const mc_RangeOpts_t *ro,
                            bson_type_t valueType,
                            const char *fieldName,
                            bson_t *out,
                            mongocrypt_status_t *status) {
    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(fieldName);
    BSON_ASSERT_PARAM(out);
    BSON_ASSERT(status || true);

    if (ro->max.set) {
        if (bson_iter_type(&ro->max.value) != valueType) {
            CLIENT_ERR("expected matching 'max' and value type. Got range option "
                       "'max' of type %s and value of type %s",
                       mc_bson_type_to_string(bson_iter_type(&ro->max.value)),
                       mc_bson_type_to_string(valueType));
            return false;
        }
        if (!bson_append_iter(out, fieldName, -1, &ro->max.value)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
        return true;
    }

    if (valueType == BSON_TYPE_INT32 || valueType == BSON_TYPE_INT64 || valueType == BSON_TYPE_DATE_TIME) {
        CLIENT_ERR("Range option 'max' is required for type: %s", mc_bson_type_to_string(valueType));
        return false;
    } else if (valueType == BSON_TYPE_DOUBLE) {
        if (!BSON_APPEND_DOUBLE(out, fieldName, DBL_MAX)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
    } else if (valueType == BSON_TYPE_DECIMAL128) {
#if MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
        const bson_decimal128_t max = mc_dec128_to_bson_decimal128(MC_DEC128_LARGEST_POSITIVE);
        if (!BSON_APPEND_DECIMAL128(out, fieldName, &max)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
#else  // ↑↑↑↑↑↑↑↑ With Decimal128 / Without ↓↓↓↓↓↓↓↓↓↓
        CLIENT_ERR("unsupported BSON type (Decimal128) for range: libmongocrypt "
                   "was built without extended Decimal128 support");
        return false;
#endif // MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
    } else {
        CLIENT_ERR("unsupported BSON type: %s for range", mc_bson_type_to_string(valueType));
        return false;
    }
    return true;
}

// Used to calculate max trim factor. Returns the number of bits required to represent any number in
// the domain.

#if defined(YGGR_EX_C99_SUPPORTED)

bool mc_getNumberOfBits(const mc_RangeOpts_t *ro,
                        bson_type_t valueType,
                        uint32_t *bitsOut,
                        mongocrypt_status_t *status) {
    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(bitsOut);

    // For each type, we use getTypeInfo to get the total number of values in the domain (-1)
    // which tells us how many bits are needed to represent the whole domain.
    // note - can't use a switch statement because of -Werror=switch-enum
    if (valueType == BSON_TYPE_INT32) {
        int32_t value = 0;
        mc_optional_int32_t rmin = {false, 0}, rmax = {false, 0};
        if (ro->min.set) {
            BSON_ASSERT(ro->max.set);
            value = bson_iter_int32(&ro->min.value);
            rmin = OPT_I32(value);
            rmax = OPT_I32(bson_iter_int32(&ro->max.value));
        }
        mc_getTypeInfo32_args_t args = {value, rmin, rmax};
        mc_OSTType_Int32 out;
        if (!mc_getTypeInfo32(args, &out, status)) {
            return false;
        }
        *bitsOut = 32 - (uint32_t)mc_count_leading_zeros_u32(out.max);
        return true;
    } else if (valueType == BSON_TYPE_INT64) {
        int64_t value = 0;
        mc_optional_int64_t rmin = {false, 0}, rmax = {false, 0};
        if (ro->min.set) {
            BSON_ASSERT(ro->max.set);
            value = bson_iter_int64(&ro->min.value);
            rmin = OPT_I64(value);
            rmax = OPT_I64(bson_iter_int64(&ro->max.value));
        }
        mc_getTypeInfo64_args_t args = {value, rmin, rmax};
        mc_OSTType_Int64 out;
        if (!mc_getTypeInfo64(args, &out, status)) {
            return false;
        }
        *bitsOut = 64 - (uint32_t)mc_count_leading_zeros_u64(out.max);
        return true;
    } else if (valueType == BSON_TYPE_DATE_TIME) {
        int64_t value = 0;
        mc_optional_int64_t rmin = {false, 0}, rmax = {false, 0};
        if (ro->min.set) {
            BSON_ASSERT(ro->max.set);
            value = bson_iter_date_time(&ro->min.value);
            rmin = OPT_I64(value);
            rmax = OPT_I64(bson_iter_date_time(&ro->max.value));
        }
        mc_getTypeInfo64_args_t args = {value, rmin, rmax};
        mc_OSTType_Int64 out;
        if (!mc_getTypeInfo64(args, &out, status)) {
            return false;
        }
        *bitsOut = 64 - (uint32_t)mc_count_leading_zeros_u64(out.max);
        return true;
    } else if (valueType == BSON_TYPE_DOUBLE) {
        double value = 0;
        mc_optional_double_t rmin = {false, 0}, rmax = {false, 0};
        mc_optional_uint32_t prec = ro->precision;
        if (ro->min.set) {
            BSON_ASSERT(ro->max.set);
            value = bson_iter_double(&ro->min.value);
            rmin = OPT_DOUBLE(value);
            rmax = OPT_DOUBLE(bson_iter_double(&ro->max.value));
        }
        mc_getTypeInfoDouble_args_t args = {value, rmin, rmax, prec};
        mc_OSTType_Double out;
        if (!mc_getTypeInfoDouble(args, &out, status)) {
            return false;
        }
        *bitsOut = 64 - (uint32_t)mc_count_leading_zeros_u64(out.max);
        return true;
    }
#if MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
    else if (valueType == BSON_TYPE_DECIMAL128) {
        mc_dec128 value = MC_DEC128_ZERO;
        mc_optional_dec128_t rmin = {false, MC_DEC128_ZERO}, rmax = {false, MC_DEC128_ZERO};
        mc_optional_uint32_t prec = ro->precision;
        if (ro->min.set) {
            BSON_ASSERT(ro->max.set);
            value = mc_dec128_from_bson_iter(&ro->min.value);
            rmin = OPT_MC_DEC128(value);
            rmax = OPT_MC_DEC128(mc_dec128_from_bson_iter(&ro->max.value));
        }
        mc_getTypeInfoDecimal128_args_t args = {value, rmin, rmax, prec};
        mc_OSTType_Decimal128 out;
        if (!mc_getTypeInfoDecimal128(args, &out, status)) {
            return false;
        }
        *bitsOut = 128 - (uint32_t)mc_count_leading_zeros_u128(out.max);
        return true;
    }
#endif
    CLIENT_ERR("unsupported BSON type: %s for range", mc_bson_type_to_string(valueType));
    return false;
}

#else

bool mc_getNumberOfBits(const mc_RangeOpts_t *ro,
						bson_type_t valueType,
						uint32_t *bitsOut,
						mongocrypt_status_t *status) {
	BSON_ASSERT_PARAM(ro);
	BSON_ASSERT_PARAM(bitsOut);

	// For each type, we use getTypeInfo to get the total number of values in the domain (-1)
	// which tells us how many bits are needed to represent the whole domain.
	// note - can't use a switch statement because of -Werror=switch-enum
	if (valueType == BSON_TYPE_INT32) {
		int32_t value = 0;
		mc_optional_int32_t rmin = {false, 0}, rmax = {false, 0};
		mc_getTypeInfo32_args_t args = {0};
		mc_OSTType_Int32 out;
		if (ro->min.set) {
			BSON_ASSERT(ro->max.set);
			value = bson_iter_int32(&ro->min.value);
			//rmin = OPT_I32(value);
			//rmax = OPT_I32(bson_iter_int32(&ro->max.value));
			rmin.set = true;
			rmin.value = value;
			rmax.set = true;
			rmax.value = bson_iter_int32(&ro->max.value);
		}
		//mc_getTypeInfo32_args_t args = {value, rmin, rmax};
		args.value = value;
		args.min = rmin;
		args.max = rmax;
		//mc_OSTType_Int32 out;
		if (!mc_getTypeInfo32(args, &out, status)) {
			return false;
		}
		*bitsOut = 32 - (uint32_t)mc_count_leading_zeros_u32(out.max);
		return true;
	} else if (valueType == BSON_TYPE_INT64) {
		int64_t value = 0;
		mc_optional_int64_t rmin = {false, 0}, rmax = {false, 0};
		mc_getTypeInfo64_args_t args = {0};
		mc_OSTType_Int64 out;
		if (ro->min.set) {
			BSON_ASSERT(ro->max.set);
			value = bson_iter_int64(&ro->min.value);
			//rmin = OPT_I64(value);
			//rmax = OPT_I64(bson_iter_int64(&ro->max.value));
			rmin.set = true;
			rmin.value = value;
			rmax.set = true;
			rmax.value = bson_iter_int64(&ro->max.value);
		}
		//mc_getTypeInfo64_args_t args = {value, rmin, rmax};
		args.value = value;
		args.min = rmin;
		args.max = rmax;
		//mc_OSTType_Int64 out;
		if (!mc_getTypeInfo64(args, &out, status)) {
			return false;
		}
		*bitsOut = 64 - (uint32_t)mc_count_leading_zeros_u64(out.max);
		return true;
	} else if (valueType == BSON_TYPE_DATE_TIME) {
		int64_t value = 0;
		mc_optional_int64_t rmin = {false, 0}, rmax = {false, 0};
		mc_getTypeInfo64_args_t args = {0};
		mc_OSTType_Int64 out;
		if (ro->min.set) {
			BSON_ASSERT(ro->max.set);
			value = bson_iter_date_time(&ro->min.value);
			//rmin = OPT_I64(value);
			//rmax = OPT_I64(bson_iter_date_time(&ro->max.value));
			rmin.set = true;
			rmin.value = value;
			rmax.set = true;
			rmax.value = bson_iter_date_time(&ro->max.value);
		}
		//mc_getTypeInfo64_args_t args = {value, rmin, rmax};
		args.value = value;
		args.min = rmin;
		args.max = rmax;
		//mc_OSTType_Int64 out;
		if (!mc_getTypeInfo64(args, &out, status)) {
			return false;
		}
		*bitsOut = 64 - (uint32_t)mc_count_leading_zeros_u64(out.max);
		return true;
	} else if (valueType == BSON_TYPE_DOUBLE) {
		double value = 0;
		mc_optional_double_t rmin = {false, 0}, rmax = {false, 0};
		mc_optional_uint32_t prec = ro->precision;
		mc_getTypeInfoDouble_args_t args = {0};
		mc_OSTType_Double out;
		if (ro->min.set) {
			BSON_ASSERT(ro->max.set);
			value = bson_iter_double(&ro->min.value);
			//rmin = OPT_DOUBLE(value);
			//rmax = OPT_DOUBLE(bson_iter_double(&ro->max.value));
			rmin.set = true;
			rmin.value = value;
			rmax.set = true;
			rmax.value = bson_iter_double(&ro->max.value);
		}
		//mc_getTypeInfoDouble_args_t args = {value, rmin, rmax, prec};
		args.value = value;
		args.min = rmin;
		args.max = rmax;
		args.precision = prec;
		//mc_OSTType_Double out;
		if (!mc_getTypeInfoDouble(args, &out, status)) {
			return false;
		}
		*bitsOut = 64 - (uint32_t)mc_count_leading_zeros_u64(out.max);
		return true;
	}
#if MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
	else if (valueType == BSON_TYPE_DECIMAL128) {
		mc_dec128 value = MC_DEC128_ZERO;
		mc_optional_dec128_t rmin = {false/*, MC_DEC128_ZERO*/}, rmax = {false/*, MC_DEC128_ZERO*/};
		mc_optional_uint32_t prec = ro->precision;
		mc_getTypeInfoDecimal128_args_t args = {0};
		mc_OSTType_Decimal128 out;
		rmin.value = MC_DEC128_ZERO;
		rmax.value = MC_DEC128_ZERO;
		if (ro->min.set) {
			BSON_ASSERT(ro->max.set);
			value = mc_dec128_from_bson_iter(&ro->min.value);
			//rmin = OPT_MC_DEC128(value);
			//rmax = OPT_MC_DEC128(mc_dec128_from_bson_iter(&ro->max.value));
			rmin.set = true;
			rmin.value = value;
			rmax.set = true;
			rmax.value = mc_dec128_from_bson_iter(&ro->max.value);
		}
		//mc_getTypeInfoDecimal128_args_t args = {value, rmin, rmax, prec};
		args.value = value;
		args.min = rmin;
		args.max = rmax;
		args.precision = prec;
		//mc_OSTType_Decimal128 out;
		if (!mc_getTypeInfoDecimal128(args, &out, status)) {
			return false;
		}
		*bitsOut = 128 - (uint32_t)mc_count_leading_zeros_u128(out.max);
		return true;
	}
#endif
	CLIENT_ERR("unsupported BSON type: %s for range", mc_bson_type_to_string(valueType));
	return false;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

#if defined(YGGR_EX_C99_SUPPORTED)

bool mc_RangeOpts_appendTrimFactor(const mc_RangeOpts_t *ro,
                                   bson_type_t valueType,
                                   const char *fieldName,
                                   bson_t *out,
                                   mongocrypt_status_t *status) {
    BSON_ASSERT_PARAM(ro);
    BSON_ASSERT_PARAM(fieldName);
    BSON_ASSERT_PARAM(out);
    BSON_ASSERT(status || true);

    if (!ro->trimFactor.set) {
        if (!BSON_APPEND_INT32(out, fieldName, 0)) {
            CLIENT_ERR("failed to append BSON");
            return false;
        }
        return true;
    }

    uint32_t nbits;
    if (!mc_getNumberOfBits(ro, valueType, &nbits, status)) {
        return false;
    }
    // if nbits = 0, we want to allow trim factor = 0.
    uint32_t test = nbits ? nbits : 1;
    if (ro->trimFactor.value >= test) {
        CLIENT_ERR("Trim factor (%d) must be less than the total number of bits (%d) used to represent "
                   "any element in the domain.",
                   ro->trimFactor.value,
                   nbits);
        return false;
    }
    if (!BSON_APPEND_INT32(out, fieldName, (int32_t)ro->trimFactor.value)) {
        CLIENT_ERR("failed to append BSON");
        return false;
    }
    return true;
}

#else

bool mc_RangeOpts_appendTrimFactor(const mc_RangeOpts_t *ro,
									bson_type_t valueType,
									const char *fieldName,
									bson_t *out,
									mongocrypt_status_t *status) {

	uint32_t nbits;
	uint32_t test = 0;
	BSON_ASSERT_PARAM(ro);
	BSON_ASSERT_PARAM(fieldName);
	BSON_ASSERT_PARAM(out);
	BSON_ASSERT(status || true);

	if (!ro->trimFactor.set) {
		if (!BSON_APPEND_INT32(out, fieldName, 0)) {
			CLIENT_ERR("failed to append BSON");
			return false;
		}
		return true;
	}

	//uint32_t nbits;
	if (!mc_getNumberOfBits(ro, valueType, &nbits, status)) {
		return false;
	}
	// if nbits = 0, we want to allow trim factor = 0.
	/*uint32_t*/ test = nbits ? nbits : 1;
	if (ro->trimFactor.value >= test) {
		CLIENT_ERR("Trim factor (%d) must be less than the total number of bits (%d) used to represent "
					"any element in the domain.",
					ro->trimFactor.value,
					nbits);
		return false;
	}
	if (!BSON_APPEND_INT32(out, fieldName, (int32_t)ro->trimFactor.value)) {
		CLIENT_ERR("failed to append BSON");
		return false;
	}
	return true;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

void mc_RangeOpts_cleanup(mc_RangeOpts_t *ro) {
    if (!ro) {
        return;
    }

    bson_destroy(ro->bson);
}
