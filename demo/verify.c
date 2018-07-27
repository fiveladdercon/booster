#include <stdio.h>

/*
 * Import the verify API.
 *
 * Note that this API has the public key hard-coded into it via
 * including the warden_public_key.h.
 *
 * It should also go without saying that despite the best efforts,
 * authorization boils down to a boolean, which can be hijacked
 * by the initiated:
 *
 * e.g. replacing the extern declaration below with:
 *
 * int WARDEN_verify (const char *imessage, const char *isignature) { return 1; };
 *
 * and recompiling, or acheiving the same effect with a couple of carefully
 * inserted processor instructions.
 */
extern int WARDEN_verify (const char *imessage, const char *isignature);

/*
 * Split the signed message into message and signature by reading lines 
 * pairwise.  To simplify the demo the separator is the newline character,
 * but normally the separator needs to be a pattern that will not be
 * in the message. 
 */
int main () {
	char msg[513];
	char sig[513];
	while (fgets(msg,513,stdin) && fgets(sig,513,stdin)) {
		fprintf(stdout, WARDEN_verify(msg,sig) ? "\e[32m" : "\e[31m");
		fprintf(stdout,"%s",msg);
	}
	fprintf(stdout,"\e[0m");
}
