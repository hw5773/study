#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

void print_mac(unsigned char *c, int len, int num);
void print_string(unsigned char *c, int len);

int main(void)
{
	BIO			*bio_md		=	NULL;

	char		*teststr	=	"hello world";

	unsigned char iv[16];
	unsigned char key[16];	// encryption key
	unsigned char rkey[16];	// read MAC key for mcTLS
	unsigned char wkey[16];	// write MAC key for mcTLS
	unsigned char ekey[16];	// endpoint MAC key for mcTLS
	unsigned char sha[SHA_DIGEST_LENGTH];

	unsigned char buf[100];

	memset(iv, 0x00, sizeof(iv));
	memset(key, 0x00, sizeof(key));
	memset(rkey, 0x01, sizeof(rkey));
	memset(wkey, 0x02, sizeof(wkey));
	memset(ekey, 0x03, sizeof(ekey));
	memset(buf, 0x00, sizeof(buf));

	int ret;

	do {
		if (!(bio_md = BIO_new(BIO_f_md()))) break;
		if (!BIO_set_md(bio_md, EVP_sha1())) break;

		ret = BIO_write(bio_md, teststr, strlen(teststr));
		BIO_gets(bio_md, sha, SHA_DIGEST_LENGTH);

		printf("SHA Result 1: %d\n", ret);
		print_mac(sha, SHA_DIGEST_LENGTH, 10);

		ret += BIO_write(bio_md, teststr, strlen(teststr));
		BIO_gets(bio_md, sha, SHA_DIGEST_LENGTH);

		printf("SHA Result 2: %d\n", ret);
		print_mac(sha, SHA_DIGEST_LENGTH, 10);

		BIO_reset(bio_md);
		unsigned char *test = (unsigned char *)malloc(strlen(teststr) * 2);
		memcpy(test, teststr, strlen(teststr));
		memcpy(test + strlen(teststr), teststr, strlen(teststr));
		ret = BIO_write(bio_md, test, strlen(test));
		BIO_gets(bio_md, sha, SHA_DIGEST_LENGTH);

		printf("SHA Result 3: %d\n", ret);
		print_mac(sha, SHA_DIGEST_LENGTH, 10);
	} while (0);

	return 0;
}

void make_mac(unsigned char *key, int klen, unsigned char *msg, int mlen, BIO *mac)
{
	
}

void print_mac(unsigned char *c, int len, int num)
{
	int i;
	for (i=0; i<len; i++)
	{
		if (i % num == 0)
			printf("\n");
		printf("%02x\t", c[i]);
	}

	printf("\n");
}

void print_string(unsigned char *c, int len)
{
	int i;
	for (i=0; i<len; i++)
		printf("%c", c[i]);
	printf("\n");
}
