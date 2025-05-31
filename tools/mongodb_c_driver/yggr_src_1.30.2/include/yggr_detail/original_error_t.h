// original_error_t.h

#ifndef __MONGODB_YGGR_EX_ORGINAL_ERROR_T_H__
#define __MONGODB_YGGR_EX_ORGINAL_ERROR_T_H__

typedef struct 
{
	bson_t reply;
	bson_error_t error;
	bool set;
} original_error_t;

#endif // __MONGODB_YGGR_EX_ORGINAL_ERROR_T_H__