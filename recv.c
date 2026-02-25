#include "stdmansos.h"

#define PD4_MAGIC 0xB105u
#define PD4_SECRET 0x5A3Cu

typedef struct __attribute__((packed)) {
    uint16_t magic;
    uint16_t sender;
    uint16_t light;
    uint16_t crc;
} Pd4Packet_t;

static uint8_t radioBuffer[RADIO_MAX_PACKET];

static uint16_t crc16_ccitt(const uint8_t *data, uint16_t len, uint16_t seed)
{
    uint16_t crc = seed;
    uint8_t i;
    while (len--) {
        crc ^= (uint16_t)(*data++) << 8;
        for (i = 0; i < 8; i++) {
            crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ 0x1021u)
                                 : (uint16_t)(crc << 1);
        }
    }
    return crc;
}

static void recvRadio(void)
{
    int16_t len = radioRecv(radioBuffer, sizeof(radioBuffer));
    if (len < 0) {
        PRINTF("radio receive failed\n");
        return;
    }

    if (len == (int16_t)sizeof(Pd4Packet_t)) {
        Pd4Packet_t *pkt = (Pd4Packet_t *)radioBuffer;
        uint16_t crc = crc16_ccitt((uint8_t *)pkt, sizeof(*pkt) - sizeof(pkt->crc), PD4_SECRET);
        if (pkt->magic == PD4_MAGIC && pkt->crc == crc) {
            PRINTF("light = %u\n", (unsigned)pkt->light);
        }
    }
}

void appMain(void)
{
    radioSetReceiveHandle(recvRadio);
    radioOn();

    PRINTF("Receiver started...\n");

    while (1) {
        mdelay(1000);
        blueLedToggle();
    }
}