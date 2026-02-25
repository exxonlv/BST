#include "stdmansos.h"
#include <string.h>

#include "pd4_packet.h"

static uint8_t radioBuffer[RADIO_MAX_PACKET];

static void recvRadio(void)
{
    int16_t len = radioRecv(radioBuffer, sizeof(radioBuffer));
    if (len < 0) {
        return;
    }

    // packets are exactly one struct size
    if (len != (int16_t)sizeof(Pd4Packet_t)) {
        return;
    }

    {
        Pd4Packet_t packet;
        uint16_t light;

        memcpy(&packet, radioBuffer, sizeof(packet));

        // compare key
        if (memcmp(packet.key, PD4_KEY_STR, PD4_KEY_LEN) != 0) {
            return;
        }

        memcpy(&light, &packet.light, sizeof(light));
        PRINTF("%u\n", (unsigned)light);
    }
}

void appMain(void)
{
    radioSetReceiveHandle(recvRadio);
    radioInit();
    radioOn();

    while (1) {
        mdelay(1000);
        blueLedToggle();
    }
}