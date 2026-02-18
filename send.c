
#include "stdmansos.h"

#define HELLO "Hello world"

static uint8_t radioBuffer[RADIO_MAX_PACKET];

#define SERIAL_BUF_LEN 250
static char serialBuffer[SERIAL_BUF_LEN];



void appMain(void)
{   
    uint16_t val;

   
    radioOn();

    PRINTF("Forwarder started...\n");

    while (1) {
       // val = photosyntheticRadiationRead(); 
        radioSend("hello", sizeof("hello"));
        redLedToggle();
        mdelay(1000);
    }
}


/*
#include "stdmansos.h"

#define SERIAL_BUF_LEN 250
static char serialBuffer[SERIAL_BUF_LEN];

static void recvSerial(uint8_t length)
{
    if (length == 0) {
        return;
    }
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

    radioInit();
    radioOn();

    PRINTF("Sender started...\n");

    while (1) {
        static const uint8_t payload[2] = { 0xFF, 0xFF };
        int8_t ret;
        PRINTF("send bytes: %02x %02x\n", (unsigned)payload[0], (unsigned)payload[1]);
        ret = radioSend(payload, sizeof(payload));
        if (ret < 0) {
            PRINTF("radio send failed: %d\n", (int)ret);
        }
        redLedToggle();
        mdelay(1000);
    }
}
	*/