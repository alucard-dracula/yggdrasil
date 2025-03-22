//archive_nvp.hpp

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

#ifndef __YGGR_SERIALIZATION_NVP_HPP__
#define __YGGR_SERIALIZATION_NVP_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/serialization/nvp.hpp>

#include <boost/static_assert.hpp>
#include <boost/ref.hpp>

namespace yggr
{
namespace serialization
{

using boost::serialization::nvp;

template<typename T>
struct fake_nvp
	: public std::pair<const char*, T*>,
		public boost::serialization::wrapper_traits<const nvp< T > >
{
	typedef std::pair<const char*, T*> base_type;

	explicit fake_nvp(const char* name_, T& t)
		: base_type(name_, boost::addressof(t))
	{
	}
	
	fake_nvp(const fake_nvp& rhs) 
		: base_type(rhs.first, rhs.second)
	{
	}

	inline operator 
#if (!defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)) || (BOOST_VERSION >= 105900)
		const
#endif // (!defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)) || (BOOST_VERSION >= 105900)
		nvp<T>(void) const
	{
		return *(reinterpret_cast< const nvp<T>* >(this));
	}
};


#if (defined(__clang__) && __clang__)
template<class T> inline
#if (!defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)) || (BOOST_VERSION >= 105900)
const
#endif // (!defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)) || (BOOST_VERSION >= 105900)
nvp<T> make_nvp(const char* name, T& t)
{
	typedef nvp<T> ret_type;
    return ret_type(name, t);
}

#else

template<class T> inline
#if (!defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)) || (BOOST_VERSION >= 105900)
const
#endif // (!defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)) || (BOOST_VERSION >= 105900)
nvp<T> make_nvp(const char* name, T& t)
{
	typedef nvp<T> ret_type;
	typedef fake_nvp<T> fake_type;
	BOOST_STATIC_ASSERT_MSG((sizeof(ret_type) == sizeof(fake_type)), 
								"sizeof(ret_type) must equal_to sizeof(fake_type)");
    return static_cast<ret_type>(fake_type(name, t));
}

#endif // #if (defined(__clang__) && __clang__)

} // namespace serialization
} // namespace yggr

//#define YGGR_SERIALIZATION_NVP BOOST_SERIALIZATION_NVP
//#define YGGR_SERIALIZATION_BASE_OBJECT_NVP BOOST_SERIALIZATION_BASE_OBJECT_NVP

#define YGGR_SERIALIZATION_NVP( __name__ ) \
	yggr::serialization::make_nvp(BOOST_PP_STRINGIZE( __name__ ), __name__)


#define YGGR_SERIALIZATION_BASE_OBJECT_NVP( __name__ ) \
	yggr::serialization::make_nvp( \
		BOOST_PP_STRINGIZE( __name__ ), \
		yggr::serialization::base_object< __name__ >(*this) )


#define YGGR_SERIALIZATION_NAME_NVP( __name__, __val__ ) \
	yggr::serialization::make_nvp( __name__, __val__ )

#define YGGR_SERIALIZATION_NVP_AR( __ar__, __val__ ) \
    __ar__ & YGGR_SERIALIZATION_NVP( __val__ )

#define YGGR_SERIALIZATION_NAME_NVP_AR( __ar__, __name__, __val__ ) \
    __ar__ & YGGR_SERIALIZATION_NAME_NVP( __name__, __val__ )


#define YGGR_SERIALIZATION_WRAP_NVP_AR( __ar__, __wrap__, __val__ ) \
	{ __wrap__ wrap_obj_tmp( __val__ ); __ar__ & yggr::serialization::make_nvp(BOOST_PP_STRINGIZE( __val__ ), wrap_obj_tmp); }

#define YGGR_SERIALIZATION_NAME_WRAP_NVP_AR( __name__, __wrap__, __val__ ) \
	{ __wrap__ wrap_obj_tmp( __val__ ); __ar__ & yggr::serialization::make_nvp( __name__, wrap_obj_tmp ); }

#endif //__YGGR_SERIALIZATION_NVP_HPP__
