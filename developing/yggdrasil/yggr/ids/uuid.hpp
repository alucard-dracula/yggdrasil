//uuid.hpp

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

#ifndef __YGGR_IDS_UUID_HPP__
#define __YGGR_IDS_UUID_HPP__

#include <algorithm>
#include <iostream>


//#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/additive_combine.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include <yggr/move/move.hpp>
//
#include <yggr/base/yggrdef.h>
//#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ids/id_n.hpp>
#include <yggr/charset/string.hpp>


#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

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
	struct get_genner : private nonable::noncreateable
	{
	private:
		typedef T r_type;
	public:
		typedef boost::uuids::basic_random_generator<r_type> genner_type;
	};
};

template<>
struct parse_uuid_genner::get_genner<void>
{
private:
    typedef void r_type;
public:
    typedef boost::uuids::random_generator genner_type;
};

class uuid : public id_n<16>
{
public:
	typedef id_n<16> base_type;

    typedef base_type::iterator iterator;
    typedef base_type::const_iterator const_iterator;

private:

	enum
    {
        variant_ncs = 0, // NCS backward compatibility
        variant_rfc_4122, // defined in RFC 4122 document
        variant_microsoft, // Microsoft Corporation backward compatibility
        variant_future, // future definition
		E_compile_u32_2 = 0xffffffff
    };

	enum
    {
        version_unknown = -1,
        version_time_based = 1,
        version_dce_security = 2,
        version_name_based_md5 = 3,
        version_random_number_based = 4,
        version_name_based_sha1 = 5,
		E_compile_u32_3 = 0xffffffff
    };

	typedef uuid this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)
public:

	uuid(void);

	template<typename Other>
	explicit uuid(const Other& right)
		: base_type(right)
	{
	}

	uuid(BOOST_RV_REF(this_type) right)
        : base_type(boost::forward<base_type>(right))
    {
    }

	uuid(const this_type& right);
	virtual ~uuid(void);

    u32 variant(void) const;
    u32 version(void) const;

    // note: linear complexity
	template<typename Other>
	inline void swap(Other& right)
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

	virtual std::size_t hash(void) const;

public:
	
	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	static bool s_from_code(this_type& id, const Basic_String<char, Traits, Alloc>& code)
	{
		typedef Basic_String<char, Traits, Alloc> string_type;
		typedef typename string_type::iterator str_iter_type;

		yggr::u64 pos = code.find_first_not_of("0123456789abcdef-");
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
private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("uuid_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

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

	id_type operator()(void) const
	{
		genner_type uuid_gen;
		return id_type(uuid_gen());
	}
};

} // namespace ids
} // namespace yggr


template<typename Other>
inline bool operator==(const Other& left, const yggr::ids::uuid& right)
{
	return right == left;
}

template<typename Other>
inline bool operator!=(const Other& left, const yggr::ids::uuid& right)
{
    return right != left;
}

template<typename Other>
inline bool operator<(const Other& left, const yggr::ids::uuid& right)
{
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
}

template<typename Other>
inline bool operator>(const Other& left, const yggr::ids::uuid& right)
{
    return right < left;
}

template<typename Other>
inline bool operator<=(const Other& left, const yggr::ids::uuid& right)
{
    return !(right < left);
}

template<typename Other>
inline bool operator>=(const Other& left, const yggr::ids::uuid& right)
{
    return !(left < right);
}

#define _YGGR_TMP_PP_UUID_SWAP_DEF() \
	template<typename Other> inline \
	void swap(Other& left, yggr::ids::uuid& right) { \
		right.swap(left); } \
	\
	template<typename Other> inline \
	void swap(yggr::ids::uuid& left, Other& right) { \
		left.swap(right); } \
	\
	void swap(yggr::ids::uuid& left, yggr::ids::uuid& right); \
	\
	inline void swap(BOOST_RV_REF(yggr::ids::uuid) left, yggr::ids::uuid& right) { \
		right.swap(left); } \
	\
	inline void swap(yggr::ids::uuid& left, BOOST_RV_REF(yggr::ids::uuid) right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_UUID_SWAP_DEF()
} //namespace std

namespace boost
{
	_YGGR_TMP_PP_UUID_SWAP_DEF()
} //namespace boost

#undef _YGGR_TMP_PP_UUID_SWAP_DEF

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::ids::uuid& right)
{
	yggr::size_type idx = 0;

	os << std::hex;
	for(yggr::ids::uuid::const_iterator i = right.begin(), isize = right.end(); i != isize; ++i, ++idx)
	{
		os << std::setw(2) << std::setfill(static_cast<Char>('0'))
			<<  static_cast<yggr::u16>(static_cast<yggr::u8>(*i));
		if(idx == 4 || idx == 6 || idx == 8 || idx == 10)
		{
			os << "-";
		}
	}

	os << std::dec;
	return os;
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

std::size_t hash_value(const yggr::ids::uuid& id);

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_IDS_UUID_HPP__
