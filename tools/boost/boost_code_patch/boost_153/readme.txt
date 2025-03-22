file boost/interprocess/detail/win32_api.hpp  fix before boost153 at win8 use msvc100 wchar_variant size fixed foo 'get_wmi_class_attribute' 
file boost/proto/transform/env.hpp fix line 448 _env_var append base_type and replace imp::data to base_type::data
file boost/archive/impl/basic_binary_iprimitive.ipp // line 118  #ifndef BOOST_NO_CWCHAR this should not be "load(char *)" and should be "load(wchar_t *)"
file boost/container/string.hpp // fix bug boost::container::string str = "abc"; str.replace(1, 1, str, 0, 3); assert(str != "aabcc") // not fix it yggr fixed