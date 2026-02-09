#pragma once
#include <stdint.h>
#include <stddef.h>

void base58_encode(const uint8_t *in, size_t inlen, char *out, size_t outlen);