// warning_check.h

#ifndef __MONGODB_YGGR_EX_WARNING_CHECK_H__
#define __MONGODB_YGGR_EX_WARNING_CHECK_H__

#ifdef _MSC_VER

//C4002: too many actual parameters for macro 'identifier'
//C4003: not enough actual parameters for macro
//C4005: 'macro' : macro redefinition
//C4006: #undef expected an identifier
//C4067: unexpected tokens following preprocessor directive - expected a newline
//C4099: 'identifier' : type name first seen using 'objecttype1' now seen using 'objecttype2'
//C4101: 'identifier' : unreferenced local variable
//C4114: same type qualifier used more than once
//C4141: 'inline' : used more than once
//C4172: returning address of local variable or temporary
//C4293: '<< or >>' : shift count negative or too big, undefined behavior
//C4390: ';' : empty controlled statement found; is this the intent?
//C4552: 'op_symbol' operator has no effect; expected operator with side-effect
//C4700: uninitialized local variable 'name' used
//C4715: 'function' : not all control paths return a value
//C4717: 'function' : recursive on all control paths, function will cause runtime stack overflow
//C4068: unknown pragma

#pragma warning (error : 4002)
#pragma warning (error : 4003)
#pragma warning (error : 4005)
#pragma warning (error : 4006)
#pragma warning (error : 4067)
#pragma warning (error : 4068)
#pragma warning (error : 4099)
#pragma warning (error : 4101)
#pragma warning (error : 4114)
#pragma warning (error : 4141)
#pragma warning (error : 4172)
#pragma warning (error : 4293)
#pragma warning (error : 4390)
#pragma warning (error : 4552)
#pragma warning (error : 4700)
#pragma warning (error : 4715)
#pragma warning (error : 4717)

#endif // _MSC_VER

#endif // __MONGODB_YGGR_EX_WARNING_CHECK_H__