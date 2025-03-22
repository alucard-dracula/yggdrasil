//sha_test.cpp

#include <sha.h>

#include <iostream>
#include <cstring>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

/*

SHA1("The quick brown fox jumps over the lazy dog") = 2fd4e1c67a2d28fced849ee1bb76e7391b93eb12
SHA256("The quick brown fox jumps over the lazy dog") = d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592

HMAC_SHA1("key", "The quick brown fox jumps over the lazy dog") = 0xde7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9
HMAC_SHA256("key", "The quick brown fox jumps over the lazy dog") = 0xf7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8
*/
int main(void)
{
	char Data[2048] = "The quick brown fox jumps over the lazy dog", Key[2048] = "key";
	SHA256_DATA SD256;
	SHA1_DATA SD1;
	SHA1(&SD1, Data, strlen(Data));
	SHA256(&SD256, Data, strlen(Data));

	//printf("\nSHA1 = %s\n",SD1.Val_String);
	//printf("SHA256 = %s\n\n",SD256.Val_String);

	HMAC_SHA1(&SD1, Data, strlen(Data), Key, strlen(Key));
	HMAC_SHA256(&SD256, Data, strlen(Data), Key, strlen(Key));
	//printf("HMAC-SHA1 = %s\n",SD1.Val_String);
	//printf("HMAC-SHA256 = %s\n",SD256.Val_String);

	//std::cout << sizeof(SHA_INT_TYPE) << std::endl;

	std::cout << sizeof(SHA1_DATA) << std::endl;
	std::cout << sizeof(SHA256_DATA) << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;

}
