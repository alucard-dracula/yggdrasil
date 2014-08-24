//lib_compatibility.cpp

#include <compatibility/stl_exception.hpp>

namespace compatibility
{

#if defined (__GNUC__)
stl_exception::stl_exception(void) throw()
	: _code(0), _msg("success")
{
}

stl_exception::stl_exception(const char* const &msg)
	: _code(0xffffffff), _msg(msg)
{
}

stl_exception::stl_exception(const char* const &msg, unsigned long code)
	: _code(code), _msg(msg)
{
}

stl_exception::stl_exception(const stl_exception& right)
	: _code(right._code), _msg(right._msg)
{
}

stl_exception::~stl_exception() throw()
{
}

stl_exception& stl_exception::operator=(const stl_exception& right)
{
	_code = right._code;
	_msg = right._msg;

	return *this;
}

const char* stl_exception::what(void) const throw()
{
	//return _msg;
	return _msg.c_str();
}

#else

#endif // __GNUC__

} // namespace compatibility
