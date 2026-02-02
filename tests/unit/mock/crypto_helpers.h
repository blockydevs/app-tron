#pragma once

#include <stdint.h>
#include <stddef.h>
#include "cx.h"

cx_err_t bip32_derive_ecdsa_sign_rs_hash_256(int curve, const uint32_t *path, size_t path_len,
                                             unsigned int mode, int hashID,
                                             const uint8_t *hash, size_t hash_len,
                                             uint8_t *sig_r, uint8_t *sig_s, unsigned int *info);

cx_err_t bip32_derive_get_pubkey_256(int curve, const uint32_t *path, size_t path_len,
                                     uint8_t *public_key, uint8_t *chain_code, int hashID);

