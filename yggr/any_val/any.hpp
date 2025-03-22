// any.hpp

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

#ifndef __YGGR_ANY_VAL_ANY_HPP__
#define __YGGR_ANY_VAL_ANY_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/aligned_storage_sizeof.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/sysbyte.hpp>
#include <yggr/typeof/default_value.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/any_val/detail/boost_any_ex.hpp>
#include <yggr/any_val/any_check.hpp>

#include <boost/ref.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/bool.hpp>

#if !(BOOST_VERSION < 105600)
#	include <boost/type_index.hpp>
#endif // !(BOOST_VERSION < 105600)

namespace yggr
{

class any
{
public: // structors

private:
	typedef
		boost::mpl::integral_c
		<
			yggr::u64,
			mplex::sysbyte_max::value
		> max_value_t_size_type;

	typedef
		boost::mpl::integral_c
		<
			yggr::u64,
			mplex::sysbyte_now::value + max_value_t_size_type::value // virtual + value
		> store_buffer_size_type;

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
private: // types
#else
public: // types (public so any_cast can be non-friend)
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	class placeholder
	{
	public: // structors

		virtual ~placeholder(void)
		{
		}

	public: // queries

		virtual const std::type_info& type(void) const = 0;
		virtual placeholder* clone(void) const = 0;
		virtual placeholder* clone(u8 (&data_buf)[store_buffer_size_type::value]) const = 0;
		virtual bool is_small(void) const = 0;

	private:
		friend class yggr::serialization::access;
		template<typename Archive> inline
		void serialize(Archive& ar, const u32 version) {}
	};

public:
	template<typename ValueType>
	class holder : public placeholder
	{
	private:
		typedef ValueType value_type;
		typedef placeholder base_type;
		typedef any parent_type;
		typedef holder this_type;

	public: // structors

		holder(void)
			: held()
		{
		}

		holder(const value_type& val)
			: held(val)
		{
		}

		virtual ~holder(void)
		{
		}

	public: // queries

		virtual const std::type_info& type(void) const
		{
			return typeid(value_type);
		}

		virtual placeholder* clone(void) const
		{
			return yggr_nothrow_new this_type(*boost::addressof(held));
		}

		virtual placeholder* clone(u8 (&data_buf)[store_buffer_size_type::value]) const
		{
			assert(YGGR_PP_ALIGNED_STORAGE_SIZEOF(this_type) <= store_buffer_size_type::value);
			memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value);
			void* pbuf = boost::addressof(data_buf[0]);
			return new (pbuf) this_type(held);
		}

		virtual bool is_small(void) const
		{
			return YGGR_PP_ALIGNED_STORAGE_SIZEOF(this_type) <= store_buffer_size_type::value;
		}

	private:
		friend class yggr::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const u32 version)
		{
			ar & YGGR_SERIALIZATION_NAME_NVP("placeholder", boost::serialization::base_object< base_type >(*this));
			ar & YGGR_SERIALIZATION_NVP(held);
		}

	public: // representation

		value_type held;

	private: // intentionally left unimplemented
		holder & operator=(const holder &);
	};


private:
	template<typename T,
				bool IsSmall = (YGGR_PP_ALIGNED_STORAGE_SIZEOF(holder<T>) <= store_buffer_size_type::value),
				typename Nil = int>
	struct any_creator_detail;

	// small operator
	template<typename T, typename Nil>
	struct any_creator_detail<T, true, Nil>
	{
	public:
		typedef T value_type;
		typedef holder<value_type> holder_type;

	public:
		inline static placeholder* create(u8 (&data_buf)[store_buffer_size_type::value], const value_type& val)
		{
			void* pbuf = static_cast<void*>(boost::addressof(data_buf[0]));
			assert(pbuf);
			memset(pbuf, 0, store_buffer_size_type::value);

			try
			{
				return static_cast<placeholder*>(new (pbuf) holder_type(val));
			}
			catch(const std::bad_alloc&)
			{
				return 0;
			}
			catch(...)
			{
				return 0;
			}
		}
	};

	// big operator
	template<typename T, typename Nil>
	struct any_creator_detail<T, false, Nil>
	{
	public:
		typedef T value_type;
		typedef holder<value_type> holder_type;

	public:
		inline static placeholder* create(u8 (&data_buf)[store_buffer_size_type::value], const value_type& val)
		{
			memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value);
			return static_cast<placeholder*>(yggr_nothrow_new holder_type(val));
		}
	};

	template<typename T>
	struct any_creator
		: public any_creator_detail<T>
	{
	public:
		typedef any_creator_detail<T> base_type;

		using base_type::create;
	};


private:
	typedef any this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	any(void)
		: _content()
	{
		memset(_data_buf, 0, sizeof(_data_buf));
	}


	template<typename ValueType>
	any(const ValueType& val)
		: _content()
	{
		memset(_data_buf, 0, sizeof(_data_buf));
		_content = any_creator<ValueType>::create(_data_buf, val);
	}

	any(BOOST_RV_REF(this_type) right)
		: _content()
	{
		memset(_data_buf, 0, sizeof(_data_buf));
		_content = this_type::prv_s_move(_data_buf, right._content);
	}

	any(const this_type& right)
		: _content()
	{
		memset(_data_buf, 0, sizeof(_data_buf));
		_content = this_type::prv_s_clone(_data_buf, right._content);
	}

	~any(void)
	{
		this_type::prv_s_destroy(_content);
	}

public: // modifiers
	template<typename ValueType>
	this_type& operator=(const ValueType& right)
	{
		this_type::prv_s_destroy(_content);
		_content = any_creator<ValueType>::create(_data_buf, right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right)
		{
			return *this;
		}

		this_type::prv_s_destroy(_content);
		_content = this_type::prv_s_move(_data_buf, right_ref._content);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		this_type::prv_s_destroy(_content);
		_content = this_type::prv_s_clone(_data_buf, right._content);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		this_type::prv_s_swap(_data_buf, _content, right._data_buf, right._content);
	}

public: // queries

	inline bool empty(void) const
	{
		return !_content;
	}

	inline const std::type_info& type(void) const
	{
		return _content? _content->type() : typeid(void);
	}

private:
	inline void prv_reset(void)
	{
		if(_content)
		{
			this_type::prv_s_destroy(_content);
			_content = 0;
			memset(_data_buf, 0, store_buffer_size_type::value);
		}
	}

private:
	template<typename T> inline
	static bool prv_s_is_small(void)
	{
		return boost::mpl::bool_<(sizeof(T) <= store_buffer_size_type::value)>::value;
	}

	inline static placeholder* prv_s_clone_small(u8 (&data_buf)[store_buffer_size_type::value],
													const placeholder* src_holder)
	{
		return
			src_holder?
				src_holder->clone(data_buf)
				: (memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value),
					static_cast<placeholder*>(0));
	}

	inline static placeholder* prv_s_clone_big(u8 (&data_buf)[store_buffer_size_type::value],
													const placeholder* src_holder)
	{
		memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value);
		return src_holder? src_holder->clone() : static_cast<placeholder*>(0);
	}

	inline static placeholder* prv_s_clone(u8 (&data_buf)[store_buffer_size_type::value],
											const placeholder* src_holder)
	{
		return
			src_holder?
				src_holder->is_small()?
					this_type::prv_s_clone_small(data_buf, src_holder)
					: this_type::prv_s_clone_big(data_buf, src_holder)
				: (memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value),
					static_cast<placeholder*>(0));
	}

	inline static placeholder* prv_s_move_small(u8 (&data_buf)[store_buffer_size_type::value], placeholder*& src_holder)
	{
		return this_type::prv_s_clone_small(data_buf, src_holder);
	}

	inline static placeholder* prv_s_move_big(u8 (&data_buf)[store_buffer_size_type::value], placeholder*& src_holder)
	{
		memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value);
		placeholder* tmp = src_holder;
		src_holder = 0;
		return tmp;
	}

	inline static placeholder* prv_s_move(u8 (&data_buf)[store_buffer_size_type::value], placeholder*& src_holder)
	{
		return
			src_holder?
				src_holder->is_small()?
					this_type::prv_s_move_small(data_buf, src_holder)
					: this_type::prv_s_move_big(data_buf, src_holder)
				: (memset(boost::addressof(data_buf[0]), 0, store_buffer_size_type::value),
					static_cast<placeholder*>(0));
	}

	inline static void prv_s_swap_small(u8 (&lbuf)[store_buffer_size_type::value], placeholder*& lp,
											u8 (&rbuf)[store_buffer_size_type::value], placeholder*& rp)
	{
		u8 tmp_buf[store_buffer_size_type::value] = {0};
		placeholder* ptmp_holder = this_type::prv_s_clone_small(tmp_buf, lp);

		lp = this_type::prv_s_clone_small(lbuf, rp);
		rp = this_type::prv_s_clone_small(rbuf, ptmp_holder);
	}

	inline static void prv_s_swap_mix(u8 (&lbuf)[store_buffer_size_type::value], placeholder*& lp,
										u8 (&rbuf)[store_buffer_size_type::value], placeholder*& rp)
	{
		assert((lp && lp->is_small()) && !(rp && rp->is_small()));

		u8 tmp_buf[store_buffer_size_type::value] = {0};
		placeholder* ptmp_holder = this_type::prv_s_clone_small(tmp_buf, lp);

		memset(boost::addressof(lbuf[0]), 0, store_buffer_size_type::value);
		lp = rp;
		rp = this_type::prv_s_clone_small(rbuf, ptmp_holder);
	}

	inline static void prv_s_swap_big(u8 (&lbuf)[store_buffer_size_type::value], placeholder*& lp,
										u8 (&rbuf)[store_buffer_size_type::value], placeholder*& rp)
	{
		::yggr::swap(lp, rp);
	}

	inline static void prv_s_swap(u8 (&lbuf)[store_buffer_size_type::value], placeholder*& lp,
									u8 (&rbuf)[store_buffer_size_type::value], placeholder*& rp)
	{
		u32 t = 0;
		t = t | (lp && lp->is_small()? 0x0000002 : 0);
		t = t | (rp && rp->is_small()? 0x0000001 : 0);

		switch(t)
		{
		case 0:
			this_type::prv_s_swap_big(lbuf, lp, rbuf, rp);
			break;
		case 1:
			this_type::prv_s_swap_mix(rbuf, rp, lbuf, lp);
			break;
		case 2:
			this_type::prv_s_swap_mix(lbuf, lp, rbuf, rp);
			break;
		case 3:
			this_type::prv_s_swap_small(lbuf, lp, rbuf, rp);
			break;
		default:
			assert(false);
		}
	}

	inline static void prv_s_destroy_small(placeholder* p)
	{
	}

	inline static void prv_s_destroy_big(placeholder* p)
	{
		if(p) delete p;
	}

	inline static void prv_s_destroy(placeholder* p)
	{
		return
			p?
				p->is_small()?
					this_type::prv_s_destroy_small(p)
					: this_type::prv_s_destroy_big(p)
				: default_value<void>();
	}


private:
	friend class yggr::serialization::access;

	template<typename Archive>
	typename boost::enable_if<typename Archive::is_saving, void>::type
		serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("content", _content);
	}

	template<typename Archive>
	typename boost::enable_if<typename Archive::is_loading, void>::type
		serialize(Archive& ar, const u32 version)
	{
		placeholder* tmp = 0;
		ar & YGGR_SERIALIZATION_NAME_NVP("content", tmp);

		if(!tmp)
		{
			this_type::prv_reset();
		}
		else
		{
			if(tmp->is_small())
			{
				_content = this_type::prv_s_clone_small(_data_buf, tmp);
				delete tmp;
			}
			else
			{
				this_type::prv_reset();
				_content = tmp;
			}
		}
	}

private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private: // representation

	template<typename ValueType, typename Any>
	friend ValueType * any_cast(Any *);

	template<typename ValueType, typename Any>
	friend ValueType * unsafe_any_cast(Any *);

#else

public: // representation (public so any_cast can be non-friend)

#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	u8 _data_buf[store_buffer_size_type::value];
	placeholder* _content;
};

class bad_any_cast : public std::bad_cast
{
public:
	virtual const char* what(void) const BOOST_NOEXCEPT_OR_NOTHROW //throw()
	{
		return "yggr::bad_any_cast: "
				"failed conversion using yggr::any_cast";
	}
};

#if BOOST_VERSION < 105600

template<typename ValueType, typename Any> inline
ValueType * any_cast(Any * operand)
{
	typedef Any any_type;
	return operand &&
#ifdef BOOST_AUX_ANY_TYPE_ID_NAME
		std::strcmp(operand->type().name(), typeid(ValueType).name()) == 0
#else
		operand->type() == typeid(ValueType)
#endif
		? boost::addressof(static_cast<typename any_type::template holder<ValueType> *>(operand->_content)->held)
		: 0;
}

#else

template<typename ValueType, typename Any> inline
ValueType * any_cast(Any * operand)
{
	typedef ValueType now_val_cv_type;
	typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<now_val_cv_type>::type now_val_type;
	typedef Any any_type;
	return operand && operand->type() == boost::typeindex::type_id<now_val_cv_type>()
		? boost::addressof(
			static_cast<typename any_type::template holder<now_val_type> *>(operand->_content)->held
			)
		: 0;
}


#endif // BOOST_VERSION < 105600

template<typename ValueType, typename Any> inline
const ValueType * any_cast(const Any * operand)
{
	typedef Any any_type;
	return yggr::any_cast<ValueType>(const_cast<any_type *>(operand));
}

template<typename ValueType, typename Any> inline
ValueType any_cast(Any & operand)
{
	typedef typename boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	// If 'nonref' is still reference type, it means the user has not
	// specialized 'remove_reference'.

	// Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
	// to generate specialization of remove_reference for your class
	// See type traits library documentation for details
	BOOST_STATIC_ASSERT(!boost::is_reference<nonref>::value);
#endif

	nonref * result = ::yggr::any_cast<nonref, Any>(&operand);
	if(!result)
		boost::throw_exception(bad_any_cast());
	return *result;
}

template<typename ValueType, typename Any> inline
ValueType any_cast(const Any & operand)
{
	typedef Any any_type;
	typedef typename boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	// The comment in the above version of 'any_cast' explains when this
	// assert is fired and what to do.
	BOOST_STATIC_ASSERT(!boost::is_reference<nonref>::value);
#endif

	return ::yggr::any_cast<const nonref &>(const_cast<any_type &>(operand));
}

// Note: The "unsafe" versions of any_cast are not part of the
// public interface and may be removed at any time. They are
// required where we know what type is stored in the any and can't
// use typeid() comparison, e.g., when our types may travel across
// different shared libraries.
template<typename ValueType, typename Any> inline
ValueType * unsafe_any_cast(Any * operand)
{
	typedef Any any_type;
	return &static_cast<typename any_type::template holder<ValueType> *>(operand->_content)->held;
}

template<typename ValueType, typename Any> inline
const ValueType * unsafe_any_cast(const Any * operand)
{
	typedef Any any_type;
	return yggr::unsafe_any_cast<ValueType>(const_cast<any_type *>(operand));
}

template<typename ValueType, typename Any> inline
ValueType unsafe_any_cast(Any & operand)
{
	typedef typename boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	// If 'nonref' is still reference type, it means the user has not
	// specialized 'remove_reference'.

	// Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
	// to generate specialization of remove_reference for your class
	// See type traits library documentation for details
	BOOST_STATIC_ASSERT(!boost::is_reference<nonref>::value);
#endif

	nonref * result = ::yggr::unsafe_any_cast<nonref, Any>(&operand);
	if(!result)
		boost::throw_exception(bad_any_cast());
	return *result;
}

template<typename ValueType, typename Any> inline
ValueType unsafe_any_cast(const Any & operand)
{
	typedef Any any_type;
	typedef typename boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	// The comment in the above version of 'any_cast' explains when this
	// assert is fired and what to do.
	BOOST_STATIC_ASSERT(!boost::is_reference<nonref>::value);
#endif

	return ::yggr::unsafe_any_cast<const nonref &>(const_cast<any_type &>(operand));
}

} // namespace yggr

YGGR_IS_ANY_DEF( yggr::any )

namespace yggr
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(yggr::any)
} // namespace swap_support

using swap_support::swap;

} // namespace yggr

namespace std
{
	//YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(yggr::any)
using yggr::swap_support::swap;
	
} // namespace std

namespace boost
{
	//YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(yggr::any)
using yggr::swap_support::swap;
} // namespace boost

#endif //__YGGR_ANY_VAL_ANY_HPP__
