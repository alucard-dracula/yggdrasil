//id_n.hpp

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

#ifndef __YGGR_IDS_ID_N_HPP__
#define __YGGR_IDS_ID_N_HPP__

#include <iomanip>
#include <memory>
#include <algorithm>

#include <boost/mpl/if.hpp>

#include <boost/functional/hash/hash.hpp>
#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/serialization/nvp.hpp>


#if defined(_MSC_VER)
#	pragma warning(push) // Save warning settings.
#	pragma warning(disable : 4996) // Disable deprecated std::swap_ranges, std::equal
#endif //_MSC_VER

namespace yggr
{
namespace ids
{

template<yggr::u64 LEN>
class id_n
{
public:
	typedef std::string string_type;
    typedef string_type::iterator iterator;
    typedef string_type::const_iterator const_iterator;

private:

	enum
	{
		E_class_length = 0,
		E_length = LEN,
		E_compile_u32_1 = 0xffffffff
	};

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
		: _data(s, e)
	{
	}

	template<typename Other>
	explicit id_n(const Other& right)
		: _data(this_type::E_length, 0)
	{
		std::copy(right.begin(), right.end(), _data.begin());
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

	explicit id_n(BOOST_RV_REF(this_type) right)
		: _data(boost::forward<string_type>(right._data))
	{
	}

#else
	explicit id_n(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_data.swap(right_ref._data);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	id_n(const this_type& right)
		: _data(right)
	{
	}

	virtual ~id_n(void)
	{
	}

	operator bool(void) const
	{
		return !is_nil();
	}

	operator string_type&(void)
	{
		return _data;
	}

	operator const string_type&(void) const
	{
		return _data;
	}

   iterator begin(void)
	{
		return _data.begin();
	}

	const_iterator begin(void) const
	{
		return _data.begin();
	}

	iterator end(void)
	{
		return _data.end();
	}

	const_iterator end(void) const
	{
		return _data.end();
	}

	template<typename InputIter>
	this_type& assign(InputIter s, InputIter e)
	{
		_data.assign(s, e);
		return *this;
	}

    size_type size(void) const
	{
		assert(_data.size() == this_type::E_length);
		return this_type::E_length;
	}

    bool is_nil(void) const
	{
		string_type::value_type chk[this_type::E_length] = {0};
		return !memcmp(&_data[0], chk, this_type::E_length);
	}

	bool empty(void) const
	{
		return is_nil();
	}

    // note: linear complexity
	template<typename Other>
	void swap(Other& right)
	{
		 std::swap_ranges(&_data[0], &_data[0] + E_length, &right[0]);
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//_data.swap(boost::forward<string_type>(right._data));
		_data.swap(right._data);
#else
		this_type& right_ref = right;
		_data.swap(right_ref._data);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_data.swap(right._data);
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_data = boost::forward<string_type>(right._data);
#else
		this_type& right_ref = right;
		_data.swap(right_ref._data);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		memcpy(&_data[0], &right._data[0], this_type::s_size());
		return *this;
	}

	string_type::value_type& operator[](std::size_t idx)
	{
		return _data[idx];
	}

	const string_type::value_type& operator[](std::size_t idx) const
	{
		return _data[idx];
	}

	template<typename Other>
	bool operator==(const Other& right) const
	{
		return 0 == memcmp(&_data[0], &right[0], this_type::E_length);
	}

	bool operator==(const this_type& right) const
	{
		return 0 == memcmp(&_data[0], &right[0], this_type::E_length);
	}

	template<typename Other>
	bool operator!=(const Other& right) const
	{
		return 0 != memcmp(&_data[0], &right[0], this_type::E_length);
	}

	bool operator!=(const this_type& right) const
	{
		return 0 != memcmp(&_data[0], &right[0], this_type::E_length);
	}

	template<typename Other>
	bool operator<(const Other& right) const
	{
		return std::lexicographical_compare(_data.begin(), _data.end(),
											right.begin(), right.end());
	}

	bool operator<(const this_type& right)
	{
		return std::lexicographical_compare(_data.begin(), _data.end(),
												right.begin(), right.end());
	}

	template<typename Other>
	bool operator<=(const Other& right) const
	{
		return !(right < *this);
	}

	bool operator<=(const this_type& right) const
	{
		return !(right < *this);
	}

	template<typename Other>
	bool operator>(const Other& right) const
	{
		return right < *this;
	}

	bool operator>(const this_type& right) const
	{
		return right < *this;
	}

	template<typename Other>
	bool operator >=(const Other& right) const
	{
		return !(*this < right);
	}

	bool operator>=(const this_type& right) const
	{
		return !(*this < right);
	}

	virtual std::size_t hash(void) const
	{
		//boost::hash<string_type> hasher;
		//return hasher(_data);
		assert(_data.size() == this_type::E_length);
		return boost::hash_range(_data.begin(), _data.end());
	}

public:
	inline static size_type s_size(void)
	{
		return this_type::E_length;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		typedef Archive archive_type;
		typedef YGGR_PP_ARCHIVE_NEED_BINARY_DATA_WRAP_TRAITS_T(archive_type) bool_type;
		typedef bool_type* bool_ptr_type;
		serialize_detail(ar, version, bool_ptr_type(0));
	}

	template<typename Archive>
	void serialize_detail(Archive& ar, const u32 version, boost::mpl::false_*) // other
	{
		ar & YGGR_SERIALIZE_NVP(_data);
	}

	template<typename Archive>
	void serialize_detail(Archive& ar, const u32 version, boost::mpl::true_*) // mongodb
	{
		typedef Archive archive_type;
		typedef typename YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS(archive_type) buffer_wrap_type;
		YGGR_SERIALIZE_WRAP_NVP_AR(ar, buffer_wrap_type, _data)
	}

protected:
	string_type _data;
};

template<yggr::u64 LEN>
class id_generator<id_n< LEN >, void>
	: private nonable::noncopyable
{
public:
	typedef id_n< LEN > id_type;

private:
	typedef typename id_type::string_type string_type;
	typedef yggr::u64 val_type;
	typedef val_type* val_ptr_type;
	enum
	{
		E_val_byte_len = sizeof(val_type),
		E_len = LEN,
		E_val_len = (LEN + E_val_byte_len - 1) / E_val_byte_len,
		E_alloc_len = E_val_len * E_val_byte_len,

		E_compile_u32 = 0xffffffff
	};

public:

	id_type operator()(void) const
	{
		typedef typename string_type::value_type char_type;
		id_generator<val_type> gen;

		char_type tmp[E_alloc_len] = {0};

		for(val_ptr_type i = reinterpret_cast<val_ptr_type>(&tmp[0]), isize = i + E_val_len;
			 i != isize; ++i)
		{
			(*i) = gen();
		}

		std::random_shuffle(&tmp[0], &tmp[0] + E_alloc_len);
		return id_type(&tmp[0], &tmp[0] + E_len);
	}
};


} // namespace ids
} // namespace yggr

#define _YGGR_TMP_PP_IDN_SWAP() \
	template<typename Other, yggr::u64 LEN> inline \
	void swap(Other& left, yggr::ids::id_n<LEN>& right) { \
		right.swap(left); } \
	\
	template<typename Other, yggr::u64 LEN> inline \
	void swap(yggr::ids::id_n<LEN>& left, Other& right) { \
		left.swap(right); } \
	\
	template<yggr::u64 LEN> inline \
	void swap(yggr::ids::id_n<LEN>& left, yggr::ids::id_n<LEN>& right) { \
		left.swap(right); } \
	\
	template<yggr::u64 LEN> inline \
	void swap(BOOST_RV_REF(yggr::ids::id_n<LEN>) left, yggr::ids::id_n<LEN>& right) { \
		right.swap(left); } \
	\
	template<yggr::u64 LEN> inline \
	void swap(yggr::ids::id_n<LEN>& left, BOOST_RV_REF(yggr::ids::id_n<LEN>) right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_IDN_SWAP()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_IDN_SWAP()
} // namespace boost

#undef _YGGR_TMP_PP_IDN_SWAP

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

#if defined(_MSC_VER)
#	pragma warning(pop) // Restore warnings to previous state.
#endif // _MSC_VER

#endif // __YGGR_IDS_ID_N_HPP__
