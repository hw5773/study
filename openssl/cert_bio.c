#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>
#include <openssl/err.h>

int main(int argc, char *argv[])
{
	const char *cert_filename	=	"ecc_server.crt";
	BIO *cert_bio 				= 	NULL;
	BIO *out_bio 				= 	NULL;
	X509 *cert					=	NULL;
	EVP_PKEY *pkey				=	NULL;
	int ret;

	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();

	cert_bio = BIO_new(BIO_s_file());
	out_bio = BIO_new_fp(stdout, BIO_NOCLOSE);

	ret = BIO_read_filename(cert_bio, cert_filename);
	if (!(cert = PEM_read_bio_X509(cert_bio, NULL, 0, NULL)))
	{
		BIO_printf(out_bio, "Error loading cert into memory\n");
		exit(-1);
	}

	if ((pkey = X509_get_pubkey(cert)) == NULL)
		BIO_printf(out_bio, "Error getting public key from certificate\n");
	
	if (pkey)
	{
		switch (EVP_PKEY_id(pkey))
		{
			case EVP_PKEY_RSA:
				BIO_printf(out_bio, "%d bit RSA Key\n\n", EVP_PKEY_bits(pkey));
				break;
			case EVP_PKEY_DSA:
				BIO_printf(out_bio, "%d bit DSA Key\n\n", EVP_PKEY_bits(pkey));
				break;
			default:
				BIO_printf(out_bio, "%d bit non-RSA/DSA\n\n", EVP_PKEY_bits(pkey));
				break;
		}
	}

	if (!PEM_write_bio_PUBKEY(out_bio, pkey))
		BIO_printf(out_bio, "Error writing public key data in PEM format\n");

	EVP_PKEY_free(pkey);
	X509_free(cert);
	BIO_free_all(cert_bio);
	BIO_free_all(out_bio);
	
	return 0;
}
