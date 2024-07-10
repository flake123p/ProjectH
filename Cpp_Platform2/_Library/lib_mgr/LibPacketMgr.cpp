
#include "Everything_Lib_Mgr.hpp"

#if 0
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
#endif

int LibPacket_Decode(PACKET_ENC_ARY_t *rawAryData, PACKET_DEC_PARA_t *decParaAry, u32 decParaAryNumber, PACKET_DEC_INSTANCE_t *decInstance)
{
	u32 endianDecBufferU32;

	bool isSrcBigEndian;

	for (u32 i = 0; i < decParaAryNumber; i++) {
		if (decParaAry[i].endian == PACKET_IS_BIG_ENDIAN)
			isSrcBigEndian = true;
		else
			isSrcBigEndian = false;

		if (decParaAry[i].lengthInByte == 0) {
			if (decParaAry[i].fieldType != FIELD_TYPE_U8_ARRAY) {
				BASIC_ASSERT(0);
			}
		} else {
			BASIC_ASSERT(decParaAry[i].lengthInByte >= SIZE_1 && decParaAry[i].lengthInByte <= SIZE_4);

			LibUtil_BytesAssembleU32(
				endianDecBufferU32,
				&(rawAryData->rawAry[decParaAry[i].startPositionInByte]),
				decParaAry[i].lengthInByte,
				isSrcBigEndian);
		}

		switch (decParaAry[i].fieldType) {
			case FIELD_TYPE_U8: {
				endianDecBufferU32 = GET_BIT_FIELD_2(endianDecBufferU32, decParaAry[i].bitLen, decParaAry[i].bitOffset);
				*((u8 *)decInstance[i]) = (u8)endianDecBufferU32;
			} break;

			case FIELD_TYPE_U16: {
				endianDecBufferU32 = GET_BIT_FIELD_2(endianDecBufferU32, decParaAry[i].bitLen, decParaAry[i].bitOffset);
				*((u16 *)decInstance[i]) = (u16)endianDecBufferU32;
			} break;

			case FIELD_TYPE_U32: {
				endianDecBufferU32 = GET_BIT_FIELD_2(endianDecBufferU32, decParaAry[i].bitLen, decParaAry[i].bitOffset);
				*((u32 *)decInstance[i]) = (u32)endianDecBufferU32;
			} break;

			case FIELD_TYPE_U8_ARRAY: {
				*((u8 **)decInstance[i]) = &(rawAryData->rawAry[decParaAry[i].startPositionInByte]);
			} break;

			default:
				BASIC_ASSERT(0);
				break;
		}
	}

	return 0;
}

void LibPacket_Demo(void)
{
#if 0
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
#else
	PACKET_DEC_PARA_t hciCmdPacketFieldTbl[] = {
		{PACKET_IS_LITTLE_ENDIAN, OFFSET_0, SIZE_2, BIT_POSITION_0,  SIZE_10, FIELD_TYPE_U16, "HCI command: OCF"},
		{PACKET_IS_LITTLE_ENDIAN, OFFSET_0, SIZE_2, BIT_POSITION_10, SIZE_6,  FIELD_TYPE_U8,  "HCI command: OGF"},
		{PACKET_IS_LITTLE_ENDIAN, OFFSET_2, SIZE_2, BIT_POSITION_0,  SIZE_8,  FIELD_TYPE_U16, "HCI command: Para Len"},
		{PACKET_IS_LITTLE_ENDIAN, OFFSET_3, DONT_CARE, DONT_CARE, DONT_CARE,  FIELD_TYPE_U8_ARRAY, "HCI command: Para Ary"},
	};
	u16 ocf;
	u8 ogf;
	u16 paraLen;
	u8 *paraAry;

	PACKET_DEC_INSTANCE_t decInstance[] = {
		(void *)&ocf,
		(void *)&ogf,
		(void *)&paraLen,
		(void *)&paraAry,
	};

	u8 inputAry[] = {0x7e, 0xfc, 0x08, 0xAC, 0x54, 0x02, 0x80, 0x03, 0x00, 0x00, 0x20};
	PACKET_ENC_ARY_t rawAry = {
		(u8 *)inputAry,
		sizeof(inputAry),
		sizeof(inputAry),
	};

	LibPacket_Decode(&rawAry, hciCmdPacketFieldTbl, LENGTH_OF_ARRAY(hciCmdPacketFieldTbl), decInstance);
	DUMPX(ocf);
	DUMPX(ogf);
	DUMPX(paraLen);
	DUMPX(paraAry[0]);
	DUMPX(paraAry[1]);

	//TODO: 1.self test 2.encode 3.high level API(future)
#endif
}