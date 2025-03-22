//utf8_foo.hpp

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

#ifndef __YGGR_CHARSET_UTF8_FOO_HPP__
#define __YGGR_CHARSET_UTF8_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/move/move.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/type_traits/is_convertible_size_t.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/typedef.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/type_traits/number_t.hpp>

#include <yggr/math/modulus.hpp>
#include <yggr/math/sign.hpp>

#if defined(YGGR_AT_ANDROID)
#	include <yggr/math/detail/android_abs_patch.hpp>
#endif // #if defined(YGGR_AT_ANDROID)

#include <yggr/container/clear.hpp>
#include <yggr/container/resize.hpp>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/str_wstr_converter.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <complex>
#include <algorithm>
#include <utility>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

class utf8_foo
	: private nonable::noncreateable
{
public:
	typedef ::yggr::size_type size_type;
private:
	typedef utf8_foo this_type;

public:
#ifdef YGGR_USE_UTF8_EX_MODE
	YGGR_STATIC_CONSTANT(yggr::u32, E_char_max_size = 6);
#else
	YGGR_STATIC_CONSTANT(yggr::u32, E_char_max_size = 4);
#endif // YGGR_USE_UTF8_EX_MODE

#ifdef YGGR_USE_UTF8_EX_MODE
	YGGR_STATIC_CONSTANT(s64, E_char_max_num = static_cast<s64>(0x7fffffff));
	YGGR_STATIC_CONSTANT(s64, E_char_max_range = static_cast<s64>(0x7fffffff) + 1);
#else
	YGGR_STATIC_CONSTANT(s32, E_char_max_num = 0x0010ffff);
	YGGR_STATIC_CONSTANT(s32, E_char_max_range = 0x0010ffff + 1);
#endif // YGGR_USE_UTF8_EX_MODE

	YGGR_STATIC_CONSTANT(size_type, npos = static_cast<size_type>(size_type(-1)));

public:
	//---------------utf8_char------------------------------

	// char_to_number
	inline static s32 char_to_number(const char* c, size_type ch_len = -1)
	{
		ch_len = ch_len == static_cast<size_type>(-1)? this_type::charlen(c) : ch_len;

		assert(0 < ch_len && ch_len <= E_char_max_size);
		assert(this_type::charlen(c) == ch_len);

		switch(ch_len)
		{
		case 1:
			return static_cast<s32>(*c);
		case 2:
			return
				(static_cast<s32>(c[1] & 0x7f))
					| (static_cast<s32>(c[0] & (u8(-1) >> (ch_len + 1))) << 6);

		case 3:
			return
				(static_cast<s32>(c[2] & 0x7f))
					| (static_cast<s32>(c[1] & 0x7f) << 6)
					| (static_cast<s32>(c[0] & (u8(-1) >> (ch_len + 1))) << 12);

		case 4:
			return
				(static_cast<s32>(c[3] & 0x7f))
					| (static_cast<s32>(c[2] & 0x7f) << 6)
					| (static_cast<s32>(c[1] & 0x7f) << 12)
					| (static_cast<s32>(c[0] & (u8(-1) >> (ch_len + 1))) << 18);

#	ifdef YGGR_USE_UTF8_EX_MODE
		case 5:
			return
				(static_cast<s32>(c[4] & 0x7f))
					| (static_cast<s32>(c[3] & 0x7f) << 6)
					| (static_cast<s32>(c[2] & 0x7f) << 12)
					| (static_cast<s32>(c[1] & 0x7f) << 18)
					| (static_cast<s32>(c[0] & (u8(-1) >> (ch_len + 1))) << 24);
		case 6:
			return
				(static_cast<u32>(c[5] & 0x7f))
					| (static_cast<s32>(c[4] & 0x7f) << 6)
					| (static_cast<s32>(c[3] & 0x7f) << 12)
					| (static_cast<s32>(c[2] & 0x7f) << 18)
					| (static_cast<s32>(c[1] & 0x7f) << 24)
					| (static_cast<s32>(c[0] & (u8(-1) >> (ch_len + 1))) << 30);
#	endif // YGGR_USE_UTF8_EX_MODE
		default:
			assert(false); // is not utf8 char
			return s32(-1);
		}
	}

	template<typename Char> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			s32
		>::type
		char_to_number(const Char* pc, size_type n = size_type(-1)) // n is byte_size
	{
		assert(pc);
		return this_type::char_to_number(this_type::get_const_char_address(pc), n);
	}

	template<typename InputIter> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< boost::is_pointer< InputIter > >
			>,
			s32
		>::type
		char_to_number(InputIter start, size_type n = size_type(-1)) // n is byte_size
	{
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		return this_type::char_to_number(this_type::get_const_char_address(start), n);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					boost::mpl::or_
					<
						is_iterator< Basic_String<Char, Traits, Alloc> >,
						is_utf8_char_t< Basic_String<Char, Traits, Alloc> >
					>
				>
			>,
			s32
		>::type
		char_to_number(const Basic_String<Char, Traits, Alloc>& c)
	{
		assert(this_type::strlen(c) == 1);
		size_type byte_size = c.size() * sizeof(Char);

		return ((size_type(E_char_max_size) < byte_size) || (c.empty()))?
				0 : this_type::char_to_number(this_type::get_const_char_address(c), byte_size);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_utf8_char_t< Basic_String<Char, Traits, Alloc> >,
			s32
		>::type
		char_to_number(const Basic_String<Char, Traits, Alloc>& c)
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(Char) == 1>));

		assert(c.size());
		return this_type::char_to_number(c.data(), c.size());
	}

	// number to char

protected:
	YGGR_CXX14_CONSTEXPR_OR_INLINE static char* pro_s_number_to_char(char* buf, s32 num, size_type buf_max_size, size_type& used_size)
	{
		assert(buf);
		assert((0 <= num && num <= E_char_max_num));

		if(1 <= buf_max_size && 0x00000000 <= num && num < 0x00000080) // 1byte
		{
			buf[0] = static_cast<char>(num);
			used_size = 1;
		}
		else if(2 <= buf_max_size && 0x00000080 <= num && num < 0x00000800) // 2byte
		{
			buf[0] = 0xc0 | static_cast<char>(num >> 6);
			buf[1] = 0x80 | static_cast<char>(num & 0x0000003f);
			used_size = 2;
		}
		else if(3 <= buf_max_size && 0x00000800 <= num && num < 0x00010000) // 3byte
		{
			buf[0] = 0xe0 | static_cast<char>(num >> 12);
			buf[1] = 0x80 | static_cast<char>((num >> 6) & 0x0000003f);
			buf[2] = 0x80 | static_cast<char>(num & 0x0000003f);
			used_size = 3;
		}
		else if(4 <= buf_max_size && 0x00010000 <= num && num < 0x00110000) // 4byte
		{
			buf[0] = 0xf0 | static_cast<char>(num >> 18);
			buf[1] = 0x80 | static_cast<char>((num >> 12) & 0x0000003f);
			buf[2] = 0x80 | static_cast<char>((num >> 6) & 0x0000003f);
			buf[3] = 0x80 | static_cast<char>(num & 0x0000003f);
			used_size = 4;
		}
#	if defined(YGGR_USE_UTF8_EX_MODE)
		else if(5 <= buf_max_size && 0x00200000 <= num && num < 0x04000000) // 5byte
		{
			buf[0] = 0xf8 | static_cast<char>(num >> 24);
			buf[1] = 0x80 | static_cast<char>((num >> 18) & 0x0000003f);
			buf[2] = 0x80 | static_cast<char>((num >> 12) & 0x0000003f);
			buf[3] = 0x80 | static_cast<char>((num >> 6) & 0x0000003f);
			buf[4] = 0x80 | static_cast<char>(num & 0x0000003f);
			used_size = 5;
		}
		else if(6 <= buf_max_size && 0x04000000 <= num && num <= 0x7fffffff) // 6byte
		{
			buf[0] = 0xfc | static_cast<char>(num >> 30);
			buf[1] = 0x80 | static_cast<char>((num >> 24) & 0x0000003f);
			buf[2] = 0x80 | static_cast<char>((num >> 18) & 0x0000003f);
			buf[3] = 0x80 | static_cast<char>((num >> 12) & 0x0000003f);
			buf[4] = 0x80 | static_cast<char>((num >> 6) & 0x0000003f);
			buf[5] = 0x80 | static_cast<char>(num & 0x0000003f);
			used_size = 6;
		}
#	endif // YGGR_USE_UTF8_EX_MODE
		else
		{
			assert(false);
			used_size = 0;
		}

		return used_size? buf : static_cast<char*>(0);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE static char* number_to_char(char* buf, s32 num, size_type buf_max_size, size_type& used_size)
	{
		return this_type::pro_s_number_to_char(buf, num, buf_max_size, used_size);
	}

	// why using SizeT, because in apple_cc yggr::size_type(unisgned long) not same std::string::size_type(unsigned long long)
	template<typename SizeT> YGGR_CXX14_CONSTEXPR_OR_INLINE 
	static char* number_to_char(char* buf, s32 num, size_type buf_max_size, SizeT& used_size)
	{
		size_type tmp_used_size = 0;
		char* ret = this_type::pro_s_number_to_char(buf, num, buf_max_size, tmp_used_size);
		used_size = static_cast<SizeT>(tmp_used_size);
		return ret;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE static char* number_to_char(char* buf, s32 num, size_type buf_max_size)
	{
		size_type tmp = 0;
		return this_type::pro_s_number_to_char(buf, num, buf_max_size, tmp);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE static s32 negative_number(s32 num)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE(sizeof(utf8_foo::E_char_max_range), s32) step_type;

		assert(0 <= num && step_type(num) < step_type(E_char_max_range));
		return num? static_cast<s32>( step_type(E_char_max_range) - static_cast<step_type>(num)) : num;
	}

	template<typename Step> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::is_same<YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(E_char_max_range), Step), Step>,
			Step
		>::type
		format_number_step(Step step)
	{
		typedef Step step_type;

		return std::abs(step) >= step_type(E_char_max_range)?
				math::modulus(step, step_type(E_char_max_range)) : step;
	}

	template<typename Step> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::disable_if
		<
			boost::is_same<YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(E_char_max_range), Step), Step>,
			YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(E_char_max_range), Step)
		>::type
		format_number_step(Step nstep)
	{
		typedef YGGR_PP_LOW_NUMBER_T_TYPE_TPL(sizeof(E_char_max_range), Step) step_type;

		step_type step = static_cast<step_type>(nstep);

		return
			std::abs(step) >= step_type(E_char_max_range)?
				math::modulus(step, step_type(E_char_max_range)) : step;
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static size_type char_max_size(void)
	{
		return static_cast<size_type>(this_type::E_char_max_size);
	}

public:
	// charlen
	YGGR_CONSTEXPR_OR_INLINE static size_type charlen(char c)
	{
		return static_cast<size_type>((c & 0x80) == 0x00);
	}

	// !!! if "pc" is unicode this first byte can be pass char len,
	// !!! so using "n" ctrl this issues,
	// !!! but if "pc" is unicode and the (pc[0] & 0x80 == 0) and "n" is 1,
	// !!! then it will passed charlen and return 1
	// !!! "n" is byte size

	YGGR_CXX14_CONSTEXPR_OR_INLINE static size_type charlen(const char* pc, size_type n = static_cast<size_type>(-1))
	{
		YGGR_MPLEX_STATIC_ASSERT(( boost::mpl::bool_< (0 < static_cast<size_type>(-1)) >));

		size_type ret = 0;

		if(pc)
		{
			char c = *pc;

			if((c & 0x80) == 0x00)
			{
				// if need check n return n == 1? 1: 0 else return 1
				// because unicode the first byte can pass this check
				// so if you give the function is non-utf8 char, the result is not right

				ret = size_type(E_char_max_size) < n? 1 : (n == 1? 1 : (assert(false), 0));
			}
#		ifdef YGGR_USE_UTF8_EX_MODE
			else if(((c & 0xfe) == 0xfc) && (6 <= n)
					&& (((pc[1] & 0xc0) == 0x80)
						&& ((pc[2] & 0xc0) == 0x80)
						&& ((pc[3] & 0xc0) == 0x80)
						&& ((pc[4] & 0xc0) == 0x80)
						&& ((pc[5] & 0xc0) == 0x80)))
			{
				ret = 6;
			}
			else if(((c & 0xfc) == 0xf8) && (5 <= n)
					&& (((pc[1] & 0xc0) == 0x80)
						&& ((pc[2] & 0xc0) == 0x80)
						&& ((pc[3] & 0xc0) == 0x80)
						&& ((pc[4] & 0xc0) == 0x80)))
			{
				ret = 5;
			}
#		endif // YGGR_USE_UTF8_EX_MODE
			else if(((c & 0xf8) == 0xf0) && (4 <= n)
					&& (((pc[1] & 0xc0) == 0x80)
						&& ((pc[2] & 0xc0) == 0x80)
						&& ((pc[3] & 0xc0) == 0x80)))
			{
				ret = 4;
			}
			else if(((c & 0xf0) == 0xe0) && (3 <= n)
					&& (((pc[1] & 0xc0) == 0x80)
					&& ((pc[2] & 0xc0) == 0x80)))
			{
				ret = 3;
			}
			else if(((c & 0xe0) == 0xc0) && (2 <= n)
					&& (((pc[1] & 0xc0) == 0x80)))
			{
				ret = 2;
			}
			else
			{
				assert(false);
				ret = 0;
			}
		}

		return ret;
	}

	template<typename Char> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		charlen(Char c, size_type n = sizeof(Char)) // n is byte_size
	{
		return this_type::charlen(this_type::get_const_char_address(&c), n);
	}

	template<typename Char> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		charlen(const Char* pc, size_type n = size_type(-1)) // n is byte_size
	{
		assert(pc);
		return this_type::charlen(this_type::get_const_char_address(pc), n);
	}

	template<typename InputIter> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< boost::is_pointer< InputIter > >
			>,
			size_type
		>::type
		charlen(InputIter start, size_type n = size_type(-1)) // n is byte_size
	{
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		return this_type::charlen(this_type::get_const_char_address(start), n);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					boost::mpl::or_
					<
						is_iterator< Basic_String<Char, Traits, Alloc> >,
						is_utf8_char_t< Basic_String<Char, Traits, Alloc> >
					>
				>
			>,
			size_type
		>::type
		charlen(const Basic_String<Char, Traits, Alloc>& c)
	{
		assert(this_type::strlen(c) == 1);

		size_type byte_size = c.size() * sizeof(Char);

		return
			((size_type(E_char_max_size) < byte_size) || (c.empty()))?
				0 : this_type::charlen(this_type::get_const_char_address(c), byte_size);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_utf8_char_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		charlen(const Basic_String<Char, Traits, Alloc>& c)
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(Char) == 1>));

		assert(c.size());
		return c.size();
	}

public:
	// charcmp
	// only support
	inline static s32 charcmp(const char* l, const char* r)
	{
		assert(l && r && this_type::charlen(l) && this_type::charlen(r));
		if(l == r)
		{
			return 0;
		}
		else
		{
			size_type llen = this_type::charlen(l);
			size_type rlen = this_type::charlen(r);
			return llen == rlen? math::sign(std::memcmp(l, r, llen)) : (llen < rlen? -1 : 1);
		}
	}

	template<typename L, typename R> inline
	static s32 charcmp(const L& l, const R& r)
	{
		return
			this_type::charcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r));
	}

public:
	// charcmp_eq
	inline static bool charcmp_eq(const char* l, const char* r)
	{
		assert(l && r && this_type::charlen(l) && this_type::charlen(r));
		if(l == r)
		{
			return true;
		}
		else
		{
			size_type llen = this_type::charlen(l);
			return (llen == this_type::charlen(r)) && (0 == std::memcmp(l, r, llen));
		}
	}

	template<typename L, typename R> inline
	static bool charcmp_eq(const L& l, const R& r)
	{
		return this_type::charcmp_eq(this_type::get_const_char_address(l),
										this_type::get_const_char_address(r));
	}

public:
	inline static const char* memchr(const char* buf, const char* chr,
										size_type cnt = size_type(-1)) // cnt is utf8_length
	{
		assert(is_utf8_string(buf));
		assert(charlen(chr));
		cnt = (cnt == size_type(-1)? this_type::strlen(buf) : cnt);
		for(;cnt && !this_type::charcmp_eq(buf, chr);buf += charlen(buf), --cnt);
		return cnt? buf : 0;
	}

	template<typename Buf, typename Char> inline
	static const char* memchr(const Buf& buf, const Char& chr, size_type cnt = size_type(-1)) // cnt is utf8_length
	{
		cnt = (cnt == size_type(-1)? this_type::strlen(buf) : cnt);
		return this_type::memchr(this_type::get_const_char_address(buf),
									this_type::get_const_char_address(chr), cnt);
	}

	//-------------------utf8_string----------------------

public:
	// strlen
	// if there is a non-UTF8 character or N is less than a UTF8 character will return 0
	static size_type strlen(const char* str, size_type n = size_type(-1)); // n is byte_size
	// first utf8_size, second char_size
	static std::pair<size_type, size_type> strlen_both(const char* str, size_type n = size_type(-1)); // n is byte_size

	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		strlen(const Char* str, size_type n = size_type(-1)) // n is byte_size
	{
		return this_type::strlen(this_type::get_const_char_address(str), n);
	}

	template<typename InputIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			size_type
		>::type
		strlen(InputIter s, size_type n = size_type(-1)) // n is byte_size
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		return strlen(this_type::get_const_char_address(s), n);
	}

	template<typename InputIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			size_type
		>::type
		strlen(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		size_type size = std::distance(s, e);
		return size? this_type::strlen(this_type::get_const_char_address(s), size * sizeof(val_type)) : 0;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_t< Basic_String<Char, Traits, Alloc> >
				>
			>,
			size_type
		>::type
		strlen(const Basic_String<Char, Traits, Alloc>& str)
	{
		return str.empty()? 0 : this_type::strlen(this_type::get_const_char_address(str), str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		strlen(const Basic_String<Char, Traits, Alloc>& str)
	{
		return str.length();
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_view_t< BasicStringView<Char, Traits> >
				>
			>,
			size_type
		>::type
		strlen(const BasicStringView<Char, Traits>& str)
	{
		return str.empty()? 0 : this_type::strlen(this_type::get_const_char_address(str), str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< BasicStringView<Char, Traits> >,
			size_type
		>::type
		strlen(const BasicStringView<Char, Traits>& str)
	{
		return str.length();
	}

public:
	// strnlen
	// if there is a non-UTF8 character or N is less than a UTF8 character
	// will return the number of previously counted characters
	static size_type strnlen(const char* str, size_type n = size_type(-1)); // the n is byte_size

	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		strnlen(const Char* str, size_type n = size_type(-1)) // n is byte_size
	{
		return this_type::strnlen(this_type::get_const_char_address(str), n);
	}

	template<typename InputIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			size_type
		>::type
		strnlen(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		size_type size = std::distance(s, e);
		return size? strnlen(this_type::get_const_char_address(s), size * sizeof(val_type)) : 0;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_t< Basic_String<Char, Traits, Alloc> >
				>
			>,
			size_type
		>::type
		strnlen(const Basic_String<Char, Traits, Alloc>& str)
	{
		return str.empty()?
				0 : this_type::strnlen(this_type::get_const_char_address(str),
										str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		strnlen(const Basic_String<Char, Traits, Alloc>& str)
	{
		return str.length();
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_view_t< BasicStringView<Char, Traits> >
				>
			>,
			size_type
		>::type
		strnlen(const BasicStringView<Char, Traits>& str)
	{
		return str.empty()?
				0 : this_type::strnlen(this_type::get_const_char_address(str),
										str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< BasicStringView<Char, Traits> >,
			size_type
		>::type
		strnlen(const BasicStringView<Char, Traits>& str)
	{
		return str.length();
	}

	// strnlen_both
	// first utf8_size, second char_size
	static std::pair<size_type, size_type> strnlen_both(const char* str, size_type n = size_type(-1)); // the n is byte_size

	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			std::pair<size_type, size_type>
		>::type
		strnlen_both(const Char* str, size_type n = size_type(-1)) // n is byte_size
	{
		return this_type::strnlen_both(this_type::get_const_char_address(str), n);
	}

	template<typename InputIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			std::pair<size_type, size_type>
		>::type
		strnlen_both(InputIter s, InputIter e)
	{
		typedef std::pair<size_type, size_type> ret_type;
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		size_type size = std::distance(s, e);
		return size? strnlen_both(this_type::get_const_char_address(s), size * sizeof(val_type)) : ret_type(0, 0);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_t< Basic_String<Char, Traits, Alloc> >
				>
			>,
			std::pair<size_type, size_type>
		>::type
		strnlen_both(const Basic_String<Char, Traits, Alloc>& str)
	{
		typedef std::pair<size_type, size_type> ret_type;

		return str.empty()?
				ret_type(0, 0)
				: this_type::strnlen_both(
					this_type::get_const_char_address(str), str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_t< Basic_String<Char, Traits, Alloc> >,
			std::pair<size_type, size_type>
		>::type
		strnlen_both(const Basic_String<Char, Traits, Alloc>& str)
	{
		typedef std::pair<size_type, size_type> ret_type;

		return ret_type(str.length(), str.size());
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_view_t< BasicStringView<Char, Traits> >
				>
			>,
			std::pair<size_type, size_type>
		>::type
		strnlen_both(const BasicStringView<Char, Traits>& str)
	{
		typedef std::pair<size_type, size_type> ret_type;

		return str.empty()?
				ret_type(0, 0)
				: this_type::strnlen_both(
					this_type::get_const_char_address(str), str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_view_t< BasicStringView<Char, Traits> >,
			std::pair<size_type, size_type>
		>::type
		strnlen_both(const BasicStringView<Char, Traits>& str)
	{
		typedef std::pair<size_type, size_type> ret_type;

		return ret_type(str.length(), str.size());
	}

public:
	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!! is_utf8_string can only determine whether certain character sets are UTF8 character sets,		!!!
	!!!	because many character sets each character component & 0x80 == 0 matches UTF8 character range,	!!!
	!!!	so this functions can only be used in a some place, or only used in ASSERT						!!!
	!!! don't using it check a string is utf8 string													!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/
	// is_utf8_string
	inline static bool is_utf8_string(const char* str, size_type n = size_type(-1)) // n is byte_size
	{
		return n == 0? false : (n == static_cast<size_type>(-1) && 0 == str[0]? true : this_type::strlen(str, n) != 0);
	}

	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		is_utf8_string(const Char* str, size_type n = size_type(-1)) // n is byte_size
	{
		assert(str);
		return
			n == 0?
				false : (n == -1 && 0 == this_type::get_const_char_address(str)[0])?
					true : 0 != this_type::strnlen(this_type::get_const_char_address(str), n);
	}

	template<typename InputIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			bool
		>::type
		is_utf8_string(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		size_type size = std::distance(s, e);
		return 0 == size? false : 0 != strlen(this_type::get_const_char_address(s), size * sizeof(val_type));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_t< Basic_String<Char, Traits, Alloc> >
				>
			>,
			bool
		>::type
		is_utf8_string(const Basic_String<Char, Traits, Alloc>& str)
	{
		return
			str.empty()?
				true
				: 0 != this_type::strlen(
							this_type::get_const_char_address(str),
							str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_t< Basic_String<Char, Traits, Alloc> >,
			bool
		>::type
		is_utf8_string(const Basic_String<Char, Traits, Alloc>& str)
	{
		return true;
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_native_char<Char>,
				boost::mpl::not_
				<
					is_utf8_string_t< BasicStringView<Char, Traits> >
				>
			>,
			bool
		>::type
		is_utf8_string(const BasicStringView<Char, Traits>& str)
	{
		return
			str.empty()?
				true
				: 0 != this_type::strlen(
							this_type::get_const_char_address(str),
							str.size() * sizeof(Char));
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::enable_if
		<
			is_utf8_string_t< BasicStringView<Char, Traits> >,
			bool
		>::type
		is_utf8_string(const BasicStringView<Char, Traits>& str)
	{
		return true;
	}

public:
	// utf8_len_to_byte_size
	static size_type utf8_length_to_byte_size(const char* str, size_type utf8_len = size_type(-1));

	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		utf8_length_to_byte_size(const Char* str, size_type utf8_len = size_type(-1)) // n is byte_size
	{
		return this_type::utf8_length_to_byte_size(this_type::get_const_char_address(str), utf8_len);
	}

public:
	// strcmp

	YGGR_CXX17_CONSTEXPR_OR_INLINE
	static s32 strcmp(const char* l, const char* r,
						size_type ln = size_type(-1), size_type rn = size_type(-1)) // "ln", "rn" is byte_size
	{
		if(ln == 0 && rn == 0)
		{
			return 0;
		}

		assert(l && r);
		s32 rst = 0;

		return
			(ln == rn)?
				math::sign(ln == static_cast<size_type>(-1)? std::strcmp(l, r) : memcmp(l, r, ln))
				: (ln = (ln == size_type(-1)? std::strlen(l) : ln),
					rn = (rn == size_type(-1)? std::strlen(r) : rn),
					rst = math::sign(memcmp(l, r, (std::min)(ln, rn))),
					(rst == 0? ln == rn? 0 : ln < rn? -1 : 1 : rst) );
	}

	template<typename InputIter1, typename InputIter2> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter1>,
				is_iterator<InputIter2>
			>,
			s32
		>::type
		strcmp(InputIter1 l, InputIter2 r,
				size_type ln = size_type(-1), size_type rn = size_type(-1)) // "ln", "rn" is byte_size
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				ln, rn);
	}

	template<typename CharL, typename TraitsL, typename AllocL, typename CharR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<CharR>,
			s32
		>::type
		strcmp(const Basic_String<CharL, TraitsL, AllocL>& l, const CharR* r, size_type rn = size_type(-1)) // rn is byte_size
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), rn);
	}

	template<typename CharL, typename CharR, typename TraitsR, typename AllocR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<CharL>,
			s32
		>::type
		strcmp(const CharL* l, const Basic_String<CharR, TraitsR, AllocR>& r, size_type ln = size_type(-1)) // ln is byte_size
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				ln, r.size());
	}

	template<typename CharL, typename TraitsL, typename AllocL,
				typename CharR, typename TraitsR, typename AllocR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringL,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringR> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static s32 strcmp(const Basic_StringL<CharL, TraitsL, AllocL>& l,
						const Basic_StringR<CharR, TraitsR, AllocR>& r)
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	template<typename CharL, typename TraitsL, typename CharR,
				template<typename _Char, typename _Traits> class BasicStringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_convertible_char<CharR>,
				is_string_view_t< BasicStringView<CharL, TraitsL> >
			>,
			s32
		>::type
		strcmp(const BasicStringView<CharL, TraitsL>& l, const CharR* r, size_type rn = size_type(-1)) // rn is byte_size
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), rn);
	}

	template<typename CharL, typename CharR, typename TraitsR,
				template<typename _Char, typename _Traits> class BasicStringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_convertible_char<CharL>,
				is_string_view_t< BasicStringView<CharR, TraitsR> >
			>,
			s32
		>::type
		strcmp(const CharL* l, const BasicStringView<CharR, TraitsR>& r, size_type ln = size_type(-1)) // ln is byte_size
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				ln, r.size());
	}

	template<typename CharL, typename TraitsL, typename AllocL,
				typename CharR, typename TraitsR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringL,
				template<typename _Char, typename _Traits> class BasicStringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static s32 strcmp(const Basic_StringL<CharL, TraitsL, AllocL>& l,
						const BasicStringView<CharR, TraitsR>& r)
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	template<typename CharL, typename TraitsL,
				typename CharR, typename TraitsR, typename AllocR,
				template<typename _Char, typename _Traits> class BasicStringView,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringR> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static s32 strcmp(const BasicStringView<CharL, TraitsL>& l,
						const Basic_StringR<CharR, TraitsR, AllocR>& r)
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	template<typename CharL, typename TraitsL,
				typename CharR, typename TraitsR,
				template<typename _Char, typename _Traits> class BasicStringViewL,
				template<typename _Char, typename _Traits> class BasicStringViewR> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static s32 strcmp(const BasicStringViewL<CharL, TraitsL>& l,
						const BasicStringViewR<CharR, TraitsR>& r)
	{
		return
			this_type::strcmp(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	// strcmp_eq
	YGGR_CXX17_CONSTEXPR_OR_INLINE
	static bool strcmp_eq(const char* l, const char* r,
							size_type ln = size_type(-1), size_type rn = size_type(-1)) // "ln", "rn" is byte_size
	{
		if(ln == 0 && rn == 0)
		{
			return true;
		}

		assert(l && r);

		ln = (ln == size_type(-1)? std::strlen(l) : ln);
		rn = (rn == size_type(-1)? std::strlen(r) : rn);

		return (ln == rn) && ((l == r) || (0 == memcmp(l, r, (std::min)(ln, rn))));
	}

	template<typename InputIter1, typename InputIter2> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter1>,
				is_iterator<InputIter2>
			>,
			bool
		>::type
		strcmp_eq(InputIter1 l, InputIter2 r,
					size_type ln = size_type(-1), size_type rn = size_type(-1)) // "ln", "rn" is byte_size
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				ln, rn);
	}

	template<typename CharL, typename TraitsL, typename AllocL, typename CharR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<CharR>,
			bool
		>::type
		strcmp_eq(const Basic_String<CharL, TraitsL, AllocL>& l, const CharR* r, size_type rn = size_type(-1)) // rn is byte_size
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), rn);
	}

	template<typename CharL, typename CharR, typename TraitsR, typename AllocR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_convertible_char<CharL>,
			bool
		>::type
		strcmp_eq(const CharL* l, const Basic_String<CharR, TraitsR, AllocR>& r, size_type ln = size_type(-1)) // ln is byte_size
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				ln, r.size());
	}

	template<typename CharL, typename TraitsL, typename AllocL,
				typename CharR, typename TraitsR, typename AllocR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringL,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringR> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static bool strcmp_eq(const Basic_StringL<CharL, TraitsL, AllocL>& l,
							const Basic_StringR<CharR, TraitsR, AllocR>& r)
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	template<typename CharL, typename TraitsL, typename CharR,
				template<typename _Char, typename _Traits> class BasicStringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_convertible_char<CharR>,
				is_string_view_t< BasicStringView<CharL, TraitsL> >
			>,
			bool
		>::type
		strcmp_eq(const BasicStringView<CharL, TraitsL>& l, const CharR* r, size_type rn = size_type(-1)) // rn is byte_size
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), rn);
	}

	template<typename CharL, typename CharR, typename TraitsR,
				template<typename _Char, typename _Traits> class BasicStringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_convertible_char<CharL>,
				is_string_view_t< BasicStringView<CharR, TraitsR> >
			>,
			bool
		>::type
		strcmp_eq(const CharL* l, const BasicStringView<CharR, TraitsR>& r, size_type ln = size_type(-1)) // ln is byte_size
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				ln, r.size());
	}

	template<typename CharL, typename TraitsL, typename AllocL,
				typename CharR, typename TraitsR,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringL,
				template<typename _Char, typename _Traits> class BasicStringView> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static bool strcmp_eq(const Basic_StringL<CharL, TraitsL, AllocL>& l,
							const BasicStringView<CharR, TraitsR>& r)
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	template<typename CharL, typename TraitsL,
				typename CharR, typename TraitsR, typename AllocR,
				template<typename _Char, typename _Traits> class BasicStringView,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_StringR> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static bool strcmp_eq(const BasicStringView<CharL, TraitsL>& l,
							const Basic_StringR<CharR, TraitsR, AllocR>& r)
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

	template<typename CharL, typename TraitsL,
				typename CharR, typename TraitsR,
				template<typename _Char, typename _Traits> class BasicStringViewL,
				template<typename _Char, typename _Traits> class BasicStringViewR> YGGR_CXX17_CONSTEXPR_OR_INLINE
	static bool strcmp_eq(const BasicStringViewL<CharL, TraitsL>& l,
							const BasicStringViewR<CharR, TraitsR>& r)
	{
		return
			this_type::strcmp_eq(
				this_type::get_const_char_address(l),
				this_type::get_const_char_address(r),
				l.size(), r.size());
	}

public:
	// find
	// the find function param "cmp" is an utf8_char, not an utf8_string

	static size_type find(const char* i1, const char* i2, const char* cmp);

	template<typename InputIter, typename Cmp> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			size_type
		>::type
		find(InputIter i1, InputIter i2, const Cmp& cmp)
	{
		typedef typename boost::iterator_value<InputIter>::type cv_val_type;
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_val_type) val_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		const char* s = this_type::get_const_char_address(i1);
		const char* e = s + this_type::byte_distance(i1, i2);

		return this_type::find(s, e, this_type::get_const_char_address(cmp));
	}

	template<typename Char, typename Traits, typename Alloc, typename Cmp,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static size_type find(const Basic_String<Char, Traits, Alloc>& str, const Cmp& cmp)
	{
		const char* s = this_type::get_const_char_address(str);
		const char* e = s + str.size() * sizeof(Char);

		return this_type::find(s, e, this_type::get_const_char_address(cmp));
	}

	template<typename Char, typename Traits, typename Cmp,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static size_type find(const BasicStringView<Char, Traits>& str, const Cmp& cmp)
	{
		const char* s = this_type::get_const_char_address(str);
		const char* e = s + str.size() * sizeof(Char);

		return this_type::find(s, e, this_type::get_const_char_address(cmp));
	}

	template<typename Size>
	static
	typename
		boost::enable_if
		<
			is_convertible_size_t<Size>,
			size_type
		>::type
		find(const char* src, Size nsrc_size, const char* cmp) // src_size is byte_size
	{
		size_type src_size = static_cast<size_type>(nsrc_size);

		if(src_size != size_type(-1))
		{
			return this_type::find(src, src + src_size, cmp);
		}
		else
		{
			if(!src_size)
			{
				return size_type(-1);
			}
			else
			{
				assert(this_type::is_utf8_string(src, src_size));
				size_type pos = 0;
				for(; *src; src += this_type::charlen(src), ++pos)
				{
					if(this_type::charcmp_eq(src, cmp))
					{
						return pos;
					}
				}

				return size_type(-1);
			}
		}
	}

	template<typename InputIter, typename Size, typename Cmp> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				is_convertible_size_t<Size>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			size_type
		>::type
		find(InputIter src, Size src_size, const Cmp& cmp) // src_size is utf8_length
	{
		//typedef typename boost::iterator_value<InputIter>::type cv_val_type;
		//typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(cv_val_type) val_type;
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		return
			this_type::find(
				this_type::get_const_char_address(src),
				src_size,
				this_type::get_const_char_address(cmp));
	}

public:
	//search
	static size_type search(const char* i1, const char* i2,
								const char* cmp_i1, const char* cmp_i2);

	template<typename InputIter1, typename InputIter2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator< InputIter1 >,
				boost::mpl::not_< is_reverse_iterator<InputIter1> >,
				is_iterator< InputIter2 >,
				boost::mpl::not_< is_reverse_iterator<InputIter2> >
			>,
			size_type
		>::type
		search(InputIter1 i1, InputIter1 i2,
				InputIter2 cmp_i1, InputIter2 cmp_i2)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter1>));
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter2>));

		//typedef typename boost::iterator_value<InputIter1>::value_type val1_type;
		//typedef typename boost::iterator_value<InputIter2>::value_type val2_type;

		const char* s1 = this_type::get_const_char_address(i1);
		const char* e1 = s1 + this_type::byte_distance(i1, i2);

		const char* s2 = this_type::get_const_char_address(cmp_i1);
		const char* e2 = s2 + this_type::byte_distance(cmp_i1, cmp_i2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Char, typename Traits, typename Alloc, typename InputIter2,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_iterator< Basic_String<Char, Traits, Alloc> > >,
				is_iterator<InputIter2>,
				boost::mpl::not_< is_reverse_iterator<InputIter2> >
			>,
			size_type
		>::type
		search(const Basic_String<Char, Traits, Alloc>& src,
				InputIter2 cmp_i1, InputIter2 cmp_i2)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter2>));

		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char);

		const char* s2 = this_type::get_const_char_address(cmp_i1);
		const char* e2 = s2 + this_type::byte_distance(cmp_i1, cmp_i2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename InputIter1, typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_iterator< Basic_String<Char, Traits, Alloc> > >,
				is_iterator<InputIter1>,
				boost::mpl::not_< is_reverse_iterator<InputIter1> >
			>,
			size_type
		>::type
		search(InputIter1 i1, InputIter1 i2,
				const Basic_String<Char, Traits, Alloc>& cmp)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter1>));

		const char* s1 = this_type::get_const_char_address(i1);
		const char* e1 = s1 + this_type::byte_distance(i1, i2);

		const char* s2 = cmp.data();
		const char* e2 = s2 + cmp.size() * sizeof(Char);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Char1, typename Traits1, typename Alloc1,
				typename Char2, typename Traits2, typename Alloc2,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String1,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String2 > inline
	static
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				is_iterator< Basic_String1<Char1, Traits1, Alloc1> >,
				is_iterator< Basic_String2<Char2, Traits2, Alloc2> >
			>,
			size_type
		>::type
		search(const Basic_String1<Char1, Traits1, Alloc1>& src,
				const Basic_String2<Char2, Traits2, Alloc2>& cmp)
	{
		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char1);

		const char* s2 = this_type::get_const_char_address(cmp);
		const char* e2 = s2 + cmp.size() * sizeof(Char2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Char, typename Traits, typename InputIter2,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_iterator< BasicStringView<Char, Traits> > >,
				is_iterator<InputIter2>,
				boost::mpl::not_< is_reverse_iterator<InputIter2> >
			>,
			size_type
		>::type
		search(const BasicStringView<Char, Traits>& src,
				InputIter2 cmp_i1, InputIter2 cmp_i2)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter2>));

		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char);

		const char* s2 = this_type::get_const_char_address(cmp_i1);
		const char* e2 = s2 + this_type::byte_distance(cmp_i1, cmp_i2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename InputIter1, typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_iterator< BasicStringView<Char, Traits> > >,
				is_iterator<InputIter1>,
				boost::mpl::not_< is_reverse_iterator<InputIter1> >
			>,
			size_type
		>::type
		search(InputIter1 i1, InputIter1 i2,
				const BasicStringView<Char, Traits>& cmp)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter1>));

		const char* s1 = this_type::get_const_char_address(i1);
		const char* e1 = s1 + this_type::byte_distance(i1, i2);

		const char* s2 = cmp.data();
		const char* e2 = s2 + cmp.size() * sizeof(Char);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Char1, typename Traits1,
				typename Char2, typename Traits2, typename Alloc2,
				template<typename _Char, typename _Traits> class BasicStringView,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String2 > inline
	static
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				is_iterator< BasicStringView<Char1, Traits1> >,
				is_iterator< Basic_String2<Char2, Traits2, Alloc2> >
			>,
			size_type
		>::type
		search(const BasicStringView<Char1, Traits1>& src,
				const Basic_String2<Char2, Traits2, Alloc2>& cmp)
	{
		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char1);

		const char* s2 = this_type::get_const_char_address(cmp);
		const char* e2 = s2 + cmp.size() * sizeof(Char2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Char1, typename Traits1, typename Alloc1,
				typename Char2, typename Traits2,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String1,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				is_iterator< Basic_String1<Char1, Traits1, Alloc1> >,
				is_iterator< BasicStringView<Char2, Traits2> >
			>,
			size_type
		>::type
		search(const Basic_String1<Char1, Traits1, Alloc1>& src,
				const BasicStringView<Char2, Traits2>& cmp)
	{
		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char1);

		const char* s2 = this_type::get_const_char_address(cmp);
		const char* e2 = s2 + cmp.size() * sizeof(Char2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Char1, typename Traits1,
				typename Char2, typename Traits2,
				template<typename _Char, typename _Traits> class BasicStringView1,
				template<typename _Char, typename _Traits> class BasicStringView2 > inline
	static
	typename
		boost::disable_if
		<
			boost::mpl::or_
			<
				is_iterator< BasicStringView1<Char1, Traits1> >,
				is_iterator< BasicStringView2<Char2, Traits2> >
			>,
			size_type
		>::type
		search(const BasicStringView1<Char1, Traits1>& src,
				const BasicStringView2<Char2, Traits2>& cmp)
	{
		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char1);

		const char* s2 = this_type::get_const_char_address(cmp);
		const char* e2 = s2 + cmp.size() * sizeof(Char2);

		return this_type::search(s1, e1, s2, e2);
	}

	template<typename Size1, typename Size2>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_convertible_size_t<Size1>,
				is_convertible_size_t<Size2>
			>,
			size_type
		>::type
		search(const char* src, Size1 nsrc_size,
				const char* cmp, Size2 ncmp_size) // nsrc_size, ncmp_size is byte_size
	{
		assert(src && cmp);

		size_type src_size = static_cast<size_type>(nsrc_size);
		size_type cmp_size = static_cast<size_type>(ncmp_size);

		if(src_size == -1)
		{
			if(cmp_size == -1)
			{
				size_type pos = 0;
				const char* tmp_i1 = 0;
				//const char* tmp_cmp_i1 = 0;

				for(; ; src += this_type::charlen(src), ++pos)
				{
					tmp_i1 = src;
					for(const char* tmp_cmp_i1 = cmp; ;
							tmp_i1 += this_type::charlen(tmp_i1),
							tmp_cmp_i1 += this_type::charlen(tmp_cmp_i1))
					{
						if(0 == *tmp_cmp_i1) return pos;
						if(0 == *tmp_i1) return size_type(-1);
						if(!this_type::charcmp_eq(tmp_cmp_i1, tmp_i1)) break;
					}
				}

				return size_type(-1);
			}
			else if(0 == cmp_size)
			{
				return 0;
			}
			else
			{
				size_type pos = 0;
				size_type cmp_byte_pos = 0;
				const char* tmp_i1 = 0;
				//const char* tmp_cmp_i1 = 0;
				for(; ; src += this_type::charlen(src), ++pos)
				{
					tmp_i1 = src;
					cmp_byte_pos = 0;
					for(const char* tmp_cmp_i1 = cmp; ;
							tmp_i1 += this_type::charlen(tmp_i1),
							cmp_byte_pos += this_type::charlen(tmp_cmp_i1),
							tmp_cmp_i1 += this_type::charlen(tmp_cmp_i1))
					{
						if(cmp_byte_pos == cmp_size) return pos;
						if(0 == *tmp_i1) return size_type(-1);
						if(!this_type::charcmp_eq(tmp_cmp_i1, tmp_i1)) break;
					}
				}

				return size_type(-1);

			}
		}
		else if(!src_size)
		{
			return size_type(-1);
		}
		else
		{
			if(cmp_size == -1)
			{
				size_type pos = 0;

				const char* tmp_i1 = 0;
				//const char* tmp_cmp_i1 = 0;

				for(size_type byte_pos = 0; ;
						byte_pos += this_type::charlen(src), src += this_type::charlen(src), ++pos)
				{
					tmp_i1 = src;
					for(const char* tmp_cmp_i1 = cmp; ;
							tmp_i1 += this_type::charlen(tmp_i1),
							tmp_cmp_i1 += this_type::charlen(tmp_cmp_i1))
					{
						assert(byte_pos <= src_size);
						if(0 == *tmp_cmp_i1) return pos;
						if(byte_pos == src_size) return size_type(-1);
						if(!this_type::charcmp_eq(tmp_cmp_i1, tmp_i1)) break;
					}
				}

				return size_type(-1);
			}
			else
			{
				return this_type::search(src, src + src_size, cmp, cmp + cmp_size);
			}
		}
	}

	template<typename InputIter1, typename Size1, typename InputIter2, typename Size2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter1>,
				is_convertible_size_t<Size1>,
				is_iterator<InputIter2>,
				is_convertible_size_t<Size2>
			>,
			size_type
		>::type
		search(InputIter1 src, Size1 nsrc_size,
				InputIter2 cmp, Size2 ncmp_size) // nsrc_size, ncmp_size is byte_size
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter1>));
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter2>));

		return this_type::search(this_type::get_const_char_address(src), nsrc_size,
									this_type::get_const_char_address(cmp), ncmp_size);
	}

public:
	//search_n
	static size_type search_n(const char* i1, const char* i2, size_type count, const char* cmp);

	template<typename InputIter, typename Cmp> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >
			>,
			size_type
		>::type
		search_n(InputIter i1, InputIter i2, size_type count, const Cmp& cmp)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		const char* s1 = this_type::get_const_char_address(i1);
		const char* e1 = s1 + this_type::byte_distance(i1, i2);

		return this_type::search_n(s1, e1, count,
									this_type::get_const_char_address(cmp));
	}

	template<typename Char, typename Traits, typename Alloc, typename Cmp,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static size_type search_n(const Basic_String<Char, Traits, Alloc>& src,
								size_type count, const Cmp& cmp)
	{
		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char);

		return this_type::search_n(s1, e1, count,
									this_type::get_const_char_address(cmp));
	}

	template<typename Char, typename Traits, typename Cmp,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static size_type search_n(const BasicStringView<Char, Traits>& src,
								size_type count, const Cmp& cmp)
	{
		const char* s1 = this_type::get_const_char_address(src);
		const char* e1 = s1 + src.size() * sizeof(Char);

		return this_type::search_n(s1, e1, count,
									this_type::get_const_char_address(cmp));
	}

	// src_size is byte_size, count is utf8_length
	// cmp is an utf8_char not an utf8_string
	template<typename Size>
	static
	typename
		boost::enable_if
		<
			is_convertible_size_t<Size>,
			size_type
		>::type
		search_n(const char* src, Size nsrc_size,
					size_type count, const char* cmp) // nsrc_size is byte_size, count is utf8_lengt
	{
		assert(src && cmp);

		size_type src_size = static_cast<size_type>(nsrc_size);
		if(src_size == size_type(-1))
		{
			if(!count)
			{
				return 0;
			}
			else
			{
				size_type i = 0;
				size_type ret_index = 0;
				size_type cmp_step = this_type::charlen(cmp);

				while(*src && i != count)
				{
					if(this_type::charcmp_eq(src, cmp))
					{
						src += cmp_step;
						++i;
					}
					else
					{
						ret_index += (i + 1);
						i = 0;
						src += this_type::charlen(src);
					}
				}

				return i == count? ret_index : size_type(-1);
			}
		}
		else
		{
			return this_type::search_n(src, src + src_size, count, cmp);
		}
	}

	template<typename InputIter, typename Size, typename Cmp> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_< is_reverse_iterator<InputIter> >,
				is_convertible_size_t<Size>
			>,
			size_type
		>::type
		search_n(InputIter src, Size src_size, size_type count, const Cmp& cmp)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<InputIter>));

		return this_type::search_n(this_type::get_const_char_address(src), src_size,
									count, this_type::get_const_char_address(cmp));
	}

public:
	// get_charset_name
	inline static const char* get_charset_name(const char* str)
	{
		return
			this_type::is_utf8_string(str)?
				YGGR_INNER_STRING_CHARSET_NAME()
				: default_charset_config::get_default_string_charset_name<char>();
	}

	inline static const string& get_str_charset_name(const char* str)
	{
		return
			this_type::is_utf8_string(str)?
				YGGR_STR_INNER_STRING_CHARSET_NAME()
				: default_charset_config::get_str_default_string_charset_name<char>();
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static typename boost::enable_if<is_native_char<Char>, const char*>::type
		get_charset_name(const Basic_String<Char, Traits, Alloc>& str)
	{
		return this_type::is_utf8_string(str.data(), str.size())?
				YGGR_INNER_STRING_CHARSET_NAME()
				: default_charset_config::get_default_string_charset_name< Char >();
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static typename boost::enable_if<is_native_char<Char>, const string&>::type
		get_str_charset_name(const Basic_String<Char, Traits, Alloc>& str)
	{
		return this_type::is_utf8_string(str.data(), str.size())?
				YGGR_STR_INNER_STRING_CHARSET_NAME()
				: default_charset_config::get_str_default_string_charset_name< Char >();
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static typename boost::enable_if<is_native_char<Char>, const char*>::type
		get_charset_name(const BasicStringView<Char, Traits>& str)
	{
		return this_type::is_utf8_string(str.data(), str.size())?
				YGGR_INNER_STRING_CHARSET_NAME()
				: default_charset_config::get_default_string_charset_name< Char >();
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView > inline
	static typename boost::enable_if<is_native_char<Char>, const string&>::type
		get_str_charset_name(const BasicStringView<Char, Traits>& str)
	{
		return this_type::is_utf8_string(str.data(), str.size())?
				YGGR_STR_INNER_STRING_CHARSET_NAME()
				: default_charset_config::get_str_default_string_charset_name< Char >();
	}

private:
	// get_const_char_address
	template<typename T> inline
	static
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<T>,
			const char*
		>::type
		get_const_char_address(const T& src)
	{
		return reinterpret_cast<const char*>(&src);
	}

	template<typename T> inline
	static
	typename
		boost::enable_if
		<
			is_convertible_char<T>,
			const char*
		>::type
		get_const_char_address(const T* src)
	{
		return reinterpret_cast<const char*>(src);
	}

	template<typename T> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<T>,
				boost::mpl::not_< boost::is_pointer<T> >,
				boost::mpl::not_< is_reverse_iterator<T> >
			>,
			const char*
		>::type
		get_const_char_address(const T& src)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<T>));
		return reinterpret_cast<const char*>(&(*src));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static
	typename
		boost::disable_if
		<
			is_iterator< Basic_String<Char, Traits, Alloc> >,
			const char*
		>::type
		get_const_char_address(const Basic_String<Char, Traits, Alloc>& src)
	{
		return reinterpret_cast<const char*>(src.data());
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static
	typename
		boost::enable_if
		<
			is_string_view_t< BasicStringView<Char, Traits> >,
			const char*
		>::type
		get_const_char_address(const BasicStringView<Char, Traits>& src)
	{
		return reinterpret_cast<const char*>(src.data());
	}

	// byte_distance
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_not_any_utf8_string_iterator_t<Iter>
			>,
			s_size_type
		>::type
		byte_distance(Iter s, Iter e)
	{
		BOOST_MPL_ASSERT((is_random_access_iterator<Iter>));
		typedef typename boost::iterator_value<Iter>::type val_type;
		return std::distance(s, e) * sizeof(val_type);
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_any_utf8_string_iterator_t<Iter>
			>,
			s_size_type
		>::type
		byte_distance(Iter s, Iter e)
	{
		return std::distance(s.org(), e.org());
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_UTF8_FOO_HPP__
