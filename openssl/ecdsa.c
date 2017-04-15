#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_ECDSA
int main(int argc, char *argv[])
{
	printf("EC Curves are disabled.\n");
	return 1;
}
#else
#include <openssl/ecdsa.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

int main(int argc, char *argv[])
{
	int 			ret, i;
	ECDSA_SIG 		*sig;
	EC_KEY 			*eckey;
	char			*teststr	=	"hello world!";
	BIO				*bio_md		=	NULL;
	unsigned char	sha[SHA_DIGEST_LENGTH];
	eckey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);

	do {
		if (!(bio_md = BIO_new(BIO_f_md()))) break;
		if (!BIO_set_md(bio_md, EVP_sha1())) break;
	} while (0);

	BIO_write(bio_md, teststr, strlen(teststr));
	BIO_gets(bio_md, sha, sizeof(sha));

	for (i=0; i<SHA_DIGEST_LENGTH; i++)
	{
		if (i % 5 == 0)
			printf("\n");
		printf("%02x\t ", sha[i]);
	}
	printf("\n");

	if (eckey == NULL)
	{
		printf("EC_KEY is wrongly generated.\n");
		return 1;
	}

	if (!EC_KEY_generate_key(eckey))
	{
		printf("EC_KEY is wrongly generated.\n");
		return 1;
	}

	sig = ECDSA_do_sign(sha, 20, eckey);
	if (sig == NULL)
	{
		printf("Signature is wrong.\n");
		return 1;
	}

	ret = ECDSA_do_verify(sha, 20, sig, eckey);

	if (ret == -1)
	{
		printf("Error\n");
		return 1;
	}
	else if (ret == 0)
	{
		printf("Signature is incorrect\n");
		return 1;
	}
	else
	{
		printf("Signature verify success\n");
		return 1;
	}
}
#endif
