#include <stdio.h>
#include <openssl/bio.h>

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
	BIO *bio_data = NULL;
	char *test = "abcdefhijklmnopqrstuvwxyz0123456789";
	char *user = "hwlee";
	char *host = "mbox3";

	do {
		if (!(bio_data = BIO_new(BIO_s_mem()))) break;

		BIO_write_int32(bio_data, test);
		BIO_write_string(bio_data, user);
		BIO_write_string(bio_data, host);
		BIO_write_string(bio_data, "");
	} while (0);

	return 0;
}
