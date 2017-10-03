#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

void print_mac(unsigned char *c, int len, int num);
void print_string(unsigned char *c, int len);

int main(void)
{
	BIO			*bio_file	=	NULL;
	BIO 		*bio_data 	= 	NULL;
	BIO			*bio_cipher	=	NULL;
	BIO			*bio_b64	=	NULL;
	char		*teststr	=	"hello world";
	char		*filename	=	"test.txt";

	unsigned char iv[16];
	unsigned char key[16];	// encryption key
	unsigned char buf[100];

	memset(iv, 0x00, sizeof(iv));
	memset(key, 0x00, sizeof(key));
	memset(buf, 0x00, sizeof(buf));

	OpenSSL_add_all_algorithms();

	do {
		if (!(bio_file = BIO_new_file(filename, "w"))) break;
		if (!(bio_data = BIO_new(BIO_f_buffer()))) break;
		if (!(bio_b64 = BIO_new(BIO_f_base64()))) break;
		if (!(bio_cipher = BIO_new(BIO_f_cipher()))) break;

		BIO_set_cipher(bio_cipher, EVP_aes_128_gcm(), key, iv, 1);

		BIO_push(bio_cipher, bio_b64);
		BIO_push(bio_b64, bio_data);
		BIO_push(bio_data, bio_file);

		int ret = BIO_write(bio_cipher, teststr, strlen(teststr));
		printf("Return Value: %d\n", ret);
		printf("Test string:\n");
		printf("%s\n", teststr);

		BIO_flush(bio_cipher);
		BIO_free_all(bio_cipher);
	} while (0);

	do {
		if (!(bio_file = BIO_new_file(filename, "r"))) break;
		if (!(bio_data = BIO_new(BIO_f_buffer()))) break;
		if (!(bio_b64 = BIO_new(BIO_f_base64()))) break;
		if (!(bio_cipher = BIO_new(BIO_f_cipher()))) break;

		BIO_set_cipher(bio_cipher, EVP_aes_128_gcm(), key, iv, 1);

		BIO_push(bio_cipher, bio_b64);
		BIO_push(bio_b64, bio_data);
		BIO_push(bio_data, bio_file);

		int ret = BIO_read(bio_cipher, buf, strlen(teststr));
		printf("Return Value: %d\n", ret);
		printf("Test string:\n");
		printf("%s\n", teststr);

		BIO_flush(bio_cipher);
		BIO_free_all(bio_cipher);
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
