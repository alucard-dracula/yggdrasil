//pointer_checker_of_wrapper.hpp

#ifndef __YGGR_MPLEX_DETAIL_POINTER_CHECKER_OF_WRAPPER_HPP__
#define __YGGR_MPLEX_DETAIL_POINTER_CHECKER_OF_WRAPPER_HPP__

#ifndef __YGGR_MPLEX_POINTER_CHECK_HPP__
#error "please include yggr/mplex/pointer_checker.hpp before this file!!!"
#endif // __YGGR_MPLEX_POINTER_CHECK_HPP__

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <yggr/base/interface_ptr.hpp>

YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::shared_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::shared_ptr<T> const)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::shared_ptr<T> volatile)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::shared_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::scoped_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::scoped_ptr<T> const)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::scoped_ptr<T> volatile)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::scoped_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::weak_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::weak_ptr<T> const)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::weak_ptr<T> volatile)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::weak_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::intrusive_ptr<T>)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::intrusive_ptr<T> const)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::intrusive_ptr<T> volatile)
YGGR_PP_IS_POINTER_WRAPPER_DEF(boost::intrusive_ptr<T> const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

namespace yggr
{
namespace mplex
{

template<typename T, 
			template<typename _T> 
				class SPtr>
struct is_pointer< yggr::interface_ptr<T, SPtr> >
	: public boost::mpl::true_
{
};

#if !defined(BOOST_NO_CV_SPECIALIZATIONS)

template<typename T, 
			template<typename _T> 
				class SPtr>
struct is_pointer< yggr::interface_ptr<T, SPtr> const >
	: public boost::mpl::true_
{
};

template<typename T, 
			template<typename _T> 
				class SPtr>
struct is_pointer< yggr::interface_ptr<T, SPtr> volatile>
	: public boost::mpl::true_
{
};

template<typename T, 
			template<typename _T> 
				class SPtr>
struct is_pointer< yggr::interface_ptr<T, SPtr> const volatile>
	: public boost::mpl::true_
{
};
#endif //BOOST_NO_CV_SPECIALIZATIONS

} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_DETAIL_POINTER_CHECKER_OF_WRAPPER_HPP__