#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

#include "warden_private_key.h"

typedef unsigned char BYTE;

int WARDEN_sign_bytes (const BYTE *msg, size_t mlen, BYTE *sig) {
	int          rc,result;
	BIGNUM       *n,*e,*d,*p,*q,*dmp1,*dmp2,*iqmp;
	RSA          *rsa;
	EVP_PKEY     *key;
	EVP_MD_CTX   *ctx;
	const EVP_MD *md;
	size_t        slen;

	/* Validate input */

	if (!msg || !mlen || !sig) return 0;

	/* Initialize */

	result = 0;
	n      = NULL;
	e      = NULL;
	d      = NULL;
	p      = NULL;
	q      = NULL;
	dmp1   = NULL;
	dmp2   = NULL;
	iqmp   = NULL;
	rsa    = NULL;
	key    = NULL;
	ctx    = NULL;

	OpenSSL_add_all_algorithms();

	do {

		/* Set the RSA private key */

		BN_hex2bn(&n,WARDEN_N);
		if (n == NULL) break;

		BN_hex2bn(&e,WARDEN_E);
		if (e == NULL) break;

		BN_hex2bn(&d,WARDEN_D);
		if (d == NULL) break;

		BN_hex2bn(&p,WARDEN_P);
		if (p == NULL) break;

		BN_hex2bn(&q,WARDEN_Q);
		if (q == NULL) break;

		BN_hex2bn(&dmp1,WARDEN_DMP1);
		if (dmp1 == NULL) break;

		BN_hex2bn(&dmp2,WARDEN_DMP2);
		if (dmp2 == NULL) break;

		BN_hex2bn(&iqmp,WARDEN_IQMP);
		if (iqmp == NULL) break;

		rsa = RSA_new();

		rc = RSA_set0_key(rsa,n,e,d);
		if (rc != 1) break;

		rc = RSA_set0_factors(rsa,p,q);
		if (rc != 1) break;

		rc = RSA_set0_crt_params(rsa,dmp1,dmp2,iqmp);
		if (rc != 1) break;

		/* Sign the message */

		key = EVP_PKEY_new();
		if (key == NULL) break;

		rc = EVP_PKEY_assign_RSA(key, RSAPrivateKey_dup(rsa));
		if (rc != 1) break;

		ctx = EVP_MD_CTX_create();
		if (ctx == NULL) break;

		md = EVP_get_digestbyname("SHA256");
		if (md == NULL) break;

		rc = EVP_DigestInit_ex(ctx,md,NULL);
		if (rc != 1) break;

		rc = EVP_DigestSignInit(ctx,NULL,md,NULL,key);
		if (rc != 1) break;

		rc = EVP_DigestSignUpdate(ctx,msg,mlen);
		if (rc != 1) break;

		slen = 256;
		rc   = EVP_DigestSignFinal(ctx,sig,&slen);
		if (rc   != 1  ) break;
		if (slen != 256) break;

		result = 1;

	} while (0);

	if (rsa) RSA_free(rsa);

	if (key) EVP_PKEY_free(key);

	if (ctx) EVP_MD_CTX_destroy(ctx);

	return result;

}

int WARDEN_sign (const char *imessage, char *osignature) {
	int     rc,result;
	BYTE   *msg;
	size_t  mlen;
	BIGNUM *hash;
	BYTE    sig[256];

	if (!imessage || !osignature) return 0;

	result = 0;
	msg    = NULL;
	hash   = NULL;

	do {

		msg  = (BYTE *) imessage;
		mlen = strlen(imessage)+1;

		rc = WARDEN_sign_bytes(msg,mlen,sig);
		if (rc != 1) break;

		hash = BN_bin2bn(sig,256,hash);
		if (hash == NULL) break;

		strcpy(osignature,BN_bn2hex(hash));

		result = 1;

	} while (0);

	if (hash) BN_free(hash);

	return result;
}

#ifdef WARDEN_DEMO
int main () {
	char msg[513];
	char sig[513];
	while (fgets(msg,513,stdin)) {
		if (WARDEN_sign(msg,sig)) {
			fprintf(stdout,"%s",msg);
			fprintf(stdout,"%s\n",sig);
		}
	}
}
#endif

