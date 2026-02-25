#include "stdmansos.h"
#include <string.h>

#include "pd4_packet.h"

void appMain(void)
{
    lightOn();
    radioInit();
    radioOn();

    while (1) {
        Pd4Packet_t packet;
        uint16_t light;

        // fill key and sensor reading
        memcpy(packet.key, PD4_KEY_STR, PD4_KEY_LEN);
        light = lightRead();
        memcpy(&packet.light, &light, sizeof(light));

        radioSend(&packet, sizeof(packet));
        redLedToggle();
        mdelay(1000);
    }
}