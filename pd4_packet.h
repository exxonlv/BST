#ifndef PD4_PACKET_H
#define PD4_PACKET_H

#include <defines.h>

// shared "key" used to recognize our packets on the same radio channel
#define PD4_KEY "PD4IVOLIVABEST"
#define PD4_KEY_LEN ((uint8_t)(sizeof(PD4_KEY) - 1))

typedef struct __attribute__((packed)) {
    uint8_t keyLen;
    uint8_t reserved;
    uint16_t light;
} Pd4PacketHeader_t;

#endif // PD4_PACKET_H
