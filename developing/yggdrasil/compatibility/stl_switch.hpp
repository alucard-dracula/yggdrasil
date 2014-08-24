//stl_switch.hpp

#ifndef __COMPATIBILITY_STL_SWITCH_HPP__
#define __COMPATIBILITY_STL_SWITCH_HPP__

#define YGGR_USE_MICROSOFT_STL 1
#define YGGR_USE_GNUC_STL 2
#define YGGR_USE_SGI_STL 3 //it non tested

#if !defined(YGGR_USE_STL_VER) || (YGGR_USE_STL_VER < 0) || (YGGR_USE_STL_VER > 3)
#	if defined(_MSC_VER)
#		define YGGR_USE_STL_VER YGGR_USE_MICROSOFT_STL
#	elif defined (__GNUC__)
#		define YGGR_USE_STL_VER YGGR_USE_GNUC_STL
#	else
#		define YGGR_USE_STL_VER 0
#		error "add now use stl version"
#	endif // _MSC_VER
#endif // YGGR_USE_STL_VER


#endif //__COMPATIBILITY_STL_SWITCH_HPP__