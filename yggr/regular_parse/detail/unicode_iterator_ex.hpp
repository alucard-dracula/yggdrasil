//unicode_iterator_ex.hpp

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

#ifndef __YGGR_REGULAR_PARSE_DETAIL_UNICODE_ITERATOR_EX_HPP__
#define __YGGR_REGULAR_PARSE_DETAIL_UNICODE_ITERATOR_EX_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/regex/pending/unicode_iterator.hpp>

#include <iterator>
#include <stdexcept>

namespace yggr
{
namespace regular_parse
{
namespace detail
{

template<typename BaseIterator, typename U8Type = u8>
class u16_to_u8_iterator
	: public 
		boost::iterator_facade
		<
			u16_to_u8_iterator<BaseIterator, U8Type>, 
			U8Type, 
			std::bidirectional_iterator_tag, 
			const U8Type
		>
{
	typedef 
		boost::iterator_facade
		<
			u16_to_u8_iterator<BaseIterator, U8Type>, 
			U8Type, 
			std::bidirectional_iterator_tag, 
			const U8Type
		> base_type;
   
#if !defined(BOOST_NO_STD_ITERATOR_TRAITS)
	typedef typename std::iterator_traits<BaseIterator>::value_type base_value_type;

	BOOST_STATIC_ASSERT(sizeof(base_value_type) * CHAR_BIT == 16);
	BOOST_STATIC_ASSERT(sizeof(U8Type) * CHAR_BIT == 8);
#endif

public:
	inline typename base_type::reference
		dereference(void) const
	{
		if(m_current == 4)
		{
			extract_current();
		}
		return m_values[m_current];
	}

	inline bool equal(const u16_to_u8_iterator& that) const
	{
		if(m_position == that.m_position)
		{
			// either the m_current's must be equal, or one must be 0 and 
			// the other 4: which means neither must have bits 1 or 2 set:
			return (m_current == that.m_current) || (((m_current | that.m_current) & 3) == 0);
		}
		return false;
	}

	inline void increment(void)
	{
		// if we have a pending read then read now, so that we know whether
		// to skip a position, or move to a low-surrogate:
		if(m_current == 4)
		{
			// pending read:
			extract_current();
		}
		// move to the next surrogate position:
		++m_current;
		// if we've reached the end skip a position:
		if(m_values[m_current] == 0)
		{
			std::advance(m_position, (m_current > 3? 2 : 1));
			m_current = 4;
		}
	}

	void decrement(void)
	{
		if((m_current & 3) == 0)
		{
			--m_position;
			if(boost::detail::is_low_surrogate(static_cast<u16>(*m_position)))
			{
				--m_position;
				if(!boost::detail::is_high_surrogate(static_cast<u16>(*m_position)))
				{
					invalid_code_point(static_cast<u16>(*m_position));
				}
			}
			extract_current();
			m_current = 3;
			while(m_current && (m_values[m_current] == 0))
			{
				--m_current;
			}
		}
		else
		{
			--m_current;
		}
	}

	inline BaseIterator base(void) const
	{
		return m_position;
	}

	// construct:
	u16_to_u8_iterator(void) 
		: m_position(), m_current(0)
	{
		m_values[0] = 0;
		m_values[1] = 0;
		m_values[2] = 0;
		m_values[3] = 0;
		m_values[4] = 0;
	}

	u16_to_u8_iterator(BaseIterator b) 
		: m_position(b), m_current(4)
	{
		m_values[0] = 0;
		m_values[1] = 0;
		m_values[2] = 0;
		m_values[3] = 0;
		m_values[4] = 0;
	}

	u16_to_u8_iterator(BaseIterator b, BaseIterator start, BaseIterator end) 
		: m_position(b), m_current(4)
	{
		u16 val = 0;
		if(start != end)
		{
			if((b != start) && (b != end))
			{
				val = *b;
				if(boost::detail::is_surrogate(val) && ((val & 0xFC00u) == 0xDC00u))
				{
					invalid_code_point(val);
				}
			}
			val = *start;
			if(boost::detail::is_surrogate(val) && ((val & 0xFC00u) == 0xDC00u))
			{
				invalid_code_point(val);
			}
			val = *--end;
			if(boost::detail::is_high_surrogate(val))
			{
				invalid_code_point(val);
			}
		}
	}

private:
	static void invalid_code_point(u16 val)
	{
#ifndef BOOST_NO_STD_LOCALE
		std::stringstream ss;
		ss << "Misplaced UTF-16 surrogate U+" 
			<< std::showbase << std::hex << val 
			<< " encountered while trying to encode UTF-8 sequence";
		std::out_of_range e(ss.str());
#else
		std::out_of_range e("Misplaced UTF-16 surrogate encountered while trying to encode UTF-8 sequence");
#endif
		boost::throw_exception(e);
	}


	inline u32 cast_to_uint32(void) const
	{
		u32 val = 0;

		if(boost::detail::is_low_surrogate(static_cast<u16>(*m_position)))
		{
			invalid_code_point(static_cast<u16>(*m_position));
		}
		else
		{
			if(boost::detail::is_surrogate(static_cast<u16>(*m_position)))
			{
				BaseIterator next(m_position);
				++next;
				if(!boost::detail::is_low_surrogate(static_cast<u16>(*m_position)))
				{
					invalid_code_point(static_cast<u16>(*m_position));
				}
				else
				{
					val = static_cast<u32>(static_cast<u16>(*m_position));
					val = (val - boost::detail::high_surrogate_base) << 10;
					val |= (static_cast<u32>(static_cast<u16>(*next)) & boost::detail::ten_bit_mask);
				}
			}
			else
			{
				val = static_cast<u32>(static_cast<u16>(*m_position));
			}
		}

		return val;
	}

	inline void extract_current(void) const
	{
		u32 c = cast_to_uint32();

		if(c > 0x10FFFFu)
		{
			boost::detail::invalid_utf32_code_point(c);
		}

		if(c < 0x80u)
		{
			m_values[0] = static_cast<unsigned char>(c);
			m_values[1] = static_cast<unsigned char>(0u);
			m_values[2] = static_cast<unsigned char>(0u);
			m_values[3] = static_cast<unsigned char>(0u);
		}
		else if(c < 0x800u)
		{
			m_values[0] = static_cast<unsigned char>(0xC0u + (c >> 6));
			m_values[1] = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
			m_values[2] = static_cast<unsigned char>(0u);
			m_values[3] = static_cast<unsigned char>(0u);
		}
		else if(c < 0x10000u)
		{
			m_values[0] = static_cast<unsigned char>(0xE0u + (c >> 12));
			m_values[1] = static_cast<unsigned char>(0x80u + ((c >> 6) & 0x3Fu));
			m_values[2] = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
			m_values[3] = static_cast<unsigned char>(0u);
		}
		else
		{
			m_values[0] = static_cast<unsigned char>(0xF0u + (c >> 18));
			m_values[1] = static_cast<unsigned char>(0x80u + ((c >> 12) & 0x3Fu));
			m_values[2] = static_cast<unsigned char>(0x80u + ((c >> 6) & 0x3Fu));
			m_values[3] = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
		}
		m_current = 0;
	}

	BaseIterator m_position;
	mutable U8Type m_values[5];
	mutable unsigned m_current;
};

template <typename BaseIterator, typename U16Type = u16>
class u8_to_u16_iterator
	: public 
		boost::iterator_facade
		<
			u8_to_u16_iterator<BaseIterator, U16Type>, 
			U16Type, 
			std::bidirectional_iterator_tag, 
			const U16Type
		>
{
	typedef 
		boost::iterator_facade
		<
			u8_to_u16_iterator<BaseIterator, U16Type>, 
			U16Type, 
			std::bidirectional_iterator_tag, 
			const U16Type
		> base_type;

#if !defined(BOOST_NO_STD_ITERATOR_TRAITS)
	typedef typename std::iterator_traits<BaseIterator>::value_type base_value_type;

	BOOST_STATIC_ASSERT(sizeof(base_value_type) * CHAR_BIT == 8);
	BOOST_STATIC_ASSERT(sizeof(U16Type) * CHAR_BIT == 16);
#endif

public:
	inline typename base_type::reference
		dereference(void) const
	{
		if(m_current == 2)
		{
			extract_current();
		}
		return m_values[m_current];
	}

	inline bool equal(const u8_to_u16_iterator& that) const
	{
		return m_position == that.m_position;
	}

	inline void increment(void)
	{
		// if we have a pending read then read now, so that we know whether
		// to skip a position, or move to a low-surrogate:
		if(m_current == 2)
		{
			// pending read:
			extract_current();
		}
		// move to the next surrogate position:
		++m_current;
		// if we've reached the end skip a position:
		if(m_values[m_current] == 0)
		{
			m_current = 2;
			std::advance(m_position, boost::detail::utf8_byte_count(*m_position));
		}
	}

	void decrement(void)
	{
		// Keep backtracking until we don't have a trailing character:
		unsigned count = 0;
		while((*--m_position & 0xC0u) == 0x80u) ++count;
		// now check that the sequence was valid:
		if(count != boost::detail::utf8_trailing_byte_count(*m_position))
		{
			invalid_sequence();
		}
		m_current = 2;
	}

	inline BaseIterator base(void) const
	{
		return m_position;
	}

	// construct:
	u8_to_u16_iterator(void) 
		: m_position(), m_current(0)
	{
		m_values[0] = 0;
		m_values[1] = 0;
		m_values[2] = 0;
	}

	u8_to_u16_iterator(BaseIterator b) 
		: m_position(b), m_current(2)
	{
		m_values[0] = 0;
		m_values[1] = 0;
		m_values[2] = 0;
	}

	//
	// Checked constructor:
	//
	u8_to_u16_iterator(BaseIterator b, BaseIterator start, BaseIterator end) 
		: m_position(b), m_current(2)
	{
		if(start != end)
		{
			unsigned char v = *start;
			if((v & 0xC0u) == 0x80u)
			{
				invalid_sequence();
			}

			if((b != start) && (b != end) && ((*b & 0xC0u) == 0x80u))
			{
				invalid_sequence();
			}

			BaseIterator pos = end;
			do
			{
				v = *--pos;
			}
			while((start != pos) && ((v & 0xC0u) == 0x80u));

			std::ptrdiff_t extra = boost::detail::utf8_byte_count(v);
			if(std::distance(pos, end) < extra)
			{
				invalid_sequence();
			}
		}
	}
private:
	static void invalid_sequence(void)
	{
		std::out_of_range e("Invalid UTF-8 sequence encountered while trying to encode UTF-16 character");
		boost::throw_exception(e);
	}

	u32 cast_to_uint32(void) const
	{
		u32 val = static_cast<u32>(static_cast<boost::uint8_t>(*m_position));
		// we must not have a continuation character:
		if((val & 0xC0u) == 0x80u)
		{
			invalid_sequence();
		}

		// see how many extra bytes we have:
		unsigned extra = boost::detail::utf8_trailing_byte_count(*m_position);
		// extract the extra bits, 6 from each extra byte:
		BaseIterator next(m_position);
		for(unsigned c = 0; c < extra; ++c)
		{
			++next;
			val <<= 6;
			// We must have a continuation byte:
			if((static_cast<u8>(*next) & 0xC0) != 0x80)
			{
				invalid_sequence();
			}

			val += static_cast<u8>(*next) & 0x3Fu;
		}
		// we now need to remove a few of the leftmost bits, but how many depends
		// upon how many extra bytes we've extracted:
		static const u32 masks[4] = 
		{
			0x7Fu,
			0x7FFu,
			0xFFFFu,
			0x1FFFFFu,
		};

		val &= masks[extra];
		// check the result is in range:
		if(val > static_cast<u32>(0x10FFFFu))
		{
			invalid_sequence();
		}
		// The result must not be a surrogate:
		if((val >= static_cast<u32>(0xD800)) && (val <= static_cast<u32>(0xDFFF)))
		{
			invalid_sequence();
		}
		// We should not have had an invalidly encoded UTF8 sequence:
		if((extra > 0) && (val <= static_cast<u32>(masks[extra - 1])))
		{
			invalid_sequence();
		}

		return val;
	}

	inline void extract_current() const
	{
		// begin by checking for a code point out of range:
		u32 v = cast_to_uint32();
		if(v >= 0x10000u)
		{
			if(v > 0x10FFFFu)
			{
				boost::detail::invalid_utf32_code_point(v);
			}
			// split into two surrogates:
			m_values[0] = static_cast<U16Type>(v >> 10) + boost::detail::high_surrogate_base;
			m_values[1] = static_cast<U16Type>(v & boost::detail::ten_bit_mask) + boost::detail::low_surrogate_base;
			m_current = 0;
			BOOST_ASSERT(boost::detail::is_high_surrogate(m_values[0]));
			BOOST_ASSERT(boost::detail::is_low_surrogate(m_values[1]));
		}
		else
		{
			// 16-bit code point:
			m_values[0] = static_cast<U16Type>(v);
			m_values[1] = 0;
			m_current = 0;
			// value must not be a surrogate:
			if(boost::detail::is_surrogate(m_values[0]))
			{
				boost::detail::invalid_utf32_code_point(v);
			}
		}
	}

	BaseIterator m_position;
	mutable U16Type m_values[3];
	mutable unsigned m_current;
};


} // namespace detail

using detail::u16_to_u8_iterator;
using detail::u8_to_u16_iterator;

} // namespace regular_parse
} // namespace yggr

namespace yggr
{
namespace regular_parse
{

// extract_to_base_iterator_t
template<typename Iter>
struct extract_to_base_iterator_t
{
	typedef Iter type;
};

template<typename Iter>
struct extract_to_base_iterator_t< boost::utf8_output_iterator<Iter> >
{
	typedef Iter type;
};

template<typename Iter>
struct extract_to_base_iterator_t< boost::utf16_output_iterator<Iter> >
{
	typedef Iter type;
};

template<typename Iter, typename T>
struct extract_to_base_iterator_t< boost::u32_to_u16_iterator<Iter, T> >
{
	typedef Iter type;
};

template<typename Iter, typename T>
struct extract_to_base_iterator_t< boost::u16_to_u32_iterator<Iter, T> >
{
	typedef Iter type;
};

template<typename Iter, typename T>
struct extract_to_base_iterator_t< boost::u32_to_u8_iterator<Iter, T> >
{
	typedef Iter type;
};

template<typename Iter, typename T>
struct extract_to_base_iterator_t< boost::u8_to_u32_iterator<Iter, T> >
{
	typedef Iter type;
};

template<typename Iter, typename T>
struct extract_to_base_iterator_t< u16_to_u8_iterator<Iter, T> >
{
	typedef Iter type;
};

template<typename Iter, typename T>
struct extract_to_base_iterator_t< u8_to_u16_iterator<Iter, T> >
{
	typedef Iter type;
};

// extract_to_base_iterator
template <typename Iter> inline
const Iter& extract_to_base_iterator(const Iter& iter)
{
   return iter;
}

template<typename Iter> inline
Iter extract_to_base_iterator(const boost::utf8_output_iterator<Iter>& iter)
{
   return iter.base();
}

template<typename Iter> inline
Iter extract_to_base_iterator(const boost::utf16_output_iterator<Iter>& iter)
{
   return iter.base();
}

template<typename Iter, typename T> inline
Iter extract_to_base_iterator(const boost::u32_to_u16_iterator<Iter, T>& iter)
{
   return iter.base();
}

template<typename Iter, typename T> inline
Iter extract_to_base_iterator(const boost::u16_to_u32_iterator<Iter, T>& iter)
{
   return iter.base();
}

template<typename Iter, typename T> inline
Iter extract_to_base_iterator(const boost::u32_to_u8_iterator<Iter, T>& iter)
{
   return iter.base();
}

template<typename Iter, typename T> inline
Iter extract_to_base_iterator(const boost::u8_to_u32_iterator<Iter, T>& iter)
{
   return iter.base();
}

template<typename Iter, typename T> inline
Iter extract_to_base_iterator(const u16_to_u8_iterator<Iter, T>& iter)
{
   return iter.base();
}

template<typename Iter, typename T> inline
Iter extract_to_base_iterator(const u8_to_u16_iterator<Iter, T>& iter)
{
   return iter.base();
}

} // namespace regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_DETAIL_UNICODE_ITERATOR_EX_HPP__
