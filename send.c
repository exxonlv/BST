#include "stdmansos.h"
#include <light.h>
#include <lib/codec/crc.h>
#include <string.h>
#include <net/address.h>

MosShortAddr localAddress __attribute__((weak)) = 0x0002;

#define LIGHTRADIO_GROUP 0x2026
#define LIGHTRADIO_VERSION 1
#define LIGHTRADIO_TYPE_LIGHT 1

typedef struct __attribute__((packed)) {
    uint8_t magic[4];     // "LRAD"
    uint8_t version;
    uint8_t type;
    uint16_t group;
    uint16_t sender;
    uint32_t seq;
    uint16_t light;
    uint16_t crc;
} LightRadioPacket;

static void fillMagic(uint8_t magic[4])
{
    magic[0] = 'L';
    magic[1] = 'R';
    magic[2] = 'A';
    magic[3] = 'D';
}

static uint16_t packetCrc(const LightRadioPacket *p)
{
    return crc16((const uint8_t *)p, sizeof(*p) - sizeof(p->crc));
}

void appMain(void)
{
    LightRadioPacket pkt;
    uint32_t seq = 0;

    memset(&pkt, 0, sizeof(pkt));
    fillMagic(pkt.magic);
    pkt.version = LIGHTRADIO_VERSION;
    pkt.type = LIGHTRADIO_TYPE_LIGHT;
    pkt.group = LIGHTRADIO_GROUP;
    pkt.sender = localAddress;

    lightInit();
    lightOn();

    radioOn();

    while (1) {
        uint16_t light = lightRead();

        pkt.seq = seq++;
        pkt.light = light;
        pkt.crc = packetCrc(&pkt);
		PRINTF("sending light=%u seq=%lu\n", (unsigned)light, (unsigned long)pkt.seq);
        radioSend(&pkt, sizeof(pkt));

        redLedToggle();
        mdelay(1000);
    }
}