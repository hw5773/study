#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <openssl/rand.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "../../log/logger.h"

#define FAIL    -1

/******************************************************************************
 *  Function		 : OpenListener
 *
 *  Input Parameters: int port
 *
 *  Output Parameter: int sd
 *
 *  Description 	  : Creat server socket by getting a port number from input
 							 and return socket descriptor as output.
******************************************************************************/
int OpenListener(int port)
{   int sd;
	struct sockaddr_in6 addr;

	sd = socket(PF_INET6, SOCK_STREAM, 0);
	
	bzero(&addr, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);
	addr.sin6_addr = in6addr_any;
	if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
	{
		perror("can't bind port");
		abort();
	}
	if ( listen(sd, 10) != 0 )
	{
		perror("Can't configure listening port");
		abort();
	}
	return sd;
}

/******************************************************************************
 *  Function		 : InitServerCTX
 *
 *  Input Parameters: None
 *
 *  Output Parameter: SSL_CTX *ctx
 *
 *  Description 	  : Initialize SSL server and create context
******************************************************************************/

SSL_CTX* InitServerCTX(void)
{   
	SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
	SSL_load_error_strings();   /* load all error messages */
	method = TLSv1_2_server_method();  /* create new server-method instance */
	ctx = SSL_CTX_new(method);   /* create new context from method */
	if ( ctx == NULL )
	{
		printf("SSL_CTX init failed!");
		abort();
	}
	return ctx;
}

/******************************************************************************
 *  Function		 : LoadCertificates
 *
 *  Input Parameters: SSL_CTX* ctx, char* CaCertFile, char* CertFile, char* KeyFile
 *
 *  Output Parameter: None
 *
 *  Description 	  : Load CA certificate, Server certificate and Key file
******************************************************************************/

void LoadCertificates(SSL_CTX* ctx, char* CaCertFile, char* CertFile, char* KeyFile)
{
	/* Load certificates for verification purpose*/
	if (SSL_CTX_load_verify_locations(ctx, CaCertFile, KeyFile) != 1)
		printf("SSL_CTX_load_verify_locations failed!\n");

	/* Set default paths for certificate verifications */
	if (SSL_CTX_set_default_verify_paths(ctx) != 1)
		printf("SSL_CTX_set_default_verify_paths failed!\n");

	/* Set the local certificate from CertFile */
	if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
		printf("SSL_CTX_use_certificate_file failed!\n");
	
	/* Set the private key from KeyFile (may be the same as CertFile) */
	if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
		printf("SSL_CTX_use_PrivateKey_file failed!\n");

	/* Verify private key */
	if ( !SSL_CTX_check_private_key(ctx) )
		printf("Private key does not match the public certificate\n");

	/* Force the client-side have a certificate*/
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
	SSL_CTX_set_verify_depth(ctx, 4);
}

/******************************************************************************
 *  Function		 : ShowCerts
 *
 *  Input Parameters: SSL_CTX* ssl
 *
 *  Output Parameter: None
 *
 *  Description 	  : Get and print the client certificate
******************************************************************************/

void ShowCerts(SSL* ssl)
{   X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
	if ( cert != NULL )
	{
		printf("Peer certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	}
	else
		printf("No certificates.\n");
}


int main(int count, char *strings[])
{  

	printf("START: %lu \n", getMicroSecond());
	SSL_CTX *ctx;
	int server;
	char *portnum, *cert, *key, *cacert;

	if ( count != 6 )
	{
		printf("Usage: %s <portnum> <cert_file> <ca_cert_file> <key_file> <loggerfile>\n", strings[0]);
		exit(0);
	}
	SSL_library_init();

	portnum = strings[1];
	cert = strings[2];
	cacert = strings[3];
	key = strings[4];
	log_init(strings[5]);

	ctx = InitServerCTX();        /* initialize SSL */
	LoadCertificates(ctx, cacert, cert, key);
	server = OpenListener(atoi(portnum));    /* create server socket */

	struct sockaddr_in6 addr;
	char str[INET6_ADDRSTRLEN];
	socklen_t len = sizeof(addr);
	SSL *ssl;

	int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
	inet_ntop(AF_INET6, &(addr.sin6_addr), str, INET6_ADDRSTRLEN);
	printf("Connecting from: %s\nPort: %d\n",str, ntohs(addr.sin6_port));
	ssl = SSL_new(ctx);              /* get new SSL state with context */
	SSL_CTX_set_cipher_list(ctx, "AES256-SHA256");
	SSL_set_fd(ssl, client);      /* set connection socket to SSL state */
	SSL_set_options(ssl, SSL_OP_NO_TICKET);

	
	if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
		ERR_print_errors_fp(stderr);
	else
	{
		char buf[16*1000];
		unsigned char *msg = NULL;
		msg = malloc(16*1000+1);

		ShowCerts(ssl);

		//Get HTTP request
      printf("Client request:\n");
      SSL_read(ssl, buf, sizeof(buf));
		printf("%s\n", buf);
		SSL_read(ssl, buf, sizeof(buf));
      printf("%s\n", buf);
      SSL_read(ssl, buf, sizeof(buf));
      printf("%s\n", buf);

      //Send HTTP response
      strcpy((char*)msg, "HTTP/1.1 200 OK\n");
      SSL_write(ssl, msg, strlen((char*)msg));
      strcpy((char*)msg, "Content-Type: text/html\n\n");
      SSL_write(ssl, msg, strlen((char*)msg));

		int bytes, i = 0;
		int sendBytes;

		//Read 1000B from client
		bytes = 0;
		sendBytes = 0;
		printf("%d random bytes generated\n",RAND_bytes(msg, 1000));

		
		unsigned long first = getMicroSecond();

		SSL_read(ssl, buf, sizeof(buf));

		unsigned long second = getMicroSecond();
		
		SSL_write(ssl, msg, 1000);
		
		unsigned long third = getMicroSecond();

		printLog(COMMUNICATION_SMALL_BYTE_START_SERVER);
		while(bytes < 1000-1){
			bytes += SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
		}   
		printLog(COMMUNICATION_SMALL_BYTE_RECEIVE_SERVER);
		
		//Send 1000B to client
		sendBytes += SSL_write(ssl, msg, 1000);   /* encrypt & send message */
		   
		printLog(COMMUNICATION_SMALL_BYTE_FINISH_SERVER);

		printf("total: %d bytes received\n", bytes);
		printf("total: %d bytes sent\n", sendBytes);
		printf("1K byte %lu us\n", second-first);
		printf("1K byte %lu us\n", third-second);
		

		//Read 100*1000B from client
		bytes = 0;
		sendBytes = 0;
		printf("%d random bytes generated\n",RAND_bytes(msg, 16*1000));

		first = getMicroSecond();
		SSL_read(ssl, buf, sizeof(buf));
		second = getMicroSecond();
		SSL_write(ssl, msg, 16*1000);
		third = getMicroSecond();

		printLog(COMMUNICATION_LARGE_BYTE_START_SERVER);
		while(bytes < 100*1000-1){
			bytes += SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
		}   

		printLog(COMMUNICATION_LARGE_BYTE_RECEIVE_SERVER);
		//Send 100*1000B to client
		for(i=0;i<100/16;i++){
			sendBytes += SSL_write(ssl, msg, 16*1000);   /* encrypt & send message */
		}   
		sendBytes += SSL_write(ssl, msg, (100%16)*1000);   /* encrypt & send message */	
		printLog(COMMUNICATION_LARGE_BYTE_FINISH_SERVER);

		printf("total: %d bytes received\n", bytes);
		printf("total: %d bytes sent\n", sendBytes);

		free(msg);
	}
		
	printf("END: %lu \n", getMicroSecond());
	SSL_free(ssl);
	SSL_CTX_free(ctx);         /* release context */
	close(client);
	close(server);          /* close server socket */
	log_close();

	return 0;
}
