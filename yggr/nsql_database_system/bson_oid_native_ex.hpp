//bson_oid_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_OID_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_OID_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/charset/hex_converter.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/bytes/byte_size.hpp>
#include <yggr/algorithm/lexicographical_c_compare.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/size.hpp>

#include <yggr/container_ex/container_category_check.hpp>

#include <yggr/range_ex/range_value_ex.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>

#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>
#include <yggr/nsql_database_system/detail/bson_value_conflict_fixer.hpp>

#include <yggr/math/sign.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/range/functions.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/iterator/iterator_traits.hpp>

/*
typedef struct {
   uint8_t bytes[12];
} bson_oid_t;
*/

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

// compare_eq

template<typename Cont,
			typename Is_Random_Access = typename container_ex::is_random_access_container<Cont>::type >
struct bson_oid_native_ex_compare_eq_helper;

template<typename Cont>
struct bson_oid_native_ex_compare_eq_helper<Cont, containter_category_check_result::true_type >
{
	typedef Cont cont_type;
	typedef typename range_ex::range_value_ex<Cont>::type value_type;

	inline bool operator()(const bson_oid_t& l, const cont_type& r) const
	{
		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		std::size_t rbyte_size = bytes::byte_size(r);

		return
			(rbyte_size == sizeof(bson_oid_t))
			&& (0 == memcmp(boost::addressof(l), boost::addressof(*boost::begin(r)), sizeof(bson_oid_t)));
	}
};

template<typename Cont>
struct bson_oid_native_ex_compare_eq_helper<Cont, containter_category_check_result::false_type>
{
	typedef Cont cont_type;
	typedef typename range_ex::range_value_ex<Cont>::type value_type;

	inline bool operator()(const bson_oid_t& l, const cont_type& r) const
	{
		typedef yggr::vector<value_type> vt_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		vt_type vt(boost::begin(r), boost::end(r));

		return
			(bytes::byte_size(vt) == sizeof(bson_oid_t))
			&& (0 == memcmp(boost::addressof(l), boost::addressof(vt[0]), sizeof(bson_oid_t)));
	}
};



// compare

template<typename Cont,
			typename Is_Random_Access = typename container_ex::is_random_access_container<Cont>::type >
struct bson_oid_native_ex_compare_helper;

template<typename Cont>
struct bson_oid_native_ex_compare_helper<Cont, containter_category_check_result::true_type >
{
	typedef Cont cont_type;
	typedef typename range_ex::range_value_ex<Cont>::type value_type;

	inline s32 operator()(const bson_oid_t& l, const cont_type& r) const
	{
		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		std::size_t r_size = yggr::size(r);
		std::size_t rbyte_size = r_size * sizeof(value_type);

		const u8* pbuf_beg = rbyte_size? reinterpret_cast<const u8*>(boost::addressof(*boost::begin(r))) : 0;

		if(rbyte_size == sizeof(bson_oid_t))
		{
			assert(pbuf_beg);
			return math::sign(memcmp(boost::addressof(l), pbuf_beg, sizeof(bson_oid_t)));
		}
		else
		{
			const u8* pbuf_end = rbyte_size? reinterpret_cast<const u8*>(advance_copy(boost::addressof(*boost::begin(r)), r_size)) : 0;

			return
				algorithm::lexicographical_c_compare(
					reinterpret_cast<const u8*>(boost::addressof(l)),
					reinterpret_cast<const u8*>(boost::addressof(l) + 1),
					pbuf_beg, pbuf_end, algorithm::c_hex_cmp());
		}
	}
};

template<typename Cont>
struct bson_oid_native_ex_compare_helper<Cont, containter_category_check_result::false_type>
{
	typedef Cont cont_type;
	typedef typename range_ex::range_value_ex<Cont>::type value_type;

	inline s32 operator()(const bson_oid_t& l, const Cont& r) const
	{
		typedef yggr::vector<value_type> vt_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		vt_type vt(boost::begin(r), boost::end(r));

		std::size_t r_size = vt.size();
		std::size_t rbyte_size = bytes::byte_size(vt);

		const u8* pbuf_beg = rbyte_size? reinterpret_cast<const u8*>(boost::addressof(*boost::begin(vt))) : 0;

		if(rbyte_size == sizeof(bson_oid_t))
		{
			assert(pbuf_beg);
			return math::sign(memcmp(boost::addressof(l), pbuf_beg, sizeof(bson_oid_t)));
		}
		else
		{
			const u8* pbuf_end = rbyte_size? reinterpret_cast<const u8*>(advance_copy(boost::addressof(*boost::begin(vt)), r_size)) : 0;

			return
				algorithm::lexicographical_c_compare(
					reinterpret_cast<const u8*>(boost::addressof(l)),
					reinterpret_cast<const u8*>(boost::addressof(l) + 1),
					pbuf_beg, pbuf_end, algorithm::c_hex_cmp());
		}
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class bson_oid_native_ex
{
public:
	YGGR_STATIC_CONSTANT(u32, E_bson_oid_byte_size = sizeof(bson_oid_t));
	YGGR_STATIC_CONSTANT(u32, E_bson_oid_string_size = (E_bson_oid_byte_size << 1)); // E_bson_oid_byte_size * 2
	YGGR_STATIC_CONSTANT(u32, E_bson_oid_char_buffer_size = (E_bson_oid_string_size + 1)); // E_bson_oid_string_size + 1

private:
	typedef bson_oid_native_ex this_type;

public:
	static bool s_bson_oid_swap(bson_oid_t* pl, bson_oid_t* pr);

public:
	// compare_eq
	static bool s_bson_oid_compare_eq(const bson_oid_t* poid, const char* str_oid);
	static bool s_bson_oid_compare_eq(const bson_oid_t* poid, const char* str_oid, std::size_t n);

#ifndef YGGR_NO_CHAR8_T
	static bool s_bson_oid_compare_eq(const bson_oid_t* poid, const c8* str_oid);
	static bool s_bson_oid_compare_eq(const bson_oid_t* poid, const c8* str_oid, std::size_t n);
#endif // YGGR_NO_CHAR8_T

	template<typename Char> inline
	static typename boost::enable_if<is_native_char<Char>, bool>::type
		s_bson_oid_compare_eq(const bson_oid_t* poid, const Char* str_oid)
	{
		typedef yggr::utf8_string inner_string_type;

		assert(poid);

		inner_string_type tmp_str_oid(str_oid);
		bson_oid_t tmp = {0};

		bson_oid_init_from_string(boost::addressof(tmp), tmp_str_oid.data());
		return (poid) && (0 == memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size));
	}

	static bool s_bson_oid_compare_eq(const bson_oid_t* pl, const bson_oid_t* pr);

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static typename boost::enable_if< charset::is_string_t< Basic_String<char, Traits, Alloc> >, bool >::type
		s_bson_oid_compare_eq(const bson_oid_t* poid, const Basic_String<char, Traits, Alloc>& str_oid)
	{
		typedef Basic_String<char, Traits, Alloc> now_string_type;

		assert(poid);

		bson_oid_t tmp = {0};

		return
			poid
			&& (((str_oid.size() == this_type::E_bson_oid_string_size)
					&& (bson_oid_init_from_string(boost::addressof(tmp), str_oid.data()), true)
					&& (0 == memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) )
				|| ((str_oid.size() == this_type::E_bson_oid_byte_size)
					&& (0 == memcmp(poid->bytes, str_oid.data(), this_type::E_bson_oid_byte_size)) ) );
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
		s_bson_oid_compare_eq(const bson_oid_t* poid, const Basic_String<Char, Traits, Alloc>& str_oid)
	{
		typedef yggr::utf8_string inner_string_type;

		assert(poid);

		inner_string_type tmp_str_oid(str_oid);
		return this_type::s_bson_oid_compare_eq(poid, tmp_str_oid);
	}

	template<typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static typename boost::enable_if< charset::is_string_view_t< BasicStringView<char, Traits> >, bool >::type
		s_bson_oid_compare_eq(const bson_oid_t* poid, const BasicStringView<char, Traits>& str_oid)
	{
		assert(poid);

		bson_oid_t tmp = {0};

		return
			poid
			&& (((str_oid.size() == this_type::E_bson_oid_string_size)
					&& (bson_oid_init_from_string(boost::addressof(tmp), str_oid.data()), true)
					&& (0 == memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) )
				|| ((str_oid.size() == this_type::E_bson_oid_byte_size)
					&& (0 == memcmp(poid->bytes, str_oid.data(), this_type::E_bson_oid_byte_size)) ) );
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static typename boost::enable_if< charset::is_string_view_t< BasicStringView<Char, Traits> >, bool >::type
		s_bson_oid_compare_eq(const bson_oid_t* poid, const BasicStringView<Char, Traits>& str_oid)
	{
		typedef yggr::utf8_string inner_string_type;

		assert(poid);

		inner_string_type tmp_str_oid(str_oid);
		return this_type::s_bson_oid_compare_eq(poid, tmp_str_oid);
	}

	template<typename T> inline
	static typename boost::disable_if<is_native_char<T>, bool>::type
		s_bson_oid_compare_eq(const bson_oid_t* poid, const T* data)
	{
		assert(poid);
		return (poid) && (data) && (0 == memcmp(data, poid, this_type::E_bson_oid_byte_size));
	}

	template<typename Cont> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_container<Cont>,
			bool
		>::type
		s_bson_oid_compare_eq(const bson_oid_t* l, const Cont& r)
	{
		typedef Cont container_type;
		typedef detail::bson_oid_native_ex_compare_eq_helper<container_type> helper_type;

		assert(l);

		helper_type helper;
		return l && helper(*l, r);
	}

	template<typename InputIter> inline
	static typename boost::enable_if<iterator_ex::is_random_access_iterator<InputIter>, bool>::type
		s_bson_oid_compare_eq(const bson_oid_t* l, InputIter rs, InputIter re)
	{
		typedef InputIter iter_type;
		typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_val_type;
		typedef typename boost::remove_cv<iter_val_type>::type value_type;
		typedef yggr::vector<value_type> vt_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));
		assert(l);

		return
			l
			&& (bytes::byte_size(rs, re) == this_type::E_bson_oid_byte_size)
			&& 0 == memcmp(l, boost::addressof(*rs), this_type::E_bson_oid_byte_size);
	}

	template<typename InputIter> inline
	static typename boost::disable_if<iterator_ex::is_random_access_iterator<InputIter>, bool>::type
		s_bson_oid_compare_eq(const bson_oid_t* l, InputIter rs, InputIter re)
	{
		typedef InputIter iter_type;
		typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_val_type;
		typedef typename boost::remove_cv<iter_val_type>::type value_type;
		typedef yggr::vector<value_type> vt_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		vt_type vt(rs, re);

		return this_type::s_bson_oid_compare_eq(l, vt);
	}

public:
	// compare
	static s32 s_bson_oid_compare(const bson_oid_t* poid, const char* str_oid);
	static s32 s_bson_oid_compare(const bson_oid_t* poid, const char* str_oid, std::size_t n);

#ifndef YGGR_NO_CHAR8_T
	static s32 s_bson_oid_compare(const bson_oid_t* poid, const c8* str_oid);
	static s32 s_bson_oid_compare(const bson_oid_t* poid, const c8* str_oid, std::size_t n);
#endif // YGGR_NO_CHAR8_T

	template<typename Char> inline
	static typename boost::enable_if<is_native_char<Char>, s32>::type
		s_bson_oid_compare(const bson_oid_t* poid, const Char* str_oid)
	{
		typedef yggr::utf8_string inner_string_type;

		assert(poid);

		inner_string_type tmp_str_oid(str_oid);
		bson_oid_t tmp = {0};

		bson_oid_init_from_string(boost::addressof(tmp), tmp_str_oid.data());
		return poid? math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size)) : str_oid? -1 : 0 ;
	}

	static s32 s_bson_oid_compare(const bson_oid_t* pl, const bson_oid_t* pr);

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static typename boost::enable_if< charset::is_string_t< Basic_String<char, Traits, Alloc> >, s32 >::type
		s_bson_oid_compare(const bson_oid_t* poid, const Basic_String<char, Traits, Alloc>& str_oid)
	{
		typedef Basic_String<char, Traits, Alloc> now_string_type;

		assert(poid);

		bson_oid_t tmp = {0};

		if(str_oid.size() == this_type::E_bson_oid_string_size)
		{
			bson_oid_init_from_string(boost::addressof(tmp), str_oid.data());
			return
				(poid)?
					math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size))
					: algorithm::lexicographical_c_compare(
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(boost::addressof(tmp)),
						reinterpret_cast<const u8*>(boost::addressof(tmp) + 1),
						algorithm::c_hex_cmp());
		}
		else if(str_oid.size() == this_type::E_bson_oid_byte_size)
		{
			return
				(poid)?
					math::sign(memcmp(poid, str_oid.data(), this_type::E_bson_oid_byte_size))
					: algorithm::lexicographical_c_compare(
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(boost::addressof(tmp)),
						reinterpret_cast<const u8*>(boost::addressof(tmp) + 1),
						algorithm::c_hex_cmp());
		}
		else
		{
			::yggr::string hex_buf;
			const u8 *pbeg = 0, *pend = 0;
			if(charset::hex_converter::s_string_to_hex(hex_buf, str_oid))
			{
				pbeg = reinterpret_cast<const u8*>(hex_buf.data());
				pend = reinterpret_cast<const u8*>(advance_copy(hex_buf.data(), hex_buf.size()));
			}

			return
				algorithm::lexicographical_c_compare(
					reinterpret_cast<const u8*>(poid),
					reinterpret_cast<const u8*>(poid) + (poid? this_type::E_bson_oid_byte_size : 0),
					pbeg,
					pend,
					algorithm::c_hex_cmp());
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, s32 >::type
		s_bson_oid_compare(const bson_oid_t* poid, const Basic_String<Char, Traits, Alloc>& str_oid)
	{
		// wchar_t only
		typedef yggr::utf8_string inner_string_type;

		assert(poid);

		inner_string_type tmp_str_oid(str_oid);
		return this_type::s_bson_oid_compare(poid, tmp_str_oid);
	}

	template<typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static typename boost::enable_if< charset::is_string_view_t< BasicStringView<char, Traits> >, s32 >::type
		s_bson_oid_compare(const bson_oid_t* poid, const BasicStringView<char, Traits>& str_oid)
	{
		typedef BasicStringView<char, Traits> now_string_viwe_type;

		assert(poid);

		bson_oid_t tmp = {0};

		if(str_oid.size() == this_type::E_bson_oid_string_size)
		{
			bson_oid_init_from_string(boost::addressof(tmp), str_oid.data());
			return
				(poid)?
					math::sign(memcmp(poid, boost::addressof(tmp), this_type::E_bson_oid_byte_size))
					: algorithm::lexicographical_c_compare(
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(boost::addressof(tmp)),
						reinterpret_cast<const u8*>(boost::addressof(tmp) + 1),
						algorithm::c_hex_cmp());
		}
		else if(str_oid.size() == this_type::E_bson_oid_byte_size)
		{
			return
				(poid)?
					math::sign(memcmp(poid, str_oid.data(), this_type::E_bson_oid_byte_size))
					: algorithm::lexicographical_c_compare(
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(poid),
						reinterpret_cast<const u8*>(boost::addressof(tmp)),
						reinterpret_cast<const u8*>(boost::addressof(tmp) + 1),
						algorithm::c_hex_cmp());
		}
		else
		{
			::yggr::string hex_buf;
			const u8 *pbeg = 0, *pend = 0;
			if(charset::hex_converter::s_string_to_hex(hex_buf, str_oid.data(), str_oid.size()))
			{
				pbeg = reinterpret_cast<const u8*>(hex_buf.data());
				pend = reinterpret_cast<const u8*>(advance_copy(hex_buf.data(), hex_buf.size()));
			}

			return
				algorithm::lexicographical_c_compare(
					reinterpret_cast<const u8*>(poid),
					reinterpret_cast<const u8*>(poid) + (poid? this_type::E_bson_oid_byte_size : 0),
					pbeg,
					pend,
					algorithm::c_hex_cmp());
		}
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static typename boost::enable_if< charset::is_string_t< BasicStringView<Char, Traits> >, s32 >::type
		s_bson_oid_compare(const bson_oid_t* poid, const BasicStringView<Char, Traits>& str_oid)
	{
		typedef yggr::utf8_string inner_string_type;

		assert(poid);

		inner_string_type tmp_str_oid(str_oid);
		return this_type::s_bson_oid_compare(poid, tmp_str_oid);
	}

	template<typename T> inline
	static typename boost::disable_if<is_native_char<T>, s32>::type
		s_bson_oid_compare(const bson_oid_t* poid, const T* data)
	{
		assert(poid);
		return
			algorithm::lexicographical_c_compare(
				reinterpret_cast<const u8*>(poid),
				reinterpret_cast<const u8*>(poid) + (poid? this_type::E_bson_oid_byte_size : 0),
				reinterpret_cast<const u8*>(data),
				reinterpret_cast<const u8*>(data) + (data? this_type::E_bson_oid_byte_size : 0),
				algorithm::c_hex_cmp());
	}

public:
	template<typename Cont> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_container<Cont>,
			s32
		>::type
		s_bson_oid_compare(const bson_oid_t* l, const Cont& r)
	{
		typedef Cont container_type;
		typedef detail::bson_oid_native_ex_compare_helper<container_type> helper_type;

		assert(l);

		helper_type helper;
		return l? helper(*l, r) : (0 == ::yggr::size(r)? 0 : -1);
	}

	template<typename InputIter> inline
	static typename boost::enable_if<iterator_ex::is_random_access_iterator<InputIter>, s32>::type
		s_bson_oid_compare(const bson_oid_t* l, InputIter rs, InputIter re)
	{
		typedef InputIter iter_type;
		typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_val_type;
		typedef typename boost::remove_cv<iter_val_type>::type value_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));
		assert(l);

		std::size_t rbyte_size = bytes::byte_size(rs, re);

		return
			l && (rbyte_size == this_type::E_bson_oid_byte_size)?
				math::sign(memcmp(l, boost::addressof(*rs), this_type::E_bson_oid_byte_size))
				: algorithm::lexicographical_c_compare(
					reinterpret_cast<const u8*>(l),
					reinterpret_cast<const u8*>(l) + (l? this_type::E_bson_oid_byte_size : 0),
					reinterpret_cast<const u8*>(rs == re? 0 : boost::addressof(*rs)),
					reinterpret_cast<const u8*>(rs == re? 0 : boost::addressof(*rs)) + rbyte_size,
					algorithm::c_hex_cmp());
	}



	template<typename InputIter> inline
	static typename boost::disable_if<iterator_ex::is_random_access_iterator<InputIter>, s32>::type
		s_bson_oid_compare(const bson_oid_t* l, InputIter rs, InputIter re)
	{
		typedef InputIter iter_type;
		typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_val_type;
		typedef typename boost::remove_cv<iter_val_type>::type value_type;
		typedef yggr::vector<value_type> vt_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<value_type>));

		vt_type vt(rs, re);

		return this_type::s_bson_oid_compare(l, vt);
	}
};

} // namespace nsql_database_system
} // namespace yggr

// bson_oid_t
// ==
template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator==(const Char* l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator==(const bson_oid_t& l, const Char* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator==(const Basic_String<Char, Traits, Alloc>& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator==(const bson_oid_t& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator==(const T* l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator==(const bson_oid_t& l, const T* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator==(const Cont& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator==(const bson_oid_t& l, const Cont& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_eq_oid(
			boost::addressof(l), boost::addressof(r));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const bson_oid_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_eq_oid(
			boost::addressof(r), boost::addressof(l));
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator==(const bson_oid_t& l, const BsonOid& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), boost::addressof(r));
}

// !=
template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator!=(const Char* l, const bson_oid_t& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator!=(const bson_oid_t& l, const Char* r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator!=(const Basic_String<Char, Traits, Alloc>& l, const bson_oid_t& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator!=(const bson_oid_t& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator!=(const T* l, const bson_oid_t& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator!=(const bson_oid_t& l, const T* r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator!=(const Cont& l, const bson_oid_t& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(r), l);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator!=(const bson_oid_t& l, const Cont& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const bson_oid_t& r)
{
	return
		!yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_eq_oid(
			boost::addressof(l), boost::addressof(r));
}


template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const bson_oid_t& l, const BsonValue& r)
{
	return
		!yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_eq_oid(
			boost::addressof(r), boost::addressof(l));
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator!=(const bson_oid_t& l, const BsonOid& r)
{
	return
		!yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare_eq(
			boost::addressof(l), boost::addressof(r));
}

// <
template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator<(const Char* l, const bson_oid_t& r)
{
	return
		0 < yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator<(const bson_oid_t& l, const Char* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) < 0;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator<(const Basic_String<Char, Traits, Alloc>& l, const bson_oid_t& r)
{
	return
		0 < yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(r), l);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator<(const bson_oid_t& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) < 0;
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator<(const T* l, const bson_oid_t& r)
{
	return
		0 < yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(r), l);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator<(const bson_oid_t& l, const T* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) < 0;
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator<(const Cont& l, const bson_oid_t& r)
{
	return
		0 < yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator<(const bson_oid_t& l, const Cont& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(l), boost::addressof(r)) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const bson_oid_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(r), boost::addressof(l)) > 0;
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator<(const bson_oid_t& l, const BsonOid& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), boost::addressof(r)) < 0;
}

// <=
template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator<=(const Char* l, const bson_oid_t& r)
{
	return
		0 <= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator<=(const bson_oid_t& l, const Char* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) <= 0;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator<=(const Basic_String<Char, Traits, Alloc>& l, const bson_oid_t& r)
{
	return
		0 <= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator<=(const bson_oid_t& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) <= 0;
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator<=(const T* l, const bson_oid_t& r)
{
	return
		0 <= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator<=(const bson_oid_t& l, const T* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) <= 0;
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator<=(const Cont& l, const bson_oid_t& r)
{
	return
		0 <= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator<=(const bson_oid_t& l, const Cont& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(l), boost::addressof(r)) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const bson_oid_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(r), boost::addressof(l)) >= 0;
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator<=(const bson_oid_t& l, const BsonOid& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), boost::addressof(r)) <= 0;
}

// >
template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator>(const Char* l, const bson_oid_t& r)
{
	return
		0 > yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator>(const bson_oid_t& l, const Char* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) > 0;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator>(const Basic_String<Char, Traits, Alloc>& l, const bson_oid_t& r)
{
	return
		0 > yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator>(const bson_oid_t& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) > 0;
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator>(const T* l, const bson_oid_t& r)
{
	return
		0 > yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator>(const bson_oid_t& l, const T* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) > 0;
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator>(const Cont& l, const bson_oid_t& r)
{
	return
		0 > yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator>(const bson_oid_t& l, const Cont& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(l), boost::addressof(r)) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const bson_oid_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(r), boost::addressof(l)) < 0;
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator>(const bson_oid_t& l, const BsonOid& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), boost::addressof(r)) > 0;
}

// >=

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator>=(const Char* l, const bson_oid_t& r)
{
	return
		0 >= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char> inline
typename boost::enable_if<yggr::is_native_char<Char>, bool>::type
	operator>=(const bson_oid_t& l, const Char* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) >= 0;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator>=(const Basic_String<Char, Traits, Alloc>& l, const bson_oid_t& r)
{
	return
		0 >= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename boost::enable_if< yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator>=(const bson_oid_t& l, const Basic_String<Char, Traits, Alloc>& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) >= 0;
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator>=(const T* l, const bson_oid_t& r)
{
	return
		0 >= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename T> inline
typename boost::disable_if<yggr::is_native_char<T>, bool>::type
	operator>=(const bson_oid_t& l, const T* r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) >= 0;
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator>=(const Cont& l, const bson_oid_t& r)
{
	return
		0 >= yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
				boost::addressof(r), l);
}

template<typename Cont> inline
typename boost::enable_if<yggr::container_ex::is_container<Cont>, bool>::type
	operator>=(const bson_oid_t& l, const Cont& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), r) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const bson_oid_t& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(l), boost::addressof(r)) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const bson_oid_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::detail::bson_value_conflict_fixer::s_value_cmp_oid(
			boost::addressof(r), boost::addressof(l)) <= 0;
}

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, bool>::type
	operator>=(const bson_oid_t& l, const BsonOid& r)
{
	return
		yggr::nsql_database_system::bson_oid_native_ex::s_bson_oid_compare(
			boost::addressof(l), boost::addressof(r)) >= 0;
}

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<>
struct bson_operator_outter<bson_oid_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const bson_oid_t& val,
					bson_json_mode_def::mode_type) const
	{
		yggr::string oid_str(bson_oid_native_ex::E_bson_oid_string_size, 0);
		bson_oid_to_string(&val, &(oid_str[0]));

		os << "{ "
			<< "\"$oid\" : \"" << oid_str.data() << "\""
			<< " }";

		return os;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

// <<
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_oid_t& val)
{
	typedef yggr::nsql_database_system::detail::bson_operator_outter<bson_oid_t> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_oid_t& l, bson_oid_t& r);

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, void>::type
	swap(bson_oid_t& l, BsonOid& r)
{
	bson_oid_native_ex::s_bson_oid_swap(boost::addressof(l), boost::addressof(r));
}

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename BsonOid> inline
typename boost::enable_if<boost::is_base_of<bson_oid_t, BsonOid>, void>::type
	swap(bson_oid_t& l, BOOST_RV_REF(BsonOid) r)
{
    BsonOid& right_ref = r;
	bson_oid_native_ex::s_bson_oid_swap(boost::addressof(l), boost::addressof(right_ref));
}
#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_OID_NATIVE_EX_HPP__
