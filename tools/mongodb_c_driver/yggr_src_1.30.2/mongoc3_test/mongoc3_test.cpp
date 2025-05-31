// mongo_test.cpp

#include <iostream>
#include <string>
#include <bson.h>
//#include <bson-private.h>
#include <b64_ntop.h>
#include <bson-iter.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#	if defined(_DEBUG)
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100-d.lib")
#			pragma comment(lib, "libcommon-vc100-d.lib")
#			pragma comment(lib, "libmongoc-vc100-d.lib")
#			pragma comment(lib, "libmongocrypt-vc100-d.lib")
#			pragma comment(lib, "libkms-message-vc100-d.lib")
#			pragma comment(lib, "libutf8proc-vc100-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100-d.lib")
#			pragma comment(lib, "libbase64-vc100-d.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110-d.lib")
#			pragma comment(lib, "libcommon-vc110-d.lib")
#			pragma comment(lib, "libmongoc-vc110-d.lib")
#			pragma comment(lib, "libmongocrypt-vc110-d.lib")
#			pragma comment(lib, "libkms-message-vc110-d.lib")
#			pragma comment(lib, "libutf8proc-vc110-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110-d.lib")
#			pragma comment(lib, "libbase64-vc110-d.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120-d.lib")
#			pragma comment(lib, "libcommon-vc120-d.lib")
#			pragma comment(lib, "libmongoc-vc120-d.lib")
#			pragma comment(lib, "libmongocrypt-vc120-d.lib")
#			pragma comment(lib, "libkms-message-vc120-d.lib")
#			pragma comment(lib, "libutf8proc-vc120-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120-d.lib")
#			pragma comment(lib, "libbase64-vc120-d.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140-d.lib")
#			pragma comment(lib, "libcommon-vc140-d.lib")
#			pragma comment(lib, "libmongoc-vc140-d.lib")
#			pragma comment(lib, "libmongocrypt-vc140-d.lib")
#			pragma comment(lib, "libkms-message-vc140-d.lib")
#			pragma comment(lib, "libutf8proc-vc140-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140-d.lib")
#			pragma comment(lib, "libbase64-vc140-d.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141-d.lib")
#			pragma comment(lib, "libcommon-vc141-d.lib")
#			pragma comment(lib, "libmongoc-vc141-d.lib")
#			pragma comment(lib, "libmongocrypt-vc141-d.lib")
#			pragma comment(lib, "libkms-message-vc141-d.lib")
#			pragma comment(lib, "libutf8proc-vc141-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141-d.lib")
#			pragma comment(lib, "libbase64-vc141-d.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142-d.lib")
#			pragma comment(lib, "libcommon-vc142-d.lib")
#			pragma comment(lib, "libmongoc-vc142-d.lib")
#			pragma comment(lib, "libmongocrypt-vc142-d.lib")
#			pragma comment(lib, "libkms-message-vc142-d.lib")
#			pragma comment(lib, "libutf8proc-vc142-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142-d.lib")
#			pragma comment(lib, "libbase64-vc142-d.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143-d.lib")
#			pragma comment(lib, "libcommon-vc143-d.lib")
#			pragma comment(lib, "libmongoc-vc143-d.lib")
#			pragma comment(lib, "libmongocrypt-vc143-d.lib")
#			pragma comment(lib, "libkms-message-vc143-d.lib")
#			pragma comment(lib, "libutf8proc-vc143-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143-d.lib")
#			pragma comment(lib, "libbase64-vc143-d.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	else // _DEBUG
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100.lib")
#			pragma comment(lib, "libcommon-vc100.lib")
#			pragma comment(lib, "libmongoc-vc100.lib")
#			pragma comment(lib, "libmongocrypt-vc100.lib")
#			pragma comment(lib, "libkms-message-vc100.lib")
#			pragma comment(lib, "libutf8proc-vc100.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100.lib")
#			pragma comment(lib, "libbase64-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110.lib")
#			pragma comment(lib, "libcommon-vc110.lib")
#			pragma comment(lib, "libmongoc-vc110.lib")
#			pragma comment(lib, "libmongocrypt-vc110.lib")
#			pragma comment(lib, "libkms-message-vc110.lib")
#			pragma comment(lib, "libutf8proc-vc110.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110.lib")
#			pragma comment(lib, "libbase64-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120.lib")
#			pragma comment(lib, "libcommon-vc120.lib")
#			pragma comment(lib, "libmongoc-vc120.lib")
#			pragma comment(lib, "libmongocrypt-vc120.lib")
#			pragma comment(lib, "libkms-message-vc120.lib")
#			pragma comment(lib, "libutf8proc-vc120.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120.lib")
#			pragma comment(lib, "libbase64-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140.lib")
#			pragma comment(lib, "libcommon-vc140.lib")
#			pragma comment(lib, "libmongoc-vc140.lib")
#			pragma comment(lib, "libmongocrypt-vc140.lib")
#			pragma comment(lib, "libkms-message-vc140.lib")
#			pragma comment(lib, "libutf8proc-vc140.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140.lib")
#			pragma comment(lib, "libbase64-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141.lib")
#			pragma comment(lib, "libcommon-vc141.lib")
#			pragma comment(lib, "libmongoc-vc141.lib")
#			pragma comment(lib, "libmongocrypt-vc141.lib")
#			pragma comment(lib, "libkms-message-vc141.lib")
#			pragma comment(lib, "libutf8proc-vc141.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141.lib")
#			pragma comment(lib, "libbase64-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142.lib")
#			pragma comment(lib, "libcommon-vc142.lib")
#			pragma comment(lib, "libmongoc-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libkms-message-vc142.lib")
#			pragma comment(lib, "libutf8proc-vc142.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142.lib")
#			pragma comment(lib, "libbase64-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143.lib")
#			pragma comment(lib, "libcommon-vc143.lib")
#			pragma comment(lib, "libmongoc-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libkms-message-vc143.lib")
#			pragma comment(lib, "libutf8proc-vc143.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143.lib")
#			pragma comment(lib, "libbase64-vc143.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	endif // _DEBUG
#endif // _MSC_VER

#ifdef _MSC_VER
//#	pragma comment(lib, "ssleay32.lib")
//#	pragma comment(lib, "libeay32.lib")

#	if defined(_DEBUG)
#		pragma comment(lib, "zlibstaticd.lib")
#		pragma comment(lib, "icuucd.lib")
#	else
#		pragma comment(lib, "zlibstatic.lib")
#		pragma comment(lib, "icuuc.lib")
#	endif // _DEBUG

#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "Dnsapi.lib")
#	pragma comment(lib, "Bcrypt.lib")
#	pragma comment(lib, "Secur32.lib")
#	pragma comment(lib, "Crypt32.lib")
#endif // _MSC_VER

#ifdef _MSC_VER
#	if 1
#		if _MSC_VER == 1600
#			pragma comment(lib, "libcrypto-vc100.lib")
#			pragma comment(lib, "libssl-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libcrypto-vc110.lib")
#			pragma comment(lib, "libssl-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libcrypto-vc120.lib")
#			pragma comment(lib, "libssl-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libcrypto-vc140.lib")
#			pragma comment(lib, "libssl-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libcrypto-vc141.lib")
#			pragma comment(lib, "libssl-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libcrypto-vc142.lib")
#			pragma comment(lib, "libssl-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libcrypto-vc143.lib")
#			pragma comment(lib, "libssl-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	else
#		if _MSC_VER == 1600
#			pragma comment(lib, "libcrypto_static-vc100.lib")
#			pragma comment(lib, "libssl_static-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libcrypto_static-vc110.lib")
#			pragma comment(lib, "libssl_static-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libcrypto_static-vc120.lib")
#			pragma comment(lib, "libssl_static-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libcrypto_static-vc140.lib")
#			pragma comment(lib, "libssl_static-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libcrypto_static-vc141.lib")
#			pragma comment(lib, "libssl_static-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libcrypto_static-vc142.lib")
#			pragma comment(lib, "libssl_static-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libcrypto_static-vc143.lib")
#			pragma comment(lib, "libssl_static-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	endif // 0,1
#endif // _MSC_VER

void bson_append_start_test(void)
{
	bson_t query;
	bson_t child, child2, child3;
	bson_init(&query);
	bson_append_array_begin(&query, "$or", -1, &child);

	bson_append_int32(&child, "", -1, 10);
	bson_append_int32(&child, "", -1, 30);
	bson_append_int32(&child, "", -1, 20);

	////0: 第一个or部分
	//bson_append_document_begin(&child, "0", -1, &child2);
	//BSON_APPEND_INT64(&child2, "id", 4);
	//bson_append_document_end(&child, &child2);

	////1:第二个or部分
	//bson_append_document_begin(&child, "1", -1, &child2);

	////field1 <= 12
	//bson_append_document_begin(&child2, "field1", -1, &child3);
	//BSON_APPEND_INT64(&child3, "$lte", 12);
	//bson_append_document_end(&child2, &child3);

	//bson_append_document_end(&child, &child2);

	bson_append_array_end(&query, &child);

	//bson_destroy (&child);

	{
		char * str = bson_as_json(&query, NULL);
		std::cout << str << std::endl;
		free(str);
	}

	bson_init(&child3);
	bson_append_code_with_scope(&child3, "cws", -1, "this is js code", &query);

	{
		char * str = bson_as_json(&child3, 0);
		std::cout << str << std::endl;
		free(str);
	}

	bson_destroy(&query);
	bson_destroy(&child3);
}

void bson_iter_test(void)
{
	bson_t bs;

	bson_init(&bs);
	bson_append_int32(&bs, "aaa", -1, 1);
	bson_append_int32(&bs, "bbb", -1, 2);
	
	bson_iter_t iter1;
	bson_iter_t iter2;
	bson_iter_t iter3;
	bson_iter_t iter4;

	bson_iter_init(&iter1, &bs);
	iter2 = iter1;
	bson_iter_next(&iter2);
	iter3 = iter2;
	bson_iter_next(&iter3);
	iter4 = iter3;
	bson_iter_next(&iter4);

	bson_destroy(&bs);
}

//static BSON_INLINE bool
//_bson_append_va (bson_t        *bson,        /* IN */
//                 uint32_t       n_bytes,     /* IN */
//                 uint32_t       n_pairs,     /* IN */
//                 uint32_t       first_len,   /* IN */
//                 const uint8_t *first_data,  /* IN */
//                 va_list        args)        /* IN */
//{
//   const uint8_t *data;
//   uint32_t data_len;
//   uint8_t *buf;
//
//   BSON_ASSERT (bson);
//   BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
//   BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
//   BSON_ASSERT (n_pairs);
//   BSON_ASSERT (first_len);
//   BSON_ASSERT (first_data);
//
//   if (BSON_UNLIKELY (!_bson_grow (bson, n_bytes))) {
//      return false;
//   }
//
//   data = first_data;
//   data_len = first_len;
//
//   buf = _bson_data (bson) + bson->len - 1;
//
//   do {
//      n_pairs--;
//      memcpy (buf, data, data_len);
//      bson->len += data_len;
//      buf += data_len;
//
//      if (n_pairs) {
//         data_len = va_arg (args, uint32_t);
//         data = va_arg (args, const uint8_t *);
//      }
//   } while (n_pairs);
//
//   _bson_encode_length (bson);
//
//   *buf = '\0';
//
//   return true;
//}

uint8_t* bson_data (const bson_t *bson) /* IN */
{
   if ((bson->flags & BSON_FLAG_INLINE))
   {
	   return const_cast<uint8_t*>(&((reinterpret_cast<const bson_impl_inline_t*>(bson))->data[0]));
   } 
   else
   {
      bson_impl_alloc_t *impl = const_cast<bson_impl_alloc_t*>(reinterpret_cast<const bson_impl_alloc_t*>(bson));
      return (*impl->buf) + impl->offset;
   }
}

void bson_encode_length (bson_t *bson) /* IN */
{
#if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
   memcpy (bson_data(bson), &bson->len, sizeof (bson->len));
#else
   uint32_t length_le = BSON_UINT32_TO_LE (bson->len);
   memcpy (bson_data(bson), &length_le, sizeof (length_le));
#endif
}

bool bson_impl_inline_grow (bson_impl_inline_t *impl, /* IN */
								size_t              size) /* IN */
{
   bson_impl_alloc_t *alloc = reinterpret_cast<bson_impl_alloc_t*>(impl);
   uint8_t *data;
   size_t req;

   BSON_ASSERT (impl);
   BSON_ASSERT (!(impl->flags & BSON_FLAG_RDONLY));
   BSON_ASSERT (!(impl->flags & BSON_FLAG_CHILD));

   if (((size_t)impl->len + size) <= sizeof impl->data) {
      return true;
   }

   req = bson_next_power_of_two (impl->len + size);

   if (req <= INT32_MAX) {
      data = reinterpret_cast<uint8_t*>(bson_malloc (req));

      memcpy (data, impl->data, impl->len);
	  {
		  bson_t* pbson = reinterpret_cast<bson_t*>(alloc);
		  assert(pbson);
		  pbson->flags &= ~BSON_FLAG_INLINE;
	  }
      //alloc->flags &= ~BSON_FLAG_INLINE;
      alloc->parent = NULL;
      alloc->depth = 0;
      alloc->buf = &alloc->alloc;
      alloc->buflen = &alloc->alloclen;
      alloc->offset = 0;
      alloc->alloc = data;
      alloc->alloclen = req;
      alloc->realloc = bson_realloc_ctx;
      alloc->realloc_func_ctx = NULL;

      return true;
   }

   return false;
}

bool bson_impl_alloc_grow (bson_impl_alloc_t *impl, /* IN */
							size_t             size) /* IN */
{
   size_t req;

   BSON_ASSERT (impl);

   /*
    * Determine how many bytes we need for this document in the buffer
    * including necessary trailing bytes for parent documents.
    */
   req = (impl->offset + impl->len + size + impl->depth);

   if (req <= *impl->buflen) {
      return true;
   }

   req = bson_next_power_of_two (req);

   if ((req <= INT32_MAX) && impl->realloc) {
      *impl->buf = reinterpret_cast<uint8_t*>(impl->realloc (*impl->buf, req, impl->realloc_func_ctx));
      *impl->buflen = req;
      return true;
   }

   return false;
}

bool bson_grow (bson_t   *bson, /* IN */
					uint32_t  size) /* IN */
{
   BSON_ASSERT (bson);
   BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));

   if ((bson->flags & BSON_FLAG_INLINE)) {
      return bson_impl_inline_grow (reinterpret_cast<bson_impl_inline_t*>(bson), size);
   }

   return bson_impl_alloc_grow (reinterpret_cast<bson_impl_alloc_t*>(bson), size);
}

bool save_name(bson_t* bson, uint8_t type, const std::string& name)
{
	BSON_ASSERT (bson);
	BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));

	if(name.empty())
	{
		return false;
	}

	if(type == BSON_TYPE_ARRAY || type == BSON_TYPE_DOCUMENT)
	{
		if((bson->flags & BSON_FLAG_INLINE)) 
		{
			BSON_ASSERT (bson->len <= 120);
			if (!bson_grow (bson, 128 - bson->len)) 
			{
				return false;
			}
			BSON_ASSERT (!(bson->flags & BSON_FLAG_INLINE));
		}
	}

	size_t byte_size = 1 + name.size() + 1;
	if (BSON_UNLIKELY (!bson_grow (bson, byte_size)))
	{
		return false;
	}

   uint8_t* buf = bson_data(bson) + bson->len - 1;
   memcpy(buf, &type, sizeof(uint8_t));
   memcpy(buf + sizeof(uint8_t), &(name[0]), name.size() /** sizeof(std::string::value_type)*/);
   buf[byte_size - 1] = 0;
#if _DEBUG
   buf[byte_size] = 0xff;
#else
   buf[byte_size] = 0;
#endif // _DEBUG
   bson->len += byte_size;

   uint8_t* pdata = bson_data(bson);

   int nnn = 0;
   return true;
}

bool save_value_of_int32(bson_t* bson, int32_t val)
{
	BSON_ASSERT (bson);
	BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
	
	uint8_t* pdata = bson_data(bson);
#ifdef _DEBUG
	uint8_t* p = bson_data(bson) + bson->len - 1;
	assert(*p == 0xff);
	*(bson_data(bson) + bson->len - 1) = 0;
#endif // _DEBUG

	int32_t val_le = BSON_UINT32_TO_LE(val);
	size_t byte_size = sizeof(int32_t); 
	if (BSON_UNLIKELY (!bson_grow (bson, byte_size))) 
	{
		return false;
	}

	uint8_t* pbuf = bson_data(bson) +  bson->len - 1;
	memcpy(pbuf, &val_le, sizeof(int32_t));
	pbuf[byte_size] = 0;
	bson->len += byte_size;

	bson_encode_length(bson);
	return true;
}

bool save_value_of_string(bson_t* bson, const std::string& val)
{
	BSON_ASSERT (bson);
	BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));
	BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
	
	uint8_t* pdata = bson_data(bson);
#ifdef _DEBUG
	uint8_t* p = bson_data(bson) + bson->len - 1;
	assert(*p == 0xff);
	*(bson_data(bson) + bson->len - 1) = 0;
#endif // _DEBUG

	int32_t lenth_le = BSON_UINT32_TO_LE(static_cast<int32_t>(val.size() + 1));
	size_t byte_size = sizeof(int32_t) + val.size() + 1; 
	if (BSON_UNLIKELY (!bson_grow (bson, byte_size))) 
	{
		return false;
	}

	uint8_t* pbuf = bson_data(bson) +  bson->len - 1;
	memcpy(pbuf, &lenth_le, sizeof(int32_t));
	if(val.size())
	{
		memcpy(pbuf + sizeof(int32_t), &val[0], val.size());
	}
	pbuf[byte_size - 1] = 0;
	pbuf[byte_size] = 0;
	bson->len += byte_size;

	bson_encode_length(bson);
	return true;
}

bool save_value_of_bson_begin (bson_t* bson,  bson_t* child) 
{
	const uint8_t empty[5] = { 5 };
	bson_impl_alloc_t *aparent = (bson_impl_alloc_t *)bson;
	bson_impl_alloc_t *achild = (bson_impl_alloc_t *)child;

	BSON_ASSERT (bson);
	BSON_ASSERT (!(bson->flags & BSON_FLAG_RDONLY));
	BSON_ASSERT (!(bson->flags & BSON_FLAG_IN_CHILD));

	/*BSON_ASSERT ((child_type == BSON_TYPE_DOCUMENT) ||
				(child_type == BSON_TYPE_ARRAY));*/
	BSON_ASSERT (child);

	uint8_t* pdata = bson_data(bson);
#ifdef _DEBUG
	uint8_t* p = bson_data(bson) + bson->len - 1;
	assert(*p == 0xff);
	*(bson_data(bson) + bson->len - 1) = 0;
#endif // _DEBUG

	const int32_t byte_size = 5;

	if (BSON_UNLIKELY (!bson_grow (bson, byte_size))) 
	{
		return false;
	}

	uint8_t* pbuf = bson_data(bson) +  bson->len - 1;
	memcpy(pbuf, empty, 5);

	pbuf[byte_size] = 0;
	bson->len += byte_size;

	bson_encode_length(bson);

	/*
	* Mark the document as working on a child document so that no
	* further modifications can happen until the caller has called
	* bson_append_{document,array}_end().
	*/
	bson->flags |= BSON_FLAG_IN_CHILD;

	/*
	* Initialize the child bson_t structure and point it at the parents
	* buffers. This allows us to realloc directly from the child without
	* walking up to the parent bson_t.
	*/
	//achild->flags = (BSON_FLAG_CHILD | BSON_FLAG_NO_FREE | BSON_FLAG_STATIC);
	child->flags = (BSON_FLAG_CHILD | BSON_FLAG_NO_FREE | BSON_FLAG_STATIC);

	if ((bson->flags & BSON_FLAG_CHILD)) 
	{
		achild->depth = ((bson_impl_alloc_t *)bson)->depth + 1;
	} 
	else
	{
		achild->depth = 1;
	}

	achild->parent = bson;
	achild->buf = aparent->buf;
	achild->buflen = aparent->buflen;
	achild->offset = aparent->offset + aparent->len - 1 - 5;
	achild->len = 5;
	achild->alloc = NULL;
	achild->alloclen = 0;
	achild->realloc = aparent->realloc;
	achild->realloc_func_ctx = aparent->realloc_func_ctx;

	return true;
}

bool save_value_of_bson_end (bson_t* bson, bson_t* child)
{
   BSON_ASSERT (bson);
   BSON_ASSERT ((bson->flags & BSON_FLAG_IN_CHILD));
   BSON_ASSERT (!(child->flags & BSON_FLAG_IN_CHILD));

   /*
    * Unmark the IN_CHILD flag.
    */
   bson->flags &= ~BSON_FLAG_IN_CHILD;

   /*
    * Now that we are done building the sub-document, add the size to the
    * parent, not including the default 5 byte empty document already added.
    */
   bson->len = (bson->len + child->len - 5);

   /*
    * Ensure we have a \0 byte at the end and proper length encoded at
    * the beginning of the document.
    */
   bson_data(bson)[bson->len - 1] = '\0';
   bson_encode_length (bson);

   return true;
}

void test_save_name_val(void)
{
	bson_t bs;
	bson_t bs_child;

	bson_init(&bs);
	//bson_init(&bs_child);
	//bson_append_int32(&bs, "aaa", -1, 1);
	//save_name(&bs, BSON_TYPE_INT32, "aaa");
	//save_value_of_int32(&bs, 1);

	//bson_append_utf8(&bs, "aaa", -1, "", -1);
	save_name(&bs, BSON_TYPE_UTF8, "aaa");
	//save_value_of_string(&bs, "bbbbb");
	save_value_of_string(&bs, "");

	//uint8_t* pdata = bson_data(&bs);
	
	//bson_append_array_begin(&bs, "aaa", -1, &bs_child);
	//bson_append_int32(&bs_child, "bbb", -1, 10);
	//bson_append_int32(&bs_child, "ccc", -1, 20);
	//bson_append_array_end(&bs, &bs_child);

	//save_name(&bs, BSON_TYPE_ARRAY, "aaa");
	//save_value_of_bson_begin(&bs, &bs_child);
	//bson_append_int32(&bs_child, "bbb", -1, 10);
	//bson_append_int32(&bs_child, "ccc", -1, 20);
	//save_value_of_bson_end(&bs, &bs_child);

	uint8_t* pdata = bson_data(&bs);

	char * str = bson_as_json(&bs, 0);
	std::cout << str << std::endl;
	free(str);

	bson_destroy(&bs);
}

void save_begin_end_test(void)
{
	bson_t bs, bs2;
	bson_init(&bs);
	const uint8_t* pdata = bson_get_data(&bs);
	bson_append_array_begin(&bs, "array", -1, &bs2);
	bson_append_int32(&bs2, "0", -1, 10);
	bson_append_array_end(&bs, &bs2);

	bson_destroy(&bs2);

	char * str = bson_as_json(&bs, 0);
	std::cout << str << std::endl;
	free(str);
	bson_destroy(&bs);
}

void bson_swap_test(void)
{
	bson_t bs1, bs2;
	bson_init(&bs1);
	bson_init(&bs2);

	bson_append_int32(&bs1, "aaa", -1, 10);
	bson_append_int32(&bs2, "bbb", -1, 20);

	bson_t tmp;
	memcpy(&tmp, &bs1, sizeof(bson_t));
	memcpy(&bs1, &bs2, sizeof(bson_t));
	memcpy(&bs2, &tmp, sizeof(bson_t));

	{
		char * str = bson_as_json(&bs1, 0);
		std::cout << str << std::endl;
		free(str);
	}

	{
		char * str = bson_as_json(&bs2, 0);
		std::cout << str << std::endl;
		free(str);
	}
	bson_destroy(&bs1);
	bson_destroy(&bs2);
}

#define ITER_TYPE(i) ((bson_type_t) *((i)->raw + (i)->type))

bool my_bson_iter_next (bson_iter_t *iter)
{
	const uint8_t *data;
	uint32_t o;
	unsigned int len;

	bson_return_val_if_fail (iter, false);

	if (!iter->raw) 
	{
		return false;
	}

	data = iter->raw;
	len = iter->len;

	uint32_t old_off = iter->off;

	iter->off = iter->next_off;
	iter->type = iter->off;
	iter->key = iter->off + 1;
	iter->d1 = 0;
	iter->d2 = 0;
	iter->d3 = 0;
	iter->d4 = 0;

	if(old_off == iter->off)
	{
		goto mark_invalid;
	}

	for (o = iter->off + 1; o < len; o++) 
	{
		if (!data [o])
		{
			iter->d1 = ++o;
			goto fill_data_fields;
		}
	}

	goto mark_invalid;

fill_data_fields:

	switch (ITER_TYPE (iter)) {
	case BSON_TYPE_DATE_TIME:
	case BSON_TYPE_DOUBLE:
	case BSON_TYPE_INT64:
	case BSON_TYPE_TIMESTAMP:
		iter->next_off = o + 8;
		break;
	case BSON_TYPE_CODE:
	case BSON_TYPE_SYMBOL:
	case BSON_TYPE_UTF8:
		{
			uint32_t l;

			if ((o + 4) >= len) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->d2 = o + 4;
			memcpy (&l, iter->raw + iter->d1, sizeof (l));
			l = BSON_UINT32_FROM_LE (l);

			if (l > (len - (o + 4))) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->next_off = o + 4 + l;

			/*
			* Make sure the string length includes the NUL byte.
			*/
			if (BSON_UNLIKELY ((l == 0) || (iter->next_off >= len))) {
			iter->err_off = o;
			goto mark_invalid;
			}

			/*
			* Make sure the last byte is a NUL byte.
			*/
			if (BSON_UNLIKELY ((iter->raw + iter->d2)[l - 1] != '\0')) {
			iter->err_off = o + 4 + l - 1;
			goto mark_invalid;
			}
		}
		break;
	case BSON_TYPE_BINARY:
		{
			uint8_t subtype;
			uint32_t l;

			if (o >= (len - 4)) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->d2 = o + 4;
			iter->d3 = o + 5;

			memcpy (&l, iter->raw + iter->d1, sizeof (l));
			l = BSON_UINT32_FROM_LE (l);

			if (l >= (len - o)) {
			iter->err_off = o;
			goto mark_invalid;
			}

			subtype = *(iter->raw + iter->d2);

			if (subtype == BSON_SUBTYPE_BINARY_DEPRECATED) {
			if (l < 4) {
				iter->err_off = o;
				goto mark_invalid;
			}
			}

			iter->next_off = o + 5 + l;
		}
		break;
	case BSON_TYPE_ARRAY:
	case BSON_TYPE_DOCUMENT:
		{
			uint32_t l;

			if (o >= (len - 4)) {
			iter->err_off = o;
			goto mark_invalid;
			}

			memcpy (&l, iter->raw + iter->d1, sizeof (l));
			l = BSON_UINT32_FROM_LE (l);

			if ((l != 0x05000000 ) && ((l > len) || (l > (len - o)))) 
			{
			iter->err_off = o;
			goto mark_invalid;
			}

			if(l == 0x05000000)
			{
				iter->next_off = o + 5;
			}
			else
			{
				iter->next_off = o + l;
			}
		}
		break;
	case BSON_TYPE_OID:
		iter->next_off = o + 12;
		break;
	case BSON_TYPE_BOOL:
		iter->next_off = o + 1;
		break;
	case BSON_TYPE_REGEX:
		{
			bool eor = false;
			bool eoo = false;

			for (; o < len; o++) {
			if (!data [o]) {
				iter->d2 = ++o;
				eor = true;
				break;
			}
			}

			if (!eor) {
			iter->err_off = iter->next_off;
			goto mark_invalid;
			}

			for (; o < len; o++) {
			if (!data [o]) {
				eoo = true;
				break;
			}
			}

			if (!eoo) {
			iter->err_off = iter->next_off;
			goto mark_invalid;
			}

			iter->next_off = o + 1;
		}
		break;
	case BSON_TYPE_DBPOINTER:
		{
			uint32_t l;

			if (o >= (len - 4)) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->d2 = o + 4;
			memcpy (&l, iter->raw + iter->d1, sizeof (l));
			l = BSON_UINT32_FROM_LE (l);

			if ((l > len) || (l > (len - o))) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->d3 = o + 4 + l;
			iter->next_off = o + 4 + l + 12;
		}
		break;
	case BSON_TYPE_CODEWSCOPE:
		{
			uint32_t l;
			uint32_t doclen;

			if ((len < 19) || (o >= (len - 14))) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->d2 = o + 4;
			iter->d3 = o + 8;

			memcpy (&l, iter->raw + iter->d1, sizeof (l));
			l = BSON_UINT32_FROM_LE (l);

			if ((l < 14) || (l >= (len - o))) {
			iter->err_off = o;
			goto mark_invalid;
			}

			iter->next_off = o + l;

			if (iter->next_off >= len) {
			iter->err_off = o;
			goto mark_invalid;
			}

			memcpy (&l, iter->raw + iter->d2, sizeof (l));
			l = BSON_UINT32_FROM_LE (l);

			if (l >= (len - o - 4 - 4)) {
			iter->err_off = o;
			goto mark_invalid;
			}

			if ((o + 4 + 4 + l + 4) >= iter->next_off) {
			iter->err_off = o + 4;
			goto mark_invalid;
			}

			iter->d4 = o + 4 + 4 + l;
			memcpy (&doclen, iter->raw + iter->d4, sizeof (doclen));
			doclen = BSON_UINT32_FROM_LE (doclen);

			if ((o + 4 + 4 + l + doclen) != iter->next_off) {
			iter->err_off = o + 4 + 4 + l;
			goto mark_invalid;
			}
		}
		break;
	case BSON_TYPE_INT32:
		iter->next_off = o + 4;
		break;
	case BSON_TYPE_MAXKEY:
	case BSON_TYPE_MINKEY:
	case BSON_TYPE_NULL:
	case BSON_TYPE_UNDEFINED:
		iter->d1 = -1;
		iter->next_off = o;
		break;
	case BSON_TYPE_EOD:
	default:
		iter->err_off = o;
		goto mark_invalid;
	}

	/*
	* Check to see if any of the field locations would overflow the
	* current BSON buffer. If so, set the error location to the offset
	* of where the field starts.
	*/
	if (iter->next_off >= len) {
		iter->err_off = o;
		goto mark_invalid;
	}

	iter->err_off = 0;

	return true;

mark_invalid:
	iter->raw = NULL;
	iter->len = 0;
	iter->next_off = 0;

	return false;
}

typedef struct
{
	uint32_t       count;
	bool           keys;
	uint32_t       depth;
	bson_string_t *str;
} bson_json_state_t;

bool
my_bson_init_static (bson_t        *bson,
					const uint8_t *data,
					size_t         length)
{
	bson_impl_alloc_t *impl = (bson_impl_alloc_t *)bson;
	uint32_t len_le;

	bson_return_val_if_fail (bson, false);
	bson_return_val_if_fail (data, false);

	const uint8_t empty[5] = {5};

	if(0 == memcmp(data, "\005\0\0\0\0", 5))
	{
		impl->flags = static_cast<bson_flags_t>(BSON_FLAG_STATIC | BSON_FLAG_RDONLY);
		impl->len = (uint32_t)length;
		impl->parent = NULL;
		impl->depth = 0;
		impl->buf = &impl->alloc;
		impl->buflen = &impl->alloclen;
		impl->offset = 0;
		impl->alloc = (uint8_t *)data;
		impl->alloclen = length;
		impl->realloc = NULL;
		impl->realloc_func_ctx = NULL;
		return true;
	}

	if ((length < 5) || (length > INT_MAX)) 
	{
		return false;
	}

	memcpy (&len_le, data, sizeof (len_le));

	if ((size_t)BSON_UINT32_FROM_LE (len_le) != length) 
	{
		return false;
	}

	if (data[length - 1])
	{
		return false;
	}

	impl->flags = static_cast<bson_flags_t>(BSON_FLAG_STATIC | BSON_FLAG_RDONLY);
	impl->len = (uint32_t)length;
	impl->parent = NULL;
	impl->depth = 0;
	impl->buf = &impl->alloc;
	impl->buflen = &impl->alloclen;
	impl->offset = 0;
	impl->alloc = (uint8_t *)data;
	impl->alloclen = length;
	impl->realloc = NULL;
	impl->realloc_func_ctx = NULL;

	return true;
}


static bool
_bson_as_json_visit_utf8 (const bson_iter_t *iter,
							const char        *key,
							size_t             v_utf8_len,
							const char        *v_utf8,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	char *escaped;

	escaped = bson_utf8_escape_for_json (v_utf8, v_utf8_len);

	if (escaped) {
		bson_string_append (state->str, "\"");
		bson_string_append (state->str, escaped);
		bson_string_append (state->str, "\"");
		bson_free (escaped);
		return false;
	}

	return true;
}


static bool
_bson_as_json_visit_int32 (const bson_iter_t *iter,
							const char        *key,
							int32_t       v_int32,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append_printf (state->str, "%" PRId32, v_int32);

	return false;
}


static bool
_bson_as_json_visit_int64 (const bson_iter_t *iter,
							const char        *key,
							int64_t       v_int64,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append_printf (state->str, "%" PRIi64, v_int64);

	return false;
}


static bool
_bson_as_json_visit_double (const bson_iter_t *iter,
							const char        *key,
							double             v_double,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

#ifdef _MSC_VER
	unsigned int current_format = _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

	bson_string_append_printf (state->str, "%.15g", v_double);

#ifdef _MSC_VER
	_set_output_format(current_format);
#endif

	return false;
}


static bool
_bson_as_json_visit_undefined (const bson_iter_t *iter,
								const char        *key,
								void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "{ \"$undefined\" : true }");

	return false;
}


static bool
_bson_as_json_visit_null (const bson_iter_t *iter,
							const char        *key,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "null");

	return false;
}


static bool
_bson_as_json_visit_oid (const bson_iter_t *iter,
							const char        *key,
							const bson_oid_t  *oid,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	char str[25];

	bson_return_val_if_fail (oid, false);

	bson_oid_to_string (oid, str);
	bson_string_append (state->str, "{ \"$oid\" : \"");
	bson_string_append (state->str, str);
	bson_string_append (state->str, "\" }");

	return false;
}


static bool
_bson_as_json_visit_binary (const bson_iter_t  *iter,
							const char         *key,
							bson_subtype_t      v_subtype,
							size_t              v_binary_len,
							const uint8_t *v_binary,
							void               *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	size_t b64_len;
	char *b64;

	b64_len = (v_binary_len / 3 + 1) * 4 + 1;
	b64 = reinterpret_cast<char*>(bson_malloc0 (b64_len));
	b64_ntop (v_binary, v_binary_len, b64, b64_len);

	bson_string_append (state->str, "{ \"$type\" : \"");
	bson_string_append_printf (state->str, "%02x", v_subtype);
	bson_string_append (state->str, "\", \"$binary\" : \"");
	bson_string_append (state->str, b64);
	bson_string_append (state->str, "\" }");
	bson_free (b64);

	return false;
}


static bool
_bson_as_json_visit_bool (const bson_iter_t *iter,
							const char        *key,
							bool        v_bool,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, v_bool ? "true" : "false");

	return false;
}


static bool
_bson_as_json_visit_date_time (const bson_iter_t *iter,
								const char        *key,
								int64_t       msec_since_epoch,
								void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "{ \"$date\" : ");
	bson_string_append_printf (state->str, "%" PRIi64, msec_since_epoch);
	bson_string_append (state->str, " }");

	return false;
}


static bool
_bson_as_json_visit_regex (const bson_iter_t *iter,
							const char        *key,
							const char        *v_regex,
							const char        *v_options,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "{ \"$regex\" : \"");
	bson_string_append (state->str, v_regex);
	bson_string_append (state->str, "\", \"$options\" : \"");
	bson_string_append (state->str, v_options);
	bson_string_append (state->str, "\" }");

	return false;
}


static bool
_bson_as_json_visit_timestamp (const bson_iter_t *iter,
								const char        *key,
								uint32_t      v_timestamp,
								uint32_t      v_increment,
								void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "{ \"$timestamp\" : { \"t\" : ");
	bson_string_append_printf (state->str, "%u", v_timestamp);
	bson_string_append (state->str, ", \"i\" : ");
	bson_string_append_printf (state->str, "%u", v_increment);
	bson_string_append (state->str, " } }");

	return false;
}


static bool
_bson_as_json_visit_dbpointer (const bson_iter_t *iter,
								const char        *key,
								size_t             v_collection_len,
								const char        *v_collection,
								const bson_oid_t  *v_oid,
								void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	char str[25];

	bson_string_append (state->str, "{ \"$ref\" : \"");
	bson_string_append (state->str, v_collection);
	bson_string_append (state->str, "\"");

	if (v_oid) {
		bson_oid_to_string (v_oid, str);
		bson_string_append (state->str, ", \"$id\" : \"");
		bson_string_append (state->str, str);
		bson_string_append (state->str, "\"");
	}

	bson_string_append (state->str, " }");

	return false;
}


static bool
_bson_as_json_visit_minkey (const bson_iter_t *iter,
							const char        *key,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "{ \"$minKey\" : 1 }");

	return false;
}


static bool
_bson_as_json_visit_maxkey (const bson_iter_t *iter,
							const char        *key,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "{ \"$maxKey\" : 1 }");

	return false;
}




static bool
_bson_as_json_visit_before (const bson_iter_t *iter,
							const char        *key,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	char *escaped;

	if (state->count) {
		bson_string_append (state->str, ", ");
	}

	if (state->keys) {
		escaped = bson_utf8_escape_for_json (key, -1);
		if (escaped) {
			bson_string_append (state->str, "\"");
			bson_string_append (state->str, escaped);
			bson_string_append (state->str, "\" : ");
			bson_free (escaped);
		} else {
			return true;
		}
	}

	state->count++;

	return false;
}


static bool
_bson_as_json_visit_code (const bson_iter_t *iter,
							const char        *key,
							size_t             v_code_len,
							const char        *v_code,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	char *escaped;

	escaped = bson_utf8_escape_for_json (v_code, v_code_len);

	if (escaped) {
		bson_string_append (state->str, "\"");
		bson_string_append (state->str, escaped);
		bson_string_append (state->str, "\"");
		bson_free (escaped);
		return false;
	}

	return true;
}


static bool
_bson_as_json_visit_symbol (const bson_iter_t *iter,
							const char        *key,
							size_t             v_symbol_len,
							const char        *v_symbol,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);

	bson_string_append (state->str, "\"");
	bson_string_append (state->str, v_symbol);
	bson_string_append (state->str, "\"");

	return false;
}


static bool
_bson_as_json_visit_codewscope (const bson_iter_t *iter,
								const char        *key,
								size_t             v_code_len,
								const char        *v_code,
								const bson_t      *v_scope,
								void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	char *escaped;

	escaped = bson_utf8_escape_for_json (v_code, v_code_len);

	if (escaped) {
		bson_string_append (state->str, "\"");
		bson_string_append (state->str, escaped);
		bson_string_append (state->str, "\"");
		bson_free (escaped);
		return false;
	}

	return true;
}

static bool
_bson_as_json_visit_document (const bson_iter_t *iter,
								const char        *key,
								const bson_t      *v_document,
								void              *data);

static bool
_bson_as_json_visit_array (const bson_iter_t *iter,
							const char        *key,
							const bson_t      *v_array,
							void              *data);

bool my_bson_iter_visit_all (bson_iter_t          *iter,    /* INOUT */
								const bson_visitor_t *visitor, /* IN */
								void                 *data);    /* IN */


static const bson_visitor_t bson_as_json_visitors = {
	_bson_as_json_visit_before,
	NULL, /* visit_after */
	NULL, /* visit_corrupt */
	_bson_as_json_visit_double,
	_bson_as_json_visit_utf8,
	_bson_as_json_visit_document,
	_bson_as_json_visit_array,
	_bson_as_json_visit_binary,
	_bson_as_json_visit_undefined,
	_bson_as_json_visit_oid,
	_bson_as_json_visit_bool,
	_bson_as_json_visit_date_time,
	_bson_as_json_visit_null,
	_bson_as_json_visit_regex,
	_bson_as_json_visit_dbpointer,
	_bson_as_json_visit_code,
	_bson_as_json_visit_symbol,
	_bson_as_json_visit_codewscope,
	_bson_as_json_visit_int32,
	_bson_as_json_visit_timestamp,
	_bson_as_json_visit_int64,
	_bson_as_json_visit_maxkey,
	_bson_as_json_visit_minkey,
};

#ifndef BSON_MAX_RECURSION
# define BSON_MAX_RECURSION 100
#endif

static bool
_bson_as_json_visit_document (const bson_iter_t *iter,
								const char        *key,
								const bson_t      *v_document,
								void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	bson_json_state_t child_state = { 0, true };
	bson_iter_t child;

	if (state->depth >= BSON_MAX_RECURSION) {
		bson_string_append (state->str, "{ ... }");
		return false;
	}

	if (bson_iter_init (&child, v_document)) {
		child_state.str = bson_string_new ("{ ");
		child_state.depth = state->depth + 1;
		if(!bson_empty(v_document))
		{
			my_bson_iter_visit_all (&child, &bson_as_json_visitors, &child_state);
		}
		bson_string_append (child_state.str, " }");
		bson_string_append (state->str, child_state.str->str);
		bson_string_free (child_state.str, true);
	}

	return false;
}

static bool
_bson_as_json_visit_array (const bson_iter_t *iter,
							const char        *key,
							const bson_t      *v_array,
							void              *data)
{
	bson_json_state_t *state = reinterpret_cast<bson_json_state_t *>(data);
	bson_json_state_t child_state = { 0, false };
	bson_iter_t child;

	if (state->depth >= BSON_MAX_RECURSION) {
		bson_string_append (state->str, "{ ... }");
		return false;
	}

	if (bson_iter_init (&child, v_array)) {
		child_state.str = bson_string_new ("[ ");
		child_state.depth = state->depth + 1;
		if(!bson_empty(v_array))
		{
			my_bson_iter_visit_all (&child, &bson_as_json_visitors, &child_state);
		}
		bson_string_append (child_state.str, " ]");
		bson_string_append (state->str, child_state.str->str);
		bson_string_free (child_state.str, true);
	}

	return false;
}

#define VISIT_FIELD(name) visitor->visit_##name && visitor->visit_##name
#define VISIT_AFTER VISIT_FIELD (after)
#define VISIT_BEFORE VISIT_FIELD (before)
#define VISIT_CORRUPT if (visitor->visit_corrupt) visitor->visit_corrupt
#define VISIT_DOUBLE VISIT_FIELD (double)
#define VISIT_UTF8 VISIT_FIELD (utf8)
#define VISIT_DOCUMENT VISIT_FIELD (document)
#define VISIT_ARRAY VISIT_FIELD (array)
#define VISIT_BINARY VISIT_FIELD (binary)
#define VISIT_UNDEFINED VISIT_FIELD (undefined)
#define VISIT_OID VISIT_FIELD (oid)
#define VISIT_BOOL VISIT_FIELD (bool)
#define VISIT_DATE_TIME VISIT_FIELD (date_time)
#define VISIT_NULL VISIT_FIELD (null)
#define VISIT_REGEX VISIT_FIELD (regex)
#define VISIT_DBPOINTER VISIT_FIELD (dbpointer)
#define VISIT_CODE VISIT_FIELD (code)
#define VISIT_SYMBOL VISIT_FIELD (symbol)
#define VISIT_CODEWSCOPE VISIT_FIELD (codewscope)
#define VISIT_INT32 VISIT_FIELD (int32)
#define VISIT_TIMESTAMP VISIT_FIELD (timestamp)
#define VISIT_INT64 VISIT_FIELD (int64)
#define VISIT_MAXKEY VISIT_FIELD (maxkey)
#define VISIT_MINKEY VISIT_FIELD (minkey)

bool my_bson_iter_visit_all (bson_iter_t          *iter,    /* INOUT */
								const bson_visitor_t *visitor, /* IN */
								void                 *data)    /* IN */
{
	const char *key;

	bson_return_val_if_fail (iter, false);
	bson_return_val_if_fail (visitor, false);

	while (my_bson_iter_next (iter)) {
		key = bson_iter_key_unsafe (iter);

		if (*key && !bson_utf8_validate (key, strlen (key), false)) {
			iter->err_off = iter->off;
			return true;
		}

		if (VISIT_BEFORE (iter, key, data)) {
			return true;
		}

		switch (bson_iter_type (iter)) {
		case BSON_TYPE_DOUBLE:

			if (VISIT_DOUBLE (iter, key, bson_iter_double (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_UTF8:
			{
			uint32_t utf8_len;
			const char *utf8;

			utf8 = bson_iter_utf8 (iter, &utf8_len);

			if (!bson_utf8_validate (utf8, utf8_len, true)) {
				iter->err_off = iter->off;
				return true;
			}

			if (VISIT_UTF8 (iter, key, utf8_len, utf8, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_DOCUMENT:
			{
			const uint8_t *docbuf = NULL;
			uint32_t doclen = 0;
			bson_t b;

			bson_iter_document (iter, &doclen, &docbuf);

			if (my_bson_init_static (&b, docbuf, doclen) &&
					VISIT_DOCUMENT (iter, key, &b, data)) 
			{
				return true;
			}
			}
			break;
		case BSON_TYPE_ARRAY:
			{
			const uint8_t *docbuf = NULL;
			uint32_t doclen = 0;
			bson_t b;

			bson_iter_array (iter, &doclen, &docbuf);

			if (bson_init_static (&b, docbuf, doclen)
				&& VISIT_ARRAY (iter, key, &b, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_BINARY:
			{
			const uint8_t *binary = NULL;
			bson_subtype_t subtype = BSON_SUBTYPE_BINARY;
			uint32_t binary_len = 0;

			bson_iter_binary (iter, &subtype, &binary_len, &binary);

			if (VISIT_BINARY (iter, key, subtype, binary_len, binary, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_UNDEFINED:

			if (VISIT_UNDEFINED (iter, key, data)) {
			return true;
			}

			break;
		case BSON_TYPE_OID:

			if (VISIT_OID (iter, key, bson_iter_oid (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_BOOL:

			if (VISIT_BOOL (iter, key, bson_iter_bool (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_DATE_TIME:

			if (VISIT_DATE_TIME (iter, key, bson_iter_date_time (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_NULL:

			if (VISIT_NULL (iter, key, data)) {
			return true;
			}

			break;
		case BSON_TYPE_REGEX:
			{
			const char *regex = NULL;
			const char *options = NULL;
			regex = bson_iter_regex (iter, &options);

			if (VISIT_REGEX (iter, key, regex, options, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_DBPOINTER:
			{
			uint32_t collection_len = 0;
			const char *collection = NULL;
			const bson_oid_t *oid = NULL;

			bson_iter_dbpointer (iter, &collection_len, &collection, &oid);

			if (VISIT_DBPOINTER (iter, key, collection_len, collection, oid,
									data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_CODE:
			{
			uint32_t code_len;
			const char *code;

			code = bson_iter_code (iter, &code_len);

			if (VISIT_CODE (iter, key, code_len, code, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_SYMBOL:
			{
			uint32_t symbol_len;
			const char *symbol;

			symbol = bson_iter_symbol (iter, &symbol_len);

			if (VISIT_SYMBOL (iter, key, symbol_len, symbol, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_CODEWSCOPE:
			{
			uint32_t length = 0;
			const char *code;
			const uint8_t *docbuf = NULL;
			uint32_t doclen = 0;
			bson_t b;

			code = bson_iter_codewscope (iter, &length, &doclen, &docbuf);

			if (bson_init_static (&b, docbuf, doclen) &&
				VISIT_CODEWSCOPE (iter, key, length, code, &b, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_INT32:

			if (VISIT_INT32 (iter, key, bson_iter_int32 (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_TIMESTAMP:
			{
			uint32_t timestamp;
			uint32_t increment;
			bson_iter_timestamp (iter, &timestamp, &increment);

			if (VISIT_TIMESTAMP (iter, key, timestamp, increment, data)) {
				return true;
			}
			}
			break;
		case BSON_TYPE_INT64:

			if (VISIT_INT64 (iter, key, bson_iter_int64 (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_MAXKEY:

			if (VISIT_MAXKEY (iter, bson_iter_key_unsafe (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_MINKEY:

			if (VISIT_MINKEY (iter, bson_iter_key_unsafe (iter), data)) {
			return true;
			}

			break;
		case BSON_TYPE_EOD:
		default:
			break;
		}

		if (VISIT_AFTER (iter, bson_iter_key_unsafe (iter), data)) {
			return true;
		}
	}

	if (iter->err_off) {
		VISIT_CORRUPT (iter, data);
	}

#undef VISIT_FIELD

	return false;
}

char* my_bson_as_json (const bson_t *bson,
						size_t       *length)
{
	bson_json_state_t state;
	bson_iter_t iter;

	bson_return_val_if_fail (bson, NULL);

	if (length) 
	{
		*length = 0;
	}

	if (bson_empty0 (bson)) {
		if (length) {
			*length = 3;
		}

		return bson_strdup ("{ }");
	}

	if (!bson_iter_init (&iter, bson)) {
		return NULL;
	}

	state.count = 0;
	state.keys = true;
	state.str = bson_string_new ("{ ");
	state.depth = 0;

	if (my_bson_iter_visit_all (&iter, &bson_as_json_visitors, &state) ||
		iter.err_off) {
		/*
		* We were prematurely exited due to corruption or failed visitor.
		*/
		bson_string_free (state.str, true);
		if (length) {
			*length = 0;
		}
		return NULL;
	}

	bson_string_append (state.str, " }");

	if (length) {
		*length = state.str->len;
	}

	return bson_string_free (state.str, false);
}

void out_bson(const bson_t* bs)
{
	char* str = my_bson_as_json(bs, 0);
	if(str)
	{
		std::cout << str << std::endl;
		free(str);
	}
}


void my_bson_iter_nex_test(void)
{
	bson_t bs, mark, mark2;

	bson_init(&bs);

	bson_init(&mark);
	bson_append_array_begin(&bs, "aaa", -1, &mark);
	bson_append_int64(&mark, "0", -1, 3);
	bson_append_int32(&mark, "1", -1, 0);
	bson_append_array_begin(&mark, "2", -1, &mark2);
	bson_append_int32(&mark2, "0", -1, 1);
	bson_append_int32(&mark2, "1", -1, 1);
	bson_append_array_end(&mark, &mark2);
	bson_append_array_begin(&mark, "3", -1, &mark2);
	bson_append_int32(&mark2, "0", -1, 2);
	bson_append_int32(&mark2, "1", -1, 2);
	bson_append_array_end(&mark, &mark2);
	bson_append_array_end(&bs, &mark);


	//while(my_bson_iter_next(&i))
	//{
	//	std::cout << bson_iter_key(&i) << std::endl;
	//	std::cout << "next_in" << std::endl;
	//}

	//bool b = my_bson_iter_next(&i);

	//if(b)
	//{
	//	const char* pname = bson_iter_key(&i);
	//	uint8_t t = bson_iter_type(&i);

	//	std::cout << pname << "\n" << (int)t << std::endl;

	//	bson_t bs2;
	//	const uint8_t* pbs = 0;
	//	uint32_t len = 0;
	//	bson_iter_document(&i, &len, &pbs);
	//	//bson_copy_to(reinterpret_cast<const bson_t*>(pbs), &bs2);
	//	bson_init_static(&bs2, pbs, len);
	//	out_bson(&bs2);
	//	//bson_destroy(&bs2);
	//}

	out_bson(&bs);
	bson_destroy(&bs);
}

int main(int argc, char* argv[])
{
	//bson_iter_test();
	//bson_append_start_test();
	//test_save_name_val();
	//save_begin_end_test();
	//bson_swap_test();

	my_bson_iter_nex_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
