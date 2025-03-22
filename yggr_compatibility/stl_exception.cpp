//lib_compatibility.cpp

#include <yggr/compatibility/stl_exception.hpp>
#include <boost/move/move.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace compatibility
{

#if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

stl_exception::stl_exception(void) _GLIBCXX_USE_NOEXCEPT
	: _code(0), _msg("success")
{
}

stl_exception::stl_exception(const char* msg) _GLIBCXX_USE_NOEXCEPT
	: _code(0xffffffff), _msg(msg)
{
}

stl_exception::stl_exception(const char* msg, unsigned long code) _GLIBCXX_USE_NOEXCEPT
	: _code(code), _msg(msg)
{
}

stl_exception::stl_exception(const std::string& msg) _GLIBCXX_USE_NOEXCEPT
	: _code(0xffffffff), _msg(msg)
{
}

stl_exception::stl_exception(const std::string& msg, unsigned long code) _GLIBCXX_USE_NOEXCEPT
	: _code(code), _msg(msg)
{
}


#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

stl_exception::stl_exception(this_type&& right) _GLIBCXX_USE_NOEXCEPT
	: _code(right._code), _msg(boost::move(right._msg))
{
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

stl_exception::stl_exception(const this_type& right) _GLIBCXX_USE_NOEXCEPT
	: _code(right._code), _msg(right._msg)
{
}

stl_exception::~stl_exception() _GLIBCXX_USE_NOEXCEPT
{
}

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

stl_exception::this_type&
	stl_exception::operator=(this_type&& right) _GLIBCXX_USE_NOEXCEPT
{
	this_type& right_chk = right;
	if(this == &right_chk)
	{
		return *this;
	}

	_code = boost::move(right._code);
	_msg = boost::move(right._msg);

	return *this;
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

stl_exception::this_type&
	stl_exception::operator=(const this_type& right) _GLIBCXX_USE_NOEXCEPT
{
	if(this == &right)
	{
		return *this;
	}

	_code = right._code;
	_msg = right._msg;

	return *this;
}

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

void stl_exception::swap(this_type&& right) _GLIBCXX_USE_NOEXCEPT
{
	this_type& right_ref = right;
	this_type::swap(right_ref);
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

void stl_exception::swap(this_type& right) _GLIBCXX_USE_NOEXCEPT
{
	if(this == &right)
	{
		return;
	}

	std::swap(_code, right._code);
	_msg.swap(right._msg);
}

const char* stl_exception::what(void) const _GLIBCXX_USE_NOEXCEPT
{
	//return _msg;
	return _msg.data();
}

#else

#endif // #if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

} // namespace compatibility
} // namespace yggr

