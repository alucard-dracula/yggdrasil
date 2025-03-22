//oserializer_bson_impl.hpp

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

#ifndef __YGGR_ARCHIVE_IMPL_OSERIALIZER_BSON_IMPL_HPP__
#define __YGGR_ARCHIVE_IMPL_OSERIALIZER_BSON_IMPL_HPP__

#include <yggr/serialization/nvp.hpp>
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
#endif // BOOST_MSVC

template<bool nld, typename T>
class pointer_oserializer< yggr::archive::bson_oarchive<nld>, T >:
    public basic_pointer_oserializer
{
private:
	typedef yggr::archive::bson_oarchive<nld> archive_type;
private:
   inline const basic_oserializer & 
    get_basic_serializer() const {
        return boost::serialization::singleton<
            oserializer<archive_type, T>
        >::get_const_instance();
    }
    BOOST_DLLEXPORT virtual void save_object_ptr(
        basic_oarchive & ar,
        const void * x
    ) const BOOST_USED;
public:
    pointer_oserializer();
    ~pointer_oserializer();
};

#ifdef BOOST_MSVC
#  pragma warning(pop)
#endif // BOOST_MSVC

template<bool nld, typename T>
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

	ar_impl << YGGR_SERIALIZATION_NAME_NVP("ptr_value", *t);
}

template<bool nld, typename T>
pointer_oserializer<yggr::archive::bson_oarchive<nld>, T>::pointer_oserializer() :
    basic_pointer_oserializer(
        boost::serialization::singleton<
            typename 
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

template<bool nld, typename T>
pointer_oserializer<yggr::archive::bson_oarchive<nld>, T>::~pointer_oserializer(){
	typedef yggr::archive::bson_oarchive<nld> archive_type;
    archive_serializer_map<archive_type>::erase(this);
}

} // namespace detail
} // namespace archive
} // namespace boost

#endif // __YGGR_ARCHIVE_IMPL_OSERIALIZER_BSON_IMPL_HPP__