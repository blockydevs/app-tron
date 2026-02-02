#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct { uint32_t dummy; } cx_sha256_t;
typedef struct { uint32_t dummy; } cx_sha3_t;
typedef struct { uint32_t dummy; } cx_hash_t;
typedef struct { uint8_t W[65]; } cx_ecfp_public_key_t;

typedef int cx_err_t;

#define CX_OK 0
#define CX_LAST 0x01
#define CX_CURVE_256K1 0x01
#define CX_ECCINFO_PARITY_ODD 0x01
#define CX_RND_RFC6979 0x02
#define CX_SHA256 0x12
#define CX_SHA512 0x13

static inline void cx_sha256_init(cx_sha256_t *ctx) { (void)ctx; }
static inline int cx_keccak_init_no_throw(cx_sha3_t *ctx, int bits) { (void)ctx; (void)bits; return 1; }
static inline int cx_hash_no_throw(cx_hash_t *hash, int mode, const uint8_t *in, size_t in_len, uint8_t *out, size_t out_len) {
    (void)hash; (void)mode; (void)in; (void)in_len; (void)out; (void)out_len; return 1;
}
static inline void cx_hash_sha256(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_len) {
    (void)in; (void)in_len; (void)out; (void)out_len;
}
static inline int cx_ecfp_init_public_key_no_throw(int curve, const uint8_t *raw, size_t raw_len, cx_ecfp_public_key_t *pk) {
    (void)curve; (void)raw; (void)raw_len; (void)pk; return 1;
}
static inline int cx_ecdsa_verify_no_throw(cx_ecfp_public_key_t *pk, const uint8_t *hash, size_t hash_len, const uint8_t *sig, size_t sig_len) {
    (void)pk; (void)hash; (void)hash_len; (void)sig; (void)sig_len; return 1;
}

#define CX_ASSERT(x) do { if (!(x)) { } } while (0)