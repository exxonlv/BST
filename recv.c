#include "stdmansos.h"
#include <lib/codec/crc.h>
#include <string.h>

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

static volatile bool havePkt;
static LightRadioPacket lastPkt;
static int8_t lastRssi;

static bool magicOk(const uint8_t magic[4])
{
    return magic[0] == 'L' && magic[1] == 'R' && magic[2] == 'A' && magic[3] == 'D';
}

static uint16_t packetCrc(const LightRadioPacket *p)
{
    return crc16((const uint8_t *)p, sizeof(*p) - sizeof(p->crc));
}

static void recvLight(void)
{
    uint8_t buffer[RADIO_MAX_PACKET];
    int16_t len = radioRecv(buffer, sizeof(buffer));
    if (len != (int16_t)sizeof(LightRadioPacket)) {
        return;
    }

    LightRadioPacket pkt;
    memcpy(&pkt, buffer, sizeof(pkt));

    if (!magicOk(pkt.magic)) return;
    if (pkt.version != LIGHTRADIO_VERSION) return;
    if (pkt.type != LIGHTRADIO_TYPE_LIGHT) return;
    if (pkt.group != LIGHTRADIO_GROUP) return;
    if (pkt.crc != packetCrc(&pkt)) return;

    lastRssi = radioGetLastRSSI();
    lastPkt = pkt;
    havePkt = true;
    greenLedToggle();
}

void appMain(void)
{
    radioInit();
    radioSetReceiveHandle(recvLight);
    radioOn();

    while (1) {
        if (havePkt) {
            havePkt = false;
            PRINTF("from=%#04x seq=%lu light=%u rssi=%d\n",
                   lastPkt.sender, (unsigned long)lastPkt.seq,
                   (unsigned)lastPkt.light, (int)lastRssi);
        }
        mdelay(1000);
        blueLedToggle();
    }
}