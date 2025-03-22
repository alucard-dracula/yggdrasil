//uuid.hpp

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

#ifndef __YGGR_IDS_UUID_HPP__
#define __YGGR_IDS_UUID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/ids/id_n.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/mplex/strict_sizeof.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/additive_combine.hpp>
#include <boost/random/lagged_fibonacci.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include <algorithm>
#include <ostream>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace ids
{

template<typename T>
struct uuid_convertible
	: public boost::mpl::false_
{
};

template<>
struct uuid_convertible<boost::uuids::uuid>
	: public boost::mpl::true_
{
};

} // namespace ids
} // namespace yggr

namespace yggr
{
namespace ids
{
namespace detail
{

template<typename T>
struct parse_uuid_genner_get_genner
{
private:
	typedef T r_type;
public:
	typedef boost::uuids::basic_random_generator<r_type> genner_type;
};

template<>
struct parse_uuid_genner_get_genner<void>
{
private:
    typedef void r_type;
public:
    typedef boost::uuids::random_generator genner_type;
};


} // namespace detail
} // namespace ids
} // namespace yggr

namespace yggr
{
namespace ids
{

struct parse_uuid_genner
{
public:
	typedef void normal_type;
	typedef boost::mt19937 mt19937_type;
	typedef boost::rand48 rand48_type;
	typedef boost::ecuyer1988 ecuyer1988_type;
	typedef boost::lagged_fibonacci607 lagged_fibonacci607_type;

	template<typename T>
	struct get_genner
		: public detail::parse_uuid_genner_get_genner<T>
	{
	};
};

} // namespace ids
} // namespace yggr

//----------------uuid---------------

namespace yggr
{
namespace ids
{

class uuid 
	: public id_n<16>
{
public:
	typedef id_n<16> base_type;

private:
	// variant
	YGGR_STATIC_CONSTANT(u32, variant_ncs = 0); // NCS backward compatibility
	YGGR_STATIC_CONSTANT(u32, variant_rfc_4122 = variant_ncs + 1); // defined in RFC 4122 document
	YGGR_STATIC_CONSTANT(u32, variant_microsoft = variant_ncs + 2); // Microsoft Corporation backward compatibility
	YGGR_STATIC_CONSTANT(u32, variant_future = variant_ncs + 3); // future definition

	// version
	YGGR_STATIC_CONSTANT(u32, version_unknown = static_cast<u32>(-1));
	YGGR_STATIC_CONSTANT(u32, version_time_based = 1);
	YGGR_STATIC_CONSTANT(u32, version_dce_security = 2);
	YGGR_STATIC_CONSTANT(u32, version_name_based_md5 = 3);
	YGGR_STATIC_CONSTANT(u32, version_random_number_based = 4);
	YGGR_STATIC_CONSTANT(u32, version_name_based_sha1 = 5);

private:
	typedef uuid this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:

	// construct/destruct
	uuid(void);

	template<typename InputIter>
	uuid(InputIter s, InputIter e)
		: base_type(s, e)
	{
	}

	template<typename Other>
	explicit uuid(const Other& right, 
					typename boost::enable_if<container_ex::is_container<Other>, void*>::type chk = 0)
		: base_type(right)
	{
	}

	template<typename Other>
	explicit uuid(const Other& right, 
					typename boost::enable_if<uuid_convertible<Other>, void*>::type chk = 0)
		: base_type(boost::begin(right), boost::end(right))
	{
	}

	uuid(BOOST_RV_REF(this_type) right)
        : base_type(boost::move(static_cast<base_type&>(right)))
    {
    }

	uuid(const this_type& right);
	~uuid(void);

public:
	// operator=
	template<typename Other>
	typename boost::enable_if<container_ex::is_container<Other>, this_type&>::type 
		operator=(const Other& right)
	{
		base_type::operator=(right);
		return *this;
	}

	template<typename Other>
	typename boost::enable_if<uuid_convertible<Other>, this_type&>::type 
		operator=(const Other& right)
	{
		base_type::assign(boost::begin(right), boost::end(right));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
    {
		base_type::operator=(boost::move(static_cast<base_type&>(right)));
        return *this;
    }

	this_type& operator=(const this_type& right);

public:
	// swap
	using base_type::swap;

	template<typename Other> inline
	typename boost::enable_if<uuid_convertible<Other>, void>::type 
		swap(BOOST_RV_REF(Other) right)
	{
		Other& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Other>
	typename boost::enable_if<uuid_convertible<Other>, void>::type 
		swap(Other& right)
	{
		base_type::pro_swap_ranges(boost::begin(right), boost::end(right));
		return *this;
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	// compare

	using base_type::compare_eq;

	template<typename Other> inline
	typename boost::enable_if<uuid_convertible<Other>, bool>::type 
		compare(const Other& right) const
	{
		return 0 == memcmp(&(base_type::_data[0]), &(*boost::begin(right)), base_type::E_length);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return base_type::compare_eq(static_cast<const base_type&>(right));
	}

	using base_type::compare;

	template<typename Other>
	typename boost::enable_if<uuid_convertible<Other>, s32>::type 
		compare(const Other& right) const
	{
		return math::sign(memcmp(&(base_type::_data[0]), &(*boost::begin(right)), base_type::E_length));
	}

	inline s32 compare(const this_type& right) const
	{
		return base_type::compare(static_cast<const base_type&>(right));
	}

	using base_type::inner_buffer;

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

	inline void assign(BOOST_RV_REF(this_type) right)
	{
		this_type::operator=(boost::move(right));
	}

	inline void assign(const this_type& right)
	{
		this_type::operator=(right);
	}

	using base_type::operator[];
	using base_type::data;
	using base_type::size;


	std::size_t hash(void) const;

	using base_type::ntoh;
	using base_type::hton;

	inline u32 variant(void) const
	{
		// variant is stored in octet 7
		// which is index 8, since indexes count backwards
		yggr::u8 octet7 = _data[8]; // octet 7 is array index 8
		if( (octet7 & 0x80) == 0x00 ) // 0b0xxxxxxx
		{ 
			return variant_ncs;
		} 
		else if( (octet7 & 0xC0) == 0x80 ) // 0b10xxxxxx
		{ 
			return variant_rfc_4122;
		} 
		else if( (octet7 & 0xE0) == 0xC0 ) // 0b110xxxxx
		{ 
			return variant_microsoft;
		}
		else 
		{
			//assert( (octet7 & 0xE0) == 0xE0 ) // 0b111xxxx
			return variant_future;
		}
	}

    inline u32 version(void) const
	{
		 //version is stored in octet 9
		// which is index 6, since indexes count backwards
		yggr::u8 octet9 = _data[6];
		if( (octet9 & 0xF0) == 0x10 )
		{
			return version_time_based;
		} 
		else if( (octet9 & 0xF0) == 0x20 ) 
		{
			return version_dce_security;
		} 
		else if( (octet9 & 0xF0) == 0x30 ) 
		{
			return version_name_based_md5;
		}
		else if( (octet9 & 0xF0) == 0x40 ) 
		{
			return version_random_number_based;
		}
		else if( (octet9 & 0xF0) == 0x50 ) 
		{
			return version_name_based_sha1;
		}
		else
		{
			return version_unknown;
		}
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
		ar & YGGR_SERIALIZATION_NAME_NVP("uuid_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

typedef uuid uuid_type;

// ==
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator==(const uuid& l, const Other& r)
{
	return l.compare_eq(r);
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator==(const Other& l, const uuid& r)
{
	return r.compare_eq(l);
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator==(const uuid& l, const Other& r)
{
	return l.compare_eq(r);
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator==(const Other& l, const uuid& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const uuid& l, const uuid& r)
{
	return l.compare_eq(r);
}

// !=
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator!=(const uuid& l, const Other& r)
{
	return !(l.compare_eq(r));
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator!=(const Other& l, const uuid& r)
{
	return !(r.compare_eq(l));
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator!=(const uuid& l, const Other& r)
{
	return !(l.compare_eq(r));
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator!=(const Other& l, const uuid& r)
{
	return !(r.compare_eq(l));
}

inline bool operator!=(const uuid& l, const uuid& r)
{
	return !(l.compare_eq(r));
}

// <
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<(const uuid& l, const Other& r)
{
	return l.compare(r) < 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<(const Other& l, const uuid& r)
{
	return !(r.compare(l) <= 0);
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator<(const uuid& l, const Other& r)
{
	return l.compare(r) < 0;
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator<(const Other& l, const uuid& r)
{
	return !(r.compare(l) <= 0);
}

inline bool operator<(const uuid& l, const uuid& r)
{
	return l.compare(r) < 0;
}

// <=
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<=(const uuid& l, const Other& r)
{
	return l.compare(r) <= 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator<=(const Other& l, const uuid& r)
{
	return !(r.compare(l) < 0);
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator<=(const uuid& l, const Other& r)
{
	return l.compare(r) <= 0;
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator<=(const Other& l, const uuid& r)
{
	return !(r.compare(l) < 0);
}

inline bool operator<=(const uuid& l, const uuid& r)
{
	return l.compare(r) <= 0;
}

// >
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>(const uuid& l, const Other& r)
{
	return l.compare(r) > 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>(const Other& l, const uuid& r)
{
	return !(r.compare(l) >= 0);
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator>(const uuid& l, const Other& r)
{
	return l.compare(r) > 0;
}

template<typename Other>
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator>(const Other& l, const uuid& r)
{
	return !(r.compare(l) >= 0);
}

inline bool operator>(const uuid& l, const uuid& r)
{
	return l.compare(r) > 0;
}

// >=
template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>=(const uuid& l, const Other& r)
{
	return l.compare(r) >= 0;
}

template<typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type  
	operator>=(const Other& l, const uuid& r)
{
	return !(r.compare(l) > 0);
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator>=(const uuid& l, const Other& r)
{
	return l.compare(r) >= 0;
}

template<typename Other> inline
typename boost::enable_if<uuid_convertible<Other>, bool>::type  
	operator>=(const Other& l, const uuid& r)
{
	return !(r.compare(l) > 0);
}

inline bool operator>=(const uuid& l, const uuid& r)
{
	return l.compare(r) >= 0;
}

// operator<<
template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::ids::uuid& right)
{
	yggr::size_type idx = 0;

	os << std::hex;
	for(yggr::ids::uuid::const_iterator i = right.begin(), isize = right.end(); i != isize; ++i, ++idx)
	{
		switch(idx)
		{
		case 4:
		case 6:
		case 8:
		case 10:
			os << "-";
		}

		os << std::setw(2) << std::setfill(static_cast<Char>('0'))
			<<  static_cast<yggr::u16>(static_cast<yggr::u8>(*i));
	}

	os << std::dec;
	return os;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::ids::uuid& right);

} // namespace ids
} // namespace yggr

//-----------swap-------------
namespace yggr
{
namespace ids
{
namespace swap_support
{


template<typename Other> inline 
typename boost::enable_if<yggr::is_container<Other>, void>::type 
	swap(Other& l, uuid& r) 
{
	r.swap(l);
} 
	
template<typename Other> inline 
typename boost::enable_if<yggr::is_container<Other>, void>::type 
	swap(uuid& l, Other& r) 
{ 
	l.swap(r); 
} 
	
template<typename Other> inline 
typename boost::enable_if<uuid_convertible<Other>, void>::type 
	swap(Other& l, uuid& r) 
{
	r.swap(l); 
} 
	
template<typename Other> inline 
typename boost::enable_if<uuid_convertible<Other>, void>::type 
	swap(uuid& l, Other& r) 
{ 
	l.swap(r);
} 
	
inline void swap(uuid& l, uuid& r)
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

//--------------id_generator-----------
namespace yggr
{
namespace ids
{

template<typename Random>
struct id_generator<uuid, Random>
{
public:
	typedef uuid id_type;

private:
	typedef boost::uuids::uuid val_type;
	typedef Random random_type;
	typedef parse_uuid_genner::get_genner<random_type> get_genner_type;
	typedef typename get_genner_type::genner_type genner_type;

public:

	inline id_type operator()(void) const
	{
		genner_type uuid_gen;
		return id_type(uuid_gen());
	}
};

} // namespace ids
} // namespace yggr

//----------hash------------
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

inline std::size_t hash_value(const yggr::ids::uuid& id)
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
struct hash<yggr::ids::uuid >
{
	typedef yggr::ids::uuid type;

	inline std::size_t operator()(const type& id) const
	{
		return id.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

//---------------strict_sizeof--------------

YGGR_PP_REG_STRICT_SIZEOF(::yggr::ids::uuid, 16)

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_IDS_UUID_HPP__
