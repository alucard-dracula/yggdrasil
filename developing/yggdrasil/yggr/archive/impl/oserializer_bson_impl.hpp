//oserializer_bson_impl.hpp

#ifndef __YGGR_ARCHIVE_IMPL_OSERIALIZER_BSON_IMPL_HPP__
#define __YGGR_ARCHIVE_IMPL_OSERIALIZER_BSON_IMPL_HPP__

#include <boost/archive/detail/oserializer.hpp>

namespace boost
{
namespace archive
{
namespace detail
{

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

template<bool nld, class T>
class pointer_oserializer< yggr::archive::bson_oarchive<nld>, T >:
    public basic_pointer_oserializer
{
private:
	typedef yggr::archive::bson_oarchive<nld> archive_type;
private:
    const basic_oserializer & 
    get_basic_serializer() const {
        return boost::serialization::singleton<
            oserializer<archive_type, T>
        >::get_const_instance();
    }
    virtual BOOST_DLLEXPORT void save_object_ptr(
        basic_oarchive & ar,
        const void * x
    ) const BOOST_USED;
public:
    pointer_oserializer();
    ~pointer_oserializer();
};

#ifdef BOOST_MSVC
#  pragma warning(pop)
#endif

template<bool nld, class T>
BOOST_DLLEXPORT void pointer_oserializer<yggr::archive::bson_oarchive<nld>, T>::save_object_ptr(
    basic_oarchive & ar,
    const void * x
) const {
	typedef yggr::archive::bson_oarchive<nld> archive_type;
    BOOST_ASSERT(NULL != x);
    // make sure call is routed through the highest interface that might
    // be specialized by the user.
    T * t = static_cast<T *>(const_cast<void *>(x));
    const unsigned int file_version = boost::serialization::version< T >::value;
    archive_type & ar_impl 
        = boost::serialization::smart_cast_reference<archive_type &>(ar);
    boost::serialization::save_construct_data_adl<archive_type, T>(
        ar_impl, 
        t, 
        file_version
    );
    ar_impl << boost::serialization::make_nvp("ptr_value", * t);
}

template<bool nld, class T>
pointer_oserializer<yggr::archive::bson_oarchive<nld>, T>::pointer_oserializer() :
    basic_pointer_oserializer(
        boost::serialization::singleton<
            BOOST_DEDUCED_TYPENAME 
            boost::serialization::type_info_implementation< T >::type
        >::get_const_instance()
    )
{
	typedef yggr::archive::bson_oarchive<nld> archive_type;
    // make sure appropriate member function is instantiated
    boost::serialization::singleton<
        oserializer<archive_type, T> 
    >::get_mutable_instance().set_bpos(this);
    archive_serializer_map<archive_type>::insert(this);
}

template<bool nld, class T>
pointer_oserializer<yggr::archive::bson_oarchive<nld>, T>::~pointer_oserializer(){
	typedef yggr::archive::bson_oarchive<nld> archive_type;
    archive_serializer_map<archive_type>::erase(this);
}

} // namespace detail
} // namespace archive
} // namespace boost

#endif // __YGGR_ARCHIVE_IMPL_OSERIALIZER_BSON_IMPL_HPP__