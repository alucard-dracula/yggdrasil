// LibCurl.cpp : Defines the entry point for the console application.
//

#define CURL_STATICLIB
#define USE_SSLEAY
#define USE_OPENSSL

#ifdef _DEBUG
#	pragma comment(lib, "libcurld.lib")
#	pragma comment(lib, "ssleay32.lib")
#	pragma comment(lib, "libeay32.lib")
#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "wldap32.lib")
#else
#	pragma comment(lib, "libcurl.lib")
#	pragma comment(lib, "ssleay32.lib")
#	pragma comment(lib, "libeay32.lib")
#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "wldap32.lib")
#endif // _DEBUG

#include <iostream>
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
    res = curl_easy_perform(curl);

    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }

  char cc = 0;
  std::cin >> cc;
  return 0;
}