#pragma once
#include <stdint.h>
#include <stddef.h>

void io_seproxyhal_io_heartbeat(void);
int io_send_response_pointer(const uint8_t *buffer, size_t len, uint16_t sw);

extern unsigned char G_io_apdu_buffer[260];