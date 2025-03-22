//id_n.hpp

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

#ifndef __YGGR_IDS_ID_N_HPP__
#define __YGGR_IDS_ID_N_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/ids/base_ids_def.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/algorithm/shuffle.hpp>
#include <yggr/algorithm/lexicographical_c_compare.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/strict_sizeof.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/bytes/byte_size.hpp>

#include <yggr/support/operator_t.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/string.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/range/functions.hpp>
#include <boost/functional/hash/hash.hpp>

#include <iomanip>
#include <memory>
#include <algorithm>
#include <iterator>

#if defined(_MSC_VER)
#	pragma warning(push) // Save warning settings.
#	pragma warning(disable : 4996) // Disable deprecated std::swap_ranges, std::equal
#endif //_MSC_VER

namespace yggr
{
namespace ids
{

template<u64 LEN>
class id_n
{
public:
	typedef ::yggr::string string_type;
    typedef string_type::iterator iterator;
    typedef string_type::const_iterator const_iterator;
	typedef string_type::reverse_iterator reverse_iterator;
    typedef string_type::const_reverse_iterator const_reverse_iterator;

public:
	YGGR_STATIC_CONSTANT(u32, E_class_length = 0);
	YGGR_STATIC_CONSTANT(u32, E_length = LEN);

private:
	typedef id_n this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	id_n(void)
		: _data(this_type::E_length, 0)
	{
	}

	template<typename InputIter>
	id_n(InputIter s, InputIter e)
		: _data(this_type::E_length, 0)
	{
		if(this_type::pro_s_range_size(s, e) == E_length)
		{
			std::copy(s, e, this_type::pro_cast_to_begin_pointer(s));
		}
	}

	template<typename Other>
	explicit id_n(const Other& right,
					typename boost::enable_if<container_ex::is_container<Other>, void*>::type chk = 0)
		: _data(this_type::E_length, 0)
	{
		if(byte_size(right) == E_length)
		{
			std::copy(
				boost::begin(right),
				boost::end(right),
				this_type::pro_cast_to_begin_pointer(boost::begin(right)));
		}
	}

	id_n(BOOST_RV_REF(this_type) right)
		: _data(boost::move(right._data))
	{
	}

	id_n(const this_type& right)
		: _data(right._data)
	{
	}

	~id_n(void)
	{
	}

public:
	template<typename Other> inline
	typename boost::enable_if<container_ex::is_container<Other>, this_type&>::type
		operator=(const Other& right)
	{
		if(byte_size(right) == E_length)
		{
			std::copy(
				boost::begin(right),
				boost::end(right),
				this_type::pro_cast_to_begin_pointer(boost::begin(right)));
		}

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == boost::addressof(right_chk))
		{
			return *this;
		}

		copy_or_move_or_swap(_data, boost::move(right._data));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}
		if(this_type::size()) memcpy(boost::addressof(_data[0]), boost::addressof(right._data[0]), this_type::size());
		return *this;
	}

public:
	// note: linear complexity
	template<typename Other>
	typename boost::enable_if<container_ex::is_container<Other>, void>::type
		swap(Other& right)
	{
		if(byte_size(right) == E_length)
		{
			std::swap_ranges(
				boost::begin(right),
				boost::end(right),
				this_type::pro_cast_to_begin_pointer(boost::begin(right)));
		}
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

		_data.swap(right._data);
	}

public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !is_nil();
	}

	inline operator string_type&(void)
	{
		return _data;
	}

	inline operator const string_type&(void) const
	{
		return _data;
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	operator Basic_String<char, Traits, Alloc>(void) const
	{
		typedef Basic_String<char, Traits, Alloc> ret_type;
		return ret_type(boost::begin(_data), boost::end(_data));
	}

public:
	inline string_type& inner_buffer(void)
	{
		return _data;
	}

	inline const string_type& inner_buffer(void) const
	{
		return _data;
	}

public:
	// begin
	inline iterator begin(void)
	{
		//return boost::begin(_data); // gcc support
		return _data.begin();
	}

	inline const_iterator begin(void) const
	{
		//return boost::const_begin(_data); // gcc support
		return _data.begin();
	}

	inline const_iterator cbegin(void) const
	{
		//return boost::const_begin(_data); // gcc support
		return _data.begin();
	}

	// end
	inline iterator end(void)
	{
		//return boost::end(_data); // gcc support
		return _data.end();
	}

	inline const_iterator end(void) const
	{
		//return boost::const_end(_data); // gcc support
		return _data.end();
	}

	inline const_iterator cend(void) const
	{
		//return boost::const_end(_data); // gcc support
		return _data.end();
	}

	// rbegin
	inline reverse_iterator rbegin(void)
	{
		//return boost::rbegin(_data); // gcc not support
		return _data.rbegin();
	}

	inline const_reverse_iterator rbegin(void) const
	{
		//return boost::const_rbegin(_data); // gcc not support
		return _data.rbegin();
	}

	inline const_reverse_iterator crbegin(void) const
	{
		//return boost::const_rbegin(_data); // gcc not support
		return _data.rbegin();
	}

	// rend
	inline reverse_iterator rend(void)
	{
		//return boost::rend(_data); // gcc not support
		return _data.rend();
	}

	inline const_reverse_iterator rend(void) const
	{
		//return boost::const_rend(_data); // gcc not support
		return _data.rend();
	}

	inline const_reverse_iterator crend(void) const
	{
		//return boost::const_rend(_data); // gcc not support
		return _data.rend();
	}

	template<typename InputIter>
	bool assign(InputIter s, InputIter e)
	{
		assert(_data.size() == E_length);

		return
			(this_type::pro_s_range_size(s, e) == E_length)
			&& (std::copy(s, e, this_type::pro_cast_to_begin_pointer(s)), true);
	}

	template<typename Other>
	typename boost::enable_if<container_ex::is_container<Other>, bool>::type
		assign(const Other& right)
	{
		assert(_data.size() == E_length);
		return
			(byte_size(right) == E_length)
			&& (std::copy(
					boost::begin(right),
					boost::end(right),
					this_type::pro_cast_to_begin_pointer(boost::begin(right))), true);
	}

	inline void assign(BOOST_RV_REF(this_type) right)
	{
		this_type::operator=(boost::move(right));
	}

	inline void assign(const this_type& right)
	{
		this_type::operator=(right);
	}

	inline void clear(void)
	{
		memset(boost::addressof(_data[0]), 0, _data.size());
	}

    bool is_nil(void) const
	{
		static const string_type::value_type chk[this_type::E_length] = {0};
		return 0 == memcmp(boost::addressof(_data[0]), chk, this_type::E_length);
	}

	inline bool empty(void) const
	{
		return is_nil();
	}

	inline string_type::value_type& operator[](std::size_t idx)
	{
		return _data[idx];
	}

	inline const char* data(void) const
	{
		return _data.data();
	}

	// don't change return string_type::value_type, because interprocess_id need it in id_data_cmp_helper
	inline const string_type::value_type& operator[](std::size_t idx) const
	{
		return _data[idx];
	}

	template<typename Other> inline
	typename boost::enable_if<container_ex::is_container<Other>, bool>::type
		compare_eq(const Other& right) const
	{
		std::size_t right_byte_size = byte_size(right);
		return
			(this_type::E_length == right_byte_size)
			&& (std::equal(
					boost::begin(right),
					boost::end(right),
					this_type::pro_cast_to_begin_pointer(boost::begin(right))));
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (0 == memcmp(boost::addressof(_data[0]), boost::addressof(right._data[0]), this_type::E_length));
	}

	template<typename Other> inline
	typename boost::enable_if<container_ex::is_container<Other>, s32>::type
		compare(const Other& right) const
	{
		return
			algorithm::lexicographical_c_compare(
				this_type::pro_cast_to_begin_pointer(boost::begin(right)),
				yggr::advance_copy(this_type::pro_cast_to_begin_pointer(boost::begin(right)), boost::size(right)),
				boost::begin(right),
				boost::end(right),
				algorithm::c_hex_cmp());
	}

	inline s32 compare(const this_type& right) const
	{
		return
			this == boost::addressof(right)?
				0
				: math::sign(memcmp(boost::addressof(_data[0]), boost::addressof(right._data[0]), this_type::E_length));
	}

	inline std::size_t hash(void) const
	{
		assert(_data.size() == this_type::E_length);
		return boost::hash_range(_data.begin(), _data.end());
	}

	inline this_type ntoh(void) const
	{
		return *this;
	}

	inline this_type hton(void) const
	{
		return *this;
	}

protected:
	template<typename InputIt> inline
	void pro_swap_ranges(InputIt s, InputIt e)
	{
		if(this_type::pro_s_range_size(s, e) == E_length)
		{
			std::swap_ranges(s, e, this_type::pro_cast_to_begin_pointer(s));
		}
	}

	template<typename Iter> inline
	typename iterator_ex::iterator_value_ex<Iter>::type*
		pro_cast_to_begin_pointer(const Iter&)
	{
		typedef Iter iter_type;
		typedef typename iterator_ex::iterator_value_ex<iter_type>::type value_type;

		return reinterpret_cast<value_type*>(boost::addressof(_data[0]));
	}

	template<typename Iter> inline
	const typename iterator_ex::iterator_value_ex<Iter>::type*
		pro_cast_to_begin_pointer(const Iter&) const
	{
		typedef Iter iter_type;
		typedef typename iterator_ex::iterator_value_ex<iter_type>::type value_type;

		return reinterpret_cast<const value_type*>(boost::addressof(_data[0]));
	}

protected:

	template<typename Iter> inline
	static std::size_t pro_s_range_size(Iter s, Iter e)
	{
		return std::distance(s, e) * this_type::pro_s_iterator_value_sizeof(s);
	}

	template<typename Iter> inline
	static std::size_t pro_s_iterator_value_sizeof(const Iter&)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type value_type;
		return mplex::strict_sizeof<value_type>::value;
	}

public:
	inline static size_type size(void)
	{
		return this_type::E_length;
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive> inline
	void serialize(Archive& ar, const u32 version)
	{
		typedef Archive archive_type;
		typedef typename YGGR_PP_ARCHIVE_NEED_BINARY_DATA_WRAP_TRAITS_T(archive_type, string_type) bool_type;
		typedef bool_type* bool_ptr_type;
		serialize_detail(ar, version, bool_ptr_type(0));
	}

	template<typename Archive> inline
	void serialize_detail(Archive& ar, const u32 version, boost::mpl::false_*) // other
	{
		ar & YGGR_SERIALIZATION_NVP(_data);
	}

	template<typename Archive> inline
	void serialize_detail(Archive& ar, const u32 version, boost::mpl::true_*) // bson
	{
		typedef Archive archive_type;
		typedef typename YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_WRAP(archive_type, string_type) buffer_wrap_type;
		YGGR_SERIALIZATION_WRAP_NVP_AR(ar, buffer_wrap_type, _data)
	}

protected:
	string_type _data;
};

// ==
template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator==(const id_n<N>& l, const Other& r)
{
	return l.compare_eq(r);
}

template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator==(const Other& l, const id_n<N>& r)
{
	return r.compare_eq(l);
}

template<u64 N> inline
bool operator==(const id_n<N>& l, const id_n<N>& r)
{
	return l.compare_eq(r);
}

// !=
template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator!=(const id_n<N>& l, const Other& r)
{
	return !(l.compare_eq(r));
}

template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator!=(const Other& l, const id_n<N>& r)
{
	return !(r.compare_eq(l));
}

template<u64 N> inline
bool operator!=(const id_n<N>& l, const id_n<N>& r)
{
	return !(l.compare_eq(r));
}

// <
template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator<(const id_n<N>& l, const Other& r)
{
	return l.compare(r) < 0;
}

template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator<(const Other& l, const id_n<N>& r)
{
	return !(r.compare(l) <= 0);
}

template<u64 N> inline
bool operator<(const id_n<N>& l, const id_n<N>& r)
{
	return l.compare(r) < 0;
}

// <=
template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator<=(const id_n<N>& l, const Other& r)
{
	return l.compare(r) <= 0;
}

template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator<=(const Other& l, const id_n<N>& r)
{
	return !(r.compare(l) < 0);
}

template<u64 N> inline
bool operator<=(const id_n<N>& l, const id_n<N>& r)
{
	return l.compare(r) <= 0;
}

// >
template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator>(const id_n<N>& l, const Other& r)
{
	return l.compare(r) > 0;
}

template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator>(const Other& l, const id_n<N>& r)
{
	return !(r.compare(l) >= 0);
}

template<u64 N> inline
bool operator>(const id_n<N>& l, const id_n<N>& r)
{
	return l.compare(r) > 0;
}

// >=
template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator>=(const id_n<N>& l, const Other& r)
{
	return l.compare(r) >= 0;
}

template<u64 N, typename Other> inline
typename boost::enable_if<container_ex::is_container<Other>, bool>::type
	operator>=(const Other& l, const id_n<N>& r)
{
	return !(r.compare(l) > 0);
}

template<u64 N> inline
bool operator>=(const id_n<N>& l, const id_n<N>& r)
{
	return l.compare(r) >= 0;
}

// operator<<

template<typename Char, typename Traits, yggr::u64 LEN>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
					const yggr::ids::id_n<LEN>& right)
{
	typedef yggr::ids::id_n<LEN> id_type;
	typedef typename id_type::const_iterator iter_type;
	yggr::size_type idx = 0;

	os << std::hex;
	for(iter_type i = right.begin(), isize = right.end(); i != isize; ++i, ++idx)
	{
		os << std::setw(2) << std::setfill(static_cast<Char>('0')) <<  static_cast<yggr::u16>(static_cast<yggr::u8>(*i));
	}

	os << std::dec;
	return os;
}

// now don't use this foo style, but don't delete it

//template<typename Char, typename Traits, yggr::u64 LEN>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//					BOOST_RV_REF_BEG yggr::ids::id_n<LEN> BOOST_RV_REF_END right)
//{
//	const yggr::ids::id_n<LEN>& right_cref= right;
//	return operator(os, right);
//}

//template<typename Char, typename Traits, yggr::u64 LEN,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//					const yggr::ids::id_n<LEN>& right);

} // namespace ids
} // namespace yggr

// ---------------swap------------------

namespace yggr
{
namespace ids
{
namespace swap_support
{

template<typename Other, yggr::u64 LEN> inline
typename boost::enable_if< ::yggr::is_container<Other>, void>::type
	swap(Other& l, id_n<LEN>& r)
{
	r.swap(l);
}

template<typename Other, yggr::u64 LEN> inline
typename boost::enable_if< ::yggr::is_container<Other>, void>::type
	swap(id_n<LEN>& l, Other& r)
{
	l.swap(r);
}

template<yggr::u64 LEN> inline
void swap(id_n<LEN>& l, id_n<LEN>& r)
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

// ---------------id_generator------------------
namespace yggr
{
namespace ids
{

namespace detail
{

template<u64 LEN >
class id_n_generator_basic
	: private nonable::noncopyable
{
public:
	typedef id_n< LEN > id_type;

protected:
	typedef typename id_type::string_type string_type;
	typedef yggr::u64 val_type;
	typedef val_type* val_ptr_type;

	YGGR_STATIC_CONSTANT(u32, E_val_byte_len = sizeof(val_type));
	YGGR_STATIC_CONSTANT(u32, E_len = LEN);
	YGGR_STATIC_CONSTANT(u32, E_val_len = ((LEN + E_val_byte_len - 1) / E_val_byte_len));
	YGGR_STATIC_CONSTANT(u32, E_alloc_len = (E_val_len * E_val_byte_len));

};

template<u64 LEN, bool le16 = (LEN <= 16) >
class id_n_generator;

template<u64 LEN>
class id_n_generator<LEN, true>
	: public id_n_generator_basic<LEN>
{
protected:
	typedef id_n_generator_basic<LEN> base_type;

public:
	typedef typename base_type::id_type id_type;

private:
	typedef typename base_type::string_type string_type;
	typedef typename base_type::val_type val_type;
	typedef typename base_type::val_ptr_type val_ptr_type;

public:
	id_type operator()(void) const
	{
		typedef typename string_type::value_type char_type;
		id_generator<val_type> gen;

		char_type tmp[base_type::E_alloc_len] = {0};

		for(val_ptr_type i = reinterpret_cast<val_ptr_type>(&tmp[0]), isize = i + base_type::E_val_len;
			 i != isize; ++i)
		{
			(*i) = gen();
		}

		return id_type(boost::addressof(tmp[0]), boost::addressof(tmp[0]) + base_type::E_len);
	}
};

template<u64 LEN>
class id_n_generator<LEN, false>
	: public id_n_generator_basic<LEN>
{
protected:
	typedef id_n_generator_basic<LEN> base_type;

public:
	typedef typename base_type::id_type id_type;

private:
	typedef typename base_type::string_type string_type;
	typedef typename base_type::val_type val_type;
	typedef typename base_type::val_ptr_type val_ptr_type;

public:
	id_type operator()(void) const
	{
		typedef typename string_type::value_type char_type;
		//id_generator<val_type> gen;

		char_type tmp[base_type::E_alloc_len] = {0};

		for(val_ptr_type i = reinterpret_cast<val_ptr_type>(&tmp[0]), isize = i + base_type::E_val_len;
			 i != isize; ++i)
		{
			(*i) = random::gen_random<val_type>();
		}

		::yggr::shuffle(boost::addressof(tmp[0]), boost::addressof(tmp[0]) + base_type::E_alloc_len);
		return id_type(boost::addressof(tmp[0]), boost::addressof(tmp[0]) + base_type::E_len);
	}
};

} // namespace detail

template<u64 LEN>
class id_generator< id_n< LEN >, void >
	: public detail::id_n_generator<LEN>
{
protected:
	typedef detail::id_n_generator<LEN> base_type;

public:
	typedef typename base_type::id_type id_type;

public:
	using base_type::operator();

};

} // namespace ids
} // namespace yggr

// ---------------hash_value------------------
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<yggr::u64 LEN>
std::size_t hash_value(const yggr::ids::id_n<LEN>& idn)
{
	return idn.hash();
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

template<yggr::u64 LEN>
struct hash<yggr::ids::id_n<LEN> >
{
	typedef yggr::ids::id_n<LEN> type;

	inline std::size_t operator()(const type& idn) const
	{
		return idn.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

//---------------strict_sizeof--------------

namespace yggr
{
namespace mplex
{
namespace detail
{

template<yggr::u64 LEN>
struct strict_sizeof_helper< yggr::ids::id_n<LEN> >
	: public
		boost::mpl::size_t
		<
			static_cast<std::size_t>(ids::id_n<LEN>::E_length)
		>::type
{
};

} // namespace detail
} // namespace mplex
} // namespace yggr

#if defined(_MSC_VER)
#	pragma warning(pop) // Restore warnings to previous state.
#endif // _MSC_VER

#endif // __YGGR_IDS_ID_N_HPP__
