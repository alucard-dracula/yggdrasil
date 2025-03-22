//blowfish_exception.cpp

#include "blowfish_exception.hpp"

#if defined (__GNUC__)

blowfish_exception::blowfish_exception(void) throw()
	: _code(0), _msg("success")
{
}

blowfish_exception::blowfish_exception(const char* const &msg)
	: _code(0xffffffff), _msg(msg)
{
}

blowfish_exception::blowfish_exception(const char* const &msg, unsigned long code)
	: _code(code), _msg(msg)
{
}

blowfish_exception::blowfish_exception(const blowfish_exception& right)
	: _code(right._code), _msg(right._msg)
{
}

blowfish_exception::~blowfish_exception() throw()
{
}

blowfish_exception& blowfish_exception::operator=(const blowfish_exception& right)
{
	_code = right._code;
	_msg = right._msg;

	return *this;
}

const unsigned long& blowfish_exception::code() const throw()
{
    return _code;
}
const char* blowfish_exception::what(void) const throw()
{
	//return _msg;
	return _msg.c_str();
}

#else
#   if defined(_MSC_VER)
#   endif // _MSC_VER
#endif // __GNUC__
