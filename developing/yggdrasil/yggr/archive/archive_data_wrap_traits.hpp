//archive_data_wrap_traits.hpp

#ifndef __YGGR_ARCHIVE_ARCHIVE_DATA_WRAP_TRAITS_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_DATA_WRAP_TRAITS_HPP__

#include <string>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace archive
{

template<typename Archive>
struct binary_data_wrap_traits
	: public boost::mpl::false_
{
	typedef std::string type;
};

} // namespace archve
} // namespace 

#define YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF( __archive__, __wrap__ ) \
	namespace yggr { namespace archive { \
	template<> struct binary_data_wrap_traits< __archive__ > \
		: public boost::mpl::true_ { typedef __wrap__ type; }; }}

#define YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS( __archive__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__ >::type

#define YGGR_PP_ARCHIVE_NEED_BINARY_DATA_WRAP_TRAITS( __archive__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__ >::value

#define YGGR_PP_ARCHIVE_NEED_BINARY_DATA_WRAP_TRAITS_T( __archive__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__ >

#endif //__YGGR_ARCHIVE_ARCHIVE_DATA_WRAP_TRAITS_HPP__