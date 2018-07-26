#include <stdio.h>

#include <openssl/rsa.h>

void WARDEN_param (FILE *f, const char *param, const BIGNUM *value) {
	fprintf(f,"#define WARDEN_%-4s \"",param);
	BN_print_fp(f,value);
	fprintf(f,"\"\n");
}

int main () {

	int result;
	const BIGNUM *n,*e,*d,*p,*q,*dmp1,*dmp2,*iqmp;
	RSA *rsa;
	FILE *pub,*prv;

	/* Initialize */

	result = 1;
	n      = NULL;
	e      = NULL;
	d      = NULL;
	p      = NULL;
	q      = NULL;
	dmp1   = NULL;
	dmp2   = NULL;
	iqmp   = NULL;
	rsa    = NULL;
	pub    = NULL;
	prv    = NULL;

	do {

		pub = fopen("warden_public_key.h","w");
		if (pub == NULL) break;

		prv = fopen("warden_private_key.h","w");
		if (prv == NULL) break;

		/* generate a key */

		/*
			The following call generates a compile warning
			regarding the call being deprecated. Upgrading
			requires digging into a new calling convention, 
			which isn't worth it since all we're doing here
			is generating and dumping a public/private key
			pair (once).
		*/

		rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
		if (rsa == NULL) break;

		/* get the parameters */

		RSA_get0_key(rsa,&n,&e,&d);
		RSA_get0_factors(rsa,&p,&q);
		RSA_get0_crt_params(rsa,&dmp1,&dmp2,&iqmp);

		/* print the public parameters */
		
		WARDEN_param(pub,"N",n);
		WARDEN_param(pub,"E",e);

		/* print the private parameters */

		WARDEN_param(prv,"N",n);
		WARDEN_param(prv,"E",e);
		WARDEN_param(prv,"D",d);

		WARDEN_param(prv,"P",p);
		WARDEN_param(prv,"Q",q);

		WARDEN_param(prv,"DMP1",dmp1);
		WARDEN_param(prv,"DMP2",dmp2);
		WARDEN_param(prv,"IQMP",iqmp);

		result = 0;

	} while (0);

	if (pub) fclose(pub);
	if (prv) fclose(prv);

	return result;

}
