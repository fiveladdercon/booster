#include <string.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

#include "warden_public_key.h"

typedef unsigned char BYTE;

int WARDEN_verify_bytes (const BYTE *msg, size_t mlen, const BYTE *sig) {
	int          rc,result;
	BIGNUM       *n,*e;
	RSA          *rsa;
	EVP_PKEY     *key;
	EVP_MD_CTX   *ctx;
	const EVP_MD *md;

	/* Validate input */

	if (!msg || !mlen || !sig) return 0;

	/* Initialize */

	result = 0;
	n      = NULL;
	e      = NULL;
	rsa    = NULL;
	key    = NULL;
	ctx    = NULL;

	OpenSSL_add_all_algorithms();

	do {

		/* Set the RSA public key */

		BN_hex2bn(&n,WARDEN_N);
		if (n == NULL) break;

		BN_hex2bn(&e,WARDEN_E);
		if (e == NULL) break;
		
		rsa = RSA_new();
		if (rsa == NULL) break;
		
		rc = RSA_set0_key(rsa,n,e,NULL);
		if (rc != 1) break;

		/* Verify the message */

		key = EVP_PKEY_new();
		if (key == NULL) break;

		rc = EVP_PKEY_assign_RSA(key, RSAPublicKey_dup(rsa));
		if (rc != 1) break;

		ctx = EVP_MD_CTX_create();
		if (ctx == NULL) break;

		md = EVP_get_digestbyname("SHA256");
		if (md == NULL) break;

		rc = EVP_DigestInit_ex(ctx,md,NULL);
		if (rc != 1) break;

		rc = EVP_DigestVerifyInit(ctx,NULL,md,NULL,key);
		if (rc != 1) break;

		rc = EVP_DigestVerifyUpdate(ctx,msg,mlen);
		if (rc != 1) break;

		ERR_clear_error();

		rc = EVP_DigestVerifyFinal(ctx,sig,256);
		if (rc != 1) break;

		result = 1;

	} while (0);

	if (rsa) RSA_free(rsa);

	if (key) EVP_PKEY_free(key);

	if (ctx) EVP_MD_CTX_destroy(ctx);

	return result;
}

int WARDEN_verify (const char *imessage, const char *isignature) {
	int     result;
	BYTE   *msg;
	size_t  mlen;
	BIGNUM *hash;
	BYTE    sig[256];
	size_t  slen;

	if (!imessage || !isignature) return 0;

	result = 0;
	msg    = NULL;
	hash   = NULL;

	do {

		msg  = (BYTE *) imessage;
		mlen = strlen(imessage)+1;

		BN_hex2bn(&hash,isignature);
		if (hash == NULL) break;

		slen = BN_bn2binpad(hash,sig,256);
		if (slen != 256) break;

		result = WARDEN_verify_bytes(msg,mlen,sig);

	} while (0);

	if (hash) BN_free(hash);

	return result;
}
