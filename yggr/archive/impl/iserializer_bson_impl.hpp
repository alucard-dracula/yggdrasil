//iserializer_bson_impl.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_ARCHIVE_IMPL_ISERIALIZER_BSON_IMPL_HPP__
#define __YGGR_ARCHIVE_IMPL_ISERIALIZER_BSON_IMPL_HPP__

#include <yggr/serialization/nvp.hpp>
#include <boost/archive/detail/iserializer.hpp>

namespace boost
{
namespace archive
{
namespace detail
{

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

#if BOOST_VERSION < 105600

template<bool skip_auto_id, typename T>
class pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>
	: public basic_pointer_iserializer
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

#else

template<bool skip_auto_id, typename T>
class pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>
	: public basic_pointer_iserializer
{
private:
	typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;

private:
    virtual void * heap_allocation() const {
        detail::heap_allocation<T> h;
        T * t = h.get();
        h.release();
        return t;
    }
    virtual const basic_iserializer & get_basic_serializer() const {
        return boost::serialization::singleton<
            iserializer<archive_type, T>
        >::get_const_instance();
    }
    BOOST_DLLEXPORT virtual void load_object_ptr(
        basic_iarchive & ar, 
        void * x,
        const unsigned int file_version
    ) const BOOST_USED;
protected:
    // this should alway be a singleton so make the constructor protected
    pointer_iserializer();
    ~pointer_iserializer();
};

#endif // BOOST_VERSION < 105600

#ifdef BOOST_MSVC
#  pragma warning(pop)
#endif // BOOST_MSVC

#if BOOST_VERSION < 105600

// note: BOOST_DLLEXPORT is so that code for polymorphic class
// serialized only through base class won't get optimized out
template<bool skip_auto_id, typename T>
BOOST_DLLEXPORT void pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::load_object_ptr(
    basic_iarchive & ar, 
    void * & x,
    const unsigned int file_version
) const
{
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

	ar_impl >> YGGR_SERIALIZATION_NAME_NVP("ptr_value", *t);
    ap.release();
}

#else

// note: BOOST_DLLEXPORT is so that code for polymorphic class
// serialized only through base class won't get optimized out
template<bool skip_auto_id, typename T>
BOOST_DLLEXPORT void pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::load_object_ptr(
    basic_iarchive & ar, 
    void * t,
    const unsigned int file_version
) const
{
	 archive_type & ar_impl = 
        boost::serialization::smart_cast_reference<archive_type &>(ar);

    // note that the above will throw std::bad_alloc if the allocation
    // fails so we don't have to address this contingency here.

    // catch exception during load_construct_data so that we don't
    // automatically delete the t which is most likely not fully
    // constructed
    BOOST_TRY {
        // this addresses an obscure situation that occurs when 
        // load_constructor de-serializes something through a pointer.
        ar.next_object_pointer(t);
        boost::serialization::load_construct_data_adl<archive_type, T>(
            ar_impl,
            static_cast<T *>(t),
            file_version
        );
    }
    BOOST_CATCH(...){
        // if we get here the load_construct failed.  The heap_allocation
        // will be automatically deleted so we don't have to do anything
        // special here.
        BOOST_RETHROW;
    }
    BOOST_CATCH_END

    ar_impl >> boost::serialization::make_nvp("ptr_value", *static_cast<T *>(t));
}

#endif // BOOST_VERSION < 105600

template<bool skip_auto_id, typename T>
pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::pointer_iserializer() :
    basic_pointer_iserializer(
        boost::serialization::singleton<
            typename 
            boost::serialization::type_info_implementation< T >::type
        >::get_const_instance()
    )
{
    boost::serialization::singleton<
        iserializer<archive_type, T>
    >::get_mutable_instance().set_bpis(this);
    archive_serializer_map<archive_type>::insert(this);
}

template<bool skip_auto_id, typename T>
pointer_iserializer<yggr::archive::bson_iarchive<skip_auto_id>, T>::~pointer_iserializer(){
    archive_serializer_map<archive_type>::erase(this);
}

} // namespace detail
} // namespace archive
} // namespace boost

#endif // __YGGR_ARCHIVE_IMPL_ISERIALIZER_BSON_IMPL_HPP__