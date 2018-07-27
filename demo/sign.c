#include <stdio.h>

/*
 * Import the sign API.
 *
 * Note that this API has the private key hard-coded into it
 * via including the warden_private_key.h, so it should not
 * be reference in the public application.
 */
extern int WARDEN_sign (const char *imessage, char *osignature);

/*
 * To simplify the demo, each *line* in the input file is signed,
 * and then the "signed line" is transmitted as two consecutive lines
 * for simplified message/signature separation in the application.
 * 
 * The correct approach is to sign all lines once as single message,
 * but that adds complexity beyond the scope of a simple demo:  
 *
 * i.e. Buffering the entire file is an option, but so is signing 
 * line-by-line using the underlying EVP_Init/Sign/Final crypto calls.  
 * It may even be that the message is not even in a file.  And all 
 * options require a message/signature separator that will not occur
 * in the message that the application can reliably use to split 
 * the signed message into it's two parts...
 */
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
