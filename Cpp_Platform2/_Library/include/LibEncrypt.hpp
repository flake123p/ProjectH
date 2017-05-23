

#ifndef _LIB_ENCRYPT_HPP_INCLUDED_

// ============================== Include ==============================
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Manager ==============================
u16 LibEncrypt_CalculateCRC16(u8 *buf, u32 len);
u16 LibEncrypt_CalculateCRC16_IBM(u8 *buf, u32 len, u16 polyRep);

// ============================== Demo ==============================
void LibEncrypt_Demo(void);

// ============================== Library: Platform Dependant ==============================



#define _LIB_ENCRYPT_HPP_INCLUDED_
#endif//_LIB_ENCRYPT_HPP_INCLUDED_



