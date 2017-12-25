

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

// ============================== Manager ==============================
int LibPacket_Decode32(u8 *srcAryU8, u32 srcAryLen, PACKET_PARA_t *paraAry, u32 paraAryLen, OUT u32 *dstAryU32, OUT u8 **payloadInSrcAry);

// ============================== Demo ==============================
void LibPacket_Demo(void);

// ============================== Library: Platform Dependant ==============================



#define _LIB_ENCRYPT_HPP_INCLUDED_
#endif//_LIB_ENCRYPT_HPP_INCLUDED_




