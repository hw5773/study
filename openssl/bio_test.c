#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

#define PROTO_ID	((uint32_t)0x0f)

int BIO_write_int32(BIO *b, uint32_t i)
{
	return BIO_write(b, &i, sizeof(uint32_t));
}

int BIO_write_string(BIO *b, char *s)
{
	int ret;

	ret = BIO_write_int32(b, (strlen(s) + 1) * 2);

	while (*s)
	{
		ret += BIO_write(b, s, 1);
		ret += BIO_write(b, "\0", 1);
		s++;
	}

	ret += BIO_write(b, "\0\0", 2);

	return ret;
}

int main(void)
{
	BIO 		*bio_data 	= 	NULL;
	BIO			*bio_req	=	NULL;
	BIO			*bio_cipher	=	NULL;
	BIO			*bio_md		=	NULL;

//	uint32_t	test		=	10000;
//	char 		*user 		= 	"hwlee";
//	char 		*host 		= 	"mbox3";
	char 		*dataptr;
	unsigned char		sha[SHA_DIGEST_LENGTH] = {0, };
	long int 	datalen 	= 	0;
//	char		*teststr	=	"hello world";
	char		*teststr	=	"  ";
//	unsigned char end[16];
	unsigned char iv[16];
	unsigned char key[16];
	unsigned char blank[4] = "    ";
	

	memset(iv, 0x00, sizeof(iv));
	memset(key, 0x00, sizeof(key));

	do {
		if (!(bio_data = BIO_new(BIO_s_mem()))) break;
		if (!(bio_req = BIO_new(BIO_s_mem()))) break;
		if (!(bio_cipher = BIO_new(BIO_f_cipher()))) break;
		if (!(bio_md = BIO_new(BIO_f_md()))) break;

		BIO_set_cipher(bio_cipher, EVP_aes_128_cbc(), key, iv, 1);

		if (!BIO_set_md(bio_md, EVP_sha1())) break;
		BIO_push(bio_md, bio_req);
		BIO_write(bio_md, teststr, strlen(teststr));

		BIO_gets(bio_md, sha, sizeof(sha));
		BIO_pop(bio_md);
		
		int i=0;
		printf("SHA_DIGEST_LENGTH: %d\n", SHA_DIGEST_LENGTH);
		printf("sha: ");
	
		for (i=0; i<SHA_DIGEST_LENGTH; i++)
		{
			if (i % 5 == 0)
				printf("\n");
			printf("%02x\t ", sha[i]);
		}
		printf("\n");	

		unsigned char get_md_str[11];
		BIO_reset(bio_md);
		BIO_push(bio_md, bio_req);
		BIO_read(bio_md, get_md_str, 11);
//		BIO_read(bio_req, get_str, 11);

		printf("get md str: ");
		for (i=0; i<sizeof(get_md_str); i++)
			printf("%c", get_md_str[i]);
		printf("\n");

		unsigned char sha2[SHA_DIGEST_LENGTH] = {0, };
		BIO_gets(bio_md, sha2, sizeof(sha2));

		printf("sha2: ");
		for (i=0; i<SHA_DIGEST_LENGTH; i++)
		{
			if (i % 5 == 0)
				printf("\n");
			printf("%02x\t", sha2[i]);
		}

		printf("\n");
	
	} while (0);

	return 0;
}
