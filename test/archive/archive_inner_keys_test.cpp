// archive_inner_keys_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/archive/archive_inner_keys.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	// output therse strings
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_obj_id)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_obj_id_ref)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_ver)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_library_ver)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_class_id)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_class_id_ref)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_class_name)
	//YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_tracking)

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_obj_id) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_obj_id) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_obj_id) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_obj_id) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_obj_id_ref) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_obj_id_ref) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_obj_id_ref) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_obj_id_ref) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_ver) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_ver) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_ver) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_ver) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_library_ver) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_library_ver) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_library_ver) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_library_ver) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_class_id) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_class_id) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_class_id) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_class_id) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_class_id_ref) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_class_id_ref) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_class_id_ref) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_class_id_ref) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_class_name) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_class_name) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_class_name) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_class_name) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_tracking) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(std::string, boost_ar_tracking) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(boost::container::string, boost_ar_tracking) << std::endl;
	std::cout << YGGR_ARCHIVE_GET_INNER_KEY(yggr::utf8_string, boost_ar_tracking) << std::endl;

	std::cout << "-------------------------------------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}