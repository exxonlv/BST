#include "stdmansos.h"
#include <light.h>
#include <lib/codec/crc.h>
#include <string.h>
#include <net/address.h>

#define MANSOS
#include "data_packet.h"

// Use a unique PAN ID to ignore other traffic on the same channel
#define LIGHT_PAN_ID_LO 0x26
#define LIGHT_PAN_ID_HI 0x02

struct Ieee802_15_4_Packet_s {
	uint8_t fcf[2];
	uint8_t seqnum;
	uint8_t panId[2];
	uint8_t dstAddress[2];
	uint8_t srcAddress[8];
	uint8_t seqnum2;
	uint8_t something1;
	uint8_t packetId;
	uint8_t something2;
	uint8_t something3;
	uint8_t typeSourceId;
	uint8_t networkAddressOrigin[2];
	uint8_t data[24];
} PACKED;

typedef struct Ieee802_15_4_Packet_s Ieee802_15_4_Packet_t;

static Ieee802_15_4_Packet_t myPacket = {
	.fcf = {0x41, 0xc8},
	.panId = {LIGHT_PAN_ID_LO, LIGHT_PAN_ID_HI},
	.dstAddress = {0xff, 0xff},
	.something1 = 0xab,
	.packetId = SAD_DATA_ID,
	.something2 = 0x01,
	.something3 = 0x23,
	.typeSourceId = 0x00,
	.networkAddressOrigin = {0x56, 0x79},
};

static uint8_t mySeqnum;

MosShortAddr localAddress = 0x0002;

void appMain(void)
{
	DataPacket_t packet;

	memset(&packet, 0, sizeof(packet));

	lightInit();
	lightOn();

	radioOn();

	memset(myPacket.srcAddress, 0, sizeof(myPacket.srcAddress));
	myPacket.srcAddress[0] = (uint8_t)(localAddress & 0xff);
	myPacket.srcAddress[1] = (uint8_t)((localAddress >> 8) & 0xff);

	PRINTF("LightRadioSend: addr=%#04x pan=%#02x%#02x\n",
			localAddress, LIGHT_PAN_ID_HI, LIGHT_PAN_ID_LO);

	while (1) {
		packet.timestamp = (uint16_t)getJiffies();
		packet.sourceAddress = localAddress;
		packet.dataSeqnum = ++mySeqnum;

		packet.islLight = lightRead();
		packet.apdsLight0 = 0xffff;
		packet.apdsLight1 = 0xffff;
		packet.sq100Light = 0xffff;
		packet.internalVoltage = 0xffff;
		packet.internalTemperature = 0xffff;
		packet.sht75Humidity = 0xffff;
		packet.sht75Temperature = 0xffff;

		packet.crc = crc16((uint8_t *)&packet, sizeof(packet) - 2);

		memcpy(&myPacket.data, &packet, sizeof(DataPacket_t));
		myPacket.seqnum = myPacket.seqnum2 = mySeqnum;

		if (radioSend(&myPacket, sizeof(myPacket)) != 0) {
			PRINTF("radioSend failed\n");
		} else {
			PRINTF("sent seq=%u light=%u\n", mySeqnum, packet.islLight);
		}

		redLedToggle();
		mdelay(1000);
	}
}
