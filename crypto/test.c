#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <json-c/json.h>

#include "so.h"
#include "params.h"

#if 1
#   define ABORT(x) { break; }
#else
#  define ABORT(x) { debugme(x "\n"); break; }
#endif

unsigned char *key = "1ΰ\333\323+\306\330n\r\375z&_\342\023\060\060\060\060\060\060\060\060\060\060\060\060\060\060\060\060";

int main()
{
   json_object *config = NULL;
   BIO *bio_file = NULL, *bio_cipher = NULL, *bio_mem = NULL, *bio_null = NULL, *bio_md = NULL;
   char buf[1024], *ptr = NULL, sha[SHA_DIGEST_LENGTH];
   unsigned char iv[AES_BLOCK_SIZE];
   int len, ret = -1;

   do {
#if 1
      memset(iv, 0x00, sizeof(iv));
      if(!(bio_file = BIO_new_file(".cache", "r"))) ABORT("Unable to open the file");
      if(!(bio_cipher = BIO_new(BIO_f_cipher()))) ABORT("Unable to initialize bio_cipher");
      BIO_set_cipher(bio_cipher, EVP_get_cipherbyname(so58), key, iv, 0);
      BIO_push(bio_cipher, bio_file);

      if(!(bio_mem = BIO_new(BIO_s_mem()))) ABORT("Unable to initialize bio_mem");
      do {
         if((len = BIO_read(bio_cipher, buf, sizeof(buf))) > 0) BIO_write(bio_mem, buf, len);
      } while(len == sizeof(buf));
      if(len < 0) ABORT("Error reading the file");

      bio_null = BIO_new(BIO_s_null());
      bio_md = BIO_new(BIO_f_md());
      if(!BIO_set_md(bio_md, EVP_sha1())) ABORT("Unable to initialize bio_md");
      BIO_push(bio_md, bio_null);

      if((len = BIO_get_mem_data(bio_mem, &ptr)) <= sizeof(sha)) ABORT("Invalid configuration file");
      if(BIO_write(bio_md, ptr, len - sizeof(sha)) != len - sizeof(sha)) ABORT("Error writing to bio_md");
      if(BIO_gets(bio_md, sha, sizeof(sha)) != sizeof(sha)) ABORT("Error reading from bio_md");
      if(memcmp(sha, &ptr[len - sizeof(sha)], sizeof(sha))) ABORT("Invalid checksum");

      ptr[len - sizeof(sha)] = '\0';
      config = json_tokener_parse(ptr);
	  printf("config: %s\n", ptr);
#else
      config = json_object_from_file(filename);
#endif
      ret = 0;
   } while(0);

   if(bio_file) BIO_free(bio_file);
   if(bio_cipher) BIO_free(bio_cipher);
   if(bio_mem) BIO_free(bio_mem);
   if(bio_null) BIO_free(bio_null);
   if(bio_md) BIO_free(bio_md);
   if(config) json_object_put(config);

   return ret;
}
