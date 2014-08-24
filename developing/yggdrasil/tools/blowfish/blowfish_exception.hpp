//blowfish_exception.hpp

#ifndef __BLOWFISH_EXCEPTION_HPP__
#define __BLOWFISH_EXCEPTION_HPP__

#include <exception>
#include <string>

#if defined (__GNUC__)
	class blowfish_exception
		: public virtual std::exception
	{
    public:
		blowfish_exception(void) throw();

		blowfish_exception(const char* const &msg);

		blowfish_exception(const char* const &msg, unsigned long code);

		blowfish_exception(const blowfish_exception& right);

		virtual ~blowfish_exception() throw();

		blowfish_exception& operator=(const blowfish_exception& right);

        const unsigned long& code() const throw();
		virtual const char* what(void) const throw();
	private:
		//const char* _msg;
		unsigned long _code;
		std::string _msg;
	};
#else
#   if defined(_MSC_VER)
	typedef std::exception blowfish_exception;
#endif // _MSC_VER
#endif // __GNUC__

#endif // __BLOWFISH_EXCEPTION_HPP__
