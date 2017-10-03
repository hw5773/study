#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <time.h>
#include <sys/time.h>

#define n2s(c,s)        ((s=(((unsigned int)((c)[0]))<< 8)| \
                            (((unsigned int)((c)[1]))    )),c+=2)

#define s2n(s,c)        ((c[0]=(unsigned char)(((s)>> 8)&0xff), \
                          c[1]=(unsigned char)(((s)    )&0xff)),c+=2)

#define l2n3(l,c)       ((c[0]=(unsigned char)(((l)>>16)&0xff), \
                          c[1]=(unsigned char)(((l)>> 8)&0xff), \
                          c[2]=(unsigned char)(((l)    )&0xff)),c+=3)

#define n2d4(c,d)       (d =((uint32_t)(*((c)++)))<<24, \
						 d|=((uint32_t)(*((c)++)))<<16, \
						 d|=((uint32_t)(*((c)++)))<< 8, \
						 d|=((uint32_t)(*((c)++))))

#define d2n4(d,c)		(*((c)++)=(unsigned char)(((d)>>24)&0xff), \
						 *((c)++)=(unsigned char)(((d)>>16)&0xff), \
						 *((c)++)=(unsigned char)(((d)>> 8)&0xff), \
						 *((c)++)=(unsigned char)(((d)    )&0xff))

#define n2t8(c,t)       (t =((uint64_t)(*((c)++)))<<56, \
                         t|=((uint64_t)(*((c)++)))<<48, \
                         t|=((uint64_t)(*((c)++)))<<40, \
                         t|=((uint64_t)(*((c)++)))<<32, \
                         t|=((uint64_t)(*((c)++)))<<24, \
                         t|=((uint64_t)(*((c)++)))<<16, \
                         t|=((uint64_t)(*((c)++)))<< 8, \
                         t|=((uint64_t)(*((c)++))))

#define t2n8(l,c)       (*((c)++)=(unsigned char)(((l)>>56)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>48)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>40)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>32)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>24)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>16)&0xff), \
                         *((c)++)=(unsigned char)(((l)>> 8)&0xff), \
                         *((c)++)=(unsigned char)(((l)    )&0xff))

int make_warrant_content_body(BIO *out, unsigned char **warrant, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub, int nid, int *len);
int make_warrant_request(BIO *out, unsigned char **request, unsigned char *warrant, int warrant_len, EVP_PKEY *carol_priv, int nid, int *len);
int make_warrant_request_with_verify_warrant(BIO *out, unsigned char **request, unsigned char *warrant, int warrant_len, EVP_PKEY *carol_priv, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub, int nid, int *len);
int make_warrant_response(BIO *out, unsigned char **response, unsigned char *request, int req_len, EVP_PKEY *alice_priv, int nid, int *len);
int make_warrant_response_with_verify_warrant(BIO *out, unsigned char **response, unsigned char *request, int req_len, EVP_PKEY *alice_priv, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub, int nid, int *len);
int make_signature_block(BIO *out, unsigned char **sigblk, unsigned char *msg, int msg_len, EVP_PKEY *priv, int nid, size_t *sigblk_len);

int verify_warrant_content_body(BIO *out, unsigned char *content, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub);
int verify_warrant_request(BIO *out, unsigned char *request, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub);
int verify_warrant_response(BIO *out, unsigned char *response, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub);
int verify_signature(BIO *out, unsigned char *msg, int msg_len, uint16_t sig_type, uint16_t sig_len, unsigned char *sig, EVP_PKEY *pub);


