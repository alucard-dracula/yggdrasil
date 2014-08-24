// non_mapping.hpp

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

	template<typename Val>
	inline bool append(const Val& val)
	{
		return true;
	}

	template<typename Tag, typename Key>
	inline bool remove(const Key& key)
	{
		return true;
	}

	template<size_type i, typename Key>
	inline bool remove(const Key& key)
	{
		return true;
	}

	template<typename TagFind, typename TagMod, typename Key1, typename Key2>
	inline bool modify(const Key1& find_key, const Key2& mod_key)
	{
		return true;
	}

	template<size_type find, size_type mod, typename Key1, typename Key2>
	inline bool modify(const Key1& find_key, const Key2& mod_key)
	{
		return true;
	}

	template<typename TagSrc, typename TagDst, typename Key>
	inline std::pair<Key, bool> 
			mapping_value(const typename Key& key) const
	{
		return std::pair<Key, bool>(key, true);
	}

	template<size_type src, size_type dst, typename Key>
	inline std::pair<Key, bool> 
			mapping_value(const typename Key& key) const
	{
		return std::pair<Key, bool>(key, true);
	}

public:
	template<size_type i>
	inline static s32 s_create_tag_object(void)
	{
		return s32();
	}
};

} // namespace mapping
} // namespace yggr

#endif // __YGGR_MAPPING_NON_MAPPING_HPP__