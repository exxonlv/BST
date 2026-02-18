
#include "stdmansos.h"

void appMain(void)
{
    lightOn();
    radioOn();

    PRINTF("Forwarder started...\n");

    while (1) {
        uint16_t light = lightRead();
        radioSend(&light, sizeof(light));
        redLedToggle();
        mdelay(1000);
    }
}