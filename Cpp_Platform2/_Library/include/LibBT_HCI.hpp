


#ifndef _LIB_BLUETOOTH_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
//UART HCI packet types
typedef enum {
	UART_HCI_COMMAND = 1,
	UART_HCI_ACL     = 2,
	UART_HCI_SCO     = 3,
	UART_HCI_EVENT   = 4,
} UART_HCI_PACKET_TYPE_t;

typedef struct {
	u8 reserved[3];
	u8 packetType;
} UART_HCI_HEADER_t;
#define UART_HCI_HEADER_SIZE (sizeof(UART_HCI_HEADER_t)-3)
#define UART_HCI_HEADER_SIZE_FOR_CHECK (1)

typedef struct {
	u8 opcode_L;
	u8 opcode_H;
	u8 parameterLen;
	u8 parameter[5];
} HCI_COMMAND_PACKET_t;
#define HCI_COMMAND_PACKET_HEADER_SIZE (sizeof(HCI_COMMAND_PACKET_t)-5)
#define HCI_COMMAND_PACKET_HEADER_SIZE_FOR_CHECK (3)

typedef struct {
	u8 handel_L;
	u8 handel_H;
	u8 dataLen_L;
	u8 dataLen_H;
	u8 ACL_data[4];
} HCI_ACL_PACKET_t;
#define HCI_ACL_PACKET_HEADER_SIZE (sizeof(HCI_ACL_PACKET_t)-4)
#define HCI_ACL_PACKET_HEADER_SIZE_FOR_CHECK (4)

typedef struct {
	u8 handel_L;
	u8 handel_H;
	u8 dataLen;
	u8 SCO_data[5];
} HCI_SCO_PACKET_t;
#define HCI_SCO_PACKET_HEADER_SIZE (sizeof(HCI_SCO_PACKET_t)-5)
#define HCI_SCO_PACKET_HEADER_SIZE_FOR_CHECK (3)

typedef struct {
	u8 eventCode;
	u8 parameterLen;
	u8 parameter[6];
} HCI_EVENT_PACKET_t;
#define HCI_EVENT_PACKET_HEADER_SIZE (sizeof(HCI_EVENT_PACKET_t)-6)
#define HCI_EVENT_PACKET_HEADER_SIZE_FOR_CHECK (2)

// ============================== Library: Cross-Platform (Manager) ==============================
void LibBT_Diagnose(void);
u32  LibBT_GetHciPduOffset(UART_HCI_PACKET_TYPE_t uartHciType, u32 precedentLen = 4);
int  LibBT_MakeHciPacketHeader(UART_HCI_PACKET_TYPE_t uartHciType, u32 header, u32 dataLen, OUT u8 *buf, u32 precedentLen = 4);
int LibBT_MakeUartHciHeader(UART_HCI_PACKET_TYPE_t uartHciType, OUT u8 *buf);
void LibBT_DumpUartHciPacket(u8 *buf, u32 bufLen);

void LibBT_Demo(void);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================

#define _LIB_BLUETOOTH_HPP_INCLUDED_
#endif//_LIB_BLUETOOTH_HPP_INCLUDED_



