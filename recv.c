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
    if (len < (int16_t)sizeof(Pd4PacketHeader_t)) {
        return;
    }

    {
        Pd4PacketHeader_t header;

        // read fixed-size header
        memcpy(&header, radioBuffer, sizeof(header));

        // filter out other senders on the same channel via key
        if (header.keyLen != PD4_KEY_LEN) {
            return;
        }
        if (len != (int16_t)(sizeof(Pd4PacketHeader_t) + header.keyLen)) {
            return;
        }
        if (memcmp(radioBuffer + sizeof(Pd4PacketHeader_t), PD4_KEY, header.keyLen) != 0) {
            return;
        }

        PRINTF("%u\n", (unsigned)header.light);
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