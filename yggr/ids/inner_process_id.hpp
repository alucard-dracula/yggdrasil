//inner_process_id.hpp

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

#ifndef __YGGR_IDS_INNER_PROCESS_ID_HPP__
#define __YGGR_IDS_INNER_PROCESS_ID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/ids/id_n.hpp>

#include <yggr/mplex/strict_sizeof.hpp>
#include <yggr/mplex/limits.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/math/sign.hpp>

#include <boost/atomic.hpp>
#include <boost/functional/hash/hash.hpp>

#include <algorithm>
#include <ostream>
#include <memory>
#include <cassert>

namespace yggr
{
namespace ids
{

class inner_process_id;

} // namespace ids
} // namespace yggr

namespace yggr
{
namespace ids
{
namespace detail
{

template<typename IDData, u64 Len, bool same_size = sizeof(IDData) == Len >
struct inner_process_id_id_data_cmp_helper;

template<typename IDData, u64 Len>
struct inner_process_id_id_data_cmp_helper<IDData, Len, true>
{
	typedef IDData id_data_type;
	typedef typename id_data_type::first_type main_id_type;
	typedef typename id_data_type::second_type inner_id_type;

	template<typename InnerProcessID> inline
	s32 operator()(const InnerProcessID& l, const id_data_type& r) const
	{
		return math::sign(memcmp(&(l[0]), &r.first, Len));
	}
};

template<typename IDData, u64 Len>
struct inner_process_id_id_data_cmp_helper<IDData, Len, false>
{
	typedef IDData id_data_type;
	typedef typename id_data_type::first_type main_id_type;
	typedef typename id_data_type::second_type inner_id_type;

	template<typename InnerProcessID> inline
	s32 operator()(const InnerProcessID& l, const id_data_type& r) const
	{
		u8 tmp[Len] = {0};
		memcpy(&(tmp[0]), &(r.first), sizeof(main_id_type));
		memcpy(&(tmp[sizeof(main_id_type)]), &(r.second), sizeof(inner_id_type));
		return math::sign(memcmp(&(l[0]), tmp, Len));
	}
};

} // namespace detail
} // namespace ids
} // namespace yggr

namespace yggr
{
namespace ids
{

//---------inner_process_id-------------

class inner_process_id
	: public id_n<sizeof(id64_type) + sizeof(id64_type)>
{
public:
	typedef id64_type main_id_type;
	typedef id64_type inner_id_type;
	typedef id_n<sizeof(main_id_type) + sizeof(inner_id_type)> base_type;
	typedef std::pair<main_id_type, inner_id_type> id_data_type;

private:
	typedef 
		detail::inner_process_id_id_data_cmp_helper
		<
			id_data_type, 
			base_type::E_length
		> id_data_cmp_helper_type;
	
private:
	typedef inner_process_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	inner_process_id(void);
	
	template<typename InputIter>
	inner_process_id(InputIter s, InputIter e)
		: base_type(s, e)
	{
	}

	inner_process_id(main_id_type main_id, inner_id_type inner_id);
	inner_process_id(const id_data_type& right);

	template<typename Other>
	explicit inner_process_id(const Other& right,
								typename boost::enable_if<container_ex::is_container<Other>, void*>::type chk = 0)
		: base_type(right)
	{
	}
	
	inner_process_id(BOOST_RV_REF(this_type) right)
        : base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	inner_process_id(const this_type& right);
	~inner_process_id(void);


public:
	this_type& operator=(const id_data_type& right);

	template<typename Other>
	typename boost::enable_if<container_ex::is_container<Other>, this_type&>::type 
		operator=(const Other& right)
	{
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(right)));
        return *this;
	}

	this_type& operator=(const this_type& right);

public:
	using base_type::swap;


	inline void swap(BOOST_RV_REF(id_data_type) right)
	{
		id_data_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(id_data_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		base_type::swap(boost::move(static_cast<base_type&>(right)));
	}

	void swap(this_type& right);

public:
	using base_type::begin;
	using base_type::cbegin;
	
	using base_type::end;
	using base_type::cend;

	using base_type::rbegin;
	using base_type::crbegin;

	using base_type::rend;
	using base_type::crend;

	using base_type::assign;
	using base_type::is_nil;
	using base_type::empty;

	inline void assign(main_id_type main_id, inner_id_type inner_id)
	{
		memcpy(&(base_type::_data[0]), &main_id, sizeof(main_id_type));
		memcpy(&(base_type::_data[sizeof(main_id_type)]), &inner_id, sizeof(inner_id_type));
	}

	inline void assign(BOOST_RV_REF(this_type) right)
	{
		this_type::operator=(boost::move(right));
	}

	inline void assign(const this_type& right)
	{
		this_type::operator=(right);
	}

	using base_type::operator[];

	using base_type::compare_eq;

	inline bool compare_eq(const id_data_type& right) const
	{
		id_data_cmp_helper_type h;
		return 0 == h(*this, right);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return base_type::compare_eq(static_cast<const base_type&>(right));
	}

	using base_type::compare;

	inline s32 compare(const id_data_type& right) const
	{
		id_data_cmp_helper_type h;
		return h(*this, right);
	}

	inline s32 compare(const this_type& right) const
	{
		return base_type::compare(static_cast<const base_type&>(right));
	}

	using base_type::hash;

	inline this_type ntoh(void) const
	{
		assert(false);
		return *this;
	}

	inline this_type hton(void) const
	{
		assert(false);
		return *this;
	}

	inline id_data_type id_data(void) const
	{
		id_data_type ids;
		memcpy(&(ids.first), &(base_type::_data[0]), sizeof(main_id_type));
		memcpy(&(ids.second), &(base_type::_data[sizeof(main_id_type)]), sizeof(inner_id_type));
		return ids;
	}

public:
	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	static bool from_code(this_type& id, const Basic_String<char, Traits, Alloc>& code)
	{
		typedef Basic_String<char, Traits, Alloc> string_type;
		typedef typename string_type::iterator str_iter_type;

		yggr::u64 pos = code.find_first_not_of("0123456789abcdefABCDEF-");
		if(pos != string_type::npos)
		{
			return false;
		}
		string_type right_code = code;
		str_iter_type new_end = std::remove(right_code.begin(), right_code.end(), '-');
		if(std::distance(right_code.begin(), new_end) != 32)
		{
			return false;
		}

		yggr::u32 n = 0;
		yggr::u32 idx = 0;
		for(str_iter_type i = right_code.begin(), isize = new_end;
				i != isize; std::advance(i, 2), ++idx)
		{
			std::stringstream ss;
			ss << std::hex << (*i) << (*(i + 1));
			ss >> n;
			id[idx] = n;
		}
		return true;
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static this_type create_of_code(const Basic_String<char, Traits, Alloc>& code)
	{
		this_type tmp;
		this_type::from_code(tmp, code);
		return tmp;
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"inner_process_id_data",
				(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

// ==
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator==(const inner_process_id& l, const Other& r)
{
	return l.compare_eq(r);
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator==(const Other& l, const inner_process_id& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const inner_process_id& l, const inner_process_id::id_data_type& r)
{
	return l.compare_eq(r);
}

inline bool operator==(const inner_process_id::id_data_type& l, const inner_process_id& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const inner_process_id& l, const inner_process_id& r)
{
	return l.compare_eq(r);
}

// !=
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator!=(const inner_process_id& l, const Other& r)
{
	return !l.compare_eq(r);
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator!=(const Other& l, const inner_process_id& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const inner_process_id& l, const inner_process_id::id_data_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator!=(const inner_process_id::id_data_type& l, const inner_process_id& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const inner_process_id& l, const inner_process_id& r)
{
	return !l.compare_eq(r);
}

// <
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<(const inner_process_id& l, const Other& r)
{
	return l.compare(r) < 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<(const Other& l, const inner_process_id& r)
{
	return !(r.compare(l) <= 0);
}

inline bool operator<(const inner_process_id& l, const inner_process_id::id_data_type& r)
{
	return l.compare(r) < 0;
}

inline bool operator<(const inner_process_id::id_data_type& l, const inner_process_id& r)
{
	return !(r.compare(l) <= 0);
}

inline bool operator<(const inner_process_id& l, const inner_process_id& r)
{
	return l.compare(r) < 0;
}

// <=
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<=(const inner_process_id& l, const Other& r)
{
	return l.compare(r) <= 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<=(const Other& l, const inner_process_id& r)
{
	return !(r.compare(l) < 0);
}

inline bool operator<=(const inner_process_id& l, const inner_process_id::id_data_type& r)
{
	return l.compare(r) <= 0;
}

inline bool operator<=(const inner_process_id::id_data_type& l, const inner_process_id& r)
{
	return !(r.compare(l) < 0);
}

inline bool operator<=(const inner_process_id& l, const inner_process_id& r)
{
	return l.compare(r) <= 0;
}

// >
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>(const inner_process_id& l, const Other& r)
{
	return l.compare(r) > 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>(const Other& l, const inner_process_id& r)
{
	return !(r.compare(l) >= 0);
}

inline bool operator>(const inner_process_id& l, const inner_process_id::id_data_type& r)
{
	return l.compare(r) > 0;
}

inline bool operator>(const inner_process_id::id_data_type& l, const inner_process_id& r)
{
	return !(r.compare(l) >= 0);
}

inline bool operator>(const inner_process_id& l, const inner_process_id& r)
{
	return l.compare(r) > 0;
}

// >=
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>=(const inner_process_id& l, const Other& r)
{
	return l.compare(r) >= 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>=(const Other& l, const inner_process_id& r)
{
	return !(r.compare(l) > 0);
}

inline bool operator>=(const inner_process_id& l, const inner_process_id::id_data_type& r)
{
	return l.compare(r) >= 0;
}

inline bool operator>=(const inner_process_id::id_data_type& l, const inner_process_id& r)
{
	return !(r.compare(l) > 0);
}

inline bool operator>=(const inner_process_id& l, const inner_process_id& r)
{
	return l.compare(r) >= 0;
}

// cout <<
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

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//					const yggr::ids::inner_process_id& right);

} // namespace ids
} // namespace yggr

// --------------swap----------------
namespace yggr
{
namespace ids
{
namespace swap_support
{
template<typename Other> inline 
typename boost::enable_if<yggr::is_container<Other>, void>::type 
	swap(Other& l,inner_process_id& r) 
{ 
	r.swap(l);
} 

template<typename Other> inline 
typename boost::enable_if<yggr::is_container<Other>, void>::type 
	swap(inner_process_id& l, Other& r) 
{ 
	l.swap(r); 
} 

inline void swap(inner_process_id::id_data_type& l, inner_process_id& r)
{
	r.swap(l);
}

inline void swap(inner_process_id& l, inner_process_id::id_data_type& r)
{
	l.swap(r);
}

inline void swap(inner_process_id& l, inner_process_id& r)
{
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace ids
} // namespace yggr

namespace std
{
	using ::yggr::ids::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::ids::swap_support::swap;
} // namespace boost

//----------------id_generator---------------
namespace yggr
{
namespace ids
{

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
		typedef mplex::numeric_limits<u16> limits_type;

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

			if(!(t_incr = (t_incr + 1) % limits_type::max_type::value))
			{
				t_fuzz = (random::gen_random<u32>()) & 0xffff0000;
			}

			re_fuzz_incr = t_fuzz | t_incr;
		} while(!fuzz_incr.compare_exchange_weak(t_fuzz_incr, re_fuzz_incr));

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

//-----------hash_value--------------

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

inline std::size_t hash_value(const yggr::ids::inner_process_id& id)
{
	return id.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<>
struct hash<yggr::ids::inner_process_id >
{
	typedef yggr::ids::inner_process_id type;

	inline std::size_t operator()(const type& ch) const
	{
		return ch.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

//---------------strict_sizeof--------------

YGGR_PP_REG_STRICT_SIZEOF(
	::yggr::ids::inner_process_id, 
	sizeof(::yggr::ids::id64_type) + sizeof(yggr::ids::id64_type) )

#endif //__YGGR_IDS_INNER_PROCESS_ID_HPP__
