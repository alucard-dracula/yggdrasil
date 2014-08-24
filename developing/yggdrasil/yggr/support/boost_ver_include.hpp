//boost_ver_include.hpp

#ifndef __YGGR_SUPPORT_BOOST_VER_INCLUDE_HPP__
#define __YGGR_SUPPORT_BOOST_VER_INCLUDE_HPP__

#include <boost/config.hpp>
#include <boost/version.hpp>

#define YGGR_PP_BOOST_VERSION() BOOST_VERSION

#define YGGR_PP_BOOST_SRC_INCLUDE(__d__, __v__, __f__) <__d__/__v__/__f__>

#endif // __YGGR_SUPPORT_BOOST_VER_INCLUDE_HPP__
