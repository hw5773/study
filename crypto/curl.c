#define _BSD_SOURCE

#define PROTO_ERR			((uint32_t)0x00)
#define PROTO_OK			((uint32_t)0x01)
#define PROTO_NO			((uint32_t)0x02)

#define PROTO_ID			((uint32_t)0x0f)
#define PROTO_EVIDENCE_SIZE	((uint32_t)0x0b)
#define PROTO_EVIDENCE		((uint32_t)0x09)

#define BIO_DECRYPT		0
#define BIO_ENCRYPT		1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <curl/curl.h>
#include <glob.h>

static size_t writefunc(char *ptr, size_t size, size_t nmemb, void *userdata);

int BIO_write_int32(BIO *b, uint32_t i)
{
	return BIO_write(b, &i, sizeof(uint32_t));
}

int main(void)
{
	CURL *curl = curl_easy_init();
	struct curl_slist *hdr = NULL;
	BIO *bio_data = NULL, *bio_req = NULL, *bio_resp = NULL, *bio_cipher = NULL, *bio_md = NULL;
	BIO *bio_cipher2 = NULL;
	char *dataptr = NULL;
	long int datalen;
	int ret = PROTO_ERR;
	char url[256], *data = NULL;
	unsigned char key[16], iv[16], sha[SHA_DIGEST_LENGTH], check[SHA_DIGEST_LENGTH];
	
	int num, i;
	uint32_t cmd;

	unsigned char *user = "hwlee";
	unsigned char *host = "mbox";

	memset(key, '\x00', sizeof(key));
	memset(iv, '\x00', sizeof(iv));

	do {
		if (!(hdr = curl_slist_append(hdr, "Content-Type: application/octet-stream"))) break;
		if (!(hdr = curl_slist_append(hdr, "Expect:"))) break;
		if (curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr)) break;
		if (curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "")) break;
		if (curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc)) break;
	} while (0);

	do {
		if (!(bio_data = BIO_new(BIO_s_mem()))) break;
		if (!(bio_req = BIO_new(BIO_s_mem()))) break;
		if (!(bio_cipher = BIO_new(BIO_f_cipher()))) break;
		if (!(bio_md = BIO_new(BIO_f_md()))) break;

		BIO_set_cipher(bio_cipher, EVP_aes_128_cbc(), key, iv, BIO_ENCRYPT);

		if (!BIO_set_md(bio_md, EVP_sha1())) break;

		BIO_push(bio_cipher, bio_req);
		BIO_push(bio_md, bio_cipher);

		BIO_write(bio_data, user, strlen(user));
		BIO_write(bio_data, host, strlen(host));

		if (bio_data)
		{
			BIO_flush(bio_data);
			datalen = BIO_get_mem_data(bio_data, &dataptr);
		}


//		BIO_write_int32(bio_cipher, PROTO_ID);
		BIO_write_int32(bio_data, PROTO_ID);

//		BIO_gets(bio_md, sha, sizeof(sha));
//		BIO_pop(bio_md);
		BIO_flush(bio_cipher);
		BIO_pop(bio_cipher);

		BIO_write(bio_req, dataptr, datalen);

		if (snprintf(url, sizeof(url), "http://%s:1127/index.html", "147.46.215.152") >= sizeof(url)) break;

		if (!(datalen = BIO_get_mem_data(bio_req, &dataptr))) break;

		if (!(bio_resp = BIO_new(BIO_s_mem()))) break;

		if (curl_easy_setopt(curl, CURLOPT_URL, url)) break;
		if (curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataptr)) break;
		if (curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, datalen)) break;
		if (curl_easy_setopt(curl, CURLOPT_WRITEDATA, bio_resp)) break;
		if (curl_easy_perform(curl)) break;

//		datalen = BIO_get_mem_data(bio_resp, &dataptr);

//		printf("resp datalen: %ld\n", datalen);
//		printf("resp data: %s\n", dataptr);

//		if (BIO_reset(bio_data) != 1) break;
//		if (BIO_reset(bio_md) != 0) break;

		if (!(bio_cipher2 = BIO_new(BIO_f_cipher()))) break;
		memset(iv, '\x00', sizeof(iv));
		BIO_set_cipher(bio_cipher2, EVP_aes_128_cbc(), key, iv, BIO_DECRYPT);
		BIO_push(bio_cipher2, bio_resp);
//		BIO_push(bio_md, bio_cipher);

		unsigned char *data = (unsigned char *)malloc(2);
		BIO_read(bio_resp, data, 1);
		data[1] = 0;

		cmd = atoi(data);

		if (cmd == PROTO_OK)
			printf("proto_id ok!\n");
		else
			printf("proto_id fail!\n");

	} while (0);

	return 0;
}

static size_t writefunc(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return BIO_write((BIO *)userdata, ptr, nmemb * size);
}
