// oserializer_patch.hpp

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

#ifndef __YGGR_ARCHIVE_DETAIL_OSERIALIZER_PATCH_HPP__
#define __YGGR_ARCHIVE_DETAIL_OSERIALIZER_PATCH_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/ref.hpp>
#include <boost/archive/detail/oserializer.hpp>

namespace boost
{
namespace archive
{
namespace detail
{

template<class Archive>
struct save_non_pointer_patch_type {
    // note this bounces the call right back to the archive
    // with no runtime overhead
    struct save_primitive {
        template<class T> inline
        static void invoke(Archive & ar, const T & t){
            save_access::save_primitive(ar, t);
        }
    };
    // same as above but passes through serialization
    struct save_only {
        template<class T> inline
        static void invoke(Archive & ar, const T & t){
            // make sure call is routed through the highest interface that might
            // be specialized by the user.
            ::boost::serialization::serialize_adl(
                ar, 
                const_cast<T &>(t), 
                ::boost::serialization::version< T >::value
            );
        }
    };
    // adds class information to the archive. This includes
    // serialization level and class version
    struct save_standard {
        template<class T> inline
        static void invoke(Archive &ar, const T & t){
            ar.save_object(
                ::boost::addressof(t), 
                ::boost::serialization::singleton<
                    oserializer<Archive, T>
                >::get_const_instance()
            );
        }
    };

    // adds class information to the archive. This includes
    // serialization level and class version
    struct save_conditional {
        template<class T> inline
        static void invoke(Archive &ar, const T &t){
            //if(0 == (ar.get_flags() & no_tracking))
                save_standard::invoke(ar, t);
            //else
            //   save_only::invoke(ar, t);
        }
    };


    template<class T>
    static void invoke(Archive & ar, const T & t){
        typedef 
            BOOST_DEDUCED_TYPENAME mpl::eval_if<
            // if its primitive
                mpl::equal_to<
                    ::boost::serialization::implementation_level< T >,
                    mpl::int_< ::boost::serialization::primitive_type>
                >,
                mpl::identity<save_primitive>,
            // else
            BOOST_DEDUCED_TYPENAME mpl::eval_if<
                // class info / version
                mpl::greater_equal<
                    ::boost::serialization::implementation_level< T >,
                    mpl::int_< ::boost::serialization::object_class_info>
                >,
                // do standard save
                mpl::identity<save_standard>,
            // else
            BOOST_DEDUCED_TYPENAME mpl::eval_if<
                    // no tracking
                mpl::equal_to<
                    ::boost::serialization::tracking_level< T >,
                    mpl::int_< ::boost::serialization::track_never>
                >,
                // do a fast save
                mpl::identity<save_only>,
            // else
                // do a fast save only tracking is turned off
                mpl::identity<save_conditional>
            > > >::type typex; 
        check_object_versioning< T >();
        typex::invoke(ar, t);
    }
    template<class T>
    static void invoke(Archive & ar, T & t){
        check_object_level< T >();
        check_object_tracking< T >();
        invoke(ar, const_cast<const T &>(t));
    }
};


} // namespace detail
} // namespace archive
} // namespace boost


/*
useage:
#define YGGR_PP_ARCHIVE_TYPE() archive
	YGGR_PP_BOOST_OSERIALIZER_PATCH( YGGR_PP_ARCHIVE_TYPE )
#undef YGGR_PP_ARCHIVE_TYPE
*/

#define YGGR_PP_BOOST_OSERIALIZER_PATCH( __archive_t__ ) \
	namespace boost{ namespace archive{ namespace detail { \
	template< > \
	struct save_non_pointer_type< __archive_t__() > \
		: public save_non_pointer_patch_type< __archive_t__() >{ }; }}}

#endif // __YGGR_ARCHIVE_DETAIL_OSERIALIZER_PATCH_HPP__