//export.hpp

#ifndef __YGGR_SERIALIZATION_EXPORT_HPP__
#define __YGGR_SERIALIZATION_EXPORT_HPP__

#include <boost/serialization/export.hpp>

#define YGGR_CLASS_EXPORT BOOST_CLASS_EXPORT
#define YGGR_CLASS_EXPORT_GUID BOOST_CLASS_EXPORT_GUID
#define YGGR_CLASS_EXPORT_KEY2 BOOST_CLASS_EXPORT_KEY2

#define YGGR_CLASS_EXPORT_IMPLEMENT BOOST_CLASS_EXPORT_IMPLEMENT

#define YGGR_CLASS_EXPORT_IMPLEMENT_DECLEAR( T ) \
	namespace boost { \
    namespace archive { \
    namespace detail { \
    namespace extra_detail { \
    template<> \
    struct init_guid< T > { \
        static guid_initializer< T > const & g; \
	}; }}}}

#define YGGR_CLASS_EXPORT_IMPLEMENT_IMPL( T ) \
	namespace boost { \
    namespace archive { \
    namespace detail { \
    namespace extra_detail { \
	 guid_initializer< T > const & init_guid< T >::g = \
        ::boost::serialization::singleton< \
            guid_initializer< T > \
        >::get_mutable_instance().export_guid(); \
    }}}}

#define YGGR_CLASS_EXPORT_GUID_DECLEAR( T, K ) \
	YGGR_CLASS_EXPORT_KEY2( T, K ) \
	YGGR_CLASS_EXPORT_IMPLEMENT_DECLEAR( T )

#define YGGR_CLASS_EXPORT_GUID_IMPL( T ) \
	YGGR_CLASS_EXPORT_IMPLEMENT_IMPL( T )

#define YGGR_CLASS_EXPORT_DECLEAR( T ) \
	YGGR_CLASS_EXPORT_GUID_DECLEAR( T, BOOST_PP_STRINGIZE(T))

#define YGGR_CLASS_EXPORT_IMPL( T ) \
	YGGR_CLASS_EXPORT_IMPLEMENT_IMPL( T )


namespace yggr
{
namespace serialization
{
} // namespace serialization
} // namespace yggr

#endif // __YGGR_SERIALIZATION_EXPORT_HPP__