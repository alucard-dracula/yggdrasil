// buffer_real_value_type_traits.hpp

#ifndef __YGGR_ARCHIVE_BUFFER_REAL_VALUE_TYPE_TRAITS_HPP__
#define __YGGR_ARCHIVE_BUFFER_REAL_VALUE_TYPE_TRAITS_HPP__

namespace yggr
{
namespace archive
{

template<typename T>
struct buffer_real_value
{
	typedef T type;
};

} // namespace archive
} // namespace yggr

#define YGGR_ARCHIVE_BUFFER_REAL_VALUE_DEF( __src__, __dst__) \
	namespace yggr{ namespace archive { \
		template<> struct buffer_real_value< __src__ > { \
			 typedef __dst__ type; }; } }

#endif // __YGGR_ARCHIVE_BUFFER_REAL_VALUE_TYPE_TRAITS_HPP__