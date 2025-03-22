// sha.cpp
#include "sha.h"
//#include <stdio.h>
//#include <string.h>
//#include <malloc.h>
//#include <memory>
#include <cstring>

//#ifdef _MSC_VER
//#	include <vld.h>
//#endif // _MSC_VER

static const SHA_INT_TYPE SHA1_H_Val[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 };

static const SHA_INT_TYPE SHA256_H_Val[] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 } ;

static const SHA_INT_TYPE SHA256_K_Val[] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
											  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
											  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
											  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
											  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
											  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
											  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
											  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 } ;

static union { unsigned long mylong; char c[4]; } endian_test = { 0x623f3f6c };

static inline bool is_little_end(void) { return endian_test.c[0] == 'l';}
static inline bool is_big_end(void) { return endian_test.c[0] == 'b';}


SHA_INT_TYPE SHA1_K(SHA_INT_TYPE t)
{
	if(t <= 19) return 0x5a827999;
	else if(t <= 39) return 0x6ed9eba1;
	else if(t <= 59) return 0x8f1bbcdc;
	else if(t <= 79) return 0xca62c1d6;
	else return 0;
}

SHA_INT_TYPE SHA1_f(SHA_INT_TYPE t, SHA_INT_TYPE B, SHA_INT_TYPE C, SHA_INT_TYPE D)
{
	if(t <= 19) return (B & C) | (~B & D);
	else if(t <= 39) return B ^ C ^ D;
	else if(t <= 59) return (B & C)|(B & D)|(C & D);
	else if(t <= 79) return B ^ C ^ D;
	else return 0;
}

SHA_INT_TYPE SHA1_rotl(SHA_INT_TYPE r, SHA_INT_TYPE x)
{
	SHA_INT_TYPE rot = r % 32;
	return (x >> (32 - rot)) | (x << rot);
}

void SHA1_Copy(unsigned char* copy, SHA1_DATA *shd)
{
	SHA_INT_TYPE Value[sha1_size::value_size];
	for(int i = 0; i != sha1_size::value_size; ++i)
	{
		Value[i] = SHA_Reverse(shd->Value[i]);
	}
	memcpy(copy, Value, sha1_size::buf_size);
}

void HMAC_SHA1_Copy(unsigned char* copy, SHA1_DATA *shd)
{
	SHA_INT_TYPE Value[sha1_size::value_size];
	for(int i = 0; i != sha1_size::value_size; ++i)
	{
		Value[i] = SHA_Reverse(shd->Value[i]);
	}
	memcpy(copy, Value, sha1_size::buf_size);
}

SHA_INT_TYPE SHA256_rotr(SHA_INT_TYPE r, SHA_INT_TYPE x)
{
	SHA_INT_TYPE rot = r % 32;
	return (x >> rot) | (x << (32-rot));
}

SHA_INT_TYPE SHA256_sigma0(SHA_INT_TYPE x)
{
	return SHA256_rotr(7, x) ^ SHA256_rotr(18, x) ^ (x >> 3);
}

SHA_INT_TYPE SHA256_sigma1(SHA_INT_TYPE x)
{
	return SHA256_rotr(17, x) ^ SHA256_rotr(19, x) ^ (x >> 10);
}

SHA_INT_TYPE SHA256_sum0(SHA_INT_TYPE x)
{
	return SHA256_rotr(2, x) ^ SHA256_rotr(13, x) ^ SHA256_rotr(22, x);
}

SHA_INT_TYPE SHA256_sum1(SHA_INT_TYPE x)
{
	return SHA256_rotr(6, x) ^ SHA256_rotr(11, x) ^ SHA256_rotr(25, x);
}

SHA_INT_TYPE SHA256_ch(SHA_INT_TYPE x, SHA_INT_TYPE y, SHA_INT_TYPE z)
{
	return (x & y)^(~x & z);
}

SHA_INT_TYPE SHA256_maj(SHA_INT_TYPE x, SHA_INT_TYPE y, SHA_INT_TYPE z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}

void SHA_Reverse_INT64(unsigned char* data, boost::uint64_t write)
{
	unsigned char cdata[8];
	memcpy(cdata, &write, sizeof(boost::uint64_t));
	if(is_little_end())
	{
		for(int i=0;i<=7;i++) *(data + i) = cdata[7-i];
	}
}

SHA_INT_TYPE SHA_Reverse(SHA_INT_TYPE d)
{
	unsigned char b_data[4], a_data[4] = {0};
	SHA_INT_TYPE ret;
	memcpy(b_data, &d, sizeof(boost::int32_t));
	if(is_little_end())
	{
		for(int i=0; i<4; i++) a_data[i] = b_data[3 - i];
	}
	memcpy(&ret, a_data, sizeof(a_data));
	return ret;
}

void SHA256_Copy(unsigned char* copy, SHA256_DATA *shd)
{
	SHA_INT_TYPE Value[sha256_size::value_size];
	if(is_little_end())
	{
		for(unsigned int i = 0; i != sha256_size::value_size; ++i)
		{
			Value[i] = SHA_Reverse(shd->Value[i]);
		}
	}
	memcpy(copy, Value, sha256_size::buf_size);
}


void HMAC_SHA256_Copy(unsigned char* copy, SHA256_DATA *shd)
{
	SHA_INT_TYPE Value[sha256_size::value_size];
	if(is_little_end())
	{
		for(unsigned int i = 0; i != sha256_size::value_size; ++i)
		{
			Value[i] = SHA_Reverse(shd->Value[i]);
		}
	}
	memcpy(copy, Value, sha256_size::buf_size);
}

void SHA1_HashBlock(SHA_INT_TYPE* SHA1_H_Data, const unsigned char* data)
{
	SHA_INT_TYPE SIT[80];
	SHA_INT_TYPE SIT_d[16];
	SHA_INT_TYPE a, b, c, d, e;
	for(int i=0, j=0;i<16;i++, j+=4)
	{
		SIT_d[i] = ((*(data + j +3)&0xFF) << 24)
					| ((*(data + j + 2)&0xFF) << 16)
					| ((*(data + j + 1)&0xFF) << 8)
					| ((*(data + j)&0xFF));
	}

	if(is_little_end())
	{
		for(int i=0;i<16;i++)
		{
			SIT[i] = SHA_Reverse(SIT_d[i]);
		}
	}

	for(int t=16;t<=79;t++)
	{
		SIT[t] = SHA1_rotl(1, SIT[t - 3] ^ SIT[t - 8] ^ SIT[t - 14] ^ SIT[t - 16]);
	}

	a = *SHA1_H_Data;
	b = *(SHA1_H_Data + 1);
	c = *(SHA1_H_Data + 2);
	d = *(SHA1_H_Data + 3);
	e = *(SHA1_H_Data + 4);

	for(int t=0;t<=79;t++)
	{
		SHA_INT_TYPE tmp;
		tmp = SHA1_rotl(5, a)+SHA1_f(t, b, c, d)+e+SIT[t]+SHA1_K(t);
		e = d;
		d = c;
		c = SHA1_rotl(30, b);
		b = a;
		a = tmp;
	}

	*SHA1_H_Data += a;
	*(SHA1_H_Data + 1) += b;
	*(SHA1_H_Data + 2) += c;
	*(SHA1_H_Data + 3) += d;
	*(SHA1_H_Data + 4) += e;
}

bool SHA1(SHA1_DATA* sha1d, const char* data, std::size_t size)
{
	std::size_t s, ns;
	SHA_INT_TYPE h[5];
	//int cnt=0;
	boost::uint64_t s64_val;
	unsigned char d[64];
	if(!(sha1d && size)) return false;
	s = size;
	memcpy(h, SHA1_H_Val, sizeof(SHA1_H_Val));

	for(std::size_t i = s, j = 0; i >= 64; i -= 64, j += 64)
	{
		SHA1_HashBlock(h, reinterpret_cast<const unsigned char*>(data + j));
	}

	ns = s % 64;

	memset(d, 0, 64);

	memcpy(d, data + (s - ns), ns);

	d[ns] = 0x80;

	if(ns >= 56)
	{
		SHA1_HashBlock(h, d);
		memset(d, 0, 56);
	}

	s64_val = s*8;

	if(is_little_end())
	{
		SHA_Reverse_INT64(&d[56], s64_val);
	}

	SHA1_HashBlock(h, d);
	memcpy(sha1d->Value, h, sizeof(h));
	//sprintf(sha1d->Val_String, "%08X %08X %08X %08X %08X", h[0], h[1], h[2], h[3], h[4]);
	return true;
}

bool HMAC_SHA1(SHA1_DATA *sha1d, const char* b_target, std::size_t tsize, const char* b_key, std::size_t ksize)
{
	unsigned char key[65], ipad[64], opad[64];
	unsigned char* tk, tk2[20], tk3[84];
	//SHA_INT_TYPE tks;
	std::size_t tks;
	SHA1_DATA SD, ret;

	if(!(sha1d && b_target && tsize && b_key && ksize))
	{
		return false;
	}

	memset(&SD, 0, sizeof(SHA1_DATA));
	memset(key, 0, 65);
	memset(ipad, 0x36, 64);
	memset(opad, 0x5c, 64);

	if(ksize > 64)
	{
		SHA1(&SD, b_key, 0);
		HMAC_SHA1_Copy(key, &SD);
	}
	else
	{
		memcpy(key, reinterpret_cast<unsigned char*>(const_cast<char*>(b_key)), ksize);
	}
	memset(&SD, 0, sizeof(SHA1_DATA));
	for(int i=0;i<64;i++)
	{
		ipad[i] = key[i] ^ ipad[i];
		opad[i] = key[i] ^ opad[i];
	}
	//tks = (tsize)?tsize:strlen(b_target) + 64;
	tks = tsize + 64;
	tk = new unsigned char[tks];
	if(!tk) return false;
	memset(tk, 0, tks);
	memcpy(tk, ipad, 64);
	memcpy(tk + 64, reinterpret_cast<unsigned char*>(const_cast<char*>(b_target)), tsize);
	SHA1(&SD, reinterpret_cast<char*>(tk), tks);
	HMAC_SHA1_Copy(tk2, &SD);
	memcpy(tk3, opad, 64);
	memcpy(tk3 + 64, tk2, 20);
	SHA1(&ret, reinterpret_cast<char*>(tk3), 84);
	memcpy(sha1d, &ret, sizeof(SHA1_DATA));
	//free(tk);
	delete[] tk;
	return true;
}


void SHA256_HashBlock(SHA_INT_TYPE* SHA256_H_Data, const unsigned char* data)
{
	SHA_INT_TYPE SIT[64];
	SHA_INT_TYPE SIT_d[16];
	SHA_INT_TYPE a, b, c, d, e, f, g, h;
	for(int i=0, j=0;i<16;i++, j+=4)
	{
		SIT_d[i] = ((*(data + j +3) & 0xFF) << 24)
					| ((*(data + j + 2) & 0xFF) << 16)
					| ((*(data + j + 1) & 0xFF) << 8)
					| ((*(data + j) & 0xFF));
	}

	if(is_little_end())
	{
		for(int i=0;i<16;i++)
		{
			SIT[i] = SHA_Reverse(SIT_d[i]);
		}
	}

	for(int t=16;t<=63;t++)
	{
		SIT[t] = SHA256_sigma1(SIT[t-2]) + SIT[t-7] + SHA256_sigma0(SIT[t-15]) + SIT[t-16];
	}

	a = *SHA256_H_Data;
	b = *(SHA256_H_Data + 1);
	c = *(SHA256_H_Data + 2);
	d = *(SHA256_H_Data + 3);
	e = *(SHA256_H_Data + 4);
	f = *(SHA256_H_Data + 5);
	g = *(SHA256_H_Data + 6);
	h = *(SHA256_H_Data + 7);

	for(int t = 0; t <= 63; t++)
	{
		SHA_INT_TYPE tmp[2];
		tmp[0] = h + SHA256_sum1(e) + SHA256_ch(e, f, g) + SHA256_K_Val[t] +SIT[t];
		tmp[1] = SHA256_sum0(a) + SHA256_maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + tmp[0];
		d = c;
		c = b;
		b = a;
		a = tmp[0] + tmp[1];
	}

	*SHA256_H_Data += a;
	*(SHA256_H_Data + 1) += b;
	*(SHA256_H_Data + 2) += c;
	*(SHA256_H_Data + 3) += d;
	*(SHA256_H_Data + 4) += e;
	*(SHA256_H_Data + 5) += f;
	*(SHA256_H_Data + 6) += g;
	*(SHA256_H_Data + 7) += h;
}

bool SHA256(SHA256_DATA* sha256d, const char* data, std::size_t size)
{
	std::size_t s, ns;
	SHA_INT_TYPE h[8];
	boost::uint64_t s64_val;
	unsigned char d[64];
	if(!(sha256d && size)) return false;
	//s = (size)?size:strlen(data);
	s = size;
	memcpy(h, SHA256_H_Val, sizeof(SHA256_H_Val));

	for(std::size_t i = s, j = 0; i >= 64 ; i -= 64, j += 64)
	{
		SHA256_HashBlock(h, reinterpret_cast<const unsigned char*>(data + j));
	}

	ns = s % 64;

	memset(d, 0, 64);

	memcpy(d, data + (s - ns), ns);

	d[ns] = 0x80;

	if(ns >= 56)
	{
		SHA256_HashBlock(h, d);
		memset(d, 0, 56);
	}

	s64_val = s*8;

	if(is_little_end())
	{
		SHA_Reverse_INT64(&d[56], s64_val);
	}

	SHA256_HashBlock(h, d);

	memcpy(sha256d->Value, h, sizeof(h));
	//sprintf(sha256d->Val_String, "%08X %08X %08X %08X %08X %08X %08X %08X", h[0], h[1], h[2], h[3], h[4], h[5], h[6], h[7]);
	return true;
}

bool HMAC_SHA256(SHA256_DATA *sha256d, const char* b_target, std::size_t tsize, const char* b_key, std::size_t ksize)
{
	unsigned char key[65], ipad[64], opad[64];
	unsigned char* tk, tk2[32], tk3[96];
	//SHA_INT_TYPE tks;
	std::size_t tks;
	SHA256_DATA SD, ret;
	if(!(sha256d && b_target && tsize && b_key && ksize)) return false;
	memset(&SD, 0, sizeof(SHA256_DATA));
	memset(key, 0, 65);
	memset(ipad, 0x36, 64);
	memset(opad, 0x5c, 64);
	if(ksize > 64)
	{
		SHA256(&SD, b_key, ksize);
		HMAC_SHA256_Copy(key, &SD);
	}
	else
	{
		memcpy(key, reinterpret_cast<unsigned char*>(const_cast<char*>(b_key)), ksize);
	}
	memset(&SD, 0, sizeof(SHA256_DATA));
	for(int i=0;i<64;i++)
	{
		ipad[i] = key[i] ^ ipad[i];
		opad[i] = key[i] ^ opad[i];
	}
	tks = tsize + 64;
	tk = new unsigned char[tks];
	if(!tk)return false;
	memset(tk, 0, tks);
	memcpy(tk, ipad, 64);
	memcpy(tk+64, reinterpret_cast<unsigned char*>(const_cast<char*>(b_target)), tsize);
	SHA256(&SD, reinterpret_cast<char*>(tk), tks);
	HMAC_SHA256_Copy(tk2, &SD);
	memcpy(tk3, opad, 64);
	memcpy(tk3 + 64, tk2, 32);
	SHA256(&ret, reinterpret_cast<char*>(tk3), 96);
	memcpy(sha256d, &ret, sizeof(SHA256_DATA));
	//free(tk);
	delete[] tk;
	return true;
}
