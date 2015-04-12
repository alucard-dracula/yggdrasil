// curl_https_test
//

#define CURL_STATICLIB
#define USE_SSLEAY
#define USE_OPENSSL

#if defined(_MSC_VER)
#   if defined(_DEBUG)
#	    pragma comment(lib, "libcurld.lib")
#	    pragma comment(lib, "ssleay32.lib")
#	    pragma comment(lib, "libeay32.lib")
#	    pragma comment(lib, "ws2_32.lib")
#	    pragma comment(lib, "wldap32.lib")
#	    pragma comment(lib, "libyggr_base-vc100-d.lib")
#	    pragma comment(lib, "libyggr_time-vc100-d.lib")
#	    pragma comment(lib, "libyggr_ids-vc100-d.lib")
//#	pragma comment(lib, "libyggr_file_system-vc100-d.lib")
#   else
#	    pragma comment(lib, "libcurl.lib")
#	    pragma comment(lib, "ssleay32.lib")
#	    pragma comment(lib, "libeay32.lib")
#	    pragma comment(lib, "ws2_32.lib")
#	    pragma comment(lib, "wldap32.lib")
#	    pragma comment(lib, "libyggr_base-vc100.lib")
#	    pragma comment(lib, "libyggr_time-vc100.lib")
#	    pragma comment(lib, "libyggr_ids-vc100.lib")
//#	pragma comment(lib, "libyggr_file_system-vc100.lib")
#   endif // _DEBUG
#endif // _MSC_VER

#include <cassert>
#include <iostream>
#include <string>
#include <curl/curl.h>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

//std::size_t process_data(void *buffer, std::size_t size, std::size_t nmemb, void *user_p)
//{
//	FILE *fp = (FILE *)user_p;
//	size_t return_size = fwrite(buffer, size, nmemb, fp);
//	std::cout << (char *)buffer << std::endl;
//
//	std::cout << "call back end" << std::endl;
//	return return_size;
//}
//
//void test_http1(void)
//{
//	std::cout << "test of " << curl_version() << std::endl;
//	/*
//	CURL_GLOBAL_DEFAULT
//	CURL_GLOBAL_ALL                //初始化所有的可能的调用。
//	CURL_GLOBAL_SSL               //初始化支持 安全套接字层。
//	CURL_GLOBAL_WIN32            //初始化win32套接字库。
//	CURL_GLOBAL_NOTHING         //没有额外的初始化。
//	*/
//
//	CURLcode ret_code = curl_global_init(CURL_GLOBAL_ALL);
//	if(ret_code)
//	{
//		std::cout << "error_code = " << ret_code << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//	CURL* pcurl = curl_easy_init();
//	if(!pcurl)
//	{
//		std::cout << "init failed" << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//
//	//std::string url("http://blog.csdn.net/JGood");
//	std::string url("http://127.0.0.1");
//
//	FILE *fp = fopen("data.html", "wb");
//	// 设置easy handle属性
//	curl_easy_setopt(pcurl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &process_data);
//	curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, fp);
//
//	// 执行数据请求
//	curl_easy_perform(pcurl);
//
//
//	curl_easy_cleanup(pcurl);
//	fclose(fp);
//
//	curl_global_cleanup();
//
//	std::cout << "run end" << std::endl;
//
//	return;
//}

#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION

//void test_https1(void)
//{
//	std::cout << "test of " << curl_version() << std::endl;
//	CURLcode ret_code = curl_global_init(CURL_GLOBAL_ALL);
//	if(ret_code)
//	{
//		std::cout << "error_code = " << ret_code << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//	CURL* pcurl = curl_easy_init();
//	if(!pcurl)
//	{
//		std::cout << "init failed" << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//
//	std::string url("https://127.0.0.1");
//
//	FILE *fp = fopen("data_https.html", "wb");
//	// 设置easy handle属性
//	curl_easy_setopt(pcurl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &process_data);
//	curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, fp);
//
//	// 执行数据请求
//
//#ifdef SKIP_PEER_VERIFICATION
//    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif
//
//#ifdef SKIP_HOSTNAME_VERIFICATION
//    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYHOST, 0L);
//#endif
//
//	curl_easy_perform(pcurl);
//
//
//	curl_easy_cleanup(pcurl);
//	fclose(fp);
//
//	curl_global_cleanup();
//
//	std::cout << "run end" << std::endl;
//
//	return;
//}
//
//void test_https_post1(void)
//{
//	std::cout << "test of " << curl_version() << std::endl;
//	CURLcode ret_code = curl_global_init(CURL_GLOBAL_ALL);
//	if(ret_code)
//	{
//		std::cout << "error_code = " << ret_code << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//	CURL* pcurl = curl_easy_init();
//	if(!pcurl)
//	{
//		std::cout << "init failed" << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//
//	std::string url("https://127.0.0.1/registerpro.php");
//
//	FILE *fp = fopen("data_https_post.html", "wb");
//	// 设置easy handle属性
//	curl_easy_setopt(pcurl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &process_data);
//	//curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);
//
//	curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, fp);
//	//curl_easy_setopt(pcurl, CURLOPT_POST, 1L);
//
//	//curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, "username=");
//	//curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, "username=u1");
//	//curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, "username=u2");
//
//	// 执行数据请求
//
//#ifdef SKIP_PEER_VERIFICATION
//    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif
//
//#ifdef SKIP_HOSTNAME_VERIFICATION
//    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYHOST, 0L);
//#endif
//
//	curl_easy_perform(pcurl);
//
//
//	curl_easy_cleanup(pcurl);
//	fclose(fp);
//
//	curl_global_cleanup();
//
//	std::cout << "run end" << std::endl;
//
//	return;
//}
//
//void test_https_post_to_ios1(void)
//{
//	std::cout << "test of " << curl_version() << std::endl;
//	CURLcode ret_code = curl_global_init(CURL_GLOBAL_ALL);
//	if(ret_code)
//	{
//		std::cout << "error_code = " << ret_code << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//	CURL* pcurl = curl_easy_init();
//	if(!pcurl)
//	{
//		std::cout << "init failed" << std::endl;
//		curl_global_cleanup();
//		return;
//	}
//
//	std::string url("https://sandbox.itunes.apple.com/verifyReceipt");
//	//std::string url("https://buy.itunes.apple.com/verifyReceipt");
//
//	FILE *fp = fopen("data_https_post_ios1.html", "wb");
//	// 设置easy handle属性
//	curl_easy_setopt(pcurl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &process_data);
//	//curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);
//
//	curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, fp);
//	//curl_easy_setopt(pcurl, CURLOPT_POST, 1L);
//
//	// 执行数据请求
//
//#ifdef SKIP_PEER_VERIFICATION
//    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif
//
//#ifdef SKIP_HOSTNAME_VERIFICATION
//    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYHOST, 0L);
//#endif
//
//	curl_easy_perform(pcurl);
//
//
//	curl_easy_cleanup(pcurl);
//	fclose(fp);
//
//	curl_global_cleanup();
//
//	std::cout << "run end" << std::endl;
//
//	return;
//}

#include <string>
#include <sstream>
#include <yggr/file_system/local_file_operator.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

//std::size_t process_data_of_ios(void *buffer, std::size_t size, std::size_t nmemb)
//{
//	typedef boost::property_tree::ptree ptree_type;
//	typedef ptree_type::const_assoc_iterator assoc_iter_type;
//	const char* p = reinterpret_cast<char*>(buffer);
//	int data_size = size * nmemb;
//	std::string str(p, p + data_size);
//	std::stringstream ss(str);
//	std::cout << ss.str() << std::endl;
//	ptree_type ptree;
//
//
//	try
//	{
//		boost::property_tree::read_json(ss, ptree);
//	}
//	catch(const boost::property_tree::ptree_error& e)
//	{
//		std::cerr << e.what() << std::endl;
//		return 0;
//	}
//
//	int code = -1;
//	try
//	{
//		code = ptree.get<int>("status");
//	}
//	catch(const boost::property_tree::ptree_error& e)
//	{
//		std::cerr << e.what() << std::endl;
//		return 0;
//	}
//
//	if(code != 0)
//	{
//		std::cout << "error code = " << code << std::endl;
//		return 0;
//	}
//
//	assoc_iter_type iter = ptree.find("receipt");
//	if(iter == ptree.not_found())
//	{
//		std::cerr << "data lost" << std::endl;
//		return 0;
//	}
//
//	try
//	{
//		std::cout << "product_id = " << iter->second.get<std::string>("product_id") << std::endl;
//	}
//	catch(const boost::property_tree::ptree_error& e)
//	{
//		std::cerr << e.what() << std::endl;
//		return 0;
//	}
//
//	std::cout << "call back end" << std::endl;
//	return 0 == code? size * nmemb : 0;
//}
//
std::size_t process_data_of_ios2(void *buffer, std::size_t size, std::size_t nmemb, void* np)
{
	typedef boost::property_tree::ptree ptree_type;
	typedef ptree_type::const_assoc_iterator assoc_iter_type;
	const char* p = reinterpret_cast<char*>(buffer);
	int data_size = size * nmemb;
	std::string str(p, p + data_size);
	std::stringstream ss(str);
	std::cout << ss.str() << std::endl;
	ptree_type ptree;

	assert(np);
	int *pint = reinterpret_cast<int*>(np);
	assert(pint);
	*pint = 100;

	try
	{
		boost::property_tree::read_json(ss, ptree);
	}
	catch(const boost::property_tree::ptree_error& e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}

	int code = -1;
	try
	{
		code = ptree.get<int>("status");
	}
	catch(const boost::property_tree::ptree_error& e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}

	if(code != 0)
	{
		std::cout << "error code = " << code << std::endl;
		return 0;
	}

	assoc_iter_type iter = ptree.find("receipt");
	if(iter == ptree.not_found())
	{
		std::cerr << "data lost" << std::endl;
		return 0;
	}

	try
	{
		std::cout << "product_id = " << iter->second.get<std::string>("product_id") << std::endl;
	}
	catch(const boost::property_tree::ptree_error& e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}

	std::cout << "call back end" << std::endl;
	return 0 == code? size * nmemb : 0;
}

void test_https_post_to_ios2(void)
{
	std::cout << "test of " << curl_version() << std::endl;
	CURLcode ret_code = curl_global_init(CURL_GLOBAL_ALL);
	if(ret_code)
	{
		std::cout << "error_code = " << ret_code << std::endl;
		curl_global_cleanup();
		return;
	}
	CURL* pcurl = curl_easy_init();
	if(!pcurl)
	{
		std::cout << "init failed" << std::endl;
		curl_global_cleanup();
		return;
	}

	std::string url("https://sandbox.itunes.apple.com/verifyReceipt");
	//std::string url("https://buy.itunes.apple.com/verifyReceipt");

	std::string data_body;
	yggr::file_system::local_file_operator_type::read_file_of_binary("1.txt", data_body);

	assert(!data_body.empty());
	std::string data = data_body;

	int n = 10;
	std::cout << "-----------------request data start-----------------" << std::endl;
	std::cout << data << std::endl;
	std::cout << "-----------------request data end-----------------" << std::endl;

	// 设置easy handle属性
	curl_easy_setopt(pcurl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &process_data_of_ios2);
	curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);

	curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, &n);
	curl_easy_setopt(pcurl, CURLOPT_POST, 1L);
	curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, data.c_str());

	// 执行数据请求

#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

	curl_easy_perform(pcurl);
	curl_easy_perform(pcurl);

	curl_easy_cleanup(pcurl);
	//delete pcurl;
	//fclose(fp);

	curl_global_cleanup();

	std::cout << "run end" << std::endl;


	return;
}


int main(int argc, char* argv[])
{
	//test_http1();
	//test_https1();
	//test_https_post1();
	//test_https_post_to_ios1();
	test_https_post_to_ios2();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
