#include "warrant.h"

// Main Function
int main(int argc, char *argv[])
{
	// Declare the variables needed
	BIO *alice = NULL;
	BIO *carol = NULL;
	BIO *alice_key = NULL;
	BIO *carol_key = NULL;
	BIO *out_bio = NULL;
	X509 *alice_cert = NULL;
	X509 *carol_cert = NULL;
	EVP_PKEY *alice_pub = NULL;
	EVP_PKEY *carol_pub = NULL;
	EVP_PKEY *alice_priv = NULL;
	EVP_PKEY *carol_priv = NULL;
	int i;

	// Declare the variables related to the message
	unsigned char *warrant_msg;
	unsigned char *warrant_content_body1;
	unsigned char *warrant_content_body;
	unsigned char *warrant_response;
	unsigned int len, len1, len2;

	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();

	// Initialize the BIOs for certs and keys
	alice = BIO_new(BIO_s_file());
	carol = BIO_new(BIO_s_file());
	alice_key = BIO_new(BIO_s_file());
	carol_key = BIO_new(BIO_s_file());
	out_bio = BIO_new_fp(stdout, BIO_NOCLOSE);

	BIO_read_filename(alice, "alice_cert.pem");
	BIO_read_filename(carol, "carol_cert.pem");
	BIO_read_filename(alice_key, "alice_priv.pem");
	BIO_read_filename(carol_key, "carol_priv.pem");

	alice_cert = PEM_read_bio_X509(alice, NULL, 0, NULL);
	carol_cert = PEM_read_bio_X509(carol, NULL, 0, NULL);

	alice_pub = X509_get_pubkey(alice_cert);
	carol_pub = X509_get_pubkey(carol_cert);
	alice_priv = PEM_read_bio_PrivateKey(alice_key, NULL, 0, NULL);
	carol_priv = PEM_read_bio_PrivateKey(carol_key, NULL, 0, NULL);

	BIO_printf(out_bio, "Read Alice's Public Key >>>\n");
	PEM_write_bio_PUBKEY(out_bio, alice_pub);
	BIO_printf(out_bio, "Read Carol's Public Key >>>\n");
	PEM_write_bio_PUBKEY(out_bio, carol_pub);

	BIO_printf(out_bio, "PROGRESS: Invoke make_warrant_content_body()\n");
	make_warrant_content_body(out_bio, &warrant_content_body, alice_pub, carol_pub, NID_sha256, &len2);
	BIO_printf(out_bio, "PROGRESS: Invoke verify_warrant_content_body()\n");
	verify_warrant_content_body(out_bio, warrant_content_body, alice_pub, carol_pub);
	BIO_printf(out_bio, "PROGRESS: Invoke make_warrant_request()\n");
	make_warrant_request(out_bio, &warrant_content_body1, warrant_content_body, len2, carol_priv, NID_sha256, &len1);
	verify_warrant_request(out_bio, warrant_content_body1, alice_pub, carol_pub);
	BIO_printf(out_bio, "PROGRESS: Invoke make_warrant_response()\n");
	make_warrant_response(out_bio, &warrant_response, warrant_content_body1, len1, alice_priv, NID_sha256, &len);
	BIO_printf(out_bio, "PROGRESS: Invoke verify_warrant_response()\n");
	verify_warrant_response(out_bio, warrant_response, alice_pub, carol_pub);

	OPENSSL_free(warrant_content_body);
	return 0;
}

// Make warrant content body
// Input
//   out: BIO for the standard output
//   content: warrant content
//   alice_pub: origin's public key
//   carol_pub: edge's public key
//   nid: signature type
//   len: length of the warrant content
// Output Success: 1, Failure: 0
int make_warrant_content_body(BIO *out, unsigned char **content, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub, int nid, int *len)
{
	// Declare the variables related to the relation
	BIO *pub_relation = NULL;
	unsigned char *pub_relation_sha;
	int shalen, i;

	// Declare the variables related to the timestamp
	struct timeval curr;
	gettimeofday(&curr, NULL);
	unsigned long ts = curr.tv_sec;
	unsigned int duration = 31536000;

	BIO_printf(out, "PROGRESS: Make H(orig||edge)\n");
	pub_relation = BIO_new(BIO_f_md());

	// Set the message digests according to the nid
	switch (nid)
	{
		case NID_sha1:
			BIO_set_md(pub_relation, EVP_sha1());
			shalen = SHA_DIGEST_LENGTH;
			BIO_printf(out, "PROGRESS: Hash algorithm is set to SHA1\n");
			break;
		case NID_sha224:
			BIO_set_md(pub_relation, EVP_sha224());
			shalen = SHA224_DIGEST_LENGTH;
			BIO_printf(out, "PROGRESS: Hash algorithm is set to SHA224\n");
			break;
		case NID_sha256:
			BIO_set_md(pub_relation, EVP_sha256());
			shalen = SHA256_DIGEST_LENGTH;
			BIO_printf(out, "PROGRESS: Hash algorithm is set to SHA256\n");
			break;
		default:
			BIO_printf(out, "PROGRESS: Unknown Hash algorithm\n");
			return 0;
	}

	pub_relation_sha = (unsigned char *)OPENSSL_zalloc(shalen);

	// Make the hash, H(alice||carol)
	PEM_write_bio_PUBKEY(pub_relation, alice_pub);
	PEM_write_bio_PUBKEY(pub_relation, carol_pub);
	BIO_gets(pub_relation, pub_relation_sha, shalen);

	// Print the info
	BIO_printf(out, "PROGRESS: Print Relation\n");

	for (i=0; i<shalen; i++)
	{
		if (i % 10 == 0)
			BIO_printf(out, "\n");
		BIO_printf(out, "%02x\t", pub_relation_sha[i]);
	}
	BIO_printf(out, "\n");

	BIO_printf(out, "PROGRESS: sizeof(unsigned long):\t %lu\n", sizeof(unsigned long));
	BIO_printf(out, "PROGRESS: sizeof(unsigned int):\t %lu\n", sizeof(unsigned int));
	BIO_printf(out, "PROGRESS: current time:\t %lu\n", ts);
	BIO_printf(out, "PROGRESS: duration:\t %u\n", duration);
	BIO_printf(out, "PROGRESS: hash algorithm type:\t %u\n", nid);

	// Make the final message
	uint16_t ht = (uint16_t) nid;
	*len = sizeof(ts) + sizeof(duration) + sizeof(ht) + shalen;
	BIO_printf(out, "PROGRESS: length of warrant_content_body: %d\n", *len);
	*content = (unsigned char *)OPENSSL_zalloc(*len);
	unsigned char *p;
	p = *content;
	t2n8(ts, p);
	d2n4(duration, p);
	s2n(ht, p);
	memcpy(p, pub_relation_sha, shalen);

	BIO_printf(out, "PROGRESS: print warrant_content_body\n");
	for (i=0; i<*len; i++)
	{
		if (i % 10 == 0)
			BIO_printf(out, "\n");
		BIO_printf(out, "%02x\t", (*content)[i]);
	}
	BIO_printf(out, "\n");

	return 1;
}

// Make the warrant request message
// Input
//   out: BIO for the standard output
//   request: the final message
//   msg: warrant content
//   msg_len: length of warrant content
//   carol_priv: edge's private key
//   nid: signature algorithm
//   len: the length of the final message
// Output
//   Success: 1, Failure: 0
int make_warrant_request(BIO *out, unsigned char **request, unsigned char *msg, int msg_len, EVP_PKEY *carol_priv, int nid, int *len)
{
	unsigned char *sigblk, *p;
	size_t sigblk_len;

	if (!make_signature_block(out, &sigblk, msg, msg_len, carol_priv, nid, &sigblk_len))
	{
		BIO_printf(out, "ERROR: make the signature block failed\n");
		goto err;
	}
	BIO_printf(out, "PROGRESS: make the signature block for the warrant content success\n");

	*len = sizeof(uint16_t) + msg_len + sigblk_len;

	BIO_printf(out, "Length of warrant request: %d\n", *len);

	// Make the final message - warrant request
	*request = (unsigned char *)OPENSSL_zalloc(*len);
	p = *request;
	s2n(msg_len, p);
	memcpy(p, msg, msg_len);
	p += msg_len;
	memcpy(p, sigblk, sigblk_len);

	OPENSSL_free(sigblk);

	return 1;

err:
	return 0;
}

int make_warrant_request_with_verify_warrant(BIO *out, unsigned char **request, unsigned char *msg, int msg_len, EVP_PKEY *carol_priv, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub, int nid, int *len)
{
	if (!verify_warrant_content_body(out, msg, alice_pub, carol_pub))
	{
		BIO_printf(out, "ERROR: Verify warrant content body failure in make_warrant_request_with_verify_warrant\n");
		return 0;
	}
	return make_warrant_request(out, request, msg, msg_len, carol_priv, nid, len);
}

// Make the warrant_response message (len || warrant_request || Signature type || Signature length || Signature)
// Input
//   out: BIO for the standard output
//   response: the warrant_response message
//   request: the warrant_request message
//   req_len: the length of the request
//   alice_priv: the alice's private key
//   alice_pub: the alice's public key
//   carol_pub: the carol's public key
//   nid: the signature algorithm
//   len: the length of the warrant_response
// Output
//   Success 1
//   Failure 0
int make_warrant_response(BIO *out, unsigned char **response, unsigned char *request, int req_len, EVP_PKEY *alice_priv, int nid, int *len)
{
	unsigned char *sigblk, *p;
	size_t sigblk_len;

	BIO_printf(out, "PROGRESS: Make the warrant response\n");

	if (!make_signature_block(out, &sigblk, request, req_len, alice_priv, nid, &sigblk_len))
	{
		BIO_printf(out, "ERROR: make the signature block failed\n");
		goto err;
	}
	BIO_printf(out, "PROGRESS: make the signature block for the warrant content success\n");

	*len = sizeof(uint16_t) + req_len + sigblk_len;

	BIO_printf(out, "Length of warrant response: %d\n", *len);

	// Make the final message - warrant response
	*response = (unsigned char *)OPENSSL_zalloc(*len);
	p = *response;
	s2n(req_len, p);
	memcpy(p, request, req_len);
	p += req_len;
	memcpy(p, sigblk, sigblk_len);

	OPENSSL_free(sigblk);

	return 1;

err:
	return 0;
}

int make_warrant_response_with_verify_request(BIO *out, unsigned char **response, unsigned char *request, int req_len, EVP_PKEY *alice_priv, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub, int nid, int *len)
{
	if (!verify_warrant_request(out, request, alice_pub, carol_pub))
	{
		BIO_printf(out, "ERROR: Verify the warrant request failed in make_warrant_response_with_verify_request\n");
		return 0;
	}
	BIO_printf(out, "PROGRESS: Verify the warrant request success in make_warrant_response_with_verify_request\n");

	return make_warrant_response(out, response, request, req_len, alice_priv, nid, len);
}

// Make the signature block composed of (Signature Type || Signature Length || Signature)

int make_signature_block(BIO *out, unsigned char **sigblk, unsigned char *msg, int msg_len, EVP_PKEY *priv, int nid, size_t *sigblk_len)
{
	int i, rc, rc1, rc2;
	EVP_MD_CTX *ctx;
	unsigned char *sig, *p;
	size_t sig_len;
	uint16_t sig_type;

	ctx = EVP_MD_CTX_create();
	if (ctx == NULL)
	{
		BIO_printf(out, "EVP_MD_CTX_create failed\n");
		goto err;
	}

	// Initialize the md according to nid
	switch (nid)
	{
		case NID_sha1:
			rc1 = EVP_DigestInit_ex(ctx, EVP_sha1(), NULL);
			rc2 = EVP_DigestSignInit(ctx, NULL, EVP_sha1(), NULL, priv);
			sig_type = NID_sha1;
			break;
		case NID_sha224:
			rc1 = EVP_DigestInit_ex(ctx, EVP_sha224(), NULL);
			rc2 = EVP_DigestSignInit(ctx, NULL, EVP_sha224(), NULL, priv);
			sig_type = NID_sha224;
			break;
		case NID_sha256:
			rc1 = EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
			rc2 = EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, priv);
			sig_type = NID_sha256;
			break;
		default:
			BIO_printf(out, "Unknown Hash algorithm\n");
			goto err;
	}

	// Make the signature
	if (rc1 != 1)
	{
		BIO_printf(out, "PROGRESS: Digest Init Failed\n");
		goto err;
	}

	if (rc2 != 1)
	{
		BIO_printf(out, "PROGRESS: DigestSign Init Failed\n");
		goto err;
	}

	rc = EVP_DigestSignUpdate(ctx, msg, msg_len);
	if (rc != 1)
	{
		BIO_printf(out, "PROGRESS: DigestSign Update Failed\n");
		goto err;
	}

	rc = EVP_DigestSignFinal(ctx, NULL, &sig_len);
	if (rc != 1)
	{
		BIO_printf(out, "PROGRESS: DigestSign Final Failed\n");
		goto err;
	}

	if (sig_len <= 0)
	{
		BIO_printf(out, "PROGRESS: DigestSign Final Failed\n");
		goto err;
	}

	BIO_printf(out, "PROGRESS: Signature length: %d\n", (int)sig_len);
	sig = OPENSSL_zalloc(sig_len);

	if (sig == NULL)
	{
		BIO_printf(out, "PROGRESS: OPENSSL_malloc error\n");
		goto err;
	}

	rc = EVP_DigestSignFinal(ctx, sig, &sig_len);
	if (rc != 1)
	{
		BIO_printf(out, "PROGRESS: DigestSign Final Failed\n");
		goto err;
	}

	*sigblk_len = 2 * sizeof(uint16_t) + sig_len;
	*sigblk = (unsigned char *)OPENSSL_zalloc(*sigblk_len);
	p = *sigblk;
	s2n(sig_type, p);
	s2n(sig_len, p);
	memcpy(p, sig, sig_len);

	BIO_printf(out, "PROGRESS: Sig in make warrant >>>\n");
	for (i=0; i<sig_len; i++)
	{
		if (i % 10 == 0)
			BIO_printf(out, "\n");
		BIO_printf(out, "%02X ", sig[i]);
	}
	BIO_printf(out, "\n");

	BIO_printf(out, "PROGRESS: Length of message: %d\n", msg_len);
	BIO_printf(out, "PROGRESS: Signature type: %d\n", (int)sig_type);
	BIO_printf(out, "PROGRESS: Length of signature: %d\n", (int)sig_len);

	OPENSSL_free(sig);
	EVP_MD_CTX_free(ctx);

	return 1;

sig_err:
	OPENSSL_free(sig);
err:
	EVP_MD_CTX_free(ctx);

	return 0;
}

// Verify the warrant content 
// Input
//   out: BIO for the standard output
//   content: warrant content
//   alice_pub: origin's public key
//   carol_pub: edge's public key
// Output
//   Success: 1, Failure: 0
int verify_warrant_content_body(BIO *out, unsigned char *content, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub)
{
	unsigned char *p = content;
	unsigned char *hash;
	unsigned char *pub_relation_sha;
	uint64_t ts;
	uint32_t duration;
	uint16_t ht;
	int len, cmp;
	struct timeval tv;
	unsigned long curr;

	BIO *pub_relation = NULL;
	
	n2t8(p, ts);
	n2d4(p, duration);

	BIO_printf(out, "PROGRESS: verify timestamp: %lu\n", ts);
	BIO_printf(out, "PROGRESS: verify duration: %u\n", duration);

	// Get the current time
	gettimeofday(&tv, NULL);
	curr = tv.tv_sec;

	// Verify whether in the valid time
	if ((curr >= ts) && (curr < ts + duration))
	{
		BIO_printf(out, "PROGRESS: current time is in the valid duration: %lu\n", curr);
	}
	else
	{
		BIO_printf(out, "PROGRESS: verify error. current time is not in the valid duration\n");
		goto err;
	}

	n2s(p, ht);
	pub_relation = BIO_new(BIO_f_md());

	// Set the hash algorithm according to nid
	switch (ht)
	{
		case NID_sha1:
			BIO_set_md(pub_relation, EVP_sha1());
			len = SHA_DIGEST_LENGTH;
			break;
		case NID_sha224:
			BIO_set_md(pub_relation, EVP_sha224());
			len = SHA224_DIGEST_LENGTH;
			break;
		case NID_sha256:
			BIO_set_md(pub_relation, EVP_sha256());
			len = SHA256_DIGEST_LENGTH;
			break;
		default:
			BIO_printf(out, "Unknown Hash Algorithm Type");
			goto err;
	}

	// Make the hash H(alice||carol)
	hash = (unsigned char *)OPENSSL_zalloc(len);
	memcpy(hash, p, len);

	PEM_write_bio_PUBKEY(pub_relation, alice_pub);
	PEM_write_bio_PUBKEY(pub_relation, carol_pub);
	pub_relation_sha = (unsigned char *)OPENSSL_zalloc(len);
	BIO_gets(pub_relation, pub_relation_sha, len);

	// Compare whether they are same
	cmp = CRYPTO_memcmp(pub_relation_sha, hash, len);

	BIO_printf(out, "PROGRESS: CMP Result: %d\n", cmp);

	if (cmp != 0)
	{
		BIO_printf(out, "PROGRESS: Verify Error. Hash is not matched\n");
		goto verify_err;
	}
	else
	{
		BIO_printf(out, "PROGRESS: Verify Success\n");
	}

	OPENSSL_free(hash);

	return 1;

verify_err:
	OPENSSL_free(hash);
err:
	return 0;
}

// Verify the warrant request
// Input
//   out: BIO for the standard output
//   request: warrant request
//   alice_pub: origin's public key
//   carol_pub: edge's public keky
// Output
//   Success 1, Failure 0
int verify_warrant_request(BIO *out, unsigned char *request, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub)
{
	int i;
	size_t len = 0, md_len = 0;
	uint16_t sig_type, sig_len;
	unsigned char *p, *warrant, *sig;

	BIO_printf(out, "PROGRESS: Invoke verify_warrant_request()\n");

	p = request;
	n2s(p, len);
	BIO_printf(out, "PROGRESS: Length of message: %d\n", (int)len);

	warrant = (unsigned char *)OPENSSL_zalloc(len);
	memcpy(warrant, p, len);
	p += len;
	n2s(p, sig_type);
	BIO_printf(out, "PROGRESS: Type of signature: %d\n", sig_type);

	n2s(p, sig_len);
	BIO_printf(out, "PROGRESS: Length of signature: %d\n", sig_len);

	sig = (unsigned char *)OPENSSL_zalloc(sig_len);
	memcpy(sig, p, sig_len);

	BIO_printf(out, "PROGRESS: Signature in verify >>>\n");
	for (i=0; i<sig_len; i++)
	{
		if (i % 10 == 0)
			BIO_printf(out, "\n");
		BIO_printf(out, "%02X ", sig[i]);
	}
	BIO_printf(out, "\n");

	if (!verify_signature(out, warrant, len, sig_type, sig_len, sig, carol_pub))
	{
		BIO_printf(out, "ERROR: Verify the signature error\n");
		return 0;
	}
	BIO_printf(out, "PROGRESS: Verify the signature success\n");

	// Verify the warrant content body
	if (!verify_warrant_content_body(out, warrant, alice_pub, carol_pub))
	{
		BIO_printf(out, "ERROR: Verify warrant content body error\n");
		goto err;
	}

	BIO_printf(out, "PROGRESS: Verify warrant content body success\n");

	OPENSSL_free(warrant);
	OPENSSL_free(sig);

	return 1;

err:
	OPENSSL_free(warrant);
	OPENSSL_free(sig);
	return 0;
}

// Verify the warrant response
int verify_warrant_response(BIO *out, unsigned char *response, EVP_PKEY *alice_pub, EVP_PKEY *carol_pub)
{
	int i;
	size_t len = 0, md_len = 0;
	uint16_t sig_type, sig_len;
	unsigned char *p, *request, *sig;

	BIO_printf(out, "PROGRESS: Invoke verify_warrant_response()\n");

	p = response;
	n2s(p, len);
	BIO_printf(out, "PROGRESS: Length of warrant_response: %d\n", (int)len);

	request = (unsigned char *)OPENSSL_zalloc(len);
	memcpy(request, p, len);
	p += len;
	n2s(p, sig_type);
	BIO_printf(out, "PROGRESS: Type of signature: %d\n", sig_type);

	n2s(p, sig_len);
	BIO_printf(out, "PROGRESS: Length of signature: %d\n", sig_len);

	sig = (unsigned char *)OPENSSL_zalloc(sig_len);
	memcpy(sig, p, sig_len);

	BIO_printf(out, "PROGRESS: Signature in verify >>>\n");
	for (i=0; i<sig_len; i++)
	{
		if (i % 10 == 0)
			BIO_printf(out, "\n");
		BIO_printf(out, "%02X ", sig[i]);
	}
	BIO_printf(out, "\n");

	if (!verify_signature(out, request, len, sig_type, sig_len, sig, alice_pub))
	{
		BIO_printf(out, "ERROR: Verify the warrant request signature error\n");
		return 0;
	}
	BIO_printf(out, "PROGRESS: Verify the warrant request signature success\n");

	// Verify the warrant content body
	if (!verify_warrant_request(out, request, alice_pub, carol_pub))
	{
		BIO_printf(out, "ERROR: Verify warrant content body error\n");
		goto err;
	}

	BIO_printf(out, "PROGRESS: Verify warrant content body success\n");

	OPENSSL_free(request);
	OPENSSL_free(sig);

	return 1;

err:
	OPENSSL_free(request);
	OPENSSL_free(sig);
	return 0;
}

// Verify the signature
// Input
//    out: BIO related to the standard output
//    sig_type: signature algorithm
//    sig_len: the length of the signature
//    sig: signature to be verified
//    pub: public key to be used for the verification
// Output
//    Success 1, Failure 0
int verify_signature(BIO *out, unsigned char *msg, int msg_len, uint16_t sig_type, uint16_t sig_len, unsigned char *sig, EVP_PKEY *pub)
{
	int rc;
	EVP_MD_CTX *ctx;

	ctx = EVP_MD_CTX_create();
	if (ctx == NULL)
	{
		BIO_printf(out, "ERROR: EVP_MD_CTX_create error\n");
		return 0;
	}

	// Verify the signature
	switch (sig_type)
	{
		case NID_sha1:
			rc = EVP_DigestVerifyInit(ctx, NULL, EVP_sha1(), NULL, pub);
			break;
		case NID_sha224:
			rc = EVP_DigestVerifyInit(ctx, NULL, EVP_sha224(), NULL, pub);
			break;
		case NID_sha256:
			rc = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pub);
			break;
		default:
			BIO_printf(out, "ERROR: Unknown Signature Type\n");
	}
	if (rc != 1)
	{
		BIO_printf(out, "ERROR: EVP_DigestVerifyInit error\n");
		goto err;
	}

	rc = EVP_DigestVerifyUpdate(ctx, msg, msg_len);
	if (rc != 1)
	{
		BIO_printf(out, "ERROR: EVP_DigestVerifyUpdate failed\n");
		goto err;
	}

	rc = EVP_DigestVerifyFinal(ctx, sig, sig_len);
	if (rc != 1)
	{
		BIO_printf(out, "ERROR: EVP_DigestVerifyFinal failed\n");
		goto err;
	}
	else
	{
		BIO_printf(out, "PROGRESS: Verify Success!\n");
	}

	EVP_MD_CTX_free(ctx);
	return 1;
err:
	EVP_MD_CTX_free(ctx);
	return 0;
}
