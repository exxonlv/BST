#include "stdmansos.h"
#include <string.h>

#include "pd4_packet.h"

void appMain(void)
{
    lightOn();
    radioInit();
    radioOn();

    while (1) {
        // packet = fixed header + variable-length key bytes
        uint8_t buf[sizeof(Pd4PacketHeader_t) + PD4_KEY_LEN];
        Pd4PacketHeader_t header;
        uint16_t light;

        // read sensor and fill packet header.
        light = lightRead();
        header.keyLen = PD4_KEY_LEN;
        header.reserved = 0;
        header.light = light;

        // serialize header and key into a contiguous buffer for radioSend()
        memcpy(buf, &header, sizeof(header));
        memcpy(buf + sizeof(header), PD4_KEY, PD4_KEY_LEN);

        radioSend(buf, sizeof(buf));
        redLedToggle();
        mdelay(1000);
    }
}