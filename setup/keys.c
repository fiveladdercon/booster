#include <stdio.h>

#include <openssl/rsa.h>

void print_param (FILE *f, const char *param, const BIGNUM *value) {
	fprintf(f,"#define KEYPARAM_%-4s \"",param);
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

		pub = fopen("public/key.h","w");
		if (pub == NULL) break;

		prv = fopen("private/key.h","w");
		if (prv == NULL) break;

		/* generate a key */

		/*
			The following call generates and compile note.
			Upgrading requires digging into a new calling
			convention, which isn't worth it since all
			we're doing here is generating and dumping
			a public/private key pair (once).
		*/

		rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
		if (rsa == NULL) break;

		/* get the parameters */

		RSA_get0_key(rsa,&n,&e,&d);
		RSA_get0_factors(rsa,&p,&q);
		RSA_get0_crt_params(rsa,&dmp1,&dmp2,&iqmp);

		/* print the public parameters */
		
		print_param(pub,"N",n);
		print_param(pub,"E",e);

		/* print the private parameters */

		print_param(prv,"N",n);
		print_param(prv,"E",e);
		print_param(prv,"D",d);

		print_param(prv,"P",p);
		print_param(prv,"Q",q);

		print_param(prv,"DMP1",dmp1);
		print_param(prv,"DMP2",dmp2);
		print_param(prv,"IQMP",iqmp);

		result = 0;

	} while (0);

	if (pub) fclose(pub);
	if (prv) fclose(prv);

	return result;

}
