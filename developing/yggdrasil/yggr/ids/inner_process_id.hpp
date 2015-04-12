//inner_process_id.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_MODULES_INNER_PROCESS_ID_HPP__
#define __YGGR_MODULES_INNER_PROCESS_ID_HPP__

#include <algorithm>
#include <iostream>
#include <memory>

#include <boost/atomic.hpp>
#include <boost/functional/hash/hash.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/ids/id_n.hpp>


namespace yggr
{
namespace ids
{

class inner_process_id
	: public id_n<sizeof(id64_type) + sizeof(id64_type)>
{
public:
	typedef id64_type main_id_type;
	typedef id64_type inner_id_type;
	typedef id_n<sizeof(main_id_type) + sizeof(inner_id_type)> base_type;
	typedef std::pair<main_id_type, inner_id_type> val_type;

private:
	typedef inner_process_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	inner_process_id(void);
	inner_process_id(const main_id_type& main_id, const inner_id_type& inner_id);
	inner_process_id(BOOST_RV_REF(this_type) right)
        : base_type(boost::forward<base_type>(right))
	{
	}

	inner_process_id(const this_type& right);
	virtual ~inner_process_id(void);

	val_type id_val(void) const;

	template<typename Other>
	void swap(Other& right)
	{
		base_type::swap(right);
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
	    base_type::operator=(boost::forward<base_type>(right));
        return *this;
	}

	this_type& operator=(const this_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		base_type::swap(right);
	}

	void swap(this_type& right);

	template<typename Other>
	inline bool operator==(const Other& right) const
	{
		return base_type::operator==(right);
	}

	inline bool operator==(const this_type& right) const
	{
		const base_type& l = *this, r = right;
		return l == r;
	}

	template<typename Other>
	inline bool operator!=(const Other& right) const
	{
		return base_type::operator!=(right);
	}

	inline bool operator!=(const this_type& right) const
	{
		const base_type& l = *this, r = right;
		return l != r;
	}

	template<typename Other>
	inline bool operator<(const Other& right) const
	{
		return base_type::operator<(right);
	}

	inline bool operator<(const this_type& right)
	{
		const base_type& l = *this, r = right;
		return l < r;
	}

	template<typename Other>
	inline bool operator<=(const Other& right) const
	{
		return base_type::operator<=(right);
	}

	inline bool operator<=(const this_type& right) const
	{
		const base_type& l = *this, r = right;
		return l <= r;
	}

	template<typename Other>
	inline bool operator>(const Other& right) const
	{
		return base_type::operator>(right);
	}

	inline bool operator>(const this_type& right) const
	{
		const base_type& l = *this, r = right;
		return l > r;
	}

	template<typename Other>
	inline bool operator >=(const Other& right) const
	{
		return base_type::operator>=(right);
	}

	inline bool operator>=(const this_type& right) const
	{
		const base_type& l = *this, r = right;
		return l >= r;
	}

	inline this_type ntoh(void) const
	{
		return *this;
	}

	inline this_type hton(void) const
	{
		return *this;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("inner_process_id_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

template<>
class id_generator<inner_process_id, void>
	: private nonable::noncopyable
{
public:
	typedef ids::inner_process_id id_type;
private:
	typedef id_type::main_id_type val_type;
	typedef id_type::inner_id_type address_type;

public:

	id_type operator()(void) const
	{
		typedef std::numeric_limits<u16> limits_type;

		static boost::atomic<u32> fuzz_incr((random::gen_random<u32>()) & 0xffff0000);

		u32 t_fuzz_incr = 0;
		u32 t_fuzz = 0;
		u32 t_incr = 0;
		u32 re_fuzz_incr = 0;

		do
		{
			t_fuzz_incr = fuzz_incr.load();
			t_fuzz = t_fuzz_incr & 0xffff0000;
			t_incr = t_fuzz_incr & 0x0000ffff;

			if(!(t_incr = (t_incr + 1) % limits_type::max()))
			{
				t_fuzz = (random::gen_random<u32>()) & 0xffff0000;
			}

			re_fuzz_incr = t_fuzz | t_incr;
		} while(!fuzz_incr.compare_exchange_strong(t_fuzz_incr, re_fuzz_incr));

		val_type use_fuzz_incr = static_cast<val_type>(t_fuzz_incr);
		val_type main_id = std::time(0);
		main_id <<= 32;
		main_id |= use_fuzz_incr;
		address_type inner_id = reinterpret_cast<address_type>(&fuzz_incr);

		return id_type(main_id, inner_id);
	}
};

} // namespace ids
} // namespace yggr

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

std::size_t hash_value(const yggr::ids::inner_process_id& id);

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP


template<typename Other>
inline bool operator==(const Other& left, const yggr::ids::inner_process_id& right)
{
	return right == left;
}

template<typename Other>
inline bool operator!=(const Other& left, const yggr::ids::inner_process_id& right)
{
    return right != left;
}

template<typename Other>
inline bool operator<(const Other& left, const yggr::ids::inner_process_id& right)
{
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
}

template<typename Other>
inline bool operator>(const Other& left, const yggr::ids::inner_process_id& right)
{
    return right < left;
}

template<typename Other>
inline bool operator<=(const Other& left, const yggr::ids::inner_process_id& right)
{
    return !(right < left);
}

template<typename Other>
inline bool operator>=(const Other& left, const yggr::ids::inner_process_id& right)
{
    return !(left < right);
}

#define _YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_DEF() \
	template<typename Other> \
	inline void swap(Other& left, yggr::ids::inner_process_id& right) { \
		right.swap(left); } \
	\
	template<typename Other> \
	inline void swap(yggr::ids::inner_process_id& left, Other& right) { \
		left.swap(right); } \
	\
	void swap(yggr::ids::inner_process_id& left, yggr::ids::inner_process_id& right); \
	\
	inline void swap(BOOST_RV_REF(yggr::ids::inner_process_id) left, yggr::ids::inner_process_id& right) { \
		right.swap(left); } \
	\
	inline void swap(yggr::ids::inner_process_id& left, BOOST_RV_REF(yggr::ids::inner_process_id) right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_DEF()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_DEF()
} // namespace boost

#undef _YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_DEF

//std::ostream& operator<<(std::ostream& os, const yggr::ids::inner_process_id& right);
template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
					const yggr::ids::inner_process_id& right)
{
	yggr::size_type idx = 0;

	os << std::hex;
	for(yggr::ids::inner_process_id::const_iterator i = right.begin(), isize = right.end(); i != isize; ++i, ++idx)
	{
		os << std::setw(2) << std::setfill(static_cast<Char>('0')) <<  static_cast<yggr::u16>(static_cast<yggr::u8>(*i));
		if(idx == 7)
		{
			os << "-";
		}
	}

	os << std::dec;
	return os;
}

#endif //__YGGR_MODULES_INNER_PROCESS_ID_HPP__
