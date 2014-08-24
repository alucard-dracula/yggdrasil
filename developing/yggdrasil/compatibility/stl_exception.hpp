//stl_exception.hpp

#ifndef __COMPATIBILITY_STL_EXCEPTION_HPP__
#define __COMPATIBILITY_STL_EXCEPTION_HPP__

#include <exception>
#include <string>
namespace compatibility
{
#if defined (__GNUC__)
	class stl_exception
		: public std::exception
	{
    public:
		stl_exception(void) throw();

		stl_exception(const char* const &msg);

		stl_exception(const char* const &msg, unsigned long code);

		stl_exception(const stl_exception& right);

		~stl_exception() throw();

		stl_exception& operator=(const stl_exception& right);

		const char* what(void) const throw();
	private:
		//const char* _msg;
		unsigned long _code;
		std::string _msg;
	};
    //typedef std::exception stl_exception;
#else
#   if defined(_MSC_VER)
	typedef std::exception stl_exception;
#endif // _MSC_VER
#endif // __GNUC__

} // namespace compatibility

#endif // __COMPATIBILITY_STL_EXCEPTION_HPP__
