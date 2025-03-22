// tuple_io.hpp

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

#ifndef __YGGR_TUPLE_TUPLE_IO_HPP__
#define __YGGR_TUPLE_TUPLE_IO_HPP__

#include <yggr/tuple/tuple.hpp>
#include <yggr/move/move.hpp>

#include <boost/tuple/tuple_io.hpp>

namespace yggr
{
namespace tuples
{

namespace detail
{

using boost::tuples::detail::format_info;

} //namespace detail


using boost::tuples::tuple_manipulator;
using boost::tuples::operator<<;

using boost::tuples::set_open;
using boost::tuples::set_close;
using boost::tuples::set_delimiter;



// -------------------------------------------------------------
// printing tuples to ostream in format (a b c)
// parentheses and space are defaults, but can be overriden with manipulators
// set_open, set_close and set_delimiter

namespace detail
{

// Note: The order of the print functions is critical
// to let a conforming compiler  find and select the correct one.

#if defined (BOOST_NO_TEMPLATED_STREAMS)

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template<class T1> inline
std::ostream& print(std::ostream& o, const cons<T1, null_type>& t)
{
	return o << t.head;
}

#endif // BOOST_NO_TEMPLATED_STREAMS

inline std::ostream& print(std::ostream& o, const null_type&)
{
	return o;
}

template<typename T1, typename T2> inline
std::ostream& print(std::ostream& o, const cons<T1, T2>& t)
{
	const char d = format_info::get_manipulator(o, format_info::delimiter);

	o << t.head;

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	YGGR_IF_CONSTEXPR(tuples::length<T2>::value == 0)
		return o;
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	o << d;

	return print(o, t.tail );
}

#else

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template<typename CharType, typename CharTrait, typename T1> inline
std::basic_ostream<CharType, CharTrait>&
	print(std::basic_ostream<CharType, CharTrait>& o, const cons<T1, null_type>& t)
{
	return o << t.head;
}

#endif  // !BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


template<typename CharType, typename CharTrait> inline
std::basic_ostream<CharType, CharTrait>&
	print(std::basic_ostream<CharType, CharTrait>& o, const null_type&)
{
	return o;
}

template<typename CharType, typename CharTrait, typename T1, typename T2> inline
std::basic_ostream<CharType, CharTrait>&
	print(std::basic_ostream<CharType, CharTrait>& o, const cons<T1, T2>& t)
{
	const CharType d = format_info::get_manipulator(o, format_info::delimiter);

	o << t.head;

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	YGGR_IF_CONSTEXPR(tuples::length<T2>::value == 0)
		return o;
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	o << d;

	return print(o, t.tail);
}

#endif  // BOOST_NO_TEMPLATED_STREAMS

using boost::tuples::detail::handle_width;

} // namespace detail

#if defined (BOOST_NO_TEMPLATED_STREAMS)

template<typename T1, typename T2> inline
std::ostream& operator<<(std::ostream& o,
							const cons<T1, T2>& t)
{
	if(!o.good())
	{
		return o;
	}

	if(detail::handle_width(o, t))
	{
		return o;
	}

	const char l =
		detail::format_info::get_manipulator(o, detail::format_info::open);
	const char r =
		detail::format_info::get_manipulator(o, detail::format_info::close);

	o << l;

	detail::print(o, t);

	o << r;

	return o;
}

template<typename T1, typename T2> inline
std::ostream& operator<<(std::ostream& o,
							BOOST_RV_REF_BEG cons<T1, T2> BOOST_RV_REF_END t)
{
	const cons<T1, T2>& right_cref = t;
	return yggr::tupls::operator<<(o, right_cref);
}

#else

template<typename CharType, typename CharTrait, typename T1, typename T2> inline
std::basic_ostream<CharType, CharTrait>&
	operator<<(std::basic_ostream<CharType, CharTrait>& o,
					const cons<T1, T2>& t)
{
	if(!o.good())
	{
		return o;
	}

	if(detail::handle_width(o, t))
	{
		return o;
	}

	const CharType l =
		detail::format_info::get_manipulator(o, detail::format_info::open);
	const CharType r =
		detail::format_info::get_manipulator(o, detail::format_info::close);

	o << l;

	detail::print(o, t);

	o << r;

	return o;
}

template<typename CharType, typename CharTrait, typename T1, typename T2> inline
std::basic_ostream<CharType, CharTrait>&
	operator<<(std::basic_ostream<CharType, CharTrait>& o,
					BOOST_RV_REF_BEG cons<T1, T2> BOOST_RV_REF_END t)
{
	const cons<T1, T2>& right_cref = t;
	return yggr::tuples::operator<<(o, right_cref);
}

#endif // BOOST_NO_TEMPLATED_STREAMS

// -------------------------------------------------------------
// input stream operators

namespace detail
{

using boost::tuples::detail::extract_and_check_delimiter;

#if defined (BOOST_NO_TEMPLATED_STREAMS)

// Note: The order of the read functions is critical to let a
// (conforming?) compiler find and select the correct one.

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
template<class T1> inline
std::istream& read(std::istream &is, cons<T1, null_type>& t1)
{
	if(!is.good())
	{
		return is;
	}
	return is >> t1.head ;
}

#else

inline std::istream& read(std::istream& i, const null_type&)
{
	return i;
}

#endif // !BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<typename T1, typename T2>

inline std::istream& read(std::istream &is, cons<T1, T2>& t1)
{

	if(!is.good()) return is;

	is >> t1.head;

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	YGGR_IF_CONSTEXPR(tuples::length<T2>::value == 0)
		return is;
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	extract_and_check_delimiter(is, format_info::delimiter);

	return read(is, t1.tail);
}

} // namespace detail

inline std::istream& operator>>(std::istream &is, null_type&)
{
	if(!is.good() )
	{
		return is;
	}

	detail::extract_and_check_delimiter(is, detail::format_info::open);
	detail::extract_and_check_delimiter(is, detail::format_info::close);

	return is;
}

template<typename T1, typename T2> inline
std::istream& operator>>(std::istream& is, cons<T1, T2>& t1)
{
	if(!is.good() )
	{
		return is;
	}

	detail::extract_and_check_delimiter(is, detail::format_info::open);

	detail::read(is, t1);

	detail::extract_and_check_delimiter(is, detail::format_info::close);

	return is;
}

#else

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
template<typename CharType, typename CharTrait, typename T1> inline
std::basic_istream<CharType, CharTrait>&
	read(std::basic_istream<CharType, CharTrait> &is, cons<T1, null_type>& t1)
{

	if(!is.good())
	{
		return is;
	}

	return is >> t1.head;
}
#else
template<typename CharType, typename CharTrait>
inline std::basic_istream<CharType, CharTrait>&
read(std::basic_istream<CharType, CharTrait>& i, const null_type&) { return i; }

#endif // !BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<typename CharType, typename CharTrait, typename T1, typename T2>
inline std::basic_istream<CharType, CharTrait>&
read(std::basic_istream<CharType, CharTrait> &is, cons<T1, T2>& t1) {

	if(!is.good()) return is;

	is >> t1.head;

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	YGGR_IF_CONSTEXPR(tuples::length<T2>::value == 0)
		return is;
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	extract_and_check_delimiter(is, format_info::delimiter);

	return read(is, t1.tail);
}

} // namespace detail


template<typename CharType, typename CharTrait>
inline std::basic_istream<CharType, CharTrait>&
operator>>(std::basic_istream<CharType, CharTrait> &is, null_type&) {

	if(!is.good() ) return is;

	detail::extract_and_check_delimiter(is, detail::format_info::open);
	detail::extract_and_check_delimiter(is, detail::format_info::close);

	return is;
}

template<typename CharType, typename CharTrait, typename T1, typename T2> inline
std::basic_istream<CharType, CharTrait>&
	operator>>(std::basic_istream<CharType, CharTrait>& is, cons<T1, T2>& t1)
{
	if(!is.good())
	{
		return is;
	}

	detail::extract_and_check_delimiter(is, detail::format_info::open);

	detail::read(is, t1);

	detail::extract_and_check_delimiter(is, detail::format_info::close);

	return is;
}

#endif // BOOST_NO_TEMPLATED_STREAMS

} // namespace tuples
} // namespace yggr

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{
namespace tuples
{
namespace std_detail
{

// output stream helper
template<std::size_t I, std::size_t ISize>
struct tail_printer
{
	template<typename Char, typename Traits, typename T> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, const T& tail) const
	{
		os << tail;
		return os;
	}
};

template<std::size_t ISize>
struct tail_printer<ISize, ISize>
{
	template<typename Char, typename Traits, typename T> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, const T& tail) const
	{
		return os;
	}
};

template<std::size_t I, std::size_t ISize>
struct printer
{
	template<typename Char, typename Traits, typename Tuple> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, const Tuple& val) const
	{
		typedef printer<I + 1, ISize> next_printer_type;
		typedef tail_printer<I, ISize - 1> tail_printer_type;

		const Char d = detail::format_info::get_manipulator(os, detail::format_info::delimiter);

		os << ::yggr::get<I>(val);

		tail_printer_type tail_printer;

		tail_printer(os, d);

		next_printer_type next_printer;

		return next_printer(os, val);
	}
};

template<std::size_t ISize>
struct printer<ISize, ISize>
{
	template<typename Char, typename Traits, typename Tuple> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, const Tuple&) const
	{
		return os;
	}
};

// input stream helper

template<std::size_t I, std::size_t ISize>
struct reader
{
	template<typename Char, typename Traits, typename Tuple> inline
	std::basic_istream<Char, Traits>&
		operator()(std::basic_istream<Char, Traits>& is, Tuple& val) const
	{
		typedef reader<I + 1, ISize> next_reader_type;

		if(!is.good()) return is;

		is >> (::yggr::get<I>(val));

		detail::extract_and_check_delimiter(is, detail::format_info::delimiter);
		
		next_reader_type next_read;
		return next_read(is, val);
	}
};

template<std::size_t ISize>
struct reader<ISize, ISize>
{
	template<typename Char, typename Traits, typename Tuple> inline
	std::basic_istream<Char, Traits>&
		operator()(std::basic_istream<Char, Traits>& is, Tuple&) const
	{
		return is;
	}
};

} // namespace std_detail
} // namespace tuples
} // namespace yggr

namespace std
{


template<typename Char, typename Traits, typename ...Args> inline
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os,
			   const std::tuple<Args...>& val)
{
	typedef std::tuple<Args...> now_tuple_type;
	typedef yggr::tuples::std_detail::printer<0, ::yggr::tuple_size<now_tuple_type>::value> printer_type;
	if(!os.good())
	{
		return os;
	}

	if(yggr::tuples::detail::handle_width(os, val))
	{
		return os;
	}

	const Char l =
		yggr::tuples::detail::format_info::get_manipulator(os, yggr::tuples::detail::format_info::open);
	const Char r =
		yggr::tuples::detail::format_info::get_manipulator(os, yggr::tuples::detail::format_info::close);

	os << l;

	printer_type printer;
	printer(os, val);

	os << r;

	return os;
}


template<typename Char, typename Traits, typename ...Args> inline
std::basic_istream<Char, Traits>& 
	operator>>(std::basic_istream<Char, Traits>& is,
				std::tuple<Args...>& val)
{
	typedef std::tuple<Args...> now_tuple_type;
	typedef yggr::tuples::std_detail::reader<0, ::yggr::tuple_size<now_tuple_type>::value> reader_type;

	if(!is.good())
	{
		return is;
	}

	yggr::tuples::detail::extract_and_check_delimiter(is, yggr::tuples::detail::format_info::open);

	reader_type reader;
	reader(is, val);

	yggr::tuples::detail::extract_and_check_delimiter(is, yggr::tuples::detail::format_info::close);

	return is;
}

} // namespace std

#endif // YGGR_NO_CXX11_HDR_TUPLE

#endif // __YGGR_TUPLE_TUPLE_IO_HPP__
