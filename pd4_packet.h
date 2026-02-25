#ifndef PD4_PACKET_H
#define PD4_PACKET_H

#include <defines.h>

#define PD4_KEY_STR "PD4K"
#define PD4_KEY_LEN 4

typedef struct __attribute__((packed)) {
    uint8_t key[PD4_KEY_LEN];
    uint16_t light;
} Pd4Packet_t;

#endif // PD4_PACKET_H
