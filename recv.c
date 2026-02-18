#include "stdmansos.h"

static uint8_t radioBuffer[RADIO_MAX_PACKET];

static void recvRadio(void)
{
    int16_t len;

    greenLedToggle();
    len = radioRecv(radioBuffer, sizeof(radioBuffer));
    if (len < 0) {
        PRINTF("radio receive failed\n");
    }
    else if (len > 0) {
        // accept only the expected payload from our sender
        static const char expected[] = "hello";
        if (len == (int16_t)sizeof(expected) &&
            memcmp(radioBuffer, expected, sizeof(expected)) == 0) {
            PRINTF("radio receive %d bytes\n", len);
            debugHexdump((uint8_t *)radioBuffer, len);
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