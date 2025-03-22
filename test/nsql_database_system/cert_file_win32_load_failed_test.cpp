// cert_file_win32_load_failed_test.cpp


#include <test/wait_any_key/wait_any_key.hpp>

#if defined(_MSC_VER)

#include <cassert>
#include <stdio.h>
#include <windows.h>

#include <vld.h>

#	pragma comment (lib, "Crypt32.lib")


PCCERT_CONTEXT my_mongoc_secure_channel_setup_certificate_from_file (const char *filename)
{
	char *pem;
	FILE *file;
	BOOL success;
	HCRYPTKEY hKey;
	long pem_length;
	HCRYPTPROV provider;
	CERT_BLOB public_blob;
	CERT_BLOB private_blob;
	const char *pem_public;
	const char *pem_private;
	LPBYTE blob_private = NULL;
	PCCERT_CONTEXT cert = NULL;
	DWORD blob_private_len = 0;
	DWORD encrypted_private_len = 0;
	LPBYTE encrypted_private = NULL;
	std::string str_pem_private;
	std::string::iterator new_end;


	file = fopen (filename, "rb");
	if (!file) {
		printf ("Couldn't open file '%s'", filename);
		return 0;
	}

	fseek (file, 0, SEEK_END);
	pem_length = ftell (file);
	fseek (file, 0, SEEK_SET);
	if (pem_length < 1) {
		printf ("Couldn't determine file size of '%s'", filename);
		return 0;
	}

	pem = (char *) malloc (pem_length);
	memset(pem, 0, pem_length);

	fread ((void *) pem, 1, pem_length, file);
	fclose (file);

	pem_public = strstr (pem, "-----BEGIN CERTIFICATE-----");
	pem_private = strstr (pem, "-----BEGIN ENCRYPTED PRIVATE KEY-----");

	if (pem_private) {
		printf ("Detected unsupported encrypted private key");
		goto fail;
	}

	pem_private = strstr (pem, "-----BEGIN RSA PRIVATE KEY-----");
	if (!pem_private) {
		pem_private = strstr (pem, "-----BEGIN PRIVATE KEY-----");
	}

	if (!pem_private) {
		printf ("Can't find private key in '%s'", filename);
		goto fail;
	}

	public_blob.cbData = (DWORD) strlen (pem_public);
	public_blob.pbData = (BYTE *) pem_public;

	private_blob.cbData = (DWORD) strlen (pem_private);
	private_blob.pbData = (BYTE *) pem_private;

	/* https://msdn.microsoft.com/en-us/library/windows/desktop/aa380264%28v=vs.85%29.aspx
	*/
	CryptQueryObject (
		CERT_QUERY_OBJECT_BLOB,      /* dwObjectType, blob or file */
		&public_blob,                /* pvObject, Unicode filename */
		CERT_QUERY_CONTENT_FLAG_ALL, /* dwExpectedContentTypeFlags */
		CERT_QUERY_FORMAT_FLAG_ALL,  /* dwExpectedFormatTypeFlags */
		0,                           /* dwFlags, reserved for "future use" */
		NULL,                        /* pdwMsgAndCertEncodingType, OUT, unused */
		NULL, /* pdwContentType (dwExpectedContentTypeFlags), OUT, unused */
		NULL, /* pdwFormatType (dwExpectedFormatTypeFlags,), OUT, unused */
		NULL, /* phCertStore, OUT, HCERTSTORE.., unused, for now */
		NULL, /* phMsg, OUT, HCRYPTMSG, only for PKC7, unused */
		(const void **) &cert /* ppvContext, OUT, the Certificate Context */
	);

	if (!cert) {
		printf ("Failed to extract public key from '%s'. Error 0x%.8X",
					filename,
					(unsigned int) GetLastError ());
		goto fail;
	}

	/* https://msdn.microsoft.com/en-us/library/windows/desktop/aa380285%28v=vs.85%29.aspx
	*/

	//str_pem_private.assign(pem_private);
	//new_end = std::remove(str_pem_private.begin(), str_pem_private.end(), '\r');
	//new_end = std::remove(str_pem_private.begin(), new_end, '\n');
	//str_pem_private.erase(new_end, str_pem_private.end());
	//pem_private = str_pem_private.data();

	success =
		CryptStringToBinaryA (pem_private,               /* pszString */
							0,                         /* cchString */
							CRYPT_STRING_BASE64HEADER, /* dwFlags */
							NULL,                      /* pbBinary */
							&encrypted_private_len,    /* pcBinary, IN/OUT */
							NULL,                      /* pdwSkip */
							NULL);                     /* pdwFlags */
	if (!success) {
		printf ("Failed to convert base64 private key. Error 0x%.8X",
					(unsigned int) GetLastError ());
		goto fail;
	}

	encrypted_private = (LPBYTE) malloc (encrypted_private_len);
	memset(encrypted_private, 0, encrypted_private_len);
	success = CryptStringToBinaryA (pem_private,
									0,
									CRYPT_STRING_BASE64HEADER,
									encrypted_private,
									&encrypted_private_len,
									NULL,
									NULL);
	if (!success) {
		printf ("Failed to convert base64 private key. Error 0x%.8X",
					(unsigned int) GetLastError ());
		goto fail;
	}

	/* https://msdn.microsoft.com/en-us/library/windows/desktop/aa379912%28v=vs.85%29.aspx
	*/

	//success = CryptDecodeObjectEx (
	//	X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, /* dwCertEncodingType */
	//	PKCS_RSA_PRIVATE_KEY,                    /* lpszStructType */
	//	encrypted_private,                       /* pbEncoded */
	//	encrypted_private_len,                   /* cbEncoded */
	//	0,                                       /* dwFlags */
	//	NULL,                                    /* pDecodePara */
	//	NULL,                                    /* pvStructInfo */
	//	&blob_private_len);                      /* pcbStructInfo */

	success = CryptDecodeObjectEx (
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, /* dwCertEncodingType */
		PKCS_RSA_PRIVATE_KEY,                    /* lpszStructType */
		encrypted_private,                       /* pbEncoded */
		encrypted_private_len,                   /* cbEncoded */
		0,                                       /* dwFlags */
		NULL,                                    /* pDecodePara */
		NULL,                                    /* pvStructInfo */
		&blob_private_len);                      /* pcbStructInfo */

	if (!success) {
		LPTSTR msg = NULL;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_ARGUMENT_ARRAY,
						NULL,
						GetLastError (),
						LANG_NEUTRAL,
						(LPTSTR) &msg,
						0,
						NULL);
		printf ("Failed to parse private key. %s (0x%.8X)",
					msg,
					(unsigned int) GetLastError ());
		LocalFree (msg);
		goto fail;
	}

	blob_private = (LPBYTE) malloc (blob_private_len);
	memset(blob_private, 0, blob_private_len);
	success = CryptDecodeObjectEx (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
									PKCS_RSA_PRIVATE_KEY,
									encrypted_private,
									encrypted_private_len,
									0,
									NULL,
									blob_private,
									&blob_private_len);
	if (!success) {
		printf ("Failed to parse private key. Error 0x%.8X",
					(unsigned int) GetLastError ());
		goto fail;
	}

	/* https://msdn.microsoft.com/en-us/library/windows/desktop/aa379886%28v=vs.85%29.aspx
	*/
	success = CryptAcquireContext (&provider,            /* phProv */
									NULL,                 /* pszContainer */
									MS_ENHANCED_PROV,     /* pszProvider */
									PROV_RSA_FULL,        /* dwProvType */
									CRYPT_VERIFYCONTEXT); /* dwFlags */
	if (!success) {
		printf ("CryptAcquireContext failed with error 0x%.8X",
					(unsigned int) GetLastError ());
		goto fail;
	}

	/* https://msdn.microsoft.com/en-us/library/windows/desktop/aa380207%28v=vs.85%29.aspx
	*/
	success = CryptImportKey (provider,         /* hProv */
								blob_private,     /* pbData */
								blob_private_len, /* dwDataLen */
								0,                /* hPubKey */
								0,                /* dwFlags */
								&hKey);           /* phKey, OUT */
	if (!success) {
		printf ("CryptImportKey for private key failed with error 0x%.8X",
					(unsigned int) GetLastError ());
		goto fail;
	}

	/* https://msdn.microsoft.com/en-us/library/windows/desktop/aa376573%28v=vs.85%29.aspx
	*/
	success = CertSetCertificateContextProperty (
		cert,                         /* pCertContext */
		CERT_KEY_PROV_HANDLE_PROP_ID, /* dwPropId */
		0,                            /* dwFlags */
		(const void *) provider);     /* pvData */
	if (success) {
		printf ("%s", "Successfully loaded client certificate\n");
		//return cert;
		goto fail;
	}

	printf ("Can't associate private key with public key: 0x%.8X",
					(unsigned int) GetLastError ());

fail:
	SecureZeroMemory (pem, pem_length);
	if(pem) free (pem);
	if (encrypted_private) {
		SecureZeroMemory (encrypted_private, encrypted_private_len);
		if(encrypted_private) free (encrypted_private);
	}

	if (blob_private) {
		SecureZeroMemory (blob_private, blob_private_len);
		if(blob_private) free(blob_private);
	}

	//if(cert)
	//{
	//	CertFreeCertificateContext(cert);
	//}

	return NULL;
}

#else

void* my_mongoc_secure_channel_setup_certificate_from_file (const char *filename)
{
	return 0;
}

#endif //_MSC_VER

int main(int argc, char* argv[])
{
	my_mongoc_secure_channel_setup_certificate_from_file("./nsql_database_system/cert/client.pem");
	my_mongoc_secure_channel_setup_certificate_from_file("./nsql_database_system/cert/server.pem");
	my_mongoc_secure_channel_setup_certificate_from_file("./nsql_database_system/cert/ca.pem");

	wait_any_key(argc, argv);
	return 0;
}
