//iserializer_bson_impl.hpp

#ifndef __YGGR_ARCHIVE_IMPL_ISERIALIZER_BSON_IMPL_HPP__
#define __YGGR_ARCHIVE_IMPL_ISERIALIZER_BSON_IMPL_HPP__

#include <boost/archive/detail/iserializer.hpp>
//#include <yggr/serialization/detail/bson_archive_compile_def.hpp>

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

template<bool skip_auto_id, typename T>
class pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>:
    public basic_pointer_iserializer
{
private:
	typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;
private:
    virtual const basic_iserializer & get_basic_serializer() const {
        return boost::serialization::singleton<
            iserializer<archive_type, T>
        >::get_const_instance();
    }
    BOOST_DLLEXPORT virtual void load_object_ptr(
        basic_iarchive & ar, 
        void * & x,
        const unsigned int file_version
    ) const BOOST_USED;
protected:
    // this should alway be a singleton so make the constructor protected
    pointer_iserializer();
    ~pointer_iserializer();
};

#ifdef BOOST_MSVC
#  pragma warning(pop)
#endif

// note: BOOST_DLLEXPORT is so that code for polymorphic class
// serialized only through base class won't get optimized out
template<bool skip_auto_id, typename T>
BOOST_DLLEXPORT void pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::load_object_ptr(
    basic_iarchive & ar, 
    void * & x,
    const unsigned int file_version
) const
{
	typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;

    archive_type & ar_impl = 
        boost::serialization::smart_cast_reference<archive_type &>(ar);

    auto_ptr_with_deleter< T > ap(heap_allocator< T >::invoke());
    if(NULL == ap.get())
        boost::serialization::throw_exception(std::bad_alloc()) ;

    T * t = ap.get();
    x = t;

    // catch exception during load_construct_data so that we don't
    // automatically delete the t which is most likely not fully
    // constructed
    BOOST_TRY {
        // this addresses an obscure situtation that occurs when 
        // load_constructor de-serializes something through a pointer.
        ar.next_object_pointer(t);
        boost::serialization::load_construct_data_adl<archive_type, T>(
            ar_impl,
            t, 
            file_version
        );
    }
    BOOST_CATCH(...){
        ap.release();
        BOOST_RETHROW;
    }
    BOOST_CATCH_END

    ar_impl >> boost::serialization::make_nvp("ptr_value", * t);
    ap.release();
}

template<bool skip_auto_id, class T>
pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::pointer_iserializer() :
    basic_pointer_iserializer(
        boost::serialization::singleton<
            BOOST_DEDUCED_TYPENAME 
            boost::serialization::type_info_implementation< T >::type
        >::get_const_instance()
    )
{
	typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;

    boost::serialization::singleton<
        iserializer<archive_type, T>
    >::get_mutable_instance().set_bpis(this);
    archive_serializer_map<archive_type>::insert(this);
}

template<bool skip_auto_id, class T>
pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::~pointer_iserializer(){
	typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;

    archive_serializer_map<archive_type>::erase(this);
}


} // namespace detail
} // namespace archive
} // namespace boost

#endif // __YGGR_ARCHIVE_IMPL_ISERIALIZER_BSON_IMPL_HPP__