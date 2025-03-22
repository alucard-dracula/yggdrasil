// detail interface_ptr.hpp

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

#ifndef __YGGR_SMART_PTR_EX_DETAIL_INTERFACE_PTR_HPP__
#define __YGGR_SMART_PTR_EX_DETAIL_INTERFACE_PTR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/nvp.hpp>

#include <boost/ref.hpp>
#include <boost/serialization/base_object.hpp>

#include <cassert>

namespace yggr
{
namespace smart_ptr_ex
{
namespace detail
{

template<typename T,
			template<typename _T>
				class SPtr = boost::shared_ptr >
class interface_ptr
	: public SPtr<T>
{
public:
	typedef SPtr<T> base_type;

protected:
	typedef typename smart_ptr_ex::shared_ptr_element_t<base_type>::type obj_type;
	typedef obj_type* ptr_type;

public:
	typedef obj_type value_type;
	typedef obj_type element_type;
	typedef ptr_type pointer;

private:
	typedef interface_ptr this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	interface_ptr(void)
	{
	}

	template<typename Y> explicit
	interface_ptr(Y* p)
		: base_type(construct_shared<obj_type>(p))
	{
	}

	template<typename Y, typename D>
	interface_ptr(Y* p, D d)
		: base_type(construct_shared<obj_type>(p, d))
	{
	}

	template<typename Y, typename D, typename A>
	interface_ptr(Y* p, D d, A a)
		: base_type(construct_shared<obj_type>(p, d, a))
	{
	}

	template<typename Y, template<typename _T> class OthSPtr> explicit
	interface_ptr(BOOST_RV_REF_BEG OthSPtr<Y> BOOST_RV_REF_END oth_p)
		: base_type(boost::move(oth_p))
	{
	}

	template<typename Y, template<typename _T> class OthSPtr> explicit
	interface_ptr(const OthSPtr<Y>& oth_p)
		: base_type(oth_p)
	{
	}

	template<typename Y, typename D,
				template<typename _T, typename _D> class OthSPtr>
	interface_ptr(BOOST_RV_REF_BEG OthSPtr<Y, D> BOOST_RV_REF_END oth_p)
		: base_type(boost::move(oth_p))
	{
	}

	// don't include this consturct foo, because base type is not supported, see shared_ptr(unique_ptr)
	//template<typename Y, typename D,
	//			template<typename _T, typename _D> class OthSPtr>
	//interface_ptr(const OthSPtr<Y, D>& oth_p);

	template<typename Y>
	interface_ptr(BOOST_RV_REF_BEG SPtr<Y> BOOST_RV_REF_END p)
		: base_type(boost::move(p))
	{
	}

	template<typename Y>
	interface_ptr(const SPtr<Y>& p)
		: base_type(p)
	{
	}

	template<typename Y>
    interface_ptr(const SPtr<Y>& right, ptr_type p)
		: base_type(right, p)
    {
    }

	interface_ptr(BOOST_RV_REF(base_type) p)
		: base_type(boost::move(p))
	{
	}

	interface_ptr(const base_type& p)
		: base_type(p)
	{
	}

	template<typename Y>
	interface_ptr(BOOST_RV_REF_BEG interface_ptr<Y> BOOST_RV_REF_END right)
		: base_type(boost::move(static_cast<typename interface_ptr<Y>::base_type&>(right)))
	{
	}

	template<typename Y>
	interface_ptr(const interface_ptr<Y>& right)
		: base_type(static_cast<const typename interface_ptr<Y>::base_type&>(right))
	{
	}

	template<typename Y>
	interface_ptr(const interface_ptr<Y>& right, ptr_type p)
		: base_type(static_cast<const typename interface_ptr<Y>::base_type&>(right), p)
	{
	}

	interface_ptr(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	interface_ptr(const this_type& right)
        :base_type(static_cast<const base_type&>(right))
	{
	}

	~interface_ptr(void)
	{
	}

public:

	template<typename Y, template<typename _T> class OthSPtr> inline
	this_type& operator=(BOOST_RV_REF_BEG OthSPtr<Y> BOOST_RV_REF_END right)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	template<typename Y, template<typename _T> class OthSPtr>
	this_type& operator=(const OthSPtr<Y>& right)
	{
		base_type::operator=(right);
		return *this;
	}

	template<typename Y, typename D,
				template<typename _T, typename _D> class OthSPtr> inline
	this_type& operator=(BOOST_RV_REF_BEG OthSPtr<Y, D> BOOST_RV_REF_END right)
	{
		base_type::operator=(right);
		return *this;
	}

	// don't include this consturct foo, because base type is not supported, see shared_ptr::operator=(unique_ptr)
	//template<typename Y, typename D,
	//			template<typename _T, typename _D> class OthSPtr>
	//this_type& operator=(const OthSPtr<Y, D>& right);

	template<typename Y> inline
	this_type& operator=(BOOST_RV_REF_BEG SPtr<Y> BOOST_RV_REF_END right)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	template<typename Y>
	this_type& operator=(const SPtr<Y>& right)
	{
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == boost::addressof(right_chk))
		{
			return *this;
		}

		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

        base_type& base = *this;
        base = right;
        return *this;
	}

public:
	void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& base_ref = right;
		base_type::swap(base_ref);
	}

	void swap(base_type& right)
	{
		base_type::swap(right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		base_type::swap(right);
	}

public:
	using base_type::get;

	// thank you for [C++/ObjC/Java]ZERO (QQ365203662 ZFFramework's Master)
	// AD:
	// ZFFramework_Address: https://github.com/ZFFramework/ZFFramework
	// ZFFramework is a cross platform gui library

	// call: child_type *p = interface_ptr return 0 is failed
	template<typename Child> inline
	operator Child*(void)
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	template<typename Child> inline
	operator const Child*(void) const
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	template<typename Child> inline
	const Child* child_cast(void) const
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	template<typename Child> inline
	Child* child_cast(void)
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	template<typename Child> inline
	const Child* unsafe_child_cast(void) const
	{
		ptr_type tmp = base_type::get();
		assert(dynamic_cast<Child*>(tmp));
		return reinterpret_cast<Child*>(tmp);
	}

	template<typename Child> inline
	Child* unsafe_child_cast(void)
	{
		ptr_type tmp = base_type::get();
		assert(dynamic_cast<Child*>(tmp));
		return reinterpret_cast<Child*>(tmp);
	}

	using base_type::operator->;

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"shared_ptr",
				boost::serialization::base_object<base_type>(*this));
	}
};

// non-member-foo

// operator==

template<typename T, typename U,
			template<typename _T> class SPtr> inline
bool operator==(const T* l, const interface_ptr<U, SPtr>& r)
{
	typedef T* chk_native_ptr_type;

	return l == reinterpret_cast<chk_native_ptr_type>(r.get());
}

template<typename T, typename U,
			template<typename _T> class SPtr> inline
bool operator==(const interface_ptr<T, SPtr>& l, const U* r)
{
	typedef U* chk_native_ptr_type;

	return reinterpret_cast<chk_native_ptr_type>(l.get()) == r;
}

#if !(defined(__GNUC__) && (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION))
template<typename T, typename U,
			template<typename _T> class SPtr1,
			template<typename _T> class SPtr2> inline
bool operator==(const SPtr1<T>& l,
				const interface_ptr<U, SPtr2>& r)
{
	typedef interface_ptr<U, SPtr2> chk_type;
	typedef typename chk_type::pointer chk_native_ptr_type;

	return reinterpret_cast<chk_native_ptr_type>(l.get()) == r.get();
}

template<typename T, typename U,
			template<typename _T> class SPtr1,
			template<typename _T> class SPtr2> inline
bool operator==(const interface_ptr<T, SPtr1>& l,
				const SPtr2<U>& r)
{
	typedef interface_ptr<T, SPtr1> chk_type;
	typedef typename chk_type::pointer chk_native_ptr_type;

	return l.get() == reinterpret_cast<chk_native_ptr_type>(r.get());
}

#endif // #if !(defined(__GNUC__) && (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION))

template<typename T, typename U,
			template<typename _T> class SPtr1,
			template<typename _T> class SPtr2> inline
bool operator==(const interface_ptr<T, SPtr1>& l,
				const interface_ptr<U, SPtr2>& r)
{
	typedef interface_ptr<T, SPtr1> chk_type;
	typedef typename chk_type::pointer chk_native_ptr_type;

	return l.get() == reinterpret_cast<chk_native_ptr_type>(r.get());
}

// operator!=
template<typename T, typename U,
			template<typename _T> class SPtr> inline
bool operator!=(const T* l, const interface_ptr<U, SPtr>& r)
{
	typedef T* chk_native_ptr_type;

	return l != reinterpret_cast<chk_native_ptr_type>(r.get());
}

template<typename T, typename U,
			template<typename _T> class SPtr> inline
bool operator!=(const interface_ptr<T, SPtr>& l, const U* r)
{
	typedef U* chk_native_ptr_type;

	return reinterpret_cast<chk_native_ptr_type>(l.get()) != r;
}

#if !(defined(__GNUC__) && (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION))

template<typename T, typename U,
			template<typename _T> class SPtr1,
			template<typename _T> class SPtr2> inline
bool operator!=(const SPtr1<T>& l,
				const interface_ptr<U, SPtr2>& r)
{
	typedef interface_ptr<U, SPtr2> chk_type;
	typedef typename chk_type::pointer chk_native_ptr_type;

	return reinterpret_cast<chk_native_ptr_type>(l.get()) != r.get();
}

template<typename T, typename U,
			template<typename _T> class SPtr1,
			template<typename _T> class SPtr2> inline
bool operator!=(const interface_ptr<T, SPtr1>& l,
				const SPtr2<U>& r)
{
	typedef interface_ptr<T, SPtr1> chk_type;
	typedef typename chk_type::pointer chk_native_ptr_type;

	return l.get() != reinterpret_cast<chk_native_ptr_type>(r.get());
}

#endif // #if !(defined(__GNUC__) && (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION))

template<typename T, typename U,
			template<typename _T> class SPtr1,
			template<typename _T> class SPtr2> inline
bool operator!=(const interface_ptr<T, SPtr1>& l,
				const interface_ptr<U, SPtr2>& r)
{
	typedef interface_ptr<T, SPtr1> chk_type;
	typedef typename chk_type::pointer chk_native_ptr_type;

	return l.get() != reinterpret_cast<chk_native_ptr_type>(r.get());
}

} // namespace detail
} // namespace smart_ptr_ex
} // namespace yggr

namespace yggr
{
namespace smart_ptr_ex
{
namespace detail
{
namespace swap_support
{

template<typename T, template<typename _T> class SPtr> inline
void swap(typename interface_ptr<T, SPtr>::base_type& l, interface_ptr<T, SPtr>& r)
{
	r.swap(l);
}

template<typename T, template<typename _T> class SPtr> inline
void swap(interface_ptr<T, SPtr>& l, typename interface_ptr<T, SPtr>::base_type& r)
{
	l.swap(r);
}


template<typename T, template<typename _T> class SPtr> inline
void swap(interface_ptr<T, SPtr>& l, interface_ptr<T, SPtr>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace smart_ptr_ex
} // namespace yggr

namespace std
{
	using ::yggr::smart_ptr_ex::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::smart_ptr_ex::detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_SMART_PTR_EX_DETAIL_INTERFACE_PTR_HPP__
