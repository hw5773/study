#include "test_message.h"
#include <stdio.h>
#include <stdint.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <sys/time.h>
#include <openssl/aes.h>

unsigned long get_current_microseconds()
{
  struct timeval curr;
  gettimeofday(&curr, NULL);

  return curr.tv_sec * 1000000 + curr.tv_usec;
}

int main()
{
  const uint8_t key[] = "01234567890123456789012345678901";
  const uint8_t iv[] = "0123456789012345";
  const uint8_t plain[16384];
  const uint8_t ciph[16384];
  uint32_t len, mlen, plen, clen;
  uint32_t ret;
  unsigned long start, end;

  printf("===== 8KB Encrypt =====\n");
  EVP_CIPHER_CTX *ctx1;
  start = get_current_microseconds();
  ctx1 = EVP_CIPHER_CTX_new();
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_new(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_EncryptInit_ex(ctx1, EVP_aes_128_cbc(), NULL, key, iv);
  end = get_current_microseconds();
  printf("EVP_EncryptInit_ex(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_EncryptUpdate(ctx1, ciph, &len, msgc, strlen(msgc));
  end = get_current_microseconds();
  printf("EVP_EncryptUpdate(): %lu\n", end - start);

  clen = len;
  start = get_current_microseconds();
  ret = EVP_EncryptFinal_ex(ctx1, ciph + len, &len);
  end = get_current_microseconds();
  printf("EVP_EncryptFinal_ex(): %lu\n", end - start);

  clen += len;

  start = get_current_microseconds();
  EVP_CIPHER_CTX_free(ctx1);
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_free(): %lu\n", end - start);

  printf("===== 8KB Decrypt =====\n");
  EVP_CIPHER_CTX *ctx2;
  start = get_current_microseconds();
  ctx2 = EVP_CIPHER_CTX_new();
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_new(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_DecryptInit_ex(ctx2, EVP_aes_128_cbc(), NULL, key, iv);
  end = get_current_microseconds();
  printf("EVP_DecryptInit_ex(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_DecryptUpdate(ctx2, plain, &len, ciph, clen);
  end = get_current_microseconds();
  printf("EVP_DecryptUpdate(): %lu\n", end - start);

  plen = len;
  start = get_current_microseconds();
  ret = EVP_DecryptFinal_ex(ctx2, plain + len, &len);
  end = get_current_microseconds();
  printf("EVP_DecryptFinal_ex(): %lu\n", end - start);

  plen += len;
  start = get_current_microseconds();
  EVP_CIPHER_CTX_free(ctx2);
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_free(): %lu\n", end - start);

  printf("===== 16KB Encrypt =====\n");
  EVP_CIPHER_CTX *ctx3;
  start = get_current_microseconds();
  ctx3 = EVP_CIPHER_CTX_new();
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_new(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_EncryptInit_ex(ctx3,
      EVP_aes_128_cbc(), NULL, key, iv);
  end = get_current_microseconds();
  printf("EVP_EncryptInit_ex(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_EncryptUpdate(ctx3, ciph, &len, msgd, strlen(msgd));
  end = get_current_microseconds();
  printf("EVP_EncryptUpdate(): %lu\n", end - start);

  clen = len;
  start = get_current_microseconds();
  ret = EVP_EncryptFinal_ex(ctx3, ciph + len, &len);
  end = get_current_microseconds();
  printf("EVP_EncryptFinal_ex(): %lu\n", end - start);

  clen += len;

  start = get_current_microseconds();
  EVP_CIPHER_CTX_free(ctx3);
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_free(): %lu\n", end - start);

  printf("===== 16KB Decrypt =====\n");
  EVP_CIPHER_CTX *ctx4;
  start = get_current_microseconds();
  ctx4 = EVP_CIPHER_CTX_new();
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_new(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_DecryptInit_ex(ctx4, EVP_aes_128_cbc(), NULL, key, iv);
  end = get_current_microseconds();
  printf("EVP_DecryptInit_ex(): %lu\n", end - start);

  start = get_current_microseconds();
  EVP_DecryptUpdate(ctx4, plain, &len, ciph, clen);
  end = get_current_microseconds();
  printf("EVP_DecryptUpdate(): %lu\n", end - start);

  plen = len;
  start = get_current_microseconds();
  ret = EVP_DecryptFinal_ex(ctx4, plain + len, &len);
  end = get_current_microseconds();
  printf("EVP_DecryptFinal_ex(): %lu\n", end - start);

  plen += len;
  start = get_current_microseconds();
  EVP_CIPHER_CTX_free(ctx4);
  end = get_current_microseconds();
  printf("EVP_CIPHER_CTX_free(): %lu\n", end - start);

  EVP_MD_CTX *ctx;
  uint8_t h[32];
  int shalen;
  ctx = EVP_MD_CTX_create();
  start = get_current_microseconds();
  EVP_DigestInit(ctx, EVP_sha1());
  EVP_DigestUpdate(ctx, msgd, strlen(msgd));
  EVP_DigestFinal(ctx, h, &shalen);
  end = get_current_microseconds();
  printf("EVP_DigestUpdate(): %lu\n", end - start);

  return 0;
}
