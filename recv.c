#include "stdmansos.h"
#include <lib/codec/crc.h>
#include <string.h>
#include <net/address.h>

#define MANSOS
#include "data_packet.h"

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

struct Ieee802_15_4_Packet_Head_s {
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
	uint8_t data[];
} PACKED;

typedef struct Ieee802_15_4_Packet_s Ieee802_15_4_Packet_t;
typedef struct Ieee802_15_4_Packet_Head_s Ieee802_15_4_Packet_Head_t;

static bool panIdMatches(const uint8_t panId[2])
{
	return panId[0] == LIGHT_PAN_ID_LO && panId[1] == LIGHT_PAN_ID_HI;
}

static void recvLight(void)
{
	uint8_t buffer[RADIO_MAX_PACKET];
	int16_t len = radioRecv(buffer, sizeof(buffer));
	if (len < (int16_t)sizeof(Ieee802_15_4_Packet_Head_t)) {
		return;
	}

	Ieee802_15_4_Packet_Head_t head;
	memcpy(&head, buffer, sizeof(head));

	if (!panIdMatches(head.panId)) {
		return;
	}
	if (head.packetId != SAD_DATA_ID) {
		return;
	}

	if (len < (int16_t)(sizeof(Ieee802_15_4_Packet_Head_t) + sizeof(DataPacket_t))) {
		return;
	}

	DataPacket_t packet;
	memcpy(&packet, buffer + sizeof(Ieee802_15_4_Packet_Head_t), sizeof(packet));

	if (packet.crc != crc16((uint8_t *)&packet, sizeof(packet) - 2)) {
		return;
	}

	PRINTF("from=%#04x seq=%u light=%u rssi=%d\n",
			packet.sourceAddress, packet.dataSeqnum, packet.islLight, (int)radioGetLastRSSI());
	greenLedToggle();
}

void appMain(void)
{
	PRINTF("LightRadioRecv: pan=%#02x%#02x\n", LIGHT_PAN_ID_HI, LIGHT_PAN_ID_LO);

	radioSetReceiveHandle(recvLight);
	radioOn();

	while (1) {
		mdelay(1000);
		blueLedToggle();
	}
}
