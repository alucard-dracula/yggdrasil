#include "VariantException.h"

BEGIN_DTL_NAMESPACE

// *********** Implementation code for VariantException *************
VariantException::VariantException(const tstring &meth, const tstring &msg) :
	  RootException(meth, msg, _TEXT("VariantException")) { }

END_DTL_NAMESPACE
