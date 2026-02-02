#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>

// Minimal Ledger OS shim for unit tests
#ifndef PIC
#define PIC(x) (x)
#endif

// THROW implemented in throw_mock.c
void THROW(unsigned int exception);

// No-op logger
static inline void PRINTF(const char *fmt, ...) { (void)fmt; }

// Big-endian helpers
#ifndef U2BE
#define U2BE(buf, off) (((uint16_t)(buf)[(off)] << 8) | (uint16_t)(buf)[(off) + 1])
#endif
#ifndef U4BE
#define U4BE(buf, off) ((U2BE(buf, off) << 16) | (U2BE(buf, off + 2) & 0xFFFF))
#endif

// Minimal strlcpy
static inline size_t strlcpy(char *dst, const char *src, size_t dstsize) {
    size_t srclen = strlen(src);
    if (dstsize > 0) {
        size_t n = (srclen >= dstsize) ? dstsize - 1 : srclen;
        memcpy(dst, src, n);
        dst[n] = '\0';
    }
    return srclen;
}

// NVRAM write stub
static inline void nvm_write(void *dst, const void *src, size_t len) {
    memcpy(dst, src, len);
}

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif