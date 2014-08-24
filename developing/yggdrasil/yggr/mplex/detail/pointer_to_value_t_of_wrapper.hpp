//pointer_to_value_t_of_wrapper.hpp

#ifndef __YGGR_MPLEX_DETAIL_POINTER_TO_VALUE_T_OF_WRAPPER_HPP__
#define __YGGR_MPLEX_DETAIL_POINTER_TO_VALUE_T_OF_WRAPPER_HPP__

#ifndef __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__
#error "please include yggr/mplex/pointer_to_value_t.hpp before this file!!!"
#endif // __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <yggr/base/interface_ptr.hpp>

YGGR_PP_POINTER_UNWRAP_DEF(boost::shared_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_DEF(boost::shared_ptr<T> const)
YGGR_PP_POINTER_UNWRAP_DEF(boost::shared_ptr<T> volatile)
YGGR_PP_POINTER_UNWRAP_DEF(boost::shared_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_DEF(boost::scoped_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_DEF(boost::scoped_ptr<T> const)
YGGR_PP_POINTER_UNWRAP_DEF(boost::scoped_ptr<T> volatile)
YGGR_PP_POINTER_UNWRAP_DEF(boost::scoped_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_DEF(boost::weak_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_DEF(boost::weak_ptr<T> const)
YGGR_PP_POINTER_UNWRAP_DEF(boost::weak_ptr<T> volatile)
YGGR_PP_POINTER_UNWRAP_DEF(boost::weak_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_DEF(boost::intrusive_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_DEF(boost::intrusive_ptr<T> const)
YGGR_PP_POINTER_UNWRAP_DEF(boost::intrusive_ptr<T> volatile)
YGGR_PP_POINTER_UNWRAP_DEF(boost::intrusive_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

namespace yggr
{
namespace mplex
{
} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_DETAIL_POINTER_TO_VALUE_T_OF_WRAPPER_HPP__