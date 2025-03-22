#include "LocalBCA.h"

#include "std_warn_off.h"
#include <sstream>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// used as a generator for raw BoundIO's
GenerateBoundIOs COLS;

// this call looks like a BoundIOs subscript call, but it generates
// a BoundIO which is not bound to any parent BoundIOs object
BoundIO GenerateBoundIOs::operator[](const tstring &name) const
{
	return BoundIO(name, BoundIO::COLUMN); // note: private constructor called
}

BoundIO GenerateBoundIOs::operator[](unsigned int paramNum) const
{
	tostringstream nmStream;

	nmStream << paramNum;

	return BoundIO(nmStream.str(), BoundIO::PARAM);
}

END_DTL_NAMESPACE

