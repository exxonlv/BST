#include "stdmansos.h"

#define PD4_MAGIC 0xB105u
#define PD4_SECRET 0x5A3Cu

typedef struct __attribute__((packed)) {
    uint16_t magic;
    uint16_t sender;
    uint16_t light;
    uint16_t crc;
} Pd4Packet_t;

static uint16_t crc16_ccitt(const uint8_t *data, uint16_t len, uint16_t seed)
{
    uint16_t crc = seed;
    while (len--) {
        crc ^= (uint16_t)(*data++) << 8;
        for (uint8_t i = 0; i < 8; i++) {
            crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ 0x1021u)
                                 : (uint16_t)(crc << 1);
        }
    }
    return crc;
}

void appMain(void)
{
    lightOn();
    radioOn();

    PRINTF("Forwarder started...\n");

    while (1) {
        Pd4Packet_t pkt;
        pkt.magic = PD4_MAGIC;
        pkt.sender = localAddress;
        pkt.light = lightRead();
        pkt.crc = crc16_ccitt((uint8_t *)&pkt, sizeof(pkt) - sizeof(pkt.crc), PD4_SECRET);

        radioSend(&pkt, sizeof(pkt));
        redLedToggle();
        mdelay(1000);
    }
}