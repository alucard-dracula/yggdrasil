//blowfis_tool.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_BLOWFISH_TOOL_HPP__
#define __YGGR_ENCRYPTION_TOOL_BLOWFISH_TOOL_HPP__

#include <yggr/base/static_constant.hpp>
#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/base/random.h>

#include <yggr/move/move.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/limits.hpp>

#include <yggr/algorithm/shuffle.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/native_array_deleter.hpp>

#include <yggr/bytes/byte_size.hpp>
#include <yggr/container/get_allocator.hpp>
#include <yggr/container/resize.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/encryption_tool/tool_type_check.hpp>
#include <yggr/network/hn_conv.hpp>

#include <boost/mpl/bool.hpp>

#include <algorithm>
#include <cassert>
#include <utility>
#include <memory>

#include <Blowfish.h>
#include <blowfish_exception.hpp>

namespace yggr
{
namespace encryption_tool
{

template<u32 Key_Len = 8, u32 Mode = CBlowFish::ECB>
class blowfish_tool
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(Key_Len >= 8) >));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(Key_Len % 8 == 0)>));
public:
	typedef CBlowFish blowfish_type;
	typedef ::yggr::shared_ptr<blowfish_type> blowfish_ptr_type;

	typedef u8 value_type;
	typedef ::yggr::shared_ptr<value_type> key_type;

	YGGR_STATIC_CONSTANT(yggr::u32, E_Base_length = 8);

private:
	typedef smart_ptr_ex::native_array_deleter<value_type> key_deleter_type;

private:
	typedef blowfish_tool this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	blowfish_tool(void)
		: _key_size(Key_Len)
	{
		_key_size = (std::max)(_key_size, static_cast<u32>(E_Base_length));
		u32 miss = _key_size % E_Base_length;
		_key_size -= miss;
	}

	template<typename Key>
	blowfish_tool(const Key& key)
		: _key_size(Key_Len)
	{
		this_type::init(key);
	}

	blowfish_tool(BOOST_RV_REF(this_type) right)
		: _key_size(boost::move(right._key_size)),
			_key(boost::move(right._key)),
			_pblowfish(boost::move(right._pblowfish))
	{
	}

	blowfish_tool(const this_type& right)
		: _key_size(right._key_size), _pblowfish(right._pblowfish)
	{
		key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
		assert(key);
		_key.swap(key);

		if(_key_size) memcpy(&(*_key), &(*right._key), _key_size);
	}

	~blowfish_tool(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_key_size, boost::move(right._key_size));
		copy_or_move_or_swap(_key, boost::move(right._key));
		copy_or_move_or_swap(_pblowfish, boost::move(right._pblowfish));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_key_size = right._key_size;
		key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
		assert(key);
		_key.swap(key);

		if(_key_size) memcpy(_key.get(), right._key.get(), _key_size);
		_pblowfish = right._pblowfish;
		return *this;
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(_key_size, right._key_size);
		_key.swap(right._key);
		_pblowfish.swap(right._pblowfish);
	}

public:
	template<typename Oth_Key>
	void init(const Oth_Key& oth_key)
	{
		if(oth_key.size() != _key_size)
		{
			init();
			return;
		}

		key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
		assert(key);
		if(_key_size) memcpy(&(*key), &(oth_key[0]), _key_size);
		_key.swap(key);

		blowfish_ptr_type pbf = ::yggr::make_shared<blowfish_type>(_key.get(), _key_size);
		assert(pbf);
		_pblowfish.swap(pbf);
	}

	void init(void)
	{
		key_type key = construct_shared<value_type>(yggr_nothrow_new value_type[_key_size], key_deleter_type());
		assert(key);
		_key.swap(key);
		for(u32 i = 0; i != _key_size; ++i)
		{
			(_key.get())[i] = random::gen_random<u8>();
		}

		::yggr::shuffle(_key.get(), _key.get() + _key_size);

		blowfish_ptr_type pbf = ::yggr::make_shared<blowfish_type>(_key.get(), _key_size);
		assert(pbf);
		_pblowfish.swap(pbf);
	}

	template<typename Buffer> inline
	bool encrypt(Buffer& buf) const
	{
		return this_type::prv_encrypt(buf);
	}

	template<typename Buffer> inline
	bool decrypt(Buffer& buf) const
	{
		return this_type::prv_decrypt(buf);
	}

	inline std::pair<key_type, u32> get_key(void) const
	{
		return std::pair<const key_type, u32>(_key, _key_size);
	}

protected:
	template<typename Buffer>
	bool prv_encrypt(Buffer& buf) const
	{
		typedef Buffer org_buf_type;
		typedef typename org_buf_type::value_type org_buf_val_type;

		if(!_pblowfish || buf.empty())
		{
			return false;
		}

		std::size_t org_size = buf.size();
		std::size_t byte_size = org_size * sizeof(org_buf_val_type);
		std::size_t byte_miss = E_Base_length - (byte_size % E_Base_length);

		std::size_t buf_byte_size = byte_size + byte_miss + E_Base_length;
		std::size_t new_org_size = (buf_byte_size + sizeof(org_buf_val_type) - 1) / sizeof(org_buf_val_type);

		if(mplex::numeric_limits<u32>::max_type::value < new_org_size * sizeof(org_buf_val_type))
		{
			assert(false); // buffer limit failed
			return false;
		}

		std::size_t crypt_byte_size = (new_org_size * sizeof(org_buf_val_type) / E_Base_length) * E_Base_length;

		org_buf_type tbuf(new_org_size, 0, yggr::get_allocator(buf));

		u32 in_size = yggr::network::hton(static_cast<u32>(byte_size));
		memcpy(&tbuf[0], &in_size, sizeof(u32));
		memcpy((reinterpret_cast<value_type*>(&tbuf[0])) + E_Base_length, &buf[0], byte_size);

		try
		{
			_pblowfish->Encrypt((value_type*)&tbuf[0], static_cast<u32>(crypt_byte_size), Mode);
		}
		catch(const blowfish_exception& /*e*/)
		{
			return false;
		}

		buf.swap(tbuf);
		return true;
	}

	template<typename Buffer>
	bool prv_decrypt(Buffer& buf) const
	{
		typedef Buffer org_buf_type;
		typedef typename org_buf_type::value_type org_buf_val_type;

		assert(_pblowfish);

		if(!_pblowfish || bytes::byte_size(buf) < sizeof(u32))
		{
			return false;
		}

		org_buf_type tbuf(yggr::get_allocator(buf));
		tbuf.swap(buf);

		std::size_t org_size = tbuf.size();
		std::size_t use_byte_size = (org_size * sizeof(org_buf_val_type) / E_Base_length) * E_Base_length;
		if(!use_byte_size)
		{
			return false;
		}

		try
		{
			_pblowfish->Decrypt(reinterpret_cast<value_type*>(&tbuf[0]), static_cast<u32>(use_byte_size), Mode);
		}
		catch(const blowfish_exception& /*e*/)
		{
			return false;
		}

		u32 byte_size = yggr::network::ntoh(*(reinterpret_cast<u32*>(&tbuf[0])));

		if((!byte_size) || (byte_size > use_byte_size - E_Base_length))
		{
			return false;
		}

		memcpy(&(tbuf[0]), (reinterpret_cast<value_type*>(&tbuf[0])) + E_Base_length, byte_size);
		yggr::resize(tbuf, byte_size / sizeof(org_buf_val_type));
		buf.swap(tbuf);
		return true;
	}

private:
	u32 _key_size;
	key_type _key;
	blowfish_ptr_type _pblowfish;
};

template<u32 Key_Len, u32 Mode>
struct is_reductionable_tool< blowfish_tool<Key_Len, Mode> >
	: public boost::mpl::true_
{
};

} // encryption_tool
} // namespace yggr

namespace yggr
{
namespace encryption_tool
{
namespace swap_support
{

template<u32 Key_Len, u32 Mode> inline
void swap(blowfish_tool<Key_Len, Mode>& l, blowfish_tool<Key_Len, Mode>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace encryption_tool
} // namespace yggr

namespace std
{
	using ::yggr::encryption_tool::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::encryption_tool::swap_support::swap;
} // namespace boost

#endif //__YGGR_ENCRYPTION_TOOL_BLOWFISH_TOOL_HPP__
