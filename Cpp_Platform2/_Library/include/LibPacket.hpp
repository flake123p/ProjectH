

#ifndef _LIB_PACKET_HPP_INCLUDED_

// ============================== Include ==============================
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
	PACKET_IS_LITTLE_ENDIAN,
	PACKET_IS_BIG_ENDIAN,
} PACKET_ENDIAN_t;
typedef struct {
	PACKET_ENDIAN_t endian;
	u32 startPositionInByte; // start position in byte
	u32 lengthInByte;
	u32 bitOffset;
	u32 bitLen;
} PACKET_PARA_t;

typedef enum {
	FIELD_TYPE_U8,
	FIELD_TYPE_U16,
	FIELD_TYPE_U32,
	FIELD_TYPE_U8_ARRAY,
} PACKET_FIELD_TYPE_t;
typedef struct {
	PACKET_ENDIAN_t endian;
	u32 startPositionInByte; // OFFSET_0
	u32 lengthInByte;        // SIZE_1
	u32 bitOffset;           // BIT_POSITION_0
	u32 bitLen;             //  SIZE_1
	PACKET_FIELD_TYPE_t fieldType;
	char *description;
} PACKET_FIELD_t;
typedef void ** PACKET_FIELD_VAR_TBL_t;

typedef struct {
	u8 *rawAry;
	u32 rawAryLen;
	u8 *payloadAry;
	u32 payloadAryLen;
	u8 *tailerAry;
	u32 tailerAryLen;
} PACKET_RAW_DATA_t;

// ============================== Manager ==============================
int LibPacket_Decode32(u8 *srcAryU8, u32 srcAryLen, PACKET_PARA_t *paraAry, u32 paraAryLen, OUT u32 *dstAryU32, OUT u8 **payloadInSrcAry);
int LibPacket_Decode(PACKET_RAW_DATA_t *rawPara, PACKET_FIELD_t *fieldAry, u32 fieldNumber, PACKET_FIELD_VAR_TBL_t dstVarTbl);
// ============================== Demo ==============================
void LibPacket_Demo(void);

// ============================== Library: Platform Dependant ==============================



#define _LIB_ENCRYPT_HPP_INCLUDED_
#endif//_LIB_ENCRYPT_HPP_INCLUDED_




