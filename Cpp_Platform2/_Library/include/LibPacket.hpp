

#ifndef _LIB_PACKET_HPP_INCLUDED_

// ============================== Include ==============================
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
	PACKET_IS_LITTLE_ENDIAN,
	PACKET_IS_BIG_ENDIAN,
} PACKET_ENDIAN_t;
#if 0
typedef struct {
	PACKET_ENDIAN_t endian;
	u32 startPositionInByte; // start position in byte
	u32 lengthInByte;
	u32 bitOffset;
	u32 bitLen;
} PACKET_PARA_t;
#endif

typedef enum {
	FIELD_TYPE_U8,
	FIELD_TYPE_U16,
	FIELD_TYPE_U32,
	FIELD_TYPE_U8_ARRAY,
} PACKET_FIELD_TYPE_t;
#define DONT_CARE (0)
typedef struct {
	PACKET_ENDIAN_t endian;
	u32 startPositionInByte; // OFFSET_0
	u32 lengthInByte;        // SIZE_1
	u32 bitOffset;           // BIT_POSITION_0
	u32 bitLen;             //  SIZE_1
	PACKET_FIELD_TYPE_t fieldType;
	const char *description;
} PACKET_DEC_PARA_t;
typedef void * PACKET_DEC_INSTANCE_t;

typedef struct {
	u8 *rawAry;
	u32 rawAryLen;
	u32 rawAryMaxLen;
} PACKET_ENC_ARY_t;

// ============================== Manager ==============================
//int LibPacket_Decode32(u8 *srcAryU8, u32 srcAryLen, PACKET_PARA_t *paraAry, u32 paraAryLen, OUT u32 *dstAryU32, OUT u8 **payloadInSrcAry);
int LibPacket_Decode(PACKET_ENC_ARY_t *rawAryData, PACKET_DEC_PARA_t *decParaAry, u32 decParaAryNumber, PACKET_DEC_INSTANCE_t *decInstance);
// ============================== Demo ==============================
void LibPacket_Demo(void);

// ============================== Library: Platform Dependant ==============================



#define _LIB_ENCRYPT_HPP_INCLUDED_
#endif//_LIB_ENCRYPT_HPP_INCLUDED_




