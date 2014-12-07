//charset_foo.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_FOO_HPP__
#define __YGGR_CHARSET_CHARSET_FOO_HPP__

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/assert.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/charset_converter.hpp>
#include <yggr/ppex/typedef.hpp>

//#include <utility>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

class charset_foo : private nonable::noncreateable
{
private:
	typedef charset_foo this_type;
public:

	template<typename String,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static typename boost::mpl::if_c<	boost::is_same<	String,
														Basic_String<Char, Traits, Alloc>
													>::value,
										const String&,
										String
									>::type
		sc_xchg(const Basic_String<Char, Traits, Alloc>& src)
	{
		typedef Basic_String<Char, Traits, Alloc> src_type;
		typedef typename src_type::value_type src_char_type;

		typedef String dst_type;
		typedef typename dst_type::value_type dst_char_type;

		BOOST_MPL_ASSERT((boost::is_same<src_char_type, dst_char_type>));
		struct same_call
		{
			const src_type& operator()(const src_type& src) const
			{
				return src;
			}
		};

		struct diff_call
		{
			dst_type operator()(const src_type& src) const
			{
				return dst_type(src.begin(), src.end());
			}
		};

		return boost::mpl::if_c<	boost::is_same< src_type, dst_type
													>::value,
									same_call,
									diff_call
								>::type()(src);
	}

private:
    template<typename TSrc, typename TDst, typename Nil_T = int>
    struct call_t
    {
        typedef TSrc src_type;
        typedef TDst dst_type;
        typedef typename src_type::value_type src_char_type;
        typedef typename dst_type::value_type dst_char_type;
        dst_type operator()(const src_type& src) const
        {
            size_type byte_size = src.size() * sizeof(src_char_type);
            size_type dst_size = (byte_size + sizeof(dst_char_type) - 1) / sizeof(dst_char_type);
            dst_type dst(dst_size, 0);
            memcpy(&dst[0], &src[0], byte_size);
            return dst;
        }
    };

    template<typename TSrc, typename Nil_T>
    struct call_t<TSrc, TSrc, Nil_T>
    {
        typedef TSrc src_type;
        const src_type& operator()(const src_type& src) const
        {
            return src;
        }
    };
public:
	// is dst charset char cref
	template<typename String,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static typename boost::mpl::if_c<	boost::is_same<	String,
														Basic_String<Char, Traits, Alloc>
													>::value,
										const String&,
										String
									>::type
		xchg(const Basic_String<Char, Traits, Alloc>& src)
	{
		typedef Basic_String<Char, Traits, Alloc> src_type;
		typedef typename src_type::value_type src_char_type;

		typedef String dst_type;
		typedef typename dst_type::value_type dst_char_type;

        typedef call_t<src_type, dst_type> call_type;
        call_type cal;
		return cal(src);
	}

	// is not dst charset wchar_t cref
	template<	typename String,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String
			>
	static String xchg(const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name, const string& dst_charset_name)
	{
		typedef String string_type;

		if(src.empty())
		{
			return string_type();
		}

		if(src_charset_name == dst_charset_name)
		{
			return this_type::xchg<string_type>(src);
		}

		charset_converter conv(src_charset_name, dst_charset_name);
		string_type dst;

		if(conv(src, dst))
		{
			return string_type(dst.c_str());
		}

		return string_type();
	}
};

} // namespace charset
} // namespace yggr

#define YGGR_PP_CHARSET_FOO_XCHG( __dst_type__, __src__, __src_charset_name__, __dst_charset_name__ ) \
		( yggr::charset::string( __src_charset_name__ ) != __dst_charset_name__ ? \
			yggr::charset::charset_foo::xchg< __dst_type__ >( __src__, __src_charset_name__, __dst_charset_name__ ) \
		: yggr::charset::charset_foo::xchg< __dst_type__ >( __src__ ) )

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_CHARSET_FOO_HPP__
