#include "logger.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <pthread.h>
#include <openssl/opensslv.h>

#define FAIL    -1

void *run(void *data);
int open_connection(const char *hostname, int port);
SSL_CTX* init_client_CTX(void);
void load_certificates(BIO *outbio, SSL_CTX* ctx, char* cert_file, char* key_file);
void print_pubkey(BIO *outbio, EVP_PKEY *pkey);
SSL_CTX *ctx;
char *hostname, *portnum;
BIO *bio_err;

// Warrant Client Implementation
int main(int count, char *strings[])
{   
    if ( count != 4 )
    {
        printf("usage: %s <hostname> <portnum> <num of threads>\n", strings[0]);
        exit(0);
    }

	int i, rc, num_of_threads;

	num_of_threads = atoi(strings[3]);

	pthread_t thread[num_of_threads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	void *status;

    SSL_library_init();
    hostname = strings[1];
    portnum = strings[2];

	bio_err = BIO_new_fp(stdout, BIO_NOCLOSE);

    ctx = init_client_CTX();

	unsigned long start, end;

	start = get_current_microseconds();
	for (i=0; i<num_of_threads; i++)
	{
		rc = pthread_create(&thread[i], &attr, run, NULL);

		if (rc)
		{
			printf("ERROR: return code from pthread_create: %d\n", rc);
			return 1;
		}
	}

	pthread_attr_destroy(&attr);

	for (i=0; i<num_of_threads; i++)
	{
		rc = pthread_join(thread[i], &status);

		if (rc)
		{
			printf("ERROR: return code from pthread_join: %d\n", rc);
			return 1;
		}
	}
	end = get_current_microseconds();

	printf("Start Time: %lu\n", start);
	printf("End Time: %lu\n", end);
	printf("TOTAL TIME: %lu\n", end - start);

	SSL_CTX_free(ctx);        /* release context */
    
    return 0;
}

void *run(void *data)
{	
	int server;
	SSL *ssl;

	server = open_connection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */

    struct timeval tv;
    gettimeofday( &tv, 0 );

	unsigned long hs_start, hs_end;
	printf("PROGRESS: TLS Handshake Start\n");
	hs_start = get_current_microseconds();
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
      	ERR_print_errors_fp(stderr);
	else
	{
		hs_end = get_current_microseconds();
    	printf("PROGRESS: TLS Handshake Complete!\nConnected with %s encryption\n", SSL_get_cipher(ssl));
		printf("ELAPSED TIME: %lu us\n", hs_end - hs_start);
	}
        
	SSL_free(ssl);        /* release connection state */
       
	close(server);         /* close socket */
}

int open_connection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
            
    if ( (host = gethostbyname(hostname)) == NULL )
    {
          perror(hostname);
          abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
         close(sd);
         perror(hostname);
         abort();
    }
         return sd;
}

void apps_ssl_info_callback(const SSL *s, int where, int ret)
{
	const char *str;
	int w;

	w = where & ~SSL_ST_MASK;

	if (w & SSL_ST_CONNECT) str = "SSL_connect";
	else if (w & SSL_ST_ACCEPT) str = "SSL_accept";
	else str = "Undefined";

	if (where & SSL_CB_LOOP)
	{
		BIO_printf(bio_err, "%s:%s\n", str, SSL_state_string_long(s));
	}
	else if (where & SSL_CB_ALERT)
	{
		str = (where & SSL_CB_READ)? "read" : "write";
		BIO_printf(bio_err, "SSL3 alert %s:%s:%s\n",
				str,
				SSL_alert_type_string_long(ret),
				SSL_alert_desc_string_long(ret));
	}
	else if (where & SSL_CB_EXIT)
	{
		if (ret == 0)
			BIO_printf(bio_err, "%s:failed in %s\n",
				str, SSL_state_string_long(s));
		else if (ret < 0)
		{
			BIO_printf(bio_err, "%s:error in %s\n",
				str, SSL_state_string_long(s));
		}
	}
}

SSL_CTX* init_client_CTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;
        
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = (SSL_METHOD *)TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
         ERR_print_errors_fp(stderr);
         abort();
    }

	SSL_CTX_set_info_callback(ctx, apps_ssl_info_callback);

    return ctx;
}
 
void load_certificates(BIO *outbio, SSL_CTX* ctx, char* cert_file, char* key_file)
{
	if ( SSL_CTX_load_verify_locations(ctx, NULL, "/etc/ssl/certs") != 1)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}
	else
		BIO_printf(outbio, "SSL_CTX_load_verify_locations success\n");

	if ( SSL_CTX_set_default_verify_paths(ctx) != 1)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}
	else
		BIO_printf(outbio, "SSL_CTX_set_default_verify_paths success\n");

    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0 )
    {
       	ERR_print_errors_fp(stderr);
       	abort();
	}
    else
		BIO_printf(outbio, "SSL_CTX_use_certificate_file success\n");

	/* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0 )
    {
       	ERR_print_errors_fp(stderr);
       	abort();
    }
	else
		BIO_printf(outbio, "SSL_CTX_use_PrivateKey_file success\n");
    
	/* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
       	ERR_print_errors_fp(stderr);
       	abort();
    }
	else
	   	BIO_printf(outbio, "Private key matches the public certificate\n");

//	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
	ERR_print_errors_fp(stderr);
	SSL_CTX_set_verify_depth(ctx, 4);
	ERR_print_errors_fp(stderr);
}

// Print the public key from the certificate
void print_pubkey(BIO *outbio, EVP_PKEY *pkey)
{
	if (pkey)
	{
		switch (EVP_PKEY_id(pkey))
		{
			case EVP_PKEY_RSA:
				BIO_printf(outbio, "%d bit RSA Key\n", EVP_PKEY_bits(pkey));
				break;
			case EVP_PKEY_DSA:
				BIO_printf(outbio, "%d bit DSA Key\n", EVP_PKEY_bits(pkey));
				break;
			case EVP_PKEY_EC:
				BIO_printf(outbio, "%d bit EC Key\n", EVP_PKEY_bits(pkey));
				break;
			default:
				BIO_printf(outbio, "%d bit non-RSA/DSA/EC Key\n", EVP_PKEY_bits(pkey));
				break;
		}
	}

	if (!PEM_write_bio_PUBKEY(outbio, pkey))
		BIO_printf(outbio, "Error writing public key data in PEM format\n");
}
