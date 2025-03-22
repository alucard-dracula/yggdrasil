
/*
author£ºHiroaki Software
mending: xu yang
*/

#ifndef __SHA_H__
#define __SHA_H__

#include <boost/cstdint.hpp>
#include <cstddef>

typedef boost::uint32_t SHA_INT_TYPE;

struct sha256_size
{
	enum
	{
		value_size = 8,
		buf_size = value_size * sizeof(SHA_INT_TYPE),
		string_size = 72,

		E_compile_sha256_size_u32 = 0xffffffff
	};
};

struct sha1_size
{
	enum
	{
		value_size = 5,
		buf_size = value_size * sizeof(SHA_INT_TYPE),
		string_size = 45,

		E_compile_sha256_size_u32 = 0xffffffff
	};
};

typedef struct tagSHA256_DATA
{
	SHA_INT_TYPE Value[sha256_size::value_size];
	//char Val_String[sha256_size::string_size];
} SHA256_DATA;

typedef struct tagSHA1_DATA
{
	SHA_INT_TYPE Value[sha1_size::value_size];
	//char Val_String[sha1_size::string_size];
} SHA1_DATA;

SHA_INT_TYPE SHA1_K(SHA_INT_TYPE);
SHA_INT_TYPE SHA1_f(SHA_INT_TYPE,SHA_INT_TYPE,SHA_INT_TYPE,SHA_INT_TYPE);
SHA_INT_TYPE SHA1_rotl(SHA_INT_TYPE,SHA_INT_TYPE);
SHA_INT_TYPE SHA256_rotr(SHA_INT_TYPE,SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sigma0(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sigma1(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sum0(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sum1(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_ch(SHA_INT_TYPE,SHA_INT_TYPE,SHA_INT_TYPE);
SHA_INT_TYPE SHA256_maj(SHA_INT_TYPE,SHA_INT_TYPE,SHA_INT_TYPE);

void SHA_Reverse_INT64(const unsigned char* , boost::uint64_t);
SHA_INT_TYPE SHA_Reverse(SHA_INT_TYPE);

void SHA1_HashBlock(SHA_INT_TYPE* , const unsigned char* );
bool SHA1(SHA1_DATA* sha1d, const char* data, std::size_t size);
void SHA1_Copy(unsigned char*, SHA1_DATA*);
void SHA256_HashBlock(SHA_INT_TYPE* , const unsigned char* );
bool SHA256(SHA256_DATA* sha256d, const char* data, std::size_t size);
void SHA256_Copy(unsigned char*, SHA256_DATA*);

bool HMAC_SHA1(SHA1_DATA* sha1d, const char* b_target, std::size_t tsize, const char* b_key, std::size_t ksize);
void HMAC_SHA1_Copy(unsigned char*, SHA1_DATA*);
bool HMAC_SHA256(SHA256_DATA *sha256d, const char* b_target, std::size_t tsize, const char* b_key, std::size_t ksize);
void HMAC_SHA256_Copy(unsigned char*, SHA256_DATA*);

#endif // __SHA_H__
