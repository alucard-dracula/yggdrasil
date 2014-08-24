//blowfis_tool.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_BLOWFISH_TOOL_HPP__
#define __YGGR_ENCRYPTION_TOOL_BLOWFISH_TOOL_HPP__

#include <Blowfish.h>
#include <blowfish_exception.hpp>

#include <algorithm>
#include <cassert>
#include <utility>
#include <memory>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/random.h>
#include <yggr/base/exception.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/encryption_tool/tool_type_check.hpp>
#include <yggr/network/hn_conv.hpp>


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
	typedef boost::shared_ptr<blowfish_type> blowfish_ptr_type;


	typedef u8 val_type;
	typedef boost::shared_ptr<val_type> key_type;

	enum
	{
		E_Base_length = 8,
		E_compile_u32 = 0xffffffff
	};

private:
	typedef blowfish_tool this_type;

public:
	blowfish_tool(void)
		: _key_size(Key_Len)
	{
		if(_key_size < E_Base_length)
		{
			_key_size = E_Base_length;
		}
		u32 miss = _key_size % E_Base_length;
		_key_size -= miss;
	}

	template<typename Key>
	blowfish_tool(const Key& key)
		: _key_size(Key_Len)
	{
		this_type::init(key);
	}

	blowfish_tool(const this_type& right)
		: _key_size(right._key_size), _pblowfish(right._pblowfish)
	{
		key_type key(new val_type[_key_size]);
		_key.swap(key);

		memcpy(&(*_key), &(*right._key), _key_size);
	}

	~blowfish_tool(void)
	{
	}

	blowfish_tool& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_key_size = right._key_size;
		key_type key(new val_type[_key_size]);
		_key.swap(key);

		memcpy(_key.get(), right._key.get(), _key_size);
		_pblowfish = right._pblowfish;
	}

	template<typename Oth_Key>
	void init(const Oth_Key& oth_key)
	{
		if(oth_key.size() != _key_size)
		{
			init();
			return;
		}

		key_type key(new val_type[_key_size]);
		memcpy(&(*key), &(oth_key[0]), _key_size);
		_key.swap(key);

		blowfish_ptr_type pbf(new blowfish_type(_key.get(), _key_size));
		_pblowfish.swap(pbf);
	}

	void init(void)
	{
		key_type key(new val_type[_key_size]);
		_key.swap(key);
		for(u32 i = 0; i != _key_size; ++i)
		{
			(_key.get())[i] = random::gen_random<u8>();
		}

		std::random_shuffle(_key.get(), _key.get() + _key_size);

		blowfish_ptr_type pbf(new blowfish_type(_key.get(), _key_size));
		_pblowfish.swap(pbf);
	}

	template<typename Buffer>
	bool encrypt(Buffer& buf) const
	{
		return prv_encrypt(buf);
	}

	template<typename Buffer>
	bool decrypt(Buffer& buf) const
	{
		return prv_decrypt(buf);
	}

	std::pair<key_type, u32> get_key(void) const
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

		u32 org_size = buf.size();
		u32 byte_size = org_size * sizeof(org_buf_val_type);
		//u32 byte_miss = E_Base_length - (E_Base_length - (byte_size % E_Base_length));
		u32 byte_miss = E_Base_length - (byte_size % E_Base_length);

		u32 buf_byte_size = byte_size + byte_miss + E_Base_length;
		u32 new_org_size = (buf_byte_size + sizeof(org_buf_val_type) - 1) / sizeof(org_buf_val_type);

		org_buf_type tbuf(new_org_size, 0);

		u32 crypt_byte_size = (new_org_size * sizeof(org_buf_val_type) / E_Base_length) * E_Base_length;

		u32 in_size = yggr::network::hton(byte_size);
		memcpy(&tbuf[0], &in_size, sizeof(u32));
		memcpy(((val_type*)&tbuf[0]) + E_Base_length, &buf[0], byte_size);

		try
		{
			_pblowfish->Encrypt((val_type*)&tbuf[0], crypt_byte_size, Mode);
		}
		catch(const blowfish_exception& e)
		{
			exception::exception::throw_error(e);
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

		if(!_pblowfish || buf.empty())
		{
			return false;
		}

		org_buf_type tbuf;
		tbuf.swap(buf);

		u32 org_size = tbuf.size();
		u32 use_byte_size = (org_size * sizeof(org_buf_val_type) / E_Base_length) * E_Base_length;
		if(!use_byte_size)
		{
			return false;
		}

		try
		{
			_pblowfish->Decrypt((val_type*)&tbuf[0], use_byte_size, Mode);
		}
		catch(const blowfish_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		u32 in_size = 0;
		memcpy(&in_size, &tbuf[0], sizeof(u32));

		u32 byte_size = yggr::network::ntoh(in_size);

		if((!byte_size) || (byte_size > use_byte_size - E_Base_length))
		{
			return false;
		}

		buf.clear();
		buf.resize(byte_size / sizeof(org_buf_val_type));
		memcpy(&(buf[0]), ((val_type*)&tbuf[0]) + E_Base_length, byte_size);

		return true;
	}

private:

	u32 _key_size;
	key_type _key;
	blowfish_ptr_type _pblowfish;
};

template<u32 Key_Len, u32 Mode>
struct is_reduction_tool< blowfish_tool<Key_Len, Mode> >
{
	typedef boost::mpl::true_ type;
};

} // encryption_tool
} // namespace yggr

#endif //__YGGR_ENCRYPTION_TOOL_BLOWFISH_TOOL_HPP__
