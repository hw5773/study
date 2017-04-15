#include <stdio.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/bio.h>

int main(int argc, char *argv[])
{
	FILE *f = fopen("ecc_server.crt", "r");
	if (f != NULL)
	{
		X509 *x509 = PEM_read_X509(f, NULL, NULL, NULL);
		if (x509 != NULL)
		{
			char *p = X509_NAME_oneline(X509_get_issuer_name(x509), 0, 0);
			if (p)
			{
				printf("NAME: %s\n", p);
				OPENSSL_free(p);
			}
			X509_free(x509);

			char *q = X509_
		}
	}
	return 0;
}
