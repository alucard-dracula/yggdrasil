// iserializer_patch.hpp

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

#ifndef __YGGR_ARCHIVE_DETAIL_ISERIALIZER_PATCH_HPP__
#define __YGGR_ARCHIVE_DETAIL_ISERIALIZER_PATCH_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/ref.hpp>
#include <boost/archive/detail/iserializer.hpp>

namespace boost
{ 
namespace archive
{
namespace detail 
{

template<class Archive>
struct load_non_pointer_patch_type {
    // note this bounces the call right back to the archive
    // with no runtime overhead
    struct load_primitive {
        template<class T> inline
        static void invoke(Archive & ar, T & t){
            load_access::load_primitive(ar, t);
        }
    };
    // note this bounces the call right back to the archive
    // with no runtime overhead
    struct load_only {
        template<class T> inline
        static void invoke(Archive & ar, const T & t){
            // short cut to user's serializer
            // make sure call is routed through the higest interface that might
            // be specialized by the user.
            ::boost::serialization::serialize_adl(
                ar, 
                const_cast<T &>(t), 
                ::boost::serialization::version< T >::value
            );
        }
    };

    // note this save class information including version
    // and serialization level to the archive
    struct load_standard {
        template<class T> inline
        static void invoke(Archive &ar, const T & t){
            void * x = ::boost::addressof(const_cast<T &>(t));
            ar.load_object(
                x, 
                ::boost::serialization::singleton<
                    iserializer<Archive, T>
                >::get_const_instance()
            );
        }
    };

    struct load_conditional {
        template<class T> inline
        static void invoke(Archive &ar, T &t){
            //if(0 == (ar.get_flags() & no_tracking))
                load_standard::invoke(ar, t);
            //else
            //    load_only::invoke(ar, t);
        }
    };

    template<class T> inline
    static void invoke(Archive & ar, T &t){
        typedef typename mpl::eval_if<
                // if its primitive
                mpl::equal_to<
                    ::boost::serialization::implementation_level< T >,
                    mpl::int_< ::boost::serialization::primitive_type>
                >,
                mpl::identity<load_primitive>,
            // else
            typename mpl::eval_if<
            // class info / version
            mpl::greater_equal<
                        ::boost::serialization::implementation_level< T >,
                        mpl::int_< ::boost::serialization::object_class_info>
                    >,
            // do standard load
            mpl::identity<load_standard>,
        // else
        typename mpl::eval_if<
            // no tracking
                    mpl::equal_to<
                        ::boost::serialization::tracking_level< T >,
                        mpl::int_< ::boost::serialization::track_never>
                >,
                // do a fast load
                mpl::identity<load_only>,
            // else
            // do a fast load only tracking is turned off
            mpl::identity<load_conditional>
        > > >::type typex;
        check_object_versioning< T >();
        check_object_level< T >();
        typex::invoke(ar, t);
    }
};

} // namespace detail
} // namespace arcive
} // namespace boost

/*
useage:
#define YGGR_PP_ARCHIVE_TYPE() archive
	YGGR_PP_BOOST_ISERIZER_PATCH( YGGR_PP_ARCHIVE_TYPE )
#undef YGGR_PP_ARCHIVE_TYPE
*/

#define YGGR_PP_BOOST_ISERIALIZER_PATCH( __archive_t__ ) \
	namespace boost{ namespace archive{ namespace detail { \
	template< > \
	struct load_non_pointer_type< __archive_t__() > \
		: public load_non_pointer_patch_type<__archive_t__()>{ }; }}}


#endif // __YGGR_ARCHIVE_DETAIL_ISERIALIZER_PATCH_HPP__
