//c_bson_binary_buffer.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_BINARY_BUFFER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_BINARY_BUFFER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/const_strings.hpp>

#include <yggr/ppex/friend.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/nsql_database_system/detail/binary_buffer_assign_helper.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>

#include <boost/range/functions.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <cassert>
#include <algorithm>

namespace yggr
{
namespace nsql_database_system
{

///****************** the Buffer recommend use network byte order*******************/
template<u32 BINARY_ID>
class c_bson_binary_buffer;

template<u32 BINARY_ID, typename Buffer>
class c_bson_binary_buffer_ref;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<u32 BINARY_ID>
struct bson_typeid< c_bson_binary_buffer<BINARY_ID> >
{
	typedef c_bson_binary_buffer<BINARY_ID> type;
	typedef c_bson_binary_buffer<BINARY_ID> wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type, 
		value = bson_typeid_def::E_BSON_TYPE_BINARY);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const
	{
		return static_cast<u8>(value);
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

// c_bson_binary_buffer_assign_iterator_helper

template<typename Buf, typename InputIter>
struct c_bson_binary_buffer_assign_iterator_helper
	: public binary_buffer_assign_iterator_helper<Buf, InputIter>
{
};

// c_bson_binary_buffer_assign_container_helper

template<typename Buf, typename Container>
struct c_bson_binary_buffer_assign_container_helper
	: public binary_buffer_assign_container_helper<Buf, Container>
{
};

// c_bson_binary_buffer_container_comparer_eq
template<typename Buf, typename Container, 
			bool is_random_access = is_random_access_container<Container>::value>
struct c_bson_binary_buffer_container_comparer_eq;

template<typename Buf, typename Container>
struct c_bson_binary_buffer_container_comparer_eq<Buf, Container, true>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

public:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

private:
	BOOST_MPL_ASSERT((is_random_access_container<Buf>));

public:
	inline bool operator()(const buf_type& l, const cont_type& r) const
	{
		u64 lsize = bytes::byte_size(l);
		return
			( lsize == bytes::byte_size(r))
			&& ((reinterpret_cast<const void*>(boost::addressof(*boost::begin(l)))
					== reinterpret_cast<const void*>(boost::addressof(*boost::begin(r))))
				|| (0 == memcmp(boost::addressof(*boost::begin(l)), boost::addressof(*boost::begin(r)), lsize)));
	}
};

template<typename Buf, typename Container>
struct c_bson_binary_buffer_container_comparer_eq<Buf, Container, false>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

public:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

private:
	BOOST_MPL_ASSERT((is_random_access_container<buf_type>));

public:
	inline bool operator()(const buf_type& l, const cont_type& r) const
	{
		return 
			(bytes::byte_size(l) == bytes::byte_size(r))
			&& (std::equal(
					boost::begin(r), boost::end(r),
					reinterpret_cast<const cont_value_type*>(boost::addressof(*boost::begin(l)))));
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<u32 BINARY_ID>
class c_bson_binary_buffer
{
public:
	YGGR_STATIC_CONSTANT(u32, E_TYPE_ID = BINARY_ID);

	typedef yggr::string buf_type;
	typedef u8 buf_val_type;

private:
	typedef c_bson_binary_buffer this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_binary_buffer(void)
	{
	}

	c_bson_binary_buffer(const bson_value_t& val)
	{
		this_type::pro_assign_bson_value_t(val);
	}

	explicit c_bson_binary_buffer(BOOST_RV_REF(buf_type) buf)
		: _buf(boost::move(buf))
	{
	}

	explicit c_bson_binary_buffer(const buf_type& buf)
		: _buf(buf)
	{
	}

	c_bson_binary_buffer(u8* pdata, u64 len)
	{
		this_type::assign(pdata, len);
	}

	template<typename Container>
	explicit c_bson_binary_buffer(const Container& cont,
									typename
										boost::enable_if
										<
											boost::mpl::and_
											<
												container_ex::is_container<Container>,
												boost::mpl::not_
												<
													boost::is_class
													<
														typename range_ex::range_value_ex<Container>::type
													>
												>
											>,
											sfinae_type
										>::type sfinae = 0)
	{
		this_type::assign(cont);
	}

	template<typename InputIter>
	c_bson_binary_buffer(InputIter s, InputIter e)
	{
		this_type::assign(s, e);
	}

	c_bson_binary_buffer(BOOST_RV_REF(this_type) right)
		: _buf(boost::move(right._buf))
	{
	}

	c_bson_binary_buffer(const this_type& right)
		: _buf(right._buf)
	{
	}

	~c_bson_binary_buffer(void)
	{
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const bson_value_t& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		this_type::assign(boost::move(buf));
		return *this;
	}

	inline this_type& operator=(const buf_type& buf)
	{
		this_type::assign(buf);
		return *this;
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			this_type&
		>::type
		operator=(const Container& cont)
	{
		this_type::assign(cont);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type::assign(boost::move(right));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		this_type::assign(right);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(buf_type) right)
	{
		buf_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(buf_type& right)
	{
		if(boost::addressof(_buf) == boost::addressof(right))
		{
			return;
		}

		_buf.swap(right);
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

		_buf.swap(right._buf);
	}

public:
	inline operator const u8*(void) const
	{
		return 
			_buf.empty()? 
			0 
			: reinterpret_cast<const u8*>(boost::addressof(_buf[0]));
	}

	inline operator u8*(void)
	{
		return 
			_buf.empty()? 
			0 
			: reinterpret_cast<u8*>(boost::addressof(_buf[0]));
	}


	//inline operator const buf_type&(void) const
	//{
	//	return _buf;
	//}

	//inline operator buf_type&(void)
	//{
	//	return _buf;
	//}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_BINARY;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_BINARY;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 buffer_typeid(void)
	{
		return this_type::E_TYPE_ID;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_buffer_typeid(void)
	{
		return this_type::E_TYPE_ID;
	}

public:
	inline size_type size(void) const
	{
		return _buf.size();
	}

	inline void resize(size_type size)
	{
		container::resize(_buf, size);
	}

	inline bool empty(void) const
	{
		return container::empty(_buf);
	}

	inline void clear(void)
	{
		container::clear(_buf);
	}

	inline buf_type& buffer(void)
	{
		return _buf;
	}

	inline const buf_type& buffer(void) const
	{
		return _buf;
	}

	inline const u8* data(void) const
	{
		return 
			this_type::empty()? 
				0 
				: reinterpret_cast<const u8*>(boost::addressof(_buf[0]));
	}

	inline u8* data(void)
	{
		return 
			this_type::empty()? 
			0 
			: reinterpret_cast<u8*>(boost::addressof(_buf[0]));
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return 
			bson_value_native_ex::s_value_cmp_eq_binary_buffer(
				&val, this_type::buffer_typeid(), this_type::data(), this_type::size());
	}

	inline bool compare_eq(const buf_type& buf) const
	{
		return _buf == buf;
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			bool
		>::type
		compare_eq(const Container& cont) const
	{
		typedef Container cv_cont_type;
		typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
		typedef 
			detail::c_bson_binary_buffer_container_comparer_eq
			<
				buf_type,
				cont_type
			> cmp_type;

		cmp_type cmp;
		return cmp(_buf, cont);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return this == boost::addressof(right) || _buf == right._buf;
	}

protected:
	inline void pro_assign_bson_value_t(const bson_value_t& val)
	{
		bool bsuccess = bson_value_native_ex::s_value_to_binary_buffer(_buf, &val, this_type::E_TYPE_ID);
		assert(bsuccess);

		if(!bsuccess)
		{
			container::clear(_buf);
		}
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, void>::type
		assign(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
	}

	inline void assign(const u8* pdata, u64 len)
	{
		if(!(pdata && len))
		{
			container::clear(_buf);
			return;
		}

		container::resize(_buf, len);
		memcpy(&_buf[0], pdata, len);
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename
							boost::remove_cv
							<
								typename iterator_ex::iterator_value_ex<InputIter>::type
							>::type
					>
				>
			>,
			void
		>::type
		assign(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef 
			detail::c_bson_binary_buffer_assign_iterator_helper
			<
				buf_type, 
				iter_type
			> h_type;

		h_type h;
		h(_buf, s, e);
	}

	inline void assign(BOOST_RV_REF(buf_type) buf)
	{
		copy_or_move_or_swap(_buf, boost::move(buf));
	}

	inline void assign(const buf_type& buf)
	{
		_buf = buf;
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			void
		>::type
		assign(const Container& cont)
	{
		typedef Container cv_cont_type;
		typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
		
		typedef 
			detail::c_bson_binary_buffer_assign_container_helper
			<
				buf_type,
				cont_type
			> h_type;

		h_type h;
		h(_buf, cont);
	}

	inline void assign(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == boost::addressof(right_chk))
		{
			return;
		}

		copy_or_move_or_swap(_buf, boost::move(right._buf));
	}

	inline void assign(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		_buf = right._buf;
	}

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );
	//save and load def

	template<typename Bson, typename String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable< String >
			>, 
			bool 
		>::type
		save(Bson& bs, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(boost::addressof(bs)))
		{
			bson_init(boost::addressof(bs));
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return 
			bson_append_binary(
				boost::addressof(bs), str_name.data(), str_name.size(),
				static_cast<bson_subtype_t>(this_type::E_TYPE_ID),
				reinterpret_cast<const u8*>(_buf.data()), _buf.size());
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return 
			bson_native_ex::s_bson_save_value_of_binary_buffer(
				boost::addressof(bs), this_type::E_TYPE_ID,
				reinterpret_cast<const u8*>(boost::addressof(*boost::begin(_buf))),
				bytes::byte_size(_buf));
	}

	//template<typename BsonIter, typename Char> inline
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			is_native_char<Char>
	//		>, 
	//		void
	//	>::type
	//	load(const BsonIter& iter, const Char* name)
	//{
	//	assert((iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID)));

	//	if(!iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID))
	//	{
	//		container::clear(_buf);
	//		return;
	//	}

	//	bson_subtype_t sub_t = BSON_SUBTYPE_BINARY;
	//	u32 bin_buf_len = 0;
	//	const u8* pbuf = 0;
	//	bson_iter_binary(boost::addressof(iter), &sub_t, &bin_buf_len, &pbuf);

	//	this_type::assign(pbuf, bin_buf_len);
	//}

	//template<typename BsonIter, typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	//typename 
	//	boost::enable_if
	//	< 
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			charset::is_string_t< Basic_String<Char, Traits, Alloc> >
	//		>,
	//		void
	//	>::type
	//	load(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name)
	//{
	//	assert((iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID)));

	//	if(!iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID))
	//	{
	//		container::clear(_buf);
	//		return;
	//	}

	//	bson_subtype_t sub_t = BSON_SUBTYPE_BINARY;
	//	u32 bin_buf_len = 0;
	//	const u8* pbuf = 0;
	//	bson_iter_binary(boost::addressof(iter), &sub_t, &bin_buf_len, &pbuf);

	//	this_type::assign(pbuf, bin_buf_len);
	//}

	template<typename BsonIter, typename String > inline
	typename 
		boost::enable_if
		< 
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable< String >
			>,
			void
		>::type
		load(const BsonIter& iter, const String& name)
	{
		assert((iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID)));

		if(!iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name, E_TYPE_ID))
		{
			container::clear(_buf);
			return;
		}

		bson_subtype_t sub_t = BSON_SUBTYPE_BINARY;
		u32 bin_buf_len = 0;
		const u8* pbuf = 0;
		bson_iter_binary(boost::addressof(iter), &sub_t, &bin_buf_len, &pbuf);

		this_type::assign(pbuf, bin_buf_len);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(BsonIter& iter)
	{
		bson_subtype_t sub_t = BSON_SUBTYPE_BINARY;
		u32 bin_buf_len = 0;
		const u8* pbuf = 0;
		bson_iter_binary(boost::addressof(iter), &sub_t, &bin_buf_len, &pbuf);

		this_type::assign(pbuf, bin_buf_len);
	}

private:
	buf_type _buf;
};

// non member foo

template<typename Container, u32 BINARY_ID> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
	operator==(const Container& l, const c_bson_binary_buffer<BINARY_ID>& r)
{
	return r.compare_eq(l);
}

template<typename Container, u32 BINARY_ID> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
	operator==(const c_bson_binary_buffer<BINARY_ID>& l, const Container& r)
{
	return l.compare_eq(r);
}

template<typename Container, u32 BINARY_ID> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
	operator!=(const Container& l, const c_bson_binary_buffer<BINARY_ID>& r)
{
	return !r.compare_eq(l);
}

template<typename Container, u32 BINARY_ID> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			container_ex::is_container<Container>,
			boost::mpl::not_
			<
				boost::is_class
				<
					typename range_ex::range_value_ex<Container>::type
				>
			>
		>,
		bool
	>::type
	operator!=(const c_bson_binary_buffer<BINARY_ID>& l, const Container& r)
{
	return !l.compare_eq(r);
}

template<u32 BINARY_ID> inline
bool operator==(const c_bson_binary_buffer<BINARY_ID>& l, const c_bson_binary_buffer<BINARY_ID>& r)
{
	return l.compare_eq(r);
}

template<u32 BINARY_ID> inline
bool operator!=(const c_bson_binary_buffer<BINARY_ID>& l, const c_bson_binary_buffer<BINARY_ID>& r)
{
	return !l.compare_eq(r);
}

// ==
template<typename BsonValue, yggr::u32 BINARY_ID> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_binary_buffer<BINARY_ID>& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue, yggr::u32 BINARY_ID> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_binary_buffer<BINARY_ID>& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

//template<yggr::u32 BINARY_ID> inline
//bool operator==(const bson_value_t& l, const c_bson_binary_buffer<BINARY_ID>& r)
//{
//	return 
//		bson_value_native_ex::s_value_cmp_eq_binary_buffer(&l, r.buffer_typeid(), r.data(), r.size());
//}
//
//template<yggr::u32 BINARY_ID> inline
//bool operator==(const c_bson_binary_buffer<BINARY_ID>& l, const bson_value_t& r)
//{
//	return 
//		bson_value_native_ex::s_value_cmp_eq_binary_buffer(&r, l.buffer_typeid(), l.data(), l.size());
//}

// !=

template<typename BsonValue, yggr::u32 BINARY_ID> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_binary_buffer<BINARY_ID>& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue, yggr::u32 BINARY_ID> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_binary_buffer<BINARY_ID>& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

//template<yggr::u32 BINARY_ID> inline
//bool operator!=(const bson_value_t& l, const c_bson_binary_buffer<BINARY_ID>& r)
//{
//	return 
//		!bson_value_native_ex::s_value_cmp_eq_binary_buffer(&l, r.buffer_typeid(), r.data(), r.size());
//}
//
//template<yggr::u32 BINARY_ID> inline
//bool operator!=(const c_bson_binary_buffer<BINARY_ID>& l, const bson_value_t& r)
//{
//	return 
//		!bson_value_native_ex::s_value_cmp_eq_binary_buffer(&r, l.buffer_typeid(), l.data(), l.size());
//}

namespace detail
{

template<u32 BINARY_ID>
struct bson_operator_outter< c_bson_binary_buffer<BINARY_ID> >
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_bson_binary_buffer<BINARY_ID>& right, 
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_binary_buffer<BINARY_ID> bin_buf_type;
		typedef typename bin_buf_type::buf_type buf_type;
		typedef yggr::string out_string_type;
		YGGR_PP_CONST_CHAR_LOCAL_DEF(char_zero, '0')

		std::ios_base::fmtflags org_flags = os.flags();
		Char org_fill = os.fill();

		std::ios_base::fmtflags hex_flags = (org_flags & ~std::ios::basefield) | std::ios::hex;
		Char hex_fill = YGGR_PP_CONST_CHAR_LOCAL_GET(char_zero, Char);

		out_string_type ostr;

		if(!right.empty())
		{
			const buf_type& buf_ref = right.buffer();
			u64 buf_byte_size = right.size();
			u64 b64_len = b64_ntop_byte_size(buf_byte_size);
			out_string_type b64_buf(b64_len, 0);
			b64_ntop(reinterpret_cast<const u8*>(&(*boost::begin(buf_ref))), buf_byte_size,
						&b64_buf[0], b64_len);
			ostr.swap(b64_buf);
		}

		switch(mode)
		{
		case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
		case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
			os << "{ \"$binary\" : { \"base64\" : \""
				<< ostr
				<< "\", \"subType\" : \"";
			os.flags(hex_flags);
			os.fill(hex_fill);
			os << std::setw(2) << BINARY_ID;
			os.flags(org_flags);
			os.fill(org_fill);
			os << "\" } }";
			break;
		default:
			os << "{ \"$binary\" : \"" 
				<< ostr
				<< "\", \"$type\" : \"";
			os.flags(hex_flags);
			os.fill(hex_fill);
			os << std::setw(2) << BINARY_ID;
			os.flags(org_flags);
			os.fill(org_fill);
			os << "\" }";
		}

		return os;
	}
};

} // namespace detail

// operator <<
template<typename Char, typename Traits, u32 BINARY_ID> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const c_bson_binary_buffer<BINARY_ID>& right)
{
	typedef c_bson_binary_buffer<BINARY_ID> bin_buf_type;
	typedef detail::bson_operator_outter<bin_buf_type> outter_type;

	outter_type outter;
	return outter(os, right, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits, u32 BINARY_ID>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF_BEG c_bson_binary_buffer<BINARY_ID> BOOST_RV_REF_END right)
//{
//	const c_bson_binary_buffer<BINARY_ID>& right_cref = right;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY> c_bson_any_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_FUNCTION> c_bson_func_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_BINARY_DEPRECATED> c_bson_old_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_UUID_DEPRECATED> c_bson_old_uuid_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_UUID> c_bson_uuid_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_MD5> c_bson_md5_binary_buffer;

typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_U8_BUFFER> c_bson_u8_binary_buffer;
typedef c_bson_binary_buffer<bson_binary_typeid_def::E_BSON_BIN_TYPE_PACKET> c_bson_packet_binary_buffer;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

template<yggr::u32 BINARY_ID> inline 
void swap(typename c_bson_binary_buffer<BINARY_ID>::buf_type& l, c_bson_binary_buffer<BINARY_ID>& r) 
{ 
	r.swap(l); 
}

template<yggr::u32 BINARY_ID> inline 
void swap(c_bson_binary_buffer<BINARY_ID>& l, typename c_bson_binary_buffer<BINARY_ID>::buf_type& r) 
{ 
	l.swap(r); 
}

template<yggr::u32 BINARY_ID> inline 
void swap(c_bson_binary_buffer<BINARY_ID>& l, c_bson_binary_buffer<BINARY_ID>& r) 
{ 
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_BINARY_BUFFER_HPP__
