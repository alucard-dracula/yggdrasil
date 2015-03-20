//export.hpp

#ifndef __YGGR_SERIALIZATION_EXPORT_HPP__
#define __YGGR_SERIALIZATION_EXPORT_HPP__

#include <boost/serialization/export.hpp>

#define YGGR_CLASS_EXPORT BOOST_CLASS_EXPORT
#define YGGR_CLASS_EXPORT_GUID BOOST_CLASS_EXPORT_GUID
#define YGGR_CLASS_EXPORT_KEY BOOST_CLASS_EXPORT_KEY
#define YGGR_CLASS_EXPORT_KEY2 BOOST_CLASS_EXPORT_KEY2
#define YGGR_CLASS_EXPORT_IMPLEMENT BOOST_CLASS_EXPORT_IMPLEMENT

namespace yggr
{
namespace serialization
{
} // namespace serialization
} // namespace yggr

#endif // __YGGR_SERIALIZATION_EXPORT_HPP__