file boost/archive/impl/basic_binary_iprimitive.ipp // line 118  #ifndef BOOST_NO_CWCHAR this should not be "load(char *)" and should be "load(wchar_t *)"
file boost/container/string.hpp // fix bug boost::container::string str = "abc"; str.replace(1, 1, str, 0, 3); assert(str != "aabcc") // not fix it yggr fixed

context msvc >= 1900 class execution_context less  BOOST_CONTEXT_DECL 