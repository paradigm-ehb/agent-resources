#include "base/base.h"

/* openssl X509 algorithm layer*/
#include <openssl/x509.h>
/* basic input and output stream */
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

/* high level open ssl certificates*/
#include <openssl/evp.h>

local_internal EVP_PKEY *
generate_key(void)
{
  /* Key container */
  EVP_PKEY *pkey = NULL;
  /* key-generation context */
  EVP_PKEY_CTX *ctx = NULL;

  /* create the context for the key generation algorithm*/
  ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
  if (!ctx)
  {
    return NULL;
  }

  if (EVP_PKEY_keygen_init(ctx) <= 0)
  {
    return NULL;
  }
  /* configure the rsa key size*/
  if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, KiB(2)) <= 0)
  {
    return NULL;
  }

  if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
  {
    return NULL;
  }

  EVP_PKEY_CTX_free(ctx);
  return pkey;
}

local_internal X509 *
generate_cert(EVP_PKEY *pkey)
{
  X509 *x509 = X509_new();
  if (!x509)
  {
    return NULL;
  }

  /* version 3 certificate */
  X509_set_version(x509, 2);

  /* serial number */
  ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

  /* validity: now to now + 365 days */
  X509_gmtime_adj(X509_get_notBefore(x509), 0);
  X509_gmtime_adj(X509_get_notAfter(x509), 60L * 60 * 24 * 365);

  /* public key */
  X509_set_pubkey(x509, pkey);

  /* subject / issuer */
  X509_NAME *name = X509_get_subject_name(x509);

  /* TODO(nasr): find a way to do some proper signing of the certificates */

  X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC,
    (unsigned char *)"BE", -1, -1, 0);
  X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC,
    (unsigned char *)"paradigm-ehb", -1, -1, 0);
  X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
    (unsigned char *)"localhost", -1, -1, 0);

  /* self-signed */
  X509_set_issuer_name(x509, name);

  /* sign */
  if (!X509_sign(x509, pkey, EVP_sha256()))
  {
    return NULL;
  }

  return x509;
}

local_internal void
generate(void)
{
    EVP_PKEY *pkey = generate_key();
    X509 *cert = generate_cert(pkey);

    FILE *f;

    f = fopen("key.pem", "wb");
    PEM_write_PrivateKey(f, pkey, NULL, NULL, 0, NULL, NULL);
    fclose(f);

    f = fopen("cert.pem", "wb");
    PEM_write_X509(f, cert);
    fclose(f);

    X509_free(cert);
    EVP_PKEY_free(pkey);

}
