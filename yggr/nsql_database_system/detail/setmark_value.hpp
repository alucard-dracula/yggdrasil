// setmark_value.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_SETMARK_VALUE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_SETMARK_VALUE_HPP__

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

struct setmark_tag_set{};
struct setmark_tag_swap{};

template<typename TVal>
struct basic_setmark_value
{
public:
	typedef TVal value_type;

private:
	typedef basic_setmark_value this_type;

public:
	inline operator value_type&(void)
	{
		return val_;
	}

	inline operator const value_type&(void) const
	{
		return val_;
	}

	inline value_type& value(void)
	{
		return val_;
	}

	inline const value_type& value(void) const
	{
		return val_;
	}

	inline bool& set(void)
	{
		return set_;
	}

	inline bool set(void) const
	{
		return set_;
	}

public:
	value_type val_;
	bool set_;
};

template<typename TVal, typename ValueSetter = void>
struct setmark_value
	: public basic_setmark_value<TVal>
{
public:
	typedef TVal value_type;
	typedef basic_setmark_value<value_type> base_type;

protected:
	typedef ValueSetter value_setter_type;

private:
	typedef setmark_value this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	// non construct operator

public:
	template<typename T> inline
	this_type& operator=(const T& right)
	{
		value_setter_type setter;
		base_type::set_ = setter((setmark_tag_set()), base_type::val_, right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		value_setter_type setter;
		setter((setmark_tag_set()), base_type::val_, boost::move(right_ref.val_));
		base_type::set_ = right_ref.set_;

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		value_setter_type setter;
		setter((setmark_tag_set()), base_type::val_, right.val_);
		base_type::set_ = right.set_;

		return *this;
	}

public:
	inline void swap(value_type& right)
	{
		value_setter_type setter;
		base_type::set_ = setter((setmark_tag_swap()), base_type::val_, right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		value_setter_type setter;
		if(setter((setmark_tag_swap()), base_type::val_, right))
		{
			::yggr::swap(base_type::set_, right.set_);
		}
	}
};

template<typename TVal>
struct setmark_value<TVal, void>
	: public basic_setmark_value<TVal>
{
public:
	typedef TVal value_type;
	typedef basic_setmark_value<value_type> base_type;

private:
	typedef setmark_value this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	// non construct operator

public:
	template<typename T> inline
	this_type& operator=(const T& right)
	{
		base_type::val_ = right;
		base_type::set_ = true;

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::val_ = boost::move(right_ref.val_);
		base_type::set_ = right_ref.set_;

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::val_ = right.val_;
		base_type::set_ = right.set_;

		return *this;
	}

public:
	inline void swap(value_type& right)
	{
		yggr::swap(base_type::val_, right);
		base_type::set_ = right.set_;
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::val_, right.val_);
		::yggr::swap(base_type::set_, right.set_);
	}

};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{
namespace swap_support
{

template<typename TVal> inline
void swap(TVal& l, setmark_value<TVal>& r)
{
	r.swap(l);
}

template<typename TVal> inline
void swap(setmark_value<TVal>& l, TVal& r)
{
	l.swap(r);
}

template<typename TVal> inline
void swap(setmark_value<TVal>& l, setmark_value<TVal>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_SETMARK_VALUE_HPP__
