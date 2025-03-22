// non_mapping.hpp

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

#ifndef __YGGR_MAPPING_NON_MAPPING_HPP__
#define __YGGR_MAPPING_NON_MAPPING_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace mapping
{

struct non_mapping
{
private:
	typedef non_mapping this_type;

public:
	non_mapping(void)
	{
	}

	~non_mapping(void)
	{
	}

public:
	inline void clear(void)
	{
	}

	inline std::size_t size(void) const
	{
		return std::size_t();
	}

	inline std::size_t mapping_value_size(void) const
	{
		return std::size_t();
	}

	inline static std::size_t s_mapping_value_size(void)
	{
		return std::size_t();
	}

	template<typename Val> inline
	bool append(const Val& val)
	{
		return true;
	}

	template<typename Tag, typename Key> inline
	bool remove(const Key& key)
	{
		return true;
	}

	template<size_type i, typename Key> inline
	bool remove(const Key& key)
	{
		return true;
	}

	template<typename TagFind, typename TagMod, typename Key1, typename Key2> inline
	bool modify(const Key1& find_key, const Key2& mod_key)
	{
		return true;
	}

	template<size_type find, size_type mod, typename Key1, typename Key2> inline
	bool modify(const Key1& find_key, const Key2& mod_key)
	{
		return true;
	}

	template<typename TagSrc, typename TagDst, typename Key> inline
	std::pair<Key, bool>
		mapping_value(const typename Key& key) const
	{
		return std::pair<Key, bool>(key, true);
	}

	template<size_type src, size_type dst, typename Key> inline
	std::pair<Key, bool>
		mapping_value(const typename Key& key) const
	{
		return std::pair<Key, bool>(key, true);
	}

public:
	template<size_type i> inline
	static s32 create_tag_object(void)
	{
		return static_cast<s32>(0);
	}
};

} // namespace mapping
} // namespace yggr

#endif // __YGGR_MAPPING_NON_MAPPING_HPP__