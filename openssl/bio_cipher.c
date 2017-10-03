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
	BIO 		*bio_data 	= 	NULL;
	BIO			*bio_enc	=	NULL;
	BIO			*bio_dec	=	NULL;
	BUF_MEM		*bptr		= 	NULL;

	char		*teststr	=	"hello world";
	char		*filename	=	"test.txt";

	int			ret;

	unsigned char iv[16];
	unsigned char key[16];	// encryption key
	unsigned char buf[100];

	memset(iv, 0x00, sizeof(iv));
	memset(key, 0x00, sizeof(key));
	memset(buf, 0x00, sizeof(buf));

	// Encryption
	do {
		if (!(bio_data = BIO_new(BIO_s_mem()))) break;
		if (!(bio_enc = BIO_new(BIO_f_cipher()))) break;

		BIO_set_cipher(bio_enc, EVP_aes_128_cbc(), key, iv, 1);

		BIO_push(bio_enc, bio_data);
		ret = BIO_write(bio_enc, teststr, strlen(teststr));
		BIO_flush(bio_enc);
		BIO_pop(bio_enc);

		printf("Return Value: %d\n", ret);
		printf("Test string:\n");
		printf("%s\n", teststr);

		BIO_get_mem_ptr(bio_data, &bptr);
		printf("Get BUF_MEM: %lu\n", bptr->length);

	} while (0);

	// Decryption
	do {
		BIO *bio_mem;
		BUF_MEM *bptr2;
		bio_mem = BIO_new_mem_buf(bptr->data, bptr->length);

		if (!(bio_dec = BIO_new(BIO_f_cipher()))) break;
		BIO_set_cipher(bio_dec, EVP_aes_128_cbc(), key, iv, 0);
		BIO_push(bio_dec, bio_mem);

		ret = BIO_read(bio_dec, buf, sizeof(buf));
		printf("Return Value: %d\n", ret);
		print_string(buf, ret);
	} while (0);

	return 0;
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
