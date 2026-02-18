#include "stdmansos.h"

#define HELLO "Hello world"

#define SERIAL_BUF_LEN 250
static char serialBuffer[SERIAL_BUF_LEN];

static void recvSerial(uint8_t length)
{
    if (length >= SERIAL_BUF_LEN) {
        length = SERIAL_BUF_LEN - 1;
    }
    serialBuffer[length] = '\0';
    PRINTF("%s", serialBuffer);
    if (serialBuffer[length - 1] != '\n') {
        PRINTF("\n");
    }
    radioSend(serialBuffer, length);
}

void appMain(void)
{
    serialSetPacketReceiveHandle(PRINTF_SERIAL_ID, recvSerial,
            (uint8_t *)serialBuffer, sizeof(serialBuffer));

    radioOn();

    PRINTF("Sender started...\n");

    while (1) {
        radioSend(HELLO, sizeof(HELLO));
        redLedToggle();
        mdelay(1000);
    }
}