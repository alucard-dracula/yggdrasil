// stdbool.h

#ifndef __MONGODB_YGGR_EX_STDBOOL_H__
#define __MONGODB_YGGR_EX_STDBOOL_H__

#include <yggr_detail/c_version_support.h>

#if defined(BUILD_MONGODB_DRIVER_YGGR_VER)
#   if (defined(MONGOC_HAVE_STDBOOL_H) \
            || defined(BSON_HAVE_STDBOOL_H) \
            || defined(MONGOCRYPT_HAVE_STDBOOL_H) \
            || defined(YGGR_EX_C99_SUPPORTED) )
#       include <stdbool.h>
#       if !defined(__bool_true_false_are_defined)
#           define false 0
#           define true 1
#       define __bool_true_false_are_defined 1
#       endif // __bool_true_false_are_defined
#   elif !defined(__bool_true_false_are_defined)
#       if !defined(__cplusplus)
            typedef signed char bool;
#           define false 0
#           define true 1
#       endif // __cplusplus
#       define __bool_true_false_are_defined 1
#   endif //MONGOC_HAVE_STDBOOL_H
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#endif // __MONGODB_YGGR_EX_STDBOOL_H__