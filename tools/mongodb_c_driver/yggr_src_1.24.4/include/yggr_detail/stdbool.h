// stdbool.h

#ifndef __MONGODB_YGGR_EX_STDBOOL_H__
#define __MONGODB_YGGR_EX_STDBOOL_H__

#ifdef MONGOC_HAVE_STDBOOL_H
#include <stdbool.h>
#elif !defined(__bool_true_false_are_defined)
#ifndef __cplusplus
typedef signed char bool;
#define false 0
#define true 1
#endif // __cplusplus
#define __bool_true_false_are_defined 1
#endif //MONGOC_HAVE_STDBOOL_H

#endif // __MONGODB_YGGR_EX_STDBOOL_H__