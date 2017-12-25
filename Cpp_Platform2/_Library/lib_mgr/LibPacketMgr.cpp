
#include "Everything_Lib_Mgr.hpp"

int LibPacket_Decode32(u8 *srcAryU8, u32 srcAryLen, PACKET_PARA_t *paraAry, u32 paraAryLen, OUT u32 *dstAryU32, OUT u8 **payloadInSrcAry)
{
	u16 tempU16;
	u32 tempU32;
	u32 maxHeaderLen = 0;
	u32 curHeaderLen;

	BASIC_ASSERT(srcAryLen != 0);

	for (u32 i = 0; i < paraAryLen; i++) {
		BASIC_ASSERT(paraAry[i].startPositionInByte + paraAry[i].lengthInByte <= srcAryLen);
		srcAryLen = srcAryLen;
		switch (paraAry[i].lengthInByte) {
			case 1: {
				tempU32 = srcAryU8[paraAry[i].startPositionInByte];
				tempU32 = GET_BIT_FIELD_2(tempU32, paraAry[i].bitLen, paraAry[i].bitOffset);
			} break;

			case 2: {
				if (paraAry[i].endian == PACKET_IS_BIG_ENDIAN) {
					LibUtil_BytesToInt16(tempU16, &(srcAryU8[paraAry[i].startPositionInByte]), true);
				} else {
					LibUtil_BytesToInt16(tempU16, &(srcAryU8[paraAry[i].startPositionInByte]), false);
				}
				tempU32 = (u32)tempU16;
				tempU32 = GET_BIT_FIELD_2(tempU32, paraAry[i].bitLen, paraAry[i].bitOffset);
			} break;

			case 3:
			case 4: {
				if (paraAry[i].endian == PACKET_IS_BIG_ENDIAN) {
					LibUtil_BytesToInt32(tempU32, &(srcAryU8[paraAry[i].startPositionInByte]), true);
				} else {
					LibUtil_BytesToInt32(tempU32, &(srcAryU8[paraAry[i].startPositionInByte]), false);
				}
				tempU32 = (u32)tempU16;
				tempU32 = GET_BIT_FIELD_2(tempU32, paraAry[i].bitLen, paraAry[i].bitOffset);
			} break;
		}
		dstAryU32[i] = tempU32;

		curHeaderLen = paraAry[i].startPositionInByte + paraAry[i].lengthInByte;
		if (curHeaderLen > maxHeaderLen)
			maxHeaderLen = curHeaderLen;
	}

	*payloadInSrcAry = srcAryU8 + maxHeaderLen;

	return 0;
}

void LibPacket_Demo(void)
{
PACKET_PARA_t hciCmdPacketParaTbl[] = {
	{PACKET_IS_LITTLE_ENDIAN, 0, 2,  0, 10}, //HCI command: OCF
	{PACKET_IS_LITTLE_ENDIAN, 0, 2, 10,  6}, //HCI command: OGF
	{PACKET_IS_LITTLE_ENDIAN, 2, 1,  0,  8}, //HCI command: Para Len
};

#pragma pack(push)
#pragma pack(4)
typedef struct {
	u32 ocf;
	u32 ogf;
	u32 paraLen;
	u8 *palyoad;
}HCI_Cmd_Pacekt_t;
#pragma pack(pop)

	u8 inputAry[] = {0x7e, 0xfc, 0x08, 0xAC, 0x54, 0x02, 0x80, 0x03, 0x00, 0x00, 0x20};
	HCI_Cmd_Pacekt_t hciCmd;
	
	LibPacket_Decode32(inputAry, LENGTH_OF_ARRAY(inputAry), hciCmdPacketParaTbl, LENGTH_OF_ARRAY(hciCmdPacketParaTbl), (u32 *)&hciCmd, (u8 **)&(hciCmd.palyoad));

	DUMPX(hciCmd.ocf);
	DUMPX(hciCmd.ogf);
	DUMPX(hciCmd.paraLen);
	DUMPX(hciCmd.palyoad[0]);
	DUMPX(hciCmd.palyoad[1]);
}