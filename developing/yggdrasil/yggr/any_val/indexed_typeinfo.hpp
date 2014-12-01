//indexed_typeinfo.hpp

#ifndef __YGGR_ANY_VAL_INDEXED_TYPEINFO_HPP__
#define __YGGR_ANY_VAL_INDEXED_TYPEINFO_HPP__

#include <boost/mpl/list.hpp>
#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace any_val
{

//struct unknow_t_ {};
//
//struct index_t
//{
//	enum
//	{
//		E_unknow = 0,
//		E_void,
//		E_char,
//		E_wchar_t,
//		E_s8,
//		E_u8
//		E_s16,
//		E_u16,
//		E_s32,
//		E_u32,
//		E_s64,
//		E_u64,
//		E_f32,
//		E_f64,
//		E_base_t_end,
//		E_user_t,
//
//		E_compile_u32 = 0xffffffff
//	};
//};
//
//template<typename T = unknow_t_ >
//struct t_to_num
//{
//	enum
//	{
//		value = index_t::E_unknow,
//		E_compile_u32 = 0xffffffff
//	};
//
//	typedef T type;
//};
//
//template<yggr::u32 index = 0>
//struct num_to_t
//{
//	enum
//	{
//		value = index,
//		E_compile_u32 = 0xffffffff
//	};
//
//	typedef unknow_t_ type;
//}

} // namespace any_val
} // namespace yggr

#endif // __YGGR_ANY_VAL_INDEXED_TYPEINFO_HPP__