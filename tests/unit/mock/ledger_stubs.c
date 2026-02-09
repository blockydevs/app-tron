#include <string.h>
#include <stdint.h>

#include "os.h"
#include "cx.h"
#include "format.h"
#include "io.h"
#include "base58.h"
#include "crypto_helpers.h"
#include "parse.h"
#include "tokens.h"
#include "settings.h"

volatile unsigned int g_last_throw = 0;
unsigned char G_io_apdu_buffer[260];
publicKeyContext_t publicKeyContext;
const internal_storage_t N_storage_real = 0;

void THROW(unsigned int exception) {
    g_last_throw = exception;
}

int format_hex(const uint8_t *in, size_t in_len, char *out, size_t out_len) {
    static const char hex[] = "0123456789abcdef";
    if (out_len < (in_len * 2 + 1)) {
        return -1;
    }
    for (size_t i = 0; i < in_len; i++) {
        out[i * 2] = hex[(in[i] >> 4) & 0xF];
        out[i * 2 + 1] = hex[in[i] & 0xF];
    }
    out[in_len * 2] = '\0';
    return (int) (in_len * 2);
}

void io_seproxyhal_io_heartbeat(void) {}

int io_send_response_pointer(const uint8_t *buffer, size_t len, uint16_t sw) {
    (void) buffer;
    (void) len;
    (void) sw;
    return 0;
}

void base58_encode(const uint8_t *in, size_t inlen, char *out, size_t outlen) {
    (void) in;
    (void) inlen;
    if (outlen > 0) {
        out[0] = '\0';
    }
}

cx_err_t bip32_derive_ecdsa_sign_rs_hash_256(int curve, const uint32_t *path, size_t path_len,
                                             unsigned int mode, int hashID,
                                             const uint8_t *hash, size_t hash_len,
                                             uint8_t *sig_r, uint8_t *sig_s, unsigned int *info) {
    (void) curve;
    (void) path;
    (void) path_len;
    (void) mode;
    (void) hashID;
    (void) hash;
    (void) hash_len;
    (void) sig_r;
    (void) sig_s;
    if (info) {
        *info = 0;
    }
    return CX_OK;
}

cx_err_t bip32_derive_get_pubkey_256(int curve, const uint32_t *path, size_t path_len,
                                     uint8_t *public_key, uint8_t *chain_code, int hashID) {
    (void) curve;
    (void) path;
    (void) path_len;
    (void) hashID;
    if (public_key) {
        memset(public_key, 0, PUBLIC_KEY_SIZE);
    }
    if (chain_code) {
        memset(chain_code, 0, CHAIN_CODE_SIZE);
    }
    return CX_OK;
}

int verifyTokenNameID(const char *tokenId,
                      const char *tokenName,
                      uint8_t decimals,
                      uint8_t *signature,
                      uint8_t signatureLength) {
    (void) tokenId;
    (void) tokenName;
    (void) decimals;
    (void) signature;
    (void) signatureLength;
    return 1;
}

int verifyExchangeID(const unsigned char *exchangeValidation,
                     uint8_t datLength,
                     uint8_t *signature,
                     uint8_t signatureLength) {
    (void) exchangeValidation;
    (void) datLength;
    (void) signature;
    (void) signatureLength;
    return 1;
}

const uint8_t SELECTOR[][4] = {
    {0xA9, 0x05, 0x9C, 0xBB},
    {0x09, 0x5E, 0xA7, 0xB3}
};

const tokenDefinition_t TOKENS_TRC20[NUM_TOKENS_TRC20] = {0};

