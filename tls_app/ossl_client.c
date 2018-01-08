#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/ocsp.h>
#include <openssl/rand.h>
#include <openssl/engine.h>

#include <time.h>
#include <sys/time.h>
#include "../../log/logger.h"

#define FAIL    -1

/******************************************************************************
 *  Function       : OpenListener
 *
 *  Input Parameters: int port
 *
 *  Output Parameter: int sd
 *
 *  Description     : Creat socket by getting a port number from input
			             and return socket descriptor as output.
******************************************************************************/
int OpenConnection(const char *hostname, char *port)
{  
	int sd;
	struct addrinfo hints, *res;

	memset (&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(hostname, port, &hints, &res);

	sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if ( connect(sd, res->ai_addr, res->ai_addrlen) != 0 )
	{
		close(sd);
		perror(hostname);
		abort();
	} 
	return sd;
}

/******************************************************************************
 *  Function       : InitCTX
 *
 *  Input Parameters: None
 *
 *  Output Parameter: SSL_CTX *ctx
 *
 *  Description     : Initialize SSL server and create context
******************************************************************************/
SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */
	method = TLSv1_2_client_method();  /* Create new client-method instance */
	ctx = SSL_CTX_new(method);   /* Create new context */
	if ( ctx == NULL )
	{
		printf("SSL_CTX init failed!");
		abort();
	}
	return ctx;
}

/******************************************************************************
 *  Function       : ShowCerts
 *
 *  Input Parameters: SSL_CTX* ssl
 *
 *  Output Parameter: None
 *
 *  Description     : Print the server certificate
******************************************************************************/

void ShowCerts(SSL* ssl)
{   X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
	if ( cert != NULL )
	{
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);       /* free the malloc'ed string */
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);       /* free the malloc'ed string */

		STACK_OF(X509_EXTENSION) *exts = cert->cert_info->extensions;		
		int num_of_exts;
		if(exts){
			num_of_exts = sk_X509_EXTENSION_num(exts);
			printf("num of exts: %d\n", num_of_exts);
		}

		X509_free(cert);     /* free the malloc'ed certificate copy */
	}
	else
		printf("No certificates.\n");
}

/******************************************************************************
 *  Function       : LoadCertificates
 *
 *  Input Parameters: SSL_CTX* ctx, char* CaCert, char* CertFile, char* KeyFile
 *
 *  Output Parameter: None
 *
 *  Description     : Load CA certificate, client certificate and Key file
******************************************************************************/
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* CaCert, char* KeyFile)
{
	/* Load certificates for verification purpose*/
	if ( SSL_CTX_load_verify_locations(ctx, CaCert, CaCert) <= 0)
		printf("SSL_CTX_load_verify_locations failed!\n");

	/* set the local certificate from CertFile */
	if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
		printf("SSL_CTX_use_certificate_file failed!\n");

	/* set the private key from KeyFile (may be the same as CertFile) */
	if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
		printf("SSL_CTX_use_PrivateKey_file failed!\n");

	/* Verify peer certificate */
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

	/* Verify private key */
	if ( !SSL_CTX_check_private_key(ctx) )
		printf("Private key does not match the public certificate\n");
}

int main(int count, char *strings[])
{   
	printf("START: %lu \n", getMicroSecond());
	SSL_CTX *ctx;
	int server;
	SSL *ssl;
	char buf[1000*1000];
	int bytes;
	char *hostname, *portnum, *cert, *cacert, *key;

	if ( count != 7 )
	{
		printf("usage: %s <host_name> <port_num> <cert_file> <ca_certfile> <key_file> <log_file>\n", strings[0]);
		exit(0);
	}
	SSL_library_init();
	hostname=strings[1];
	portnum=strings[2];
	cert=strings[3];
	cacert = strings[4];
	key=strings[5];
	log_init(strings[6]);

	ctx = InitCTX();
	LoadCertificates(ctx, cert, cacert, key);
	server = OpenConnection(hostname, portnum);
	ssl = SSL_new(ctx);      /* create new SSL connection state */
	SSL_CTX_set_cipher_list(ctx, "AES256-SHA256");
	SSL_set_options(ssl, SSL_OP_NO_TICKET);
	SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_OFF);

	SSL_set_fd(ssl, server);    /* attach the socket descriptor */
	

	if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
		printf("SSL_connect failed!\n");
	else
	{  
			printf("same!\n");

			unsigned char *msg = NULL;
			msg = malloc(16*1000+1);
			int i=0;

			//Client send HTTP request
      	strcpy((char*)msg, "POST / HTTP/1.1");
      	SSL_write(ssl, msg, strlen((char*)msg));
      	strcpy((char*)msg, "Connection: Keep-Alive");
      	SSL_write(ssl, msg, strlen((char*)msg));
      	strcpy((char*)msg, "Content-Type: text/html");
      	SSL_write(ssl, msg, strlen((char*)msg));

      	//Get response from server
      	printf("Server Response:\n");
      	SSL_read(ssl, buf, sizeof(buf));
      	printf("%s", buf);
      	SSL_read(ssl, buf, sizeof(buf));
      	printf("%s", buf);

			//Send 1000B to server
			bytes = 0;
			printf("%d random bytes generated\n",RAND_bytes(msg, 1000));


			unsigned long first = getMicroSecond();

			SSL_write(ssl, msg, 1000);

			unsigned long second = getMicroSecond();
			SSL_read(ssl, msg, sizeof(buf));
			
			unsigned long third = getMicroSecond();


			printLog(COMMUNICATION_SMALL_BYTE_START_CLIENT);
			SSL_write(ssl, msg, 1000);   /* encrypt & send message */
			printLog(COMMUNICATION_SMALL_BYTE_FINISH_CLIENT);

			//Read 1000B from server
			while(bytes < 1000-1){
				bytes += SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
			}
			printLog(COMMUNICATION_SMALL_BYTE_RECEIVE_CLIENT);

			printf("total: %d bytes received\n", bytes);
		
			printf("1KB %lu us\n", second-first);
			printf("1KB %lu us\n", third-second);


			//Send 100*1000B to server
			printf("%d random bytes generated\n",RAND_bytes(msg, 16*1000));
			bytes = 0;

			first = getMicroSecond();

			SSL_write(ssl, msg, 16*1000);

			second = getMicroSecond();
			SSL_read(ssl, msg, sizeof(buf));
			
			third = getMicroSecond();
	


			printLog(COMMUNICATION_LARGE_BYTE_START_CLIENT);
			for(i=0;i<100/16;i++){
				SSL_write(ssl, msg, 16*1000);   /* encrypt & send message */
			}
			SSL_write(ssl, msg, (100%16)*1000);   /* encrypt & send message */
			printLog(COMMUNICATION_LARGE_BYTE_FINISH_CLIENT);

			//Read 100*1000B from server
			while(bytes < 100*1000-1){
				bytes += SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
			}
			printLog(COMMUNICATION_LARGE_BYTE_RECEIVE_CLIENT);
			printf("total: %d bytes received\n", bytes);
			
			free(msg);
	
	printf("16KB %lu us\n", second-first);
	printf("16KB %lu us\n", third-second);




	}

	printf("END: %lu \n", getMicroSecond());
	close(server);         /* close socket */
	SSL_CTX_free(ctx);        /* release context */
	log_close();

	return 0;
}
