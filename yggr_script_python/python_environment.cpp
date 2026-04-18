// python_environment.cpp

#include <yggr/base/yggrdef.h>

#if !((defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
		|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG))

#include <yggr/script/python_environment.hpp>

namespace yggr
{
namespace script
{
namespace python
{

namespace detail
{

/*static*/ char failed_dict::dict_buff[sizeof(boost::python::dict)] = {0};

} // namespace detail

/*static*/python_environment::py_dict_ptr_type python_environment::_s_pglobal;

} // nemspace python
} // namespace script
} // namespace yggr

#endif // YGGR_MSVC_USING_MTD_FLAG