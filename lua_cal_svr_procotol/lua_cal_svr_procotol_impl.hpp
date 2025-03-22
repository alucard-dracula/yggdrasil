//lua_cal_svr_procotol_impl.hpp

#include <iostream>
#include "lua_cal_svr_procotol.hpp"

#ifndef __CAL_SVR_PROCOTOL_IMPL_HPP__
#define __CAL_SVR_PROCOTOL_IMPL_HPP__

typedef cdt_procotol<> cdt_procotol_type;
typedef rst_procotol<> rst_procotol_type;

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const cdt_procotol_type& cdt)
{
	os << "number1 = " << cdt.num1 << ", number2 = " << cdt.num2;
	return os;
}

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const rst_procotol_type& rst)
{
	os << "rst = " << rst.rst;
	return os;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Traits> class Basic_OStream>
//Basic_OStream<Char, Traits>& 
//	operator<<(Basic_OStream<Char, Traits>& os, const cdt_procotol_type& cdt)
//{
//	os << "number1 = " << cdt.num1 << ", number2 = " << cdt.num2;
//	return os;
//}
//
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Traits> class Basic_OStream>
//Basic_OStream<Char, Traits>& 
//	operator<<(Basic_OStream<Char, Traits>& os, const rst_procotol_type& rst)
//{
//	os << "rst = " << rst.rst;
//	return os;
//}

#endif //__CAL_SVR_PROCOTOL_IMPL_HPP__
