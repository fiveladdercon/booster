#include <stdio.h>

/*
 * Import the verify API
 */
extern int WARDEN_verify (const char *imessage, const char *isignature);

/*
 * Split the signed message into message and signature by reading lines 
 * pairwise.
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
