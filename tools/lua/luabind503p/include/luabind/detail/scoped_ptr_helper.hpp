// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef LUABIND_SCOPED_PTR_HELPER_HPP_INCLUDED
#define LUABIND_SCOPED_PTR_HELPER_HPP_INCLUDED

#include <boost/scoped_ptr.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>

namespace luabind
{
namespace detail
{
namespace scoped_ptr_helper
{

template<typename T>
class fake_scoped_ptr
{
public:
    T * px;
};

template <class T> inline
fake_scoped_ptr<T>& cast_to_fake_scoped_ptr(boost::scoped_ptr<T>& p)
{
	typedef boost::scoped_ptr<T> org_type;
	typedef fake_scoped_ptr<T> fake_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(org_type) == sizeof(fake_type))>));

	return reinterpret_cast<fake_type&>(p);
}

template <class T> inline
void release_ownership_scoped_ptr(boost::scoped_ptr<T>& p)
{
	cast_to_fake_scoped_ptr(p).px = 0;
}

template <class T> inline
T* release_ownership_trans_scoped_ptr(const boost::scoped_ptr<T>& p)
{
	typedef T* ptr_type;
    typedef boost::scoped_ptr<T> org_type;
	typedef fake_scoped_ptr<T> fake_type;

	fake_type& fake_ref = cast_to_fake_scoped_ptr(const_cast<org_type&>(p));
	ptr_type ret = fake_ref.px;
	fake_ref.px = 0;
	return ret;
}

} // namespace scoped_ptr_helper

} // namespace detail
} // namespace luabind

#endif // LUABIND_SCOPED_PTR_HELPER_HPP_INCLUDED

